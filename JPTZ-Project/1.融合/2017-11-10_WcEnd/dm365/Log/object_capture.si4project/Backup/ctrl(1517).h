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
#include <ti/sdo/dmai/Display.h> 

#include "ui.h"
#include "encodets/searchdir.h"
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
	Display_Handle          hdisplay;
} CtrlEnv;

/* Thread function prototype */
extern Void *ctrlThrFxn(Void *arg);

#define SPACE_MIN_LIMIT  10

#define  BMP_FILE

#define SCREEN_WIDTH         736//720 //800 
#define SCREEN_HEIGHT        576 //576 //600 

#define RSZ_WIDTH            736 //SCREEN_WIDTH 
#define RSZ_HEIGHT           576 //SCREEN_HEIGHT

#define DISP_WIDTH           736
#define DISP_HEIGHT          576

#define YUV_WIDTH            SCREEN_WIDTH 
#define YUV_HEIGHT           SCREEN_HEIGHT

#define AVI_WIDTH            SCREEN_WIDTH  
#define AVI_HEIGHT           SCREEN_HEIGHT

#define AVI_FRAMERATE        25
#define VIDEO_FILE_LENGTH    25*60*10        //54000

#define CMD_REC              0xAA
#define CMD_STOP             0x55
#define CMD_NONE             0xFF

#define STATUS_STOP          0x00
#define STATUS_REC           0x01

#define ZOOM_1X              0x01
#define ZOOM_2X              0x02
#define ZOOM_4X              0x03

typedef struct UsartInfoSt{
	int powerInfo;
	int LeftKeyInfo;
	int RightKeyInfo;
	int LeftRoll;
	int RightRoll;
	int ShowMode;
	int ScreenState;
	int EnlargementFactor; 
}P_UsartInfoSt;

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
int  gl_adcvalue;
int  gl_sdcard_full;

int gl_carvalue;
int gl_speed;

int gl_cam_time;

P_UsartInfoSt glUsartInfoSt;
int gl_ShowPicFlag;
int gl_ShowVideoFlag;

struct llist_node_st *ShowImageNode;
LLIST *gl_MyBmpLList;
LLIST *gl_MyH264LList;

int gl_light;

#endif /* _CTRL_H */
