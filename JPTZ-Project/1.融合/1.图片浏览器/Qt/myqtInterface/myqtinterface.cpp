#include "myqtinterface.h"
#include "ui_myqtinterface.h"

myqtInterface::myqtInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myqtInterface)
{
    ui->setupUi(this);
}

myqtInterface::~myqtInterface()
{
    delete ui;
}
