#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include "lcdfarme.h"
#include "videomanager.h"

int main(int argc,char **argv){
	T_VideoBuf	VdBuffer;
	T_VideoDevice 	VdDevice;
	T_FbInfo	FbInfo;

	V4l2InitDevice("/dev/video0",&VdDevice);
	V4l2StartCapture(&VdDevice);
	FbFrambufferInit(&FbInfo);
	
//	struct timeval tv;
//	tv.tv_sec  = 0;
//	tv.tv_usec = 10000;
	while(1){
//		select(0, NULL,NULL,NULL,&tv);

		V4l2ReadFarme(&VdDevice,&VdBuffer);
		FbDisplay(&FbInfo,VdBuffer.aucPixelDatas,10,10,720,576,32);	
		V4l2PutFarme(&VdDevice);
	}
	
	V4l2StopCapture(&VdDevice);
	V4l2ExitDevice (&VdDevice);
	FbambufferExit(&FbInfo);	
	return 0;
}


