#include "openH264.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv){

	PT_LoaderHandle loader;
	T_OneFarmeData	oneFarme;
	int ret;
	
	loader = FileLoaderCreate("./out.264",24);
	if(loader == NULL){
		printf("FileLoaderCreate error.\n");
		return -1;
	}
	while(1){
		ret = FileLoaderGetNextFarme(loader,&oneFarme);
		if(!ret)
			break;
	}
	int i;
	for(i = 0;i < oneFarme.DataLen;i++)
	printf("%x ",oneFarme.DataAddr[i]);
	printf("\n");
	printf("HeadSize:%d,iFileSize:%d,FoundCountFlag:%d,CountFrame:%d\n",loader->HeadSize,\
	loader->iFileSize,loader->FoundCountFlag,loader->CountFrame);
	FileLoaderDestory(loader);
	return 0;
}

