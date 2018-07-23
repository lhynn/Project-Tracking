#include <QtGui/QApplication>
#include "se_terminal.h"
#include <QTextCodec>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QPoint>
#include <QScreen>
#include "osdmanager.h"

int main(int argc, char *argv[])
{
    OSDManager osd;
    printf("\r\n[SGL-JPAD] Starting SGL-JPAD\r\n");
    printf("[SGL-JPAD] BUILD DATA:%s TIME:%s\r\n",__DATE__, __TIME__);

    if (!osd.setDisplayOutput(OSDManager::OUTPUT_COMPONENT))
    {
        qFatal("Could not set display output in interface\n");
        return -1;
    }
    if (!osd.setDisplayMode(OSDManager::MODE_720P_60))
    {
        qFatal("Could not set display mode in interface\n");
        return -1;
    }

    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    se_terminal w;
    w.resize(1280,720);
    w.show();

    //osd.hideOSD();
   // osd.showTransOSD();
    return a.exec();
}
