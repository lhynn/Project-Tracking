#include "../char_table.h"
#include "../ctrl.h"
//flag 1,远红外开 2，远白炽  3，都开
int osd_print_CarLight(int flag,int start_x, int start_y, unsigned char* bufptr){
	int offset;

    int i, j, k,l;
    int x, y;
	unsigned char *tab;

    unsigned char ch;
    unsigned short table;
	int x_tmp,y_tmp;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

	x_tmp=start_x;
	y_tmp=start_y;

	if(flag == 1 || flag == 3)   tab= Hongwai;
	if(flag == 2)				 tab= BaiZhi;
	for(i=0;i<2;i++)
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
					 *(bufptr + offset) =149; 	//Y channel 
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 43;
					*(bufptr + uvstart + vvoffset) = 21;//128;//200; 	//set VV 
					}													
				}				 		
			}				
		}
	}
///////////////////////////////////////////////////////
	if(flag == 3) 
	{
		tab= BaiZhi;
	    x_tmp=start_x + 24*2;
	    y_tmp=start_y;
	  	for(i=0;i<2;i++)
		{
			x_tmp=x_tmp+24;
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
						 *(bufptr + offset) = 0xff; 	//Y channel 
						uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
						vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
						*(bufptr + uvstart + uuoffset) = 0;
						*(bufptr + uvstart + vvoffset) = 0;//128;//200; 	//set VV 
						}													
					}						
				}				
			}
		} 
	}
		return 1;
}

//flag 0 车体翻转,flag 1 红外补光
int osd_print_CarStat(int flag,int start_x, int start_y, unsigned char* bufptr)
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

	x_tmp=start_x;
	y_tmp=start_y;

	if( flag & 0x01)  tab= CarFanZhuan;
	else tab= HongWaiBuGuang;
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
					                    //*(bufptr + offset) = 0xff;
			       *(bufptr + offset) = 0xff;     //Y channel
			//point to pixel (x,y)	
			        uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
			        vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
				    *(bufptr + uvstart + uuoffset) = 0;
				    *(bufptr + uvstart + vvoffset) = 0;//128;//200;     //set VV             		 		
					}													
				}				 		
			}				
		}
	}
}
//dianji	 1 	left   0 	right
//baojing	 0 dinaliu 1	wendu

int osd_print_baojing(int dianji,int baojing,int start_x, int start_y, unsigned char* bufptr)
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

	x_tmp=start_x;
	y_tmp=start_y;

	if(dianji&0x01)   tab= LeftDianJi;
	else tab= RightDianJi;
	for(i=0;i<3;i++)
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
					 *(bufptr + offset) = 0xff; 	//Y channel 
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 0;
					*(bufptr + uvstart + vvoffset) = 0;//128;//200; 	//set VV 
					}													
				}				 		
			}				
		}
	}
///////////////////////////////////////////////////////
	if(baojing&0x01)   tab= WenDuBaoJing;
	else tab= DianLiuBaoJing;
	x_tmp=start_x + 24*2;
	y_tmp=start_y;
		for(i=0;i<4;i++)
		{
			x_tmp=x_tmp+24;
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
						 *(bufptr + offset) = 0xff; 	//Y channel 
						uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
						vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
						*(bufptr + uvstart + uuoffset) = 0;
						*(bufptr + uvstart + vvoffset) = 0;//128;//200; 	//set VV 
						}													
					}						
				}				
			}
		} 
    return 1;
}


//flag 0 	left 1 	right
int osd_print_dianjibaohu(int flag,int start_x, int start_y, unsigned char* bufptr)
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

	x_tmp=start_x;
	y_tmp=start_y;

	if(flag&0x01)   tab= LeftDianJi;
	else tab= RightDianJi;
	for(i=0;i<3;i++)
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
					
 *(bufptr + offset) = 0xff;     //Y channel	
uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
*(bufptr + uvstart + uuoffset) = 0;
*(bufptr + uvstart + vvoffset) = 0;//128;//200;     //set VV 	              		 		
					}													
				}				 		
			}				
		}
	}
///////////////////////////////////////////////////////
	tab= BaoHu;
	x_tmp=start_x + 24*3;
	y_tmp=start_y;
		for(i=0;i<2;i++)
		{
			x_tmp=x_tmp+24*i;
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
						 *(bufptr + offset) = 0xff;     //Y channel	
						uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
						vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
						*(bufptr + uvstart + uuoffset) = 0;
						*(bufptr + uvstart + vvoffset) = 0;//128;//200;     //set VV 									
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

int osd_print_fix( int start_x, int start_y, unsigned char* bufptr)
{    
	int offset;

    int i, j, k,l;
    int x, y;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;
    unsigned char ch;
    unsigned short table;
	int x_tmp,y_tmp;

		x_tmp=start_x;
		y_tmp=start_y;
	for(i=0;i<5;i++)
	{
		x_tmp=start_x+24*i;
	 for (j = 0; j < 24; j++)      			 
	 {      			 		
	 	for(l=0;l< 3;l++)		   				
		{                        	
			table = carspeed[i*72+j*3+l];           		
			    for (k = 0; k < 8; k++)           					
		    	{								
					x = x_tmp + k + 8*l;              		
					y = y_tmp + j;               					
					offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;		
					
				if((table>>(7-k)) & 0x01)		             		   			
					{                  			 		
					 *(bufptr + offset) = 0xff;     //Y channel	
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 0;
					*(bufptr + uvstart + vvoffset) = 0;//128;//200;     //set VV               		 		
					}													
				}				 		
			}				
		}
	}

///////////////////////////////////////////////////////
	x_tmp=start_x;
	y_tmp=start_y+26;
		for(i=0;i<5;i++)
		{
			x_tmp=start_x+24*i;
		 for (j = 0; j < 24; j++)				 
		 {							
			for(l=0;l<3;l++)						
			{							
				
				table = bat[i*72+j*3+l];					
					for (k = 0; k < 8; k++) 							
					{								
						x = x_tmp + k+ 8*l; 					
						y = y_tmp + j;									
						offset = x + y*DISP_WIDTH;		//	y*vScreenInfo.xres; 	
						
					if((table>>(7-k)) & 0x01)										
						{									
						 *(bufptr + offset) = 0xff;     //Y channel	
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 0;
					*(bufptr + uvstart + vvoffset) = 0;//128;//200;     //set 						
						}													
					}						
				}				
			}
		}

   
               
    return 1;
}



int osd_print_string(unsigned char* str, int len, int start_x, int start_y, unsigned short color, unsigned char* bufptr)
{    
	int offset;

    int i, j, k;
    int x, y;

    unsigned char ch;
    unsigned short table;
    int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;
    // Update OSD memory
    for (i = 0; i < len; i++)
    {
        ch = (unsigned char)*(str+i);
        if((ch>MAX_CHAR) || (ch<MIN_CHAR)) 
		{
			ch = UNKNOWN_CHAR;	
			break;
		}
        ch -= MIN_CHAR;

        for (j = 0; j < CHAR_HEIGTH; j++)
        {
            table = CHAR_LUT[ch][j];

            for (k = 0; k < CHAR_WIDTH; k++)
            {
                x = start_x + i*CHAR_WIDTH + k;
                y = start_y + j;

                offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;

                if((table>>k) & 0x01)
                {
					 *(bufptr + offset) = 0xff; 	//Y channel 
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 0;
					*(bufptr + uvstart + vvoffset) = 0;//128;//200; 	//set VV 
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


int osd_print_sdfull(int start_x, int start_y, unsigned char* bufptr)
{    
    int j, k;
    int x, y;
	int offset;

    unsigned int  iconptr = 0;
    unsigned char bitoffset = 0;
	unsigned char buffer;

 	buffer = SDCARDFULL[0];

    
    for (j=0; j<27; j++)
    {
        for (k=0; k<72; k++)  //icon width
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


int osd_print_photo(int start_x, int start_y, unsigned char* bufptr)
{    
    int j, k;
    int x, y;
	int offset;

    unsigned int  iconptr = 0;
    unsigned char bitoffset = 0;
	unsigned char buffer;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;
	
 	buffer = PHOTO[0];

    
    for (j=0; j<27; j++)
    {
        for (k=0; k<24; k++)  //icon width
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
				buffer = PHOTO[iconptr];
			}
        }
    }
    return 1;
}

int osd_print_battery(int start_x, int start_y, unsigned char value, unsigned char* bufptr)
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
			x = start_x + k;
			y = start_y + j;
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
            x = start_x + k + 2;
            y = start_y + j + 2;
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



int osd_print_mini_str(unsigned char* str, int len, int start_x, int start_y, unsigned char* bufptr)
{    
	int offset;

    int i, j, k;
    int x, y;

    unsigned char ch;
    unsigned short line_pix;
    
    // Update OSD memory
    for (i=0; i<len; i++)   //draw a character
    {
        ch = (unsigned char)*(str+i);
        if((ch>MAX_CHAR) || (ch<MIN_CHAR)) 
		{
			ch = UNKNOWN_CHAR;	
			break;
		}
				
     //  ch -= MIN_CHAR;  //get positon in font buffer
  		//	 printf("ch=%d \n",ch);
        for (j = 0; j < MINI_CHAR_HEIGHT; j++)
        {
            line_pix = MINI_CHAR_LUT[ch][j];  //get pixels in a line

            for (k = 0; k < MINI_CHAR_WIDTH; k++)   //draw a line
            {
                x = start_x + i*MINI_CHAR_WIDTH + k;
                y = start_y + j;

                offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;

                if((line_pix>>(7-k)) & 0x01)   //draw a pixel
                {
                    *(bufptr + offset) = 0xff;
                }
            }
        }
    }

    return 1;
}

int osd_print_24(unsigned char* str, int len, int start_x, int start_y, unsigned char* bufptr)
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
                    //*(bufptr + offset) = 0xff;
			    *(bufptr + offset) = 0xff;     //Y channel
			//point to pixel (x,y)	
			    uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
			    vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
				*(bufptr + uvstart + uuoffset) = 0;
				*(bufptr + uvstart + vvoffset) = 0;//128;//200;     //set VV
                }
            }
		 }
       }
    }
    return 1;
}

	
