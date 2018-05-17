
#include "keydel.h"
#include <QDebug>
#include <QtCore/qmath.h>

unsigned char g_SendSTM[6] = {0xee,0x03,0x00,0x00,0x00,0xff};

/********************************************
 抛出串口线程
 ********************************************/
KeyDel::KeyDel()
{
    RockerPressFlag = 0;
    MemLastRockerPos_X = 0;
    MemLastRockerPos_Y = 0;
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
     //   CapacityDelKeyDate(7);
        CapacityDelKeyDate(9);
        CapacityDelKeyDate(10);
        /*
         CapacityDelKeyDate(8);
        CapacityDelKeyDate(11);
        */
        CapacityDelTouch(g_iKeyDate.TouchDate_X,g_iKeyDate.TouchDate_Y);

        iUsart->UsartSend(g_SendSTM,6);
        if(g_SendSTM[4]) g_SendSTM[4] = 0;
     // printf("g_SendSTM:[2]:0x%x,g_SendSTM:[3]:0x%x\n\n\n",g_SendSTM[2],g_SendSTM[3]);
        //  fflush(NULL);
        // printf("g_SendSTM:[0]:%d  [1]:%d  [2]:%d  [3]:%d  [4]:%d  [5]:%d \n",\
        // g_SendSTM[0],g_SendSTM[1],g_SendSTM[2],g_SendSTM[3],g_SendSTM[4],g_SendSTM[5]);
        usleep(50000);
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
/********************************************
触摸屏判断
 ********************************************/

void KeyDel::PeripheryCircle( float *x_tmp,float *y_tmp){
    float tmp_angle;

    if((((*x_tmp)*(*x_tmp)+(*y_tmp)*(*y_tmp))<=R_CIRCLE_OUT*R_CIRCLE_OUT)&&(((*x_tmp)*(*x_tmp)+(*y_tmp)*(*y_tmp))>R_CIRCLE_IN*R_CIRCLE_IN))
    {
        if(((*x_tmp)>0)&&((*y_tmp)>0))
        {
            tmp_angle=asin((*y_tmp)/sqrt((*x_tmp)*(*x_tmp)+(*y_tmp)*(*y_tmp)));
            (*y_tmp)=sin(tmp_angle)*R_CIRCLE_IN;
            (*x_tmp)=cos(tmp_angle)*R_CIRCLE_IN;

        }else if(((*x_tmp)<0)&&((*y_tmp)>0))
        {
            tmp_angle=asin(((*y_tmp)/sqrt((*x_tmp)*(*x_tmp)+(*y_tmp)*(*y_tmp))));
            (*y_tmp)=sin(tmp_angle)*R_CIRCLE_IN;
            (*x_tmp)=(-1)*cos(tmp_angle)*R_CIRCLE_IN;
        }else if(((*x_tmp)<0)&&((*y_tmp)<0))
        {
            tmp_angle=asin((double)((-1)*(*y_tmp)/sqrt((*x_tmp)*(*x_tmp)+(*y_tmp)*(*y_tmp))));
            (*y_tmp)=(-1)*sin(tmp_angle)*R_CIRCLE_IN;
            (*x_tmp)=(-1)*cos(tmp_angle)*R_CIRCLE_IN;
        }else if(((*x_tmp)>0)&&((*y_tmp)<0))
        {
            tmp_angle=asin((double)((-1)*(*y_tmp)/sqrt((*x_tmp)*(*x_tmp)+(*y_tmp)*(*y_tmp))));
            (*y_tmp)=sin(tmp_angle)*R_CIRCLE_IN*(-1);
            (*x_tmp)=cos(tmp_angle)*R_CIRCLE_IN;
        }
    }
}

void KeyDel::CapacityDelTouch(int iX,int iY){
    /*
    * 触屏有8个区域
    * 键盘区域：上、下、左、右、确认、 控制区左 左下、右下  控制区右 左下、右下
    *           0  1  2   3   4           5   6             7     8
    * 摇杆区域：
    * 如果iX\iY匹配到区域就发出信号：
    *   1）在按键区域经过判断则带回按键的键值，int SelectTouchCoord(int x,int y);
    *   2）在摇杆区域则带回坐标 Signal(iX,iY);
    */
   // qDebug() <<"iX" << iX <<"iY"<< iY;
      /*按键区域*/
  //  printf("iX:%d  iY:%d\n",g_iKeyDate.TouchDate_X,g_iKeyDate.TouchDate_Y);

   if(iX > 90 && iX < 177 && iY > 89 && iY <144){
       if(iButtonMode == BUTTONFREE){
            emit TouchButtonMsg(BUTTONUPPRESS);
            iButtonMode = BUTTONUPPRESS;
       }
    }else if(iButtonMode == BUTTONUPPRESS){
       emit TouchButtonMsg(BUTTONUPRELEASE);
       iButtonMode = BUTTONFREE;
    }

    if(iX > 27 && iX < 84 && iY > 149 && iY <242){
        if(iButtonMode == BUTTONFREE){
             emit TouchButtonMsg(BUTTONLEFTPRESS);
             iButtonMode = BUTTONLEFTPRESS;
        }
    }else if(iButtonMode == BUTTONLEFTPRESS){
        emit TouchButtonMsg(BUTTONLEFTRELEASE);
        iButtonMode = BUTTONFREE;
    }
    if(iX > 103 && iX < 163 && iY > 166 && iY < 233){
        if(iButtonMode == BUTTONFREE){
             emit TouchButtonMsg(BUTTONENTERPRESS);
             iButtonMode = BUTTONENTERPRESS;
        }
    }else if(iButtonMode == BUTTONENTERPRESS){
        emit TouchButtonMsg(BUTTONENTERRELEASE);
        iButtonMode = BUTTONFREE;
    }

    if(iX > 180 && iX < 239 && iY > 149 && iY < 234){
        if(iButtonMode == BUTTONFREE){
             emit TouchButtonMsg(BUTTONRIGHTFPRESS);
             iButtonMode = BUTTONRIGHTFPRESS;
        }
    }else if(iButtonMode == BUTTONRIGHTFPRESS){
        emit TouchButtonMsg(BUTTONRIGHTRELEASE);
        iButtonMode = BUTTONFREE;
    }

    if(iX > 90 && iX < 179 && iY > 242 && iY < 299){
        if(iButtonMode == BUTTONFREE){
             emit TouchButtonMsg(BUTTONDOWNPRESS);
             iButtonMode = BUTTONDOWNPRESS;
        }
    }else if(iButtonMode == BUTTONDOWNPRESS){
        emit TouchButtonMsg(BUTTONDOWNRELEASE);
        iButtonMode = BUTTONFREE;
    }

    if(iX > 45 && iX < 95 && iY > 333 && iY < 381){
        if(iButtonMode == BUTTONFREE){
             emit TouchButtonMsg(BUTTONLDLPRESS);
             iButtonMode = BUTTONLDLPRESS;
        }
        iCountTime++;
    }else if(iButtonMode == BUTTONLDLPRESS){
        if(iCountTime < 10)
            emit TouchButtonMsg(BUTTONLDLRELEASE);
        else
            emit TouchButtonMsg(BUTTONLDLRELEASEL);
        iCountTime = 0;
        iButtonMode = BUTTONFREE;
    }

    if(iX > 176 && iX < 227 && iY > 333 && iY < 381){
        if(iButtonMode == BUTTONFREE){
             emit TouchButtonMsg(BUTTONLDRPRESS);
             iButtonMode = BUTTONLDRPRESS;
        }
    }else if(iButtonMode == BUTTONLDRPRESS){
        emit TouchButtonMsg(BUTTONLDRRELEASE);
        iButtonMode = BUTTONFREE;
    }

    if(iX > 1034 && iX < 1083 && iY > 346 && iY < 395){
        if(iButtonMode == BUTTONFREE){
             emit TouchButtonMsg(BUTTONRDLPRESS);
             iButtonMode = BUTTONRDLPRESS;
        }
    }else if(iButtonMode == BUTTONRDLPRESS){
        emit TouchButtonMsg(BUTTONRDLRELEASE);
        iButtonMode = BUTTONFREE;
    }

    if(iX > 1164 && iX < 1211 && iY > 346 && iY < 395){
        if(iButtonMode == BUTTONFREE){
             emit TouchButtonMsg(BUTTONRDRPRESS);
             iButtonMode = BUTTONRDRPRESS;
        }
    }else if(iButtonMode == BUTTONRDRPRESS){
        emit TouchButtonMsg(BUTTONRDRRELEASE);
        iButtonMode = BUTTONFREE;
    }
   if((g_SendSTM[3]&0x80)){
        /*判断触摸点是否在摇杆区域*/
        float RockerTmp_X,RockerTmp_Y;
        RockerTmp_X = iX - MIXROCKLAB_X; //(iX-MAXROCKCENTER_X)*(iX-MAXROCKCENTER_X);
        RockerTmp_Y = iY - MIXROCKLAB_Y; //(iY-MAXROCKCENTER_Y)*(iY-MAXROCKCENTER_Y);
        printf("RockerTmp_X:%f,RockerTmp_Y:%f \n\n",RockerTmp_X,RockerTmp_Y);
        if(RockerTmp_X * RockerTmp_X + RockerTmp_Y*RockerTmp_Y < 130*130 || RockerTmp_X*RockerTmp_X + RockerTmp_Y*RockerTmp_Y == 130*130)
        {
           // printf("--------------------------------\n\n");
            PeripheryCircle( &RockerTmp_X,&RockerTmp_Y);
            RockerPressFlag = 1;
            if(MemLastRockerPos_X != RockerTmp_X || \
                    MemLastRockerPos_Y != RockerTmp_Y)
                emit RockerSgl(RockerTmp_X,RockerTmp_Y,RockerPressFlag);
            MemLastRockerPos_X = RockerTmp_X;
            MemLastRockerPos_Y = RockerTmp_Y;
        }else{
            if(RockerPressFlag){
                RockerPressFlag = 0;
                emit RockerSgl(0,0,RockerPressFlag);
            }
        }
    }
}



