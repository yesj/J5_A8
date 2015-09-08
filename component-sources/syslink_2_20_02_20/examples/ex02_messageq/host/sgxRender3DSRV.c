#include "sgxRender3DSRV.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <GLES2/gl2ext.h>

static int stride = 3+2+2+1+1;
static unsigned int prevLUT=0xdead;
static unsigned int prevblendLUT=0xbeef;

//#define ENABLE_CAR_SIDE_PANES 1
//#define ENABLE_GLOBAL_BLENDING 1
#define ENABLE_BOWL_ROTATION 1

//Layout
#define LAYOUT_NUM_SINGLEVIEWS_HORIZ 2
#define LAYOUT_NUM_SINGLEVIEWS_VERT 2
//Index generation logic
#define MAX_POINTS_WIDTH 440
#define MAX_POINTS_HEIGHT 540
static unsigned short bowl_indexgrid[6*(MAX_POINTS_WIDTH-1)*(MAX_POINTS_HEIGHT-1)];
//Mesh splitting logic
#define MAX_VBO_MESH_SPLIT 8
static GLuint vboId[MAX_VBO_MESH_SPLIT*3];
static int splitcount = 0;
static int splitpoint = 0;

#ifdef ENABLE_BOWL_ROTATION
static GLfloat bowl_matrix[16];
#endif

static const char gSgxRender3DSRV_vertexShader_1VBO[] =
        " #extension GL_OES_EGL_image_external : require \n"
        "  attribute vec3 aVertexPosition;\n "
        "  attribute vec2 aTextureCoord1;\n "
        "  attribute vec2 aTextureCoord2;\n "
        "  attribute float dspImageIndex1;\n "
        "  attribute float dspImageIndex2;\n "        
        "  attribute vec2 blendVals;\n "        
        "  uniform samplerExternalOES uSampler0;\n "
        "  uniform samplerExternalOES uSampler1;\n "
        "  uniform samplerExternalOES uSampler2;\n "
        "  uniform samplerExternalOES uSampler3;\n "   
        "  uniform mat4 uMVMatrix;\n "                             
        "  mediump float RANGE_X = 220.0; \n "
        "  mediump float RANGE_Y = 270.0; \n "
        "  mediump float TEXTURE_X = 1280.0; \n "
        "  mediump float TEXTURE_Y = 720.0;\n "
		"  mediump float normTextureX;\n"
		"  mediump float normTextureY;\n"		
		"  mediump float normTexture1X;\n"
		"  mediump float normTexture1Y;\n"		
		" vec4 iFragColor1; \n "
        " vec4 iFragColor2; \n "
        " varying vec4 outColor; \n "
 " void main(void) {\n "
 "     gl_Position = uMVMatrix * vec4((-RANGE_X+aVertexPosition.x), (RANGE_Y-aVertexPosition.y), aVertexPosition.z, 1.0);\n "
 "     normTextureX = aTextureCoord1.t/TEXTURE_X;\n"
"     normTextureY = aTextureCoord1.s/TEXTURE_Y;\n" 
       " if(dspImageIndex1 < 1.0) iFragColor1 = texture2D(uSampler0,vec2(normTextureX, normTextureY));\n "
 		  "else if(dspImageIndex1 < 2.0) iFragColor1 = texture2D(uSampler1,vec2(normTextureX, normTextureY));\n   "
		    "else if(dspImageIndex1 < 3.0) iFragColor1 = texture2D(uSampler2,vec2(normTextureX, normTextureY));\n   "
		    "else iFragColor1 = texture2D(uSampler3,vec2(normTextureX, normTextureY));                         \n  "		
 "     normTexture1X = aTextureCoord2.t/TEXTURE_X;\n"
"     normTexture1Y = aTextureCoord2.s/TEXTURE_Y;\n" 			
		    "if(dspImageIndex2 < 1.0) iFragColor2 = texture2D(uSampler0,vec2(normTexture1X, normTexture1Y));     \n  "
		    "else if(dspImageIndex2 < 2.0) iFragColor2 = texture2D(uSampler1,vec2(normTexture1X, normTexture1Y));\n  "
		    "else if(dspImageIndex2 < 3.0) iFragColor2 = texture2D(uSampler2,vec2(normTexture1X, normTexture1Y));\n  "
		    "else iFragColor2 = texture2D(uSampler3,vec2(normTexture1X, normTexture1Y));                         \n  "
 "      outColor = (2.0*blendVals.x)*iFragColor1 + (2.0*blendVals.y)*iFragColor2;\n "
 "}\n"
;


// "     gl_Position = uMVMatrix * vec4((-RANGE_X+aVertexPosition.x), (RANGE_Y-aVertexPosition.y), 0.5, 1.0);\n "

//        "  mediump float RANGE_X = 220.0; \n "
//        "  mediump float RANGE_Y = 270.0; \n "

 //"     gl_Position = uMVMatrix * vec4(-1.0+aVertexPosition.x/RANGE_X, 1.0-aVertexPosition.y/RANGE_Y, aVertexPosition.z/RANGE_Z, 1.0);\n "

static const char gSgxRender3DSRV_fragmentShader_1VBO[] =
        " precision mediump float;\n "
        " varying vec4 outColor;\n"
        " void main(){\n"
        "   gl_FragColor = outColor;\n"
        " }\n"
   ;

int generate_short_indices(int width, int height, unsigned short* out)
{
	int i, j;
	unsigned int indexlen = 0;
	if(width > MAX_POINTS_WIDTH) return -1;
	if(height > MAX_POINTS_HEIGHT) return -1;
	for(i = 0;i < height-1;i ++)
	{
		int inc = i*width;
		for(j = 0;j < width-1;j ++)
		{
			out[indexlen + 0] = 0+inc;
			out[indexlen + 1] = width+inc;
			out[indexlen + 2] = width+1+inc;
			out[indexlen + 3] = 0+inc;
			out[indexlen + 4] = width+1+inc;
			out[indexlen + 5] = 1+inc;
			inc ++;
			indexlen += 6;
		}
	}
	return 0;
}   
   
GLuint SgxRender3DSRV_loadShader(GLenum shaderType, const char* pSource) {
   GLuint shader = glCreateShader(shaderType);
   if (shader) {
       glShaderSource(shader, 1, &pSource, NULL);
       glCompileShader(shader);
       GLint compiled = 0;
       glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
       if (!compiled) {
           GLint infoLen = 0;
           glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
           if (infoLen) {
               char* buf = (char*) malloc(infoLen);
               if (buf) {
                   glGetShaderInfoLog(shader, infoLen, NULL, buf);
                   D_PRINTF(" GL: Could not compile shader %d:\n%s\n",
                       shaderType, buf);
                   free(buf);
               }
           } else {
               D_PRINTF(" GL: Guessing at GL_INFO_LOG_LENGTH size\n");
               char* buf = (char*) malloc(0x1000);
               if (buf) {
                   glGetShaderInfoLog(shader, 0x1000, NULL, buf);
                   D_PRINTF(" GL: Could not compile shader %d:\n%s\n",
                   shaderType, buf);
                   free(buf);
               }
           }
           glDeleteShader(shader);
           shader = 0;
       }
   }
   return shader;
}

GLuint SgxRender3DSRV_createProgram(const char* pVertexSource, const char* pFragmentSource) {
   GLuint vertexShader = SgxRender3DSRV_loadShader(GL_VERTEX_SHADER, pVertexSource);
   if (!vertexShader) {
       return 0;
   }

   GLuint pixelShader = SgxRender3DSRV_loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
   if (!pixelShader) {
       return 0;
   }

   GLuint program = glCreateProgram();
   if (program) {
       glAttachShader(program, vertexShader);
       //System_eglCheckGlError("glAttachShader");
       glAttachShader(program, pixelShader);
       //System_eglCheckGlError("glAttachShader");
       glLinkProgram(program);
       GLint linkStatus = GL_FALSE;
       glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
       if (linkStatus != GL_TRUE) {
           GLint bufLength = 0;
           glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
           if (bufLength) {
               char* buf = (char*) malloc(bufLength);
               if (buf) {
                   glGetProgramInfoLog(program, bufLength, NULL, buf);
                   D_PRINTF(" GL: Could not link program:\n%s\n", buf);
                   free(buf);
               }
           }
           glDeleteProgram(program);
           program = 0;
       }
   }
   if(vertexShader && pixelShader && program)
   {
	 glDeleteShader(vertexShader);
	 glDeleteShader(pixelShader);
	}
   return program;
}

//Vertices init for surround view (VBO approach)
static int surroundview_init_vertices_vbo(SgxRender3DSRV_Obj *pObj, GLuint vertexId, GLuint blendId, GLuint indexId, 
										  void* vertexBuff, void* blendBuff, void* indexBuff,
										  int vertexBuffSize, int blendBuffSize, int indexBuffSize
										  )
{  
     //upload the vertex and texture and image index interleaved array
     //Bowl LUT - Interleaved data (5 data)
     glBindBuffer(GL_ARRAY_BUFFER, vertexId);

     glBufferData(GL_ARRAY_BUFFER, vertexBuffSize, vertexBuff, GL_STATIC_DRAW);
     glVertexAttribPointer(pObj->vertexPositionAttribLoc, 3, GL_LUT_DATATYPE, GL_FALSE, stride*sizeof(LUT_DATATYPE), 0);     
     
     glVertexAttribPointer(pObj->vertexTexCoord1AttribLoc, 2, GL_LUT_DATATYPE, GL_FALSE, (stride)*sizeof(LUT_DATATYPE), (GLvoid*)(3*sizeof(LUT_DATATYPE)));
     glVertexAttribPointer(pObj->vertexTexCoord2AttribLoc, 2, GL_LUT_DATATYPE, GL_FALSE, (stride)*sizeof(LUT_DATATYPE), (GLvoid*)(5*sizeof(LUT_DATATYPE)));
     glVertexAttribPointer(pObj->vertexIndexImage1AttribLoc, 1, GL_LUT_DATATYPE, GL_FALSE, (stride)*sizeof(LUT_DATATYPE), (GLvoid*)(7*sizeof(LUT_DATATYPE)));
     glVertexAttribPointer(pObj->vertexIndexImage2AttribLoc, 1, GL_LUT_DATATYPE, GL_FALSE, (stride)*sizeof(LUT_DATATYPE), (GLvoid*)(8*sizeof(LUT_DATATYPE)));
     GL_CHECK(glVertexAttribPointer);

     //blend LUT
     glBindBuffer(GL_ARRAY_BUFFER, blendId);
     glBufferData(GL_ARRAY_BUFFER, blendBuffSize, blendBuff, GL_STATIC_DRAW);
     glVertexAttribPointer(pObj->blendAttribLoc, 2, GL_BLENDLUT_DATATYPE, GL_TRUE, 2*sizeof(char), 0);
     GL_CHECK(glVertexAttribPointer);
                                                 
     //Index buffer
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffSize, indexBuff, GL_STATIC_DRAW);
     GL_CHECK(glBufferData);

     //Enable for the rendering
     glEnableVertexAttribArray(pObj->vertexPositionAttribLoc);
     glEnableVertexAttribArray(pObj->vertexTexCoord1AttribLoc);
     glEnableVertexAttribArray(pObj->vertexTexCoord2AttribLoc);
     glEnableVertexAttribArray(pObj->vertexIndexImage1AttribLoc);
     glEnableVertexAttribArray(pObj->vertexIndexImage2AttribLoc);

     glEnableVertexAttribArray(pObj->blendAttribLoc);     

     return 0;
}

void surroundview_init_vertices_vbo_wrap(SgxRender3DSRV_Obj *pObj)
{
 int i;
 int vertexoffset = 0, blendoffset = 0;
 int ret;
 
 //Generate the index grid first
 ret = generate_short_indices(POINTS_WIDTH, POINTS_HEIGHT, bowl_indexgrid);
 if(ret) 
 {
	D_PRINTF("FATAL Error: index generation failed. Width or Height exceeding %d x %d\n", MAX_POINTS_WIDTH, MAX_POINTS_HEIGHT);
	return;
 }
 	//splitpoint is num of squares in top half
	//Split mesh approach - Height is split into two portions - (HEIGHT-SPLIT_POINT)+(SPLIT_POINT)
	splitcount = 4;
	if(splitcount > MAX_VBO_MESH_SPLIT || splitcount == 0)
	{
		D_PRINTF("FATAL error: Mesh split count (%d) cannot be 0 or > %d \n", splitcount, MAX_VBO_MESH_SPLIT);
		return;
	}
	if(POINTS_HEIGHT > 2)
		splitpoint = (POINTS_HEIGHT-1)/splitcount;
	else if(POINTS_HEIGHT == 2)
		splitpoint = 1;
	else
	{
		D_PRINTF("FATAL error: Cannot draw triangles with a single line mesh...\n");
		return;
	}
	glGenBuffers(splitcount*3, vboId);
	for(i = 0;i < splitcount;i ++)
	{
		int numblocks = splitpoint;
		if(i == splitcount-1) numblocks = (POINTS_HEIGHT-1)-(i*splitpoint);
		surroundview_init_vertices_vbo(
     pObj,
			vboId[i*3], vboId[i*3+1], vboId[i*3+2],
			(char*)pObj->LUT3D + vertexoffset,
			(char*)pObj->blendLUT3D + blendoffset, 
			(char*)bowl_indexgrid,
			sizeof(LUT_DATATYPE)*stride*(POINTS_WIDTH*(numblocks+1)),
			sizeof(BLENDLUT_DATATYPE)*2*(POINTS_WIDTH*(numblocks+1)),
			sizeof(short)*6*(POINTS_WIDTH-1)*(numblocks)
			);
		//Start with the last line as start of next set
		vertexoffset += sizeof(LUT_DATATYPE)*stride*(POINTS_WIDTH*(numblocks));
		blendoffset += sizeof(BLENDLUT_DATATYPE)*2*(POINTS_WIDTH*(numblocks));
	}
}

#define RANGE_X_MESH 220.0
#define RANGE_Y_MESH 270.0
#define RANGE_Z_MESH 430.0
void onscreen_mesh_state_restore_program_textures_attribs(SgxRender3DSRV_Obj *pObj, GLuint *texYuv)
{
     //set the program we need
     glUseProgram(pObj->uiProgramObject);
     //font texture
     glUniform1i(pObj->samplerLocation0, 0);
     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_EXTERNAL_OES, texYuv[0]);
     GL_CHECK(glBindTexture);
     glUniform1i(pObj->samplerLocation1, 1);  
     glActiveTexture(GL_TEXTURE1);
     glBindTexture(GL_TEXTURE_EXTERNAL_OES, texYuv[1]);
     GL_CHECK(glBindTexture);
     glUniform1i(pObj->samplerLocation2, 2);
     glActiveTexture(GL_TEXTURE2);
     glBindTexture(GL_TEXTURE_EXTERNAL_OES, texYuv[2]);
     GL_CHECK(glBindTexture);
     glUniform1i(pObj->samplerLocation3, 3);
     glActiveTexture(GL_TEXTURE3);
     glBindTexture(GL_TEXTURE_EXTERNAL_OES, texYuv[3]); 
     GL_CHECK(glBindTexture);
	 
     //Enable the attributes
     glEnableVertexAttribArray(pObj->vertexPositionAttribLoc);
     glEnableVertexAttribArray(pObj->vertexTexCoord1AttribLoc);
     glEnableVertexAttribArray(pObj->vertexTexCoord2AttribLoc);
     glEnableVertexAttribArray(pObj->vertexIndexImage1AttribLoc);
     glEnableVertexAttribArray(pObj->vertexIndexImage2AttribLoc);

     glEnableVertexAttribArray(pObj->blendAttribLoc);

	//Update the transformation
#if 0 //OLD METHOD
    pObj->mouse += pObj->delta;
    //if ((pObj->mouse > 0.55) || (pObj->mouse < 0.45)) { pObj->delta *= -1; };
	if ((pObj->mouse > 0.75) || (pObj->mouse < 0.25)) { pObj->delta *= -1; };
#ifdef ENABLE_BOWL_ROTATION	
    //get_matrix_output_mesh(3.5f, (pObj->mouse-0.5)*135, (pObj->mouse-0.5)*135, 1.0/RANGE_X_MESH, 1.0/RANGE_Y_MESH, 1.0/RANGE_Z_MESH, bowl_matrix);
	get_matrix_output_mesh(4.5f, 0.0, (pObj->mouse-0.5)*135, 1.0/RANGE_X_MESH, 1.0/RANGE_Y_MESH, 1.0/RANGE_Z_MESH, bowl_matrix);
    glUniformMatrix4fv(pObj->mvMatrixLocation, 1, GL_FALSE, bowl_matrix);
    GL_CHECK(glUniformMatrix4fv);     	 
#endif	
#else //NEW METHOD
	static float angleX = 0;
	static float angleY = 0;
	static float angleZ = 0;
	static float scale = 0;
 static int stage = 1;
	angleZ += 1.0;
  if(stage == 1)
 {
	  scale += 0.007;
	  if(angleZ > 90) 
	  {
     angleZ -= 0.5;
		  angleX += 0.5;
		  //angleY += 0.5;
		  if(angleX > 70) stage = 2;
	  }
 }
 else
 { 
   if(angleZ > 270){angleZ = angleY = angleX = 0; scale = 0; stage = 1;}
 }

#ifdef ENABLE_BOWL_ROTATION	
	//get_matrix_output_mesh(4.5f, angleX, 0, angleZ, (1.0+scale)/RANGE_X_MESH, (1.0+scale)/RANGE_Y_MESH, 1.0/RANGE_Z_MESH, bowl_matrix);//hkc
    glUniformMatrix4fv(pObj->mvMatrixLocation, 1, GL_FALSE, bowl_matrix);
    GL_CHECK(glUniformMatrix4fv);     	 
#endif	

#endif
}

void onscreen_mesh_state_restore_vbo(SgxRender3DSRV_Obj *pObj,
                                        GLuint vertexId, GLuint blendId, GLuint indexId)
{
	       
     //restore the vertices and indices
     glBindBuffer(GL_ARRAY_BUFFER, vertexId);
     glVertexAttribPointer(pObj->vertexPositionAttribLoc, 3, GL_LUT_DATATYPE, GL_FALSE, stride*sizeof(LUT_DATATYPE), 0);
     glVertexAttribPointer(pObj->vertexTexCoord1AttribLoc, 2, GL_LUT_DATATYPE, GL_FALSE, (stride)*sizeof(LUT_DATATYPE), (GLvoid*)(3*sizeof(LUT_DATATYPE)));
     glVertexAttribPointer(pObj->vertexTexCoord2AttribLoc, 2, GL_LUT_DATATYPE, GL_FALSE, (stride)*sizeof(LUT_DATATYPE), (GLvoid*)(5*sizeof(LUT_DATATYPE)));
     glVertexAttribPointer(pObj->vertexIndexImage1AttribLoc, 1, GL_LUT_DATATYPE, GL_FALSE, (stride)*sizeof(LUT_DATATYPE), (GLvoid*)(7*sizeof(LUT_DATATYPE)));
     glVertexAttribPointer(pObj->vertexIndexImage2AttribLoc, 1, GL_LUT_DATATYPE, GL_FALSE, (stride)*sizeof(LUT_DATATYPE), (GLvoid*)(8*sizeof(LUT_DATATYPE)));

     glBindBuffer(GL_ARRAY_BUFFER, blendId);
     glVertexAttribPointer(pObj->blendAttribLoc, 2, GL_BLENDLUT_DATATYPE, GL_TRUE, 2*sizeof(char), 0);
     
     //Index buffer
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
}


int SgxRender3DSRV_setup(SgxRender3DSRV_Obj *pObj)
{
    //STEP1 - shader setup
    pObj->uiProgramObject = SgxRender3DSRV_createProgram(
                        gSgxRender3DSRV_vertexShader_1VBO,
                        gSgxRender3DSRV_fragmentShader_1VBO
                     );
    if (pObj->uiProgramObject==0)
    {
       return -1;
    }

    glUseProgram(pObj->uiProgramObject);
    //System_eglCheckGlError("glUseProgram");hkc

    //locate sampler uniforms
    pObj->samplerLocation0 = glGetUniformLocation(pObj->uiProgramObject, "uSampler0");
    glUniform1i(pObj->samplerLocation0, 0);
    GL_CHECK(glUniform1i);
    pObj->samplerLocation1 = glGetUniformLocation(pObj->uiProgramObject, "uSampler1");
    glUniform1i(pObj->samplerLocation1, 1);
    GL_CHECK(glUniform1i);
    pObj->samplerLocation2 = glGetUniformLocation(pObj->uiProgramObject, "uSampler2");
    glUniform1i(pObj->samplerLocation2, 2);
    GL_CHECK(glUniform1i);
    pObj->samplerLocation3 = glGetUniformLocation(pObj->uiProgramObject, "uSampler3");
    glUniform1i(pObj->samplerLocation3, 3);
    GL_CHECK(glUniform1i);

    pObj->mvMatrixLocation = glGetUniformLocation(pObj->uiProgramObject, "uMVMatrix");
    GL_CHECK(glGetAttribLocation);
    pObj->vertexPositionAttribLoc = glGetAttribLocation(pObj->uiProgramObject, "aVertexPosition");
    GL_CHECK(glGetAttribLocation);
    pObj->blendAttribLoc = glGetAttribLocation(pObj->uiProgramObject, "blendVals");
    GL_CHECK(glGetAttribLocation);
    pObj->vertexTexCoord1AttribLoc = glGetAttribLocation(pObj->uiProgramObject, "aTextureCoord1");
    GL_CHECK(glGetAttribLocation);
    pObj->vertexTexCoord2AttribLoc = glGetAttribLocation(pObj->uiProgramObject, "aTextureCoord2");
    GL_CHECK(glGetAttribLocation);
    pObj->vertexIndexImage1AttribLoc = glGetAttribLocation(pObj->uiProgramObject, "dspImageIndex1");
    GL_CHECK(glGetAttribLocation);
    pObj->vertexIndexImage2AttribLoc = glGetAttribLocation(pObj->uiProgramObject, "dspImageIndex2");
    GL_CHECK(glGetAttribLocation);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //System_eglCheckGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    //System_eglCheckGlError("glClear");

    glDisable(GL_DEPTH_TEST);
#ifdef ENABLE_GLOBAL_BLENDING	
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif	

    pObj->mouse = 0.5; 
    pObj->delta = 0.0025; //0.00125;

    //STEP2 - initialise the vertices
    //car_init_vertices_vbo(&pObj->car_gl1);
    //GL_CHECK(car_init_vertices_vbo);
    
    //STEP3 - initialise the individual views
    //screen1_init_vbo();
    //GL_CHECK(screen1_init_vbo);

	//20150303  3D AVM
 //	avm_init_vbo();
	GL_CHECK(avm_init_vbo);
		
	//cull
	//glEnable(GL_CULL_FACE);
 
    return 0;
}

void SgxRender3DSRV_drawWrapper(SgxRender3DSRV_Obj *pObj, GLuint *texYuv)
{
 int i;
 if(prevLUT == 0xdead) 
 {
   prevLUT = (unsigned int)pObj->LUT3D;
   prevblendLUT = (unsigned int)pObj->blendLUT3D;
   surroundview_init_vertices_vbo_wrap(pObj);
 }
 else if(prevLUT != (unsigned int)pObj->LUT3D) D_PRINTF("FATAL ERROR - LUT3D addr %x\n", (int)pObj->LUT3D);
 else if(prevblendLUT != (unsigned int)pObj->blendLUT3D) D_PRINTF("FATAL ERROR - blendLUT3D addr %x\n", (int)pObj->blendLUT3D);     
 
    //First setup the program and textures once  
	onscreen_mesh_state_restore_program_textures_attribs(
			pObj, texYuv);
	//then change the meshes and draw
	for(i = 0;i < splitcount;i ++)
	{
		int numblocks = splitpoint;
		if(i == splitcount-1) numblocks = POINTS_HEIGHT-1-(i*splitpoint);
		//First round
		onscreen_mesh_state_restore_vbo(
			pObj, vboId[i*3], vboId[i*3+1], vboId[i*3+2]);
		GL_CHECK(onscreen_mesh_state_restore_vbo);
		glDrawElements(GL_TRIANGLES, (POINTS_WIDTH-1)*(numblocks)*6, GL_UNSIGNED_SHORT,  0);
		GL_CHECK(glDrawElements);
	}
}

void SgxRender3DSRV_renderFrame3DSRV(SgxRender3DSRV_Obj *pObj, System_EglObj *pEglObj, GLuint *texYuv)
{
/*    glClear(GL_COLOR_BUFFER_BIT);

	//layout for 2x subsampled mesh
	int width_bowlview = (POINTS_WIDTH*2);
	int height_bowlview = (POINTS_HEIGHT*2);
	//20150317
	if(height_bowlview > pObj->screen_height)
		height_bowlview = pObj->screen_height;
	
	int width_singleview = (pObj->screen_width-width_bowlview)/LAYOUT_NUM_SINGLEVIEWS_HORIZ;

	int height_singleview = ((pObj->screen_height-200)/LAYOUT_NUM_SINGLEVIEWS_VERT);
	//Vps_printf("●(￣^￣ )●$  pObj->screen_width: %d height: %d\n", pObj->screen_width, pObj->screen_height); //1280x800
   // Draw the other panes
     {
        glViewport(0, height_singleview, width_singleview,height_singleview);
		//glViewport(0, 0, pObj->screen_width,pObj->screen_height);  // 校正用
        screen1_draw_vbo(texYuv[0]);
#ifdef ENABLE_CAR_SIDE_PANES		
       car_draw_vbo(&pObj->car_gl1, 0);               
#endif	   
     }
     {
        glViewport(0, 0, width_singleview,height_singleview);
		//glViewport(width_singleview, 0, width_bowlview,640);
        screen1_draw_vbo(texYuv[1]);
#ifdef ENABLE_CAR_SIDE_PANES				
       car_draw_vbo(&pObj->car_gl1, 1);               
#endif
     }
     {
        glViewport(width_bowlview + width_singleview, height_singleview, width_singleview,height_singleview);		
        screen1_draw_vbo(texYuv[2]);
#ifdef ENABLE_CAR_SIDE_PANES						
       car_draw_vbo(&pObj->car_gl1, 2);                       
#endif	   
     }
     {
        glViewport(width_bowlview + width_singleview, 0, width_singleview,height_singleview);			 
        screen1_draw_vbo(texYuv[3]);
#ifdef ENABLE_CAR_SIDE_PANES						
       car_draw_vbo(&pObj->car_gl1, 3);                       
#endif	   
     }

    {
       glViewport(width_singleview, 0, width_bowlview,height_bowlview);
	   avm_draw_vbo(texYuv);//繪圖用
       //SgxRender3DSRV_drawWrapper(pObj, texYuv);
       car_draw_vbo(&pObj->car_gl1, 4);
     }*/
}

void SgxRender3DSRV_renderFrame(SgxRender3DSRV_Obj *pObj, System_EglObj *pEglObj, GLuint *texYuv)
{
    SgxRender3DSRV_renderFrame3DSRV(pObj, pEglObj, texYuv);
}

