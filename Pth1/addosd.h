#ifndef ADDOSD_H
#define ADDOSD_H

#include <QDialog>
#include <QPainter>
#include <QPoint>
#include "usarttest.h"


class AddOsd : public QDialog
{
    Q_OBJECT
public:
    explicit AddOsd(QDialog *parent = 0);
protected:
    void paintEvent(QPaintEvent *);
private:

signals:

private slots:
    void timerDone();
};

#endif // ADDOSD_H
