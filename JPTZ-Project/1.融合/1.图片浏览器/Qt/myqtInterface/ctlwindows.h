#ifndef CTLWINDOWS_H
#define CTLWINDOWS_H

#include <QWidget>

#include "messageserver.h"
#include "osdmanager.h"
#include "Imagedilog.h"

#include <QLabel>
#include <QGridLayout>
#include <QVector>
#include <QTimer>

typedef struct FileInfo{
    QStringList fileName;
    QStringList filePath;
}FILEINFO,*PT_FILEINFO;


class Ctlwindows : public QWidget{

    Q_OBJECT

public:
    Ctlwindows(MessageServer * msgServer, OSDManager * osd);
    ~Ctlwindows();
    void SearchDir(QString Path,QString type,PT_FILEINFO pt_fileinfo);
    void ShowImageFrame(int );


protected Q_SLOTS:
    void timerDone1();
    void timerDone2();
    void timerDone3();
    void timerDone4();

private:
    QLabel *photo;
    QLabel *video;
    QLabel *picture;
    QLabel *play;
    QLabel *seting;
    QGridLayout *priviewShow;


    Imagedilog *Image1;
    Imagedilog *Image2;
    Imagedilog *Image3;
    Imagedilog *Image4;

    FILEINFO fileinfo;


    QTimer      *paizhao;
    QTimer      *luxiang;
    QTimer      *caidan;
    int count1;
    QTimer      *yulan;
    QLabel      *showBig;
    int count2;
};

#endif // CTLWINDOWS_H
