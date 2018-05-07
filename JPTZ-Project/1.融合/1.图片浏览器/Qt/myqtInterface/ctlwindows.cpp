#include "ctlwindows.h"
#include <QDir>

Ctlwindows:: Ctlwindows(MessageServer * msgServer, OSDManager * osd)
{
    count1 = 0;
    count2 = 0;

    photo = new QLabel(this);
    video = new QLabel(this);

    picture = new QLabel(this);
    seting  = new QLabel(this);
    play    = new QLabel(this);


    showBig    = new QLabel(this);

    Image1 = new Imagedilog;
    Image2 = new Imagedilog;
    Image3 = new Imagedilog;
    Image4 = new Imagedilog;
    //Image4->ImageWindow->setPixmap(QPixmap("PIX/pix4.bmp"));
    //Image4->NameWindow->setText("pix4.bmp");

    priviewShow = new QGridLayout(this);
    priviewShow->addLayout(Image1->vLayOut,0,0);
    priviewShow->addLayout(Image2->vLayOut,0,1);
    priviewShow->addLayout(Image3->vLayOut,1,0);
    priviewShow->addLayout(Image4->vLayOut,1,1);

   // priviewShow->setGeometry(QRect(40,40,580,536));

    SearchDir(QString("PIX/"),QString("*.bmp"),&fileinfo);
    //int i;
    //for(i = 0;i< fileinfo.fileName.length();i++)
    //qDebug() << fileinfo.fileName.at(i)<<fileinfo.filePath.at(i);


    paizhao = new QTimer;
    luxiang = new QTimer;
    caidan  = new QTimer;
    yulan   = new QTimer;
    connect(paizhao, SIGNAL(timeout()),this, SLOT(timerDone1()));
    connect(luxiang, SIGNAL(timeout()),this, SLOT(timerDone2()));
    connect(caidan , SIGNAL(timeout()),this, SLOT(timerDone3()));
    connect(yulan  , SIGNAL(timeout()),this, SLOT(timerDone4()));

    paizhao ->start(10);
    this->setFixedSize(720,576);
}

Ctlwindows::~Ctlwindows(){


}

void Ctlwindows::SearchDir(QString Path,QString type,PT_FILEINFO pt_fileinfo){
    int i = 0;
    QDir dir(Path);//定义一个QDir变量,设置路径为当前目录的music文件夹(这里可用绝对路径也可以用相对路径)
    QStringList FileTypeNameList;//用来保存歌曲名的表
    QFileInfoList FileNameList;//文件信息表

    QFileInfo FileInfo;//文件信息变量
    QString FilePath;//用于保存文件信息字符串
    QString FileName;

    FileTypeNameList << type;//把所有mp3文件加入到stringlist_song表中
    dir.setNameFilters(FileTypeNameList);//过滤文件
    FileNameList = dir.entryInfoList();//列出所有符合的文件
    while( i<FileNameList.length() ){
        FileInfo = FileNameList.at(i);//把fileinfolist表中某个位置的歌曲名赋给fileinfo变量
        FilePath = FileInfo.filePath();//把fileinfo变量中的文件名转换成QString并赋给string_song
        FileName = FileInfo.fileName();
        pt_fileinfo->fileName << FileName;
        pt_fileinfo->filePath << FilePath;
        i++;
    }
}
void Ctlwindows::timerDone1(){
    photo->setFixedSize(40,40);
    photo->setPixmap(QPixmap("Image/photo.bmp"));
    photo->setScaledContents(true);
    photo->setGeometry(20,263,40,40);
    paizhao->stop();
    luxiang ->start(2000);
}
void Ctlwindows::timerDone2(){
    photo->hide();
    video->setFixedSize(40,40);
    video->setPixmap(QPixmap("Image/video.bmp"));
    video->setScaledContents(true);
    video->setGeometry(680,263,40,40);
    luxiang->stop();
    caidan  ->start(2000);
}
void Ctlwindows::timerDone3(){
    video->hide();
    picture->setFixedSize(40,40);
    picture->setPixmap(QPixmap("Image/picture.bmp"));
    picture->setScaledContents(true);
    picture->setGeometry(300,516,40,40);

    seting->setFixedSize(40,40);
    seting->setPixmap(QPixmap("Image/seting.bmp"));
    seting->setScaledContents(true);
    seting->setGeometry(340,516,40,40);

    play->setFixedSize(40,40);
    play->setPixmap(QPixmap("Image/play.bmp"));
    play->setScaledContents(true);
    play->setGeometry(380,516,40,40);

    switch(count1){
       case 0:
        picture->setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(255,0,0);");
        count1++;
        break;
       case 1:
        picture->setStyleSheet("color: rgb(0, 0, 0);");
        seting->setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(255,0,0);");
        count1++;
        break;
       case 2:
        picture->setStyleSheet("color: rgb(0, 0, 0);");
        seting->setStyleSheet("color: rgb(0, 0, 0);");
        play->setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(255,0,0);");
        yulan  ->start(100);
        luxiang->stop();
        break;
    }

}
void Ctlwindows::timerDone4(){
    picture->hide();
    play   ->hide();
    seting ->hide();

    Image1->ImageWindow->setPixmap(QPixmap(fileinfo.filePath.at(0)));
    Image1->NameWindow->setText(fileinfo.fileName.at(0));

    Image2->ImageWindow->setPixmap(QPixmap(fileinfo.filePath.at(1)));
    Image2->NameWindow->setText(fileinfo.fileName.at(1));

    Image3->ImageWindow->setPixmap(QPixmap(fileinfo.filePath.at(2)));
    Image3->NameWindow->setText(fileinfo.fileName.at(2));

    Image4->ImageWindow->setPixmap(QPixmap(fileinfo.filePath.at(3)));
    Image4->NameWindow->setText(fileinfo.fileName.at(3));
    if(count2 > 3) count2 = 0;
    ShowImageFrame(count2);
    count2++;

}


void Ctlwindows::ShowImageFrame(int num){
    qDebug() << num;
    switch(num){
    case 0:
        Image2->DelImageFrame();
        Image3->DelImageFrame();
        Image4->DelImageFrame();
        Image1->SetImageFrame();
        break;
    case 1:
        Image1->DelImageFrame();
        Image3->DelImageFrame();
        Image4->DelImageFrame();
        Image2->SetImageFrame();
        break;
    case 2:
        Image1->DelImageFrame();
        Image2->DelImageFrame();
        Image4->DelImageFrame();
        Image3->SetImageFrame();
        break;
    case 3:
        Image1->DelImageFrame();
        Image2->DelImageFrame();
        Image3->DelImageFrame();
        Image4->SetImageFrame();
    }
}
