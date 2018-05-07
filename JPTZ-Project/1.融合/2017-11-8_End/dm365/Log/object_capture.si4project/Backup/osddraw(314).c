#include "../ctrl.h"
#include "../osdmanager.h"


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
                x = start_x + i*12 + k;
                y = start_y + j;
                offset = x + y*DISP_WIDTH;      //  y*vScreenInfo.xres;
				//printf("----->%d\n",offset);
                if((table<<k) & 0x80){
                 if(clock != 0xff && clock != 0){
 					*(bufptr + offset) = 152; 
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 56;
					*(bufptr + uvstart + vvoffset) = 20;//128;//200; 	//set VV 
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

			if( (buffer & 0x80)  == 0x80 )
            {
           		 if(clock != 0xff && clock != 0){
 					*(bufptr + offset) = 152; 
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 56;
					*(bufptr + uvstart + vvoffset) = 20;//128;//200; 	//set VV 
				 }
				 else{
					*(bufptr + offset) = clock; 
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 127;
					*(bufptr + uvstart + vvoffset) = 127;//128;//200; 	//set VV 
				 }
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

    for (j = 0; j < 25; j++)
    {
        for (k = 0; k < 80; k++)
        {
			x= start_x + k;

            y= start_y + j;

            offset = x + y*SCREEN_WIDTH;//y*vScreenInfo.xres;

            if(clock != 0xff && clock != 0){
 					*(bufptr + offset) = 152; 
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 56;
					*(bufptr + uvstart + vvoffset) = 20;//128;//200; 	//set VV 
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
	return 1;
}


