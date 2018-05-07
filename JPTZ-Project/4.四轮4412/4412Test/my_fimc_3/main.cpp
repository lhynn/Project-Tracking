#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#ifdef __cplusplus
extern "C"{
#endif
#include "mfcmanager.h"
#include "videomanager.h"
#ifdef __cplusplus
};
#endif

#include "FimcConvert.h"
int main(int argc, char ** argv) {
	/*********************************lcd**************************/
	T_FbInfo        FbInfo;
	T_VideoBuf      VdBuffer;
	T_VideoDevice   VdDevice;

	V4l2InitDevice("/dev/video0",&VdDevice);
	V4l2StartCapture(&VdDevice);
	FbFrambufferInit(&FbInfo);
	/*-------------------------fimc-----------------------------------------*/
	MyFimcConvert *mine_fimc = new MyFimcConvert();
	mine_fimc->open();
	//--------------------------MFC---------------------------------------//
	FILE *outf = 0;
	int ouput_buf_size;
	void *ouput_buf;

	outf = fopen("out.264", "wb");
	SSBSIP_MFC_ERROR_CODE ret;
	unsigned char *header;

	ret = InitMyMfc(720,576,30);
	if(ret<0){
		printf("init mfc failed !!\n");
	}
	int headerSize = MyMfcGetHeader(&header);
	fwrite(header,headerSize,1,outf);
	int count = 0;
	while(1){
		V4l2ReadFarme(&VdDevice,&VdBuffer);
		FbDisplay(&FbInfo,VdBuffer.aucPixelDatas,10,10,720,576,32);
	
		if(count == 2){
			memcpy(mine_fimc->inbuf_vir,VdBuffer.aucPixelDatas,720*576*4);
			mine_fimc->Convert();
			ouput_buf_size = MyMfcEncode(mine_fimc->outbuf_vir,&ouput_buf);
			printf("-------------ouput_buf_size:%d\n",ouput_buf_size);
			fwrite(ouput_buf,ouput_buf_size,1,outf);count = 0;
		}
		count++;
	}
	ExitMyMfc();
	return 0;
}

