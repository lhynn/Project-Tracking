#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#ifdef __cplusplus
extern "C"{
#endif
#include "videomanager.h"
#ifdef __cplusplus
};
#endif
#include "encoderVideo.h"
int main(int argc, char ** argv) {
	/*********************************lcd**************************/
	T_FbInfo        FbInfo;
	T_VideoBuf      VdBuffer;
	T_VideoDevice   VdDevice;
	int num;

	V4l2InitDevice("/dev/video0",&VdDevice);
	V4l2StartCapture(&VdDevice);
	FbFrambufferInit(&FbInfo);
	/*-------------------------fimc-----------------------------------------*/
	encoderVideo *VideoSave = new encoderVideo;
recreatefile:
	num = 0;		
	VideoSave->CreateVideoFile();
	while(1){
		V4l2ReadFarme(&VdDevice,&VdBuffer);
		FbDisplay(&FbInfo,VdBuffer.aucPixelDatas,10,10,720,576,32);
		num++;
		if(num == 400){
			VideoSave->SaveImageFile(VdBuffer.aucPixelDatas);
			VideoSave->WriteFileOver();
			goto recreatefile;
		}
		VideoSave->WriteOneFrame(VdBuffer.aucPixelDatas);
	}

	return 0;
}

