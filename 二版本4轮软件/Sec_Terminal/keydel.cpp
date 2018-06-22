
#include "keydel.h"
#include <QDebug>
#include <QtCore/qmath.h>

unsigned char g_SendSTM[6] = {0xee,0x03,0x00,0x00,0x00,0xff};

/********************************************
 抛出串口线程
 ********************************************/
KeyDel::KeyDel()
{
    PhyTouchButtonFlag = -1;
    DelayUpdateOsdFlag = 0;
    iButtonMode = BUTTONFREE;
    iUsart = new UsartClassThr;
    iUsart->setPriority(QThread::InheritPriority);
    iUsart->start();
    iCountTime = 0;
    MenButton = 2;
}
/********************************************
按键处理线程
 ********************************************/
void KeyDel::run(){
    //CapacityDelKeyDate
    int i;
    for(i = 0;i < KEYNUM;i++)
        g_iKeyDate.KeyVale[i] = RELEASE;

    while(1){
        CapacityDelKeyDate(0);//传入按键处理
        CapacityDelKeyDate(1);
        CapacityDelKeyDate(2);
        CapacityDelKeyDate(3);
        CapacityDelKeyDate(4);
        CapacityDelKeyDate(5);
        CapacityDelKeyDate(6);
        CapacityDelKeyDate(7);
        CapacityDelKeyDate(8);
        CapacityDelKeyDate(9);
        CapacityDelKeyDate(10);
        CapacityDelKeyDate(11);
               /*
         CapacityDelKeyDate(8);
         CapacityDelKeyDate(11);
        */
        iUsart->UsartSend(g_SendSTM,6);
        if(g_SendSTM[4]) g_SendSTM[4] = 0;
        usleep(80000);
        DelayUpdateOsdFlag++;
        if(DelayUpdateOsdFlag == 3){
            emit OsdUpdate(PhyTouchButtonFlag);
            PhyTouchButtonFlag = -1;
            DelayUpdateOsdFlag = 0;
        }
    }
}
/********************************************
输入按键检测
 ********************************************/
void KeyDel::CapacityDelKeyDate(int Num){

    switch (g_iKeyDate.KeyVale[Num]){
    case RELEASE:
            break;
    case SHORTPRESS:
      //  ShortDelFun();
        if(PhyTouchButtonFlag < 0){
            emit PhyButtonMsg(Num,0);                         //发送按键信号并带回按键值
            PhyTouchButtonFlag = Num;
        }
        g_iKeyDate.KeyVale[Num] = 0;
        break;
    case LONGPRESS:
      //  LongDelFun();
        if(PhyTouchButtonFlag < 0){
            emit PhyButtonMsg(Num,1);
            PhyTouchButtonFlag = Num;
        }
        g_iKeyDate.KeyVale[Num] = 0;
        break;
    default:
        break;
    }
}
