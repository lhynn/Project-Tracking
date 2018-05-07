#ifndef _ENCODERVIDEO_H__
#define _ENCODERVIDEO_H__
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
#include "mfcmanager.h"
#ifdef __cplusplus
}
#endif

#include "FimcConvert.h"
class encoderVideo{
public:
	/*初始化FIMC*/
  	encoderVideo();  								 
	~encoderVideo();	

	/*创建视频文件*/
	void CreateVideoFile();	

	/*保存一帧到图片文件*/
	void SaveImageFile(unsigned char *Buffer);	

	/*写一帧到音频文件*/
	void WriteOneFrame(unsigned char *Buffer);	

	/*第一音频文件结束*/
	void WriteFileOver();
private:
	MyFimcConvert *mine_fimc;
	SSBSIP_MFC_ERROR_CODE ret;
	char    bmpfilestr[64];
	int  	count;
	char 	outputfile[64];
	int  	ouput_buf_size;
	int  	headerSize;
	FILE 	*outf;
	FILE 	*B_fd;
	void    *ouput_buf;
	unsigned char *header;
};

#endif
