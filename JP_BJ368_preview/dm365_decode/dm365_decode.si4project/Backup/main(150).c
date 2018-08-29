/*
 * main.c
 *
 * This source file has the main() for the 'encodedecode demo' on DM365 platform
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <strings.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#include <xdc/std.h>

#include <ti/sdo/ce/trace/gt.h>
#include <ti/sdo/ce/CERuntime.h>

#include <ti/sdo/fc/rman/rman.h>

#include <ti/sdo/dmai/Dmai.h>
#include <ti/sdo/dmai/Fifo.h>
#include <ti/sdo/dmai/Pause.h>
#include <ti/sdo/dmai/Capture.h>
#include <ti/sdo/dmai/BufferGfx.h>
#include <ti/sdo/dmai/Rendezvous.h>

#include "display.h"
#include "video.h"
#include "../ctrl.h"
#include "../demo.h"
#include "avilib.h"

/* The levels of initialization */
#define DISPLAYTHREADCREATED    0x2
#define CAPTURETHREADCREATED    0x4
#define VIDEOTHREADCREATED      0x8


/* Thread priorities */
#define CAPTURE_THREAD_PRIORITY sched_get_priority_max(SCHED_FIFO) - 1
#define AUDIO_THREAD_PRIORITY   sched_get_priority_max(SCHED_FIFO) - 2
#define VIDEO_THREAD_PRIORITY   sched_get_priority_max(SCHED_FIFO) - 1
#define DISPLAY_THREAD_PRIORITY sched_get_priority_max(SCHED_FIFO)

/* Add argument number x of string y */
#define addArg(x, y)                     \
    argv[(x)] = malloc(strlen((y)) + 1); \
    if (argv[(x)] == NULL)               \
        return FAILURE;                  \
    strcpy(argv[(x)++], (y))

typedef struct Args {
    VideoStd_Type           videoStd;
    Char                   *videoStdString;
    Int                     videoBitRate;
    Char                   *videoBitRateString;
    VideoCodec              videoCodec;
    Char                   *videoCodecString;
    Capture_Input           videoInput;
    Int                     passThrough;
    Int                     keyboard;
    Int                     time;
    Int                     osd;
    Int32                   imageWidth;
    Int32                   imageHeight;
} Args;

#define DEFAULT_ARGS \
    { VideoStd_D1_PAL, "D1 PAL", -1, NULL, H264, NULL, \
      Capture_Input_COUNT, FALSE, FALSE, FOREVER, FALSE, 0, 0}

/* Global variable declarations for this application */
GlobalData gbl 	= GBL_DATA_INIT;

pthread_mutex_t avmutex;

avi_t* 			avifile;

char filepath[32] = "AVI_0001.avi"; 
   
/******************************************************************************
 * uiSetup
 ******************************************************************************/
static Void uiSetup(UI_Handle hUI, Args *argsp)
{
    /* Initialize values */
    UI_updateValue(hUI, UI_Value_DemoName, "Encode Decode");
    UI_updateValue(hUI, UI_Value_DisplayType, argsp->videoStdString);

    if (argsp->passThrough) {
        UI_updateValue(hUI, UI_Value_VideoCodec, "N/A");
        UI_setRow(hUI, UI_Value_VideoKbps, UI_Row_NONE);
    }
    else {
        UI_updateValue(hUI, UI_Value_VideoCodec,
                       engine->videoDecoders[argsp->videoCodec].uiString);
    }

    /* No sound support */
    UI_setRow(hUI, UI_Value_SoundCodec, UI_Row_NONE);
    UI_setRow(hUI, UI_Value_SoundKbps, UI_Row_NONE);
    UI_setRow(hUI, UI_Value_SoundFrequency, UI_Row_NONE);
    UI_setRow(hUI, UI_Value_Time, UI_Row_5);
}


/******************************************************************************
 * main
 ******************************************************************************/
Int main(Int argc, Char *argv[])
{
    Args                    args                = DEFAULT_ARGS;
    Uns                     initMask            = 0;
    Int                     status              = EXIT_SUCCESS;
    Pause_Attrs             pAttrs              = Pause_Attrs_DEFAULT;
    Rendezvous_Attrs        rzvAttrs            = Rendezvous_Attrs_DEFAULT;
    Fifo_Attrs              fAttrs              = Fifo_Attrs_DEFAULT;
    UI_Attrs                uiAttrs;

    Rendezvous_Handle       hRendezvousInit     = NULL;
    Rendezvous_Handle       hRendezvousCleanup  = NULL;
	Rendezvous_Handle       hRendezvousLoop     = NULL;
    Pause_Handle            hPauseProcess       = NULL;
    Pause_Handle            hPausePrime         = NULL;
    UI_Handle               hUI                 = NULL;
	Int                     syncCnt             = 0;
    struct sched_param      schedParam;
    Int                     numThreads;
    pthread_t               videoThread;
    pthread_t               displayThread;
    //pthread_t               captureThread;
    pthread_attr_t          attr;
    VideoEnv                videoEnv;
    //CaptureEnv              captureEnv;
    DisplayEnv              displayEnv;
    CtrlEnv                 ctrlEnv;
    Void                   *ret;
    Bool                    stopped;

    /* Zero out the thread environments */
    Dmai_clear(videoEnv);
    //Dmai_clear(captureEnv);
    Dmai_clear(displayEnv);
    Dmai_clear(ctrlEnv);

    printf("Decode demo started.\n");

    avifile = AVI_open_input_file(filepath, 1); //打开源文件 

    pthread_mutex_init(&avmutex, NULL);
    /* Initialize the mutex which protects the global data */
    pthread_mutex_init(&gbl.mutex, NULL);
    /* Set the priority of this whole process to max (requires root) */
    setpriority(PRIO_PROCESS, 0, -20);
    /* Initialize Codec Engine runtime */
    CERuntime_init();
    /* Initialize Davinci Multimedia Application Interface */
    Dmai_init();

    /* Create the user interface */
    uiAttrs.osd = args.osd;
    uiAttrs.videoStd = args.videoStd;

    hUI = UI_create(&uiAttrs);
    if (hUI == NULL) {
        cleanup(EXIT_FAILURE);
    }

    /* Set up the user interface */
    uiSetup(hUI, &args);
    
    /* Create the Pause objects */
    hPauseProcess = Pause_create(&pAttrs);
    hPausePrime = Pause_create(&pAttrs);
    if (hPauseProcess == NULL || hPausePrime == NULL) {
        ERR("Failed to create Pause objects\n");
        cleanup(EXIT_FAILURE);
    }

    numThreads = 3;  ///add ctrl thread DECODE FOR AVI V2.0
    /* Create the objects which synchronizes the thread init and cleanup */
    //hRendezvousCapStd  = Rendezvous_create(2, &rzvAttrs);
    hRendezvousInit = Rendezvous_create(numThreads, &rzvAttrs);
    hRendezvousCleanup = Rendezvous_create(numThreads, &rzvAttrs);
	hRendezvousLoop = Rendezvous_create(syncCnt, &rzvAttrs);

    if ( hRendezvousInit == NULL ||   hRendezvousLoop == NULL ||  //hRendezvousCapStd  == NULL ||
        hRendezvousCleanup == NULL) {
        ERR("Failed to create Rendezvous objects\n");
        cleanup(EXIT_FAILURE);
    }

    /* Create the display fifos */
    displayEnv.hInFifo = Fifo_create(&fAttrs);
    displayEnv.hOutFifo = Fifo_create(&fAttrs);

    if (displayEnv.hInFifo == NULL || displayEnv.hOutFifo == NULL) {
        ERR("Failed to create display fifos\n");
        cleanup(EXIT_FAILURE);
    }

    /* Initialize the thread attributes */
    if (pthread_attr_init(&attr)) {
        ERR("Failed to initialize thread attrs\n");
        cleanup(EXIT_FAILURE);
    }

    /* Force the thread to use custom scheduling attributes */
    if (pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED)) {
        ERR("Failed to set schedule inheritance attribute\n");
        cleanup(EXIT_FAILURE);
    }

    /* Set the thread to be fifo real time scheduled */
    if (pthread_attr_setschedpolicy(&attr, SCHED_FIFO)) {
        ERR("Failed to set FIFO scheduling policy\n");
        cleanup(EXIT_FAILURE);
    }

    /* Set the display thread priority */
    schedParam.sched_priority = DISPLAY_THREAD_PRIORITY;
    if (pthread_attr_setschedparam(&attr, &schedParam)) {
        ERR("Failed to set scheduler parameters\n");
        cleanup(EXIT_FAILURE);
    }

    /* Create the display thread */
    displayEnv.videoStd           = args.videoStd;
    displayEnv.hRendezvousInit    = hRendezvousInit;
    displayEnv.hRendezvousCleanup = hRendezvousCleanup;
    displayEnv.hPauseProcess      = hPauseProcess;
    displayEnv.hPausePrime        = hPausePrime;
    displayEnv.osd                = args.osd;
    displayEnv.imageWidth         = args.imageWidth;
    displayEnv.imageHeight        = args.imageHeight;
    displayEnv.passThrough        = args.passThrough;

    if (pthread_create(&displayThread, &attr, displayThrFxn, &displayEnv)) {
        ERR("Failed to create display thread\n");
        cleanup(EXIT_FAILURE);
    }

    initMask |= DISPLAYTHREADCREATED;

    /* Set the video thread priority */
    schedParam.sched_priority = VIDEO_THREAD_PRIORITY;
    if (pthread_attr_setschedparam(&attr, &schedParam)) 
    {
        ERR("Failed to set scheduler parameters\n");
        cleanup(EXIT_FAILURE);
    }

    /* Create the video thread */
    videoEnv.hRendezvousInit    = hRendezvousInit;
    videoEnv.hRendezvousCleanup = hRendezvousCleanup;
    videoEnv.hPauseProcess      = hPauseProcess;
    videoEnv.hPausePrime        = hPausePrime;
    videoEnv.hDisplayOutFifo    = displayEnv.hOutFifo;
    videoEnv.hDisplayInFifo     = displayEnv.hInFifo;
    videoEnv.videoBitRate       = args.videoBitRate;
    videoEnv.passThrough        = args.passThrough;
    videoEnv.engineName         = engine->engineName;
    videoEnv.videoDecoder       = engine->videoDecoders[args.videoCodec].codecName;
    videoEnv.decParams          = engine->videoDecoders[args.videoCodec].params;
    videoEnv.decDynParams       = engine->videoDecoders[args.videoCodec].dynParams;
    videoEnv.videoEncoder       = engine->videoEncoders[args.videoCodec].codecName;
    videoEnv.encParams          = engine->videoEncoders[args.videoCodec].params;
    videoEnv.encDynParams       = engine->videoEncoders[args.videoCodec].dynParams;
    videoEnv.imageWidth         = args.imageWidth; //captureEnv.imageWidth;
    videoEnv.imageHeight        = args.imageHeight;//captureEnv.imageHeight;
    videoEnv.videoStd           = args.videoStd;    

    if (pthread_create(&videoThread, &attr, videoThrFxn, (Void *) &videoEnv)) {
        ERR("Failed to create video thread\n");
        cleanup(EXIT_FAILURE);
    }

    initMask |= VIDEOTHREADCREATED;

    /* Main thread becomes the control thread */
    ctrlEnv.hRendezvousInit    = hRendezvousInit;
    ctrlEnv.hRendezvousCleanup = hRendezvousCleanup;
    ctrlEnv.hPauseProcess      = hPauseProcess;
    ctrlEnv.keyboard           = args.keyboard;
    ctrlEnv.time               = args.time;
    ctrlEnv.hUI                = hUI;
    ctrlEnv.engineName         = engine->engineName;
    ctrlEnv.osd                = args.osd;

    ret = ctrlThrFxn(&ctrlEnv);

    if (ret == THREAD_FAILURE) {
        status = EXIT_FAILURE;
    }

cleanup:
    /* Make sure the other threads aren't waiting for us */
//    if (hRendezvousCapStd) Rendezvous_force(hRendezvousCapStd);
    if (hRendezvousInit) Rendezvous_force(hRendezvousInit);
    if (hPauseProcess) Pause_off(hPauseProcess);
    if (hPausePrime) Pause_off(hPausePrime);
    if (hRendezvousLoop) Rendezvous_force(hRendezvousLoop);

    /* Wait for the threads to complete */
    if (initMask & VIDEOTHREADCREATED) {
        if (pthread_join(videoThread, &ret) == 0) {
            if (ret == THREAD_FAILURE) {
                status = EXIT_FAILURE;
            }
        }
    }


    if (initMask & DISPLAYTHREADCREATED) {
        if (pthread_join(displayThread, &ret) == 0) {
            if (ret == THREAD_FAILURE) {
                status = EXIT_FAILURE;
            }
        }
    }

    if (displayEnv.hOutFifo) {
        Fifo_delete(displayEnv.hOutFifo);
    }

    if (displayEnv.hInFifo) {
        Fifo_delete(displayEnv.hInFifo);
    }

    if (hRendezvousLoop) {
        Rendezvous_delete(hRendezvousLoop);
    }

    if (hRendezvousCleanup) {
        Rendezvous_delete(hRendezvousCleanup);
    }

    if (hRendezvousInit) {
        Rendezvous_delete(hRendezvousInit);
    }

    if (hPauseProcess) {
        Pause_delete(hPauseProcess);
    }

    if (hPausePrime) {
        Pause_delete(hPausePrime);
    }

    if (hUI) {
        UI_delete(hUI);
    }

	if (avifile) {
		AVI_close(avifile);
	}

    /* 
     * In the past, there were instances where we have seen system memory
     * continually reduces by 28 bytes at a time whenever there are file 
     * reads or file writes. This is for the application to recapture that
     * memory (SDOCM00054899)
     */
    system("sync");
    system("echo 3 > /proc/sys/vm/drop_caches");

  	pthread_mutex_destroy(&avmutex);
    pthread_mutex_destroy(&gbl.mutex);

    exit(status);
}
