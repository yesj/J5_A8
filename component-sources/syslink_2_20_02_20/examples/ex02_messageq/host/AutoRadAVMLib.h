//void AutoRadAVMFun(unsigned char* VideoInY[6],unsigned char* VideoInUV[6],unsigned char* VideoOutY,unsigned char* VideoOutUV, char Firststart);
//void AutoRadAVMFun(unsigned char* VideoInY0,unsigned char* VideoInY1,unsigned char* VideoInY2,unsigned char* VideoInY3,unsigned char* VideoInY4,unsigned char* VideoInY5,unsigned char* VideoOutY);
void AutoRadHomoFun(unsigned char* VideoInY,unsigned char* VideoInUV,unsigned char* VideoOutY );


// 20150322 ®y¼ÐÂà´«
void AutoRadHomoInit(int CamIndex);
void AutoRadBirds2UD(float x_birds, float y_birds, float *x_ud, float *y_ud, int CamIndex);
void AutoRadUDtoORG(float X_UD, float Y_UD, float* X_ORG, float* Y_ORG, int Camch);
void AutoRadORGtoUD(short X_ORG, short Y_ORG, float* X_UD, float* Y_UD, int Camch);
void AutoRadGetPivot(float *x_ud, float *y_ud, int CamIndex);
void AutoRadGetVanishing(float *x_ud, float *y_ud, int CamIndex);
void AutoRadGetfeature(float *x_ud, float *y_ud, int CamIndex, int ID);




