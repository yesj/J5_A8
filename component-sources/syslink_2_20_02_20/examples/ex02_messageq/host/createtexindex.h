#ifndef CREATETEXINDEX_H
#define CREATETEXINDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "model.h"
#include <string.h>
#include "AutoRadAVMLib.h"

#define outPutWidth (736)
#define outPutHeight (480)

void BirdsEyeViewInit();
void Tex_bottom_front(float* BF_ptr, float* BR_ptr, float* BL_ptr, float car_width, float extend, int mesh_width, int mesh_height,int camID);
void Tex_bottom_left(float* BF_ptr, float* BR_ptr, float* BL_ptr, float car_height, float extend, int mesh_width, int mesh_height,int camID);
void texture_vertex_create(AVM_ptr *verticesB, AVM_ptr *verticesT, float car_width, float car_length, int mesh_offset, int extend_opt);
void pointintersect(float point11_x, float point11_y, float point12_x, float point12_y, float point21_x, float point21_y, float point22_x, float point22_y, float* result_x, float* result_y);


#endif /* CUBE_H */
