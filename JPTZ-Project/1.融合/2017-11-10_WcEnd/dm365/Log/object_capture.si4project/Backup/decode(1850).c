/* --COPYRIGHT--,BSD
 * Copyright (c) 2010, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*
 * This applications decodes an elementary stream video file using a specified
 * codec to a raw yuv file. The format of the yuv file depends on the device,
 * for dm6467 it's 420 planar while on dm355 it's 422 interleaved.
 */

#include <stdio.h>
#include <string.h>

#include <xdc/std.h>

#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/CERuntime.h>

#include <ti/sdo/dmai/Dmai.h>
#include <ti/sdo/dmai/Time.h>
#include <ti/sdo/dmai/Cpu.h>
#include <ti/sdo/dmai/BufTab.h>
#include <ti/sdo/dmai/Loader.h>
#include <ti/sdo/dmai/Display.h>
#include <ti/sdo/dmai/ColorSpace.h>
#include <ti/sdo/dmai/VideoStd.h>
#include <ti/sdo/dmai/ce/Vdec2.h>
#include <ti/sdo/dmai/BufferGfx.h>
#include "../demo.h"



/* Align buffers to this cache line size (in bytes)*/
#define BUFSIZEALIGN            128

/* vbuf size that has been selected based on size/performance tradeoff */
#define VBUFSIZE                20480
 
static Char vbuffer[VBUFSIZE];

/* These definitions missing in some OS build environments (eg: WinCE) */
#ifndef _IOFBF
    #define _IOFBF  0
#endif  /* _IOFBF */
#ifndef _IOLBF
    #define _IOLBF  1
#endif  /* _IOLBF */
#ifndef _IONBF
    #define _IONBF  2
#endif  /* _IONBF */

/******************************************************************************
 * resizeBufTab
******************************************************************************/
Int resizeBufTab(Vdec2_Handle hVd2, Int displayBufs)
{
    BufTab_Handle hBufTab = Vdec2_getBufTab(hVd2);
    Int numBufs, numCodecBuffers, numExpBufs;
    Buffer_Handle hBuf;
    Int32 frameSize;

    /* How many buffers can the codec keep at one time? */
    numCodecBuffers = Vdec2_getMinOutBufs(hVd2);

    if (numCodecBuffers < 0) {
        fprintf(stderr, "Failed to get buffer requirements\n");
        return -1;
    }

    /*
     * Total number of frames needed are the number of buffers the codec
     * can keep at any time, plus the number of frames in the display pipe.
     */
    numBufs = numCodecBuffers + displayBufs;

    /* Get the size of output buffers needed from codec */
    frameSize = Vdec2_getOutBufSize(hVd2);

    /*
     * Get the first buffer of the BufTab to determine buffer characteristics.
     * All buffers in a BufTab share the same characteristics.
     */
    hBuf = BufTab_getBuf(hBufTab, 0);

    /* Do we need to resize the BufTab? */
    if (numBufs > BufTab_getNumBufs(hBufTab) ||
        frameSize < Buffer_getSize(hBuf)) {

        /* Should we break the current buffers in to many smaller buffers? */
        if (frameSize < Buffer_getSize(hBuf)) {

            /* First undo any previous chunking done */
            BufTab_collapse(Vdec2_getBufTab(hVd2));

            /*
             * Chunk the larger buffers of the BufTab in to smaller buffers
             * to accomodate the codec requirements.
             */
            numExpBufs = BufTab_chunk(hBufTab, numBufs, frameSize);

            if (numExpBufs < 0) {
                fprintf(stderr, "Failed to chunk %d bufs size %d to %d bufsize %d\n",
                    BufTab_getNumBufs(hBufTab), (Int)Buffer_getSize(hBuf),
                    numBufs, (Int)frameSize);
                return -1;
            }

            /*
             * Did the current BufTab fit the chunked buffers,
             * or do we need to expand the BufTab (numExpBufs > 0)?
             */
            if (BufTab_expand(hBufTab, numExpBufs) < 0) {
                fprintf(stderr, "Failed to expand BufTab with %d buffers\n",
                       numExpBufs);
                return -1;
            }
        }
        else {
            /* Just expand the BufTab with more buffers */
            if (BufTab_expand(hBufTab, numBufs - BufTab_getNumBufs(hBufTab)) < 0) {
                fprintf(stderr,"Failed to expand BufTab with %d buffers\n",
                       numCodecBuffers);
                return -1;
            }
        }
    }

    return numBufs;
}

unsigned char * writeFrame420P(Buffer_Handle hBuf,unsigned char *tmp)
{
    Int8 *yPtr, *cbcrPtr;
    UInt32 offset;
    Int y, x;
	Buffer_Handle		  hDisBuf;	
	Int8 *ptr = NULL;
	BufferGfx_Dimensions dim;

	BufferGfx_getDimensions(hBuf, &dim);
    offset = dim.y * dim.lineLength + dim.x;

    yPtr = Buffer_getUserPtr(hBuf) + offset;
    for(y = 0;y < dim.height; y++)
    	memcpy(&tmp[y*dim.width],&yPtr[y*dim.lineLength],dim.width);
    
    offset = dim.y * dim.lineLength / 2 + dim.x;
    cbcrPtr = Buffer_getUserPtr(hBuf) + offset + Buffer_getSize(hBuf) * 2 / 3;
	
    for (y = 0; y < dim.height / 2; y++) 
		memcpy(&tmp[dim.height*dim.width + y*dim.width],&cbcrPtr[y*dim.lineLength],dim.width);
 
    return NULL;
}

/******************************************************************************
 * appMain
 ******************************************************************************/
Void *decodeThrFxn(Void *arg)
{
	Display_Handle		  hDisplay = (Display_Handle )arg;

    VIDDEC2_Params          params       = Vdec2_Params_DEFAULT;
    VIDDEC2_DynamicParams   dynParams    = Vdec2_DynamicParams_DEFAULT;
    Loader_Attrs            lAttrs       = Loader_Attrs_DEFAULT;
    BufferGfx_Attrs         gfxAttrs     = BufferGfx_Attrs_DEFAULT;
    Time_Attrs              tAttrs       = Time_Attrs_DEFAULT;
    Vdec2_Handle            hVd2         = NULL;
    Loader_Handle           hLoader      = NULL;
    Engine_Handle           hEngine      = NULL;
    BufTab_Handle           hBufTab      = NULL;
    Buffer_Handle           hDstBuf      = NULL;
    Time_Handle             hTime        = NULL;
    FILE                   *outFile      = NULL;
    Bool                    flushed      = FALSE;
    Int                     ret          = Dmai_EOK;
    Int                     numFrame     = 1;
    Int                     numFrameDisp = 1;
    Buffer_Handle           hInBuf, hOutBuf, hFreeBuf;
    ColorSpace_Type         colorSpace;
    Cpu_Device              device;
    Int                     numBufs;
    UInt32                  time;
    BufferGfx_Dimensions    dim;
    Bool                    vc1Offset   = FALSE;
    Bool                    mpeg4Offset = FALSE;
	Buffer_Handle		  hDisBuf;	
	unsigned char* ptr =NULL; //Buffer_getUserPtr(hOutBuf);
	unsigned char* ptr_tmp =NULL;// Buffer_getUserPtr(hDisBuf);
	char *mine[20];
	int me = 0;


    printf("Starting application...\n");
    
    colorSpace = ColorSpace_YUV420PSEMI;
    numBufs    = 5;
  

    outFile = fopen("z.yuv", "wb");

    /* Open the codec engine */
    hEngine = Engine_open("encodedecode", NULL, NULL);
    if (hEngine == NULL) 
	{
        ret = Dmai_EFAIL;
        fprintf(stderr, "Failed to open codec engine \n");
        goto cleanup;
    }
	
    params.forceChromaFormat    = XDM_YUV_420SP;
  	params.maxWidth			  = 800;
	params.maxHeight 		  = 576;
    /* Create the XDM 1.2 based video decoder */
    hVd2 = Vdec2_create(hEngine, "h264dec", &params, &dynParams);
    if (hVd2 == NULL) 
    {
        ret = Dmai_EFAIL;
        fprintf(stderr,"Failed to create video decoder: \n");
        goto cleanup;
    }

    /* Align buffers to cache line boundary */    
    gfxAttrs.bAttrs.memParams.align = lAttrs.mParams.align = BUFSIZEALIGN; 
    gfxAttrs.colorSpace     = colorSpace;
    gfxAttrs.dim.width      = params.maxWidth;
    gfxAttrs.dim.height     = params.maxHeight;
    gfxAttrs.dim.lineLength = BufferGfx_calcLineLength(params.maxWidth,
                                                       colorSpace);
    /* Create a table of output buffers of the size requested by the codec */
    hBufTab = BufTab_create(numBufs, 
        Dmai_roundUp(Vdec2_getOutBufSize(hVd2), BUFSIZEALIGN),
        BufferGfx_getBufferAttrs(&gfxAttrs));
    if (hBufTab == NULL) 
	{
        ret = Dmai_EFAIL;
        fprintf(stderr,"Failed to allocate contiguous buffers\n");
        goto cleanup;
    }

    /* Set output buffer table */
    Vdec2_setBufTab(hVd2, hBufTab);
    /* Ask the codec how much input data it needs */
    lAttrs.readSize = Vdec2_getInBufSize(hVd2);
    /* Make the total ring buffer larger */
    lAttrs.readBufSize = Dmai_roundUp(lAttrs.readSize * 2, BUFSIZEALIGN); 
    /* Increase the stdio buffer size for loader for better RTDX performance */
    lAttrs.vBufSize = VBUFSIZE;
    /* Create the elementary stream file loader */
    hLoader = Loader_create("FILE_264_0002.264", &lAttrs);
    if (hLoader == NULL) 
	{
        ret = Dmai_EFAIL;
        fprintf(stderr, "Failed to create loader\n");
        goto cleanup;
    }

    /* Prime the file loader */
    Loader_prime(hLoader, &hInBuf);
    printf("Loader created and primed, entering main loop..\n");

    /* Main loop */
    while (1) 
	{
          hDstBuf = BufTab_getFreeBuf(hBufTab);
          if (hDstBuf == NULL){
                ret = Dmai_EFAIL;
                fprintf(stderr, "Failed to get a free contiguous buffer from BufTab\n");
                BufTab_print(hBufTab);
                goto cleanup;
         }
        
        /* Make sure the whole buffer is used for output */
        BufferGfx_resetDimensions(hDstBuf);
        /* Decode the video buffer */
        ret = Vdec2_process(hVd2, hInBuf, hDstBuf);
        if (ret < 0) 
		{
            ret = Dmai_EFAIL;
            fprintf(stderr, "Failed to decode video buffer\n");
            goto cleanup;
        }

        if (numFrame == 1) 
		{
            /* Resize the BufTab after the first frame has been processed */
            numBufs = resizeBufTab(hVd2, numBufs);
            if (numBufs < 0) 
			{
                ret = 1;
                goto cleanup;
            }
        }
        hOutBuf = Vdec2_getDisplayBuf(hVd2);
        if (flushed&&(hOutBuf==NULL)){
            Buffer_delete(hInBuf);
            break;
        }

        while (hOutBuf) 
		{
		
          	  if (numFrameDisp++ >= 1) {
			  //	if(me== 0){
	          //	 printf("Display Frame %d: ", numFrameDisp - 1);
				 
        		if(Display_get(hDisplay, &hDisBuf) < 0) 
				{
					 printf("Failed to get display buffer\n");
					 return -1;
				}
				ptr_tmp = Buffer_getUserPtr(hDisBuf);
				ptr = writeFrame420P(hOutBuf,ptr_tmp);
			
		   		Buffer_setUserPtr(hDisBuf, ptr_tmp);
				if (Display_put(hDisplay, hDisBuf) < 0) {
					//printf("Failed to put display buffer\n");
					//return -1;
				}  
					//me += 2;
			 	//}else {me--;}
				gblIncFrames();	  
            }
            hOutBuf = Vdec2_getDisplayBuf(hVd2);
        }

        /* Get a buffer to free from the codec */
        hFreeBuf = Vdec2_getFreeBuf(hVd2);
        while (hFreeBuf) 
		{
            BufTab_freeBuf(hFreeBuf);
            hFreeBuf = Vdec2_getFreeBuf(hVd2);
        }

        if (!flushed) 
		{
            /* Load a new frame from the file system */
            Loader_getFrame(hLoader, hInBuf);
			numFrame++;
            if ( Buffer_getUserPtr(hInBuf) == NULL){
                flushed = TRUE;
                Vdec2_flush(hVd2);
                hInBuf = Buffer_create(1, BufferGfx_getBufferAttrs(&gfxAttrs));
                if (hInBuf == NULL) 
				{
                    ret = Dmai_EFAIL;
                    fprintf(stderr, "Failed to allocate dummy buffer\n");
                    goto cleanup;
                }   
                Buffer_setNumBytesUsed(hInBuf, 1);
            }   
        }

        if (flushed) 
		{
            Buffer_setNumBytesUsed(hInBuf, 1);
        }
    }

cleanup:
    /* Clean up the application */
    if (hLoader) {
        Loader_delete(hLoader);
    }

    if (hVd2) {
        Vdec2_delete(hVd2);
    }

    if (hBufTab) {
        BufTab_delete(hBufTab);
    }

    if (hEngine) {
        Engine_close(hEngine);
    }

    if (hTime) {
        Time_delete(hTime);
    }

    if (outFile) {
        fclose(outFile);
    }
    printf("End of application.\n");

    if (ret == Dmai_EFAIL)     
        return 1;
    else
        return 0;
}
