/*
 *******************************************************************************
 *
 * Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 *******************************************************************************
 */

#ifndef _SGXRENDER3DSRV_H_
#define _SGXRENDER3DSRV_H_

#ifdef __cplusplus
extern "C" {
#endif
//define
#define SYSTEM_EGL_MAX_TEXTURES    (100)
typedef void *EGLImageKHR;
//*****************************
//#include <osa.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <GLES2/gl2ext.h>

//#include <mcfw/interfaces/link_api/system_const.h>
typedef struct {

  //  System_VideoDataFormat dataFormat;
    /**< SUPPORTED Formats,
     *    SYSTEM_DF_YUV420SP_UV
     */

  //  UInt32 width;
    /**< in pixels */

 //   UInt32 height;
    /**< in lines */

 //   UInt32 pitch[SYSTEM_MAX_PLANES];
    /**< in bytes, only pitch[0] used right now */

} System_EglTexProperty;
typedef struct
{
  /*  EGLDisplay display;
    EGLConfig config;
    EGLContext context;
    EGLSurface surface;

    GLuint      texYuv[SYSTEM_EGL_MAX_TEXTURES];
    EGLImageKHR texImg[SYSTEM_EGL_MAX_TEXTURES];
    Void        *bufAddr[SYSTEM_EGL_MAX_TEXTURES];
    int numBuf;

    int width;
    int height;
*/
} System_EglObj;
//#include <linux/src/system/system_drm_egl.h>
//#include "system_drm_egl.h"

//#define POINTS_WIDTH 320
//#define POINTS_HEIGHT 180

//#define POINTS_WIDTH 2
//#define POINTS_HEIGHT 2

#define POINTS_WIDTH (440)
#define POINTS_HEIGHT (540)

#define ENABLE_VBO_DRAW
#define ENABLE_CPP

#if 0
#define D_PRINTF  printf
#define GL_CHECK(x) \
{ \
int err = glGetError(); \
printf("GL Error = %x for %s\n", err, (char*)(#x)); \
}
#else
static inline void dummy_printf(char* in, ...){};
#define D_PRINTF dummy_printf
#define GL_CHECK(x)
#endif


  typedef unsigned short LUT_DATATYPE;
  typedef unsigned char BLENDLUT_DATATYPE;
  #define GL_LUT_DATATYPE GL_UNSIGNED_SHORT
  #define GL_BLENDLUT_DATATYPE GL_UNSIGNED_BYTE


/*
typedef struct _gl_state
{
    int program;
    GLuint textureID[4];
    GLuint attribIndices[3]; // 20150526 vertex, texture, alpha
    GLint mvMatrixOffsetLoc;
    GLint samplerLoc;
    GLuint vboID[4];
    float carMouse[5];    
    float delta[5];
    float carMouseMax[5];    
    float carMouseMin[5];        
}gl_state;*/

#ifdef TEXTURE_FROM_LOCAL
#define TEXTURE_WIDTH 1
#define TEXTURE_HEIGHT 1
#else
#define TEXTURE_WIDTH 736
#define TEXTURE_HEIGHT 480
#endif

#define MAX_IMAGES      (4)

#ifndef EGL_TI_raw_video
#  define EGL_TI_raw_video 1
#  define EGL_RAW_VIDEO_TI                                      0x333A  /* eglCreateImageKHR target */
#  define EGL_GL_VIDEO_FOURCC_TI                                0x3331  /* eglCreateImageKHR attribute */
#  define EGL_GL_VIDEO_WIDTH_TI                                 0x3332  /* eglCreateImageKHR attribute */
#  define EGL_GL_VIDEO_HEIGHT_TI                                0x3333  /* eglCreateImageKHR attribute */
#  define EGL_GL_VIDEO_BYTE_STRIDE_TI                           0x3334  /* eglCreateImageKHR attribute */
#  define EGL_GL_VIDEO_BYTE_SIZE_TI                             0x3335  /* eglCreateImageKHR attribute */
#  define EGL_GL_VIDEO_YUV_FLAGS_TI                             0x3336  /* eglCreateImageKHR attribute */
#endif

#ifndef EGLIMAGE_FLAGS_YUV_CONFORMANT_RANGE
#  define EGLIMAGE_FLAGS_YUV_CONFORMANT_RANGE (0 << 0)
#  define EGLIMAGE_FLAGS_YUV_FULL_RANGE       (1 << 0)
#  define EGLIMAGE_FLAGS_YUV_BT601            (0 << 1)
#  define EGLIMAGE_FLAGS_YUV_BT709            (1 << 1)
#endif

#define FOURCC(a, b, c, d) ((uint32_t)(uint8_t)(a) | ((uint32_t)(uint8_t)(b) << 8) | ((uint32_t)(uint8_t)(c) << 16) | ((uint32_t)(uint8_t)(d) << 24 ))
#define FOURCC_STR(str)    FOURCC(str[0], str[1], str[2], str[3])

/*struct EglimgTexMap {
        Bool mapped;
        GLuint tex;
        EGLImageKHR eglimg;
        char *imgbuf;
};*/
struct GLConfig {
        EGLDisplay display;
        EGLConfig config;
        EGLContext context;
        EGLSurface surface;
};
struct GLConfig gl;
//struct EglimgTexMap eglMapArray[MAX_IMAGES];

typedef struct
{
   GLuint vertexPositionAttribLoc; //4 panes+1 car
   GLuint vertexTexCoord1AttribLoc; //4 panes+1 car
   GLuint vertexTexCoord2AttribLoc; //4 panes+1 car
   GLuint vertexIndexImage1AttribLoc; //4 panes+1 car
   GLuint vertexIndexImage2AttribLoc; //4 panes+1 car
   GLuint blendAttribLoc; //blend of LUT
   GLuint uiProgramObject;
   gl_state car_gl1;

   GLint samplerLocation0;
   GLint samplerLocation1;
   GLint samplerLocation2;
   GLint samplerLocation3;
   GLint projMatrixLocation;
   GLint mvMatrixLocation;

   float mvMatrix[16];
   float projMatrix[16];

   float mouse;

   void * LUT3D;
   void * blendLUT3D;
   void * PALUT3D;
   
   float delta;
   System_EglTexProperty texProp;
   
   int screen_width;
   int screen_height;

} SgxRender3DSRV_Obj;

int SgxRender3DSRV_setup(SgxRender3DSRV_Obj *pObj);
void SgxRender3DSRV_renderFrame(SgxRender3DSRV_Obj *pObj, System_EglObj *pEglObj, GLuint *texYuv);

/* used by rendering internally.
 * NOT to be used by sgxDisplayLink directly
 */
GLuint SgxRender3DSRV_createProgram(const char* pVertexSource, const char* pFragmentSource); 
void SgxRender3DSRV_renderFrame3DSRV(SgxRender3DSRV_Obj *pObj, System_EglObj *pEglObj, GLuint *texYuv);
void get_matrix_output_car(float translate, float angleX, float angleY, float angleZ, float scaleX, float scaleY, float scaleZ, float* matrix, int viewid);
void get_matrix_output_mesh(float translate, float angleX, float angleY, float angleZ, float scaleX, float scaleY, float scaleZ, float* matrix);


int car_init_vertices_vbo();
void car_draw_vbo();

void screen1_draw_vbo(int texYuv);
void screen1_init_vbo();

// 20150303 3D AVM
void avm_draw_vbo(int bufferindex1,int bufferindex2,int bufferindex3,int bufferindex4);
//void avm_init_vbo();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*   _SGXRENDER3DSRV_H_    */
