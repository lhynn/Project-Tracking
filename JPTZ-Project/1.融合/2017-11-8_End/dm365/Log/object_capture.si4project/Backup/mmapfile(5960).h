#ifndef _MMAPFILE_H_
#define _MMAPFILE_H_

#include <stdio.h>

typedef struct FileMap {
	char *strFileName;   /* 文件名 *///////////路径
	FILE *tFp;              /* 文件句柄 */
	int iFileSize;           /* 文件大小 */
	unsigned char *pucFileMapMem;  /* 使用mmap函数映射文件得到的内存 */
}T_FileMap, *PT_FileMap;

int MapFile(PT_FileMap ptFileMap);
void UnMapFile(PT_FileMap ptFileMap);
void GetPixelDatasFrmBMP(unsigned char *aFileHead,unsigned char *dst);
#endif

