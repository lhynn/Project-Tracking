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
#include "jpg_preview.h"

/* Video display is triple buffered */
#define NUM_DISPLAY_BUFS         4
#define DISPLAYLOOPLATENCY       (40000-1) //33332

#define 		i_OFFSETY			48
#define 		i_OFFSETX			24

#define 		i_HEIGHT 			576
#define 		i_LINELEN 			800

#define 		i_ZOOM3_LINELEN		798
/*
#define			i_AFTZOOMH			192
#define			i_AFTZOOMW			266
#define			i_DISZOOMW			246
*/

#define 		i_ZOOM5_12_LINELEN		792//798 5/12

#define			i_AFTZOOMH				240
#define			i_AFTZOOMW				330
#define			i_DISZOOMW				308

#define 	 	CIRCULARADIUS			 44



Int DrawCirculARadius(unsigned char *AfterZoomPtr){
		int y,x,tmp;
		int CenterX = 148,CenterY = 114;
		unsigned char *DstPtr = AfterZoomPtr;

		int StartX = CenterX - 38,StartY = CenterY - 38;
		DstPtr += StartY * 330 + StartX;
		for(y = 0 ;y < 76;y++){
		    for(x = 0 ;x < 76;x++){
		    	tmp = (x - 38)*(x - 38) + (y - 38)*(y - 38);
		    	if(tmp < 38*38){
		    		if(tmp > 36*36)
							*(DstPtr + y * 330 + x) = 0x0;
		    		else if(tmp > 30*30)
							*(DstPtr + y * 330 + x) = 0xff;
				}
			}
		}
		StartX = 20;
		StartY = 20;
		DstPtr += StartY * 330 + StartX;
		for(y = 0 ;y < 40;y++){
		    for(x = 0 ;x < 40;x++){
		    	if(x > 6 && 2*y > 1*x && 2*y < -1*x + 80){
					if(x > 8 && 2*y > 1*x - 4 && 2*y < -1*x + 84)
						*(DstPtr + y * 330 + x) = 0xff;
		    		else
						*(DstPtr + y * 330 + x) = 0x0;
		    	}			
			}
		}
		return 0;
}

/******************************************************************************
 * encodedecode_blackFill
 ******************************************************************************/
Void Clear_Screen_Black_Fill(Buffer_Handle hBuf)
{
            Int8  *yPtr     = Buffer_getUserPtr(hBuf);
            Int32  ySize    = Buffer_getSize(hBuf) * 2 / 3;
            Int8  *cbcrPtr  = yPtr + ySize;
            Int    y;
            
            yPtr += i_OFFSETY * i_LINELEN + i_OFFSETX;
            for (y = 0; y < i_HEIGHT; y++) {
                memset(yPtr, 0x0,i_LINELEN);
                yPtr += i_LINELEN;
            }
            cbcrPtr += i_OFFSETY * i_LINELEN/ 2 + i_OFFSETX;
            for (y = 0; y < i_HEIGHT / 2; y++) {
                memset(cbcrPtr, 0x80,i_LINELEN);
                cbcrPtr += i_LINELEN;
            }
}

/*
 *Yuv420_Paint_Rect: 在屏幕上画矩形
 *		[in] 	hSrcBuf  ： 需要Display缓冲区
 *		[in] 	rect_x   ： 相对屏幕水平偏移
 *		[in] 	rect_y   ： 相对屏幕垂直偏移	   
 *		[in] 	rect_w   ： 矩形框的宽度
 *		[in] 	rect_h   ： 矩形框的高度
 *		[in] 	Y   	 ： 矩形的颜色
 *		[in] 	U   	 ： 
 *		[in] 	V   	 ： 
 */
Void Yuv420_Paint_Rect(Buffer_Handle *hSrcBuf, int rect_x, int rect_y, int rect_w, int rect_h, int Y, int U, int V)
{
	int j, k;
	const int border = 4;
	unsigned char *ptrDst	= Buffer_getUserPtr(hSrcBuf);
	Int32			ySize	= Buffer_getSize(hSrcBuf) * 2 / 3;
	unsigned char *cbcrPtr = ptrDst + ySize;

 	ptrDst  += i_OFFSETY * i_LINELEN + i_OFFSETX + rect_x + rect_y * i_LINELEN;
 	cbcrPtr += (i_OFFSETY + rect_y) * i_LINELEN /2  + i_OFFSETX + rect_x;

	
    for(j = rect_y; j < rect_y + rect_h; j++){
        for(k = rect_x; k < rect_x + rect_w; k++){
            if (k < (rect_x + border) || k > (rect_x + rect_w - border) ||\
                j < (rect_y + border) || j > (rect_y + rect_h - border)){
                /* Components of YUV's storage address index */
                int y_index = j * i_LINELEN + k;
			  	ptrDst [y_index]     = Y;
            }
        }
    }
    for(j = rect_y; j < rect_y + rect_h; j += 2){
        for(k = rect_x; k < rect_x + rect_w; k += 2){
            if (k < (rect_x + border) || k > (rect_x + rect_w - border) ||\
                j < (rect_y + border) || j > (rect_y + rect_h - border)){
				int u_index = j * i_LINELEN / 2 + k;
				cbcrPtr[u_index]     = U;
				cbcrPtr[u_index + 1] = V;
            }
        }
    }
  
}

inline int Init_Display_Buffer(void){
		IDisplayBuffer.front = 0;
		IDisplayBuffer.rear  = 0;
		IDisplayBuffer.count = 0;
		IDisplayBuffer.cycle = 0;		
}

/*After_Zoom_To_Display:
 *		[in] 	hSrcBuf       ： 需要Display缓冲区
 *		[in] 	ZoomPtr       ： 需要显示的数据
 *		[in] 	offsetx   	  ： 相对显示Buffer水平方向位移	   
 *		[in] 	offsety   	  ： 相对显示Buffer垂直方向位移
 */
Void After_Zoom_To_Display(Buffer_Handle *hSrcBuf,unsigned char *ZoomPtr,int offsetx,int offsety){

        int y,x;
		unsigned char *tmptr   = ZoomPtr;
		unsigned char *ptrDst  = Buffer_getUserPtr(hSrcBuf);
		Int32  		   ySize   = Buffer_getSize(hSrcBuf) * 2 / 3;
		unsigned char *cbcrPtr  = ptrDst + ySize;

        ptrDst += (offsety + i_OFFSETY) * i_LINELEN + i_OFFSETX + offsetx;
        for (y = 0; y < i_AFTZOOMH; y++) {
            memcpy(ptrDst,tmptr,i_DISZOOMW);
        	tmptr  += i_AFTZOOMW;
            ptrDst += i_LINELEN;
        }
		cbcrPtr += (offsety + i_OFFSETY)* i_LINELEN /2  + i_OFFSETX + offsetx ;
		for (y = 0; y < i_AFTZOOMH/2; y++) {
			memcpy(cbcrPtr,tmptr,i_DISZOOMW);
			tmptr   += i_AFTZOOMW;
			cbcrPtr += i_LINELEN;
		}
}

/*nv12_nearest_scale:
 *		[in] 	SrcBuffer     ： 源数据buffer
 *		[out] 	dst           ： 目标的数据
 *		[in] 	srcWidth   	  ： 源数据水平宽度   
 *		[in] 	srcHeight     ： 源数据垂直宽度    
  *		[in] 	dstWidth   	  ： 目标数据水平宽度   
 *		[in] 	dstHeight     ： 目标数据垂直宽度   
 */
void nv12_nearest_scale(Buffer_Handle SrcBuffer, unsigned char *dst, \
					int srcWidth, int srcHeight, int dstWidth, int dstHeight)     
{

	unsigned char  *src;  
	Int32  ySize;	 
	unsigned char* src_uv;
	src     = Buffer_getUserPtr(SrcBuffer);	
	ySize	= Buffer_getSize(SrcBuffer) * 2 / 3; 
	src_uv  = src + ySize; 	
	src	   += i_OFFSETY * i_LINELEN + i_OFFSETX ;
    src_uv += i_OFFSETY * i_LINELEN /2  + i_OFFSETX ;

    register int sw = srcWidth;  //register keyword is for local var to accelorate 
    register int sh = srcHeight;
    register int dw = dstWidth;
    register int dh = dstHeight;
    register int y, x;
    unsigned long int srcy, srcx, src_index, dst_index;
    unsigned long int xrIntFloat_16 = (sw << 16) / dw + 1; //better than float division
    unsigned long int yrIntFloat_16 = (sh << 16) / dh + 1;

    unsigned char* dst_uv = dst + dh * dw; //memory start pointer of dest uv
    unsigned char* dst_uv_yScanline;
    unsigned char* src_uv_yScanline;
    unsigned char* dst_y_slice = dst; //memory start pointer of dest y
    unsigned char* src_y_slice;
    unsigned char* sp;
    unsigned char* dp;
 
    for (y = 0; y < (dh & ~7); ++y)  //'dh & ~7' is to generate faster assembly code
    {
        srcy = (y * yrIntFloat_16) >> 16;
        src_y_slice = src + srcy * sw;

        if((y & 1) == 0)
        {
            dst_uv_yScanline = dst_uv + (y / 2) * dw;
            src_uv_yScanline = src_uv + (srcy / 2) * sw;
        }

        for(x = 0; x < (dw & ~7); ++x)
        {
            srcx = (x * xrIntFloat_16) >> 16;
            dst_y_slice[x] = src_y_slice[srcx];

            if((y & 1) == 0) //y is even
            {
                if((x & 1) == 0) //x is even
                {
                    src_index = (srcx / 2) * 2;
            
                    sp = dst_uv_yScanline + x;
                    dp = src_uv_yScanline + src_index;
                    *sp = *dp;
                    ++sp;
                    ++dp;
                    *sp = *dp;
                }
             }
         }
         dst_y_slice += dw;
    }
    DrawCirculARadius(dst);
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
	
	Init_Display_Buffer();
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
	
	//Zoom_Decode_Avi_Third_5_12(IZoomBuffer.SrcBuffer,IZoomBuffer.AfterZoomPtr,4);
	for(i = 0;i < NUM_DISPLAY_BUFS;i++){
		nv12_nearest_scale(IZoomBuffer.SrcBuffer[i],IZoomBuffer.AfterZoomPtr[i],i_LINELEN,i_HEIGHT,i_AFTZOOMW,i_AFTZOOMH);
	}
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
 
	  	Clear_Screen_Black_Fill(hSrcBuf);

     	After_Zoom_To_Display(hSrcBuf,IZoomBuffer.AfterZoomPtr[0],30 ,14);
		After_Zoom_To_Display(hSrcBuf,IZoomBuffer.AfterZoomPtr[1],382,14);
		After_Zoom_To_Display(hSrcBuf,IZoomBuffer.AfterZoomPtr[1],30 ,294);
		After_Zoom_To_Display(hSrcBuf,IZoomBuffer.AfterZoomPtr[1],382,294);
		//	memset(ptrDst,0x0f80,800*576*3/2);
		//ptrDst += (48*800 + 24)/2;
		//memcpy(ptrDst,IZoomBuffer.AfterZoomPtr[1], 200*144*3/2);
		
	 	//nv12_border(hSrcBuf,56-24,58-48,270,196,0,0,255);
		//Yuv420_Paint_Rect(hSrcBuf,56-24,58-48,270,196,60,60,60);

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

