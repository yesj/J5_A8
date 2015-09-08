#define outPutWidth (736)
#define outPutHeight (480)

//Car type
//#define WISH



extern FeaturePts featurePts[4];//8ch AVM[4];            	// 每一隻攝影機有六/八個影像特徵點
extern unsigned short VLines;
extern unsigned char* AllImages1[4];//[5];            // 全部輸入影像指標: video input*4 + car*1 8CH
extern unsigned char* AllImagesUV[4];//[5];            // 全部輸入影像指標: video input*4 + car*1 8CH
//extern unsigned char aroundview[1280*720*2];     // 輸出影像矩陣
extern unsigned char* aroundview;     // 輸出影像矩陣
extern unsigned char* aroundviewuv;     // 輸出影像矩陣
extern unsigned short corner_X[16];   	 // 世界座標中所有特徵點的X座標
extern unsigned short corner_Y[16];	 // 世界座標中所有特徵點的Y座標
extern unsigned short points_X[16];	// 8CH AVM[20];    // 世界座標中所有特徵點的X座標
extern unsigned short points_Y[16];	// 8CH AVM[20];    // 世界座標中所有特徵點的Y座標
extern Image_AXIS Video_AXIS[16];//8ch avm[20];           	// 宣告影像座標陣列結構，最多可將AVM分成20區塊
extern LS_AXIS Homo_AXIS[4];		//[4];  8CH AVM				 	// 記錄每隻攝影機的(Tx,Ty)及(Bx,By)==> least square solution

extern unsigned int  CamNum[4]; //8ch AVM  	// 紀錄AVM區塊每一隻攝影機共有幾個影像點
extern FeaturePtsFloat fPtsFloat[4];//8ch AVM[4];        	// 每一隻攝影機有六/八個影像特徵點(浮點數)
extern CamUD_Para UD_Para; // For New Model
extern float 	CS_b[20];		// CubicSpline各區段間的參數b
extern float 	CS_c[20];		// CubicSpline各區段間的參數c
extern float 	CS_d[20];		// CubicSpline各區段間的參數d 
extern unsigned short Camera_UD_Parameter[4][25];  
extern LS_AXIS Homo_AXIS[4];		//[4];  8CH AVM				 	// 記錄每隻攝影機的(Tx,Ty)及(Bx,By)==> least square solution


     // AVM映射表
extern unsigned int AXIS_mapping1[outPutWidth*outPutHeight*1]; 	// 視角轉換後的影像陣列
extern unsigned char AVMX_f[outPutWidth*outPutHeight*1];       	// 視角轉換雙線性內插權重值 X
extern unsigned char AVMY_f[outPutWidth*outPutHeight*1];       	// 視角轉換雙線性內插權重值 Y
extern unsigned int  LeftPos[outPutWidth*outPutHeight*1];      	// AVM 查表時，每一個點在aroundview的位置
extern unsigned int UVPos[outPutWidth*outPutHeight*1];

extern float matrix[9][9];                  	// homography計算矩陣
extern float HH[3][3];							// homography轉換矩陣
extern unsigned int AXISUV_mapping1[outPutWidth*outPutHeight*1]; 	// 視角轉換後的影像陣列

extern int Corners[6];	
extern CornerPts cornerPts[6];			 //角點座標

//20150321 座標轉換
extern float hh1[3][3];
extern float hh2[3][3];
extern float hh3[3][3];
extern float hh4[3][3];

