#include <stdio.h>
#include <string.h>

// AutoRAD function include Files 

//#include "All_Define.h"        // AVM functions 
#include "AVM_Homography.h"
#include "AVM_Fun.h"
#include "All_define.h"
#include "AutoRadAVMLib.h"


// 20150321 座標轉換用
float hh1[3][3];
float hh2[3][3];
float hh3[3][3];
float hh4[3][3];
FeaturePtsFloat fPivotFloat[4]; //用來存4個ch的pivot(反扭曲座標)
FeaturePts fPivot[4]; //用來存4個ch的pivot(原圖座標)
FeaturePtsFloat VanishingFloat[4]; //用來存4個ch的vanishing point(反扭曲座標)


					 // 校正模式會用到的指標變數
float matrix[9][9];                  	// homography計算矩陣
float HH[3][3];							// homography轉換矩陣					 
unsigned short points_X[16];	// AVM[20];    // 世界座標中所有特徵點的X座標
unsigned short points_Y[16];	// AVM[20];    // 世界座標中所有特徵點的Y座標				 
unsigned short corner_X[16];   	 // 世界座標中所有特徵點的X座標
unsigned short corner_Y[16];	 // 世界座標中所有特徵點的Y座標
	
Image_AXIS Video_AXIS[16];      	// 宣告影像座標陣列結構，最多可將AVM分成20區塊    
LS_AXIS Homo_AXIS[4];		 	// 記錄每隻攝影機的(Tx,Ty)及(Bx,By)==> least square solution    
    

  // Cubic Spline計算後所得參數
float 	CS_b[20];		// CubicSpline各區段間的參數b
float 	CS_c[20];		// CubicSpline各區段間的參數c
float 	CS_d[20];		// CubicSpline各區段間的參數d 
unsigned short Camera_UD_Parameter[4][25];   
//unsigned short TI_Camera[25]={0 ,140 ,260, 350, 415, 460, 492, 516, 535, 550,560, 570, 579, 600,0 ,0 ,0 ,0 ,0 ,0 ,12 ,12 ,15 ,260 ,250}; 
//unsigned short TI_Camera_CH0[25]={0 ,121 ,227, 310, 372, 418, 452, 478, 498, 515,527, 538, 546, 620,0 ,0 ,0 ,0 ,0 ,0 ,12 ,12 ,15 ,310 ,307};//CH0


#ifdef WISH 
unsigned short TI_Camera_CH0[25]={0,94,166,212,240,258,270,280,286,291,295,298,301,303,305,308,310,312,316,370,18,18,30,239,212};//CH0 CS30A180N for WISH
#else
unsigned short TI_Camera_CH0[25]={0 ,48 ,93 ,130 ,158 ,181 ,198 ,211 ,222 ,230 ,237 ,243,248 ,266 ,0 ,0 ,0 ,0 ,0 ,0 ,12 ,12 ,18 ,212 ,182}; //C902
//unsigned short TI_Camera_CH0[25]={0 ,122 ,191 ,225 ,243 ,254 ,262 ,268 ,272 ,276 ,278 ,280, 282 ,286 ,0 ,0 ,0 ,0 ,0 ,0 ,12 ,12 ,15 ,258 ,228}; //C139 data2
//unsigned short TI_Camera_CH0[25]={0 ,123 ,194 ,228 ,248 ,258 ,265 ,270 ,274 ,278 ,281 ,282, 283 ,289 ,0 ,0 ,0 ,0 ,0 ,0 ,12 ,12 ,15 ,256 ,226}; //C139 data4
//unsigned short TI_Camera_CH0[25]={0 ,125 ,196 ,230 ,248 ,259 ,268 ,273 ,278 ,281 ,283 ,285, 287 ,290 ,0 ,0 ,0 ,0 ,0 ,0 ,12 ,12 ,15 ,257 ,230}; //C139 data1
 

#endif
/*
unsigned short TI_Camera_CH0[25]={0 ,121 ,227, 310, 372, 418, 452, 478, 498, 515,527, 538, 546, 600,0 ,0 ,0 ,0 ,0 ,0 ,12 ,12 ,15 ,310 ,307};//CH0
unsigned short TI_Camera_CH1[25]={0 ,120 ,226, 308, 369, 416, 450, 476, 497, 512,526, 536, 544, 600,0 ,0 ,0 ,0 ,0 ,0 ,12 ,12 ,15 ,416 ,398};//CH1   //改到3
unsigned short TI_Camera_CH2[25]={0 ,116 ,221, 307, 370, 417, 453, 481, 502, 519,533, 544, 553, 600,0 ,0 ,0 ,0 ,0 ,0 ,12 ,12 ,15 ,307 ,301};//CH2 
unsigned short TI_Camera_CH3[25]={0 ,120 ,222, 301, 360, 405, 438, 464, 485, 501,514, 525, 534, 600,0 ,0 ,0 ,0 ,0 ,0 ,12 ,12 ,15 ,361 ,357};//CH3  //改到1
*/
//unsigned char ResizedCar[1280*720*2]; 	// 用來放resize之後的車車圖片
unsigned int  CamNum[4]={0,0,0,0}; 	// 紀錄AVM區塊每一隻攝影機共有幾個影像點	
FeaturePtsFloat fPtsFloat[4];       	// 每一隻攝影機有六/八個影像特徵點(浮點數)	 
FeaturePts featurePts[4];            	// 每一隻攝影機有六/八個影像特徵點
CamUD_Para UD_Para; // For New Model
LS_AXIS Homo_AXIS[4];			 	// 記錄每隻攝影機的(Tx,Ty)及(Bx,By)==> least square solution


    // 映射表
unsigned int AXIS_mapping1[outPutWidth*outPutHeight*1]; 	// 視角轉換後的影像陣列
unsigned char AVMX_f[outPutWidth*outPutHeight*1];       	// 視角轉換雙線性內插權重值 X
unsigned char AVMY_f[outPutWidth*outPutHeight*1];       	// 視角轉換雙線性內插權重值 Y
unsigned int  LeftPos[outPutWidth*outPutHeight*1];      	// AVM 查表時，每一個點在aroundview的位置
unsigned int UVPos[outPutWidth*outPutHeight*1];
unsigned int AXISUV_mapping1[outPutWidth*outPutHeight*1]; 	// 視角轉換後的影像陣列


unsigned char* AllImages1[4];            // 全部輸入影像指標: video input*4 + car*1 8CH
unsigned char* AllImagesUV[4];          // 全部輸入影像指標: video input*4 + car*1 8CH
//unsigned char aroundview[1280*720*2];     // 輸出影像矩陣
unsigned char *aroundview;     // 輸出影像矩陣
unsigned char *aroundviewuv;     // 輸出影像矩陣
unsigned short VLines = outPutHeight;

unsigned char Firststart=1;


int Corners[6];	
CornerPts cornerPts[6];			 //角點座標




void AutoRadHomoFun(unsigned char* VideoInY,unsigned char* VideoInUV,unsigned char* VideoOutY)
{

	int i;
	  
		//要增加輸入變數
	  aroundview = VideoOutY;
      AllImages1[0]=VideoInY;
      AllImages1[1]=VideoInY;
	  AllImages1[2]=VideoInY;
	  AllImages1[3]=VideoInY;
	
	  AllImagesUV[0]=VideoInUV;
	  AllImagesUV[1]=VideoInUV;
	  AllImagesUV[2]=VideoInUV;
	  AllImagesUV[3]=VideoInUV;


  
	if(Firststart ==1) //程式第一次啟動，需做表 
	{
		Firststart=0;
    	
		// 設定調校點與影像範圍
		PointsCalculation_Homo();

		// 特徵點分組===>將points設定到Video_AXIS裡
		PointsSetting();

    	// Setting UD parameter
    	memcpy(Camera_UD_Parameter[0], TI_Camera_CH0, sizeof(TI_Camera_CH0)); 


	#ifdef WISH 

		//WISH //槫槫槫前後影像ROTATE翻轉後(正的座標)
		//前
		featurePts[0].x[0] = 204;	featurePts[0].y[0] = 265;	//左�W
		featurePts[0].x[1] = 534;	featurePts[0].y[1] = 265;	//右�W
		featurePts[0].x[2] = 117;	featurePts[0].y[2] = 312;	//左�U
		featurePts[0].x[3] = 623;	featurePts[0].y[3] = 315;	//右�U
		//右
		featurePts[1].x[0] = 302;	featurePts[1].y[0] = 267;	//左�W
		featurePts[1].x[1] = 562;	featurePts[1].y[1] = 255;	//右�W
		featurePts[1].x[2] = 275;	featurePts[1].y[2] = 345;	//左�U
		featurePts[1].x[3] = 616;	featurePts[1].y[3] = 300;	//右�U
		//後
		featurePts[2].x[0] = 239;	featurePts[2].y[0] = 230;	//左�W
		featurePts[2].x[1] = 517;	featurePts[2].y[1] = 229;	//右�W
		featurePts[2].x[2] = 191;	featurePts[2].y[2] = 286;	//左�U
		featurePts[2].x[3] = 569;	featurePts[2].y[3] = 285;	//右�U
		//左	
		featurePts[3].x[0] = 147;	featurePts[3].y[0] = 255;	//左�W
		featurePts[3].x[1] = 404;	featurePts[3].y[1] = 262;	//右�W
		featurePts[3].x[2] = 95;    featurePts[3].y[2] = 301;   //左�U
		featurePts[3].x[3] = 434;	featurePts[3].y[3] = 336;	//右�U
		
		// Image point below the camera
		fPivot[0].x[0] = 361;	fPivot[0].y[0] = 528; // ch0 front
		fPivot[1].x[0] = 370;	fPivot[1].y[0] = 430; // ch1 right
		fPivot[2].x[0] = 363;	fPivot[2].y[0] = 411;  // ch2 rear
		fPivot[3].x[0] = 338;	fPivot[3].y[0] = 426; // ch3 left

	#else

		//demo c ar
		//前
		featurePts[0].x[0] = 138;	featurePts[0].y[0] = 226;	//左�W
		featurePts[0].x[1] = 563;	featurePts[0].y[1] = 224;	//右�W
		featurePts[0].x[2] = 92;	featurePts[0].y[2] = 317;	//左�U
		featurePts[0].x[3] = 612;	featurePts[0].y[3] = 315;	//右�U
		//右
		featurePts[1].x[0] = 294;	featurePts[1].y[0] = 245;	//左�W
		featurePts[1].x[1] = 600;	featurePts[1].y[1] = 251;	//右�W
		featurePts[1].x[2] = 269;	featurePts[1].y[2] = 356;	//左�U
		featurePts[1].x[3] = 627;	featurePts[1].y[3] = 309;	//右�U
		//後
		featurePts[2].x[0] = 176;	featurePts[2].y[0] = 246;	//左�W
		featurePts[2].x[1] = 557;	featurePts[2].y[1] = 244;	//右�W
		featurePts[2].x[2] = 140;	featurePts[2].y[2] = 330;	//左�U
		featurePts[2].x[3] = 594;	featurePts[2].y[3] = 327;	//右�U
		//左	
		featurePts[3].x[0] = 117;	featurePts[3].y[0] = 253;	//左�W
		featurePts[3].x[1] = 421;	featurePts[3].y[1] = 242;	//右�W
		featurePts[3].x[2] = 91;   featurePts[3].y[2] = 313;   //左�U
		featurePts[3].x[3] = 433;	featurePts[3].y[3] = 357;	//右�U

		// Image point below the camera
		fPivot[0].x[0] = 354;	fPivot[0].y[0] = 382; // ch0 front
		fPivot[1].x[0] = 350;	fPivot[1].y[0] = 374; // ch1 right
		fPivot[2].x[0] = 370;	fPivot[2].y[0] = 373; // ch2 rear
		fPivot[3].x[0] = 371;	fPivot[3].y[0] = 365; // ch3 left

	#endif



		memset(CamNum, 0, sizeof(CamNum));
		memset(LeftPos, 0, sizeof(LeftPos));
		memset(AXIS_mapping1, 0, sizeof(AXIS_mapping1));
		memset(AVMX_f, 0, sizeof(AVMX_f));
		memset(AVMY_f, 0, sizeof(AVMY_f));
		memset(UVPos, 0, sizeof(UVPos));
		memset(AXISUV_mapping1, 0, sizeof(AXISUV_mapping1));

		// 3.5 由原始影像的座標正向映射到反扭曲影像的座標(浮點數)
		memset(fPtsFloat,0, sizeof(fPtsFloat));
		memset(fPivotFloat,0, sizeof(fPivotFloat)); // 20150322
		memset(VanishingFloat,0, sizeof(VanishingFloat)); // 20150331
		for (i=0;i < 4	;i++)
		{
			UDPosGet(0, featurePts[i], &fPtsFloat[i]);
			UDPosGet(0, fPivot[i], &fPivotFloat[i]); // 20150322
		}
		UDVanishingGet(fPtsFloat, VanishingFloat); //20150331 指標參數

		int a00, a01;
		a00 = fPivotFloat[0].x[0] * 1000000;
		a01 = fPivotFloat[0].y[0] * 1000000;
		//Vps_printf("●(￣^￣ )●$ fPtsFloat: x %d y %d  \n", a00, a01);hkc

		for (i=0; i < 4; i++)
		{
			a00 = VanishingFloat[i].x[0] * 1000000;
			a01 = VanishingFloat[i].y[0] * 1000000;
			//Vps_printf("●(￣^￣ )●$ VanishingFloat[%d]: x %d y %d  \n", i, a00, a01);hkc
		}

		
		FPts_Setting_Homo(fPtsFloat); 								
		       

		for (i=0;i < 1	;i++)
		{							
			// 3.2 matrix設定：輸入座標結構, 輸出接下來要計算的矩陣
			LSMatrixMaking_Homo(Homo_AXIS[i]);
			Homography_TableMaking( i, AXIS_mapping1, AVMX_f, AVMY_f, LeftPos, CamNum); // 3.4.1 計算 AVM 映射表 -- 有灰階補償
		}
				
      } //	if(Firststart ==1) //程式第一次啟動，需做表 


	//Homographymapping(aroundview, 							  VideoInY, VideoInUV, &AVMX_f[0],  							&AVMY_f[0],								&AXIS_mapping1[0],								&LeftPos[0], 							CamNum[0]-1,  &UVPos[0], 							&AXISUV_mapping1[0]								,0);	  
	//Homographymapping(aroundview+outPutWidth*outPutHeight*2,  VideoInY, VideoInUV, &AVMX_f[CamNum[0]],  					&AVMY_f[CamNum[0]],						&AXIS_mapping1[CamNum[0]],						&LeftPos[CamNum[0]], 					CamNum[1]-1,  &UVPos[CamNum[0]], 					&AXISUV_mapping1[CamNum[0]]						,1);		
	//Homographymapping(aroundview+outPutWidth*outPutHeight*4,  VideoInY, VideoInUV, &AVMX_f[CamNum[0]+CamNum[1]],  			&AVMY_f[CamNum[0]+CamNum[1]],			&AXIS_mapping1[CamNum[0]+CamNum[1]],			&LeftPos[CamNum[0]+CamNum[1]], 			CamNum[2]-1,  &UVPos[CamNum[0]+CamNum[1]], 			&AXISUV_mapping1[CamNum[0]+CamNum[1]]			,2);	
	//Homographymapping(aroundview+outPutWidth*outPutHeight*6,  VideoInY, VideoInUV, &AVMX_f[CamNum[0]+CamNum[1]+CamNum[2]],  &AVMY_f[CamNum[0]+CamNum[1]+CamNum[2]],	&AXIS_mapping1[CamNum[0]+CamNum[1]+CamNum[2]],	&LeftPos[CamNum[0]+CamNum[1]+CamNum[2]],CamNum[3]-1,  &UVPos[CamNum[0]+CamNum[1]+CamNum[2]],&AXISUV_mapping1[CamNum[0]+CamNum[1]+CamNum[2]]	,3);	

}


//由4個特徵點計算消失點
void UDVanishingGet(FeaturePtsFloat* NfPts, FeaturePtsFloat* VanishingfPts)
{
	int i;

	for (i = 0; i < 4; i++) // 4 cameras
	{
		float f[2][3]; // ax + by + c = 0

		// 二點式
		// (y-y1)(x2-x1) = (y2-y1)(x-x1) ==> (y2-y1)x + (x1-x2)y + y1x2 - y2x1 = 0

		// 0 1
		// 2 3

		// line(0, 2)
		f[0][0] = NfPts[i].y[0] - NfPts[i].y[2]; // interface_y[i] - inner_y[i];
		f[0][1] = NfPts[i].x[2] - NfPts[i].x[0]; // inner_x[i] - interface_x[i];
		f[0][2] = NfPts[i].y[2] * NfPts[i].x[0] - NfPts[i].y[0] * NfPts[i].x[2]; // inner_y[i] * interface_x[i] - interface_y[i] * inner_x[i];

		// line(1, 3)
		f[1][0] = NfPts[i].y[1] - NfPts[i].y[3];
		f[1][1] = NfPts[i].x[3] - NfPts[i].x[1];
		f[1][2] = NfPts[i].y[3] * NfPts[i].x[1] - NfPts[i].y[1] * NfPts[i].x[3];

		// 兩直線交點
		//Cramer's Rule
		//float detA = f[y+1][0] * f2[x][1] - f[y+1][1] * f2[x][0];
		//float_x0 = (-f[y+1][2] * f2[x][1] - f[y+1][1] * -f2[x][2]) / detA;
		//float_y0 = (f[y+1][0] * -f2[x][2] + f[y+1][2] * f2[x][0]) / detA;

		float detA = f[0][0] * f[1][1] - f[0][1] * f[1][0];
		VanishingfPts[i].x[0] = (-f[0][2] * f[1][1] - f[0][1] * -f[1][2]) / detA;
		VanishingfPts[i].y[0] = (f[0][0] * - f[1][2] + f[0][2] * f[1][0]) / detA;
		
	}
}


// 20150322 座標轉換
void AutoRadHomoInit(int CamIndex)
{
	LSMatrixMaking_Homo(Homo_AXIS[CamIndex]);
	Homography_Init(CamIndex);
}

void AutoRadBirds2UD(float x_birds, float y_birds, float *x_ud, float *y_ud, int CamIndex)
{
	BirdsEyeView2UD(x_birds, y_birds, x_ud, y_ud, CamIndex);
}

void AutoRadUDtoORG(float X_UD, float Y_UD, float* X_ORG, float* Y_ORG, int Camch)
{
	UDtoORG(X_UD, Y_UD, X_ORG, Y_ORG, 0);
}

void AutoRadORGtoUD(short X_ORG, short Y_ORG, float* X_UD, float* Y_UD, int Camch)
{
	ORGtoUD(X_ORG, Y_ORG, X_UD, Y_UD, 0);
}

void AutoRadGetPivot(float *x_ud, float *y_ud, int CamIndex)
{
	*x_ud = fPivotFloat[CamIndex].x[0];
	*y_ud = fPivotFloat[CamIndex].y[0];
}

void AutoRadGetVanishing(float *x_ud, float *y_ud, int CamIndex)
{
	*x_ud = VanishingFloat[CamIndex].x[0];
	*y_ud = VanishingFloat[CamIndex].y[0];
}

void AutoRadGetfeature(float *x_ud, float *y_ud, int CamIndex, int ID)
{
	*x_ud = fPtsFloat[CamIndex].x[ID];
	*y_ud = fPtsFloat[CamIndex].y[ID];
}

	
