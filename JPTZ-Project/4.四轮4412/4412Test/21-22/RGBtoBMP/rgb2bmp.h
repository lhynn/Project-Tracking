#ifndef _RGB2BMP_H__
#define _RGB2BMP_H__


#include <stdio.h>

typedef struct
{
	long imageSize;
	long blank;
	long startPosition;
}BmpHead;

typedef struct
{
	long    Length;
	long    width;
	long    height;
	unsigned short    colorPlane;
	unsigned short    bitColor;
	long    zipFormat;
	long    realSize;
	long    xPels;
	long    yPels;
	long    colorUse;
	long    colorImportant;
}InfoHead;

#endif
