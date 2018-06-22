#include "sec_terminal.h"
#include "ui_sec_terminal.h"

Sec_Terminal::Sec_Terminal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sec_Terminal)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    msgServer.start(QThread::HighPriority);
    RectGround = new QLabel(this);
    RectGround->resize(1280,720);
    RectGround->setPixmap(QPixmap("/Sec_Picture/BackGround.png"));
    RectGround->move(0,0);

    LittleRectGround = new QLabel(this);
    LittleRectGround->resize(288,587);
    LittleRectGround->setPixmap(QPixmap("/Sec_Picture/LittleBack.png"));
    LittleRectGround->move(998,77);

    RectTitle = new QLabel(this);
    RectTitle->resize(1280,50);
    RectTitle->setPixmap(QPixmap("/Sec_Picture/TitlePanel.png"));
    RectTitle->move(0,0);

    SpeedWatch = new QLabel(this);
    SpeedWatch->resize(257,257);
    SpeedWatch->setPixmap(QPixmap("/Sec_Picture/SpeedWatch.png"));
    SpeedWatch->move(1012,138);

    SpeedPoint = new QLabel;
    SpeedPoint->resize(42,164);
    SpeedPoint->setPixmap(QPixmap("/Sec_Picture/SpeedPoint.png"));
    SpeedPoint->move(1038,192);
    SpeedPoint->setStyleSheet("background: transparent;border:0px");

    MBpScene = new QGraphicsScene;
    MBpScene->addWidget(SpeedPoint);
    MBpView = new QGraphicsView(MBpScene, this);
    MBpView->setStyleSheet( "background: transparent;border:0px");
    MBpView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    MBpView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    MBpView->setGeometry(1057,192,164,164);//1019

    Direction = new QLabel(this);
    Direction->resize(183,183);
    Direction->setPixmap(QPixmap("/Sec_Picture/Direction.png"));
    Direction->move(1044,433);

    DirPoint = new QLabel;
    DirPoint->resize(12,106);
    DirPoint->setPixmap(QPixmap("/Sec_Picture/DirPoint.png"));
    DirPoint->move(1130,472);
    DirPoint->setStyleSheet( "background: transparent;border:0px");

    HXpPointRoter = 0;
    HXpScene = new QGraphicsScene;
    HXpScene->addWidget(DirPoint);
    HXpView = new QGraphicsView(HXpScene, this);
    HXpView->setStyleSheet( "background: transparent;border:0px");
    HXpView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    HXpView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    HXpView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    HXpView->setGeometry(1080,472,106,106);//1019


    ShortKey_0 = new QLabel(this);
    ShortKey_0->resize(94,48);
    ShortKey_0->setPixmap(QPixmap("/Sec_Picture/ShortKey.png"));
    ShortKey_0->move(160,668);

    int tmp_iX = 326;
    ShortKey_1 = new QLabel(this);
    ShortKey_1->resize(94,48);
    ShortKey_1->setPixmap(QPixmap("/Sec_Picture/ShortKey.png"));
    ShortKey_1->move(tmp_iX,668);

    ShortKey_2 = new QLabel(this);
    ShortKey_2->resize(94,48);
    ShortKey_2->setPixmap(QPixmap("/Sec_Picture/ShortKey.png"));
    tmp_iX += 180;
    ShortKey_2->move(tmp_iX,668);

    ShortKey_3 = new QLabel(this);
    ShortKey_3->resize(94,48);
    ShortKey_3->setPixmap(QPixmap("/Sec_Picture/ShortKey.png"));
    tmp_iX += 180;
    ShortKey_3->move(tmp_iX,668);

    ShortKey_4 = new QLabel(this);
    ShortKey_4->resize(94,48);
    ShortKey_4->setPixmap(QPixmap("/Sec_Picture/ShortKey.png"));
    tmp_iX += 180;
    ShortKey_4->move(tmp_iX,668);

    ShortKey_5 = new QLabel(this);
    ShortKey_5->resize(94,48);
    ShortKey_5->setPixmap(QPixmap("/Sec_Picture/ShortKey.png"));
    tmp_iX += 180;
    ShortKey_5->move(tmp_iX,668);

    TerminalPower = new QLabel(this);
    TerminalPower->move(1214,10);
    TerminalPower->setPixmap(QPixmap("/Sec_Picture/Power/100.png"));

/*****************************文字*******************************************/
    CamerCtl = new QLabel(this);
    CamerCtl->resize(945,28);
    CamerCtl->setPixmap(QPixmap("/Sec_Picture/CamerCtl.png"));
    CamerCtl->move(177,677);
/****************************灯光*******************************************/

    LightIconQ = new QLabel(this);
    LightIconQ->resize(70,70);
    LightIconQ->setPixmap(QPixmap("/Sec_Picture/Light/QLightNow.png"));
 //   LightIconQ->setScaledContents(true);
    LightIconQ->move(1010,86);

    LightIconH = new QLabel(this);
    LightIconH->resize(70,70);
    LightIconH->setPixmap(QPixmap("/Sec_Picture/Light/HLightNow.png"));
   // LightIconH->setScaledContents(true);
    LightIconH->move(1196,88);

    LightIconS = new QLabel(this);
    LightIconS->resize(70,70);
    LightIconS->setPixmap(QPixmap("/Sec_Picture/Light/SLightNow.png"));
   // LightIconS->setScaledContents(true);
    LightIconS->move(1008,380);

    LightIconY = new QLabel(this);
    LightIconY->resize(70,70);
    LightIconY->setPixmap(QPixmap("/Sec_Picture/Light/YLightNow.png"));
   // LightIconY->setScaledContents(true);
    LightIconY->move(1198,382);

/**************************参数显示**********************************************/

    CarStateArgc = new QLabel(this);
    CarStateArgc->resize(140,28);
    CarStateArgc->move(90,100);
    CarStateArgc->setAlignment(Qt::AlignLeft);
    CarStateArgc->setFont(QFont("JPTZ",26,QFont::AnyStyle));
    CarStateArgc->setStyleSheet("QLabel{background:transparent;color:white;};");
    CarStateArgc->setText("车体参数");

    int tmp_iY = 140;
    FuYangSt= new QLabel(this);
    FuYangSt->resize(140,24);
    FuYangSt->move(60,tmp_iY);
    FuYangSt->setAlignment(Qt::AlignLeft);
    FuYangSt->setFont(QFont("JPTZ",22,QFont::AnyStyle));
    FuYangSt->setStyleSheet("QLabel{background:transparent;color:white;};");
    FuYangSt->setText("俯仰:   100°");

    tmp_iY += 32;
    HeGunSt = new QLabel(this);
    HeGunSt->resize(140,24);
    HeGunSt->move(60,tmp_iY);
    HeGunSt->setAlignment(Qt::AlignLeft);
    HeGunSt->setFont(QFont("JPTZ",22,QFont::AnyStyle));
    HeGunSt->setStyleSheet("QLabel{background:transparent;color:white;};");
    HeGunSt->setText("横滚:   100°");

    tmp_iY += 32;
    HangXiangSt = new QLabel(this);
    HangXiangSt->resize(140,24);
    HangXiangSt->move(60,tmp_iY);
    HangXiangSt->setAlignment(Qt::AlignLeft);
    HangXiangSt->setFont(QFont("JPTZ",22,QFont::AnyStyle));
    HangXiangSt->setStyleSheet("QLabel{background:transparent;color:white;};");
    HangXiangSt->setText("航向:   100°");
//#if 0
    tmp_iY += 32;
    BaiBiSt = new QLabel(this);
    BaiBiSt->resize(140,24);
    BaiBiSt->move(60,tmp_iY);
    BaiBiSt->setAlignment(Qt::AlignLeft);
    BaiBiSt->setFont(QFont("JPTZ",22,QFont::AnyStyle));
    BaiBiSt->setStyleSheet("QLabel{background:transparent;color:white;};");
    BaiBiSt->setText("摆臂:   100°");
//#endif
    tmp_iY += 32;
    WeDuSt  = new QLabel(this);
    WeDuSt->resize(200,24);
    WeDuSt->move(60,tmp_iY);
    WeDuSt->setAlignment(Qt::AlignLeft);
    WeDuSt->setFont(QFont("JPTZ",22,QFont::AnyStyle));
    WeDuSt->setStyleSheet("QLabel{background:transparent;color:white;};");
    WeDuSt->setText("温度:   -30℃");

    tmp_iY += 32;
    SuDuDangWeiSt  = new QLabel(this);
    SuDuDangWeiSt->resize(140,24);
    SuDuDangWeiSt->move(60,tmp_iY);
    SuDuDangWeiSt->setAlignment(Qt::AlignLeft);
    SuDuDangWeiSt->setFont(QFont("JPTZ",22,QFont::AnyStyle));
    SuDuDangWeiSt->setStyleSheet("QLabel{background:transparent;color:white;};");
    SuDuDangWeiSt->setText("挡位:   高");

    tmp_iY += 32;
    CameraAngle  = new QLabel(this);
    CameraAngle->resize(200,24);
    CameraAngle->move(60,tmp_iY);
    CameraAngle->setAlignment(Qt::AlignLeft);
    CameraAngle->setFont(QFont("JPTZ",22,QFont::AnyStyle));
    CameraAngle->setStyleSheet("QLabel{background:transparent;color:white;};");
    CameraAngle->setText("CCD角度:   100°");

    tmp_iY += 32;
    SuDuSt  = new QLabel(this);
    SuDuSt->resize(140,24);
    SuDuSt->move(60,tmp_iY);
    SuDuSt->setAlignment(Qt::AlignLeft);
    SuDuSt->setFont(QFont("JPTZ",22,QFont::AnyStyle));
    SuDuSt->setStyleSheet("QLabel{background:transparent;color:white;};");
    SuDuSt->setText("速度:   2.5m/s");

 #if 0
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()), this, SLOT(timerDone()) );
    timer->start(2000); // 2秒单触发定时器
 #endif
 /****************************菜单显示****************************************/

    MenuList = new QListWidget(this);
    MenuList->resize(200,220);
    MenuList->move(40,440);
    MenuList->setIconSize(QSize(46,34));
    //光感 放大 触屏
    MenuList->addItem(new QListWidgetItem(QIcon("/Sec_Picture/VideoIco.png"),tr("    视频")));
    MenuList->addItem(new QListWidgetItem(QIcon("/Sec_Picture/PhotoIco.png"),tr("    图片")));
    MenuList->addItem(new QListWidgetItem(QIcon("/Sec_Picture/EnLarge.png") ,tr("    缩放")));
    MenuList->addItem(new QListWidgetItem(QIcon("/Sec_Picture/GMIco.png")   ,tr("   光敏")));
    MenuList->setFont(QFont("JPTZ",28,QFont::AnyStyle));
    MenuList->setStyleSheet("QListWidget{background-color:rgb(106,106,106);border:6px solid gray;border-color:rgb(0,0,0);border-radius:10px}"
                            "QListWidget::Item{padding-top:8px; padding-bottom:8px; }");
                        //"QListWidget::item:selected:!active{border-width:0px; background:lightgreen; color:red;}");
    MenuList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); 	//垂直方向
    MenuList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); 		//水平方向
    MenuListFlag = 0;
    MenuList->setCurrentRow(MenuListFlag);

/****************************按键线程****************************************/
    iButtonMsg = new KeyDel;
    // iButtonMsg->setPriority(QThread::TimeCriticalPriority);
    iButtonMsg ->start();
   connect(iButtonMsg,SIGNAL(PhyButtonMsg(int,int)),this,SLOT(PhyButtonDeal(int,int)));
   connect(iButtonMsg,SIGNAL(OsdUpdate(int)),this,SLOT(OsdUpdateFunc(int)));

/**************************视频调用**********************************************/
    SET_AUDIO_BYPASS();
    system("./encodertsp -v 1.264 -a 1.aac -y 2 -I 1 &");
    ui->setupUi(this);
    tmp = 0;
}

Sec_Terminal::~Sec_Terminal()
{
    delete ui;
}


void Sec_Terminal::timerDone(void){
    slotActionSave();
    system("sync");
    printf("Timer OutTimer.\n");
    timer->stop();
}


/*****************************************************
物理按键处理函数
*****************************************************/
void  Sec_Terminal::PhyButtonDeal(int Num,int Type){
    switch(Num){
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:                             //Select Mode
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;
    case 11:
        break;
    }
}

void  Sec_Terminal::OsdUpdateFunc(int RetSt){
    printf("--------------OsdUpdateFunc-------------------\n");

    tmp +=20;
    if(tmp > 100)
    tmp = 20;
    QString  str;
    str.sprintf("/Sec_Picture/Power/%d.png",tmp);
    TerminalPower->setPixmap(QPixmap(str));

   // HXpPointRoter += 10;
   // if(HXpPointRoter > 350)
  //      HXpPointRoter = 0;
      HXpView->rotate(10);
      MBpView->rotate(10);


}

/*****************************************************
Qt抓屏
*****************************************************/
void Sec_Terminal::slotActionSave()
{
    QPixmap pix = QPixmap::grabWidget(this,0,0,1280,720);
    pix.save("/home/root/media/pixture.png","png");
}

/*****************************************************
音频设置输入输出
*****************************************************/
int Sec_Terminal::SetGPIO(int value)  //write GPIO22
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
void Sec_Terminal::SET_AUDIO_BYPASS()
{
    SetGPIO(0);
}

/*****************************************************
音频 输出
*****************************************************/
void Sec_Terminal::SET_AUDIO_368OUT()
{
    SetGPIO(1);
}
