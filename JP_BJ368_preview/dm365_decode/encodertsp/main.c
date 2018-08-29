/*
 * main.c
 *
 * This source file has the main() for the 'encode demo' on DM365 platform
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
#include <ti/sdo/dmai/Dmai.h>
#include <ti/sdo/dmai/Fifo.h>
#include <ti/sdo/dmai/Pause.h>
#include <ti/sdo/dmai/Sound.h>
#include <ti/sdo/dmai/VideoStd.h>
#include <ti/sdo/dmai/Capture.h>
#include <ti/sdo/dmai/BufferGfx.h>
#include <ti/sdo/dmai/Rendezvous.h>
#include <ti/sdo/fc/rman/rman.h>

#include "video.h"
#include "audio.h"
#include "capture.h"
#include "writer.h"
#include "speech.h"
#include "../ctrl.h"
#include "../demo.h"
#include "../ui.h"
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <memory.h>
#include <dirent.h>
#include "net.h"
#include "rtspservice.h"
#include "rtputils.h"
#include "ringfifo.h"



/* The levels of initialization */
#define LOGSINITIALIZED         0x1
#define DISPLAYTHREADCREATED    0x20
#define CAPTURETHREADCREATED    0x40
#define WRITERTHREADCREATED     0x80
#define VIDEOTHREADCREATED      0x100
#define SPEECHTHREADCREATED     0x200
#define AUDIOTHREADCREATED      0x400

/* Thread priorities */
#define SPEECH_THREAD_PRIORITY  sched_get_priority_max(SCHED_FIFO) - 2
#define VIDEO_THREAD_PRIORITY   sched_get_priority_max(SCHED_FIFO) - 1
#define AUDIO_THREAD_PRIORITY   sched_get_priority_max(SCHED_FIFO) - 2

/* Add argument number x of string y */
#define addArg(x, y)                     \
    argv[(x)] = malloc(strlen((y)) + 1); \
    if (argv[(x)] == NULL)               \
        return FAILURE;                  \
    strcpy(argv[(x)++], (y))





typedef struct Args {
    VideoStd_Type  videoStd;
    Char          *videoStdString;
    Sound_Input    soundInput;
    Capture_Input  videoInput;
    Char          *speechFile;
    Char          *audioFile;
    Char          *videoFile;
    Codec         *speechEncoder;
    Codec         *videoEncoder;
    Codec         *audioEncoder;
    Int32          imageWidth;
    Int32          imageHeight;
    Int            videoBitRate;
    Char          *videoBitRateString;
    Int            soundBitRate;
    Char          *soundBitRateString;
    Int            sampleRate;
    Char          *sampleRateString;
    Int            keyboard;
    Int            time;
    Int            osd;
    Bool           previewDisabled;
    Bool           writeDisabled;
} Args;

#define DEFAULT_ARGS \
    { VideoStd_720P_60, "720P 60Hz", Sound_Input_MIC, Capture_Input_COUNT, \
      NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, -1, NULL, 96000, NULL, \
      16000, NULL, FALSE, FOREVER, FALSE, FALSE, FALSE }

//#define DEFAULT_ARGS \
//    { VideoStd_D1_PAL, "D1-PAL 50Hz", Sound_Input_MIC, Capture_Input_COUNT, \
//     NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, -1, NULL, 96000, NULL, \
//      16000, NULL, FALSE, FOREVER, FALSE, FALSE, FALSE }

/* Global variable declarations for this application */
GlobalData gbl = GBL_DATA_INIT;
pthread_mutex_t avmutex;


/******************************************************************************
 * getCodec
 ******************************************************************************/
static Codec *getCodec(Char *extension, Codec *codecs)
{
    Codec *codec = NULL;
    Int i, j;

    i = 0;
    while (codecs[i].codecName) {
        j = 0;
        while (codecs[i].fileExtensions[j]) {
            if (strcmp(extension, codecs[i].fileExtensions[j]) == 0) {
                codec = &codecs[i];
            }
            j++;
        }
        i++;
    }

    return codec;
}

/******************************************************************************
 * usage
 ******************************************************************************/
static void usage(void)
{
    fprintf(stderr, "Usage: encode [options]\n\n"
      "Options:\n"
      "-s | --speechfile       Speech file to record to\n"
      "-a | --audiofile        Audio file to play\n"
      "-v | --videofile        Video file to record to\n"
      "-y | --display_standard Video standard to use for display (see below).\n"
      "                        Same video standard is used at input.\n"
      "-r | --resolution       Video resolution ('width'x'height')\n"
      "                        [video standard default]\n"
      "-b | --videobitrate     Bit rate to encode video at [variable]\n"
      "-p | --soundbitrate     Bit rate to encode audio at [96000]\n"
      "-u | --samplerate       Sample rate to encode audio at [16000]\n"
      "-w | --preview_disable  Disable preview [preview enabled]\n"
      "-f | --write_disable    Disable recording of encoded file [enabled]\n"
      "-I | --video_input      Video input source [video standard default]\n"
      "-l | --linein           Use linein as sound input instead of mic \n"
      "                        [off]\n"
      "-k | --keyboard         Enable keyboard interface [off]\n"
      "-t | --time             Number of seconds to run the demo [infinite]\n"
      "-o | --osd              Show demo data on an OSD [off]\n"
      "-h | --help             Print this message\n\n"
      "Video standards available\n"
      "\t1\tD1 @ 30 fps (NTSC)\n"
      "\t2\tD1 @ 25 fps (PAL)\n"
      "\t3\t720P @ 60 fps [Default]\n"
      "\t5\t1080I @ 30 fps - for DM368\n"
      "Video inputs available:\n"
      "\t1\tComposite\n"
      "\t2\tS-video\n"
      "\t3\tComponent\n"
      "\t4\tImager/Camera - for DM368\n"
      "You must supply at least a video or a speech file or both\n"
      "with appropriate extensions for the file formats.\n\n");
}

/******************************************************************************
 * parseArgs
 ******************************************************************************/
static Void parseArgs(Int argc, Char *argv[], Args *argsp)
{
    const Char shortOptions[] = "s:a:v:y:r:b:p:u:wfI:lkt:oh";
    const struct option longOptions[] = {
        {"speechfile",       required_argument, NULL, 's'},
        {"audiofile",        required_argument, NULL, 'a'},
        {"videofile",        required_argument, NULL, 'v'},
        {"display_standard", required_argument, NULL, 'y'},
        {"resolution",       required_argument, NULL, 'r'},
        {"videobitrate",     required_argument, NULL, 'b'},
        {"soundbitrate",     required_argument, NULL, 'p'},
        {"samplerate",       required_argument, NULL, 'u'},
        {"preview_disable",  no_argument,       NULL, 'w'},
        {"write_disable",    no_argument,       NULL, 'f'},
        {"video_input",      required_argument, NULL, 'I'},
        {"linein",           no_argument,       NULL, 'l'},
        {"keyboard",         no_argument,       NULL, 'k'},
        {"time",             required_argument, NULL, 't'},
        {"osd",              no_argument,       NULL, 'o'},
        {"help",             no_argument,       NULL, 'h'},
        {0, 0, 0, 0}
    };

    Int     index;
    Int     c;
    Char    *extension;

    for (;;) {
        c = getopt_long(argc, argv, shortOptions, longOptions, &index);

        if (c == -1) {
            break;
        }

        switch (c) {
            case 0:
                break;
            
            case 'a':
                extension = rindex(optarg, '.');
                argsp->audioEncoder =
                    getCodec(extension, engine->audioEncoders);

                if (!argsp->audioEncoder) {
                    fprintf(stderr, "Unknown audio file extension: %s\n",
                            extension);     
                    exit(EXIT_FAILURE);
                }
                argsp->audioFile = optarg;

                break;

            case 's':
                extension = rindex(optarg, '.');
                if (extension == NULL) {
                    fprintf(stderr, "Speech file without extension: %s\n",
                            optarg);
                    exit(EXIT_FAILURE);
                }

                argsp->speechEncoder =
                    getCodec(extension, engine->speechEncoders);

                if (!argsp->speechEncoder) {
                    fprintf(stderr, "Unknown speech file extension: %s\n",
                            extension);
                    exit(EXIT_FAILURE);
                }
                argsp->speechFile = optarg;

                break;

            case 'v':
                extension = rindex(optarg, '.');
                if (extension == NULL) {
                    fprintf(stderr, "Video file without extension: %s\n",
                            optarg);
                    exit(EXIT_FAILURE);
                }

                argsp->videoEncoder =
                    getCodec(extension, engine->videoEncoders);

                if (!argsp->videoEncoder) {
                    fprintf(stderr, "Unknown video file extension: %s\n",
                            extension);
                    exit(EXIT_FAILURE);
                }
                argsp->videoFile = optarg;

                break;

            case 'y':
                switch (atoi(optarg)) {
                    case 1:
                        argsp->videoStd = VideoStd_D1_NTSC;
                        argsp->videoStdString = "D1 NTSC";
                        break;
                    case 2:
                        argsp->videoStd = VideoStd_D1_PAL;
                        argsp->videoStdString = "D1 PAL";
                        break;
                    case 3:
                        argsp->videoStd = VideoStd_720P_60;
                        argsp->videoStdString = "720P 60Hz";
                        break;
                    case 5:
                        argsp->videoStd = VideoStd_1080I_30;
                        argsp->videoStdString = "1080I 30Hz";
                        break;
                    default:
                        fprintf(stderr, "Unknown display standard\n");
                        usage();
                        exit(EXIT_FAILURE);
                }
                break;

            case 'I':
                switch (atoi(optarg)) {
                    case 1:
                        argsp->videoInput = Capture_Input_COMPOSITE;
                        break;
                    case 2:
                        argsp->videoInput = Capture_Input_SVIDEO;
                        break;
                    case 3:
                        argsp->videoInput = Capture_Input_COMPONENT;
                        break;
                    case 4:
                        argsp->videoInput = Capture_Input_CAMERA;
                        break;
                    default:
                        fprintf(stderr, "Unknown video input\n");
                        usage();
                        exit(EXIT_FAILURE);
                }
                break;

            case 'r':
            {
                Int32 imageWidth, imageHeight;

                if (sscanf(optarg, "%ldx%ld", &imageWidth,
                                              &imageHeight) != 2) {
                    fprintf(stderr, "Invalid resolution supplied (%s)\n",
                            optarg);
                    usage();
                    exit(EXIT_FAILURE);
                }

                /* Sanity check resolution */
                if (imageWidth < 2UL || imageHeight < 2UL ||
                    imageWidth > VideoStd_1080I_WIDTH ||
                    imageHeight > VideoStd_1080I_HEIGHT) {
                    fprintf(stderr, "Video resolution must be between %dx%d "
                            "and %dx%d\n", 2, 2, VideoStd_1080I_WIDTH,
                            VideoStd_1080I_HEIGHT);
                    exit(EXIT_FAILURE);
                }

                argsp->imageWidth  = imageWidth;
                argsp->imageHeight = imageHeight;
                break;
            }

            case 'b':
                argsp->videoBitRate = atoi(optarg);
                argsp->videoBitRateString = optarg;
                break;

            case 'p':
                argsp->soundBitRate = atoi(optarg);
                argsp->soundBitRateString = optarg;
                break;

            case 'u':
                argsp->sampleRate = atoi(optarg);
                argsp->sampleRateString = optarg;
                break;

            case 'l':
                argsp->soundInput = Sound_Input_LINE;
                break;

            case 'k':
                argsp->keyboard = TRUE;
                break;

            case 't':
                argsp->time = atoi(optarg);
                break;

            case 'o':
                argsp->osd = TRUE;
                break;

            case 'w':
                argsp->previewDisabled = TRUE;
                break;

            case 'f':
                argsp->writeDisabled = TRUE;
                break;

            case 'h':
                usage();
                exit(EXIT_SUCCESS);

            default:
                usage();
                exit(EXIT_FAILURE);
        }
    }

    /* 
     * If video input is not set, set it to the default based on display
     * video standard.
     */
    if (argsp->videoInput == Capture_Input_COUNT) {
        switch (argsp->videoStd) {
            case VideoStd_D1_NTSC:
            case VideoStd_D1_PAL:
                argsp->videoInput = Capture_Input_COMPOSITE;
                break;
            case VideoStd_720P_60:
                argsp->videoInput = Capture_Input_COMPONENT;
                break;
            case VideoStd_1080I_30:
                argsp->videoInput = Capture_Input_CAMERA;
                break;
            default:
                fprintf(stderr, "Unknown display standard\n");
                usage();
                exit(EXIT_FAILURE);
                break;
        }
    }
}

/******************************************************************************
 * validateArgs
 ******************************************************************************/
static Int validateArgs(Args *argsp)
{
    Bool failed = FALSE;

    /* Need at least one file to encode and only one sound file */
    if ((!argsp->videoFile && !(argsp->audioFile || argsp->speechFile)) ||
        (argsp->audioFile && argsp->speechFile)) {
        usage();
        return FAILURE;
    }

    /* Verify video standard is supported by input */
    switch (argsp->videoInput) {
        case Capture_Input_COMPOSITE:
        case Capture_Input_SVIDEO:
            if ((argsp->videoStd != VideoStd_D1_PAL) && (argsp->videoStd !=
                VideoStd_D1_NTSC)) {
                failed = TRUE;
            }
            break;
        case Capture_Input_COMPONENT:
            if ((argsp->videoStd != VideoStd_720P_60) && (argsp->videoStd
                != VideoStd_1080I_30)) {
                failed = TRUE;
            }
            break;
        case Capture_Input_CAMERA:
            break;
        default:
            fprintf(stderr, "Invalid video input found in validation.\n");
            usage();
            return FAILURE;
    }

    if (failed) {
            fprintf(stderr, "This combination of video input and video" 
                "standard is not supported.\n");
            usage();
            return FAILURE;
    }
    
    return SUCCESS;
}

/******************************************************************************
 * uiSetup
 ******************************************************************************/
static Void uiSetup(UI_Handle hUI, Args *argsp)
{
    /* Initialize values */
    UI_updateValue(hUI, UI_Value_DemoName, "Encode");
    UI_updateValue(hUI, UI_Value_DisplayType, argsp->videoStdString);

    if (argsp->videoEncoder) {
        UI_updateValue(hUI, UI_Value_VideoCodec, argsp->videoEncoder->uiString);
    }
    else {
        UI_updateValue(hUI, UI_Value_VideoCodec, "N/A");
    }

    if (argsp->audioEncoder) {
        UI_updateValue(hUI, UI_Value_SoundCodec,argsp->audioEncoder->uiString);
    }
    else if (argsp->speechEncoder) {
        UI_updateValue(hUI, UI_Value_SoundCodec,argsp->speechEncoder->uiString);
    }
    else {
        UI_updateValue(hUI, UI_Value_SoundCodec, "N/A");
    }

    UI_updateValue(hUI, UI_Value_ImageResolution, "N/A");
    UI_updateValue(hUI, UI_Value_SoundFrequency, "N/A");
}


/******************************************************************************
 * launchInterface
 ******************************************************************************/
static Int launchInterface(Args * argsp)
{
    Char *argv[1000];
    Int i = 0;
    Int pid;

    addArg(i, "./qtInterface");
    addArg(i, "-qws");
    addArg(i, "-d");
    addArg(i, "Encode");

    if (argsp->speechFile) {
        addArg(i, "-s");
        addArg(i, argsp->speechFile);
    }

    if (argsp->videoFile) {
        addArg(i, "-v");
        addArg(i, argsp->videoFile);
    }

    if (argsp->audioFile) {
        addArg(i, "-a");
        addArg(i, argsp->audioFile);
    }

    if (argsp->videoBitRateString) {
        addArg(i, "-b");
        addArg(i, argsp->videoBitRateString);
    }

    if (argsp->soundBitRateString) {
        addArg(i, "-p");
        addArg(i, argsp->soundBitRateString);
    }

    if (argsp->sampleRateString) {
        addArg(i, "-u");
        addArg(i, argsp->sampleRateString);
    }

    if (argsp->previewDisabled) {
        addArg(i, "-w");
    }

    if (argsp->writeDisabled) {
        addArg(i, "-f");
    }

    if (argsp->videoInput) {
        addArg(i, "-I");
        switch (argsp->videoInput) {
            case Capture_Input_COMPOSITE:
                addArg(i, "1");
                break;
            case Capture_Input_SVIDEO:
                addArg(i, "2");
                break;
            case Capture_Input_COMPONENT:
                addArg(i, "3");
                break;
            case Capture_Input_CAMERA:
                addArg(i, "4");
                break;
            default:
                ERR("Invalid video standard\n");
                return FAILURE;
       } 
    }

    /* 
     * Provide Video Standard and set display output
     * based on it. See qtInterface/main.cpp for semantics of numerical
     * arguments used.
     */
    addArg(i, "-y");
    switch(argsp->videoStd) {
        case VideoStd_D1_NTSC:
            addArg(i, "1");
            addArg(i, "-O");
            addArg(i, "1"); /* Composite */
            break;
        case VideoStd_D1_PAL:
            addArg(i, "2");
            addArg(i, "-O");
            addArg(i, "1"); /* Composite */
            break;
        case VideoStd_720P_60:
            addArg(i, "3");
            addArg(i, "-O");
            addArg(i, "3"); /* Component */
            break;
        case VideoStd_1080I_30:
            addArg(i, "5");
            addArg(i, "-O");
            addArg(i, "3"); /* Component */
            break;
        default:
            ERR("Invalid video standard\n");
            return FAILURE;
    }

    pid = fork();
    if (pid == -1) {
        ERR("Could not fork child process.\n");
        return FAILURE;
    }
    else if (pid == 0) {
        if (execv("./qtInterface", argv) == -1) {
            ERR("Could not execute QT Interface\n");
            return FAILURE;
        }
    }

    return SUCCESS;
}

/******************************************************************************
 * getConfigFromInterface
 ******************************************************************************/
static Int getConfigFromInterface(Args * argsp, UI_Handle hUI, Bool * stopped)
{
    Char * extension;
    Char * cfgString;
    Char option = 0;

    *stopped = FALSE;

    UI_getConfig(hUI, &option, &cfgString);

    /* Keep getting configuration strings until ETB */
    while (option != '\27') {
        switch(option) {
            case 'a':
                if (strcmp(cfgString, "") == 0) {
                    /* 
                     * If string is empty, cancel sound file options selected 
                     * on command line. 
                     */
                    argsp->audioEncoder = NULL;
                    argsp->audioFile = NULL;
                    argsp->speechEncoder = NULL;
                    argsp->speechFile = NULL;
                    break;
                }
                extension = rindex(cfgString, '.');
                argsp->audioEncoder =
                    getCodec(extension, engine->audioEncoders);

                if (!argsp->audioEncoder) {
                    fprintf(stderr, "Unknown audio file extension: %s\n",
                            extension);     
                    exit(EXIT_FAILURE);
                }
                argsp->audioFile = cfgString;

                break;

            case 's':
                if (strcmp(cfgString, "") == 0) {
                    /* 
                     * If string is empty, cancel sound file options selected 
                     * on command line. 
                     */
                    argsp->speechEncoder = NULL;
                    argsp->speechFile = NULL;
                    argsp->speechEncoder = NULL;
                    argsp->speechFile = NULL;
                    break;
                }
                extension = rindex(cfgString, '.');
                if (extension == NULL) {
                    fprintf(stderr, "Speech file without extension: %s\n",
                            cfgString);
                    exit(EXIT_FAILURE);
                }

                argsp->speechEncoder =
                    getCodec(extension, engine->speechEncoders);

                if (!argsp->speechEncoder) {
                    fprintf(stderr, "Unknown speech file extension: %s\n",
                            extension);
                    exit(EXIT_FAILURE);
                }
                argsp->speechFile = cfgString;

                break;

            case 'v':
                if (strcmp(cfgString, "") == 0) {
                    /* 
                     * If string is empty, cancel video file option selected 
                     * on command line. 
                     */
                    argsp->videoEncoder = NULL;
                    argsp->videoFile = NULL;
                    break;
                }
                extension = rindex(cfgString, '.');
                if (extension == NULL) {
                    fprintf(stderr, "Video file without extension: %s\n",
                            cfgString);
                    exit(EXIT_FAILURE);
                }

                argsp->videoEncoder =
                    getCodec(extension, engine->videoEncoders);

                if (!argsp->videoEncoder) {
                    fprintf(stderr, "Unknown video file extension: %s\n",
                            extension);
                    exit(EXIT_FAILURE);
                }
                argsp->videoFile = cfgString;

                break;

            case 'y':
                switch (atoi(cfgString)) {
                    case 1:
                        argsp->videoStd = VideoStd_D1_NTSC;
                        argsp->videoStdString = "D1 NTSC";
                        break;
                    case 2:
                        argsp->videoStd = VideoStd_D1_PAL;
                        argsp->videoStdString = "D1 PAL";
                        break;
                    case 3:
                        argsp->videoStd = VideoStd_720P_60;
                        argsp->videoStdString = "720P 60Hz";
                        break;
                    case 5:
                        argsp->videoStd = VideoStd_1080I_30;
                        argsp->videoStdString = "1080I 30Hz";
                        break;
                    default:
                        fprintf(stderr, "Invalid display standard set by"
                            " interface\n");
                        exit(EXIT_FAILURE);
                }
                break;

            case 'b':
                argsp->videoBitRate = atoi(cfgString);
                argsp->videoBitRateString = cfgString;
                break;

            case 'p':
                argsp->soundBitRate = atoi(cfgString);
                argsp->soundBitRateString = cfgString;
                break;

            case 'u':
                argsp->sampleRate = atoi(cfgString);
                argsp->sampleRateString = cfgString;
                break;

            case 'w':
                argsp->previewDisabled = TRUE;
                break;

            case 'f':
                argsp->writeDisabled = TRUE;
                break;

            case '\33':
                *stopped = TRUE;
                return SUCCESS;

            default:
                ERR("Error in getting configuration from interface\n");
                return FAILURE;
        }
        UI_getConfig(hUI, &option, &cfgString);
    }
    return SUCCESS;
}



unsigned int get_max_filename(const char* ext)
{
	DIR *dir;
	struct dirent *ptr;

	unsigned int file_max_count = 0;
	unsigned int current_file_count = 0;
	char filename[64];
	char strbuf[10];

printf("-----> open dir\r\n");

	dir = NULL;
	dir = opendir("/home/root/media/");

	if(dir != NULL)
	{
		printf("-----> read dir\r\n");
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
					current_file_count = (filename[4]-'0')*10000 + (filename[5]-'0')*1000 + 
										 (filename[6]-'0')*100 + (filename[7]-'0')*10 + (filename[8]-'0')*1;

					if(current_file_count > file_max_count)
					{
						file_max_count = current_file_count;
					}
				}
	//printf("-----> read avi ==\r\n");
			}
			else if(strcmp("bmp", ext) == 0)  //a bmp file
			{
				if( strcmp("BMP_", strbuf) == 0) //test first 8 byte of filename
				{
					current_file_count = (filename[4]-'0')*10000 + (filename[5]-'0')*1000 + 
										 (filename[6]-'0')*100 + (filename[7]-'0')*10 + (filename[8]-'0')*1;

					if(current_file_count > file_max_count)
					{
						//printf("4 - bmp count\r\n");
						file_max_count = current_file_count;
					}
				}
	//printf("-----> read bmp ==\r\n");
			}
			else if(strcmp("jpg", ext) == 0)  //a bmp file
			{
				if( strcmp("JPG_", strbuf) == 0) //test first 8 byte of filename
				{
					current_file_count = (filename[4]-'0')*10000 + (filename[5]-'0')*1000 + 
										 (filename[6]-'0')*100 + (filename[7]-'0')*10 + (filename[8]-'0')*1;

					if(current_file_count > file_max_count)
					{
						//printf("4 - jpg count\r\n");
						file_max_count = current_file_count;
					}
				}
			}
		}

		printf("----->  close dir \r\n");
		close(dir);
		printf("-----> return max file \r\n");
	}

	return file_max_count;
}



/******************************************************************************
 * main
 ******************************************************************************/
Int main(Int argc, Char *argv[])
{
    Args                args                = DEFAULT_ARGS;
    Uns                 initMask            = 0;
    Int                 status              = EXIT_SUCCESS;
    Pause_Attrs         pAttrs              = Pause_Attrs_DEFAULT;
    Rendezvous_Attrs    rzvAttrs            = Rendezvous_Attrs_DEFAULT;
    Fifo_Attrs          fAttrs              = Fifo_Attrs_DEFAULT;
    UI_Attrs            uiAttrs;
    Rendezvous_Handle   hRendezvousCapStd   = NULL;
    Rendezvous_Handle   hRendezvousInit     = NULL;
    Rendezvous_Handle   hRendezvousWriter   = NULL;
    Rendezvous_Handle   hRendezvousCleanup  = NULL;
    Pause_Handle        hPauseProcess       = NULL;
    UI_Handle           hUI                 = NULL;
    struct sched_param  schedParam;
    pthread_t           captureThread;
    pthread_t           writerThread;
    pthread_t           videoThread;
    pthread_t           audioThread;
    pthread_t           speechThread;
    CaptureEnv          captureEnv;
    WriterEnv           writerEnv;
    VideoEnv            videoEnv;
    SpeechEnv           speechEnv;
    AudioEnv            audioEnv;
    CtrlEnv             ctrlEnv;
    Int                 numThreads;
    pthread_attr_t      attr;
    Void               *ret;
    Bool                stopped;
    FILE               *testfile     = NULL;
gl_screen_div=0;
#if 1
	if(fopen("/dev/mmcblk0p1", "r") == NULL)
	{
		printf("No TF Card Insert!\r\n");
	  	gl_sdcard_exist = 0;
	}
	else
	{
		system("mount -t vfat /dev/mmcblk0p1 /home/root/media"); 
		printf("Mount TF Card OK!  (/dev/mmcblk0p1)\r\n");
		//system("insmod /dm368/g_file_storage.ko file=/dev/mmcblk0p1 stall=0 removable=1");
		gl_sdcard_exist = 1;
	}
    if(0 == gl_sdcard_exist)
	{
		if(fopen("/dev/mmcblk0", "r") == NULL)
		{
			printf("No TF Card Insert!\r\n");
		  	gl_sdcard_exist = 0;
		}
		else
		{
			system("mount -t vfat /dev/mmcblk0 /home/root/media"); 
			printf("Mount TF Card OK!  (/dev/mmcblk0)\r\n");
			//system("insmod /dm368/g_file_storage.ko file=/dev/mmcblk0 stall=0 removable=1");
			gl_sdcard_exist = 1;
		}
	}
#endif


	if(1 == gl_sdcard_exist)
	{
//		gl_max_bmp = 0;
		gl_max_avi = 0;
		gl_max_jpg = 0;

		gl_max_jpg = get_max_filename("jpg");
		printf("----->jpg max=%d\r\n", gl_max_jpg);

		gl_max_avi = get_max_filename("avi");
		printf("----->avi max=%d\r\n", gl_max_avi);

		if(gl_max_jpg >= JPG_MAX_CNT)
		{
			gl_osd_jpgmax = 1;
		}
		if(gl_max_avi >= AVI_MAX_CNT)
		{
			gl_osd_avimax = 1;
		}
	}

#if 0
	if(fopen("/dev/mtdblock4", "r") == NULL)
	{
		printf("!!!!!!!  FAILED OPEN NAND USER DATA BLOCK !!!!!!!\r\n");
	  	gl_sdcard_exist = 0;
	}
	else
	{
		//system("ubiattach /dev/ubi_ctrl -m 4 -d 1");  
		//system("mount -t ubifs ubi1_0 /home/root/media");

		///system("mount -t yaffs2 /dev/mtdblock4 /home/root/media"); 
		//printf("#########MOUNT NAND USER DATA BLOCK OK !!(mtdblock4->ubifs) #######\r\n");

		gl_sdcard_exist = 1;
	}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FUN_RTSP
	ringmalloc(64*1024);

	printf("RTSP server START\n");
	PrefsInit();
	printf("listen for client connecting...\n");
	signal(SIGINT, IntHandl);

	s32MainFd = tcp_listen(SERVER_RTSP_PORT_DEFAULT);

	if (ScheduleInit() == ERR_FATAL)
	{
		fprintf(stderr,"Fatal: Can't start scheduler %s, %i \nServer is aborting.\n", __FILE__, __LINE__);
	}
	RTP_port_pool_init(RTP_DEFAULT_PORT);
#endif
//	system("./RTSPServer &");
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


    /* Zero out the thread environments */
    Dmai_clear(captureEnv);
    Dmai_clear(writerEnv);
    Dmai_clear(videoEnv);
    Dmai_clear(speechEnv);
    Dmai_clear(audioEnv);
    Dmai_clear(ctrlEnv);


    /* Parse the arguments given to the app and set the app environment */
    parseArgs(argc, argv, &args);

    printf("Encode demo started.\n");


#if 0
    /* Launch interface app */
    if (args.osd) {
        if (launchInterface(&args) == FAILURE) {
            exit(EXIT_FAILURE);
        }
    }
#endif
    pthread_mutex_init(&avmutex, NULL);
    /* Initialize the mutex which protects the global data */
    pthread_mutex_init(&gbl.mutex, NULL);

    /* Set the priority of this whole process to max (requires root) */
    setpriority(PRIO_PROCESS, 0, -20);

    /* Initialize Codec Engine runtime */
    CERuntime_init();

    /* Initialize Davinci Multimedia Application Interface */
    Dmai_init();

    initMask |= LOGSINITIALIZED;

    /* Create the user interface */
    uiAttrs.osd = args.osd;
    uiAttrs.videoStd = args.videoStd;


    hUI = UI_create(&uiAttrs);

    if (hUI == NULL) {
        cleanup(EXIT_FAILURE);
    }

#if 0
    /* Get configuration from QT interface if necessary */
    if (args.osd) {
        status = getConfigFromInterface(&args, hUI, &stopped);
        if (status == FAILURE) {
            ERR("Failed to get valid configuration from the GUI\n");
            cleanup(EXIT_FAILURE);
        }
        else if (stopped == TRUE) {
            cleanup(EXIT_SUCCESS);
        }
    }
#endif

    /* Validate arguments */
//    if (validateArgs(&args) == FAILURE) {
//        cleanup(EXIT_FAILURE);
//    }
    
    /* Set up the user interface */
    uiSetup(hUI, &args);
    
    /* Create the Pause object */
    hPauseProcess = Pause_create(&pAttrs);

    if (hPauseProcess == NULL) {
        ERR("Failed to create Pause object\n");
        cleanup(EXIT_FAILURE);
    }

    /* Determine the number of threads needing synchronization */
    numThreads = 1;

    if (args.videoFile) {
        numThreads += 3;
    }

   	if (args.audioFile || args.speechFile) 
	{
      //  numThreads += 1;
    }

    /* Create the objects which synchronizes the thread init and cleanup */
    hRendezvousCapStd  = Rendezvous_create(2, &rzvAttrs);
    hRendezvousInit = Rendezvous_create(numThreads, &rzvAttrs);
    hRendezvousCleanup = Rendezvous_create(numThreads, &rzvAttrs);
    hRendezvousWriter = Rendezvous_create(2, &rzvAttrs);

    if (hRendezvousCapStd  == NULL || hRendezvousInit == NULL || 
        hRendezvousCleanup == NULL || hRendezvousWriter == NULL) {
        ERR("Failed to create Rendezvous objects\n");
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

    /* Create the video threads if a file name is supplied */
    if (args.videoFile) {
        /* Create the capture fifos */
        captureEnv.hInFifo = Fifo_create(&fAttrs);
        captureEnv.hOutFifo = Fifo_create(&fAttrs);

        if (captureEnv.hInFifo == NULL || captureEnv.hOutFifo == NULL) {
            ERR("Failed to open display fifos\n");
            cleanup(EXIT_FAILURE);
        }

        /* Create the capture thread */
        captureEnv.hRendezvousInit    = hRendezvousInit;
        captureEnv.hRendezvousCapStd  = hRendezvousCapStd;
        captureEnv.hRendezvousCleanup = hRendezvousCleanup;
        captureEnv.hPauseProcess      = hPauseProcess;
        captureEnv.videoStd           = args.videoStd;
        captureEnv.videoInput         = args.videoInput;
        captureEnv.imageWidth         = args.imageWidth;
        captureEnv.imageHeight        = args.imageHeight;
        captureEnv.previewDisabled    = args.previewDisabled;

        if (pthread_create(&captureThread, NULL, captureThrFxn, &captureEnv)) {
            ERR("Failed to create capture thread\n");
            cleanup(EXIT_FAILURE);
        }

        initMask |= CAPTURETHREADCREATED;

        /*
         * Once the capture thread has detected the video standard, make it
         * available to other threads. The capture thread will set the
         * resolution of the buffer to encode in the environment (derived
         * from the video standard if the user hasn't passed a resolution).
         */
        Rendezvous_meet(hRendezvousCapStd);

        /* Create the writer fifos */
        writerEnv.hInFifo = Fifo_create(&fAttrs);
        writerEnv.hOutFifo = Fifo_create(&fAttrs);

        if (writerEnv.hInFifo == NULL || writerEnv.hOutFifo == NULL) {
            ERR("Failed to open display fifos\n");
            cleanup(EXIT_FAILURE);
        }

        /* Set the video thread priority */
        schedParam.sched_priority = VIDEO_THREAD_PRIORITY;
        if (pthread_attr_setschedparam(&attr, &schedParam)) {
            ERR("Failed to set scheduler parameters\n");
            cleanup(EXIT_FAILURE);
        }

        /* Create the video thread */
        videoEnv.hRendezvousInit    = hRendezvousInit;
        videoEnv.hRendezvousCleanup = hRendezvousCleanup;
        videoEnv.hRendezvousWriter  = hRendezvousWriter;
        videoEnv.hPauseProcess      = hPauseProcess;
        videoEnv.hCaptureOutFifo    = captureEnv.hOutFifo;
        videoEnv.hCaptureInFifo     = captureEnv.hInFifo;
        videoEnv.hWriterOutFifo     = writerEnv.hOutFifo;
        videoEnv.hWriterInFifo      = writerEnv.hInFifo;
        videoEnv.videoEncoder       = args.videoEncoder->codecName;
        videoEnv.params             = args.videoEncoder->params;
        videoEnv.dynParams          = args.videoEncoder->dynParams;
        videoEnv.videoBitRate       = args.videoBitRate;
        videoEnv.imageWidth         = captureEnv.imageWidth;
        videoEnv.imageHeight        = captureEnv.imageHeight;
        videoEnv.engineName         = engine->engineName;

        if (args.videoStd == VideoStd_D1_PAL) 
		{
            videoEnv.videoFrameRate     = 25000;
        } 
		else 
		{
     		videoEnv.videoFrameRate     = 50000; //25000;
        }

        if (pthread_create(&videoThread, &attr, videoThrFxn, &videoEnv)) {
            ERR("Failed to create video thread\n");
            cleanup(EXIT_FAILURE);
        }

        initMask |= VIDEOTHREADCREATED;

        /*
         * Wait for the codec to be created in the video thread before
         * launching the writer thread (otherwise we don't know which size
         * of buffers to use).
         */
        Rendezvous_meet(hRendezvousWriter);

        /* Create the writer thread */
        writerEnv.hRendezvousInit    = hRendezvousInit;
        writerEnv.hRendezvousCleanup = hRendezvousCleanup;
        writerEnv.hPauseProcess      = hPauseProcess;
        writerEnv.videoFile          = args.videoFile;
        writerEnv.outBufSize         = videoEnv.outBufSize;
        writerEnv.writeDisabled      = args.writeDisabled;

        if (pthread_create(&writerThread, NULL, writerThrFxn, &writerEnv)) {
            ERR("Failed to create writer thread\n");
            cleanup(EXIT_FAILURE);
        }

        initMask |= WRITERTHREADCREATED;
    }

#if 0
/* Create the audio thread if a file name is supplied */
    if (args.audioFile) 
	{
        /* Set the thread priority */
        schedParam.sched_priority = AUDIO_THREAD_PRIORITY;
        if (pthread_attr_setschedparam(&attr, &schedParam)) {
            ERR("Failed to set scheduler parameters\n");
            cleanup(EXIT_FAILURE);
        }

        /* Create the audio thread */
        audioEnv.hRendezvousInit    = hRendezvousInit;
        audioEnv.hRendezvousCleanup = hRendezvousCleanup;
        audioEnv.hPauseProcess      = hPauseProcess;
        audioEnv.engineName         = engine->engineName;
        audioEnv.audioEncoder       = args.audioEncoder->codecName;
        audioEnv.params             = args.audioEncoder->params;
        audioEnv.dynParams          = args.audioEncoder->dynParams;
        audioEnv.audioFile          = args.audioFile;
        audioEnv.soundInput         = args.soundInput;
        audioEnv.soundBitRate       = args.soundBitRate;
        audioEnv.sampleRate         = args.sampleRate;
        audioEnv.writeDisabled      = args.writeDisabled;
        if (pthread_create(&audioThread, &attr, audioThrFxn, &audioEnv)) {
            ERR("Failed to create audio thread\n");
            cleanup(EXIT_FAILURE);
        }

        initMask |= AUDIOTHREADCREATED;
    }
#endif
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
    if (args.osd) {
        int rv;
        if (hUI) {
            /* Stop the UI */
            UI_stop(hUI);
        }
        wait(&rv);      /* Wait for child process to end */
    }

    /* Make sure the other threads aren't waiting for init to complete */
    if (hRendezvousCapStd) Rendezvous_force(hRendezvousCapStd);
    if (hRendezvousWriter) Rendezvous_force(hRendezvousWriter);
    if (hRendezvousInit) Rendezvous_force(hRendezvousInit);
    if (hPauseProcess) Pause_off(hPauseProcess);

    /* Wait until the other threads terminate */
    if (initMask & SPEECHTHREADCREATED) {
        if (pthread_join(speechThread, &ret) == 0) {
            if (ret == THREAD_FAILURE) {
                status = EXIT_FAILURE;
            }
        }
    }

    if (initMask & AUDIOTHREADCREATED) {
        if (pthread_join(audioThread, &ret) == 0) {
            if (ret == THREAD_FAILURE) {
                status = EXIT_FAILURE;
            }
        }
    }
    if (initMask & VIDEOTHREADCREATED) {
        if (pthread_join(videoThread, &ret) == 0) {
            if (ret == THREAD_FAILURE) {
                status = EXIT_FAILURE;
            }
        }
    }

    if (initMask & WRITERTHREADCREATED) {
        if (pthread_join(writerThread, &ret) == 0) {
            if (ret == THREAD_FAILURE) {
                status = EXIT_FAILURE;
            }
        }
    }

    if (writerEnv.hOutFifo) {
        Fifo_delete(writerEnv.hOutFifo);
    }

    if (writerEnv.hInFifo) {
        Fifo_delete(writerEnv.hInFifo);
    }

    if (initMask & CAPTURETHREADCREATED) {
        if (pthread_join(captureThread, &ret) == 0) {
            if (ret == THREAD_FAILURE) {
                status = EXIT_FAILURE;
            }
        }
    }

    if (captureEnv.hOutFifo) {
        Fifo_delete(captureEnv.hOutFifo);
    }

    if (captureEnv.hInFifo) {
        Fifo_delete(captureEnv.hInFifo);
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

    if (hUI) {
        UI_delete(hUI);
    }

    /* 
     * In the past, there were instances where we have seen system memory
     * continually reduces by 28 bytes at a time whenever there are file 
     * reads or file writes. This is for the application to recapture that
     * memory (SDOCM00054899)
     */
    system("sync");
    system("echo 3 > /proc/sys/vm/drop_caches");


    pthread_mutex_destroy(&gbl.mutex);
  	pthread_mutex_destroy(&avmutex);

    exit(status);
}
