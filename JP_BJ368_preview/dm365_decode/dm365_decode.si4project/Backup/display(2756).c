/*
 * diplay.c
 *
 * This source file has the implementations for the 'display' function
 * for the DVSDK encodedecode demos on DM355 platform.
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
#include <string.h>

#include <xdc/std.h>

#include <ti/sdo/dmai/Time.h>
#include <ti/sdo/dmai/Fifo.h>
#include <ti/sdo/dmai/Pause.h>
#include <ti/sdo/dmai/Blend.h>
#include <ti/sdo/dmai/Display.h>
#include <ti/sdo/dmai/BufferGfx.h>
#include <ti/sdo/dmai/Rendezvous.h>
#include <ti/sdo/dmai/Resize.h>

#include "display.h"
#include "../demo.h"
#include "../ctrl.h"

/* Video display is triple buffered */
#define NUM_DISPLAY_BUFS         4
#define DISPLAYLOOPLATENCY       (40000-1) //33332

/******************************************************************************
 * encodedecode_blackFill
 ******************************************************************************/
Void encodedecode_blackFill(Buffer_Handle hBuf)
{
    switch (BufferGfx_getColorSpace(hBuf)) {

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

        default:
        {
            printf("Unsupported color space for _Dmai_blackFill\n");
            break;
        }
    }
}
/******************************************************************************
 * displayThrFxn
 ******************************************************************************/
Void *displayThrFxn_01(Void *arg)
{
    DisplayEnv             *envp       = (DisplayEnv *) arg;
    Display_Attrs           dAttrs     = Display_Attrs_DM365_VID_DEFAULT;
    Display_Handle          hDisplay   = NULL;
    Void                   *status     = THREAD_SUCCESS;
    Uns                     frameCnt   = 0;
    Buffer_Handle           hSrcBuf, hDstBuf;
    Buffer_Handle			test_buf;
    Int                     fifoRet;
    ColorSpace_Type         colorSpace = ColorSpace_YUV420PSEMI;
    BufferGfx_Attrs         gfxAttrs = BufferGfx_Attrs_DEFAULT;
    BufTab_Handle           hBufTab  = NULL;
    Int32                   bufSize;
    BufferGfx_Dimensions    srcDim;
    Int                     bufCnt = 1;

    Time_Attrs              tAttrs   = Time_Attrs_DEFAULT;
    Time_Handle             hTime    = NULL;
    Int32                   time, waitTime;

    hTime = Time_create(&tAttrs);

    if (hTime == NULL) {
        ERR("Failed to create Time object\n");
        cleanup(THREAD_FAILURE);
    }

    if(Time_reset(hTime) != Dmai_EOK) {
        ERR("Failed to reset timer\n");
        cleanup(THREAD_FAILURE);
    }

	unsigned char stringbuf[20];
    /* Signal that initialization is done and wait for other threads */
    Rendezvous_meet(envp->hRendezvousInit);
	UInt16 useMask;
	int I_testFlag  = 0 ;
//	int   
	int I_count 	= 0;
	
	Buffer_Handle	 tmp_Buffer;

    while (!gblGetQuit()) {
        /* Pause for priming? */
        Pause_test(envp->hPausePrime);
        /* Get decoded video frame */
		fifoRet = Fifo_get(envp->hInFifo, &hSrcBuf);
		if (fifoRet < 0) {
		    ERR("Failed to get buffer from video thread\n");
		    cleanup(THREAD_FAILURE);
		}
		/* Did the video thread flush the fifo? */
		if (fifoRet == Dmai_EFLUSH) {
		    cleanup(THREAD_SUCCESS);
		}
        BufferGfx_getDimensions(hSrcBuf, &srcDim);
//		srcDim.lineLength = 736;
//		BufferGfx_setDimensions(hSrcBuf, &srcDim);
        /* Prime the display driver with the first NUM_DISPLAY_BUFS buffers */
        if (bufCnt <= NUM_DISPLAY_BUFS) 
		{

            if (bufCnt == 1) {  // Create the Display at the first frame
				if(I_CutFormat){
					Display_delete(hDisplay);
					BufTab_delete(hBufTab);
		    		gfxAttrs.dim.width 		=   736;
					gfxAttrs.dim.height 	=	576;
					gfxAttrs.dim.lineLength =	736;
					gfxAttrs.dim.x		    = 	0;
					gfxAttrs.dim.y 			= 	0;
					printf(">>>>>>>>>>width       :%d\n",gfxAttrs.dim.width );
					printf(">>>>>>>>>>height      :%d\n",gfxAttrs.dim.height);
					printf(">>>>>>>>>>lineLength  :%d\n",gfxAttrs.dim.lineLength);
					printf(">>>>>>>>>>x           :%d\n",gfxAttrs.dim.x);
					printf(">>>>>>>>>>y			  :%d\n",gfxAttrs.dim.y);
				}
				gfxAttrs.dim.width =  srcDim.width;
				gfxAttrs.dim.height =  srcDim.height;
				gfxAttrs.dim.lineLength =  srcDim.lineLength;
				gfxAttrs.dim.x = srcDim.x;
				gfxAttrs.dim.y = srcDim.y;   //48
		
                if (colorSpace ==  ColorSpace_YUV420PSEMI) {
                    bufSize = gfxAttrs.dim.lineLength * gfxAttrs.dim.height * 3 / 2;
                } else {
                    bufSize = gfxAttrs.dim.lineLength * gfxAttrs.dim.height * 2;
                }

                /* Create a table of buffers to use with the device drivers */
                gfxAttrs.colorSpace = colorSpace;
                //NUM_DISPLAY_BUFS
                hBufTab = BufTab_create(1, bufSize,BufferGfx_getBufferAttrs(&gfxAttrs));
                if (hBufTab == NULL) {
                    ERR("Failed to create buftab\n");
                    cleanup(THREAD_FAILURE);
                }
    
                /* Create the display device instance */
                dAttrs.delayStreamon = TRUE;
                dAttrs.numBufs = NUM_DISPLAY_BUFS;
                dAttrs.videoStd = VideoStd_D1_PAL; //envp->videoStd;
				dAttrs.videoOutput = Display_Output_COMPOSITE;

                /* 
                 * Round down the width to a multiple of 32 as required by 
                 * display driver. Otherwise, the driver would internally round
                 * up the width, resulting in the codec padding showing up
                 * on the display when the image width is not a multiple of 32.
                 */
                dAttrs.width = ((gfxAttrs.dim.width & 0x1f) ?
                    (gfxAttrs.dim.width & ~(0x1f)) : gfxAttrs.dim.width);

                dAttrs.height = gfxAttrs.dim.height;
                dAttrs.colorSpace  = colorSpace;
                hDisplay = Display_create(hBufTab, &dAttrs);
                if (hDisplay == NULL) {
                    ERR("Failed to create display device\n");
                    cleanup(THREAD_FAILURE);
                }

            }
        
            bufCnt++;
        }else{
				/* Get a buffer from the display device driver */
		        if (Display_get(hDisplay, &hDstBuf) < 0) {
		            ERR("Failed to get display buffer\n");
		            cleanup(THREAD_FAILURE);
		        }
		       
		        /* Send buffer back to the video thread */
		        if (Fifo_put(envp->hOutFifo, hDstBuf) < 0) {
		            ERR("Failed to send buffer to video thread\n");
		            cleanup(THREAD_FAILURE);
		        }
        }

	//	Buffer_print(hSrcBuf);
        /* Incremement statistics for the user interface */
        if (Time_delta(hTime, (UInt32*)&time) < 0) {
            ERR("Failed to get timer delta\n");
            cleanup(THREAD_FAILURE);
        }
        waitTime = DISPLAYLOOPLATENCY - time;
        if(waitTime > 0) {
            usleep(waitTime);
        }
        if(Time_reset(hTime) != Dmai_EOK) {
            ERR("Failed to reset timer\n");
            cleanup(THREAD_FAILURE);
        }

		I_testFlag++;
		if(I_testFlag == 15){
  		//	I_CutFormat = 1;
  			//bufCnt = 1;
		}
	
////////////////////OSD display/////////////////////////////////////////////////
	//    Buffer_copy(hSrcBuf,tmp_Buffer);
		unsigned char* ptr = Buffer_getUserPtr(hSrcBuf);

	//	memset(ptr,0x0f80,720*576*3/2);
 	//	printf("----------\n");
		int framesecond =  gl_decode_frame/25;
		if(gl_decode_pause == 0)
		{
			sprintf(stringbuf, "               PLAY\r\n", framesecond/60, framesecond%60);
			osd_print_string(stringbuf, 20, 80, 60, 0x0f80, ptr);
		}
		else if(gl_decode_pause == 1)
		{
			sprintf(stringbuf, "               PAUSE\r\n", framesecond/60, framesecond%60);
			osd_print_string(stringbuf, 20, 80, 60, 0x0f80, ptr);
		}
		else if(gl_decode_pause == 2)
		{
			sprintf(stringbuf, "STOP \r\n");
			osd_print_string(stringbuf, 5, 320, 60, 0x0f80, ptr);
		}
////////////////////OSD display/////////////////////////////////////////////////
	           

        /* Give a filled buffer back to the display device driver */
        if (Display_put(hDisplay, hSrcBuf) < 0) {
            ERR("Failed to put display buffer\n");
            cleanup(THREAD_FAILURE);
        }
        /* Pause processing? */
 	//   Pause_test(envp->hPauseProcess);
        frameCnt++;	
    } 
cleanup:
    /* Make sure the other threads aren't waiting for us */
    Rendezvous_force(envp->hRendezvousInit);
    Pause_off(envp->hPauseProcess);
    Pause_off(envp->hPausePrime);
    Fifo_flush(envp->hOutFifo);

    /* Meet up with other threads before cleaning up */
    Rendezvous_meet(envp->hRendezvousCleanup);

    /* Clean up the thread before exiting */
    if (hDisplay) {
        Display_delete(hDisplay);
    }
    
    /* Clean up the thread before exiting */
    if (hBufTab) {
        BufTab_delete(hBufTab);
    }

    return status;
}


// static int decBufSize;
/**************************************************\
*				在yuv420中画矩形 					   *
\**************************************************/
int nv12_border(char *pic, int pic_w, int pic_h, int rect_x, int rect_y, int rect_w, int rect_h, int R, int G, int B)
{
    /* Set up the rectangle border size */
    const int border = 5;

    /* RGB convert YUV */
    int Y, U, V;
    Y =  0.299  * R + 0.587  * G + 0.114  * B;
    U = -0.1687 * R + 0.3313 * G + 0.5    * B + 128;
    V =  0.5    * R - 0.4187 * G - 0.0813 * B + 128;
	
	
    /* Locking the scope of rectangle border range */
    int j, k;
    for(j = rect_y; j < rect_y + rect_h; j++){
        for(k = rect_x; k < rect_x + rect_w; k++){
            if (k < (rect_x + border) || k > (rect_x + rect_w - border) ||\
                    j < (rect_y + border) || j > (rect_y + rect_h - border)){
                /* Components of YUV's storage address index */
                int y_index = j * pic_w + k;
                int u_index = (y_index / 2 - pic_w / 2 * ((j + 1) / 2)) * 2 + pic_w * pic_h;
                int v_index = u_index + 1;
                /* set up YUV's conponents value of rectangle border */
                pic[y_index] =  Y ;
                pic[u_index] =  U ;
                pic[v_index] =  V ;
            }
        }
    }

    return 0;
}

inline int Init_Display_Buffer_Queue(void){
		IDisplayBuffer.front = 0;
		IDisplayBuffer.rear  = 0;
		IDisplayBuffer.count = 0;
		IDisplayBuffer.cycle = 0;		
}

#define 		i_HEIGHT 		576
#define 		i_LINELEN 		800
#define 		i_ZOOM3_LINELEN	798

#define 		i_OFFSETY	48
#define 		i_OFFSETX	24

#define			i_AFTZOOMH	192
#define			i_AFTZOOMW	266
#define			i_DISZOOMW	246
/*********************************************************************\

\*********************************************************************/

void AfterZoomToDisplay(Buffer_Handle*hSrcBuf,unsigned char *ZoomPtr,int offsetx,int offsety){

        int y,x;
		unsigned char *tmptr   = ZoomPtr;
		unsigned char *ptrDst  = Buffer_getUserPtr(hSrcBuf);
		Int32  		   ySize   = Buffer_getSize(hSrcBuf) * 2 / 3;
		unsigned char *cbcrPtr = ptrDst + ySize;

        ptrDst += i_OFFSETY * i_LINELEN + i_OFFSETX + offsetx + offsety * i_LINELEN;
        for (y = 0; y < i_AFTZOOMH; y++) {
		 	memcpy(ptrDst,tmptr,i_DISZOOMW);
        	tmptr +=  i_AFTZOOMW;
            ptrDst += i_LINELEN;
        }
        
   	   cbcrPtr += (i_OFFSETY + offsety) * i_LINELEN /2  + i_OFFSETX + offsetx;
       for (y = 0; y < i_AFTZOOMH/2; y++) {
		 	memcpy(cbcrPtr,tmptr,i_DISZOOMW);
        	tmptr += i_AFTZOOMW;
            cbcrPtr += i_LINELEN;
        }
}

void Zoom_Four_Pic_Half(void){
	int i ,w,h;
	unsigned char  *yPtr;  
	Int32  ySize;    
    Int8  *cbcrPtr; 

	unsigned char *dstptr; 
	
	for(i = 0;i < 4;i++){
		dstptr  = IZoomBuffer.AfterZoomPtr[i];
		
		yPtr    = Buffer_getUserPtr(IZoomBuffer.SrcBuffer[i]);
		ySize   = Buffer_getSize(IZoomBuffer.SrcBuffer[i]) * 2 / 3; 
		cbcrPtr = yPtr + ySize;			//	16位数据 
		
		yPtr += i_OFFSETY * i_LINELEN + i_OFFSETX ;
		for (h = 0; h < i_HEIGHT ; h += 4) {
			 for(w = 0 ; w < i_LINELEN * 2; w += 4){		//800/2
				*dstptr++  = (*(yPtr - 1) + *(yPtr + 1)) / 2;
				yPtr++;
				*dstptr++  = (*(yPtr - 1) + *(yPtr + 1)) / 2;
			 	yPtr += 3;
			  }
			yPtr += i_LINELEN  * 2;
		}

		cbcrPtr += i_OFFSETY * i_LINELEN /2  + i_OFFSETX ;
		for (h = 0; h < i_HEIGHT / 2; h += 2) {
			  for(w = 0 ; w < i_LINELEN; w += 4){		//800/2
				*dstptr++ = (*(cbcrPtr - 1) + *(cbcrPtr + 1)) / 2;
				 cbcrPtr++;
				*dstptr++ = (*(cbcrPtr - 1) + *(cbcrPtr + 1)) / 2;
				 cbcrPtr += 3;
			  }
			  cbcrPtr += i_LINELEN ;
		}
	}
}

void Zoom_Four_Pic(void){			//		缩小4倍 
	int i ,w,h;
	unsigned char  *yPtr;  
	Int32  ySize;    
    Int8  *cbcrPtr; 

	unsigned char *dstptr; 
	
	for(i = 0;i < 4;i++){
		dstptr  = IZoomBuffer.AfterZoomPtr[i];
		yPtr    = Buffer_getUserPtr(IZoomBuffer.SrcBuffer[i]);
		ySize   = Buffer_getSize(IZoomBuffer.SrcBuffer[i]) * 2 / 3; 
		cbcrPtr = yPtr + ySize;			//	16位数据 
		
		yPtr += i_OFFSETY * i_LINELEN + i_OFFSETX ;
		for (h = 0; h < i_HEIGHT ; h += 6) {
			 for(w = 0 ; w < i_ZOOM3_LINELEN * 2; w += 6){		//800/2
				*dstptr++ = *yPtr++;
				*dstptr++  = *yPtr++;
			 	yPtr += 4;
			 	if(w == 792) yPtr += 2;
			  }
			  yPtr += 2;
			yPtr += i_LINELEN  * 4;
		}

		cbcrPtr += i_OFFSETY * i_LINELEN /2  + i_OFFSETX ;
		for (h = 0; h < i_HEIGHT / 2; h += 3) {
			  for(w = 0 ; w < i_ZOOM3_LINELEN; w += 6){		//800/2
				*dstptr++ = *cbcrPtr++;
				*dstptr++ = *cbcrPtr++;
				 cbcrPtr += 4;
			  }
			  cbcrPtr += 2;
			  cbcrPtr += i_LINELEN *2;
		}
	}
}


/******************************************************************************
 * displayThrFxn
 ******************************************************************************/
Void *displayThrFxn(Void *arg)
{
    DisplayEnv             *envp       = (DisplayEnv *) arg;
    Display_Attrs           dAttrs     = Display_Attrs_DM365_VID_DEFAULT;
    Display_Handle          hDisplay   = NULL;
    Void                   *status     = THREAD_SUCCESS;
    Uns                     frameCnt   = 0;
  	Buffer_Handle            hSrcBuf,getDstBuf;
    Buffer_Handle			test_buf;
    Int                     fifoRet;
    ColorSpace_Type         colorSpace = ColorSpace_YUV420PSEMI;
    BufferGfx_Attrs         gfxAttrs = BufferGfx_Attrs_DEFAULT;
    BufTab_Handle           hBufTab  = NULL;
    Int32                   bufSize;
    BufferGfx_Dimensions    srcDim;
    Int                     bufCnt = 1;

    Time_Attrs              tAttrs   = Time_Attrs_DEFAULT;
    Time_Handle             hTime    = NULL;
    Int32                   time, waitTime;

    hTime = Time_create(&tAttrs);
    if (hTime == NULL) {
        ERR("Failed to create Time object\n");
        cleanup(THREAD_FAILURE);
    }
    if(Time_reset(hTime) != Dmai_EOK) {
        ERR("Failed to reset timer\n");
        cleanup(THREAD_FAILURE);
    }
	unsigned char stringbuf[20];
	UInt16 useMask;

	gfxAttrs.dim.width 		= 736;
	gfxAttrs.dim.height 	= 576;
	gfxAttrs.dim.lineLength = 800;
	gfxAttrs.dim.x 			= 24;
	gfxAttrs.dim.y 			= 48;	 
	if (colorSpace ==  ColorSpace_YUV420PSEMI) {
		bufSize = gfxAttrs.dim.lineLength * gfxAttrs.dim.height * 3 / 2;
	} else {
		bufSize = gfxAttrs.dim.lineLength * gfxAttrs.dim.height * 2;
	}
	/* Create a table of buffers to use with the device drivers */
	gfxAttrs.colorSpace = colorSpace;
	
	hBufTab = BufTab_create(NUM_DISPLAY_BUFS, bufSize,BufferGfx_getBufferAttrs(&gfxAttrs));
	if (hBufTab == NULL) {
	ERR("Failed to create buftab\n");
	cleanup(THREAD_FAILURE);
	}

	/* Create the display device instance */
	dAttrs.delayStreamon = TRUE;
	dAttrs.numBufs = NUM_DISPLAY_BUFS;
	dAttrs.videoStd = VideoStd_D1_PAL; //envp->videoStd;
	dAttrs.videoOutput = Display_Output_COMPOSITE;

	/* 
	* Round down the width to a multiple of 32 as required by 
	* display driver. Otherwise, the driver would internally round
	* up the width, resulting in the codec padding showing up
	* on the display when the image width is not a multiple of 32.
	*/
	dAttrs.width = ((gfxAttrs.dim.width & 0x1f) ?
	(gfxAttrs.dim.width & ~(0x1f)) : gfxAttrs.dim.width);

	dAttrs.height = gfxAttrs.dim.height;
	dAttrs.colorSpace  = colorSpace;
	hDisplay = Display_create(hBufTab, &dAttrs);
	if (hDisplay == NULL) {
	ERR("Failed to create display device\n");
	cleanup(THREAD_FAILURE);
	}
	
	int i;
	for(i = 0;i < NUM_DISPLAY_BUFS;i++){
		IDisplayBuffer.hDecoderBuf[i] = Buffer_create(bufSize,BufferGfx_getBufferAttrs(&gfxAttrs));
		if (IDisplayBuffer.hDecoderBuf[i] == NULL) {
			ERR("Failed to create hDecoderBuf[%d]\n",i);
			cleanup(THREAD_FAILURE);
		}

		IZoomBuffer.SrcBuffer[i] = Buffer_create(bufSize,BufferGfx_getBufferAttrs(&gfxAttrs));
		if (IZoomBuffer.SrcBuffer[i] == NULL) {
			ERR("Failed to create SrcBuffer[%d]\n",i);
			cleanup(THREAD_FAILURE);
		}

		IZoomBuffer.AfterZoomPtr[i] = (unsigned char *)malloc(i_AFTZOOMW * i_AFTZOOMH * 3 / 2);
		if (IZoomBuffer.AfterZoomPtr[i] == NULL) {
			ERR("Failed to create DstPtr[%d]\n",i);
			cleanup(THREAD_FAILURE);
		}		
		usleep(30000);
	}
	
	Init_Display_Buffer_Queue();
	Pause_on (glb_Pause);
	Pause_test(glb_Pause);
	for(i = 0;i < NUM_DISPLAY_BUFS;i++){
		Buffer_copy(IDisplayBuffer.hDecoderBuf[i],IZoomBuffer.SrcBuffer[i]);
		if (Display_put(hDisplay,IDisplayBuffer.hDecoderBuf[i]) < 0) {
			ERR("Failed to put display buffer\n");
			cleanup(THREAD_FAILURE);
		}
		Buffer_print(IDisplayBuffer.hDecoderBuf[i]);
	}
	Zoom_Four_Pic();

while (!gblGetQuit()) {

		/* Get a buffer from the display device driver */
		if (Display_get(hDisplay, &hSrcBuf) < 0) {
			ERR("Failed to get display buffer\n");
			cleanup(THREAD_FAILURE);
		}
        /* Incremement statistics for the user interface */
        if (Time_delta(hTime, (UInt32*)&time) < 0) {
            ERR("Failed to get timer delta\n");
            cleanup(THREAD_FAILURE);
        }
        waitTime = DISPLAYLOOPLATENCY - time;
        if(waitTime > 0) {
            usleep(waitTime);
        }
        if(Time_reset(hTime) != Dmai_EOK) {
            ERR("Failed to reset timer\n");
            cleanup(THREAD_FAILURE);
        }
 
	 	encodedecode_blackFill(hSrcBuf);

		AfterZoomToDisplay(hSrcBuf,IZoomBuffer.AfterZoomPtr[0],60 ,62);
		AfterZoomToDisplay(hSrcBuf,IZoomBuffer.AfterZoomPtr[1],416,62);
		AfterZoomToDisplay(hSrcBuf,IZoomBuffer.AfterZoomPtr[2],60 ,325);
	 	AfterZoomToDisplay(hSrcBuf,IZoomBuffer.AfterZoomPtr[3],416,325);
	//	memset(ptrDst,0x0f80,800*576*3/2);
		//ptrDst += (48*800 + 24)/2;
		//memcpy(ptrDst,IZoomBuffer.AfterZoomPtr[1], 200*144*3/2);
		
	 //	nv12_border(ptrDst,800,576, 50,50,300,200, 0, 0, 255);

		//nv12_border
		//unsigned char* ptrSrc = Buffer_getUserPtr(ResizeOutBuf);
 	//	memset(ptrDst,0x0f80,800*576*3/2);
		//ptrDst += 40000;
	 	//ret = memcpy(ptrDst,ptrSrc,Buffer_getNumBytesUsed(ResizeOutBuf));

		printf("FUNCTION: %s ________ LINE: %d\n",__FUNCTION__,__LINE__);
		// memset(ptrDst,0x6666,800*576*3/2);

#if 0
		if(glob_hSrcBuf){
			unsigned char* ptrDst = Buffer_getUserPtr(hSrcBuf); 
			offset = 48*800 + 24;
			yPtr =  Buffer_getUserPtr(glob_hSrcBuf)+ offset;
			for(y = 0;y < 576; y++)
				memcpy(&ptrDst[y*736],yPtr[y*800],736);
 		  	offset = 48*800 / 2 + 24;
 		    cbcrPtr = Buffer_getUserPtr(glob_hSrcBuf) + offset + Buffer_getSize(glob_hSrcBuf) * 2 / 3;
			for (y = 0; y < 576 / 2; y++)
				memcpy(&ptrDst[736*576 + y*736],&cbcrPtr[y*800],736);
		} 
 #endif
		//Fifo_get(iPICPipe,getDstBuf);
		////////////////////OSD display/////////////////////////////////////////////////
			// Buffer_copy(hSrcBuf,tmp_Buffer);
		//printf("----------------------->FUNCTION:%s line:%d\n",__FUNCTION__,__LINE__);
	    //	unsigned char* ptrDst = Buffer_getUserPtr(getDstBuf);
	    //	unsigned char* hSrcBuf = Buffer_getUserPtr(hSrcBuf);
	    //   memset(ptrDst,0xffff,800*576*3/2);
 	 	// memcpy(hSrcBuf, getDstBuf, Buffer_getNumBytesUsed(hDumpBuf));
	#if 0
	 	int framesecond =  gl_decode_frame/25;
		if(gl_decode_pause == 0)
		{
			sprintf(stringbuf, "               PLAY\r\n", framesecond/60, framesecond%60);
			osd_print_string(stringbuf, 20, 80, 60, 0x0f80, ptr);
		}
	

      if(0){
		if (Display_put(hDisplay,glob_hSrcBuf) < 0) {
			ERR("Failed to put display buffer\n");
			cleanup(THREAD_FAILURE);
		}
      }else{

   #endif
   
 		Buffer_print(hSrcBuf);
		if (Display_put(hDisplay,hSrcBuf) < 0) {
		    ERR("Failed to put display buffer\n");
		    cleanup(THREAD_FAILURE);
		}
	    usleep(10000);
        frameCnt++;	
        Pause_on (glb_Pause);
		Pause_test(glb_Pause);	
  	}
  
cleanup:
    Rendezvous_force(envp->hRendezvousInit);
    Pause_off(envp->hPauseProcess);
    Pause_off(envp->hPausePrime);

    /* Meet up with other threads before cleaning up */
    Rendezvous_meet(envp->hRendezvousCleanup);

	for(i = 0;i < NUM_DISPLAY_BUFS;i++)
		Buffer_delete(IDisplayBuffer.hDecoderBuf[i]);
    /* Clean up the thread before exiting */
    if (hDisplay) {
        Display_delete(hDisplay);
    }
    
    /* Clean up the thread before exiting */
    if (hBufTab) {
        BufTab_delete(hBufTab);
    }

    return status;
}

