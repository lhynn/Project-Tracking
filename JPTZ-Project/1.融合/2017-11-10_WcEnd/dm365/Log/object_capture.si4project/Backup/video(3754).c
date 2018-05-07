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


#include "video.h"


#include <sys/socket.h>
#include <netinet/in.h>
#include "config.h"

#include <sys/types.h>  
#include <sys/stat.h>  
#include <string.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <limits.h>  
#include <errno.h>  
#include <sys/shm.h>
#include <stdio.h>

#include "../demo.h"
#include "../ctrl.h"
#include <sys/types.h>
#include <dirent.h>


#define VPFE_CMD_Capture_led_play _IOR('V', BASE_VIDIOC_PRIVATE + 3, void *)
#define VPFE_CMD_Capture_led_stop _IOR('V', BASE_VIDIOC_PRIVATE + 4, void *)


#ifndef YUV_420SP
#define YUV_420SP 256
#endif 

/* Number of buffers in the pipe to the capture thread */
/* Note: It needs to match capture.c pipe size */
#define VIDEO_PIPE_SIZE           4 

extern int led_set(int bit_mask, unsigned char onoff);
extern unsigned char uart_command;
extern unsigned char SYSTEM_STATUS; //0-stop 1-play
extern int capture_fd;



#define FIFO_NAME   "/tmp/h264fifo"
#define BUFFERSIZE   2048*1024   //1024*1024//PIPE_BUF 

//#define H264_FIFO_OUT   1

#define SEND_H264_UDP_NET 0
//#define SEND_H264_3G_NET    1




int      sinlen;
unsigned int idrstart = 0;
unsigned int udpconnect = 0;
unsigned int udp3g_connect = 0;

int    sockfd;          //this socket used for send h.264 data
struct sockaddr_in addr_client;
struct sockaddr_in addr_server;



int    sockfd_3g;       //this socket used for send h.264 data 3g net
struct sockaddr_in addr_3gclient;
struct sockaddr_in addr_3gserver;




extern VideoStd_Type         vidStd;

int pipe_fd;



struct shared_use_st
{
	int written;
	unsigned int buflen;
	char buffer[BUFFERSIZE];
};




///////////////////////
unsigned int create_filename(char* filestr, const char* ext)
{
	DIR *dir;
	struct dirent *ptr;

	unsigned int file_max_count = 0;
	unsigned int current_file_count = 0;
	char filename[64];
	char strbuf[10];
	
	dir = opendir("/home/root/media/");
	while( (ptr = readdir(dir)) != NULL)
	{
		memset(filename, 0, 64);
		memcpy(filename, ptr->d_name, 64); 

		memset(strbuf, 0, 10);
		memcpy(strbuf, filename, 4);

		if(strcmp("avi", ext) == 0)  //a avi file
		{
			if( strcmp("AVI_", strbuf) == 0) //test first 8 byte of filename
			{
				current_file_count = (filename[4]-'0')*1000 + (filename[5]-'0')*100 + (filename[6]-'0')*10 + (filename[7]-'0');

				if(current_file_count > file_max_count)
				{
					file_max_count = current_file_count;
				}
			}
		}
		else if(strcmp("jpg", ext) == 0)  //a jpg file
		{
			if( strcmp("JPG_", strbuf) == 0) //test first 8 byte of filename
			{
				current_file_count = (filename[4]-'0')*1000 + (filename[5]-'0')*100 + (filename[6]-'0')*10 + (filename[7]-'0');

				if(current_file_count > file_max_count)
				{
					file_max_count = current_file_count;
				}
			}
		}
		else if(strcmp("bmp", ext) == 0)  //a bmp file
		{
			if( strcmp("BMP_", strbuf) == 0) //test first 8 byte of filename
			{
				current_file_count = (filename[4]-'0')*1000 + (filename[5]-'0')*100 + (filename[6]-'0')*10 + (filename[7]-'0');

				if(current_file_count > file_max_count)
				{
					file_max_count = current_file_count;
				}
			}
		}
	}
	close(dir);

	file_max_count += 1;

	if(strcmp("avi", ext) == 0)  //a avi file
	{
		sprintf(filename, "/home/root/media/AVI_%04d.%s", file_max_count, ext);
	}
	else if(strcmp("jpg", ext) == 0)  //a jpg file
	{
		sprintf(filename, "/home/root/media/JPG_%04d.%s", file_max_count, ext);
	}
	else if(strcmp("bmp", ext) == 0)  //a jpg file
	{
		sprintf(filename, "/home/root/media/BMP_%04d.%s", file_max_count, ext);
	}
	memcpy(filestr, filename, strlen(filename));
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
void udp_init(void)
{
	int ret;
	char recvbuf[100];
    unsigned int recvlen = 0;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if (sockfd == -1) 
    {
		printf("socket error\r\n");
		return;
	}
	printf("[1]udp socket ok\r\n");


	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(5131);
	addr_server.sin_addr.s_addr = inet_addr("192.168.1.100");
	bzero(&(addr_server.sin_zero), 8);

	ret = bind(sockfd, (struct sockaddr*)&addr_server, sizeof(struct sockaddr));
	if (ret == -1) 
    {
		printf("bind error");    
		return;
	}
	printf("[2]udp bind ok\r\n");
	
	sinlen = sizeof(struct sockaddr_in);

	udpconnect = 0;

	while(1)
	{
		recvlen = recvfrom(sockfd, recvbuf, 100, 0, (struct sockaddr*)&addr_client, &sinlen);

		idr_trigger = 1;
		udpconnect = 1;

		printf("======udp recvlen = %d ==========\r\n", recvlen);

	}
	return;
}

///////////////////////////////////////////////////////////////////////////////////////
void udp_command_init(void)
{
	int ret;
	char recvbuf[100];
    unsigned int recvlen = 0;

	struct sockaddr_in command_client;
	struct sockaddr_in command_server;

	int sockcmdfd = socket(AF_INET, SOCK_DGRAM, 0);

	if (sockcmdfd == -1) 
    {
		printf("socket error\r\n");
		return;
	}
	printf("[1]udp socket ok\r\n");


	command_server.sin_family = AF_INET;
	command_server.sin_port = htons(5200);
	command_server.sin_addr.s_addr = inet_addr("192.168.1.100");
	bzero(&(command_server.sin_zero), 8);

	ret = bind(sockcmdfd, (struct sockaddr*)&command_server, sizeof(struct sockaddr));
	if (ret == -1) 
    {
		printf("bind error");    
		return;
	}
	printf("[2]udp bind ok\r\n");
	
	sinlen = sizeof(struct sockaddr_in);



///////////////start record at starpup /////
/*
	if (SYSTEM_STATUS == 0)
	{
		printf("Play Command\r\n");
		SYSTEM_STATUS = 1;
		uart_command = 0xAA;
		ioctl(capture_fd, VPFE_CMD_Capture_led_play);

		idr_trigger = 1;
	}
*/
//////////////////////////////////////////////////

	while(1)
	{
		recvlen = recvfrom(sockcmdfd, recvbuf, 100, 0, (struct sockaddr*)&command_client, &sinlen);

		printf("======udp command  recvlen = %d ==========\r\n", recvlen);
		if(recvlen != 0)
		{
			if(1 == gl_sdcard_exist)
			{
				if( (recvbuf[0] ==  0x55) && (recvbuf[1] == 0xaa) )
				{
					if( (recvbuf[2]=='P') && (recvbuf[3]=='L') && (recvbuf[4]=='A') && (recvbuf[5]=='Y') )
					{
						if (SYSTEM_STATUS == 0)
						{
							printf("Play Command\r\n");
							SYSTEM_STATUS = 1;
							uart_command = 0xAA;
							ioctl(capture_fd, VPFE_CMD_Capture_led_play);

							idr_trigger = 1;
						}					
					}
					else if( (recvbuf[2]=='S') && (recvbuf[3]=='T') && (recvbuf[4]=='O') && (recvbuf[5]=='P') )
					{
						if (SYSTEM_STATUS == 1)
						{
							printf("Stop Command\r\n");
							SYSTEM_STATUS = 0;
							uart_command = 0x55;
							ioctl(capture_fd, VPFE_CMD_Capture_led_stop);

							idr_trigger = 0;
						}					
					}
				}
			}
		}
	}
	return;
}


///////////////////////////////////////////////////////////////////////////////////////
void udp_3g_init(void)
{
	int ret;

	sockfd_3g = socket(AF_INET, SOCK_DGRAM, 0);

	if (sockfd_3g == -1) 
    {
		printf("3g socket error\r\n");
		return;
	}
	printf("[1]3g udp socket ok\r\n");


	addr_3gserver.sin_family = AF_INET;
	addr_3gserver.sin_port = htons(5300);    //3g send port
	addr_3gserver.sin_addr.s_addr = inet_addr("192.168.1.100"); //3g local IP addr,need rework
	bzero(&(addr_3gserver.sin_zero), 8);

	char client_ip[16];
	memset(client_ip, 0, 16);

	//read_conf_value("CLIENT_IP", client_ip, "config.ini");
	printf("=====CLIENT_IP:%s\r\n", client_ip);

	addr_3gclient.sin_family = AF_INET;
	addr_3gclient.sin_port = htons(5131);    //3g recv port @ client PC
	addr_3gclient.sin_addr.s_addr = inet_addr(client_ip); //3g client IP addr, need rework
	bzero(&(addr_3gclient.sin_zero), 8);

	printf("[1]3g udp client set ok!\r\n");

	ret = bind(sockfd_3g, (struct sockaddr*)&addr_3gserver, sizeof(struct sockaddr));
	if (ret == -1) 
    {
		printf("3g bind error");    
		return;
	}
	printf("[2]3g udp bind ok\r\n");
	
	sinlen = sizeof(struct sockaddr_in);

	udp3g_connect = 1;

//	while(1)
	{
/*
		recvlen = recvfrom(sockfd_3g, recvbuf, 100, 0, (struct sockaddr*)&addr_3gclient, &sinlen);

		printf("======3g udp recvlen = %d ==========\r\n", recvlen);
		if(recvlen != 0)
		{
			idr_trigger = 1;
			udp3g_connect = 1;
		}
*/
	}
	return;
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
		dynParams->forceFrame = IVIDEO_IDR_FRAME;
	}

	mystatus = VIDENC1_control(enghandle, XDM_SETPARAMS, dynParams, &tEncStatus);

	if(mystatus != VIDENC1_EOK) 
	{
    	VIDENC1_delete(enghandle);
    	return;
	}
}


int SendH264Data(unsigned char *data,unsigned int size)
{
    pipe_fd = open(FIFO_NAME, O_WRONLY);// | O_NONBLOCK);  

    if(pipe_fd == -1)  
    {  
        return 0;  
    }  
	else
	{
//		printf("[RTSPStream] open fifo result = [%d]\n", pipe_fd);  
	}

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
	char                    bmpfilestr[64];
	unsigned char          	*framesource;

    /* Open the codec engine */
    hEngine = Engine_open(envp->engineName, NULL, NULL);

    if (hEngine == NULL) {
        ERR("Failed to open codec engine %s\n", envp->engineName);
        cleanup(THREAD_FAILURE);
    }

    /* In case of HD resolutions the video buffer will be allocated
       by capture thread. */
    if (((envp->imageWidth == VideoStd_720P_WIDTH) && 
        (envp->imageHeight == VideoStd_720P_HEIGHT)) ||
      ((envp->imageWidth == VideoStd_1080I_WIDTH) &&
        (envp->imageHeight == VideoStd_1080I_HEIGHT))) {
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
    if (envp->videoBitRate < 0) {
        /* Variable bit rate */
        params->rateControlPreset = IVIDEO_NONE;
//  		params->rateControlPreset = IVIDEO_STORAGE;
        /*
         * If variable bit rate use a bogus bit rate value (> 0)
         * since it will be ignored.
         */
//       params->maxBitRate        = 3000000;
    }
    else {
        /* Constant bit rate */
        params->rateControlPreset = IVIDEO_STORAGE;
        params->maxBitRate        = 3000000;//envp->videoBitRate;
    }

    params->maxBitRate        = 500000;
    params->rateControlPreset = IVIDEO_NONE;

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
		//if(0 < idr_frame_promote)
		if(idr_frame_promote > 0)
		{
			ForceIDR(hVe1, dynParams, 1);
			idr_frame_promote --;
		}
		else
		{
			ForceIDR(hVe1, dynParams, 0);
		}

if(idr_trigger)
{
		ForceIDR(hVe1, dynParams, 1);
		idr_trigger=0;
}


        /* Encode the video buffer */
        if (Venc1_process(hVe1, hCapBuf, hDstBuf) < 0) {
            ERR("Failed to encode video buffer\n");
            cleanup(THREAD_FAILURE);
        }

		unsigned long vidbuflen = Buffer_getNumBytesUsed(hDstBuf);
		unsigned char* sendptr = Buffer_getUserPtr(hDstBuf);

			if( (gl_get_picture != 0) && (1 == gl_sdcard_exist) )
		{
			gl_get_picture = 0;
#ifdef BMP_FILE
			memset(bmpfilestr, 0, 64);
			create_filename(bmpfilestr, "bmp");
			framesource = Buffer_getUserPtr(hCapBuf);
			printf("open bmp file: %s\r\n", bmpfilestr);
			convert(framesource, bmpfilestr);
#endif
		}

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
        gblIncVideoBytesProcessed(Buffer_getNumBytesUsed(hDstBuf));

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

