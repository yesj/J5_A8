#include "AVM_Homography.h"
#include "All_define.h"
#include <string.h>
#include "AVM_Fun.h"

void Matrix_Solve()
{
	float zero=0.00000001, temp;  
    int x=0, y=0, j=0, i=0, m=8, n=9;
//    float T[3];
	
    // 解matrix
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
    
    ////////// 視點轉換矩陣計算完畢
}


// 優化後的AVM查表，四隻攝影機分開查表，無灰階補償
void Homographymapping(unsigned char * PtrOut, unsigned char * PtrImg, unsigned char * PtrImgUV,unsigned char * PtrX_f, unsigned char * PtrY_f, unsigned int * PtrAVMMap1, unsigned int * PtrPos, unsigned int CamNum,unsigned int * UVPos, unsigned int * PtrAVMUVMap1, unsigned char ch)
{
	int i;


	// 輸出影像指標
	unsigned char * PtrOutY;
	unsigned char * PtrOutUV;
	// AVM輸出位置平移
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



