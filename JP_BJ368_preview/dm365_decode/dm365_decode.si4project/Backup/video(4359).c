/*
 * video.c
 *
 * This source file has the implementations for the video thread
 * functions implemented for 'DVSDK encodedecode demo' on DM365 platform
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <xdc/std.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/osal/Memory.h>

#include <ti/sdo/dmai/Fifo.h>
#include <ti/sdo/dmai/Pause.h>
#include <ti/sdo/dmai/BufTab.h>
#include <ti/sdo/dmai/VideoStd.h>
#include <ti/sdo/dmai/BufferGfx.h>
#include <ti/sdo/dmai/Rendezvous.h>
#include <ti/sdo/dmai/ce/Vdec2.h>
#include <ti/sdo/dmai/ce/Venc1.h>

#include <ti/sdo/codecs/h264dec/ih264vdec.h>

#include "video.h"
#include "../demo.h"
#include "avilib.h"
#include "../ctrl.h"

#define CAPTURE_PIPE_SIZE       4  
#define DISPLAY_PIPE_SIZE       7
#define NUM_VIDEO_BUFS          CAPTURE_PIPE_SIZE + DISPLAY_PIPE_SIZE

/* The masks to use for knowing when a buffer is free */
#define CODEC_FREE              0x1
#define DISPLAY_FREE            0x2

#ifndef YUV_420SP
#define YUV_420SP 256
#endif 

#define VIDEOHEADER 12

extern pthread_mutex_t avmutex;
extern avi_t* avifile;
char  videobuf[100*1024]={0};
unsigned long frames;
unsigned int gl_videoend = 0;


/******************************************************************************
 * encodedecode
 ******************************************************************************/
static Int encodedecode(Vdec2_Handle hVd2,
                        Buffer_Handle hDumpBuf, Buffer_Handle hEncBuf,
                        Buffer_Handle hDecBuf, Fifo_Handle displayFifo)
{
    Buffer_Handle           hOutBuf, hFreeBuf;
    Int                     ret;
    BufferGfx_Dimensions    srcDim;
	Buffer_Handle	 tmp_Buffer;

    unsigned char* srcptr;
    unsigned char* dstptr;	

    /* Make sure the whole buffer is used for input */
    BufferGfx_resetDimensions(hDecBuf);
    srcptr = Buffer_getUserPtr(hDumpBuf); 
  	dstptr = Buffer_getUserPtr(hEncBuf);
    memcpy(dstptr, srcptr, Buffer_getNumBytesUsed(hDumpBuf));
    Buffer_setNumBytesUsed(hEncBuf, (Int32)Buffer_getNumBytesUsed(hDumpBuf));
	Buffer_setSize(hEncBuf, Buffer_getNumBytesUsed(hDumpBuf));
	if(srcptr[4]==0x67)
	{
	//	printf("first step : current frame is IDR %02X\r\n", srcptr[4]);
	}

 
    /* Decode the video buffer */
    ret = Vdec2_process(hVd2, hEncBuf, hDecBuf);
    if (ret != Dmai_EOK) 
    {
        ERR("Failed to decode video buffer\n");
        return FAILURE;
    }
 //   printf("\nDecode successful!\n\n\n");
    /* Send display frames to display thread */
    hOutBuf = Vdec2_getDisplayBuf(hVd2);
	
    while (hOutBuf){
        if (Fifo_put(displayFifo, hOutBuf) < 0) {
            ERR("Failed to send buffer to display thread\n");
            return FAILURE;
        }
        hOutBuf = Vdec2_getDisplayBuf(hVd2);
    }

    /* Free up released frames */
    hFreeBuf = Vdec2_getFreeBuf(hVd2);
    while (hFreeBuf) {
        Buffer_freeUseMask(hFreeBuf, CODEC_FREE);
        hFreeBuf = Vdec2_getFreeBuf(hVd2);
    }

    return SUCCESS;
}



/**************************************************\
*	解码初始化：
*		success：	ret 0
*		faild  :	ret -1		 
\**************************************************/
int init_decode(VideoEnv *envp){
	Void                   *status               = THREAD_SUCCESS;
	VIDDEC2_Params          defaultDecParams     = Vdec2_Params_DEFAULT;
	VIDDEC2_DynamicParams   defaultDecDynParams  = Vdec2_DynamicParams_DEFAULT;
	BufferGfx_Attrs         gfxAttrs             = BufferGfx_Attrs_DEFAULT;
	Buffer_Attrs            bAttrs               = Buffer_Attrs_DEFAULT;
	Vdec2_Handle            hVd2                 = NULL;
    Engine_Handle           hEngine              = NULL;
    Buffer_Handle           hEncBuf              = NULL;
    Buffer_Handle           hDumpBuf             = NULL;
    Int                     ret                  = Dmai_EOK;
	Buffer_Handle           hDispBuf; 
	VIDDEC2_Params         *decParams;
	VIDDEC2_DynamicParams  *decDynParams;
	ColorSpace_Type 		colorSpace = ColorSpace_YUV420PSEMI;
	IH264VDEC_Params 	    extnParams;
	
   	Int32                   nbDecVideoBufs;
    BufTab_Handle           hDecBufTab           = NULL;
    Buffer_Handle           hDecBuf              = NULL;
    Int32                   decBufSize;  
    Int32                   width, height; 

	/* Use supplied params if any, otherwise use defaults */
		decParams = envp->decParams ? envp->decParams : &defaultDecParams;
		decDynParams = envp->decDynParams ? envp->decDynParams :
											&defaultDecDynParams;
		width  = 720;
		height = 576;
		envp->imageWidth  = Dmai_roundUp(width, 32); 
		envp->imageHeight = height;
		gfxAttrs.colorSpace 	= colorSpace;
		gfxAttrs.dim.width		= envp->imageWidth;
		gfxAttrs.dim.height 	= envp->imageHeight;
		gfxAttrs.dim.lineLength =
				Dmai_roundUp(BufferGfx_calcLineLength(gfxAttrs.dim.width,
									   gfxAttrs.colorSpace), 32);							 
		/* Open the codec engine */
		hEngine = Engine_open(envp->engineName, NULL, NULL);
		if (hEngine == NULL){
			ERR("Failed to open codec engine %s\n", envp->engineName);
			return -1;
		}
	
		decParams->maxBitRate = 10485760;
		decParams->maxWidth   = envp->imageWidth;
		decParams->maxHeight  = Dmai_roundUp(envp->imageHeight, CODECHEIGHTALIGN);
		decParams->maxFrameRate = 25000;
		decParams->forceChromaFormat = XDM_YUV_420SP;
		
		if (!strcmp(envp->videoDecoder, "h264dec")) 
		{
			extnParams.displayDelay = 8;
			extnParams.levelLimit = 0;
			extnParams.disableHDVICPeveryFrame = 0;
			extnParams.inputDataMode = 1;
			extnParams.sliceFormat = 1;
			extnParams.frame_closedloop_flag = 1;
			decParams->size = sizeof(IH264VDEC_Params);
		}				
	
		extnParams.viddecParams = *decParams;
		decDynParams->displayWidth = 800;
	
		/* Create the video decoder */
		hVd2 = Vdec2_create(hEngine, envp->videoDecoder, (VIDDEC2_Params*)&extnParams, decDynParams);
		if (hVd2 == NULL){
			ERR("Failed to create video decoder: %s\n", envp->videoDecoder);
			return -1;
		}
		
		/* Which output buffer size does the decoder require? */
		decBufSize	   = Vdec2_getOutBufSize(hVd2);
		int encbufsize = Vdec2_getInBufSize(hVd2);
		
		/* Allocate buffer for encoded data */
		hEncBuf = Buffer_create(encbufsize, &bAttrs); 
		if( hEncBuf == NULL)  {
			ERR("Failed to allocate buffer for encoded data\n");
			return -1;
		}
	
		hDumpBuf = Buffer_create(encbufsize, &bAttrs); 
		if( hDumpBuf == NULL) {
			ERR("Failed to allocate buffer for encoded data\n");
			return -1;
		}
		
		/* A hDecBufTab buffer can be owned either by codec or display */
		gfxAttrs.bAttrs.useMask = CODEC_FREE | DISPLAY_FREE;
		nbDecVideoBufs = DISPLAY_PIPE_SIZE;
		
		/* Allocate buffers for decoded output. It is not possible to use
		 * the buffers from the encoder input BufTab because buffers have 
		 * different sizes.
		 */
		hDecBufTab = BufTab_create(nbDecVideoBufs, decBufSize, BufferGfx_getBufferAttrs(&gfxAttrs));
		if (hDecBufTab == NULL) {
			ERR("Failed to create BufTab for display pipe\n");
			return -1;
		}
		/* The decoder is going to use this BufTab for output buffers */
		Vdec2_setBufTab(hVd2, hDecBufTab);
	return 0;
}


/******************************************************************************
 * videoThrFxn
 ******************************************************************************/
Void *videoThrFxn_01(Void *arg)
{
    VideoEnv               *envp                 = (VideoEnv *) arg;
    Void                   *status               = THREAD_SUCCESS;
    VIDDEC2_Params          defaultDecParams     = Vdec2_Params_DEFAULT;
    VIDDEC2_DynamicParams   defaultDecDynParams  = Vdec2_DynamicParams_DEFAULT;
    BufferGfx_Attrs         gfxAttrs             = BufferGfx_Attrs_DEFAULT;
    Buffer_Attrs            bAttrs               = Buffer_Attrs_DEFAULT;
    Vdec2_Handle            hVd2                 = NULL;
    Engine_Handle           hEngine              = NULL;
    Buffer_Handle           hEncBuf              = NULL;
    Buffer_Handle           hDumpBuf             = NULL;
    Int                     ret                  = Dmai_EOK;
    Buffer_Handle           hDispBuf; 
    VIDDEC2_Params         *decParams;
    VIDDEC2_DynamicParams  *decDynParams;
    Int                     fifoRet;
    Int                     bufIdx;
    ColorSpace_Type         colorSpace = ColorSpace_YUV420PSEMI;
    IH264VDEC_Params        extnParams;

    Int32                   nbDecVideoBufs;
    BufTab_Handle           hDecBufTab           = NULL;
    Buffer_Handle           hDecBuf              = NULL;
    Int32                   decBufSize;  
    Int32                   width, height;

	unsigned char* streamptr;
	Int streamlen;
	unsigned char* ptr;
	int i = 0;
/******************************************************/
	gl_videoend = 0;
	long framesize;
    int frame;
    int framew = 0;
    int frameh = 0;
    int framerate = 0.0f;
	int iskeyframe;//if it's keyframe
/*******************************************************/
    /* Use supplied params if any, otherwise use defaults */
    decParams = envp->decParams ? envp->decParams : &defaultDecParams;
    decDynParams = envp->decDynParams ? envp->decDynParams :
                                        &defaultDecDynParams;
	width  = 720;
	height = 576;
    envp->imageWidth  = Dmai_roundUp(width, 32); 
    envp->imageHeight = height;
    gfxAttrs.colorSpace     = colorSpace;
    gfxAttrs.dim.width      = envp->imageWidth;
    gfxAttrs.dim.height     = envp->imageHeight;
    gfxAttrs.dim.lineLength =
            Dmai_roundUp(BufferGfx_calcLineLength(gfxAttrs.dim.width,
                                   gfxAttrs.colorSpace), 32);                            
  //printf("************** envp->imagewidth = %d\r\n", envp->imageWidth);
	/* Open the codec engine */
	hEngine = Engine_open(envp->engineName, NULL, NULL);
	if (hEngine == NULL){
		ERR("Failed to open codec engine %s\n", envp->engineName);
		cleanup(THREAD_FAILURE);
	}

	decParams->maxBitRate = 10485760;
	decParams->maxWidth   = envp->imageWidth;
	decParams->maxHeight  = Dmai_roundUp(envp->imageHeight, CODECHEIGHTALIGN);
	decParams->maxFrameRate = 25000;
	decParams->forceChromaFormat = XDM_YUV_420SP;
	
	if (!strcmp(envp->videoDecoder, "h264dec")) 
	{
		extnParams.displayDelay = 8;
		extnParams.levelLimit = 0;
		extnParams.disableHDVICPeveryFrame = 0;
		extnParams.inputDataMode = 1;
		extnParams.sliceFormat = 1;
		extnParams.frame_closedloop_flag = 1;
		decParams->size = sizeof(IH264VDEC_Params);
	}               

	extnParams.viddecParams = *decParams;
	decDynParams->displayWidth = 800;

	/* Create the video decoder */
	hVd2 = Vdec2_create(hEngine, envp->videoDecoder, (VIDDEC2_Params*)&extnParams, decDynParams);
	if (hVd2 == NULL){
		ERR("Failed to create video decoder: %s\n", envp->videoDecoder);
		cleanup(THREAD_FAILURE);
	}
	
	/* Which output buffer size does the decoder require? */
	decBufSize	   = Vdec2_getOutBufSize(hVd2);
	int encbufsize = Vdec2_getInBufSize(hVd2);
	
	/* Allocate buffer for encoded data */
	hEncBuf = Buffer_create(encbufsize, &bAttrs); 
	if( hEncBuf == NULL)  {
		ERR("Failed to allocate buffer for encoded data\n");
		cleanup(THREAD_FAILURE);
	}

	hDumpBuf = Buffer_create(encbufsize, &bAttrs); 
	if( hDumpBuf == NULL) {
		ERR("Failed to allocate buffer for encoded data\n");
		cleanup(THREAD_FAILURE);
	}
    
    /* A hDecBufTab buffer can be owned either by codec or display */
    gfxAttrs.bAttrs.useMask = CODEC_FREE | DISPLAY_FREE;
	nbDecVideoBufs = DISPLAY_PIPE_SIZE;
	
    /* Allocate buffers for decoded output. It is not possible to use
     * the buffers from the encoder input BufTab because buffers have 
     * different sizes.
     */
    hDecBufTab = BufTab_create(nbDecVideoBufs, decBufSize, BufferGfx_getBufferAttrs(&gfxAttrs));
    if (hDecBufTab == NULL) {
        ERR("Failed to create BufTab for display pipe\n");
        cleanup(THREAD_FAILURE);
    }

	//printf("************** hDecBufTab lineLength = %d\r\n", gfxAttrs.dim.lineLength);
	/* The decoder is going to use this BufTab for output buffers */
	Vdec2_setBufTab(hVd2, hDecBufTab);
    /* Make sure the display thread is stopped when it's unlocked */
    Pause_on(envp->hPausePrime);
    /* Signal that initialization is done and wait for other threads */
    Rendezvous_meet(envp->hRendezvousInit);

#if 1
/******************************************************/
	if(avifile != NULL) {
		pthread_mutex_lock(&avmutex);	
		frames = AVI_video_frames(avifile);
//		framew = AVI_video_width(avifile);
//		frameh = AVI_video_height(avifile);
//		framerate = AVI_frame_rate(avifile);   
		pthread_mutex_unlock(&avmutex);	
//		printf("\r\nall paras are: %d,%d,%d,%5.3f",frames,framew,frameh,framerate);
	}
	if (AVI_seek_start(avifile) )  {	//find the SOF
		printf("bad seek start\n");         
	}

	/* Prime the display thread */
	for (bufIdx=0; bufIdx < NUM_VIDEO_BUFS-CAPTURE_PIPE_SIZE-1; bufIdx++) //NUM_VIDEO_BUFS - CAPTURE_PIPE_SIZE
	{
	    /* Get a free buffer from hDecBufTab */
	    hDecBuf = BufTab_getFreeBuf(hDecBufTab);
	    if (hDecBuf == NULL)  {
	        ERR("Failed to get free buffer from BufTab\n");
	        BufTab_print(hDecBufTab);
	        cleanup(THREAD_FAILURE);
	    }
		frame = bufIdx;
		pthread_mutex_lock(&avmutex);				
		AVI_set_video_position(avifile, frame);//find the entile packet 
		streamlen = AVI_read_frame(avifile,(char *)videobuf, &iskeyframe);
		pthread_mutex_unlock(&avmutex);	
		
		streamptr = videobuf;
		/*
		 * Encode and decode the buffer from the capture thread and
		 * send any display buffers to the display thread.
		 */
		Buffer_setUserPtr(hDumpBuf, streamptr);
		Buffer_setSize(hDumpBuf, streamlen);
		Buffer_setNumBytesUsed(hDumpBuf, streamlen);
		if (Buffer_getNumBytesUsed(hDumpBuf) != 0)//cui added
		{
			ret = encodedecode(hVd2, hDumpBuf, hEncBuf, hDecBuf, envp->hDisplayInFifo);//hVidBuf, 
			if (ret < 0) 
			{
				cleanup(THREAD_FAILURE);
				gblSetQuit();		
			}
		}
	}
#endif

	/* Release the display thread, it is now fully primed */
	Pause_off(envp->hPausePrime);
	for(frame=0; frame<frames; frame++)//if start with "7",it will down
	{
		/* Pause processing? */
		Pause_test(envp->hPauseProcess);
		gl_decode_frame = frame;
		if(frame == (frames-1) ){
            Pause_on(envp->hPauseProcess);
			gl_decode_pause = 2;

			//system("amixer sset 'Mono DAC' 0 &");
			while(1){
				usleep(100000);
			}
		}

        /* Get a free buffer from hDecBufTab */
        hDecBuf = BufTab_getFreeBuf(hDecBufTab);
        if (hDecBuf == NULL) {
            ERR("Failed to get free buffer from BufTab\n");
            BufTab_print(hDecBufTab);
            cleanup(THREAD_FAILURE);
        }		
   		
		pthread_mutex_lock(&avmutex);	
		AVI_set_video_position(avifile, frame);////find the entile packet 
		streamlen = AVI_read_frame(avifile,(char *)videobuf, &iskeyframe);
		pthread_mutex_unlock(&avmutex);	

		streamptr = videobuf;
		if(streamptr[4]==0x67) {
			//printf("current frame is %02X\r\n", streamptr[4]);
			//frametype = 1;
		}
		if(streamptr[4]==0x68) {
			//printf("current frame is %02X\r\n", streamptr[4]);
			//frametype = 1;
		}
		 
		Buffer_setUserPtr(hDumpBuf, streamptr);
		Buffer_setSize(hDumpBuf, streamlen);
		Buffer_setNumBytesUsed(hDumpBuf, streamlen);
	    if (Buffer_getNumBytesUsed(hDumpBuf) != 0)//cui added &&  
        {
		 	ret = encodedecode(hVd2, hDumpBuf, hEncBuf, hDecBuf, envp->hDisplayInFifo);//hVidBuf,
		 	if (ret < 0){
			     	cleanup(THREAD_FAILURE);
			     	gblSetQuit();	     
		 	}
		 	 
			if (ret != Dmai_EFIRSTFIELD) {
				do {
					fifoRet = Fifo_get(envp->hDisplayOutFifo, &hDispBuf);
					if (fifoRet < 0) {
						ERR("Failed to get buffer from video thread\n");
						cleanup(THREAD_FAILURE);
					}
					/* Did the display thread flush the fifo? */
					if (fifoRet == Dmai_EFLUSH) {
						cleanup(THREAD_SUCCESS);
					}
					if (!envp->passThrough) {
						/* The display thread is no longer using the buffer */
						Buffer_freeUseMask(BufTab_getBuf(hDecBufTab,
									   Buffer_getId(hDispBuf)),DISPLAY_FREE);
					}
				} while (Fifo_getNumEntries(envp->hDisplayOutFifo) > 0);
			}	
  	   } 
  	  // if(I_CutFormat) Pause_on(envp->hPausePrime);
	} //for frame

	gl_videoend = 1;
    printf("video frame end\r\n");
	printf("----------------------->FUNCTION:%s line:%d\n",__FUNCTION__,__LINE__);
cleanup:
    /* Make sure the other threads aren't waiting for us */
    Rendezvous_force(envp->hRendezvousInit);
    Pause_off(envp->hPauseProcess);
    Pause_off(envp->hPausePrime);
    Fifo_flush(envp->hDisplayInFifo);
    /* Meet up with other threads before cleaning up */
    Rendezvous_meet(envp->hRendezvousCleanup);

    /* Clean up the thread before exiting */
    if (hEncBuf) {
        Buffer_delete(hEncBuf);
    }

    if (hDumpBuf) {
        Buffer_delete(hDumpBuf);
    }

    if (hVd2) {
        Vdec2_delete(hVd2);
    }

    if (hEngine) {
        Engine_close(hEngine);
    }

    return status;

}



static Int encodedecode_01(Vdec2_Handle hVd2,
                        Buffer_Handle hDumpBuf, Buffer_Handle hEncBuf,
                        Buffer_Handle hDecBuf)
{
    Buffer_Handle           hOutBuf, hFreeBuf;
    Int                     ret;
    BufferGfx_Dimensions    srcDim;
	Buffer_Handle	 tmp_Buffer;

    unsigned char* srcptr;
    unsigned char* dstptr;	

    /* Make sure the whole buffer is used for input */
    BufferGfx_resetDimensions(hDecBuf);
    srcptr = Buffer_getUserPtr(hDumpBuf); 
  	dstptr = Buffer_getUserPtr(hEncBuf);
    memcpy(dstptr, srcptr, Buffer_getNumBytesUsed(hDumpBuf));
    Buffer_setNumBytesUsed(hEncBuf, (Int32)Buffer_getNumBytesUsed(hDumpBuf));
	Buffer_setSize(hEncBuf, Buffer_getNumBytesUsed(hDumpBuf));
	if(srcptr[4]==0x67)
	{
	//	printf("first step : current frame is IDR %02X\r\n", srcptr[4]);
	}

    /* Decode the video buffer */
    ret = Vdec2_process(hVd2, hEncBuf, hDecBuf);
    if (ret != Dmai_EOK) 
    {
        ERR("Failed to decode video buffer\n");
        return FAILURE;
    }
 //   printf("\nDecode successful!\n\n\n");
    /* Send display frames to display thread */

    //pthread_mutex_lock(&display_mutex);
    glob_hSrcBuf = Vdec2_getDisplayBuf(hVd2);
	printf("---------------------decode\n");
    while (glob_hSrcBuf){
         //Fifo_put(displayFifo, hOutBuf);
         glob_hSrcBuf = Vdec2_getDisplayBuf(hVd2);
    }
	//pthread_mutex_unlock(&display_mutex);

    /* Free up released frames */
    hFreeBuf = Vdec2_getFreeBuf(hVd2);
    while (hFreeBuf) {
        Buffer_freeUseMask(hFreeBuf, CODEC_FREE);
        hFreeBuf = Vdec2_getFreeBuf(hVd2);
    }

    return SUCCESS;
}



/******************************************************************************
 * videoThrFxn
 ******************************************************************************/
Void *videoThrFxn(Void *arg)
{
    VideoEnv               *envp                 = (VideoEnv *) arg;
    Void                   *status               = THREAD_SUCCESS;
    VIDDEC2_Params          defaultDecParams     = Vdec2_Params_DEFAULT;
    VIDDEC2_DynamicParams   defaultDecDynParams  = Vdec2_DynamicParams_DEFAULT;
    BufferGfx_Attrs         gfxAttrs             = BufferGfx_Attrs_DEFAULT;
    Buffer_Attrs            bAttrs               = Buffer_Attrs_DEFAULT;
    Vdec2_Handle            hVd2                 = NULL;
    Engine_Handle           hEngine              = NULL;
    Buffer_Handle           hEncBuf              = NULL;
    Buffer_Handle           hDumpBuf             = NULL;
    Int                     ret                  = Dmai_EOK;
    Buffer_Handle           hDispBuf; 
    VIDDEC2_Params         *decParams;
    VIDDEC2_DynamicParams  *decDynParams;
    Int                     fifoRet;
    Int                     bufIdx;
    ColorSpace_Type         colorSpace = ColorSpace_YUV420PSEMI;
    IH264VDEC_Params        extnParams;

    Int32                   nbDecVideoBufs;
    BufTab_Handle           hDecBufTab           = NULL;
    Buffer_Handle           hDecBuf              = NULL;
    Int32                   decBufSize;  
    Int32                   width, height;

	unsigned char* streamptr;
	Int streamlen;
	unsigned char* ptr;
	int i = 0;
/******************************************************/
	gl_videoend = 0;
	long framesize;
    int frame;
    int framew = 0;
    int frameh = 0;
    int framerate = 0.0f;
	int iskeyframe;//if it's keyframe
/*******************************************************/
    /* Use supplied params if any, otherwise use defaults */
    decParams = envp->decParams ? envp->decParams : &defaultDecParams;
    decDynParams = envp->decDynParams ? envp->decDynParams :
                                        &defaultDecDynParams;
	width  = 720;
	height = 576;
    envp->imageWidth  = Dmai_roundUp(width, 32); 
    envp->imageHeight = height;
    gfxAttrs.colorSpace     = colorSpace;
    gfxAttrs.dim.width      = envp->imageWidth;
    gfxAttrs.dim.height     = envp->imageHeight;
    gfxAttrs.dim.lineLength =
            Dmai_roundUp(BufferGfx_calcLineLength(gfxAttrs.dim.width,
                                   gfxAttrs.colorSpace), 32);                            
  //printf("************** envp->imagewidth = %d\r\n", envp->imageWidth);
	/* Open the codec engine */
	hEngine = Engine_open(envp->engineName, NULL, NULL);
	if (hEngine == NULL){
		ERR("Failed to open codec engine %s\n", envp->engineName);
		cleanup(THREAD_FAILURE);
	}

	decParams->maxBitRate = 10485760;
	decParams->maxWidth   = envp->imageWidth;
	decParams->maxHeight  = Dmai_roundUp(envp->imageHeight, CODECHEIGHTALIGN);
	decParams->maxFrameRate = 25000;
	decParams->forceChromaFormat = XDM_YUV_420SP;
	
	if (!strcmp(envp->videoDecoder, "h264dec")){
		extnParams.displayDelay = 8;
		extnParams.levelLimit = 0;
		extnParams.disableHDVICPeveryFrame = 0;
		extnParams.inputDataMode = 1;
		extnParams.sliceFormat = 1;
		extnParams.frame_closedloop_flag = 1;
		decParams->size = sizeof(IH264VDEC_Params);
	}               

	extnParams.viddecParams = *decParams;
	decDynParams->displayWidth = 800;

	/* Create the video decoder */
	hVd2 = Vdec2_create(hEngine, envp->videoDecoder, (VIDDEC2_Params*)&extnParams, decDynParams);
	if (hVd2 == NULL){
		ERR("Failed to create video decoder: %s\n", envp->videoDecoder);
		cleanup(THREAD_FAILURE);
	}
	
	/* Which output buffer size does the decoder require? */
	decBufSize	   = Vdec2_getOutBufSize(hVd2);
	int encbufsize = Vdec2_getInBufSize(hVd2);
	
	/* Allocate buffer for encoded data */
	hEncBuf = Buffer_create(encbufsize, &bAttrs); 
	if( hEncBuf == NULL)  {
		ERR("Failed to allocate buffer for encoded data\n");
		cleanup(THREAD_FAILURE);
	}

	hDumpBuf = Buffer_create(encbufsize, &bAttrs); 
	if( hDumpBuf == NULL) {
		ERR("Failed to allocate buffer for encoded data\n");
		cleanup(THREAD_FAILURE);
	}
    
    /* A hDecBufTab buffer can be owned either by codec or display */
    gfxAttrs.bAttrs.useMask = CODEC_FREE | DISPLAY_FREE;
	nbDecVideoBufs = DISPLAY_PIPE_SIZE;
	
    /* Allocate buffers for decoded output. It is not possible to use
     * the buffers from the encoder input BufTab because buffers have 
     * different sizes.
     */
    hDecBufTab = BufTab_create(nbDecVideoBufs, decBufSize, BufferGfx_getBufferAttrs(&gfxAttrs));
    if (hDecBufTab == NULL) {
        ERR("Failed to create BufTab for display pipe\n");
        cleanup(THREAD_FAILURE);
    }

	//printf("************** hDecBufTab lineLength = %d\r\n", gfxAttrs.dim.lineLength);
	/* The decoder is going to use this BufTab for output buffers */
	Vdec2_setBufTab(hVd2, hDecBufTab);


#if 1
/******************************************************/
	if(avifile != NULL) {
		pthread_mutex_lock(&avmutex);	
		frames = AVI_video_frames(avifile);
//		framew = AVI_video_width(avifile);
//		frameh = AVI_video_height(avifile);
//		framerate = AVI_frame_rate(avifile);   
		pthread_mutex_unlock(&avmutex);	
//		printf("\r\nall paras are: %d,%d,%d,%5.3f",frames,framew,frameh,framerate);
	}
	if (AVI_seek_start(avifile) )  {	//find the SOF
		printf("bad seek start\n");         
	}
#endif
#if 0
	/* Prime the display thread */
	for (bufIdx=0; bufIdx < NUM_VIDEO_BUFS-CAPTURE_PIPE_SIZE-1; bufIdx++) //NUM_VIDEO_BUFS - CAPTURE_PIPE_SIZE
	{
	    /* Get a free buffer from hDecBufTab */
	    hDecBuf = BufTab_getFreeBuf(hDecBufTab);
	    if (hDecBuf == NULL)  {
	        ERR("Failed to get free buffer from BufTab\n");
	        BufTab_print(hDecBufTab);
	        cleanup(THREAD_FAILURE);
	    }
		frame = bufIdx;
		pthread_mutex_lock(&avmutex);				
		AVI_set_video_position(avifile, frame);//find the entile packet 
		streamlen = AVI_read_frame(avifile,(char *)videobuf, &iskeyframe);
		pthread_mutex_unlock(&avmutex);	
		
		streamptr = videobuf;
		/*
		 * Encode and decode the buffer from the capture thread and
		 * send any display buffers to the display thread.
		 */
		Buffer_setUserPtr(hDumpBuf, streamptr);
		Buffer_setSize(hDumpBuf, streamlen);
		Buffer_setNumBytesUsed(hDumpBuf, streamlen);
		if (Buffer_getNumBytesUsed(hDumpBuf) != 0)//cui added
		{
			ret = encodedecode_01(hVd2, hDumpBuf, hEncBuf, hDecBuf);//hVidBuf, 
			if (ret < 0) 
			{
				cleanup(THREAD_FAILURE);
				gblSetQuit();		
			}
			
		}
	}
#endif
	for(frame=0; frame<frames; frame++)//if start with "7",it will down
	{
		/* Pause processing? */
		Pause_test(envp->hPauseProcess);
		gl_decode_frame = frame;
		if(frame == (frames-1) ){
            Pause_on(envp->hPauseProcess);
			gl_decode_pause = 2;

			//system("amixer sset 'Mono DAC' 0 &");
			while(1){
				usleep(100000);
			}
		}
        /* Get a free buffer from hDecBufTab */
        hDecBuf = BufTab_getFreeBuf(hDecBufTab);
        if (hDecBuf == NULL) {
            ERR("Failed to get free buffer from BufTab\n");
            BufTab_print(hDecBufTab);
            cleanup(THREAD_FAILURE);
        }		
   		
		pthread_mutex_lock(&avmutex);	
		AVI_set_video_position(avifile, 2);////find the entile packet 
		streamlen = AVI_read_frame(avifile,(char *)videobuf, &iskeyframe);
		pthread_mutex_unlock(&avmutex);	

		streamptr = videobuf;
		if(streamptr[4]==0x67) {
			//printf("current frame is %02X\r\n", streamptr[4]);
			//frametype = 1;
		}
		if(streamptr[4]==0x68) {
			//printf("current frame is %02X\r\n", streamptr[4]);
			//frametype = 1;
		}
		Buffer_setUserPtr(hDumpBuf, streamptr);
		Buffer_setSize(hDumpBuf, streamlen);
		Buffer_setNumBytesUsed(hDumpBuf, streamlen);
	    if (Buffer_getNumBytesUsed(hDumpBuf) != 0) 
        {
		 	ret = encodedecode_01(hVd2,hDumpBuf,hEncBuf,hDecBuf);
		 	if (ret < 0){
			     	cleanup(THREAD_FAILURE);
			     	gblSetQuit();	     
		 	}
			if (ret != Dmai_EFIRSTFIELD) {
					/* The display thread is no longer using the buffer */
					Buffer_freeUseMask(BufTab_getBuf(hDecBufTab,Buffer_getId(hDecBuf)),DISPLAY_FREE);
			}
			usleep(100000);
  	   } 
  	   frame = frames;
  	  
	} //for frame

	gl_videoend = 1;
    printf("video frame end\r\n");
	printf("----------------------->FUNCTION:%s line:%d\n",__FUNCTION__,__LINE__);
cleanup:
    /* Make sure the other threads aren't waiting for us */
    Rendezvous_force(envp->hRendezvousInit);
    Pause_off(envp->hPauseProcess);
    Pause_off(envp->hPausePrime);
    Fifo_flush(envp->hDisplayInFifo);
    /* Meet up with other threads before cleaning up */
    Rendezvous_meet(envp->hRendezvousCleanup);

    /* Clean up the thread before exiting */
    if (hEncBuf) {
        Buffer_delete(hEncBuf);
    }

    if (hDumpBuf) {
        Buffer_delete(hDumpBuf);
    }

    if (hVd2) {
        Vdec2_delete(hVd2);
    }

    if (hEngine) {
        Engine_close(hEngine);
    }

    return status;

}

