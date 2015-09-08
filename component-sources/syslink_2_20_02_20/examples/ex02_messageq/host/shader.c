/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009 - 2011 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

/*
 * shader.c
 * Functions for loading and process shaders.
 */

#include "shader.h"
#include "common.h"
#include "sgxRender3DSRV.h"

#define DEBUG
/*
 * Loads the shader source into memory.
 *
 * sFilename: String holding filename to load
 */



char* load_shader(char *sFilename)
{
    char *pResult = NULL;
    FILE *pFile = NULL;
    long iLen = 0;

    pFile = fopen(sFilename, "r");

    if(pFile == NULL)
    {
        fprintf(stderr, "Error: Cannot read file '%s'\n", sFilename);
  		exit(-1);
    }
    
    fseek(pFile, 0, SEEK_END); /* Seek end of file */
    iLen = ftell(pFile);
    fseek(pFile, 0, SEEK_SET); /* Seek start of file again */
    pResult = calloc(iLen+1, sizeof(char));
    fread(pResult, sizeof(char), iLen, pFile);
    pResult[iLen] = '\0';
    fclose(pFile);

    return pResult;
}

/*
 * Create shader, load in source, compile, dump debug as necessary.
 *
 * pShader: Pointer to return created shader ID.
 * sFilename: Passed-in filename from which to load shader source.
 * iShaderType: Passed to GL, e.g. GL_VERTEX_SHADER.
 */
GLuint process_shader(char *sFilename, GLint iShaderType)
{
	GLint iStatus=0;
	GLuint pShader;
	const char *aStrings[1] = { NULL };

	/* Create shader and load into GL. */
	pShader = glCreateShader(iShaderType);

	aStrings[0] = load_shader(sFilename);

	glShaderSource(pShader, 1, aStrings, NULL);
        //glShaderSource(pShader, 1, vshader_avm[0], NULL);

	/* Clean up shader source. */
	free((void *)aStrings[0]);
	aStrings[0] = NULL;

	/* Try compiling the shader. */
	glCompileShader(pShader);
	glGetShaderiv(pShader, GL_COMPILE_STATUS, &iStatus);

	// Dump debug info (source and log) if compilation failed.
	if(iStatus != GL_TRUE)
    {
    #ifdef DEBUG
		GLint iLen=3274;
		char *sDebugSource = NULL;
		char *sErrorLog = NULL;

		/* Get shader source. */
		glGetShaderiv(pShader, GL_SHADER_SOURCE_LENGTH, &iLen);

		sDebugSource = malloc(iLen);

		glGetShaderSource(pShader, iLen, NULL, sDebugSource);

		printf("Debug source START:\n%s\nDebug source END\n\n", sDebugSource);
		free(sDebugSource);

		/* Now get the info log. */
		glGetShaderiv(pShader, GL_INFO_LOG_LENGTH, &iLen);

		sErrorLog = malloc(iLen);

		glGetShaderInfoLog(pShader, iLen, NULL, sErrorLog);

		printf("Log START:\n%s\nLog END\n\n", sErrorLog);
		free(sErrorLog);
    #endif

		exit(-1);
	}
	return pShader;
}

/*void process_shader2(GLuint *pShader, char *sFilename, GLint iShaderType)
{
	GLint iStatus;
	//GLuint pShader;
	const char *aStrings[1] = { NULL };

	// Create shader and load into GL.
	*pShader = GL_CHECK(glCreateShader(iShaderType));

	aStrings[0] = load_shader(sFilename);

	GL_CHECK(glShaderSource(*pShader, 1, aStrings, NULL));

	// Clean up shader source.
	free((void *)aStrings[0]);
	aStrings[0] = NULL;

	// Try compiling the shader.
	GL_CHECK(glCompileShader(*pShader));
	GL_CHECK(glGetShaderiv(*pShader, GL_COMPILE_STATUS, &iStatus));

	// Dump debug info (source and log) if compilation failed.
	if(iStatus != GL_TRUE)
    {
    #ifdef DEBUG
		GLint iLen;
		char *sDebugSource = NULL;
		char *sErrorLog = NULL;

		// Get shader source.
		GL_CHECK(glGetShaderiv(*pShader, GL_SHADER_SOURCE_LENGTH, &iLen));

		sDebugSource = malloc(iLen);

		GL_CHECK(glGetShaderSource(*pShader, iLen, NULL, sDebugSource));

		printf("Debug source START:\n%s\nDebug source END\n\n", sDebugSource);
		free(sDebugSource);

		// Now get the info log.
		GL_CHECK(glGetShaderiv(*pShader, GL_INFO_LOG_LENGTH, &iLen));

		sErrorLog = malloc(iLen);

		GL_CHECK(glGetShaderInfoLog(*pShader, iLen, NULL, sErrorLog));

		printf("Log START:\n%s\nLog END\n\n", sErrorLog);
		free(sErrorLog);
    #endif

		exit(-1);
	}
	//return pShader;
}*/

GLuint AVM3D_createProgram()
{
    GLint bufLength = 0;
    GLint linkStatus = GL_FALSE;

    GLuint vertexshader=process_shader("shader.vert", GL_VERTEX_SHADER);
    if (!vertexshader)
    {return 0;}

    GLuint pixelshader =process_shader("shader.frag", GL_FRAGMENT_SHADER);
    if (!pixelshader)
    {return 0;}

    GLuint program = glCreateProgram();
    if (program)
    {
        glAttachShader(program, vertexshader);
        //System_eglCheckGlError("glAttachShader");
        glAttachShader(program, pixelshader);
        //System_eglCheckGlError("glAttachShader");
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE)
        {
           glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
           if (bufLength)
           {
               char* buf = (char*) malloc(bufLength);
               if (buf)
               {
                   glGetProgramInfoLog(program, bufLength, NULL, buf);
                   printf(" GL: Could not link program:\n%s\n", buf);
                   free(buf);
               }
           }
           glDeleteProgram(program);
           program = 0;
       }
   }
   if(vertexshader && pixelshader && program)
   {
       glDeleteShader(vertexshader);
       glDeleteShader(pixelshader);
   }
   return program;

}


GLuint AVM3D_createProgram2()
{
    GLint bufLength = 0;
    GLint linkStatus = GL_FALSE;

    GLuint vertexshader=process_shader("/opt/shader2.vert", GL_VERTEX_SHADER);
    if (!vertexshader)
    {return 0;}

    GLuint pixelshader =process_shader("/opt/shader2.frag", GL_FRAGMENT_SHADER);
    if (!pixelshader)
    {return 0;}

    GLuint program = glCreateProgram();
    if (program)
    {
        glAttachShader(program, vertexshader);
        //System_eglCheckGlError("glAttachShader");
        glAttachShader(program, pixelshader);
        //System_eglCheckGlError("glAttachShader");
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE)
        {
           glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
           if (bufLength)
           {
               char* buf = (char*) malloc(bufLength);
               if (buf)
               {
                   glGetProgramInfoLog(program, bufLength, NULL, buf);
                   printf(" GL: Could not link program:\n%s\n", buf);
                   free(buf);
               }
           }
           glDeleteProgram(program);
           program = 0;
       }
   }
   if(vertexshader && pixelshader && program)
   {
       glDeleteShader(vertexshader);
       glDeleteShader(pixelshader);
   }
   return program;

}


GLuint AVM3D_createProgram2_Rotate()
{
    GLint bufLength = 0;
    GLint linkStatus = GL_FALSE;

    GLuint vertexshader=process_shader("/opt/shader2.vert", GL_VERTEX_SHADER);
    if (!vertexshader)
    {return 0;}

    GLuint pixelshader =process_shader("/opt/shader2_Rot.frag", GL_FRAGMENT_SHADER);
    if (!pixelshader)
    {return 0;}

    GLuint program = glCreateProgram();
    if (program)
    {
        glAttachShader(program, vertexshader);
        //System_eglCheckGlError("glAttachShader");
        glAttachShader(program, pixelshader);
        //System_eglCheckGlError("glAttachShader");
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE)
        {
           glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
           if (bufLength)
           {
               char* buf = (char*) malloc(bufLength);
               if (buf)
               {
                   glGetProgramInfoLog(program, bufLength, NULL, buf);
                   printf(" GL: Could not link program:\n%s\n", buf);
                   free(buf);
               }
           }
           glDeleteProgram(program);
           program = 0;
       }
   }
   if(vertexshader && pixelshader && program)
   {
       glDeleteShader(vertexshader);
       glDeleteShader(pixelshader);
   }
   return program;

}

GLuint AVM3D_createProgram3()
{
    GLint bufLength = 0;
    GLint linkStatus = GL_FALSE;

    GLuint vertexshader=process_shader("/opt/shader3.vert", GL_VERTEX_SHADER);
    if (!vertexshader)
    {return 0;}

    GLuint pixelshader =process_shader("/opt/shader3.frag", GL_FRAGMENT_SHADER);
    if (!pixelshader)
    {return 0;}

    GLuint program = glCreateProgram();
    if (program)
    {
        glAttachShader(program, vertexshader);
        //System_eglCheckGlError("glAttachShader");
        glAttachShader(program, pixelshader);
        //System_eglCheckGlError("glAttachShader");
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE)
        {
           glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
           if (bufLength)
           {
               char* buf = (char*) malloc(bufLength);
               if (buf)
               {
                   glGetProgramInfoLog(program, bufLength, NULL, buf);
                   printf(" GL: Could not link program:\n%s\n", buf);
                   free(buf);
               }
           }
           glDeleteProgram(program);
           program = 0;
       }
   }
   if(vertexshader && pixelshader && program)
   {
       glDeleteShader(vertexshader);
       glDeleteShader(pixelshader);
   }
   return program;

}


GLuint AVM3D_createProgram4()
{
    GLint bufLength = 0;
    GLint linkStatus = GL_FALSE;

    GLuint vertexshader=process_shader("/opt/shader4.vert", GL_VERTEX_SHADER);
    if (!vertexshader)
    {return 0;}

    GLuint pixelshader =process_shader("/opt/shader4.frag", GL_FRAGMENT_SHADER);
    if (!pixelshader)
    {return 0;}

    GLuint program = glCreateProgram();
    if (program)
    {
        glAttachShader(program, vertexshader);
        //System_eglCheckGlError("glAttachShader");
        glAttachShader(program, pixelshader);
        //System_eglCheckGlError("glAttachShader");
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE)
        {
           glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
           if (bufLength)
           {
               char* buf = (char*) malloc(bufLength);
               if (buf)
               {
                   glGetProgramInfoLog(program, bufLength, NULL, buf);
                   printf(" GL: Could not link program:\n%s\n", buf);
                   free(buf);
               }
           }
           glDeleteProgram(program);
           program = 0;
       }
   }
   if(vertexshader && pixelshader && program)
   {
       glDeleteShader(vertexshader);
       glDeleteShader(pixelshader);
   }
   return program;

}


//**************************************-line-***************************************************
void shader_line_init(gl_state *screen_state)
{
    screen_state->program = AVM3D_createProgram();
    if(screen_state->program == 0)
    {
        printf("1screen program could not be created\n");
        return;
    }
    glUseProgram(screen_state->program);
    GL_CHECK(glUseProgram);

    screen_state->mvMatrixOffsetLoc = glGetUniformLocation(screen_state->program, "mvp");
    //screen_state->samplerLoc = glGetUniformLocation(screen_state->program, "uSampler0");

	//locate attributes
	screen_state->attribIndices[0] = glGetAttribLocation(screen_state->program, "av4position");
	GL_CHECK(glGetAttribLocation);
	screen_state->attribIndices[1] = glGetAttribLocation(screen_state->program, "av3colour");
	GL_CHECK(glGetAttribLocation);

	glEnableVertexAttribArray(screen_state->attribIndices[0]);
	glEnableVertexAttribArray(screen_state->attribIndices[1]);
}



void vertices_line_init(gl_state* screen_state, float* avm_mesh, int meshSize, float* avm_color, int colorSize, unsigned short* avm_index_mesh, int indexSize)
{
    glGenBuffers(1, &screen_state->vboID[0]);
    glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, meshSize*sizeof(float), avm_mesh, GL_STATIC_DRAW);
    glVertexAttribPointer(screen_state->attribIndices[0], 3, GL_FLOAT, GL_FALSE, 0, 0);
    GL_CHECK(glVertexAttribPointer);

    glGenBuffers(1, &screen_state->vboID[1]);
    glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[1]);
    glBufferData(GL_ARRAY_BUFFER, colorSize*sizeof(float), avm_color, GL_STATIC_DRAW);
    glVertexAttribPointer(screen_state->attribIndices[1], 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &screen_state->vboID[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_state->vboID[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize*sizeof(short), avm_index_mesh, GL_STATIC_DRAW);

}

void avm_line_restore_vbo(gl_state* screen_state)
{
	glUseProgram(screen_state->program);
	GL_CHECK(glUseProgram);

	//restore the vertices and indices - not required ?
	glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[0]);
	glVertexAttribPointer(screen_state->attribIndices[0], 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glVertexAttribPointer(screen_state->attribIndices[1], 2, GL_FLOAT, GL_FALSE, (stride)*sizeof(float), (GLvoid*)(3*sizeof(float)));
	GL_CHECK(glBindBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[1]);
	glVertexAttribPointer(screen_state->attribIndices[1], 3, GL_FLOAT, GL_FALSE, 0, 0);

	GL_CHECK(glBindBuffer);
	//Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_state->vboID[2]);

	//Enable the attributes
	glEnableVertexAttribArray(screen_state->attribIndices[0]);
	glEnableVertexAttribArray(screen_state->attribIndices[1]);
	GL_CHECK(glEnableVertexAttribArray);

    //Bind the texture
	//region 0
    //glUniform1i(screen_state->samplerLoc, 0);
	//glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_EXTERNAL_OES, texYuv);
    //GL_CHECK(glBindTexture);
}
//**************************************-line-***************************************************
//**************************************texture***************************************************
void shader_texture_rotate_init(gl_state *screen_state)
{
    screen_state->program = AVM3D_createProgram2_Rotate();
    if(screen_state->program == 0)
    {
        printf("1screen program could not be created\n");
        return;
    }
    glUseProgram(screen_state->program);
    GL_CHECK(glUseProgram);

    screen_state->mvMatrixOffsetLoc = glGetUniformLocation(screen_state->program, "mvp");
    screen_state->samplerLoc = glGetUniformLocation(screen_state->program, "streamtexture");

	//locate attributes
	screen_state->attribIndices[0] = glGetAttribLocation(screen_state->program, "av4position");
	GL_CHECK(glGetAttribLocation);
	screen_state->attribIndices[1] = glGetAttribLocation(screen_state->program, "aTextureCoord1");
	GL_CHECK(glGetAttribLocation);

}

void shader_texture_init(gl_state *screen_state)
{
    screen_state->program = AVM3D_createProgram2();
    if(screen_state->program == 0)
    {
        printf("1screen program could not be created\n");
        return;
    }
    glUseProgram(screen_state->program);
    GL_CHECK(glUseProgram);

    screen_state->mvMatrixOffsetLoc = glGetUniformLocation(screen_state->program, "mvp");
    screen_state->samplerLoc = glGetUniformLocation(screen_state->program, "streamtexture");

	//locate attributes
	screen_state->attribIndices[0] = glGetAttribLocation(screen_state->program, "av4position");
	GL_CHECK(glGetAttribLocation);
	screen_state->attribIndices[1] = glGetAttribLocation(screen_state->program, "aTextureCoord1");
	GL_CHECK(glGetAttribLocation);

}


void vertices_texture_init(gl_state* screen_state, float* avm_mesh, int meshSize, float* avm_texture, int textureSize,  unsigned short* avm_index_mesh, int indexSize)
{
    glGenBuffers(1, &screen_state->vboID[0]);
    glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, meshSize*sizeof(float), avm_mesh, GL_STATIC_DRAW);
    glVertexAttribPointer(screen_state->attribIndices[0], 3, GL_FLOAT, GL_FALSE, 0, 0);
    GL_CHECK(glVertexAttribPointer);

    glGenBuffers(1, &screen_state->vboID[1]);
    glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[1]);
    glBufferData(GL_ARRAY_BUFFER, textureSize*sizeof(float), avm_texture, GL_STATIC_DRAW);
    glVertexAttribPointer(screen_state->attribIndices[1], 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &screen_state->vboID[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_state->vboID[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize*sizeof(short), avm_index_mesh, GL_STATIC_DRAW);

	glEnableVertexAttribArray(screen_state->attribIndices[0]);
	glEnableVertexAttribArray(screen_state->attribIndices[1]);

}

void avm_texture_restore_vbo(gl_state* screen_state, int texYuv)
{
	glUseProgram(screen_state->program);
	GL_CHECK(glUseProgram);

	//restore the vertices and indices - not required ?
	glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[0]);
	glVertexAttribPointer(screen_state->attribIndices[0], 3, GL_FLOAT, GL_FALSE, 0, 0);
	GL_CHECK(glBindBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[1]);
	glVertexAttribPointer(screen_state->attribIndices[1], 2, GL_FLOAT, GL_FALSE, 0, 0);
	GL_CHECK(glBindBuffer);

	//Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_state->vboID[2]);

	//Enable the attributes
	glEnableVertexAttribArray(screen_state->attribIndices[0]);
	glEnableVertexAttribArray(screen_state->attribIndices[1]);
	GL_CHECK(glEnableVertexAttribArray);

    //Bind the texture
	//region 0
    glUniform1i(screen_state->samplerLoc, 0);
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_STREAM_IMG, ptex_objs[texYuv]);
    GL_CHECK(glBindTexture);
}


void avm_car_texture_restore_vbo(gl_state* screen_state, int texYuv)
{
	glUseProgram(screen_state->program);
	GL_CHECK(glUseProgram);

	//restore the vertices and indices - not required ?
	glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[0]);
	glVertexAttribPointer(screen_state->attribIndices[0], 3, GL_FLOAT, GL_FALSE, 0, 0);
	GL_CHECK(glBindBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[1]);
	glVertexAttribPointer(screen_state->attribIndices[1], 2, GL_FLOAT, GL_FALSE, 0, 0);
	GL_CHECK(glBindBuffer);

	//Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_state->vboID[2]);

	//Enable the attributes
	glEnableVertexAttribArray(screen_state->attribIndices[0]);
	glEnableVertexAttribArray(screen_state->attribIndices[1]);
	GL_CHECK(glEnableVertexAttribArray);

    //Bind the texture
	//region 0
    glUniform1i(screen_state->samplerLoc, 0);
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texYuv);
    GL_CHECK(glBindTexture);
}

//**************************************texture***************************************************
//**************************************overlap***************************************************
void shader_overlap_init(gl_state *screen_state)
{
    screen_state->program = AVM3D_createProgram3();
    if(screen_state->program == 0)
    {
        printf("1screen program could not be created\n");
        return;
    }
    glUseProgram(screen_state->program);
    GL_CHECK(glUseProgram);

    screen_state->mvMatrixOffsetLoc = glGetUniformLocation(screen_state->program, "mvp");
    screen_state->samplerLoc = glGetUniformLocation(screen_state->program, "streamtexture");

	//locate attributes
	screen_state->attribIndices[0] = glGetAttribLocation(screen_state->program, "av4position");
	GL_CHECK(glGetAttribLocation);
	screen_state->attribIndices[1] = glGetAttribLocation(screen_state->program, "aTextureCoord1");
	GL_CHECK(glGetAttribLocation);
	screen_state->attribIndices[2] = glGetAttribLocation(screen_state->program, "inAlpha");
	GL_CHECK(glGetAttribLocation);
}

void vertices_overlap_init(gl_state* screen_state, float* avm_mesh, int meshSize, float* avm_texture, int textureSize,   unsigned short* avm_index_mesh, int indexSize, float* avm_alpha, int alphaSize)
{
    glGenBuffers(1, &screen_state->vboID[0]);
    glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, meshSize*sizeof(float), avm_mesh, GL_STATIC_DRAW);
    glVertexAttribPointer(screen_state->attribIndices[0], 3, GL_FLOAT, GL_FALSE, 0, 0);
    GL_CHECK(glVertexAttribPointer);

    glGenBuffers(1, &screen_state->vboID[1]);
    glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[1]);
    glBufferData(GL_ARRAY_BUFFER, textureSize*sizeof(float), avm_texture, GL_STATIC_DRAW);
    glVertexAttribPointer(screen_state->attribIndices[1], 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &screen_state->vboID[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_state->vboID[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize*sizeof(short), avm_index_mesh, GL_STATIC_DRAW);


	glGenBuffers(1, &screen_state->vboID[3]);
    glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[3]);
    glBufferData(GL_ARRAY_BUFFER, alphaSize*sizeof(float), avm_alpha, GL_STATIC_DRAW);
    glVertexAttribPointer(screen_state->attribIndices[2], 1, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(screen_state->attribIndices[0]);
	glEnableVertexAttribArray(screen_state->attribIndices[1]);
	glEnableVertexAttribArray(screen_state->attribIndices[2]);

}


void avm_overlap_restore_vbo(gl_state* screen_state, int texYuv)
{
	glUseProgram(screen_state->program);
	GL_CHECK(glUseProgram);

	//restore the vertices and indices - not required ?
	glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[0]);
	glVertexAttribPointer(screen_state->attribIndices[0], 3, GL_FLOAT, GL_FALSE, 0, 0);
	GL_CHECK(glBindBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[1]);
	glVertexAttribPointer(screen_state->attribIndices[1], 2, GL_FLOAT, GL_FALSE, 0, 0);
	GL_CHECK(glBindBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[3]);
	glVertexAttribPointer(screen_state->attribIndices[2], 1, GL_FLOAT, GL_FALSE, 0, 0);
	GL_CHECK(glBindBuffer);

	//Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_state->vboID[2]);

	//Enable the attributes
	glEnableVertexAttribArray(screen_state->attribIndices[0]);
	glEnableVertexAttribArray(screen_state->attribIndices[1]);
	glEnableVertexAttribArray(screen_state->attribIndices[2]);
	GL_CHECK(glEnableVertexAttribArray);

    //Bind the texture
	//region 0
    glUniform1i(screen_state->samplerLoc, 0);
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_STREAM_IMG, ptex_objs[texYuv]);
    GL_CHECK(glBindTexture);
}
//**************************************overlap***************************************************




void shader_car_texture_init(gl_state *screen_state)
{
    screen_state->program = AVM3D_createProgram4();
    if(screen_state->program == 0)
    {
        printf("1screen program could not be created\n");
        return;
    }
    glUseProgram(screen_state->program);
    GL_CHECK(glUseProgram);

    screen_state->mvMatrixOffsetLoc = glGetUniformLocation(screen_state->program, "mvp");
    screen_state->samplerLoc = glGetUniformLocation(screen_state->program, "streamtexture");

	//locate attributes
	screen_state->attribIndices[0] = glGetAttribLocation(screen_state->program, "av4position");
	GL_CHECK(glGetAttribLocation);
	screen_state->attribIndices[1] = glGetAttribLocation(screen_state->program, "aTextureCoord1");
	GL_CHECK(glGetAttribLocation);

}




int avm_init_vertices_vbo(gl_state* screen_state, float* avm_mesh, int meshSize, float* avm_texture, int textureSize, unsigned short* avm_index_mesh, int indexSize)
{
	//upload the vertex and texture and image index interleaved array
	//Bowl LUT - Interleaved data (5 data)
	glGenBuffers(1, &screen_state->vboID[0]);
	glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[0]);

	//glBufferData(GL_ARRAY_BUFFER, sizeof(avm_mesh1), avm_mesh1, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, meshSize, avm_mesh, GL_STATIC_DRAW);
	glVertexAttribPointer(screen_state->attribIndices[0], 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
	//glVertexAttribPointer(screen_state->attribIndices[1], 2, GL_FLOAT, GL_FALSE, (stride)*sizeof(float), (GLvoid*)(3*sizeof(float)));
	GL_CHECK(glVertexAttribPointer);


	//20150304 avm texture
	glGenBuffers(1, &screen_state->vboID[1]);
	glBindBuffer(GL_ARRAY_BUFFER, screen_state->vboID[1]);
	
	//glBufferData(GL_ARRAY_BUFFER, sizeof(avm_texture1), avm_texture1, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, textureSize, avm_texture, GL_STATIC_DRAW);
	glVertexAttribPointer(screen_state->attribIndices[1], 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
	GL_CHECK(glVertexAttribPointer);

	
	//Index buffer
	glGenBuffers(1, &screen_state->vboID[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_state->vboID[2]);

	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(avm_index_mesh1), avm_index_mesh1, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, avm_index_mesh, GL_STATIC_DRAW);
	GL_CHECK(glBufferData);

	//Enable for the rendering
	glEnableVertexAttribArray(screen_state->attribIndices[0]);
	glEnableVertexAttribArray(screen_state->attribIndices[1]);

	return 0;
}







