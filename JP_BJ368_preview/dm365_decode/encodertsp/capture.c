/*
 * capture.c
 *
 * This source file implements the video capture for the encode demo 
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

#include <xdc/std.h>
#include <string.h>
#include <stdlib.h>
#include <ti/sdo/dmai/Fifo.h>
#include <ti/sdo/dmai/Pause.h>
#include <ti/sdo/dmai/BufTab.h>
#include <ti/sdo/dmai/Capture.h>
#include <ti/sdo/dmai/Display.h>
#include <ti/sdo/dmai/VideoStd.h>
#include <ti/sdo/dmai/BufferGfx.h>
#include <ti/sdo/dmai/Rendezvous.h>
#include <ti/sdo/dmai/Dmai.h>
#include <ti/sdo/dmai/BufferGfx.h>
#include <ti/sdo/dmai/Resize.h>

#include "capture.h"
#include "../demo.h"
#include "../ctrl.h"

/* Buffering for the display driver */
#define NUM_DISPLAY_BUFS         3

/* Buffering for the capture driver */
#define NUM_CAPTURE_BUFS         3

/* Number of buffers in the pipe to the capture thread */
/* Note: It needs to match capture.c pipe size */
#define VIDEO_PIPE_SIZE          4 

#define NUM_BUFS (NUM_CAPTURE_BUFS + NUM_DISPLAY_BUFS + VIDEO_PIPE_SIZE)

/* Black color in UYVY format */
#define UYVY_BLACK      0x10801080






/******************************************************************************
 * CapBuf_blackFill
 * Note: This function uses x, y parameters to calculate the offset for the
 * buffers.
 ******************************************************************************/

Void CapBuf_blackFill(Buffer_Handle hBuf)
{
    switch (BufferGfx_getColorSpace(hBuf)) {
        case ColorSpace_YUV422PSEMI:
        {
            Int8  *yPtr     = Buffer_getUserPtr(hBuf);
            Int32  ySize    = Buffer_getSize(hBuf) / 2;
            Int8  *cbcrPtr  = yPtr + ySize;
            Int    bpp = ColorSpace_getBpp(ColorSpace_YUV422PSEMI);
            Int    y;
            BufferGfx_Dimensions dim;
            UInt32 offset;

            BufferGfx_getDimensions(hBuf, &dim);
            offset = dim.y * dim.lineLength + dim.x * bpp / 8;
            for (y = 0; y < dim.height; y++) {
                memset(yPtr + offset, 0x0, dim.width * bpp / 8);
                yPtr += dim.lineLength;
            }

            for (y = 0; y < dim.height; y++) {
                memset(cbcrPtr + offset, 0x80, dim.width * bpp / 8);
                cbcrPtr += dim.lineLength;
            }

            break;
        }

        case ColorSpace_YUV420PSEMI:
        {
            Int8  *yPtr     = Buffer_getUserPtr(hBuf);
            Int32  ySize    = Buffer_getSize(hBuf) * 2 / 3;
            Int8  *cbcrPtr  = yPtr + ySize;
            Int    bpp = ColorSpace_getBpp(ColorSpace_YUV420PSEMI);
            Int    y;
            BufferGfx_Dimensions dim;

            BufferGfx_getDimensions(hBuf, &dim);
            yPtr += dim.y * dim.lineLength + dim.x * bpp / 8;
            for (y = 0; y < dim.height; y++) {
                memset(yPtr, 0x0, dim.width * bpp / 8);
                yPtr += dim.lineLength;
            }

            cbcrPtr += dim.y * dim.lineLength / 2 + dim.x * bpp / 8;
            for (y = 0; y < dim.height / 2; y++) {
                memset(cbcrPtr, 0x80, dim.width * bpp / 8);
                cbcrPtr += dim.lineLength;
            }
            
            break;
        }

        case ColorSpace_UYVY:
        {
            Int32 *bufPtr = (Int32*)Buffer_getUserPtr(hBuf);
            Int    bpp    = ColorSpace_getBpp(ColorSpace_UYVY);
            Int    i, j;
            BufferGfx_Dimensions dim;

            BufferGfx_getDimensions(hBuf, &dim);
            bufPtr += (dim.y * dim.lineLength + dim.x * bpp / 8) / sizeof(Int32);

            /* Make sure display buffer is 4-byte aligned */
            assert((((UInt32) bufPtr) & 0x3) == 0);

            for (i = 0; i < dim.height; i++) {
                for (j = 0; j < dim.width / 2; j++) {
                    bufPtr[j] = UYVY_BLACK;
                }
                bufPtr += dim.lineLength / sizeof(Int32);
            }

            break;
        }

        case ColorSpace_RGB565:
        {
            memset(Buffer_getUserPtr(hBuf), 0, Buffer_getSize(hBuf));
            break;
        }

        default:
        {
                ERR("Unsupported color space (%d) for _Dmai_blackFill\n",
                BufferGfx_getColorSpace(hBuf));
            break;
        }
    }
}






/******************************************************************************
 * captureThrFxn
 ******************************************************************************/
Void *captureThrFxn(Void *arg)
{
    CaptureEnv           *envp     = (CaptureEnv *) arg;
    Void                 *status   = THREAD_SUCCESS;
    Capture_Attrs         cAttrs   = Capture_Attrs_DM365_DEFAULT;
    Display_Attrs         dAttrs   = Display_Attrs_DM365_VID_DEFAULT;
    BufferGfx_Attrs       gfxAttrs = BufferGfx_Attrs_DEFAULT;    
    Capture_Handle        hCapture = NULL;
    Display_Handle        hDisplay = NULL;
    BufTab_Handle         hBufTab  = NULL;
    BufTab_Handle         hDispBufTab = NULL;
    BufTab_Handle         hFifoBufTab = NULL;
    Buffer_Handle         hDstBuf, hCapBuf, hDisBuf, hBuf;
    BufferGfx_Dimensions  capDim;
    VideoStd_Type         videoStd;
    Int32                 width, height, bufSize;
    Int                   fifoRet;
    ColorSpace_Type       colorSpace = ColorSpace_YUV420PSEMI;
    Int                   bufIdx;
    Int                   numCapBufs;
	Int i;

	int framesecond = 0;

	gl_capture_fd = 0;

// 	cAttrs.videoStd = global_VideoStd; 
//  cAttrs.videoInput = Capture_Input_COMPONENT;
//	cAttrs.videoInput = Capture_Input_COMPOSITE;

    /* Create capture device driver instance */
    cAttrs.numBufs    = NUM_CAPTURE_BUFS;
    cAttrs.videoInput = envp->videoInput;
    cAttrs.videoStd   = envp->videoStd;
    cAttrs.colorSpace = colorSpace;

    videoStd          = envp->videoStd;


    if (envp->imageWidth > 0 && envp->imageHeight > 0) {
        if (VideoStd_getResolution(videoStd, &width, &height) < 0) {
            ERR("Failed to calculate resolution of video standard\n");
            cleanup(THREAD_FAILURE);
        }

        if (width < envp->imageWidth && height < envp->imageHeight) {
            ERR("User resolution (%ldx%ld) larger than detected (%ldx%ld)\n",
                envp->imageWidth, envp->imageHeight, width, height);
            cleanup(THREAD_FAILURE);
        }

envp->imageWidth  =720;
envp->imageHeight = 576;

       /*
        * Capture driver provides 32 byte aligned data. We 32 byte align the
        * capture and video buffers to perform zero copy encoding.
        */
        envp->imageWidth  = Dmai_roundUp(envp->imageWidth,32);
        capDim.x          = 0;
        capDim.y          = 0;
        capDim.height     = envp->imageHeight;
        capDim.width      = envp->imageWidth;
        capDim.lineLength = BufferGfx_calcLineLength(capDim.width, colorSpace);
    } 
    else {
        /* Calculate the dimensions of a video standard given a color space */
        if (BufferGfx_calcDimensions(videoStd, colorSpace, &capDim) < 0) {
            ERR("Failed to calculate Buffer dimensions\n");
            cleanup(THREAD_FAILURE);
        }

       /*
        * Capture driver provides 32 byte aligned data. We 32 byte align the
        * capture and video buffers to perform zero copy encoding.
        */
        capDim.width      = Dmai_roundUp(capDim.width,32);
        envp->imageWidth  = capDim.width;
        envp->imageHeight = capDim.height;
    }

    numCapBufs = NUM_CAPTURE_BUFS;

    gfxAttrs.dim.height = capDim.height;
    gfxAttrs.dim.width = capDim.width;
    gfxAttrs.dim.lineLength = Dmai_roundUp(BufferGfx_calcLineLength(gfxAttrs.dim.width, colorSpace), 32);

    gfxAttrs.dim.x = 0;
    gfxAttrs.dim.y = 0;


    if (colorSpace ==  ColorSpace_YUV420PSEMI) 
	{
        bufSize = (gfxAttrs.dim.lineLength * gfxAttrs.dim.height * 3 / 2);

	//	bufSize += RSZ_WIDTH*RSZ_HEIGHT*2;//720*576*2; 
    } 
    else {
        bufSize = gfxAttrs.dim.lineLength * gfxAttrs.dim.height * 2; 
    }

    /* Create a table of buffers to use with the capture driver */
    gfxAttrs.colorSpace = colorSpace;
    hBufTab = BufTab_create(numCapBufs, bufSize,
                            BufferGfx_getBufferAttrs(&gfxAttrs));
    if (hBufTab == NULL) {
        ERR("Failed to create buftab\n");
        cleanup(THREAD_FAILURE);
    }

    /* Create a table of buffers to use to prime Fifo to video thread */
    hFifoBufTab = BufTab_create(VIDEO_PIPE_SIZE, bufSize,
                            BufferGfx_getBufferAttrs(&gfxAttrs));
    if (hFifoBufTab == NULL) {
        ERR("Failed to create buftab\n");
        cleanup(THREAD_FAILURE);
    }

    /* Update global data for user interface */
    gblSetImageWidth(envp->imageWidth);
    gblSetImageHeight(envp->imageHeight);

    /* Report the video standard and image size back to the main thread */
    Rendezvous_meet(envp->hRendezvousCapStd);

    /* Capture at half frame rate if using COMPONENT input at 720P */
    if ((envp->videoStd == VideoStd_720P_60) 
        && (envp->videoInput == Capture_Input_COMPONENT)) {
        cAttrs.videoStd = VideoStd_720P_30;
    }
    else {
        cAttrs.videoStd = envp->videoStd;    
    }

    /*If its component input and video std is 1080I_30 then make it 1080I_60.*/
    if (cAttrs.videoStd == VideoStd_1080I_30 && cAttrs.videoInput 
                        == Capture_Input_COMPONENT) {
        cAttrs.videoStd = VideoStd_1080I_60;
    }

    cAttrs.numBufs    = NUM_CAPTURE_BUFS;    
    cAttrs.colorSpace = colorSpace;
    cAttrs.captureDimension = &gfxAttrs.dim;


    cAttrs.videoStd   = VideoStd_D1_PAL;          //VideoStd_720P_60;       
    cAttrs.videoInput = Capture_Input_COMPOSITE;  //Capture_Input_COMPONENT;


    /* Create the capture device driver instance */
    hCapture = Capture_create(hBufTab, &cAttrs);

    if (hCapture == NULL) 
	{
        ERR("Failed to create capture device. Is video input connected?\n");
        cleanup(THREAD_FAILURE);
    }

	gl_capture_fd = Capture_get_fd(hCapture);


////////////////////////////////////////////////////////////////////////
#if 1
    gfxAttrs.dim.width =  RSZ_WIDTH;//720;
    gfxAttrs.dim.height = RSZ_HEIGHT;

    gfxAttrs.dim.lineLength = Dmai_roundUp(BufferGfx_calcLineLength(gfxAttrs.dim.width, colorSpace), 32);

    gfxAttrs.dim.x = 0;
    gfxAttrs.dim.y = 0;

	printf(">>>>>>>>>>width       :%d\n",gfxAttrs.dim.width );
	printf(">>>>>>>>>>height      :%d\n",gfxAttrs.dim.height);
	printf(">>>>>>>>>>lineLength	 :%d\n",gfxAttrs.dim.lineLength);
	printf(">>>>>>>>>>x           :%d\n",gfxAttrs.dim.x);
	printf(">>>>>>>>>>y			 :%d\n",gfxAttrs.dim.y);

    if (colorSpace ==  ColorSpace_YUV420PSEMI) {
        bufSize = (gfxAttrs.dim.lineLength * gfxAttrs.dim.height * 3 / 2);
    } 
    else {
        bufSize = gfxAttrs.dim.lineLength * gfxAttrs.dim.height * 2; 

    }

	printf("gfxAttrs.dim.lineLength = %d\r\n", gfxAttrs.dim.lineLength);


    /* Create a table of buffers to use with the capture driver */
    gfxAttrs.colorSpace = colorSpace;
#endif

//////////////////////////////////////////////////////////////////////////////////////

    /* Create a table of buffers to use with the display driver */
    hDispBufTab = BufTab_create(NUM_DISPLAY_BUFS, bufSize,
                            BufferGfx_getBufferAttrs(&gfxAttrs));
    if (hDispBufTab == NULL) {
        ERR("Failed to create buftab\n");
        cleanup(THREAD_FAILURE);
    }

    /* Create display device driver instance if preview is needed */
    if (!envp->previewDisabled) {
        dAttrs.videoStd = envp->videoStd;
        if ( (dAttrs.videoStd == VideoStd_CIF) ||
            (dAttrs.videoStd == VideoStd_SIF_NTSC) ||
            (dAttrs.videoStd == VideoStd_SIF_PAL) ||
            (dAttrs.videoStd == VideoStd_VGA) ||
            (dAttrs.videoStd == VideoStd_D1_NTSC) ||        
            (dAttrs.videoStd == VideoStd_D1_PAL) ) {
            dAttrs.videoOutput = Display_Output_COMPOSITE;
        } else {
            dAttrs.videoOutput = Display_Output_COMPONENT;
        } 
    

		dAttrs.videoStd   = VideoStd_D1_PAL;
		dAttrs.videoOutput = Display_Output_COMPOSITE;


  //  dAttrs.videoStd    = VideoStd_720P_60;//VideoStd_576P;//videoStd;
  //  dAttrs.videoOutput = Display_Output_COMPONENT;//args->videoOutput;

   

        dAttrs.numBufs    = NUM_DISPLAY_BUFS;
        dAttrs.colorSpace = colorSpace;

        dAttrs.width  = RSZ_WIDTH;    //736;//720;//capDim.width;
        dAttrs.height = RSZ_HEIGHT;   //capDim.height;

//		system("i2cset -f -y 1 0x0e 0x10 0x04");
//		system("i2cset -f -y 1 0x0f 0x10 0x04");

        hDisplay = Display_create(hDispBufTab, &dAttrs);

        if (hDisplay == NULL) {
            ERR("Failed to create display device\n");
            cleanup(THREAD_FAILURE);
        }

//		system("i2cset -f -y 1 0x0e 0x10 0x00");
//		system("i2cset -f -y 1 0x0f 0x10 0x00");
    }

    for (bufIdx = 0; bufIdx < VIDEO_PIPE_SIZE; bufIdx++) {
        /* Queue the video buffers for main thread processing */
        hBuf = BufTab_getFreeBuf(hFifoBufTab);
        if (hBuf == NULL) {
            ERR("Failed to fill video pipeline\n");
            cleanup(THREAD_FAILURE);            
        }

       /* Fill with black the buffer */
       CapBuf_blackFill(hBuf);

        /* Send buffer to video thread for encoding */
        if (Fifo_put(envp->hOutFifo, hBuf) < 0) {
            ERR("Failed to send buffer to display thread\n");
            cleanup(THREAD_FAILURE);
        }
    }

    /* Signal that initiaprintf("get capture buffer\n");lization is done and wait for other threads */
    Rendezvous_meet(envp->hRendezvousInit);

	////////////////////////////////////////////////////////////////////////////
	unsigned char stringbuf[20];

#if 1
//////osd not in h264 stream///////
    while (!gblGetQuit()) {

//		framenum ++;

        /* Capture a frame */
        if (Capture_get(hCapture, &hCapBuf) < 0)     //1:get capture empty buffer
		{
            ERR("Failed to get capture buffer\n");
            cleanup(THREAD_FAILURE);
        }

        /* Send buffer to video thread for encoding */
        if (Fifo_put(envp->hOutFifo, hCapBuf) < 0) {
            ERR("Failed to send buffer to display thread\n");
            cleanup(THREAD_FAILURE);
        }

		usleep(25000); /////////////////////////

		if (!envp->previewDisabled)
		{
			if(Display_get(hDisplay, &hDisBuf) < 0) 
			{
				ERR("Failed to get display buffer\n");
				cleanup(THREAD_FAILURE);
            }
			unsigned char* ptr = Buffer_getUserPtr(hCapBuf);// + 3110400;


			if(gl_system_status == STATUS_REC) //display "REC" icon
			{
				//sprintf(stringbuf, "REC\r\n");
				framesecond = gl_frame_count/25;
				//sprintf(stringbuf, "%02d:%02d\r\n", framesecond/60, framesecond%60 );
				if(gl_screen_div==0)
				{		sprintf(stringbuf, "REC\r\n");
					osd_print_string(stringbuf, 3, 320, 20, 0x0f80, ptr);
						sprintf(stringbuf, "%02d:%02d\r\n", framesecond/60, framesecond%60 );
					osd_print_string(stringbuf, 5, 50, 20, 0x0f80, ptr);   //printf record time
				}else
				{
				sprintf(stringbuf, "REC\r\n");
					osd_print_string_div(stringbuf, 3, 368, 47, 0x0f80, ptr);
					sprintf(stringbuf, "%02d:%02d\r\n", framesecond/60, framesecond%60 );
					osd_print_string_div(stringbuf, 5, 130, 47, 0x0f80, ptr);
				}



				//osd_print_string_div(stringbuf, 5, 130, 47, 0x0f80, ptr);
				//osd_print_string(stringbuf, 5, 50, 20, 0x0f80, ptr);   //printf record time

				if( (0 == gl_picture_osd) )
				{
					if(gl_screen_div==0)
						osd_print_string(gl_filename, 13, 500, 20, 0x0f80, ptr);  //printf filename
					else 
						osd_print_string_div(gl_filename, 13, 708, 47, 0x0f80, ptr);  //printf filename
				}
			}
			else //if(gl_system_status == STATUS_REC) //display "REC" icon
			{
				sprintf(stringbuf, "LIVE\r\n");
					if(gl_screen_div==0)
							osd_print_string(stringbuf, 4, 320, 20, 0x0f80, ptr);
					else
							osd_print_string_div(stringbuf, 4, 384, 47, 0x0f80, ptr);
			}



			if(1 == gl_osd_avimax)
			{
				sprintf(stringbuf, "AVI MAX\r\n");
					if(gl_screen_div==0)
						osd_print_string(stringbuf, 7, 600, 480, 0x0f80, ptr);
					else
						osd_print_string_div(stringbuf, 7, 712, 507, 0x0f80, ptr);
			}
			if(1 == gl_osd_jpgmax)
			{
				sprintf(stringbuf, "JPG MAX\r\n");
				if(gl_screen_div==0)
					osd_print_string(stringbuf, 7, 600, 520, 0x0f80, ptr);
				else
					osd_print_string_div(stringbuf, 7, 642, 547, 0x0f80, ptr);
			}

			if(gl_sdcard_full)   //display sd card full icon
			{
				if(gl_screen_div==0)
					osd_print_sdfull(20, 520, ptr);
				else
					osd_print_sdfull_div(92,547,ptr);
			}

			//====================PHOTO ICON and name===================================
			if(gl_picture_osd > 0)
			{
				if(firstjpg == 0)  //no first jpeg file
				{
					gl_picture_osd --;

					if(gl_screen_div==0)
					{
						osd_print_photo(400, 20, ptr);
						osd_print_string(gl_jpgname, 13, 500, 20, 0x0f80, ptr);  //printf filename
					}else
					{
						//osd_print_photo(400, 20, ptr);
						osd_print_photo_div(424,47,ptr);
						//osd_print_string(gl_jpgname, 13, 500, 20, 0x0f80, ptr);  //printf filename
						osd_print_string_div(gl_jpgname, 13, 708, 47, 0x0f80, ptr);  //printf filename

					}
				
				}
			}

			Buffer_setUserPtr(hDisBuf, ptr);

	        if (Display_put(hDisplay, hDisBuf) < 0) 
			{
	            ERR("Failed to put display buffer\n");
	            cleanup(THREAD_FAILURE);
	        }
		}

        /* Get a buffer from the video thread */
        fifoRet = Fifo_get(envp->hInFifo, &hDstBuf);

        if (fifoRet < 0) {
            ERR("Failed to get buffer from video thread\n");
            cleanup(THREAD_FAILURE);
        }

        /* Did the video thread flush the fifo? */
        if (fifoRet == Dmai_EFLUSH) {
            cleanup(THREAD_SUCCESS);
        }

        // Return the processed buffer to the capture driver
        if (Capture_put(hCapture, hDstBuf) < 0) {
            ERR("Failed to put capture buffer\n");
            cleanup(THREAD_FAILURE);
        } 
  
        /* Increment statistics for the user interface */
        gblIncFrames();
    }
#endif


#if 0
//osd in h264 stream/////
    while (!gblGetQuit()) {
		framenum++;

        /* Capture a frame */
        if (Capture_get(hCapture, &hCapBuf) < 0)     //11111:  get capture empty buffer
		{
            ERR("Failed to get capture buffer\n");
            cleanup(THREAD_FAILURE);
        }

		//if (!envp->previewDisabled)
		if(1)
		{
			if(Display_get(hDisplay, &hDisBuf) < 0)  //22222: get display buffer from disp driver
			{
				ERR("Failed to get display buffer\n");
				cleanup(THREAD_FAILURE);
            }
            unsigned char* ptr = Buffer_getUserPtr(hCapBuf);

#if 0
//OSD flag encode in 264 stream//////

			osd_print_cross(344, 264, ptr); //print cross icon

			if(gl_zoom_size == ZOOM_1X)
			{
				sprintf(stringbuf, "1X\r\n");
				osd_print_string(stringbuf, 2, 50, 20, 0x0f80, ptr);
			}
			else if(gl_zoom_size == ZOOM_2X)
			{
				sprintf(stringbuf, "2X\r\n");
				osd_print_string(stringbuf, 2, 50, 20, 0x0f80, ptr);
			}
			else if(gl_zoom_size == ZOOM_4X)
			{
				sprintf(stringbuf, "4X\r\n");
				osd_print_string(stringbuf, 2, 50, 20, 0x0f80, ptr);
			}


			if(gl_sdcard_full)   //display sd card full icon
				osd_print_sdfull(600, 530, ptr);

			if(gl_disp_type == 2) //colour
				osd_print_color(420, 20, ptr);
			else if(gl_disp_type == 1) //blackhot
				osd_print_blackhot(420, 20, ptr);
			else if(gl_disp_type == 0) //whitehot
				osd_print_whitehot (420, 20, ptr);
			

			if(gl_system_status == STATUS_REC) //display "REC" icon
			{
				sprintf(stringbuf, "REC\r\n");
				osd_print_string(stringbuf, 3, 320, 20, 0x0f80, ptr);
			}
			else //if(gl_system_status == STATUS_REC) //display "REC" icon
			{
				sprintf(stringbuf, "LIVE\r\n");
				osd_print_string(stringbuf, 4, 320, 20, 0x0f80, ptr);
			}

//===================BATTERY ICON====================================
			int batval = gl_adcvalue;
			char batstr[8];
			sprintf(batstr, "%d%\r\n", batval);
			osd_print_mini_str(batstr, 4, 624, 25, ptr);  //printf battery value width = 4*6=24
			osd_print_battery(658, 26, batval, ptr);

//====================PHOTO ICON===================================
			if(gl_picture_osd > 0)
			{
				gl_picture_osd --;
				osd_print_photo(350, 50, ptr);
			}
#endif

			Buffer_setUserPtr(hDisBuf, ptr);       //33333: set osd display buffer pointer

	        if (Display_put(hDisplay, hDisBuf) < 0)   //44444: return buffer to display driver 
			{
	            ERR("Failed to put display buffer\n");
	            cleanup(THREAD_FAILURE);
	        }
		}

        /* Send buffer to video thread for encoding */
        if (Fifo_put(envp->hOutFifo, hCapBuf) < 0)   /////55555
		{
            ERR("Failed to send buffer to display thread\n");
            cleanup(THREAD_FAILURE);
        }
        /* Get a buffer from the video thread */
        fifoRet = Fifo_get(envp->hInFifo, &hDstBuf);  /////66666:
        if (fifoRet < 0) {
            ERR("Failed to get buffer from video thread\n");
            cleanup(THREAD_FAILURE);
        }

        /* Did the video thread flush the fifo? */
        if (fifoRet == Dmai_EFLUSH) {
            cleanup(THREAD_SUCCESS);
        }

        // Return the processed buffer to the capture driver
        if (Capture_put(hCapture, hDstBuf) < 0)   //////77777:
		{
            ERR("Failed to put capture buffer\n");
            cleanup(THREAD_FAILURE);
        } 
        /* Increment statistics for the user interface */
        gblIncFrames();
    }
#endif


cleanup:
    /* Make sure the other threads aren't waiting for us */
    Rendezvous_force(envp->hRendezvousCapStd);
    Rendezvous_force(envp->hRendezvousInit);
    Pause_off(envp->hPauseProcess);
    Fifo_flush(envp->hOutFifo);

    /* Meet up with other threads before cleaning up */
    Rendezvous_meet(envp->hRendezvousCleanup);

    if (hDisplay) {
        Display_delete(hDisplay);
    }

    if (hCapture) {
        Capture_delete(hCapture);
    }

    /* Clean up the thread before exiting */
    if (hBufTab) {
        BufTab_delete(hBufTab);
    }
    if (hFifoBufTab) {
        BufTab_delete(hFifoBufTab);
    }
    if (hDispBufTab) {
        BufTab_delete(hDispBufTab);
    }

    return status;
}

