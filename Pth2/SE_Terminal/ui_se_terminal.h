/********************************************************************************
** Form generated from reading UI file 'se_terminal.ui'
**
** Created: Fri Apr 27 02:11:01 2018
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SE_TERMINAL_H
#define UI_SE_TERMINAL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_se_terminal
{
public:

    void setupUi(QWidget *se_terminal)
    {
        if (se_terminal->objectName().isEmpty())
            se_terminal->setObjectName(QString::fromUtf8("se_terminal"));
        se_terminal->resize(1280, 720);
        se_terminal->setStyleSheet(QString::fromUtf8(""));

        retranslateUi(se_terminal);

        QMetaObject::connectSlotsByName(se_terminal);
    } // setupUi

    void retranslateUi(QWidget *se_terminal)
    {
        se_terminal->setWindowTitle(QApplication::translate("se_terminal", "se_terminal", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class se_terminal: public Ui_se_terminal {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SE_TERMINAL_H
