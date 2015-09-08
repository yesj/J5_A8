#include "AVM_Homography.h"
#include "All_define.h"
#include "AVM_Fun.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "model.h"

int CubicSpline_last=0;			// 加速Cubic Spline計算得值時所需變數  
int iflag=0;					// Cubic Spline計算過程是否正常指標


   // 1.2 車長車寬和長度比例，計算特徵點世界座標(pixel)
void PointsCalculation_Homo()
{
	   int X_right, X_left, Y_up, Y_down;
   float fX_right, fX_left, fY_up, fY_down;
	//要畫的影像大小CH0
    points_X[0]=1;      		 points_Y[0]=1;
    points_X[1]=outPutWidth -1;  points_Y[1]=1;
    points_X[2]=1;   			 points_Y[2]=outPutHeight -1;        
    points_X[3]=outPutWidth -1;  points_Y[3]=outPutHeight -1;

	//要畫的影像大小CH1
    points_X[4]=1;      		points_Y[4]=1;
    points_X[5]=outPutWidth-1;  points_Y[5]=1;
    points_X[6]=1;   			points_Y[6]=outPutHeight -1;        
    points_X[7]=outPutWidth-1;  points_Y[7]=outPutHeight -1;

	//要畫的影像大小CH2
    points_X[8]=1;      		points_Y[8]=1;
    points_X[9]=outPutWidth-1;  points_Y[9]=1;
    points_X[10]=1;   			points_Y[10]=outPutHeight-1;        
    points_X[11]=outPutWidth-1; points_Y[11]=outPutHeight-1;

	//要畫的影像大小CH3
    points_X[12]=1;      		points_Y[12]=1;
    points_X[13]=outPutWidth-1; points_Y[13]=1;
    points_X[14]=1;   			points_Y[14]=outPutHeight-1;        
    points_X[15]=outPutWidth-1; points_Y[15]=outPutHeight-1;


    X_right=(2*xyextend+carwidth)/2+125;
    X_left =(2*xyextend+carwidth)/2-125;
    Y_up  =xyextend-100;
    Y_down=xyextend-30;

    fX_right=(outPutWidth*X_right)/(2*xyextend+carwidth);
    fX_left =(outPutWidth*X_left)/(2*xyextend+carwidth);
    fY_up   =(outPutHeight*Y_up)/xyextend;
    fY_down =(outPutHeight*Y_down)/xyextend;

    X_right=fX_right+0.5f;
    X_left =fX_left +0.5f;
    Y_up   =fY_up   +0.5f;
    Y_down =fY_down +0.5f;



	//Oval model -pattern feature points are the same with 2D AVM's points
	//特徵點在空間中位置CH0
	//特徵點在空間中位置CH0(前)
	corner_X[0]=X_left; 			corner_Y[0]=Y_up;
	corner_X[1]=X_right;		 corner_Y[1]=Y_up;
	corner_X[2]=X_left; 		   corner_Y[2]=Y_down;
	corner_X[3]=X_right;		 corner_Y[3]=Y_down;
	
	//特徵點在空間中位置CH2(後)
	corner_X[8]=X_left; 		   corner_Y[8]=Y_up;
	corner_X[9]=X_right;		 corner_Y[9]=Y_up;
	corner_X[10]=X_left;		 corner_Y[10]=Y_down;
	corner_X[11]=X_right;		  corner_Y[11]=Y_down;
	
	 X_right=(2*xyextend+carlength)/2+125;
	 X_left =(2*xyextend+carlength)/2-125;
	 Y_up  =xyextend-100;
	 Y_down=xyextend-30;
	
	 fX_right=(outPutWidth*X_right)/(2*xyextend+carlength);
	 fX_left =(outPutWidth*X_left)/(2*xyextend+carlength);
	 fY_up	 =(outPutHeight*Y_up)/xyextend;
	 fY_down =(outPutHeight*Y_down)/xyextend;
	
	 X_right=fX_right+0.5f;
	 X_left =fX_left +0.5f;
	 Y_up	=fY_up	 +0.5f;
	 Y_down =fY_down +0.5f;
	
	
	//特徵點在空間中位置CH1(右)
	corner_X[4]=X_left; 		   corner_Y[4]=Y_up;
	corner_X[5]=X_right;		 corner_Y[5]=Y_up;
	corner_X[6]=X_left; 		   corner_Y[6]=Y_down;
	corner_X[7]=X_right;		 corner_Y[7]=Y_down;
	
	//特徵點在空間中位置CH3(左)
	corner_X[12]=X_left;		 corner_Y[12]=Y_up;
	corner_X[13]=X_right;		  corner_Y[13]=Y_up;
	corner_X[14]=X_left;		 corner_Y[14]=Y_down;
	corner_X[15]=X_right;		  corner_Y[15]=Y_down;


}


  
void PointsSetting()
{   
   unsigned char i=0;

   for(i=0;i<4;i++)
   {PointsSetting_LS2( &Homo_AXIS[i], i*4+0, i*4+1, i*4+2, i*4+3);}
  
    for(i=0;i<4;i++)
    {PointsSetting_1(&Video_AXIS[i], i*4+0, i*4+1, i*4+2, i*4+3);}
 
}
	
			//**************************//角偵測
        // 1.3.2 依序設定特徵點世界座標(子函式)==>4個點(自動調校)
void PointsSetting_LS2( LS_AXIS* AXIS, unsigned char PN1, unsigned char PN2, unsigned char PN3, unsigned char PN4)
{
    AXIS->TY1 = corner_Y[PN1];
    AXIS->TX1 = corner_X[PN1];
    AXIS->TY2 = corner_Y[PN2];
    AXIS->TX2 = corner_X[PN2];
    AXIS->TY3 = corner_Y[PN3];
    AXIS->TX3 = corner_X[PN3];
    AXIS->TY4 = corner_Y[PN4];
    AXIS->TX4 = corner_X[PN4];
} 
      // 1.3.1 依序設定特徵點世界座標(子函式)
void PointsSetting_1(Image_AXIS* Video_AXIS, unsigned char PN1, unsigned char PN2, unsigned char PN3, unsigned char PN4)
{
    Video_AXIS->CY1 = Video_AXIS->TY1 = points_Y[PN1];
    Video_AXIS->CX1 = Video_AXIS->TX1 = points_X[PN1];
    Video_AXIS->CY2 = Video_AXIS->TY2 = points_Y[PN2];
    Video_AXIS->CX2 = Video_AXIS->TX2 = points_X[PN2];
    Video_AXIS->CY3 = Video_AXIS->TY3 = points_Y[PN3];
    Video_AXIS->CX3 = Video_AXIS->TX3 = points_X[PN3];
    Video_AXIS->CY4 = Video_AXIS->TY4 = points_Y[PN4];
    Video_AXIS->CX4 = Video_AXIS->TX4 = points_X[PN4];
}




// 準備Cubic Spline所需資料
void SplineDataPrepare(CamUD_Para *Para, int ch)
{
	int i=0;

	Para->Max_Num = Camera_UD_Parameter[ch][20];
	Para->Max_GRID_X = Camera_UD_Parameter[ch][21];
	Para->Max_GRID_D = Camera_UD_Parameter[ch][22];
	Para->gama = (float)(Camera_UD_Parameter[ch][24])/(float)(Camera_UD_Parameter[ch][23]);  //BEN



	// Center:1:(Max_Num-1)
	for(i=0; i<Para->Max_Num; i++)
	{
		// 特徵點長度
		Para->CS_PIXEL[i] = Camera_UD_Parameter[ch][i];
		// 反扭曲轉換距離
		Para->CS_GRID[i] = i;
	}
	// Max_Num
	Para->CS_PIXEL[Para->Max_Num] = Camera_UD_Parameter[ch][Para->Max_Num];
		// 反扭曲轉換距離
	Para->CS_GRID[Para->Max_Num] = Para->Max_GRID_X;
//CubicSpline_PIXEL[Max_Num+1] = SquareRootFloat( (CSPoints[Max_Num+1].x-CSPoints[0].x)*(CSPoints[Max_Num+1].x-CSPoints[0].x)
//								    			   +(CSPoints[Max_Num+1].y-CSPoints[0].y)*(CSPoints[Max_Num+1].y-CSPoints[0].y) );
	// Max_Num+1
	Para->CS_PIXEL[Para->Max_Num+1] = Camera_UD_Parameter[ch][Para->Max_Num+1];
		// 反嶆授鉥奎Z離
	Para->CS_GRID[Para->Max_Num+1] = Para->Max_GRID_D;

}

// // cubic spline4: 計算曲線參數
int spline(int n, int end1, int end2, double slope1, double slope2, float x[], float y[], float b[], float c[], float d[], int *iflag)
{ 
	int    nm1, ib, i;
	double t;
	int    ascend;

	nm1    = n - 1;
	*iflag = 0;

	if (n < 2)
	{  
		*iflag = 1;
		goto LeaveSpline;
	}

	ascend = 1;
	for (i = 1; i < n; ++i) if (x[i] <= x[i-1]) ascend = 0;
	if (!ascend)
	{
		*iflag = 2;
		goto LeaveSpline;
	}

	if (n >= 3)
	{    
		d[0] = x[1] - x[0];
		c[1] = (y[1] - y[0]) / d[0];
		for (i = 1; i < nm1; ++i)
		{
			d[i]   = x[i+1] - x[i];
			b[i]   = 2.0 * (d[i-1] + d[i]);
			c[i+1] = (y[i+1] - y[i]) / d[i];
			c[i]   = c[i+1] - c[i];
		}

		/* ---- Default End conditions
		Third derivatives at x[0] and x[n-1] obtained
		from divided differences  */
		b[0]   = -d[0];
		b[nm1] = -d[n-2];
		c[0]   = 0.0;
		c[nm1] = 0.0;
		if (n != 3)
		{
			c[0]   = c[2] / (x[3] - x[1]) - c[1] / (x[2] - x[0]);
			c[nm1] = c[n-2] / (x[nm1] - x[n-3]) - c[n-3] / (x[n-2] - x[n-4]);
			c[0]   = c[0] * d[0] * d[0] / (x[3] - x[0]);
			c[nm1] = -c[nm1] * d[n-2] * d[n-2] / (x[nm1] - x[n-4]);
		}

		/* Alternative end conditions -- known slopes */
		if (end1 == 1)
		{
			b[0] = 2.0 * (x[1] - x[0]);
			c[0] = (y[1] - y[0]) / (x[1] - x[0]) - slope1;
		}
		if (end2 == 1)
		{
			b[nm1] = 2.0 * (x[nm1] - x[n-2]);
			c[nm1] = slope2 - (y[nm1] - y[n-2]) / (x[nm1] - x[n-2]);
		}

		/* Forward elimination */
		for (i = 1; i < n; ++i)
		{
			t    = d[i-1] / b[i-1];
			b[i] = b[i] - t * d[i-1];
			c[i] = c[i] - t * c[i-1];
		}

		/* Back substitution */
		c[nm1] = c[nm1] / b[nm1];
		for (ib = 0; ib < nm1; ++ib)
		{
			i    = n - ib - 2;
			c[i] = (c[i] - d[i] * c[i+1]) / b[i];
		}

		/* c[i] is now the sigma[i] of the text */

		/* Compute the polynomial coefficients */
		b[nm1] = (y[nm1] - y[n-2]) / d[n-2] + d[n-2] * (c[n-2] + 2.0 * c[nm1]);
		for (i = 0; i < nm1; ++i)
		{
			b[i] = (y[i+1] - y[i]) / d[i] - d[i] * (c[i+1] + 2.0 * c[i]);
			d[i] = (c[i+1] - c[i]) / d[i];
			c[i] = 3.0 * c[i];
		}
		c[nm1] = 3.0 * c[nm1];
		d[nm1] = d[n-2];

	}  /* at least quadratic */
	else  /* if n < 3 */
	{  /* linear segment only  */
		b[0] = (y[1] - y[0]) / (x[1] - x[0]);
		c[0] = 0.0;
		d[0] = 0.0;
		b[1] = b[0];
		c[1] = 0.0;
		d[1] = 0.0;
	}

LeaveSpline:
	return 0;
} 
    // 2.3.1 快速計算平方根
float SquareRootFloat(float number)    
{         
     int j; // loop counter
     int i;         
     float x, y;       
     const float f = 1.5F;         
     x = number * 0.5F;         
     y = number;         
     int * ii = (int*)&y; //hkc    
     i=*ii;    
     i = 0x5f3759df - (i>>1);          
     float * yy = (float*)&i;
     y=*yy;
         
     //#pragma MUST_ITERATE(1,4,4)
     for(j=0; j<4; j++) // 迭代越多次 精度越高  
     {y*=(f-(x*y*y));}
     
     return (number * y); 
} 
// cubic spline3: 計算 x=u 時的數值
void seval (int n, float u, float *value, float x[], float y[], float b[], float c[], float d[], int *last)
{
	int   i, j, k;
	float w;

	i = *last;
	if (i >= n-1) i = 0;
	if (i < 0)  i = 0;

	if ((x[i] > u) || (x[i+1] < u)) // 如果u不在上次的區間內
	{  /* ---- perform a binary search ---- */
		i = 0;
		j = n;
		do
		{
			k = (i + j) / 2;         /* split the domain to search */
			if (u < x[k])  j = k;    /* move the upper bound */
			if (u >= x[k]) i = k;    /* move the lower bound */
		}                          /* there are no more segments to search */
		while (j > i+1);
	}
	*last = i;

	/* ---- Evaluate the spline ---- */
	w = u - x[i];
	*value = y[i] + w * (b[i] + w * (c[i] + w * d[i]));
}
// Cubic Spline1: 正向映射(ORG->UD), 內建gama補償
void ORGtoUD(short X_ORG, short Y_ORG, float* X_UD, float* Y_UD, int Camch)
{
    float X, Y;
	float gXdis, gYdis;
	float gORG_r;
	float UD_R;
	int Cx,Cy;


	Cx=720 /2;
	Cy=480 /2; 

	X = X_ORG;
    Y = Y_ORG;
	gXdis = (X-Cx)     *(UD_Para.gama<1 ?   (UD_Para.gama):1);	// X-distance is gama compensated
	gYdis = (Y-Cy)*(UD_Para.gama>1 ? (1/UD_Para.gama):1); // Y-distance is gama compensated
	
	// Cubic Spline: 正向映射	
	gORG_r = SquareRootFloat(gXdis*gXdis + gYdis*gYdis);	// distance from center of gama ORG
		// 正向映射: 利用cubic spline內差將gama_ORG的長度轉換到UD的sub-grid
	seval((int)(UD_Para.Max_Num+2), gORG_r, &UD_R, UD_Para.CS_PIXEL, UD_Para.CS_GRID, CS_b, CS_c, CS_d, &CubicSpline_last);
	
	//          方向分量
	*X_UD = UD_R *((gXdis)/gORG_r);
	*Y_UD = UD_R *((gYdis)/gORG_r);       
}

    // 3.5 將手動設定特徵點的由原始影像座標正向映射到反扭曲影像的座標
void UDPosGet(unsigned char CAMIndex, FeaturePts fPts, FeaturePtsFloat* NfPts)
{
    float X, Y;
    float NUD_X, NUD_Y;
    int i;  
      // 準備Cubic Spline 所需資料
	SplineDataPrepare(&UD_Para,CAMIndex);
	// (正向映射需要)計算 Gama_Org(pixel) to UD(grid) 時的數值
	memset(CS_b, 0, sizeof(CS_b));				
	memset(CS_c, 0, sizeof(CS_c));
	memset(CS_d, 0, sizeof(CS_d));
	spline ((int)(UD_Para.Max_Num+2), 0, 0, 0, 0, UD_Para.CS_PIXEL, UD_Para.CS_GRID,  CS_b, CS_c, CS_d, &iflag);
	// ********注意:UD_Para.CS_PIXEL和UD_Para.CS_GRID的位置與UDtoORG()用的相反*********
	
    for(i=0;i<4;i++)
    {   
		X=fPts.x[i];
        Y=fPts.y[i];

		// Cubic Spline1: 正向映射(ORG->UD), 內建gama補償
		ORGtoUD(X, Y, &NUD_X, &NUD_Y,0);

        NfPts->x[i] = NUD_X;
        NfPts->y[i] = NUD_Y;
    }   
}


// Cubic Spline2: 反向映射(UD->ORG_float), 內建gama補償
void UDtoORG(float X_UD, float Y_UD, float* X_ORG, float* Y_ORG, int Camch)
{
    float r;                   
    float i,j;                                 
    float tempA;
	int Cx,Cy;
	

     i=X_UD;
	 j=Y_UD;
	// Cubic Spline: 反向映射				
		// 計算反扭曲影像上目標點鴾中萿漯曮?sub-grid)
	r = SquareRootFloat(i*i + j*j);
	
		// 反向映射: 利用cubic spline內差將sub-grid轉成gama_ORG的長度(tempA)
	seval((int)(UD_Para.Max_Num+2), r, &tempA, UD_Para.CS_GRID, UD_Para.CS_PIXEL, CS_b, CS_c,CS_d, &CubicSpline_last);

	Cx=720/2;
	Cy=480 /2; 
	    //               方向分量                  gama 補償
	*X_ORG = Cx+      tempA*(i/r) * (UD_Para.gama<1 ? 1/UD_Para.gama:1);
    *Y_ORG = Cy+      tempA*(j/r) * (UD_Para.gama>1 ? UD_Para.gama:1);	 
	
}
//==== 3 視角轉換

        // 3.1.1 特徵點分配子函式(影像座標)
void PointsSetting_2(FeaturePtsFloat Fpts, Image_AXIS* Video_AXIS, unsigned char PN1, unsigned char PN2, unsigned char PN3, unsigned char PN4)
{
    Video_AXIS->BY1 = Fpts.y[PN1];
    Video_AXIS->BX1 = Fpts.x[PN1];
    Video_AXIS->BY2 = Fpts.y[PN2];
    Video_AXIS->BX2 = Fpts.x[PN2];
    Video_AXIS->BY3 = Fpts.y[PN3];
    Video_AXIS->BX3 = Fpts.x[PN3];
    Video_AXIS->BY4 = Fpts.y[PN4];
    Video_AXIS->BX4 = Fpts.x[PN4];
}



    // 3.1 填入特徵點影陵y標
void FPts_Setting_Homo(FeaturePtsFloat* fPts) //bird view image calibration point setting for 3D AVM homography
{
	int i=0;
	
	for (i=0;i < 4	;i++)
	{
   		PointsSetting_2LS2(fPts[i], &Homo_AXIS[i]); 
	}
}

void PointsSetting_2LS2(FeaturePtsFloat Fpts, LS_AXIS* AXIS)
{
	AXIS->BY1 = Fpts.y[0];
	AXIS->BX1 = Fpts.x[0];
	AXIS->BY2 = Fpts.y[1];
	AXIS->BX2 = Fpts.x[1];
	AXIS->BY3 = Fpts.y[2];
	AXIS->BX3 = Fpts.x[2];
	AXIS->BY4 = Fpts.y[3];
	AXIS->BX4 = Fpts.x[3];
} 

	
	//**************************//角偵測
    // 3.2 matrix設定：輸入座標結構, 輸出接下來要計算的轉換矩陣
void LSMatrixMaking_Homo(LS_AXIS AXIS)// 東奕  改為八個
{

    int i, j, k;    
    float m16x8[8][8], m8x16[8][8], m16x1[8];

    // matrix起始化
    for (i=0; i<8; i++)
    {
        for (j=0; j<9; j++)
        {matrix[i][j]=0;}
    }
    /// Set the matrix to all zeros.
    for (i=0;i<8;i++)
    {
        for (j=0;j<8;j++)
        {
           m16x8[i][j] = 0;
           m8x16[j][i] = 0;
        }
    }

    m16x8[ 0][0]=m16x8[ 1][3]=(float)AXIS.TY1;
    m16x8[ 2][0]=m16x8[ 3][3]=(float)AXIS.TY2;
    m16x8[ 4][0]=m16x8[ 5][3]=(float)AXIS.TY3;
    m16x8[ 6][0]=m16x8[ 7][3]=(float)AXIS.TY4;
 

    m16x8[ 0][1]=m16x8[ 1][4]=(float)AXIS.TX1;
    m16x8[ 2][1]=m16x8[ 3][4]=(float)AXIS.TX2;
    m16x8[ 4][1]=m16x8[ 5][4]=(float)AXIS.TX3;
    m16x8[ 6][1]=m16x8[ 7][4]=(float)AXIS.TX4;



    // pt 1
    m16x8[0][6]=-(float)((AXIS.BY1)*(float)(AXIS.TY1));
    m16x8[0][7]=-(float)((AXIS.BY1)*(float)(AXIS.TX1));
    m16x8[0][2]=1;
    m16x8[1][6]=-(float)((AXIS.BX1)*(float)(AXIS.TY1));
    m16x8[1][7]=-(float)((AXIS.BX1)*(float)(AXIS.TX1));
    m16x8[1][5]=1;
    // pt 2
    m16x8[2][6]=-(float)((AXIS.BY2)*(float)(AXIS.TY2));
    m16x8[2][7]=-(float)((AXIS.BY2)*(float)(AXIS.TX2));
    m16x8[2][2]=1;
    m16x8[3][6]=-(float)((AXIS.BX2)*(float)(AXIS.TY2));
    m16x8[3][7]=-(float)((AXIS.BX2)*(float)(AXIS.TX2));
    m16x8[3][5]=1;
    // pt 3
    m16x8[4][6]=-(float)((AXIS.BY3)*(float)(AXIS.TY3));
    m16x8[4][7]=-(float)((AXIS.BY3)*(float)(AXIS.TX3));
    m16x8[4][2]=1;
    m16x8[5][6]=-(float)((AXIS.BX3)*(float)(AXIS.TY3));
    m16x8[5][7]=-(float)((AXIS.BX3)*(float)(AXIS.TX3));
    m16x8[5][5]=1;
    // pt 4
    m16x8[6][6]=-(float)((AXIS.BY4)*(float)(AXIS.TY4));
    m16x8[6][7]=-(float)((AXIS.BY4)*(float)(AXIS.TX4));
    m16x8[6][2]=1;
    m16x8[7][6]=-(float)((AXIS.BX4)*(float)(AXIS.TY4));
    m16x8[7][7]=-(float)((AXIS.BX4)*(float)(AXIS.TX4));
    m16x8[7][5]=1;

    // 轉置矩陣 m8x12
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {m8x16[j][i] = m16x8[i][j];}
    }

    // array multiply
    for(i = 0; i<8; i++)
    {
        for(j = 0; j<8; j++)
        {
            for(k= 0; k<8; k++)
            {matrix[i][j]= matrix[i][j]+(m8x16[i][k]*m16x8[k][j]);}
        } 
    }

    /// Setting the right hand side m12x1.
    m16x1[ 0] = AXIS.BY1;
    m16x1[ 1] = AXIS.BX1;
    m16x1[ 2] = AXIS.BY2;
    m16x1[ 3] = AXIS.BX2;
    m16x1[ 4] = AXIS.BY3;
    m16x1[ 5] = AXIS.BX3;
    m16x1[ 6] = AXIS.BY4;
    m16x1[ 7] = AXIS.BX4;


    /// Construct the right hand side.
    for(i =0; i<8; i++)
    {
       for(k = 0; k<8; k++)
       {matrix[i][8] += m8x16[i][k]*m16x1[k];}
    }
}
	//**************************//角偵測




    // 3.4.1 計算 AVM 映射表---含攝影機翻轉功能
void Homography_TableMaking(char CamChan, unsigned int* MAP, unsigned char* Xf, unsigned char* Yf, unsigned int* LPos, unsigned int* CNum)
{   
		 // 視點轉換
		 float zero=0.00000001, temp;  
		 int x=0, y=0, j=0, i=0, m=8, n=9;
		 float HH[3][3];
		 //float T1[3], T[3];
		 //unsigned char tempx_f,tempy_f;
		 //unsigned short tempx,tempy;	 // 目前的點所計算出來的扭曲校正後的座標
		 //float tempXX,tempYY;	   // 計算過程中需要的

 
    ////////////// 高司消去法 ==> 計算視點轉換的轉換矩陣

    while(x<9 && y<8)
    {  
        // 如果列首為零，則找尋可以互換的列  
        while(x<9 && matrix[y][x]<zero && matrix[y][x]>(-zero))
        {  
            j=y+1;  
            while(j<m && matrix[j][x]<zero && matrix[j][x]>(-zero))
            {j++;}    
            // 此行都為零，移至下一行  
            if(j >= m)
            { 
                x++; 
                continue; 
            }  
            // 找到列首不為零的列，兩列互換  
            for(i=x; i<n; i++)
            {  
                temp=matrix[j][i];  
                matrix[j][i]=matrix[y][i];  
                matrix[y][i]=temp;  
            }  
            // 互換結束跳出迴圈  
            break;  
        }  
        if(x>=9)
        {break;}  
        // 所有列值都除以列首，列首為(1)處z  
        for(i=n-1; i>x; i--)
        {matrix[y][i]/=matrix[y][x];}  
        matrix[y][x]=1;
        // 消去上下列  
        for(j=0; j<m; j++)
        {  
            // 跳過選取的列  
            if(j == y)
            {continue;}     
            // 選取的列消去其他列  
            for(i=n-1; i>=x; i--)
            {matrix[j][i]-=matrix[y][i]*matrix[j][x];}  
        }  
        x++;  
        y++;  
    }  
    // HH = 鉥垢x陣
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {HH[i][j] = matrix[i*3+j][8];}
    }
    HH[2][2] = 1;   
    ////////// 視點轉換矩陣計算完畢

int a00 = HH[0][0] * 1000000;
int a01 = HH[0][1] * 1000000;
int a02 = HH[0][2] * 1000000;
int a10 = HH[1][0] * 1000000;
int a11 = HH[1][1] * 1000000;
int a12 = HH[1][2] * 1000000;
int a20 = HH[2][0] * 1000000;
int a21 = HH[2][1] * 1000000;
int a22 = HH[2][2] * 1000000;
/*
printf("●(￣^￣ )●$ tableHH[0][1]: %d \n", a00);
printf("●(￣^￣ )●$ tableHH[0][1]: %d \n", a01);
printf("●(￣^￣ )●$ tableHH[0][2]: %d \n", a02);
printf("●(￣^￣ )●$ tableHH[1][0]: %d \n", a10);
printf("●(￣^￣ )●$ tableHH[1][1]: %d \n", a11);
printf("●(￣^￣ )●$ tableHH[1][2]: %d \n", a12);
printf("●(￣^￣ )●$ tableHH[2][0]: %d \n", a20);
printf("●(￣^￣ )●$ tableHH[2][1]: %d \n", a21);
printf("●(￣^￣ )●$ tableHH[2][2]: %d \n", a22);
*/
	// 準備Cubic Spline 所需資料
		SplineDataPrepare(&UD_Para,0);
	    // (反向映射需要)計算 UD(gird) to Gama_Org(pixel) 時的數值
		memset(CS_b, 0, sizeof(CS_b));				
		memset(CS_c, 0, sizeof(CS_c));
		memset(CS_d, 0, sizeof(CS_d));
		spline (UD_Para.Max_Num+2, 0, 0, 0, 0, UD_Para.CS_GRID, UD_Para.CS_PIXEL, CS_b, CS_c, CS_d, &iflag);



    
    // 製作mapping table
    /*for (i=0; i<=outPutHeight; i+=2)        // Y 軸
    {
		for (j=0; j<=outPutWidth; j+=2)    // X 軸
		{   // 從校正區域中以方形逐點掃描，需落在校正線性規畫區域內才作計算    
 
                //   以視點轉換矩陣計算反向映射點座標
                T[0] = HH[0][0]*i + HH[0][1]*j + HH[0][2];
                T[1] = HH[1][0]*i + HH[1][1]*j + HH[1][2];
                T[2] = HH[2][0]*i + HH[2][1]*j + HH[2][2];          
                T1[0] = (T[0]/T[2]);        // 視點轉換後影像座標對釣憼曲校正後影像座標的 Y (float)
                T1[1] = (T[1]/T[2]);        // 視點轉換後影像座標對應到扭曲校正後影像座標的 X (float)
                
				if (i == 0 && j == 468)
					{
					a00=T1[1] * 1000000;
					a01=T1[0] * 1000000;
					Vps_printf("●(￣^￣ )●$  tempXX: %d tempYY: %d \n", a00, a01);					
					}
				if (i == 0 && j == 812)
					{
					a00=T1[1] * 1000000;
					a01=T1[0] * 1000000;
					Vps_printf("●(￣^￣ )●$  tempXX: %d tempYY: %d \n", a00, a01);
					}
				if (i == 244 && j == 468)
					{
					a00=T1[1] * 1000000;
					a01=T1[0] * 1000000;
					Vps_printf("●(￣^￣ )●$  tempXX: %d tempYY: %d \n", a00, a01);
					}
				if (i == 244 && j == 812)
					{
					a00=T1[1] * 1000000;
					a01=T1[0] * 1000000;
					Vps_printf("●(￣^￣ )●$  tempXX: %d tempYY: %d \n", a00, a01);
					}
    
                ///////// 扭曲校正      
	
				// Cubic Spline2: 反向映射(UD->ORG_float), 內建gama補償
				UDtoORG(T1[1], T1[0], &tempXX, &tempYY,0);
				if (i == 0 && j == 468)
					{
					a00=tempXX;
					a01=tempYY;
					Vps_printf("●(￣^￣ )●$  tempXX: %d tempYY: %d \n", a00, a01);
					}
				if (i == 0 && j == 812)
					{
					a00=tempXX;
					a01=tempYY;
					Vps_printf("●(￣^￣ )●$  tempXX: %d tempYY: %d \n", a00, a01);
					}
				if (i == 244 && j == 468)
					{
					a00=tempXX;
					a01=tempYY;
					Vps_printf("●(￣^￣ )●$  tempXX: %d tempYY: %d \n", a00, a01);
					}
				if (i == 244 && j == 812)
					{
					a00=tempXX;
					a01=tempYY;
					Vps_printf("●(￣^￣ )●$  tempXX: %d tempYY: %d \n", a00, a01);
					}
				
                tempx=(unsigned short)tempXX;
                tempy=(unsigned short)tempYY;       


                if (tempx>10 && tempx<outPutWidth-10 && tempy>10 && tempy<outPutHeight-10)    // 確定取點為原影像範圍內
                {   
                    tempx_f=(tempXX - tempx)*256; // tempx 的小數點
                    tempy_f=(tempYY - tempy)*256; // tempy 的小數點
                    
                    MAP [CNum[0]+CNum[1]+CNum[2]+CNum[3]] = tempx + tempy*outPutWidth;
                    Xf  [CNum[0]+CNum[1]+CNum[2]+CNum[3]] = tempx_f;
                    Yf  [CNum[0]+CNum[1]+CNum[2]+CNum[3]] = tempy_f;
                    LPos[CNum[0]+CNum[1]+CNum[2]+CNum[3]] = 0+(j/2+i*outPutWidth/2);
					//Vps_printf("●(￣^￣ )●$ MAP[%d]: \n", CNum[0]+CNum[1]+CNum[2]+CNum[3]);
					if(((i % 4)==0) && ((j % 4) ==0))
					{
			           	UVPos[CNum[0]+CNum[1]+CNum[2]+CNum[3]] = ((j/2)+(i/2)*outPutWidth/2);//AVMStart+(j+(i/2)*1280);	
						if((tempy%2)!=0)
						{tempy--;}
						if((tempx%2)!=0)
					 	{tempx--;}	 
						AXISUV_mapping1[CNum[0]+CNum[1]+CNum[2]+CNum[3]] = tempx + ((tempy/2))*outPutWidth;				
					}
				    else
					{UVPos[CNum[0]+CNum[1]+CNum[2]+CNum[3]] = 0;}	
			 
                    CNum[CamChan]++;
					//Vps_printf("●(￣^￣ )●$ CNum[%d]: %d \n", CamChan, CNum[CamChan]);
                }
       }
    }*/
}

void Homography_Init(int CamIndex)
{   
		 // 視點轉換
		 float zero=0.00000001, temp;  
		 int x=0, y=0, j=0, i=0, m=8, n=9;
		 float HH[3][3];//, T[3];
		 //float T1[3];
		 //unsigned char tempx_f,tempy_f;
		 //unsigned short tempx,tempy;	 // 目前的點所計算出來的扭曲校正後的座標
		 //float tempXX,tempYY;	   // 計算過程中需要的

 
    ////////////// 高司消去法 ==> 計算視點轉換的轉換矩陣

    while(x<9 && y<8)
    {  
        // 如果列首為零，則找尋可以互換的列  
        while(x<9 && matrix[y][x]<zero && matrix[y][x]>(-zero))
        {  
            j=y+1;  
            while(j<m && matrix[j][x]<zero && matrix[j][x]>(-zero))
            {j++;}    
            // 此行都為零，移至下一行  
            if(j >= m)
            { 
                x++; 
                continue; 
            }  
            // 找到列首不為零的列，兩列互換  
            for(i=x; i<n; i++)
            {  
                temp=matrix[j][i];  
                matrix[j][i]=matrix[y][i];  
                matrix[y][i]=temp;  
            }  
            // 互換結束跳出迴圈  
            break;  
        }  
        if(x>=9)
        {break;}  
        // 所有列值都除以列首，列首為(1)處z  
        for(i=n-1; i>x; i--)
        {matrix[y][i]/=matrix[y][x];}  
        matrix[y][x]=1;
        // 消去上下列  
        for(j=0; j<m; j++)
        {  
            // 跳過選取的列  
            if(j == y)
            {continue;}     
            // 選取的列消去其他列  
            for(i=n-1; i>=x; i--)
            {matrix[j][i]-=matrix[y][i]*matrix[j][x];}  
        }  
        x++;  
        y++;  
    }  
    // HH = 鉥垢x陣
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
           HH[i][j] = matrix[i*3+j][8];
        }
    }
    HH[2][2] = 1;   
    ////////// 視點轉換矩陣計算完畢

	switch (CamIndex)
	{
	case 0:		
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
				hh1[i][j] = HH[i][j];		
		break;
		
	case 1:		
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
				hh2[i][j] = HH[i][j];		
		break;
		
	case 2:		
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
				hh3[i][j] = HH[i][j];		
		break;
		
	case 3:		
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
				hh4[i][j] = HH[i][j];		
		break;
	}


	// 準備Cubic Spline 所需資料
		SplineDataPrepare(&UD_Para,0);
	    // (反向映射需要)計算 UD(gird) to Gama_Org(pixel) 時的數值
		memset(CS_b, 0, sizeof(CS_b));				
		memset(CS_c, 0, sizeof(CS_c));
		memset(CS_d, 0, sizeof(CS_d));
		spline (UD_Para.Max_Num+2, 0, 0, 0, 0, UD_Para.CS_GRID, UD_Para.CS_PIXEL, CS_b, CS_c, CS_d, &iflag);



/*
    // 製作mapping table
    for (i=0; i<=outPutHeight; i+=2)        // Y 軸
    {
		for (j=0; j<=outPutWidth; j+=2)    // X 軸
		{   // 從校正區域中以方形逐點掃描，需落在校正線性規畫區域內才作計算    
 
                //   以視點轉換矩陣計算反向映射點座標
                T[0] = HH[0][0]*i + HH[0][1]*j + HH[0][2];
                T[1] = HH[1][0]*i + HH[1][1]*j + HH[1][2];
                T[2] = HH[2][0]*i + HH[2][1]*j + HH[2][2];          
                T1[0] = (T[0]/T[2]);        // 視點轉換後影像座標對釣憼曲校正後影像座標的 Y (float)
                T1[1] = (T[1]/T[2]);        // 視點轉換後影像座標對應到扭曲校正後影像座標的 X (float)

                ///////// 扭曲校正      
	
				// Cubic Spline2: 反向映射(UD->ORG_float), 內建gama補償
				UDtoORG(T1[1], T1[0], &tempXX, &tempYY,0);				
                tempx=(unsigned short)tempXX;
                tempy=(unsigned short)tempYY;       


                if (tempx>10 && tempx<outPutWidth-10 && tempy>10 && tempy<outPutHeight-10)    // 確定取點為原影像範圍內
                {   
                    tempx_f=(tempXX - tempx)*256; // tempx 的小數點
                    tempy_f=(tempYY - tempy)*256; // tempy 的小數點
                    
                    MAP [CNum[0]+CNum[1]+CNum[2]+CNum[3]] = tempx + tempy*outPutWidth;
                    Xf  [CNum[0]+CNum[1]+CNum[2]+CNum[3]] = tempx_f;
                    Yf  [CNum[0]+CNum[1]+CNum[2]+CNum[3]] = tempy_f;
                    LPos[CNum[0]+CNum[1]+CNum[2]+CNum[3]] = 0+(j/2+i*outPutWidth/2);
					//Vps_printf("●(￣^￣ )●$ MAP[%d]: \n", CNum[0]+CNum[1]+CNum[2]+CNum[3]);
					if(((i % 4)==0) && ((j % 4) ==0))
					{
			           	UVPos[CNum[0]+CNum[1]+CNum[2]+CNum[3]] = ((j/2)+(i/2)*outPutWidth/2);//AVMStart+(j+(i/2)*1280);	
						if((tempy%2)!=0)
						{tempy--;}
						if((tempx%2)!=0)
					 	{tempx--;}	 
						AXISUV_mapping1[CNum[0]+CNum[1]+CNum[2]+CNum[3]] = tempx + ((tempy/2))*outPutWidth;				
					}
				    else
					{UVPos[CNum[0]+CNum[1]+CNum[2]+CNum[3]] = 0;}	
			 
                    CNum[CamChan]++;
					//Vps_printf("●(￣^￣ )●$ CNum[%d]: %d \n", CamChan, CNum[CamChan]);
                }
       }
    }
*/
}

void BirdsEyeView2UD(float x_birds, float y_birds, float *x_ud, float *y_ud, int CamIndex)
{
	float HH[3][3], T[3];
	float T1[3];

	int i, j;
	
	switch (CamIndex)
	{
	case 0:		
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
				HH[i][j] = hh1[i][j];		
		break;

	case 1:		
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
				HH[i][j] = hh2[i][j];		
		break;

	case 2:		
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
				HH[i][j] = hh3[i][j];		
		break;

	case 3:		
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
				HH[i][j] = hh4[i][j];		
		break;
	default:
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
				HH[i][j] = 0;		
			
		break;		
	}
/*
	int a00 = HH[0][0] * 1000000;
	int a01 = HH[0][1] * 1000000;
	int a02 = HH[0][2] * 1000000;
	int a10 = HH[1][0] * 1000000;
	int a11 = HH[1][1] * 1000000;
	int a12 = HH[1][2] * 1000000;
	int a20 = HH[2][0] * 1000000;
	int a21 = HH[2][1] * 1000000;
	int a22 = HH[2][2] * 1000000;
	
	Vps_printf("●(￣^￣ )●$ b2uHH[0][1]: %d \n", a00);
	Vps_printf("●(￣^￣ )●$ b2uHH[0][1]: %d \n", a01);
	Vps_printf("●(￣^￣ )●$ b2uHH[0][2]: %d \n", a02);
	Vps_printf("●(￣^￣ )●$ b2uHH[1][0]: %d \n", a10);
	Vps_printf("●(￣^￣ )●$ b2uHH[1][1]: %d \n", a11);
	Vps_printf("●(￣^￣ )●$ b2uHH[1][2]: %d \n", a12);
	Vps_printf("●(￣^￣ )●$ b2uHH[2][0]: %d \n", a20);
	Vps_printf("●(￣^￣ )●$ b2uHH[2][1]: %d \n", a21);
	Vps_printf("●(￣^￣ )●$ b2uHH[2][2]: %d \n", a22);
*/

	//	 以視點轉換矩陣計算反向映射點座標
	//a00 = x_birds * 1000000;
	//a01 = y_birds * 1000000;
	//Vps_printf("●(￣^￣ )●$ x_birds: %d \n", a00);
	//Vps_printf("●(￣^￣ )●$ y_birds: %d \n", a01);
	
	T[0] = HH[0][0]*y_birds + HH[0][1]*x_birds + HH[0][2];
	T[1] = HH[1][0]*y_birds + HH[1][1]*x_birds + HH[1][2];
	T[2] = HH[2][0]*y_birds + HH[2][1]*x_birds + HH[2][2];			
	T1[0] = (T[0]/T[2]);		// 視點轉換後影像座標對釣憼曲校正後影像座標的 Y (float)
	T1[1] = (T[1]/T[2]);		// 視點轉換後影像座標對應到扭曲校正後影像座標的 X (float)

	//a00 = T1[1] * 1000000;
	//a01 = T1[0] * 1000000;	
	//Vps_printf("●(￣^￣ )●$ b2ux: %d \n", a00);
	//Vps_printf("●(￣^￣ )●$ b2uy: %d \n", a01);

	
	*y_ud = T1[0];
	*x_ud = T1[1];

	//a00 = *x_ud;
	//a01 = *y_ud;	
	//Vps_printf("●(￣^￣ )●$ b2ux: %d \n", a00);
	//Vps_printf("●(￣^￣ )●$ b2uy: %d \n", a01);


	
	float tempXX,tempYY;
	UDtoORG(*x_ud, *y_ud, &tempXX, &tempYY,0);

	//a00 = tempXX;
	//a01 = tempYY;
	//Vps_printf("●(￣^￣ )●$ b2u2ORGx: %d \n", a00);
	//Vps_printf("●(￣^￣ )●$ b2u2ORGy: %d \n", a01);
	

	
}

