void Small_TableMaking_BUS(unsigned int* SMap, unsigned char* SMapXf, unsigned char* SMapYf);

void menuActiveDemo(unsigned char cmd,Menu menu,DisplayParameter DP);

void PointsCalculation_Homo();
void PointsSetting_LS2( LS_AXIS* AXIS, unsigned char PN1, unsigned char PN2, unsigned char PN3, unsigned char PN4);
void PointsSetting_1(Image_AXIS* Video_AXIS, unsigned char PN1, unsigned char PN2, unsigned char PN3, unsigned char PN4);

void PointsCalculation_LS_Homo( unsigned short* LS_X, unsigned short* LS_Y);

void PointsSetting();
void PointsSetting_2LS2(FeaturePtsFloat Fpts, LS_AXIS* AXIS);


void AVMLayout(unsigned short* points_X, unsigned short* points_Y, unsigned char AVMType, unsigned char Draw, unsigned char Pattern);
void ResetFpts(unsigned char DPpattern, unsigned char pattern, unsigned char DPAVMType, unsigned char MENUAVMType);

void UDPosGet(unsigned char CAMIndex, FeaturePts fPts, FeaturePtsFloat* NfPts);

void SplineDataPrepare(CamUD_Para *Para, int ch);
void seval (int n, float u, float *value, float x[], float y[], float b[], float c[], float d[], int *last);

int spline (int n, int end1, int end2, double slope1, double slope2, float x[], float y[], float b[], float c[], float d[], int *iflag);

void FPts_Setting_Homo(FeaturePtsFloat* fPts);

void LSMatrixMaking2(LS_AXIS AXIS);//

void MatrixMaking(Image_AXIS Video_AXIS);

void LSMatrixMaking_Homo(LS_AXIS AXIS);
void CalZone(Image_AXIS Video_AXIS, Zone_Para* Zone);

void Homography_TableMaking(char CamChan, unsigned int* MAP, unsigned char* Xf, unsigned char* Yf, unsigned int* LPos, unsigned int* CNum);

//由4個特徵點計算消失點
void UDVanishingGet(FeaturePtsFloat* NfPts, FeaturePtsFloat* VanishingfPts);

// 20150322 座標轉換
void Homography_Init(int CamIndex);
void BirdsEyeView2UD(float x_birds, float y_birds, float *x_ud, float *y_ud, int CamIndex);
void UDtoORG(float X_UD, float Y_UD, float* X_ORG, float* Y_ORG, int Camch);

 
void Matrix_Solve();

float SquareRootFloat(float number);

void gray_table();
void gray_weight();

void Homographymapping(unsigned char * PtrOut, unsigned char * PtrImg, unsigned char * PtrImgUV,unsigned char * PtrX_f, unsigned char * PtrY_f, unsigned int * PtrAVMMap1, unsigned int * PtrPos, unsigned int CamNum ,unsigned int * UVPos, unsigned int * PtrAVMUVMap1,unsigned char ch);

int Corner(CornerPts *CPts,int Camera,int Threshold,int X1 ,int Y1 ,int X2 ,int Y2, unsigned char* Image2 );
void AVM2UDPosGet(unsigned char CAMIndex, double AVMx, double AVMy, float* ORGx, float* ORGy);
void ChangPoints();
