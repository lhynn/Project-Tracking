#include <QtGui/QApplication>
#include <QWidget>
#include "ctlwindows.h"
#include "messageserver.h"

#define STATUS_X       (QApplication::desktop()->width() / 4)
#define STATUS_Y       (QApplication::desktop()->height() / 8)
#define STATUS_WIDTH   (QApplication::desktop()->width() / 2)
#define STATUS_HEIGHT  (QApplication::desktop()->height() / 3)
#define STATUS_MARGIN  (STATUS_HEIGHT / 8)


int main(int argc, char *argv[])
{
    OSDManager osd;
    osd.setDisplayOutput(OSDManager::OUTPUT_COMPOSITE);
    osd.setDisplayMode(OSDManager::MODE_PAL);



   /* Create message server */
   MessageServer msgServer;
    /* Start message server to interact with demo */
   msgServer.start(QThread::HighPriority);
   QApplication a(argc, argv);

   Ctlwindows mainwindows(&msgServer,&osd);

/*
    w.setAttribute(Qt::WA_TranslucentBackground);      //背景透明
    w.setWindowFlags(Qt::FramelessWindowHint);        //窗体隐藏

    w.setWindowOpacity(0.3);//设置透明1-全体透明
    w.setAttribute(Qt::WA_TranslucentBackground, true);//设置透明2-窗体标题栏不透明,背景透明
    w.show();
*/
    
/*
    osd.setBoxDimensions(0,0,720,576);*/
  // osd.showTransOSD();

   //mainwindows.showFullScreen();
   //mainwindows.setWindowOpacity(1);

    mainwindows.setWindowFlags(Qt::FramelessWindowHint);
    mainwindows.setStyleSheet("background-color: rgb(255,255,255);");
    mainwindows.show();


     qDebug() << STATUS_X - STATUS_MARGIN << \
     STATUS_Y - STATUS_MARGIN << STATUS_WIDTH + 2 * STATUS_MARGIN \
              << STATUS_HEIGHT + 2 * STATUS_MARGIN;
    osd.setBoxDimensions(158,48,408,240);

    //mainwindows.setWindowOpacity(0.4);

    osd.showTransOSD();


    //osd.hideOSD();

    return a.exec();
}
