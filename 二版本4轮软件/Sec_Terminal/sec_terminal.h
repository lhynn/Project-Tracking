#ifndef SEC_TERMINAL_H
#define SEC_TERMINAL_H

#include <QWidget>
#include <QLabel>
#include <QMatrix>
#include <QDebug>
#include <QString>
#include <QVBoxLayout>
#include <QListWidget>
#include <QFileInfoList>
#include <QMatrix>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDialog>
#include <QDir>
#include <QSlider>
#include <unistd.h>
#include <QPainter>
#include <QFrame>
#include <QTimer>
#include "messageserver.h"
//#include "keydel.h"
//#include "searchdir.h"
#include <QByteArray>
#include <sys/vfs.h>

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "keydel.h"

namespace Ui {
class Sec_Terminal;
}

class Sec_Terminal : public QWidget
{
    Q_OBJECT
    
public:
    explicit Sec_Terminal(QWidget *parent = 0);
    ~Sec_Terminal();
    void slotActionSave();

private slots:
    void  timerDone(void);
    void  PhyButtonDeal(int Num,int Type);
    void  OsdUpdateFunc(int);
private:
    QLabel *RectGround;
    QLabel *LittleRectGround;
    QLabel *RectTitle;
    QLabel *SpeedWatch;
    QLabel *SpeedPoint;
    QLabel *Direction;
    QLabel *DirPoint;


    QLabel *ShortKey_0;
    QLabel *ShortKey_1;
    QLabel *ShortKey_2;
    QLabel *ShortKey_3;
    QLabel *ShortKey_4;
    QLabel *ShortKey_5;

    QLabel *TerminalPower;
    MessageServer msgServer;
    /**************************参数显示**********************************************/
    QLabel  *CarStateArgc;
    QLabel  *FuYangSt;
    QLabel  *HeGunSt;
    QLabel  *HangXiangSt;
    QLabel  *BaiBiSt;
    QLabel  *WeDuSt;
    QLabel  *SuDuSt;
    QLabel  *SuDuDangWeiSt;
    QLabel  *CameraAngle;

    /*****************************文字*******************************************/
    QLabel  *FileType;
    QLabel  *CamerCtl;
    /****************************航向*******************************************/
    int HXpPointRoter;
    QGraphicsScene* HXpScene;
    QGraphicsView * HXpView;

    /****************************码表*******************************************/
    QGraphicsScene* MBpScene;
    QGraphicsView * MBpView;
    /****************************灯光*******************************************/
    QLabel *LightIconQ;
    QLabel *LightIconH;
    QLabel *LightIconS;
    QLabel *LightIconY;

    QListWidget *MenuList;          //菜单
    int MenuListFlag;
    /*---------------音频---------------*/
    int fd_gpio;
    unsigned char gpiobuf[5];
    int SetGPIO(int value); //write GPIO22
    void SET_AUDIO_BYPASS();
    void SET_AUDIO_368OUT();
  //  unsigned char DetectTFCard(void);

    /*---------------定时器--------------*/
    QTimer *timer;
    KeyDel *iButtonMsg;


    int tmp;
    Ui::Sec_Terminal *ui;
};

#endif // SEC_TERMINAL_H
