#include "mymenugui.h"
#include "../char_table.h"

#define GREENPIC	1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*********************************************************************
    osd_print_Mypic(40,0xff,450,480,ptr,MYPHOTO);
	osd_print_Mypic(40,0xff,500,480,ptr,MYLUXIANG);
	osd_print_Mypic(40,0xff,550,480,ptr,MYRETURN);
	osd_print_Mypic(40,0xff,600,480,ptr,MYBUCHANG);
	osd_print_Mypic(40,0xff,650,480,ptr,MYENTER);

	osd_print_Mypic(40,0,100,100,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,108,108,ptr,MYMOSHI);
	
	osd_print_Mypic(40,0,150,100,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,158,108,ptr,MYTUPIAN);
	
	osd_print_Mypic(40,0,200,100,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,208,108,ptr,MYSHIPIN);
	
	osd_print_Mypic(40,0,250,100,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,258,108,ptr,MYCAIDAN);

*********************************************************************/
static void InitMenuHead(unsigned char*ptr){
	
	osd_print_Mypic(40,0xff,680,268,ptr,MYPHOTO);
	osd_print_Mypic(40,0xff,20,268,ptr,MYBUCHANG);

}

static void InitMenuSec(unsigned char*ptr){

	osd_print_Mypic(40,0xff,20,268,ptr,MYRETURN);
	osd_print_Mypic(40,0xff,680,268,ptr,MYENTER);

	
	osd_print_Mypic(40,GREENPIC,470,486,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,478,494,ptr,MYMOSHI);
	
	osd_print_Mypic(40,0,520,486,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,528,494,ptr,MYTUPIAN);
	
	osd_print_Mypic(40,0,570,486,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,578,494,ptr,MYSHIPIN);
	
	osd_print_Mypic(40,0,620,486,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,628,494,ptr,MYCAIDAN);

}

static void DealMenuSec(unsigned char*ptr){

	
	osd_print_Mypic(40,GREENPIC,520,486,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,GREENPIC,528,494,ptr,MYTUPIAN);
	
}

static void InitMenuMoShi(unsigned char*ptr){
	osd_print_Mypic(40,0,100,100,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,108,108,ptr,MYMOSHI);
}

static void InitMenuTuPian(unsigned char*ptr){
	osd_print_Mypic(40,0,150,100,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,158,108,ptr,MYTUPIAN);
}

static void InitMenuShiPin(unsigned char*ptr){
	osd_print_Mypic(40,0,150,100,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,158,108,ptr,MYTUPIAN);
}

static void InitMenuCaiDan(unsigned char*ptr){	
	osd_print_Mypic(40,0,250,100,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,258,108,ptr,MYCAIDAN);
}

PT_MenuGuiSt MyCreateMenuHead(void){
	PT_MenuGuiSt MenuHead = NULL;
	MenuHead = malloc(sizeof(struct myMenuGui_St));
	if(MenuHead == NULL){
		printf("malloc MenuHead error.\n");
		return NULL;
	}

	strcpy(MenuHead->name,"MenuHead");
	MenuHead->InitMenu   = InitMenuHead;
	MenuHead->DealMenu   = NULL;
	
	MenuHead->FarthMenu  = NULL;
	MenuHead->ChildMenu1 = NULL;
	MenuHead->ChildMenu3 = NULL;
	MenuHead->ChildMenu2 = NULL;
	MenuHead->ChildMenu4 = NULL;
	return MenuHead;
}

static void InsertChildMenuHead(PT_MenuGuiSt FarthMenu,int num,MYINITFUN initfun){
	PT_MenuGuiSt MenuChild = NULL;
	
	char str[20];
	
	MenuChild = malloc(sizeof(struct myMenuGui_St));
	if(MenuChild == NULL){
			printf("malloc MenuHead error.\n");
			return NULL;
	}

	sprintf(str,"Menu%d",num);
	strcpy(MenuChild->name,str);
	MenuChild->InitMenu   = initfun;
	
	MenuChild->DealMenu   = NULL;
	MenuChild->FarthMenu  = FarthMenu;
	MenuChild->ChildMenu1 = NULL;
	MenuChild->ChildMenu2 = NULL;
	MenuChild->ChildMenu3 = NULL;
	MenuChild->ChildMenu4 = NULL;
	if(num == 1)
		FarthMenu->ChildMenu1 = MenuChild;
	if(num == 2)
		FarthMenu->ChildMenu2 = MenuChild;
	if(num == 3)
		FarthMenu->ChildMenu3 = MenuChild;
	if(num == 4)
		FarthMenu->ChildMenu4 = MenuChild;
	
}


void CreateSecMenu(PT_MenuGuiSt Head){
	InsertChildMenuHead(Head,1,InitMenuSec);
}




