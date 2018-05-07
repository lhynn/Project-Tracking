#include "../ctrl.h"
#include "../osdmanager.h"


int osd_print_string(unsigned char* str, int len, int start_x, int start_y,\
						   unsigned char* bufptr,unsigned short mine[][27])
{    
	int offset;

    int i,j, k;
    int x, y;

    unsigned char ch;
    unsigned short table;

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
            table = mine[ch][j];

            for (k = 0; k < CHAR_WIDTH; k++)
            {
                x = start_x + i*CHAR_WIDTH + k;
                y = start_y + j;

                offset = x + y*SCREEN_WIDTH;      //  y*vScreenInfo.xres;

                if((table>>k) & 0x01)
                {
                    *(bufptr + offset) = 0xff;
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
 					*(bufptr + offset) = 0xff; 
					uuoffset = (y/2)*DISP_WIDTH + (x/2)*2;
					vvoffset = (y/2)*DISP_WIDTH + (x/2)*2 + 1;
					*(bufptr + uvstart + uuoffset) = 0;
					*(bufptr + uvstart + vvoffset) = 0;//128;//200; 	//set VV 
				}
				 else{
					*(bufptr + offset) = clock; 
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


int osd_print_rect(unsigned char* str, int len, int start_x, int start_y, unsigned short color, unsigned char* bufptr)
{
    int offset;

    int i, j, k;
    int x, y;
    // Update OSD memory

    for (j = 0; j < 100; j++)
    {
        for (k = 0; k < 100; k++)
        {
			x= start_x + k;

            y= start_y + j;

            offset = x + y*SCREEN_WIDTH;//y*vScreenInfo.xres;

            *(bufptr + offset) = 0xff; 
        }
    }
	return 1;
}


