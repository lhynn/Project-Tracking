#ifndef _MYMENUGUI_H_
#define _MYMENUGUI_H
#include "../ctrl.h"


typedef void (* MYINITFUN)(unsigned char*);


typedef struct myMenuGui_St{
	char name[20];
	void (* InitMenu)(unsigned char*);
	void (* DealMenu)(void);

	struct myMenuGui_St *FarthMenu;
	struct myMenuGui_St *ChildMenu1;
	struct myMenuGui_St *ChildMenu3;
	struct myMenuGui_St *ChildMenu2;
	struct myMenuGui_St *ChildMenu4;
}*PT_MenuGuiSt;



PT_MenuGuiSt MyCreateMenuHead(void);
void CreateSecMenu(PT_MenuGuiSt Head);




#endif 
