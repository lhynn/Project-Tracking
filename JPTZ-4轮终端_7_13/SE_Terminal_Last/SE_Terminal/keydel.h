#ifndef KEYDEL_H
#define KEYDEL_H
#include <QThread>
#include <QObject>
#include "usarttest.h"

#define BUTTONFREE            0
#define BUTTONUPPRESS         1
#define BUTTONUPRELEASE       2
#define BUTTONLEFTPRESS       3
#define BUTTONLEFTRELEASE     4
#define BUTTONENTERPRESS      5
#define BUTTONENTERRELEASE    6
#define BUTTONRIGHTFPRESS     7
#define BUTTONRIGHTRELEASE    8
#define BUTTONDOWNPRESS       9
#define BUTTONDOWNRELEASE     10

#define BUTTONLDLPRESS        11
#define BUTTONLDLRELEASE      12
#define BUTTONLDLRELEASEL      19

#define BUTTONLDRPRESS        13
#define BUTTONLDRRELEASE      14

#define BUTTONRDLPRESS        15
#define BUTTONRDLRELEASE      16
#define BUTTONRDRPRESS        17
#define BUTTONRDRRELEASE      18


#define RELEASE         0
#define SHORTPRESS      1
#define LONGPRESS       2


#define MAXROCKLAB_X            1020
#define MAXROCKLAB_Y            107

#define MIXROCKLAB_X            1120
#define MIXROCKLAB_Y            207


#define R_CIRCLE_OUT            130
#define R_CIRCLE_IN             100

extern unsigned char g_SendSTM[6];        //按键、触屏信息

class KeyDel : public QThread
{
    Q_OBJECT
protected:
   void run();
signals:
   void PhyButtonMsg(int,int);
   void TouchButtonMsg(int);
   void RockerSgl(float,float,int);
   void OsdUpdate(int);
public:
   KeyDel();
   void CapacityDelKeyDate(int Num);
   void CapacityDelTouch(int iX,int iY);
   void PeripheryCircle(float*,float *y_tmp);
private:
   int RockerPressFlag;
   UsartClassThr *iUsart;
   int MemLastRockerPos_X;
   int MemLastRockerPos_Y;
   int PhyTouchButtonFlag;
   int DelayUpdateOsdFlag;
   int iButtonMode;

   int iCountTime;
   int MenButton;
};

#endif // KEYDEL_H
