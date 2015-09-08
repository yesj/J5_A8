#include "model.h"

void model_create(AVM_ptr *verticesB, AVM_ptr *verticesT, float car_width, float car_length, int offset, int extend_opt)
{
    int i,j,k;
    int mesh_width,mesh_length;
    float extend=0;
    int FBsize,RLsize,Csize;
    float center_x, center_y, center_z, radious, radious2;
    //float temp_z[10];

    float F_length, interval_x, interval_y, F_temp;
    float *BF_ptr,*BL_ptr,*BR_ptr,*BB_ptr;
    float angle;

    /*float temp_vertices0[9*30*3];
    float temp_vertices1[9*30*3];
    float temp_vertices2[9*30*3];
    float temp_vertices3[9*30*3];

    BF_ptr=vertices->_front;
    BL_ptr=vertices->left;
    BR_ptr=vertices->right;
    BB_ptr=vertices->_back;*/

    //*********************建立底部模型頂點資料*******************************************

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
    /*FBsize=((car_width/10)+1)*((extend/10)+1)*3;           //修改為固定格數的模型
    RLsize=((car_length/10)+1)*((extend/10)+1)*3;
    Csize=(Vslices+1)*(extend/10)*3+3;*/

    FBsize=((FBVslices+1)*(Cslices+1))*3;
    RLsize=((LRVslices+1)*(Cslices+1))*3;
    Csize=(Vslices+1)*(Cslices)*3+3;

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

    /*Csize=(Vslices+1)*(Hslices+1)*3;
    FBsize=((car_width/10)+1)*(Hslices+1)*3;
    RLsize=((car_length/10)+1)*(Hslices+1)*3;*/

    Csize=(Vslices+1)*(Hslices+1)*3;
    FBsize=(FBVslices+1)*(Hslices+1)*3;
    RLsize=(LRVslices+1)*(Hslices+1)*3;


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


    BF_ptr=verticesB->_front;
    BL_ptr=verticesB->left;
    BR_ptr=verticesB->right;
    BB_ptr=verticesB->_back;


    //temp=1<<(offset-1);
    //interval=1/temp;
    //mesh_length=((extend/10)+1)*temp;
    interval_x=(car_width/10)/FBVslices;
    interval_y=(extend/10)/Cslices;
    mesh_width=FBVslices+1;
    mesh_length=Cslices+1;
    //vvvvvvvvvvvvvvvvv前方底部vvvvvvvvvvvvvvvvvvvvvv
    center_x=-(car_width/2)/10;
    center_y=((car_length/2)+extend)/10;
    BF_ptr=verticesB->_front;
    for(j=0;j<mesh_length;j++)
    {
        center_z=center_x;
        for(i=0;i<mesh_width;i++)
        {
            *(BF_ptr)=center_z;
            *(BF_ptr+1)=center_y;
            *(BF_ptr+2)=0;

            center_z=center_z+interval_x;
            BF_ptr=BF_ptr+3;
        }
        center_y=center_y-interval_y;
    }

    //vvvvvvvvvvvvvvvvv右方底部vvvvvvvvvvvvvvvvvvvvvv
    //mesh_width=((car_length/10)+1)*temp;
    interval_x=(extend/10)/Cslices;
    interval_y=(car_length/10)/LRVslices;
    mesh_width=LRVslices+1;
    center_x=((car_width/2)+extend)/10;
    center_y=(car_length/2)/10;
    BF_ptr=verticesB->right;
    for(j=0;j<mesh_length;j++)
    {
        center_z=center_y;
        for(i=0;i<mesh_width;i++)
        {
            *(BF_ptr)=center_x;
            *(BF_ptr+1)=center_z;
            *(BF_ptr+2)=0;

            center_z=center_z-interval_y;
            BF_ptr=BF_ptr+3;
        }
        center_x=center_x-interval_x;
    }

    //vvvvvvvvvvvvvvvvv後方底部vvvvvvvvvvvvvvvvvvvvvv
    //mesh_width=((car_width/10)+1)*temp;
    interval_x=(car_width/10)/FBVslices;
    interval_y=(extend/10)/Cslices;
    mesh_width=FBVslices+1;

    center_x=(car_width/2)/10;
    center_y=-((car_length/2)+extend)/10;
    BF_ptr=verticesB->_back;
    for(j=0;j<mesh_length;j++)
    {
        center_z=center_x;
        for(i=0;i<mesh_width;i++)
        {
            *(BF_ptr)=center_z;
            *(BF_ptr+1)=center_y;
            *(BF_ptr+2)=0;

            center_z=center_z-interval_x;
            BF_ptr=BF_ptr+3;
        }
        center_y=center_y+interval_y;
    }

    //vvvvvvvvvvvvvvvvv左方底部vvvvvvvvvvvvvvvvvvvvvv
    //mesh_width=((car_length/10)+1)*temp;
    interval_x=(extend/10)/Cslices;
    interval_y=(car_length/10)/LRVslices;
    mesh_width=LRVslices+1;
    center_x=-((car_width/2)+extend)/10;
    center_y=-(car_length/2)/10;
    BF_ptr=verticesB->left;
    for(j=0;j<mesh_length;j++)
    {
        center_z=center_y;
        for(i=0;i<mesh_width;i++)
        {
            *(BF_ptr)=center_x;
            *(BF_ptr+1)=center_z;
            *(BF_ptr+2)=0;

            center_z=center_z+interval_y;
            BF_ptr=BF_ptr+3;
        }
        center_x=center_x+interval_x;
    }


    BF_ptr=verticesB->front_left;
    BB_ptr=verticesB->front_right;

    BL_ptr=verticesB->back_left;
    BR_ptr=verticesB->back_right;


    //vvvvvvvvvvvvvvvvvvvvvvvvvvvv底部1/4圓頂點建構vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    interval_y=(extend/10)/Cslices;
    for(k=0;k<4;k++)
    {
        switch(k)                              //初始化各1/4元的圓心/半徑/起始角度/指標等
        {
            case 0:
                center_x=(car_width/2)/10;
                center_y=(car_length/2)/10;
                radious=extend/10;
                angle=(PI/2)-(PI/2/Vslices);
                BF_ptr=verticesB->front_right+3;
            break;

            case 1:
                center_x=-(car_width/2)/10;
                center_y=(car_length/2)/10;
                radious=extend/10;
                angle=(PI)-(PI/2/Vslices);
                BF_ptr=verticesB->front_left+3;
            break;

            case 2:
                center_x=-(car_width/2)/10;
                center_y=-(car_length/2)/10;
                radious=extend/10;
                angle=(PI*3/2)-(PI/2/Vslices);
                BF_ptr=verticesB->back_left+3;
            break;


            case 3:
                center_x=(car_width/2)/10;
                center_y=-(car_length/2)/10;
                radious=extend/10;
                angle=0-(PI/2/Vslices);
                BF_ptr=verticesB->back_right+3;
            break;
        }
        for(i=0;i<Cslices;i++)                       //改變半徑
        {
            for(j=0;j<Vslices-1;j++)                   //切分為幾等份
            {

                *(BF_ptr)=center_x + radious*cos(angle);
                *(BF_ptr+1)=center_y + radious*sin(angle);
                *(BF_ptr+2)=0;

                BF_ptr=BF_ptr+3;
                angle=angle-(PI/2/Vslices);
             }
             BF_ptr=BF_ptr+6;
             radious=radious-interval_y;
             switch(k)
            {
                case 0:
                    angle=(PI/2)-(PI/2/Vslices);
                break;
                case 1:
                    angle=PI-(PI/2/Vslices);
                break;
                case 2:
                    angle=(PI*3/2)-(PI/2/Vslices);
                break;
                case 3:
                    angle=0-(PI/2/Vslices);
                break;
            }
        }
        BF_ptr=BF_ptr-3;
        *(BF_ptr)=center_x;
        *(BF_ptr+1)=center_y;
        *(BF_ptr+2)=0;
    }

    mesh_width=(FBVslices+1)*3;
    mesh_length=(LRVslices+1)*3;
    j=(Vslices+1)*3;

    BF_ptr=verticesB->_front;
    BR_ptr=verticesB->right;
    BB_ptr=verticesB->front_right;

    for(k=0;k<Cslices;k++)
    {
        *(BB_ptr)=*(BF_ptr+mesh_width-3);
        *(BB_ptr+1)=*(BF_ptr+mesh_width-2);
        *(BB_ptr+2)=*(BF_ptr+mesh_width-1);

        *(BB_ptr+j-3)=*(BR_ptr);
        *(BB_ptr+j-2)=*(BR_ptr+1);
        *(BB_ptr+j-1)=*(BR_ptr+2);

        BF_ptr=BF_ptr+mesh_width;
        BR_ptr=BR_ptr+mesh_length;
        BB_ptr=BB_ptr+j;
    }

    BF_ptr=verticesB->_back;
    BR_ptr=verticesB->left;
    BB_ptr=verticesB->back_left;

    for(k=0;k<Cslices;k++)
    {
        *(BB_ptr)=*(BF_ptr+mesh_width-3);
        *(BB_ptr+1)=*(BF_ptr+mesh_width-2);
        *(BB_ptr+2)=*(BF_ptr+mesh_width-1);

        *(BB_ptr+j-3)=*(BR_ptr);
        *(BB_ptr+j-2)=*(BR_ptr+1);
        *(BB_ptr+j-1)=*(BR_ptr+2);

        BF_ptr=BF_ptr+mesh_width;
        BR_ptr=BR_ptr+mesh_length;
        BB_ptr=BB_ptr+j;
    }

    BF_ptr=verticesB->left;
    BR_ptr=verticesB->_front;
    BB_ptr=verticesB->front_left;

    for(k=0;k<Cslices;k++)
    {
        *(BB_ptr)=*(BF_ptr+mesh_length-3);
        *(BB_ptr+1)=*(BF_ptr+mesh_length-2);
        *(BB_ptr+2)=*(BF_ptr+mesh_length-1);

        *(BB_ptr+j-3)=*(BR_ptr);
        *(BB_ptr+j-2)=*(BR_ptr+1);
        *(BB_ptr+j-1)=*(BR_ptr+2);

        BF_ptr=BF_ptr+mesh_length;
        BR_ptr=BR_ptr+mesh_width;
        BB_ptr=BB_ptr+j;
    }

    BF_ptr=verticesB->right;
    BR_ptr=verticesB->_back;
    BB_ptr=verticesB->back_right;

    for(k=0;k<Cslices;k++)
    {
        *(BB_ptr)=*(BF_ptr+mesh_length-3);
        *(BB_ptr+1)=*(BF_ptr+mesh_length-2);
        *(BB_ptr+2)=*(BF_ptr+mesh_length-1);

        *(BB_ptr+j-3)=*(BR_ptr);
        *(BB_ptr+j-2)=*(BR_ptr+1);
        *(BB_ptr+j-1)=*(BR_ptr+2);

        BF_ptr=BF_ptr+mesh_length;
        BR_ptr=BR_ptr+mesh_width;
        BB_ptr=BB_ptr+j;
    }

    Csize=(Vslices+1)*(Cslices)*3+3;

    memcpy(verticesB->right_up, verticesB->front_right, Csize*sizeof(float));

    memcpy(verticesB->left_up,verticesB->front_left, Csize*sizeof(float));

    memcpy(verticesB->right_down,verticesB->back_right, Csize*sizeof(float));

    memcpy(verticesB->left_down,verticesB->back_left, Csize*sizeof(float));



    //^^^^^^^^^^^^^^^^^^^^^^^^^^^底部1/4圓頂點建構^^^^^^^^^^^^^^^^^^^^^^^^^

    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^建立底部模型頂點資料^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvv建立周圍模型頂點資料vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv


    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvv周圍1/4圓頂點建構vvvvvvvvvvvvvvvvvvvvvvvvv
    Csize=(Vslices+1)*(Hslices+1)*3;
    radious=(extend/10)*2;
    F_temp=radious/2;                               //測試修正 起始為俯角0度  終止為俯角30度(起始角度為俯角30度  終止角度為俯角60度)
    //F_length=(radious/2)*sqrt(3)-F_temp;
    //radious2=radious*cos(PI/6);                    //第一圈的半徑

    F_length=radious/2;
    radious2=radious;

    F_temp=F_length/Hslices;


    for(k=0;k<4;k++)
    {
        switch(k)                              //初始化各1/4元的圓心/半徑/起始角度/指標等
        {
            case 0:
                center_x=(car_width/2)/10;
                center_y=(car_length/2)/10;
                center_z=F_length;
                angle=PI/2;
                //radious2=radious*cos(PI/6);
                //radious2=radious;
                radious2=extend/10;
                BF_ptr=verticesT->front_right;
            break;

            case 1:
                center_x=-(car_width/2)/10;
                center_y=(car_length/2)/10;
                center_z=F_length;
                angle=PI;
                //radious2=radious*cos(PI/6);
                //radious2=radious;
                radious2=extend/10;
                BF_ptr=verticesT->front_left;
            break;

            case 2:
                center_x=-(car_width/2)/10;
                center_y=-(car_length/2)/10;
                center_z=F_length;
                angle=PI*3/2;
                //radious2=radious*cos(PI/6);
                //radious2=radious;
                radious2=extend/10;
                BF_ptr=verticesT->back_left;
            break;


            case 3:
                center_x=(car_width/2)/10;
                center_y=-(car_length/2)/10;
                center_z=F_length;
                angle=0;
                //radious2=radious*cos(PI/6);
                //radious2=radious;
                radious2=extend/10;
                BF_ptr=verticesT->back_right;
            break;
        }
        for(j=0;j<Hslices;j++)
        {
            for(i=0;i<(Vslices+1);i++)
            {
                if(i==0)
                {
                    switch(k)                              //初始化各1/4元的圓心/半徑/起始角度/指標等
                    {
                        case 0:
                            *(BF_ptr)  =center_x;
                            *(BF_ptr+1)=center_y + radious2;
                            *(BF_ptr+2)=center_z;
                        break;

                        case 1:
                            *(BF_ptr)  =center_x - radious2;
                            *(BF_ptr+1)=center_y;
                            *(BF_ptr+2)=center_z;
                        break;

                        case 2:
                            *(BF_ptr)  =center_x;
                            *(BF_ptr+1)=center_y - radious2;
                            *(BF_ptr+2)=center_z;
                        break;

                        case 3:
                            *(BF_ptr)  =center_x + radious2;
                            *(BF_ptr+1)=center_y;
                            *(BF_ptr+2)=center_z;
                        break;
                    }
                }
                else if(i==Vslices)
                {
                    switch(k)                              //初始化各1/4元的圓心/半徑/起始角度/指標等
                    {
                        case 0:
                            *(BF_ptr)  =center_x + radious2;
                            *(BF_ptr+1)=center_y;
                            *(BF_ptr+2)=center_z;
                        break;

                        case 1:
                            *(BF_ptr)  =center_x;
                            *(BF_ptr+1)=center_y + radious2;
                            *(BF_ptr+2)=center_z;
                        break;

                        case 2:
                            *(BF_ptr)  =center_x - radious2;
                            *(BF_ptr+1)=center_y;
                            *(BF_ptr+2)=center_z;
                        break;

                        case 3:
                            *(BF_ptr)  =center_x;
                            *(BF_ptr+1)=center_y - radious2;
                            *(BF_ptr+2)=center_z;
                        break;
                    }
                }
                else
                {
                    *(BF_ptr)=center_x + radious2*cos(angle);
                    *(BF_ptr+1)=center_y + radious2*sin(angle);
                    *(BF_ptr+2)=center_z;
                }

                BF_ptr=BF_ptr+3;
                angle=angle-(PI/2/Vslices);
            }
            //F_width=((radious/2)*sqrt(3)-F_length)+(j+1)*F_temp;
            //F_width=(j+1)*F_temp;
            //radious2=sqrt((radious*radious)-(F_width*F_width));
            center_z=center_z-F_temp;
            switch(k)
            {
                case 0:
                    angle=PI/2;
                break;
                case 1:
                    angle=PI;
                break;
                case 2:
                    angle=PI*3/2;
                break;
                case 3:
                    angle=0;
                break;
            }
        }
    }
    memcpy(verticesT->front_right+(Vslices+1)*3*Hslices, verticesB->front_right, (Vslices+1)*3*sizeof(float));  //cpy底部1/4的最外圈 作為周維模型的內圈
    memcpy(verticesT->right_up,verticesT->front_right,Csize*sizeof(float));                   //cpy重疊區的結構

    memcpy(verticesT->front_left+(Vslices+1)*3*Hslices, verticesB->front_left, (Vslices+1)*3*sizeof(float));
    memcpy(verticesT->left_up,verticesT->front_left,Csize*sizeof(float));

    memcpy(verticesT->back_right+(Vslices+1)*3*Hslices, verticesB->back_right, (Vslices+1)*3*sizeof(float));
    memcpy(verticesT->right_down,verticesT->back_right,Csize*sizeof(float));

    memcpy(verticesT->back_left+(Vslices+1)*3*Hslices, verticesB->back_left, (Vslices+1)*3*sizeof(float));
    memcpy(verticesT->left_down,verticesT->back_left,Csize*sizeof(float));


    //^^^^^^^^^^^^^^^^^^^^^^^^^^^周圍1/4圓頂點建構^^^^^^^^^^^^^^^^^^^^^^^


    //vvvvvvvvvvvvvvvvvvvvvvvvvvv周圍頂點建構vvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    BB_ptr=verticesT->front_right;
    BF_ptr=verticesT->_front;
    BR_ptr=verticesT->right;

    mesh_width=(FBVslices+1)*3;
    mesh_length=(LRVslices+1)*3;
    j=(Vslices+1)*3;

    for(i=0;i<Hslices+1;i++)                  //front_right 兩側複製給top_front 與 top_right
    {
        *(BF_ptr+mesh_width-3)=*(BB_ptr);
        *(BF_ptr+mesh_width-2)=*(BB_ptr+1);
        *(BF_ptr+mesh_width-1)=*(BB_ptr+2);

        *(BR_ptr)=*(BB_ptr+j-3);
        *(BR_ptr+1)=*(BB_ptr+j-2);
        *(BR_ptr+2)=*(BB_ptr+j-1);

        BF_ptr=BF_ptr+mesh_width;
        BR_ptr=BR_ptr+mesh_length;
        BB_ptr=BB_ptr+j;
    }


    BB_ptr=verticesT->back_right;
    BF_ptr=verticesT->_back;
    BR_ptr=verticesT->right;

    for(i=0;i<Hslices+1;i++)            //back_right 兩側複製給top_back 與 top_right
    {
        *(BR_ptr+mesh_length-3)=*(BB_ptr);
        *(BR_ptr+mesh_length-2)=*(BB_ptr+1);
        *(BR_ptr+mesh_length-1)=*(BB_ptr+2);

        *(BF_ptr)=*(BB_ptr+j-3);
        *(BF_ptr+1)=*(BB_ptr+j-2);
        *(BF_ptr+2)=*(BB_ptr+j-1);

        BF_ptr=BF_ptr+mesh_width;
        BR_ptr=BR_ptr+mesh_length;
        BB_ptr=BB_ptr+j;
    }


    BB_ptr=verticesT->back_left;
    BF_ptr=verticesT->_back;
    BR_ptr=verticesT->left;

    for(i=0;i<Hslices+1;i++)          //back_left 兩側複製給top_back 與 top_left
    {
        *(BF_ptr+mesh_width-3)=*(BB_ptr);
        *(BF_ptr+mesh_width-2)=*(BB_ptr+1);
        *(BF_ptr+mesh_width-1)=*(BB_ptr+2);

        *(BR_ptr)=*(BB_ptr+j-3);
        *(BR_ptr+1)=*(BB_ptr+j-2);
        *(BR_ptr+2)=*(BB_ptr+j-1);

        BF_ptr=BF_ptr+mesh_width;
        BR_ptr=BR_ptr+mesh_length;
        BB_ptr=BB_ptr+j;
    }

    BB_ptr=verticesT->front_left;
    BF_ptr=verticesT->_front;
    BR_ptr=verticesT->left;

    for(i=0;i<Hslices+1;i++)         //front_left 兩側複製給top_front 與 top_left
    {
        *(BR_ptr+mesh_length-3)=*(BB_ptr);
        *(BR_ptr+mesh_length-2)=*(BB_ptr+1);
        *(BR_ptr+mesh_length-1)=*(BB_ptr+2);

        *(BF_ptr)=*(BB_ptr+j-3);
        *(BF_ptr+1)=*(BB_ptr+j-2);
        *(BF_ptr+2)=*(BB_ptr+j-1);

        BF_ptr=BF_ptr+mesh_width;
        BR_ptr=BR_ptr+mesh_length;
        BB_ptr=BB_ptr+j;
    }


    mesh_width=(FBVslices-1);
    mesh_length=(LRVslices-1);
    interval_x=(car_width/10)/FBVslices;
    interval_y=(car_length/10)/LRVslices;
    BF_ptr=verticesT->_front;
    BB_ptr=verticesT->_back;
    BR_ptr=BF_ptr+3;
    BL_ptr=BB_ptr+3;
    for(k=0;k<Hslices+1;k++)
    {
        for(i=0;i<mesh_width;i++)
        {
            *(BR_ptr)=*(BF_ptr)+interval_x;
            *(BR_ptr+1)=*(BF_ptr+1);
            *(BR_ptr+2)=*(BF_ptr+2);

            *(BL_ptr)=*(BB_ptr)-interval_x;
            *(BL_ptr+1)=*(BB_ptr+1);
            *(BL_ptr+2)=*(BB_ptr+2);

            BF_ptr=BF_ptr+3;
            BR_ptr=BR_ptr+3;
            BB_ptr=BB_ptr+3;
            BL_ptr=BL_ptr+3;
        }
        BF_ptr=BF_ptr+6;
        BR_ptr=BR_ptr+6;
        BB_ptr=BB_ptr+6;
        BL_ptr=BL_ptr+6;
    }

    BF_ptr=verticesT->left;
    BB_ptr=verticesT->right;
    BR_ptr=BF_ptr+3;
    BL_ptr=BB_ptr+3;
    for(k=0;k<Hslices+1;k++)
    {
        for(i=0;i<mesh_length;i++)
        {
            *(BR_ptr)=*(BF_ptr);
            *(BR_ptr+1)=*(BF_ptr+1)+interval_y;
            *(BR_ptr+2)=*(BF_ptr+2);

            *(BL_ptr)=*(BB_ptr);
            *(BL_ptr+1)=*(BB_ptr+1)-interval_y;
            *(BL_ptr+2)=*(BB_ptr+2);

            BF_ptr=BF_ptr+3;
            BR_ptr=BR_ptr+3;
            BB_ptr=BB_ptr+3;
            BL_ptr=BL_ptr+3;
        }
        BF_ptr=BF_ptr+6;
        BR_ptr=BR_ptr+6;
        BB_ptr=BB_ptr+6;
        BL_ptr=BL_ptr+6;
    }


}

void Line_index_create(AVM_index_ptr *index_lineB, AVM_index_ptr *index_lineT, int car_width, int car_length, int offset, int extend_opt)
{
    int i,j;
   // int temp;
   // int mesh_width,mesh_length;
    int extend;
    int FBsize,RLsize,Csize;

    unsigned short *BF_ptr,*BL_ptr,*BR_ptr;

    //*********************建立底部模型頂點資料*******************************************

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
    FBsize=(FBVslices*(Cslices+1)+(FBVslices+1)*Cslices)*2;
    RLsize=(LRVslices*(Cslices+1)+(LRVslices+1)*Cslices)*2;
    Csize=((Vslices*Cslices)+((Vslices+1)*Cslices))*2;

    index_lineB->circle=malloc(Csize*sizeof(short));
    index_lineB->front_back=malloc(FBsize*sizeof(short));
    index_lineB->right_left=malloc(RLsize*sizeof(short));

    FBsize=(FBVslices*(Hslices)+(FBVslices+1)*(Hslices))*2;
    RLsize=(LRVslices*(Hslices)+(LRVslices+1)*(Hslices))*2;
    Csize=((Vslices*Hslices)+((Vslices+1)*Hslices))*2;

    index_lineT->circle=malloc(Csize*sizeof(short));
    index_lineT->front_back=malloc(FBsize*sizeof(short));
    index_lineT->right_left=malloc(RLsize*sizeof(short));


    //**********************底部骨架的index建構************************
    BF_ptr=index_lineB->front_back;
    BR_ptr=index_lineB->right_left;
    BL_ptr=index_lineB->circle;

    for(j=0;j<Cslices+1;j++)
    {
        for(i=0;i<FBVslices;i++)
        {
            *(BF_ptr)=j*(FBVslices+1)+i;
            *(BF_ptr+1)=j*(FBVslices+1)+i+1;
            BF_ptr=BF_ptr+2;
        }
    }
    for(j=0;j<Cslices;j++)
    {
        for(i=0;i<FBVslices+1;i++)
        {
            *(BF_ptr)=j*(FBVslices+1)+i;
            *(BF_ptr+1)=j*(FBVslices+1)+i+(FBVslices+1);
            BF_ptr=BF_ptr+2;
        }
    }


    for(j=0;j<Cslices+1;j++)
    {
        for(i=0;i<LRVslices;i++)
        {
            *(BR_ptr)=j*(LRVslices+1)+i;
            *(BR_ptr+1)=j*(LRVslices+1)+i+1;
            BR_ptr=BR_ptr+2;
        }
    }
    for(j=0;j<Cslices;j++)
    {
        for(i=0;i<LRVslices+1;i++)
        {
            *(BR_ptr)=j*(LRVslices+1)+i;
            *(BR_ptr+1)=j*(LRVslices+1)+i+(LRVslices+1);
            BR_ptr=BR_ptr+2;
        }
    }


    for(j=0;j<Cslices;j++)
    {
        for(i=0;i<Vslices;i++)
        {
            *(BL_ptr)=j*(Vslices+1)+i;
            *(BL_ptr+1)=j*(Vslices+1)+i+1;
            BL_ptr=BL_ptr+2;
        }
    }
    for(j=0;j<Cslices;j++)
    {
        if(j==Cslices-1)
        {
            for(i=0;i<Vslices+1;i++)
            {
                *(BL_ptr)=j*(Vslices+1)+i;
                *(BL_ptr+1)=j*(Vslices+1)+(Vslices+1);
                BL_ptr=BL_ptr+2;
            }
        }
        else
        {
            for(i=0;i<Vslices+1;i++)
            {
                *(BL_ptr)=j*(Vslices+1)+i;
                *(BL_ptr+1)=j*(Vslices+1)+i+(Vslices+1);
                BL_ptr=BL_ptr+2;
            }
        }
    }
    //****************底部骨架的index建構*************************

    //**********************周圍骨架的index建構************************
    BF_ptr=index_lineT->front_back;
    BR_ptr=index_lineT->right_left;
    BL_ptr=index_lineT->circle;

    for(j=0;j<Hslices;j++)
    {
        for(i=0;i<FBVslices;i++)
        {
            *(BF_ptr)=j*(FBVslices+1)+i;
            *(BF_ptr+1)=j*(FBVslices+1)+i+1;
            BF_ptr=BF_ptr+2;
        }
    }
    for(j=0;j<Hslices;j++)
    {
        for(i=0;i<FBVslices+1;i++)
        {
            *(BF_ptr)=j*(FBVslices+1)+i;
            *(BF_ptr+1)=j*(FBVslices+1)+i+(FBVslices+1);
            BF_ptr=BF_ptr+2;
        }
    }


    for(j=0;j<Hslices;j++)
    {
        for(i=0;i<LRVslices;i++)
        {
            *(BR_ptr)=j*(LRVslices+1)+i;
            *(BR_ptr+1)=j*(LRVslices+1)+i+1;
            BR_ptr=BR_ptr+2;
        }
    }
    for(j=0;j<Hslices;j++)
    {
        for(i=0;i<LRVslices+1;i++)
        {
            *(BR_ptr)=j*(LRVslices+1)+i;
            *(BR_ptr+1)=j*(LRVslices+1)+i+(LRVslices+1);
            BR_ptr=BR_ptr+2;
        }
    }


    for(j=0;j<Hslices;j++)
    {
        for(i=0;i<Vslices;i++)
        {
            *(BL_ptr)=j*(Vslices+1)+i;
            *(BL_ptr+1)=j*(Vslices+1)+i+1;
            BL_ptr=BL_ptr+2;
        }
    }
    for(j=0;j<Hslices;j++)
    {
        for(i=0;i<Vslices+1;i++)
        {
            *(BL_ptr)=j*(Vslices+1)+i;
            *(BL_ptr+1)=j*(Vslices+1)+i+(Vslices+1);
            BL_ptr=BL_ptr+2;
        }
    }
    //****************周圍骨架的index建構*************************



}



void Triangles_index_create(AVM_index_ptr *index_triB, AVM_index_ptr *index_triT, int car_width, int car_length, int offset, int extend_opt)
{
    int i,j;
   // int temp;
    //int mesh_width,mesh_length;
    int extend;
    int FBsize,RLsize,Csize;

    unsigned short *BF_ptr,*BL_ptr,*BR_ptr;

    //*********************建立底部模型頂點資料*******************************************

    switch(extend_opt)              //決定底部可視範圍為2M/2.5M/3M/3.5M/4M   (需與model.h中的xyextend)
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
    FBsize=(FBVslices)*2*(Cslices)*3;
    RLsize=(LRVslices)*2*(Cslices)*3;
    Csize=(Vslices)*2*(Cslices-1)*3+(Vslices)*3;

    index_triB->circle=malloc(Csize*sizeof(short));
    index_triB->front_back=malloc(FBsize*sizeof(short));
    index_triB->right_left=malloc(RLsize*sizeof(short));


    FBsize=(FBVslices)*2*(Hslices)*3;
    RLsize=(LRVslices)*2*(Hslices)*3;
    Csize=(Vslices)*2*(Hslices)*3;

    index_triT->circle=malloc(Csize*sizeof(short));
    index_triT->front_back=malloc(FBsize*sizeof(short));
    index_triT->right_left=malloc(RLsize*sizeof(short));



    BF_ptr=index_triB->front_back;
    BL_ptr=index_triB->right_left;
    BR_ptr=index_triB->circle;
    //***************底部三角index建立***************************
    for(j=0;j<Cslices;j++)
    {
        for(i=0;i<FBVslices;i++)
        {
            *(BF_ptr)=j*(FBVslices+1)+i;                            //index 須逆時針轉方為正面
            *(BF_ptr+1)=j*(FBVslices+1)+i+(FBVslices+1);
            *(BF_ptr+2)=j*(FBVslices+1)+i+1;

            BF_ptr=BF_ptr+3;
        }
    }
    for(j=0;j<Cslices;j++)
    {
        for(i=0;i<FBVslices;i++)
        {
            *(BF_ptr)  =(j+1)*(FBVslices+1)+i;
            *(BF_ptr+1)=(j+1)*(FBVslices+1)+i+1;
            *(BF_ptr+2)=j*(FBVslices+1)+i+1;

            BF_ptr=BF_ptr+3;
        }
    }

    for(j=0;j<Cslices;j++)
    {
        for(i=0;i<LRVslices;i++)
        {
            *(BL_ptr)  =j*(LRVslices+1)+i;
            *(BL_ptr+1)=j*(LRVslices+1)+i+(LRVslices+1);
            *(BL_ptr+2)=j*(LRVslices+1)+i+1;

            BL_ptr=BL_ptr+3;
        }
    }
    for(j=0;j<Cslices;j++)
    {
        for(i=0;i<LRVslices;i++)
        {
            *(BL_ptr)  =(j+1)*(LRVslices+1)+i;
            *(BL_ptr+1)=(j+1)*(LRVslices+1)+i+1;
            *(BL_ptr+2)=j*(LRVslices+1)+i+1;

            BL_ptr=BL_ptr+3;
        }
    }

    for(j=0;j<Cslices;j++)
    {
        if(j==Cslices-1)
        {
            for(i=0;i<Vslices;i++)
            {
                *(BR_ptr)=j*(Vslices+1)+i;
                *(BR_ptr+1)=j*(Vslices+1)+(Vslices+1);
                *(BR_ptr+2)=j*(Vslices+1)+i+1;

                BR_ptr=BR_ptr+3;
            }
        }
        else
        {
            for(i=0;i<Vslices;i++)
            {
                *(BR_ptr)=j*(Vslices+1)+i;
                *(BR_ptr+1)=j*(Vslices+1)+i+(Vslices+1);
                *(BR_ptr+2)=j*(Vslices+1)+i+1;

                BR_ptr=BR_ptr+3;
            }
        }
    }
    for(j=0;j<(Cslices)-1;j++)
    {
        for(i=0;i<Vslices;i++)
        {
            *(BR_ptr)=(j+1)*(Vslices+1)+i;
            *(BR_ptr+1)=(j+1)*(Vslices+1)+i+1;
            *(BR_ptr+2)=j*(Vslices+1)+i+1;

            BR_ptr=BR_ptr+3;
        }
    }
    //***************底部三角index建立***************************
    BF_ptr=index_triT->front_back;
    BL_ptr=index_triT->right_left;
    BR_ptr=index_triT->circle;
    //***************周圍三角index建立***************************

    for(j=0;j<Hslices;j++)
    {
        for(i=0;i<FBVslices;i++)
        {
            *(BF_ptr)=j*(FBVslices+1)+i;                            //index 須逆時針轉方為正面
            *(BF_ptr+1)=j*(FBVslices+1)+i+(FBVslices+1);
            *(BF_ptr+2)=j*(FBVslices+1)+i+1;

            BF_ptr=BF_ptr+3;
        }
    }
    for(j=0;j<Hslices;j++)
    {
        for(i=0;i<FBVslices;i++)
        {
            *(BF_ptr)  =(j+1)*(FBVslices+1)+i;
            *(BF_ptr+1)=(j+1)*(FBVslices+1)+i+1;
            *(BF_ptr+2)=j*(FBVslices+1)+i+1;

            BF_ptr=BF_ptr+3;
        }
    }

    for(j=0;j<Hslices;j++)
    {
        for(i=0;i<LRVslices;i++)
        {
            *(BL_ptr)  =j*(LRVslices+1)+i;
            *(BL_ptr+1)=j*(LRVslices+1)+i+(LRVslices+1);
            *(BL_ptr+2)=j*(LRVslices+1)+i+1;

            BL_ptr=BL_ptr+3;
        }
    }
    for(j=0;j<Hslices;j++)
    {
        for(i=0;i<LRVslices;i++)
        {
            *(BL_ptr)  =(j+1)*(LRVslices+1)+i;
            *(BL_ptr+1)=(j+1)*(LRVslices+1)+i+1;
            *(BL_ptr+2)=j*(LRVslices+1)+i+1;

            BL_ptr=BL_ptr+3;
        }
    }

    for(j=0;j<Hslices;j++)
    {
        for(i=0;i<Vslices;i++)
        {
            *(BR_ptr)=j*(Vslices+1)+i;
            *(BR_ptr+1)=j*(Vslices+1)+i+(Vslices+1);
            *(BR_ptr+2)=j*(Vslices+1)+i+1;
            BR_ptr=BR_ptr+3;
        }
    }
    for(j=0;j<Hslices;j++)
    {
        for(i=0;i<Vslices;i++)
        {
            *(BR_ptr)=(j+1)*(Vslices+1)+i;
            *(BR_ptr+1)=(j+1)*(Vslices+1)+i+1;
            *(BR_ptr+2)=j*(Vslices+1)+i+1;
            BR_ptr=BR_ptr+3;
        }
    }
    //***************周圍三角index建立***************************
}

void overlap_create(overlap_ptr *alpha)          //沒有彈性的寫法!!!!!!!!!!!!! Vslice不可以修改
{
    int i;
    int BT_size, TP_size;
    float *index;
    BT_size=(Vslices+1)*(Cslices)+1;
    TP_size=(Vslices+1)*(Hslices+1);

    alpha->BT_left=malloc(BT_size*sizeof(float));
    alpha->BT_right=malloc(BT_size*sizeof(float));

    alpha->TP_left_up=malloc(TP_size*sizeof(float));
    alpha->TP_left_down=malloc(TP_size*sizeof(float));
    alpha->TP_right_up=malloc(TP_size*sizeof(float));
    alpha->TP_right_down=malloc(TP_size*sizeof(float));

    index=alpha->BT_left;
    for(i=0;i<Cslices;i++)
    {
      /*  *(index)  =0.0;
        *(index+1)=0.0;
        *(index+2)=0.2;
        *(index+3)=0.3;
        *(index+4)=0.4;
        *(index+5)=0.6;
        *(index+6)=0.7;
        *(index+7)=0.8;
        *(index+8)=1.0;
        *(index+9)=1.0;
        index=index+10;
*/
		*(index)  =0.0;
		*(index+1)=0.2;
		*(index+2)=0.4;
		*(index+3)=0.6;
		*(index+4)=0.8;
		*(index+5)=1.0;
		*(index+6)=1.0;
		*(index+7)=1.0;
		*(index+8)=1.0;
		*(index+9)=1.0;
		*(index+10)=1.0;
		index=index+11;

		
    }
    *(index)=1;

    index=alpha->BT_right;
    for(i=0;i<Cslices;i++)
    {
  /*      *(index)  =1.0;
        *(index+1)=1.0;
        *(index+2)=0.8;
        *(index+3)=0.7;
        *(index+4)=0.6;
        *(index+5)=0.4;
        *(index+6)=0.3;
        *(index+7)=0.2;
        *(index+8)=0.0;
        *(index+9)=0.0;
        index=index+10;*/

	  *(index)	=1.0;
	  *(index+1)=1.0;
	  *(index+2)=1.0;
	  *(index+3)=1.0;
	  *(index+4)=1.0;
	  *(index+5)=0.8;
	  *(index+6)=0.6;
	  *(index+7)=0.4;
	  *(index+8)=0.2;
	  *(index+9)=0.0;
	  *(index+10)=0.0;
	  index=index+11;

    }
    *(index)=1;

    index=alpha->TP_right_up;
    for(i=0;i<Hslices+1;i++)
    {
        *(index)  =0.0;
        *(index+1)=0.0;
        *(index+2)=0.0;
        *(index+3)=0.0;
        *(index+4)=0.0;
        *(index+5)=0.5;
        *(index+6)=1.0;
        *(index+7)=1.0;
        *(index+8)=1.0;
        *(index+9)=1.0;
		*(index+10)=1.0;
		index=index+11;

    }

    index=alpha->TP_right_down;
    for(i=0;i<Hslices+1;i++)
    {
        *(index)  =1.0;
        *(index+1)=1.0;
        *(index+2)=1.0;
        *(index+3)=1.0;
        *(index+4)=1.0;
        *(index+5)=0.5;
        *(index+6)=0.0;
        *(index+7)=0.0;
        *(index+8)=0.0;
        *(index+9)=0.0;
		*(index+10)=0.0;
		index=index+11;

    }

    index=alpha->TP_left_down;
    for(i=0;i<Hslices+1;i++)
    {
        *(index)  =0.0;
        *(index+1)=0.0;
        *(index+2)=0.0;
        *(index+3)=0.0;
        *(index+4)=0.0;
        *(index+5)=0.5;
        *(index+6)=1.0;
        *(index+7)=1.0;
        *(index+8)=1.0;
        *(index+9)=1.0;
		*(index+10)=1.0;
		index=index+11;

    }

    index=alpha->TP_left_up;
    for(i=0;i<Hslices+1;i++)
    {
        *(index)  =1.0;
        *(index+1)=1.0;
        *(index+2)=1.0;
        *(index+3)=1.0;
        *(index+4)=1.0;
        *(index+5)=0.5;
        *(index+6)=0.0;
        *(index+7)=0.0;
        *(index+8)=0.0;
        *(index+9)=0.0;
		*(index+10)=0.0;
		index=index+11;

    }
}
