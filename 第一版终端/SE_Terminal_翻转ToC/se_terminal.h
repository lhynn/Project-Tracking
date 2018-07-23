#ifndef SE_TERMINAL_H
#define SE_TERMINAL_H

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
#include "keydel.h"
#include "searchdir.h"
#include <QByteArray>
#include <sys/vfs.h>

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
extern "C" {
    #include <unistd.h>
}

/*ButtonBroad current state*/
#define    CAMERCTLMODE    0
#define    LIGHTCTLMODE    1
#define    ROBOTCTLMODE    2
#define    MENUCTLMODE     3

/*MENU current state*/
#define    MENUSTATEHIDE   0
#define    MENUSTATESHOW   1
#define    MENUSTATESEC    2


#define WIDTH_3DBOX   222//240//221
#define HEIGHT_3DBOX  232//200//231
#define IMAGE_TYPE  0
#define AVI_TYPE    1
namespace Ui {
    class se_terminal;
}

typedef  void (*TouchCallBack)(void);
class se_terminal : public QWidget
{
    Q_OBJECT
public:
    explicit se_terminal(QWidget *parent = 0);
    ~se_terminal();
    void UpdateDateTime(int year,int month,int day,int H,int M,int S);
    static  void QdebugCallBack1(void);
    static void QdebugCallBack2(void);
    int SearchDirFile(char *fileType);
    void iWriteToFile(int AviNum,int BmpNum);
    void i3DSelectApproximateRote(int,int,int *,int *);

private slots:
    void  TouchButtonDeal(int x);
    void  PhyButtonDeal(int Num,int Type);
    void  TouchRockerDeal(float x,float y,int press);
    void  OsdUpdateFunc(int);
    void  timerDone(void);
    void  timerDecodeTimer(void);
    void  timerFanZhuan(void);

private:
    QLabel *RectGround;
    /*7 Button*/
    QLabel *ButtonLeft;
    QLabel *ButtonRight;
    QLabel *ButtonUp;
    QLabel *ButtonDown;
    QLabel *ButtonEnter;
    QLabel *ButtonDownLeft;
    QLabel *ButtonDownRight;
    QLabel *ButtonRightDownLeft;
    QLabel *ButtonRightDownRight;
    QLabel *ZTCT;
    /*8 TileIcon*/
    QLabel *SignalNow;
    QLabel *WIFISignal;
    QLabel *LightIcon;
    QLabel *LightIconQ;
    QLabel *LightIconS;
    QLabel *LightIconY;
    QLabel *Photo;
    QLabel *Rec;
    QLabel *gps;
    QLabel *LuoPan;
    QLabel *LuoPanPtr;
    QLabel *CarBody;
    QLabel *SwingArm;

    QLabel *CarPower;
    QLabel *TerminalPower;
    QLabel *CarPowerStr;
    QLabel *TerminalPowerStr;
    /*5 Rocker*/
    QLabel *UpDirection;
    QLabel *LeftDirection;
    QLabel *RightDirection;
    QLabel *DownDirection;
    QLabel *RockerGround;
    QLabel *Rocker;
    /*STATE Show*/
    QLabel  *FuYangSt;
    QLabel  *HeGunSt;
    QLabel  *HangXiangSt;
    QLabel  *BaiBiSt;
    QLabel  *WeDuSt;
    QLabel  *SuDuSt;
    QLabel  *SuDuDangWeiSt;
    QLabel  *CameraAngle;
    /*3 Title*/
    QLabel *CTKZ;
    QLabel *ButtonKZName;
    QLabel *CTFZ;//车体翻转

    QLabel *TouchSN;
    QLabel *GMN;

    QLabel *ButtonSDPZLXName;

    KeyDel *iButtonMsg;
    int ICURRENTMODE;
    void UpdateCtlMode(void);

    int  MenuCurState;
    void ButtonUpDealFunc(void);
    void ButtonDownDealFunc(void);
    void ButtonLeftDealFunc(void);
    void ButtonRightDealFunc(void);
    void ButtonEnterDealFunc(void);
    void ButtonRightDownDealFunc(void);
    void ButtonLeftDownDealFunc(int Type);
    int IsFullFile(char *filename,int Type);
    int  VideoViewFlag;
    QLabel *VideoViewLabel;
    void ButtonRightDownRightDealFunc(void);


    void PowerValSelect(int,QLabel *);
    void ButtonUpMenuDeal(void);
    void ButtonDownMenuDeal(void);
    void ButtonLeftMenuDeal(void);
    void ButtonRightMenuDeal(void);

    void ButtonDownRightLeftFunc(void);
    void ShowPictureToView(void);



    QLabel *ShowFileName;
    QLabel *ShowFileMark;
    QListWidget *MenuList;
    QListWidget *SecMenuList;
    int OnelistFlag;
    int SeclistFlag;
    QStringList FileList;//用来保存wenjian名的表
    //void GetDirFile(QString Type);
    void SecFileListDestory();
    void SecFileListCreate(int num);
    void iDirFileToLink(QString Type);

    QLabel      *BlowUpssLable;
    int MemTime;
    int FIFOCmd;
    int GuanBiaoZhiWei;
    int CararFlag;//挡位标志位
    int RobotHandFlag;//摆臂标志位
    int BlowUpssFlag;//放大标志位
    QString BlowUpssStr;
    void slotActionSave();//截屏函数

    int fd_gpio;
    unsigned char gpiobuf[5];
    int SetGPIO(int value); //write GPIO22
    void SET_AUDIO_BYPASS();
    void SET_AUDIO_368OUT();
    unsigned char DetectTFCard(void);

    MessageServer msgServer;
    char cmdbuf[128];
    int DecodeFlag;
    int PixtureFlag;
    QLabel *ShowPhoto;
    int PauseFlag;


    QLabel *i3DPixShow;
    int width_3d;
    int height_3d;
    int bytePerLine_3d;

    unsigned char m_ScanData[1024*1024];
    unsigned int  m_ScanDataLen;
    unsigned char framedata[WIDTH_3DBOX*HEIGHT_3DBOX*2];

    QImage* image_draw3d;

    int LPPointRoter;
    QGraphicsScene* pScene;
    QGraphicsView * pView;

    int SAPointRoter;
    QGraphicsScene* SApScene;
    QGraphicsView * SApView;

    QTimer *timer;
    QTimer *timer2;
    QTimer *timer3;
    QString TestStr;

    int MenFlag;
    LLIST *gl_MyBmpLList;
    int gl_MenCount;
    int gl_maxfilecnt;
    struct llist_node_st *ShowImageNode;

    int CarPowerMen;
    int TerminalPowerMen;
    QLabel *ShowHelpPic;

    int SelectTF;
    int SelectTFFull;
    QLabel *SelectTFLable;
    unsigned int CheckoutDiskSpace(void);

    int MemPitch;
    int MenRoll;

    int IWaringCount;
    int IWaringFlag;
    QLabel *IWaring;

    int IWaringGWCount;
    int IWaringGWFlag;
    QLabel *IWaringGW;

    int PressOutTimer;
    Ui::se_terminal *ui;
};

#endif // SE_TERMINAL_H
