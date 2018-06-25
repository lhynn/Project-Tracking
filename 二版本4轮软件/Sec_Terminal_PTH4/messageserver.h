#ifndef MESSAGESERVER_H
#define MESSAGESERVER_H

#include "qtinterface.h"

#include <QThread>
#include <QObject>

class MessageServer : public QThread
{
    Q_OBJECT

protected:
     void run();

public:
    void sendCmd(QtInterface_Command cmd);
    void sendConfig(char option, QString cfgString);
    void quit();

    QtInterface_Command getKbCmd();

signals:
    void statusMsg(int position, QString label, QString value);
    void playMsg();
    void pauseMsg();
    void stopMsg();
    void incMsg();
    void decMsg();
    void hideMsg();

private:
    void getStatus();

    FILE * m_fpStatus;
    FILE * m_fpConfig;
    FILE * m_fpCmd;
    FILE * m_fpKbCmd;
    bool m_quit;
};

#endif // MESSAGESERVER_H
