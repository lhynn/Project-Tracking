#include "se_terminal.h"
#include "ui_se_terminal.h"
#include <QFrame>

extern"C"
{
LLIST *mySearchDirFun(char *str,char *fileType);
void llist_destroy(LLIST *);
}
/*****************************************************
    终端界面类的构造函数
*****************************************************/
se_terminal::se_terminal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::se_terminal)
{
    /******************************************************/
    this->setWindowFlags(Qt::FramelessWindowHint);
    CarPowerMen = 0;
    TerminalPowerMen = 0;
    SelectTF = DetectTFCard();
    msgServer.start(QThread::HighPriority);
    PixtureFlag = 0;
    MemTime = 0;
    FIFOCmd = 0;
    GuanBiaoZhiWei= 0;
    RectGround = new QLabel(this);
    RectGround->resize(1280,720);
    RectGround->setPixmap(QPixmap("/Icon_TranRot/IconShow/RectGround.png"));
    RectGround->move(0,0);

    /*7 Button*/
    ButtonLeft  = new QLabel(this);
    ButtonLeft->move(18,89);
    ButtonLeft->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonLeft.png"));

    ButtonRight = new QLabel(this);
    ButtonRight->move(164,90);
    ButtonRight->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonRight.png"));

    ButtonUp    = new QLabel(this);
    ButtonUp->move(35,74);
    ButtonUp->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonUp.png"));

    ButtonDown  = new QLabel(this);
    ButtonDown->move(35,220);
    ButtonDown->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonDown.png"));

    ButtonEnter  = new QLabel(this);
    ButtonEnter->move(92,147);
    ButtonEnter->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonEnter.png"));

    ButtonDownLeft  = new QLabel(this);
    ButtonDownLeft->move(30,317);
    ButtonDownLeft->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonDownLeft.png"));

    ButtonDownRight  = new QLabel(this);
    ButtonDownRight->move(160,316);
    ButtonDownRight->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonDownRight.png"));

    ButtonRightDownLeft  = new QLabel(this);
    ButtonRightDownLeft->move(1018,331);
    ButtonRightDownLeft->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonDownLeft.png"));

    ButtonRightDownRight  = new QLabel(this);
    ButtonRightDownRight->move(1149,331);
    ButtonRightDownRight->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonDownRight.png"));

    ZTCT = new QLabel(this);
    ZTCT->move(1036,360);
    ZTCT->setPixmap(QPixmap("/Icon_TranRot/IconShow/MSSC.png"));

    CTFZ  = new QLabel(this);
    CTFZ->move(260,50);
    CTFZ->setPixmap(QPixmap("/Icon_TranRot/IconShow/CTFZ.png"));
    CTFZ->hide();

    SelectTFLable = new QLabel(this);
    SelectTFLable->setFont(QFont("JPTZ",22,QFont::AnyStyle));
    SelectTFLable->setStyleSheet("QLabel{background:transparent;color:white;};");
    SelectTFLable->move(580,12);
   // SelectTFLable->setText("SD卡错误");
  //  SelectTFLable->setText("SD卡满");
    //SelectTFLable->hide();
    /********************************************************************************/
    /*8 Tile*/
    SignalNow = new QLabel(this);
    SignalNow->move(30,10);
    SignalNow->setPixmap(QPixmap("/Icon_TranRot/Signal/signal2.png"));
#if 0
    WIFISignal = new QLabel(this);
    WIFISignal->move(105,10);
    WIFISignal->resize(36,30);
    WIFISignal->setScaledContents(true);
    WIFISignal->setPixmap(QPixmap("/Icon_TranRot/Signal/WIFIS.png"));
#endif
    LightIcon = new QLabel(this);
    LightIcon->move(120,10);
    LightIcon->setPixmap(QPixmap("/Icon_TranRot/IconShow/HLight.png"));
 //   LightIcon->hide();

    LightIconQ = new QLabel(this);
    LightIconQ->move(190,10);
    LightIconQ->setPixmap(QPixmap("/Icon_TranRot/IconShow/QLight.png"));
 //   LightIconQ->hide();

    LightIconS = new QLabel(this);
    LightIconS->move(260,10);
    LightIconS->setPixmap(QPixmap("/Icon_TranRot/IconShow/SLight.png"));
 //   LightIconS->hide();

    LightIconY = new QLabel(this);
    LightIconY->move(340,10);
    LightIconY->setPixmap(QPixmap("/Icon_TranRot/IconShow/SLight.png"));
 //   LightIconY->hide();
/*
    Photo = new QLabel(this);
    Photo->move(517,14);
    Photo->setPixmap(QPixmap("/Icon_TranRot/IconShow/Photo.png"));
    Photo->hide();

    Rec = new QLabel(this);
    Rec->move(626,14);
    Rec->setPixmap(QPixmap("/Icon_TranRot/IconShow/Rec.png"));
    Rec->hide();
*/

    CarBody = new QLabel(this);
    CarBody->move(410,1);//410
    CarBody->resize(60,40);
    CarBody->setScaledContents(true);
    CarBody->setPixmap(QPixmap("/Icon_TranRot/IconShow/CarBody.png"));

    SwingArm = new QLabel;
    SwingArm->move(413,2);//413
    SwingArm->resize(36,36);
    SwingArm->setPixmap(QPixmap("/Icon_TranRot/IconShow/SwingArm.png"));
    SwingArm->setScaledContents(true);
    SwingArm->setStyleSheet( "background: transparent;border:0px");

 // SAPointRoter = 0;
 // QGraphicsScene* SApScene;
 // QGraphicsView * SApView;

   SAPointRoter = 0;
   SApScene = new QGraphicsScene;
   SApScene->addWidget(SwingArm);
   SApView = new QGraphicsView(SApScene, this);
   SApView->setStyleSheet( "background: transparent;border:0px");
   SApView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   SApView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   SApView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
   SApView->setGeometry(413,2,36,36);//1019


    CarPowerStr = new QLabel(this);
    CarPowerStr->resize(50,20);
    CarPowerStr->move(1073,18);
    CarPowerStr->setAlignment(Qt::AlignLeft);
    CarPowerStr->setFont(QFont("JPTZ",16,QFont::AnyStyle));
    CarPowerStr->setStyleSheet("QLabel{background:transparent;color:white;};");
    CarPowerStr->setText("0\%");

    TerminalPowerStr = new QLabel(this);
    TerminalPowerStr->resize(50,20);
    TerminalPowerStr->move(1168,18);
    TerminalPowerStr->setAlignment(Qt::AlignLeft);
    TerminalPowerStr->setFont(QFont("JPTZ",16,QFont::AnyStyle));
    TerminalPowerStr->setStyleSheet("QLabel{background:transparent;color:white;};");
    TerminalPowerStr->setText("0\%");

    CarPower = new QLabel(this);
    CarPower->move(1115,18);
    CarPower->setPixmap(QPixmap("/Icon_TranRot/power/100.png"));

    TerminalPower = new QLabel(this);
    TerminalPower->move(1210,18);
    TerminalPower->setPixmap(QPixmap("/Icon_TranRot/power/100.png"));


    TouchSN = new QLabel(this);
    TouchSN->resize(30,35);
    TouchSN->move(845,2);
    TouchSN->setScaledContents(true);
    TouchSN->setPixmap(QPixmap("/Icon_TranRot/IconShow/ZCMPN.png"));//ZCMPN.png"));
 //   TouchSN->hide(); 温度

    GMN = new QLabel(this);
    GMN->move(933,6);
  //GMN->move(740,6);
    GMN->resize(60,30);
    GMN->setScaledContents(true);   //setScaleContents
    GMN->setPixmap(QPixmap("/Icon_TranRot/IconShow/LEDSD.png")); //   GMN->hide();

    LuoPan = new QLabel(this);
//    LuoPan->move(963,2);
    LuoPan->move(750,2);
    LuoPan->resize(44,40);
    LuoPan->setScaledContents(true);
    LuoPan->setPixmap(QPixmap("/Icon_TranRot/IconShow/LuoPan.png"));

    LuoPanPtr = new QLabel;
    LuoPanPtr->move(756,7);
//    LuoPanPtr->move(969,7);
    LuoPanPtr->setPixmap(QPixmap("/Icon_TranRot/IconShow/LuoPanPtr.png"));
    LuoPanPtr->setStyleSheet( "background: transparent;border:0px");


    LPPointRoter = 0;
    pScene = new QGraphicsScene;
    pScene->addWidget(LuoPanPtr);
    pView = new QGraphicsView(pScene, this);
    pView->setStyleSheet( "background: transparent;border:0px");
    pView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pView->setGeometry(756,7,30,30);//1019
//    pView->setGeometry(969,7,30,30);//1019
    /********************************************************************************/

    RockerGround  = new QLabel(this);
    RockerGround->move(MAXROCKLAB_X,MAXROCKLAB_Y);
    RockerGround->setPixmap(QPixmap("/Icon_TranRot/IconShow/RockerGround.png"));

    Rocker  = new QLabel(this);
    Rocker->move(MIXROCKLAB_X-44,MIXROCKLAB_Y-45);
    Rocker->setPixmap(QPixmap("/Icon_TranRot/IconShow/Rocker.png"));

    /*1 Title*/
    CTKZ = new QLabel(this);
    CTKZ->move(1077,56);
    CTKZ->setPixmap(QPixmap("/Icon_TranRot/IconShow/CTKZ.png"));

    ButtonKZName = new QLabel(this);
    ButtonKZName->move(30,56);
    ButtonKZName->setPixmap(QPixmap("/Icon_TranRot/IconShow/SXTKZ.png"));

    ButtonSDPZLXName = new QLabel(this);
    ButtonSDPZLXName->move(37,344);
    ButtonSDPZLXName->setPixmap(QPixmap("/Icon_TranRot/IconShow/CALXDW.png"));

    iButtonMsg = new KeyDel;
    // iButtonMsg->setPriority(QThread::TimeCriticalPriority);
    iButtonMsg ->start();
    connect(iButtonMsg,SIGNAL(TouchButtonMsg(int)),this,SLOT(TouchButtonDeal(int)));
    connect(iButtonMsg,SIGNAL(PhyButtonMsg(int,int)),this,SLOT(PhyButtonDeal(int,int)));
    connect(iButtonMsg,SIGNAL(RockerSgl(float,float,int)),this,SLOT(TouchRockerDeal(float,float,int)));
    connect(iButtonMsg,SIGNAL(OsdUpdate(int)),this,SLOT(OsdUpdateFunc(int)));

    ShowPhoto = new QLabel(this);
    ShowPhoto->resize(720,576);
    ShowPhoto->move(261,75);
  //  ShowPhoto->setPixmap(QPixmap("/Icon_TranRot/IconShow/HelpRead.png"));
    ShowPhoto->hide();

    ShowHelpPic = new QLabel(this);
    ShowHelpPic->move(300,115);
    ShowHelpPic->setPixmap(QPixmap("/Icon_TranRot/IconShow/HelpRead.png"));
    ShowHelpPic->hide();
    /*State Info*/
    FuYangSt= new QLabel(this);
    FuYangSt->resize(140,24);
    FuYangSt->move(1020,480);
    FuYangSt->setAlignment(Qt::AlignLeft);
    FuYangSt->setFont(QFont("JPTZ",20,QFont::AnyStyle));
    FuYangSt->setStyleSheet("QLabel{background:transparent;color:white;};");

    HeGunSt = new QLabel(this);
    HeGunSt->resize(140,24);
    HeGunSt->move(1140,480);
    HeGunSt->setAlignment(Qt::AlignLeft);
    HeGunSt->setFont(QFont("JPTZ",20,QFont::AnyStyle));
    HeGunSt->setStyleSheet("QLabel{background:transparent;color:white;};");

    HangXiangSt = new QLabel(this);
    HangXiangSt->resize(140,24);
    HangXiangSt->move(1020,514);
    HangXiangSt->setAlignment(Qt::AlignLeft);
    HangXiangSt->setFont(QFont("JPTZ",20,QFont::AnyStyle));
    HangXiangSt->setStyleSheet("QLabel{background:transparent;color:white;};");

    BaiBiSt = new QLabel(this);
    BaiBiSt->resize(140,24);
    BaiBiSt->move(1140,514);
    BaiBiSt->setAlignment(Qt::AlignLeft);
    BaiBiSt->setFont(QFont("JPTZ",20,QFont::AnyStyle));
    BaiBiSt->setStyleSheet("QLabel{background:transparent;color:white;};");

    WeDuSt  = new QLabel(this);
    WeDuSt->resize(200,24);
    WeDuSt->move(1020,548);
    WeDuSt->setAlignment(Qt::AlignLeft);
    WeDuSt->setFont(QFont("JPTZ",20,QFont::AnyStyle));
    WeDuSt->setStyleSheet("QLabel{background:transparent;color:white;};");

    SuDuSt  = new QLabel(this);
    SuDuSt->resize(140,24);
    SuDuSt->move(1140,548);
    SuDuSt->setAlignment(Qt::AlignLeft);
    SuDuSt->setFont(QFont("JPTZ",20,QFont::AnyStyle));
    SuDuSt->setStyleSheet("QLabel{background:transparent;color:white;};");

    CameraAngle  = new QLabel(this);
    CameraAngle->resize(200,24);
    CameraAngle->move(1020,582);
    CameraAngle->setAlignment(Qt::AlignLeft);
    CameraAngle->setFont(QFont("JPTZ",20,QFont::AnyStyle));
    CameraAngle->setStyleSheet("QLabel{background:transparent;color:white;};");

    SuDuDangWeiSt  = new QLabel(this);
    SuDuDangWeiSt->resize(140,24);
    SuDuDangWeiSt->move(1020,618);
    SuDuDangWeiSt->setAlignment(Qt::AlignLeft);
    SuDuDangWeiSt->setFont(QFont("JPTZ",20,QFont::AnyStyle));
    SuDuDangWeiSt->setStyleSheet("QLabel{background:transparent;color:white;};");

    BlowUpssFlag = 0;
    BlowUpssLable = new QLabel(this);
    BlowUpssLable->resize(60,22);
    BlowUpssLable->move(948,48);//1020,140);
    BlowUpssLable->setAlignment(Qt::AlignLeft);
    BlowUpssLable->setFont(QFont("JPTZ",22,QFont::AnyStyle));
    BlowUpssLable->setStyleSheet("QLabel{background:transparent;color:white;};");
    BlowUpssStr.sprintf("X%.1f",(float)(BlowUpssFlag+10)/10);
    BlowUpssLable->setText(BlowUpssStr);

    VideoViewFlag = 0;
    VideoViewLabel = new QLabel(this);
    VideoViewLabel->resize(120,26);
    VideoViewLabel->move(840,48);//1020,140);
    VideoViewLabel->setAlignment(Qt::AlignLeft);
    VideoViewLabel->setFont(QFont("JPTZ",22,QFont::AnyStyle));
    VideoViewLabel->setStyleSheet("QLabel{background:transparent;color:white;};");
    VideoViewLabel->setText("前主后辅");

    ShowFileName  = new QLabel(this);
    ShowFileName->resize(180,26);
    ShowFileName->move(550,45);
    ShowFileName->setAlignment(Qt::AlignLeft);
    ShowFileName->setFont(QFont("JPTZ",24,QFont::Bold));
    ShowFileName->setStyleSheet("QLabel{background:transparent;color:white;};");
    ShowFileName->hide();

    ShowFileMark  = new QLabel(this);
    ShowFileMark->resize(24,24);
    ShowFileMark->setScaledContents(true);   //setScaleContents
    ShowFileMark->move(720,45);
    ShowFileMark->setPixmap(QPixmap("/Icon_TranRot/IconShow/UDSelect.png"));
    ShowFileMark->hide();

    CararFlag = 0;
    ICURRENTMODE = CAMERCTLMODE;
    MenuCurState = MENUSTATEHIDE;
    /***********************************MENU********************************************/
    OnelistFlag = 0;
    SeclistFlag = 0;
    MenuList = new QListWidget(this);
    MenuList->resize(260,236);
    MenuList->move(1005,418);
    MenuList->setIconSize(QSize(40,26));
    //光感 放大 触屏
    MenuList->addItem(new QListWidgetItem(QIcon("/Icon_TranRot/IconShow/SHIPIN.png"),tr("        视频")));
    MenuList->addItem(new QListWidgetItem(QIcon("/Icon_TranRot/IconShow/TUPIAN.png"),tr("        图片")));
    MenuList->addItem(new QListWidgetItem(QIcon("/Icon_TranRot/IconShow/SFJ.png"), tr("        缩放")));
    MenuList->addItem(new QListWidgetItem(QIcon("/Icon_TranRot/IconShow/GM.png"), tr("        光敏")));
    MenuList->addItem(new QListWidgetItem(QIcon("/Icon_TranRot/IconShow/CMP.png"), tr("         摇杆")));
    MenuList->setFont(QFont("JPTZ",22,QFont::Bold));
    MenuList->setStyleSheet("QListWidget{background-color:rgb(70,70,70);border:8px solid gray;border-color:rgb(60,60,60);}"
                            "QListWidget::Item{padding-top:8px; padding-bottom:8px; }");
    //"QListWidget::item:selected:!active{border-width:0px; background:lightgreen; color:red;}");

    SecMenuList = new QListWidget(this);
    SecMenuList->resize(260,236);
    SecMenuList->move(1005,418);
    SecMenuList->setIconSize(QSize(40,26));
    SecMenuList->setFont(QFont("JPTZ",22,QFont::AnyStyle));
    SecMenuList->setStyleSheet("QListWidget{background-color:rgb(70,70,70);border:8px solid gray;border-color:rgb(60,60,60);}"
                               "QListWidget::Item{padding-top:8px; padding-bottom:8px; }");
    //    "QListWidget::item:selected:!active{border-width:0px; background:lightgreen; color:red;}");
    SecMenuList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); 	//垂直方向
    SecMenuList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); 		//水平方向

    SecMenuList->setCurrentRow(SeclistFlag);
    SecMenuList->hide();
    MenuList->setCurrentRow(OnelistFlag);
    MenuList->hide();

    DecodeFlag = 0;
    PauseFlag = 0;
    ui->setupUi(this);
    SET_AUDIO_BYPASS();
    system("./encodertsp -v 1.264 -a 1.aac -y 2 -I 1 &");
#if 0
    /***********************83D Show***********************/
    width_3d  = WIDTH_3DBOX;
    height_3d = HEIGHT_3DBOX;
    bytePerLine_3d = WIDTH_3DBOX*2;
    // m_ScanDataLen = sizeof(TestData); ///////use test data
    // memcpy(m_ScanData, TestData,  m_ScanDataLen);

    api_sgl(m_ScanData,framedata,m_ScanDataLen,width_3d,height_3d);
    image_draw3d = new QImage(framedata, width_3d, height_3d, bytePerLine_3d, QImage::Format_RGB555);

    i3DPixShow = new QLabel(this);
    i3DPixShow->setFrameShape (QFrame::Box);
    i3DPixShow->setStyleSheet("background-color: rgb(0, 0, 255);border:8px solid gray;border-color:rgb(60,60,60);");
    i3DPixShow->setGeometry(28,420,width_3d, height_3d);
    i3DPixShow->show();
    i3DPixShow->setPixmap(QPixmap::fromImage(*image_draw3d));
    i3DPixShow->show();
    delete image_draw3d;
 #endif
    i3DPixShow = new QLabel(this);
   // i3DPixShow->setStyleSheet("background-color: rgb(0, 0, 255);border:8px solid gray;border-color:rgb(60,60,60);");
    i3DPixShow->resize(226,236);
    i3DPixShow->setScaledContents(true);
    i3DPixShow->setStyleSheet("background-color: rgb(0, 0, 255);border:8px solid gray;border-color:rgb(60,60,60);");
    i3DPixShow->move(22,420);
    TestStr.sprintf("/Icon_TranRot/CarStat/00.png");
    i3DPixShow->setPixmap(QPixmap(TestStr));

    MenFlag = 0;
    gl_MenCount = 0;

    timer = new QTimer(this);
   // connect( timer, SIGNAL(timeout()), this, SLOT(timerDone()) );
   // timer->start(600000); // 2秒单触发定时器
    SelectTFFull = 0;
    MemPitch = 0;
    MenRoll = 0;

    IWaringCount  = 0;
    IWaringFlag = 0;
    IWaring = new QLabel(this);
    IWaring->resize(100,45);
    IWaring->move(570,672);
    IWaring->setScaledContents(true);
    IWaring->setPixmap(QPixmap("/Icon_TranRot/IconShow/WaringGL.png"));
    IWaring->hide();


    IWaringGWCount  = 0;
    IWaringGWFlag = 0;
    IWaringGW = new QLabel(this);
    IWaringGW->resize(100,45);
    IWaringGW->move(700,672);
    IWaringGW->setScaledContents(true);
    IWaringGW->setPixmap(QPixmap("/Icon_TranRot/IconShow/WaringGW.png"));
    IWaringGW->hide();
}

se_terminal::~se_terminal()
{
    delete ui;
}
void se_terminal::timerDone(void){
   unsigned int  Count;
/*
    Count = CheckoutDiskSpace();
    if(Count < 400){
        SelectTFFull = 1;
        SelectTFLable->setText("SD卡满");
        msgServer.sendCmd(QtInterface_Stop);
        system("sync");
    }
    printf("Timer OutTimer.\n");*/
}

unsigned int se_terminal::CheckoutDiskSpace(void)
{
    struct statfs diskInfo;
    unsigned long long totalBlocks;
    unsigned long long totalSize;
    size_t mbTotalsize;
    unsigned long long freeDisk;
    size_t mbFreedisk;

    statfs("/home/root/media", &diskInfo);
    totalBlocks = diskInfo.f_bsize;
    totalSize = totalBlocks*diskInfo.f_blocks;
    mbTotalsize = totalSize >> 20;//compute the size by unit MB, 20 = 1MB
    freeDisk = diskInfo.f_bfree*totalBlocks;
    mbFreedisk = freeDisk >> 20;
    printf("/dev/sda1 total = %dMB, free = %dMB\n", mbTotalsize, mbFreedisk);
    return mbFreedisk;
}

/*****************************************************
    挂在TF卡
*****************************************************/
unsigned char se_terminal::DetectTFCard(void)
{
    unsigned char cardexist = 0;
    if(fopen("/dev/mmcblk0p1", "r") == NULL){                              //是否有TF卡设备
        printf("No TF Card Insert!\r\n");
        cardexist = 0;
    }else{
        system("mount -t vfat /dev/mmcblk0p1 /home/root/media/");           //挂在TF卡
        system("insmod /dm368/g_file_storage.ko file=/dev/mmcblk0p1 stall=0 removable=1");//安装TF卡导出驱动
        printf("Mount TF Card OK!  (/dev/mmcblk0p1)\r\n");
        cardexist = 1;
        return cardexist;
    }
    if(0 == cardexist){
        if(fopen("/dev/mmcblk0", "r") == NULL){
            printf("No TF Card Insert!\r\n");
            cardexist = 0;
        }else{
            system("mount -t vfat /dev/mmcblk0 /home/root/media/");
            system("insmod /dm368/g_file_storage.ko file=/dev/mmcblk0 stall=0 removable=1");
            printf("Mount TF Card OK!  (/dev/mmcblk0)\r\n");
            cardexist = 1;
        }
    }
    return cardexist;
}
/*****************************************************
 触摸屏虚拟摇杆
*****************************************************/
void se_terminal::TouchRockerDeal(float iX,float iY,int iPress){
    if(iPress){
        Rocker->move(iX+MIXROCKLAB_X-44,iY+MIXROCKLAB_Y-45);
        Rocker->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/RockerNow.png"));
    }else{
        Rocker->setPixmap(QPixmap("/Icon_TranRot/IconShow/Rocker.png"));
        Rocker->move(MIXROCKLAB_X-44,MIXROCKLAB_Y-45);
    }
}
/*****************************************************
 触摸屏按键处理
*****************************************************/
void se_terminal::TouchButtonDeal(int x){                                      //Touch  5  Button
    switch(x){
    case BUTTONUPPRESS:
        ButtonUp->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonUpNow.png"));
        break;
    case BUTTONUPRELEASE:
        ButtonUp->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonUp.png"));
        ButtonUpDealFunc();
        break;
    case BUTTONLEFTPRESS:
        ButtonLeft->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonLeftNow.png"));
        break;
    case BUTTONLEFTRELEASE:
        ButtonLeft->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonLeft.png"));
        ButtonLeftDealFunc();
        break;
    case BUTTONENTERPRESS:
        ButtonEnter->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonEnterNow.png"));
        break;
    case BUTTONENTERRELEASE:
        ButtonEnter->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonEnter.png"));
        ButtonEnterDealFunc();
        break;
    case BUTTONRIGHTFPRESS:
        ButtonRight->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonRightNow.png"));
        break;
    case BUTTONRIGHTRELEASE:
        ButtonRight->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonRight.png"));
        ButtonRightDealFunc();
        break;
    case BUTTONDOWNPRESS:
        ButtonDown->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonDownNow.png"));
        break;
    case BUTTONDOWNRELEASE:
        ButtonDown->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonDown.png"));
        ButtonDownDealFunc();
        break;
    case BUTTONLDLPRESS:
        ButtonDownLeft->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonDownLeftNow.png"));
        break;
    case BUTTONLDLRELEASE:
        ButtonDownLeft->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonDownLeft.png"));
        ButtonLeftDownDealFunc(0);
        break;
    case BUTTONLDLRELEASEL:
        ButtonDownLeft->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonDownLeft.png"));
        ButtonLeftDownDealFunc(1);
        break;
    case BUTTONLDRPRESS:
        ButtonDownRight->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonDownRightNow.png"));
        break;
    case BUTTONLDRRELEASE:
        ButtonDownRight->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonDownRight.png"));
        ButtonRightDownDealFunc();
        break;
    case BUTTONRDLPRESS:
        ButtonRightDownLeft->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonDownLeftNow.png"));
        break;
    case BUTTONRDLRELEASE:
        ButtonRightDownLeft->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonDownLeft.png"));
        if( MenuCurState == MENUSTATEHIDE){
            ICURRENTMODE++;
            if(ICURRENTMODE > MENUCTLMODE)
                ICURRENTMODE = CAMERCTLMODE;
            UpdateCtlMode();
        }
     //   ButtonDownRightLeftFunc();
        break;
    case BUTTONRDRPRESS:
        ButtonRightDownRight->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonDownRightNow.png"));
        break;
    case BUTTONRDRRELEASE:
        ButtonRightDownRight->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonDownRight.png"));
         ButtonRightDownRightDealFunc();
        break;
    }
}

/*****************************************************
 更新界面的控制模式
*****************************************************/
void  se_terminal::UpdateCtlMode(void){
    switch(ICURRENTMODE){
    case CAMERCTLMODE:
        g_SendSTM[3] &= ~0x60;
        ButtonKZName->setPixmap(QPixmap("/Icon_TranRot/IconShow/SXTKZ.png"));
        break;
    case LIGHTCTLMODE:
        g_SendSTM[3] &= ~0x60;
        g_SendSTM[3] |= 0x40;
        ButtonKZName->setPixmap(QPixmap("/Icon_TranRot/IconShow/DGKZ.png"));
        break;
    case ROBOTCTLMODE:
        g_SendSTM[3] &= ~0x60;
        g_SendSTM[3] |= 0x20;
        if(g_iCarStateInfo.CheTiFanZhuan)
            ButtonKZName->setPixmap(QPixmap("/Icon_TranRot/IconShow/BBKZ.png"));
        else
            ButtonKZName->setPixmap(QPixmap("/Icon_TranRot/IconShow/BBKZFW.png"));
        break;
    case MENUCTLMODE:
        g_SendSTM[3] &= ~0x60;
        g_SendSTM[3] |= 0x40;
        ButtonKZName->setPixmap(QPixmap("/Icon_TranRot/IconShow/CDKZ.png"));
        break;
    }

}

void se_terminal::ButtonRightDownRightDealFunc(void){
    if(MenuCurState != MENUSTATEHIDE){
        if(DecodeFlag){
            if(PauseFlag){
                msgServer.sendCmd(QtInterface_Play);
                PauseFlag = 0;
            }else{
                msgServer.sendCmd(QtInterface_Pause);
                PauseFlag = 1;
            }
        }
    }else {
        if(VideoViewFlag){
            VideoViewFlag = 0;
             g_SendSTM[2] &= ~0x10;
            VideoViewLabel->setText("前主后辅");
        }else{
            VideoViewFlag = 1;
            g_SendSTM[2] |= 0x10;
            VideoViewLabel->setText("前辅后主");
        }
    }
}
/*****************************************************
物理按键处理函数
*****************************************************/
void  se_terminal::PhyButtonDeal(int Num,int Type){
    switch(Num){
    case 0:
        ButtonUp->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonUpNow.png"));
        ButtonUpDealFunc();
        break;
    case 1:
        ButtonDown->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonDownNow.png"));
        ButtonDownDealFunc();
        break;
    case 2:
        ButtonLeft->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonLeftNow.png"));
        ButtonLeftDealFunc();
        break;
    case 3:
        ButtonRight->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonRightNow.png"));
        ButtonRightDealFunc();
        break;
    case 4:
        ButtonEnter->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonEnterNow.png"));
        ButtonEnterDealFunc();
        break;
    case 5:
        ButtonDownRight->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonDownRightNow.png"));
        ButtonRightDownDealFunc();
        break;
    case 6:
        ButtonDownLeft->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonDownLeftNow.png"));
        ButtonLeftDownDealFunc(Type);
        break;
    case 7:                             //Select Mode
        break;
    case 8:break;
    case 9:
         ButtonRightDownRight->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonDownRightNow.png"));
         ButtonRightDownRightDealFunc();
        break;
    case 10:
        ButtonRightDownLeft->setPixmap(QPixmap("/Icon_TranRot/ButtonNow/ButtonDownLeftNow.png"));
        if( MenuCurState == MENUSTATEHIDE){
            ICURRENTMODE++;
            if(ICURRENTMODE > MENUCTLMODE)
                ICURRENTMODE = CAMERCTLMODE;
            UpdateCtlMode();
        }
      //   ButtonDownRightLeftFunc();
        break;
    case 11:break;
    }
}
#if 0
/*****************************************************
获取目录中的类型文件
*****************************************************/
void se_terminal::GetDirFile(QString Type){
    QDir dir("/home/root/media/");//定义一个QDir变量,设置路径为当前目录的music文件夹(这里可用绝对路径也可以用相对路径)
    QFileInfoList FileInfoList;//文件信息表
    QFileInfo FileInfo;//文件信息变量
    QString StringSong;//用于保存文件信息字符串

    FileList << Type;//把所有mp3文件加入到stringlist_song表中
    dir.setNameFilters(FileList);//过滤文件
    FileInfoList = dir.entryInfoList();//列出所有符合的文件
    for( int i = 0;i < FileInfoList.length();i++){
        FileInfo = FileInfoList.at(i);//把fileinfolist表中某个位置的歌曲名赋给fileinfo变量
        StringSong = FileInfo.fileName();//把fileinfo变量中的文件名转换成QString并赋给string_song
        FileList << StringSong;//把歌曲名赋给stringlist_song表
    }
}

#endif

void se_terminal::iDirFileToLink(QString Type){
    QByteArray  ba = Type.toLatin1(); // must
    char *str = ba.data();
    printf("str:%s\n",str);
    fflush(NULL);
    gl_MyBmpLList = mySearchDirFun("/home/root/media/",str);
    if(gl_MyBmpLList == NULL){
        printf("Search Dir error.\n");
    }
    ShowImageNode = gl_MyBmpLList->head.next;
    gl_MyBmpLList->getMemSum(gl_MyBmpLList,&gl_maxfilecnt);
}
/*****************************************************
文件列表窗口ChuangJian
*****************************************************/
void se_terminal::SecFileListCreate(int num){
#if 0
    int i;
    QDir dir("/home/root/media/");//定义一个QDir变量,设置路径为当前目录的music文件夹(这里可用绝对路径也可以用相对路径)
    QFileInfoList FileInfoList;//文件信息表
    QFileInfo FileInfo;//文件信息变量
    QString StringSong;//用于保存文件信息字符串

    FileList << Type;//把所有mp3文件加入到stringlist_song表中
    dir.setNameFilters(FileList);//过滤文件
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
   // printf("dir.count() :%d\n",dir.count());
  //  fflush(NULL);
   //FileInfoList = dir.entryInfoList();//列出所有符合的文件
    for(i = 0;i < 6;i++){//dir.entryInfoList().length();i++){
        FileInfo = dir.entryInfoList().at(i);//把fileinfolist表中某个位置的歌曲名赋给fileinfo变量
        StringSong = FileInfo.fileName();//把fileinfo变量中的文件名转换成QString并赋给string_song
        // FileList << StringSong;//把歌曲名赋给stringlist_song表
         QListWidgetItem *lst = new QListWidgetItem(StringSong);
         SecMenuList->addItem(lst);
     }
/*
    for(int i = 1; i < FileList.length();i++){
        QListWidgetItem *lst = new QListWidgetItem(FileList.at(i));
        SecMenuList->addItem(lst);
    }
    */
#endif
    QString Tmp;
    PT_MyFileSt data = NULL;
    for(int i = 0;i < num;i++){
        if(ShowImageNode  != &gl_MyBmpLList->head){
            data = (PT_MyFileSt)ShowImageNode->data;
            printf("data->name :%s\n",data->name);
            fflush(NULL);
            Tmp = QString(QLatin1String(data->name));
            QListWidgetItem *lst = new QListWidgetItem(Tmp);
            SecMenuList->addItem(lst);
            ShowImageNode = ShowImageNode->next;
            gl_MenCount++;
         }
    }
     printf("gl_maxfilecnt %d gl_MenCount:%d\n",gl_maxfilecnt,gl_MenCount);
}

/*****************************************************
文件列表窗口清空
*****************************************************/
void se_terminal::SecFileListDestory(){
    int index;
    for(index = 0;index < gl_MenCount ;index++){
        QListWidgetItem *item = SecMenuList->takeItem(0);
        delete item;
    }
    printf("SecFileListDestory gl_MenCount:%d\n",gl_MenCount);
    gl_MenCount = 0;
    gl_maxfilecnt = 0;
    llist_destroy(gl_MyBmpLList);
}

/*****************************************************
菜单选择 上 处理函数
*****************************************************/
void se_terminal::ShowPictureToView(void){
         ShowFileName->setText(SecMenuList->item(SeclistFlag)->text().toLatin1().data());
         ShowFileName->show();
         ShowFileMark->show();
         memset(cmdbuf,0,128);
         sprintf(cmdbuf, "/home/root/media/%s.jpg",SecMenuList->item(SeclistFlag)->text().toLatin1().data());
         printf("cmdbuf :%s\n\r",cmdbuf);
         ShowPhoto->setPixmap(QPixmap(cmdbuf));
         ShowPhoto->show();
}
/*****************************************************
菜单选择 上 处理函数
*****************************************************/
void se_terminal::ButtonUpMenuDeal(void){
    int sum;
    switch(MenuCurState){
    case MENUSTATEHIDE: break;
    case MENUSTATESHOW:
        sum = MenuList->count();
        if((--OnelistFlag) < 0) OnelistFlag = sum - 1;
        MenuList->setCurrentRow(OnelistFlag);
        break;
    case MENUSTATESEC:                                     //判读菜单进入的选项
        switch(OnelistFlag){
        case 0:                                          //视频和图片项
            if((--SeclistFlag) < 0) SeclistFlag = 0;//'sum - 1;
            SecMenuList->setCurrentRow(SeclistFlag);
            break;
        case 1:
            if((--SeclistFlag) < 0) SeclistFlag = 0;//'sum - 1;
            SecMenuList->setCurrentRow(SeclistFlag);
            if(PixtureFlag)
                ShowPictureToView();
            break;
        case 2:
        case 3:
        case 4:
            sum = SecMenuList->count();
            if((--SeclistFlag) < 0) SeclistFlag = sum - 1;
            SecMenuList->setCurrentRow(SeclistFlag);
            break;
        }
        break;
    }
}

/*****************************************************
菜单选择 下 处理函数
*****************************************************/
void se_terminal::ButtonDownMenuDeal(void){
    int sum;
    switch(MenuCurState){
    case MENUSTATEHIDE: break;
    case MENUSTATESHOW:
        sum = MenuList->count();
        if((++OnelistFlag) > sum - 1) OnelistFlag = 0;
        MenuList->setCurrentRow(OnelistFlag);
        break;
    case MENUSTATESEC:
        switch(OnelistFlag){
        case 0: //视频和图片项
            if(SeclistFlag < gl_maxfilecnt - 1)SeclistFlag++;
            SecFileListCreate(1);
            SecMenuList->setCurrentRow(SeclistFlag);
            break;
        case 1:
            if(SeclistFlag < gl_maxfilecnt - 1)SeclistFlag++;
            SecFileListCreate(1);
            SecMenuList->setCurrentRow(SeclistFlag);
            if(PixtureFlag)
                 ShowPictureToView();
            break;
        case 2:
        case 3:
        case 4:
            sum = SecMenuList->count();
             if((++SeclistFlag) > sum - 1) SeclistFlag = 0;
            SecMenuList->setCurrentRow(SeclistFlag);
            break;
        }
        break;
    }
}

/*****************************************************
菜单选择 左 处理函数
*****************************************************/
void se_terminal::ButtonLeftMenuDeal(void){
    int index;
    if(DecodeFlag){
        system("pkill -9 decode");
        MenFlag = 1;
        ShowFileName->hide();
        ShowFileMark->hide();
        DecodeFlag = 0;
        ShowHelpPic->show();
    }else{
        switch(MenuCurState){
        case MENUSTATEHIDE: break;
        case MENUSTATESHOW:
            ZTCT->setPixmap(QPixmap("/Icon_TranRot/IconShow/MSSC.png"));
            OnelistFlag = 0;
            MenuList->hide();
            MenuCurState = MENUSTATEHIDE;
            SeclistFlag = 0;
            SecMenuList->hide();
         //  SecFileListDestory();
            break;
        case MENUSTATESEC:

             MenuCurState = MENUSTATESHOW;
             SeclistFlag = 0;
             SecMenuList->hide();
             MenuList->show();
             MenuList->setCurrentRow(OnelistFlag);
             if(PixtureFlag){
                 ShowFileName->hide();
                 ShowFileMark->hide();
                 ShowPhoto->hide();
                 PixtureFlag = 0;
                 //printf("__________________________________________1\n");
                 //fflush(NULL);
                 SET_AUDIO_BYPASS();
                 system("./encodertsp -v 1.264 -a 1.aac -y 2 -I 1 &");
             }
             if(MenFlag){
                 ShowHelpPic->hide();
                //  printf("__________________________________________2\n");
                 // fflush(NULL);
                 SET_AUDIO_BYPASS();
                 system("./encodertsp -v 1.264 -a 1.aac -y 2 -I 1 &");
                 MenFlag = 0;
             }
            switch(OnelistFlag){
            case 0:                                          //视频和图片项
                g_SendSTM[3] &= ~0x60;
                g_SendSTM[3] |= 0x40;
                SecFileListDestory();
                 break;
            case 1:
                g_SendSTM[3] &= ~0x60;
                g_SendSTM[3] |= 0x40;
                SecFileListDestory();
                break;
            case 2:
                for(index = 0;index < 7;index++){
                    QListWidgetItem *item = SecMenuList->takeItem(0);
                    delete item;
                }break;
            case 3:
            case 4:
                for(index = 0;index < 2;index++){
                    QListWidgetItem *item = SecMenuList->takeItem(0);
                    delete item;
                }
                break;
            }
            break;
        }
  }
}

/*****************************************************
菜单选择 右 处理函数
*****************************************************/
void se_terminal::ButtonRightMenuDeal(void){
    switch(MenuCurState){
    case MENUSTATEHIDE:
        break;
    case MENUSTATESHOW:
        MenuCurState = MENUSTATESEC;
        MenuList->hide();

        switch(OnelistFlag){
        case 0:
            g_SendSTM[3] |= 0x60;
          // SecFileListCreate(".avi");                    //"* .avi"
            iDirFileToLink(".avi");
             if(gl_maxfilecnt > 6)
                SecFileListCreate(6);
             else
                SecFileListCreate(gl_maxfilecnt);
            SecMenuList->show();
            SecMenuList->setCurrentRow(SeclistFlag);break;      //视频和图片项
        case 1:
            g_SendSTM[3] |= 0x60;
            iDirFileToLink(".jpg");                   //"*.bmp"
            if(gl_maxfilecnt > 6)
                SecFileListCreate(6);
            else
                SecFileListCreate(gl_maxfilecnt );
            SecMenuList->show();
            SecMenuList->setCurrentRow(SeclistFlag);      //视频和图片项
            break;
        case 2:
            SecMenuList->show();
            SecMenuList->addItem(new QListWidgetItem("               X 1.0"));
            SecMenuList->addItem(new QListWidgetItem("               X 1.5"));
            SecMenuList->addItem(new QListWidgetItem("               X 2.0"));
            SecMenuList->addItem(new QListWidgetItem("               X 2.5"));
            SecMenuList->addItem(new QListWidgetItem("               X 3.0"));
            SecMenuList->addItem(new QListWidgetItem("               X 3.5"));
            SecMenuList->addItem(new QListWidgetItem("               X 4.0"));
            SecMenuList->setCurrentRow(SeclistFlag);
            break;
        case 3:
            SecMenuList->show();
            SecMenuList->addItem(new QListWidgetItem("               打开")); //打开 关闭
            SecMenuList->addItem(new QListWidgetItem("               关闭"));
            SecMenuList->setCurrentRow(SeclistFlag);
            break;
        case 4:
           // SecFileListDestory();
            SecMenuList->show();
            SecMenuList->addItem(new QListWidgetItem("               打开"));
            SecMenuList->addItem(new QListWidgetItem("               关闭"));
            SecMenuList->setCurrentRow(SeclistFlag);
            break;
        }
        break;
    case MENUSTATESEC:
        switch(OnelistFlag){
        case 0:                                          //视频和图片项
            if(SecMenuList->count()){
                 ShowHelpPic->hide();
                 if(DecodeFlag && !PixtureFlag){
                     printf("MENUSTATESEC 0 :pkill -9  decode \n\n");
                     fflush(NULL);
                     system("pkill -9  decode");
                 }else{
                     printf("MENUSTATESEC 0 :pkill -9  encodertsp\n\n");
                     fflush(NULL);
                     system("pkill -9 encodertsp");
                    //sleep(1);
                 }
                 ShowFileName->setText(SecMenuList->item(SeclistFlag)->text().toLatin1().data());
                ShowFileName->show();
                ShowFileMark->show();
                memset(cmdbuf,0,128);
                sprintf(cmdbuf, "./decode -y 2 -v h264 -f /home/root/media/%s.avi &",\
                                  SecMenuList->item(SeclistFlag)->text().toLatin1().data());


                system(cmdbuf);
                SET_AUDIO_368OUT();
                PauseFlag = 0;
                DecodeFlag = 1;
            }
            break;
        case 1:
            if(SecMenuList->count()){
                if(!PixtureFlag){
                    printf("MENUSTATESEC 1:pkill -9  encodertsp\n\n");
                    fflush(NULL);
                    system("pkill -9 encodertsp");
                    ShowPictureToView();
                    PixtureFlag = 1;
                }
            }
            break;
        case 2:
            switch(SeclistFlag){
            case 0:BlowUpssFlag = 10;break;
            case 1:BlowUpssFlag = 15;break;
            case 2:BlowUpssFlag = 20;break;
            case 3:BlowUpssFlag = 25;break;
            case 4:BlowUpssFlag = 30;break;
            case 5:BlowUpssFlag = 35;break;
            case 6:BlowUpssFlag = 40;break;
            }
            BlowUpssStr.sprintf("X%.1f",(float)(BlowUpssFlag)/10);
            BlowUpssLable->setText(BlowUpssStr);
            g_SendSTM[3] &= ~0x1F;
            g_SendSTM[3] |= (BlowUpssFlag - 10);
            break;
        case 3:
            if(SeclistFlag){
                g_SendSTM[2] &= ~0x20;              //光感
                GMN->setPixmap(QPixmap("/Icon_TranRot/IconShow/LEDSD.png"));
                GuanBiaoZhiWei = 0;
            }else{
                g_SendSTM[2] |= 0x20;
                GMN->setPixmap(QPixmap("/Icon_TranRot/IconShow/LEDZD.png"));
                GuanBiaoZhiWei = 1;
            }break;
        case 4:
            if(SeclistFlag){                      //触屏
                g_SendSTM[3] &= ~0x80;
                TouchSN->setPixmap(QPixmap("/Icon_TranRot/IconShow/ZCMPN.png"));
               // TouchSN->show();
            }else{
                g_SendSTM[3] |= 0x80;
                 TouchSN->setPixmap(QPixmap("/Icon_TranRot/IconShow/CMP.png"));//ZCMPN.png"));
                //TouchSN->hide();
            } break;
        }
        break;
    }
}
/*****************************************************
  上按键触发处理函数
*****************************************************/
void se_terminal::ButtonUpDealFunc(void){
    switch(ICURRENTMODE){
    case CAMERCTLMODE:
       // g_SendSTM[2] |= 0x00;
        break;
    case LIGHTCTLMODE:
        if(!GuanBiaoZhiWei){
            if(g_SendSTM[2]&0x04)
                g_SendSTM[2] &= ~0x04;          //上灯
            else
                g_SendSTM[2] |= 0x04;
       }break;
    case ROBOTCTLMODE:
        break;
    case MENUCTLMODE:
        ButtonUpMenuDeal();
        break;
    }
}
/*****************************************************
 下 键触发处理函数
*****************************************************/
//Byte[2] bit7—bit6   高低速档    0—低   1—中  2—高
void se_terminal::ButtonDownDealFunc(void){
    switch(ICURRENTMODE){
    case CAMERCTLMODE:
      //  g_SendSTM[2] |= 0x10;
        break;
    case LIGHTCTLMODE:
        if(g_SendSTM[2]&0x02)           //后灯
            g_SendSTM[2] &= ~0x02;
        else
            g_SendSTM[2] |= 0x02;
        break;
    case ROBOTCTLMODE:
        break;
    case MENUCTLMODE:
        ButtonDownMenuDeal();
        break;
    }
}
//  01—最上 10—最下  11—中间(摄像
// 01—爬坡位置  10—复位位置  11—翻身(机械臂)
/*****************************************************
  左 键触发处理函数
*****************************************************/
void se_terminal::ButtonLeftDealFunc(void){
    switch(ICURRENTMODE){
    case CAMERCTLMODE://01—最上
            g_SendSTM[4] &= ~0x03;
            g_SendSTM[4] |= 0x01;
        break;
    case LIGHTCTLMODE:
        if(!GuanBiaoZhiWei){
            if(g_SendSTM[2]&0x08)               //预警灯
                g_SendSTM[2] &= ~0x08;
            else
                g_SendSTM[2] |= 0x08;
        }
        break;
    case ROBOTCTLMODE://10—复位位置
        g_SendSTM[4] &= ~0x0c;
        g_SendSTM[4] |= 0x08;
        break;
    case MENUCTLMODE:
        ButtonLeftMenuDeal();
        break;
    }
}
/*****************************************************
   右键触发处理函数
*****************************************************/
void se_terminal::ButtonRightDealFunc(void){
    switch(ICURRENTMODE){
    case CAMERCTLMODE:// 10—最下
        g_SendSTM[4] &= ~0x03;
        g_SendSTM[4] |= 0x02;
        break;
    case LIGHTCTLMODE:
        if(g_SendSTM[2]&0x01)
            g_SendSTM[2] &= ~0x01;          //shang灯
        else
            g_SendSTM[2] |= 0x01;
        break;
    case ROBOTCTLMODE:// 01—爬坡位置
        g_SendSTM[4] &= ~0x0c;
        g_SendSTM[4] |= 0x04;
        break;
    case MENUCTLMODE:
        ButtonRightMenuDeal();
        break;
    }
}
/*****************************************************
   中 键触发处理函数
*****************************************************/
void se_terminal::ButtonEnterDealFunc(void){
    switch(ICURRENTMODE){
    case CAMERCTLMODE://11—中间
        g_SendSTM[4] &= ~0x03;
        g_SendSTM[4] |= 0x03;
        break;
    case LIGHTCTLMODE://全关
       g_SendSTM[2] &= ~0x0f;
        break;
    case ROBOTCTLMODE://11—翻身
        g_SendSTM[4] &= ~0x0c;
        g_SendSTM[4] |= 0x0c;
        break;
    case MENUCTLMODE:
        if(MenuCurState == MENUSTATEHIDE){
            ZTCT->setPixmap(QPixmap("/Icon_TranRot/IconShow/MSZT.png"));
            MenuCurState = MENUSTATESHOW;
            MenuList->show();
            MenuList->setCurrentRow(OnelistFlag);
        }
        break;
    }
}

void se_terminal::ButtonDownRightLeftFunc(void){

}
/*****************************************************
挡位按键信号处理函数
*****************************************************/
void se_terminal::ButtonRightDownDealFunc(void){
    CararFlag++;
   if(CararFlag > 2)    CararFlag = 0;
    g_SendSTM[2] &= ~0xc0;
    g_SendSTM[2] |= CararFlag << 6;
}

int se_terminal::SearchDirFile(char *fileType){
    struct dirent **namelist;
    int i,n,len = 0,tmp;
    int RetNum = 0;
    int flag = 0;
    char TempStr[10] = {"0"};
    char *Strtok = NULL;
    n = scandir("/home/root/media/",&namelist,0,alphasort);
    for(i = 0;i < n;i++){
        len = strlen(namelist[i]->d_name);
        if(strcmp(&namelist[i]->d_name[len-4],fileType) == 0){
            flag = 1;
            strncpy(TempStr,namelist[i]->d_name,10);
            Strtok = strtok(TempStr,"_");
            Strtok = strtok(NULL,"_");
            tmp = atoi(Strtok);
        //    printf("Num:%d\n",tmp);
            if(tmp - RetNum > 1) break;
            else  RetNum = tmp;
        }
        free(namelist[i]);
    }
    free(namelist);
    if(flag) RetNum += 1;
    return RetNum;
}
void se_terminal::iWriteToFile(int AviNum,int BmpNum){
    FILE *fp;
    //char *MyStr;
    fp = fopen("/dm368/filecnt.ini","w+");
    if(fp == NULL){
        printf("open /dm368/filecnt.ini faild.\n");
        return ;
    }
    printf("BMPCNT=%d\nAVICNT=%d\n",BmpNum,AviNum);
    fprintf(fp,"BMPCNT=%d\nAVICNT=%d\n",BmpNum,AviNum);
    //fwrite(MyStr,1,128,fp);
    fclose(fp);
}

/*****************************************************
拍照录像按键信号处理函数
*****************************************************/
void se_terminal::ButtonLeftDownDealFunc(int Type){
   // int AviNum,BmpNum;
    if(SelectTF && !SelectTFFull){
      //  AviNum = SearchDirFile(".avi");
      //  BmpNum = SearchDirFile(".bmp");
        if(Type){
            if(FIFOCmd & 0x02){
                FIFOCmd &= ~0x02;
                msgServer.sendCmd(QtInterface_Stop);
                system("sync");
                ButtonSDPZLXName->setPixmap(QPixmap("/Icon_TranRot/IconShow/CALXDW.png"));
            }else{
               // iWriteToFile(AviNum,BmpNum);
                FIFOCmd |= 0x02;
                msgServer.sendCmd(QtInterface_Record);
                ButtonSDPZLXName->setPixmap(QPixmap("/Icon_TranRot/IconShow/CATXDW.png"));
            }
        }else{
          //  iWriteToFile(AviNum,BmpNum);
            FIFOCmd |=  0x01;
            MemTime = 1;
            msgServer.sendCmd(QtInterface_Photo);
        }
    }else{
      SelectTFLable->setText("SD卡错误");
    }
}
/*****************************************************
Qt抓屏
*****************************************************/
void se_terminal::slotActionSave()
{
    QPixmap pix = QPixmap::grabWidget(this,0,0,1280,720);
    pix.save("/home/root/media/pixture.png","png");
}
/*****************************************************
车体信息显示刷新
*****************************************************/
void  se_terminal::OsdUpdateFunc(int RetSt){

    // CarInformation
    // 俯仰 横滚 摆臂 温度 速度 电机温度 左转速 右转速  左电极电流 右电极电流  电池电压 速度挡位 航向
    if(RobotHandFlag == 1 && RetSt != 0) {
        ButtonUp->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonUp.png"));
        RobotHandFlag = 0;
    }
    if(RobotHandFlag == 2 && RetSt != 1){
        ButtonDown->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonDown.png"));
        RobotHandFlag = 0;
    }
    switch(RetSt){
    case 0:
        if(ICURRENTMODE == ROBOTCTLMODE || ICURRENTMODE == CAMERCTLMODE)
            RobotHandFlag = 1;
        else
            ButtonUp->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonUp.png"));
        break;
    case 1:
        if(ICURRENTMODE == ROBOTCTLMODE || ICURRENTMODE == CAMERCTLMODE)
            RobotHandFlag = 2;
        else
            ButtonDown->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonDown.png"));
        break;
    case 2:
         ButtonLeft->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonLeft.png"));
         break;
    case 3:
         ButtonRight->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonRight.png"));
         break;
    case 4:
         ButtonEnter->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonEnter.png"));
         break;
    case 5:
         ButtonDownRight->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonDownRight.png"));
         break;
    case 6:
        ButtonDownLeft->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonDownLeft.png"));
        break;
    case 9:
         ButtonRightDownRight->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonDownRight.png"));

        break;
    case 10:
         ButtonRightDownLeft->setPixmap(QPixmap("/Icon_TranRot/IconShow/ButtonDownLeft.png"));
         break;
    }

    if(MemTime) MemTime++;
    if(MemTime == 6){
        FIFOCmd &= ~0x01;
        MemTime = 0;
    }
    if(MenuCurState == MENUSTATEHIDE){
        QString   TmpString;
        if(g_iCarStateInfo.Pitch > 90) g_iCarStateInfo.Pitch = 90;
        if(g_iCarStateInfo.Pitch < -90) g_iCarStateInfo.Pitch = -90;
        TmpString  = "俯仰: " + QString("%0").arg(g_iCarStateInfo.Pitch) + "°";
        FuYangSt->setText(TmpString);

        if(g_iCarStateInfo.Roll > 180) g_iCarStateInfo.Roll = 180;
        if(g_iCarStateInfo.Roll < -180) g_iCarStateInfo.Roll = -180;
        TmpString  = "横滚: " + QString("%0").arg(g_iCarStateInfo.Roll) + "°";
        HeGunSt->setText(TmpString);
        if(g_iCarStateInfo.Dir > 360) g_iCarStateInfo.Dir = 360;
        if(g_iCarStateInfo.Dir < 0)   g_iCarStateInfo.Dir = 0;
        /*罗盘旋转*/
        int TmpState;
        TmpState = LPPointRoter;
        if(g_iCarStateInfo.Dir - TmpState != 0){
            pView->rotate(g_iCarStateInfo.Dir - TmpState);
            LPPointRoter = g_iCarStateInfo.Dir;
        }
        int SATmpState;
        SATmpState =  SAPointRoter;
        if(g_iCarStateInfo.BaiBiRot - SATmpState != 0){
            SApView->rotate(g_iCarStateInfo.BaiBiRot - SATmpState);
            SAPointRoter = g_iCarStateInfo.BaiBiRot;
         }
        TmpString  = "航向: " + QString("%0").arg(g_iCarStateInfo.Dir) + "°";
        HangXiangSt->setText(TmpString);

        if(g_iCarStateInfo.BaiBiRot > 360) g_iCarStateInfo.BaiBiRot = 360;
        if(g_iCarStateInfo.BaiBiRot < 0) g_iCarStateInfo.BaiBiRot = 0;
        TmpString  = "摆臂: " + QString("%0").arg(g_iCarStateInfo.BaiBiRot) + "°";
        BaiBiSt->setText(TmpString);
        TmpString  = "温度: " + QString("%0").arg(g_iCarStateInfo.HuanJingWenDu) + "℃";
        WeDuSt->setText(TmpString);
        TmpString  = "速度: " + QString("%0").arg(g_iCarStateInfo.CarSpeed) + "m/s";
        SuDuSt->setText(TmpString);
        switch(CararFlag){
        case 0:
            TmpString  = "挡位: 低";
            break;
        case 1:
            TmpString  = "挡位: 中";
            break;
        case 2:
            TmpString  = "挡位: 高";
            break;
        }
        SuDuDangWeiSt->setText(TmpString);

        TmpString  = "CCD角度: " + QString("%0").arg(g_iCarStateInfo.CameraAngle) + "°";
        CameraAngle->setText(TmpString);
    }
    /***************************灯光控制********************************/
    if(g_iCarStateInfo.YuJingDengFlag)
        LightIconY->setPixmap(QPixmap("/Icon_TranRot/IconShow/YLightNow.png"));
    else
        LightIconY->setPixmap(QPixmap("/Icon_TranRot/IconShow/SLight.png"));

    if(g_iCarStateInfo.QianDeng)
        LightIcon->setPixmap(QPixmap("/Icon_TranRot/IconShow/HLightNow.png"));
    else
        LightIcon->setPixmap(QPixmap("/Icon_TranRot/IconShow/HLight.png"));

    if(g_iCarStateInfo.ShangDeng)
        LightIconS->setPixmap(QPixmap("/Icon_TranRot/IconShow/SLightNow.png"));
    else
        LightIconS->setPixmap(QPixmap("/Icon_TranRot/IconShow/SLight.png"));

    if(g_iCarStateInfo.HouDeng)
        LightIconQ->setPixmap(QPixmap("/Icon_TranRot/IconShow/QLightNow.png"));
    else
        LightIconQ->setPixmap(QPixmap("/Icon_TranRot/IconShow/QLight.png"));

    if(g_iCarStateInfo.CheTiFanZhuan){
        CTFZ->show();
        if(ICURRENTMODE == ROBOTCTLMODE)
            ButtonKZName->setPixmap(QPixmap("/Icon_TranRot/IconShow/BBKZ.png"));
    }else{
        CTFZ->hide();
        if(ICURRENTMODE == ROBOTCTLMODE)
            ButtonKZName->setPixmap(QPixmap("/Icon_TranRot/IconShow/BBKZFW.png"));
    }

/***************************车体电量********************************/
    if(g_iCarStateInfo.CarPowerVal != CarPowerMen || g_iCarStateInfo.TerPowerVal != TerminalPowerMen){
        PowerValSelect(g_iCarStateInfo.CarPowerVal,CarPower);
        PowerValSelect(g_iCarStateInfo.TerPowerVal,TerminalPower);
        QString tmp;
        if(g_iCarStateInfo.CarPowerVal > 100) g_iCarStateInfo.CarPowerVal = 100;
        if(g_iCarStateInfo.TerPowerVal > 100) g_iCarStateInfo.TerPowerVal = 100;
        tmp.sprintf("%.2d\%",g_iCarStateInfo.CarPowerVal);
        CarPowerStr->setText(tmp);
        tmp.sprintf("%.2d\%",g_iCarStateInfo.TerPowerVal);
        TerminalPowerStr->setText(tmp);
        CarPowerMen = g_iCarStateInfo.CarPowerVal;
        TerminalPowerMen = g_iCarStateInfo.TerPowerVal;
    }

    if(DecodeFlag || PixtureFlag){
        if(FIFOCmd & 0x02)
             FIFOCmd &= ~0x02;
        system("sync");
        ButtonSDPZLXName->setPixmap(QPixmap("/Icon_TranRot/IconShow/CALXDW.png"));
    }
/*
    draw_event(m_ScanData, m_ScanDataLen);
    image_draw3d = new QImage(framedata, width_3d, height_3d, bytePerLine_3d, QImage::Format_RGB555);
    i3DPixShow->setPixmap(QPixmap::fromImage(*image_draw3d));
    i3DPixShow->show();
    delete image_draw3d;
*/
    // g_iCarStateInfo.Pitch (-90~90)        俯仰
    // g_iCarStateInfo.Roll(180~-180)        横滚

    int pitch,roll;
    int TmpPitch,TmpRoll;
    if(g_iCarStateInfo.Pitch < 0)
        TmpPitch  = (g_iCarStateInfo.Pitch*-1);
     else
        TmpPitch =  g_iCarStateInfo.Pitch + 90;
    if(g_iCarStateInfo.Roll < 0)
        TmpRoll  = (g_iCarStateInfo.Roll -180)*-1;
    else
        TmpRoll  = g_iCarStateInfo.Roll;


    if((g_iCarStateInfo.Pitch < 8 && g_iCarStateInfo.Pitch > 0) || (g_iCarStateInfo.Pitch > -8 && g_iCarStateInfo.Pitch < 0))
        TmpPitch = 90;
    if((g_iCarStateInfo.Roll < 15 && g_iCarStateInfo.Roll > 0) ||(g_iCarStateInfo.Roll > -15 && g_iCarStateInfo.Roll < 0))
        TmpRoll = 0;

 //  pitch = TmpPitch  - TmpPitch%15;
  //  roll =  TmpRoll  - TmpRoll%30;
    i3DSelectApproximateRote(TmpPitch,TmpRoll,&pitch,&roll);
  //  printf("g_iCarStateInfo.Pitch:%d,g_iCarStateInfo.Roll:%d,TmpPitch:%d,TmpRoll:%d\n",g_iCarStateInfo.Pitch,g_iCarStateInfo.Roll,TmpPitch,TmpRoll);
  //  printf("pitch:%d,roll:%d\n",pitch,roll);
     if(MemPitch != pitch || MenRoll != roll){
      //  printf("pitch:%d roll:%d\n",pitch,roll);
        TestStr.sprintf("/Icon_TranRot/CarStat/%d%d.png",pitch,roll);
  //      printf("TestStr:%s\n",TestStr);
        i3DPixShow->setPixmap(QPixmap(TestStr));
        MemPitch = pitch;
        MenRoll = roll;
    }

    if(g_iCarStateInfo.WaringFlag & 0x03){
        IWaringGWFlag = 1;
        IWaringGW->show();
        IWaringGWCount++;
        if(IWaringGWCount == 8){
            IWaringGW->hide();
            IWaringGWCount = 0;
        }
    }else{
        if(IWaringGWFlag){
         IWaringGW->hide();
         IWaringGWFlag= 0;
        }
    }

    if(g_iCarStateInfo.WaringFlag & 0x0c){
        IWaringFlag = 1;
        IWaring->show();
        IWaringCount++;
        if(IWaringCount == 8){
            IWaring->hide();
            IWaringCount = 0;
        }
    }else{
        if(IWaringFlag){
         IWaring->hide();
         IWaringFlag= 0;
        }
    }
}
/*****************************************************
3D_Select
*****************************************************/
void se_terminal::i3DSelectApproximateRote(int Pitch,int Roll,int *iPitch,int *iRoll)
{
    if(Pitch <= 7)                      *iPitch = 0;
    else if(Pitch > 7  && Pitch <= 23)  *iPitch = 15;
    else if(Pitch > 23 && Pitch <= 38)  *iPitch = 30;
    else if(Pitch > 38 && Pitch <= 53)  *iPitch = 45;
    else if(Pitch > 53 && Pitch <= 68)  *iPitch = 60;
    else if(Pitch > 68 && Pitch <= 83)  *iPitch = 75;
    else if(Pitch > 83 && Pitch <= 98)  *iPitch = 90;
    else if(Pitch > 98 && Pitch <= 113) *iPitch = 105;
    else if(Pitch > 113&& Pitch <= 128) *iPitch = 120;
    else if(Pitch > 128&& Pitch <= 143) *iPitch = 135;
    else if(Pitch > 143&& Pitch <= 158) *iPitch = 150;
    else if(Pitch > 158&& Pitch <= 173) *iPitch = 165;
    else if(Pitch > 173)               *iPitch = 180;

    if(Roll <= 15)                       *iRoll = 0;
    else if(Roll > 15 && Roll <= 45 )    *iRoll = 30;
    else if(Roll > 45 && Roll <= 75 )    *iRoll = 60;
    else if(Roll > 75 && Roll <= 105)    *iRoll = 90;
    else if(Roll > 105&& Roll <= 135)    *iRoll = 120;
    else if(Roll > 135&& Roll <= 165)    *iRoll = 150;
    else if(Roll > 165&& Roll <= 195)    *iRoll = 180;
    else if(Roll > 195&& Roll <= 225)    *iRoll = 210;
    else if(Roll > 225&& Roll <= 255)    *iRoll = 240;
    else if(Roll > 255&& Roll <= 285)    *iRoll = 270;
    else if(Roll > 285&& Roll <= 315)    *iRoll = 300;
    else if(Roll > 315&& Roll <= 345)    *iRoll = 330;
    else if(Roll > 345)                 *iRoll = 360;
}
/*****************************************************
电量显示选择
*****************************************************/
void se_terminal::PowerValSelect(int vale,QLabel *lable){
    if(vale < 2)
        lable->setPixmap(QPixmap("/Icon_TranRot/power/10.png"));
    else if(vale > 1 && vale <= 20)
        lable->setPixmap(QPixmap("/Icon_TranRot/power/20.png"));
    else if(vale > 20 && vale <= 40)
        lable->setPixmap(QPixmap("/Icon_TranRot/power/40.png"));
    else if(vale > 40 && vale <= 60)
        lable->setPixmap(QPixmap("/Icon_TranRot/power/60.png"));
    else if(vale > 60 && vale <= 80)
        lable->setPixmap(QPixmap("/Icon_TranRot/power/80.png"));
    else if(vale > 80 && vale <= 100)
        lable->setPixmap(QPixmap("/Icon_TranRot/power/100.png"));
}

/*****************************************************
音频设置输入输出
*****************************************************/
int se_terminal::SetGPIO(int value)  //write GPIO22
{
    fd_gpio = open("/dev/dm368_gpio", O_RDWR);
    if (fd_gpio < 0){
        printf("Can't open device /dev/dm368_gpio\r\n");
    } else {
        gpiobuf[0] = value;
        write(fd_gpio, gpiobuf, 2);
    }
    // UsartClassThr::close_fd(fd_gpio);
    return 0;
}

/*****************************************************
音频 输入
*****************************************************/
void se_terminal::SET_AUDIO_BYPASS()
{
    SetGPIO(0);
}

/*****************************************************
音频 输出
*****************************************************/
void se_terminal::SET_AUDIO_368OUT()
{
    SetGPIO(1);
}
