#define outPutWidth (736)
#define outPutHeight (480)

//Car type
//#define WISH



extern FeaturePts featurePts[4];//8ch AVM[4];            	// �C�@����v������/�K�Ӽv���S�x�I
extern unsigned short VLines;
extern unsigned char* AllImages1[4];//[5];            // ������J�v������: video input*4 + car*1 8CH
extern unsigned char* AllImagesUV[4];//[5];            // ������J�v������: video input*4 + car*1 8CH
//extern unsigned char aroundview[1280*720*2];     // ��X�v���x�}
extern unsigned char* aroundview;     // ��X�v���x�}
extern unsigned char* aroundviewuv;     // ��X�v���x�}
extern unsigned short corner_X[16];   	 // �@�ɮy�Ф��Ҧ��S�x�I��X�y��
extern unsigned short corner_Y[16];	 // �@�ɮy�Ф��Ҧ��S�x�I��Y�y��
extern unsigned short points_X[16];	// 8CH AVM[20];    // �@�ɮy�Ф��Ҧ��S�x�I��X�y��
extern unsigned short points_Y[16];	// 8CH AVM[20];    // �@�ɮy�Ф��Ҧ��S�x�I��Y�y��
extern Image_AXIS Video_AXIS[16];//8ch avm[20];           	// �ŧi�v���y�а}�C���c�A�̦h�i�NAVM����20�϶�
extern LS_AXIS Homo_AXIS[4];		//[4];  8CH AVM				 	// �O���C����v����(Tx,Ty)��(Bx,By)==> least square solution

extern unsigned int  CamNum[4]; //8ch AVM  	// ����AVM�϶��C�@����v���@���X�Ӽv���I
extern FeaturePtsFloat fPtsFloat[4];//8ch AVM[4];        	// �C�@����v������/�K�Ӽv���S�x�I(�B�I��)
extern CamUD_Para UD_Para; // For New Model
extern float 	CS_b[20];		// CubicSpline�U�Ϭq�����Ѽ�b
extern float 	CS_c[20];		// CubicSpline�U�Ϭq�����Ѽ�c
extern float 	CS_d[20];		// CubicSpline�U�Ϭq�����Ѽ�d 
extern unsigned short Camera_UD_Parameter[4][25];  
extern LS_AXIS Homo_AXIS[4];		//[4];  8CH AVM				 	// �O���C����v����(Tx,Ty)��(Bx,By)==> least square solution


     // AVM�M�g��
extern unsigned int AXIS_mapping1[outPutWidth*outPutHeight*1]; 	// �����ഫ�᪺�v���}�C
extern unsigned char AVMX_f[outPutWidth*outPutHeight*1];       	// �����ഫ���u�ʤ����v���� X
extern unsigned char AVMY_f[outPutWidth*outPutHeight*1];       	// �����ഫ���u�ʤ����v���� Y
extern unsigned int  LeftPos[outPutWidth*outPutHeight*1];      	// AVM �d��ɡA�C�@���I�baroundview����m
extern unsigned int UVPos[outPutWidth*outPutHeight*1];

extern float matrix[9][9];                  	// homography�p��x�}
extern float HH[3][3];							// homography�ഫ�x�}
extern unsigned int AXISUV_mapping1[outPutWidth*outPutHeight*1]; 	// �����ഫ�᪺�v���}�C

extern int Corners[6];	
extern CornerPts cornerPts[6];			 //���I�y��

//20150321 �y���ഫ
extern float hh1[3][3];
extern float hh2[3][3];
extern float hh3[3][3];
extern float hh4[3][3];

