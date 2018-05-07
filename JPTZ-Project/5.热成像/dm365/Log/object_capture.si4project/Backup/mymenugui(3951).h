#ifndef _MYMENUGUI_H_
#define _MYMENUGUI_H
#include "../ctrl.h"

extern int DUIBIDUCOUNT;
extern int LIANGDUCOUNT;
extern int JIXINGCOUNT;
extern int WEICAICOUNT;
extern int FANGDACOUNT;
extern int FENHUACOUNT;	
extern int TUQIANGCOUNT;
	

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
	struct myMenuGui_St *ChildMenu5;
	struct myMenuGui_St *ChildMenu6;
	struct myMenuGui_St *ChildMenu7;
};

void ShowPower(unsigned char *ptr);
PT_MenuGuiSt MyCreateMenuHead(void);
void CreateSecMenu(PT_MenuGuiSt Head);

void ShowPicString(unsigned char *ptr,char *string);
void CheckUsartData(void);

#endif 
