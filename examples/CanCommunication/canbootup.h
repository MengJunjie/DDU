#ifndef CANBOOTUP_H
#define CANBOOTUP_H

#include <QObject>
#include <QTimer>

#include "gui_config.h"

#define CAN_TIME_OUT 2000

class CanBootUp : public QObject
{
    Q_OBJECT
public:
    explicit CanBootUp(QObject *parent = 0);

    void timerCan1_Start();
    void timerCan2_Start();
signals:

public slots:
    void slot_Can1_TimeOut();
    void slot_Can2_TimeOut();
private:
    QTimer * timerCan1;
    QTimer * timerCan2;

    int can1SendCount, can2SendCount;
};

#endif // CANBOOTUP_H
