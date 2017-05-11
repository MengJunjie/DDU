#ifndef THREADFAULTRECORD_H
#define THREADFAULTRECORD_H

#include <QThread>
#include "gui_config.h"

class ThreadFaultRecord : public QThread
{
    Q_OBJECT
public:
    explicit ThreadFaultRecord(QObject *parent = 0);

    void run();
    void startThreadRun();

    void car_VVVFFaultOperation(CAR_ID carId, int vvvfIndex);
    void car_BCUFaultOperation(CAR_ID carId);
    void car_ACCFaultOperation(CAR_ID carId);
    void car_SIVFaultOperation(CAR_ID carId);
    void car_DoorFaultOperation(CAR_ID carId);

signals:

public slots:
    void slot_TimerOut_400MS();

private:
    bool bVVVFAllFaultFlag[6][16];
    bool bBCUAllFaultFlag[6][23];
    bool bACCAllFaultFlag[6][28];
    bool bSIVAllFaultFlag[2][18];
    bool bDoorAllFaultFlag[48][16];

    QTimer * timer_400MS;

    QList<int> mVVVFCodeList;
    QList<int> mBCUCodeList;
    QList<int> mACCCodeList;
    QList<int> mSIVCodeList;
    QList<int> mDoorCodeList;
};

#endif // THREADFAULTRECORD_H
