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
    ret = uart_init(1,0);
    if(ret < 0)
        printf("uart_init error.\n");
    while(1){
        ret = read(ut_fd,CarInformation,BUFFSIZE);
        if(ret < 0){
            printf("read Error.\n");
            return;
        }
         if(CarInformation[0] == 0xee && CarInformation[23] == 0xff){
            g_iKeyDate.KeyVale[8]  = (CarInformation[1] >> 0) & 0x03;
            g_iKeyDate.KeyVale[7]  = (CarInformation[1] >> 2) & 0x03;
            g_iKeyDate.KeyVale[0]  = (CarInformation[1] >> 4) & 0x03;
            g_iKeyDate.KeyVale[9]  = (CarInformation[1] >> 6) & 0x03;
            g_iKeyDate.KeyVale[6]  = (CarInformation[2] >> 0) & 0x03;
            g_iKeyDate.KeyVale[1]  = (CarInformation[2] >> 2) & 0x03;
            g_iKeyDate.KeyVale[2]  = (CarInformation[2] >> 4) & 0x03;
            g_iKeyDate.KeyVale[5]  = (CarInformation[2] >> 6) & 0x03;
            g_iKeyDate.KeyVale[11]  = (CarInformation[3] >> 0) & 0x03;
            g_iKeyDate.KeyVale[3]  = (CarInformation[3] >> 2) & 0x03;
            g_iKeyDate.KeyVale[4] = (CarInformation[3] >> 4) & 0x03;
            g_iKeyDate.KeyVale[10] = (CarInformation[3] >> 6) & 0x03;

            g_iKeyDate.KeyVale[12] = (CarInformation[4] >> 0) & 0x03;
            g_iKeyDate.KeyVale[13] = (CarInformation[4] >> 2) & 0x03;
            g_iKeyDate.KeyVale[14] = (CarInformation[4] >> 4) & 0x03;
            g_iKeyDate.KeyVale[15] = (CarInformation[4] >> 6) & 0x03;
       //     g_iKeyDate.TouchDate_X =(CarInformation[5] << 8) + CarInformation[4];
       //     g_iKeyDate.TouchDate_Y =(CarInformation[7] << 8) + CarInformation[6];

            g_iCarStateInfo.CarPowerVal = CarInformation[9];
            g_iCarStateInfo.TerPowerVal = CarInformation[10];
            if(CarInformation[11] & 0x80)
                g_iCarStateInfo.Pitch = (CarInformation[11] & 0x7F)*(-1);
            else
                g_iCarStateInfo.Pitch = (CarInformation[11] & 0x7F);

            if(CarInformation[13] & 0x80)
                g_iCarStateInfo.Roll = (((CarInformation[13] & 0x7f) << 8)  + CarInformation[12])*(-1);
            else
                g_iCarStateInfo.Roll = ((CarInformation[13] & 0x7f) << 8)  + CarInformation[12];

            g_iCarStateInfo.Dir = (CarInformation[15] << 8) + CarInformation[14];

            if(CarInformation[16] & 0x80)
                g_iCarStateInfo.CarSpeed = (((float)(CarInformation[16] & 0x0F)/10) + ((CarInformation[16] & 0x70) >> 4))*-1;
            else
                g_iCarStateInfo.CarSpeed = ((float)(CarInformation[16] & 0x0F)/10) + ((CarInformation[16] & 0x70) >> 4);

            g_iCarStateInfo.BaiBiRot = ((CarInformation[18] & 0x0F) << 8) + CarInformation[17];

            g_iCarStateInfo.YuJingDengFlag =CarInformation[18] & 0x80;
            g_iCarStateInfo.QianDeng =  CarInformation[18] & 0x40;
            g_iCarStateInfo.HouDeng =CarInformation[18] & 0x20;
            g_iCarStateInfo.ShangDeng =CarInformation[18] & 0x10;
            g_iCarStateInfo.CheTiFanZhuan =CarInformation[19] & 0x40;

            if(CarInformation[21] > 9) CarInformation[21] = 9;
            if(CarInformation[20] & 0x80)
                g_iCarStateInfo.HuanJingWenDu = (float)(CarInformation[20] & 0x7f)*-1 + ((float)CarInformation[21])/10*-1;
            else
                g_iCarStateInfo.HuanJingWenDu = (float)(CarInformation[20] & 0x7f)  + ((float)CarInformation[21])/10;

            if(CarInformation[22] & 0x80)
                g_iCarStateInfo.CameraAngle = (CarInformation[22] & 0x7f)*-1;
             else
                g_iCarStateInfo.CameraAngle = (CarInformation[22] & 0x7f);

               g_iCarStateInfo.WaringFlag = CarInformation[19] & 0x0f;


 #if 0
printf("[0]:0x%x [1]:0x%x  [2]:0x%x  [3]:0x%x  [4]:0x%x  [5]:0x%x [6]:0x%x \
        [7]:0x%x  [8]:0x%x  [9]:0x%x  [10]:0x%x [11]:0x%x [12]:0x%x \
        [13]:0x%x [14]:0x%x [15]:0x%x [16]:0x%x [17]:0x%x [18]:0x%x \
        [19]:0x%x [20]:0x%x [21]:0x%x [22]:0x%x [23]:0x%x\n",CarInformation[0],\
        CarInformation[1],CarInformation[2],CarInformation[3],CarInformation[4],CarInformation[5],
        CarInformation[6],CarInformation[7],CarInformation[8],CarInformation[9],CarInformation[10],
        CarInformation[11],CarInformation[12],CarInformation[13],CarInformation[14],CarInformation[15]\
        ,CarInformation[16],CarInformation[17],CarInformation[18],CarInformation[19],CarInformation[20],\
        CarInformation[21],CarInformation[22],CarInformation[23]);
 #endif

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
