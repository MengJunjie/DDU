#include "threaddistance.h"

#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

ThreadDistance::ThreadDistance(QObject *parent) : QThread(parent)
{
    mCurStationId = 0;

    timer_1S = new QTimer();
}

void ThreadDistance::run(){
    connect(timer_1S, SIGNAL(timeout()), this, SLOT(slot_TimerOut_1S()));
}

void ThreadDistance::startThreadRun(){
    if(!timer_1S->isActive()){
        timer_1S->start(1000);
    }

    start();
}

void ThreadDistance::slot_TimerOut_1S(){
    if(canSignalsSlots->iCurrStation != mCurStationId){
        mCurStationId = canSignalsSlots->iCurrStation;
        canSignalsSlots->fCurStationDistance = 0;
    }
    else{
        canSignalsSlots->fCurStationDistance += carSpeedValue[1]/3.6;
    }
}
