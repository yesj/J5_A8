#include "AVM_Homography.h"
#include "All_define.h"
#include <string.h>
#include "AVM_Fun.h"

void Matrix_Solve()
{
	float zero=0.00000001, temp;  
    int x=0, y=0, j=0, i=0, m=8, n=9;
//    float T[3];
	
    // ��matrix
    while(x<9 && y<8)
    {  
        // �p�G�C�����s�A�h��M�i�H�������C  
        while(x<9 && matrix[y][x]<zero && matrix[y][x]>(-zero))
        {  
            j=y+1;  
            while(j<m && matrix[j][x]<zero && matrix[j][x]>(-zero))
            {j++;}    
            // ���泣���s�A���ܤU�@��  
            if(j >= m)
            { 
                x++; 
                continue; 
            }  
            // ���C�������s���C�A��C����  
            for(i=x; i<n; i++)
            {  
                temp=matrix[j][i];  
                matrix[j][i]=matrix[y][i];  
                matrix[y][i]=temp;  
            }  
            // �����������X�j��  
            break;  
        }  
        if(x>=9)
        {break;}  
        // �Ҧ��C�ȳ����H�C���A�C����(1)�Bz  
        for(i=n-1; i>x; i--)
        {matrix[y][i]/=matrix[y][x];}  
        matrix[y][x]=1;
        // ���h�W�U�C  
        for(j=0; j<m; j++)
        {  
            // ���L������C  
            if(j == y)
            {continue;}     
            // ������C���h��L�C  
            for(i=n-1; i>=x; i--)
            {matrix[j][i]-=matrix[y][i]*matrix[j][x];}  
        }  
        x++;  
        y++;  
    }  
    
    ////////// ���I�ഫ�x�}�p�⧹��
}


// �u�ƫ᪺AVM�d��A�|����v�����}�d��A�L�Ƕ����v
void Homographymapping(unsigned char * PtrOut, unsigned char * PtrImg, unsigned char * PtrImgUV,unsigned char * PtrX_f, unsigned char * PtrY_f, unsigned int * PtrAVMMap1, unsigned int * PtrPos, unsigned int CamNum,unsigned int * UVPos, unsigned int * PtrAVMUVMap1, unsigned char ch)
{
	int i;


	// ��X�v������
	unsigned char * PtrOutY;
	unsigned char * PtrOutUV;
	// AVM��X��m����
	PtrOutY = PtrOut;
	PtrOutUV = PtrOut+outPutWidth*outPutHeight;

	
	//#pragma MUST_ITERATE(2,,2);
	for(i=0; i<CamNum; i++)
	{	
		  *(PtrOutY+ *(PtrPos+i)) = *(PtrImg + *(PtrAVMMap1+i));
	}

	for(i=0; i<CamNum; i++)//UV
	{	 	
		if(*(UVPos+i))
		{
			memcpy((PtrOutUV+(*(UVPos+i))), (PtrImgUV+(*(PtrAVMUVMap1+i))),2); 
		}
	} 

}



