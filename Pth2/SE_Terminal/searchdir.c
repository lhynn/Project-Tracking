#include "searchdir.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <string.h>

LLIST *mySearchDirFun(char *str,char *fileType){

    LLIST *handler;
    T_MyFileSt tmp;
    int len = 0,count = 0;
    struct dirent **namelist;
    int n,i;

    handler = llist_create(sizeof(T_MyFileSt));
    if(handler == NULL){
        printf("llist_create() failed.\n");
        return NULL;
    }
    /* tmp.name = malloc(14);
     if(tmp.name == NULL){
         printf("malloc NameAddr error!\n");
         exit(1);
     }*/
    char *Strtok = NULL;
    n = scandir(str,&namelist,0,alphasort);
    for(i = 0;i < n;i++){
        len = strlen(namelist[i]->d_name);
        if(strcmp(&namelist[i]->d_name[len-4],fileType) == 0){
            count++;
            Strtok = strtok(namelist[i]->d_name,".");
            //tmp.name = namelist[i]->d_name;
            strncpy(tmp.name,Strtok,11);
      //      tmp.countfile   = count;
            handler->insert(handler,&tmp,LLIST_BACKWARD);
        }
        free(namelist[i]);
    }
    free(namelist);
    return handler;
}












