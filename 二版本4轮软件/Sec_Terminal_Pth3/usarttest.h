#ifndef USARTTEST_H
#define USARTTEST_H

#include <QThread>
#include <QObject>

#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termio.h>
#include <syslog.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <bits/signum.h>
#include <sys/resource.h>

#define BUFFSIZE  32
#define KEYNUM    16

typedef struct KeyDate{              //按键、触屏信息
    int  KeyVale[KEYNUM];
    int  KeyLPressFlags[KEYNUM];
   // int  TouchDate_X;
   // int  TouchDate_Y;
}T_KEYDATE;

typedef struct CarStateInfo{         //车体状态信息
    int CarPowerVal;
    int TerPowerVal;
    int Pitch;
    int Roll;
    int Dir;
    float CarSpeed;
    int BaiBiRot;
    int YuJingDengFlag;
    int QianDeng;
    int HouDeng;
    int ShangDeng;

    int CheTiFanZhuan;
    float HuanJingWenDu;
    int  CameraAngle;
    int  WaringFlag;
}T_CARSTATEINFO;

extern T_KEYDATE      g_iKeyDate;           //按键、触屏信息
extern T_CARSTATEINFO g_iCarStateInfo;      //车体状态信息

class UsartClassThr : public QThread
{
    Q_OBJECT
protected:
    void run();
public:
    unsigned char CarInformation[BUFFSIZE];
    //void GetUartKeyDate(T_KEYDATE iKeyDate);
    void UsartSend(unsigned char *SendData,int size);
    void close_fd(int fd);
private:
    int  ut_fd;
    int  uart_init(int arg,int baud);
};

#endif // USARTTEST_H
