#include "sec_terminal.h"
#include "ui_sec_terminal.h"

Sec_Terminal::Sec_Terminal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sec_Terminal)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    msgServer.start(QThread::HighPriority);
    PauseFlag = 0;
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

/*****************************速度表盘****************************************/
    SpeedWatch = new QLabel(this);
    SpeedWatch->resize(216,216);
    SpeedWatch->setScaledContents(true);
    SpeedWatch->setPixmap(QPixmap("/Sec_Picture/SpeedWatch.png"));
    SpeedWatch->move(1018,91);

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
    MBpView->setGeometry(1052,115,164,164);//(1058,186,164,164);//1019

    SpeedDescription = new QLabel(this);
    SpeedDescription->resize(90,25);
    SpeedDescription->setFont(QFont("JPTZ",18,QFont::AnyStyle));
    SpeedDescription->setStyleSheet("QLabel{ color:white;};");
    SpeedDescription->move(1085,254);
    SpeedDescription->setText("0.0 km/h");
/*****************************航向表盘****************************************/
    Direction = new QLabel(this);
    Direction->resize(124,124);
    Direction->setScaledContents(true);
    Direction->setPixmap(QPixmap("/Sec_Picture/Direction.png"));
    Direction->move(1072,466);

    DirPoint = new QLabel;
    DirPoint->resize(12,106);
    DirPoint->setPixmap(QPixmap("/Sec_Picture/DirPoint.png"));
    DirPoint->move(1130,474);
    DirPoint->setStyleSheet( "background: transparent;border:0px");

    HXpPointRoter = 0;
    HXpScene = new QGraphicsScene;
    HXpScene->addWidget(DirPoint);
    HXpView = new QGraphicsView(HXpScene, this);
    HXpView->setStyleSheet( "background: transparent;border:0px");
    HXpView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    HXpView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    HXpView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    HXpView->setGeometry(1082,474,106,106);//1019

    int tmp_iX = 100;
    ShortKey_0 = new QLabel(this);
    ShortKey_0->resize(94,48);
    ShortKey_0->setPixmap(QPixmap("/Sec_Picture/ShortKey.png"));
    ShortKey_0->move(tmp_iX,660);


    ShortKey_1 = new QLabel(this);
    ShortKey_1->resize(94,48);
    ShortKey_1->setPixmap(QPixmap("/Sec_Picture/ShortKey.png"));
    tmp_iX += 160;
    ShortKey_1->move(tmp_iX,660);

    ShortKey_2 = new QLabel(this);
    ShortKey_2->resize(94,48);
    ShortKey_2->setPixmap(QPixmap("/Sec_Picture/ShortKey.png"));
    tmp_iX += 160;
    ShortKey_2->move(tmp_iX,660);

    ShortKey_3 = new QLabel(this);
    ShortKey_3->resize(94,48);
    ShortKey_3->setPixmap(QPixmap("/Sec_Picture/ShortKey.png"));
    tmp_iX += 160;
    ShortKey_3->move(tmp_iX,660);

    ShortKey_4 = new QLabel(this);
    ShortKey_4->resize(94,48);
    ShortKey_4->setPixmap(QPixmap("/Sec_Picture/ShortKey.png"));
    tmp_iX += 160;
    ShortKey_4->move(tmp_iX,660);

    ShortKey_5 = new QLabel(this);
    ShortKey_5->resize(94,48);
    ShortKey_5->setPixmap(QPixmap("/Sec_Picture/ShortKey.png"));
    tmp_iX += 160;
    ShortKey_5->move(tmp_iX,660);

    TerminalPower = new QLabel(this);
    TerminalPower->move(1214,10);
    TerminalPower->setPixmap(QPixmap("/Sec_Picture/Power/100.png"));

/*****************************文字*******************************************/
    ModeStatePic = new QLabel(this);
    ModeStatePic->resize(945,28);
 //   ModeStatePic->setPixmap(QPixmap("/Sec_Picture/CamerCtl.png"));
    ModeStatePic->move(177,677);
/****************************灯光*******************************************/

    LightIconQ = new QLabel(this);
    LightIconQ->resize(50,50);
    LightIconQ->setPixmap(QPixmap("/Sec_Picture/Light/QLight.png"));
    LightIconQ->setScaledContents(true);
    LightIconQ->move(1017,557);

    LightIconH = new QLabel(this);
    LightIconH->resize(50,50);
    LightIconH->setPixmap(QPixmap("/Sec_Picture/Light/HLight.png"));
    LightIconH->setScaledContents(true);
    LightIconH->move(1196,558);

    LightIconS = new QLabel(this);
    LightIconS->resize(50,50);
    LightIconS->setPixmap(QPixmap("/Sec_Picture/Light/SLight.png"));
    LightIconS->setScaledContents(true);
    LightIconS->move(1074,606);

    LightIconY = new QLabel(this);
    LightIconY->resize(50,50);
    LightIconY->setPixmap(QPixmap("/Sec_Picture/Light/YLight.png"));
    LightIconY->setScaledContents(true);
    LightIconY->move(1138,608);

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
    CameraAngle  = new QLabel(this);
    CameraAngle->resize(200,24);
    CameraAngle->move(60,tmp_iY);
    CameraAngle->setAlignment(Qt::AlignLeft);
    CameraAngle->setFont(QFont("JPTZ",22,QFont::AnyStyle));
    CameraAngle->setStyleSheet("QLabel{background:transparent;color:white;};");
    CameraAngle->setText("CCD角度:   100°");

    tmp_iY += 32;
    SuDuDangWeiSt  = new QLabel(this);
    SuDuDangWeiSt->resize(140,24);
    SuDuDangWeiSt->move(60,tmp_iY);
    SuDuDangWeiSt->setAlignment(Qt::AlignLeft);
    SuDuDangWeiSt->setFont(QFont("JPTZ",22,QFont::AnyStyle));
    SuDuDangWeiSt->setStyleSheet("QLabel{background:transparent;color:white;};");
    SuDuDangWeiSt->setText("挡位: 低");

    tmp_iY += 32;
    SuDuSt  = new QLabel(this);
    SuDuSt->resize(180,24);
    SuDuSt->move(60,tmp_iY);
    SuDuSt->setAlignment(Qt::AlignLeft);
    SuDuSt->setFont(QFont("JPTZ",22,QFont::AnyStyle));
    SuDuSt->setStyleSheet("QLabel{background:transparent;color:white;};");
   // SuDuSt->setText("速度:   2.5m/s");

    FangDaLab  = new QLabel(this);
    FangDaLab->resize(160,24);
    FangDaLab->move(1040,340);
    FangDaLab->setAlignment(Qt::AlignLeft);
    FangDaLab->setFont(QFont("JPTZ",22,QFont::AnyStyle));//,QFont::Bold));
    FangDaLab->setStyleSheet("QLabel{background:transparent;color:white;};");
    FangDaLab->setText("放大倍数: X1.0");

    GuangMinLab  = new QLabel(this);
    GuangMinLab->resize(160,24);
    GuangMinLab->move(1040,370);
    GuangMinLab->setAlignment(Qt::AlignLeft);
    GuangMinLab->setFont(QFont("JPTZ",22,QFont::AnyStyle));//QFont::Bold));
    GuangMinLab->setStyleSheet("QLabel{background:transparent;color:white;};");
    GuangMinLab->setText("灯光控制: 自动");

 #if 0
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()), this, SLOT(timerDone()) );
    timer->start(5000); // 2秒单触发定时器
 #endif
 /****************************菜单显示****************************************/
    int MenX = -8,MenY = -20;
    MenuList = new QListWidget(this);
    MenuList->resize(200,220);
    MenuList->move(44 + MenX,440 + MenY);
    MenuList->setIconSize(QSize(36,28));
    //光感 放大 触屏
    MenuList->addItem(new QListWidgetItem(QIcon("/Sec_Picture/VideoIco.png"),tr("      视频")));
    MenuList->addItem(new QListWidgetItem(QIcon("/Sec_Picture/PhotoIco.png"),tr("      图片")));
    MenuList->addItem(new QListWidgetItem(QIcon("/Sec_Picture/EnLarge.png") ,tr("      缩放")));
    MenuList->addItem(new QListWidgetItem(QIcon("/Sec_Picture/GMIco.png")   ,tr("     光敏")));
    MenuList->setFont(QFont("JPTZ",26,QFont::AnyStyle));
    MenuList->setStyleSheet("QListWidget{background-color:rgb(160,160,160);border:8px solid gray;\
                                         border-color:rgb(80,80,80);border-radius:16px}"
                            "QListWidget::Item{padding-top:9px; padding-bottom:9px;}");
                        //"QListWidget::item:selected:!active{border-width:0px; background:lightgreen; color:red;}");
    MenuList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); 	//垂直方向
    MenuList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); 		//水平方向
    MenuListFlag = 0;
    MenuList->setCurrentRow(MenuListFlag);
   // MenuList->hide();

    MenuStateFlag = MENUSTATESHOW;

    int SecMenX = -8,SecMenY = -20;
    SecMenuList = new QListWidget(this);
    SecMenuList->resize(200,220);
    SecMenuList->move(44 + SecMenX,440 + SecMenY);
    SecMenuList->setIconSize(QSize(36,28));
    //光感 放大 触屏
    SecMenuList->setFont(QFont("JPTZ",26,QFont::AnyStyle));
    SecMenuList->setStyleSheet("QListWidget{background-color:rgb(160,160,160);border:8px solid gray;\
                                         border-color:rgb(80,80,80);border-radius:16px}"
                            "QListWidget::Item{padding-top:9px; padding-bottom:9px;}");

                        //"QListWidget::item:selected:!active{border-width:0px; background:lightgreen; color:red;}");
    SecMenuList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); 	//垂直方向
    SecMenuList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); 		//水平方向

    SecMenuList->hide();
    SeclistFlag = 0;
    MenuDes  = new QLabel(this);
    MenuDes->resize(208,228);
    MenuDes->move(38 + MenX,438 + MenY);
    MenuDes->setPixmap(QPixmap("/Sec_Picture/MenuDes.png"));
   // MenuDes->hide();
/****************************按键线程****************************************/
    iButtonMsg = new KeyDel;
    // iButtonMsg->setPriority(QThread::TimeCriticalPriority);
    iButtonMsg ->start();
   connect(iButtonMsg,SIGNAL(PhyButtonMsg(int,int)),this,SLOT(PhyButtonDeal(int,int)));
   connect(iButtonMsg,SIGNAL(OsdUpdate(int)),this,SLOT(OsdUpdateFunc(int)));

/**************************视频调用**********************************************/
    SET_AUDIO_BYPASS();
    system("./encodertsp -v 1.264 -a 1.aac -y 2 -I 1 &");
    sleep(3);
    ui->setupUi(this);
    CararFlag       = 0;
    VideoOrPicFlag  = 0;
    FangDaOrGuangMi = 0;
    FIFOCmd         = 0;
    GuanMiZhiWei    = 0;
    LongPressFlag   = 0;
    TerminalPowerMen= 0;
    MemMBpView      = 125;
    ICURRENTMODE    = CAMERCTLMODE;

    fileview = new FileViewer(this);
    fileview->setWindowFlags(Qt::FramelessWindowHint);
    fileview->setGeometry(255, 80, 720, 576);
    fileview->hide();

    ShowPhoto = new QLabel(this);
    ShowPhoto->resize(736,576);
    ShowPhoto->move(261,80);
  //  ShowPhoto->setPixmap(QPixmap("/Icon_TranRot/IconShow/HelpRead.png"));
    ShowPhoto->hide();
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

int Sec_Terminal::MenuOneShow(void){
    if(MenuStateFlag == MENUSTATEHIDE){
        MenuList->show();
        MenuDes->show();
        MenuStateFlag = MENUSTATESHOW;
        return 1;
    }
    return 0;
}

/*****************************************************
判断是否为空文件
*****************************************************/
int Sec_Terminal::IsEmptyFile(char *file_name){
    if(!file_name) return 1;
    struct stat stat;
    char tmp[128];
    memset(tmp,0,128);
    sprintf(tmp,"/home/root/media/%s",file_name);
    lstat(tmp, &stat);
    if(stat.st_size < 20)
          return 1;
    return 0;
}

/*****************************************************
文件列表窗口清空
*****************************************************/
void Sec_Terminal::MenuKeyUP(void){
    int sum;
//    if(MenuOneShow())  return;
    switch(MenuStateFlag){
        case MENUSTATESHOW:
            sum = MenuList->count();
            if((--MenuListFlag) < 0) MenuListFlag = sum - 1;
            MenuList->setCurrentRow(MenuListFlag);
            break;
        case MENUSTATESEC :
            fileview->prevrow();
            break;
        case MENUSTATSETING :
            sum = SecMenuList->count();
            if((--SeclistFlag) < 0) SeclistFlag = sum - 1;
            SecMenuList->setCurrentRow(SeclistFlag);
            break;
        case MENUSTATLOOKPHOTO :
            fileview->prevrow();
            if(!IsEmptyFile(fileview->GetFileName())){
                memset(cmdbuf,0,128);
                sprintf(cmdbuf, "/home/root/media/%s",fileview->GetFileName());
                ShowPhoto->setPixmap(QPixmap(cmdbuf));
            }
            break;
    }
}

void Sec_Terminal::MenuKeyDown   (void){
    int sum;
   // if(MenuOneShow())  return;
    switch(MenuStateFlag){
        case MENUSTATESHOW:
            sum = MenuList->count();
            if((++MenuListFlag) > sum - 1) MenuListFlag = 0;
            MenuList->setCurrentRow(MenuListFlag);
            break;
        case MENUSTATESEC :
            fileview->nextrow();
            break;
        case MENUSTATSETING :
            sum = SecMenuList->count();
            if((++SeclistFlag) > sum - 1) SeclistFlag = 0;
            SecMenuList->setCurrentRow(SeclistFlag);
            break;
        case MENUSTATLOOKPHOTO :
           fileview->nextrow();
           if(!IsEmptyFile(fileview->GetFileName())){
               memset(cmdbuf,0,128);
               sprintf(cmdbuf, "/home/root/media/%s",fileview->GetFileName());
               ShowPhoto->setPixmap(QPixmap(cmdbuf));
           }
         break;
    }
}

void Sec_Terminal::MenuKeyEnter(void){
   // if(MenuOneShow())  return;
    switch(MenuStateFlag){
        case MENUSTATESHOW:
            switch(MenuListFlag){
                case 0:
                        printf("MENUSTATESEC 0 :pkill -9  encodertsp\n\n");
                        fflush(NULL);
                        FangDaLab->hide();
                        GuangMinLab->hide();
                        system("pkill -9 encodertsp");
                        fileview->iDirFileToLink(".avi");
                        fileview->showFileInfoList(10);
                        fileview->show();
                        MenuStateFlag =  MENUSTATESEC;
                        break;
                case 1:
                        printf("MENUSTATESEC 0 :pkill -9  encodertsp\n\n");
                        fflush(NULL);
                        FangDaLab->hide();
                        GuangMinLab->hide();
                        system("pkill -9 encodertsp");
                        fileview->iDirFileToLink(".jpg");
                        fileview->showFileInfoList(10);
                        fileview->show();
                        VideoOrPicFlag = 1;
                        MenuStateFlag =  MENUSTATESEC;
                        break;
                case 2:
                        SecMenuList->show();
                        SecMenuList->addItem(new QListWidgetItem("        X 1.0"));
                        SecMenuList->addItem(new QListWidgetItem("        X 1.5"));
                        SecMenuList->addItem(new QListWidgetItem("        X 2.0"));
                        SecMenuList->addItem(new QListWidgetItem("        X 2.5"));
                        SecMenuList->addItem(new QListWidgetItem("        X 3.0"));
                        SecMenuList->addItem(new QListWidgetItem("        X 3.5"));
                        SecMenuList->addItem(new QListWidgetItem("        X 4.0"));
                        SecMenuList->setCurrentRow(SeclistFlag);
                        MenuStateFlag =  MENUSTATSETING;
                        break;
                case 3:
                        SecMenuList->show();
                        SecMenuList->addItem(new QListWidgetItem("         打开")); //打开 关闭
                        SecMenuList->addItem(new QListWidgetItem("         关闭"));
                        SecMenuList->setCurrentRow(SeclistFlag);
                        FangDaOrGuangMi = 1;
                        MenuStateFlag =  MENUSTATSETING;
                        break;
             }
            break;
        case MENUSTATESEC :
            if(!VideoOrPicFlag){
                memset(cmdbuf,0,128);
                if(!IsEmptyFile(fileview->GetFileName())){
                    sprintf(cmdbuf, "./decode -y 2 -v h264 -f /home/root/media/%s  &",fileview->GetFileName());
                    system(cmdbuf);
                    SET_AUDIO_368OUT();
                    MenuStateFlag = MENUSTATPLAYING;
                    fileview->hide();
                }
            }else{
                if(!IsEmptyFile(fileview->GetFileName())){
                    memset(cmdbuf,0,128);
                    sprintf(cmdbuf, "/home/root/media/%s",fileview->GetFileName());
                    ShowPhoto->setPixmap(QPixmap(cmdbuf));
                    ShowPhoto->show();
                    fileview->hide();
                    MenuStateFlag = MENUSTATLOOKPHOTO;
                 }
            }      break;
        case MENUSTATSETING :
                if(!FangDaOrGuangMi){
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
                    FangDaLab->setText(BlowUpssStr);
                    g_SendSTM[3] &= ~0x1F;
                    g_SendSTM[3] |= (BlowUpssFlag - 10);
                }else{
                    if(SeclistFlag){
                        g_SendSTM[2] &= ~0x20;              //光感
                        GuangMinLab->setText("手动");
                        GuanMiZhiWei = 0;
                    }else{
                        g_SendSTM[2] |= 0x20;
                        GuangMinLab->setText("自动");
                        GuanMiZhiWei = 1;
                    }
                 }
               // SeclistFlag
            break;
        case MENUSTATPLAYING :
            if(PauseFlag){
                msgServer.sendCmd(QtInterface_Play);
                PauseFlag = 0;
            }else{
                msgServer.sendCmd(QtInterface_Pause);
                PauseFlag = 1;
            } break;
        case MENUSTATLOOKPHOTO :

            break;
    }
}

void Sec_Terminal::MenuKeyReturn (void){
    switch(MenuStateFlag){
        case MENUSTATESHOW:
          //  MenuList->hide();
       //     MenuDes->hide();
        //    MenuListFlag  = 0;
       //     MenuList->setCurrentRow(MenuListFlag);
        //    MenuStateFlag = MENUSTATEHIDE;
            break;
        case MENUSTATESEC :
            fileview->FileInfoListDestory();
            fileview->hide();
            fileview->currentrow = 0;
            VideoOrPicFlag = 0;
            SET_AUDIO_BYPASS();
            system("./encodertsp -v 1.264 -a 1.aac -y 2 -I 1 &");
            sleep(3);
            FangDaLab->show();
            GuangMinLab->show();
            MenuStateFlag = MENUSTATESHOW;
            break;
        case MENUSTATSETING :
            SecMenuList->hide();
            SeclistFlag = 0;
            SecFileListDestory();
            FangDaOrGuangMi = 0;
            MenuStateFlag = MENUSTATESHOW;
            break;
       case MENUSTATPLAYING :
            fileview->show();
            printf("MENUSTATESEC 0 :pkill -9  decode \n\n");
            system("pkill -9  decode");
            PauseFlag = 0;
            MenuStateFlag = MENUSTATESEC;
          break;
       case MENUSTATLOOKPHOTO :
            ShowPhoto->hide();
            fileview->show();
            MenuStateFlag = MENUSTATESEC;
        break;
    }

}
/*****************************************************
SecMenuList窗口清空
*****************************************************/
void Sec_Terminal::SecFileListDestory(){
    int index,tmp;
    tmp = SecMenuList->count();
    for(index = 0;index < tmp;index++){
        QListWidgetItem *item = SecMenuList->takeItem(0);
        delete item;
    }
}

/*****************************************************
拍照录像
*****************************************************/
void Sec_Terminal::GetPhotoOrVideo    (int Type){
    if(Type){
        if(FIFOCmd & 0x02){
            FIFOCmd &= ~0x02;
            msgServer.sendCmd(QtInterface_Stop);
            system("sync");
        }else{
            FIFOCmd |= 0x02;
            msgServer.sendCmd(QtInterface_Record);
        }
    }else{
        FIFOCmd |=  0x01;
        msgServer.sendCmd(QtInterface_Photo);
    }
}
/********************Mode切换函数**********************/
void Sec_Terminal::IUpdateMode(void){
    switch(ICURRENTMODE){
    case CAMERCTLMODE:
        g_SendSTM[3] &= ~0x60;
        ModeStatePic->setPixmap(QPixmap("/Sec_Picture/CamerCtl.png"));
        break;
    case LIGHTCTLMODE:
        g_SendSTM[3] &= ~0x60;
        g_SendSTM[3] |= 0x40;
        ModeStatePic->setPixmap(QPixmap("/Sec_Picture/LightCtl.png"));
        break;
    case ROBOTCTLMODE:
        g_SendSTM[3] &= ~0x60;
        g_SendSTM[3] |= 0x20;
        ModeStatePic->setPixmap(QPixmap("/Sec_Picture/baibiname.png"));
        break;
#if 0
        case MENUCTLMODE:
        g_SendSTM[3] &= ~0x60;
        g_SendSTM[3] |= 0x40;
        ButtonKZName->setPixmap(QPixmap("/Icon_TranRot/IconShow/CDKZ.png"));
        break;
#endif
    }

}
void Sec_Terminal::ButtonKey_1_Func(void){          //1
    switch(ICURRENTMODE){
    case CAMERCTLMODE:
        break;
    case LIGHTCTLMODE:                      //前灯
        if(!GuanMiZhiWei){
            if(g_SendSTM[2]&0x04)
                g_SendSTM[2] &= ~0x04;
            else
                g_SendSTM[2] |= 0x04;
       }break;
    case ROBOTCTLMODE:
        break;
    }
}

void Sec_Terminal::ButtonKey_2_Func(void){        //2
    switch(ICURRENTMODE){
    case CAMERCTLMODE:
        break;
    case LIGHTCTLMODE:                            //后灯
        if(g_SendSTM[2]&0x02)
            g_SendSTM[2] &= ~0x02;
        else
            g_SendSTM[2] |= 0x02;
        break;
    case ROBOTCTLMODE:
        break;
    }
}

void Sec_Terminal::ButtonKey_3_Func(void){               // 3
    switch(ICURRENTMODE){
    case CAMERCTLMODE:
        g_SendSTM[4] &= ~0x03;               //摄像头复位
        g_SendSTM[4] |= 0x03;
        break;
    case LIGHTCTLMODE:
        if(g_SendSTM[2]&0x01)
            g_SendSTM[2] &= ~0x01;          //顶灯
        else
            g_SendSTM[2] |= 0x01;
        break;
    case ROBOTCTLMODE:                      //摆臂复位
        g_SendSTM[4] &= ~0x0c;
        g_SendSTM[4] |= 0x0c;
        break;
    }
}

void Sec_Terminal::ButtonKey_4_Func(void){               //4
    switch(ICURRENTMODE){
    case CAMERCTLMODE:                           //摄像头仰视
        g_SendSTM[4] &= ~0x03;
        g_SendSTM[4] |= 0x02;
        break;
    case LIGHTCTLMODE:
        if(!GuanMiZhiWei){
              if(g_SendSTM[2]&0x08)               //预警灯
                  g_SendSTM[2] &= ~0x08;
              else
                  g_SendSTM[2] |= 0x08;
         } break;
    case ROBOTCTLMODE:                              //侦察
        g_SendSTM[4] &= ~0x0c;
        g_SendSTM[4] |= 0x08;
        break;
    }
}

void Sec_Terminal::ButtonKey_5_Func(void){            //5
    switch(ICURRENTMODE){
    case CAMERCTLMODE:                          //摄像头俯视
        g_SendSTM[4] &= ~0x03;
        g_SendSTM[4] |= 0x01;
        break;
    case LIGHTCTLMODE:                          //全关
        g_SendSTM[2] &= ~0x0f;
        break;
    case ROBOTCTLMODE:                          //越障
        g_SendSTM[4] &= ~0x0c;
        g_SendSTM[4] |= 0x04;
        break;
    }
}
/*****************************************************
物理按键处理函数
  0     1       2      3      4      5
模式    仰      俯     复位   仰视   俯视

  6     7     8      9
 上选  下选  确定    返回
*****************************************************/
void  Sec_Terminal::PhyButtonDeal(int Num,int Type){
    switch(Num){
    case 0:
         ShortKey_0->setPixmap(QPixmap("/Sec_Picture/ShortKeyPress.png"));
         ICURRENTMODE++;
         if(ICURRENTMODE > 2) ICURRENTMODE = 0;
        //   printf("--------------ICURRENTMODE :%d--------------------\n",ICURRENTMODE);
         IUpdateMode();
        break;
    case 1:
         ShortKey_1->setPixmap(QPixmap("/Sec_Picture/ShortKeyPress.png"));
         ButtonKey_1_Func();
        break;
    case 2:
         ShortKey_2->setPixmap(QPixmap("/Sec_Picture/ShortKeyPress.png"));
         ButtonKey_2_Func();
        break;
    case 3:
         ShortKey_3->setPixmap(QPixmap("/Sec_Picture/ShortKeyPress.png"));
         ButtonKey_3_Func();
        break;
    case 4:
         ShortKey_4->setPixmap(QPixmap("/Sec_Picture/ShortKeyPress.png"));
         ButtonKey_4_Func();
        break;
    case 5:
         ShortKey_5->setPixmap(QPixmap("/Sec_Picture/ShortKeyPress.png"));
         ButtonKey_5_Func();
        break;
    case 6: MenuKeyUP();              break;
    case 7: MenuKeyDown();            break;
    case 8: MenuKeyEnter();           break;
    case 9: MenuKeyReturn();          break;
    case 10:GetPhotoOrVideo(Type);    break;
    case 11:ButtonKey_11_Func(Type);  break;
    }
}
void  Sec_Terminal::ButtonKey_11_Func(int Type){
    QString   TmpString;
    CararFlag++;
    if(CararFlag > 2)    CararFlag = 0;
    g_SendSTM[2] &= ~0xc0;
    g_SendSTM[2] |= CararFlag << 6;
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
}

void  Sec_Terminal::OsdUpdateFunc(int RetSt){
   // printf("--------------OsdUpdateFunc-------------------\n");
    QString ISpeedString;
    ISpeedString.sprintf("速度：%.1fm/s",g_iCarStateInfo.CarSpeed);
    SuDuSt->setText(ISpeedString);

    QString ISdString;
    float iCarTmp = g_iCarStateInfo.CarSpeed;
    if(iCarTmp < 0) iCarTmp = iCarTmp * -1;
    ISdString.sprintf("%.1f km/h",iCarTmp*3.6);
    SpeedDescription->setText(ISdString);
    float iRot,DiffValue;
    iRot = iCarTmp*3.6/0.25*6.125;
    DiffValue = iRot - MemMBpView;
    if(DiffValue != 0){
        MBpView->rotate(DiffValue);
        MemMBpView += DiffValue;
    }
   // printf("--------------OsdUpdate Key Release-------------------\n");
    if(LongPressFlag == 1 && RetSt != 1) {
        ShortKey_1->setPixmap(QPixmap("/Sec_Picture/ShortKey.png"));
        LongPressFlag = 0;
    }
    if(LongPressFlag == 2 && RetSt != 2){
        ShortKey_2->setPixmap(QPixmap("/Sec_Picture/ShortKey.png"));
        LongPressFlag = 0;
    }
    switch(RetSt){
    case 0:
         ShortKey_0->setPixmap(QPixmap("/Sec_Picture/ShortKey.png")); break;
    case 1:
        if(ICURRENTMODE == ROBOTCTLMODE || ICURRENTMODE == CAMERCTLMODE)
         LongPressFlag = 1;
        else
         ShortKey_1->setPixmap(QPixmap("/Sec_Picture/ShortKey.png")); break;
    case 2:
        if(ICURRENTMODE == ROBOTCTLMODE || ICURRENTMODE == CAMERCTLMODE)
         LongPressFlag = 2;
        else
         ShortKey_2->setPixmap(QPixmap("/Sec_Picture/ShortKey.png")); break;
    case 3:
         ShortKey_3->setPixmap(QPixmap("/Sec_Picture/ShortKey.png")); break;
    case 4:
         ShortKey_4->setPixmap(QPixmap("/Sec_Picture/ShortKey.png")); break;
    case 5:
         ShortKey_5->setPixmap(QPixmap("/Sec_Picture/ShortKey.png")); break;
    case 6:
        break;
    case 9:
        break;
    case 10:
         break;
    }
   // printf("--------------OsdUpdateFunc Carsate-------------------\n");
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
    TmpState = HXpPointRoter;
    if(g_iCarStateInfo.Dir == 360) g_iCarStateInfo.Dir = 0;
    if(g_iCarStateInfo.Dir - TmpState != 0){
      HXpView->rotate(g_iCarStateInfo.Dir - TmpState);
      HXpPointRoter = g_iCarStateInfo.Dir;
    }

    TmpString  = "航向: " + QString("%0").arg(g_iCarStateInfo.Dir) + "°";
    HangXiangSt->setText(TmpString);

    if(g_iCarStateInfo.BaiBiRot > 360) g_iCarStateInfo.BaiBiRot = 360;
    if(g_iCarStateInfo.BaiBiRot < 0) g_iCarStateInfo.BaiBiRot = 0;
    TmpString  = "摆臂: " + QString("%0").arg(g_iCarStateInfo.BaiBiRot) + "°";
    BaiBiSt->setText(TmpString);
    TmpString  = "温度: " + QString("%0").arg(g_iCarStateInfo.HuanJingWenDu) + "℃";
    WeDuSt->setText(TmpString);

    TmpString  = "CCD角度: " + QString("%0").arg(g_iCarStateInfo.CameraAngle) + "°";
    CameraAngle->setText(TmpString);

    if(g_iCarStateInfo.YuJingDengFlag)
        LightIconY->setPixmap(QPixmap("/Sec_Picture/Light/YLightNow.png"));
    else
        LightIconY->setPixmap(QPixmap("/Sec_Picture/Light/YLight.png"));

    if(g_iCarStateInfo.QianDeng)
        LightIconQ->setPixmap(QPixmap("/Sec_Picture/Light/QLightNow.png"));
    else
        LightIconQ->setPixmap(QPixmap("/Sec_Picture/Light/QLight.png"));

    if(g_iCarStateInfo.ShangDeng)
        LightIconS->setPixmap(QPixmap("/Sec_Picture/Light/SLightNow.png"));
    else
        LightIconS->setPixmap(QPixmap("/Sec_Picture/Light/SLight.png"));

    if(g_iCarStateInfo.HouDeng)
       LightIconH->setPixmap(QPixmap("/Sec_Picture/Light/HLightNow.png"));
    else
       LightIconH->setPixmap(QPixmap("/Sec_Picture/Light/HLight.png"));

 /***************************车体电量********************************/
    int iTerPowerTmpFlag;
    if(g_iCarStateInfo.TerPowerVal != TerminalPowerMen){
        if(g_iCarStateInfo.TerPowerVal > 100) g_iCarStateInfo.TerPowerVal = 100;
        QString  str;
        iTerPowerTmpFlag = g_iCarStateInfo.TerPowerVal - g_iCarStateInfo.TerPowerVal%20;
        if(iTerPowerTmpFlag < 20) iTerPowerTmpFlag = 20;
        str.sprintf("/Sec_Picture/Power/%d.png",iTerPowerTmpFlag);
        TerminalPower->setPixmap(QPixmap(str));
        TerminalPowerMen = g_iCarStateInfo.TerPowerVal;
    }
    // if(g_iCarStateInfo.CarPowerVal != CarPowerMen  ){
    // PowerValSelect(g_iCarStateInfo.CarPowerVal,CarPower);
    // if(g_iCarStateInfo.CarPowerVal > 100) g_iCarStateInfo.CarPowerVal = 100;
    // tmp.sprintf("%.2d\%",g_iCarStateInfo.CarPowerVal);
    // CarPowerMen = g_iCarStateInfo.CarPowerVal;
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
