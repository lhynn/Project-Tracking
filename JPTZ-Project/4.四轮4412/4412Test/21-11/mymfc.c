#include "mfcmanager.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>


static T_InitMfcSt MfcInitDate;

SSBSIP_MFC_ERROR_CODE InitMyMfc(int w, int h, int qb){
	SSBSIP_MFC_ERROR_CODE ret;

	MfcInitDate.iWidth = w;
	MfcInitDate.iHeight = h;
	MfcInitDate.EncParam = (SSBSIP_MFC_ENC_H264_PARAM*)malloc(sizeof(SSBSIP_MFC_ENC_H264_PARAM));
	memset(MfcInitDate.EncParam, 0 , sizeof(SSBSIP_MFC_ENC_H264_PARAM));

	MfcInitDate.EncParam->codecType = H264_ENC;
	MfcInitDate.EncParam->SourceWidth = MfcInitDate.iWidth;
	MfcInitDate.EncParam->SourceHeight = MfcInitDate.iHeight;

	MfcInitDate.EncParam->FrameRate=30;//30//18
	MfcInitDate.EncParam->FrameQp=qb;
	MfcInitDate.EncParam->QSCodeMax=51;//0
	MfcInitDate.EncParam->QSCodeMin=10;//0


	MfcInitDate.EncParam->FrameQp_P = MfcInitDate.EncParam->FrameQp+1;
	MfcInitDate.EncParam->FrameQp_B = MfcInitDate.EncParam->FrameQp+3;

	MfcInitDate.hOpen = SsbSipMfcEncOpen();
	if(MfcInitDate.hOpen == NULL)
	{
		printf("SsbSipMfcEncOpen Failed\n");
		ret = MFC_RET_FAIL;
		return ret;
	}

	if(SsbSipMfcEncInit(MfcInitDate.hOpen,MfcInitDate.EncParam) != MFC_RET_OK)
	{
		printf("SsbSipMfcEncInit Failed\n");
		ret = MFC_RET_FAIL;
		goto out;
	}

	if(SsbSipMfcEncGetInBuf(MfcInitDate.hOpen, &MfcInitDate.InputInfoEnc) != MFC_RET_OK)
	{
		printf("SsbSipMfcEncGetInBuf Failed\n");
		ret = MFC_RET_FAIL;
		goto out;
	}

	ret=SsbSipMfcEncGetOutBuf(MfcInitDate.hOpen, &MfcInitDate.OutPutInfoenc);
	if(MfcInitDate.OutPutInfoenc.headerSize <= 0)
	{
		printf("Header Encoding Failed\n");
		ret = MFC_RET_FAIL;
		goto out;
	}
	MfcInitDate.headerSize = MfcInitDate.OutPutInfoenc.headerSize;
	memcpy(MfcInitDate.header,MfcInitDate.OutPutInfoenc.StrmVirAddr,MfcInitDate.headerSize);
	printf("MFC init success:: Yphy(0x%08x) Cphy(0x%08x)\n",
			MfcInitDate.InputInfoEnc.YPhyAddr,MfcInitDate.InputInfoEnc.CPhyAddr);
	return ret;
out:
	SsbSipMfcEncClose(MfcInitDate.hOpen);
	return ret;
}

int  MyMfcGetHeader(unsigned char** p){

	*p = MfcInitDate.header;
	return MfcInitDate.headerSize;
}

void MyMfcGetInputBuf(void **Y,void **UV){

	*Y = MfcInitDate.InputInfoEnc.YVirAddr;
	*UV = MfcInitDate.InputInfoEnc.CVirAddr;
}

int  MyMfcEncode(void *input_buf,void **output_buf){

	SSBSIP_MFC_ERROR_CODE err;
	printf("::MFC::encode start !!!\n");
	
	
	memcpy(MfcInitDate.InputInfoEnc.YVirAddr, (void*)input_buf, MfcInitDate.iWidth*MfcInitDate.iHeight);//¿camera¿¿¿Y¿¿
	memcpy(MfcInitDate.InputInfoEnc.CVirAddr,(void*)input_buf+(MfcInitDate.iWidth*MfcInitDate.iHeight), MfcInitDate.iWidth*MfcInitDate.iHeight/2);	
	err = SsbSipMfcEncSetInBuf(MfcInitDate.hOpen,&MfcInitDate.InputInfoEnc);
	if(err<0) {
		fprintf(stderr,"Error: SsbSipMfcEncSetInBuf. Code %d\n",err);
		return 0;
	}
	if(SsbSipMfcEncExe(MfcInitDate.hOpen) != MFC_RET_OK){
		printf("Encoding Failed\n");
		return 0;
	}
	SsbSipMfcEncGetOutBuf(MfcInitDate.hOpen, &MfcInitDate.OutPutInfoenc);
	if(MfcInitDate.OutPutInfoenc.StrmVirAddr == NULL)
	{
		printf("SsbSipMfcEncGetOutBuf Failed\n");
		return 0;
	}
	// memcpy(output_buf, output_info_enc.StrmVirAddr,  output_info_enc.dataSize);
	*output_buf = MfcInitDate.OutPutInfoenc.StrmVirAddr;
	return MfcInitDate.OutPutInfoenc.dataSize;
}

void ExitMyMfc(void){

	SsbSipMfcEncClose(MfcInitDate.hOpen);
}
