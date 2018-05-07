#ifndef _SEARCHDIR_H_
#define _SEARCHDIR_H_

#define FILEDIRPATH 	"/home/topeet/Image"

typedef  enum {
	SEARCHIMAGE,
 	SEARCHVIDEO
}E_SEARCHTYPE;

typedef struct MyFileList {
  char *name;
  char *FilePath;
  int count;
  struct MyFileList *prev,*next;
}T_MyFileList,*PT_MyFileList;


PT_MyFileList myCreateFileList(void);
int myInsertFileListMember(PT_MyFileList ,E_SEARCHTYPE );
int  myGetFileListMemberSum(PT_MyFileList );

void myDisplayFileListMember(PT_MyFileList );
void myDestoryFileListMember(PT_MyFileList);


#endif



