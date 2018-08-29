/*
 * ctrl.h
 *
 * This module handles the definitions and declarations for 
 * Remote Control commands and Processes Key Actions for DM365 platform.
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

#ifndef _CTRL_H
#define _CTRL_H

#include <xdc/std.h>
#include <ti/sdo/dmai/Pause.h>
#include <ti/sdo/dmai/Rendezvous.h>
#include <ti/sdo/dmai/Capture.h>
#include <ti/sdo/dmai/Fifo.h>
#include <ti/sdo/dmai/ce/Vdec2.h>
#include <ti/sdo/dmai/BufTab.h>

#include "ui.h"

#define CMD_GET_PIC    0x3100
#define CMD_START_REC  0x3200
#define CMD_STOP_REC   0x3300
#define CMD_TRIG_PIC   0x3400
#define CMD_OPEN_DIR   0x3500
#define CMD_KEY_UP     0x3F01
#define CMD_KEY_DOWN   0x3F02
#define CMD_KEY_ENTER  0x3F05
#define CMD_KEY_EXIT   0x3F07


#define VPFE_CMD_Capture_led_play  _IOR('V', BASE_VIDIOC_PRIVATE + 3, void *)
#define VPFE_CMD_Capture_led_stop  _IOR('V', BASE_VIDIOC_PRIVATE + 4, void *)
#define VPFE_CMD_OLED_ON           _IOR('V', BASE_VIDIOC_PRIVATE + 5, void *)
#define VPFE_CMD_OLED_OFF          _IOR('V', BASE_VIDIOC_PRIVATE + 6, void *)
#define VPFE_CMD_OLED_BRIGHT_UP    _IOR('V', BASE_VIDIOC_PRIVATE + 7, void *)
#define VPFE_CMD_OLED_BRIGHT_DOWN  _IOR('V', BASE_VIDIOC_PRIVATE + 8, void *)

/* Defining infinite time */
#define FOREVER         -1

/* Environment passed when creating the thread */
typedef struct CtrlEnv {
    UI_Handle               hUI;
    Rendezvous_Handle       hRendezvousInit;
    Rendezvous_Handle       hRendezvousCleanup;
    Pause_Handle            hPauseProcess;
    Int                     keyboard;
    Int                     time;
    Char                   *engineName;
    Bool                    osd;
} CtrlEnv;

/* Thread function prototype */
extern Void *ctrlThrFxn(Void *arg);

#define AVI_MIN_LIMIT  100
#define BMP_MIN_LIMIT  50
#define JPG_MIN_LINIT  50


//#define  BMP_FILE
#define  JPEG_FILE

#define SCREEN_WIDTH         720 //720 //800 
#define SCREEN_HEIGHT        576 //576 //600 

#define RSZ_WIDTH            720//736 //SCREEN_WIDTH 
#define RSZ_HEIGHT           576 //SCREEN_HEIGHT

#define DISP_WIDTH           736
#define DISP_HEIGHT          576

#define JPG_WIDTH   736
#define JPG_HEIGHT  576

#define YUV_WIDTH            SCREEN_WIDTH 
#define YUV_HEIGHT           SCREEN_HEIGHT

#define AVI_WIDTH            SCREEN_WIDTH  
#define AVI_HEIGHT           SCREEN_HEIGHT

#define AVI_FRAMERATE        25
#define VIDEO_FILE_LENGTH    25*60*10 //25*60*10        //test 1 min

#define CMD_REC              0xAA
#define CMD_STOP             0x55
#define CMD_NONE             0xFF

#define STATUS_STOP          0x00
#define STATUS_REC           0x01

#define ZOOM_1X              0x01
#define ZOOM_2X              0x02
#define ZOOM_4X              0x03

#define ENCODE_STATUS_STOP      0
#define ENCODE_STATUS_REC       1


#define BMP_MAX_CNT         99999
#define AVI_MAX_CNT         99999
#define JPG_MAX_CNT         99999


//#define FUN_RTSP                1
#define H264_FIFO_OUT   1
//#define H264_SHMEM_OUT 1

int  fd_com;
int  fd_gpio;
int  gl_get_picture;
int  gl_picture_osd;
int  video_command;
int  gl_system_status;
int  gl_delete_file;
int  gl_capture_fd;
int  gl_sdcard_exist;
int  s32MainFd;

int  idr_trigger;
int  video_frame_count;
int  gl_frame_count;
int  idr_frame_promote;
int  gl_zoom_size;
int  gl_adcvalue;
int  gl_disp_type;
int  gl_lcd_on;
int  gl_sdcard_full;
int  gl_disp_invert;

int  gl_decode_pause;
int  gl_decode_frame;
int  gl_screen_div;

char gl_filename[100];
char gl_bmpname[100];
char gl_jpgname[100];

int  gl_osd_bmpmax;
int  gl_osd_avimax;
int  gl_osd_jpgmax;

unsigned int  gl_max_bmp;
unsigned int  gl_max_avi;
unsigned int  gl_max_jpg;

int pipe_fd;
int firstjpg;
int I_CutFormat;

Buffer_Handle		  glob_hSrcBuf;
pthread_t 			  display_mutex;
int 				  glob_hSrcBuf_falg;


//extern pthread_mutex_t mutex_buffer;

#endif /* _CTRL_H */
