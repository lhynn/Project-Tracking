#include "mymenugui.h"
#include "../char_table.h"
#include "searchdir.h"
#define GREENPAN		1
static int  GREENPICFLAG      = 1;
static int  PICTURESELCETFLAG = 1;

static int  LinkCreateFlag    = 0;


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

static PT_MenuGuiSt DealMenuHead(PT_MenuGuiSt ptr){
	PT_MenuGuiSt pt_retnode = NULL;
	
	if(glUsartInfoSt.LeftRoll || glUsartInfoSt.RightRoll){
		pt_retnode = ptr->ChildMenu1;	
		glUsartInfoSt.LeftRoll  = 0;
		glUsartInfoSt.RightRoll = 0;
	}
	if(glUsartInfoSt.LeftKeyInfo)
	{
		
	}
	return pt_retnode;
}

static void InitMenuSec(unsigned char*ptr){

	osd_print_Mypic(40,0xff,20,268,ptr,MYRETURN);
	osd_print_Mypic(40,0xff,680,268,ptr,MYENTER);

	osd_print_Mypic(40,0,470,486,ptr,MYHEISEYUAN);
	if(GREENPICFLAG & 0x01)		
		osd_print_Mypic(24,GREENPAN,478,494,ptr,MYMOSHI);			//绿字
	else
		osd_print_Mypic(24,0xff,478,494,ptr,MYMOSHI);
	
	osd_print_Mypic(40,0,520,486,ptr,MYHEISEYUAN);
	if(GREENPICFLAG & 0x02)
		osd_print_Mypic(24,GREENPAN,528,494,ptr,MYTUPIAN);
	else
		osd_print_Mypic(24,0xff,528,494,ptr,MYTUPIAN);
	
	osd_print_Mypic(40,0,570,486,ptr,MYHEISEYUAN);
	if(GREENPICFLAG & 0x04)
		osd_print_Mypic(24,GREENPAN,578,494,ptr,MYSHIPIN);
	else
		osd_print_Mypic(24,0xff,578,494,ptr,MYSHIPIN);
	
	osd_print_Mypic(40,0,620,486,ptr,MYHEISEYUAN);
	if(GREENPICFLAG & 0x08)
		osd_print_Mypic(24,GREENPAN,628,494,ptr,MYCAIDAN);
	else
		osd_print_Mypic(24,0xff,628,494,ptr,MYCAIDAN);
}

static PT_MenuGuiSt DealMenuSec(PT_MenuGuiSt ptr){
	PT_MenuGuiSt pt_retnode = NULL;

	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
			GREENPICFLAG = GREENPICFLAG >> 1;
			if(GREENPICFLAG == 0){
				GREENPICFLAG = 8;
			}
		glUsartInfoSt.RightRoll = 0;
	}
	if(glUsartInfoSt.LeftRoll){
			GREENPICFLAG = GREENPICFLAG << 1;
			printf("GREENPICFLAG:%d\n",GREENPICFLAG);
			if(GREENPICFLAG > 8){
				GREENPICFLAG = 1;
			}
	       glUsartInfoSt.LeftRoll = 0;
	}

	if(glUsartInfoSt.LeftKeyInfo == 1){
		pt_retnode = ptr->FarthMenu;
		glUsartInfoSt.LeftKeyInfo = 0;
	}

	if(glUsartInfoSt.RightKeyInfo == 1){
		if(GREENPICFLAG & 0x01){}
			
		if(GREENPICFLAG & 0x02){
			pt_retnode = ptr->ChildMenu2;
		}
		if(GREENPICFLAG & 0x04){}
			
		if(GREENPICFLAG & 0x08){}
	}
	return pt_retnode;
}


static void InitMenuThree_MoShi(unsigned char*ptr){
	osd_print_Mypic(40,0xff,20,268,ptr,MYRETURN);
	osd_print_Mypic(40,0xff,680,268,ptr,MYENTER);
		
	osd_print_rect(478,490,ptr);

}

static PT_MenuGuiSt DealMenuThree_MoShi(PT_MenuGuiSt ptr){

	PT_MenuGuiSt pt_retnode = NULL;
	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
		
	}
	if(glUsartInfoSt.LeftRoll){
		;
	}

	if(glUsartInfoSt.LeftKeyInfo == 1){
		pt_retnode = ptr->FarthMenu;
		glUsartInfoSt.LeftKeyInfo = 0;
	}

	if(glUsartInfoSt.RightKeyInfo == 1){
		
	}
	return pt_retnode;
}

static void CreatTuPianShow(int num,unsigned char*ptr,LLIST *head,int Select){
	int i;
	struct llist_node_st *tmp = head->head.next;
	PT_MyFileSt			  data = NULL;
	
	for(i = 0;i < num ;i++){
		data = tmp->data;
		printf("---------------------.\n");
		printf("%s--------%s.\n",data->name,data->filepath);
		/*osd_print_rect(250+100*i+(4-num)*100,490,0,ptr);
		if(i == (Select - 1))
			osd_print_string(data->name,5,254+100*i+(4-num)*100,486,GREENPAN,ptr,MYASCIIDOT);
		else
			osd_print_string(data->name,5,254+100*i+(4-num)*100,486,0xff,ptr,MYASCIIDOT);	
			*/
		//tmp = tmp->next; 
	}
	tmp = NULL;
	data = NULL;
}	
static int gl_maxfilecnt;

static void InitMenuThree_TuPian(unsigned char*ptr){
	int maxfilecnt;
	LLIST *gl_MyBmpLList = NULL;
	
	osd_print_Mypic(40,0xff,20,268,ptr,MYRETURN);
	osd_print_Mypic(40,0xff,680,268,ptr,MYENTER);	
	//PICTURESELCETFLAG
	
	if(LinkCreateFlag == 0){
		gl_MyBmpLList = mySearchDirFun("/home/root/media/",".bmp");
		if(gl_MyBmpLList == NULL){
			printf("Search Dir error.\n");
			exit(1);
		}
		gl_MyBmpLList->getMemSum(gl_MyBmpLList,&gl_maxfilecnt);
		LinkCreateFlag = 1;	
	}
	
	printf("gl_maxfilecnt:%d.\n",gl_maxfilecnt);
	
	if(gl_maxfilecnt < 4){
		//CreatTuPianShow(gl_maxfilecnt,ptr,gl_MyBmpLList->head,PICTURESELCETFLAG);
	}else{
		
	}
	
	CreatTuPianShow(4,ptr,gl_MyBmpLList,PICTURESELCETFLAG);
	//myDestoryFileListMember(headImage);
	
}


static PT_MenuGuiSt DealMenuThree_TuPian(PT_MenuGuiSt ptr){

	PT_MenuGuiSt pt_retnode = NULL;
	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
		PICTURESELCETFLAG -= 1;
		if(PICTURESELCETFLAG < 1){
				PICTURESELCETFLAG = 1;
		
		}
		glUsartInfoSt.RightRoll = 0;
	}
	
	if(glUsartInfoSt.LeftRoll){
		PICTURESELCETFLAG += 1;
		if(gl_maxfilecnt < 4){
			if(PICTURESELCETFLAG > (gl_maxfilecnt-1)){
				 PICTURESELCETFLAG = gl_maxfilecnt-1;
			
		
			}
		}else if(PICTURESELCETFLAG > 4){
			PICTURESELCETFLAG = 4;
		
		}
		glUsartInfoSt.LeftRoll = 0;
	}

	if(glUsartInfoSt.LeftKeyInfo == 1){
		pt_retnode = ptr->FarthMenu;
		glUsartInfoSt.LeftKeyInfo = 0;
	}

	if(glUsartInfoSt.RightKeyInfo == 1){
		
	}
	return pt_retnode;
}



static void InitMenuThree_Shipin(unsigned char*ptr){
	osd_print_Mypic(40,0xff,20,268,ptr,MYRETURN);
	osd_print_Mypic(40,0xff,680,268,ptr,MYENTER);
		
	osd_print_rect(478,490,ptr);
}


static PT_MenuGuiSt DealMenuThree_Shipin(PT_MenuGuiSt ptr){

	PT_MenuGuiSt pt_retnode = NULL;
	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
		
	}
	if(glUsartInfoSt.LeftRoll){
		;
	}

	if(glUsartInfoSt.LeftKeyInfo == 1){
	
	}

	if(glUsartInfoSt.RightKeyInfo == 1){
		
	}
	return pt_retnode;
}



static void InitMenuThree_CaiDan(unsigned char*ptr){	
	osd_print_Mypic(40,0xff,20,268,ptr,MYRETURN);
	osd_print_Mypic(40,0xff,680,268,ptr,MYENTER);
		
	osd_print_rect(478,490,ptr);

}

static PT_MenuGuiSt DealMenuThree_CaiDan(PT_MenuGuiSt ptr){

	PT_MenuGuiSt pt_retnode = NULL;
	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
		
	}
	if(glUsartInfoSt.LeftRoll){
		;
	}

	if(glUsartInfoSt.LeftKeyInfo == 1){
	
	}

	if(glUsartInfoSt.RightKeyInfo == 1){
		
	}
	return pt_retnode;
}



PT_MenuGuiSt MyCreateMenuHead(void){
	PT_MenuGuiSt MenuHead = NULL;
	MenuHead = malloc(sizeof(struct myMenuGui_St));
	if(MenuHead == NULL){
		printf("malloc MenuHead error.\n");
		return NULL;
	}

	strcpy(MenuHead->name,"MenuHead");
	MenuHead->count 	 = 1;
	MenuHead->InitMenu   = InitMenuHead;
	MenuHead->DealMenu   = DealMenuHead;
	MenuHead->FarthMenu  = NULL;
	MenuHead->ChildMenu1 = NULL;
	MenuHead->ChildMenu3 = NULL;
	MenuHead->ChildMenu2 = NULL;
	MenuHead->ChildMenu4 = NULL;
	return MenuHead;
}

static void InsertChildMenuHead(PT_MenuGuiSt FarthMenu,int num,MYINITFUN initfun,MYDEALFUN dealfun){
	PT_MenuGuiSt MenuChild = NULL;
	
	char str[20];
	MenuChild = malloc(sizeof(struct myMenuGui_St));
	if(MenuChild == NULL){
			printf("malloc MenuHead error.\n");
			return NULL;
	}

	sprintf(str,"Menu%d",num);
	strcpy(MenuChild->name,str);
	MenuChild->count 	  = 1;
	MenuChild->InitMenu   = initfun;
	MenuChild->DealMenu   = dealfun;
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
	InsertChildMenuHead(Head,1,InitMenuSec,DealMenuSec);
	InsertChildMenuHead(Head->ChildMenu1,1,InitMenuThree_MoShi ,DealMenuThree_MoShi);
	InsertChildMenuHead(Head->ChildMenu1,2,InitMenuThree_TuPian,DealMenuThree_TuPian);
	InsertChildMenuHead(Head->ChildMenu1,3,InitMenuThree_Shipin,DealMenuThree_Shipin);
	InsertChildMenuHead(Head->ChildMenu1,4,InitMenuThree_CaiDan,DealMenuThree_CaiDan);
}




