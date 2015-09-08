typedef struct{
	float x[8];
   	float y[8];
}FeaturePtsFloat;
typedef struct {
	short x[8];
   	short y[8];
}FeaturePts;
typedef struct {
	unsigned char page;	   
	unsigned char item;	   
	unsigned char subPage;
	unsigned char subItem; 
	unsigned char lock;
	unsigned char AVMType;
	unsigned char CAMIndex;
	unsigned char CAMType[6]; //8CH AVM
	unsigned char CAMRotate[6]; //8CH AVM
	unsigned char FPIndex; 
	unsigned char IFType;
	unsigned char ParameterNum;
}Menu;
//角偵測
typedef struct{
	unsigned short x[200];
   	unsigned short y[200];
}CornerPts; 
typedef struct {
	short x;
	short y;
}OnePoint;
typedef struct {
	unsigned char DPSizeH;		// high byte of sizeof(DP)
	unsigned char DPSizeL;		// low byte of sizeof(DP)
	// AVM layout
		// 所需參數
	unsigned char AVMType;		// 0:小車, 1:truck, 2:pickup, 3:Bus
	unsigned short CarWidth;	// 車寬(cm)
	unsigned short CarLength;	// 車長(cm)
	unsigned short X_Exten;		// X方向AVM延伸長度
	unsigned short Y_Exten;		// Y方向AVM延伸長度
	unsigned char PatternType;	// Pattern類型
		// 計算結果
	short AVM_Width;			// AVM 寬度(pixel)
	short AVM_Length;			// AVM 長度(pixel)
	int AVMStart;				// AVM 起始位置(pixel)	
	// UD
		// 所需參數
	unsigned char CAMType[6];//8ch AVM[4];	// 攝影機型號
	unsigned char CAMRotate[6];//8ch AVM[4]; // 安裝方向(正裝:0, 倒裝:1)

	// Homography
		// 所需參數	
	FeaturePts FPs[6];//8ch AVM[4];			// 6組特徵點，一組六點
		// 計算結果
	unsigned int CamNum[4];//8ch AVM[4];		// AVM各區塊中每隻攝影機各有幾個影像點	
	unsigned int CamNumWSG[4];//8ch AVM[4];	// 左右攝影機的影像疊合區域點數
	
	unsigned char Off_or_Rear;
	// 縫合類型或者是融合類型
	unsigned char Stich_or_Blend;
	// 螢幕影像顯示修正比例
	float MonitorRatio;
	// 螢幕左側黑邊寬度
	unsigned short LeftShift;
	// 螢幕右側黑邊寬度
	unsigned short RightShift;
	unsigned char  wROI;
	unsigned char isManualCal;
}DisplayParameter;
// 視點轉換參數結構 T:校正點世界座標(cm) B:校正點影像座標(pixel) C:校正區域世界座標(cm)
typedef struct {
	// 世界座標(cm)
	short	  TX1;
	short	  TY1;
	short	  TX2;
	short	  TY2;
	short	  TX3;
	short	  TY3;
	short	  TX4;
	short	  TY4;
	// 影像座標(cm)
	float	  BX1;
	float	  BY1;
	float	  BX2;
	float	  BY2;
	float	  BX3;
	float	  BY3;
	float	  BX4;
	float	  BY4;
	// 校正區域世界座標(cm)
	short    CX1;
	short    CY1;
	short    CX2;
	short    CY2;
	short    CX3;
	short    CY3;
	short    CX4;
	short    CY4;
	// 影像頻道
	unsigned short    CamChan;
}Image_AXIS;

// 校正區域參數(線性規劃)
typedef struct {
	int MinY;
	int MinX;
	int MaxY;
	int MaxX;
	double m[6]; //6個邊界的斜率陣列 //8CH AVM
	double c[6]; //6個邊界方程式的常數 //8CH AVM
}Zone_Para;

// 視點轉換參數結構 T:校正點世界座標(cm) B:校正點影像座標(pixel)
typedef struct {
	// 世界座標(cm)
	short	  TX1;
	short	  TY1;
	short	  TX2;
	short	  TY2;
	short	  TX3;
	short	  TY3;
	short	  TX4;
	short	  TY4;
	short	  TX5;
	short	  TY5;
	short	  TX6;
	short	  TY6;
	// 影像座標(cm)
	float	  BX1;
	float	  BY1;
	float	  BX2;
	float	  BY2;
	float	  BX3;
	float	  BY3;
	float	  BX4;
	float	  BY4;	
	float	  BX5;
	float	  BY5;
	float	  BX6;
	float	  BY6;
}LS_AXIS;

typedef struct {
	// Cubic Spline計算所需參數
	float 			gama;			// dY/dX. 小於1:原影像X方向縮小, 大於1:原影像Y方向縮小
	unsigned char 	Max_Num;		// 1+Max_Num+1 = 中心點+水平特徵點+斜角最遠點點數, Max_Num需<=18
	unsigned char 	Max_GRID_X;		// 水平方向最遠點格數 
	unsigned char 	Max_GRID_D;		// 斜角方向最遠點格數
	float 			CS_PIXEL[20];	// 手動設定特徵點對應之長度陣列(有效數量為1+Max_Num+1)
	float 			CS_GRID[20];	// 手動設定特徵點對應之GRID陣列(有效數量為1+Max_Num+1)
}CamUD_Para;  

typedef struct{
	unsigned short n;
	unsigned int s;
	float w;
	float h;
	float x;
	float y;
	unsigned short minX;
	unsigned short minY;
	unsigned short maxX;
	unsigned short maxY;
}Clumps;


typedef struct{
	unsigned short n;
	float x;
}Mpoint;

//八連通
typedef struct QNode
{
	int data;
	struct QNode *next;
}QNode;

typedef struct Queue
{
	struct QNode* first;
	struct QNode* last;
}Queue; 
//***********************//角偵測
void ORGtoUD(short X_ORG, short Y_ORG, float* X_UD, float* Y_UD, int Camch);
//static int NeighborDirection[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}}; //八連通

