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


typedef struct MyMfcOpr
{
	char				  *name;
	SSBSIP_MFC_ERROR_CODE (*initMfc)	(int w, int h, int qb);
	int					  (*getHeader)	(unsigned char** p);
	void				  (*getInputBuf)(void **Y,void **UV);
	int					  (*encode)		(void *input_buf,void **output_buf);
	void				  (*exitMfc)	(void);

	struct MyMfcOpr *Next;
}T_MyMfcOpr,*PT_MyMfcOpr;


void InitDevice(void);
SSBSIP_MFC_ERROR_CODE InitMfcDevice(int w, int h, int qb);
int                   GetDateHeader(unsigned char** p);
void                  GetInputBuf(void **Y,void **UV);
int                   Encode(void *input_buf,void **output_buf);
void                  ExitMfc(void);


void RegisterMfcDevice(PT_MyMfcOpr pt_yMfcOpr);

#endif
