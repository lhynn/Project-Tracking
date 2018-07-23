#include "usarttest.h"

T_KEYDATE g_iKeyDate;
T_CARSTATEINFO g_iCarStateInfo;

/*****************************************************
            串口处理线程函数
*****************************************************/
/***************************************************/

//static int num;
/***************************************************/
void UsartClassThr::run(){
    int ret;
    ret = uart_init(1, 0);
    if(ret < 0)
        printf("uart_init error.\n");
    while(1){
        ret = read(ut_fd,CarInformation,BUFFSIZE);
        if(ret < 0){
            printf("read Error.\n");
            return;
        }
        if(CarInformation[0] == 0xee && CarInformation[22] == 0xff){
            g_iKeyDate.KeyVale[0]  = (CarInformation[1] >> 0) & 0x03;
            g_iKeyDate.KeyVale[1]  = (CarInformation[1] >> 2) & 0x03;
            g_iKeyDate.KeyVale[2]  = (CarInformation[1] >> 4) & 0x03;
            g_iKeyDate.KeyVale[3]  = (CarInformation[1] >> 6) & 0x03;
            g_iKeyDate.KeyVale[4]  = (CarInformation[2] >> 0) & 0x03;
            g_iKeyDate.KeyVale[5]  = (CarInformation[2] >> 2) & 0x03;
            g_iKeyDate.KeyVale[6]  = (CarInformation[2] >> 4) & 0x03;
        //   g_iKeyDate.KeyVale[6] = 1;
            g_iKeyDate.KeyVale[9]  = (CarInformation[2] >> 6) & 0x03;
            g_iKeyDate.KeyVale[10]  = (CarInformation[3] >> 0) & 0x03;
            g_iKeyDate.KeyVale[7]  = (CarInformation[3] >> 2) & 0x03;   //you xia Right
            g_iKeyDate.KeyVale[8] = (CarInformation[3] >> 4) & 0x03;   //you xia Left
            g_iKeyDate.KeyVale[11] = (CarInformation[3] >> 6) & 0x03;
         //   printf("CarInformation[0]:%d  CarInformation[1]:%d  \n",g_iKeyDate.KeyVale[0], g_iKeyDate.KeyVale[1] );
            g_iKeyDate.TouchDate_X =(CarInformation[5] << 8) + CarInformation[4];
            g_iKeyDate.TouchDate_Y =(CarInformation[7] << 8) + CarInformation[6];

            g_iCarStateInfo.CarPowerVal = CarInformation[8];
            g_iCarStateInfo.TerPowerVal = CarInformation[9];
            if(CarInformation[10] & 0x80)
                g_iCarStateInfo.Pitch = (CarInformation[10] & 0x7F)*(-1);
            else
                g_iCarStateInfo.Pitch = (CarInformation[10] & 0x7F);

            if(CarInformation[12] & 0x80)
                g_iCarStateInfo.Roll = (((CarInformation[12] & 0x7f) << 8)  + CarInformation[11])*(-1);
            else
                g_iCarStateInfo.Roll = ((CarInformation[12] & 0x7f) << 8)  + CarInformation[11];

            g_iCarStateInfo.Dir = (CarInformation[14] << 8) + CarInformation[13];

            if(CarInformation[15] & 0x80)
                g_iCarStateInfo.CarSpeed = (((float)(CarInformation[15] & 0x0F)/10) + ((CarInformation[15] & 0x70) >> 4))*-1;
            else
                g_iCarStateInfo.CarSpeed = ((float)(CarInformation[15] & 0x0F)/10) + ((CarInformation[15] & 0x70) >> 4);

            g_iCarStateInfo.BaiBiRot = ((CarInformation[17] & 0x0F) << 8) + CarInformation[16];
          //  num += 10;
         //   if(num > 360) num = 0;
         //   g_iCarStateInfo.BaiBiRot = num;
            g_iCarStateInfo.YuJingDengFlag =CarInformation[17] & 0x80;
            g_iCarStateInfo.QianDeng =  CarInformation[17] & 0x40;
            g_iCarStateInfo.HouDeng =CarInformation[17] & 0x20;
            g_iCarStateInfo.ShangDeng =CarInformation[17] & 0x10;
            g_iCarStateInfo.CheTiFanZhuan =CarInformation[18] & 0x40;

            if(CarInformation[20] > 9) CarInformation[20] = 9;
            if(CarInformation[19] & 0x80)
                g_iCarStateInfo.HuanJingWenDu = (float)(CarInformation[19] & 0x7f)*-1 + ((float)CarInformation[20])/10*-1;
            else
                g_iCarStateInfo.HuanJingWenDu = (float)(CarInformation[19] & 0x7f)  + ((float)CarInformation[20])/10;

            if(CarInformation[21] & 0x80)
                g_iCarStateInfo.CameraAngle = (CarInformation[21] & 0x7f)*-1;
             else
                g_iCarStateInfo.CameraAngle = (CarInformation[21] & 0x7f);
//printf("[18]:0x%x ",CarInformation[18]);
             //CarInformation[18] = 0x0f;

               g_iCarStateInfo.WaringFlag = CarInformation[18] & 0x0f;

           // g_iCarStateInfo.Pitch = -60;
          //  g_iCarStateInfo.Roll  = -60;
          //  printf("CameraAngle :%d  CarSpeed:%f\n",g_iCarStateInfo.CameraAngle,g_iCarStateInfo.CarSpeed);

        //       printf("g_iKeyDate.KeyVale[1]:%d\n",g_iKeyDate.KeyVale[1]);
 #if 0
            printf("[0]:0x%x ",CarInformation[0]);
            printf("[1]:0x%x ",CarInformation[1]);
            printf("[2]:0x%x ",CarInformation[2]);
            printf("[3]:0x%x ",CarInformation[3]);
            printf("[4]:0x%x ",CarInformation[4]);
            printf("[5]:0x%x ",CarInformation[5]);
            printf("[6]:0x%x ",CarInformation[6]);
            printf("[7]:0x%x ",CarInformation[7]);
 #endif
        //    printf("[8]: %d ",CarInformation[8]);
       //     printf("[9]: %d\n",CarInformation[9]);
 #if 0
            printf("[10]:0x%x ",CarInformation[10]);
            printf("[11]:0x%x ",CarInformation[11]);
            printf("[12]:0x%x ",CarInformation[12]);
            printf("[13]:0x%x ",CarInformation[13]);
            printf("[14]:0x%x ",CarInformation[14]);
            printf("[15]:0x%x ",CarInformation[15]);
            printf("[16]:0x%x ",CarInformation[16]);
            printf("[17]:0x%x ",CarInformation[17]);
            printf("[18]:0x%x ",CarInformation[18]);
            printf("[19]:0x%x ",CarInformation[19]);
            printf("[20]:0x%x ",CarInformation[20]);
            printf("[21]:0x%x ",CarInformation[21]);
            printf("[22]:0x%x\n",CarInformation[22]);
  #endif
         //   printf("[11]:0x%x ",CarInformation[11]);
           // printf("[12]:0x%x \n\n\n",CarInformation[12]);
     //  sleep(1);
        }
    }
}
/*****************************************************
   串口初始化
*****************************************************/
int UsartClassThr::uart_init(int arg, int baud)
{
    char port[20];
    struct termios Opt;
    int uartbiit[50]= {B115200,B9600,B19200,B4800,B2400,B1200};
    sprintf(port,"/dev/ttyS%d",arg);
    printf("Use port: %s \n", port);
    ut_fd = open(port, O_RDWR);
    if (ut_fd < 0){
        printf("open error.\n");
        return -1;
    }
    tcgetattr(ut_fd,&Opt);
    tcflush(ut_fd,TCIFLUSH);
    cfsetispeed(&Opt,uartbiit[baud]);
    cfsetospeed(&Opt,uartbiit[baud]);
    Opt.c_cflag |= CS8;
    Opt.c_cflag &= ~PARENB;
    Opt.c_oflag &= ~(OPOST);
    Opt.c_cflag &= ~CSTOPB;
    Opt.c_lflag &= ~(ICANON|ISIG|ECHO|IEXTEN);
    Opt.c_iflag &= ~(INPCK|BRKINT|ICRNL|ISTRIP|IXON);
    Opt.c_cc[VMIN] = 21;
    Opt.c_cc[VTIME] = 1;
    if (tcsetattr(ut_fd,TCSANOW,&Opt) != 0){
        perror("SetupSerial!\n");
        close(ut_fd);
        return -1;
    }
    return 0;
}
/*****************************************************
   串口发送
*****************************************************/
void UsartClassThr::UsartSend(unsigned char *SendData,int size){
    int ret;
    do{
        ret = write(ut_fd, SendData,size);
        if(ret < 0){
            printf("UsartSend Error.\n");
            return;
        }
    }while(ret != size);
}

/*****************************************************
   串口关闭
*****************************************************/
void UsartClassThr::close_fd(int fd){
    if(fd == 0)
        return;
    close(fd);
    fd = 0;
}
