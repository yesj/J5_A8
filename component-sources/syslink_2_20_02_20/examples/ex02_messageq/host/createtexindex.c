#define DEBUG
#include "createtexindex.h"
unsigned char videoOutput[outPutWidth*outPutHeight*3/2];
float bottom_FLUD[(Vslices+1)*2];        //bottom Front Left UD
float bottom_FRUD[(Vslices+1)*2];
float bottom_BLUD[(Vslices+1)*2];
float bottom_BRUD[(Vslices+1)*2];

float bottom_RUUD[(Vslices+1)*2];        //bottom Right Up UD
float bottom_RDUD[(Vslices+1)*2];
float bottom_LUUD[(Vslices+1)*2];
float bottom_LDUD[(Vslices+1)*2];

void BirdsEyeViewInit()
{
    memset(videoOutput,0x80,sizeof(videoOutput));
    AutoRadHomoFun(videoOutput, videoOutput,videoOutput);
}

void pointintersect(float point11_x, float point11_y, float point12_x, float point12_y, float point21_x, float point21_y, float point22_x, float point22_y, float* result_x, float* result_y)
{
    float A_x, A_y;
    float B_x, B_y;
    float S_x, S_y;

    float crossAB;
    float crossSB;

    A_x=point12_x-point11_x;    A_y=point12_y-point11_y;
    B_x=point22_x-point21_x;    B_y=point22_y-point21_y;
    S_x=point21_x-point11_x;    S_y=point21_y-point11_y;


    crossAB = (A_x*B_y)-(A_y*B_x);
    crossSB = (S_x*B_y)-(S_y*B_x);

    *(result_x)= point11_x + (A_x*(crossSB/crossAB));
    *(result_y)= point11_y + (A_y*(crossSB/crossAB));
}

void Tex_bottom_front(float* BF_ptr, float* BR_ptr, float* BL_ptr, float car_width, float extend, int mesh_width, int mesh_height,int camID)
{
    int i,j,k,temp;
    float *front_start, *right_start, *left_start;
    float *LUD_array, *RUD_array;
    float ftotal_width, ftotal_height;
    float start_x, start_y;
    float finish_x, finish_y;
    float interval_x, interval_y;
    float UD_x, UD_y, ORG_x, ORG_y, bird_x, bird_y;
    float interval_angle, angle, radious;


    //***********紀錄底部1/4最外圈的UD值,計算周圍模型會用到*******
    if(camID==0)
    {
        LUD_array=&bottom_FLUD[0];
        RUD_array=&bottom_FRUD[0];
        RUD_array=RUD_array+2;
    }
    else
    {
        LUD_array=&bottom_BLUD[0];
        RUD_array=&bottom_BRUD[0];
        RUD_array=RUD_array+2;
    }
    //***********紀錄底部1/4最外圈的UD值,計算周圍模型會用到*******

    interval_angle=PI/2/Vslices;
    front_start=BF_ptr;
    right_start=BR_ptr;
    left_start =BL_ptr;
    AutoRadHomoInit(camID);


    ftotal_width=car_width+2*extend;
    ftotal_height=extend;
    start_x =(outPutWidth*extend)/ftotal_width;
    finish_x=(outPutWidth*(extend+car_width))/ftotal_width;

    interval_x=(finish_x-start_x)/(mesh_width-1);
    interval_y=outPutHeight/(mesh_height-1);

    bird_x=start_x;
    bird_y=0;

    //vvvvvvvvvvvvvvvv  bottom_front_texture  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    for(i=0;i<mesh_height;i++)
    {
        for(j=0;j<mesh_width;j++)
        {
            AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
            AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
            ORG_x=ORG_x/outPutWidth;
            ORG_y=ORG_y/outPutHeight;
            if (ORG_x < 0) {ORG_x = 0;}
			if (ORG_x > 1) {ORG_x = 1;}
			if (ORG_y < 0) {ORG_y = 0;}
			if (ORG_y > 1) {ORG_y = 1;}

			*(BF_ptr)=ORG_x;
			*(BF_ptr+1)=ORG_y;
			BF_ptr=BF_ptr+2;

			bird_x=bird_x+interval_x;
        }
        bird_x=start_x;
        bird_y=bird_y+interval_y;
    }
    //^^^^^^^^^^^^^^^^  bottom_front_texture  ^^^^^^^^^^^^^^^^^^^^^^^^^^^

    //***************將 bottom_front 兩側座標直接配置給right的左側與left的右側************
    BF_ptr=front_start;
    BR_ptr=right_start;
    BL_ptr=left_start+Vslices*2;
    for(i=0;i<mesh_height-1;i++)
    {
        *(BL_ptr)=*(BF_ptr);                   *(BL_ptr+1)=*(BF_ptr+1);
        *(BR_ptr)=*(BF_ptr+(mesh_width-1)*2);  *(BR_ptr+1)=*(BF_ptr+(mesh_width-1)*2+1);

        if(i!=mesh_height-2)
        {BL_ptr=BL_ptr+(Vslices+1)*2;}
        BR_ptr=BR_ptr+(Vslices+1)*2;
        BF_ptr=BF_ptr+mesh_width*2;
    }
    BL_ptr=BL_ptr+2;
    *(BL_ptr)=*(BF_ptr);                   *(BL_ptr+1)=*(BF_ptr+1);
    *(BR_ptr)=*(BF_ptr+(mesh_width-1)*2);  *(BR_ptr+1)=*(BF_ptr+(mesh_width-1)*2+1);


    //***************將 bottom_front 兩側座標直接配置給right的左側與left的右側************
    BF_ptr=front_start;
    BR_ptr=right_start;
    BL_ptr=left_start;

    //vvvvvvvvvvvvvvvv  bottom_front_left_texture  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    angle=PI;
    radious=extend;
    temp=0;
    bird_x= 0.0f;                                      //先將起始點填入材質矩陣(用三角函數計算怕會有誤差)
    bird_y=outPutHeight;
    AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
    AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
    ORG_x=ORG_x/outPutWidth;
    ORG_y=ORG_y/outPutHeight;
    if (ORG_x < 0) {ORG_x = 0;}
    if (ORG_x > 1) {ORG_x = 1;}
    if (ORG_y < 0) {ORG_y = 0;}
    if (ORG_y > 1) {ORG_y = 1;}
    *(BL_ptr)=ORG_x;
    *(BL_ptr+1)=ORG_y;
    BL_ptr=BL_ptr+2;

    *(LUD_array)=UD_x;
    *(LUD_array+1)=UD_y;
    LUD_array=LUD_array+2;

    //*************1/4圓最外圈 需計算與x軸  y軸的交點************************
    start_x=extend;
    start_y=extend;
    for(i=0;i<Vslices-1;i++)
    {
        angle=angle+interval_angle;
        finish_x=start_x+(radious*cos(angle));
        finish_y=start_y+(radious*sin(angle));
        if(temp==0)
        {
            pointintersect(start_x, start_y,finish_x,finish_y,
                              0.0f,    0.0f,    0.0f,    3.0f,
                           &bird_x, &bird_y);
            bird_x=0.0f;
            bird_y=bird_y*outPutHeight/extend;
            if(bird_y<0)
            {temp=1;}
            else
            {
                AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
                AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
                ORG_x=ORG_x/outPutWidth;
                ORG_y=ORG_y/outPutHeight;
                if (ORG_x < 0) {ORG_x = 0;}
                if (ORG_x > 1) {ORG_x = 1;}
                if (ORG_y < 0) {ORG_y = 0;}
                if (ORG_y > 1) {ORG_y = 1;}
                *(BL_ptr)=ORG_x;
                *(BL_ptr+1)=ORG_y;
                BL_ptr=BL_ptr+2;

                *(LUD_array)=UD_x;
                *(LUD_array+1)=UD_y;
                LUD_array=LUD_array+2;
            }
        }
        if(temp==1)
        {
            pointintersect(start_x, start_y,finish_x,finish_y,
                              0.0f,    0.0f,    3.0f,    0.0f,
                           &bird_x, &bird_y);
            bird_x=bird_x*(outPutWidth/ftotal_width);
            bird_y=0.0f;

            AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
            AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
            ORG_x=ORG_x/outPutWidth;
            ORG_y=ORG_y/outPutHeight;
            if (ORG_x < 0) {ORG_x = 0;}
            if (ORG_x > 1) {ORG_x = 1;}
            if (ORG_y < 0) {ORG_y = 0;}
            if (ORG_y > 1) {ORG_y = 1;}
            *(BL_ptr)=ORG_x;
            *(BL_ptr+1)=ORG_y;
            BL_ptr=BL_ptr+2;

            *(LUD_array)=UD_x;
            *(LUD_array+1)=UD_y;
            LUD_array=LUD_array+2;
        }

    }
    //*************1/4圓最外圈 需計算與x軸  y軸的交點************************
    BL_ptr=BL_ptr+2;
    //*************1/4圓內圈************************
    interval_x=extend/Cslices;
    radious=radious-interval_x;
    angle=PI;
    start_x=extend;
    start_y=extend;
    for(i=0;i<(int)Cslices-1;i++)
    {
        for(j=0;j<Vslices;j++)
        {
            if(j==0)
            {
                bird_x=(extend-radious)*outPutWidth/ftotal_width;
                bird_y=outPutHeight;
                AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
                AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
                ORG_x=ORG_x/outPutWidth;
                ORG_y=ORG_y/outPutHeight;
                if (ORG_x < 0) {ORG_x = 0;}
                if (ORG_x > 1) {ORG_x = 1;}
                if (ORG_y < 0) {ORG_y = 0;}
                if (ORG_y > 1) {ORG_y = 1;}
                *(BL_ptr)=ORG_x;
                *(BL_ptr+1)=ORG_y;
                BL_ptr=BL_ptr+2;
            }
            else
            {
                angle=angle+interval_angle;
                bird_x=(start_x+radious*cos(angle))*outPutWidth/ftotal_width;
                bird_y=(start_y+radious*sin(angle))*outPutHeight/ftotal_height;
                AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
                AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
                ORG_x=ORG_x/outPutWidth;
                ORG_y=ORG_y/outPutHeight;
                if (ORG_x < 0) {ORG_x = 0;}
                if (ORG_x > 1) {ORG_x = 1;}
                if (ORG_y < 0) {ORG_y = 0;}
                if (ORG_y > 1) {ORG_y = 1;}
                *(BL_ptr)=ORG_x;
                *(BL_ptr+1)=ORG_y;
                BL_ptr=BL_ptr+2;
            }
        }
        BL_ptr=BL_ptr+2;
        angle=PI;
        radious=radious-interval_x;
    }
    //*************1/4圓內圈*************************/

    //*************1/4圓最外圈 需計算與x軸  y軸的交點************************
    BR_ptr=right_start+2;     //
    temp=0;
    angle=PI*3/2;
    radious=extend;
    start_x=extend+car_width;
    start_y=extend;
    for(i=0;i<Vslices-1;i++)
    {
        angle=angle+interval_angle;
        finish_x=start_x+(radious*cos(angle));
        finish_y=start_y+(radious*sin(angle));
        if(temp==0)
        {
            pointintersect(start_x, start_y,finish_x,finish_y,
                              0.0f,    0.0f,    5.0f,    0.0f,
                           &bird_x, &bird_y);

            if(bird_x>ftotal_width)
            {temp=1;}
            else
            {
                bird_x=bird_x*outPutWidth/ftotal_width;
                bird_y=0;
                AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
                AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
                ORG_x=ORG_x/outPutWidth;
                ORG_y=ORG_y/outPutHeight;
                if (ORG_x < 0) {ORG_x = 0;}
                if (ORG_x > 1) {ORG_x = 1;}
                if (ORG_y < 0) {ORG_y = 0;}
                if (ORG_y > 1) {ORG_y = 1;}
                *(BR_ptr)=ORG_x;
                *(BR_ptr+1)=ORG_y;
                BR_ptr=BR_ptr+2;

                *(RUD_array)=UD_x;
                *(RUD_array+1)=UD_y;
                RUD_array=RUD_array+2;
            }
        }
        if(temp==1)
        {
            pointintersect(start_x, start_y,finish_x,finish_y,
                           ftotal_width, 0.0f, ftotal_width, 10.0f,
                           &bird_x, &bird_y);
            bird_x=outPutWidth;
            bird_y=bird_y*outPutHeight/ftotal_height;

            AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
            AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
            ORG_x=ORG_x/outPutWidth;
            ORG_y=ORG_y/outPutHeight;
            if (ORG_x < 0) {ORG_x = 0;}
            if (ORG_x > 1) {ORG_x = 1;}
            if (ORG_y < 0) {ORG_y = 0;}
            if (ORG_y > 1) {ORG_y = 1;}
            *(BR_ptr)=ORG_x;
            *(BR_ptr+1)=ORG_y;
            BR_ptr=BR_ptr+2;

            *(RUD_array)=UD_x;
            *(RUD_array+1)=UD_y;
            RUD_array=RUD_array+2;
        }

    }
    //*************1/4圓最外圈 需計算與x軸  y軸的交點************************
    bird_x=outPutWidth;                                      //將結束點填入材質矩陣(用三角函數計算怕會有誤差)
    bird_y=outPutHeight;
    AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
    AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
    ORG_x=ORG_x/outPutWidth;
    ORG_y=ORG_y/outPutHeight;
    if (ORG_x < 0) {ORG_x = 0;}
    if (ORG_x > 1) {ORG_x = 1;}
    if (ORG_y < 0) {ORG_y = 0;}
    if (ORG_y > 1) {ORG_y = 1;}
    *(BR_ptr)=ORG_x;
    *(BR_ptr+1)=ORG_y;
    BR_ptr=BR_ptr+4;

    *(RUD_array)=UD_x;
    *(RUD_array+1)=UD_y;
    //*************1/4圓內圈************************
    radious=radious-interval_x;
    angle=(PI*3/2);
    start_x=extend+car_width;
    start_y=extend;
    for(i=0;i<Cslices-1;i++)
    {
        for(j=0;j<Vslices;j++)
        {
            if(j==Vslices-1)
            {
                bird_x=(extend+car_width+radious)*outPutWidth/ftotal_width;
                bird_y=outPutHeight;
                AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
                AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
                ORG_x=ORG_x/outPutWidth;
                ORG_y=ORG_y/outPutHeight;
                if (ORG_x < 0) {ORG_x = 0;}
                if (ORG_x > 1) {ORG_x = 1;}
                if (ORG_y < 0) {ORG_y = 0;}
                if (ORG_y > 1) {ORG_y = 1;}
                *(BR_ptr)=ORG_x;
                *(BR_ptr+1)=ORG_y;
                BR_ptr=BR_ptr+2;
            }
            else
            {
                angle=angle+interval_angle;
                bird_x=(start_x+radious*cos(angle))*outPutWidth/ftotal_width;
                bird_y=(start_y+radious*sin(angle))*outPutHeight/ftotal_height;
                AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
                AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
                ORG_x=ORG_x/outPutWidth;
                ORG_y=ORG_y/outPutHeight;
                if (ORG_x < 0) {ORG_x = 0;}
                if (ORG_x > 1) {ORG_x = 1;}
                if (ORG_y < 0) {ORG_y = 0;}
                if (ORG_y > 1) {ORG_y = 1;}
                *(BR_ptr)=ORG_x;
                *(BR_ptr+1)=ORG_y;
                BR_ptr=BR_ptr+2;
            }
        }
        BR_ptr=BR_ptr+2;
        angle=(PI*3/2);
        radious=radious-interval_x;
    }
    //*************1/4圓內圈*************************/


}

void Tex_bottom_left(float* BF_ptr, float* BR_ptr, float* BL_ptr, float car_height, float extend, int mesh_width, int mesh_height,int camID)
{
    int i,j,k,temp;
    float *front_start, *right_start, *left_start;
    float ftotal_width, ftotal_height;
    float start_x, start_y;
    float *LUD_array, *RUD_array;
    float finish_x, finish_y;
    float interval_x, interval_y;
    float UD_x, UD_y, ORG_x, ORG_y, bird_x, bird_y;
    float interval_angle, angle, radious;



    //***********紀錄底部1/4最外圈的UD值,計算周圍模型會用到*******
    if(camID==1)
    {
        LUD_array=&bottom_RUUD[0];
        RUD_array=&bottom_RDUD[0];
        RUD_array=RUD_array+2;
    }
    else
    {
        LUD_array=&bottom_LDUD[0];
        RUD_array=&bottom_LUUD[0];
        RUD_array=RUD_array+2;
    }
    //***********紀錄底部1/4最外圈的UD值,計算周圍模型會用到*******


    interval_angle=PI/2/Vslices;
    front_start=BF_ptr;
    right_start=BR_ptr;
    left_start =BL_ptr;
    AutoRadHomoInit(camID);


    ftotal_width=car_height+2*extend;
    ftotal_height=extend;
    start_x =(outPutWidth*extend)/ftotal_width;
    finish_x=(outPutWidth*( extend+car_height))/ftotal_width;

    interval_x=(finish_x-start_x)/(mesh_width-1);
    interval_y=outPutHeight/(mesh_height-1);

    bird_x=start_x;
    bird_y=0;

    //vvvvvvvvvvvvvvvv  bottom_left_texture  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    for(i=0;i<mesh_height;i++)
    {
        for(j=0;j<mesh_width;j++)
        {
            AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
            AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
            ORG_x=ORG_x/outPutWidth;
            ORG_y=ORG_y/outPutHeight;
            if (ORG_x < 0) {ORG_x = 0;}
			if (ORG_x > 1) {ORG_x = 1;}
			if (ORG_y < 0) {ORG_y = 0;}
			if (ORG_y > 1) {ORG_y = 1;}

			*(BF_ptr)=ORG_x;
			*(BF_ptr+1)=ORG_y;
			BF_ptr=BF_ptr+2;

			bird_x=bird_x+interval_x;
        }
        bird_x=start_x;
        bird_y=bird_y+interval_y;
    }
    //^^^^^^^^^^^^^^^^  bottom_left_texture  ^^^^^^^^^^^^^^^^^^^^^^^^^^^

    //***************將 bottom_left/right 兩側座標直接配置給right的左側與left的右側************
    BF_ptr=front_start;
    BR_ptr=right_start;
    BL_ptr=left_start+Vslices*2;
    for(i=0;i<mesh_height-1;i++)
    {
        *(BL_ptr)=*(BF_ptr);                   *(BL_ptr+1)=*(BF_ptr+1);
        *(BR_ptr)=*(BF_ptr+(mesh_width-1)*2);  *(BR_ptr+1)=*(BF_ptr+(mesh_width-1)*2+1);

        if(i!=mesh_height-2)
        {BL_ptr=BL_ptr+(Vslices+1)*2;}
        BR_ptr=BR_ptr+(Vslices+1)*2;
        BF_ptr=BF_ptr+mesh_width*2;
    }
    BL_ptr=BL_ptr+2;
    *(BL_ptr)=*(BF_ptr);                   *(BL_ptr+1)=*(BF_ptr+1);
    *(BR_ptr)=*(BF_ptr+(mesh_width-1)*2);  *(BR_ptr+1)=*(BF_ptr+(mesh_width-1)*2+1);


    //***************將 bottom_front 兩側座標直接配置給right的左側與left的右側************
    BF_ptr=front_start;
    BR_ptr=right_start;
    BL_ptr=left_start;

    //vvvvvvvvvvvvvvvv  bottom_front_left_texture  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    angle=PI;
    radious=extend;
    temp=0;
    bird_x= 0.0f;                                      //先將起始點填入材質矩陣(用三角函數計算怕會有誤差)
    bird_y=outPutHeight;
    AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
    AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
    ORG_x=ORG_x/outPutWidth;
    ORG_y=ORG_y/outPutHeight;
    if (ORG_x < 0) {ORG_x = 0;}
    if (ORG_x > 1) {ORG_x = 1;}
    if (ORG_y < 0) {ORG_y = 0;}
    if (ORG_y > 1) {ORG_y = 1;}
    *(BL_ptr)=ORG_x;
    *(BL_ptr+1)=ORG_y;
    BL_ptr=BL_ptr+2;

    *(LUD_array)=UD_x;
    *(LUD_array+1)=UD_y;
    LUD_array=LUD_array+2;

    //*************1/4圓最外圈 需計算與x軸  y軸的交點************************
    start_x=extend;
    start_y=extend;
    for(i=0;i<Vslices-1;i++)
    {
        angle=angle+interval_angle;
        finish_x=start_x+(radious*cos(angle));
        finish_y=start_y+(radious*sin(angle));
        if(temp==0)
        {
            pointintersect(start_x, start_y,finish_x,finish_y,
                              0.0f,    0.0f,    0.0f,    3.0f,
                           &bird_x, &bird_y);
            bird_x=0.0f;
            bird_y=bird_y*outPutHeight/extend;
            if(bird_y<0)
            {temp=1;}
            else
            {
                AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
                AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
                ORG_x=ORG_x/720;
                ORG_y=ORG_y/480;
                if (ORG_x < 0) {ORG_x = 0;}
                if (ORG_x > 1) {ORG_x = 1;}
                if (ORG_y < 0) {ORG_y = 0;}
                if (ORG_y > 1) {ORG_y = 1;}
                *(BL_ptr)=ORG_x;
                *(BL_ptr+1)=ORG_y;
                BL_ptr=BL_ptr+2;

                *(LUD_array)=UD_x;
                *(LUD_array+1)=UD_y;
                LUD_array=LUD_array+2;
            }
        }
        if(temp==1)
        {
            pointintersect(start_x, start_y,finish_x,finish_y,
                              0.0f,    0.0f,    3.0f,    0.0f,
                           &bird_x, &bird_y);
            bird_x=bird_x*(outPutWidth/ftotal_width);
            bird_y=0.0f;

            AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
            AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
            ORG_x=ORG_x/outPutWidth;
            ORG_y=ORG_y/outPutHeight;
            if (ORG_x < 0) {ORG_x = 0;}
            if (ORG_x > 1) {ORG_x = 1;}
            if (ORG_y < 0) {ORG_y = 0;}
            if (ORG_y > 1) {ORG_y = 1;}
            *(BL_ptr)=ORG_x;
            *(BL_ptr+1)=ORG_y;
            BL_ptr=BL_ptr+2;

            *(LUD_array)=UD_x;
            *(LUD_array+1)=UD_y;
            LUD_array=LUD_array+2;
        }

    }
    //*************1/4圓最外圈 需計算與x軸  y軸的交點************************
    BL_ptr=BL_ptr+2;
    //*************1/4圓內圈************************
    interval_x=extend/Cslices;
    radious=radious-interval_x;
    angle=PI;
    start_x=extend;
    start_y=extend;
    for(i=0;i<Cslices-1;i++)
    {
        for(j=0;j<Vslices;j++)
        {
            if(j==0)
            {
                bird_x=(extend-radious)*outPutWidth/ftotal_width;
                bird_y=outPutHeight;
                AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
                AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
                ORG_x=ORG_x/outPutWidth;
                ORG_y=ORG_y/outPutHeight;
                if (ORG_x < 0) {ORG_x = 0;}
                if (ORG_x > 1) {ORG_x = 1;}
                if (ORG_y < 0) {ORG_y = 0;}
                if (ORG_y > 1) {ORG_y = 1;}
                *(BL_ptr)=ORG_x;
                *(BL_ptr+1)=ORG_y;
                BL_ptr=BL_ptr+2;
            }
            else
            {
                angle=angle+interval_angle;
                bird_x=(start_x+radious*cos(angle))*outPutWidth/ftotal_width;
                bird_y=(start_y+radious*sin(angle))*outPutHeight/ftotal_height;
                AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
                AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
                ORG_x=ORG_x/outPutWidth;
                ORG_y=ORG_y/outPutHeight;
                if (ORG_x < 0) {ORG_x = 0;}
                if (ORG_x > 1) {ORG_x = 1;}
                if (ORG_y < 0) {ORG_y = 0;}
                if (ORG_y > 1) {ORG_y = 1;}
                *(BL_ptr)=ORG_x;
                *(BL_ptr+1)=ORG_y;
                BL_ptr=BL_ptr+2;
            }
        }
        BL_ptr=BL_ptr+2;
        angle=PI;
        radious=radious-interval_x;
    }
    //*************1/4圓內圈*************************/

    //*************1/4圓最外圈 需計算與x軸  y軸的交點************************
    BR_ptr=right_start+2;     //
    temp=0;
    angle=PI*3/2;
    radious=extend;
    start_x=extend+car_height;
    start_y=extend;
    for(i=0;i<Vslices-1;i++)
    {
        angle=angle+interval_angle;
        finish_x=start_x+(radious*cos(angle));
        finish_y=start_y+(radious*sin(angle));
        if(temp==0)
        {
            pointintersect(start_x, start_y,finish_x,finish_y,
                              0.0f,    0.0f,    5.0f,    0.0f,
                           &bird_x, &bird_y);

            if(bird_x>ftotal_width)
            {temp=1;}
            else
            {
                bird_x=bird_x*outPutWidth/ftotal_width;
                bird_y=0;
                AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
                AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
                ORG_x=ORG_x/outPutWidth;
                ORG_y=ORG_y/outPutHeight;
                if (ORG_x < 0) {ORG_x = 0;}
                if (ORG_x > 1) {ORG_x = 1;}
                if (ORG_y < 0) {ORG_y = 0;}
                if (ORG_y > 1) {ORG_y = 1;}
                *(BR_ptr)=ORG_x;
                *(BR_ptr+1)=ORG_y;
                BR_ptr=BR_ptr+2;

                *(RUD_array)=UD_x;
                *(RUD_array+1)=UD_y;
                RUD_array=RUD_array+2;
            }
        }
        if(temp==1)
        {
            pointintersect(start_x, start_y,finish_x,finish_y,
                           ftotal_width, 0.0f, ftotal_width, 10.0f,
                           &bird_x, &bird_y);
            bird_x=outPutWidth;
            bird_y=bird_y*outPutHeight/ftotal_height;

            AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
            AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
            ORG_x=ORG_x/outPutWidth;
            ORG_y=ORG_y/outPutHeight;
            if (ORG_x < 0) {ORG_x = 0;}
            if (ORG_x > 1) {ORG_x = 1;}
            if (ORG_y < 0) {ORG_y = 0;}
            if (ORG_y > 1) {ORG_y = 1;}
            *(BR_ptr)=ORG_x;
            *(BR_ptr+1)=ORG_y;
            BR_ptr=BR_ptr+2;

            *(RUD_array)=UD_x;
            *(RUD_array+1)=UD_y;
            RUD_array=RUD_array+2;
        }

    }
    //*************1/4圓最外圈 需計算與x軸  y軸的交點************************
    bird_x=outPutWidth;                                      //將結束點填入材質矩陣(用三角函數計算怕會有誤差)
    bird_y=outPutHeight;
    AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
    AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
    ORG_x=ORG_x/outPutWidth;
    ORG_y=ORG_y/outPutHeight;
    if (ORG_x < 0) {ORG_x = 0;}
    if (ORG_x > 1) {ORG_x = 1;}
    if (ORG_y < 0) {ORG_y = 0;}
    if (ORG_y > 1) {ORG_y = 1;}
    *(BR_ptr)=ORG_x;
    *(BR_ptr+1)=ORG_y;
    BR_ptr=BR_ptr+4;

    *(RUD_array)=UD_x;
    *(RUD_array+1)=UD_y;
    //*************1/4圓內圈************************
    radious=radious-interval_x;
    angle=(PI*3/2);
    start_x=extend+car_height;
    start_y=extend;
    for(i=0;i<Cslices-1;i++)
    {
        for(j=0;j<Vslices;j++)
        {
            if(j==Vslices-1)
            {
                bird_x=(extend+car_height+radious)*outPutWidth/ftotal_width;
                bird_y=outPutHeight;
                AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
                AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
                ORG_x=ORG_x/outPutWidth;
                ORG_y=ORG_y/outPutHeight;
                if (ORG_x < 0) {ORG_x = 0;}
                if (ORG_x > 1) {ORG_x = 1;}
                if (ORG_y < 0) {ORG_y = 0;}
                if (ORG_y > 1) {ORG_y = 1;}
                *(BR_ptr)=ORG_x;
                *(BR_ptr+1)=ORG_y;
                BR_ptr=BR_ptr+2;
            }
            else
            {
                angle=angle+interval_angle;
                bird_x=(start_x+radious*cos(angle))*outPutWidth/ftotal_width;
                bird_y=(start_y+radious*sin(angle))*outPutHeight/ftotal_height;
                AutoRadBirds2UD(bird_x, bird_y, &UD_x, &UD_y, camID);
                AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
                ORG_x=ORG_x/outPutWidth;
                ORG_y=ORG_y/outPutHeight;
                if (ORG_x < 0) {ORG_x = 0;}
                if (ORG_x > 1) {ORG_x = 1;}
                if (ORG_y < 0) {ORG_y = 0;}
                if (ORG_y > 1) {ORG_y = 1;}
                *(BR_ptr)=ORG_x;
                *(BR_ptr+1)=ORG_y;
                BR_ptr=BR_ptr+2;
            }
        }
        BR_ptr=BR_ptr+2;
        angle=(PI*3/2);
        radious=radious-interval_x;
    }
    //*************1/4圓內圈*************************/


}
void Tex_top_right(float* BF_ptr, float* BR_ptr, float* BL_ptr, float* BT_ptr, float* BTL_ptr,float* BTR_ptr, int mesh_width, int mesh_height, int camID)
{
    int i,j;
    float *front_start, *right_start, *left_start;
    float start_x, start_y;
    float finish_x, finish_y;
    float interval_x, interval_y;
    float left_x, left_y, right_x, right_y;
    float UD_x, UD_y, ORG_x, ORG_y, bird_x, bird_y;
    float pivot_x,pivot_y, vanish_x, vanish_y;
    float H_x, H_y, top_x, top_y;
    float temp_x1, temp_y1, temp_x2, temp_y2;
    float slope1, slope2;
    float distance_y, distance_y2;
    float LTOP_UD[(Hslices)*2];
    float RTOP_UD[(Hslices)*2];
    float CTOP_UD[(Hslices)*2];
    float *LUD_array, *RUD_array, *Larray_start, *Rarray_start;


    front_start=BF_ptr;
    right_start=BR_ptr;
    left_start =BL_ptr;

    //***********紀錄底部1/4最外圈的UD值,計算周圍模型會用到*******
    if(camID==1)
    {
        LUD_array=Larray_start=&bottom_RUUD[0];
        RUD_array=Rarray_start=&bottom_RDUD[0];
    }
    else
    {
        LUD_array=Larray_start=&bottom_LDUD[0];
        RUD_array=Rarray_start=&bottom_LUUD[0];
    }
    //***********紀錄底部1/4最外圈的UD值,計算周圍模型會用到*******

    AutoRadHomoInit(camID);
    AutoRadGetPivot(&pivot_x, &pivot_y, camID);                //取得攝影機正下方UD點
    AutoRadGetVanishing(&vanish_x, &vanish_y, camID);          //取得攝影機消失UD點
    AutoRadBirds2UD(outPutWidth/2, 0, &UD_x, &UD_y, camID);      //取得底部中間外側UD點


    bird_x=(xyextend*outPutWidth)/(2*xyextend+carlength);
    AutoRadBirds2UD(bird_x, 0, &left_x, &left_y, camID);
    bird_x=((xyextend+carlength)*outPutWidth)/(2*xyextend+carlength);
    AutoRadBirds2UD(bird_x, 0, &right_x, &right_y, camID);


    *(LUD_array+(Vslices*2))=left_x;
    *(LUD_array+(Vslices*2)+1)=left_y;
    *(RUD_array)=right_x;
    *(RUD_array+1)=right_y;

    AutoRadGetfeature(&temp_x1,&temp_y1,camID,0);
    AutoRadGetfeature(&temp_x2,&temp_y2,camID,1);
    bird_y=(temp_y1+temp_y2)/2;
    slope1=(temp_y2-temp_y1)/(temp_x2-temp_x1);

    AutoRadGetfeature(&temp_x1,&temp_y1,camID,2);
    AutoRadGetfeature(&temp_x2,&temp_y2,camID,3);
    ORG_y=(temp_y1+temp_y2)/2;

    slope2=(temp_y2-temp_y1)/(temp_x2-temp_x1);

    slope2=((slope1-slope2)/fabs(bird_y-ORG_y));


    //interval_y2=(UD_y-vanish_y)/2;
    distance_y=(UD_y-vanish_y)/2;

    top_x=UD_x;    top_y=vanish_y-4*distance_y;
    slope1=slope1+(slope2*fabs(top_y-bird_y))/2;
    H_x=top_x+5;   H_y=top_y+(5*slope1);

    for(i=0;i<mesh_height-1;i++)
    {
        pointintersect(top_x, top_y, H_x, H_y,
                    pivot_x, pivot_y,  left_x, left_y,
                    &start_x, &start_y);

        pointintersect(top_x, top_y, H_x, H_y,
                    pivot_x, pivot_y,  right_x, right_y,
                    &finish_x, &finish_y);

        //**********計算側邊貼圖時所需要的UD資料********
        LTOP_UD[2*i]  =start_x;
        LTOP_UD[2*i+1]=start_y;
        CTOP_UD[2*i]  =top_x;
        CTOP_UD[2*i+1]=top_y;
        RTOP_UD[2*i]  =finish_x;
        RTOP_UD[2*i+1]=finish_y;
        //**********計算側邊貼圖時所需要的UD資料********


        interval_x=(finish_x-start_x)/(mesh_width-1);
        interval_y=(finish_y-start_y)/(mesh_width-1);
        UD_x=start_x;       UD_y=start_y;
        for(j=0;j<mesh_width;j++)
        {
            AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
            ORG_x=ORG_x/outPutWidth;
            ORG_y=ORG_y/outPutHeight;
            if (ORG_x < 0) {ORG_x = 0;}
			if (ORG_x > 1) {ORG_x = 1;}
			if (ORG_y < 0) {ORG_y = 0;}
			if (ORG_y > 1) {ORG_y = 1;}
            *(BF_ptr)=ORG_x;
            *(BF_ptr+1)=ORG_y;
            BF_ptr=BF_ptr+2;
            UD_x=UD_x+interval_x;
            UD_y=UD_y+interval_y;
        }
        top_y=top_y+distance_y;
        H_y=top_y+distance_y;
    }
    memcpy(BF_ptr,BT_ptr,mesh_width*2*sizeof(float));
    //*****************TOP_left/right材質座標計算*************************
    BF_ptr=front_start;
    BL_ptr=left_start;
    BR_ptr=right_start;
    //將front_back兩側的資料複製給front_left/front_right**************
    BL_ptr=BL_ptr+Vslices*2;
    for(i=0;i<mesh_height-1;i++)
    {
        *(BL_ptr)=*(BF_ptr);                    *(BL_ptr+1)=*(BF_ptr+1);
        *(BR_ptr)=*(BF_ptr+(mesh_width-1)*2);       *(BR_ptr+1)=*(BF_ptr+(mesh_width-1)*2+1);

        BL_ptr=BL_ptr+(Vslices+1)*2;
        BR_ptr=BR_ptr+(Vslices+1)*2;
        BF_ptr=BF_ptr+mesh_width*2;
    }
    //將front_back兩側的資料複製給front_left/front_right**************
    BF_ptr=front_start;
    BL_ptr=left_start;
    BR_ptr=right_start;
    temp_y1=vanish_y;
    //****************front_left材質座標計算***************************
    for(j=0;j<Hslices;j++)
    {
        distance_y2=(vanish_y-CTOP_UD[2*j+1])/Hslices;
        for(i=0;i<Vslices;i++)
        {
            pointintersect(CTOP_UD[2*j], CTOP_UD[2*j+1], LTOP_UD[2*j], LTOP_UD[2*j+1],
                    pivot_x, pivot_y, *(LUD_array), *(LUD_array+1),
                    &UD_x, &UD_y);

            if(UD_x < LTOP_UD[2*j])       //判斷交點是否錯誤
            {
                AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
                ORG_x=ORG_x/outPutWidth;
                ORG_y=ORG_y/outPutHeight;
                if (ORG_x < 0) {ORG_x = 0;}
                if (ORG_x > 1) {ORG_x = 1;}
                if (ORG_y < 0) {ORG_y = 0;}
                if (ORG_y > 1) {ORG_y = 1;}
                *(BL_ptr)=ORG_x;
                *(BL_ptr+1)=ORG_y;
                BL_ptr=BL_ptr+2;
            }
            else
            {
                *(BL_ptr)=0;
                *(BL_ptr+1)=0;
                BL_ptr=BL_ptr+2;
            }
            temp_y1=temp_y1-distance_y2;
            LUD_array=LUD_array+2;
        }
        BL_ptr=BL_ptr+2;
        temp_y1=vanish_y;
        LUD_array=Larray_start;
    }
    memcpy(BL_ptr,BTL_ptr,(Vslices+1)*2*sizeof(float));
    //****************front_left材質座標計算***************************
    BF_ptr=front_start;
    BL_ptr=left_start;
    BR_ptr=right_start+2;
    //temp_y1=CTOP_UD[1];
    RUD_array=RUD_array+2;
    //****************front_right材質座標計算***************************
    for(j=0;j<Hslices;j++)
    {
        distance_y2=(vanish_y-CTOP_UD[2*j+1])/Hslices;
        temp_y1=CTOP_UD[2*j+1]+distance_y2;
        for(i=0;i<Vslices;i++)
        {
            pointintersect(CTOP_UD[2*j], CTOP_UD[2*j+1], RTOP_UD[2*j], RTOP_UD[2*j+1],
                    pivot_x, pivot_y, *(RUD_array), *(RUD_array+1),
                    &UD_x, &UD_y);

            if(UD_x > RTOP_UD[2*j])       //判斷交點是否錯誤
            {
                AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
                ORG_x=ORG_x/outPutWidth;
                ORG_y=ORG_y/outPutHeight;
                if (ORG_x < 0) {ORG_x = 0;}
                if (ORG_x > 1) {ORG_x = 1;}
                if (ORG_y < 0) {ORG_y = 0;}
                if (ORG_y > 1) {ORG_y = 1;}
                *(BR_ptr)=ORG_x;
                *(BR_ptr+1)=ORG_y;
                BR_ptr=BR_ptr+2;
            }
            else
            {
                *(BR_ptr)=0;
                *(BR_ptr+1)=0;
                BR_ptr=BR_ptr+2;
            }
            temp_y1=temp_y1+distance_y2;
            RUD_array=RUD_array+2;
        }
        BR_ptr=BR_ptr+2;
        //temp_y1=CTOP_UD[1];
        RUD_array=Rarray_start+2;
    }
    BR_ptr=BR_ptr-2;
    memcpy(BR_ptr,BTR_ptr,(Vslices+1)*2*sizeof(float));
    //****************front_right材質座標計算***************************


}

void Tex_top_front(float* BF_ptr, float* BR_ptr, float* BL_ptr, float* BT_ptr, float* BTL_ptr,float* BTR_ptr, int mesh_width, int mesh_height, int camID)
{
    int i,j;
    float *front_start, *right_start, *left_start;
    float start_x, start_y;
    float finish_x, finish_y;
    float interval_x, interval_y;
    float left_x, left_y, right_x, right_y;
    float UD_x, UD_y, ORG_x, ORG_y, bird_x, bird_y;
    float pivot_x,pivot_y, vanish_x, vanish_y;
    float H_x, H_y, top_x, top_y;
    float slope1, slope2;
    float temp_x1, temp_y1, temp_x2, temp_y2;
    float distance_y, distance_y2;
    float height_ratio;
    float LTOP_UD[(Hslices)*2];
    float RTOP_UD[(Hslices)*2];
    float CTOP_UD[(Hslices)*2];
    float *LUD_array, *RUD_array, *Larray_start, *Rarray_start;



    front_start=BF_ptr;
    right_start=BR_ptr;
    left_start =BL_ptr;


    //***********紀錄底部1/4最外圈的UD值,計算周圍模型會用到*******
    if(camID==0)
    {
        LUD_array=Larray_start=&bottom_FLUD[0];
        RUD_array=Rarray_start=&bottom_FRUD[0];
        height_ratio=LRcam_height/Fcam_height;
    }
    else
    {
        LUD_array=Larray_start=&bottom_BLUD[0];
        RUD_array=Rarray_start=&bottom_BRUD[0];
        height_ratio=LRcam_height/Bcam_height;
    }
    //***********紀錄底部1/4最外圈的UD值,計算周圍模型會用到*******

    AutoRadHomoInit(camID);
    AutoRadGetPivot(&pivot_x, &pivot_y, camID);                //取得攝影機正下方UD點
    AutoRadGetVanishing(&vanish_x, &vanish_y, camID);          //取得攝影機消失UD點
    AutoRadBirds2UD(outPutWidth/2, 0, &UD_x, &UD_y, camID);      //取得底部中間外側UD點

    bird_x=(xyextend*outPutWidth)/(2*xyextend+carwidth);
    AutoRadBirds2UD(bird_x, 0, &left_x, &left_y, camID);            //鳥瞰左側端點
    bird_x=((xyextend+carwidth)*outPutWidth)/(2*xyextend+carwidth);
    AutoRadBirds2UD(bird_x, 0, &right_x, &right_y, camID);//鳥瞰右側端點

    *(LUD_array+(Vslices*2))=left_x;
    *(LUD_array+(Vslices*2)+1)=left_y;
    *(RUD_array)=right_x;
    *(RUD_array+1)=right_y;


    AutoRadGetfeature(&temp_x1,&temp_y1,camID,0);
    AutoRadGetfeature(&temp_x2,&temp_y2,camID,1);
    bird_y=(temp_y1+temp_y2)/2;
    slope1=(temp_y2-temp_y1)/(temp_x2-temp_x1);

    AutoRadGetfeature(&temp_x1,&temp_y1,camID,2);
    AutoRadGetfeature(&temp_x2,&temp_y2,camID,3);
    ORG_y=(temp_y1+temp_y2)/2;

    slope2=(temp_y2-temp_y1)/(temp_x2-temp_x1);

    slope2=((slope1-slope2)/fabs(bird_y-ORG_y));



    distance_y=((UD_y-vanish_y)/2)*height_ratio;



    top_x=UD_x;    top_y=vanish_y-4*distance_y;
    slope1=slope1+(slope2*fabs(top_y-bird_y))/2;
    H_x=top_x+5;   H_y=top_y+(5*slope1);

    distance_y2=(vanish_y-top_y)/Hslices;
    //*****************front_back材質座標計算*************************
    for(i=0;i<mesh_height-1;i++)
    {
        pointintersect(top_x, top_y, H_x, H_y,
                    pivot_x, pivot_y,  left_x, left_y,
                    &start_x, &start_y);

        pointintersect(top_x, top_y, H_x, H_y,
                    pivot_x, pivot_y,  right_x, right_y,
                    &finish_x, &finish_y);

        //**********計算側邊貼圖時所需要的UD資料********
        LTOP_UD[2*i]  =start_x;
        LTOP_UD[2*i+1]=start_y;
        CTOP_UD[2*i]  =top_x;
        CTOP_UD[2*i+1]=top_y;
        RTOP_UD[2*i]  =finish_x;
        RTOP_UD[2*i+1]=finish_y;
        //**********計算側邊貼圖時所需要的UD資料********

        interval_x=(finish_x-start_x)/(mesh_width-1);
        interval_y=(finish_y-start_y)/(mesh_width-1);
        UD_x=start_x;       UD_y=start_y;
        for(j=0;j<mesh_width;j++)
        {
            AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
            ORG_x=ORG_x/outPutWidth;
            ORG_y=ORG_y/outPutHeight;
            if (ORG_x < 0) {ORG_x = 0;}
			if (ORG_x > 1) {ORG_x = 1;}
			if (ORG_y < 0) {ORG_y = 0;}
			if (ORG_y > 1) {ORG_y = 1;}
            *(BF_ptr)=ORG_x;
            *(BF_ptr+1)=ORG_y;
            BF_ptr=BF_ptr+2;
            UD_x=UD_x+interval_x;
            UD_y=UD_y+interval_y;
        }
        top_y=top_y+distance_y;
        H_y=H_y+distance_y;
    }
    memcpy(BF_ptr,BT_ptr,mesh_width*2*sizeof(float));
    //*****************front_back材質座標計算*************************
    BF_ptr=front_start;
    BL_ptr=left_start;
    BR_ptr=right_start;
    //將front_back兩側的資料複製給front_left/front_right**************
    BL_ptr=BL_ptr+Vslices*2;
    for(i=0;i<mesh_height-1;i++)
    {
        *(BL_ptr)=*(BF_ptr);                    *(BL_ptr+1)=*(BF_ptr+1);
        *(BR_ptr)=*(BF_ptr+(mesh_width-1)*2);       *(BR_ptr+1)=*(BF_ptr+(mesh_width-1)*2+1);

        BL_ptr=BL_ptr+(Vslices+1)*2;
        BR_ptr=BR_ptr+(Vslices+1)*2;
        BF_ptr=BF_ptr+mesh_width*2;
    }
    //將front_back兩側的資料複製給front_left/front_right**************
    BF_ptr=front_start;
    BL_ptr=left_start;
    BR_ptr=right_start;
    temp_y1=vanish_y;
    //****************front_left材質座標計算***************************

    for(j=0;j<Hslices;j++)
    {
        distance_y2=(vanish_y-CTOP_UD[2*j+1])/Hslices;
        for(i=0;i<Vslices;i++)
        {
            pointintersect(CTOP_UD[2*j], CTOP_UD[2*j+1], LTOP_UD[2*j], LTOP_UD[2*j+1],
                    pivot_x, pivot_y, *(LUD_array), *(LUD_array+1),
                    &UD_x, &UD_y);

            if(UD_x < LTOP_UD[2*j])       //判斷交點是否錯誤
            {
                AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
                ORG_x=ORG_x/outPutWidth;
                ORG_y=ORG_y/outPutHeight;
                if (ORG_x < 0) {ORG_x = 0;}
                if (ORG_x > 1) {ORG_x = 1;}
                if (ORG_y < 0) {ORG_y = 0;}
                if (ORG_y > 1) {ORG_y = 1;}
                *(BL_ptr)=ORG_x;
                *(BL_ptr+1)=ORG_y;
                BL_ptr=BL_ptr+2;
            }
            else
            {
                *(BL_ptr)=0;
                *(BL_ptr+1)=0;
                BL_ptr=BL_ptr+2;
            }
            temp_y1=temp_y1-distance_y2;
            LUD_array=LUD_array+2;
        }
        BL_ptr=BL_ptr+2;
        temp_y1=vanish_y;
        LUD_array=Larray_start;
    }
    memcpy(BL_ptr,BTL_ptr,(Vslices+1)*2*sizeof(float));
    //****************front_left材質座標計算***************************
    BF_ptr=front_start;
    BL_ptr=left_start;
    BR_ptr=right_start+2;

    RUD_array=RUD_array+2;
    //****************front_right材質座標計算***************************

    for(j=0;j<Hslices;j++)
    {
        distance_y2=(vanish_y-CTOP_UD[2*j+1])/Hslices;
        temp_y1=CTOP_UD[2*j+1]+distance_y2;
        for(i=0;i<Vslices;i++)
        {
            pointintersect(CTOP_UD[2*j], CTOP_UD[2*j+1], RTOP_UD[2*j], RTOP_UD[2*j+1],
                    pivot_x, pivot_y, *(RUD_array), *(RUD_array+1),
                    &UD_x, &UD_y);

            if(UD_x > RTOP_UD[2*j])       //判斷交點是否錯誤
            {
                AutoRadUDtoORG(UD_x, UD_y, &ORG_x, &ORG_y, camID);
                ORG_x=ORG_x/outPutWidth;
                ORG_y=ORG_y/outPutHeight;
                if (ORG_x < 0) {ORG_x = 0;}
                if (ORG_x > 1) {ORG_x = 1;}
                if (ORG_y < 0) {ORG_y = 0;}
                if (ORG_y > 1) {ORG_y = 1;}
                *(BR_ptr)=ORG_x;
                *(BR_ptr+1)=ORG_y;
                BR_ptr=BR_ptr+2;
            }
            else
            {
                *(BR_ptr)=0;
                *(BR_ptr+1)=0;
                BR_ptr=BR_ptr+2;
            }
            temp_y1=temp_y1+distance_y2;
            RUD_array=RUD_array+2;
        }
        BR_ptr=BR_ptr+2;
        //temp_y1=CTOP_UD[1];
        RUD_array=Rarray_start+2;
    }
    BR_ptr=BR_ptr-2;
    memcpy(BR_ptr,BTR_ptr,(Vslices+1)*2*sizeof(float));
    //****************front_right材質座標計算***************************

}

void texture_vertex_create(AVM_ptr *verticesB, AVM_ptr *verticesT, float car_width, float car_length, int mesh_offset, int extend_opt)
{
    int i,j,k;
    int temp;
    int mesh_width,mesh_length;
    int extend;
    int FBsize,RLsize,Csize;
    float center_x, center_y, center_z, radious, radious2;
    //float temp_z[10];

    float F_width,F_length, interval,F_temp;
    float *BF_ptr,*BL_ptr,*BR_ptr,*BB_ptr;
    float *BTL_ptr, *BTR_ptr;
    float angle;

    switch(extend_opt)              //決定底部可視範圍為2m或是3m
    {
        case 1:
            extend=200;
            break;
        case 2:
            extend=250;
            break;
        case 3:
            extend=300;
            break;
        case 4:
            extend=350;
            break;
        case 5:
            extend=400;
            break;
    }

    FBsize=(FBVslices+1)*(Cslices+1)*2;
    RLsize=(LRVslices+1)*(Cslices+1)*2;
    Csize=(Vslices+1)*Cslices*2+2;

    verticesB->_front=malloc(FBsize*sizeof(float));
    verticesB->_back=malloc(FBsize*sizeof(float));
    verticesB->left=malloc(RLsize*sizeof(float));
    verticesB->right=malloc(RLsize*sizeof(float));
    verticesB->front_left=malloc(Csize*sizeof(float));
    verticesB->front_right=malloc(Csize*sizeof(float));
    verticesB->back_left=malloc(Csize*sizeof(float));
    verticesB->back_right=malloc(Csize*sizeof(float));
    verticesB->left_up=malloc(Csize*sizeof(float));
    verticesB->left_down=malloc(Csize*sizeof(float));
    verticesB->right_up=malloc(Csize*sizeof(float));
    verticesB->right_down=malloc(Csize*sizeof(float));

    Csize=(Vslices+1)*(Hslices+1)*2;
    FBsize=(FBVslices+1)*(Hslices+1)*2;
    RLsize=(LRVslices+1)*(Hslices+1)*2;


    verticesT->_front=malloc(FBsize*sizeof(float));
    verticesT->_back=malloc(FBsize*sizeof(float));
    verticesT->left=malloc(RLsize*sizeof(float));
    verticesT->right=malloc(RLsize*sizeof(float));
    verticesT->front_left=malloc(Csize*sizeof(float));
    verticesT->front_right=malloc(Csize*sizeof(float));
    verticesT->back_left=malloc(Csize*sizeof(float));
    verticesT->back_right=malloc(Csize*sizeof(float));
    verticesT->left_up=malloc(Csize*sizeof(float));
    verticesT->left_down=malloc(Csize*sizeof(float));
    verticesT->right_up=malloc(Csize*sizeof(float));
    verticesT->right_down=malloc(Csize*sizeof(float));

    BirdsEyeViewInit();
    BF_ptr=verticesB->_front;
    BR_ptr=verticesB->front_right;
    BL_ptr=verticesB->front_left;
    Tex_bottom_front(BF_ptr, BR_ptr, BL_ptr, car_width, extend, FBVslices+1, Cslices+1,0);

    BF_ptr=verticesB->right;
    BR_ptr=verticesB->right_down;
    BL_ptr=verticesB->right_up;
    Tex_bottom_left(BF_ptr, BR_ptr, BL_ptr, car_length, extend, LRVslices+1, Cslices+1,1);


    BF_ptr=verticesB->_back;
    BR_ptr=verticesB->back_right;
    BL_ptr=verticesB->back_left;
    Tex_bottom_front(BF_ptr, BL_ptr, BR_ptr, car_width, extend, FBVslices+1, Cslices+1,2);



    BF_ptr=verticesB->left;
    BR_ptr=verticesB->left_up;
    BL_ptr=verticesB->left_down;
    Tex_bottom_left(BF_ptr, BR_ptr, BL_ptr, car_length, extend, LRVslices+1, Cslices+1,3);
    //*************************************************************************************************************
    //************************周圍貼圖座標計算***********************************************************
    BF_ptr=verticesT->right;
    BR_ptr=verticesT->right_down;
    BL_ptr=verticesT->right_up;

    BB_ptr=verticesB->right;
    BTL_ptr=verticesB->right_up;
    BTR_ptr=verticesB->right_down;
    Tex_top_right(BF_ptr, BR_ptr, BL_ptr, BB_ptr, BTL_ptr, BTR_ptr, LRVslices+1, Hslices+1, 1);

    BF_ptr=verticesT->left;
    BR_ptr=verticesT->left_up;
    BL_ptr=verticesT->left_down;
    BB_ptr=verticesB->left;
    BTL_ptr=verticesB->left_down;
    BTR_ptr=verticesB->left_up;
    Tex_top_right(BF_ptr, BR_ptr, BL_ptr, BB_ptr, BTL_ptr, BTR_ptr, LRVslices+1, Hslices+1, 3);

    BF_ptr=verticesT->_front;
    BR_ptr=verticesT->front_right;
    BL_ptr=verticesT->front_left;

    BB_ptr=verticesB->_front;
    BTL_ptr=verticesB->front_left;
    BTR_ptr=verticesB->front_right;

    Tex_top_front(BF_ptr, BR_ptr, BL_ptr, BB_ptr, BTL_ptr, BTR_ptr, FBVslices+1, Hslices+1, 0);


    BF_ptr=verticesT->_back;
    BR_ptr=verticesT->back_right;
    BL_ptr=verticesT->back_left;

    BB_ptr=verticesB->_back;
    BTL_ptr=verticesB->back_left;
    BTR_ptr=verticesB->back_right;

    Tex_top_front(BF_ptr, BL_ptr, BR_ptr, BB_ptr, BTR_ptr, BTL_ptr, FBVslices+1, Hslices+1, 2);
    //************************周圍貼圖座標計算***********************************************************

}
