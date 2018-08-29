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
 * This application decodes an elementary stream image file using a specified
 * codec to a raw yuv file using an xDM 1.0 codec. The format of the yuv file
 * depends on the device, for dm6446 amd dm355 it's 422 interleaved.
 */

#include <stdio.h>
#include <string.h>

#include <xdc/std.h>

#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/CERuntime.h>

#include <ti/sdo/dmai/Dmai.h>
#include <ti/sdo/dmai/Time.h>
#include <ti/sdo/dmai/Buffer.h>
#include <ti/sdo/dmai/ColorSpace.h>
#include <ti/sdo/dmai/ce/Idec1.h>
#include <ti/sdo/dmai/BufferGfx.h>
#include <ti/sdo/dmai/Cpu.h>

#include "appMain.h"

/* Align buffers to this cache line size (in bytes)*/
#define BUFSIZEALIGN            128

/* vbuf size that has been selected based on size/performance tradeoff */
#define VBUFSIZE                20480

static Char vbuffer[VBUFSIZE];



/******************************************************************************
 * writeFrameSemiPlanar
******************************************************************************/
Int writeFrameSemiPlanar(Buffer_Handle hBuf, FILE *outFile)
{
    Int8   *yPtr, *cbcrPtr;        
    UInt32  ySize;                /* Size of Luma data */
    Uint32  cbcrSize;             /* Size of Chroma (cb and cr) data */
    Char   *colorFmt;
    Int     y, x;

    BufferGfx_Dimensions dim;
    BufferGfx_getDimensions(hBuf, &dim);    

    /*  The three color planes are evenly spaced in the output buffer. Each 
     *  plane has been allocated a size equal to the input creation parameter 
     *  maxWidth x maxHeight.              
     */         
    ySize = dim.width * dim.height;

	cbcrSize = dim.width * dim.height / 2;
	colorFmt = "420P";

    /* Write Y plane */
    yPtr = Buffer_getUserPtr(hBuf);
    if (fwrite(yPtr, 1, ySize, outFile) != ySize ) {
        fprintf(stderr,"Failed to write y data to disk\n");
        return -1;
    }

    /* Separate Cb from CbCr interleaved and save Cb plane */
    cbcrPtr = Buffer_getUserPtr(hBuf) + Buffer_getSize(hBuf) * 2/3;
    for (y = 0; y < dim.height / 2; y++) {
      for (x = 0; x < dim.width; x += 2) {
        if (fwrite(&cbcrPtr[x], 1, 1, outFile) != 1) {
            fprintf(stderr,"Failed to write data to disk\n");
            return -1;
        }
      }
      cbcrPtr += dim.lineLength;
    }

    /* Separate Cr from CbCr interleaved and save Cr plane */
    cbcrPtr = Buffer_getUserPtr(hBuf) + Buffer_getSize(hBuf) * 2 / 3;
    for (y = 0; y < dim.height / 2; y++) {
      for (x = 1; x < dim.width; x += 2) {
        if (fwrite(&cbcrPtr[x], 1, 1, outFile) != 1) {
            fprintf(stderr,"Failed to write data to disk\n");
            return -1;
        }
      }
      cbcrPtr += dim.lineLength;
    }

    printf("Wrote %s image size %d (%dx%d) to disk\n",
           colorFmt, 
           (Int) (ySize + cbcrSize),
           (Int) dim.width, (Int) dim.height);

    return 0;
}
/******************************************************************************
 * appMain
 ******************************************************************************/
Int appMain(Args * args)
{
    IMGDEC1_Params          params       = Idec1_Params_DEFAULT;
    IMGDEC1_DynamicParams   dynParams    = Idec1_DynamicParams_DEFAULT;
    Buffer_Attrs            bAttrs       = Buffer_Attrs_DEFAULT;
    BufferGfx_Attrs         gfxAttrs     = BufferGfx_Attrs_DEFAULT;
    Idec1_Handle            hId          = NULL;
    Engine_Handle           hEngine      = NULL;
    Buffer_Handle           hInBuf       = NULL;
    Buffer_Handle           hOutBuf      = NULL;
    FILE                   *outFile      = NULL;
    FILE                   *inFile       = NULL;
    Int                     numBytes     = 0;
    Int                     ret          = Dmai_EOK;

    printf("Starting application...\n");
     
    /* Initialize the codec engine run time */
    CERuntime_init();
    /* Initialize DMAI */
    Dmai_init();

    /* Open input file */
    inFile = fopen(args->inFile, "rb");

    if (inFile == NULL) {
        ret = Dmai_EFAIL;
        fprintf(stderr,"Failed to open file %s\n", args->inFile);
        goto cleanup;
    }
    
    /* Open output file */
    outFile = fopen(args->outFile, "wb");
    if (outFile == NULL) {
        ret = Dmai_EFAIL;
        fprintf(stderr,"Failed to create output file %s\n", args->outFile);
        goto cleanup;
    }

    /* Using a larger vbuf to enhance performance of file i/o */
    if (setvbuf(outFile, vbuffer, _IOFBF, sizeof(vbuffer)) != 0) {
        ret = Dmai_EFAIL;
        fprintf(stderr,"Failed to setvbuf on file descriptor\n");
        goto cleanup;   
    }
    
    /* Open the codec engine */
    hEngine = Engine_open(args->engineName, NULL, NULL);
    if (hEngine == NULL) {
        ret = Dmai_EFAIL;
        fprintf(stderr,"Failed to open codec engine %s\n", args->engineName);
        goto cleanup;
    }

	params.forceChromaFormat = XDM_YUV_420SP;
	params.maxHeight = VideoStd_720P_HEIGHT;
	params.maxWidth  = VideoStd_720P_WIDTH;

    /* Create the image decoder */
    hId = Idec1_create(hEngine, args->codecName, &params, &dynParams);
    if (hId == NULL) {
        ret = Dmai_EFAIL;
        fprintf(stderr,"Failed to create image decoder: %s\n", args->codecName);
        goto cleanup;
    }

    /* Align buffers to cache line boundary */    
    gfxAttrs.bAttrs.memParams.align = bAttrs.memParams.align = BUFSIZEALIGN;  
    gfxAttrs.colorSpace     = args->oColorSpace;
    gfxAttrs.dim.width      = params.maxWidth;
    gfxAttrs.dim.height     = params.maxHeight;
    gfxAttrs.dim.lineLength = BufferGfx_calcLineLength(params.maxWidth,
                                                       gfxAttrs.colorSpace);
    
	/* Create an output buffer for decoded data */
	hOutBuf = Buffer_create(
	Dmai_roundUp(Idec1_getOutBufSize(hId), BUFSIZEALIGN), 
	BufferGfx_getBufferAttrs(&gfxAttrs));

    if (hOutBuf == NULL) {
        ret = Dmai_EFAIL;
        fprintf(stderr,"Failed to create contiguous buffers\n");
        goto cleanup;
    }

    /* Create an input buffer for encoded data */
    hInBuf = Buffer_create(Dmai_roundUp(Idec1_getInBufSize(hId), BUFSIZEALIGN),&bAttrs);
    if (hInBuf == NULL) {
        ret = Dmai_EFAIL;
        fprintf(stderr,"Failed to create contiguous buffers\n");
        goto cleanup;
    }
   
    /* Read encoded image data */
    numBytes = fread(Buffer_getUserPtr(hInBuf), 1,Idec1_getInBufSize(hId), inFile);             
    Buffer_setNumBytesUsed(hInBuf, numBytes);
     
    printf("Decoding image...\n");
        
    /* Decode the image frame */
    ret = Idec1_process(hId, hInBuf, hOutBuf);
    Buffer_print(hOutBuf);
    if (ret < 0) {
        ret = Dmai_EFAIL;
        fprintf(stderr,"Failed to decode image buffer\n");
        goto cleanup;
    }

    if (writeFrameSemiPlanar(hOutBuf, outFile) < 0) {
       ret = Dmai_EFAIL;
       fprintf(stderr,"Failed to write image to file\n");
       goto cleanup; 
   }

cleanup:
    /* Clean up the application */
    if (hId) 
        Idec1_delete(hId);

    if (hInBuf) 
        Buffer_delete(hInBuf);

    if (hOutBuf) 
        Buffer_delete(hOutBuf);

    if (hEngine) 
        Engine_close(hEngine);

  
    if (inFile) 
        fclose(inFile);

    if (outFile) 
        fclose(outFile);
    printf("End of application.\n");
    if (ret == Dmai_EFAIL) 
        return 1;
    else    
        return 0;
}
