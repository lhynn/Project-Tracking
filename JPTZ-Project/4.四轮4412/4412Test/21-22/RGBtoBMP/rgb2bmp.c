#include "rgb2bmp.h"
/***************************************************
	RGB加上头部信息转换成BMP
	参数说明:
	rgb_buffer	:数据文件中的信息
	nWidth		:图像宽度的像素数
	nHeight		:图像高度的像素数
	fp1		:所存放的文件

****************************************************/
int RGB2BMP(unsigned char *rgb_buffer,int nWidth,int nHeight,FILE*fp1)
{
	BmpHead m_BMPHeader;
	char bfType[2]={'B','M'};
	m_BMPHeader.imageSize=3*nWidth*nHeight+54;
	m_BMPHeader.blank=0;
	m_BMPHeader.startPosition=54;

	fwrite(bfType,1,sizeof(bfType),fp1);
	fwrite(&m_BMPHeader.imageSize,1,sizeof(m_BMPHeader.imageSize),fp1);
	fwrite(&m_BMPHeader.blank,1,sizeof(m_BMPHeader.blank),fp1);
	fwrite(&m_BMPHeader.startPosition,1,sizeof(m_BMPHeader.startPosition),fp1);

	InfoHead  m_BMPInfoHeader;
	m_BMPInfoHeader.Length=40;
	m_BMPInfoHeader.width=nWidth;
	m_BMPInfoHeader.height=nHeight;
	m_BMPInfoHeader.colorPlane=1;
	m_BMPInfoHeader.bitColor=24;
	m_BMPInfoHeader.zipFormat=0;
	m_BMPInfoHeader.realSize=3*nWidth*nHeight;
	m_BMPInfoHeader.xPels=0;
	m_BMPInfoHeader.yPels=0;
	m_BMPInfoHeader.colorUse=0;
	m_BMPInfoHeader.colorImportant=0;

	fwrite(&m_BMPInfoHeader.Length,1,sizeof(m_BMPInfoHeader.Length),fp1);
	fwrite(&m_BMPInfoHeader.width,1,sizeof(m_BMPInfoHeader.width),fp1);
	fwrite(&m_BMPInfoHeader.height,1,sizeof(m_BMPInfoHeader.height),fp1);
	fwrite(&m_BMPInfoHeader.colorPlane,1,sizeof(m_BMPInfoHeader.colorPlane),fp1);
	fwrite(&m_BMPInfoHeader.bitColor,1,sizeof(m_BMPInfoHeader.bitColor),fp1);
	fwrite(&m_BMPInfoHeader.zipFormat,1,sizeof(m_BMPInfoHeader.zipFormat),fp1);
	fwrite(&m_BMPInfoHeader.realSize,1,sizeof(m_BMPInfoHeader.realSize),fp1);
	fwrite(&m_BMPInfoHeader.xPels,1,sizeof(m_BMPInfoHeader.xPels),fp1);
	fwrite(&m_BMPInfoHeader.yPels,1,sizeof(m_BMPInfoHeader.yPels),fp1);
	fwrite(&m_BMPInfoHeader.colorUse,1,sizeof(m_BMPInfoHeader.colorUse),fp1);
	fwrite(&m_BMPInfoHeader.colorImportant,1,sizeof(m_BMPInfoHeader.colorImportant),fp1);
	fwrite(rgb_buffer,3*nWidth*nHeight,1,fp1);
	return 0;
}





