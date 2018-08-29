#include "jpg_preview.h"

/* Align buffers to this cache line size (in bytes)*/
#define BUFSIZEALIGN            128

typedef struct Jpg_decode_st{
	Engine_Handle           i_hEngine;
	Idec1_Handle            i_hId;
	FILE*                   i_InFile;
	Buffer_Handle           i_hInBuf;
    Buffer_Handle           i_hOutBuf;	
}JPG_DECODE_ST;

static JPG_DECODE_ST jpg_decode_st;
Void After_Zoom_To_Display_jpg(Buffer_Handle hSrcBuf,JPG_DATA_FORMAT ZoomPtr,int offsetx,int offsety){

        int y,x;
		unsigned char *tmptr   = ZoomPtr;
		unsigned char *ptrDst  = Buffer_getUserPtr(hSrcBuf);
		Int32  		   ySize   = Buffer_getSize(hSrcBuf) * 2 / 3;
		unsigned char *cbcrPtr = ptrDst + ySize;

        ptrDst += (offsety + i_OFFSETY) * i_LINELEN + i_OFFSETX + offsetx;
        for (y = 0; y < Jpg_zoom_HID; y++) {
            memcpy(ptrDst,tmptr,Jpg_zoom_WID);
        	tmptr  += Jpg_zoom_WID;// i_AFTZOOMW;
            ptrDst += i_LINELEN;
        }
        
		cbcrPtr += (offsety + i_OFFSETY)* i_LINELEN /2  + i_OFFSETX + offsetx ;
		for (y = 0; y < Jpg_zoom_HID/2; y++) {
			memcpy(cbcrPtr,tmptr,Jpg_zoom_WID);
			tmptr   +=  Jpg_zoom_WID;// i_AFTZOOMW;
			cbcrPtr += i_LINELEN;
		} 
}


void After_Jpg_Date_To_Diplay(Buffer_Handle dis_Dstbuf)
{
    Int8   *yPtr, *cbcrPtr;        
    BufferGfx_Dimensions dim;
    BufferGfx_getDimensions(jpg_decode_st.i_hOutBuf, &dim);          
    printf("dim.width:%d  dim.height:%d\n",dim.width,dim.height);
    Buffer_print(jpg_decode_st.i_hOutBuf);
	int 		i;
	Int8   		*Dsty,*Dst_uv;  
	UInt32  	DstySize; 
	Dsty    	= Buffer_getUserPtr(dis_Dstbuf);	
	DstySize	= Buffer_getSize(dis_Dstbuf) * 2 / 3; 
	Dst_uv 	 	= Dsty + DstySize; 	
	Dsty   		+= i_OFFSETY * i_LINELEN + i_OFFSETX ;
    Dst_uv 		+= i_OFFSETY * i_LINELEN /2  + i_OFFSETX ;

    /* Write Y plane */
    yPtr = Buffer_getUserPtr(jpg_decode_st.i_hOutBuf);
    for(i = 0;i < dim.height;i++){
		memcpy(Dsty, yPtr, dim.width);      //copy yy
		Dsty += i_LINELEN;
		yPtr += dim.width;
	}
	
    /* Separate Cb from CbCr interleaved and save Cb plane */
    cbcrPtr = Buffer_getUserPtr(jpg_decode_st.i_hOutBuf) + Buffer_getSize(jpg_decode_st.i_hOutBuf) * 2/3;
    for (i = 0; i < dim.height/2; i++) {
   		memcpy(Dst_uv, cbcrPtr, dim.width);  //copy yy
		Dst_uv  += i_LINELEN;
		cbcrPtr += dim.width;;	
    }
}

Int JPG_Decode_Engine_Init(void){
	IMGDEC1_Params          params       = Idec1_Params_DEFAULT;
    IMGDEC1_DynamicParams   dynParams    = Idec1_DynamicParams_DEFAULT;
    Buffer_Attrs            bAttrs       = Buffer_Attrs_DEFAULT;
    BufferGfx_Attrs         gfxAttrs     = BufferGfx_Attrs_DEFAULT;
    
    /* Open the codec engine */
    jpg_decode_st.i_hEngine = Engine_open("encodedecode", NULL, NULL);
    if (jpg_decode_st.i_hEngine == NULL) {
        printf("Failed to open codec engine decode\n");
        return -1;
    }

	params.forceChromaFormat = XDM_YUV_420SP;
	params.maxHeight = VideoStd_720P_HEIGHT;
	params.maxWidth  = VideoStd_720P_WIDTH;

    /* Create the image decoder */
    jpg_decode_st.i_hId = Idec1_create(jpg_decode_st.i_hEngine,"jpegdec", &params, &dynParams);
    if ( jpg_decode_st.i_hId  == NULL) {
    	JPG_Decode_Engine_Destory();
        printf("Failed to create image decoder jpegdec\n");
        return -1;
    }

    /* Align buffers to cache line boundary */    
    gfxAttrs.bAttrs.memParams.align = bAttrs.memParams.align = BUFSIZEALIGN;  
    gfxAttrs.colorSpace     = ColorSpace_YUV420PSEMI;
    gfxAttrs.dim.width      = params.maxWidth;
    gfxAttrs.dim.height     = params.maxHeight;
    gfxAttrs.dim.lineLength = BufferGfx_calcLineLength(params.maxWidth,
                                                       gfxAttrs.colorSpace);
    
	/* Create an output buffer for decoded data */
	jpg_decode_st.i_hOutBuf = Buffer_create(
	Dmai_roundUp(Idec1_getOutBufSize(jpg_decode_st.i_hId), BUFSIZEALIGN), 
	BufferGfx_getBufferAttrs(&gfxAttrs));
    if (jpg_decode_st.i_hOutBuf == NULL) {   
    	JPG_Decode_Engine_Destory();
        printf("Failed to create contiguous buffers\n");
        return -1;
    }

    /* Create an input buffer for encoded data */
    jpg_decode_st.i_hInBuf = Buffer_create(Dmai_roundUp(Idec1_getInBufSize(jpg_decode_st.i_hId), BUFSIZEALIGN),&bAttrs);
    if (jpg_decode_st.i_hInBuf == NULL) {
    	JPG_Decode_Engine_Destory();
        printf("Failed to create contiguous buffers\n");
         return -1;
    }
 
	return 0;
}

void JPG_Decode_Engine_Destory(void){
	 if (jpg_decode_st.i_hId) 
        Idec1_delete(jpg_decode_st.i_hId);
    if (jpg_decode_st.i_hInBuf) 
        Buffer_delete(jpg_decode_st.i_hInBuf);
    if (jpg_decode_st.i_hOutBuf) 
        Buffer_delete(jpg_decode_st.i_hOutBuf);
    if (jpg_decode_st.i_hEngine) 
        Engine_close(jpg_decode_st.i_hEngine);
}

Int JPG_Decode_File_Open(unsigned char *filepath){
    Int    numBytes = 0,ret = 0;
    
	if( filepath == NULL){
		printf("%s:argc error.\n",__FUNCTION__);
		return -1;
	}
	
    jpg_decode_st.i_InFile = fopen(filepath, "rb");
    if (jpg_decode_st.i_InFile == NULL) {
        printf("Failed to open input file \n");
        return -1;
    }
	/* Read encoded image data */
    numBytes = fread(Buffer_getUserPtr(jpg_decode_st.i_hInBuf), 1,Idec1_getInBufSize(jpg_decode_st.i_hId), jpg_decode_st.i_InFile);			   
    Buffer_setNumBytesUsed(jpg_decode_st.i_hInBuf, numBytes);
	
    printf("Decoding image...\n");
	   
    /* Decode the image frame */
    ret = Idec1_process(jpg_decode_st.i_hId,jpg_decode_st.i_hInBuf,jpg_decode_st.i_hOutBuf);
    if (ret < 0) {
      JPG_Decode_File_Close();
	  printf("Failed to decode image buffer\n");
	  return -1;
    }
   return 0;
}

void JPG_Decode_File_Close(){
	if (jpg_decode_st.i_InFile) 
        fclose(jpg_decode_st.i_InFile);
}

static void Jpg_YUV420sp_Rest_Scale(unsigned char *dst,int dstWidth, int dstHeight)     
{
	Int8   *src, *src_uv; 	   
	BufferGfx_Dimensions dim;
	BufferGfx_getDimensions(jpg_decode_st.i_hOutBuf, &dim);			
	src    = Buffer_getUserPtr(jpg_decode_st.i_hOutBuf);
	src_uv = Buffer_getUserPtr(jpg_decode_st.i_hOutBuf) + Buffer_getSize(jpg_decode_st.i_hOutBuf) * 2/3;

	register int sw = I_JPG_WID;  //register keyword is for local var to accelorate 
    register int sh = I_JPG_HID;
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
        if((y & 1) == 0){
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
}


JPG_DATA_FORMAT  JPG_Get_Zoom_Decode_Date(Int DstWidth,Int DstHeight){
	JPG_DATA_FORMAT DstData = NULL;
	DstData = ( JPG_DATA_FORMAT )malloc(DstWidth * DstHeight * 3 / 2);
	if (DstData == NULL) {
		printf("Failed to malloc\n");
		return NULL;
	}	
    Jpg_YUV420sp_Rest_Scale(DstData,DstWidth,DstHeight);
	return DstData;
}

Void JPG_Destory_Zoom_Decode_Date(JPG_DATA_FORMAT DstData){
	free(DstData);	
}



#if 0
Int Decode_JPG_Preview(Buffer_Handle dstbuffer,unsigned char *filepath)
{
    IMGDEC1_Params          params       = Idec1_Params_DEFAULT;
    IMGDEC1_DynamicParams   dynParams    = Idec1_DynamicParams_DEFAULT;
    Buffer_Attrs            bAttrs       = Buffer_Attrs_DEFAULT;
    BufferGfx_Attrs         gfxAttrs     = BufferGfx_Attrs_DEFAULT;
    Idec1_Handle            hId          = NULL;
    Engine_Handle           hEngine      = NULL;
    Buffer_Handle           hInBuf       = NULL;
    Buffer_Handle           hOutBuf      = NULL;
    FILE                   *inFile       = NULL;
    Int                     numBytes     = 0;
    Int                     ret          = Dmai_EOK;

    inFile = fopen(filepath, "rb");
    if (inFile == NULL) {
        ret = Dmai_EFAIL;
        printf("Failed to open input file \n");
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

	After_Jpg_Date_To_Diplay(hOutBuf, dstbuffer);

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

    printf("End of application.\n");
    if (ret == Dmai_EFAIL) 
        return 1;
    else    
        return 0;
}

#endif

