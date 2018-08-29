/*
 * video.c
 *
 * This source file has the implementations for the video thread
 * functions implemented for 'DVSDK encode demo' on DM365 platform
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

#include <xdc/std.h>
#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/dmai/Fifo.h>
#include <ti/sdo/dmai/Pause.h>
#include <ti/sdo/dmai/BufTab.h>
#include <ti/sdo/dmai/VideoStd.h>
#include <ti/sdo/dmai/BufferGfx.h>
#include <ti/sdo/dmai/Rendezvous.h>
#include <ti/sdo/dmai/ce/Venc1.h>
#include <ti/sdo/dmai/Capture.h>
#include <ti/sdo/dmai/ce/Ienc1.h>
#include "video.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>  
#include <sys/stat.h>  
#include <string.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <limits.h>  
#include <errno.h>  
#include <sys/shm.h>
#include <dirent.h>
#include <time.h>
#include "../ctrl.h"
#include "../demo.h"
#include <sys/shm.h>
#include <sys/sem.h>


#ifndef YUV_420SP
#define YUV_420SP 256
#endif 

#define BUFSIZEALIGN            128
#define VIDEO_PIPE_SIZE         4 

#define FIFO_NAME   "/tmp/h264fifo"
#define BUFFERSIZE   PIPE_BUF  

#define SHARE_MEM_ID   (key_t)1234
 
int m_sem_id;
int shmid;

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};

typedef struct share_mem
{
	int len;
	unsigned char data[1024*1024];
}share_mem;

struct share_mem *pch_shmem;

#ifdef H264_FIFO_OUT
int SendH264Data(unsigned char *data,unsigned int size)
{
//    pipe_fd = open(FIFO_NAME, O_WRONLY);

	pipe_fd = open(FIFO_NAME, O_WRONLY | O_NONBLOCK); 

    if(pipe_fd == -1)  
    {  
        return 0;  
    }  
	//else
	//{
//	printf("[RTSPStream] open fifo result = [%d]\n", pipe_fd);  
	//}

	if(pipe_fd > 0)
	{
		int send_size = 0;  
		int remain_size = size;  
		while(send_size < size)  
		{  
		    int data_len = (remain_size<BUFFERSIZE) ? remain_size : BUFFERSIZE;  
		    int len = write(pipe_fd, data+send_size, data_len);  
		    if(len == -1)  
		    {  
		        static int resend_conut = 0;  
		        if(errno == EAGAIN && ++resend_conut<=3)  
		        {  
		            printf("[RTSPStream] write fifo error,resend..\n");  
		            continue;  
		        }  
		        resend_conut = 0;  
		        printf("[RTSPStream] write fifo error,errorcode[%d],send_size[%d]\n",errno,send_size);  
		        break;  
		    }  
		    else  
		    {    
		        send_size+= len;  
		        remain_size-= len;  
		    }  
		}  
	}
    close(pipe_fd);  

    return 0;  
}
#endif



unsigned int create_filename(char* filestr, const char* ext)
{
	char filename[64];
	char strbuf[10];
	unsigned int filecnt;

	if(strcmp("avi", ext) == 0)  //a avi file
	{
		gl_max_avi ++;
		sprintf(gl_filename, "AVI_%05d.avi", gl_max_avi);
		sprintf(filename, "/home/root/media/AVI_%05d.avi", gl_max_avi);
	}
	else if(strcmp("bmp", ext) == 0)  //a jpg file
	{
		gl_max_bmp ++;
		sprintf(gl_bmpname, "BMP_%05d.bmp", gl_max_bmp);
		sprintf(filename, "/home/root/media/BMP_%05d.bmp", gl_max_bmp);
	}
	else if(strcmp("jpg", ext) == 0)  //a jpg file
	{
		gl_max_jpg ++;
		sprintf(gl_jpgname, "JPG_%05d.jpg", gl_max_jpg);
		sprintf(filename, "/home/root/media/JPG_%05d.jpg", gl_max_jpg);
	}
	memcpy(filestr, filename, strlen(filename));
	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////
void ForceIDR(Venc1_Handle hVe1, VIDENC1_DynamicParams *dynParams, unsigned int idr)
{
	Int mystatus;     
	VIDENC1_Status tEncStatus;
	VIDENC1_Handle enghandle = Venc1_getVisaHandle(hVe1);
	tEncStatus.size = sizeof(VIDENC1_Status);
	tEncStatus.data.buf = NULL;

	if(0 == idr)
	{
		dynParams->forceFrame = IVIDEO_NA_FRAME;
	}
	else
	{	
		dynParams->forceFrame = IVIDEO_IDR_FRAME;  //IVIDEO_IDR_FRAME;
	}

	mystatus = VIDENC1_control(enghandle, XDM_SETPARAMS, dynParams, &tEncStatus);

	if(mystatus != VIDENC1_EOK) 
	{
    	VIDENC1_delete(enghandle);
    	return;
	}
}


void yuv420SP_to_yuv420P(unsigned char* source, unsigned char* dest)
{
	int i;
	unsigned int   pixnumber = JPG_WIDTH*YUV_HEIGHT;

	unsigned char* psrcyy   = source;
	unsigned char* psrcuuvv = psrcyy + pixnumber;

	unsigned char* pdstyy   = dest;
	unsigned char* pdstuu   = pdstyy + pixnumber;
	unsigned char* pdstvv   = pdstuu + pixnumber/4;

	memcpy(pdstyy, psrcyy, pixnumber);  //copy yy

	for(i=0; i<pixnumber/4; i++)
	{
		*pdstuu ++ = *psrcuuvv ++;  //get uu
		*pdstvv ++ = *psrcuuvv ++;  //get vv
	}
}


extern int rtsp_idr_trigger;
/******************************************************************************
 * videoThrFxn
 ******************************************************************************/
Void *videoThrFxn(Void *arg)
{
    VideoEnv               *envp                = (VideoEnv *) arg;
    Void                   *status              = THREAD_SUCCESS;
    VIDENC1_Params          defaultParams       = Venc1_Params_DEFAULT;
    VIDENC1_DynamicParams   defaultDynParams    = Venc1_DynamicParams_DEFAULT;
    BufferGfx_Attrs         gfxAttrs            = BufferGfx_Attrs_DEFAULT;
    Venc1_Handle            hVe1                = NULL;
    Engine_Handle           hEngine             = NULL;
    BufTab_Handle           hBufTab             = NULL;
    Int                     frameCnt            = 0;
    Buffer_Handle           hCapBuf, hDstBuf;
    VIDENC1_Params         *params;
    VIDENC1_DynamicParams  *dynParams;
    Int                     fifoRet;
    Int                     bufIdx;
    ColorSpace_Type         colorSpace = ColorSpace_YUV420PSEMI;
    Bool                    localBufferAlloc = TRUE;
    BufferGfx_Dimensions    dim;
    Int32                   bufSize;


    Ienc1_Handle            hIe1            = NULL;
    IMGENC1_Params          IMGparams       = Ienc1_Params_DEFAULT;
    IMGENC1_DynamicParams   IMGdynParams    = Ienc1_DynamicParams_DEFAULT;
    Buffer_Handle           hJpegOutBuf     = NULL;
    Buffer_Handle           hJpegInBuf      = NULL;
    Buffer_Attrs            bAttrs          = Buffer_Attrs_DEFAULT;
    BufferGfx_Attrs         IMGgfxAttrs     = BufferGfx_Attrs_DEFAULT;
    FILE                    *jpgoutFile     = NULL;

	Int                     jpegoutBufSize;
	Int                     jpeginBufSize;
	unsigned long           jpgfilelen;
	unsigned char           *jpgfileptr;
	char                    jpgfilestr[64];
	char                    bmpfilestr[64];
	Int                     ret;
	unsigned char          	*framesource;
	unsigned char           *framedest;
	unsigned char*          yuv2bmpbuf[736*576*2];
	int can_save_bmp = 0;

 	firstjpg = 1;

#ifdef H264_FIFO_OUT
	if( access(FIFO_NAME, F_OK) == -1 )
	{
		printf("access fifo\r\n");
		int res = mkfifo(FIFO_NAME, 0777);
		if(res != 0)
		{
			printf("mkfifo failed\r\n");
		}
	}
	else
	{
		printf(" fifo exist\r\n");
	}
//	pthread_t thread;
//	pthread_create(&thread, NULL, send_fifo, 0);
#endif

#ifdef H264_SHMEM_OUT
	printf("sizeof(struct share_mem) : %d",sizeof(struct share_mem));

	m_sem_id = semget((key_t)3443, 1, 0666 | IPC_CREAT);

	union semun sem_union;

    sem_union.val = 1;

    if(semctl(m_sem_id, 0, SETVAL, sem_union) == -1)
        perror("semctl error");
    
	shmid = shmget(SHARE_MEM_ID, sizeof(share_mem), (IPC_CREAT|0666));

	if(shmid < 0)
		perror("shmget error");

	pch_shmem = ( struct share_mem* )shmat(shmid,( const void* )0,0 );
	if(pch_shmem == (void *)-1)
	{
		perror("shmem error\n");
	}
////////////////////////////////////////
#endif

    /* Open the codec engine */
    hEngine = Engine_open(envp->engineName, NULL, NULL);

    if (hEngine == NULL) 
	{
        ERR("Failed to open codec engine %s\n", envp->engineName);
        cleanup(THREAD_FAILURE);
    }

////////////////////////////////////////////////////////////////////////////////
#ifdef JPEG_FILE
    IMGparams.maxWidth             = JPG_WIDTH;
    IMGparams.maxHeight            = YUV_HEIGHT;
	IMGparams.forceChromaFormat    = XDM_YUV_420P;
    IMGdynParams.inputWidth        = IMGparams.maxWidth;
    IMGdynParams.inputHeight       = IMGparams.maxHeight;
    IMGdynParams.captureWidth      = IMGparams.maxWidth;
	IMGdynParams.inputChromaFormat = XDM_YUV_420P;
    IMGdynParams.qValue            = 80;
//	bAttrs.memParams.align         = BUFSIZEALIGN; 

    /* Create the image encoder */
    hIe1 = Ienc1_create(hEngine, "jpegenc", &IMGparams, &IMGdynParams);

    if (hIe1 == NULL) 
	{
       printf("create jpegenc failed\r\n");
    }
	else
	{
       printf("success jpegenc\r\n");
	}

    /* Align buffers to cache line boundary */    
    IMGgfxAttrs.bAttrs.memParams.align = bAttrs.memParams.align = BUFSIZEALIGN; 
    /* Calculate the buffer attributes */
    IMGgfxAttrs.dim.width  = JPG_WIDTH;
    IMGgfxAttrs.dim.height = YUV_HEIGHT;
    IMGgfxAttrs.dim.lineLength = BufferGfx_calcLineLength(JPG_WIDTH,  ColorSpace_YUV420P);
    IMGgfxAttrs.colorSpace = ColorSpace_YUV420P;

    jpeginBufSize = Ienc1_getInBufSize(hIe1);

    /* Create jpeg input buffer */
    hJpegInBuf = Buffer_create(Dmai_roundUp(jpeginBufSize, BUFSIZEALIGN), BufferGfx_getBufferAttrs(&IMGgfxAttrs));

    if (hJpegInBuf == NULL) 
	{
		printf("Failed to create jpeg INBUF!!\r\n"); 
    }

    jpegoutBufSize = Ienc1_getOutBufSize(hIe1);

    /* Create output buffer */
    hJpegOutBuf = Buffer_create(Dmai_roundUp(jpegoutBufSize, BUFSIZEALIGN), &bAttrs);

    if (hJpegOutBuf == NULL) 
	{
		printf("Failed to create jpeg OUTBUF!!\r\n"); 
    }
#endif
////////////////////////////////////////////////////////////////////////////////

    /* In case of HD resolutions the video buffer will be allocated
       by capture thread. */
    if (((envp->imageWidth == VideoStd_720P_WIDTH) && 
        (envp->imageHeight == VideoStd_720P_HEIGHT)) ||
      	((envp->imageWidth == VideoStd_1080I_WIDTH) &&
        (envp->imageHeight == VideoStd_1080I_HEIGHT))) 
	{
        localBufferAlloc = FALSE;
    } 
    
    /* Use supplied params if any, otherwise use defaults */
    params = envp->params ? envp->params : &defaultParams;
    dynParams = envp->dynParams ? envp->dynParams : &defaultDynParams;

    /* 
     * Set up codec parameters. We round up the height to accomodate for 
     * alignment restrictions from codecs 
     */
    params->maxWidth        = envp->imageWidth;
    params->maxHeight       = Dmai_roundUp(envp->imageHeight, CODECHEIGHTALIGN);
    params->encodingPreset  = XDM_HIGH_SPEED;
    if (colorSpace ==  ColorSpace_YUV420PSEMI) { 
        params->inputChromaFormat = XDM_YUV_420SP;
    } else {
        params->inputChromaFormat = XDM_YUV_422ILE;
    }
    params->reconChromaFormat = XDM_YUV_420SP;
    params->maxFrameRate      = envp->videoFrameRate;

    /* Set up codec parameters depending on bit rate */
    if (envp->videoBitRate < 0) 
	{
        /* Variable bit rate */
        params->rateControlPreset = IVIDEO_NONE;
        /*
         * If variable bit rate use a bogus bit rate value (> 0)
         * since it will be ignored.
         */
		params->maxBitRate        = 512000;
    }
    else 
	{
        /* Constant bit rate */
        params->rateControlPreset = IVIDEO_STORAGE;
        params->maxBitRate        = envp->videoBitRate;
    }

	/* Variable bit rate */
    params->rateControlPreset = IVIDEO_STORAGE; //IVIDEO_NONE;
    params->maxBitRate        = 1000000;   //512000;


//	params->rateControlPreset = IVIDEO_NONE;


    dynParams->targetBitRate   = params->maxBitRate;
    dynParams->inputWidth      = envp->imageWidth;
    dynParams->inputHeight     = envp->imageHeight;    
    dynParams->refFrameRate    = params->maxFrameRate;
    dynParams->targetFrameRate = params->maxFrameRate;
    dynParams->interFrameInterval = 0;
    
    /* Create the video encoder */
    hVe1 = Venc1_create(hEngine, envp->videoEncoder, params, dynParams);

    if (hVe1 == NULL) {
        ERR("Failed to create video encoder: %s\n", envp->videoEncoder);
        cleanup(THREAD_FAILURE);
    }

    /* Store the output buffer size in the environment */
    envp->outBufSize = Venc1_getOutBufSize(hVe1);

    /* Signal that the codec is created and output buffer size available */
    Rendezvous_meet(envp->hRendezvousWriter);
    if (localBufferAlloc == TRUE) {
        gfxAttrs.colorSpace = colorSpace;
        gfxAttrs.dim.width  = envp->imageWidth;
        gfxAttrs.dim.height = envp->imageHeight;
        gfxAttrs.dim.lineLength = 
            Dmai_roundUp(BufferGfx_calcLineLength(gfxAttrs.dim.width,
                                   gfxAttrs.colorSpace), 32);
        
        /* 
         *Calculate size of codec buffers based on rounded LineLength 
         *This will allow to share the buffers with Capture thread. If the
         *size of the buffers was obtained from the codec through 
         *Venc1_getInBufSize() there would be a mismatch with the size of
         * Capture thread buffers when the width is not 32 byte aligned.
         */
        if (colorSpace ==  ColorSpace_YUV420PSEMI) 
		{
            bufSize = gfxAttrs.dim.lineLength * gfxAttrs.dim.height * 3 / 2;
        } 
        else 
		{
            bufSize = gfxAttrs.dim.lineLength * gfxAttrs.dim.height * 2;
        } 

        /* Create a table of buffers with size based on rounded LineLength */
        hBufTab = BufTab_create(VIDEO_PIPE_SIZE, bufSize,
                                BufferGfx_getBufferAttrs(&gfxAttrs));

        if (hBufTab == NULL) {
            ERR("Failed to allocate contiguous buffers\n");
            cleanup(THREAD_FAILURE);
        }

        /* Send buffers to the capture thread to be ready for main loop */
        for (bufIdx = 0; bufIdx < VIDEO_PIPE_SIZE; bufIdx++) {
            if (Fifo_put(envp->hCaptureInFifo,
                         BufTab_getBuf(hBufTab, bufIdx)) < 0) {
                ERR("Failed to send buffer to display thread\n");
                cleanup(THREAD_FAILURE);
            }
        }
    } else {
        /* Send buffers to the capture thread to be ready for main loop */
        for (bufIdx = 0; bufIdx < VIDEO_PIPE_SIZE; bufIdx++) {
            fifoRet = Fifo_get(envp->hCaptureOutFifo, &hCapBuf);

            if (fifoRet < 0) {
                ERR("Failed to get buffer from capture thread\n");
                cleanup(THREAD_FAILURE);
            }

            /* Did the capture thread flush the fifo? */
            if (fifoRet == Dmai_EFLUSH) {
                cleanup(THREAD_SUCCESS);
            }
            /* Return buffer to capture thread */
            if (Fifo_put(envp->hCaptureInFifo, hCapBuf) < 0) {
                ERR("Failed to send buffer to display thread\n");
                cleanup(THREAD_FAILURE);
            }
        }
    }


    /* Signal that initialization is done and wait for other threads */
    Rendezvous_meet(envp->hRendezvousInit);
    
    while (!gblGetQuit()) {
        /* Pause processing? */
        Pause_test(envp->hPauseProcess);

        /* Get a buffer to encode from the capture thread */
        fifoRet = Fifo_get(envp->hCaptureOutFifo, &hCapBuf);

        if (fifoRet < 0) {
            ERR("Failed to get buffer from video thread\n");
            cleanup(THREAD_FAILURE);
        }
////////////////////////////////////////////////////////////////////////////////
   	 	/* Encode the image buffer */
		if( (gl_get_picture != 0) || (firstjpg == 1) ) 
		{
			can_save_bmp = 0;
			if(gl_system_status == STATUS_STOP)   //rec stop status
			{
				if( CheckoutDiskSpace() > BMP_MIN_LIMIT)  //check tf space
				{
					can_save_bmp = 1;   //can save bmp file
				} 
				else
				{
					gl_sdcard_full = 1;
				}
			}
			else if(gl_system_status == STATUS_REC)   //rec status
			{
				if( gl_sdcard_full == 0)  //check tf space
				{
					can_save_bmp = 1;   //can save bmp file
				} 
			}

			if(1 == can_save_bmp)
			{
				//printf("TF card ok, create a bmp file\r\n");
#ifdef BMP_FILE
				memset(bmpfilestr, 0, 64);
				//printf("1 - filestr set to 0\r\n");

				create_filename(bmpfilestr, "bmp");
				//printf("2 - set bmp file name\r\n");
				gl_picture_osd = 40;

				int capbuflen = Buffer_getNumBytesUsed(hCapBuf);

				if(capbuflen>0)
				{
					//framesource = NULL;
					framesource = Buffer_getUserPtr(hCapBuf);
					//printf("3 - set cap ptr\r\n");
					if(framesource != NULL)
					{
						//printf("4 - copy bmp buf\r\n");
						//memcpy(yuv2bmpbuf, framesource, capbuflen);
						printf("5 - open bmp file: %s\r\n", bmpfilestr);
						convert(framesource, bmpfilestr);
					}
				}

				printf("6 - convert bmp end\r\n");
#endif

#ifdef JPEG_FILE
				framesource = Buffer_getUserPtr(hCapBuf);
				framedest   = Buffer_getUserPtr(hJpegInBuf);
				yuv420SP_to_yuv420P(framesource, framedest);
				Buffer_setNumBytesUsed(hJpegInBuf, JPG_WIDTH*YUV_HEIGHT*3/2);

				if (Ienc1_process(hIe1, hJpegInBuf, hJpegOutBuf) < 0) 
				{
					ret = Dmai_EFAIL;
					printf("Failed to encode jpeg buffer\n");
				}
				else
				{
					gl_picture_osd = 40;
					jpgfilelen = Buffer_getNumBytesUsed(hJpegOutBuf);
					jpgfileptr = Buffer_getUserPtr(hJpegOutBuf);


					memset(jpgfilestr, 0, 64);
					create_filename(jpgfilestr, "jpg");

					if(firstjpg == 1)  //first jpeg file
					{
						jpgoutFile = fopen("/home/root/media/.test.jpg", "w");
					//	printf(" .test.jpg opened\r\n");
						
					}
					else
					{
						jpgoutFile = fopen(jpgfilestr, "w");
						printf("===== create jpeg file: %s =====\r\n", jpgfilestr );
					}

					fwrite(jpgfileptr, jpgfilelen, 1, jpgoutFile);

					if (jpgoutFile) 
					{	
						fflush(jpgoutFile);
						fsync(jpgoutFile);
						fclose(jpgoutFile);
					} 
				//	system("sync");  //file write sync control!!!!!!
	
					if(firstjpg)  //first jpeg file
					{
						system("rm -rf /home/root/media/.test.jpg");
					//	printf(" .test.jpg deleted\r\n");
						gl_picture_osd = 0;
					}
					firstjpg = 0;
				}
#endif
			}
			else
			{
				printf("TF card full, no bmp created\r\n");
			}
			gl_get_picture = 0;
			printf("\r\n");
		}
////////////////////////////////////////////////////////////////////////////////

        /* Did the capture thread flush the fifo? */
        if (fifoRet == Dmai_EFLUSH) {
            cleanup(THREAD_SUCCESS);
        }

        /* Get a buffer to encode to from the writer thread */
        fifoRet = Fifo_get(envp->hWriterOutFifo, &hDstBuf);

        if (fifoRet < 0) {
            ERR("Failed to get buffer from video thread\n");
            cleanup(THREAD_FAILURE);
        }

        /* Did the writer thread flush the fifo? */
        if (fifoRet == Dmai_EFLUSH) {
            cleanup(THREAD_SUCCESS);
        }

        /* Make sure the whole buffer is used for input */
        BufferGfx_resetDimensions(hCapBuf);

        /* Ensure that the video buffer has dimensions accepted by codec */
        BufferGfx_getDimensions(hCapBuf, &dim);
        dim.height = Dmai_roundUp(dim.height, CODECHEIGHTALIGN);
        BufferGfx_setDimensions(hCapBuf, &dim);


		if(idr_frame_promote > 0) 
		{		
			idr_frame_promote --;
			ForceIDR(hVe1, dynParams, 1);
			idr_trigger = 1;
		}
		else if(rtsp_idr_trigger > 0) 
		{
			rtsp_idr_trigger --;
			ForceIDR(hVe1, dynParams, 1);
			idr_trigger = 1;
		}

		video_frame_count ++;


		if(video_frame_count % 100 == 0)
		{
			ForceIDR(hVe1, dynParams, 1);
			idr_trigger = 1;
		}
		//else
		//{
		//	ForceIDR(hVe1, dynParams, 0);
		//	idr_trigger = 0;
		//}

        /* Encode the video buffer */
        if (Venc1_process(hVe1, hCapBuf, hDstBuf) < 0) {
            ERR("Failed to encode video buffer\n");
            cleanup(THREAD_FAILURE);
        }

		unsigned long vidbuflen = Buffer_getNumBytesUsed(hDstBuf);
		unsigned char* sendptr = Buffer_getUserPtr(hDstBuf);


#ifdef FUN_RTSP
		DM368PutH264DataToBuffer(sendptr, vidbuflen, idr_trigger);
		idr_trigger = 0;
#endif


//send h.264 stream to rtspserver through fifo
#ifdef H264_FIFO_OUT
		SendH264Data(sendptr, vidbuflen);
#endif

#ifdef H264_SHMEM_OUT
		struct sembuf sem_b;
    	sem_b.sem_num = 0;
    	sem_b.sem_op = -1;//P()
    	sem_b.sem_flg = SEM_UNDO;
    	if(semop(m_sem_id, &sem_b, 1) == -1)
    	{
        	printf("semaphore_p failed\n");
    	}

		pch_shmem->len = vidbuflen;
		memcpy(pch_shmem->data, sendptr, pch_shmem->len);

		sem_b.sem_num = 0;
		sem_b.sem_op = 1;//P()
		sem_b.sem_flg = SEM_UNDO;
		if(semop(m_sem_id, &sem_b, 1) == -1)
		{
			printf("semaphore_p failed\n");
		}
#endif

        /* Reset the dimensions to what they were originally */
        BufferGfx_resetDimensions(hCapBuf);

        /* Send encoded buffer to writer thread for filesystem output */
        if (Fifo_put(envp->hWriterInFifo, hDstBuf) < 0) {
            ERR("Failed to send buffer to display thread\n");
            cleanup(THREAD_FAILURE);
        }

        /* Return buffer to capture thread */
        if (Fifo_put(envp->hCaptureInFifo, hCapBuf) < 0) {
            ERR("Failed to send buffer to display thread\n");
            cleanup(THREAD_FAILURE);
        }

        /* Increment statistics for the user interface */
//        gblIncVideoBytesProcessed(Buffer_getNumBytesUsed(hDstBuf));

        frameCnt++;
    }

cleanup:
    /* Make sure the other threads aren't waiting for us */
    Rendezvous_force(envp->hRendezvousInit);
    Rendezvous_force(envp->hRendezvousWriter);
    Pause_off(envp->hPauseProcess);
    Fifo_flush(envp->hWriterInFifo);
    Fifo_flush(envp->hCaptureInFifo);

    /* Make sure the other threads aren't waiting for init to complete */
    Rendezvous_meet(envp->hRendezvousCleanup);

    /* Clean up the thread before exiting */
    if (hBufTab) {
        BufTab_delete(hBufTab);
    }

    if (hVe1) {
        Venc1_delete(hVe1);
    }

    if (hEngine) {
        Engine_close(hEngine);
    }

    return status;
}

