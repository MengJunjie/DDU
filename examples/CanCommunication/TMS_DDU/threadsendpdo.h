#ifndef THREADSENDPDO_H
#define THREADSENDPDO_H

#include <QThread>

#include "gui_config.h"

class ThreadSendPDO : public QThread
{
    Q_OBJECT
public:
    explicit ThreadSendPDO(QObject *parent = 0);

    void run();
    void startPDOSend();
    void stopPDOSend();

signals:

public slots:
    void slot_TimerOut_200MS();
    void slot_TimerOut_1S();

private:
    QTimer * timer_200MS;
    QTimer * timer_1S;
};

#endif // THREADSENDPDO_H
