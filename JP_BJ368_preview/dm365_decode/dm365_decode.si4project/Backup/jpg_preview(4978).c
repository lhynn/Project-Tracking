#include <stdio.h>
#include "jpeglib.h"
#include <setjmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>

void read_jpg_file(char *path){
	FILE * infile;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	unsigned char *buffer;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	if ((infile = fopen(path, "rb")) == NULL) {
		printf("can't open %s\n", path);
		return -1;
	}
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);
	
	printf("image_width = %d\n", cinfo.image_width);
	printf("image_height = %d\n", cinfo.image_height);
	printf("num_components = %d\n", cinfo.num_components);

	
	printf("enter scale M/N:\n");  // 缩放比例M/N:
	scanf("%d/%d", &cinfo.scale_num, &cinfo.scale_denom);
	printf("scale to : %d/%d\n", cinfo.scale_num, cinfo.scale_denom);

	jpeg_start_decompress(&cinfo);

	printf("output_width = %d\n", cinfo.output_width);
	printf("output_height = %d\n", cinfo.output_height);
	printf("output_components = %d\n", cinfo.output_components);

	int row_stride;
	
	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = malloc(row_stride);

/*	while (cinfo.output_scanline < cinfo.output_height) 
	{
		(void) jpeg_read_scanlines(&cinfo, &buffer, 1);
		FBShowLine(0, cinfo.output_width, cinfo.output_scanline, buffer);
	}
	*/
	free(buffer);
	jpeg_finish_decompress(&cinfo);
 	jpeg_destroy_decompress(&cinfo);

}


	




