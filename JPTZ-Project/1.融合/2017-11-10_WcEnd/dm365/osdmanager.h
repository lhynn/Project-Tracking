/*
	Osdmanager.h
	Programmed by Cuilei May 20, 2014
*/
#ifndef _OSDMANAGER_H_
#define _OSDMANAGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>


#include <linux/videodev2.h>

#define MAX_TRANSPARENCY            0x77
#define MIN_TRANSPARENCY            0x00

#define DEFAULT_BACKGROUND          0x18e3
#define ACTIVE_BACKGROUND           0x630c
#define DEFAULT_FOREGROUND          0xffff

#define   CHAR_WIDTH          16
#define   CHAR_HEIGTH         27

#define   MAX_CHAR            0X7E
#define   MIN_CHAR            0X20

#define   UNKNOWN_CHAR        0X7F


int osd_setup(char* fbdev, int width, int height);
//int osd_print_string(unsigned char* str, int len, int start_x, int start_y, unsigned short color);
int osd_init();
int osd_showBmp(char* filename, int start_x, int start_y, int width, int height);

#endif
