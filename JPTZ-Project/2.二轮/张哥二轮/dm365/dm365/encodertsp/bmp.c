#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/videodev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <time.h>


struct file_header
{
  unsigned short bfType; // Picture tpye, must set to 19778
  int bfSize; // The file size in bytes
  int bfRev; // Reserved 
  int bfOffBits; // the offset from the beginning of the file to the bitmap data.
}__attribute__ ((packed)); 

struct info_header
{
  int biSize; // info_header's size in bytes
  int biWidth; // width in pixels
  int biHeight;//height in pixels
  short biPlanes; //the number of planes of the target device
  short biBitCount; //the number of bits per pixel
  int biCompression;//the type of compression
  int biSizeImage; //
  int biXPelsPerMeter;//usually set to zero
  int biYPelsPerMeter;//usually set to zero
  int biClrUsed;//the number of colors used in the bitmap
  int biClrImportant;
}__attribute__ ((packed)); 


#define BMP_WIDTH  736
#define BMP_HEIGHT 576

struct bmp_file
{
  struct file_header header;
  struct info_header info;
  unsigned char bits[BMP_WIDTH*BMP_HEIGHT*3];
}__attribute__ ((packed));



static void cvt_420p_to_rgb(int width, int height, const unsigned char *src, unsigned char *dst)
{
	int r, g, b;
	int yy, uu, vv;
	int xoff, yoff;
	int numpix = BMP_WIDTH*BMP_HEIGHT;
	unsigned char *pout = dst + width*height*3;

	unsigned char* pyy  = src;
	unsigned char *puv = src + numpix;

  	for(yoff=0; yoff<height; yoff++)
	{
		pout = dst + width*(height-yoff-1)*3;  //point to the bmp line head

		for(xoff=0; xoff<width; xoff++)
		{	
			yy = *(pyy + yoff*BMP_WIDTH + xoff);
			uu = *(puv + (yoff/2)*BMP_WIDTH + (xoff/2)*2 );
			vv = *(puv + (yoff/2)*BMP_WIDTH + (xoff/2)*2 + 1) ;

			uu -= 128;
			vv -= 128;

			r = yy + vv + ((vv*103)>>8);
			g = yy - ((uu*88)>>8) - ((vv*183)>>8);
			b = yy + uu + ((uu*198)>>8);

			if(r>255) { r = 255; }
			if(g>255) { g = 255; }
			if(b>255) { b = 255; }
			if(r<0) { r = 0;}
			if(g<0) { g = 0;}
			if(b<0) { b = 0;}

			*pout = (unsigned char)b;
			pout++;
			*pout = (unsigned char)g;
			pout++;
			*pout = (unsigned char)r;
			pout++;
		}
	}
}

int convert(unsigned char *imageptr, char* filename)
{
	struct bmp_file myfile;
	int fd;

	printf("sizeof short is %d\n", sizeof(short));

	myfile.header.bfType = 19778;
	myfile.header.bfSize = sizeof(struct bmp_file);
	myfile.header.bfRev = 0;
	myfile.header.bfOffBits = 54;

	myfile.info.biSize = 0x28;
	myfile.info.biWidth = BMP_WIDTH;  //640;
	myfile.info.biHeight = BMP_HEIGHT;  //480;
	myfile.info.biPlanes = 1;
	myfile.info.biBitCount = 24;
	myfile.info.biCompression = 0;
	myfile.info.biSizeImage = 0;
	myfile.info.biClrUsed = 256*256*256;
	myfile.info.biClrImportant = 0;
	myfile.info.biXPelsPerMeter = 2048;
	myfile.info.biYPelsPerMeter = 2048;

	fd = fopen(filename, "wb");
	if(fd < 0)
	{
		printf("open file error!\n");
		return 0;
	}
	   
	cvt_420p_to_rgb(BMP_WIDTH, BMP_HEIGHT, imageptr, myfile.bits);
	fwrite(&myfile, 1, sizeof(struct bmp_file), fd); //write bmp head
	fclose(fd);  
	return 0;
}

