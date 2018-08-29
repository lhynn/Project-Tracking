/*
 * ctrl.c
 *
 * This module handles the Remote Control commands and Processes Key Actions
 * for DM365 platform.
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
 *
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <xdc/std.h>
#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/dmai/Cpu.h>
#include <ti/sdo/dmai/Pause.h>
#include <ti/sdo/dmai/Rendezvous.h>
#include <ti/sdo/dmai/Capture.h>
#include "ctrl.h"
#include "demo.h"
#include "ui.h"
#include <time.h>
#include <dirent.h>
#include <sys/vfs.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <net/if.h>
#include <netinet/in.h>
#include <pthread.h>

 
/* How often to poll for new keyboard commands */
#define CONTROLLATENCY  5000 //10000   //600000 //  UNIT: us

/* Keyboard command prompt */
#define COMMAND_PROMPT       "Command [ 'help' for usage ] > "

/* Maximum length of a keyboard command */
#define MAX_CMD_LENGTH       80



//unsigned char 	SYSTEM_STATUS; //0-stop 1-play
int             gl_encode_status = ENCODE_STATUS_STOP;
unsigned int    gl_osd_photo;
int     		capture_fd;
unsigned int    gl_system_command = 0xffff;
int  			s32MainFd;
VideoStd_Type   global_VideoStd;
char 			videofile[64];



/* Structure containing the state of the OSD */
typedef struct OsdData {
    Int           time;
    ULong         firstTime;
    ULong         prevTime;
    Int           samplingFrequency;
    Int           imageWidth;
    Int           imageHeight;
} OsdData;

pthread_mutex_t mutex_buffer;

/* Initial values of osd data structure */
#define OSD_DATA_INIT        { -1, 0, 0, 0, 0, 0 }


/******************************************************************************
 * drawDynamicData
 ******************************************************************************/
static Void drawDynamicData(Engine_Handle hEngine, Cpu_Handle hCpu,
                            UI_Handle hUI, OsdData *op)
{
    Char                  tmpString[20];
    struct timeval        tv;
    struct tm            *timePassed;
    time_t                spentTime;
    ULong                 newTime;
    ULong                 deltaTime;
    Int                   armLoad;
    Int                   fps;
    Int                   videoKbps;
    Int                   soundKbps;
    Float                 fpsf;
    Float                 videoKbpsf;
    Float                 soundKbpsf;
    Int                   imageWidth;
    Int                   imageHeight;
    Int                   freq;
    Int                   khz;
    Int                   decimal;

    op->time = -1;

    if (gettimeofday(&tv, NULL) == -1) {
        ERR("Failed to get os time\n");
        return;
    }

    newTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    if (!op->firstTime) {
        op->firstTime = newTime;
        op->prevTime = newTime;
        return;
    }

    /* Only update user interface every second */
    deltaTime = newTime - op->prevTime;
    if (deltaTime <= 1000) {
        return;
    }

    op->prevTime = newTime;

    spentTime = (newTime - op->firstTime) / 1000;
    if (spentTime <= 0) {
        return;
    }

    op->time     = spentTime;

    /* Calculate the frames per second */
    fpsf         = gblGetAndResetFrames() * 1000.0 / deltaTime;
    fps          = fpsf + 0.5;

    /* Calculate the video bit rate */
    videoKbpsf   = gblGetAndResetVideoBytesProcessed() * 8.0 / deltaTime;
    videoKbps    = videoKbpsf + 0.5;

    /* Calculate the audio or speech bit rate */
    soundKbpsf   = gblGetAndResetSoundBytesProcessed() * 8.0 / deltaTime;
    soundKbps    = soundKbpsf + 0.5;

    /* Get the local ARM cpu load */
    if (Cpu_getLoad(hCpu, &armLoad) < 0) {
        armLoad = 0;
        ERR("Failed to get ARM CPU load\n");
    }

    timePassed = localtime(&spentTime);
    if (timePassed == NULL) {
        return;
    }

    /* Update the UI */
    sprintf(tmpString, "%.2d:%.2d:%.2d", timePassed->tm_hour,
                                         timePassed->tm_min,
                                         timePassed->tm_sec);

    UI_updateValue(hUI, UI_Value_Time, tmpString);

    freq = gblGetSamplingFrequency();

    if (freq != op->samplingFrequency) {
        khz = freq / 1000;
        decimal = (freq % 1000) / 100; /* Only save one decimal */

        if (decimal) {
            sprintf(tmpString, "%d.%d KHz samp rate", khz, decimal);
        }
        else {
            sprintf(tmpString, "%d KHz samp rate", khz);
        }

        UI_updateValue(hUI, UI_Value_SoundFrequency, tmpString);
    }

    imageWidth = gblGetImageWidth();
    imageHeight = gblGetImageHeight();

    if (imageWidth != op->imageWidth ||
        imageHeight != op->imageHeight) {

        sprintf(tmpString, "%dx%d", imageWidth, imageHeight);

        UI_updateValue(hUI, UI_Value_ImageResolution, tmpString);

        op->imageWidth = imageWidth;
        op->imageHeight = imageHeight;
    } 

    sprintf(tmpString, "%d%%", armLoad);
    UI_updateValue(hUI, UI_Value_ArmLoad, tmpString);


    sprintf(tmpString, "%d fps", fps);
    UI_updateValue(hUI, UI_Value_Fps, tmpString);

    sprintf(tmpString, "%d kbps", videoKbps);
    UI_updateValue(hUI, UI_Value_VideoKbps, tmpString);

    sprintf(tmpString, "%d kbps", soundKbps);
    UI_updateValue(hUI, UI_Value_SoundKbps, tmpString);

    UI_update(hUI);
}



/******************************************************************************
 * keyAction
 ******************************************************************************/
static Int keyAction(UI_Key key, UI_Handle hUI, Pause_Handle hPauseProcess)
{
    switch(key) {
#ifdef APP_DEC
        case UI_Key_PLAY:
			if(gl_decode_pause == 1)
			{
				decode_snd_resume();
	 			Pause_off(hPauseProcess);
				gl_decode_pause = 0;
			}
		break;

        case UI_Key_PAUSE:
			if(gl_decode_pause == 0)
			{
				decode_snd_pause();
		        Pause_on(hPauseProcess);
				gl_decode_pause = 1;
		        UI_pause(hUI);
			}
            break;
#endif

        case UI_Key_RECORD:
			if(gl_system_status == STATUS_STOP)
			{
				if(1 == gl_sdcard_exist) 
				{
					if(gl_max_avi >= AVI_MAX_CNT)
					{
						gl_osd_avimax = 1;
					}
					else
					{
						video_command = CMD_REC;
						if(gl_capture_fd)
						{
							ioctl(gl_capture_fd, VPFE_CMD_Capture_led_play);
						}
						printf("[encode] start record command\r\n");
					}
				}
			}
			
            Pause_off(hPauseProcess);
            //UI_play(hUI);
            break;

		case UI_Key_PHOTO:
			if(1 == gl_sdcard_exist) 
			{
				if(gl_max_bmp >= BMP_MAX_CNT)
				{
					gl_osd_bmpmax = 1;
				}
				else
				{
					gl_osd_bmpmax = 0;
					gl_get_picture = 1;
					printf("[encode] get photo command\r\n");
				}
			}
			break;

        case UI_Key_STOP:
            //UI_stop(hUI);
            //gblSetQuit();
#ifdef APP_ENC
			if(gl_system_status == STATUS_REC)
			{
				video_command = CMD_STOP;
				if(gl_capture_fd)
				{
					ioctl(gl_capture_fd, VPFE_CMD_Capture_led_stop);
				}
				printf("[encode] stop record command\r\n");
			}
#endif
            break;

		case UI_Key_EXIT:
#ifdef APP_ENC
			if(gl_system_status == STATUS_REC)
			{
				video_command = CMD_STOP;
				if(gl_capture_fd)
				{
					ioctl(gl_capture_fd, VPFE_CMD_Capture_led_stop);
				}
				printf("[encode] stop record command\r\n");
			}
#endif
			gblSetQuit();
			break;
			case  UI_Key_HIDE:
					gl_screen_div=1;
			break;
			case  UI_Key_DEC :
					  gl_screen_div=0;
			break;
        default:
            break;
    }

    return SUCCESS;
}



unsigned int CheckoutDiskSpace(void)
{	
    struct statfs diskInfo;
    unsigned long long totalBlocks;
    unsigned long long totalSize;
    size_t mbTotalsize;
    unsigned long long freeDisk;
    size_t mbFreedisk;

	statfs("/home/root/media", &diskInfo);
	totalBlocks = diskInfo.f_bsize;
	totalSize = totalBlocks*diskInfo.f_blocks;
	mbTotalsize = totalSize >> 20;//compute the size by unit MB, 20 = 1MB 
	freeDisk = diskInfo.f_bfree*totalBlocks;
	mbFreedisk = freeDisk >> 20;
//	printf("/dev/sda1 total = %dMB, free = %dMB\n", mbTotalsize, mbFreedisk);
	return mbFreedisk;
}



void handle()
{
	printf ("=========handle===== \r\n");
}

/******************************************************************************
 * ctrlThrFxn
 ******************************************************************************/
Void *ctrlThrFxn(Void *arg)
{
    CtrlEnv                *envp                = (CtrlEnv *) arg;
    Void                   *status              = THREAD_SUCCESS;
//    OsdData                 osdData             = OSD_DATA_INIT;
    Cpu_Attrs               cpuAttrs            = Cpu_Attrs_DEFAULT;
    Engine_Handle           hEngine             = NULL;
    Cpu_Handle              hCpu                = NULL;
	
	int i;
    UI_Key                  key;
//	unsigned int ret;
//	pthread_t thread;
//	pthread_t threadkey;
	struct sigaction  newact;
	
//	char shellcmd[64];

	gl_decode_pause = 0;

	gl_sdcard_full = 0;
	gl_zoom_size   = ZOOM_1X;
	gl_get_picture = 0;
	gl_disp_type   = 0;
	gl_disp_invert = 0;
	gl_adcvalue    = 100;

    newact.sa_handler = handle;
    newact.sa_flags    = 0;
    if(sigaction(SIGPIPE , &newact ,NULL) == -1)
    {
         printf("### SIGPIPE ### \r\n");
    }

//	pthread_create(&threadkey, NULL, poll_key, 0);
//	pthread_create(&thread, NULL,  command_server, 0);


    /* Open the codec engine */
    hEngine = Engine_open(envp->engineName, NULL, NULL);

    if (hEngine == NULL) {
        ERR("Failed to open codec engine %s\n", envp->engineName);
        cleanup(THREAD_FAILURE);
    }

    /* Create the Cpu object to obtain ARM cpu load */
    hCpu = Cpu_create(&cpuAttrs);

    if (hCpu == NULL) {
        ERR("Failed to create Cpu Object\n");
    }

    /* Signal that initialization is done and wait for other threads */
    Rendezvous_meet(envp->hRendezvousInit);

    if (envp->keyboard) {
        printf(COMMAND_PROMPT);
        fflush(stdout);
    }

///////get no used cmd from fifo////

	for(i=0; i<50; i++)
	{
		if (UI_getCmd(envp->hUI, &key) == FAILURE) 
		{
		    ERR("Failed to get cmd.\n");
		    cleanup(THREAD_FAILURE);
		}
		//else
		//{
		//	printf("=====Get invalid key command from fifo=====\r\n");
		//}
	}


#ifdef APP_DEC
	printf("\r\n");
	printf("##################################\r\n");
	printf("#       DECODE FOR AVI V2.0     #\r\n");
    printf("#   BUILD DATA:%s TIME:%s       #\r\n",__DATE__, __TIME__);
	printf("##################################\r\n");
	printf("\r\n");
#endif


#ifdef APP_ENC
	printf("\r\n");
	printf("##################################\r\n");
	printf("#       ENCODE FOR AVI V2.0      #\r\n");
    printf("#    BUILD DATA:%s TIME:%s       #\r\n",__DATE__, __TIME__);
#ifdef FUN_RTSP
	printf("#         RTSP Server OPEN       #\r\n");
#endif
	printf("##################################\r\n");
	printf("\r\n");


//	gl_system_status =  STATUS_STOP; //STATUS_REC;
//	video_command    =  CMD_STOP;    //CMD_PLAY;

//	gl_system_status =  STATUS_STOP;
//	video_command    =  CMD_REC;

////////////////////////////////////////////////////////////////////////////////
/*
	if(gl_system_status == STATUS_STOP)
		video_command = CMD_REC;
	if(gl_capture_fd)
		ioctl(gl_capture_fd, VPFE_CMD_Capture_led_play);

	printf("\r\nSTART REC command\r\n");
*/
////////////////////////////////////////////////////////////////////////////////
#endif



    while (!gblGetQuit()) 
	{
#ifdef APP_ENC

#ifdef FUN_RTSP
		EventLoop(s32MainFd);
#endif

#endif
        if (UI_getCmd(envp->hUI, &key) == FAILURE) {
                ERR("Failed to get cmd.\n");
                cleanup(THREAD_FAILURE);
        }

        if (key != 0) {
            if (keyAction(key, envp->hUI, envp->hPauseProcess) == FAILURE) {
                ERR("keyAction failed.\n");
                cleanup(THREAD_FAILURE);
            }
        }
       
        usleep(CONTROLLATENCY);
    }

cleanup:
    /* Make sure the other threads aren't waiting for us */
    Rendezvous_force(envp->hRendezvousInit);
    Pause_off(envp->hPauseProcess);

    /* Meet up with other threads before cleaning up */
    Rendezvous_meet(envp->hRendezvousCleanup);

    /* Clean up the thread before exiting */
    if (hCpu) {
        Cpu_delete(hCpu);
    }

    if (hEngine) {
        Engine_close(hEngine);
    }
	
    return status;
}

