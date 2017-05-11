#include "threadfaultrecord.h"

#include "../objectdictdata.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

ThreadFaultRecord::ThreadFaultRecord(QObject *parent) : QThread(parent)
{
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 23; j++){
            bBCUAllFaultFlag[i][j] = false;
        }
        for(int j = 0; j < 28; j++){
            bACCAllFaultFlag[i][j] = false;
        }
        for(int j = 0; j < 16; j++){
            bVVVFAllFaultFlag[i][j] = false;
        }
    }

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 18; j++){
            bSIVAllFaultFlag[i][j] = false;
        }
    }

    for(int i = 0; i < 48; i++){
        for(int j = 0; j < 16; j++){
            bDoorAllFaultFlag[i][j] = false;
        }
    }
    mVVVFCodeList.clear();
    mBCUCodeList.clear();
    mACCCodeList.clear();
    mSIVCodeList.clear();
    mDoorCodeList.clear();

    mVVVFCodeList<<0x01<<0x02<<0x03<<0x04<<0x05<<0x06<<0x07<<0x08<<0x09<<0x0B<<0x0C<<0x0A<<0x0D<<0x0C<<0x0E<<0x0F;
    mBCUCodeList<<0x90<<0x91<<0x92<<0x93<<0x94<<0x95<<0x96<<0x97<<0x98<<0x9A<<0x9B<<0x9C<<0x9D<<0x9E<<0x9F<<0xA0<<0xA1<<0xA2<<0xA3<<0xA4<<0xA5<<0xA6<<0xA7;
    mACCCodeList<<0x01<<0x02<<0x03<<0x04<<0x05<<0x06<<0x07<<0x08<<0x09<<0x0A<<0x0B<<0x0C<<0x0D<<0x0E<<0x0F<<0x10<<0x11<<0x12<<0x13<<0x14<<0x15<<0x16<<0x17<<0x18<<0x19<<0x1A<<0x1B<<0x1C;
    mSIVCodeList<<0x01<<0x02<<0x03<<0x04<<0x05<<0x06<<0x07<<0x08<<0x11<<0x12<<0x13<<0x14<<0x15<<0x16<<0x18<<0x21<<0x22<<0xFF;
    mDoorCodeList<<0x01<<0x02<<0x03<<0x04<<0x05<<0x06<<0x07<<0x08<<0x09/*<<0x10<<0x11*/<<0x12<<0x13<<0x14<<0x15<<0x16/*<<0x17*/<<0x24<<0x25;

    timer_400MS = new QTimer();
}

void ThreadFaultRecord::run(){
    connect(timer_400MS, SIGNAL(timeout()), this, SLOT(slot_TimerOut_400MS()));
}

void ThreadFaultRecord::startThreadRun(){
    if(!timer_400MS->isActive()){
        timer_400MS->start(400);
    }
    start();
}

void ThreadFaultRecord::car_VVVFFaultOperation(CAR_ID carId, int vvvfIndex){
    int iFaultCode = 0,vvvfStateData[7] = {0};
    int tempCode = 0;
    bool bFirstFlag = false;
    bool bVVVFFaultFlag[16] = {false};
    switch (carId) {
    case CAR_1:
        if(1 == vvvfIndex){
            vvvfStateData[0] = VVVF1Data_MC1[0];
            vvvfStateData[1] = VVVF1Data_MC1[6];
            vvvfStateData[2] = VVVF1Data_MC1[7];
            vvvfStateData[3] = VVVF1Data_MC1[10];
            vvvfStateData[4] = VVVF1Data_MC1[11];
            vvvfStateData[5] = VVVF1Data_MC1[12];
            vvvfStateData[6] = VVVF1Data_MC1[13];
        }
        else{
            vvvfStateData[0] = VVVF2Data_MC1[0];
            vvvfStateData[1] = VVVF2Data_MC1[6];
            vvvfStateData[2] = VVVF2Data_MC1[7];
            vvvfStateData[3] = VVVF2Data_MC1[10];
            vvvfStateData[4] = VVVF2Data_MC1[11];
            vvvfStateData[5] = VVVF2Data_MC1[12];
            vvvfStateData[6] = VVVF2Data_MC1[13];
        }
        break;
    case CAR_3:
        if(1 == vvvfIndex){
            vvvfStateData[0] = VVVF1Data_M[0];
            vvvfStateData[1] = VVVF1Data_M[6];
            vvvfStateData[2] = VVVF1Data_M[7];
            vvvfStateData[3] = VVVF1Data_M[10];
            vvvfStateData[4] = VVVF1Data_M[11];
            vvvfStateData[5] = VVVF1Data_M[12];
            vvvfStateData[6] = VVVF1Data_M[13];
        }
        else{
            vvvfStateData[0] = VVVF2Data_M[0];
            vvvfStateData[1] = VVVF2Data_M[6];
            vvvfStateData[2] = VVVF2Data_M[7];
            vvvfStateData[3] = VVVF2Data_M[10];
            vvvfStateData[4] = VVVF2Data_M[11];
            vvvfStateData[5] = VVVF2Data_M[12];
            vvvfStateData[6] = VVVF2Data_M[13];
        }
        break;
    case CAR_6:
        if(1 == vvvfIndex){
            vvvfStateData[0] = VVVF1Data_MC2[0];
            vvvfStateData[1] = VVVF1Data_MC2[6];
            vvvfStateData[2] = VVVF1Data_MC2[7];
            vvvfStateData[3] = VVVF1Data_MC2[10];
            vvvfStateData[4] = VVVF1Data_MC2[11];
            vvvfStateData[5] = VVVF1Data_MC2[12];
            vvvfStateData[6] = VVVF1Data_MC2[13];
        }
        else{
            vvvfStateData[0] = VVVF2Data_MC2[0];
            vvvfStateData[1] = VVVF2Data_MC2[6];
            vvvfStateData[2] = VVVF2Data_MC2[7];
            vvvfStateData[3] = VVVF2Data_MC2[10];
            vvvfStateData[4] = VVVF2Data_MC2[11];
            vvvfStateData[5] = VVVF2Data_MC2[12];
            vvvfStateData[6] = VVVF2Data_MC2[13];
        }
        break;
    default:
        break;
    }

    bVVVFFaultFlag[0] = canSignalsSlots->getCanDataBitFromByte(vvvfStateData[3], 2);  // 1  :  01h
    bVVVFFaultFlag[1] = !canSignalsSlots->getCanDataBitFromByte(vvvfStateData[3], 2) && (canSignalsSlots->getCanDataBitFromByte(vvvfStateData[3], 1) || canSignalsSlots->getCanDataBitFromByte(vvvfStateData[3], 3) || canSignalsSlots->getCanDataBitFromByte(vvvfStateData[3], 4) ||
                                      canSignalsSlots->getCanDataBitFromByte(vvvfStateData[3], 5) || canSignalsSlots->getCanDataBitFromByte(vvvfStateData[3], 6) || canSignalsSlots->getCanDataBitFromByte(vvvfStateData[3], 7) || canSignalsSlots->getCanDataBitFromByte(vvvfStateData[4], 0) ||
                                      canSignalsSlots->getCanDataBitFromByte(vvvfStateData[4], 1) || canSignalsSlots->getCanDataBitFromByte(vvvfStateData[4], 2) || canSignalsSlots->getCanDataBitFromByte(vvvfStateData[4], 3) || canSignalsSlots->getCanDataBitFromByte(vvvfStateData[4], 5) ||
                                      canSignalsSlots->getCanDataBitFromByte(vvvfStateData[4], 7));   // 2  :  02h
    bVVVFFaultFlag[2] = canSignalsSlots->getCanDataBitFromByte(vvvfStateData[2], 2) || canSignalsSlots->getCanDataBitFromByte(vvvfStateData[2], 3) || canSignalsSlots->getCanDataBitFromByte(vvvfStateData[2], 4);   // 3  :  03h
    bVVVFFaultFlag[3] = canSignalsSlots->getCanDataBitFromByte(vvvfStateData[5], 1);   // 4  :  04h
    bVVVFFaultFlag[4] = canSignalsSlots->getCanDataBitFromByte(vvvfStateData[5], 3);   // 5  :  05h
    bVVVFFaultFlag[5] = canSignalsSlots->getCanDataBitFromByte(vvvfStateData[5], 2);   // 6  :  06h
    bVVVFFaultFlag[6] = canSignalsSlots->getCanDataBitFromByte(vvvfStateData[5], 4);   // 7  :  07h
    bVVVFFaultFlag[7] = canSignalsSlots->getCanDataBitFromByte(vvvfStateData[5], 5);   // 8  :  08h
    bVVVFFaultFlag[8] = canSignalsSlots->getCanDataBitFromByte(vvvfStateData[5], 6);   // 9  :  09h
    bVVVFFaultFlag[9] = canSignalsSlots->getCanDataBitFromByte(vvvfStateData[5], 7);   // 10  :  0Bh
    bVVVFFaultFlag[10] = canSignalsSlots->getCanDataBitFromByte(vvvfStateData[6], 5);    // 11  :  0Ch
    bVVVFFaultFlag[11] = canSignalsSlots->getCanDataBitFromByte(vvvfStateData[6], 7);    // 12  :  0Ah
    bVVVFFaultFlag[12] = canSignalsSlots->getCanDataBitFromByte(vvvfStateData[4], 4) || canSignalsSlots->getCanDataBitFromByte(vvvfStateData[6], 3);   // 13  :  0Dh
    bVVVFFaultFlag[13] = canSignalsSlots->getCanDataBitFromByte(vvvfStateData[1], 6) || canSignalsSlots->getCanDataBitFromByte(vvvfStateData[1], 7);   // 14  :  0Ch
    bVVVFFaultFlag[14] = canSignalsSlots->getCanDataBitFromByte(vvvfStateData[0], 3);   // 15  :  0Eh
    bVVVFFaultFlag[15] = canSignalsSlots->getCanDataBitFromByte(vvvfStateData[6], 2);   // 16  :  0Fh

    for(int i = 0; i < 16; i++){
        if(bVVVFFaultFlag[i]){
            iFaultCode = mVVVFCodeList.at(i);
            if(1 == vvvfIndex){
                if(!bFirstFlag){
                    canSignalsSlots->mVVVFFirstFaultCode[int(carId/3)] = iFaultCode;
                    bFirstFlag = true;
                }
                if(!bVVVFAllFaultFlag[int(carId/3)][i]){
                    bVVVFAllFaultFlag[int(carId/3)][i] = true;
                    canSignalsSlots->car_FaultRecordOperation_Update(carId, 4, iFaultCode, true);  // VVVF1 Error Happend
                }
            }
            else{
                if(!bFirstFlag){
                    canSignalsSlots->mVVVFFirstFaultCode[int(carId/3+3)] = iFaultCode;
                    bFirstFlag = true;
                }
                if(!bVVVFAllFaultFlag[int(carId/3+3)][i]){
                    bVVVFAllFaultFlag[int(carId/3+3)][i] = true;
                    canSignalsSlots->car_FaultRecordOperation_Update(carId, 5, iFaultCode, true);  // VVVF2 Error Happend
                }
            }
        }
        else{
            tempCode = mVVVFCodeList.at(i);
            if(1 == vvvfIndex){
                if(bVVVFAllFaultFlag[int(carId/3)][i]){
                    bVVVFAllFaultFlag[int(carId/3)][i] = false;
                    canSignalsSlots->car_FaultRecordOperation_Update(carId, 4, tempCode, false);  // VVVF1 Error Miss
                }
            }
            else{
                if(bVVVFAllFaultFlag[int(carId/3+3)][i]){
                    bVVVFAllFaultFlag[int(carId/3+3)][i] = false;
                    canSignalsSlots->car_FaultRecordOperation_Update(carId, 5, tempCode, false);  // VVVF2 Error Miss
                }
            }
        }
    }

    if(!bFirstFlag){
        if(1 == vvvfIndex){
            canSignalsSlots->mVVVFFirstFaultCode[int(carId/3)] = 0;
        }
        else{
            canSignalsSlots->mVVVFFirstFaultCode[int(carId/3+3)] = 0;
        }
    }
}

void ThreadFaultRecord::car_BCUFaultOperation(CAR_ID carId){
    int iFaultCode = 0, bcuStateData[5] = {0};
    int tempCode = 0;
    bool bFirstFlag = false;
    bool bBCUFaultFlag[23] = {false};
    switch (carId) {
    case CAR_1:
        for(int i = 0; i < 5; i++){
            bcuStateData[i] = BCUData_MC1[i];
        }
        break;
    case CAR_2:
        for(int i = 0; i < 5; i++){
            bcuStateData[i] = BCUData_T[i];
        }
        break;
    case CAR_3:
        for(int i = 0; i < 5; i++){
            bcuStateData[i] = BCUData_M[i];
        }
        break;
    case CAR_4:
        for(int i = 0; i < 5; i++){
            bcuStateData[i] = BCUData_T1[i];
        }
        break;
    case CAR_5:
        for(int i = 0; i < 5; i++){
            bcuStateData[i] = BCUData_T2[i];
        }
        break;
    case CAR_6:
        for(int i = 0; i < 5; i++){
            bcuStateData[i] = BCUData_MC2[i];
        }
        break;
    default:
        break;
    }

    bBCUFaultFlag[0] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[0], 7);   // 1  : 90h
    bBCUFaultFlag[1] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[0], 6);   // 2  : 91h
    bBCUFaultFlag[2] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[0], 3);   // 3  : 92h
    bBCUFaultFlag[3] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[0], 4);   // 4  : 93h
    bBCUFaultFlag[4] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[0], 5);   // 5  : 94h
    bBCUFaultFlag[5] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[1], 0);   // 6  : 95h
    bBCUFaultFlag[6] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[0], 1);   // 7  : 96h
    bBCUFaultFlag[7] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[0], 0);   // 8  : 97h
    bBCUFaultFlag[8] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[1], 2);   // 9  : 98h
    bBCUFaultFlag[9] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[1], 4);   // 10  : 9Ah
    bBCUFaultFlag[10] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[1], 5);   // 11  : 9Bh
    bBCUFaultFlag[11] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[1], 6);   // 12  : 9Ch
    bBCUFaultFlag[12] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[1], 7);   // 13  : 9Dh
    bBCUFaultFlag[13] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[1], 1);   // 14  : 9Eh
    bBCUFaultFlag[14] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[4], 1);   // 15  : 9Fh
    bBCUFaultFlag[15] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[3], 0);   // 16  : A0h
    bBCUFaultFlag[16] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[3], 2);   // 17  : A1h
    bBCUFaultFlag[17] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[3], 1);   // 18  : A2h
    bBCUFaultFlag[18] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[3], 4);   // 19  : A3h
    bBCUFaultFlag[19] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[3], 3);   // 20  : A4h
    bBCUFaultFlag[20] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[2], 5);   // 21  : A5h
    bBCUFaultFlag[21] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[2], 6);   // 22  : A6h
    bBCUFaultFlag[22] = canSignalsSlots->getCanDataBitFromByte(bcuStateData[1], 3);   // 23  : A7h

    for(int i = 0; i < 23; i++){
        if(bBCUFaultFlag[i]){
            iFaultCode = mBCUCodeList.at(i);
            if(!bFirstFlag){
                canSignalsSlots->mBCUFirstFaultCode[int(carId-1)] = iFaultCode;
                bFirstFlag = true;
            }
            if(!bBCUAllFaultFlag[int(carId-1)][i]){
                bBCUAllFaultFlag[int(carId-1)][i] = true;
                canSignalsSlots->car_FaultRecordOperation_Update(carId, 3, iFaultCode, true);  // BCU Error Happend
            }
        }
        else{
            tempCode = mBCUCodeList.at(i);
            if(bBCUAllFaultFlag[int(carId-1)][i]){
                bBCUAllFaultFlag[int(carId-1)][i] = false;
                canSignalsSlots->car_FaultRecordOperation_Update(carId, 3, tempCode, false);  // VVVF1 Error Miss
            }
        }
    }
    if(!bFirstFlag){
        canSignalsSlots->mBCUFirstFaultCode[int(carId-1)] = 0;
    }
}

void ThreadFaultRecord::car_ACCFaultOperation(CAR_ID carId){
    int iFaultCode = 0, accStateData[5] = {0};
    int tempCode = 0;
    bool bFirstFlag = false;
    bool bACCFaultFlag[28] = {false};
    switch (carId) {
    case CAR_1:
        accStateData[0] = ACCData_MC1[2];
        accStateData[1] = ACCData_MC1[3];
        accStateData[2] = ACCData_MC1[4];
        accStateData[3] = ACCData_MC1[5];
        accStateData[4] = ACCData_MC1[7];
        break;
    case CAR_2:
        accStateData[0] = ACCData_T[2];
        accStateData[1] = ACCData_T[3];
        accStateData[2] = ACCData_T[4];
        accStateData[3] = ACCData_T[5];
        accStateData[4] = ACCData_T[7];
        break;
    case CAR_3:
        accStateData[0] = ACCData_M[2];
        accStateData[1] = ACCData_M[3];
        accStateData[2] = ACCData_M[4];
        accStateData[3] = ACCData_M[5];
        accStateData[4] = ACCData_M[7];
        break;
    case CAR_4:
        accStateData[0] = ACCData_T1[2];
        accStateData[1] = ACCData_T1[3];
        accStateData[2] = ACCData_T1[4];
        accStateData[3] = ACCData_T1[5];
        accStateData[4] = ACCData_T1[7];
        break;
    case CAR_5:
        accStateData[0] = ACCData_T2[2];
        accStateData[1] = ACCData_T2[3];
        accStateData[2] = ACCData_T2[4];
        accStateData[3] = ACCData_T2[5];
        accStateData[4] = ACCData_T2[7];
        break;
    case CAR_6:
        accStateData[0] = ACCData_MC2[2];
        accStateData[1] = ACCData_MC2[3];
        accStateData[2] = ACCData_MC2[4];
        accStateData[3] = ACCData_MC2[5];
        accStateData[4] = ACCData_MC2[7];
        break;
    default:
        break;
    }

    bACCFaultFlag[0] = canSignalsSlots->getCanDataBitFromByte(accStateData[4], 4);  // 1: 01h PWF 空调电源故障
    bACCFaultFlag[1] = canSignalsSlots->getCanDataBitFromByte(accStateData[4], 5);  // 2: 02h EVF 紧急通风逆变器故障
    bACCFaultFlag[2] = canSignalsSlots->getCanDataBitFromByte(accStateData[0], 4);  // 3: 03h VEF11 空调机组1通风机1故障
    bACCFaultFlag[3] = canSignalsSlots->getCanDataBitFromByte(accStateData[2], 4);  // 4: 04h VEF21 空调机组2通风机1故障
    bACCFaultFlag[4] = canSignalsSlots->getCanDataBitFromByte(accStateData[0], 5);  // 5: 05h VEF12 空调机组1通风机2故障
    bACCFaultFlag[5] = canSignalsSlots->getCanDataBitFromByte(accStateData[2], 5);  // 6: 06h VEF22 空调机组2通风机2故障
    bACCFaultFlag[6] = canSignalsSlots->getCanDataBitFromByte(accStateData[0], 6);  // 7: 07h VEF13 空调机组1通风机3故障
    bACCFaultFlag[7] = canSignalsSlots->getCanDataBitFromByte(accStateData[2], 6);  // 8: 08h VEF23 空调机组2通风机3故障
    bACCFaultFlag[8] = canSignalsSlots->getCanDataBitFromByte(accStateData[0], 7);  // 9: 09h VEF14 空调机组1通风机4故障
    bACCFaultFlag[9] = canSignalsSlots->getCanDataBitFromByte(accStateData[2], 7);  // 10: 0Ah VEF24 空调机组4通风机4故障
    bACCFaultFlag[10] = canSignalsSlots->getCanDataBitFromByte(accStateData[1], 1);  // 11: 0Bh CPF11 空调机组1压缩机1过流故障
    bACCFaultFlag[11] = canSignalsSlots->getCanDataBitFromByte(accStateData[3], 1);  // 12: 0Ch CPF21 空调机组2压缩机1过流故障
    bACCFaultFlag[12] = canSignalsSlots->getCanDataBitFromByte(accStateData[1], 7);  // 13: 0Dh CPF12 空调机组1压缩机2过流故障
    bACCFaultFlag[13] = canSignalsSlots->getCanDataBitFromByte(accStateData[3], 7);  // 14: 0Eh CPF22 空调机组2压缩机2过流故障
    bACCFaultFlag[14] = canSignalsSlots->getCanDataBitFromByte(accStateData[1], 0);  // 15: 0Fh CPP11 空调机组1压缩机1压力保护
    bACCFaultFlag[15] = canSignalsSlots->getCanDataBitFromByte(accStateData[3], 0);  // 16: 10h CPP21 空调机组2压缩机1压力保护
    bACCFaultFlag[16] = canSignalsSlots->getCanDataBitFromByte(accStateData[1], 6);  // 17: 11h CPP12 空调机组1压缩机2压力保护
    bACCFaultFlag[17] = canSignalsSlots->getCanDataBitFromByte(accStateData[3], 6);  // 18: 12h CPP22 空调机组2压缩机2压力保护
    bACCFaultFlag[18] = canSignalsSlots->getCanDataBitFromByte(accStateData[0], 0);  // 19: 13h COF11 空调机组1冷凝风机1故障
    bACCFaultFlag[19] = canSignalsSlots->getCanDataBitFromByte(accStateData[2], 0);  // 20: 14h COF21 空调机组2冷凝风机1故障
    bACCFaultFlag[20] = canSignalsSlots->getCanDataBitFromByte(accStateData[0], 1);  // 21: 15h COF12 空调机组1冷凝风机2故障
    bACCFaultFlag[21] = canSignalsSlots->getCanDataBitFromByte(accStateData[2], 1);  // 22: 16h COF22 空调机组2冷凝风机2故障
    bACCFaultFlag[22] = canSignalsSlots->getCanDataBitFromByte(accStateData[4], 3);  // 23: 17h FTS1 新风温度传感器1故障
    bACCFaultFlag[23] = canSignalsSlots->getCanDataBitFromByte(accStateData[4], 2);  // 24: 18h FTS2 新风温度传感器2故障
    bACCFaultFlag[24] = canSignalsSlots->getCanDataBitFromByte(accStateData[4], 1);  // 25: 19h RTS1 回风温度传感器1故障
    bACCFaultFlag[25] = canSignalsSlots->getCanDataBitFromByte(accStateData[4], 0);  // 26: 1Ah RTS2 回风温度传感器2故障
    bACCFaultFlag[26] = canSignalsSlots->getCanDataBitFromByte(accStateData[4], 7);  // 27: 1Bh FAN1 废排风机1故障
    bACCFaultFlag[27] = canSignalsSlots->getCanDataBitFromByte(accStateData[4], 6);  // 28: 1Ch FAN2 废排风机2故障

    for(int i = 0; i < 28; i++){
        if(bACCFaultFlag[i]){
            iFaultCode = mACCCodeList.at(i);
            if(!bFirstFlag){
                canSignalsSlots->mACCFirstFaultCode[int(carId-1)] = iFaultCode;
                bFirstFlag = true;
            }
            if(!bACCAllFaultFlag[int(carId-1)][i]){
                bACCAllFaultFlag[int(carId-1)][i] = true;
                canSignalsSlots->car_FaultRecordOperation_Update(carId, 2, iFaultCode, true);  // ACC Error Happend
            }
        }
        else{
            tempCode = mACCCodeList.at(i);
            if(bACCAllFaultFlag[int(carId-1)][i]){
                bACCAllFaultFlag[int(carId-1)][i] = false;
                canSignalsSlots->car_FaultRecordOperation_Update(carId, 2, tempCode, false);  // ACC Error Miss
            }
        }
    }
    if(!bFirstFlag){
        canSignalsSlots->mACCFirstFaultCode[int(carId-1)] = 0;
    }
}

void ThreadFaultRecord::car_SIVFaultOperation(CAR_ID carId){
    int iFaultCode = 0, sivStateData[3] = {0};
    int tempCode = 0;
    bool bFirstFlag = false;
    bool bSIVFaultFlag[18] = {false};
    switch (carId) {
    case CAR_2:
        sivStateData[0] = SIVData_T[3];
        sivStateData[1] = SIVData_T[5];
        sivStateData[2] = SIVData_T[6];
        break;
    case CAR_4:
        sivStateData[0] = SIVData_T1[3];
        sivStateData[1] = SIVData_T1[5];
        sivStateData[2] = SIVData_T1[6];
        break;
    default:
        break;
    }

    bSIVFaultFlag[0] = false;  // 1: 01h  预留
    bSIVFaultFlag[1] = false;  // 2: 02h  预留
    bSIVFaultFlag[2] = canSignalsSlots->getCanDataBitFromByte(sivStateData[2], 2);  // 3 : 03h
    bSIVFaultFlag[3] = canSignalsSlots->getCanDataBitFromByte(sivStateData[2], 3);  // 4 : 04h
    bSIVFaultFlag[4] = canSignalsSlots->getCanDataBitFromByte(sivStateData[2], 4);  // 5 : 05h
    bSIVFaultFlag[5] = canSignalsSlots->getCanDataBitFromByte(sivStateData[2], 5);  // 6 : 06h
    bSIVFaultFlag[6] = canSignalsSlots->getCanDataBitFromByte(sivStateData[2], 6);  // 7 : 07h
    bSIVFaultFlag[7] = canSignalsSlots->getCanDataBitFromByte(sivStateData[2], 7);  // 8 : 08h
    bSIVFaultFlag[8] = canSignalsSlots->getCanDataBitFromByte(sivStateData[1], 0);  // 9 : 11h
    bSIVFaultFlag[9] = canSignalsSlots->getCanDataBitFromByte(sivStateData[1], 1);  // 10 : 12h
    bSIVFaultFlag[10] = canSignalsSlots->getCanDataBitFromByte(sivStateData[1], 2);  // 11 : 13h
    bSIVFaultFlag[11] = canSignalsSlots->getCanDataBitFromByte(sivStateData[1], 3);  // 12 : 14h
    bSIVFaultFlag[12] = canSignalsSlots->getCanDataBitFromByte(sivStateData[1], 4);  // 13 : 15h
    bSIVFaultFlag[13] = canSignalsSlots->getCanDataBitFromByte(sivStateData[1], 5);  // 14 : 16h
    bSIVFaultFlag[14] = canSignalsSlots->getCanDataBitFromByte(sivStateData[1], 7);  // 15 : 18h
    bSIVFaultFlag[15] = canSignalsSlots->getCanDataBitFromByte(sivStateData[2], 0);  // 16 : 21h
    bSIVFaultFlag[16] = canSignalsSlots->getCanDataBitFromByte(sivStateData[2], 1);  // 17 : 22h
    bSIVFaultFlag[17] = canSignalsSlots->getCanDataBitFromByte(sivStateData[0], 7);  // 18 : FFh

    for(int i = 0; i < 18; i++){
        if(bSIVFaultFlag[i]){
            iFaultCode = mSIVCodeList.at(i);
            if(!bFirstFlag){
                canSignalsSlots->mSIVFirstFaultCode[int(carId/2-1)] = iFaultCode;
                bFirstFlag = true;
            }
            if(!bSIVAllFaultFlag[int(carId/2-1)][i]){
                bSIVAllFaultFlag[int(carId/2-1)][i] = true;
                canSignalsSlots->car_FaultRecordOperation_Update(carId, 6, iFaultCode, true);  // SIV Error Happend
            }
        }
        else{
            tempCode = mSIVCodeList.at(i);
            if(bSIVAllFaultFlag[int(carId/2-1)][i]){
                bSIVAllFaultFlag[int(carId/2-1)][i] = false;
                canSignalsSlots->car_FaultRecordOperation_Update(carId, 6, tempCode, false);  // SIV Error Miss
            }
        }
    }
    if(!bFirstFlag){
        canSignalsSlots->mSIVFirstFaultCode[int(carId/2-1)] = iFaultCode;
    }
}

void ThreadFaultRecord::car_DoorFaultOperation(CAR_ID carId){
    int iFaultCode = 0, doorStateData[32] = {0};
    bool bDoorFaultFlag[8][16];
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 16; j++){
            bDoorFaultFlag[i][j] = false;
        }
    }
    bool bFirstFlag[8] = {false};

    switch (carId) {
    case CAR_1:
        for(int i = 0; i < 32; i++){
            doorStateData[i] = DoorData_MC1[i];
        }
        break;
    case CAR_2:
        for(int i = 0; i < 32; i++){
            doorStateData[i] = DoorData_T[i];
        }
        break;
    case CAR_3:
        for(int i = 0; i < 32; i++){
            doorStateData[i] = DoorData_M[i];
        }
        break;
    case CAR_4:
        for(int i = 0; i < 32; i++){
            doorStateData[i] = DoorData_T1[i];
        }
        break;
    case CAR_5:
        for(int i = 0; i < 32; i++){
            doorStateData[i] = DoorData_T2[i];
        }
        break;
    case CAR_6:
        for(int i = 0; i < 32; i++){
            doorStateData[i] = DoorData_MC2[i];
        }
        break;
    default:
        break;
    }

    for(int i = 0; i < 8; i++){
        bDoorFaultFlag[i][0] = canSignalsSlots->getCanDataBitFromByte(doorStateData[i*4+1], 7);
        bDoorFaultFlag[i][1] = canSignalsSlots->getCanDataBitFromByte(doorStateData[i*4+1], 6);
        bDoorFaultFlag[i][2] = canSignalsSlots->getCanDataBitFromByte(doorStateData[i*4+1], 5);
        bDoorFaultFlag[i][3] = canSignalsSlots->getCanDataBitFromByte(doorStateData[i*4+1], 4);
        bDoorFaultFlag[i][4] = canSignalsSlots->getCanDataBitFromByte(doorStateData[i*4+1], 3);
        bDoorFaultFlag[i][5] = canSignalsSlots->getCanDataBitFromByte(doorStateData[i*4+1], 2);
        bDoorFaultFlag[i][6] = canSignalsSlots->getCanDataBitFromByte(doorStateData[i*4+1], 1);
        bDoorFaultFlag[i][7] = canSignalsSlots->getCanDataBitFromByte(doorStateData[i*4+1], 0);
        bDoorFaultFlag[i][8] = canSignalsSlots->getCanDataBitFromByte(doorStateData[i*4+2], 7);
        bDoorFaultFlag[i][9] = canSignalsSlots->getCanDataBitFromByte(doorStateData[i*4+2], 4);
        bDoorFaultFlag[i][10] = canSignalsSlots->getCanDataBitFromByte(doorStateData[i*4+2], 3);
        bDoorFaultFlag[i][11] = canSignalsSlots->getCanDataBitFromByte(doorStateData[i*4+2], 2);
        bDoorFaultFlag[i][12] = canSignalsSlots->getCanDataBitFromByte(doorStateData[i*4+2], 1);
        bDoorFaultFlag[i][13] = canSignalsSlots->getCanDataBitFromByte(doorStateData[i*4+2], 0);
        bDoorFaultFlag[i][14] = canSignalsSlots->getCanDataBitFromByte(doorStateData[i*4+3], 7);
        bDoorFaultFlag[i][15] = canSignalsSlots->getCanDataBitFromByte(doorStateData[i*4+3], 6);
    }

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 16; j++){
            if(bDoorFaultFlag[i][j]){
                iFaultCode = mDoorCodeList.at(j);
                if(!bFirstFlag[i]){
                    canSignalsSlots->mDoorFirstFaultCode[int(carId-1)*8+i] = iFaultCode;
                    bFirstFlag[i] = true;
                }
                if(!bDoorAllFaultFlag[int(carId-1)*8+i][j]){
                    bDoorAllFaultFlag[int(carId-1)*8+i][j] = true;
                    canSignalsSlots->car_FaultRecordOperation_Update(carId, 1, iFaultCode, true);  // Door Error Happend
                }
            }
            else{
                if(bDoorAllFaultFlag[int(carId-1)*8+i][j]){
                    bDoorAllFaultFlag[int(carId-1)*8+i][j] = false;
                    canSignalsSlots->car_FaultRecordOperation_Update(carId, 1, iFaultCode, false);  // Door Error Miss
                }
            }
        }
    }
    for(int i = 0; i < 8; i++){
        if(!bFirstFlag[i]){
            canSignalsSlots->mDoorFirstFaultCode[int(carId-1)*8+i] = 0;
        }
    }
}

void ThreadFaultRecord::slot_TimerOut_400MS(){
    car_VVVFFaultOperation(CAR_1, 1);
    car_VVVFFaultOperation(CAR_1, 2);
    car_VVVFFaultOperation(CAR_3, 1);
    car_VVVFFaultOperation(CAR_3, 2);
    car_VVVFFaultOperation(CAR_6, 1);
    car_VVVFFaultOperation(CAR_6, 2);

    for(int i = 0; i < 6; i++){
        car_BCUFaultOperation(CAR_ID(i+1));
        car_ACCFaultOperation(CAR_ID(i+1));
        car_DoorFaultOperation(CAR_ID(i+1));
    }
    car_SIVFaultOperation(CAR_2);
    car_SIVFaultOperation(CAR_4);
}
