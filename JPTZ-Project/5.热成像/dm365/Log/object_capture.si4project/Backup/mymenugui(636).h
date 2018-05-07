#ifndef _MYMENUGUI_H_
#define _MYMENUGUI_H
#include "../ctrl.h"

typedef struct myMenuGui_St* PT_MenuGuiSt;

typedef void (* MYINITFUN)(unsigned char*);
typedef PT_MenuGuiSt (* MYDEALFUN)(PT_MenuGuiSt );


struct myMenuGui_St{
	char name[20];
	int  count;
	void (* InitMenu)(unsigned char*);
	PT_MenuGuiSt (* DealMenu)(PT_MenuGuiSt );

	struct myMenuGui_St *FarthMenu;
	struct myMenuGui_St *ChildMenu1;
	struct myMenuGui_St *ChildMenu3;
	struct myMenuGui_St *ChildMenu2;
	struct myMenuGui_St *ChildMenu4;
};



PT_MenuGuiSt MyCreateMenuHead(void);
void CreateSecMenu(PT_MenuGuiSt Head);


#endif 
