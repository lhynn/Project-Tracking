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
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>



#include <stdio.h>
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

#include "uart.h"
#include <time.h>

#include <dirent.h>
#include <sys/vfs.h>
#include "encodets/decode.h"

#define SYSTEM_MODULE          0x01c40000  
#define GPIO_BASE_ADDR         0x01c67000

#define GPIO_DIR01_OFFSET      0x10
#define GPIO_OUT01_OFFSET      0x14
#define GPIO_SET01_OFFSET      0x18
#define GPIO_CLR01_OFFSET      0x1C
#define GPIO_INPUT01_OFFSET    0x20

#define GPIO_DIR23_OFFSET      0x38
#define GPIO_OUT23_OFFSET      0x3C
#define GPIO_SET23_OFFSET      0x40
#define GPIO_CLR23_OFFSET      0x44
#define GPIO_INPUT23_OFFSET    0x48

#define GPIO_DIR45_OFFSET      0x60
#define GPIO_SET45_OFFSET      0x68
#define GPIO_INPUT45_OFFSET    0x70   
 
/* How often to poll for new keyboard commands */
#define CONTROLLATENCY  5000 //600000   UNIT: us

/* Keyboard command prompt */
#define COMMAND_PROMPT       "Command [ 'help' for usage ] > "

/* Maximum length of a keyboard command */
#define MAX_CMD_LENGTH       80

#define VPFE_CMD_Capture_led_play _IOR('V', BASE_VIDIOC_PRIVATE + 3, \
					void *)
#define VPFE_CMD_Capture_led_stop _IOR('V', BASE_VIDIOC_PRIVATE + 4, \
					void *)


///////////////////hwx
#define BURST_LEN 128

#define IDLE 0
#define PRE1 1
#define PRE2 2
#define PRE3 3
#define CMD 4
#define TNUM 5
#define SNO1 6
#define SNO2 7
#define SEV1 8
#define SEV2 9
#define SEV3 10
#define SEV4 11
#define SEV5 12
#define SEV6 13
#define CKSUM 14
///////////////////xwh
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
extern int led_set(int bit_mask, unsigned char onoff);

extern int capture_fd;

unsigned char uart_command = 0;
unsigned char SYSTEM_STATUS = 0; //0-stop 1-play


#define CMDLEN  128
#define STXCODE 0xAA
/**************************************************************************
int process_uartcmd(Pause_Handle hPauseProcess,Display_Handle	hdisplay)
{
    int ret;
    char serbuf[CMDLEN]={0};

    ret = read(fd_com, serbuf, CMDLEN);
    if(serbuf[0] == 0x3e) //start code
    {
        printf("get cmd OK\r\n");
        if(serbuf[2]==0x01)
        {
			if(serbuf[3]==0x01) 
			{
				gl_get_picture = 1;
				gl_picture_osd = 50;
				printf("\r\nPHOTO command\r\n");

			}else if(serbuf[3]==0x02)
			{
					printf("\r\n change status! \n");	
						if(gl_system_status == STATUS_STOP)
						{
								video_command = CMD_REC;
								printf("\r\nSTART REC command\r\n");	
							if(gl_capture_fd)
							ioctl(gl_capture_fd, VPFE_CMD_Capture_led_play);
			
						}else if(gl_system_status == STATUS_REC)
						{			
							video_command = CMD_STOP;							
							printf("\r\nSTOP REC command\r\n");
							if(gl_capture_fd)
								ioctl(gl_capture_fd, VPFE_CMD_Capture_led_stop);
						}
			}
        }else if(serbuf[2]==0x02)
        	{
					gl_adcvalue = serbuf[5];
					if(gl_adcvalue > 100)
						gl_adcvalue = 100;
					if(gl_adcvalue < 0)
						gl_adcvalue = 0;

						gl_carvalue = serbuf[4];
					if(gl_carvalue > 100)
						gl_carvalue = 100;
					if(gl_carvalue < 0)
						gl_carvalue = 0;

					
						gl_speed = serbuf[3];
						gl_light = serbuf[6];
			
        	}else if(serbuf[2]==0x03) //ֹͣcap diplay 
        	{
				Pause_on(hPauseProcess);
				printf("Pause_on \n");

        	}else if(serbuf[2]==0x04)
        	{
				Pause_off(hPauseProcess);
				printf("Pause_off \n");
        	}
			else if(serbuf[2]==0x05)
			{
				
				}
    	}
	return 0;
}
***************************************************************************/
int process_uartcmd(Pause_Handle hPauseProcess,Display_Handle	hdisplay)
{
    int ret;
    char serbuf[CMDLEN]={0};

    ret = read(fd_com, serbuf, CMDLEN);
   	if(serbuf[0] == 0x3e){ //start code
			
		glUsartInfoSt.powerInfo    		= serbuf[1];
		glUsartInfoSt.LeftKeyInfo  		= serbuf[2]&0x03;
		glUsartInfoSt.RightKeyInfo 		= (serbuf[2]&0x0C) >> 2;
		glUsartInfoSt.LeftRoll	  		= (serbuf[2]&0x10) >> 4;
		glUsartInfoSt.RightRoll			= (serbuf[2]&0x20) >> 5;
		glUsartInfoSt.ShowMode			= (serbuf[2]&0x40) >> 6;
		glUsartInfoSt.ScreenState   	= (serbuf[2]&0x80) >> 7;
		glUsartInfoSt.EnlargementFactor = serbuf[3];
	}
	printf("powerInfo ：%d LeftKeyInfo %d RightKeyInfo：%d LeftRoll：%d RightRoll:%d,ShowMode:%d ScreenState %d,EnlargementFactor：%d \n",glUsartInfoSt.powerInfo,glUsartInfoSt.LeftKeyInfo,\
			glUsartInfoSt.RightKeyInfo,glUsartInfoSt.LeftRoll,glUsartInfoSt.RightRoll,glUsartInfoSt.ShowMode,\
			glUsartInfoSt.ScreenState,glUsartInfoSt.EnlargementFactor);
	return 0;
}


/******************************************************************************
 * cmd_content_save-------cmd_content)
 ******************************************************************************/
unsigned int cmd_change_cmd_save(unsigned char *desc, unsigned char *source, int cmd_length)
{
	int m = 0;
	for(m = 0; m < cmd_length ;m++)
	{
		if((*(source + m) >> 4) < 10)
			*(desc + 3*m) = (*(source + m) >> 4) + '0';
		else//
			*(desc + 3*m) = (*(source + m) >> 4) - 10 + 'A';//

		if((*(source + m) & 0x0F) < 10)
			*(desc + 3*m +1) = (*(source + m ) & 0x0F) + '0';
		else
			*(desc + 3*m +1) = (*(source + m )& 0x0F) - 10 + 'A';

		*(desc + 3*m + 2) = ' ';
	}

	return (3*cmd_length);
}

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
 * getKbdCommand
 ******************************************************************************/
Int getKbdCommand(UI_Key *keyPtr)
{
    struct timeval tv;
    fd_set         fds;
    Int            ret;
    Char           string[MAX_CMD_LENGTH];

    FD_ZERO(&fds);
    FD_SET(fileno(stdin), &fds);

    /* Timeout of 0 means polling, we don't want to block */
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    ret = select(FD_SETSIZE, &fds, NULL, NULL, &tv);

    if (ret == -1) {
        ERR("Select failed on stdin\n");
        return FAILURE;
    }

    if (ret == 0) {
        return SUCCESS;
    }

    if (FD_ISSET(fileno(stdin), &fds)) {
        if (fgets(string, MAX_CMD_LENGTH, stdin) == NULL) {
            return FAILURE;
        }

        /* Remove the end of line */
        strtok(string, "\n");

        /* Assign corresponding key */
        if (strcmp(string, "play") == 0) {									
            *keyPtr = UI_Key_PLAY;
        }
        else if (strcmp(string, "pause") == 0) {
            *keyPtr = UI_Key_PAUSE;
        }
        else if (strcmp(string, "stop") == 0) {
            *keyPtr = UI_Key_STOP;
        }
        else if (strcmp(string, "inc") == 0) {
            *keyPtr = UI_Key_DEC;
        }
        else if (strcmp(string, "dec") == 0) {
            *keyPtr = UI_Key_INC;
        }
        else if (strcmp(string, "hide") == 0) {
            *keyPtr = UI_Key_HIDE;
        }
        else if (strcmp(string, "help") == 0) {
            printf("\nAvailable commands:\n"
                   "    play  - Play / record video and sound\n"
                   "    pause - Pause video and sound\n"
                   "    stop  - Quit demo\n"
                   "    inc   - Increase OSD visibility\n"
                   "    dec   - Decrease OSD visibility\n"
                   "    hide  - Show / hide the OSD\n"
                   "    help  - Show this help screen\n\n");
        }
        else {
            printf("Unknown command: [ %s ]\n", string);
        }
        if (*keyPtr != UI_Key_STOP) {
            printf(COMMAND_PROMPT);
            fflush(stdout);
        }
        else {
            printf("\n");
        }
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

unsigned char save_A_cmd = 0;
unsigned char save_B_cmd = 0;
unsigned char save_C_cmd = 0;
unsigned char save_Video = 0;
unsigned char file_save_status = 0;

int gps_file_fd = 0;

int frame_count = 0;


#define PRINTOUT 1


int j;
unsigned char data_buf[256];
int uart_data_rec;

int verify(unsigned char cmdl[15])
{
    int i;

    for (i = 0; i < 15; i++)
    {
        printf("%02x ", cmdl[i]);
    }
    printf("\r\n");
    return 1;
}

int do_reccmd(unsigned char cmdl[15], unsigned char cmdb)
{
    if (cmdb == 0x03)
    {
		if (SYSTEM_STATUS == 0)
		{
		    SYSTEM_STATUS = 1;
			uart_command = 0xAA;
			ioctl(capture_fd, VPFE_CMD_Capture_led_play);
		}
    }
    else if (cmdb == 0x04)
    {
		if (SYSTEM_STATUS == 1)
		{
		    SYSTEM_STATUS = 0;
			uart_command = 0x55;
			ioctl(capture_fd, VPFE_CMD_Capture_led_stop);
		}
    }
    else if (cmdb == 0x01)
    {
		if(1 == gl_sdcard_exist && gps_file_fd == 0)
		{
			printf("open file\r\n");
			gps_file_fd = open("/home/root/media/uart_data_file", O_RDWR | O_CREAT | O_APPEND);	
		}
		uart_data_rec = 1;
		memcpy(data_buf, cmdl, 15);
		j = 15;
    }
    else if (cmdb == 0x02)
    {
		if (gps_file_fd > 0)
		{
			printf("close file\r\n");
			write(gps_file_fd, data_buf, j);
			close(gps_file_fd);
			gps_file_fd = 0;
		}
		uart_data_rec = 0;
		j = 15;
    }
    return 0;
}

int deal(unsigned char cmdl[15], int uart_fd)
{
    unsigned char resp[15] = {0};

    memset(resp, 0, 15);
    resp[0] = 0x68;
    resp[1] = 0x66;
    resp[2] = 0x00;
    
    if (cmdl[3] == 0x0A)
    {
        resp[3] = 0x0A;
        resp[4] = cmdl[4];
        resp[5] = cmdl[5];
        resp[6] = cmdl[6];
        resp[7] = cmdl[7];
        resp[14] = 0xAB;
        do_reccmd(cmdl, cmdl[4]);
        write(uart_fd, resp, 15);
    }
    else if (cmdl[3] == 0x01)
    {
        resp[3] = 0x02;
        resp[4] = 0x01;
        resp[5] = 0x00;
        resp[6] = 0xAB;
        write(uart_fd, resp, 7);
    }
    else 
        return -1;
	return 0;
    
}


/******************************************************************************
 * ctrlThrFxn
 ******************************************************************************/
Void *ctrlThrFxn(Void *arg)
{
    CtrlEnv                *envp                = (CtrlEnv *) arg;
    Void                   *status              = THREAD_SUCCESS;
    OsdData                 osdData             = OSD_DATA_INIT;
    Cpu_Attrs               cpuAttrs            = Cpu_Attrs_DEFAULT;
    Engine_Handle           hEngine             = NULL;
    Cpu_Handle              hCpu                = NULL;



	pthread_mutex_init(&mutex_buffer, NULL);

	fd_com = check_port_open("/dev/ttyS1", 115200);   
	clear_send(fd_com);

    /* Open the codec engine */
    hEngine = Engine_open(envp->engineName, NULL, NULL);

    if (hEngine == NULL)
	{
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

    if (envp->keyboard) 
	{
        printf(COMMAND_PROMPT);
        fflush(stdout);
    }

	gl_system_status = STATUS_STOP; //STATUS_REC;
	video_command	 = CMD_NONE;	//CMD_PLAY;
    while (!gblGetQuit()) 
	{

   		process_uartcmd(envp->hPauseProcess,envp->hdisplay);
		if(gl_system_status == STATUS_REC)	//play
		{
//			system("sync");  //file write sync control!!!!!!
#ifdef PRINTOUT
//			printf("Playing Sync......\r\n");
#endif
			if( CheckoutDiskSpace() < 30) //////not enough disk space
			{
				printf("Not enough disk space, send stop command!\r\n");
				gl_sdcard_full = 1;
				gl_system_status = STATUS_STOP;
				video_command    = CMD_STOP;	
			}
		}

        /* Update the dynamic data, either on the OSD or on the console */
		drawDynamicData(hEngine, hCpu, envp->hUI, &osdData);

        /* Wait a while before polling the keyboard again */
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

