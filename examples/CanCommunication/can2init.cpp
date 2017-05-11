#include "can2init.h"
#include "can2operation.h"
//extern "C"{
#include "gui_config.h"
#include <unistd.h>
//}
//#include <QtDebug>

#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

void ConsumerHeartbeatAlarm2(CO_Data* d, UNS32 id){
  UNS8 nodeId = (UNS8)(((d->ConsumerHeartbeatEntries[id]) & (UNS32)0x00FF0000) >> (UNS8)16);
  /*MSG_WAR(0x00, "ConsumerHearbeatAlarm", 0x00);*/

  /* timer have been notified and is now free (non periodic)*/
  /* -> avoid deleting re-assigned timer if message is received too late*/
  d->ConsumerHeartBeatTimers[id]=TIMER_NONE;

  /* set node state */
  d->NMTable[nodeId] = Disconnected;
  /*! call heartbeat error with NodeId */
  (*d->heartbeatError)(d, nodeId);
}

void can2Init_heartbeatError(CO_Data* /*d*/, UNS8 /*heartbeatID*/){
//    eprintf("can2Init_heartbeatError %d\n", heartbeatID);

    if(6 == deviceCarId){
        bCan2HMI1HeartFlag = false;
        canSignalsSlots->masterHMIUpdate();
    }

//    DelAlarm(TIMER_HANDLE(heartbeatID-1));
//    if(6 == deviceCarId){
//        SetAlarm(d, 1, (TimerCallback_t)ConsumerHeartbeatAlarm2, MS_TO_TIMEVAL(2500), 0);
//    }
//    eprintf("can2Init_heartbeatError  Reset SetAlarm @@@@@@@@@@@@@@@\n");
}

void can2Init_initialisation(CO_Data* /*d*/){
//    if(masterHMIFlag){
//        UNS16 EventTime = SEND_PDO_CYCLE_200MS;
//        UNS32 size = sizeof(UNS16);
//        //200ms周期发送公共逻辑数据
//        writeLocalDict(&Can2ObjDict_Data, 0x1800, 0x05, &EventTime, &size, RW);
//        //200ms周期发送列车到站信息数据
//        writeLocalDict(&Can2ObjDict_Data, 0x180C, 0x05, &EventTime, &size, RW);
//        writeLocalDict(&Can2ObjDict_Data, 0x180D, 0x05, &EventTime, &size, RW);

//        EventTime = SEND_PDO_CYCLE_1S;
//        //1s周期发送列车参数：温度，车轮直径，AS压力满，AS压力空
//        writeLocalDict(&Can2ObjDict_Data, 0x1804, 0x05, &EventTime, &size, RW);
//        writeLocalDict(&Can2ObjDict_Data, 0x1805, 0x05, &EventTime, &size, RW);
//        writeLocalDict(&Can2ObjDict_Data, 0x180A, 0x05, &EventTime, &size, RW);
//        writeLocalDict(&Can2ObjDict_Data, 0x180B, 0x05, &EventTime, &size, RW);
//    }
//    eprintf("can2Init_initialisation\n");
}

void can2Init_preOperational(CO_Data* /*d*/){
    can2OperationalFlag = false;
//    eprintf("can2Init_preOperational\n");
}

void can2Init_operational(CO_Data* /*d*/){
    can2OperationalFlag = true;
    canSignalsSlots->canWorkStateUpdate();
    eprintf("can2Init_operational !!!!!!!  \n");
}

void can2Init_stopped(CO_Data* /*d*/){
//    eprintf("can2Init_stopped\n");
}

void can2Init_post_sync(CO_Data* /*d*/){
//      eprintf("can2Init_post_sync\n");
}

void can2Init_post_TPDO(CO_Data* /*d*/){
//    eprintf("can2Init_post_TPDO\n");

}

void can2Init_storeODSubIndex(CO_Data* /*d*/, UNS16 /*wIndex*/, UNS8 /*bSubindex*/){
    /*TODO :
     * - call getODEntry for index and subindex,
     * - save content to file, database, flash, nvram, ...
     *
     * To ease flash organisation, index of variable to store
     * can be established by scanning d->objdict[d->ObjdictSize]
     * for variables to store.
     *
     * */
//    eprintf("can2Init_storeODSubIndex : %4.4x %2.2x\n", wIndex,  bSubindex);
}

void can2Init_post_emcy(CO_Data* /*d*/, UNS8 /*nodeID*/, UNS16 /*errCode*/, UNS8 /*errReg*/){
//    eprintf("Can2 @@@@@@@@@@@@@@@@@ Slave received EMCY message. Node: %2.2x  ErrorCode: %4.4x  ErrorRegister: %2.2x\n", nodeID, errCode, errReg);

//    QString strErrCode = QString::number(errCode, 16);
//    int intBit1 = strErrCode.mid(0, 1).toInt();
//    int intBit2 = strErrCode.mid(1, 1).toInt();

//    bool canChangeFlag = false;
//    if(intBit1 >= 1 && intBit1 <= 6){
//        if(0 == intBit2){
//            canChangeFlag = false;
//        }
//        else{
//            canChangeFlag = true;
//        }
//    }
//    if(canChangeFlag){
//        canWorkFlag = CAN_2;
//    }
//    eprintf("Can2 ################### Can Change Finished, Current Can Id ==  %d \n", canWorkFlag);
}


