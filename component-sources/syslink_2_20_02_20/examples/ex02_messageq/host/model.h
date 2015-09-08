/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009 - 2011 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Car type
//#define WISH


#define PI 3.1415926535897932384626433832795f

#ifdef WISH
#define carwidth 200.0   //WISH
#define carlength 480.0
#define LRcam_height 97.0   
#define Fcam_height 57.0
#define Bcam_height 94.0
#else
#define carwidth 210.0   //家ó
#define carlength 510.0
#define LRcam_height 100.0   
#define Fcam_height 70.0
#define Bcam_height 100.0
#endif

#define xyextend 400
#define Vslices 10
#define Hslices 5
#define FBVslices 16             //玡┏场家㏕﹚ち澄(程2nΩよ,家郴翴篶俱埃ぃ穦瞷粇畉)
#define LRVslices 32             //オ┏场家㏕﹚ち澄
#define Cslices 16




typedef struct
{
	float* front_left;
	float* _front;
	float* front_right;

	float* right_up;
	float* right;
	float* right_down;

	float* left_up;
	float* left;
	float* left_down;

	float* back_left;
	float* _back;
	float* back_right;
} AVM_ptr;

typedef struct
{
	unsigned short* front_back;
	unsigned short* right_left;
	unsigned short* circle;
} AVM_index_ptr;

typedef struct
{
	float* BT_right;
	float* BT_left;
	float* TP_right_up;
	float* TP_right_down;
	float* TP_left_up;
	float* TP_left_down;
} overlap_ptr;

void model_create(AVM_ptr *verticesB,AVM_ptr *verticesT, float car_width, float car_length, int mesh_offset,int extend_opt);
void Line_index_create(AVM_index_ptr *index_lineB, AVM_index_ptr *index_lineT, int car_width, int car_length, int offset, int extend_opt);
void Triangles_index_create(AVM_index_ptr *index_triB, AVM_index_ptr *index_triT, int car_width, int car_length, int offset, int extend_opt);
void overlap_create(overlap_ptr *alpha);
#endif /* CUBE_H */
