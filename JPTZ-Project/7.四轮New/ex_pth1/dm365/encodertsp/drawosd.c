#include "../char_table.h"
#include "../ctrl.h"

int osd_print_GroundRect(int height,int width,int start_x, int start_y,unsigned char **bufptr)
{

    int offset;
    int i, j, k;
    int x, y;
    // Update OSD memory
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

    for (j = 0; j < height; j++){
        for (k = 0; k < width; k++){
			x= start_x + k;
            y= start_y + j;
            offset = x + y*RSZ_WIDTH;//y*vScreenInfo.xres;
 			*(*bufptr + offset) = 0; 
			uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
			vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
			*(*bufptr  + uvstart + uuoffset) =128;
			*(*bufptr  + uvstart + vvoffset) =128;//128;//200; 	//set VV 				
        }
    }
	return 1;
}

int osd_print_Red_Point(int start_x, int start_y,unsigned char* bufptr,int CoverFlag)
{
    int offset;

    int i, j, k;
    int x, y;
    // Update OSD memory
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

	if(CoverFlag){
		start_x = 736 - start_x -8;
		start_y = 576 - start_y -8;
	}
    for (j = 0; j < 8; j++)
    {
        for (k = 0; k < 8; k++)
        {
			x= start_x + k;
            y= start_y + j;
            offset = x + y*RSZ_WIDTH;//y*vScreenInfo.xres;
 			*(bufptr + offset) = 80; 
			uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
			vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
			*(bufptr + uvstart + uuoffset) = 80;
			*(bufptr + uvstart + vvoffset) = 247;//128;//200; 	//set VV 				
        }
    }
	return 1;
}

int osd_print_HongWai(int start_x, int start_y, unsigned char* bufptr,int CoverFlag)
{    
	int offset;

    int i, j, k,l;
    int x, y;
	unsigned char *tab;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

    unsigned char ch;
    unsigned short table;
	int x_tmp,y_tmp;
	int tmp;
	if(CoverFlag){
		start_x = 736 - start_x -24*2;
		start_y = 576 - start_y -24;
	}	
		osd_print_GroundRect(24,24*2,start_x,start_y,&bufptr);
	x_tmp=start_x;
	y_tmp=start_y;

	tab= GuangMin;
	for(i=0;i<2;i++){	
	 for (j = 1; j < 24; j++) {      			 		
	 	for(l=0;l<3;l++){                        	
			table = tab[i*72+j*3+l];      
			table = CoverFlag ? tab[(2-i)*72-j*3+l] : tab[i*72+j*3+l];      
			    for (k = 0; k < 8; k++)           					
		    	{								
					x = CoverFlag ?x_tmp + k+ (2-l)*8 : x_tmp + k+ 8*l;  
					y = y_tmp + j;               					
					offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;		
					
					tmp = CoverFlag ? table>>k & 0x01: table>>(7-k) & 0x01;
					if(tmp){                  			 		
			       *(bufptr + offset) = 0xff;     //Y channel
			//point to pixel (x,y)	
			        uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
			        vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
				    *(bufptr + uvstart + uuoffset) = 0;
				   // *(bufptr + uvstart + vvoffset) = 0;//128;//200;     //set VV             		 		
					}													
				}				 		
			}				
		}
	 x_tmp += 24;
	}
}



int osd_print_CarSpeedMode(int flag,int start_x, int start_y, unsigned char* bufptr,int CoverFlag){
	int offset;

    int i, j, k,l;
    int x, y;
	unsigned char *tab;

    unsigned char ch;
    unsigned short table;
	int x_tmp,y_tmp;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;
	int tmp;

	if(CoverFlag){
		start_x = DISP_WIDTH  - start_x - 24*2;
		start_y = DISP_HEIGHT - start_y - 24;
	}
	osd_print_GroundRect(24,24*2,start_x,start_y,&bufptr);

	x_tmp=start_x;
	y_tmp=start_y;
	if(flag == 1)  tab= GaoSu;
	else tab= DiSu;
		
	for(i=0;i<2;i++){
		for (j = 1; j < 24; j++){      			 		
	 		for(l=0;l<3;l++){                        	
				table = CoverFlag ? tab[(2-i)*72-j*3+l] : tab[i*72+j*3+l];      
			    for (k = 0; k < 8; k++){								
					x = CoverFlag ?x_tmp + k+ (2-l)*8 : x_tmp + k+ 8*l;              		
					y = y_tmp + j;               					
					offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;		
					tmp = CoverFlag ? table>>k & 0x01: table>>(7-k) & 0x01;
					if(tmp){                  			 		
						 *(bufptr + offset) =0xff; 	//Y channel 
						uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
						vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
						*(bufptr + uvstart + uuoffset) = 0;
					//*(bufptr + uvstart + vvoffset) = 21;//128;//200; 	//set VV 
					}													
				}				 		
			}				
		}
		x_tmp += 24;
	}
		return 1;
}

int osd_print_SheShidu(int start_x, int start_y, unsigned char *bufptr,int CoverFlag)
{    
	int offset;

    int i, j, k,l;
    int x, y;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;
    unsigned char ch;
    unsigned short table;
	int x_tmp,y_tmp;
	int tmp;
	if(CoverFlag){
		start_x = 736 - start_x - 24;
		start_y = 576 - start_y - 24;
	}	
	osd_print_GroundRect(24,24,start_x,start_y,&bufptr);

	x_tmp=start_x;
	y_tmp=start_y;
	for(i=0;i<1;i++){	
		for (j = 1; j < 24; j++){      			 		
	 		for(l=0;l< 3;l++){                        	
			table = CoverFlag ? SheShiDu[(1-i)*72-j*3+l] : SheShiDu[i*72+j*3+l];
			    for (k = 0; k < 8; k++){								
					x = CoverFlag ?x_tmp + k+ (2-l)*8 : x_tmp + k+ 8*l;               		
					y = y_tmp + j;               					
					offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;		
					tmp = CoverFlag ? table>>k & 0x01: table>>(7-k) & 0x01;
					if(tmp){                			 		
						 	*(bufptr + offset) = 0xff;     //Y channel	
							uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
							vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
							*(bufptr + uvstart + uuoffset) = 0;
							//*(bufptr + uvstart + vvoffset) = 0;//128;//200;     //set VV               		 		
					}													
				}				 		
			}				
		}
	 	x_tmp += 24;
	}  
};

int osd_print_CheTiWenDu(int start_x, int start_y, unsigned char* bufptr,int CoverFlag)
{    
	int offset;

    int i, j, k,l;
    int x, y;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;
    unsigned char ch;
    unsigned short table;
	int x_tmp,y_tmp;
	int tmp;
	int rect_x  = start_x;
	if(CoverFlag){
		start_x = 736 - start_x - 24*5;
		start_y = 576 - start_y - 24;
		rect_x = start_x+12;
	}	
	osd_print_GroundRect(24,24*4+12,rect_x,start_y,&bufptr);
	x_tmp=start_x;
	y_tmp=start_y;
	
	for(i=0;i<5;i++){	
		for (j = 1; j < 24; j++){      			 		
	 		for(l=0;l< 3;l++){                        	
			table = CoverFlag ? HuanJinWenDu[(5-i)*72-j*3+l] : HuanJinWenDu[i*72+j*3+l];
			    for (k = 0; k < 8; k++){								
					x = CoverFlag ?x_tmp + k+ (2-l)*8 : x_tmp + k+ 8*l;               		
					y = y_tmp + j;               					
					offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;		
					tmp = CoverFlag ? table>>k & 0x01: table>>(7-k) & 0x01;
					if(tmp){                			 		
						 	*(bufptr + offset) = 0xff;     //Y channel	
							uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
							vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
							*(bufptr + uvstart + uuoffset) = 0;
							//*(bufptr + uvstart + vvoffset) = 0;//128;//200;     //set VV               		 		
					}													
				}				 		
			}				
		}
	 	x_tmp += 24;
	}  
	
}


//flag 1,远红外开 2，远白炽  3，都开
int osd_print_CarLight(int flag,int start_x, int start_y, unsigned char* bufptr,int CoverFlag){
	int offset;

    int i, j, k,l;
    int x, y;
	unsigned char *tab;

    unsigned char ch;
    unsigned short table;
	int x_tmp,y_tmp;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;
	int tmp;

	if(CoverFlag){
		if(flag != 3)
			start_x = DISP_WIDTH  - start_x - 48;
		else
			start_x = DISP_WIDTH  - start_x - 24*2;
		start_y = DISP_HEIGHT - start_y - 24;
	}
	x_tmp=start_x;
	y_tmp=start_y;
	if(!flag)
		return -1;
	if(flag == 1 || flag == 3)
		tab= ZuoDeng;
	if(flag == 2)
		tab= YouDeng;
		
	int tmp_x = x_tmp;
	if(flag == 3){
		if(CoverFlag) tmp_x = x_tmp -48;
		osd_print_GroundRect(24,24*4,tmp_x,y_tmp,&bufptr);
	}
	else
	osd_print_GroundRect(24,24*2,x_tmp,y_tmp,&bufptr);
	for(i=0;i<2;i++){
		
		for (j = 0; j < 24; j++){      			 		
	 		for(l=0;l<3;l++){                        	
				table = CoverFlag ? tab[(2-i)*72-j*3+l] : tab[i*72+j*3+l];           		
			    for (k = 0; k < 8; k++){								
					x = CoverFlag ?x_tmp + k+ (2-l)*8 : x_tmp + k+ 8*l;              		
					y = y_tmp + j;               					
					offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;		
					tmp = CoverFlag ? table>>k & 0x01: table>>(7-k) & 0x01;
					if(tmp){                  			 		
						 *(bufptr + offset) =0xff; 	//Y channel 
						uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
						vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
						*(bufptr + uvstart + uuoffset) = 0;
					//*(bufptr + uvstart + vvoffset) = 21;//128;//200; 	//set VV 
					}													
				}				 		
			}				
		}
		x_tmp += 24;
	}
///////////////////////////////////////////////////////
	if(flag == 3) 
	{
		tab= YouDeng;
		if(CoverFlag)
	    	x_tmp=start_x - 24*2;
		else
			x_tmp =start_x + 24*2;
		y_tmp = start_y;

	  	for(i=0;i<2;i++){
			for (j = 0; j < 24; j++){							
				for(l=0;l<3;l++){							
					table = CoverFlag ? tab[(2-i)*72-j*3+l] : tab[i*72+j*3+l];					
					for (k = 0; k < 8; k++){								
						x = CoverFlag ?x_tmp + k+ (2-l)*8 : x_tmp + k+ 8*l;					
						y = y_tmp + j;									
						offset = x + y*DISP_WIDTH;		//	y*vScreenInfo.xres; 	
						tmp = CoverFlag ? table>>k & 0x01: table>>(7-k) & 0x01;
						if(tmp){  							
						 *(bufptr + offset) = 0xff; 	//Y channel 
						 uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
						 vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
						 *(bufptr + uvstart + uuoffset) = 0;
						 //*(bufptr + uvstart + vvoffset) = 38;//128;//200; 	//set VV 
						}													
					}						
				}				
			}
			 x_tmp += 24;
		} 
	}
		return 1;
}

//flag 0 车体翻转,flag 1 红外补光
int osd_print_CarStat(int start_x, int start_y, unsigned char* bufptr,int CoverFlag)
{    
	int offset;

    int i, j, k,l;
    int x, y;
	unsigned char *tab;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

    unsigned char ch;
    unsigned short table;
	int x_tmp,y_tmp;
	int tmp;
	int rect_x  = start_x;
	if(CoverFlag){
		start_x = 736 -start_x - 24*4;
		start_y = 576 - start_y -24;
		rect_x = start_x;
	}	
	osd_print_GroundRect(24,24*4,rect_x,start_y,&bufptr);
		
	x_tmp=start_x;
	y_tmp=start_y;

	tab= CarFanZhuan;
	for(i=0;i<4;i++){	
	 for (j = 1; j < 24; j++) {      			 		
	 	for(l=0;l<3;l++){                        	
			table = tab[i*72+j*3+l];      
			table = CoverFlag ? tab[(4-i)*72-j*3+l] : tab[i*72+j*3+l];
			    for (k = 0; k < 8; k++)           					
		    	{								
					x = CoverFlag ?x_tmp + k+ (2-l)*8 : x_tmp + k+ 8*l;  
					y = y_tmp + j;               					
					offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;		
					
					tmp = CoverFlag ? table>>k & 0x01: table>>(7-k) & 0x01;
					if(tmp){                  			 		
			       *(bufptr + offset) = 0xff;     //Y channel
			//point to pixel (x,y)	
			        uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
			        vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
				    *(bufptr + uvstart + uuoffset) = 0;
				   // *(bufptr + uvstart + vvoffset) = 0;//128;//200;     //set VV             		 		
					}													
				}				 		
			}				
		}
	 x_tmp += 24;
	}
}
//dianji	 1 	left   0 	right
//baojing	 0 dinaliu 1	wendu

int osd_print_baojing(int dianji,int baojing,int start_x, int start_y, unsigned char* bufptr,int CoverFlag)
{    
	int offset;

    int i, j, k,l;
    int x, y;
	unsigned char *tab;
	int tmp;
    unsigned char ch;
    unsigned short table;
	int x_tmp,y_tmp;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

	if(CoverFlag){
		start_x = 736 - start_x -24*7;
		start_y = 576 - start_y - 24;
	}
	osd_print_GroundRect(24,24*7,start_x,start_y,&bufptr);
	x_tmp=start_x;
	y_tmp=start_y;
	
	if(dianji&0x01)   tab= LeftDianJi;
	else tab= RightDianJi;
	
	if(CoverFlag){
		x_tmp = start_x + 24*4;
		y_tmp = start_y;
	}
	for(i=0;i<3;i++){
		for (j = 0; j < 24; j++){      			 		
	 		for(l=0;l<3;l++){                        	
				//table = tab[i*72+j*3+l];           		
				table = CoverFlag ? tab[(3-i)*72-j*3+l] : tab[i*72+j*3+l];
				for (k = 0; k < 8; k++){								
					x = CoverFlag ?x_tmp + k+ (2-l)*8 : x_tmp + k+ 8*l;             		
					y = y_tmp + j;               					
					offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;		
					//if((table>>(7-k)) & 0x01)		             		   			
					tmp = CoverFlag ? table>>k & 0x01: table>>(7-k) & 0x01;
					if(tmp){                  			 		
					 *(bufptr + offset) = 0xff; 	//Y channel 
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 0;
					//*(bufptr + uvstart + vvoffset) = 0;//128;//200; 	//set VV 
					}													
				}				 		
			}			
		}
		x_tmp += 24;
	}
///////////////////////////////////////////////////////
	if(baojing&0x01)   tab= WenDuBaoJing;
	else tab= DianLiuBaoJing;
	x_tmp=start_x + 24*3;
	y_tmp=start_y;
	if(CoverFlag){
		x_tmp = start_x;
		y_tmp = start_y;
	}
	for(i=0;i<4;i++){
		for (j = 0; j < 24; j++){							
			for(l=0;l<3;l++){							
			//	table = tab[i*72+j*3+l];					
				table = CoverFlag ? tab[(4-i)*72-j*3+l] : tab[i*72+j*3+l];
				for (k = 0; k < 8; k++){								
					x = CoverFlag ? x_tmp + k+ (2-l)*8 : x_tmp + k+ 8*l; 					
					y = y_tmp + j;									
					offset = x + y*DISP_WIDTH;		//	y*vScreenInfo.xres; 	
					tmp = CoverFlag ? table>>k & 0x01: table>>(7-k) & 0x01;
					if(tmp){							
						*(bufptr + offset) = 0xff; 	//Y channel 
						uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
						vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
						 *(bufptr + uvstart + uuoffset) = 0;
						// *(bufptr + uvstart + vvoffset) = 0;//128;//200; 	//set VV 
					}													
				}						
			}				
		}
		x_tmp += 24;
	} 
    return 1;
}


//flag 0 	left 1 	right
int osd_print_dianjibaohu(int flag,int start_x, int start_y, unsigned char* bufptr,int CoverFlag)
{    
	int offset;
	

    int i, j, k,l;
    int x, y;
	unsigned char *tab;

    unsigned char ch;
    unsigned short table;
	int x_tmp,y_tmp;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;
	int tmp = 0;
	if(CoverFlag){
			start_x = 736 - start_x - 24*5;
			start_y = 576 - start_y - 24;
	}
	osd_print_GroundRect(24,24*5,start_x,start_y,&bufptr);
	x_tmp=start_x;
	y_tmp=start_y;

	if(flag&0x01)
		tab = LeftDianJi;
	else
		tab = RightDianJi;
	if(CoverFlag){
		x_tmp = start_x + 24*2;
		y_tmp = start_y;
	}
	for(i=0;i<3;i++){
	 	for (j = 0; j < 24; j++){      			 		
	 		for(l=0;l<3;l++){          
				table = CoverFlag ? tab[(3-i)*72-j*3+l] : tab[i*72+j*3+l];
				for (k = 0; k < 8; k++){								
					x = CoverFlag ?x_tmp + k+ (2-l)*8 : x_tmp + k+ 8*l;              		
					y = y_tmp + j;               					
					offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;		
					tmp = CoverFlag ? table>>k & 0x01: table>>(7-k) & 0x01;
					if(tmp){                  			 		
 					*(bufptr + offset) = 0xff;     //Y channel	
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 0;            		 					
					}								
				}				 		
			}				
		}	
		x_tmp += 24;
	}
///////////////////////////////////////////////////////
	tab= BaoHu;
	x_tmp=start_x + 24*3;
	y_tmp=start_y;
	if(CoverFlag){
		x_tmp = start_x;
		y_tmp = start_y;
	}
	for(i=0;i<2;i++){
			x_tmp=x_tmp+24*i;
		 for (j = 23; j >= 0; j--){							
			for(l = 0;l < 3;l++){	
				table = CoverFlag ? tab[(2-i)*72-j*3+l] : tab[i*72+j*3+l];
				for (k = 0; k < 8; k++){								
					x = CoverFlag ? x_tmp + k+ (2-l)*8 : x_tmp + k+ 8*l;     			
					y = y_tmp + j;									
					offset = x + y*DISP_WIDTH;		//	y*vScreenInfo.xres; 	
					//.if((table<<(7-k)) & 0x80)
					tmp = CoverFlag ? table>>k & 0x01: table>>(7-k) & 0x01;
					if(tmp){
						 *(bufptr + offset) = 0xff;     //Y channel	
						uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
						vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
						*(bufptr + uvstart + uuoffset) = 0;
						//*(bufptr + uvstart + vvoffset) = 0;//128;//200;     //set VV 									
						}													
				}						
			}				
		}
	} 
    return 1;
}


int osd_print_light( int start_x, int start_y, unsigned char* bufptr)
{    
	int offset;

    int i, j, k,l;
    int x, y;
	unsigned char *tab;

    unsigned char ch;
    unsigned short table;
	int x_tmp,y_tmp;

	x_tmp=start_x;
	y_tmp=start_y;

	if(gl_light&0x01)   tab= light_on;
	else tab= light_off;
	for(i=0;i<4;i++)
	{
		x_tmp=start_x+24*i;
	 for (j = 0; j < 24; j++)      			 
	 {      			 		
	 	for(l=0;l<3;l++)		   				
		{                        	
			
			table = tab[i*72+j*3+l];           		
			    for (k = 0; k < 8; k++)           					
		    	{								
					x = x_tmp + k+ 8*l;              		
					y = y_tmp + j;               					
					offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;		
					
				if((table>>(7-k)) & 0x01)		             		   			
					{                  			 		
					*(bufptr + offset) = 0xff;              		 		
					}													
				}				 		
			}				
		}
	}

///////////////////////////////////////////////////////
	
	if(gl_light&0x02)	tab= red_on;
	else tab= red_off;

	x_tmp=start_x;
	y_tmp=start_y+26;
		for(i=0;i<4;i++)
		{
			x_tmp=start_x+24*i;
		 for (j = 0; j < 24; j++)				 
		 {							
			for(l=0;l<3;l++)						
			{							
				
				table = tab[i*72+j*3+l];					
					for (k = 0; k < 8; k++) 							
					{								
						x = x_tmp + k+ 8*l; 					
						y = y_tmp + j;									
						offset = x + y*DISP_WIDTH;		//	y*vScreenInfo.xres; 	
						
					if((table>>(7-k)) & 0x01)										
						{									
						*(bufptr + offset) = 0xff;								
						}													
					}						
				}				
			}
		} 
    return 1;
}

int osd_print_fix( int start_x, int start_y, unsigned char* bufptr,int CoverFlag)
{    
	int offset;

    int i, j, k,l;
    int x, y;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;
    unsigned char ch;
    unsigned short table;
	int x_tmp,y_tmp;
	int tmp;
	int rect_x  = start_x;
	if(CoverFlag){
		start_x = 736 - start_x - 24*5;
		start_y = 576 - start_y - 24;
		rect_x = start_x + 12;
	}	
	osd_print_GroundRect(24,24*4+12,rect_x,start_y,&bufptr);

	x_tmp=start_x;
	y_tmp=start_y;
	for(i=0;i<5;i++){	
		for (j = 1; j < 24; j++){      			 		
	 		for(l=0;l< 3;l++){                        	
			table = CoverFlag ? carspeed[(5-i)*72-j*3+l] : carspeed[i*72+j*3+l];
			    for (k = 0; k < 8; k++){								
					x = CoverFlag ?x_tmp + k+ (2-l)*8 : x_tmp + k+ 8*l;               		
					y = y_tmp + j;               					
					offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;		
					tmp = CoverFlag ? table>>k & 0x01: table>>(7-k) & 0x01;
					if(tmp){                			 		
						 	*(bufptr + offset) = 0xff;     //Y channel	
							uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
							vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
							*(bufptr + uvstart + uuoffset) = 0;
							//*(bufptr + uvstart + vvoffset) = 0;//128;//200;     //set VV               		 		
					}													
				}				 		
			}				
		}
	 	x_tmp += 24;
	}  
///////////////////////////////////////////////////////
	x_tmp=start_x;
	if(CoverFlag){
		y_tmp=start_y-24;
		rect_x = x_tmp+ 12;
	}
	else
		y_tmp=start_y+24;
	osd_print_GroundRect(24,24*4+12,rect_x,y_tmp,&bufptr);

	
	for(i=0;i<5;i++){
		for (j = 1; j < 24; j++){							
			for(l=0;l<3;l++){							
				table = CoverFlag ? bat[(5-i)*72-j*3+l] : bat[i*72+j*3+l];
					for (k = 0; k < 8; k++){								
						x = CoverFlag ?x_tmp + k+ (2-l)*8 : x_tmp + k+ 8*l;					
						y = y_tmp + j;									
						offset = x + y*DISP_WIDTH;		//	y*vScreenInfo.xres; 	
						tmp = CoverFlag ? table>>k & 0x01: table>>(7-k) & 0x01;
						if(tmp){    							
							 *(bufptr + offset) = 0xff;     //Y channel	
							uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
							vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
							*(bufptr + uvstart + uuoffset) = 0;
							//*(bufptr + uvstart + vvoffset) = 0;//128;//200;     //set 						
						}													
					}						
				}				
			}
			 x_tmp += 24;
		}         
    return 1;
}



int osd_print_string(unsigned char* str, int len, int start_x, int start_y, unsigned short color, unsigned char* bufptr,int CoverFlag)
{    
	int offset;

    int i, j, k;
    int x, y;

    unsigned char ch;
    unsigned short table;
    int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;
	int tmp;
		
    // Update OSD memory
    for (i = 0; i < len; i++)
   	 {
   		if(CoverFlag)
			ch = (unsigned char)*(str+len-1-i);
		else
      	 	ch = (unsigned char)*(str+i);	
        if((ch>MAX_CHAR) || (ch<MIN_CHAR)) 
		{
			ch = UNKNOWN_CHAR;	
			break;
		}
        ch -= MIN_CHAR;

        for (j = 0; j < CHAR_HEIGTH; j++)
        {
          //  table = CHAR_LUT[ch][j];
			table = CoverFlag ? CHAR_LUT[ch][CHAR_WIDTH*CHAR_HEIGTH -j] : CHAR_LUT[ch][j];
            for (k = 0; k < CHAR_WIDTH; k++)
            {
                x = start_x + i*CHAR_WIDTH + k;
                y = start_y + j;

                offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;

                tmp = CoverFlag ? table>>(7-k) & 0x01 : table>>k & 0x01;
				if(tmp){   
					 *(bufptr + offset) = 0xff; 	//Y channel 
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 0;
				//	*(bufptr + uvstart + vvoffset) = 0;//128;//200; 	//set VV 
                }
            }
        }
    }

    return 1;
}



int osd_print_icon(int start_x, int start_y, unsigned char iconid, unsigned char* bufptr)
{    
    int j, k;
    int x, y;
	int offset;

    unsigned int iconptr = 0;
    unsigned char bitoffset = 0;
	unsigned char buffer;

	if(iconid < 6)
 		buffer= SYSTEMICON[iconid][0];

    
    for (j=0; j<ICON_HEIGHT; j++)
    {
        for (k=0; k<ICON_WIDTH; k++)
        {
            x = start_x + k;
            y = start_y + j;

            offset = x + y*DISP_WIDTH;

			if( (buffer & 0x80)  == 0x80 )
            {
                *(bufptr + offset) = 0xff;
			}

			buffer <<= 1;
 			bitoffset ++;

            if(bitoffset == 8)
			{
				bitoffset = 0;
				iconptr++;
				buffer = SYSTEMICON[iconid][iconptr];
			}
        }
    }
    return 1;
}


int osd_print_cross(int start_x, int start_y, unsigned char* bufptr)
{    
    int j, k;
    int x, y;
	int offset;

    unsigned int  iconptr = 0;
    unsigned char bitoffset = 0;
	unsigned char buffer;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

 	buffer = SDCARDFULL[0];

    
    for (j=0; j<48; j++)
    {
        for (k=0; k<48; k++)  //icon width
        {
            x = start_x + k;
            y = start_y + j;

            offset = x + y*DISP_WIDTH;

			if( (buffer & 0x80)  == 0x80 )
            {
 				*(bufptr + offset) = 0xff;     //Y channel	
				uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
				vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
				*(bufptr + uvstart + uuoffset) = 0;
				*(bufptr + uvstart + vvoffset) = 0;//128;//200;     //set VV
			}

			buffer <<= 1;
 			bitoffset ++;

            if(bitoffset == 8)
			{
				bitoffset = 0;
				iconptr++;
				buffer = CROSS[iconptr];
			}
        }
    }
    return 1;
}


int osd_print_sdfull(int start_x, int start_y, unsigned char* bufptr,int CoverFlag)
{    
    int j, k;
    int x, y;
	int offset;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

    unsigned int  iconptr = 0;
    unsigned char bitoffset = 0;
	unsigned char buffer;

 	buffer = SDCARDFULL[0];

	if(CoverFlag){
		start_x = 736 - start_x - 72;
		start_y = 576 - start_y;
	}
	osd_print_GroundRect(27,72,start_x,start_y,&bufptr);
    //243
    for (j=0; j<27; j++)
    {
        for (k=0; k<72; k++)  //icon width
        {
           
			if(CoverFlag){
				x = start_x + (71-k);
				y = start_y + (26-j);
			}else{
				x = start_x + k;
          		y = start_y + j;
			}
            offset = x + y*DISP_WIDTH;

			if( (buffer & 0x80)  == 0x80 )
            {
               *(bufptr + offset) = 0xff;     //Y channel	
				uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
				vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
				*(bufptr + uvstart + uuoffset) = 0;
			}

			buffer <<= 1;
 			bitoffset ++;

            if(bitoffset == 8)
			{
				bitoffset = 0;
				iconptr++;
				buffer = SDCARDFULL[iconptr];
			}
        }
    }
    return 1;
}

int osd_print_blackhot(int start_x, int start_y, unsigned char* bufptr)
{    
    int j, k;
    int x, y;
	int offset;

    unsigned int  iconptr = 0;
    unsigned char bitoffset = 0;
	unsigned char buffer;

 	buffer = BLACKHOT[0];

    
    for (j=0; j<27; j++)
    {
        for (k=0; k<48; k++)  //icon width
        {
            x = start_x + k;
            y = start_y + j;

            offset = x + y*DISP_WIDTH;

			if( (buffer & 0x80)  == 0x80 )
            {
                *(bufptr + offset) = 0xff;
			}

			buffer <<= 1;
 			bitoffset ++;

            if(bitoffset == 8)
			{
				bitoffset = 0;
				iconptr++;
				buffer = BLACKHOT[iconptr];
			}
        }
    }
    return 1;
}

int osd_print_whitehot(int start_x, int start_y, unsigned char* bufptr)
{    
    int j, k;
    int x, y;
	int offset;

    unsigned int  iconptr = 0;
    unsigned char bitoffset = 0;
	unsigned char buffer;

 	buffer = WHITEHOT[0];

    
    for (j=0; j<27; j++)
    {
        for (k=0; k<48; k++)  //icon width
        {
            x = start_x + k;
            y = start_y + j;

            offset = x + y*DISP_WIDTH;

			if( (buffer & 0x80)  == 0x80 )
            {
                *(bufptr + offset) = 0xff;
			}

			buffer <<= 1;
 			bitoffset ++;

            if(bitoffset == 8)
			{
				bitoffset = 0;
				iconptr++;
				buffer = WHITEHOT[iconptr];
			}
        }
    }
    return 1;
}


int osd_print_color(int start_x, int start_y, unsigned char* bufptr)
{    
    int j, k;
    int x, y;
	int offset;

    unsigned int  iconptr = 0;
    unsigned char bitoffset = 0;
	unsigned char buffer;

 	buffer = COLOUR[0];

    
    for (j=0; j<27; j++)
    {
        for (k=0; k<48; k++)  //icon width
        {
            x = start_x + k;
            y = start_y + j;

            offset = x + y*DISP_WIDTH;

			if( (buffer & 0x80)  == 0x80 )
            {
                *(bufptr + offset) = 0xff;
			}

			buffer <<= 1;
 			bitoffset ++;

            if(bitoffset == 8)
			{
				bitoffset = 0;
				iconptr++;
				buffer = COLOUR[iconptr];
			}
        }
    }
    return 1;
}


int osd_print_photo(int start_x, int start_y, unsigned char* bufptr,int CoverFlag)
{    
    int j, k;
    int x, y;
	int offset;

    unsigned int  iconptr = 0;
    unsigned char bitoffset = 0;
	unsigned char buffer;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

	if(CoverFlag){
		start_x = 720 - start_x - 12;
		start_y = 576 - start_y - 26;
	}
			
 	buffer = PHOTO[0];

    for (j=0; j<27; j++)
    {
        for (k=0; k<24; k++)  //icon width
        {
       		 if(CoverFlag){
				x = start_x + (23-k);
				y = start_y + (26-j);
			}else{
				x = start_x + k;
          		y = start_y + j;
			}
             offset = x + y*DISP_WIDTH;

			if( (buffer & 0x80)  == 0x80 )
            {  
 				*(bufptr + offset) = 0xff;     //Y channel	
				uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
				vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
				*(bufptr + uvstart + uuoffset) = 0;
				//*(bufptr + uvstart + vvoffset) = 0;//128;//200;     //set VV 
			}
			buffer <<= 1;
 			bitoffset ++;

            if(bitoffset == 8)
			{
				bitoffset = 0;
				iconptr++;
				buffer = PHOTO[iconptr];
			}
        }
    }
    return 1;
}

int osd_print_battery(int start_x, int start_y, unsigned char value, unsigned char* bufptr,int CoverFlag)
{    
    int j, k;
    int x, y;
	int offset;
		
	int uvstart = DISP_WIDTH*DISP_HEIGHT;
	int uuoffset;
	int vvoffset;
		
    unsigned int iconptr = 0;
    unsigned char bitoffset = 0;
	unsigned char buffer;
	if(CoverFlag){
		start_x = 720 - start_x - BATT_ICON_WIDTH/2 - 2;
		start_y = 576 - start_y - BATT_ICON_HEIGHT;
	}
	
	if(value>100) value=100;
	else if(value<0) value=0;
	
	unsigned char fill_width = value/4;  //max=25, min=0;
	if(fill_width == 0)
		fill_width = 2;
	
	buffer = BATTICON[0];
		
	//1: draw a empty battery icon
    for (j=0; j<BATT_ICON_HEIGHT; j++)  
    {
        for (k=0; k<BATT_ICON_WIDTH; k++)
        { 
        	if(CoverFlag){
				x = start_x + (BATT_ICON_WIDTH - 1 - k);
				y = start_y + (BATT_ICON_HEIGHT - 1 - j);
			}else{
				x = start_x + k;
          		y = start_y + j;
			}
			offset = x + y*DISP_WIDTH;
			if( (buffer & 0x80)  == 0x80 )
			{
				*(bufptr + offset) = 0xff;  //Y channel

				uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
				vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;

				*(bufptr + uvstart + uuoffset) = 128;     //set UU
				//if(value > 15)
				*(bufptr + uvstart + vvoffset) = 128;     //set VV
				//else
				//	*(bufptr + uvstart + vvoffset) = 255;//200;     //set VV

			}
			
			buffer <<= 1;
			bitoffset ++;

			if(bitoffset == 8)
			{
				bitoffset = 0;
				iconptr ++;
				buffer = BATTICON[iconptr];
			}
        }
    }
    

    //2:filled battery depend on value
    for (j=0; j<BATT_FILL_HEIGHT-2; j++)  
    {
        for (k=0; k<fill_width; k++)
        {
       		if(CoverFlag){
				x = start_x + BATT_ICON_WIDTH - k - 3;
           		y = start_y + BATT_FILL_HEIGHT - 1 - j;
			}else{
				x = start_x + k + 2;
          		y = start_y + j + 2;
			}
            offset = x + y*DISP_WIDTH;
			if(value > 15)
			*(bufptr + offset) = 0xff;     //Y channel
			else
			 *(bufptr + offset) = 0;
			//point to pixel (x,y)	
			uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
			vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;

			//*(bufptr + uvstart + uuoffset) = 0;//128;     //set UU
			if(value > 15){
				*(bufptr + uvstart + uuoffset) = 0;
				*(bufptr + uvstart + vvoffset) = 0;//128;//200;     //set VV
			}
			else{
				*(bufptr + uvstart + uuoffset) = 128;
				*(bufptr + uvstart + vvoffset) = 255;//200;     //set VV
			}
        }
    } 

    return 1;
}



int osd_print_mini_str(unsigned char* str, int len, int start_x, int start_y, unsigned char* bufptr,int CoverFlag)
{    
	int offset;

    int i, j, k;
    int x, y;

    unsigned char ch;
    unsigned short line_pix;
	int tmp;
	if(CoverFlag){
		start_x = 720 - start_x- len - 8*(len-1);
		start_y = 576 - start_y - MINI_CHAR_HEIGHT;
	}
    // Update OSD memory
    for (i=0; i<len; i++)   //draw a character
    {
		if(CoverFlag)
			ch = (unsigned char)*(str+len-1-i);
		else
      		ch = (unsigned char)*(str+i);
        if((ch>MAX_CHAR) || (ch<MIN_CHAR)) 
		{
			ch = UNKNOWN_CHAR;	
			break;
		}
        for (j = 1; j < MINI_CHAR_HEIGHT; j++)
        {
           // line_pix = MINI_CHAR_LUT[ch][j];  //get pixels in a line
			line_pix =	CoverFlag ? MINI_CHAR_LUT[ch][16-j] : MINI_CHAR_LUT[ch][j];
            for (k = 0; k < MINI_CHAR_WIDTH; k++)   //draw a line
            {
                //x = start_x + i*MINI_CHAR_WIDTH + k;
				x = CoverFlag ? start_x + i*8 + k + 8: start_x + i*8 + k;
                y = start_y + j;

                offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;

              	tmp = CoverFlag ? line_pix>>k & 0x01: line_pix>>(7-k) & 0x01;
				if(tmp){ 
                    *(bufptr + offset) = 0xff;
                }
            }
        }
    }
    return 1;
}
int osd_print_LuoPanDate(unsigned char* str, int len, int start_x, int start_y, unsigned char* bufptr,int CoverFlag)
{    
	int offset;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

    int i, j, k;
    int x, y;

    unsigned char ch;
    unsigned short line_pix;
    int me1;
	int tmp;
	int tmp_x = start_x;
	if(CoverFlag){
		tmp_x = 736 - start_x - 16*len;
		start_y = 576 - start_y - 24;
		start_x = tmp_x-2;
	}
	osd_print_GroundRect(24,16*len,tmp_x,start_y,&bufptr);
   for (i=0; i<len; i++){
   		if(CoverFlag)
			ch = (unsigned char)*(str+len-1-i) - 32;
		else
      	  ch = (unsigned char)*(str+i) - 32;	
        for (j = 0; j < 24; j++)
        {
              //get pixels in a line
		 for(me1 = 0;me1 < 2;me1++){
			 line_pix =  CoverFlag ? CHAR_LUT_24[ch][48-j*2+me1] : CHAR_LUT_24[ch][j*2+me1];
            for (k = 0; k < 8; k++)   //draw a line
            {
				x = CoverFlag ? start_x + i*12 + k + (1-me1)*8 :start_x + i*12 + k + me1*8; 
                y = start_y + j;
                offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;
              	tmp = CoverFlag ? line_pix>>k & 0x01: line_pix>>(7-k) & 0x01;
				if(tmp){                  			 		
			    	*(bufptr + offset) = 0xff;     //Y channel
					//point to pixel (x,y)	
			   		 uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
			  		 vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 0;
                }
            }
		 }
       }
    }
    return 1;
}

int osd_print_LuoPan(unsigned char* str, int len, int start_x, int start_y, unsigned char* bufptr,int CoverFlag)
{    
	int offset;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

    int i, j, k;
    int x, y;

    unsigned char ch;
    unsigned short line_pix;
    int me1;
	int tmp;
	int tmp_x = start_x;
	if(CoverFlag){
		tmp_x = 736 - start_x - 16*len;
		start_y = 576 - start_y - 24-1;
		start_x = tmp_x + 12;
	}
	osd_print_GroundRect(24,16*len,tmp_x,start_y,&bufptr);
   for (i=0; i<len; i++){
   		if(CoverFlag)
			ch = (unsigned char)*(str+len-1-i) - 32;
		else
      	  ch = (unsigned char)*(str+i) - 32;	
        for (j = 0; j < 24; j++)
        {
              //get pixels in a line
		 for(me1 = 0;me1 < 2;me1++){
			 line_pix =  CoverFlag ? CHAR_LUT_24[ch][48-j*2+me1] : CHAR_LUT_24[ch][j*2+me1];
            for (k = 0; k < 8; k++)   //draw a line
            {
				x = CoverFlag ? start_x + i*12 + k + (1-me1)*8 :start_x + i*12 + k + me1*8; 
                y = start_y + j;
                offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;
              	tmp = CoverFlag ? line_pix>>k & 0x01: line_pix>>(7-k) & 0x01;
				if(tmp){                  			 		
			    	*(bufptr + offset) = 0xff;     //Y channel
					//point to pixel (x,y)	
			   		 uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
			  		 vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 0;
                }
            }
		 }
       }
    }
    return 1;
}

int osd_print_Speed(unsigned char* str, int len, int start_x, int start_y, unsigned char* bufptr,int CoverFlag)
{    
	int offset;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

    int i, j, k;
    int x, y;

    unsigned char ch;
    unsigned short line_pix;
    int me1;
	int tmp;
	int tmpx = start_x; 
	if(CoverFlag){
		tmpx = 736 - start_x - 16*len;
		start_x = tmpx+18;
		start_y = 576 - start_y - 24;
		 
	}
	osd_print_GroundRect(24,16*len,tmpx,start_y,&bufptr);
   for (i=0; i<len; i++){
   		if(CoverFlag)
			ch = (unsigned char)*(str+len-1-i) - 32;
		else
      	  ch = (unsigned char)*(str+i) - 32;	
        for (j = 0; j < 24; j++)
        {
              //get pixels in a line
		 for(me1 = 0;me1 < 2;me1++){
			 line_pix =  CoverFlag ? CHAR_LUT_24[ch][48-j*2+me1] : CHAR_LUT_24[ch][j*2+me1];
            for (k = 0; k < 8; k++)   //draw a line
            {
				x = CoverFlag ? start_x + i*12 + k + (1-me1)*8 :start_x + i*12 + k + me1*8; 
                y = start_y + j;
                offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;
              	tmp = CoverFlag ? line_pix>>k & 0x01: line_pix>>(7-k) & 0x01;
				if(tmp){                  			 		
			    	*(bufptr + offset) = 0xff;     //Y channel
					//point to pixel (x,y)	
			   		 uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
			  		 vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 0;
                }
            }
		 }
       }
    }
    return 1;
}

int osd_print_24(unsigned char* str, int len, int start_x, int start_y, unsigned char* bufptr,int CoverFlag)
{    
	int offset;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

    int i, j, k;
    int x, y;

    unsigned char ch;
    unsigned short line_pix;
    int me1;
	int tmp;
	int tmpx; 
	if(CoverFlag){
		start_x = 736 - start_x - 16*len;
		start_y = 576 - start_y - 24;
		 
	}
	osd_print_GroundRect(24,16*len,start_x,start_y,&bufptr);
   for (i=0; i<len; i++){
   		if(CoverFlag)
			ch = (unsigned char)*(str+len-1-i) - 32;
		else
      	  ch = (unsigned char)*(str+i) - 32;	
        for (j = 0; j < 24; j++)
        {
              //get pixels in a line
		 for(me1 = 0;me1 < 2;me1++){
			 line_pix =  CoverFlag ? CHAR_LUT_24[ch][48-j*2+me1] : CHAR_LUT_24[ch][j*2+me1];
            for (k = 0; k < 8; k++)   //draw a line
            {
				x = CoverFlag ? start_x + i*12 + k + (1-me1)*8 :start_x + i*12 + k + me1*8; 
                y = start_y + j;
                offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;
              	tmp = CoverFlag ? line_pix>>k & 0x01: line_pix>>(7-k) & 0x01;
				if(tmp){                  			 		
			    	*(bufptr + offset) = 0xff;     //Y channel
					//point to pixel (x,y)	
			   		 uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
			  		 vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 0;
                }
            }
		 }
       }
    }
    return 1;
}
int myOsdPrintFreeRect(int width,int height,int start_x, int start_y, int clock,unsigned char* bufptr,int CoverFlag)
{
    int offset;

    int i, j, k;
    int x, y;
    // Update OSD memory
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;
	if(CoverFlag)
		start_y = 576 - start_y;
	
    for (j = 0; j < height; j++)
    {
        for (k = 0; k < width; k++)
        {
			x= start_x + k;
            y= start_y + j;
            offset = x + y*DISP_WIDTH;//y*vScreenInfo.xres;
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
					*(bufptr + offset) =clock; 
			
        }
    }
	return 1;
}

void LCD_DrawLine(int x1, int y1, int x2, int y2,int clock,unsigned char* bufptr,int CoverFlag)
{
	int t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;
	int incx,incy,uRow,uCol; 
	if(CoverFlag){
		y1 = 576 - y1;
		y2 = 576 - y2;
	}
	delta_x=x2-x1; 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; 
	else if(delta_x==0)incx=0;
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0; 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x;  
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++){  
		*(bufptr + uCol * 736 + uRow) = 0xff;
		if(clock == 1){
					*(bufptr + uCol * 736 + uRow) = 50; 
					uuoffset = (uCol/2)*DISP_WIDTH + (uRow/2)*2;
					vvoffset = (uCol/2)*DISP_WIDTH + (uRow/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 40;
					*(bufptr + uvstart + vvoffset) = 10;//128;//200; 	//set VV 
           	
		}else if(clock == 2){
				*(bufptr + uCol * 736 + uRow) = 0xff;     //Y channel
			   uuoffset = (uCol/2)*DISP_WIDTH + (uRow/2)*2;
			   vvoffset = (uCol/2)*DISP_WIDTH + (uRow/2)*2 + 1;
				*(bufptr + uvstart + uuoffset) = 0;
		} else
			 *(bufptr + uCol * 736 + uRow) =clock; 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) { 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) { 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    


