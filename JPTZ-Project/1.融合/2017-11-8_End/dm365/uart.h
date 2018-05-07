#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <memory.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <linux/unistd.h>
#include <linux/fb.h>

#define GPIO_BASE_ADDR         0x01c67000

#define GPIO_DIR01_OFFSET      0x10
#define GPIO_DIR23_OFFSET      0x38

#define SYS_CTRL_BASE_ADDR     0x01c40000

#define PINMUX3_OFFSET         0x0c
#define PINMUX4_OFFSET         0x10

//extern int uart1_pinmux_set(void);
//extern int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
//extern int open_port(int fd,int comport);

int uart1_pinmux_set(void)
{
    void *map_base = NULL;

    int fd;
    int reg_val;

    fd = open("/dev/mem", O_RDWR | O_SYNC);

    if (-1 == fd)
    {
        printf("open device failed!\n");
        return -1;
    }

    map_base = mmap(0, 0xff, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE_ADDR);

    if (NULL == map_base)
    {
        printf("mmap error!\n");
        close(fd);
        return -1;
    }

    reg_val = *(volatile unsigned int *)(map_base + GPIO_DIR01_OFFSET);
    printf("GPIO_DIR01_OFFSET = %08X\n", reg_val);
    *(volatile unsigned int *)(map_base + GPIO_DIR01_OFFSET) &= 0xfdffffff;
    reg_val = *(volatile unsigned int *)(map_base + GPIO_DIR01_OFFSET);
    printf("GPIO_DIR01_OFFSET = %08X\n", reg_val);

    reg_val = *(volatile unsigned int *)(map_base + GPIO_DIR23_OFFSET);
    printf("GPIO_DIR23_OFFSET = %08X\n", reg_val);
    *(volatile unsigned int *)(map_base + GPIO_DIR23_OFFSET) |= 0x00000004;
    reg_val = *(volatile unsigned int *)(map_base + GPIO_DIR23_OFFSET);
    printf("GPIO_DIR23_OFFSET = %08X\n", reg_val);

    munmap(map_base, 0xff);

    map_base = mmap(0, 0xff, PROT_READ | PROT_WRITE, MAP_SHARED, fd, SYS_CTRL_BASE_ADDR);

    if (NULL == map_base)
    {
        printf("mmap error!\n");
        close(fd);
        return -1;
    }

    reg_val = *(volatile unsigned int *)(map_base + PINMUX3_OFFSET);
    printf("PINMUX3_OFFSET = %08X\n", reg_val);
    *(volatile unsigned int *)(map_base + PINMUX3_OFFSET) |= 0x60000000;
    reg_val = *(volatile unsigned int *)(map_base + PINMUX3_OFFSET);
    printf("PINMUX3_OFFSET = %08X\n", reg_val);

    reg_val = *(volatile unsigned int *)(map_base + PINMUX4_OFFSET);
    printf("PINMUX4_OFFSET = %08X\n", reg_val);
    *(volatile unsigned int *)(map_base + PINMUX4_OFFSET) |= 0x0000c000;
    reg_val = *(volatile unsigned int *)(map_base + PINMUX4_OFFSET);
    printf("PINMUX4_OFFSET = %08X\n", reg_val);

    munmap(map_base, 0xff);

    close(fd);

    return 0;
}

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;

    if  ( tcgetattr( fd,&oldtio)  !=  0) {
        perror("SetupSerial 1");
        return -1;
    }

    bzero( &newtio, sizeof( newtio ) );
    newtio.c_cflag  |=  CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch( nBits )
    {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
    }

    switch( nEvent )
    {
        case 'O':
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N':
            newtio.c_cflag &= ~PARENB;
            break;
    }

    switch( nSpeed )
    {
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
    }

    if( nStop == 1 )
        newtio.c_cflag &=  ~CSTOPB;
    else if ( nStop == 2 )
    newtio.c_cflag |=  CSTOPB;

    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH);

    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return -1;
    }

    printf("set done!\n");

    return 0;
}

int open_port(int fd,int comport)
{
    char *dev[]={"/dev/ttyS0", "/dev/ttyS1", "/dev/ttyS2"};

    long  vdisable;

    if (comport==1)
    {   fd = open( "/dev/ttyS0", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd){
            perror("Can't Open Serial Port");
            return(-1);
        }
        else
            printf("open ttyS0 .....\n");
    }
    else if(comport==2)
    {   fd = open( "/dev/ttyS1", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd){
            perror("Can't Open Serial Port");
            return(-1);
        }
        else
            printf("open ttyS1 .....\n");
    }
    else if (comport==3)
    {
        fd = open( "/dev/ttyS2", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd){
            perror("Can't Open Serial Port");
            return(-1);
        }
        else
            printf("open ttyS2 .....\n");
    }

    if(fcntl(fd, F_SETFL, 0)<0)
        printf("fcntl failed!\n");
    else
        printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));

    if(isatty(STDIN_FILENO)==0)
        printf("standard input is not a terminal device\n");
    else
        printf("isatty success!\n");

    printf("fd-open=%d\n",fd);

    return fd;
}


int uart_init()
{
    int fd;
    unsigned char rx_buffer[200];
    int i, nread;


    uart1_pinmux_set();

    if((fd = open_port(fd, 2))<0)
	{
        printf("open_port error\n");
        return 0;
    }

    if((i=set_opt(fd, 230400, 8, 'N', 1))<0)
	{
        printf("set_opt error");
        close(fd);
        return 0;
    }

//	write(fd, "uart1 opened\n", 10);

	return fd;
/*
    while(1)
    {    
        nread=read(fd, rx_buffer, 200);
        if (nread>0)
        {
            //init_osd();
            //disp_str(rx_buffer, nread, 60, 60);
            rx_buffer[nread] = '\0';
            printf("%s\n", rx_buffer);
        }
		//else
			//printf("read buffer error!\n");
        usleep(100000);
    }

    close(fd);
    return 1;
*/
}
