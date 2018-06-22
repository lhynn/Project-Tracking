/********************************************************************************
** Form generated from reading UI file 'sec_terminal.ui'
**
** Created: Mon Jun 18 20:13:17 2018
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEC_TERMINAL_H
#define UI_SEC_TERMINAL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Sec_Terminal
{
public:

    void setupUi(QWidget *Sec_Terminal)
    {
        if (Sec_Terminal->objectName().isEmpty())
            Sec_Terminal->setObjectName(QString::fromUtf8("Sec_Terminal"));
        Sec_Terminal->resize(400, 300);

        retranslateUi(Sec_Terminal);

        QMetaObject::connectSlotsByName(Sec_Terminal);
    } // setupUi

    void retranslateUi(QWidget *Sec_Terminal)
    {
        Sec_Terminal->setWindowTitle(QApplication::translate("Sec_Terminal", "Sec_Terminal", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Sec_Terminal: public Ui_Sec_Terminal {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEC_TERMINAL_H
