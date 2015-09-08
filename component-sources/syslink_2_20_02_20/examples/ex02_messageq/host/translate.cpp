
#define REDFLAG

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
extern "C"
{
    #include "translate.h"
    #include "model.h"
}
using namespace glm;

static void math_view_matrix(glm::vec3 eye, glm::vec3 center, glm::vec3 up, float* out)
{
    glm::mat4 Projection = glm::perspective(45.0f, 720.0f/480.0f,  0.1f, 200.f);
    glm::mat4 lookat =glm::lookAt(eye,center,up);
    glm::mat4 MVP = Projection*lookat;
    memcpy(out, glm::value_ptr(MVP), 4*4*sizeof(float));

}

static void Vmath_view_matrix(glm::vec3 eye, glm::vec3 center, glm::vec3 up, float* out)
{
    glm::mat4 Projection = glm::perspective(90.0f, 720.0f/960.0f,  0.1f, 200.f);
    glm::mat4 lookat =glm::lookAt(eye,center,up);
    glm::mat4 MVP = Projection*lookat;
    memcpy(out, glm::value_ptr(MVP), 4*4*sizeof(float));

}


static void car_math_view_matrix(glm::vec3 eye, glm::vec3 center, glm::vec3 up, float* out)
{
#ifdef REDFLAG
	//london
	glm::mat4 Projection = glm::perspective(45.0f, 720.0f/480.0f,  0.1f, 200.f);
	glm::mat4 lookat =glm::lookAt(eye,center,up);
	glm::mat4 ViewRotateY =glm::rotate(lookat,-180.0f,glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 ViewRotateX =glm::rotate(ViewRotateY,90.0f,glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 MoveZ =glm::translate(ViewRotateX,glm::vec3(0.0f, 0.0f, 0.2f));			//london
	glm::mat4 model =glm::scale(glm::mat4(1.0f),glm::vec3(4.8f, 4.8f, 4.8f));			//london
	glm::mat4 MVP = Projection*MoveZ*model;
	memcpy(out, glm::value_ptr(MVP), 4*4*sizeof(float));

#else
	//TI CAR  JEEP
    glm::mat4 Projection = glm::perspective(45.0f, 720.0f/480.0f,  0.1f, 200.f);
    glm::mat4 lookat =glm::lookAt(eye,center,up);
    glm::mat4 ViewRotateY =glm::rotate(lookat,-180.0f,glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 ViewRotateX =glm::rotate(ViewRotateY,90.0f,glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 MoveZ =glm::translate(ViewRotateX,glm::vec3(0.0f, 10.0f, 4.5f));
    //glm::mat4 MoveZ =glm::translate(lookat,glm::vec3(0.0f, 10.0f, 4.5f));
    glm::mat4 model =glm::scale(glm::mat4(1.0f),glm::vec3(8.0f, 6.0f, 8.0f)); //ªø °ª ¼e

    //glm::mat4 MVP = Projection*lookat*model;
    //glm::mat4 MVP = Projection*ViewRotateX*model;
    glm::mat4 MVP = Projection*MoveZ*model;
    memcpy(out, glm::value_ptr(MVP), 4*4*sizeof(float));
#endif
}


static void Vcar_math_view_matrix(glm::vec3 eye, glm::vec3 center, glm::vec3 up, float* out)
{
    glm::mat4 Projection = glm::perspective(90.0f, 720.0f/960.0f,  0.1f, 200.f);
    glm::mat4 lookat =glm::lookAt(eye,center,up);
    glm::mat4 ViewRotateY =glm::rotate(lookat,-PI,glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 ViewRotateX =glm::rotate(ViewRotateY,PI/2,glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 MoveZ =glm::translate(ViewRotateX,glm::vec3(0.0f, 0.0f, 0.2f));
    glm::mat4 model =glm::scale(glm::mat4(1.0f),glm::vec3(4.8f, 4.8f, 4.8f));    //london
    //glm::mat4 model =glm::scale(glm::mat4(1.0f),glm::vec3(8.2f, 6.2f, 8.2f));         //jeep
    //glm::mat4 model =glm::scale(glm::mat4(1.0f),glm::vec3(0.005f, 0.005f, 0.005f)); //BMW
    //glm::mat4 model =glm::scale(glm::mat4(1.0f),glm::vec3(8.0f, 8.0f, 8.0f));         //jeep
    //glm::mat4 MVP = Projection*lookat*model;
    //glm::mat4 MVP = Projection*ViewRotateX*model;
    glm::mat4 MVP = Projection*MoveZ*model;
    memcpy(out, glm::value_ptr(MVP), 4*4*sizeof(float));

}

extern "C" void camera_LookAt(float eye_x, float eye_y, float eye_z, float center_x, float center_y, float center_z, float up_x, float up_y, float up_z, float *matrix)
{
    math_view_matrix(glm::vec3(eye_x,eye_y,eye_z), glm::vec3(center_x,center_y,center_z), glm::vec3(up_x,up_y,up_z), matrix);
}

extern "C" void car_LookAt(float eye_x, float eye_y, float eye_z, float center_x, float center_y, float center_z, float up_x, float up_y, float up_z, float *matrix)
{
    car_math_view_matrix(glm::vec3(eye_x,eye_y,eye_z), glm::vec3(center_x,center_y,center_z), glm::vec3(up_x,up_y,up_z), matrix);
}


extern "C" void Vcamera_LookAt(float eye_x, float eye_y, float eye_z, float center_x, float center_y, float center_z, float up_x, float up_y, float up_z, float *matrix)
{
    Vmath_view_matrix(glm::vec3(eye_x,eye_y,eye_z), glm::vec3(center_x,center_y,center_z), glm::vec3(up_x,up_y,up_z), matrix);
}

extern "C" void Vcar_LookAt(float eye_x, float eye_y, float eye_z, float center_x, float center_y, float center_z, float up_x, float up_y, float up_z, float *matrix)
{
    Vcar_math_view_matrix(glm::vec3(eye_x,eye_y,eye_z), glm::vec3(center_x,center_y,center_z), glm::vec3(up_x,up_y,up_z), matrix);
}


