#include "openH264.h"

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>


PT_LoaderHandle FileLoaderCreate(const char *fileName,int headsize){
	PT_LoaderHandle Handle = NULL;
	int fd,ret; 
	struct stat iStat;

	Handle = (PT_LoaderHandle)malloc(sizeof(T_LoaderHandle));
	if(Handle == NULL){
		printf("FileLoaderCreate malloc error.\n");
		return NULL;
	}	
	
	fd = open(fileName,O_RDWR);
	if(fd < 0){
		printf("open file error.\n");
		goto createrr;
	}

	ret = fstat(fd,&iStat);
	if(ret < 0){
		printf("get file stat faild\n");
		goto createrr;
	}
	Handle->mmapaddr = (unsigned char *)mmap(0,iStat.st_size,PROT_READ | PROT_WRITE, MAP_SHARED,fd,0);
	if(Handle->mmapaddr == MAP_FAILED){
		printf("file mmap error.\n");
		goto createrr;
	}	
	Handle->iFileSize   = iStat.st_size;
	Handle->HeadSize    = headsize;
	Handle->FoundCountFlag = headsize + 1;
	Handle->CountFrame     = 0;
	close(fd);
	return Handle;
createrr:
	free(Handle);	
	if(!ret)
		close(fd);
	return NULL;
}

int  FileLoaderGetNextFarme(PT_LoaderHandle handle,PT_OneFarmeData Dst){
	int tmp = handle->FoundCountFlag;
	while(handle->FoundCountFlag != handle->iFileSize){
		if(handle->mmapaddr[handle->FoundCountFlag] == 0x00 &&\
		   handle->mmapaddr[handle->FoundCountFlag + 1] == 0x00 && \
		   handle->mmapaddr[handle->FoundCountFlag + 2] == 0x00 && \
		   handle->mmapaddr[handle->FoundCountFlag + 3] == 0x01){
			handle->CountFrame++;
			break;
		}
		handle->FoundCountFlag++;
	}
	
	Dst->DataLen  = handle->FoundCountFlag - tmp;
	Dst->DataAddr = handle->mmapaddr + tmp;
	if(handle->FoundCountFlag != handle->iFileSize)
		handle->FoundCountFlag +=  4;
	else return 0;
	return 1;
}

void FileLoaderDestory(PT_LoaderHandle handle){
	munmap(handle->mmapaddr,handle->iFileSize);
	free(handle);
}	

