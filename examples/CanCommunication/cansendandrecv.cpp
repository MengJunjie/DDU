#include "cansendandrecv.h"

#include "objectdictdata.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

void canSendCarDataUpdate(){  // TPDO1  && TPDO16
    if(masterHMIFlag){
        if(1 == deviceCarId){
            for(int i = 0; i < 8; i++){
                can1_CarData_HMI1[i] = Car_Data_HMI[i];
                can2_CarData_HMI1[i] = Car_Data_HMI[i];
            }
            Can1ObjDict_Data.PDO_status[0].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 0);
            Can2ObjDict_Data.PDO_status[0].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 0);
        }
        else{
            for(int i = 0; i < 8; i++){
                can1_CarData_HMI2[i] = Car_Data_HMI[i];
                can2_CarData_HMI2[i] = Car_Data_HMI[i];
            }
            Can1ObjDict_Data.PDO_status[15].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 15);
            Can2ObjDict_Data.PDO_status[15].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 15);
//            qDebug()<<"HMI2  Send Car Common Data ###########################";
        }
    }
}

void canSendCarDateTimeUpdate(){  // TPDO2  && TPDO17
    if(masterHMIFlag && 1 == deviceCarId){
        for(int i = 0; i < 7; i++){
            can1_DateTime_HMI1[i] = Date_Time_HMI[i];
            can2_DateTime_HMI1[i] = Date_Time_HMI[i];
        }
        Can1ObjDict_Data.PDO_status[1].last_message.cob_id = 0;
        sendOnePDOevent(&Can1ObjDict_Data, 1);
        Can2ObjDict_Data.PDO_status[1].last_message.cob_id = 0;
        sendOnePDOevent(&Can2ObjDict_Data, 1);
    }
    else if(masterHMIFlag && 6 == deviceCarId){
        for(int i = 0; i < 7; i++){
            can1_DateTime_HMI2[i] = Date_Time_HMI[i];
            can2_DateTime_HMI2[i] = Date_Time_HMI[i];
        }
        Can1ObjDict_Data.PDO_status[16].last_message.cob_id = 0;
        sendOnePDOevent(&Can1ObjDict_Data, 16);
        Can2ObjDict_Data.PDO_status[16].last_message.cob_id = 0;
        sendOnePDOevent(&Can2ObjDict_Data, 16);
    }
    else{
        // do nothing
    }
}

void canSendSetParaData_Update(){  // TPDO3
    for(int i = 0; i < 4; i++){
        can1_SetParaData_HMI[i] = SetParaData_HMI[i];
        can2_SetParaData_HMI[i] = SetParaData_HMI[i];
    }
    Can1ObjDict_Data.PDO_status[2].last_message.cob_id = 0;
    sendOnePDOevent(&Can1ObjDict_Data, 2);
    Can2ObjDict_Data.PDO_status[2].last_message.cob_id = 0;
    sendOnePDOevent(&Can2ObjDict_Data, 2);
}

void canSendGetFaultData_Update(){  // TPDO4  && TPDO18
    if(1 == deviceCarId){
        if(CAN_2 == canWorkFlag){
            for(int i = 0; i < 2; i++){
                can2_GetFaultData_HMI1[i] = GetFaultData_HMI[i];
            }
            Can2ObjDict_Data.PDO_status[3].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 3);
        }
        else{
            for(int i = 0; i < 2; i++){
                can1_GetFaultData_HMI1[i] = GetFaultData_HMI[i];
            }
            Can1ObjDict_Data.PDO_status[3].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 3);
        }
    }
    else if(6 == deviceCarId){
        if(CAN_2 == canWorkFlag){
            for(int i = 0; i < 2; i++){
                can2_GetFaultData_HMI2[i] = GetFaultData_HMI[i];
            }
            Can2ObjDict_Data.PDO_status[17].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 17);
        }
        else{
            for(int i = 0; i < 2; i++){
                can1_GetFaultData_HMI2[i] = GetFaultData_HMI[i];
            }
            Can1ObjDict_Data.PDO_status[17].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 17);
        }
    }
    else{
        // do nothing
    }
}

void canSendParaData1_Update(){  // TPDO5  && TPDO19
    if(masterHMIFlag){
        if(1 == deviceCarId){
            for(int i = 0; i < 7; i++){
                can1_ParaData_HMI1_1[i] = ParaData_HMI[i];
                can2_ParaData_HMI1_1[i] = ParaData_HMI[i];
            }
            Can1ObjDict_Data.PDO_status[4].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 4);
            Can2ObjDict_Data.PDO_status[4].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 4);
        }
        else{
            for(int i = 0; i < 7; i++){
                can1_ParaData_HMI2_1[i] = ParaData_HMI[i];
                can2_ParaData_HMI2_1[i] = ParaData_HMI[i];
            }
            Can1ObjDict_Data.PDO_status[18].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 18);
            Can2ObjDict_Data.PDO_status[18].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 18);
        }
    }
}

void canSendParaData2_Update(){  // TPDO6  && TPDO20
    if(masterHMIFlag){
        if(1 == deviceCarId){
            for(int i = 0; i < 8; i++){
                can1_ParaData_HMI1_2[i] = ParaData_HMI[i+7];
                can2_ParaData_HMI1_2[i] = ParaData_HMI[i+7];
            }
            Can1ObjDict_Data.PDO_status[5].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 5);
            Can2ObjDict_Data.PDO_status[5].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 5);
        }
        else{
            for(int i = 0; i < 8; i++){
                can1_ParaData_HMI2_2[i] = ParaData_HMI[i+7];
                can2_ParaData_HMI2_2[i] = ParaData_HMI[i+7];
            }
            Can1ObjDict_Data.PDO_status[19].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 19);
            Can2ObjDict_Data.PDO_status[19].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 19);
        }
    }
}

void canSendTempData_Update(){  // TPDO7
    for(int i = 0; i < 2; i++){
        can1_TempData_HMI[i] = TempData_HMI[i];
        can2_TempData_HMI[i] = TempData_HMI[i];
    }
    Can1ObjDict_Data.PDO_status[6].last_message.cob_id = 0;
    sendOnePDOevent(&Can1ObjDict_Data, 6);
    Can2ObjDict_Data.PDO_status[6].last_message.cob_id = 0;
    sendOnePDOevent(&Can2ObjDict_Data, 6);
}

void canSendGetFaultInfo_Update(){  // TPDO8  && TPDO21
    if(1 == deviceCarId){
        if(CAN_2 == canWorkFlag){
            can2_GetFaultInfo_HMI1 = GetFaultInfo_HMI;
            Can2ObjDict_Data.PDO_status[7].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 7);
        }
        else{
            can1_GetFaultInfo_HMI1 = GetFaultInfo_HMI;
            Can1ObjDict_Data.PDO_status[7].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 7);
        }
    }
    else if(6 == deviceCarId){
//        qDebug()<<"@@@@@@@@@@   HMI Get Fault Count From ERM !!!!!!!!   ";
        if(CAN_2 == canWorkFlag){
            can2_GetFaultInfo_HMI2 = GetFaultInfo_HMI;
            Can2ObjDict_Data.PDO_status[20].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 20);
        }
        else{
            can1_GetFaultInfo_HMI2 = GetFaultInfo_HMI;
            Can1ObjDict_Data.PDO_status[20].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 20);
        }
    }
    else{
        //do nothing
    }
}

void canSendDeleteData_Update(){  // TPDO9  && TPDO22
    if(1 == deviceCarId){
        for(int i = 0; i < 3; i++){
            can1_DeleteData_HMI1[i] = DeleteData_HMI[i];
            can2_DeleteData_HMI1[i] = DeleteData_HMI[i];
        }
        Can1ObjDict_Data.PDO_status[8].last_message.cob_id = 0;
        sendOnePDOevent(&Can1ObjDict_Data, 8);
        Can2ObjDict_Data.PDO_status[8].last_message.cob_id = 0;
        sendOnePDOevent(&Can2ObjDict_Data, 8);
    }
    else if(6 == deviceCarId){
        for(int i = 0; i < 3; i++){
            can1_DeleteData_HMI2[i] = DeleteData_HMI[i];
            can2_DeleteData_HMI2[i] = DeleteData_HMI[i];
        }
        Can1ObjDict_Data.PDO_status[21].last_message.cob_id = 0;
        sendOnePDOevent(&Can1ObjDict_Data, 21);
        Can2ObjDict_Data.PDO_status[21].last_message.cob_id = 0;
        sendOnePDOevent(&Can2ObjDict_Data, 21);
    }
    else{
        // do nothing
    }
}

void canSendCarInitData_Update(){  // TPDO10  && TPDO23
    if(1 == deviceCarId){
        for(int i = 0; i < 8; i++){
            can1_InitSetData_HMI1[i] = InitData_HMI[i];
            can2_InitSetData_HMI1[i] = InitData_HMI[i];
        }
        Can1ObjDict_Data.PDO_status[9].last_message.cob_id = 0;
        sendOnePDOevent(&Can1ObjDict_Data, 9);
        Can2ObjDict_Data.PDO_status[9].last_message.cob_id = 0;
        sendOnePDOevent(&Can2ObjDict_Data, 9);
    }
    else if(6 == deviceCarId){
        for(int i = 0; i < 8; i++){
            can1_InitSetData_HMI2[i] = InitData_HMI[i];
            can2_InitSetData_HMI2[i] = InitData_HMI[i];
        }
        Can1ObjDict_Data.PDO_status[22].last_message.cob_id = 0;
        sendOnePDOevent(&Can1ObjDict_Data, 22);
        Can2ObjDict_Data.PDO_status[22].last_message.cob_id = 0;
        sendOnePDOevent(&Can2ObjDict_Data, 22);
    }
    else{
        // do nothing
    }
}

void canSendParaData3_Update(){  // TPDO11  && TPDO24
    if(masterHMIFlag){
        if(1 == deviceCarId){
            for(int i = 0; i < 8; i++){
                can1_ParaData_HMI1_3[i] = ParaData_HMI[i+7+8];
                can2_ParaData_HMI1_3[i] = ParaData_HMI[i+7+8];
            }
            Can1ObjDict_Data.PDO_status[10].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 10);
            Can2ObjDict_Data.PDO_status[10].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 10);
        }
        else{
            for(int i = 0; i < 8; i++){
                can1_ParaData_HMI2_3[i] = ParaData_HMI[i+7+8];
                can2_ParaData_HMI2_3[i] = ParaData_HMI[i+7+8];
            }
            Can1ObjDict_Data.PDO_status[23].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 23);
            Can2ObjDict_Data.PDO_status[23].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 23);
        }
    }
}

void canSendParaData4_Update(){  // TPDO12  && TPDO25
    if(masterHMIFlag){
        if(1 == deviceCarId){
            for(int i = 0; i < 8; i++){
                can1_ParaData_HMI1_4[i] = ParaData_HMI[i+7+8+8];
                can2_ParaData_HMI1_4[i] = ParaData_HMI[i+7+8+8];
            }
            Can1ObjDict_Data.PDO_status[11].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 11);
            Can2ObjDict_Data.PDO_status[11].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 11);
        }
        else{
            for(int i = 0; i < 8; i++){
                can1_ParaData_HMI2_4[i] = ParaData_HMI[i+7+8+8];
                can2_ParaData_HMI2_4[i] = ParaData_HMI[i+7+8+8];
            }
            Can1ObjDict_Data.PDO_status[24].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 24);
            Can2ObjDict_Data.PDO_status[24].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 24);
        }
    }
}

void canSendCarArriveData1_Update(){  // TPDO13  && TPDO26
    if(masterHMIFlag){
        if(1 == deviceCarId){
            for(int i = 0; i < 2; i++){
                can1_CarArriveData_HMI1_1[i] = CarArriveData_HMI[i];
                can2_CarArriveData_HMI1_1[i] = CarArriveData_HMI[i];
            }
            Can1ObjDict_Data.PDO_status[12].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 12);
            Can2ObjDict_Data.PDO_status[12].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 12);
        }
        else{
            for(int i = 0; i < 2; i++){
                can1_CarArriveData_HMI2_1[i] = CarArriveData_HMI[i];
                can2_CarArriveData_HMI2_1[i] = CarArriveData_HMI[i];
            }
            Can1ObjDict_Data.PDO_status[25].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 25);
            Can2ObjDict_Data.PDO_status[25].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 25);
//            qDebug()<<"HMI2  Send CarArriveData1 Data ###########################";
        }
    }
}

void canSendCarArriveData2_Update(){  // TPDO14  && TPDO27
    if(masterHMIFlag){
        if(1 == deviceCarId){
            for(int i = 0; i < 8; i++){
                can1_CarArriveData_HMI1_2[i] = CarArriveData_HMI[i+2];
                can2_CarArriveData_HMI1_2[i] = CarArriveData_HMI[i+2];
            }
            Can1ObjDict_Data.PDO_status[13].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 13);
            Can2ObjDict_Data.PDO_status[13].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 13);
        }
        else{
            for(int i = 0; i < 8; i++){
                can1_CarArriveData_HMI2_2[i] = CarArriveData_HMI[i+2];
                can2_CarArriveData_HMI2_2[i] = CarArriveData_HMI[i+2];
            }
            Can1ObjDict_Data.PDO_status[26].last_message.cob_id = 0;
            sendOnePDOevent(&Can1ObjDict_Data, 26);
            Can2ObjDict_Data.PDO_status[26].last_message.cob_id = 0;
            sendOnePDOevent(&Can2ObjDict_Data, 26);
//            qDebug()<<"HMI2  Send CarArriveData2 Data ###########################";
        }
    }
}

void canSendHMISetDateTime_Update(){  // TPDO15
    for(int i = 0; i < 7; i++){
        can1_SetDateTime_HMI[i] = SetDate_Time_HMI[i];
        can2_SetDateTime_HMI[i] = SetDate_Time_HMI[i];
    }
    Can1ObjDict_Data.PDO_status[14].last_message.cob_id = 0;
    sendOnePDOevent(&Can1ObjDict_Data, 14);
    Can2ObjDict_Data.PDO_status[14].last_message.cob_id = 0;
    sendOnePDOevent(&Can2ObjDict_Data, 14);
}

void canSendSDOConfig(CO_Data *d, u_int8_t nodeId){
    UNS32 val = 0;
    UNS32 size = 0;

    //can 开始传输SDO
    val = 0x600 | nodeId;
    size = sizeof(UNS32);
    writeLocalDict(d, 0x1280, 0x01, &val, &size, RW);   //修改客户端TSDO node id
    val = 0x580 | nodeId;
    writeLocalDict(d, 0x1280, 0x02, &val, &size, RW);   //修改客户端RSDO node id
    val = nodeId;
    size = sizeof(UNS8);
    writeLocalDict(d,0x1280,0x03,&val,&size,RW);   //修改客户端的服务器 node id 
}

void canSendSDOData(int iSDOId, int iCanFlag){
    switch (iSDOId) {
    case 1:
        canSendDoorCheckData_Update(iCanFlag);
        break;
    case 2:
        canSendACPCheckData_Update(iCanFlag);
        break;
    case 3:
        canSendSIVCheckData_Update(iCanFlag);
        break;
    case 4:
        canSendBCUCheckData_Update(iCanFlag);
        break;
    case 5:
        canSendSpeedCheckData_Update(iCanFlag);
        break;
    case 6:
        canSendFaultInfoOperation_Update(iCanFlag);
        break;
    case 7:
        canSendFaultInfoDoFinished_Update(iCanFlag);
        break;
    case 8:
        canSendSpeedInfoDoFinished_Update(iCanFlag);
        break;
    default:
        break;
    }
}

void canSendDoorCheckData_Update(int canFlag){
    nowSendSDOID = 1;
    if(1 == canFlag){
        for(int i = 0; i < 12; i++){
            can1_DoorCheckData_1[i] = DoorCheck_Data[i];
        }
        canSendSDOConfig(&Can1ObjDict_Data, NODEID_ERM1);
        writeNetworkDictCallBack(&Can1ObjDict_Data, NODEID_ERM1, 0x4000, 0x01, 0x000C, domain, can1_DoorCheckData_1, CheckWriteSDO, 0);
    }
    else if(2 == canFlag){
        for(int i = 0; i < 12; i++){
            can2_DoorCheckData_1[i] = DoorCheck_Data[i];
        }
        canSendSDOConfig(&Can2ObjDict_Data, NODEID_ERM2);
        writeNetworkDictCallBack(&Can2ObjDict_Data, NODEID_ERM2, 0x4000, 0x01, 0x000C, domain, can2_DoorCheckData_1, CheckWriteSDO, 0);
    }
    else{
        // do nothing
    }
}

void canSendACPCheckData_Update(int canFlag){
    nowSendSDOID = 2;
    if(1 == canFlag){
        for(int i = 0; i < 2; i++){
            can1_ACPCheckData_1[i] = ACPCheck_Data[i];
        }
        canSendSDOConfig(&Can1ObjDict_Data, NODEID_ERM1);
        writeNetworkDictCallBack(&Can1ObjDict_Data, NODEID_ERM1, 0x4001, 0x01, 0x0004, domain, can1_ACPCheckData_1, CheckWriteSDO, 0);
    }
    else if(2 == canFlag){
        for(int i = 0; i < 2; i++){
            can2_ACPCheckData_1[i] = ACPCheck_Data[i];
        }
        canSendSDOConfig(&Can2ObjDict_Data, NODEID_ERM2);
        writeNetworkDictCallBack(&Can2ObjDict_Data, NODEID_ERM2, 0x4001, 0x01, 0x0004, domain, can2_ACPCheckData_1, CheckWriteSDO, 0);
    }
    else{
        // do nothing
    }
}

void canSendSIVCheckData_Update(int canFlag){
    nowSendSDOID = 3;
    if(1 == canFlag){
        for(int i = 0; i < 2; i++){
            can1_SIVCheckData_1[i] = SIVCheck_Data[i];
        }
        canSendSDOConfig(&Can1ObjDict_Data, NODEID_ERM1);
        writeNetworkDictCallBack(&Can1ObjDict_Data, NODEID_ERM1, 0x4002, 0x01, 0x0002, domain, can1_SIVCheckData_1, CheckWriteSDO, 0);
    }
    else if(2 == canFlag){
        for(int i = 0; i < 2; i++){
            can2_SIVCheckData_1[i] = SIVCheck_Data[i];
        }
        canSendSDOConfig(&Can2ObjDict_Data, NODEID_ERM2);
        writeNetworkDictCallBack(&Can2ObjDict_Data, NODEID_ERM2, 0x4002, 0x01, 0x0002, domain, can2_SIVCheckData_1, CheckWriteSDO, 0);
    }
    else{
        // do nothing
    }
}

void canSendBCUCheckData_Update(int canFlag){
    nowSendSDOID = 4;
    if(1 == canFlag){
        for(int i = 0; i < 77; i++){
            can1_BCUCheckData_1[i] = BCUCheck_Data[i];
        }
        canSendSDOConfig(&Can1ObjDict_Data, NODEID_ERM1);
        writeNetworkDictCallBack(&Can1ObjDict_Data, NODEID_ERM1, 0x4003, 0x01, 0x0082, domain, can1_BCUCheckData_1, CheckWriteSDO, 0);
    }
    else if(2 == canFlag){
        for(int i = 0; i < 77; i++){
            can2_BCUCheckData_1[i] = BCUCheck_Data[i];
        }
        canSendSDOConfig(&Can2ObjDict_Data, NODEID_ERM2);
        writeNetworkDictCallBack(&Can2ObjDict_Data, NODEID_ERM2, 0x4003, 0x01, 0x0082, domain, can2_BCUCheckData_1, CheckWriteSDO, 0);
    }
    else{
        // do nothing
    }
}

void canSendSpeedCheckData_Update(int canFlag){
    nowSendSDOID = 5;
    if(1 == canFlag){
        for(int i = 0; i < 13; i++){
            can1_SpeedCheckData_1[i] = SpeedCheck_Data[i];
        }
        canSendSDOConfig(&Can1ObjDict_Data, NODEID_ERM1);
        writeNetworkDictCallBack(&Can1ObjDict_Data, NODEID_ERM1, 0x4004, 0x01, 0x000D, domain, can1_SpeedCheckData_1, CheckWriteSDO, 0);
    }
    else if(2 == canFlag){
        for(int i = 0; i < 13; i++){
            can2_SpeedCheckData_1[i] = SpeedCheck_Data[i];
        }
        canSendSDOConfig(&Can2ObjDict_Data, NODEID_ERM2);
        writeNetworkDictCallBack(&Can2ObjDict_Data, NODEID_ERM2, 0x4004, 0x01, 0x000D, domain, can2_SpeedCheckData_1, CheckWriteSDO, 0);
    }
    else{
        //do nothing
    }
}

void canSendFaultInfoOperation_Update(int canFlag){
    nowSendSDOID = 6;
    if(1 == canFlag){
        for(int i = 0; i < 20; i++){
            can1_FaultInfo_HMI_1[i] = FaultInfo_ERM[i];
        }
        canSendSDOConfig(&Can1ObjDict_Data, NODEID_ERM1);
        writeNetworkDictCallBack(&Can1ObjDict_Data, NODEID_ERM1, 0x400B, 0x01, 0x0014, domain, &can1_FaultInfo_HMI_1, CheckWriteSDO, 0);
    }
    else if(2 == canFlag){
        for(int i = 0; i < 20; i++){
            can2_FaultInfo_HMI_1[i] = FaultInfo_ERM[i];
        }
        canSendSDOConfig(&Can2ObjDict_Data, NODEID_ERM2);
        writeNetworkDictCallBack(&Can2ObjDict_Data, NODEID_ERM2, 0x400B, 0x01, 0x0014, domain, &can2_FaultInfo_HMI_1, CheckWriteSDO, 0);
    }
    else{
        //do nothing
    }
}

void canSendFaultInfoDoFinished_Update(int canFlag){
    nowSendSDOID = 7;
    if(1 == canFlag){
        can1_FaultInfo_ERM_2 = 1;
        canSendSDOConfig(&Can1ObjDict_Data, NODEID_ERM1);
        writeNetworkDictCallBack(&Can1ObjDict_Data, NODEID_ERM1, 0x4005, 0x02, 1, uint8, &can1_FaultInfo_ERM_2, CheckWriteSDO, 0);
    }
    else if(2 == canFlag){
        can2_FaultInfo_ERM_2 = 1;
        canSendSDOConfig(&Can2ObjDict_Data, NODEID_ERM2);
        writeNetworkDictCallBack(&Can2ObjDict_Data, NODEID_ERM2, 0x4005, 0x02, 1, uint8, &can2_FaultInfo_ERM_2, CheckWriteSDO, 0);
    }
    else{
        //do nothing
    }
}

void canSendSpeedInfoDoFinished_Update(int canFlag){
    nowSendSDOID = 8;
    if(1 == canFlag){
        can1_SpeedData_ERM_2 = 1;
        canSendSDOConfig(&Can1ObjDict_Data, NODEID_ERM1);
        writeNetworkDictCallBack(&Can1ObjDict_Data, NODEID_ERM1, 0x400A, 0x02, 1, uint8, &can1_SpeedData_ERM_2, CheckWriteSDO, 0);
    }
    else if(2 == canFlag){
        can2_SpeedData_ERM_2 = 1;
        canSendSDOConfig(&Can2ObjDict_Data, NODEID_ERM2);
        writeNetworkDictCallBack(&Can2ObjDict_Data, NODEID_ERM2, 0x400A, 0x02, 1, uint8, &can2_SpeedData_ERM_2, CheckWriteSDO, 0);
    }
    else{
        //do nothing
    }
}

void CheckWriteSDO(CO_Data *d, u_int8_t nodeid){
    UNS32 abortCode;
    UNS8 sdoSendResult = getWriteResultNetworkDict(d, nodeid, &abortCode);
//    printf("\nSDO  SDO SDO SDO Send Send Result :  sdoSendResult == %2.2x \n", sdoSendResult);
    if(/*getWriteResultNetworkDict(d, nodeid, &abortCode)*/ sdoSendResult != SDO_FINISHED){
        /* Finalize last SDO transfer with this node */
        closeSDOtransfer(d, nodeid, SDO_CLIENT);
//        if(&Can1ObjDict_Data == d){
//            printf("\nSDO  SDO SDO SDO Result : CAN1  CAN1 Failed in getting information for slave %2.2x, AbortCode :%4.4x , SDO Send Result = %2.2x \n", nodeid, abortCode, sdoSendResult);
//            if(nowResendSDOCount[0] < 2){
//                nowSendSDOFailedID[0] = nowSendSDOID;
//                nowResendSDOCount[0]++;
//                canSendSDOData(nowSendSDOFailedID[0], 1);
//            }
//            else{
//                nowSendSDOFailedID[0] = 0;
//                nowResendSDOCount[0] = 0;
//            }
//        }
//        else{
//            printf("\nSDO  SDO SDO SDO Result : CAN2 CAN2 Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
//            if(nowResendSDOCount[1] < 2){
//                nowSendSDOFailedID[1] = nowSendSDOID;
//                nowResendSDOCount[1]++;
//                canSendSDOData(nowSendSDOFailedID[1], 2);
//            }
//            else{
//                nowSendSDOFailedID[1] = 0;
//                nowResendSDOCount[1] = 0;
//            }
//        }
    }
    else{
        /* Finalize last SDO transfer with this node */
        closeSDOtransfer(d, nodeid, SDO_CLIENT);

//        if(&Can1ObjDict_Data == d){
//            printf("\nSDO SDO SDO SDO  CAN1 CAN1 CAN1 Send data OK\n");
//            if(nowResendSDOCount[0] > 0){
//                nowSendSDOFailedID[0] = 0;
//                nowResendSDOCount[0] = 0;
//            }
//        }
//        else{
//            printf("\nSDO SDO SDO SDO  CAN2 CAN2 CAN2 Send data OK\n");
//            if(nowResendSDOCount[1] > 0){
//                nowSendSDOFailedID[1] = 0;
//                nowResendSDOCount[1] = 0;
//            }
//        }
    }

    UNS32 val = 0;
    UNS32 size = 0;

    val = 0 ;
    size = sizeof(UNS32);
    writeLocalDict(d,0x1280,0x01,&val,&size,RW);   //修改客户端TSDO node id
    val = 0;
    writeLocalDict(d,0x1280,0x02,&val,&size,RW);   //修改客户端RSDO node id
    val = 0;
    size = sizeof(UNS8);
    writeLocalDict(d,0x1280,0x03,&val,&size,RW);   //修改客户端的服务器 node id
}

UNS32 Oncan_DIData_MC1_Update(CO_Data *d, const indextable * , UNS8 ){
    bool bChangeFlag = false;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        for(int i = 0; i < 7; i++){
            if(can1_DIData_MC1[i] != DI_Data_MC1[i]){
                DI_Data_MC1[i] = can1_DIData_MC1[i];
                bChangeFlag = true;
            }
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        for(int i = 0; i < 7; i++){
            if(can2_DIData_MC1[i] != DI_Data_MC1[i]){ 
                DI_Data_MC1[i] = can2_DIData_MC1[i];
                bChangeFlag = true;
            }
        }
    }
    else{
        bChangeFlag = false;
    }
    if(bChangeFlag){
//        canSignalsSlots->car_DI_Data_MC1_Update();
        canSignalsSlots->car_DoorOperation_Update(CAR_1);
    }
    return 0;
}

UNS32 Oncan_DIData_T_Update(CO_Data *d, const indextable * , UNS8 ){
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        for(int i = 0 ; i < 2; i++){
            if(can1_DIData_T[i] != DI_Data_T[i]){
                DI_Data_T[i] = can1_DIData_T[i];
            }
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        for(int i = 0 ; i < 2; i++){
            if(can2_DIData_T[i] != DI_Data_T[i]){
                DI_Data_T[i] = can2_DIData_T[i];
            }
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_DIData_M_Update(CO_Data *d, const indextable * , UNS8 ){
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        for(int i = 0 ; i < 2; i++){
            if(can1_DIData_M[i] != DI_Data_M[i]){
                DI_Data_M[i] = can1_DIData_M[i];
            }
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        for(int i = 0 ; i < 2; i++){
            if(can2_DIData_M[i] != DI_Data_M[i]){
                DI_Data_M[i] = can2_DIData_M[i];
            }
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_DIData_T1_Update(CO_Data *d, const indextable * , UNS8 ){
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        for(int i = 0 ; i < 2; i++){
            if(can1_DIData_T1[i] != DI_Data_T1[i]){
                DI_Data_T1[i] = can1_DIData_T1[i];
            }
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        for(int i = 0 ; i < 2; i++){
            if(can2_DIData_T1[i] != DI_Data_T1[i]){
                DI_Data_T1[i] = can2_DIData_T1[i];
            }
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_DIData_T2_Update(CO_Data *d, const indextable * , UNS8 ){
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        for(int i = 0 ; i < 2; i++){
            if(can1_DIData_T2[i] != DI_Data_T2[i]){
                DI_Data_T2[i] = can1_DIData_T2[i];
            }
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        for(int i = 0 ; i < 2; i++){
            if(can2_DIData_T2[i] != DI_Data_T2[i]){
                DI_Data_T2[i] = can2_DIData_T2[i];
            }
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_DIData_MC2_Update(CO_Data *d, const indextable * , UNS8 ){
    bool bChangeFlag = false;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        for(int i = 0; i < 7; i++){
            if(can1_DIData_MC2[i] != DI_Data_MC2[i]){
                DI_Data_MC2[i] = can1_DIData_MC2[i];
                bChangeFlag = true;
            }
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        for(int i = 0; i < 7; i++){
            if(can2_DIData_MC2[i] != DI_Data_MC2[i]){
                DI_Data_MC2[i] = can2_DIData_MC2[i];
                bChangeFlag = true;
            }
        }
    }
    else{
        bChangeFlag = false;
    }
    if(bChangeFlag){
//        canSignalsSlots->car_DI_Data_MC2_Update();
        canSignalsSlots->car_DoorOperation_Update(CAR_6);
    }
    return 0;
}

UNS32 Oncan_CarData_HMI_Update(CO_Data *d, const indextable * , UNS8 ){
    if(masterHMIFlag && 1 == deviceCarId){
        for(int i = 0; i < 7; i++){
            can1_CarData_HMI1[i] = Car_Data_HMI[i];
            can2_CarData_HMI1[i] = Car_Data_HMI[i];
        }
        return 0;
    }

    if(&Can1ObjDict_Data == d && !bCan1HMI1HeartFlag){
        bCan1HMI1HeartFlag = true;
        canSignalsSlots->masterHMIUpdate();
//        qDebug()<<"AAAAAAAAAAAAAAAAAAaaa  Can1 HMI1 Car Data Receive AAAAAAAAAAAAAAAAAAAAAAAA";
    }
    else if(&Can2ObjDict_Data == d && !bCan2HMI1HeartFlag){
        bCan2HMI1HeartFlag = true;
        canSignalsSlots->masterHMIUpdate();
//        qDebug()<<"BBBBBBBBBBBBBBBBBBBb  Can2 HMI1 Car Data Receive BBBBBBBBBBBBBBBBBBBBBbbbB";
    }
    else{
        // do nothing
    }

    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        for(int i = 0; i < 7; i++){
            if(can1_CarData_HMI1[i] != Car_Data_HMI[i]){
                Car_Data_HMI[i] = can1_CarData_HMI1[i];
            }
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        for(int i = 0; i < 7; i++){
            if(can2_CarData_HMI1[i] != Car_Data_HMI[i]){
                Car_Data_HMI[i] = can2_CarData_HMI1[i];
            }
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_ATCDateTime_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    bool bChangeFlag = false;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2007:
            for(int i = 0; i < 6; i++){
                if(can1_ATC_DateTime_MC1[i] != ATCDateTime_MC1[i]){
                    ATCDateTime_MC1[i] = can1_ATC_DateTime_MC1[i];
                    bChangeFlag = true;
                }
            }
            break;
        case 0x2075:
            for(int i = 0; i < 6; i++){
                if(can1_ATC_DateTime_MC2[i] != ATCDateTime_MC2[i]){
                    ATCDateTime_MC2[i] = can1_ATC_DateTime_MC2[i];
                    bChangeFlag = true;
                }
            }
            break;
        default:
            bChangeFlag = false;
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2007:
            for(int i = 0; i < 6; i++){
                if(can2_ATC_DateTime_MC1[i] != ATCDateTime_MC1[i]){
                    ATCDateTime_MC1[i] = can2_ATC_DateTime_MC1[i];
                    bChangeFlag = true;
                }
            }
            break;
        case 0x2075:
            for(int i = 0; i < 6; i++){
                if(can2_ATC_DateTime_MC2[i] != ATCDateTime_MC2[i]){
                    ATCDateTime_MC2[i] = can2_ATC_DateTime_MC2[i];
                    bChangeFlag = true;
                }
            }
            break;
        default:
            bChangeFlag = false;
            break;
        }
    }
    else{
        bChangeFlag = false;
    }
    if(bChangeFlag){
        canSignalsSlots->car_ATCDateTime_Update();
    }
    return 0;
}

UNS32 Oncan_DateTime_HMI_Update(CO_Data *d, const indextable * , UNS8 ){
    bool bChangeFlag = false;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        for(int i = 0; i < 7; i++){
            if(can1_DateTime_HMI1[i] != Date_Time_HMI[i]){
                Date_Time_HMI[i] = can1_DateTime_HMI1[i];
                bChangeFlag = true;
            }
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        for(int i = 0; i < 7; i++){
            if(can2_DateTime_HMI1[i] != Date_Time_HMI[i]){
                Date_Time_HMI[i] = can2_DateTime_HMI1[i];
                bChangeFlag = true;
            }
        }
    }
    else{
        bChangeFlag = false;
    }
    if(bChangeFlag){
        canSignalsSlots->car_DateTime_HMI_Update();
    }
    return 0;
}

UNS32 Oncan_DoorData_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2009:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_MC1_12[i] != DoorData_MC1[i]){
                    DoorData_MC1[i] = can1_DoorData_MC1_12[i];
                }
            }
            break;
        case 0x200A:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_T_12[i] != DoorData_T[i]){
                    DoorData_T[i] = can1_DoorData_T_12[i];
                }
            }
            break;
        case 0x200B:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_M_12[i] != DoorData_M[i]){
                    DoorData_M[i] = can1_DoorData_M_12[i];
                }
            }
            break;
        case 0x200C:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_T1_12[i] != DoorData_T1[i]){
                    DoorData_T1[i] = can1_DoorData_T1_12[i];
                }
            }
            break;
        case 0x200D:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_T2_12[i] != DoorData_T2[i]){
                    DoorData_T2[i] = can1_DoorData_T2_12[i];
                }
            }
            break;
        case 0x200E:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_MC2_12[i] != DoorData_MC2[i]){
                    DoorData_MC2[i] = can1_DoorData_MC2_12[i];
                }
            }
            break;
        case 0x206F:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_MC1_34[i] != DoorData_MC1[8+i]){
                    DoorData_MC1[8+i] = can1_DoorData_MC1_34[i];
                }
            }
            break;
        case 0x2070:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_T_34[i] != DoorData_T[8+i]){
                    DoorData_T[8+i] = can1_DoorData_T_34[i];
                }
            }
            break;
        case 0x2071:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_M_34[i] != DoorData_M[8+i]){
                    DoorData_M[8+i] = can1_DoorData_M_34[i];
                }
            }
            break;
        case 0x2072:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_T1_34[i] != DoorData_T1[8+i]){
                    DoorData_T1[8+i] = can1_DoorData_T1_34[i];
                }
            }
            break;
        case 0x2073:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_T2_34[i] != DoorData_T2[8+i]){
                    DoorData_T2[8+i] = can1_DoorData_T2_34[i];
                }
            }
            break;
        case 0x2074:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_MC2_34[i] != DoorData_MC2[8+i]){
                    DoorData_MC2[8+i] = can1_DoorData_MC2_34[i];
                }
            }
            break;
        case 0x20A1:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_MC1_56[i] != DoorData_MC1[8+8+i]){
                    DoorData_MC1[8+8+i] = can1_DoorData_MC1_56[i];
                }
            }
            break;
        case 0x20A2:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_T_56[i] != DoorData_T[8+8+i]){
                    DoorData_T[8+8+i] = can1_DoorData_T_56[i];
                }
            }
            break;
        case 0x20A3:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_M_56[i] != DoorData_M[8+8+i]){
                    DoorData_M[8+8+i] = can1_DoorData_M_56[i];
                }
            }
            break;
        case 0x20A4:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_T1_56[i] != DoorData_T1[8+8+i]){
                    DoorData_T1[8+8+i] = can1_DoorData_T1_56[i];
                }
            }
            break;
        case 0x20A5:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_T2_56[i] != DoorData_T2[8+8+i]){
                    DoorData_T2[8+8+i] = can1_DoorData_T2_56[i];
                }
            }
            break;
        case 0x20A6:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_MC2_56[i] != DoorData_MC2[8+8+i]){
                    DoorData_MC2[8+8+i] = can1_DoorData_MC2_56[i];
                }
            }
            break;
        case 0x20A7:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_MC1_78[i] != DoorData_MC1[8+8+8+i]){
                    DoorData_MC1[8+8+8+i] = can1_DoorData_MC1_78[i];
                }
            }
            break;
        case 0x20A8:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_T_78[i] != DoorData_T[8+8+8+i]){
                    DoorData_T[8+8+8+i] = can1_DoorData_T_78[i];
                }
            }
            break;
        case 0x20A9:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_M_78[i] != DoorData_M[8+8+8+i]){
                    DoorData_M[8+8+8+i] = can1_DoorData_M_78[i];
                }
            }
            break;
        case 0x20AA:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_T1_78[i] != DoorData_T1[8+8+8+i]){
                    DoorData_T1[8+8+8+i] = can1_DoorData_T1_78[i];
                }
            }
            break;
        case 0x20AB:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_T2_78[i] != DoorData_T2[8+8+8+i]){
                    DoorData_T2[8+8+8+i] = can1_DoorData_T2_78[i];
                }
            }
            break;
        case 0x20AC:
            for(int i = 0; i < 8; i++){
                if(can1_DoorData_MC2_78[i] != DoorData_MC2[8+8+8+i]){
                    DoorData_MC2[8+8+8+i] = can1_DoorData_MC2_78[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2009:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_MC1_12[i] != DoorData_MC1[i]){
                    DoorData_MC1[i] = can2_DoorData_MC1_12[i];
                }
            }
            break;
        case 0x200A:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_T_12[i] != DoorData_T[i]){
                    DoorData_T[i] = can2_DoorData_T_12[i];
                }
            }
            break;
        case 0x200B:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_M_12[i] != DoorData_M[i]){
                    DoorData_M[i] = can2_DoorData_M_12[i];
                }
            }
            break;
        case 0x200C:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_T1_12[i] != DoorData_T1[i]){
                    DoorData_T1[i] = can2_DoorData_T1_12[i];
                }
            }
            break;
        case 0x200D:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_T2_12[i] != DoorData_T2[i]){
                    DoorData_T2[i] = can2_DoorData_T2_12[i];
                }
            }
            break;
        case 0x200E:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_MC2_12[i] != DoorData_MC2[i]){
                    DoorData_MC2[i] = can2_DoorData_MC2_12[i];
                }
            }
            break;
        case 0x206F:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_MC1_34[i] != DoorData_MC1[8+i]){
                    DoorData_MC1[8+i] = can2_DoorData_MC1_34[i];
                }
            }
            break;
        case 0x2070:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_T_34[i] != DoorData_T[8+i]){
                    DoorData_T[8+i] = can2_DoorData_T_34[i];
                }
            }
            break;
        case 0x2071:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_M_34[i] != DoorData_M[8+i]){
                    DoorData_M[8+i] = can2_DoorData_M_34[i];
                }
            }
            break;
        case 0x2072:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_T1_34[i] != DoorData_T1[8+i]){
                    DoorData_T1[8+i] = can2_DoorData_T1_34[i];
                }
            }
            break;
        case 0x2073:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_T2_34[i] != DoorData_T2[8+i]){
                    DoorData_T2[8+i] = can2_DoorData_T2_34[i];
                }
            }
            break;
        case 0x2074:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_MC2_34[i] != DoorData_MC2[8+i]){
                    DoorData_MC2[8+i] = can2_DoorData_MC2_34[i];
                }
            }
            break;
        case 0x20A1:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_MC1_56[i] != DoorData_MC1[8+8+i]){
                    DoorData_MC1[8+8+i] = can2_DoorData_MC1_56[i];
                }
            }
            break;
        case 0x20A2:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_T_56[i] != DoorData_T[8+8+i]){
                    DoorData_T[8+8+i] = can2_DoorData_T_56[i];
                }
            }
            break;
        case 0x20A3:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_M_56[i] != DoorData_M[8+8+i]){
                    DoorData_M[8+8+i] = can2_DoorData_M_56[i];
                }
            }
            break;
        case 0x20A4:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_T1_56[i] != DoorData_T1[8+8+i]){
                    DoorData_T1[8+8+i] = can2_DoorData_T1_56[i];
                }
            }
            break;
        case 0x20A5:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_T2_56[i] != DoorData_T2[8+8+i]){
                    DoorData_T2[8+8+i] = can2_DoorData_T2_56[i];
                }
            }
            break;
        case 0x20A6:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_MC2_56[i] != DoorData_MC2[8+8+i]){
                    DoorData_MC2[8+8+i] = can2_DoorData_MC2_56[i];
                }
            }
            break;
        case 0x20A7:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_MC1_78[i] != DoorData_MC1[8+8+8+i]){
                    DoorData_MC1[8+8+8+i] = can2_DoorData_MC1_78[i];
                }
            }
            break;
        case 0x20A8:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_T_78[i] != DoorData_T[8+8+8+i]){
                    DoorData_T[8+8+8+i] = can2_DoorData_T_78[i];
                }
            }
            break;
        case 0x20A9:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_M_78[i] != DoorData_M[8+8+8+i]){
                    DoorData_M[8+8+8+i] = can2_DoorData_M_78[i];
                }
            }
            break;
        case 0x20AA:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_T1_78[i] != DoorData_T1[8+8+8+i]){
                    DoorData_T1[8+8+8+i] = can2_DoorData_T1_78[i];
                }
            }
            break;
        case 0x20AB:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_T2_78[i] != DoorData_T2[8+8+8+i]){
                    DoorData_T2[8+8+8+i] = can2_DoorData_T2_78[i];
                }
            }
            break;
        case 0x20AC:
            for(int i = 0; i < 8; i++){
                if(can2_DoorData_MC2_78[i] != DoorData_MC2[8+8+8+i]){
                    DoorData_MC2[8+8+8+i] = can2_DoorData_MC2_78[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else{
    }

    return 0;
}

UNS32 Oncan_ACCData_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x200F:
            for(int i = 0; i < 8; i++){
                if(can1_ACCData_MC1[i] != ACCData_MC1[i]){
                    ACCData_MC1[i] = can1_ACCData_MC1[i];
                }
            }
            break;
        case 0x2010:
            for(int i = 0; i < 8; i++){
                if(can1_ACCData_T[i] != ACCData_T[i]){
                    ACCData_T[i] = can1_ACCData_T[i];
                }
            }
            break;
        case 0x2011:
            for(int i = 0; i < 8; i++){
                if(can1_ACCData_M[i] != ACCData_M[i]){
                    ACCData_M[i] = can1_ACCData_M[i];
                }
            }
            break;
        case 0x2012:
            for(int i = 0; i < 8; i++){
                if(can1_ACCData_T1[i] != ACCData_T1[i]){
                    ACCData_T1[i] = can1_ACCData_T1[i];
                }
            }
            break;
        case 0x2013:
            for(int i = 0; i < 8; i++){
                if(can1_ACCData_T2[i] != ACCData_T2[i]){
                    ACCData_T2[i] = can1_ACCData_T2[i];
                }
            }
            break;
        case 0x2014:
            for(int i = 0; i < 8; i++){
                if(can1_ACCData_MC2[i] != ACCData_MC2[i]){
                    ACCData_MC2[i] = can1_ACCData_MC2[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x200F:
            for(int i = 0; i < 8; i++){
                if(can2_ACCData_MC1[i] != ACCData_MC1[i]){
                    ACCData_MC1[i] = can2_ACCData_MC1[i];
                }
            }
            break;
        case 0x2010:
            for(int i = 0; i < 8; i++){
                if(can2_ACCData_T[i] != ACCData_T[i]){
                    ACCData_T[i] = can2_ACCData_T[i];
                }
            }
            break;
        case 0x2011:
            for(int i = 0; i < 8; i++){
                if(can2_ACCData_M[i] != ACCData_M[i]){
                    ACCData_M[i] = can2_ACCData_M[i];
                }
            }
            break;
        case 0x2012:
            for(int i = 0; i < 8; i++){
                if(can2_ACCData_T1[i] != ACCData_T1[i]){
                    ACCData_T1[i] = can2_ACCData_T1[i];
                }
            }
            break;
        case 0x2013:
            for(int i = 0; i < 8; i++){
                if(can2_ACCData_T2[i] != ACCData_T2[i]){
                    ACCData_T2[i] = can2_ACCData_T2[i];
                }
            }
            break;
        case 0x2014:
            for(int i = 0; i < 8; i++){
                if(can2_ACCData_MC2[i] != ACCData_MC2[i]){
                    ACCData_MC2[i] = can2_ACCData_MC2[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_ACCData_2_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2063:
            if(can1_ACCData_MC1_2 != ACCData_MC1[8]){
                ACCData_MC1[8] = can1_ACCData_MC1_2;
            }
            break;
        case 0x2064:
            if(can1_ACCData_T_2 != ACCData_T[8]){
                ACCData_T[8] = can1_ACCData_T_2;
            }
            break;
        case 0x2065:
            if(can1_ACCData_M_2 != ACCData_M[8]){
                ACCData_M[8] = can1_ACCData_M_2;
            }
            break;
        case 0x2066:
            if(can1_ACCData_T1_2 != ACCData_T1[8]){
                ACCData_T1[8] = can1_ACCData_T1_2;
            }
            break;
        case 0x2067:
            if(can1_ACCData_T2_2 != ACCData_T2[8]){
                ACCData_T2[8] = can1_ACCData_T2_2;
            }
            break;
        case 0x2068:
            if(can1_ACCData_MC2_2 != ACCData_MC2[8]){
                ACCData_MC2[8] = can1_ACCData_MC2_2;
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2063:
            if(can2_ACCData_MC1_2 != ACCData_MC1[8]){
                ACCData_MC1[8] = can2_ACCData_MC1_2;
            }
            break;
        case 0x2064:
            if(can2_ACCData_T_2 != ACCData_T[8]){
                ACCData_T[8] = can2_ACCData_T_2;
            }
            break;
        case 0x2065:
            if(can2_ACCData_M_2 != ACCData_M[8]){
                ACCData_M[8] = can2_ACCData_M_2;
            }
            break;
        case 0x2066:
            if(can2_ACCData_T1_2 != ACCData_T1[8]){
                ACCData_T1[8] = can2_ACCData_T1_2;
            }
            break;
        case 0x2067:
            if(can2_ACCData_T2_2 != ACCData_T2[8]){
                ACCData_T2[8] = can2_ACCData_T2_2;
            }
            break;
        case 0x2068:
            if(can2_ACCData_MC2_2 != ACCData_MC2[8]){
                ACCData_MC2[8] = can2_ACCData_MC2_2;
            }
            break;
        default:
            break;
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_BCUData_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2015:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_MC1_1[i] != BCUData_MC1[i]){
                    BCUData_MC1[i] = can1_BCUData_MC1_1[i];
                }
            }
            break;
        case 0x2016:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_MC1_2[i] != BCUData_MC1[i+8]){
                    BCUData_MC1[i+8] = can1_BCUData_MC1_2[i];
                }
            }
            break;
        case 0x2017:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_MC1_3[i] != BCUData_MC1[i+8+8]){
                    BCUData_MC1[i+8+8] = can1_BCUData_MC1_3[i];
                }
            }
            break;
        case 0x2018:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_T_1[i] != BCUData_T[i]){
                    BCUData_T[i] = can1_BCUData_T_1[i];
                }
            }
            break;
        case 0x2019:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_T_2[i] != BCUData_T[i+8]){
                    BCUData_T[i+8] = can1_BCUData_T_2[i];
                }
            }
            break;
        case 0x201A:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_T_3[i] != BCUData_T[i+8+8]){
                    BCUData_T[i+8+8] = can1_BCUData_T_3[i];
                }
            }
            break;
        case 0x201B:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_M_1[i] != BCUData_M[i]){
                    BCUData_M[i] = can1_BCUData_M_1[i];
                }
            }
            break;
        case 0x201C:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_M_2[i] != BCUData_M[i+8]){
                    BCUData_M[i+8] = can1_BCUData_M_2[i];
                }
            }
            break;
        case 0x201D:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_M_3[i] != BCUData_M[i+8+8]){
                    BCUData_M[i+8+8] = can1_BCUData_M_3[i];
                }
            }
            break;
        case 0x201E:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_T1_1[i] != BCUData_T1[i]){
                    BCUData_T1[i] = can1_BCUData_T1_1[i];
                }
            }
            break;
        case 0x201F:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_T1_2[i] != BCUData_T1[i+8]){
                    BCUData_T1[i+8] = can1_BCUData_T1_2[i];
                }
            }
            break;
        case 0x2020:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_T1_3[i] != BCUData_T1[i+8+8]){
                    BCUData_T1[i+8+8] = can1_BCUData_T1_3[i];
                }
            }
            break;
        case 0x2021:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_T2_1[i] != BCUData_T2[i]){
                    BCUData_T2[i] = can1_BCUData_T2_1[i];
                }
            }
            break;
        case 0x2022:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_T2_2[i] != BCUData_T2[i+8]){
                    BCUData_T2[i+8] = can1_BCUData_T2_2[i];
                }
            }
            break;
        case 0x2023:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_T2_3[i] != BCUData_T2[i+8+8]){
                    BCUData_T2[i+8+8] = can1_BCUData_T2_3[i];
                }
            }
            break;
        case 0x2024:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_MC2_1[i] != BCUData_MC2[i]){
                    BCUData_MC2[i] = can1_BCUData_MC2_1[i];
                }
            }
            break;
        case 0x2025:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_MC2_2[i] != BCUData_MC2[i+8]){
                    BCUData_MC2[i+8] = can1_BCUData_MC2_2[i];
                }
            }
            break;
        case 0x2026:
            for(int i = 0; i < 8; i++){
                if(can1_BCUData_MC2_3[i] != BCUData_MC2[i+8+8]){
                    BCUData_MC2[i+8+8] = can1_BCUData_MC2_3[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2015:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_MC1_1[i] != BCUData_MC1[i]){
                    BCUData_MC1[i] = can2_BCUData_MC1_1[i];
                }
            }
            break;
        case 0x2016:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_MC1_2[i] != BCUData_MC1[i+8]){
                    BCUData_MC1[i+8] = can2_BCUData_MC1_2[i];
                }
            }
            break;
        case 0x2017:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_MC1_3[i] != BCUData_MC1[i+8+8]){
                    BCUData_MC1[i+8+8] = can2_BCUData_MC1_3[i];
                }
            }
            break;
        case 0x2018:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_T_1[i] != BCUData_T[i]){
                    BCUData_T[i] = can2_BCUData_T_1[i];
                }
            }
            break;
        case 0x2019:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_T_2[i] != BCUData_T[i+8]){
                    BCUData_T[i+8] = can2_BCUData_T_2[i];
                }
            }
            break;
        case 0x201A:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_T_3[i] != BCUData_T[i+8+8]){
                    BCUData_T[i+8+8] = can2_BCUData_T_3[i];
                }
            }
            break;
        case 0x201B:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_M_1[i] != BCUData_M[i]){
                    BCUData_M[i] = can2_BCUData_M_1[i];
                }
            }
            break;
        case 0x201C:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_M_2[i] != BCUData_M[i+8]){
                    BCUData_M[i+8] = can2_BCUData_M_2[i];
                }
            }
            break;
        case 0x201D:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_M_3[i] != BCUData_M[i+8+8]){
                    BCUData_M[i+8+8] = can2_BCUData_M_3[i];
                }
            }
            break;
        case 0x201E:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_T1_1[i] != BCUData_T1[i]){
                    BCUData_T1[i] = can2_BCUData_T1_1[i];
                }
            }
            break;
        case 0x201F:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_T1_2[i] != BCUData_T1[i+8]){
                    BCUData_T1[i+8] = can2_BCUData_T1_2[i];
                }
            }
            break;
        case 0x2020:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_T1_3[i] != BCUData_T1[i+8+8]){
                    BCUData_T1[i+8+8] = can2_BCUData_T1_3[i];
                }
            }
            break;
        case 0x2021:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_T2_1[i] != BCUData_T2[i]){
                    BCUData_T2[i] = can2_BCUData_T2_1[i];
                }
            }
            break;
        case 0x2022:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_T2_2[i] != BCUData_T2[i+8]){
                    BCUData_T2[i+8] = can2_BCUData_T2_2[i];
                }
            }
            break;
        case 0x2023:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_T2_3[i] != BCUData_T2[i+8+8]){
                    BCUData_T2[i+8+8] = can2_BCUData_T2_3[i];
                }
            }
            break;
        case 0x2024:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_MC2_1[i] != BCUData_MC2[i]){
                    BCUData_MC2[i] = can2_BCUData_MC2_1[i];
                }
            }
            break;
        case 0x2025:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_MC2_2[i] != BCUData_MC2[i+8]){
                    BCUData_MC2[i+8] = can2_BCUData_MC2_2[i];
                }
            }
            break;
        case 0x2026:
            for(int i = 0; i < 8; i++){
                if(can2_BCUData_MC2_3[i] != BCUData_MC2[i+8+8]){
                    BCUData_MC2[i+8+8] = can2_BCUData_MC2_3[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else{
    }

    return 0;
}

UNS32 Oncan_ATCData_1_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2027:
            for(int i = 0; i < 7; i++){
                if(can1_ATCData_MC1_1[i] != ATCData_MC1[i]){
                    ATCData_MC1[i] = can1_ATCData_MC1_1[i];
                }
            }
            break;
        case 0x202B:
            for(int i = 0; i < 7; i++){
                if(can1_ATCData_MC2_1[i] != ATCData_MC2[i]){
                    ATCData_MC2[i] = can1_ATCData_MC2_1[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2027:
            for(int i = 0; i < 7; i++){
                if(can2_ATCData_MC1_1[i] != ATCData_MC1[i]){
                    ATCData_MC1[i] = can2_ATCData_MC1_1[i];
                }
            }
            break;
        case 0x202B:
            for(int i = 0; i < 7; i++){
                if(can2_ATCData_MC2_1[i] != ATCData_MC2[i]){
                    ATCData_MC2[i] = can2_ATCData_MC2_1[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_ATCData_2_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2028:
            for(int i = 0; i < 8; i++){
                if(can1_ATCData_MC1_2[i] != ATCData_MC1[i+7]){
                    ATCData_MC1[i+7] = can1_ATCData_MC1_2[i];
                }
            }
            break;
        case 0x202C:
            for(int i = 0; i < 8; i++){
                if(can1_ATCData_MC2_2[i] != ATCData_MC2[i+7]){
                    ATCData_MC2[i+7] = can1_ATCData_MC2_2[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2028:
            for(int i = 0; i < 8; i++){
                if(can2_ATCData_MC1_2[i] != ATCData_MC1[i+7]){
                    ATCData_MC1[i+7] = can2_ATCData_MC1_2[i];
                }
            }
            break;
        case 0x202C:
            for(int i = 0; i < 8; i++){
                if(can2_ATCData_MC2_2[i] != ATCData_MC2[i+7]){
                    ATCData_MC2[i+7] = can2_ATCData_MC2_2[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_ATCData_3_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2029:
            for(int i = 0; i < 8; i++){
                if(can1_ATCData_MC1_3[i] != ATCData_MC1[i+7+8]){
                    ATCData_MC1[i+7+8] = can1_ATCData_MC1_3[i];
                }
            }
            break;
        case 0x202D:
            for(int i = 0; i < 8; i++){
                if(can1_ATCData_MC2_3[i] != ATCData_MC2[i+7+8]){
                    ATCData_MC2[i+7+8] = can1_ATCData_MC2_3[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2029:
            for(int i = 0; i < 8; i++){
                if(can2_ATCData_MC1_3[i] != ATCData_MC1[i+7+8]){
                    ATCData_MC1[i+7+8] = can2_ATCData_MC1_3[i];
                }
            }
            break;
        case 0x202D:
            for(int i = 0; i < 8; i++){
                if(can2_ATCData_MC2_3[i] != ATCData_MC2[i+7+8]){
                    ATCData_MC2[i+7+8] = can2_ATCData_MC2_3[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_ATCData_4_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x202A:
            for(int i = 0; i < 4; i++){
                if(can1_ATCData_MC1_4[i] != ATCData_MC1[i+7+8+8]){
                    ATCData_MC1[i+7+8+8] = can1_ATCData_MC1_4[i];
                }
            }
            break;
        case 0x202E:
            for(int i = 0; i < 4; i++){
                if(can1_ATCData_MC2_4[i] != ATCData_MC2[i+7+8+8]){
                    ATCData_MC2[i+7+8+8] = can1_ATCData_MC2_4[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x202A:
            for(int i = 0; i < 4; i++){
                if(can2_ATCData_MC1_4[i] != ATCData_MC1[i+7+8+8]){
                    ATCData_MC1[i+7+8+8] = can2_ATCData_MC1_4[i];
                }
            }
            break;
        case 0x202E:
            for(int i = 0; i < 4; i++){
                if(can2_ATCData_MC2_4[i] != ATCData_MC2[i+7+8+8]){
                    ATCData_MC2[i+7+8+8] = can2_ATCData_MC2_4[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else{
    }

    return 0;
}

UNS32 Oncan_PISData_1_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x202F:
            for(int i = 0; i < 8; i++){
                if(can1_PISData_MC1_1[i] != PISData_MC1[i]){
                    PISData_MC1[i] = can1_PISData_MC1_1[i];
                }
            }
            break;
        case 0x2031:
            for(int i = 0; i < 8; i++){
                if(can1_PISData_MC2_1[i] != PISData_MC2[i]){
                    PISData_MC2[i] = can1_PISData_MC2_1[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x202F:
            for(int i = 0; i < 8; i++){
                if(can2_PISData_MC1_1[i] != PISData_MC1[i]){
                    PISData_MC1[i] = can2_PISData_MC1_1[i];
                }
            }
            break;
        case 0x2031:
            for(int i = 0; i < 8; i++){
                if(can2_PISData_MC2_1[i] != PISData_MC2[i]){
                    PISData_MC2[i] = can2_PISData_MC2_1[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else{
    }

    return 0;
}

UNS32 Oncan_PISData_2_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2030:
            for(int i = 0; i < 3; i++){
                if(can1_PISData_MC1_2[i] != PISData_MC1[i+8]){
                    PISData_MC1[i+8] = can1_PISData_MC1_2[i];
                }
            }
            break;
        case 0x2032:
            for(int i = 0; i < 3; i++){
                if(can1_PISData_MC2_2[i] != PISData_MC2[i+8]){
                    PISData_MC2[i+8] = can1_PISData_MC2_2[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2030:
            for(int i = 0; i < 3; i++){
                if(can2_PISData_MC1_2[i] != PISData_MC1[i+8]){
                    PISData_MC1[i+8] = can2_PISData_MC1_2[i];
                }
            }
            break;
        case 0x2032:
            for(int i = 0; i < 3; i++){
                if(can2_PISData_MC2_2[i] != PISData_MC2[i+8]){
                    PISData_MC2[i+8] = can2_PISData_MC2_2[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else{
    }

    return 0;
}

UNS32 Oncan_VVVFData_1_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    CAR_ID carId = CAR_NULL;
    bool bChangeFlag = false;
    int vvvfIndex = 0;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2033:
            carId = CAR_1;
            for(int i = 0; i < 8; i++){
                if(can1_VVVF1Data_MC1_1[i] != VVVF1Data_MC1[i]){
                    VVVF1Data_MC1[i] = can1_VVVF1Data_MC1_1[i];
                    bChangeFlag = false;
                    vvvfIndex = 1;
                }
            }
            break;
        case 0x2038:
            carId = CAR_1;
            for(int i = 0; i < 8; i++){
                if(can1_VVVF2Data_MC1_1[i] != VVVF2Data_MC1[i]){
                    VVVF2Data_MC1[i] = can1_VVVF2Data_MC1_1[i];
                    bChangeFlag = true;
                    vvvfIndex = 2;
                }
            }
            break;
        case 0x203D:
            carId = CAR_3;
            for(int i = 0; i < 8; i++){
                if(can1_VVVF1Data_M_1[i] != VVVF1Data_M[i]){
                    VVVF1Data_M[i] = can1_VVVF1Data_M_1[i];
                    bChangeFlag = true;
                    vvvfIndex = 1;
                }
            }
            break;
        case 0x2042:
            carId = CAR_3;
            for(int i = 0; i < 8; i++){
                if(can1_VVVF2Data_M_1[i] != VVVF2Data_M[i]){
                VVVF2Data_M[i] = can1_VVVF2Data_M_1[i];
                bChangeFlag = true;
                vvvfIndex = 2;
                }
            }
            break;
        case 0x2047:
            carId = CAR_6;
            for(int i = 0; i < 8; i++){
                if(can1_VVVF1Data_MC2_1[i] != VVVF1Data_MC2[i]){
                    VVVF1Data_MC2[i] = can1_VVVF1Data_MC2_1[i];
                    bChangeFlag = true;
                    vvvfIndex = 1;
                }
            }
            break;
        case 0x204C:
            carId = CAR_6;
            for(int i = 0; i < 8; i++){
                if(can1_VVVF2Data_MC2_1[i] != VVVF2Data_MC2[i]){
                    VVVF2Data_MC2[i] = can1_VVVF2Data_MC2_1[i];
                    bChangeFlag = true;
                    vvvfIndex = 2;
                }
            }
            break;
        default:
            carId = CAR_NULL;
            bChangeFlag = false;
            vvvfIndex = 0;
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2033:
            carId = CAR_1;
            for(int i = 0; i < 8; i++){
                if(can2_VVVF1Data_MC1_1[i] != VVVF1Data_MC1[i]){
                    VVVF1Data_MC1[i] = can2_VVVF1Data_MC1_1[i];
                    bChangeFlag = false;
                    vvvfIndex = 1;
                }
            }
            break;
        case 0x2038:
            carId = CAR_1;
            for(int i = 0; i < 8; i++){
                if(can2_VVVF2Data_MC1_1[i] != VVVF2Data_MC1[i]){
                    VVVF2Data_MC1[i] = can2_VVVF2Data_MC1_1[i];
                    bChangeFlag = true;
                    vvvfIndex = 2;
                }
            }
            break;
        case 0x203D:
            carId = CAR_3;
            for(int i = 0; i < 8; i++){
                if(can2_VVVF1Data_M_1[i] != VVVF1Data_M[i]){
                    VVVF1Data_M[i] = can2_VVVF1Data_M_1[i];
                    bChangeFlag = true;
                    vvvfIndex = 1;
                }
            }
            break;
        case 0x2042:
            carId = CAR_3;
            for(int i = 0; i < 8; i++){
                if(can2_VVVF2Data_M_1[i] != VVVF2Data_M[i]){
                VVVF2Data_M[i] = can2_VVVF2Data_M_1[i];
                bChangeFlag = true;
                vvvfIndex = 2;
                }
            }
            break;
        case 0x2047:
            carId = CAR_6;
            for(int i = 0; i < 8; i++){
                if(can2_VVVF1Data_MC2_1[i] != VVVF1Data_MC2[i]){
                    VVVF1Data_MC2[i] = can2_VVVF1Data_MC2_1[i];
                    bChangeFlag = true;
                    vvvfIndex = 1;
                }
            }
            break;
        case 0x204C:
            carId = CAR_6;
            for(int i = 0; i < 8; i++){
                if(can2_VVVF2Data_MC2_1[i] != VVVF2Data_MC2[i]){
                    VVVF2Data_MC2[i] = can2_VVVF2Data_MC2_1[i];
                    bChangeFlag = true;
                    vvvfIndex = 2;
                }
            }
            break;
        default:
            carId = CAR_NULL;
            bChangeFlag = false;
            vvvfIndex = 0;
            break;
        }
    }
    else{
        carId = CAR_NULL;
        bChangeFlag = false;
        vvvfIndex = 0;
    }
    if(bChangeFlag){
        canSignalsSlots->car_VVVFData_1_Update(carId, vvvfIndex);
    }
    return 0;
}

UNS32 Oncan_VVVFData_2_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2034:
            for(int i = 0; i < 6; i++){
                if(can1_VVVF1Data_MC1_2[i] != VVVF1Data_MC1[i+8]){
                    VVVF1Data_MC1[i+8] = can1_VVVF1Data_MC1_2[i];
//                    qDebug()<<"MC1 Car  Can1 VVVF1 PDO2 Data == "<<can1_VVVF1Data_MC1_2[0]<<" "<<can1_VVVF1Data_MC1_2[1]<<" "<<can1_VVVF1Data_MC1_2[2]<<" "<<can1_VVVF1Data_MC1_2[3]<<" "<<can1_VVVF1Data_MC1_2[4]<<" "<<can1_VVVF1Data_MC1_2[5];
                }
            }
            break;
        case 0x2039:
            for(int i = 0; i < 6; i++){
                if(can1_VVVF2Data_MC1_2[i] != VVVF2Data_MC1[i+8]){
                    VVVF2Data_MC1[i+8] = can1_VVVF2Data_MC1_2[i];
                }
            }
            break;
        case 0x203E:
            for(int i = 0; i < 6; i++){
                if(can1_VVVF1Data_M_2[i] != VVVF1Data_M[i+8]){
                    VVVF1Data_M[i+8] = can1_VVVF1Data_M_2[i];
                }
            }
            break;
        case 0x2043:
            for(int i = 0; i < 6; i++){
                if(can1_VVVF2Data_M_2[i] != VVVF2Data_M[i+8]){
                    VVVF2Data_M[i+8] = can1_VVVF2Data_M_2[i];
                }
            }
            break;
        case 0x2048:
            for(int i = 0; i < 6; i++){
                if(can1_VVVF1Data_MC2_2[i] != VVVF1Data_MC2[i+8]){
                    VVVF1Data_MC2[i+8] = can1_VVVF1Data_MC2_2[i];
                }
            }
            break;
        case 0x204D:
            for(int i = 0; i < 6; i++){
                if(can1_VVVF2Data_MC2_2[i] != VVVF2Data_MC2[i+8]){
                    VVVF2Data_MC2[i+8] = can1_VVVF2Data_MC2_2[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2034:
            for(int i = 0; i < 6; i++){
                if(can2_VVVF1Data_MC1_2[i] != VVVF1Data_MC1[i+8]){
                    VVVF1Data_MC1[i+8] = can2_VVVF1Data_MC1_2[i];
//                    qDebug()<<"MC1 Car  Can1 VVVF1 PDO2 Data == "<<can2_VVVF1Data_MC1_2[0]<<" "<<can2_VVVF1Data_MC1_2[1]<<" "<<can2_VVVF1Data_MC1_2[2]<<" "<<can2_VVVF1Data_MC1_2[3]<<" "<<can2_VVVF1Data_MC1_2[4]<<" "<<can2_VVVF1Data_MC1_2[5];
                }
            }
            break;
        case 0x2039:
            for(int i = 0; i < 6; i++){
                if(can2_VVVF2Data_MC1_2[i] != VVVF2Data_MC1[i+8]){
                    VVVF2Data_MC1[i+8] = can2_VVVF2Data_MC1_2[i];
                }
            }
            break;
        case 0x203E:
            for(int i = 0; i < 6; i++){
                if(can2_VVVF1Data_M_2[i] != VVVF1Data_M[i+8]){
                    VVVF1Data_M[i+8] = can2_VVVF1Data_M_2[i];
                }
            }
            break;
        case 0x2043:
            for(int i = 0; i < 6; i++){
                if(can2_VVVF2Data_M_2[i] != VVVF2Data_M[i+8]){
                    VVVF2Data_M[i+8] = can2_VVVF2Data_M_2[i];
                }
            }
            break;
        case 0x2048:
            for(int i = 0; i < 6; i++){
                if(can2_VVVF1Data_MC2_2[i] != VVVF1Data_MC2[i+8]){
                    VVVF1Data_MC2[i+8] = can2_VVVF1Data_MC2_2[i];
                }
            }
            break;
        case 0x204D:
            for(int i = 0; i < 6; i++){
                if(can2_VVVF2Data_MC2_2[i] != VVVF2Data_MC2[i+8]){
                    VVVF2Data_MC2[i+8] = can2_VVVF2Data_MC2_2[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_VVVFData_3_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2035:
            for(int i = 0; i < 8; i++){
                if(can1_VVVF1Data_MC1_3[i] != VVVF1Data_MC1[i+8+6]){
                    VVVF1Data_MC1[i+8+6] = can1_VVVF1Data_MC1_3[i];
                }
            }
            break;
        case 0x203A:
            for(int i = 0; i < 8; i++){
                if(can1_VVVF2Data_MC1_3[i] != VVVF2Data_MC1[i+8+6]){
                    VVVF2Data_MC1[i+8+6] = can1_VVVF2Data_MC1_3[i];
                }
            }
            break;
        case 0x203F:
            for(int i = 0; i < 8; i++){
                if(can1_VVVF1Data_M_3[i] != VVVF1Data_M[i+8+6]){
                    VVVF1Data_M[i+8+6] = can1_VVVF1Data_M_3[i];
                }
            }
            break;
        case 0x2044:
            for(int i = 0; i < 8; i++){
                if(can1_VVVF2Data_M_3[i] != VVVF2Data_M[i+8+6]){
                    VVVF2Data_M[i+8+6] = can1_VVVF2Data_M_3[i];
                }
            }
            break;
        case 0x2049:
            for(int i = 0; i < 8; i++){
                if(can1_VVVF1Data_MC2_3[i] != VVVF1Data_MC2[i+8+6]){
                    VVVF1Data_MC2[i+8+6] = can1_VVVF1Data_MC2_3[i];
                }
            }
            break;
        case 0x204E:
            for(int i = 0; i < 8; i++){
                if(can1_VVVF2Data_MC2_3[i] != VVVF2Data_MC2[i+8+6]){
                    VVVF2Data_MC2[i+8+6] = can1_VVVF2Data_MC2_3[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2035:
            for(int i = 0; i < 8; i++){
                if(can2_VVVF1Data_MC1_3[i] != VVVF1Data_MC1[i+8+6]){
                    VVVF1Data_MC1[i+8+6] = can2_VVVF1Data_MC1_3[i];
                }
            }
            break;
        case 0x203A:
            for(int i = 0; i < 8; i++){
                if(can2_VVVF2Data_MC1_3[i] != VVVF2Data_MC1[i+8+6]){
                    VVVF2Data_MC1[i+8+6] = can2_VVVF2Data_MC1_3[i];
                }
            }
            break;
        case 0x203F:
            for(int i = 0; i < 8; i++){
                if(can2_VVVF1Data_M_3[i] != VVVF1Data_M[i+8+6]){
                    VVVF1Data_M[i+8+6] = can2_VVVF1Data_M_3[i];
                }
            }
            break;
        case 0x2044:
            for(int i = 0; i < 8; i++){
                if(can2_VVVF2Data_M_3[i] != VVVF2Data_M[i+8+6]){
                    VVVF2Data_M[i+8+6] = can2_VVVF2Data_M_3[i];
                }
            }
            break;
        case 0x2049:
            for(int i = 0; i < 8; i++){
                if(can2_VVVF1Data_MC2_3[i] != VVVF1Data_MC2[i+8+6]){
                    VVVF1Data_MC2[i+8+6] = can2_VVVF1Data_MC2_3[i];
                }
            }
            break;
        case 0x204E:
            for(int i = 0; i < 8; i++){
                if(can2_VVVF2Data_MC2_3[i] != VVVF2Data_MC2[i+8+6]){
                    VVVF2Data_MC2[i+8+6] = can2_VVVF2Data_MC2_3[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_VVVFData_4_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2036:
            for(int i = 0; i < 8; i++){
                if(can1_VVVF1Data_MC1_4[i] != VVVF1Data_MC1[i+8+6+8]){
                    VVVF1Data_MC1[i+8+6+8] = can1_VVVF1Data_MC1_4[i];
                }
            }
            break;
        case 0x203B:
            for(int i = 0; i < 8; i++){
                if(can1_VVVF2Data_MC1_4[i] != VVVF2Data_MC1[i+8+6+8]){
                    VVVF2Data_MC1[i+8+6+8] = can1_VVVF2Data_MC1_4[i];
                }
            }
            break;
        case 0x2040:
            for(int i = 0; i < 8; i++){
                if(can1_VVVF1Data_M_4[i] != VVVF1Data_M[i+8+6+8]){
                    VVVF1Data_M[i+8+6+8] = can1_VVVF1Data_M_4[i];
                }
            }
            break;
        case 0x2045:
            for(int i = 0; i < 8; i++){
                if(can1_VVVF2Data_M_4[i] != VVVF2Data_M[i+8+6+8]){
                    VVVF2Data_M[i+8+6+8] = can1_VVVF2Data_M_4[i];
                }
            }
            break;
        case 0x204A:
            for(int i = 0; i < 8; i++){
                if(can1_VVVF1Data_MC2_4[i] != VVVF1Data_MC2[i+8+6+8]){
                    VVVF1Data_MC2[i+8+6+8] = can1_VVVF1Data_MC2_4[i];
                }
            }
            break;
        case 0x204F:
            for(int i = 0; i < 8; i++){
                if(can1_VVVF2Data_MC2_4[i] != VVVF2Data_MC2[i+8+6+8]){
                    VVVF2Data_MC2[i+8+6+8] = can1_VVVF2Data_MC2_4[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2036:
            for(int i = 0; i < 8; i++){
                if(can2_VVVF1Data_MC1_4[i] != VVVF1Data_MC1[i+8+6+8]){
                    VVVF1Data_MC1[i+8+6+8] = can2_VVVF1Data_MC1_4[i];
                }
            }
            break;
        case 0x203B:
            for(int i = 0; i < 8; i++){
                if(can2_VVVF2Data_MC1_4[i] != VVVF2Data_MC1[i+8+6+8]){
                    VVVF2Data_MC1[i+8+6+8] = can2_VVVF2Data_MC1_4[i];
                }
            }
            break;
        case 0x2040:
            for(int i = 0; i < 8; i++){
                if(can2_VVVF1Data_M_4[i] != VVVF1Data_M[i+8+6+8]){
                    VVVF1Data_M[i+8+6+8] = can2_VVVF1Data_M_4[i];
                }
            }
            break;
        case 0x2045:
            for(int i = 0; i < 8; i++){
                if(can2_VVVF2Data_M_4[i] != VVVF2Data_M[i+8+6+8]){
                    VVVF2Data_M[i+8+6+8] = can2_VVVF2Data_M_4[i];
                }
            }
            break;
        case 0x204A:
            for(int i = 0; i < 8; i++){
                if(can2_VVVF1Data_MC2_4[i] != VVVF1Data_MC2[i+8+6+8]){
                    VVVF1Data_MC2[i+8+6+8] = can2_VVVF1Data_MC2_4[i];
                }
            }
            break;
        case 0x204F:
            for(int i = 0; i < 8; i++){
                if(can2_VVVF2Data_MC2_4[i] != VVVF2Data_MC2[i+8+6+8]){
                    VVVF2Data_MC2[i+8+6+8] = can2_VVVF2Data_MC2_4[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_VVVFData_5_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2037:
            for(int i = 0; i < 6; i++){
                if(can1_VVVF1Data_MC1_5[i] != VVVF1Data_MC1[i+8+6+8+8]){
                    VVVF1Data_MC1[i+8+6+8+8] = can1_VVVF1Data_MC1_5[i];
                }
            }
            break;
        case 0x203C:
            for(int i = 0; i < 6; i++){
                if(can1_VVVF2Data_MC1_5[i] != VVVF2Data_MC1[i+8+6+8+8]){
                    VVVF2Data_MC1[i+8+6+8+8] = can1_VVVF2Data_MC1_5[i];
                }
            }
            break;
        case 0x2041:
            for(int i = 0; i < 6; i++){
                if(can1_VVVF1Data_M_5[i] != VVVF1Data_M[i+8+6+8+8]){
                    VVVF1Data_M[i+8+6+8+8] = can1_VVVF1Data_M_5[i];
                }
            }
            break;
        case 0x2046:
            for(int i = 0; i < 6; i++){
                if(can1_VVVF2Data_M_5[i] != VVVF2Data_M[i+8+6+8+8]){
                    VVVF2Data_M[i+8+6+8+8] = can1_VVVF2Data_M_5[i];
                }
            }
            break;
        case 0x204B:
            for(int i = 0; i < 6; i++){
                if(can1_VVVF1Data_MC2_5[i] != VVVF1Data_MC2[i+8+6+8+8]){
                    VVVF1Data_MC2[i+8+6+8+8] = can1_VVVF1Data_MC2_5[i];
                }
            }
            break;
        case 0x2050:
            for(int i = 0; i < 6; i++){
                if(can1_VVVF2Data_MC2_5[i] != VVVF2Data_MC2[i+8+6+8+8]){
                    VVVF2Data_MC2[i+8+6+8+8] = can1_VVVF2Data_MC2_5[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2037:
            for(int i = 0; i < 6; i++){
                if(can2_VVVF1Data_MC1_5[i] != VVVF1Data_MC1[i+8+6+8+8]){
                    VVVF1Data_MC1[i+8+6+8+8] = can2_VVVF1Data_MC1_5[i];
                }
            }
            break;
        case 0x203C:
            for(int i = 0; i < 6; i++){
                if(can2_VVVF2Data_MC1_5[i] != VVVF2Data_MC1[i+8+6+8+8]){
                    VVVF2Data_MC1[i+8+6+8+8] = can2_VVVF2Data_MC1_5[i];
                }
            }
            break;
        case 0x2041:
            for(int i = 0; i < 6; i++){
                if(can2_VVVF1Data_M_5[i] != VVVF1Data_M[i+8+6+8+8]){
                    VVVF1Data_M[i+8+6+8+8] = can2_VVVF1Data_M_5[i];
                }
            }
            break;
        case 0x2046:
            for(int i = 0; i < 6; i++){
                if(can2_VVVF2Data_M_5[i] != VVVF2Data_M[i+8+6+8+8]){
                    VVVF2Data_M[i+8+6+8+8] = can2_VVVF2Data_M_5[i];
                }
            }
            break;
        case 0x204B:
            for(int i = 0; i < 6; i++){
                if(can2_VVVF1Data_MC2_5[i] != VVVF1Data_MC2[i+8+6+8+8]){
                    VVVF1Data_MC2[i+8+6+8+8] = can2_VVVF1Data_MC2_5[i];
                }
            }
            break;
        case 0x2050:
            for(int i = 0; i < 6; i++){
                if(can2_VVVF2Data_MC2_5[i] != VVVF2Data_MC2[i+8+6+8+8]){
                    VVVF2Data_MC2[i+8+6+8+8] = can2_VVVF2Data_MC2_5[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else{
    }

    return 0;
}

UNS32 Oncan_SIVData_1_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2051:
            for(int i = 0; i < 7; i++){
                if(can1_SIVData_T_1[i] != SIVData_T[i]){
                    SIVData_T[i] = can1_SIVData_T_1[i];
                }
            }
            break;
        case 0x2054:
            for(int i = 0; i < 7; i++){
                if(can1_SIVData_T1_1[i] != SIVData_T1[i]){
                    SIVData_T1[i] = can1_SIVData_T1_1[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2051:
            for(int i = 0; i < 7; i++){
                if(can2_SIVData_T_1[i] != SIVData_T[i]){
                    SIVData_T[i] = can2_SIVData_T_1[i];
                }
            }
            break;
        case 0x2054:
            for(int i = 0; i < 7; i++){
                if(can2_SIVData_T1_1[i] != SIVData_T1[i]){
                    SIVData_T1[i] = can2_SIVData_T1_1[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_SIVData_2_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2052:
            for(int i = 0; i < 8; i++){
                if(can1_SIVData_T_2[i] != SIVData_T[i+7]){
                    SIVData_T[i+7] = can1_SIVData_T_2[i];
                }
            }
            break;
        case 0x2055:
            for(int i = 0; i < 8; i++){
                if(can1_SIVData_T1_2[i] != SIVData_T1[i+7]){
                    SIVData_T1[i+7] = can1_SIVData_T1_2[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2052:
            for(int i = 0; i < 8; i++){
                if(can2_SIVData_T_2[i] != SIVData_T[i+7]){
                    SIVData_T[i+7] = can2_SIVData_T_2[i];
                }
            }
            break;
        case 0x2055:
            for(int i = 0; i < 8; i++){
                if(can2_SIVData_T1_2[i] != SIVData_T1[i+7]){
                    SIVData_T1[i+7] = can2_SIVData_T1_2[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_SIVData_3_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2053:
            for(int i = 0; i < 3; i++){
                if(can1_SIVData_T_3[i] != SIVData_T[i+7+8]){
                    SIVData_T[i+7+8] = can1_SIVData_T_3[i];
                }
            }
            break;
        case 0x2056:
            for(int i = 0; i < 3; i++){
                if(can1_SIVData_T1_3[i] != SIVData_T1[i+7+8]){
                    SIVData_T1[i+7+8] = can1_SIVData_T1_3[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2053:
            for(int i = 0; i < 3; i++){
                if(can2_SIVData_T_3[i] != SIVData_T[i+7+8]){
                    SIVData_T[i+7+8] = can2_SIVData_T_3[i];
                }
            }
            break;
        case 0x2056:
            for(int i = 0; i < 3; i++){
                if(can2_SIVData_T1_3[i] != SIVData_T1[i+7+8]){
                    SIVData_T1[i+7+8] = can2_SIVData_T1_3[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_ComnErr_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2057:
            for(int i = 0; i < 2; i++){
                if(can1_ComnErr_MC1[i] != SubDev_ComnErr_MC1[i]){
                    SubDev_ComnErr_MC1[i] = can1_ComnErr_MC1[i];
                }
            }
            break;
        case 0x2058:
            for(int i = 0; i < 2; i++){
                if(can1_ComnErr_T[i] != SubDev_ComnErr_T[i]){
                    SubDev_ComnErr_T[i] = can1_ComnErr_T[i];
                }
            }
            break;
        case 0x2059:
            for(int i = 0; i < 2; i++){
                if(can1_ComnErr_M[i] != SubDev_ComnErr_M[i]){
                    SubDev_ComnErr_M[i] = can1_ComnErr_M[i];
                }
            }
            break;
        case 0x205A:
            for(int i = 0; i < 2; i++){
                if(can1_ComnErr_T1[i] != SubDev_ComnErr_T1[i]){
                    SubDev_ComnErr_T1[i] = can1_ComnErr_T1[i];
                }
            }
            break;
        case 0x205B:
            for(int i = 0; i < 2; i++){
                if(can1_ComnErr_T2[i] != SubDev_ComnErr_T2[i]){
                    SubDev_ComnErr_T2[i] = can1_ComnErr_T2[i];
                }
            }
            break;
        case 0x205C:
            for(int i = 0; i < 2; i++){
                if(can1_ComnErr_MC2[i] != SubDev_ComnErr_MC2[i]){
                    SubDev_ComnErr_MC2[i] = can1_ComnErr_MC2[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2057:
            for(int i = 0; i < 2; i++){
                if(can2_ComnErr_MC1[i] != SubDev_ComnErr_MC1[i]){
                    SubDev_ComnErr_MC1[i] = can2_ComnErr_MC1[i];
                }
            }
            break;
        case 0x2058:
            for(int i = 0; i < 2; i++){
                if(can2_ComnErr_T[i] != SubDev_ComnErr_T[i]){
                    SubDev_ComnErr_T[i] = can2_ComnErr_T[i];
                }
            }
            break;
        case 0x2059:
            for(int i = 0; i < 2; i++){
                if(can2_ComnErr_M[i] != SubDev_ComnErr_M[i]){
                    SubDev_ComnErr_M[i] = can2_ComnErr_M[i];
                }
            }
            break;
        case 0x205A:
            for(int i = 0; i < 2; i++){
                if(can2_ComnErr_T1[i] != SubDev_ComnErr_T1[i]){
                    SubDev_ComnErr_T1[i] = can2_ComnErr_T1[i];
                }
            }
            break;
        case 0x205B:
            for(int i = 0; i < 2; i++){
                if(can2_ComnErr_T2[i] != SubDev_ComnErr_T2[i]){
                    SubDev_ComnErr_T2[i] = can2_ComnErr_T2[i];
                }
            }
            break;
        case 0x205C:
            for(int i = 0; i < 2; i++){
                if(can2_ComnErr_MC2[i] != SubDev_ComnErr_MC2[i]){
                    SubDev_ComnErr_MC2[i] = can2_ComnErr_MC2[i];
                }
            }
            break;
        default:
            break;
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_TMSState_CCU_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    if(bCCUJudgeFlag){
        return 0;
    }
    UNS16 index = unsused_indextable->index;
    switch (index) {
    case 0x205D:
        if(&Can1ObjDict_Data == d){
            for(int i = 0; i < 5; i++){
                if(can1_TMSState_CCU1[i] != TMSStateData_CCU1[i]){
                    TMSStateData_CCU1[i] = can1_TMSState_CCU1[i];
                }
            }
        }
        else{
            for(int i = 0; i < 5; i++){
                if(can2_TMSState_CCU1[i] != TMSStateData_CCU1[i]){
                    TMSStateData_CCU1[i] = can2_TMSState_CCU1[i];
                }
            }
        }
        break;
    case 0x2093:
        if(&Can1ObjDict_Data == d){
            for(int i = 0; i < 5; i++){
                if(can1_TMSState_CCU2[i] != TMSStateData_CCU2[i]){
                    TMSStateData_CCU2[i] = can1_TMSState_CCU2[i];
                }
            }
        }
        else{
            for(int i = 0; i < 5; i++){
                if(can2_TMSState_CCU2[i] != TMSStateData_CCU2[i]){
                    TMSStateData_CCU2[i] = can2_TMSState_CCU2[i];
                }
            }
        }
        break;
    default:
        break;
    }
    return 0;
}

UNS32 Oncan_SetParaData_HMI_Update(CO_Data *d, const indextable *, UNS8 ){
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        for(int i = 0; i < 4; i++){
            HMISetParaData_HMI[i] = can1_SetParaData_HMI[i];
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        for(int i = 0; i < 4; i++){
            HMISetParaData_HMI[i] = can2_SetParaData_HMI[i];
        }
    }
    else{
        // do nothing
    }
    if(0 != HMISetParaData_HMI[0]){
        canSignalsSlots->car_SetParaData_HMI_Update();
    }
    return 0;
}

UNS32 Oncan_GetFaultData_HMI_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(&Can1ObjDict_Data == d){
        switch (index) {
        case 0x205F:
            for(int i = 0; i < 2; i++){
                M_GetFaultData_HMI[i] = can1_GetFaultData_HMI1[i];
            }
            break;
        case 0x2096:
            for(int i = 0; i < 2; i++){
                M_GetFaultData_HMI[i] = can1_GetFaultData_HMI2[i];
            }
            break;
        default:
            break;
        }
    }
    else if(&Can2ObjDict_Data == d){
        switch (index) {
        case 0x205F:
            for(int i = 0; i < 2; i++){
                M_GetFaultData_HMI[i] = can2_GetFaultData_HMI1[i];
            }
            break;
        case 0x2096:
            for(int i = 0; i < 2; i++){
                M_GetFaultData_HMI[i] = can2_GetFaultData_HMI2[i];
            }
            break;
        default:
            break;
        }
    }
    else{
        // do nothing
    }
//    if(0 != M_GetFaultData_HMI[0] && 0 != M_GetFaultData_HMI[1]){
//        canSignalsSlots->car_AnotherHMIGetFaultData_Update();
//    }
    return 0;
}

UNS32 Oncan_ParaData_7B_HMI_Update(CO_Data *d, const indextable *, UNS8 ){
    bool bChangeFlag = false;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        for(int i = 0; i < 7; i++){
            if(can1_ParaData_HMI1_1[i] != ParaData_HMI[i]){
                ParaData_HMI[i] = can1_ParaData_HMI1_1[i];
                bChangeFlag = true;
            }
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        for(int i = 0; i < 7; i++){
            if(can2_ParaData_HMI1_1[i] != ParaData_HMI[i]){
                ParaData_HMI[i] = can2_ParaData_HMI1_1[i];
                bChangeFlag = true;
            }
        }
    } 
    else{
        bChangeFlag = false;
    }
    if(bChangeFlag && !masterHMIFlag){
//     qDebug()<<"Para 1  Data  ==  "<<ParaData_HMI[0]<<"   "<<840-ParaData_HMI[1]<<"   "<<840-ParaData_HMI[2]<<"   "<<840-ParaData_HMI[3]<<"   "<<840-ParaData_HMI[4]<<"   "<<840-ParaData_HMI[5]<<"   "<<840-ParaData_HMI[6];
        canSignalsSlots->car_ParaData_7B_HMI_Update();
    }
    return 0;
}

UNS32 Oncan_ParaData_8B_HMI_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    bool bChangeFlag = false;
    int paraDataIndex = 0;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2061:
            for(int i = 0; i < 8; i++){
                if(can1_ParaData_HMI1_2[i] != ParaData_HMI[i+7]){
                    ParaData_HMI[i+7] = can1_ParaData_HMI1_2[i];
                    bChangeFlag = true;
                    paraDataIndex = 1;
                }
            }
            break;
        case 0x206C:
            for(int i = 0; i < 8; i++){
                if(can1_ParaData_HMI1_3[i] != ParaData_HMI[i+7+8]){
                    ParaData_HMI[i+7+8] = can1_ParaData_HMI1_3[i];
                    bChangeFlag = true;
                    paraDataIndex = 2;
                }
            }
            break;
        case 0x206D:
            for(int i = 0; i < 8; i++){
                if(can1_ParaData_HMI1_4[i] != ParaData_HMI[i+7+8+8]){
                    ParaData_HMI[i+7+8+8] = can1_ParaData_HMI1_4[i];
                    bChangeFlag = true;
                    paraDataIndex = 3;
                }
            }
            break;
        default:
            bChangeFlag = false;
            paraDataIndex = 0;
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2061:
            for(int i = 0; i < 8; i++){
                if(can2_ParaData_HMI1_2[i] != ParaData_HMI[i+7]){
                    ParaData_HMI[i+7] = can2_ParaData_HMI1_2[i];
                    bChangeFlag = true;
                    paraDataIndex = 1;
                }
            }
            break;
        case 0x206C:
            for(int i = 0; i < 8; i++){
                if(can2_ParaData_HMI1_3[i] != ParaData_HMI[i+7+8]){
                    ParaData_HMI[i+7+8] = can2_ParaData_HMI1_3[i];
                    bChangeFlag = true;
                    paraDataIndex = 2;
                }
            }
            break;
        case 0x206D:
            for(int i = 0; i < 8; i++){
                if(can2_ParaData_HMI1_4[i] != ParaData_HMI[i+7+8+8]){
                    ParaData_HMI[i+7+8+8] = can2_ParaData_HMI1_4[i];
                    bChangeFlag = true;
                    paraDataIndex = 3;
                }
            }
            break;
        default:
            bChangeFlag = false;
            paraDataIndex = 0;
            break;
        }
    }
    else{
        bChangeFlag = false;
    }
    if(bChangeFlag && !masterHMIFlag){
        canSignalsSlots->car_ParaData_8B_HMI_Update(paraDataIndex);
    }
    return 0;
}

UNS32 Oncan_TempData_HMI_Update(CO_Data *d, const indextable *, UNS8 ){
    bool bChangeFlag = true;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        for(int i = 0; i < 2; i++){
            if(can1_TempData_HMI[i] != TempData_HMI[i]){
                TempData_HMI[i] = can1_TempData_HMI[i];
                bChangeFlag = true;
            }
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        for(int i = 0; i < 2; i++){
            if(can2_TempData_HMI[i] != TempData_HMI[i]){
                TempData_HMI[i] = can2_TempData_HMI[i];
                bChangeFlag = true;
            }
        }
    }
    else{
        bChangeFlag = false;
    }
    if(bChangeFlag && masterHMIFlag){
        canSignalsSlots->car_TempData_HMI_Update();
    }
    return 0;
}

UNS32 Oncan_GetFaultRunSpeed_HMI_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(&Can1ObjDict_Data == d){
        switch (index) {
        case 0x2069:
            M_GetFaultInfo_HMI = can1_GetFaultInfo_HMI1;
            break;
        case 0x2099:
            M_GetFaultInfo_HMI = can1_GetFaultInfo_HMI2;
            break;
        default:
            break;
        }
    }
    else if(&Can2ObjDict_Data == d){
        switch (index) {
        case 0x2069:
            M_GetFaultInfo_HMI = can2_GetFaultInfo_HMI1;
            break;
        case 0x2099:
            M_GetFaultInfo_HMI = can2_GetFaultInfo_HMI2;
            break;
        default:
            break;
        }
    }
    else{
        // do nothing
    }
    if(3 == M_GetFaultInfo_HMI){
        canSignalsSlots->car_AnotherHMIGetSpeedData_Update();
    }
    return 0;
}

UNS32 Oncan_InitSetData_HMI_Update(CO_Data *d, const indextable *, UNS8 ){
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){

    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
    }
    else {
    }
    return 0;
}

UNS32 Oncan_TMSDevStart_CCU_Update(CO_Data *d, const indextable *, UNS8 ){
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        if(can1_TMSDevData_CCU != TMSDevStart_CCU){
            TMSDevStart_CCU = can1_TMSDevData_CCU;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        if(can2_TMSDevData_CCU != TMSDevStart_CCU){
            TMSDevStart_CCU = can2_TMSDevData_CCU;
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_FaultDataCount_ERM_Update(CO_Data *d, const indextable *, UNS8 ){
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        for(int i = 0; i < 7; i++){
            FltDataCount_ERM[i] = can1_FaultDataCount_ERM[i];
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        for(int i = 0; i < 7; i++){
            FltDataCount_ERM[i] = can2_FaultDataCount_ERM[i];
        }
    }
    else{
        // do nothing
    }
    canSignalsSlots->car_FaultDataCount_Update();
    return 0;
}

UNS32 Oncan_ACPData_1_Update(CO_Data */*d*/, const indextable *, UNS8 ){
    return 0;
}

UNS32 Oncan_ACPData_2_Update(CO_Data */*d*/, const indextable *, UNS8 ){
    return 0;
}

UNS32 Oncan_TramCodeData_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    bool bChangeFlag = false;
    CAR_ID carId = CAR_NULL;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x207F:
            carId = CAR_1;
            for(int i = 0; i < 2; i++){
                if(can1_TramCode_MC1[i] != TramCode_MC1[i]){
                    TramCode_MC1[i] = can1_TramCode_MC1[i];
                    bChangeFlag = true;
                }
            }
            break;
        case 0x2080:
            carId = CAR_2;
            for(int i = 0; i < 2; i++){
                if(can1_TramCode_T[i] != TramCode_T[i]){
                    TramCode_T[i] = can1_TramCode_T[i];
                    bChangeFlag = true;
                }
            }
            break;
        case 0x2081:
            carId = CAR_3;
            for(int i = 0; i < 2; i++){
                if(can1_TramCode_M[i] != TramCode_M[i]){
                    TramCode_M[i] = can1_TramCode_M[i];
                    bChangeFlag = true;
                }
            }
            break;
        case 0x2082:
            carId = CAR_4;
            for(int i = 0; i < 2; i++){
                if(can1_TramCode_T1[i] != TramCode_T1[i]){
                    TramCode_T1[i] = can1_TramCode_T1[i];
                    bChangeFlag = true;
                }
            }
            break;
        case 0x2083:
            carId = CAR_5;
            for(int i = 0; i < 2; i++){
                if(can1_TramCode_T2[i] != TramCode_T2[i]){
                    TramCode_T2[i] = can1_TramCode_T2[i];
                    bChangeFlag = true;
                }
            }
            break;
        case 0x2084:
            carId = CAR_6;
            for(int i = 0; i < 2; i++){
                if(can1_TramCode_MC2[i] != TramCode_MC2[i]){
                    TramCode_MC2[i] = can1_TramCode_MC2[i];
                    bChangeFlag = true;
                }
            }
            break;
        default:
            carId = CAR_NULL;
            bChangeFlag = false;
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x207F:
            carId = CAR_1;
            for(int i = 0; i < 2; i++){
                if(can2_TramCode_MC1[i] != TramCode_MC1[i]){
                    TramCode_MC1[i] = can2_TramCode_MC1[i];
                    bChangeFlag = true;
                }
            }
            break;
        case 0x2080:
            carId = CAR_2;
            for(int i = 0; i < 2; i++){
                if(can2_TramCode_T[i] != TramCode_T[i]){
                    TramCode_T[i] = can2_TramCode_T[i];
                    bChangeFlag = true;
                }
            }
            break;
        case 0x2081:
            carId = CAR_3;
            for(int i = 0; i < 2; i++){
                if(can2_TramCode_M[i] != TramCode_M[i]){
                    TramCode_M[i] = can2_TramCode_M[i];
                    bChangeFlag = true;
                }
            }
            break;
        case 0x2082:
            carId = CAR_4;
            for(int i = 0; i < 2; i++){
                if(can2_TramCode_T1[i] != TramCode_T1[i]){
                    TramCode_T1[i] = can2_TramCode_T1[i];
                    bChangeFlag = true;
                }
            }
            break;
        case 0x2083:
            carId = CAR_5;
            for(int i = 0; i < 2; i++){
                if(can2_TramCode_T2[i] != TramCode_T2[i]){
                    TramCode_T2[i] = can2_TramCode_T2[i];
                    bChangeFlag = true;
                }
            }
            break;
        case 0x2084:
            carId = CAR_6;
            for(int i = 0; i < 2; i++){
                if(can2_TramCode_MC2[i] != TramCode_MC2[i]){
                    TramCode_MC2[i] = can2_TramCode_MC2[i];
                    bChangeFlag = true;
                }
            }
            break;
        default:
            carId = CAR_NULL;
            bChangeFlag = false;
            break;
        }
    }
    else{
        carId = CAR_NULL;
        bChangeFlag = false;
    }
    if(bChangeFlag){
        canSignalsSlots->car_TramCode_Update(carId);
    }
    return 0;
}

UNS32 Oncan_DelResult_ERM_Update(CO_Data *d, const indextable *, UNS8 ){
    if(&Can1ObjDict_Data == d){
        for(int i = 0; i < 3; i++){
            DelResult_ERM[i] = can1_DelResult_ERM[i];
        }
    }
    else if(&Can2ObjDict_Data == d){
        for(int i = 0; i < 3; i++){
            DelResult_ERM[i] = can2_DelResult_ERM[i];
        }
    }
    else{
        // do nothing
    }

    if(1 == DelResult_ERM[2]){
        canSignalsSlots->car_ERM_DelResult_Update();
    }
    return 0;
}

UNS32 Oncan_SubDevFltData_MDU_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    int iDataIndex = 0;
    bool bChangeFlag = false;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x2086:
            for(int i = 0; i < 7; i++){
                if(can1_SubDevFltData_MDU_1[i] != SubDevFltData_MDU[i]){
                    SubDevFltData_MDU[i] = can1_SubDevFltData_MDU_1[i];
                    bChangeFlag = true;
                    iDataIndex = 1;
                }
            }
            break;
        case 0x2087:
            for(int i = 0; i < 7; i++){
                if(can1_SubDevFltData_MDU_2[i] != SubDevFltData_MDU[i+7]){
                    SubDevFltData_MDU[i+7] = can1_SubDevFltData_MDU_2[i];
                    bChangeFlag = true;
                    iDataIndex = 2;
                }
            }
            break;
        default:
            bChangeFlag = false;
            iDataIndex = 0;
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x2086:
            for(int i = 0; i < 7; i++){
                if(can2_SubDevFltData_MDU_1[i] != SubDevFltData_MDU[i]){
                    SubDevFltData_MDU[i] = can2_SubDevFltData_MDU_1[i];
                    bChangeFlag = true;
                    iDataIndex = 1;
                }
            }
            break;
        case 0x2087:
            for(int i = 0; i < 7; i++){
                if(can2_SubDevFltData_MDU_2[i] != SubDevFltData_MDU[i+7]){
                    SubDevFltData_MDU[i+7] = can2_SubDevFltData_MDU_2[i];
                    bChangeFlag = true;
                    iDataIndex = 2;
                }
            }
            break;
        default:
            bChangeFlag = false;
            iDataIndex = 0;
            break;
        }
    }
    else{
        bChangeFlag = false;
        iDataIndex = 0;
    }
    if(bChangeFlag){
        canSignalsSlots->car_SubDev_FaultData_Update(iDataIndex);
    }
    return 0;
}

UNS32 Oncan_SetDateTime_HMI_Update(CO_Data *d, const indextable *, UNS8 ){
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        for(int i = 0; i < 7; i++){
            SetDate_Time_HMI[i] = can1_SetDateTime_HMI[i];
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        for(int i = 0; i < 7; i++){
            SetDate_Time_HMI[i] = can2_SetDateTime_HMI[i];
        }
    }
    else{
        // do nothing
    }
    if(masterHMIFlag && 1 == SetDate_Time_HMI[0]){
        canSignalsSlots->car_HMI_2_SetDateTime();
    }
    return 0;
}

UNS32 Oncan_BCUData_4_Update(CO_Data *d, const indextable *unsused_indextable, UNS8 ){
    UNS16 index = unsused_indextable->index;
    if(CAN_1 == canWorkFlag && &Can1ObjDict_Data == d){
        switch (index) {
        case 0x208C:
            if(can1_BCUData_MC1_4 != BCUData_MC1[24]){
                BCUData_MC1[24] = can1_BCUData_MC1_4;
            }
            break;
        case 0x208D:
            if(can1_BCUData_T_4 != BCUData_T[24]){
                BCUData_T[24] = can1_BCUData_T_4;
            }
            break;
        case 0x208E:
            if(can1_BCUData_M_4 != BCUData_M[24]){
                BCUData_M[24] = can1_BCUData_M_4;
            }
            break;
        case 0x208F:
            if(can1_BCUData_T1_4 != BCUData_T1[24]){
                BCUData_T1[24] = can1_BCUData_T1_4;
            }
            break;
        case 0x2090:
            if(can1_BCUData_T2_4 != BCUData_T2[24]){
                BCUData_T2[24] = can1_BCUData_T2_4;
            }
            break;
        case 0x2091:
            if(can1_BCUData_MC2_4 != BCUData_MC2[24]){
                BCUData_MC2[24] = can1_BCUData_MC2_4;
            }
            break;
        default:
            break;
        }
    }
    else if(CAN_2 == canWorkFlag && &Can2ObjDict_Data == d){
        switch (index) {
        case 0x208C:
            if(can2_BCUData_MC1_4 != BCUData_MC1[24]){
                BCUData_MC1[24] = can2_BCUData_MC1_4;
            }
            break;
        case 0x208D:
            if(can2_BCUData_T_4 != BCUData_T[24]){
                BCUData_T[24] = can2_BCUData_T_4;
            }
            break;
        case 0x208E:
            if(can2_BCUData_M_4 != BCUData_M[24]){
                BCUData_M[24] = can2_BCUData_M_4;
            }
            break;
        case 0x208F:
            if(can2_BCUData_T1_4 != BCUData_T1[24]){
                BCUData_T1[24] = can2_BCUData_T1_4;
            }
            break;
        case 0x2090:
            if(can2_BCUData_T2_4 != BCUData_T2[24]){
                BCUData_T2[24] = can2_BCUData_T2_4;
            }
            break;
        case 0x2091:
            if(can2_BCUData_MC2_4 != BCUData_MC2[24]){
                BCUData_MC2[24] = can2_BCUData_MC2_4;
            }
            break;
        default:
            break;
        }
    }
    else{
    }
    return 0;
}

UNS32 Oncan_PTU_CMD_Update(CO_Data *d, const indextable */*unsused_indextable*/, UNS8 ){
    bool bChangeFlag = false;
    if(/*CAN_1 == canWorkFlag && */&Can1ObjDict_Data == d){
        if(can1_PTU_CMD_ERM != PTUCMD_ERM){
            PTUCMD_ERM = can1_PTU_CMD_ERM;
            bChangeFlag = true;
        }
    }
    else if(/*CAN_2 == canWorkFlag && */&Can2ObjDict_Data == d){
        if(can2_PTU_CMD_ERM != PTUCMD_ERM){
            PTUCMD_ERM = can2_PTU_CMD_ERM;
            bChangeFlag = true;
        }
    }
    else{
        bChangeFlag = false;
    }
    if(/*masterHMIFlag && */bChangeFlag && 0 != PTUCMD_ERM){
        canSignalsSlots->car_PTU_CMD_Update();
    }
    return 0;
}

UNS32 Oncan_DIData_ERM_Update(CO_Data *d, const indextable *, u_int8_t ){
    if(&Can1ObjDict_Data == d){
        for(int i = 0; i < 7; i++){
            DI_Data_ERM1[i] = can1_DIData_ERM[i];
        }
    }
    else{
        for(int i = 0; i < 7; i++){
            DI_Data_ERM2[i] = can2_DIData_ERM[i];
        }
    }
    return 0;
}

UNS32 Oncan_SpeedCheck_HMI_Update(CO_Data *d, const indextable */*unsused_indextable*/, UNS8 ){
    if(&Can1ObjDict_Data == d && 1 == can1_SpeedCheckData_2){
        if(canSignalsSlots->mSpeedCheckSendData[1] < canSignalsSlots->mSpeedCheckList.length()){
            canSignalsSlots->mSpeedCheckSendData[1]++;
            canSignalsSlots->sendSpeedCheckData(1);
        }
    }
    else if(&Can2ObjDict_Data == d && 1 == can2_SpeedCheckData_2){
        if(canSignalsSlots->mSpeedCheckSendData[2] < canSignalsSlots->mSpeedCheckList.length()){
            canSignalsSlots->mSpeedCheckSendData[2]++;
            canSignalsSlots->sendSpeedCheckData(2);
        }
    }
    else{
        // do nothing
    }
    return 0;
}

UNS32 Oncan_FaultInfo_ERM_Update(CO_Data *d, const indextable */*unsused_indextable*/, UNS8 ){
    int mERMFlag = 0;
    if(&Can1ObjDict_Data == d){
        for(int i = 0; i < 20; i++){
            FaultInfo_ERM[i] = can1_FaultInfo_ERM_1[i];
        }
        mERMFlag = 1;
    }
    else if(&Can2ObjDict_Data == d){
        for(int i = 0; i < 20; i++){
            FaultInfo_ERM[i] = can2_FaultInfo_ERM_1[i];
        }
        mERMFlag = 2;
    }
    else{
        mERMFlag = 0;
    }
    if(0 != mERMFlag){
        canSignalsSlots->car_FaultInfo_ERM_Update(mERMFlag);
    }
    return 0;
}

UNS32 Oncan_RunInfo_ERM_Update(CO_Data *d, const indextable */*unsused_indextable*/, UNS8 ){
    bool bChangeFlag = false;
    if(&Can1ObjDict_Data == d){
        for(int i = 0; i < 64; i++){
            RunData_ERM[i] = can1_RunData_ERM_1[i];
        }
        bChangeFlag = true;
    }
    else if(&Can2ObjDict_Data == d){
        for(int i = 0; i < 64; i++){
            RunData_ERM[i] = can2_RunData_ERM_1[i];
        }
        bChangeFlag = true;
    }
    else{
        bChangeFlag = false;
    }
    if(bChangeFlag){
        canSignalsSlots->car_RunInfo_ERM_Update();
    }
    return 0;
}

UNS32 Oncan_AllSpeedInfo_ERM_Update(CO_Data *d, const indextable */*unsused_indextable*/, UNS8 ){
    int mERMFlag = 0;
    if(&Can1ObjDict_Data == d){
        for(int i = 0; i < 13; i++){
            AllSpeedInfo_ERM[i] = can1_SpeedData_ERM_1[i];
        }
        mERMFlag = 1;
    }
    else if(&Can2ObjDict_Data == d){
        for(int i = 0; i < 13; i++){
            AllSpeedInfo_ERM[i] = can2_SpeedData_ERM_1[i];
        }
        mERMFlag = 2;
    }
    else{
        mERMFlag = 0;
    }

    if(0 != mERMFlag){
        canSignalsSlots->car_AllSpeedInfo_ERM_Update(mERMFlag);
    }
    return 0;
}
