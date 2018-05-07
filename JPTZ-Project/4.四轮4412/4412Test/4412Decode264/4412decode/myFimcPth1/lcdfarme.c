#include "lcdfarme.h"
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <linux/fb.h>

int FbFrambufferInit(PT_FbInfo ptFbInfo)
{
    struct fb_var_screeninfo vinfo;
    ptFbInfo->iFd = open(FDPATH,O_RDWR);
    if (ptFbInfo->iFd < 0) {
		printf("function:<%s>:line-%d\n",__FUNCTION__,__LINE__);
		return -1;
    }
    if (ioctl(ptFbInfo->iFd, FBIOGET_VSCREENINFO, &vinfo)) {
        printf("Error reading variable information.\n");
        exit(1);
    }
    ptFbInfo->iWidth  = vinfo.xres;
    ptFbInfo->iHeight = vinfo.yres;
    ptFbInfo->iBpp	= vinfo.bits_per_pixel;
    ptFbInfo->iFbSize =  vinfo.xres*vinfo.yres*vinfo.bits_per_pixel/8;
//	printf("iWidth:%d iHeight:%d  iBpp: %d iFbSize:%d",ptFbInfo->iWidth,ptFbInfo->iHeight,ptFbInfo->iBpp,ptFbInfo->iFbSize);
//	fflush(NULL); 
	if ((ptFbInfo->MmapAddr = (unsigned char *)mmap(0, ptFbInfo->iFbSize,
            PROT_READ | PROT_WRITE, MAP_SHARED, ptFbInfo->iFd, 0)) < 0) {
		printf("function:<%s>:line-%d\n",__FUNCTION__,__LINE__);
        return -1;
    }

	return 0;
}
int FbDisplay(PT_FbInfo fbinfo,unsigned char *DataAddr,int startx,int starty,int Width,int Height,int iBpp)
{
	int offsetDst;
	int i;	
	int SrcLineLen  = Width * iBpp/8;
	int SrcStartLen = startx * fbinfo->iBpp/8;
	
	int DstLineLen  = fbinfo->iWidth * fbinfo->iBpp/8;
	int DstStartOff = SrcStartLen + starty * DstLineLen;

	if(iBpp != 32 || (starty + Height) > fbinfo->iHeight){
		printf("iBPP || Height || starty  argument error!.\n");
		return -1;
	}
	for(i = starty; i < Height + starty ; i++){
		memcpy(fbinfo->MmapAddr + DstStartOff,DataAddr,SrcLineLen);
		DstStartOff += DstLineLen;
		DataAddr    += SrcLineLen;
	}

	return 0;
}

int FbambufferExit(PT_FbInfo ptFbInfo){
	munmap(ptFbInfo->MmapAddr, ptFbInfo->iFbSize);	
	close(ptFbInfo->iFd);
	return 0;
}

