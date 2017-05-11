#include "canbootup.h"

//extern "C"{
#include "can1operation.h"
#include "can2operation.h"
//}
#include <unistd.h>

#include "cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

CanBootUp::CanBootUp(QObject *parent) : QObject(parent)
{
    can1SendCount = 0;
    can2SendCount = 0;

    timerCan1 = new QTimer();
    connect(timerCan1, SIGNAL(timeout()), this, SLOT(slot_Can1_TimeOut()));

    timerCan2 = new QTimer();
    connect(timerCan2, SIGNAL(timeout()), this, SLOT(slot_Can2_TimeOut()));

    qDebug()<<"Can Boot Up Start ::::   Device Id ==  "<<deviceCarId;
}

void CanBootUp::timerCan1_Start(){
    qDebug()<<"Can1 Timer Start !!!";
    can1Connect();
    can1SendCount++;
    timerCan1->start(CAN_TIME_OUT);
}

void CanBootUp::timerCan2_Start(){
    qDebug()<<"Can2 Timer Start !!!";
    can2Connect();
    can2SendCount++;
    timerCan2->start(CAN_TIME_OUT);
}

void CanBootUp::slot_Can1_TimeOut(){
    qDebug()<<"can1 Time Out ### , can1SendCount ==  "<<can1SendCount << "   , Can Working == "<<canWorkFlag;
    if(can1OperationalFlag){
        timerCan1->stop();
        while (1) {
            usleep(CALLBACK_DELAY_TIME);
            break;
        }
    }
    else{
        if(5 == can1SendCount){
            qDebug()<<"CAN1 Timer 5S  Ago !!!      Can Working == "<<canWorkFlag;
            timerCan1->stop();
        }
        else{
            can1BootUp();
            can1SendCount++;
        }
    }
}

void CanBootUp::slot_Can2_TimeOut(){
    qDebug()<<"can2 Time Out ### , can2SendCount == "<<can2SendCount << "   , Can Working == "<<canWorkFlag;
    if(can2OperationalFlag){
        timerCan2->stop();
        while (1) {
            usleep(CALLBACK_DELAY_TIME);
            break;
        }
//        can2RegisterCallBack();
    }
    else{
        if(5 == can2SendCount){
            qDebug()<<"CAN2 Timer 5S Ago !!!         Can Working == "<<canWorkFlag;
            timerCan2->stop();
        }
        else{
            can2BootUp();
            can2SendCount++;
        }
    }
}
