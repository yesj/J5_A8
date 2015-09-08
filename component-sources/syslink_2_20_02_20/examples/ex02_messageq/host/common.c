/*****************************************************************************
 * common.c
 *
 * bc-cat module unit test - common functions
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *   
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 *   
 *   Neither the name of Texas Instruments Incorporated nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/fb.h>
#include "common.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <GLES2/gl2ext.h>

#define CUBE_V_LEN   1.8

#define BARS     8  /*number of color bars*/
#define STEPS   16  /*moving steps of a bar*/

//#define WISH
#define REDFLAG


GLuint *ptex_objs;
GLint model_view_idx[2], proj_idx[2];



/* 3D data. Vertex range -0.5..0.5 in all axes.
* Z -0.5 is near, 0.5 is far. */

void avm_line_init();
//void avm_drawline_VBO();
void avm_texture_init();
void car_texture_init();


#ifdef REDFLAG
#include "London.h"
#include "LondonWH.h"
#include "LondonGL.h"
#else
#include "jeep_object.h"
#endif
static int setup_shaders(int bcdev_id, int num_bufs);


int gQuit = FALSE;
int profiling = FALSE;
PFNGLTEXBINDSTREAMIMGPROC glTexBindStreamIMG = NULL;

EGLDisplay dpy;
EGLSurface surface = EGL_NO_SURFACE;

static EGLContext context = EGL_NO_CONTEXT;



void signalHandler(int signum) { (void)signum; gQuit=TRUE; }

int get_disp_resolution(int *w, int *h)
{
    int fb_fd, ret = -1;
    struct fb_var_screeninfo vinfo;

    if ((fb_fd = open("/dev/fb0", O_RDONLY)) < 0) {
        printf("failed to open fb0 device\n");
        return ret;
    }

    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("FBIOGET_VSCREENINFO");
        goto exit;
    }

    *w = vinfo.xres; 	//704    
    *h = vinfo.yres;	//480


    if (*w && *h)
        ret = 0;

exit:
    close(fb_fd);
    return ret;
}



int initTexExt(int bcdev_id, tex_buffer_info_t *binfo)
{
    PFNGLGETTEXSTREAMDEVICEATTRIBUTEIVIMGPROC glGetTexAttrIMG = NULL;
    PFNGLGETTEXSTREAMDEVICENAMEIMGPROC glGetTexDeviceIMG = NULL;
    const GLubyte *glext;
    const GLubyte *dev_name;

    if (!binfo)
        return -5;

    if (!(glext = glGetString(GL_EXTENSIONS)))
        return -1;

    if (!strstr((char *)glext, "GL_IMG_texture_stream2"))

        return -2;

    glTexBindStreamIMG =
        (PFNGLTEXBINDSTREAMIMGPROC)eglGetProcAddress("glTexBindStreamIMG");
    glGetTexAttrIMG = (PFNGLGETTEXSTREAMDEVICEATTRIBUTEIVIMGPROC)
        eglGetProcAddress("glGetTexStreamDeviceAttributeivIMG");
    glGetTexDeviceIMG = (PFNGLGETTEXSTREAMDEVICENAMEIMGPROC)
        eglGetProcAddress("glGetTexStreamDeviceNameIMG");

    if (!glTexBindStreamIMG || !glGetTexAttrIMG || !glGetTexDeviceIMG)
        return -3;

    dev_name = glGetTexDeviceIMG(bcdev_id);
    if (!dev_name)
        return -4;

    glGetTexAttrIMG(bcdev_id, GL_TEXTURE_STREAM_DEVICE_NUM_BUFFERS_IMG, &binfo->n);
    glGetTexAttrIMG(bcdev_id, GL_TEXTURE_STREAM_DEVICE_WIDTH_IMG, &binfo->w);
    glGetTexAttrIMG(bcdev_id, GL_TEXTURE_STREAM_DEVICE_HEIGHT_IMG, &binfo->h);
    glGetTexAttrIMG(bcdev_id, GL_TEXTURE_STREAM_DEVICE_FORMAT_IMG, &binfo->fmt);

    if (setup_shaders(bcdev_id, binfo->n))
        return -5;


    printf("\ndevice: %s num: %d, width: %d, height: %d, format: 0x%x\n",
        dev_name, binfo->n, binfo->w, binfo->h, binfo->fmt);

    return 0;
}

static void print_err(char *name)
{
    char *err_str[] = {
          "EGL_SUCCESS",
          "EGL_NOT_INITIALIZED",
          "EGL_BAD_ACCESS",
          "EGL_BAD_ALLOC",
          "EGL_BAD_ATTRIBUTE",    
          "EGL_BAD_CONFIG",
          "EGL_BAD_CONTEXT",   
          "EGL_BAD_CURRENT_SURFACE",
          "EGL_BAD_DISPLAY",
          "EGL_BAD_MATCH",
          "EGL_BAD_NATIVE_PIXMAP",
          "EGL_BAD_NATIVE_WINDOW",
          "EGL_BAD_PARAMETER",
          "EGL_BAD_SURFACE" };

    EGLint ecode = eglGetError();

    printf("'%s': egl error '%s' (0x%x)\n",
           name, err_str[ecode-EGL_SUCCESS], ecode);
}

void deInitEGL(int n_buf)
{

    if (ptex_objs) {
        glDeleteTextures (n_buf, ptex_objs);
        glDisable(GL_TEXTURE_STREAM_IMG);
        free(ptex_objs);
        ptex_objs = NULL;
    }
   
    eglMakeCurrent(dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (context != EGL_NO_CONTEXT)
        eglDestroyContext(dpy, context);
    if (surface != EGL_NO_SURFACE)
        eglDestroySurface(dpy, surface);
    eglTerminate(dpy);
}

int initEGL(int n_buf)
{

    EGLint  context_attr[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

    EGLint            disp_w, disp_h;
    EGLNativeDisplayType disp_type;
    EGLNativeWindowType  window;
    EGLConfig         cfgs[2];
    EGLint            n_cfgs;
    EGLint            egl_attr[] = {
                         EGL_BUFFER_SIZE, EGL_DONT_CARE,
                         EGL_RED_SIZE,    8,
                         EGL_GREEN_SIZE,  8,
                         EGL_BLUE_SIZE,   8,
                         EGL_DEPTH_SIZE,  8,
                         EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                         EGL_NONE };

    if (get_disp_resolution(&disp_w, &disp_h)) {
        printf("ERROR: get display resolution failed\n");
        return -1;
    }

    printf("\n\nliuxu, 04/21/2014, get_disp_resolution, disp_w=%d, disp_h=%d\n\n", disp_w, disp_h);

    disp_type = (EGLNativeDisplayType)EGL_DEFAULT_DISPLAY;
    window  = 0;


    dpy = eglGetDisplay(disp_type);

    if (eglInitialize(dpy, NULL, NULL) != EGL_TRUE) {
        print_err("eglInitialize");
        return -1;
    }

    if (eglGetConfigs(dpy, cfgs, 2, &n_cfgs) != EGL_TRUE) {
        print_err("eglGetConfigs");
        goto cleanup;
    }
    
    if (eglChooseConfig(dpy, egl_attr, cfgs, 2, &n_cfgs) != EGL_TRUE) {
        print_err("eglChooseConfig");
        goto cleanup;
    }

    surface = eglCreateWindowSurface(dpy, cfgs[0], window, NULL);
    if (surface == EGL_NO_SURFACE) {
        print_err("eglCreateWindowSurface");
        goto cleanup;
    }

    context = eglCreateContext(dpy, cfgs[0], EGL_NO_CONTEXT, context_attr);

    if (context == EGL_NO_CONTEXT) {
        print_err("eglCreateContext");
        goto cleanup;
    }

    if (eglMakeCurrent(dpy, surface, surface, context) != EGL_TRUE) {
        print_err("eglMakeCurrent");
        goto cleanup;
    }
  
    /* 0 - do not sync with video frame */
    if (profiling == TRUE) {
        if (eglSwapInterval(dpy, 0) != EGL_TRUE) {
            print_err("eglSwapInterval");
            goto cleanup;
        }
    }


    return 0;

cleanup:
    deInitEGL(n_buf);
    return -1;
}


int reset_shaders_idx0(int bcdev_id)//liuxu, 10/10/2013.
{

      glBindTexture (GL_TEXTURE_STREAM_IMG, ptex_objs[0]);
   
      /* specify filters */
      glTexParameterf(GL_TEXTURE_STREAM_IMG, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_STREAM_IMG, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   
      // assign the buffer
      glTexBindStreamIMG(bcdev_id, 0);

      //printf("\nliuxu, reset_shaders_idx0\n");
      return 0;

}

static int setup_shaders(int bcdev_id, int num_bufs)
{
    int status, idx;

    printf("\r\n avm setup_shaders init \n");

    model_create(&bottom_avm,&top_avm, carwidth, carlength, 1, 5);
    Line_index_create(&bottom_line_avm, &top_line_avm, carwidth, carlength, 1, 5);
    Triangles_index_create(&bottom_tri_avm, &top_tri_avm, carwidth, carlength, 1, 5);
    texture_vertex_create(&bottom_texture_avm, &top_texture_avm, carwidth, carlength, 1, 5);
    overlap_create(&overlap_alpha);
    avm_line_init();   				//line init!!!!!!!!!!!!!!!!!!!!!!!              
    avm_texture_init();             //texture init!!!!!!!!!!!!!!!!!!!!!!!
	car_texture_init();



    printf("\r\n avm setup_shaders end \n");


   
    /* allocate texture objects */
    ptex_objs = malloc(sizeof (GLuint) * num_bufs);
    if (!ptex_objs) {
        printf("ERROR: malloc failed in %s\n", __FUNCTION__);
        return -1;
    }
    glGenTextures(num_bufs, ptex_objs);
   
    /* activate texture unit */
    glActiveTexture(GL_TEXTURE0);
	
    /* associate buffers to textures */
    for (idx = 0; idx < num_bufs; idx++) {
      glBindTexture (GL_TEXTURE_STREAM_IMG, ptex_objs[idx]);
   
      /* specify filters */
      glTexParameterf(GL_TEXTURE_STREAM_IMG, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_STREAM_IMG, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   
      // assign the buffer
      glTexBindStreamIMG(bcdev_id, idx);
    }
   
    return 0;
}



void avm_line_init()
{
   BFB_size=(FBVslices+1)*(Cslices+1)*3;
    BLR_size=(LRVslices+1)*(Cslices+1)*3;
    BC_size=(Vslices+1)*Cslices*3+3;

    TFB_size=(FBVslices+1)*(Hslices+1)*3;
    TLR_size=(LRVslices+1)*(Hslices+1)*3;
    TC_size=(Vslices+1)*(Hslices+1)*3;


    BFBL_index_size=(FBVslices*(Cslices+1)+(FBVslices+1)*(Cslices))*2;     //BFBL(bottom front back line)
    BLRL_index_size=(LRVslices*(Cslices+1)+(LRVslices+1)*(Cslices))*2;
    BCL_index_size=((Vslices*Cslices)+((Vslices+1)*Cslices))*2;

    TFBL_index_size=(FBVslices*(Hslices)+(FBVslices+1)*(Hslices))*2;
    TLRL_index_size=(LRVslices*(Hslices)+(LRVslices+1)*(Hslices))*2;
    TCL_index_size=((Vslices*Hslices)+((Vslices+1)*Hslices))*2;

    /*shader_line_init(&bottom_front);
    vertices_line_init(&bottom_front, bottom_avm._front, BFB_size, aColours, BFB_size, bottom_line_avm.front_back, BFBL_index_size);

    shader_line_init(&bottom_back);
    vertices_line_init(&bottom_back,  bottom_avm._back,  BFB_size, aColours, BFB_size, bottom_line_avm.front_back, BFBL_index_size);

    shader_line_init(&bottom_right);
    vertices_line_init(&bottom_right, bottom_avm.right, BLR_size, aColours, BLR_size, bottom_line_avm.right_left, BLRL_index_size);
    shader_line_init(&bottom_left);
    vertices_line_init(&bottom_left,  bottom_avm.left,  BLR_size, aColours, BLR_size, bottom_line_avm.right_left, BLRL_index_size);

    shader_line_init(&Bfront_right);
    vertices_line_init(&Bfront_right, bottom_avm.front_right, BC_size, aColours, BC_size, bottom_line_avm.circle, BCL_index_size);
    shader_line_init(&Bfront_left);
    vertices_line_init(&Bfront_left, bottom_avm.front_left, BC_size, aColours, BC_size, bottom_line_avm.circle, BCL_index_size);

    shader_line_init(&Bback_right);
    vertices_line_init(&Bback_right, bottom_avm.back_right, BC_size, aColours, BC_size, bottom_line_avm.circle, BCL_index_size);
    shader_line_init(&Bback_left);
    vertices_line_init(&Bback_left, bottom_avm.back_left, BC_size, aColours, BC_size, bottom_line_avm.circle, BCL_index_size);

    shader_line_init(&Bright_up);
    vertices_line_init(&Bright_up, bottom_avm.right_up, BC_size, aColours, BC_size, bottom_line_avm.circle, BCL_index_size);
    shader_line_init(&Bright_down);
    vertices_line_init(&Bright_down, bottom_avm.right_down, BC_size, aColours, BC_size, bottom_line_avm.circle, BCL_index_size);

    shader_line_init(&Bleft_up);
    vertices_line_init(&Bleft_up, bottom_avm.left_up, BC_size, aColours, BC_size, bottom_line_avm.circle, BCL_index_size);
    shader_line_init(&Bleft_down);
    vertices_line_init(&Bleft_down, bottom_avm.left_down, BC_size, aColours, BC_size, bottom_line_avm.circle, BCL_index_size);


    shader_line_init(&top_front);
    vertices_line_init(&top_front, top_avm._front, TFB_size, aColours, TFB_size, top_line_avm.front_back, TFBL_index_size);
    shader_line_init(&top_back);
    vertices_line_init(&top_back,  top_avm._back,  TFB_size, aColours, TFB_size, top_line_avm.front_back, TFBL_index_size);

    shader_line_init(&top_right);
    vertices_line_init(&top_right, top_avm.right, TLR_size, aColours, TLR_size, top_line_avm.right_left, TLRL_index_size);
    shader_line_init(&top_left);
    vertices_line_init(&top_left,  top_avm.left,  TLR_size, aColours, TLR_size, top_line_avm.right_left, TLRL_index_size);

    shader_line_init(&Tfront_right);
    vertices_line_init(&Tfront_right, top_avm.front_right, TC_size, aColours, TC_size, top_line_avm.circle, TCL_index_size);
    shader_line_init(&Tfront_left);
    vertices_line_init(&Tfront_left, top_avm.front_left, TC_size, aColours, TC_size, top_line_avm.circle, TCL_index_size);

    shader_line_init(&Tback_right);
    vertices_line_init(&Tback_right, top_avm.back_right, TC_size, aColours, TC_size, top_line_avm.circle, TCL_index_size);
    shader_line_init(&Tback_left);
    vertices_line_init(&Tback_left, top_avm.back_left, TC_size, aColours, TC_size, top_line_avm.circle, TCL_index_size);

    shader_line_init(&Tright_up);
    vertices_line_init(&Tright_up, top_avm.right_up, TC_size, aColours, TC_size, top_line_avm.circle, TCL_index_size);
    shader_line_init(&Tright_down);
    vertices_line_init(&Tright_down, top_avm.right_down, TC_size, aColours, TC_size, top_line_avm.circle, TCL_index_size);

    shader_line_init(&Tleft_up);
    vertices_line_init(&Tleft_up, top_avm.left_up, TC_size, aColours, TC_size, top_line_avm.circle, TCL_index_size);
    shader_line_init(&Tleft_down);
    vertices_line_init(&Tleft_down, top_avm.left_down, TC_size, aColours, TC_size, top_line_avm.circle, TCL_index_size);*/
}
/*
void avm_drawline_VBO()
{
    int iXangle = 0, iYangle = 0, iZangle = 0;
    float aRotate[16], aModelView[16], aPerspective[16];

    rotate_matrix(iXangle, 1.0, 0.0, 0.0, aModelView);
    rotate_matrix(iYangle, 0.0, 1.0, 0.0, aRotate);

    multiply_matrix(aRotate, aModelView, aModelView);

    rotate_matrix(iZangle, 0.0, 1.0, 0.0, aRotate);

    multiply_matrix(aRotate, aModelView, aModelView);


    aModelView[14] -= 55;//2.5

    perspective_matrix(90.0, (double)uiWidth/(double)uiHeight, 0.01, 200.0, aPerspective);
    multiply_matrix(aPerspective, aModelView, aMVP);

    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

    avm_line_restore_vbo(&bottom_front);
    GL_CHECK(glUniformMatrix4fv(bottom_front.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, BFBL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&bottom_back);
    GL_CHECK(glUniformMatrix4fv(bottom_back.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, BFBL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&bottom_right);
    GL_CHECK(glUniformMatrix4fv(bottom_right.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, BLRL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&bottom_left);
    GL_CHECK(glUniformMatrix4fv(bottom_left.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, BLRL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&Bfront_right);
    GL_CHECK(glUniformMatrix4fv(Bfront_right.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, BCL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&Bfront_left);
    GL_CHECK(glUniformMatrix4fv(Bfront_left.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, BCL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&Bback_right);
    GL_CHECK(glUniformMatrix4fv(Bback_right.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, BCL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&Bback_left);
    GL_CHECK(glUniformMatrix4fv(Bback_left.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, BCL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&Bright_up);
    GL_CHECK(glUniformMatrix4fv(Bright_up.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, BCL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&Bright_down);
    GL_CHECK(glUniformMatrix4fv(Bright_down.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, BCL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&Bleft_up);
    GL_CHECK(glUniformMatrix4fv(Bleft_up.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, BCL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&Bleft_down);
    GL_CHECK(glUniformMatrix4fv(Bleft_down.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, BCL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&Tfront_right);
    GL_CHECK(glUniformMatrix4fv(Tfront_right.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, TCL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&Tfront_left);
    GL_CHECK(glUniformMatrix4fv(Tfront_left.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, TCL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&Tback_right);
    GL_CHECK(glUniformMatrix4fv(Tback_right.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, TCL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&Tback_left);
    GL_CHECK(glUniformMatrix4fv(Tback_left.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, TCL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&top_front);
    GL_CHECK(glUniformMatrix4fv(top_front.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, TFBL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&top_back);
    GL_CHECK(glUniformMatrix4fv(top_back.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, TFBL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&top_right);
    GL_CHECK(glUniformMatrix4fv(top_right.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, TLRL_index_size, GL_UNSIGNED_SHORT,  0);

    avm_line_restore_vbo(&top_left);
    GL_CHECK(glUniformMatrix4fv(top_left.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP));
    glDrawElements(GL_LINES, TLRL_index_size, GL_UNSIGNED_SHORT,  0);

	//iXangle += 3;
	//iYangle += 2;
	//iZangle += 1;

	//if(iXangle >= 360) iXangle -= 360;
	//if(iXangle < 0) iXangle += 360;
	if(iYangle >= 90) iYangle -= 180;
	if(iYangle < 0) iYangle += 360;
	//if(iZangle >= 360) iZangle -= 360;
	//if(iZangle < 0) iZangle += 360;


}
*/

void avm_texture_init()
{
    //GLuint TextureID[4];

    BFBT_index_size=FBVslices*2*Cslices*3;
    BLRT_index_size=LRVslices*2*Cslices*3;
    BCT_index_size=(Vslices)*2*(Cslices-1)*3+(Vslices)*3;

    TFBT_index_size=FBVslices*2*(Hslices)*3;
    TLRT_index_size=LRVslices*2*(Hslices)*3;
    TCT_index_size=(Vslices)*2*(Hslices)*3;


#ifdef WISH
	shader_texture_rotate_init(&bottom_front);
#else
    shader_texture_init(&bottom_front);
#endif
	vertices_texture_init(&bottom_front,bottom_avm._front, BFB_size, bottom_texture_avm._front, BFB_size/3*2, bottom_tri_avm.front_back, BFBT_index_size);

#ifdef WISH
	shader_texture_rotate_init(&Bfront_left);
#else	
    shader_texture_init(&Bfront_left);
#endif
	vertices_texture_init(&Bfront_left,bottom_avm.front_left, BC_size, bottom_texture_avm.front_left, BC_size/3*2, bottom_tri_avm.circle, BCT_index_size);

#ifdef WISH
	shader_texture_rotate_init(&Bfront_right);
#else	
    shader_texture_init(&Bfront_right);
#endif
    vertices_texture_init(&Bfront_right,bottom_avm.front_right, BC_size, bottom_texture_avm.front_right, BC_size/3*2, bottom_tri_avm.circle, BCT_index_size);

#ifdef WISH
	shader_texture_rotate_init(&bottom_back);
#else	
    shader_texture_init(&bottom_back);
#endif
    vertices_texture_init(&bottom_back,bottom_avm._back, BFB_size, bottom_texture_avm._back, BFB_size/3*2, bottom_tri_avm.front_back, BFBT_index_size);

#ifdef WISH
	shader_texture_rotate_init(&Bback_left);
#else	
    shader_texture_init(&Bback_left);
#endif
    vertices_texture_init(&Bback_left,bottom_avm.back_left, BC_size, bottom_texture_avm.back_left, BC_size/3*2, bottom_tri_avm.circle, BCT_index_size);

#ifdef WISH
	shader_texture_rotate_init(&Bback_right);
#else	
    shader_texture_init(&Bback_right);
#endif
    vertices_texture_init(&Bback_right,bottom_avm.back_right, BC_size, bottom_texture_avm.back_right, BC_size/3*2, bottom_tri_avm.circle, BCT_index_size);


    shader_texture_init(&bottom_left);
    vertices_texture_init(&bottom_left,bottom_avm.left, BLR_size, bottom_texture_avm.left, BLR_size/3*2, bottom_tri_avm.right_left, BLRT_index_size);

    shader_texture_init(&bottom_right);
    vertices_texture_init(&bottom_right,bottom_avm.right, BLR_size, bottom_texture_avm.right, BLR_size/3*2, bottom_tri_avm.right_left, BLRT_index_size);


#ifdef WISH
	shader_texture_rotate_init(&top_front);
#else
    shader_texture_init(&top_front);
#endif
    vertices_texture_init(&top_front,top_avm._front, TFB_size, top_texture_avm._front, TFB_size/3*2, top_tri_avm.front_back, TFBT_index_size);

#ifdef WISH
	shader_texture_rotate_init(&top_back);
#else
    shader_texture_init(&top_back);
#endif
    vertices_texture_init(&top_back,top_avm._back, TFB_size, top_texture_avm._back, TFB_size/3*2, top_tri_avm.front_back, TFBT_index_size);

    shader_texture_init(&top_right);
    vertices_texture_init(&top_right,top_avm.right, TLR_size, top_texture_avm.right, TLR_size/3*2, top_tri_avm.right_left, TLRT_index_size);

    shader_texture_init(&top_left);
    vertices_texture_init(&top_left,top_avm.left, TLR_size, top_texture_avm.left, TLR_size/3*2, top_tri_avm.right_left, TLRT_index_size);

#ifdef WISH
	shader_texture_rotate_init(&Tfront_left);
#else
    shader_texture_init(&Tfront_left);
#endif
    vertices_texture_init(&Tfront_left,top_avm.front_left, TC_size, top_texture_avm.front_left, TC_size/3*2, top_tri_avm.circle, TCT_index_size);

#ifdef WISH
	shader_texture_rotate_init(&Tfront_right);
#else
    shader_texture_init(&Tfront_right);
#endif
	vertices_texture_init(&Tfront_right,top_avm.front_right, TC_size, top_texture_avm.front_right, TC_size/3*2, top_tri_avm.circle, TCT_index_size);

#ifdef WISH
	shader_texture_rotate_init(&Tback_left);
#else
    shader_texture_init(&Tback_left);
#endif
    vertices_texture_init(&Tback_left,top_avm.back_left, TC_size, top_texture_avm.back_left, TC_size/3*2, top_tri_avm.circle, TCT_index_size);

#ifdef WISH
	shader_texture_rotate_init(&Tback_right);
#else
    shader_texture_init(&Tback_right);
#endif
    vertices_texture_init(&Tback_right,top_avm.back_right, TC_size, top_texture_avm.back_right, TC_size/3*2, top_tri_avm.circle, TCT_index_size);

    //******************************overlap region***********************************
    shader_overlap_init(&Bright_up);
    vertices_overlap_init(&Bright_up,bottom_avm.right_up, BC_size, bottom_texture_avm.right_up, BC_size/3*2, bottom_tri_avm.circle, BCT_index_size,overlap_alpha.BT_left,BC_size/3);

    shader_overlap_init(&Bright_down);
    vertices_overlap_init(&Bright_down,bottom_avm.right_down, BC_size, bottom_texture_avm.right_down, BC_size/3*2, bottom_tri_avm.circle, BCT_index_size,overlap_alpha.BT_right,BC_size/3);

    shader_overlap_init(&Bleft_up);
    vertices_overlap_init(&Bleft_up,bottom_avm.left_up, BC_size, bottom_texture_avm.left_up, BC_size/3*2, bottom_tri_avm.circle, BCT_index_size,overlap_alpha.BT_right,BC_size/3);

    shader_overlap_init(&Bleft_down);
    vertices_overlap_init(&Bleft_down,bottom_avm.left_down, BC_size, bottom_texture_avm.left_down, BC_size/3*2, bottom_tri_avm.circle, BCT_index_size,overlap_alpha.BT_left,BC_size/3);

    shader_overlap_init(&Tright_up);
    vertices_overlap_init(&Tright_up,top_avm.right_up, TC_size, top_texture_avm.right_up, TC_size/3*2, top_tri_avm.circle, TCT_index_size, overlap_alpha.TP_right_up,TC_size/3);

    shader_overlap_init(&Tright_down);
    vertices_overlap_init(&Tright_down,top_avm.right_down, TC_size, top_texture_avm.right_down, TC_size/3*2, top_tri_avm.circle, TCT_index_size, overlap_alpha.TP_right_down,TC_size/3);

    shader_overlap_init(&Tleft_up);
    vertices_overlap_init(&Tleft_up,top_avm.left_up, TC_size, top_texture_avm.left_up, TC_size/3*2, top_tri_avm.circle, TCT_index_size, overlap_alpha.TP_left_up,TC_size/3);
    shader_overlap_init(&Tleft_down);
    vertices_overlap_init(&Tleft_down,top_avm.left_down, TC_size, top_texture_avm.left_down, TC_size/3*2, top_tri_avm.circle, TCT_index_size, overlap_alpha.TP_left_down,TC_size/3);


    //******************************overlap region***********************************

    /*glGenTextures(1, &TextureID[0]);
    GL_CHECK(glGenTextures);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureID[0]);
    GL_CHECK(glBindTexture);
    //load_CVimage(TextureID[0], "jeep_outside.bmp");

    glGenTextures(1, &TextureID[1]);
    GL_CHECK(glGenTextures);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, TextureID[1]);
    GL_CHECK(glBindTexture);
    //load_CVimage(TextureID[1], "jeep_outside.bmp");

    glGenTextures(1, &TextureID[2]);
    GL_CHECK(glGenTextures);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, TextureID[2]);
    GL_CHECK(glBindTexture);
    //load_CVimage(TextureID[2], "jeep_outside.bmp");

    glGenTextures(1, &TextureID[3]);
    GL_CHECK(glGenTextures);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, TextureID[3]);
    GL_CHECK(glBindTexture);
    //load_CVimage(TextureID[3], "jeep_outside.bmp");

    bottom_front.textureID=Bfront_left.textureID=Bfront_right.textureID=TextureID[0];
    bottom_right.textureID=Bright_up.textureID=Bright_down.textureID=TextureID[1];

    bottom_back.textureID=Bback_left.textureID=Bback_right.textureID=TextureID[2];
    bottom_left.textureID=Bleft_up.textureID=Bleft_down.textureID=TextureID[3];

    top_right.textureID=Tright_up.textureID=Tright_down.textureID=TextureID[1];
    top_left.textureID=Tleft_up.textureID=Tleft_down.textureID=TextureID[3];
    top_front.textureID=Tfront_right.textureID=Tfront_left.textureID=TextureID[0];
    top_back.textureID=Tback_right.textureID=Tback_left.textureID=TextureID[2];*/


}



void car_texture_init()
{
#ifdef REDFLAG

	GLuint carTexture;
	GLuint carwhTexture;
	GLuint carglTexture;

	shader_car_texture_init(&jeep_car);
	vertices_texture_init(&jeep_car, car_grid, sizeof(car_grid)/sizeof(float), car_texture, sizeof(car_texture)/sizeof(float), car_indexgrid, sizeof(car_indexgrid)/sizeof(short));

	shader_car_texture_init(&jeep_carWH);
	vertices_texture_init(&jeep_carWH, carwh_grid, sizeof(carwh_grid)/sizeof(float), carwh_texture, sizeof(carwh_texture)/sizeof(float), carwh_indexgrid, sizeof(carwh_indexgrid)/sizeof(short));

	shader_car_texture_init(&jeep_carGL);
	vertices_texture_init(&jeep_carGL, cargl_grid, sizeof(cargl_grid)/sizeof(float), cargl_texture, sizeof(cargl_texture)/sizeof(float), cargl_indexgrid, sizeof(cargl_indexgrid)/sizeof(short));

	//printf("car_grid: %d \n",sizeof(car_indexgrid)/sizeof(short));
	glGenTextures(1, &carTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, carTexture);
	load_texture_from_raw_file(carTexture, 1024, 1024, GL_RGB,  "/opt/London.bmp");
	
	glGenTextures(1, &carwhTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, carwhTexture);
	load_texture_from_raw_file(carwhTexture, 512, 512, GL_RGB,  "/opt/LondonWH.bmp");


	glGenTextures(1, &carglTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, carglTexture);
	load_texture_from_raw_file(carglTexture, 32, 32, GL_RGB,  "/opt/LondonGL.bmp");


	jeep_car.textureID = carTexture;
	jeep_carWH.textureID = carwhTexture;
	jeep_carGL.textureID = carglTexture;
	
	jeep_car.car_texture_size = sizeof(car_indexgrid)/sizeof(short);
	jeep_carWH.car_texture_size = sizeof(carwh_indexgrid)/sizeof(short);
	jeep_carGL.car_texture_size = sizeof(cargl_indexgrid)/sizeof(short);


#else

    GLuint carTexture;

    shader_car_texture_init(&jeep_car);
    vertices_texture_init(&jeep_car, car_grid, sizeof(car_grid)/sizeof(float), car_texture, sizeof(car_texture)/sizeof(float), car_indexgrid, sizeof(car_indexgrid)/sizeof(short));

    glGenTextures(1, &carTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, carTexture);
	load_texture_from_raw_file(carTexture, 512, 512, GL_RGB,  "/opt/jeep1.bmp");

    jeep_car.textureID = carTexture;
	jeep_car.car_texture_size = sizeof(car_indexgrid)/sizeof(short);

#endif
}



