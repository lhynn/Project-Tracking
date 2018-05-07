#include "encoderVideo.h"
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

static unsigned int create_default_filename(char* filestr)
{
	DIR *dir;
	struct dirent *ptr;

	unsigned int file_max_count = 0;
	unsigned int current_file_count = 0;
	char filename[64];
	char strbuf[10];

	dir = opendir("/video/");
	while( (ptr = readdir(dir)) != NULL)
	{
		memset(filename, 0, 64);
		memcpy(filename, ptr->d_name, 64);

		memset(strbuf, 0, 10);
		memcpy(strbuf, filename, 2);

		if( strcmp("V_", strbuf) == 0) //test first 8 byte of filename
		{
			current_file_count = (filename[2]-'0')*10000 + (filename[3]-'0')*1000 + (filename[4]-'0')*100 + (filename[5]-'0')*10+(filename[6]-'0');

			if(current_file_count > file_max_count)
			{
				file_max_count = current_file_count;
			}
		}
	}
	closedir(dir);
	file_max_count += 1;
	sprintf(filename, "/video/V_%05d.264", file_max_count);
	memcpy(filestr, filename, strlen(filename));
}

unsigned int create_filename(char* filestr, const char* ext)
{
	DIR *dir;
	struct dirent *ptr;

	unsigned int file_max_count = 0;
	unsigned int current_file_count = 0;
	char filename[64];
	char strbuf[10];
	
	dir = opendir("/Image/");
	while( (ptr = readdir(dir)) != NULL)
	{
		memset(filename, 0, 64);
		memcpy(filename, ptr->d_name, 64); 

		memset(strbuf, 0, 10);
		memcpy(strbuf, filename, 2);

		if(strcmp("bmp", ext) == 0)  //a bmp file
		{
			if( strcmp("B_", strbuf) == 0) //test first 8 byte of filename
			{
				current_file_count = (filename[2]-'0')*10000 + (filename[3]-'0')*1000 + (filename[4]-'0')*100 + (filename[5]-'0')*10+(filename[6]-'0');

				if(current_file_count > file_max_count)
				{
					file_max_count = current_file_count;
				}
			}
		}
	}
	closedir(dir);
	file_max_count += 1;
	if(strcmp("bmp", ext) == 0)  //a jpg file
	{
		sprintf(filename, "/Image/B_%05d.%s", file_max_count, ext);
	}
	memcpy(filestr, filename, strlen(filename));
	return 0;
}

int RGB2BMP(unsigned char *rgb_buffer,int nWidth,int nHeight,FILE*fp1)
{
	BmpHead m_BMPHeader;
	char bfType[2]={'B','M'};
	m_BMPHeader.imageSize=4*nWidth*nHeight+54;
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
	m_BMPInfoHeader.bitColor=32;
	m_BMPInfoHeader.zipFormat=0;
	m_BMPInfoHeader.realSize=4*nWidth*nHeight;
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
	fwrite(rgb_buffer,4*nWidth*nHeight,1,fp1);
	return 0;
}

encoderVideo :: encoderVideo(){
	mine_fimc = new MyFimcConvert();
	mine_fimc->open();
	count = 0;
}

encoderVideo :: ~encoderVideo(){
	delete mine_fimc;
}

void encoderVideo ::CreateVideoFile(){
	ret = InitMyMfc(720,576,30);
	if(ret < 0){
		printf("init mfc failed !!\n");
	}
	headerSize = MyMfcGetHeader(&header);
	memset(outputfile, 0, 64);
	create_default_filename(outputfile);
	outf = fopen(outputfile, "w");
	fwrite(header,headerSize,1,outf);
}
void encoderVideo ::SaveImageFile(unsigned char *Buffer){
	memset(bmpfilestr, 0, 64);
	create_filename(bmpfilestr, "bmp");	
	B_fd = fopen(bmpfilestr, "w+");
	if(B_fd < 0)
		printf("SaveImageFile error!\n");
	RGB2BMP(Buffer,720,576,B_fd);	
	fclose(B_fd);
}

void encoderVideo ::WriteOneFrame(unsigned char *Buffer){
	if(count == 2){
		memcpy(mine_fimc->inbuf_vir,Buffer,720*576*4);
		mine_fimc->Convert();
		ouput_buf_size = MyMfcEncode(mine_fimc->outbuf_vir,&ouput_buf);
		printf("-------------ouput_buf_size:%d\n",ouput_buf_size);
		fwrite(ouput_buf,ouput_buf_size,1,outf);count = 0;
	}
	count++;
}

void encoderVideo ::WriteFileOver(){
	ExitMyMfc();
	fclose(outf);
	sync();
}
