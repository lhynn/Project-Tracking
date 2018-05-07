#include "SearchDir.h"

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#define IMAGEPOSTFIX 	".bmp"
#define VOIDPOSTFIX	".264"


PT_MyFileList myCreateFileList(void){
	PT_MyFileList pt_header;

	pt_header = malloc(sizeof(T_MyFileList));
	if(pt_header == NULL){
		perror("CreateFileList error.\n");
		return NULL;
	}
	pt_header->name = NULL;
	pt_header->count = 0;
	pt_header->FilePath = NULL;
	pt_header->prev = pt_header->next = pt_header;
	return pt_header;
}

int myInsertFileListMember(PT_MyFileList ptr,E_SEARCHTYPE type){
	
	DIR *dirptr = NULL;
	struct dirent *entry;
	size_t len;
	
	int num = 0;
	char path[100];
	PT_MyFileList tmp = NULL;
	PT_MyFileList me  = ptr; 	
	
	if((dirptr = opendir(FILEDIRPATH)) == NULL){
		printf("opendir error.\n");
		return -1;
	}
	strcpy(path,FILEDIRPATH);
	strcat(path,"/");
	do{
		entry =  readdir(dirptr);
		if(entry != NULL){
			if(SEARCHIMAGE == type){		//.bmp
				if((len = strlen(entry->d_name)) < 4)
					continue;
				else {
					if(strcmp(IMAGEPOSTFIX,&(entry->d_name[len-4])) == 0){
						tmp = malloc(sizeof(T_MyFileList));
						if(tmp == NULL){
							perror("myInsertFileListMember error.\n");
							return -3;
						}
						tmp->name = entry->d_name;
						tmp->FilePath = malloc(100);
						if(tmp->FilePath == NULL){
							perror("malloc FilePath error.\n");
							return -4;
						}
						memcpy(tmp->FilePath,path,100);
						strncat(tmp->FilePath,entry->d_name,len);
						num++;
						tmp->count = num;						
						tmp->prev = me;
						tmp->next = me->next;
						tmp->prev->next = tmp;
						tmp->next->prev = tmp;
					}
				}

			}
			else if(SEARCHVIDEO == type){	
				if((len = strlen(entry->d_name)) < 4)
					continue;
				else {
					if(strcmp(VOIDPOSTFIX,&(entry->d_name[len-4])) == 0){
						 tmp = malloc(sizeof(T_MyFileList));
                                                if(tmp == NULL){
                                                        perror("myInsertFileListMember error.\n");
                                                        return -3;
                                                }
                                                tmp->name = entry->d_name;
                                                tmp->FilePath = malloc(100);
                                                if(tmp->FilePath == NULL){
                                                        perror("malloc FilePath error.\n");
                                                        return -4;
                                                }
                                                memcpy(tmp->FilePath,path,100);
                                                strncat(tmp->FilePath,entry->d_name,len);
						num++;
						tmp->count = num;						 
                                                tmp->prev = me;
                                                tmp->next = me->next;
                                                tmp->prev->next = tmp;
                                                tmp->next->prev = tmp;
					}
				}
			}else {
				printf("SEARCH FILE ERROR.\n");
				return ;
			}
		}
	}while(entry);
	close(dirptr);
	return ;
}

int  myGetFileListMemberSum(PT_MyFileList ptr){
 	PT_MyFileList me = ptr->next;
        int num = 0;
	while(me->next != ptr){
		num++;
                me = me->next;
        }
	num++;
	return num;
}

void myDisplayFileListMember(PT_MyFileList ptr){

	PT_MyFileList me = ptr->next;
	while(me->next != ptr){
		printf("name :%s,namepath:%s ,count:%d\n",me->name,me->FilePath,me->count);
		me = me->next;
	}
	printf("name :%s,namepath:%s ,count:%d\n",me->name,me->FilePath,me->count);
		
	return;
}

void myDestoryFileListMember(PT_MyFileList ptr){
	PT_MyFileList cur,next;
	for(cur = ptr->next; cur != ptr; cur = next)
	{	
		next = cur->next;
		free(cur->FilePath);	
		free(cur);
	}
	free(ptr);
}

