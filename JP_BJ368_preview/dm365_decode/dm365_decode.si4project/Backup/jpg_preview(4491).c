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

#include "jpg_preview.h"

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

Int Decode_JPG_Preview(void)
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

    /* Open input file */
    inFile = fopen("/mnt/JPG_00001.jpg", "rb");

    if (inFile == NULL) {
        ret = Dmai_EFAIL;
        printf("Failed to open input file \n");
        goto cleanup;
    }
    
    /* Open output file */
    outFile = fopen("out.yuv", "wb");
    if (outFile == NULL) {
        ret = Dmai_EFAIL;
        printf("Failed to open output file\n");
        goto cleanup;
    }

    /* Using a larger vbuf to enhance performance of file i/o */
    if (setvbuf(outFile, vbuffer, _IOFBF, sizeof(vbuffer)) != 0) {
        ret = Dmai_EFAIL;
        fprintf(stderr,"Failed to setvbuf on file descriptor\n");
        goto cleanup;   
    }
    
    /* Open the codec engine */
    hEngine = Engine_open("encodedecode", NULL, NULL);
    if (hEngine == NULL) {
        ret = Dmai_EFAIL;
        printf("Failed to open codec engine decode\n");
        goto cleanup;
    }

	params.forceChromaFormat = XDM_YUV_420SP;
	params.maxHeight = VideoStd_720P_HEIGHT;
	params.maxWidth  = VideoStd_720P_WIDTH;

    /* Create the image decoder */
    hId = Idec1_create(hEngine,"jpegdec", &params, &dynParams);
    if (hId == NULL) {
        ret = Dmai_EFAIL;
        printf("Failed to create image decoder jpegdec\n");
        goto cleanup;
    }

    /* Align buffers to cache line boundary */    
    gfxAttrs.bAttrs.memParams.align = bAttrs.memParams.align = BUFSIZEALIGN;  
    gfxAttrs.colorSpace     = ColorSpace_YUV420PSEMI;
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


	




