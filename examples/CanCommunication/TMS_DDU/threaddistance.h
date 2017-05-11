#ifndef THREADDISTANCE_H
#define THREADDISTANCE_H

#include <QThread>
#include "gui_config.h"

class ThreadDistance : public QThread
{
    Q_OBJECT
public:
    explicit ThreadDistance(QObject *parent = 0);

    void run();
    void startThreadRun();

signals:

public slots:
    void slot_TimerOut_1S();

private:
    QTimer * timer_1S;

    int mCurStationId;
};

#endif // THREADDISTANCE_H
