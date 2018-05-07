#include "mmapfile.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>

#define max(x,y) (x > y)?(x):(y) 
#define min(x,y) (x < y)?(x):(y) 

typedef struct tagBITMAPFILEHEADER { /* bmfh */
	unsigned short bfType; 
	unsigned long  bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned long  bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER { /* bmih */
	unsigned long  biSize;
	unsigned long  biWidth;
	unsigned long  biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned long  biCompression;
	unsigned long  biSizeImage;
	unsigned long  biXPelsPerMeter;
	unsigned long  biYPelsPerMeter;
	unsigned long  biClrUsed;
	unsigned long  biClrImportant;
} BITMAPINFOHEADER;
void  RGB24ToYUV420(int Width,int Height,unsigned char* RgbBuffer,unsigned char*YuvBuffer)  
{   
	unsigned char* yuvBuf=YuvBuffer;//YUV空间  
	int nWidth=Width;  
	int nHeight=Height;  
	/////////////////////下面转换算法是网上查到的  
	int i, j;   
	unsigned char*bufY = yuvBuf;   
	unsigned char*bufU = yuvBuf + nWidth * nHeight;   
	unsigned char*bufV = bufU + (nWidth* nHeight* 1/4);   
	unsigned char*Y=bufY;  
	unsigned char*U=bufU;  
	unsigned char*V=bufV;  
	unsigned char*bufRGB;  
	unsigned char y, u, v, r, g, b,testu,testv;  
	
	for (j = 0; j<nHeight;j++)  
	{  
		bufRGB = RgbBuffer + nWidth * (nHeight - 1-j) * 3 ;   
		for (i = 0;i<nWidth;i++)  
		{  
			int pos = nWidth * i + j;  
			r= *(bufRGB++);  
			g = *(bufRGB++);  
			b = *(bufRGB++);  
			y =(unsigned char)(( 66 * r + 129 * g +  25 * b + 128) >>8) + 16;//16  
			v = (unsigned char)((-38 * r -  74 * g + 112 * b + 128) >>8) +128  ; //128            
			u = (unsigned char)((112 * r -  94 * g -  18 * b + 128) >> 8) + 128 ;  
			*(bufY++)=max(0,min(y, 255 ));  

			if (j%2==0&&i%2 ==0)  
			{  
				if (u>255)  
				{  
					u=255;  
				}  
				if (u<0)  
				{  
					u = 0;  
				}  
				*(bufU++) =u;  
				//存u分量  
			}  
			else  
			{  
				//存v分量  
				if (i%2==0)  
				{  
					if (v>255)  
					{  
						v = 255;  
					}  
					if (v<0)  
					{  
						v = 0;  
					}  
					*(bufV++) =v;  
				}  
			}  
		}  
	}  
}  

void GetPixelDatasFrmBMP(unsigned char *aFileHead,unsigned char *myBuffer)
{
	BITMAPFILEHEADER *ptBITMAPFILEHEADER;
	BITMAPINFOHEADER *ptBITMAPINFOHEADER;

	int iWidth;
	int iHeight;
	int iBMPBpp;
	int y;

	unsigned char *pucSrc;
	unsigned char *pucDest;
	int iLineWidthAlign;
	int iLineWidthReal;
	
	ptBITMAPFILEHEADER = (BITMAPFILEHEADER *)aFileHead;
	ptBITMAPINFOHEADER = (BITMAPINFOHEADER *)(aFileHead + sizeof(BITMAPFILEHEADER));

	iWidth = ptBITMAPINFOHEADER->biWidth;
	iHeight = ptBITMAPINFOHEADER->biHeight;
	iBMPBpp = ptBITMAPINFOHEADER->biBitCount;

	printf("%d--%d--%d.\n",iWidth,iHeight,iBMPBpp);
	if (iBMPBpp != 24){
		printf("bmp ibpp != 24.\n");
		return ;
	}
	iLineWidthReal = iWidth * iBMPBpp / 8;
	iLineWidthAlign = (iLineWidthReal + 3) & ~0x3;   /* 向4取整 */
		
	pucSrc = aFileHead + ptBITMAPFILEHEADER->bfOffBits;
	pucSrc = pucSrc + (iHeight - 1) * iLineWidthAlign;
	
	RGB24ToYUV420(736,576,pucSrc,myBuffer);
}
int MapFile(PT_FileMap ptFileMap){
	int iFd;
	FILE *tFp;
	struct stat tStat;

	/* 打开文件 */
	tFp = fopen("./pix2.bmp", "r+");
	if (tFp == NULL)
	{
		printf("can't open %s\n", ptFileMap->strFileName);
		return -1;
	}
	ptFileMap->tFp = tFp;
	iFd = fileno(tFp);

	fstat(iFd, &tStat);
	ptFileMap->iFileSize = tStat.st_size;
	ptFileMap->pucFileMapMem = (unsigned char *)mmap(NULL , tStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, iFd, 0);
	if (ptFileMap->pucFileMapMem == (unsigned char *)-1)
	{
		printf("mmap error!\n");
		return -1;
	}
	return 0;
}

void UnMapFile(PT_FileMap ptFileMap)
{
	munmap(ptFileMap->pucFileMapMem, ptFileMap->iFileSize);
	fclose(ptFileMap->tFp);
}
