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
#include "../openH264.h"
#ifdef __cplusplus
};
#endif

#include "FimcConvert.h"

#define MFC_INPUT_BUFFER_NUM_MAX            2
#define DEFAULT_MFC_INPUT_BUFFER_SIZE    1024 * 1024 * MFC_INPUT_BUFFER_NUM_MAX    /*DEFAULT_VIDEO_INPUT_BUFFER_SIZE*/

typedef struct _SEC_MFC_H264DEC_HANDLE
{
    void * hMFCHandle;
    void * pMFCStreamBuffer;
    void * pMFCStreamPhyBuffer;
    unsigned long  indexTimestamp;
    unsigned long  outputIndexTimestamp;
    int 	   bConfiguredMFC;
    int 	   bFlashPlayerMode;
    signed long   returnCodec;
} SEC_MFC_H264DEC_HANDLE;

typedef struct _MFC_DEC_INPUT_BUFFER
{
    void *PhyAddr;      // physical address
    void *VirAddr;      // virtual address
    int   bufferSize;   // input buffer alloc size
    int   dataSize;     // Data length
} MFC_DEC_INPUT_BUFFER;

int main(int argc, char ** argv) {
	SEC_MFC_H264DEC_HANDLE hMFCH264Handle;
	MFC_DEC_INPUT_BUFFER 	iPutBuffer;
	/********************************lcd**************************/
#if 0
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
	InitDevice();
	SSBSIP_MFC_ERROR_CODE ret;
	unsigned char *header;

	ret = InitMfcDevice(720,576,30);
	if(ret<0){
		printf("init mfc failed !!\n");
	}
	int headerSize = GetDateHeader(&header);
	int i;
	for(i = 0; i < headerSize;i++){
		printf("%x ",header[i]);	
	}
	printf("\n");
	fwrite(header,headerSize,1,outf);
	int count = 0;
	while(1){
		V4l2ReadFarme(&VdDevice,&VdBuffer);
		FbDisplay(&FbInfo,VdBuffer.aucPixelDatas,10,10,720,576,32);
	
		if(count == 2){
			memcpy(mine_fimc->inbuf_vir,VdBuffer.aucPixelDatas,720*576*4);
			mine_fimc->Convert();
			ouput_buf_size = Encode(mine_fimc->outbuf_vir,&ouput_buf);
			printf("-------------ouput_buf_size:%d\n",ouput_buf_size);
			fwrite(ouput_buf,ouput_buf_size,1,outf);count = 0;
		}
		count++;
	}
fclose(outf);
#endif 
//#if 0
	T_FbInfo        FbInfo;
	FbFrambufferInit(&FbInfo);
	PT_LoaderHandle loader;
	T_OneFarmeData  oneFarme; 
	int ret;
	void *pStreamBuffer;
	void *pStreamPhyBuffer;
	SSBSIP_MFC_DEC_OUTPUT_INFO outputinfo;

	loader = FileLoaderCreate("./out.264",24);
	if(loader == NULL){
                printf("FileLoaderCreate error.\n");
                return -1;
        }

	void *hMFCHandle       = NULL;
	void *inbuf_vir,*inbuf_phy;
	int  bufferSize,returnCodec,setConfVal = 0;	
	hMFCHandle = SsbSipMfcDecOpen();
	if (hMFCHandle == NULL) {
    		printf("error open Decode.\n");
		return 0;
	}
	hMFCH264Handle.hMFCHandle = hMFCHandle;
	inbuf_vir = SsbSipMfcDecGetInBuf(hMFCHandle, &inbuf_phy, DEFAULT_MFC_INPUT_BUFFER_SIZE / 2);
	
	 SsbSipMfcDecSetConfig(hMFCHandle,MFC_DEC_SETCONF_IMMEDIATELY_DISPLAY, &setConfVal);
	SSBSIP_MFC_CODEC_TYPE eCodecType = H264_DEC;
	returnCodec = SsbSipMfcDecInit(hMFCHandle, eCodecType,DEFAULT_MFC_INPUT_BUFFER_SIZE / 2);

#if 0
	s3c_mem_alloc m_addr;
	Mys3cMem m_memtool;
	 if(m_memtool.open() < 0){
                printf("faild to use sec-mem.\n");
                return -1;
        }
	m_addr.size = 720*576*2;
	if(m_memtool.aquire(&m_addr) < 0){
                printf("faild to aquire.\n");
                return -1;
        }
	inbuf_phy = (void *) m_addr.phy_addr;
       // inbuf_vir = (char *) m_addr.vir_addr;
	
    	inbuf_vir = SsbSipMfcDecGetInBuf(hOpen,&inbuf_phy,720*576*2);
/*
	SSBSIP_MFC_CODEC_TYPE inputype = H264_DEC;
	if(SsbSipMfcDecInit(hOpen,inputype,MAX_DECODER_INPUT_BUFFER_SIZE)!= MFC_RET_OK){
		printf("SsbSipMfcDecInit Failed\n");
		return -1;
	}
*/
#if 0

	while(1){
		ret = FileLoaderGetNextFarme(loader,&oneFarme);
		if(!ret)
			break;
		memcpy(inbuf_vir,oneFarme.DataAddr,oneFarme.DataLen);
		if(SsbSipMfcDecSetInBuf(hOpen,inbuf_phy,inbuf_vir,oneFarme.DataLen) != MFC_RET_OK){
			printf("SsbSipMfcEncInit Failed\n");
			return -1;
		}
		if(SsbSipMfcDecExe(hOpen,720*576*2) != MFC_RET_OK){
			printf("Decodeing Failed\n");
			return -1;
		}
		SsbSipMfcDecGetOutBuf(hOpen,&outputinfo);
		if(outputinfo.YVirAddr == NULL||outputinfo.CVirAddr == NULL)
		{
			printf("SsbSipMfcDecGetOutBuf Failed\n");
			return -1;
		}
	}
	FileLoaderDestory(loader);
//#endif 
#endif
#endif
	return 0;
}

