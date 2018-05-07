#ifndef  _MFCMANAGER_H_
#define _MFCMANAGER_H_

#include "SsbSipMfcApi.h"

typedef struct InitMfcSt
{
	unsigned char header[100];
	int headerSize;
	void *hOpen;
	int iWidth,iHeight;
	SSBSIP_MFC_ENC_H264_PARAM *EncParam;
	SSBSIP_MFC_ENC_INPUT_INFO InputInfoEnc;
	SSBSIP_MFC_ENC_OUTPUT_INFO OutPutInfoenc;
	SSBSIP_MFC_CODEC_TYPE CodecType;
}T_InitMfcSt,*PT_InitMfcSt;

SSBSIP_MFC_ERROR_CODE InitMyMfc(int w, int h, int qb);

int  MyMfcGetHeader(unsigned char** p);

void MyMfcGetInputBuf(void **Y,void **UV);

int  MyMfcEncode(void *input_buf,void **output_buf);

void ExitMyMfc(void);

#endif
