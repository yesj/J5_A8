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
//������
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
		// �һݰѼ�
	unsigned char AVMType;		// 0:�p��, 1:truck, 2:pickup, 3:Bus
	unsigned short CarWidth;	// ���e(cm)
	unsigned short CarLength;	// ����(cm)
	unsigned short X_Exten;		// X��VAVM��������
	unsigned short Y_Exten;		// Y��VAVM��������
	unsigned char PatternType;	// Pattern����
		// �p�⵲�G
	short AVM_Width;			// AVM �e��(pixel)
	short AVM_Length;			// AVM ����(pixel)
	int AVMStart;				// AVM �_�l��m(pixel)	
	// UD
		// �һݰѼ�
	unsigned char CAMType[6];//8ch AVM[4];	// ��v������
	unsigned char CAMRotate[6];//8ch AVM[4]; // �w�ˤ�V(����:0, �˸�:1)

	// Homography
		// �һݰѼ�	
	FeaturePts FPs[6];//8ch AVM[4];			// 6�կS�x�I�A�@�դ��I
		// �p�⵲�G
	unsigned int CamNum[4];//8ch AVM[4];		// AVM�U�϶����C����v���U���X�Ӽv���I	
	unsigned int CamNumWSG[4];//8ch AVM[4];	// ���k��v�����v���|�X�ϰ��I��
	
	unsigned char Off_or_Rear;
	// �_�X�����Ϊ̬O�ĦX����
	unsigned char Stich_or_Blend;
	// �ù��v����ܭץ����
	float MonitorRatio;
	// �ù���������e��
	unsigned short LeftShift;
	// �ù��k������e��
	unsigned short RightShift;
	unsigned char  wROI;
	unsigned char isManualCal;
}DisplayParameter;
// ���I�ഫ�ѼƵ��c T:�ե��I�@�ɮy��(cm) B:�ե��I�v���y��(pixel) C:�ե��ϰ�@�ɮy��(cm)
typedef struct {
	// �@�ɮy��(cm)
	short	  TX1;
	short	  TY1;
	short	  TX2;
	short	  TY2;
	short	  TX3;
	short	  TY3;
	short	  TX4;
	short	  TY4;
	// �v���y��(cm)
	float	  BX1;
	float	  BY1;
	float	  BX2;
	float	  BY2;
	float	  BX3;
	float	  BY3;
	float	  BX4;
	float	  BY4;
	// �ե��ϰ�@�ɮy��(cm)
	short    CX1;
	short    CY1;
	short    CX2;
	short    CY2;
	short    CX3;
	short    CY3;
	short    CX4;
	short    CY4;
	// �v���W�D
	unsigned short    CamChan;
}Image_AXIS;

// �ե��ϰ�Ѽ�(�u�ʳW��)
typedef struct {
	int MinY;
	int MinX;
	int MaxY;
	int MaxX;
	double m[6]; //6����ɪ��ײv�}�C //8CH AVM
	double c[6]; //6����ɤ�{�����`�� //8CH AVM
}Zone_Para;

// ���I�ഫ�ѼƵ��c T:�ե��I�@�ɮy��(cm) B:�ե��I�v���y��(pixel)
typedef struct {
	// �@�ɮy��(cm)
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
	// �v���y��(cm)
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
	// Cubic Spline�p��һݰѼ�
	float 			gama;			// dY/dX. �p��1:��v��X��V�Y�p, �j��1:��v��Y��V�Y�p
	unsigned char 	Max_Num;		// 1+Max_Num+1 = �����I+�����S�x�I+�ר��̻��I�I��, Max_Num��<=18
	unsigned char 	Max_GRID_X;		// ������V�̻��I��� 
	unsigned char 	Max_GRID_D;		// �ר���V�̻��I���
	float 			CS_PIXEL[20];	// ��ʳ]�w�S�x�I���������װ}�C(���ļƶq��1+Max_Num+1)
	float 			CS_GRID[20];	// ��ʳ]�w�S�x�I������GRID�}�C(���ļƶq��1+Max_Num+1)
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

//�K�s�q
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
//***********************//������
void ORGtoUD(short X_ORG, short Y_ORG, float* X_UD, float* Y_UD, int Camch);
//static int NeighborDirection[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}}; //�K�s�q

