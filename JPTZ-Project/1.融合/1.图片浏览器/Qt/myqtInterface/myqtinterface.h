#ifndef MYQTINTERFACE_H
#define MYQTINTERFACE_H

#include <QWidget>
#include "osdmanager.h"

namespace Ui {
class myqtInterface;
}

class myqtInterface : public QWidget
{
    Q_OBJECT
    
public:
    explicit myqtInterface(QWidget *parent = 0);
    ~myqtInterface();
    
private:
    Ui::myqtInterface *ui;
};

#endif // MYQTINTERFACE_H
