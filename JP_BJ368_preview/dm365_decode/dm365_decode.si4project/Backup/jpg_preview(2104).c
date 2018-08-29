#include <stdio.h>
#include "jpeglib.h"
#include <setjmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>

/*Read_Jpg_File:
 *		[in] 	filepath   					 ： 输入的源文件
 *		[out] 	Pic_Preview_Buffer           ： 目标的数据
 */
int  Read_Jpg_File(char *filepath,unsigned char *Pic_Preview_Buffer){
	FILE * infile;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	if(filepath == NULL || Pic_Preview_Buffer != NULL){
		printf("%s:argc error.\n",__FUNCTION__);
		return -1;
	}
		
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	if ((infile = fopen(filepath, "rb")) == NULL) {
		printf("can't open %s\n", filepath);
		return -1;
	}

	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);
	cinfo.out_color_space = JCS_YCbCr;
	/* 源信息 */
	printf("image_width = %d\n", cinfo.image_width);
	printf("image_height = %d\n", cinfo.image_height);
	printf("num_components = %d\n", cinfo.num_components);
	printf("jpeg_color_space  = %d\n",cinfo.out_color_space);
#if 0	
	//printf("enter scale M/N:\n");  // 缩放比例M/N:
	//scanf("%d/%d", &cinfo.scale_num, &cinfo.scale_denom);
	//printf("scale to : %d/%d\n", cinfo.scale_num, cinfo.scale_denom);
#endif
	jpeg_start_decompress(&cinfo);

	printf("output_width = %d\n", cinfo.output_width);
	printf("output_height = %d\n", cinfo.output_height);
	printf("output_components = %d\n", cinfo.output_components);
	printf("out_color_space  = %d\n",cinfo.out_color_space);
	
	
	Pic_Preview_Buffer =(unsigned char *)malloc(cinfo.output_width*cinfo.output_components*cinfo.output_height);
	if(Pic_Preview_Buffer == NULL){
		printf("%s:Pic_Preview_Buffer malloc error\n",__FUNCTION__);
		return -1;	
	}
	int row_stride;
	unsigned char *buffer = NULL;
	
	// 一行的数据长度
	row_stride = cinfo.output_width * cinfo.output_components;
	buffer =(unsigned char *)malloc(row_stride);
	if(buffer == NULL){
		printf("%s:buffer malloc error\n",__FUNCTION__);
		return -1;	
	}
	//jpeg_read_raw_data

	while (cinfo.output_scanline < cinfo.output_height) 
	{
		(void) jpeg_read_scanlines(&cinfo, &buffer, 1);
		memcpy(Pic_Preview_Buffer,buffer,cinfo.output_width*cinfo.output_components);
	}
	
	free(Pic_Preview_Buffer);
	jpeg_finish_decompress(&cinfo);
 	jpeg_destroy_decompress(&cinfo);
	return 0;
}


	




