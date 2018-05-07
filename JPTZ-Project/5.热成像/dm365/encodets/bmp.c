#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/videodev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include "mymenugui.h"


struct file_header
{
  unsigned short bfType; // Picture tpye, must set to 19778
  int bfSize; // The file size in bytes
  int bfRev; // Reserved 
  int bfOffBits; // the offset from the beginning of the file to the bitmap data.
}__attribute__ ((packed)); 

struct info_header
{
  int biSize; // info_header's size in bytes
  int biWidth; // width in pixels
  int biHeight;//height in pixels
  short biPlanes; //the number of planes of the target device
  short biBitCount; //the number of bits per pixel
  int biCompression;//the type of compression
  int biSizeImage; //
  int biXPelsPerMeter;//usually set to zero
  int biYPelsPerMeter;//usually set to zero
  int biClrUsed;//the number of colors used in the bitmap
  int biClrImportant;
}__attribute__ ((packed)); 


#define BMP_WIDTH  736
#define BMP_HEIGHT 576

struct bmp_file
{
  struct file_header header;
  struct info_header info;
  unsigned char bits[BMP_WIDTH*BMP_HEIGHT*3];
}__attribute__ ((packed));



static void cvt_420p_to_rgb(int width, int height,unsigned char *src, unsigned char *dst)
{
	int r, g, b;
	int yy, uu, vv;
	int xoff, yoff;
	int numpix = BMP_WIDTH*BMP_HEIGHT;
	unsigned char *pout = dst + width*height*3;
	unsigned char* pyy  = src;
	unsigned char *puv = src + numpix;

  	for(yoff=0; yoff<height; yoff++)
	{
		pout = dst + width*(height-yoff-1)*3;  //point to the bmp line head

		for(xoff=0; xoff<width; xoff++)
		{	
			yy = *(pyy + yoff*BMP_WIDTH + xoff);
			uu = *(puv + (yoff/2)*BMP_WIDTH + (xoff/2)*2);
			vv = *(puv + (yoff/2)*BMP_WIDTH + (xoff/2)*2 + 1) ;

			uu -= 128;
			vv -= 128;

			r = yy + vv + ((vv*103)>>8);
			g = yy - ((uu*88)>>8) - ((vv*183)>>8);
			b = yy + uu + ((uu*198)>>8);

			if(r>255) { r = 255; }
			if(g>255) { g = 255; }
			if(b>255) { b = 255; }
			if(r<0) { r = 0;}
			if(g<0) { g = 0;}
			if(b<0) { b = 0;}

			*pout = (unsigned char)b;
			pout++;
			*pout = (unsigned char)g;
			pout++;
			*pout = (unsigned char)r;
			pout++;
		}
	}
}

int convert(unsigned char *imageptr, char* filename)
{
	struct bmp_file myfile;
	int fd;

	//printf("sizeof short is %d\n", sizeof(short));

	myfile.header.bfType = 19778;
	myfile.header.bfSize = sizeof(struct bmp_file);
	myfile.header.bfRev = 0;
	myfile.header.bfOffBits = 54;

	myfile.info.biSize = 0x28;
	myfile.info.biWidth = BMP_WIDTH;  //640;
	myfile.info.biHeight = BMP_HEIGHT;  //480;
	myfile.info.biPlanes = 1;
	myfile.info.biBitCount = 24;
	myfile.info.biCompression = 0;
	myfile.info.biSizeImage = 0;
	myfile.info.biClrUsed = 256*256*256;
	myfile.info.biClrImportant = 0;
	myfile.info.biXPelsPerMeter = 2048;
	myfile.info.biYPelsPerMeter = 2048;

	fd = fopen(filename, "w+");
	if(fd < 0)
	{
		printf("open file error!\n");
		return 0;
	}
	   
	cvt_420p_to_rgb(BMP_WIDTH, BMP_HEIGHT, imageptr, myfile.bits);
	fwrite(&myfile, 1, sizeof(struct bmp_file), fd); //write bmp head
	fclose(fd);  
	return 0;
}

#define min(x,y) ((x)>(y)?(y):(x))
#define max(x,y) ((x)>(y)?(x):(y))

void cvt_rgb_to_420p(int width, int height,unsigned char *src, unsigned char *dst)
{
	int r, g, b;
	int yy, uu, vv;
	int xoff, yoff;
	int numpix = BMP_WIDTH*BMP_HEIGHT;
	unsigned char *pout = NULL;//src + width*height*3;
	
	//unsigned char* pyy  = dst;
	//unsigned char *puv = dst + numpix;

  	for(yoff=0; yoff<height; yoff++)
	{
		pout = src + width*(height-yoff-1)*3;  //point to the bmp line head

		for(xoff=0; xoff<width; xoff++)
		{	
			b=*(pout++);
			g=*(pout++);
			r=*(pout++);
			*(dst + yoff*BMP_WIDTH + xoff) =(unsigned char)(( 66 * r + 129 * g +  25 * b + 128) >>8) + 16;//16
			*(dst +numpix+ (yoff/2)*BMP_WIDTH + (xoff/2)*2) = (unsigned char)((-38 * r -  74 * g + 112 * b + 128) >>8) +128  ; //128          
			*(dst +numpix+ (yoff/2)*BMP_WIDTH + (xoff/2)*2 + 1) = (unsigned char)((112 * r -  94 * g -  18 * b + 128) >> 8) + 128 ;	
		}
	}
}

void RGB24ToYUV420(int Width,int Height,unsigned char * RgbBuffer,unsigned char *YuvBuffer)
{ 
	unsigned char * yuvBuf=YuvBuffer;//YUV空间
	int nWidth=Width;
	int nHeight=Height;
	/////////////////////下面转换算法是网上查到的
	int i, j; 
	unsigned char *bufY = yuvBuf; 
	unsigned char *bufU = yuvBuf + nWidth * nHeight; 
	unsigned char *bufV = bufU + (nWidth* nHeight* 1/4); 
	unsigned char *Y=bufY;
	unsigned char *U=bufU;
	unsigned char *V=bufV;
	unsigned char *bufRGB;
	unsigned char y, u, v, r, g, b,testu,testv;
	if (NULL==RgbBuffer)
	{
	   return ;
	}
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



int bmp_resole(unsigned char *imageptr, char* filename)
{
	int bmp_fd;
	struct bmp_file myfile;
	struct stat tStat;
	unsigned char *FileMem;
	unsigned char *rgb;
	unsigned char str_name[30]="/home/root/media/";
	strcat(str_name,filename);
	bmp_fd = open(str_name, O_RDONLY);
	if (0 > bmp_fd)
	{
		printf("can't open text file %s\n", filename);
		return -1;
	}

	if(fstat(bmp_fd, &tStat))
	{
		printf("can't get fstat\n");
		return -1;
	}
	printf("tStat.st_size:%d\n",tStat.st_size);
	FileMem = (unsigned char *)mmap(NULL , tStat.st_size, PROT_READ, MAP_SHARED, bmp_fd, 0);
	if (FileMem == (unsigned char *)-1)
	{
		printf("can't mmap for text file\n");
		return -1;
	}
	rgb=FileMem+tStat.st_size-BMP_WIDTH*BMP_HEIGHT*3;
	//RGB24ToYUV420(BMP_WIDTH,BMP_HEIGHT,rgb,imageptr);
	
	cvt_rgb_to_420p(BMP_WIDTH, BMP_HEIGHT,rgb, imageptr);
	ShowPicString(imageptr,filename);
	return 0;
}


