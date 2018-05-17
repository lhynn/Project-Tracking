#include "addosd.h"
#include <QFont>
#include <QPen>
#include <QColor>


AddOsd::AddOsd(QDialog *parent) :
    QDialog(parent)
{

  //   connect(, SIGNAL(OsdUpdate()), this, SLOT(timerDone()));

}

void AddOsd::paintEvent(QPaintEvent *event){

    QPainter painter(this);
   // painter.setPen(QColor(255,255,255));
    painter.setPen(QColor(100,100,255));
    painter.translate(360,640);
    painter.rotate(270);
//    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setFont(QFont("JPTZ",22,QFont::Bold));
    painter.drawText(QPointF(430, -270),"视频 北 京 晶 品 特 装 摄像头控制 导航姿态 车体参数");

}
void AddOsd::timerDone(){
    repaint();
}
