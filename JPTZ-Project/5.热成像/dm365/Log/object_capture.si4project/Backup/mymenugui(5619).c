#include "mymenugui.h"
#include "../char_table.h"
#include "searchdir.h"
#include "decode.h"
#define 	GREENPAN		1
#define 	YELLOWPAN		2

static int  LinkCreateFlag    = 0;

static int  GREENPICFLAG      = 1;
static int  PICTURESELCETFLAG = 1;
static int 	VIDEOSELCETFLAG   = 1;
static int 	CAIDANSELCETFLAG  = 1;

static int  FENHUAFLAG        = 1;
static int  FENHUACOUNT       = 1;

static int  JIXINGFLAG        = 1;
static int  JIXINGCOUNT       = 1;

static int  TUQIANGFLAG       = 0;
static int  TUQIANGCOUNT      = 0;

static int  FANGDAFLAG		  = 0;
static int  FANGDACOUNT       = 0;


static int  DUIBIDUCOUNT	  = 120;
static int  LIANGDUCOUNT	  = 120;


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <pthread.h>



#define BASE_VIDIOC_PRIVATE	192		

#define VPFE_CMD_Capture_led_play  _IOR('V', BASE_VIDIOC_PRIVATE + 3, void *)
#define VPFE_CMD_Capture_led_stop  _IOR('V', BASE_VIDIOC_PRIVATE + 4, void *)
#define VPFE_CMD_OLED_BRIGHT_UP    _IOR('V', BASE_VIDIOC_PRIVATE + 7, void *)
#define VPFE_CMD_OLED_BRIGHT_DOWN  _IOR('V', BASE_VIDIOC_PRIVATE + 8, void *)
#define VPFE_CMD_OLED_CONTRAST     _IOR('V', BASE_VIDIOC_PRIVATE + 9, void *)


 
int osd_print_24(unsigned char* str, int len, int start_x, int start_y,int clock,unsigned char* bufptr)
{    
	int offset;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

    int i, j, k;
    int x, y;

    unsigned char ch;
    unsigned short line_pix;
    int me1;
    for (i=0; i<len; i++)   //draw a character
    {
        ch = (unsigned char)*(str+i) - 32;
		
        for (j = 0; j < 24; j++)
        {
              //get pixels in a line
		 for(me1 = 0;me1 < 2;me1++){
			 line_pix = CHAR_LUT_24[ch][j*2+me1];
            for (k = 0; k < 8; k++)   //draw a line
            {
                x = start_x + i*12 + k + me1*8;
                y = start_y + j;
                offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;
                if((line_pix>>(7-k)) & 0x01)   //draw a pixel
                {
			     if(clock == 1){
					*(bufptr + offset) = 50; 
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 40;
					*(bufptr + uvstart + vvoffset) = 10;//128;//200; 	//set VV 
           	
				}else if(clock == 2){
					*(bufptr + offset) = 0xff;     //Y channel
			   	 	uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
			    	vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 0;
				} else
					*(bufptr + offset) = clock; 
                }
            }
		 }
       }
    }
    return 1;
}
static char SendData[3] = {0x3e,0x00,0xff};
void ProcessUsartSend(){
	int ret;
	do{
	ret = write(fd_com, SendData, 3);
		if(ret < 0){	
			printf("UsartSend Error.\n");
			return;
		}
	}while(ret != 3);
}

void CheckUsartData(){
 	JIXINGCOUNT  = glUsartInfoSt.Polarity;
 	TUQIANGCOUNT = glUsartInfoSt.TuQiang;
 	FANGDACOUNT  = glUsartInfoSt.BlowUp;
}

void ShowPicString(unsigned char *ptr,char *string){	
	osd_print_Mypic(40,0,20,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,28,276,ptr,MYRETURN);
	
	osd_print_Mypic(40,0,680,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,688,276,ptr,XUNHUAN_PIC);

	osd_print_Mypic(24,0,214,532,ptr,YUAN_24);
	osd_print_Mypic(16,0xff,218,536,ptr,SHANGYIGE_PIC);

	osd_print_24(string, 11,306,532,0xff,ptr);

	osd_print_Mypic(24,0,506,532,ptr,YUAN_24);
	osd_print_Mypic(16,0xff,510,536,ptr,XIAYIGE_PIC);
}

void ShowVideoString(unsigned char *ptr,char *string){	
	osd_print_Mypic(40,0,20,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,28,276,ptr,MYRETURN);
	
	osd_print_Mypic(40,0,680,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,688,276,ptr,BOFANG_PIC);
	if(ProState == DECODEPROSTOP){
	osd_print_Mypic(40,0,680,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,688,276,ptr,ZANTING_PIC);
	}

	osd_print_Mypic(24,0,214,532,ptr,YUAN_24);
	osd_print_Mypic(16,0xff,218,536,ptr,SHANGYIGE_PIC);

	osd_print_24(string,11,306,532,0xff,ptr);

	osd_print_Mypic(24,0,506,532,ptr,YUAN_24);
	osd_print_Mypic(16,0xff,510,536,ptr,XIAYIGE_PIC);

}

void ShowPower(unsigned char *ptr){
	
	int batval = glUsartInfoSt.powerInfo;
	char batstr[8];
	char fangda[2];
	sprintf(batstr, "%d%\r\n", batval);
	osd_print_string(batstr, 4, 624, 24,0xff,ptr,MYASCIIDOT);  //printf battery value width = 4*6=24 FENHUA
	osd_print_battery(658, 26, batval, ptr,BATTICON);
	
	if(FENHUACOUNT == 2)
		osd_print_cross(344, 264, ptr,SDCARDFULL,CROSS);

	sprintf(fangda,"%dX",FANGDACOUNT+1);
	osd_print_string(fangda,2,60,25,0xff,ptr,MYASCIIDOT);
	if(JIXINGCOUNT)
		osd_print_Chinese(100,25,0xff,ptr,HEIRE,2,4);
	else
		osd_print_Chinese(100,25,0xff,ptr,BAIRE,2,4);

	osd_print_24("T",1,699,75,0xff,ptr);
	osd_print_jindutiaoRect(3,90, 704,120,0xff,ptr);
	osd_print_24("W",1,699,233,0xff,ptr);
	osd_print_Mypic(24,0xff,698,104,ptr,SANJIAO);	
	osd_print_Mypic(24,0xff,698,205,ptr,DAOSANJIAO);
	//CheckUsartData();
			
}
void ShowRECTime(unsigned char* ptr){
	char timeShow[10];	
	sprintf(timeShow,"%0.2d:%0.2d",gl_cam_time/60,gl_cam_time%60);
	
	if(gl_system_status == STATUS_REC){
		osd_print_Red_Point(350,40,ptr);
		osd_print_string(timeShow,5,365,38,0xff,ptr,MYASCIIDOT);
	}
}

static void InitMenuHead(unsigned char*ptr){
	ShowRECTime(ptr);
	
	osd_print_Mypic(40,0,678,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,688,276,ptr,MYPHOTO);
		
	osd_print_Mypic(40,0,20,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,28,276,ptr,MYBUCHANG);
	
	if(gl_picture_osd > 0){
		if(!gl_sdcard_full)
			osd_print_photo(350, 50, ptr,PHOTO);
		else
			osd_print_string("SD FULL!",8,300,50,0xff,ptr,MYASCIIDOT);
		gl_picture_osd--;
	}	
	if(gl_video_osd > 0){
		osd_print_Mypic(40,0,678,268,ptr,MYHEISEYUAN);
		osd_print_Mypic(24,0xff,688,276,ptr,MYLUXIANG);
		gl_video_osd--;
	}
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
		SendData[1] = 0x04;
		ProcessUsartSend();
		glUsartInfoSt.LeftKeyInfo = 0;
	}
	if(glUsartInfoSt.RightKeyInfo == 1)
	{
		if(!gl_sdcard_full){
		printf("----------picture get--------.\n");
		fflush(NULL);
		gl_get_picture = 1;
		printf("\r\nPHOTO command\r\n");
		}
		gl_picture_osd = 30;
		glUsartInfoSt.RightKeyInfo = 0;
	}
	if(glUsartInfoSt.RightKeyInfo == 2)
	{
		if(!gl_sdcard_full){
			if(gl_system_status == STATUS_STOP)
			{
			video_command = CMD_REC;
			printf("\r\nSTART REC command\r\n");	
			if(gl_capture_fd)
			ioctl(gl_capture_fd, VPFE_CMD_Capture_led_play);
				gl_video_osd = 40;
			}else if(gl_system_status == STATUS_REC)
			{			
				video_command = CMD_STOP;							
				printf("\r\nSTOP REC command\r\n");
				if(gl_capture_fd)
					ioctl(gl_capture_fd, VPFE_CMD_Capture_led_stop);
				gl_video_osd = 40;
			}
		}else gl_picture_osd = 30;
		glUsartInfoSt.RightKeyInfo = 0;
	}

	return pt_retnode;
}

static void InitMenuSec(unsigned char*ptr){
	ShowRECTime(ptr);

	osd_print_Mypic(40,0,20,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,28,276,ptr,MYRETURN);
	
	osd_print_Mypic(40,0,678,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,688,276,ptr,MYENTER);
	
	if(GREENPICFLAG & 0x01)		
		osd_print_Mypic(40,GREENPAN,260,500,ptr,MYHEISEYUAN);			
	else
		osd_print_Mypic(40,0,260,500,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,268,508,ptr,MYMOSHI);
	osd_print_Chinese(262, 548,YELLOWPAN,ptr,MOSHI,2,4);


	if(GREENPICFLAG & 0x02)
		osd_print_Mypic(40,GREENPAN,320,500,ptr,MYHEISEYUAN);
	else
		osd_print_Mypic(40,0,320,500,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,328,508,ptr,MYTUPIAN);
	osd_print_Chinese(322, 548,YELLOWPAN,ptr,TUPIAN,2,4);
	
	if(GREENPICFLAG & 0x04)
		osd_print_Mypic(40,GREENPAN,380,500,ptr,MYHEISEYUAN);
	else
		osd_print_Mypic(40,0,380,500,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,390,508,ptr,MYSHIPIN);
	osd_print_Chinese(382, 548,YELLOWPAN,ptr,SHIPIN,2,4);
	
	if(GREENPICFLAG & 0x08)
		osd_print_Mypic(40,GREENPAN,440,500,ptr,MYHEISEYUAN);
	else
		osd_print_Mypic(40,0,440,500,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,448,508,ptr,MYCAIDAN);
	osd_print_Chinese(442, 548,YELLOWPAN,ptr,CAIDAN,2,4);
}

static PT_MenuGuiSt DealMenuSec(PT_MenuGuiSt ptr){
	PT_MenuGuiSt pt_retnode = NULL;

	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
		GREENPICFLAG = GREENPICFLAG << 1;
		printf("GREENPICFLAG:%d\n",GREENPICFLAG);
		if(GREENPICFLAG > 8){
			GREENPICFLAG = 1;
		}
		glUsartInfoSt.RightRoll = 0;
	}
	if(glUsartInfoSt.LeftRoll){
		GREENPICFLAG = GREENPICFLAG >> 1;
		if(GREENPICFLAG == 0){
			GREENPICFLAG = 8;
		}
	    glUsartInfoSt.LeftRoll = 0;
	}

	if(glUsartInfoSt.LeftKeyInfo == 1){
		pt_retnode = ptr->FarthMenu;
		glUsartInfoSt.LeftKeyInfo = 0;
	}

	if(glUsartInfoSt.RightKeyInfo == 1){
		//if(GREENPICFLAG & 0x01)
			//pt_retnode = ptr->ChildMenu1;
		if(GREENPICFLAG & 0x02){
			pt_retnode = ptr->ChildMenu2;
		}
		if(GREENPICFLAG & 0x04){
			pt_retnode = ptr->ChildMenu3;
		}
		if(GREENPICFLAG & 0x08){
			pt_retnode = ptr->ChildMenu4;
		}
		glUsartInfoSt.RightKeyInfo = 0;
	}
	return pt_retnode;
}

static void InitMenuThree_MoShi(unsigned char*ptr){
	ShowRECTime(ptr);
	
	osd_print_Mypic(40,0,20,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,28,276,ptr,MYRETURN);
	
	osd_print_Mypic(40,0,678,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,688,276,ptr,MYENTER);
	
}

static PT_MenuGuiSt DealMenuThree_MoShi(PT_MenuGuiSt ptr){

	PT_MenuGuiSt pt_retnode = NULL;
	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
		
	}
	if(glUsartInfoSt.LeftRoll){
		
	}

	if(glUsartInfoSt.LeftKeyInfo == 1){
		pt_retnode = ptr->FarthMenu;
		glUsartInfoSt.LeftKeyInfo = 0;
	}

	if(glUsartInfoSt.RightKeyInfo == 1){
		
	}
	return pt_retnode;
}

static int 	gl_num = 0;
static void CreatTuPianShow(int num,unsigned char*ptr,LLIST *head,int offset,int type){
	int i = 0;
	PT_MyFileSt			  data = NULL;
	struct llist_node_st *tmp = head->head.next;
	
	//struct llist_node_st *test = gl_MyBmpLList->head.next;
	for(i = 0;i < offset && tmp != &head->head;i++){
			tmp = tmp->next;
	}
	for(i = 0;i < num ;i++){
		data = tmp->data;
	//	printf("data->name:%s.\n",data->name);
		if(i == (type-1)){
				osd_print_rect(200+80*i+(4-num)*80,530,GREENPAN,ptr);
				ShowImageNode = tmp;
			}
		else
			 osd_print_rect(200+80*i+(4-num)*80,530,0,ptr);
		osd_print_string(data->name,7,202+80*i+(4-num)*80,533,0xff,ptr,MYASCIIDOT);	
		
		tmp = tmp->next; 
	}
}	

static int gl_maxfilecnt;
static void InitMenuThree_TuPian(unsigned char*ptr){
	int maxfilecnt;
	
	ShowRECTime(ptr);
	osd_print_Mypic(40,0,20,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,28,276,ptr,MYRETURN);
	
	osd_print_Mypic(40,0,678,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,688,276,ptr,MYENTER);

	
	if(LinkCreateFlag == 0){
		gl_MyBmpLList = mySearchDirFun("/home/root/media/",".bmp");
		if(gl_MyBmpLList == NULL){
			printf("Search Dir error.\n");
			exit(1);
		}
		gl_MyBmpLList->getMemSum(gl_MyBmpLList,&gl_maxfilecnt);
		
		LinkCreateFlag = 1;	
	}
	if(gl_maxfilecnt == 0){
		osd_print_rect(330,500,0,ptr);
		osd_print_string("NO FILE",7,335,502,GREENPAN,ptr,MYASCIIDOT);
		return ;
	}

	if(gl_maxfilecnt < 4){
		CreatTuPianShow(gl_maxfilecnt,ptr,gl_MyBmpLList,gl_num,PICTURESELCETFLAG);
	}else{
		CreatTuPianShow(4,ptr,gl_MyBmpLList,gl_num,PICTURESELCETFLAG);
	}
}

static PT_MenuGuiSt DealMenuThree_TuPian(PT_MenuGuiSt ptr){
	PT_MenuGuiSt pt_retnode = NULL;
	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
			PICTURESELCETFLAG += 1;
		if(gl_maxfilecnt > 4){
			if(PICTURESELCETFLAG > 4){
				PICTURESELCETFLAG = 4;
				if(gl_num != (gl_maxfilecnt - 4)) 
						gl_num++;
			}
		}else{
			if(PICTURESELCETFLAG > gl_maxfilecnt){
				PICTURESELCETFLAG = gl_maxfilecnt;
			}
		}
		glUsartInfoSt.RightRoll = 0;
	}
	
	if(glUsartInfoSt.LeftRoll){
		PICTURESELCETFLAG -= 1;
		if(PICTURESELCETFLAG < 1){
			PICTURESELCETFLAG = 1;
			if(gl_num > 0)gl_num--;
			gl_num = 0; 	
		}
		glUsartInfoSt.LeftRoll = 0;
	}
	
	if(glUsartInfoSt.LeftKeyInfo == 1){
		llist_destroy(gl_MyBmpLList);
		PICTURESELCETFLAG = 1;
		LinkCreateFlag  = 0;
		pt_retnode = ptr->FarthMenu;
		glUsartInfoSt.LeftKeyInfo = 0;
	}

	if(glUsartInfoSt.RightKeyInfo == 1){
		gl_ShowPicFlag = 1;		
		glUsartInfoSt.RightKeyInfo = 0;	
	}
	return pt_retnode;
}

static int maxvideocnt;
static int gl_videonum = 0;

static void InitMenuThree_Shipin(unsigned char*ptr){
	   ShowRECTime(ptr);
		
	   osd_print_Mypic(40,0,20,268,ptr,MYHEISEYUAN);
	   osd_print_Mypic(24,0xff,28,276,ptr,MYRETURN);
	
	   osd_print_Mypic(40,0,678,268,ptr,MYHEISEYUAN);
	   osd_print_Mypic(24,0xff,688,276,ptr,MYENTER);	
		//PICTURESELCETFLAG
		
		if(LinkCreateFlag == 0){
			gl_MyH264LList = mySearchDirFun("/home/root/media/",".264");
			if(gl_MyH264LList == NULL){
				printf("Search Dir error.\n");
				exit(1);
			}
			gl_MyH264LList->getMemSum(gl_MyH264LList,&maxvideocnt);
			LinkCreateFlag = 1; 
		}
		if(maxvideocnt == 0){
				osd_print_rect(330,500,0,ptr);
				osd_print_string("NO FILE",7,335,502,GREENPAN,ptr,MYASCIIDOT);
				return ;
		}
		if(maxvideocnt < 4){
			CreatTuPianShow(maxvideocnt,ptr,gl_MyH264LList,gl_videonum,VIDEOSELCETFLAG);
		}else{
			CreatTuPianShow(4,ptr,gl_MyH264LList,gl_videonum,VIDEOSELCETFLAG);
		}
		
}

static PT_MenuGuiSt DealMenuThree_Shipin(PT_MenuGuiSt ptr){
	PT_MenuGuiSt pt_retnode = NULL;
	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
		VIDEOSELCETFLAG += 1;
		if(maxvideocnt > 4){
			if(VIDEOSELCETFLAG > 4){
				VIDEOSELCETFLAG = 4;
				if(gl_videonum != (maxvideocnt - 4)) 
						gl_videonum++;
			}
		}else{
			if(VIDEOSELCETFLAG > maxvideocnt){
				VIDEOSELCETFLAG = maxvideocnt;
			}
		}
		glUsartInfoSt.RightRoll = 0;
	}
	if(glUsartInfoSt.LeftRoll){
				VIDEOSELCETFLAG -= 1;
		if(VIDEOSELCETFLAG < 1){
				VIDEOSELCETFLAG = 1;
				if(gl_videonum > 0)gl_videonum--;
				gl_videonum = 0; 	
		}
		
		glUsartInfoSt.LeftRoll = 0;
	}
	if(glUsartInfoSt.LeftKeyInfo == 1){
		llist_destroy(gl_MyH264LList);
		VIDEOSELCETFLAG = 1;
		LinkCreateFlag  = 0;
		pt_retnode = ptr->FarthMenu;
		glUsartInfoSt.LeftKeyInfo = 0;
	}
	
	if(glUsartInfoSt.RightKeyInfo == 1){
		gl_ShowVideoFlag = 1;		
		glUsartInfoSt.RightKeyInfo = 0;	
	}
	return pt_retnode;
}

static void InitMenuThree_CaiDan(unsigned char*ptr){	
	ShowRECTime(ptr);
	osd_print_Mypic(40,0,20,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,28,276,ptr,MYRETURN);
	
	osd_print_Mypic(40,0,678,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,688,276,ptr,MYENTER);

		//osd_print_Chinese(100,100,);
	if(CAIDANSELCETFLAG == 1)
		osd_print_rect(130,530,YELLOWPAN,ptr);
	else
		osd_print_rect(130,530,0,ptr);
	osd_print_Chinese(142, 532,YELLOWPAN,ptr,LIANGDU,2,4);

	if(CAIDANSELCETFLAG == 2)
		osd_print_rect(210,530,GREENPAN,ptr);
	else
		osd_print_rect(210,530,0,ptr);
	osd_print_Chinese(212,532,YELLOWPAN,ptr,DUIBIDU,3,4);

	if(CAIDANSELCETFLAG == 3)
		osd_print_rect(290,530,GREENPAN,ptr);
	else
		osd_print_rect(290,530,0,ptr);
	osd_print_Chinese(302, 532,YELLOWPAN,ptr,FANGDA,2,4);

	if(CAIDANSELCETFLAG == 4)
		osd_print_rect(370,530,GREENPAN,ptr);
	else 
		osd_print_rect(370,530,0,ptr);
	osd_print_Chinese(382, 532,YELLOWPAN,ptr,JIXING,2,4);
	
	if(CAIDANSELCETFLAG == 5)
		osd_print_rect(450,530,GREENPAN,ptr);
	else
		osd_print_rect(450,530,0,ptr);
	osd_print_Chinese(462, 532,YELLOWPAN,ptr,FENHUA,2,4);

	if(CAIDANSELCETFLAG == 6)
		osd_print_rect(530,530,GREENPAN,ptr);
	else
		osd_print_rect(530,530,0,ptr);
	osd_print_Chinese(530, 532,YELLOWPAN,ptr,TUXIANGZHENGQIANG,4,-1);
}

static PT_MenuGuiSt DealMenuThree_CaiDan(PT_MenuGuiSt ptr){
	PT_MenuGuiSt pt_retnode = NULL;
	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
		CAIDANSELCETFLAG += 1;
		if(CAIDANSELCETFLAG > 6)
				CAIDANSELCETFLAG = 1;
		glUsartInfoSt.RightRoll = 0;
	}
	
	if(glUsartInfoSt.LeftRoll){
		CAIDANSELCETFLAG -= 1;
		if(CAIDANSELCETFLAG < 1)
				CAIDANSELCETFLAG = 6;
		glUsartInfoSt.LeftRoll = 0;
	}
	
	if(glUsartInfoSt.LeftKeyInfo == 1){
		CAIDANSELCETFLAG = 1;
		pt_retnode = ptr->FarthMenu;
		glUsartInfoSt.LeftKeyInfo = 0;
	}
		
	if(glUsartInfoSt.RightKeyInfo == 1){
		if(CAIDANSELCETFLAG == 1)
			pt_retnode = ptr->ChildMenu1;
		if(CAIDANSELCETFLAG == 2)
			pt_retnode = ptr->ChildMenu2;
		if(CAIDANSELCETFLAG == 3)
			pt_retnode = ptr->ChildMenu3;
		if(CAIDANSELCETFLAG == 4)
			pt_retnode = ptr->ChildMenu4;
		if(CAIDANSELCETFLAG == 5)
			pt_retnode = ptr->ChildMenu5;
		if(CAIDANSELCETFLAG == 6)
			pt_retnode = ptr->ChildMenu6;
		
		glUsartInfoSt.RightKeyInfo = 0;	
	}
	return pt_retnode;
}
static void InitMenuFour_LD(unsigned char*ptr){
	ShowRECTime(ptr);

	char ldstr[10];
	osd_print_Mypic(40,0,20,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,28,276,ptr,MYRETURN);	
	
	osd_print_Mypic(24,YELLOWPAN,244,520,ptr,LIANGDUJINDUTIAO1);
	osd_print_jindutiaoRect(200,4,274,530,YELLOWPAN,ptr);	
	osd_print_jindutiaoRect(6,20,274+(LIANGDUCOUNT*5/6), 522, YELLOWPAN,ptr);
	osd_print_jindutiaoRect((LIANGDUCOUNT*5/6),4,274, 530, GREENPAN,ptr);
	
	ldstr[0] = '%';
	if( LIANGDUCOUNT == 240)
		sprintf(&ldstr[1], "%d", LIANGDUCOUNT*100/240);
	else
		sprintf(&ldstr[1], "%0.2d", LIANGDUCOUNT*100/240);
	
	osd_print_string(ldstr,4,484,525,YELLOWPAN,ptr,MYASCIIDOT);  //printf battery value width = 4*6=24
}
static PT_MenuGuiSt DealMenuFour_LD(PT_MenuGuiSt ptr){
		PT_MenuGuiSt pt_retnode = NULL;
		//if(glUsartInfoSt.RightKeyInfo)
		//if(glUsartInfoSt.LeftKeyInfo)
		if(glUsartInfoSt.RightRoll){
			LIANGDUCOUNT += 16;
			if(LIANGDUCOUNT > 240) LIANGDUCOUNT = 240;
			ioctl(gl_capture_fd, VPFE_CMD_OLED_BRIGHT_DOWN,LIANGDUCOUNT);
			glUsartInfoSt.RightRoll = 0;
		}
		if(glUsartInfoSt.LeftRoll){
			LIANGDUCOUNT -= 16;
			if(LIANGDUCOUNT < 0) LIANGDUCOUNT = 0;
			ioctl(gl_capture_fd, VPFE_CMD_OLED_BRIGHT_DOWN,LIANGDUCOUNT);
			glUsartInfoSt.LeftRoll = 0;
		}
		if(glUsartInfoSt.LeftKeyInfo == 1){
			pt_retnode = ptr->FarthMenu;
			glUsartInfoSt.LeftKeyInfo = 0;
		}
		return pt_retnode;
}
static void InitMenuFour_DBD(unsigned char*ptr){
	ShowRECTime(ptr);
	char dbdstr[10];
	osd_print_Mypic(40,0,20,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,28,276,ptr,MYRETURN);

	osd_print_Mypic(24,YELLOWPAN,244,520,ptr,DUIBIDUJINDUTIAO);
	osd_print_jindutiaoRect(200,4,274,530, YELLOWPAN,ptr);
	osd_print_jindutiaoRect(6,20,274+(DUIBIDUCOUNT*5/6), 522, YELLOWPAN,ptr);	
	osd_print_jindutiaoRect( (DUIBIDUCOUNT*5/6),4,274, 530, GREENPAN,ptr);

	dbdstr[0] = '%';
	if( DUIBIDUCOUNT == 240)
		sprintf(&dbdstr[1], "%d", DUIBIDUCOUNT*100/240);
	else
		sprintf(&dbdstr[1], "%0.2d", DUIBIDUCOUNT*100/240);
	
	osd_print_string(dbdstr, 4, 484,525,YELLOWPAN,ptr,MYASCIIDOT); 
}
static PT_MenuGuiSt DealMenuFour_DBD(PT_MenuGuiSt ptr){
	PT_MenuGuiSt pt_retnode = NULL;
	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
		DUIBIDUCOUNT += 16;
		if(DUIBIDUCOUNT > 240) DUIBIDUCOUNT = 240;
		
		ioctl(gl_capture_fd, VPFE_CMD_OLED_CONTRAST,DUIBIDUCOUNT);
		glUsartInfoSt.RightRoll = 0;
	}
	if(glUsartInfoSt.LeftRoll){
		DUIBIDUCOUNT -= 16;
		if(DUIBIDUCOUNT < 0) DUIBIDUCOUNT = 0;
		ioctl(gl_capture_fd, VPFE_CMD_OLED_CONTRAST,DUIBIDUCOUNT);
		glUsartInfoSt.LeftRoll = 0;
	}
	if(glUsartInfoSt.LeftKeyInfo == 1){
		pt_retnode = ptr->FarthMenu;
		glUsartInfoSt.LeftKeyInfo = 0;
	}
	return pt_retnode;
}
static void InitMenuFour_FD(unsigned char*ptr){
	ShowRECTime(ptr);
	osd_print_Mypic(40,0,20,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,28,276,ptr,MYRETURN);
	
	osd_print_Mypic(40,0,678,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,688,276,ptr,MYENTER);

	if(FANGDAFLAG == 0)
		osd_print_rect(280,530,GREENPAN,ptr);
	else
		osd_print_rect(280,530,0,ptr);
	osd_print_string("1X",2,304,532,YELLOWPAN,ptr,MYASCIIDOT);
	
	if(FANGDAFLAG == 1)
		osd_print_rect(360,530,GREENPAN,ptr);
	else
		osd_print_rect(360,530,0,ptr);
	osd_print_string("2X",2,384,532,YELLOWPAN,ptr,MYASCIIDOT);

}
static PT_MenuGuiSt DealMenuFour_FD(PT_MenuGuiSt ptr){
	PT_MenuGuiSt pt_retnode = NULL;
	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
		FANGDAFLAG -= 1;
		if(FANGDAFLAG < 0)
				FANGDAFLAG = 1;
		glUsartInfoSt.RightRoll = 0;
	}
	
	if(glUsartInfoSt.LeftRoll){
		FANGDAFLAG += 1;
			if(FANGDAFLAG > 1)
				FANGDAFLAG = 0;
		glUsartInfoSt.LeftRoll = 0;
	}
	if(glUsartInfoSt.LeftKeyInfo == 1){
		FANGDAFLAG = FANGDACOUNT;
		pt_retnode = ptr->FarthMenu;
		glUsartInfoSt.LeftKeyInfo = 0;
	}
		
	if(glUsartInfoSt.RightKeyInfo == 1){
		FANGDACOUNT = FANGDAFLAG;
		SendData[1] = 0x20 | (FANGDAFLAG << 1);
		ProcessUsartSend();

		glUsartInfoSt.RightKeyInfo = 0;	
	}
	return pt_retnode;
}
static void InitMenuFour_JX(unsigned char*ptr){
	ShowRECTime(ptr);

	osd_print_Mypic(40,0,20,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,28,276,ptr,MYRETURN);
	
	osd_print_Mypic(40,0,678,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,688,276,ptr,MYENTER);

	if(JIXINGFLAG)
		osd_print_rect(290,530,GREENPAN,ptr);
	else
		osd_print_rect(290,530,0xff,ptr);
	osd_print_Chinese(302, 532,YELLOWPAN,ptr,HEIRE,2,4);

	if(!JIXINGFLAG)
		osd_print_rect(370,530,GREENPAN,ptr);
	else
		osd_print_rect(370,530,0xff,ptr);
	osd_print_Chinese(382, 532,YELLOWPAN,ptr,BAIRE,2,4);
}
static PT_MenuGuiSt DealMenuFour_JX(PT_MenuGuiSt ptr){
	PT_MenuGuiSt pt_retnode = NULL;
	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
		JIXINGFLAG -= 1;
		if(JIXINGFLAG < 0)
				JIXINGFLAG = 1;
		glUsartInfoSt.RightRoll = 0;
	}
	if(glUsartInfoSt.LeftRoll){
		JIXINGFLAG += 1;
			if(JIXINGFLAG > 1)
				JIXINGFLAG = 0;
		glUsartInfoSt.LeftRoll = 0;
	}
	if(glUsartInfoSt.LeftKeyInfo == 1){
		JIXINGFLAG = JIXINGCOUNT;
		pt_retnode = ptr->FarthMenu;
		glUsartInfoSt.LeftKeyInfo = 0;
	}
	if(glUsartInfoSt.RightKeyInfo == 1){
		JIXINGCOUNT = JIXINGFLAG;
		SendData[1] = 0x10 | JIXINGFLAG;
		ProcessUsartSend();
		glUsartInfoSt.RightKeyInfo = 0;	
	}
	return pt_retnode;
}
static void InitMenuFour_FH(unsigned char*ptr){
	ShowRECTime(ptr);
	osd_print_Mypic(40,0,20,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,28,276,ptr,MYRETURN);
	
	osd_print_Mypic(40,0,678,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,688,276,ptr,MYENTER);

	if(FENHUAFLAG == 1)
		osd_print_rect(290,530,GREENPAN,ptr);
	else
		osd_print_rect(290,530,0xff,ptr);
	osd_print_Chinese(302, 532,YELLOWPAN,ptr,GUANBI,2,4);

	if(FENHUAFLAG == 2)
		osd_print_rect(370,530,GREENPAN,ptr);
	else
		osd_print_rect(370,530,0xff,ptr);
	osd_print_Chinese(382, 532,YELLOWPAN,ptr,DAKAI,2,4);
}
static PT_MenuGuiSt DealMenuFour_FH(PT_MenuGuiSt ptr){
	PT_MenuGuiSt pt_retnode = NULL;
	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
		FENHUAFLAG -= 1;
		if(FENHUAFLAG < 1)
				FENHUAFLAG = 2;
		glUsartInfoSt.RightRoll = 0;
	}
	if(glUsartInfoSt.LeftRoll){
		FENHUAFLAG += 1;
			if(FENHUAFLAG > 2)
				FENHUAFLAG = 1;
		glUsartInfoSt.LeftRoll = 0;
	}
	if(glUsartInfoSt.LeftKeyInfo == 1){
		FENHUAFLAG = FENHUACOUNT;
		pt_retnode = ptr->FarthMenu;
		glUsartInfoSt.LeftKeyInfo = 0;
	}
		
	if(glUsartInfoSt.RightKeyInfo == 1){
		FENHUACOUNT = FENHUAFLAG;
		glUsartInfoSt.RightKeyInfo = 0;	
	}
	return pt_retnode;
}

static void InitMenuFour_TQ(unsigned char*ptr){
	ShowRECTime(ptr);
	osd_print_Mypic(40,0,20,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,28,276,ptr,MYRETURN);
	
	osd_print_Mypic(40,0,678,268,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,688,276,ptr,MYENTER);

	if(TUQIANGFLAG == 0)
		osd_print_rect(290,530,GREENPAN,ptr);
	else
		osd_print_rect(290,530,0xff,ptr);
	osd_print_Chinese(302, 532,YELLOWPAN,ptr,GUANBI,2,4);

	if(TUQIANGFLAG == 1)
		osd_print_rect(370,530,GREENPAN,ptr);
	else
		osd_print_rect(370,530,0xff,ptr);
	osd_print_Chinese(382, 532,YELLOWPAN,ptr,DAKAI,2,4);
}
static PT_MenuGuiSt DealMenuFour_TQ(PT_MenuGuiSt ptr){
	PT_MenuGuiSt pt_retnode = NULL;
	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
		TUQIANGFLAG -= 1;
		if(TUQIANGFLAG < 0)
				TUQIANGFLAG = 1;
		glUsartInfoSt.RightRoll = 0;
	}
	if(glUsartInfoSt.LeftRoll){
		TUQIANGFLAG += 1;
			if(TUQIANGFLAG > 1)
				TUQIANGFLAG = 0;
		glUsartInfoSt.LeftRoll = 0;
	}
	if(glUsartInfoSt.LeftKeyInfo == 1){
		 TUQIANGFLAG = TUQIANGCOUNT;
		pt_retnode = ptr->FarthMenu;
		glUsartInfoSt.LeftKeyInfo = 0;
	}
		
	if(glUsartInfoSt.RightKeyInfo == 1){
		TUQIANGCOUNT = TUQIANGFLAG;
		SendData[1] = 0x80 | (TUQIANGFLAG << 3);
		ProcessUsartSend();
		glUsartInfoSt.RightKeyInfo = 0;	
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
	MenuHead->ChildMenu5 = NULL;
	MenuHead->ChildMenu6 = NULL;
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
	MenuChild->ChildMenu5 = NULL;
	MenuChild->ChildMenu6 = NULL;
	if(num == 1)
		FarthMenu->ChildMenu1 = MenuChild;
	if(num == 2)
		FarthMenu->ChildMenu2 = MenuChild;
	if(num == 3)
		FarthMenu->ChildMenu3 = MenuChild;
	if(num == 4)
		FarthMenu->ChildMenu4 = MenuChild;
	if(num == 5)
		FarthMenu->ChildMenu5 = MenuChild;
	if(num == 6)
		FarthMenu->ChildMenu6 = MenuChild;
}

void CreateSecMenu(PT_MenuGuiSt Head){
	InsertChildMenuHead(Head,1,InitMenuSec,DealMenuSec);
	InsertChildMenuHead(Head->ChildMenu1,1,InitMenuThree_MoShi ,DealMenuThree_MoShi);
	InsertChildMenuHead(Head->ChildMenu1,2,InitMenuThree_TuPian,DealMenuThree_TuPian);
	InsertChildMenuHead(Head->ChildMenu1,3,InitMenuThree_Shipin,DealMenuThree_Shipin);
	InsertChildMenuHead(Head->ChildMenu1,4,InitMenuThree_CaiDan,DealMenuThree_CaiDan);

	InsertChildMenuHead(Head->ChildMenu1->ChildMenu4,1,InitMenuFour_LD,DealMenuFour_LD);
	InsertChildMenuHead(Head->ChildMenu1->ChildMenu4,2,InitMenuFour_DBD,DealMenuFour_DBD);
	InsertChildMenuHead(Head->ChildMenu1->ChildMenu4,3,InitMenuFour_FD,DealMenuFour_FD);
	InsertChildMenuHead(Head->ChildMenu1->ChildMenu4,4,InitMenuFour_JX,DealMenuFour_JX);
	InsertChildMenuHead(Head->ChildMenu1->ChildMenu4,5,InitMenuFour_FH,DealMenuFour_FH);
	InsertChildMenuHead(Head->ChildMenu1->ChildMenu4,6,InitMenuFour_TQ,DealMenuFour_TQ);
}




