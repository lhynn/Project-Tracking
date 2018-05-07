/********************************************************************************
** Form generated from reading UI file 'myqtinterface.ui'
**
** Created: Tue Sep 26 02:34:43 2017
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYQTINTERFACE_H
#define UI_MYQTINTERFACE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_myqtInterface
{
public:

    void setupUi(QWidget *myqtInterface)
    {
        if (myqtInterface->objectName().isEmpty())
            myqtInterface->setObjectName(QString::fromUtf8("myqtInterface"));
        myqtInterface->resize(400, 300);

        retranslateUi(myqtInterface);

        QMetaObject::connectSlotsByName(myqtInterface);
    } // setupUi

    void retranslateUi(QWidget *myqtInterface)
    {
        myqtInterface->setWindowTitle(QApplication::translate("myqtInterface", "myqtInterface", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class myqtInterface: public Ui_myqtInterface {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYQTINTERFACE_H
