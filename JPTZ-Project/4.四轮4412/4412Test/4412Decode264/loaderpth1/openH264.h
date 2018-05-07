#ifndef _OPENH264_H_
#define _OPENH264_H_

typedef struct LoaderObject{
	int HeadSize;			//file head size
	int iFileSize;			//file Size
	int FoundCountFlag;		//now offset 
	int CountFrame;			//how many frame
	unsigned char *mmapaddr;	//file mmap addr
}T_LoaderHandle,*PT_LoaderHandle;

typedef struct OneFarmeData{
	int DataLen;			//frame len
	unsigned char *DataAddr;	//frame addr
}T_OneFarmeData,*PT_OneFarmeData;

PT_LoaderHandle FileLoaderCreate(const char *fileName,int FileHeadSize);

/*如果到达文件尾 返回0  读到下一帧返回1*/
int  		FileLoaderGetNextFarme(PT_LoaderHandle handle,PT_OneFarmeData Dst);

void 		FileLoaderDestory(PT_LoaderHandle handle);
#endif
