#ifndef __LCDFARME_H__
#define __LCDFARME_H__

#define FDPATH "/dev/fb0"

typedef struct FbInfo{
	int iFd;
	int iWidth;
	int iHeight;
	int iBpp;
	int iFbSize;
	unsigned char *MmapAddr;
}T_FbInfo,*PT_FbInfo;
 
int FbFrambufferInit(PT_FbInfo ptFbInfo);

int FbDisplay(PT_FbInfo fbinfo,unsigned char *DataAddr,int startx,int starty,int Width,int Height,int iBpp);

int FbambufferExit(PT_FbInfo ptFbInfo);
#endif
