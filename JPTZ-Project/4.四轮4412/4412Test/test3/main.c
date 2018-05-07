#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include "lcdfarme.h"
#include "videomanager.h"
#include "picarr.h"
#include "demo.h"

int main(int argc,char **argv){
	T_VideoBuf	VdBuffer;
	T_VideoDevice 	VdDevice;
	T_FbInfo	FbInfo;

	V4l2InitDevice("/dev/video0",&VdDevice);
	V4l2StartCapture(&VdDevice);
	FbFrambufferInit(&FbInfo);
	pthread_t pth;

	pthread_create(&pth,NULL,PrintAllData,NULL);
	struct timeval tv;
	FbDisplay(&FbInfo,gImage_pic,480,860,240,160,32);

	while(1){
//		tv.tv_sec  = 0;
//		tv.tv_usec = 19000;
//		select(0, NULL,NULL,NULL,&tv);
		//usleep(19000);
		V4l2ReadFarme(&VdDevice,&VdBuffer);
		FbDisplay(&FbInfo,VdBuffer.aucPixelDatas,10,10,720,576,32);	
		gblIncFrames();
	}
	
	V4l2StopCapture(&VdDevice);
	V4l2ExitDevice (&VdDevice);
	FbambufferExit(&FbInfo);	
	pthread_join(pth,NULL);
	return 0;
}



