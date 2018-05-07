#ifndef _SEARCHDIR_H_
#define _SEARCHDIR_H_
#include "llist.h"


typedef struct MyFile_St{
	char name[20];
	char filepath[20];
	int  countfile;
}T_MyFileSt,*PT_MyFileSt;


LLIST *mySearchDirFun(char *str,char *fileType);

#endif
