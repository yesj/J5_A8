/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009 - 2011 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

#ifndef SHADER_H
#define SHADER_H

#include "GLES2/gl2.h"
#include "EGL/egl.h"
//#include "model.h"


typedef struct _gl_state
{
    int program;
    GLuint textureID;
    GLuint attribIndices[3]; // 20150526 vertex, texture, alpha
    GLint mvMatrixOffsetLoc;
    GLint samplerLoc;
    GLuint vboID[4];
    float carMouse[5];    
    float delta[5];
    float carMouseMax[5];    
    float carMouseMin[5]; 
	int car_texture_size;
}gl_state;

//GLuint AVM3D_createProgram(const char* VertexSource, const char* FragmentSource);
GLuint AVM3D_createProgram();


char* load_shader(char *sFilename);
GLuint process_shader(char *sFilename, GLint iShaderType);
void shader_line_init(gl_state *screen_state);
void vertices_line_init(gl_state* screen_state,float* avm_mesh, int meshSize,float* avm_color, int colorSize,unsigned short* avm_index_mesh, int indexSize);
void avm_line_restore_vbo(gl_state* screen_state);

void shader_texture_init(gl_state *screen_state);
void vertices_texture_init(gl_state* screen_state, float* avm_mesh, int meshSize, float* avm_texture, int textureSize, unsigned short* avm_index_mesh, int indexSize);
void avm_texture_restore_vbo(gl_state* screen_state, int texYuv);

void shader_overlap_init(gl_state *screen_state);
void vertices_overlap_init(gl_state* screen_state, float* avm_mesh, int meshSize, float* avm_texture, int textureSize, unsigned short* avm_index_mesh, int indexSize, float* avm_alpha, int alphaSize);
void avm_overlap_restore_vbo(gl_state* screen_state, int texYuv);

int avm_init_vertices_vbo(gl_state* screen_state, float* avm_mesh, int meshSize, float* avm_texture, int textureSize, unsigned short* avm_index_mesh, int indexSize);

#endif

