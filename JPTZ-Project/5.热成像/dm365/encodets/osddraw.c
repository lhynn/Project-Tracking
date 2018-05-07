#include "../ctrl.h"
#include "../osdmanager.h"
int osd_print_photo(int start_x, int start_y, unsigned char* bufptr,unsigned char *bufarr)
{    
    int j, k;
    int x, y;
	int offset;

    unsigned int  iconptr = 0;
    unsigned char bitoffset = 0;
	unsigned char buffer;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;
	
 	buffer = bufarr[0];

    
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
				//*(bufptr + uvstart + vvoffset) = 0;//128;//200;     //set VV 
			}
			buffer <<= 1;
 			bitoffset ++;

            if(bitoffset == 8)
			{
				bitoffset = 0;
				iconptr++;
				buffer = bufarr[iconptr];
			}
        }
    }
    return 1;
}

int osd_print_Chinese(int start_x, int start_y, int clock,unsigned char* bufptr,unsigned char *charstr,int len,int Interval)
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

    tab= charstr;
	for(i=0;i<len;i++)
	{
		x_tmp=start_x+(Interval+16)*i;
	 for (j = 0; j < 16; j++)      			 
	 {      			 		
	 	for(l=0;l<2;l++)		   				
		{                        	
			    table = tab[i*32+j*2+l];           		
			    for (k = 0; k < 8; k++)           					
		    	{								
					x = x_tmp + k+ 8*l;              		
					y = y_tmp + j;               					
					offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;		
					
					if((table>>(7-k)) & 0x01)		             		   			
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
							*(bufptr + offset) =clock;      		 		
					}													
				}				 		
			}				
		}
	}
}


int osd_print_string(unsigned char* str,int len, int start_x, int start_y,int clock,unsigned char* bufptr,unsigned char *mine)
{    
	int offset;

    int i,j, k;
    int x, y;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;
		
    unsigned char ch;
    unsigned short table;

    for (i = 0; i < len; i++)
    {
        ch = (unsigned char)*(str+i);
        if((ch > MAX_CHAR) || (ch < MIN_CHAR)) 
		{
			ch = UNKNOWN_CHAR;	
			break;
		}
        ch -= MIN_CHAR;
        for (j = 0; j < 16; j++)
        {
            table = mine[ch*16+j];
            for (k = 0; k < 8; k++)
            {
                x = start_x + i*8 + k;
                y = start_y + j;
                offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;
				//printf("----->%d\n",offset);
                if((table<<k) & 0x80){
                 if(clock != 0xff && clock != 0){
				 	if(clock == 1){
 						*(bufptr + offset) = 152; 
						uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
						vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
						*(bufptr + uvstart + uuoffset) = 56;
						*(bufptr + uvstart + vvoffset) = 20;//128;//200; 	//set VV 
				 		}else if(clock == 2) {
							*(bufptr + offset) = 0xff;     //Y channel
			        		uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
			        		vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
				    		*(bufptr + uvstart + uuoffset) = 0;           
					}
				 }
				 else{
					*(bufptr + offset) = clock; 
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 127;
					*(bufptr + uvstart + vvoffset) = 127;//128;//200; 	//set VV 
				 }
                }
            }
        }
    }
    return 1;
}

int osd_print_Mypic(int size,int clock,int start_x, int start_y, unsigned char* bufptr,char *myPic)
{    
    int j, k;
    int x, y;
	int offset;
    unsigned int  iconptr = 0;
    unsigned char bitoffset = 0;
	unsigned char buffer;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

 	buffer = myPic[0];
    for (j=0; j<size; j++)
    {
        for (k=0; k<size; k++)  //icon width
        {	
			x = start_x + k;
            y = start_y + j;
            offset = x + y*DISP_WIDTH;
			if( (buffer & 0x80) == 0x80){
           		if(clock == 1){
					*(bufptr + offset) = 150; 
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 60;
					*(bufptr + uvstart + vvoffset) = 100;//128;//200; 	//set VV 
				}else if(clock == 2){
					*(bufptr + offset) = 0xff;     //Y channel
			   	 	uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
			    	vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 0;
				} else
					*(bufptr + offset) =clock; 
			}
			
			buffer <<= 1;
 			bitoffset ++;

            if(bitoffset == 8)
			{
				bitoffset = 0;
				iconptr++;
				buffer = myPic[iconptr];
			}
        }
    }
    return 1;
}


int osd_print_rect(int start_x, int start_y, int clock,unsigned char* bufptr)
{
    int offset;

    int i, j, k;
    int x, y;
    // Update OSD memory
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

    for (j = 0; j < 20; j++)
    {
        for (k = 0; k < 60; k++)
        {
			x= start_x + k;

            y= start_y + j;

            offset = x + y*SCREEN_WIDTH;//y*vScreenInfo.xres;
			if(clock != 0xff && clock != 0){
					*(bufptr + offset) = 150; 
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 60;
					*(bufptr + uvstart + vvoffset) = 100;//128;//200; 	//set VV 
           	
			}else
					 *(bufptr + offset) = 0; 
			
        }
    }
	return 1;
}
int osd_print_Red_Point(int start_x, int start_y,unsigned char* bufptr)
{
    int offset;

    int i, j, k;
    int x, y;
    // Update OSD memory
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

    for (j = 0; j < 8; j++)
    {
        for (k = 0; k < 8; k++)
        {
			x= start_x + k;
            y= start_y + j;
            offset = x + y*SCREEN_WIDTH;//y*vScreenInfo.xres;
 			*(bufptr + offset) = 80; 
			uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
			vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
			*(bufptr + uvstart + uuoffset) = 80;
			*(bufptr + uvstart + vvoffset) = 247;//128;//200; 	//set VV 				
        }
    }
	return 1;
}

int osd_print_battery(int start_x, int start_y, unsigned char value, unsigned char* bufptr,unsigned char *mypower)
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
	
	buffer = mypower[0];
		
	//1: draw a empty battery icon
    for (j=0; j<12; j++)  
    {
        for (k=0; k<32; k++)
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
				buffer = mypower[iconptr];
			}
        }
    }
    

    //2:filled battery depend on value
    for (j=0; j<10-2; j++)  
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

int osd_print_cross(int start_x, int start_y, unsigned char* bufptr,unsigned char *cross1,unsigned char *cross2)
{    
    int j, k;
    int x, y;
	int offset;

    unsigned int  iconptr = 0;
    unsigned char bitoffset = 0;
	unsigned char buffer;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

 	buffer = cross1[0];

    for (j=0; j<48; j++)
    {
        for (k=0; k<48; k++)  //icon width
        {
            x = start_x + k;
            y = start_y + j;

            offset = x + y*DISP_WIDTH;

			if( (buffer & 0x80)  == 0x80 ){
 				*(bufptr + offset) = 0xff;     //Y channel	
			}

			buffer <<= 1;
 			bitoffset ++;

            if(bitoffset == 8)
			{
				bitoffset = 0;
				iconptr++;
				buffer = cross2[iconptr];
			}
        }
    }
    return 1;
}
int osd_print_jindutiao(int start_x, int start_y, unsigned char* bufptr,char *myPic)
{    
    int j, k;
    int x, y;
	int offset;
    unsigned int  iconptr = 0;
    unsigned char bitoffset = 0;
	unsigned char buffer;
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;
	

 	buffer = myPic[0];
    for (j=0; j< 24; j++)
    {
        for (k = 0; k< 250; k++)  //icon width
        {
            x = start_x + k;
            y = start_y + j;

            offset = x + y*DISP_WIDTH;
			if( (buffer & 0x80)  == 0x80 )
 					*(bufptr + offset) = 0xff; 
					
			buffer <<= 1;
 			bitoffset ++;

            if(bitoffset == 8)
			{
				bitoffset = 0;
				iconptr++;
				buffer = myPic[iconptr];
			}
        }
    }
    return 1;
}

int osd_print_jindutiaoRect(int width,int height,int start_x, int start_y, int clock,unsigned char* bufptr)
{
    int offset;

    int i, j, k;
    int x, y;
    // Update OSD memory
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

    for (j = 0; j < height; j++)
    {
        for (k = 0; k < width; k++)
        {
			x= start_x + k;

            y= start_y + j;

            offset = x + y*SCREEN_WIDTH;//y*vScreenInfo.xres;
			if(clock == 1){
					*(bufptr + offset) = 150; 
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 60;
					*(bufptr + uvstart + vvoffset) = 100;//128;//200; 	//set VV 
           	
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

int osd_print_hollow_Rect(int width,int height,int start_x, int start_y, int clock,unsigned char* bufptr)
{
    int offset;

    int i, j, k;
    int x, y;
    // Update OSD memory
	int uuoffset,vvoffset;
	int uvstart = DISP_WIDTH*DISP_HEIGHT;

    for (j = 0; j < height; j++)
    {
       		 for (k = 0; k < width; k++){
				x= start_x + k;
         		y= start_y + j;
           		offset = x + y*SCREEN_WIDTH;//y*vScreenInfo.xres;
           		if(j == 0  || j == height-1){
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
				}else if(k == 0 || k == width - 1){
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
			
	}
	return 1;
}

 
