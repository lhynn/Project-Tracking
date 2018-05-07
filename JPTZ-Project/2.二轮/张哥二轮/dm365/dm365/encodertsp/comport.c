#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <termios.h>

#define R_BUF_LEN (1024)

int speed_arr[] = {B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300};
int name_arr[]  = {115200,  38400,  19200,  9600,  4800,  2400,  1200,  300};

int status;

void request_send(int fd)
{
    ioctl(fd, TIOCMGET, &status);
    status &= ~TIOCM_RTS;
    ioctl(fd, TIOCMSET, &status);
}

void clear_send(int fd)
{
    ioctl(fd, TIOCMGET, &status);
    status |= TIOCM_RTS;
    ioctl(fd, TIOCMSET, &status);
}


void close_port(int fd)
{
    if(fd == 0)
        return;
    close(fd);
    fd = 0;
}

int set_parity(int fd, int databits, int stopbits, int parity)
{
    struct termios options;
    if  ( tcgetattr( fd,&options)  !=  0)
    {
        perror("SetupSerial 1");
        return(0);
    }

    options.c_cflag |= CLOCAL |CREAD ;
    options.c_cflag &= ~CSIZE;
    switch (databits)
    {
    case 7:
        options.c_cflag |= CS7;
        break;
    case 8:
        options.c_cflag |= CS8;
        break;
    default:
        fprintf(stderr,"Unsupported data size\n");
        return (0);
    }
    switch (parity)
    {
    case 'n':
    case 'N':
        options.c_cflag &= ~PARENB;
        options.c_iflag &= ~INPCK;
        break;
    case 'o':
    case 'O':
        options.c_cflag |= (PARODD | PARENB);
        options.c_iflag |= INPCK;
        break;
    case 'e':
    case 'E':
        options.c_cflag |= PARENB;
        options.c_cflag &= ~PARODD;
        options.c_iflag |= INPCK;
        break;
    case 'S':
    case 's':
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        break;
    default:
        fprintf(stderr,"Unsupported parity\n");
        return (0);
    }

    switch (stopbits)
    {
    case 1:
        options.c_cflag &= ~CSTOPB;
        break;
    case 2:
        options.c_cflag |= CSTOPB;
        break;
    default:
        fprintf(stderr,"Unsupported stop bits\n");
        return (0);
    }

    /*No hardware control*/
    options.c_cflag &= ~CRTSCTS;
    /*No software control*/
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    /*delay time set */

    //options.c_cflag|= IXON|IXOFF|IXANY;

    if(parity != 'n') options.c_iflag |= INPCK;
    options.c_cc[VTIME] = 2;  // 150;
    options.c_cc[VMIN] =  0;

//	options.c_cc[VTIME] = 0;
//	options.c_cc[VMIN] = 14;
    /*raw model*/
    options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag  &= ~OPOST;

    options.c_iflag &= ~(INLCR|IGNCR|ICRNL);
    options.c_iflag &= ~(ONLCR|OCRNL);

    options.c_oflag &= ~(INLCR|IGNCR|ICRNL);
    options.c_oflag &= ~(ONLCR|OCRNL);

    tcflush(fd,TCIFLUSH);
    if (tcsetattr(fd,TCSANOW,&options) != 0)
    {
        perror("SetupSerial 3");
        return (0);
    }

    return (1);
}



void set_speed(int fd, int speed)
{
	int i;
	int status;
    struct termios Opt;

    tcgetattr(fd, &Opt);

    for(i = 0; i < sizeof(speed_arr)/sizeof(int); i++)
    {
        if(speed == name_arr[i])
        {
            tcflush(fd, TCIOFLUSH);
            cfsetispeed(&Opt, speed_arr[i]);
            cfsetospeed(&Opt, speed_arr[i]);
            status = tcsetattr(fd, TCSANOW, &Opt);
            if(status != 0) perror("tcsetattr fd1");
            return;
        }
        tcflush(fd,TCIOFLUSH);
    }
}


int check_port_open(const char *dev, unsigned int baud)
{
    int fd = 0;

    fd = open(dev, O_RDWR);
    if(-1 == fd)
    {
        fd = 0;
        printf("Can't Open Serial Port!\n");
        return -1;
    }
    else
    {
        printf("dev=%s\r\n",dev);
    }

    set_speed(fd, baud);
    set_parity(fd, 8, 1, 'N');

//    fcntl(fd,F_SETFL,FNDELAY);

    return fd;
}



int open_gpio(const char *dev)
{
    int fd = 0;
    fd =  open(dev, O_RDWR);
    if(fd < 0)
    {
        return -1;
    }
    else
    {
		printf("DM36X GPIO OPEN OK!\r\n");
        return fd;
    }
}

