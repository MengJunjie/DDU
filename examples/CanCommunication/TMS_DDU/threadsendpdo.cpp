#include "threadsendpdo.h"

#include "../cansendandrecv.h"

ThreadSendPDO::ThreadSendPDO(QObject *parent) : QThread(parent)
{
    timer_200MS = new QTimer();
    timer_1S = new QTimer();
}

void ThreadSendPDO::run(){
    connect(timer_200MS, SIGNAL(timeout()), this, SLOT(slot_TimerOut_200MS()));
    connect(timer_1S, SIGNAL(timeout()), this, SLOT(slot_TimerOut_1S()));
}

void ThreadSendPDO::startPDOSend(){
    if(!timer_200MS->isActive()){
        timer_200MS->start(200);
    }

    if(!timer_1S->isActive()){
        timer_1S->start(1000);
    }
    start();
}

void ThreadSendPDO::stopPDOSend(){
    if(timer_200MS->isActive()){
        timer_200MS->stop();
    }

    if(timer_1S->isActive()){
        timer_1S->stop();
    }
}

void ThreadSendPDO::slot_TimerOut_200MS(){
    if(!bPTUDownloading){
        canSendCarDataUpdate();
        canSendCarArriveData1_Update();
        canSendCarArriveData2_Update();
    }
}

void ThreadSendPDO::slot_TimerOut_1S(){
    if(!bPTUDownloading){
        canSendParaData1_Update();
        canSendParaData2_Update();
        canSendParaData3_Update();
        canSendParaData4_Update();
    }
}
