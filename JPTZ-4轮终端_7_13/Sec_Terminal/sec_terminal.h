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
#include <QFileDialog>

//#include "keydel.h"
//#include "searchdir.h"
#include <QByteArray>
extern "C" {
    #include <sys/vfs.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <dirent.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <unistd.h>
};
#include "fileviewer.h"
#include "keydel.h"
#include "messageserver.h"

#define LOADINGTIME 400
enum{
    CAMERCTLMODE =  0,
    LIGHTCTLMODE,
    ROBOTCTLMODE,
};

enum{
    MENUSTATEHIDE =  0,
    MENUSTATESHOW,
    MENUSTATESEC,
    MENUSTATSETING,
    MENUSTATPLAYING,
    MENUSTATLOOKPHOTO,
};


namespace Ui {
class Sec_Terminal;
}

class Sec_Terminal : public QWidget
{
    Q_OBJECT
    
public:
    explicit Sec_Terminal(QWidget *parent = 0);
    ~Sec_Terminal();
    void slotActionSave     (void);
    void IUpdateMode        (void);
    int  MenuOneShow        (void);
    void SecFileListDestory (void);
    void MenuKeyUP          (void);
    void MenuKeyDown        (void);
    void MenuKeyReturn      (void);
    void MenuKeyEnter       (void);
    void GetPhotoOrVideo    (int Type);
    int  IsEmptyFile        (char *file_name);
    void ButtonKey_1_Func   (void);
    void ButtonKey_2_Func   (void);
    void ButtonKey_3_Func   (void);
    void ButtonKey_4_Func   (void);
    void ButtonKey_5_Func   (void);
    void ButtonKey_11_Func  (int Type);

    int TerminalPowerMen;//电量标志位
    int LongPressFlag;//长恩标志位
    int FIFOCmd;
    int ICURRENTMODE;
private slots:
    void  timerDone(void);
    void  LoadingIrq(void);
    void  PhyButtonDeal(int Num,int Type);
    void  OsdUpdateFunc(int);
private:
    QLabel *RectGround;
    QLabel *LittleRectGround;
    QLabel *RectTitle;
    QLabel *VideoGround;

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
    QLabel *TerminalPowerNum;
    MessageServer msgServer;
    int          PauseFlag;
    /**************************参数显示**********************************************/
    QLabel  *DataShow;
    QLabel  *CarStateArgc;
    QLabel  *FuYangSt;
    QLabel  *HeGunSt;
    QLabel  *HangXiangSt;
    QLabel  *BaiBiSt;
    QLabel  *WeDuSt;
    QLabel  *SuDuSt;
    QLabel  *CarPower;
    QLabel  *CameraAngle;
    QLabel  *CarBodyState;
    QLabel  *DJCurrent;
    QLabel  *DJTemperature;


    QLabel  *SuDuDangWeiSt;
    QLabel  *FangDaLab;
    QLabel  *GuangMinLab;
    QLabel  *VideoView;

    QLabel  *CheTiCanShu;
    QLabel  *KongZiXinXi;

    QString BlowUpssStr;//放大字符串
    int BlowUpssFlag;//放大标志位
    int GuanMiZhiWei;
    int VideoViewFlag;
    /*****************************文字*******************************************/
    QLabel  *ModeStatePic;
    QLabel  *ModeStateChar;
    /****************************航向*******************************************/
    int HXpPointRoter;
    QGraphicsScene* HXpScene;
    QGraphicsView * HXpView;
    /****************************码表*******************************************/
    QGraphicsScene* MBpScene;
    QGraphicsView * MBpView;
    QLabel  *SpeedDescription ;
    float MemMBpView;
    /****************************灯光*******************************************/
    QLabel *LightIconQ;
    QLabel *LightIconH;
    QLabel *LightIconS;
    QLabel *LightIconY;

    QListWidget *MenuList;          //菜单
    int MenuListFlag;
    QLabel *MenuDes;

    int MenuStateFlag;
    FileViewer *fileview;
    char cmdbuf[128];

    QListWidget *SecMenuList;          //Sec菜单
    int          SeclistFlag;
    int          VideoOrPicFlag;
    int          FangDaOrGuangMi;

    QLabel *ShowPhoto;
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

    int CararFlag;              //挡位Flag
    Ui::Sec_Terminal *ui;

    /*---------------加载--------------*/
    QTimer *LoadingTimer;
    QLabel  *LoadingLal;
    int     LoadingTimerCount;
    int      LoadingLalFlag;
};

#endif // SEC_TERMINAL_H
