#include "cansignalsslots.h"
#include <QProcess>
#include <QTextStream>
#include <unistd.h>

#include "objectdictdata.h"
#include "can1operation.h"
#include "can2operation.h"
#include "cansendandrecv.h"

CanSignalsSlots::CanSignalsSlots(QObject *parent) : QObject(parent)
{
    tempValueInit();

    getDDUConfigInfo();
    getCarStationInfo();

    timer_1S = new QTimer();
    connect(timer_1S, SIGNAL(timeout()), this, SLOT(slot_1S_Timer_Out()));
    timer_1S->start(TIME_UPDATE);

    timerCCUJudge = new QTimer();
    connect(timerCCUJudge, SIGNAL(timeout()), this, SLOT(slot_CCUJudge_Time_Out()));

    timer_400ms = new QTimer();
    connect(timer_400ms, SIGNAL(timeout()), this, SLOT(slot_ScreenData_Refresh_400MS()));

    mThreadSendPDO = new ThreadSendPDO();
    mThreadDistance = new ThreadDistance();
    mThreadFaultRecord = new ThreadFaultRecord();

    if(masterHMIFlag){
        QTimer::singleShot(SYS_TIME_UPDATE, this, SLOT(slot_120S_Timer_Out()));
    }

    connect(this, SIGNAL(sig_CanWorkState_Update()), this, SLOT(slot_CanWorkState_Update()));
}

void CanSignalsSlots::tempValueInit(){
    deviceCarId = 1;
    nowCtrlHMIFlag = false;
    masterHMIFlag = false;
    atoModelFlag = false;
    carTempValue = 0;
    for(int i = 0; i < 2; i++){
        carVoltageValue[i] = 0;
        carCurrentValue[i] = 0;
        carSpeedValue[i] = 0;
    }
    for(int i = 0; i < 6; i++){
        carCodeNum[i] = 0;
    }
    carHeaderFlag = CAR_NO_HEADER;
    lastCarHeaderFlag = CAR_NO_HEADER;
    can1OperationalFlag = false;
    can2OperationalFlag = false;
    bPTUDownloading = false;

    canWorkFlag = CAN_NONE;
    bCCUJudgeFlag = false;
//    mCCUWorkFlag = 0;

    for(int i= 0; i < 6; i++){
        m_CarWheel[i] = 0;
        m_CarASFull[i] = 0;
        m_CarASEmpty[i] = 0;
        m_BCULoadRate[i] = 128;
        m_BCUASPress[i] = 0;
        m_BCUBCPress[i] = 0;
    }

    m_CarPWMValue = 0;
    for(int i = 0; i < 8; i++){
        doorMC1State[i] = -1;
        doorTState[i] = -1;
        doorMState[i] = -1;
        doorT1State[i] = -1;
        doorT2State[i] = -1;
        doorMC2State[i] = -1;
    }
    stationCodeList.clear();
    stationNameList.clear();
    iCurrStation = 0;
    iNextStation = 0;
    iEndStation = 0;

    bATODateTimeUpdateFlag = false;
    bATOModeFirstFlag = false;

    bCarRunStopFlag = true;
    fCurStationDistance = 0;

    for(int i = 0; i < 6; i++){
        m_FaultRecordCount[i] = 0;
        m_BCUFaultCount[i] = 0;
        mVVVFFirstFaultCode[i] = 0;
        mBCUFirstFaultCode[i] = 0;
        mACCFirstFaultCode[i] = 0;
    }

    for(int i = 0; i < 3; i ++){
        m_VVVF1FaultCount[i] = 0;
        m_VVVF2FaultCount[i] = 0;
    }
    for(int i = 0; i < 2; i++){
        m_SIVFaultCount[i] = 0;
        mSIVFirstFaultCode[i] = 0;
    }
    for(int i = 0; i < 48; i++){
        mDoorFirstFaultCode[i] = 0;
    }

    for(int i = 0; i < 70; i++){
        mBCUCheckSendData[i] = -1;
    }

    for(int i = 0; i < 3; i++){
        mSpeedCheckSendData[i] = 0;
    }
    mSpeedCheckList.clear();

    car1FaultList.clear();
    car2FaultList.clear();
    car3FaultList.clear();
    car4FaultList.clear();
    car5FaultList.clear();
    car6FaultList.clear();
    car1VVVF1ErrList.clear();
    car1VVVF2ErrList.clear();
    car3VVVF1ErrList.clear();
    car3VVVF2ErrList.clear();
    car6VVVF1ErrList.clear();
    car6VVVF2ErrList.clear();
    car2SIVErrList.clear();
    car4SIVErrList.clear();
    car1BCUErrList.clear();
    car2BCUErrList.clear();
    car3BCUErrList.clear();
    car4BCUErrList.clear();
    car5BCUErrList.clear();
    car6BCUErrList.clear();
    carSpeedDataList.clear();

    for(int i = 0; i < 12; i++){
        bTMSDevComnErrFlag[i] = true;
    }

    bCCUJudgeFirstFlag = false;

    for(int i = 0; i < 6; i ++){
        for(int j = 0; j < 2; j++){
            mVVVF_Elec_BCU_Data[i][j] = -1;
            mBCU_BCKpa_Data[i][j] = -1;
            mBCU_LoadRate_Data[i][j] = -1;
        }
        for(int k = 0; k < 3; k++){
            mACC_Temp_Data[i][k] = -1;
        }
    }

    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 5; j++){
            mACC_DevInfo_Data[i][j] = -1;
        }
    }

    u8DoorOperationData[0] = 0;
    u8DoorOperationData[1] = 0;

    bNowShowERMFaultFlag = false;
    bNowShowSubDevErrFlag = false;
    bNowShowSpeedFlag = false;
}

void CanSignalsSlots::getDDUConfigInfo(){
    QSettings * iniReader = new QSettings("/appbin/DDUConfig.ini", QSettings::IniFormat);
    deviceCarId = iniReader->value("/DDU/CarId").toInt();
    carTempValue = iniReader->value("DDU/CarTemp").toInt();
    for(int i = 0; i < 6; i++){
        m_CarWheel[i] = iniReader->value(QString("DDU/Car%1Wheel").arg(i+1)).toInt();
        m_CarASFull[i] = iniReader->value(QString("DDU/Car%1ASFull").arg(i+1)).toInt();
        m_CarASEmpty[i] = iniReader->value(QString("DDU/Car%1ASEmpty").arg(i+1)).toInt();
    }
    if(1 == deviceCarId){
        masterHMIFlag = true;
    }
    ParaData_HMI[0] = carTempValue;
    for(int i = 0 ; i < 6; i++){
        ParaData_HMI[i+1] = 840-m_CarWheel[i];
        ParaData_HMI[i*4+1+6] = m_CarASFull[i]>>8 & 0xFF;
        ParaData_HMI[i*4+1+1+6] = m_CarASFull[i] & 0xFF;
        ParaData_HMI[i*4+2+1+6] = m_CarASEmpty[i]>>8 & 0xFF;
        ParaData_HMI[i*4+3+1+6] = m_CarASEmpty[i] & 0xFF;
    }
    delete iniReader;
    qDebug()<<"Currrent CAR No = "<<deviceCarId << "    ,  masterHMIFlag  = "<<masterHMIFlag;
}

void CanSignalsSlots::setDDUConfigInfo(int dataFlag, int carId, int iValue){
    if(0 == iValue){
        return ;
    }

    QSettings * iniWriter = new QSettings("/appbin/DDUConfig.ini", QSettings::IniFormat);
    switch (dataFlag) {
    case 1:   //CarId
        iniWriter->setValue(tr("/DDU/CarId"), iValue);
        break;
    case 2:  //Car Temp
        iniWriter->setValue(tr("/DDU/CarTemp"), iValue);
        break;
    case 3:  //Car Wheel
        iniWriter->setValue(tr("/DDU/Car%1Wheel").arg(carId), iValue);
        break;
    case 4:  //Car AS Full
        iniWriter->setValue(tr("/DDU/Car%1ASFull").arg(carId), iValue);
        break;
    case 5:  //Car AS Empty
        iniWriter->setValue(tr("/DDU/Car%1ASEmpty").arg(carId), iValue);
        break;
    default:
        break;
    }
    delete iniWriter;
}

void CanSignalsSlots::getCarStationInfo(){
    QFile file("/appbin/StationInfo.txt");
    if(file.open(QIODevice::ReadOnly)){
        QTextStream readStream(&file);
        while (!readStream.atEnd()) {
            QString strText = readStream.readLine();
            char * tempData;
            int iStationCode = strtol(strText.section(",", 0, 0).toUtf8().data(), &tempData, 16);
            stationCodeList.append(iStationCode);
            stationNameList.append(strText.section(",", 1, 1));
        }
    }
//    for(int i = 0; i < stationCodeList.length(); i++){
//        qDebug()<<" Station Info  ::    "<< stationCodeList.at(i) <<"     "<<stationNameList.at(i);
//    }
}

bool CanSignalsSlots::getCanDataBitFromByte(UNS8 byteValue, int indexValue){
    int tempValue = 0;
    switch (indexValue) {
    case 0:
        tempValue = 0x80;   break;
    case 1:
        tempValue = 0x40;   break;
    case 2:
        tempValue = 0x20;   break;
    case 3:
        tempValue = 0x10;   break;
    case 4:
        tempValue = 0x08;   break;
    case 5:
        tempValue = 0x04;   break;
    case 6:
        tempValue = 0x02;   break;
    case 7:
        tempValue = 0x01;   break;
    default:
        tempValue = 0;
        break;
    }
    return (byteValue & tempValue) > 0 ? true : false;
}

u_int8_t CanSignalsSlots::setCanDataBitFromByte(u_int8_t byteValue, int indexValue, int iValue){
    u_int8_t tempValue = 0;
    switch (iValue) {
    case 0:
        switch (indexValue) {
        case 0:
            tempValue = 0x7F;   break;
        case 1:
            tempValue = 0xBF;   break;
        case 2:
            tempValue = 0xDF;   break;
        case 3:
            tempValue = 0xEF;   break;
        case 4:
            tempValue = 0xF7;   break;
        case 5:
            tempValue = 0xFB;   break;
        case 6:
            tempValue = 0xFD;   break;
        case 7:
            tempValue = 0xFE;   break;
        default:
            tempValue = 0;
            break;
        }
        byteValue = byteValue & tempValue;
        break;
    case 1:
        switch (indexValue) {
        case 0:
            tempValue = 0x80;   break;
        case 1:
            tempValue = 0x40;   break;
        case 2:
            tempValue = 0x20;   break;
        case 3:
            tempValue = 0x10;   break;
        case 4:
            tempValue = 0x08;   break;
        case 5:
            tempValue = 0x04;   break;
        case 6:
            tempValue = 0x02;   break;
        case 7:
            tempValue = 0x01;   break;
        default:
            tempValue = 0;
            break;
        }
        byteValue = byteValue | tempValue;
        break;
    default:
        tempValue = 0;
        break;
    }
    return byteValue;
}

int CanSignalsSlots::getRealValueFromByte(u_int8_t data){
    int tempValue = 0;
    if(data < 128){
        tempValue = data;
    }
    else{
        tempValue = tempValue xor 0xFF;
        tempValue = 0-tempValue-1;
    }
    return tempValue;
}

void CanSignalsSlots::masterHMIUpdate(){
    if(!bCan1HMI1HeartFlag && !bCan2HMI1HeartFlag){
        if(!masterHMIFlag){
            masterHMIFlag = true;
//            qDebug()<<"HMI2 CanNot Receive HMI1 Heart ,and Set HMI2 to master  @@@@";
        }
    }
    else{
        if(masterHMIFlag){
            masterHMIFlag = false;
//            qDebug()<<"@@@@@  HMI2 Receive HMI1 Common Data ,and Set HMI2 to Not master can1HMI1HeartFalg = "<<bCan1HMI1HeartFlag<<"    can2HMI2HeartFlag = "<<bCan2HMI1HeartFlag;
        }
    }
}

void CanSignalsSlots::canWorkStateUpdate(){
    emit sig_CanWorkState_Update();
}

void CanSignalsSlots::car_DI_Data_MC1_Update(){
    bool bACPFlag = false, bUSFlag = false, bBIDFlag = true;
    if(1 == bTMSDevComnErrFlag[2]){
        bACPFlag = false;
        bUSFlag = false;
        bBIDFlag = true;
    }
    else{
        bACPFlag = getCanDataBitFromByte(DI_Data_MC1[1], 4);
        bUSFlag = getCanDataBitFromByte(DI_Data_MC1[1], 3);
        bBIDFlag = getCanDataBitFromByte(DI_Data_MC1[1], 2);
    }

    if(bACPFlag != bACP_State_Data[0]){
        bACP_State_Data[0] = bACPFlag;
        emit sig_AirPump_State_Change(CAR_1, bACPFlag);
    }
    if(bUSFlag != bUrgent_Short_Data[0]){
        bUrgent_Short_Data[0] = bUSFlag;
        emit sig_UrgentShort(CAR_1, bUSFlag);
    }
    if(bBIDFlag != bBrakeIsolateDoor_Data[0]){
        bBrakeIsolateDoor_Data[0] = bBIDFlag;
        emit sig_BrakeIsolateDoor_Change(CAR_1, bBIDFlag);
    }

    car_VVVF_Elec_BCU_Update(CAR_1, 1);
    car_VVVF_Elec_BCU_Update(CAR_1, 2);
    car_BCU_BCKpa_Update(CAR_1);
    car_Load_Rate_Update(CAR_1);
}

void CanSignalsSlots::car_DI_Data_T_Update(){
    bool bBHBFlag = false, bBLBFlag = false, bBIDFlag = true;
    if(1 == bTMSDevComnErrFlag[3]){
        bBHBFlag = false;
        bBLBFlag = false;
        bBIDFlag = true;
    }
    else{
        bBHBFlag = getCanDataBitFromByte(DI_Data_T[0], 0);
        bBLBFlag = getCanDataBitFromByte(DI_Data_T[0], 1);
        bBIDFlag = getCanDataBitFromByte(DI_Data_T[1], 2);
    }

    if(bBHBFlag != bBHB_BLB_Data[0][0] || bBLBFlag != bBHB_BLB_Data[0][1]){
        bBHB_BLB_Data[0][0] = bBHBFlag;
        bBHB_BLB_Data[0][1] = bBLBFlag;
        emit sig_BHB_BLB_State_Change(CAR_2, bBHBFlag, bBLBFlag);
    }
    if(bBIDFlag != bBrakeIsolateDoor_Data[1]){
        bBrakeIsolateDoor_Data[1] = bBIDFlag;
        emit sig_BrakeIsolateDoor_Change(CAR_2, bBIDFlag);
    }
    car_BCU_BCKpa_Update(CAR_2);
    car_Load_Rate_Update(CAR_2);
    car_SIV_Voltage_Freq_Update(CAR_2);
}

void CanSignalsSlots::car_DI_Data_M_Update(){
    bool bBHBFlag = false, bBLBFlag = false, bBIDFlag = true;
    if(1 == bTMSDevComnErrFlag[4]){
        bBHBFlag = false;
        bBLBFlag = false;
        bBIDFlag = true;
    }
    else{
        bBHBFlag = getCanDataBitFromByte(DI_Data_M[0], 0);
        bBLBFlag = getCanDataBitFromByte(DI_Data_M[0], 1);
        bBIDFlag = getCanDataBitFromByte(DI_Data_M[0], 4);
    }

    if(bBHBFlag != bBHB_BLB_Data[1][0] || bBLBFlag != bBHB_BLB_Data[1][1]){
        bBHB_BLB_Data[1][0] = bBHBFlag;
        bBHB_BLB_Data[1][1] = bBLBFlag;
        emit sig_BHB_BLB_State_Change(CAR_3, bBHBFlag, bBLBFlag);
    }
    if(bBIDFlag != bBrakeIsolateDoor_Data[2]){
        bBrakeIsolateDoor_Data[2] = bBIDFlag;
        emit sig_BrakeIsolateDoor_Change(CAR_3, bBIDFlag);
    }

    car_VVVF_Elec_BCU_Update(CAR_3, 1);
    car_VVVF_Elec_BCU_Update(CAR_3, 2);
    car_BCU_BCKpa_Update(CAR_3);
    car_Load_Rate_Update(CAR_3);
}

void CanSignalsSlots::car_DI_Data_T1_Update(){
    bool bBIDFlag = true;
    if(1 == bTMSDevComnErrFlag[5]){
        bBIDFlag = true;
    }
    else{
        bBIDFlag = getCanDataBitFromByte(DI_Data_T1[1], 2);
    }

    if(bBIDFlag != bBrakeIsolateDoor_Data[3]){
        bBrakeIsolateDoor_Data[3] = bBIDFlag;
        emit sig_BrakeIsolateDoor_Change(CAR_4, bBIDFlag);
    }

    car_BCU_BCKpa_Update(CAR_4);
    car_Load_Rate_Update(CAR_4);
    car_SIV_Voltage_Freq_Update(CAR_4);
}

void CanSignalsSlots::car_DI_Data_T2_Update(){
    bool bBIDFlag = true;
    if(1 == bTMSDevComnErrFlag[6]){
        bBIDFlag = true;
    }
    else{
        bBIDFlag = getCanDataBitFromByte(DI_Data_T2[1], 1);
    }

    if(bBIDFlag != bBrakeIsolateDoor_Data[4]){
        bBrakeIsolateDoor_Data[4] = bBIDFlag;
        emit sig_BrakeIsolateDoor_Change(CAR_5, bBIDFlag);
    }

    car_BCU_BCKpa_Update(CAR_5);
    car_Load_Rate_Update(CAR_5);
}

void CanSignalsSlots::car_DI_Data_MC2_Update(){
    bool bACPFlag = false, bUSFlag = false, bBIDFlag = true;
    if(1 == bTMSDevComnErrFlag[7]){
        bACPFlag = false;
        bUSFlag = false;
        bBIDFlag = true;
    }
    else{
        bACPFlag = getCanDataBitFromByte(DI_Data_MC2[1], 4);
        bUSFlag = getCanDataBitFromByte(DI_Data_MC2[1], 3);
        bBIDFlag = getCanDataBitFromByte(DI_Data_MC2[1], 2);
    }

    if(bACPFlag != bACP_State_Data[1]){
        bACP_State_Data[1] = bACPFlag;
        emit sig_AirPump_State_Change(CAR_6, bACPFlag);
    }
    if(bUSFlag != bUrgent_Short_Data[1]){
        bUrgent_Short_Data[1] = bUSFlag;
        emit sig_UrgentShort(CAR_6, bUSFlag);
    }
    if(bBIDFlag != bBrakeIsolateDoor_Data[5]){
        bBrakeIsolateDoor_Data[5] = bBIDFlag;
        emit sig_BrakeIsolateDoor_Change(CAR_6, bBIDFlag);
    }

    car_VVVF_Elec_BCU_Update(CAR_6, 1);
    car_VVVF_Elec_BCU_Update(CAR_6, 2);
    car_BCU_BCKpa_Update(CAR_6);
    car_Load_Rate_Update(CAR_6);
}

void CanSignalsSlots::car_SetParaData_HMI_Update(){
    if(1 == HMISetParaData_HMI[1] || 0 == HMISetParaData_HMI[1]){
        return ;
    }
    int tempValue = HMISetParaData_HMI[2]<<8 | HMISetParaData_HMI[3];
    int carId = HMISetParaData_HMI[0];

//    qDebug()<<"HMI Set Para Dara  ==  "<<HMISetParaData_HMI[0] << "   "<<HMISetParaData_HMI[1]<< "   "<<HMISetParaData_HMI[2]<< "   "<<HMISetParaData_HMI[3] << "  Value  ==  "<<tempValue;
    if(2 == HMISetParaData_HMI[1]){
        if(m_CarWheel[carId-1] != tempValue){
            m_CarWheel[carId-1] = tempValue;
            ParaData_HMI[carId] = 840 - tempValue;

            setDDUConfigInfo(3, carId, tempValue);

            canSendParaData1_Update();
            emit sig_Car_ParaData_Update(2);
        }
    }
    else{
        switch (carId) {
        case 1:   // Car 1
            if(3 == HMISetParaData_HMI[1]){
                if(m_CarASFull[0] != tempValue){
                    m_CarASFull[0] = tempValue;
                    ParaData_HMI[7] = tempValue>>8 & 0xFF;
                    ParaData_HMI[8] = tempValue & 0xFF;
                    setDDUConfigInfo(4, 1, tempValue);
                    canSendParaData2_Update();
                    emit sig_Car_ParaData_Update(3);
                }
            }
            else{
                if(m_CarASEmpty[0] != tempValue){
                    m_CarASEmpty[0] = tempValue;
                    ParaData_HMI[9] = tempValue>>8 & 0xFF;
                    ParaData_HMI[10] = tempValue & 0xFF;
                    setDDUConfigInfo(5, 1, tempValue);
                    canSendParaData2_Update();
                    emit sig_Car_ParaData_Update(4);
                }
            }
            break;
        case 2:   // Car 2
            if(3 == HMISetParaData_HMI[1]){
                if(m_CarASFull[1] != tempValue){
                    m_CarASFull[1] = tempValue;
                    ParaData_HMI[11] = tempValue>>8 & 0xFF;
                    ParaData_HMI[12] = tempValue & 0xFF;
                    setDDUConfigInfo(4, 2, tempValue);
                    canSendParaData2_Update();
                    emit sig_Car_ParaData_Update(3);
                }
            }
            else{
                if(m_CarASEmpty[1] != tempValue){
                    m_CarASEmpty[1] = tempValue;
                    ParaData_HMI[13] = tempValue>>8 & 0xFF;
                    ParaData_HMI[14] = tempValue & 0xFF;
                    setDDUConfigInfo(5, 2, tempValue);
                    canSendParaData2_Update();
                    emit sig_Car_ParaData_Update(4);
                }
            }
            break;
        case 3:   // Car 3
            if(3 == HMISetParaData_HMI[1]){
                if(m_CarASFull[2] != tempValue){
                    m_CarASFull[2] = tempValue;
                    ParaData_HMI[15] = tempValue>>8 & 0xFF;
                    ParaData_HMI[16] = tempValue & 0xFF;
                    setDDUConfigInfo(4, 3, tempValue);
                    canSendParaData3_Update();
                    emit sig_Car_ParaData_Update(3);
                }
            }
            else{
                if(m_CarASEmpty[2] != tempValue){
                    m_CarASEmpty[2] = tempValue;
                    ParaData_HMI[17] = tempValue>>8 & 0xFF;
                    ParaData_HMI[18] = tempValue & 0xFF;
                    setDDUConfigInfo(5, 3, tempValue);
                    canSendParaData3_Update();
                    emit sig_Car_ParaData_Update(4);
                }
            }
            break;
        case 4:   // Car 4
            if(3 == HMISetParaData_HMI[1]){
                if(m_CarASFull[3] != tempValue){
                    m_CarASFull[3] = tempValue;
                    ParaData_HMI[19] = tempValue>>8 & 0xFF;
                    ParaData_HMI[20] = tempValue & 0xFF;
                    setDDUConfigInfo(4, 4, tempValue);
                    canSendParaData3_Update();
                    emit sig_Car_ParaData_Update(3);
                }
            }
            else{
                if(m_CarASEmpty[3] != tempValue){
                    m_CarASEmpty[3] = tempValue;
                    ParaData_HMI[21] = tempValue>>8 & 0xFF;
                    ParaData_HMI[22] = tempValue & 0xFF;
                    setDDUConfigInfo(5, 4, tempValue);
                    canSendParaData3_Update();
                    emit sig_Car_ParaData_Update(4);
                }
            }
            break;
        case 5:   // Car 5
            if(3 == HMISetParaData_HMI[1]){
                if(m_CarASFull[4] != tempValue){
                    m_CarASFull[4] = tempValue;
                    ParaData_HMI[23] = tempValue>>8 & 0xFF;
                    ParaData_HMI[24] = tempValue & 0xFF;
                    setDDUConfigInfo(4, 5, tempValue);
                    canSendParaData4_Update();
                    emit sig_Car_ParaData_Update(3);
                }
            }
            else{
                if(m_CarASEmpty[4] != tempValue){
                    m_CarASEmpty[4] = tempValue;
                    ParaData_HMI[25] = tempValue>>8 & 0xFF;
                    ParaData_HMI[26] = tempValue & 0xFF;
                    setDDUConfigInfo(5, 5, tempValue);
                    canSendParaData4_Update();
                    emit sig_Car_ParaData_Update(4);
                }
            }
            break;
        case 6:   // Car 6
            if(3 == HMISetParaData_HMI[1]){
                if(m_CarASFull[5] != tempValue){
                    m_CarASFull[5] = tempValue;
                    ParaData_HMI[27] = tempValue>>8 & 0xFF;
                    ParaData_HMI[28] = tempValue & 0xFF;
                    setDDUConfigInfo(4, 6, tempValue);
                    canSendParaData4_Update();
                    emit sig_Car_ParaData_Update(3);
                }
            }
            else{
                if(m_CarASEmpty[5] != tempValue){
                    m_CarASEmpty[5] = tempValue;
                    ParaData_HMI[29] = tempValue>>8 & 0xFF;
                    ParaData_HMI[30] = tempValue & 0xFF;
                    setDDUConfigInfo(5, 6, tempValue);
                    canSendParaData4_Update();
                    emit sig_Car_ParaData_Update(4);
                }
            }
            break;
        default:
            break;
        }
    }
}

void CanSignalsSlots::car_SubDev_FaultData_Sort(int iDevFalg){
    if(0 == iDevFalg){
        return ;
    }
    QList<structSubDevErr*> tempDevErrList;
    tempDevErrList.clear();
    switch (iDevFalg) {
    case 11:   //Car1 VVVF1
        tempDevErrList = car1VVVF1ErrList;
        break;
    case 13:   //Car3 VVVF1
        tempDevErrList = car3VVVF1ErrList;
        break;
    case 16:   //Car6 VVVF1
        tempDevErrList = car6VVVF1ErrList;
        break;
    case 21:   //Car1 VVVF2
        tempDevErrList = car1VVVF2ErrList;
        break;
    case 23:   //Car3 VVVF2
        tempDevErrList = car3VVVF2ErrList;
        break;
    case 26:   //Car6 VVVF2
        tempDevErrList = car6VVVF2ErrList;
        break;
    case 32:   //Car2 SIV
        tempDevErrList = car2SIVErrList;
        break;
    case 34:   //Car4 SIV
        tempDevErrList = car4SIVErrList;
        break;
    case 41:   //Car1 BCU
        tempDevErrList = car1BCUErrList;
        break;
    case 42:   //Car2 BCU
        tempDevErrList = car2BCUErrList;
        break;
    case 43:   //Car3 BCU
        tempDevErrList = car3BCUErrList;
        break;
    case 44:   //Car4 BCU
        tempDevErrList = car4BCUErrList;
        break;
    case 45:   //Car5 BCU
        tempDevErrList = car5BCUErrList;
        break;
    case 46:   //Car6 BCU
        tempDevErrList = car6BCUErrList;
        break;
    default:
        break;
    }
    int iLength = tempDevErrList.length();
    if(iLength < 2){
        return ;
    }
    structSubDevErr * mErrCurrent = NULL;
    structSubDevErr * mErrNext = NULL;
    for(int  i = iLength; i > 0; i--){
        for(int j = 0; j < i-1; j++){
            mErrCurrent = tempDevErrList.at(j);
            mErrNext = tempDevErrList.at(j+1);
            if(mErrCurrent->strDate > mErrNext->strDate){
                tempDevErrList.swap(j, j+1);
            }
            else if(mErrCurrent->strDate == mErrNext->strDate && mErrCurrent->strTime > mErrNext->strTime){
                tempDevErrList.swap(j, j+1);
            }
            else{
                // do nothing
            }
        }
    }

    switch (iDevFalg) {
    case 11:   //Car1 VVVF1
        car1VVVF1ErrList = tempDevErrList;
        break;
    case 13:   //Car3 VVVF1
        car3VVVF1ErrList  = tempDevErrList;
        break;
    case 16:   //Car6 VVVF1
        car6VVVF1ErrList  = tempDevErrList;
        break;
    case 21:   //Car1 VVVF2
        car1VVVF2ErrList  = tempDevErrList;
        break;
    case 23:   //Car3 VVVF2
        car3VVVF2ErrList  = tempDevErrList;
        break;
    case 26:   //Car6 VVVF2
        car6VVVF2ErrList  = tempDevErrList;
        break;
    case 32:   //Car2 SIV
        car2SIVErrList  = tempDevErrList;
        break;
    case 34:   //Car4 SIV
        car4SIVErrList  = tempDevErrList;
        break;
    case 41:   //Car1 BCU
        car1BCUErrList  = tempDevErrList;
        break;
    case 42:   //Car2 BCU
        car2BCUErrList  = tempDevErrList;
        break;
    case 43:   //Car3 BCU
        car3BCUErrList  = tempDevErrList;
        break;
    case 44:   //Car4 BCU
        car4BCUErrList  = tempDevErrList;
        break;
    case 45:   //Car5 BCU
        car5BCUErrList  = tempDevErrList;
        break;
    case 46:   //Car6 BCU
        car6BCUErrList  = tempDevErrList;
        break;
    default:
        break;
    }
}

void CanSignalsSlots::car_SubDev_FaultData_Update(int iDataIndex){
    if(0 == iDataIndex || !bNowShowSubDevErrFlag){
        return ;
    }
    structSubDevErr * m_SubDevErr = new structSubDevErr;
    QString strDate = "", strTime = "";
    bool bExistFlag = false;
    if(1 == iDataIndex){
        int iYear = SubDevFltData_MDU[4]/16*10+SubDevFltData_MDU[4]%16;
        int iMonth = SubDevFltData_MDU[5]/16*10+SubDevFltData_MDU[5]%16;
        int iDay = SubDevFltData_MDU[6]/16*10+SubDevFltData_MDU[6]%16;
        strDate = (iYear < 10 ? QString("0%1").arg(iYear) : QString("%1").arg(iYear))+"-"+(iMonth < 10 ? QString("0%1").arg(iMonth) : QString("%1").arg(iMonth))+"-"+(iDay < 10 ? QString("0%1").arg(iDay) : QString("%1").arg(iDay));
        switch (SubDevFltData_MDU[0]) {
        case 1:  // VVVF1 Error
            switch (SubDevFltData_MDU[1]) {
            case 1: // Car1
                    for(int i = 0; i < car1VVVF1ErrList.length(); i++){
                        if(SubDevFltData_MDU[2] == car1VVVF1ErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car1VVVF1ErrList.at(i)->strErrCode = getVVVFErrorInfo(int(SubDevFltData_MDU[3]));
                            car1VVVF1ErrList.at(i)->strDate = strDate;
                            car_SubDev_FaultData_Sort(11);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[2];
                        m_SubDevErr->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[3]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car1VVVF1ErrList.length() < m_VVVF1FaultCount[0]){
                            car1VVVF1ErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 3:  // Car3
                    for(int i = 0; i < car3VVVF1ErrList.length(); i++){
                        if(SubDevFltData_MDU[2] == car3VVVF1ErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car3VVVF1ErrList.at(i)->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[3]);
                            car3VVVF1ErrList.at(i)->strDate = strDate;
                            car_SubDev_FaultData_Sort(13);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[2];
                        m_SubDevErr->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[3]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car3VVVF1ErrList.length() < m_VVVF1FaultCount[1]){
                            car3VVVF1ErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 6:  // Car6
                    for(int i = 0; i < car6VVVF1ErrList.length(); i++){
                        if(SubDevFltData_MDU[2] == car6VVVF1ErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car6VVVF1ErrList.at(i)->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[3]);
                            car6VVVF1ErrList.at(i)->strDate = strDate;
                            car_SubDev_FaultData_Sort(16);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[2];
                        m_SubDevErr->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[3]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car6VVVF1ErrList.length() < m_VVVF1FaultCount[2]){
                            car6VVVF1ErrList.append(m_SubDevErr);
                        }
                    }
                break;
            default:
                break;
            }
            break;
        case 2:  // VVVF2 Error
            switch (SubDevFltData_MDU[1]) {
            case 1: // Car1
                    for(int i = 0; i < car1VVVF2ErrList.length(); i++){
                        if(SubDevFltData_MDU[2] == car1VVVF2ErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car1VVVF2ErrList.at(i)->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[3]);
                            car1VVVF2ErrList.at(i)->strDate = strDate;
                            car_SubDev_FaultData_Sort(21);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[2];
                        m_SubDevErr->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[3]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car1VVVF2ErrList.length() < m_VVVF2FaultCount[0]){
                            car1VVVF2ErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 3:  // Car3
                    for(int i = 0; i < car3VVVF2ErrList.length(); i++){
                        if(SubDevFltData_MDU[2] == car3VVVF2ErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car3VVVF2ErrList.at(i)->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[3]);
                            car3VVVF2ErrList.at(i)->strDate = strDate;
                            car_SubDev_FaultData_Sort(23);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[2];
                        m_SubDevErr->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[3]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car3VVVF2ErrList.length() < m_VVVF2FaultCount[1]){
                            car3VVVF2ErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 6:  // Car6
                    for(int i = 0; i < car6VVVF2ErrList.length(); i++){
                        if(SubDevFltData_MDU[2] == car6VVVF2ErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car6VVVF2ErrList.at(i)->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[3]);
                            car6VVVF2ErrList.at(i)->strDate = strDate;
                            car_SubDev_FaultData_Sort(26);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[2];
                        m_SubDevErr->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[3]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car6VVVF2ErrList.length() < m_VVVF2FaultCount[2]){
                            car6VVVF2ErrList.append(m_SubDevErr);
                        }
                    }
                break;
            default:
                break;
            }
            break;
        case 3:  // SIV Error
            switch (SubDevFltData_MDU[1]) {
            case 2: // Car2
                    for(int i = 0; i < car2SIVErrList.length(); i++){
                        if(SubDevFltData_MDU[2] == car2SIVErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car2SIVErrList.at(i)->strErrCode = getSIVErrorInfo(SubDevFltData_MDU[3]);
                            car2SIVErrList.at(i)->strDate = strDate;
                            car_SubDev_FaultData_Sort(32);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[2];
                        m_SubDevErr->strErrCode = getSIVErrorInfo(SubDevFltData_MDU[3]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car2SIVErrList.length() < m_SIVFaultCount[0]){
                            car2SIVErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 4:  // Car4
                    for(int i = 0; i < car4SIVErrList.length(); i++){
                        if(SubDevFltData_MDU[2] == car4SIVErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car4SIVErrList.at(i)->strErrCode = getSIVErrorInfo(SubDevFltData_MDU[3]);
                            car4SIVErrList.at(i)->strDate = strDate;
                            car_SubDev_FaultData_Sort(34);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[2];
                        m_SubDevErr->strErrCode = getSIVErrorInfo(SubDevFltData_MDU[3]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car4SIVErrList.length() < m_SIVFaultCount[1]){
                            car4SIVErrList.append(m_SubDevErr);
                        }
                    }
                break;
            default:
                break;
            }
            break;
        case 4:  // BCU Error
            switch (SubDevFltData_MDU[1]) {
            case 1: // Car1
                    for(int i = 0; i < car1BCUErrList.length(); i++){
                        if(SubDevFltData_MDU[2] == car1BCUErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car1BCUErrList.at(i)->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[3]);
                            car1BCUErrList.at(i)->strDate = strDate;
                            car_SubDev_FaultData_Sort(41);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[2];
                        m_SubDevErr->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[3]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car1BCUErrList.length() < m_BCUFaultCount[0]){
                            car1BCUErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 2: // Car2
                    for(int i = 0; i < car2BCUErrList.length(); i++){
                        if(SubDevFltData_MDU[2] == car2BCUErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car2BCUErrList.at(i)->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[3]);
                            car2BCUErrList.at(i)->strDate = strDate;
                            car_SubDev_FaultData_Sort(42);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[2];
                        m_SubDevErr->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[3]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car2BCUErrList.length() < m_BCUFaultCount[1]){
                            car2BCUErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 3:  // Car3
                    for(int i = 0; i < car3BCUErrList.length(); i++){
                        if(SubDevFltData_MDU[2] == car3BCUErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car3BCUErrList.at(i)->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[3]);
                            car3BCUErrList.at(i)->strDate = strDate;
                            car_SubDev_FaultData_Sort(43);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[2];
                        m_SubDevErr->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[3]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car3BCUErrList.length() < m_BCUFaultCount[2]){
                            car3BCUErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 4: // Car4
                    for(int i = 0; i < car4BCUErrList.length(); i++){
                        if(SubDevFltData_MDU[2] == car4BCUErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car4BCUErrList.at(i)->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[3]);
                            car4BCUErrList.at(i)->strDate = strDate;
                            car_SubDev_FaultData_Sort(44);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[2];
                        m_SubDevErr->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[3]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car4BCUErrList.length() < m_BCUFaultCount[3]){
                            car4BCUErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 5: // Car5
                    for(int i = 0; i < car5BCUErrList.length(); i++){
                        if(SubDevFltData_MDU[2] == car5BCUErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car5BCUErrList.at(i)->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[3]);
                            car5BCUErrList.at(i)->strDate = strDate;
                            car_SubDev_FaultData_Sort(45);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[2];
                        m_SubDevErr->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[3]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car5BCUErrList.length() < m_BCUFaultCount[4]){
                            car5BCUErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 6:  // Car6
                    for(int i = 0; i < car6BCUErrList.length(); i++){
                        if(SubDevFltData_MDU[2] == car6BCUErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car6BCUErrList.at(i)->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[3]);
                            car6BCUErrList.at(i)->strDate = strDate;
                            car_SubDev_FaultData_Sort(46);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[2];
                        m_SubDevErr->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[3]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car6BCUErrList.length() < m_BCUFaultCount[5]){
                            car6BCUErrList.append(m_SubDevErr);
                        }
                    }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
    else{
        int iHour = SubDevFltData_MDU[11]/16*10+SubDevFltData_MDU[11]%16;
        int iMinute = SubDevFltData_MDU[12]/16*10+SubDevFltData_MDU[12]%16;
        int iSecond = SubDevFltData_MDU[13]/16*10+SubDevFltData_MDU[13]%16;
        strTime = (iHour < 10 ? QString("0%1").arg(iHour) : QString("%1").arg(iHour))+":"+(iMinute < 10 ? QString("0%1").arg(iMinute) : QString("%1").arg(iMinute))+":"+(iSecond < 10 ? QString("0%1").arg(iSecond) : QString("%1").arg(iSecond));
        switch (SubDevFltData_MDU[7]) {
        case 1:  // VVVF1 Error
            switch (SubDevFltData_MDU[8]) {
            case 1: // Car1
                    for(int i = 0; i < car1VVVF1ErrList.length(); i++){
                        if(SubDevFltData_MDU[9] == car1VVVF1ErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car1VVVF1ErrList.at(i)->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[10]);
                            car1VVVF1ErrList.at(i)->strTime = strTime;
                            car_SubDev_FaultData_Sort(11);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[9];
                        m_SubDevErr->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[10]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car1VVVF1ErrList.length() < m_VVVF1FaultCount[0]){
                            car1VVVF1ErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 3:  // Car3
                    for(int i = 0; i < car3VVVF1ErrList.length(); i++){
                        if(SubDevFltData_MDU[9] == car3VVVF1ErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car3VVVF1ErrList.at(i)->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[10]);
                            car3VVVF1ErrList.at(i)->strTime = strTime;
                            car_SubDev_FaultData_Sort(13);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[9];
                        m_SubDevErr->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[10]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car3VVVF1ErrList.length() < m_VVVF1FaultCount[1]){
                            car3VVVF1ErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 6:  // Car6
                    for(int i = 0; i < car6VVVF1ErrList.length(); i++){
                        if(SubDevFltData_MDU[9] == car6VVVF1ErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car6VVVF1ErrList.at(i)->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[10]);
                            car6VVVF1ErrList.at(i)->strTime = strTime;
                            car_SubDev_FaultData_Sort(16);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[9];
                        m_SubDevErr->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[10]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car6VVVF1ErrList.length() < m_VVVF1FaultCount[2]){
                            car6VVVF1ErrList.append(m_SubDevErr);
                        }
                    }
                break;
            default:
                break;
            }
            break;
        case 2:  // VVVF2 Error
            switch (SubDevFltData_MDU[8]) {
            case 1: // Car1
                    for(int i = 0; i < car1VVVF2ErrList.length(); i++){
                        if(SubDevFltData_MDU[9] == car1VVVF2ErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car1VVVF2ErrList.at(i)->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[10]);
                            car1VVVF2ErrList.at(i)->strTime = strTime;
                            car_SubDev_FaultData_Sort(21);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[9];
                        m_SubDevErr->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[10]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car1VVVF2ErrList.length() < m_VVVF2FaultCount[0]){
                            car1VVVF2ErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 3:  // Car3
                    for(int i = 0; i < car3VVVF2ErrList.length(); i++){
                        if(SubDevFltData_MDU[9] == car3VVVF2ErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car3VVVF2ErrList.at(i)->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[10]);
                            car3VVVF2ErrList.at(i)->strTime = strTime;
                            car_SubDev_FaultData_Sort(23);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[9];
                        m_SubDevErr->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[10]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car3VVVF2ErrList.length() < m_VVVF2FaultCount[1]){
                            car3VVVF2ErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 6:  // Car6
                    for(int i = 0; i < car6VVVF2ErrList.length(); i++){
                        if(SubDevFltData_MDU[9] == car6VVVF2ErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car6VVVF2ErrList.at(i)->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[10]);
                            car6VVVF2ErrList.at(i)->strTime = strTime;
                            car_SubDev_FaultData_Sort(26);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[9];
                        m_SubDevErr->strErrCode = getVVVFErrorInfo(SubDevFltData_MDU[10]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car6VVVF2ErrList.length() < m_VVVF2FaultCount[2]){
                            car6VVVF2ErrList.append(m_SubDevErr);
                        }
                    }
                break;
            default:
                break;
            }
            break;
        case 3:  // SIV Error
            switch (SubDevFltData_MDU[8]) {
            case 2: // Car2
                    for(int i = 0; i < car2SIVErrList.length(); i++){
                        if(SubDevFltData_MDU[9] == car2SIVErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car2SIVErrList.at(i)->strErrCode = getSIVErrorInfo(SubDevFltData_MDU[10]);
                            car2SIVErrList.at(i)->strTime = strTime;
                            car_SubDev_FaultData_Sort(32);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[9];
                        m_SubDevErr->strErrCode = getSIVErrorInfo(SubDevFltData_MDU[10]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car2SIVErrList.length() < m_SIVFaultCount[0]){
                            car2SIVErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 4:  // Car4
                    for(int i = 0; i < car4SIVErrList.length(); i++){
                        if(SubDevFltData_MDU[9] == car4SIVErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car4SIVErrList.at(i)->strErrCode = getSIVErrorInfo(SubDevFltData_MDU[10]);
                            car4SIVErrList.at(i)->strTime = strTime;
                            car_SubDev_FaultData_Sort(34);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[9];
                        m_SubDevErr->strErrCode = getSIVErrorInfo(SubDevFltData_MDU[10]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car4SIVErrList.length() < m_SIVFaultCount[1]){
                            car4SIVErrList.append(m_SubDevErr);
                        }
                    }
                break;
            default:
                break;
            }
            break;
        case 4:  // BCU Error
            switch (SubDevFltData_MDU[8]) {
            case 1: // Car1
                    for(int i = 0; i < car1BCUErrList.length(); i++){
                        if(SubDevFltData_MDU[9] == car1BCUErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car1BCUErrList.at(i)->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[10]);
                            car1BCUErrList.at(i)->strTime = strTime;
                            car_SubDev_FaultData_Sort(41);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[9];
                        m_SubDevErr->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[10]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car1BCUErrList.length() < m_BCUFaultCount[0]){
                            car1BCUErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 2: // Car2
                    for(int i = 0; i < car2BCUErrList.length(); i++){
                        if(SubDevFltData_MDU[9] == car2BCUErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car2BCUErrList.at(i)->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[10]);
                            car2BCUErrList.at(i)->strTime = strTime;
                            car_SubDev_FaultData_Sort(42);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[9];
                        m_SubDevErr->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[10]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car2BCUErrList.length() < m_BCUFaultCount[1]){
                            car2BCUErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 3:  // Car3
                    for(int i = 0; i < car3BCUErrList.length(); i++){
                        if(SubDevFltData_MDU[9] == car3BCUErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car3BCUErrList.at(i)->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[10]);
                            car3BCUErrList.at(i)->strTime = strTime;
                            car_SubDev_FaultData_Sort(43);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[9];
                        m_SubDevErr->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[10]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car3BCUErrList.length() < m_BCUFaultCount[2]){
                            car3BCUErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 4: // Car4
                    for(int i = 0; i < car4BCUErrList.length(); i++){
                        if(SubDevFltData_MDU[9] == car4BCUErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car4BCUErrList.at(i)->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[10]);
                            car4BCUErrList.at(i)->strTime = strTime;
                            car_SubDev_FaultData_Sort(44);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[9];
                        m_SubDevErr->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[10]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car4BCUErrList.length() < m_BCUFaultCount[3]){
                            car4BCUErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 5: // Car5
                    for(int i = 0; i < car5BCUErrList.length(); i++){
                        if(SubDevFltData_MDU[9] == car5BCUErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car5BCUErrList.at(i)->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[10]);
                            car5BCUErrList.at(i)->strTime = strTime;
                            car_SubDev_FaultData_Sort(45);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[9];
                        m_SubDevErr->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[10]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car5BCUErrList.length() < m_BCUFaultCount[4]){
                            car5BCUErrList.append(m_SubDevErr);
                        }
                    }
                break;
            case 6:  // Car6
                    for(int i = 0; i < car6BCUErrList.length(); i++){
                        if(SubDevFltData_MDU[9] == car6BCUErrList.at(i)->iErrId){
                            bExistFlag = true;
//                            car6BCUErrList.at(i)->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[10]);
                            car6BCUErrList.at(i)->strTime = strTime;
                            car_SubDev_FaultData_Sort(46);
                            break;
                        }
                    }
                    if(!bExistFlag){
                        m_SubDevErr->iErrId = SubDevFltData_MDU[9];
                        m_SubDevErr->strErrCode = getBCUErrorInfo(SubDevFltData_MDU[10]);
                        m_SubDevErr->strDate = strDate;
                        m_SubDevErr->strTime = strTime;
                        if(car6BCUErrList.length() < m_BCUFaultCount[5]){
                            car6BCUErrList.append(m_SubDevErr);
                        }
                    }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
    emit sig_FaultRecordData_Get_Update();
}

void CanSignalsSlots::car_PTU_CMD_Update(){
    if(1 == PTUCMD_ERM && !bPTUDownloading){
        bPTUDownloading = true;
        emit sig_PTUDownloadState_Update(true);
    }
    else if(2 == PTUCMD_ERM && bPTUDownloading){
        bPTUDownloading = false;
        emit sig_PTUDownloadState_Update(false);
    }
    else{
        bPTUDownloading = false;
    }
}

void CanSignalsSlots::car_Header_Update(){
    CAR_HEADER temp = CAR_NO_HEADER;
    if(getCanDataBitFromByte(DI_Data_MC1[2], 0) && !getCanDataBitFromByte(DI_Data_MC2[2], 0)){
        temp = CAR_1_HEADER;
//        Car_Data_HMI[1] = setCanDataBitFromByte(Car_Data_HMI[1], 6, 1);
//        Car_Data_HMI[1] = setCanDataBitFromByte(Car_Data_HMI[1], 7, 0);
        Car_Data_HMI[1] = 2;
    }
    else if(!getCanDataBitFromByte(DI_Data_MC1[2], 0) && getCanDataBitFromByte(DI_Data_MC2[2], 0)){
        temp = CAR_6_HEADER;
//        Car_Data_HMI[1] = setCanDataBitFromByte(Car_Data_HMI[1], 6, 0);
//        Car_Data_HMI[1] = setCanDataBitFromByte(Car_Data_HMI[1], 7, 1);
        Car_Data_HMI[1] = 1;
    }
    else{
        temp = CAR_NO_HEADER;
        Car_Data_HMI[1] = 0;
    }
    if(CAR_NO_HEADER == temp){
        lastCarHeaderFlag = carHeaderFlag;
    }
    if(temp != carHeaderFlag){
        carHeaderFlag = temp;

        if((CAR_1_HEADER == carHeaderFlag && 1 == deviceCarId) || (CAR_6_HEADER == carHeaderFlag && 6 == deviceCarId)){
            nowCtrlHMIFlag = true;
        }
        else{
            nowCtrlHMIFlag = false;
        }
        if(masterHMIFlag){
            car_Direction_Update();
        }
    }
}

void CanSignalsSlots::car_ATOModel_Update(){
    if(masterHMIFlag){
        u_int8_t atoModel = 0;
        if(bTMSDevComnErrFlag[2] || bTMSDevComnErrFlag[7]){
            atoModel = 2;
        }
        else if(!getCanDataBitFromByte(DI_Data_MC1[2], 4) && !getCanDataBitFromByte(DI_Data_MC2[2], 4)){
            atoModel = 0;
        }
        else if(getCanDataBitFromByte(DI_Data_MC1[2], 5) || getCanDataBitFromByte(DI_Data_MC2[2], 5)){
            atoModel = 1;
        }
        else if(getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 6) && getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 6)){
            atoModel = 2;
        }
        else if(!getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 6)){
            if(getCanDataBitFromByte(ATCData_MC1[1], 3)){
                atoModel = 4;
            }
            else{
                atoModel = 3;
            }
        }
        else if(getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 6) && !getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 6)){
            if(getCanDataBitFromByte(ATCData_MC2[1], 3)){
                atoModel = 4;
            }
            else{
                atoModel = 3;
            }
        }
        else{
            // do nothing
        }
        if(Car_Data_HMI[0] != atoModel){
            Car_Data_HMI[0] = atoModel;
            if(4 == atoModel){
                atoModelFlag = true;
                bATOModeFirstFlag = true;
                CarArriveData_HMI[0] = setCanDataBitFromByte(CarArriveData_HMI[0], 0, 1);
//                car_ATCDateTime_Update();
            }
            else{
                atoModelFlag = false;
                CarArriveData_HMI[0] = setCanDataBitFromByte(CarArriveData_HMI[0], 0, 0);
            }
            emit sig_Car_ATOModel_Change(atoModel);
        }
    }
}

void CanSignalsSlots::setCarStationManual(int curStaId, int endStaId){
    if(!atoModelFlag){
        iCurrStation = curStaId;
        iEndStation = endStaId;
        emit sig_Car_Arrive_Station_Update(1, curStaId);
        int nextStaId = 0, iStaCount = stationCodeList.length();
        if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
            for(int i = 0; i < iStaCount; i++){
                if(curStaId == stationCodeList.at(i)){
                    if(i == 0){
                        nextStaId = 0;
                    }
                    else{
                        nextStaId = stationCodeList.at(i-1);
                    }
                    break;
                }
            }
        }
        else{
            for(int i = 0; i < iStaCount; i++){
                if(curStaId == stationCodeList.at(i)){
                    if(i == iStaCount-1){
                        nextStaId = 0;
                    }
                    else{
                        nextStaId = stationCodeList.at(i+1);
                    }
                    break;
                }
            }
        }
        emit sig_Car_Arrive_Station_Update(2, nextStaId);
        emit sig_Car_Arrive_Station_Update(3, endStaId);
    }
}

void CanSignalsSlots::car_FaultRecordOperation_Update(CAR_ID carId, int devCode, int errCode, bool bFlag){
    if(masterHMIFlag){
        qDebug()<<"Fault Info Operation , CarId == "<<carId<<"  DevId == "<<devCode<<"   ErrCode  ==  "<<errCode<<"    ErrFalg ==  "<<bFlag;
        QString strDate = QDate::currentDate().toString("yy_MM_dd");
        QString strTime = QTime::currentTime().toString("hh:mm:ss");
        int mCurStationDistance = qRound(fCurStationDistance);

        FaultInfo_ERM[0] = int(carId);
        FaultInfo_ERM[1] = bFlag ? 1 : 2;
        FaultInfo_ERM[2] = 0;
        FaultInfo_ERM[3] = strDate.section("_", 0, 0).toInt();
        FaultInfo_ERM[4] = strDate.section("_", 1, 1).toInt();
        FaultInfo_ERM[5] = strDate.section("_", 2, 2).toInt();
        FaultInfo_ERM[6] = strTime.section(":", 0, 0).toInt();
        FaultInfo_ERM[7] = strTime.section(":", 1, 1).toInt();
        FaultInfo_ERM[8] = strTime.section(":", 2, 2).toInt();
        FaultInfo_ERM[9] = CarArriveData_HMI[4];   // iCurrStation >> 8
        FaultInfo_ERM[10] = CarArriveData_HMI[5];   // iCurrStation << 8 >> 8
        FaultInfo_ERM[11] = mCurStationDistance >> 8 & 0xFF;
        FaultInfo_ERM[12] = mCurStationDistance & 0xFF;
        FaultInfo_ERM[13] = devCode;
        FaultInfo_ERM[14] = errCode;
        FaultInfo_ERM[15] = mVVVF_BCU_Level_Data[0];
        FaultInfo_ERM[16] = mVVVF_BCU_Level_Data[1];
        FaultInfo_ERM[17] = carSpeedValue[1];
        FaultInfo_ERM[18] = (carVoltageValue[1] >> 8) & 0xFF;
        FaultInfo_ERM[19] = carVoltageValue[1] & 0xFF;

        //HMI发送故障履历数据
        sendSDOCheckData(6);
    }
}

void CanSignalsSlots::car_SysDateTime_Update(){
    QString tempTime = "";
    QString strYear = QString::number(2000 + Date_Time_HMI[1]);
    QString strMonth = Date_Time_HMI[2] < 10 ? "0"+ QString::number(Date_Time_HMI[2]) : QString::number(Date_Time_HMI[2]);
    QString strDay = Date_Time_HMI[3] < 10 ? "0"+ QString::number(Date_Time_HMI[3]) : QString::number(Date_Time_HMI[3]);
    QString strHour = Date_Time_HMI[4] < 10 ? "0"+ QString::number(Date_Time_HMI[4]) : QString::number(Date_Time_HMI[4]);
    QString strMinute = Date_Time_HMI[5] < 10 ? "0"+ QString::number(Date_Time_HMI[5]) : QString::number(Date_Time_HMI[5]);
    QString strSecond = Date_Time_HMI[6] < 10 ? "0"+ QString::number(Date_Time_HMI[6]) : QString::number(Date_Time_HMI[6]);

    tempTime = strYear+"-"+strMonth+"-"+strDay+" "+strHour+":"+strMinute+":"+strSecond;
    qDebug()<<"HMI Update The Car DateTime, HMI HMI HMI HMI HMI HMI HMI  DateTime Set == "<<tempTime;
    QProcess::execute(QString("date \"%1\"").arg(tempTime));
    QProcess::execute("hwclock -w");
}

void CanSignalsSlots::car_HMI_2_SetDateTime(){
    sendCarDateTimeHMI(SetDate_Time_HMI[1], SetDate_Time_HMI[2], SetDate_Time_HMI[3], SetDate_Time_HMI[4], SetDate_Time_HMI[5], SetDate_Time_HMI[6]);
    QString tempTime = "";
    QString strYear = QString::number(2000 + Date_Time_HMI[1]);
    QString strMonth = Date_Time_HMI[2] < 10 ? "0"+ QString::number(Date_Time_HMI[2]) : QString::number(Date_Time_HMI[2]);
    QString strDay = Date_Time_HMI[3] < 10 ? "0"+ QString::number(Date_Time_HMI[3]) : QString::number(Date_Time_HMI[3]);
    QString strHour = Date_Time_HMI[4] < 10 ? "0"+ QString::number(Date_Time_HMI[4]) : QString::number(Date_Time_HMI[4]);
    QString strMinute = Date_Time_HMI[5] < 10 ? "0"+ QString::number(Date_Time_HMI[5]) : QString::number(Date_Time_HMI[5]);
    QString strSecond = Date_Time_HMI[6] < 10 ? "0"+ QString::number(Date_Time_HMI[6]) : QString::number(Date_Time_HMI[6]);

    tempTime = strYear+"-"+strMonth+"-"+strDay+" "+strHour+":"+strMinute+":"+strSecond;
    qDebug()<<"HMI Update The Car DateTime, HMI HMI HMI HMI HMI HMI HMI  DateTime Set == "<<tempTime;
    QProcess::execute(QString("date \"%1\"").arg(tempTime));
    QProcess::execute("hwclock -w");
}

void CanSignalsSlots::car_AnotherHMIGetFaultData_Update(){
    int errType = M_GetFaultData_HMI[0];
    int carId = M_GetFaultData_HMI[1];

    if(1 == errType && bNowShowERMFaultFlag){
//        switch (CAR_ID(carId)) {
//        case CAR_1:
//            qDeleteAll(car1FaultList);
//            car1FaultList.clear();
//            break;
//        case CAR_2:
//            qDeleteAll(car2FaultList);
//            car2FaultList.clear();
//            break;
//        case CAR_3:
//            qDeleteAll(car3FaultList);
//            car3FaultList.clear();
//            break;
//        case CAR_4:
//            qDeleteAll(car4FaultList);
//            car4FaultList.clear();
//            break;
//        case CAR_5:
//            qDeleteAll(car5FaultList);
//            car5FaultList.clear();
//            break;
//        case CAR_6:
//            qDeleteAll(car6FaultList);
//            car6FaultList.clear();
//            break;
//        default:
//            break;
//        }
    }
    else if(bNowShowSubDevErrFlag){
        switch (errType) {
        case 1:   //故障履历
            break;
        case 2:  //VVVF1
            switch (CAR_ID(carId)) {
            case CAR_1:
                qDeleteAll(car1VVVF1ErrList);
                car1VVVF1ErrList.clear();
                break;
            case CAR_3:
                qDeleteAll(car3VVVF1ErrList);
                car3VVVF1ErrList.clear();
                break;
            case CAR_6:
                qDeleteAll(car6VVVF1ErrList);
                car6VVVF1ErrList.clear();
                break;
            default:
                break;
            }
            break;
        case 3:  //VVVF2
            switch (CAR_ID(carId)) {
            case CAR_1:
                qDeleteAll(car1VVVF2ErrList);
                car1VVVF2ErrList.clear();
                break;
            case CAR_3:
                qDeleteAll(car3VVVF2ErrList);
                car3VVVF2ErrList.clear();
                break;
            case CAR_6:
                qDeleteAll(car6VVVF2ErrList);
                car6VVVF2ErrList.clear();
                break;
            default:
                break;
            }
            break;
        case 4:  //SIV
            switch (CAR_ID(carId)) {
            case CAR_2:
                qDeleteAll(car2SIVErrList);
                car2SIVErrList.clear();
                break;
            case CAR_4:
                qDeleteAll(car4SIVErrList);
                car4SIVErrList.clear();
                break;
            default:
                break;
            }
            break;
        case 5:  //BCU
            switch (CAR_ID(carId)) {
            case CAR_1:
                qDeleteAll(car1BCUErrList);
                car1BCUErrList.clear();
                break;
            case CAR_2:
                qDeleteAll(car2BCUErrList);
                car2BCUErrList.clear();
                break;
            case CAR_3:
                qDeleteAll(car3BCUErrList);
                car3BCUErrList.clear();
                break;
            case CAR_4:
                qDeleteAll(car4BCUErrList);
                car4BCUErrList.clear();
                break;
            case CAR_5:
                qDeleteAll(car5BCUErrList);
                car5BCUErrList.clear();
                break;
            case CAR_6:
                qDeleteAll(car6BCUErrList);
                car6BCUErrList.clear();
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
    else{
        // do nothing
    }
}

void CanSignalsSlots::car_AnotherHMIGetSpeedData_Update(){
    if(bNowShowSpeedFlag){
        qDeleteAll(carSpeedDataList);
        carSpeedDataList.clear();
    }
}

void CanSignalsSlots::sendCarDateTimeHMI(int iYear, int iMonth, int iDay, int iHour, int iMinute, int iSecond){
    if(masterHMIFlag){
        Date_Time_HMI[0] = 1;
        Date_Time_HMI[1] = iYear;
        Date_Time_HMI[2] = iMonth;
        Date_Time_HMI[3] = iDay;
        Date_Time_HMI[4] = iHour;
        Date_Time_HMI[5] = iMinute;
        Date_Time_HMI[6] = iSecond;
        canSendCarDateTimeUpdate();
    }
    else{
        SetDate_Time_HMI[0] = 1;
        SetDate_Time_HMI[1] = iYear;
        SetDate_Time_HMI[2] = iMonth;
        SetDate_Time_HMI[3] = iDay;
        SetDate_Time_HMI[4] = iHour;
        SetDate_Time_HMI[5] = iMinute;
        SetDate_Time_HMI[6] = iSecond;
        canSendHMISetDateTime_Update();
    }
}

void CanSignalsSlots::car_Car_Data_HMI_Update(){
//    qDebug()<<"@!@!@!@ car_Car_Data_HMI_Update   Data  =  "<<Car_Data_HMI[0]<<"  "<<Car_Data_HMI[1]<<"  "<<Car_Data_HMI[2]<<"  "<<Car_Data_HMI[3]<<"  "<<Car_Data_HMI[4]<<"  "<<Car_Data_HMI[5]<<"  "<<Car_Data_HMI[6];
    CAR_HEADER temp = CAR_NO_HEADER;

    if(1 == Car_Data_HMI[1]){
        temp = CAR_6_HEADER;
    }
    else if(2 == Car_Data_HMI[1]){
        temp = CAR_1_HEADER;
    }
    else{
        temp = CAR_NO_HEADER;
    }

    if(temp != carHeaderFlag){
        lastCarHeaderFlag = carHeaderFlag;
        carHeaderFlag = temp;
        if(CAR_6_HEADER == carHeaderFlag && 6 == deviceCarId){
            nowCtrlHMIFlag = true;
        }
        else{
            nowCtrlHMIFlag = false;
        }
        car_Direction_Update();
    }
    if(4 == Car_Data_HMI[0]){
        atoModelFlag = true;
//        getCarDestinationID();
//        getCarNowStationID();
//        getCarNextStationID();
    }
    else{
        atoModelFlag = false;
    }
    emit sig_Car_ATOModel_Change(Car_Data_HMI[0]);

//    qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
//    if(nowCtrlHMIFlag){
////        qDebug()<<"@#$%^&*()*&^%$#! ################$$$$$$$$$$$$$$$$$$$$$$$ @#@@@@@@@@@@@@@@#@$@#$@$   Device Id == "<<deviceCarId;
////        sendCarParaData_Changed(CAR_ID(1), 2);
////        sendCarParaData_Changed(CAR_ID(4), 2);
//    }
}

void CanSignalsSlots::car_ATCDateTime_Update(){
    if(masterHMIFlag){
        if(bATOModeFirstFlag && bATODateTimeUpdateFlag){
            if(!getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 6)){
                Date_Time_HMI[0] = 1;
                for(int i = 0; i < 6; i++){
                    Date_Time_HMI[i+1] = ATCDateTime_MC1[i]/16*10+ATCDateTime_MC1[i]%16;
                }
            }
            else if(getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 6) && !getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 6)){
                Date_Time_HMI[0] = 1;
                for(int i = 0; i < 6; i++){
                    Date_Time_HMI[i+1] = ATCDateTime_MC2[i]/16*10+ATCDateTime_MC2[i]%16;
                }
            }
            else {
                //do nothing
            }
            if(bATOModeFirstFlag && bATODateTimeUpdateFlag){
                canSendCarDateTimeUpdate();
//                car_SysDateTime_Update();
                bATOModeFirstFlag = false;
                bATODateTimeUpdateFlag = false;
            }
        }
    }
}

void CanSignalsSlots::car_DateTime_HMI_Update(){
    if(!masterHMIFlag && 1 == Date_Time_HMI[0]){
        car_SysDateTime_Update();
    }
//    else if(masterHMIFlag && 2 == Date_Time_HMI[0]){
//        Date_Time_HMI[0] = 1;
//        canSendCarDateTimeUpdate();
//        car_SysDateTime_Update();
//    }
//    else{
//        //do nothing
//    }
}

void CanSignalsSlots::car_DoorData_Update(CAR_ID carId){
    bool bMDUComnErrFlag = false;
    switch (carId) {
    case CAR_1:
        bMDUComnErrFlag = bTMSDevComnErrFlag[2];
        for(int i = 0; i < 8; i++){
            if(bMDUComnErrFlag || getCanDataBitFromByte(SubDev_ComnErr_MC1[0], i)){
                if(0 != doorMC1State[i]){
                    doorMC1State[i] = 0;
                    emit sig_Door_State_Change(carId, DOOR_ID(i+1), RUN_DOOR_COMN_ERR);
                }
            }
            else{
                door_OneSideState_Update(carId, DOOR_ID(i+1));
            }
        }
        break;
    case CAR_2:
        bMDUComnErrFlag = bTMSDevComnErrFlag[3];
        for(int i = 0; i < 8; i++){
            if(bMDUComnErrFlag || getCanDataBitFromByte(SubDev_ComnErr_T[0], i)){
                if(0 != doorTState[i]){
                    doorTState[i] = 0;
                    emit sig_Door_State_Change(carId, DOOR_ID(i+1), RUN_DOOR_COMN_ERR);
                }
            }
            else{
                door_OneSideState_Update(carId, DOOR_ID(i+1));
            }
        }
        break;
    case CAR_3:
        bMDUComnErrFlag = bTMSDevComnErrFlag[4];
        for(int i = 0; i < 8; i++){
            if(bMDUComnErrFlag || getCanDataBitFromByte(SubDev_ComnErr_M[0], i)){
                if(0 != doorMState[i]){
                    doorMState[i] = 0;
                    emit sig_Door_State_Change(carId, DOOR_ID(i+1), RUN_DOOR_COMN_ERR);
                }
            }
            else{
                door_OneSideState_Update(carId, DOOR_ID(i+1));
            }
        }
        break;
    case CAR_4:
        bMDUComnErrFlag = bTMSDevComnErrFlag[5];
        for(int i = 0; i < 8; i++){
            if(bMDUComnErrFlag || getCanDataBitFromByte(SubDev_ComnErr_T1[0], i)){
                if(0 != doorT1State[i]){
                    doorT1State[i] = 0;
                    emit sig_Door_State_Change(carId, DOOR_ID(i+1), RUN_DOOR_COMN_ERR);
                }
            }
            else{
                door_OneSideState_Update(carId, DOOR_ID(i+1));
            }
        }
        break;
    case CAR_5:
        bMDUComnErrFlag = bTMSDevComnErrFlag[6];
        for(int i = 0; i < 8; i++){
            if(bMDUComnErrFlag || getCanDataBitFromByte(SubDev_ComnErr_T2[0], i)){
                if(0 != doorT2State[i]){
                    doorT2State[i] = 0;
                    emit sig_Door_State_Change(carId, DOOR_ID(i+1), RUN_DOOR_COMN_ERR);
                }
            }
            else{
                door_OneSideState_Update(carId, DOOR_ID(i+1));
            }
        }
        break;
    case CAR_6:
        bMDUComnErrFlag = bTMSDevComnErrFlag[7];
        for(int i = 0; i < 8; i++){
            if(bMDUComnErrFlag || getCanDataBitFromByte(SubDev_ComnErr_MC2[0], i)){
                if(0 != doorMC2State[i]){
                    doorMC2State[i] = 0;
                    emit sig_Door_State_Change(carId, DOOR_ID(i+1), RUN_DOOR_COMN_ERR);
                }
            }
            else{
                door_OneSideState_Update(carId, DOOR_ID(i+1));
            }
        }
        break;
    default:
        break;
    }
//    if(masterHMIFlag){
//        car_Door_OpenClose_State_Update();
//    }
}

void CanSignalsSlots::car_ACCData_Update(CAR_ID carId){
    car_ACC_SetInfo_Update(carId);
    car_ACC_Temp_Update(carId);
}

void CanSignalsSlots::car_ACCData_2_Update(){
    car_ACC_Ctrl_Mode_Update();
}

void CanSignalsSlots::car_BCUData_Update(CAR_ID carId, int dataIndex){
    if(1 == dataIndex || 2 == dataIndex){
        car_BCU_BCKpa_Update(carId);
        car_Load_Rate_Update(carId);
//        if(masterHMIFlag){
//            car_Load_Rate_AVG_Update();
//        }
//    }
//    if(3 == dataIndex){
//        car_VVVF_BCU_Level_Update();
//        if(masterHMIFlag){
//            car_TotalWeight_Update();
//        }
    }
}

void CanSignalsSlots::car_BCUData_4_Update(CAR_ID carId){
    if(CAR_NULL == carId){
        return ;
    }
    bool bChangeFlag = false;
    int iTempFaultCount = 0;
    switch (carId) {
    case CAR_1:
        iTempFaultCount = getBCUErrorCount(BCUData_MC1[24]);
        if(iTempFaultCount != m_BCUFaultCount[0]){
            m_BCUFaultCount[0] = iTempFaultCount;
            bChangeFlag = true;
        }
        break;
    case CAR_2:
        iTempFaultCount = getBCUErrorCount(BCUData_T[24]);
        if(iTempFaultCount != m_BCUFaultCount[1]){
            m_BCUFaultCount[1] = iTempFaultCount;
            bChangeFlag = true;
        }
        break;
    case CAR_3:
        iTempFaultCount = getBCUErrorCount(BCUData_M[24]);
        if(iTempFaultCount != m_BCUFaultCount[2]){
            m_BCUFaultCount[2] = iTempFaultCount;
            bChangeFlag = true;
        }
        break;
    case CAR_4:
        iTempFaultCount = getBCUErrorCount(BCUData_T1[24]);
        if(iTempFaultCount != m_BCUFaultCount[3]){
            m_BCUFaultCount[3] = iTempFaultCount;
            bChangeFlag = true;
        }
        break;
    case CAR_5:
        iTempFaultCount = getBCUErrorCount(BCUData_T2[24]);
        if(iTempFaultCount != m_BCUFaultCount[4]){
            m_BCUFaultCount[4] = iTempFaultCount;
            bChangeFlag = true;
        }
        break;
    case CAR_6:
        iTempFaultCount = getBCUErrorCount(BCUData_MC2[24]);
        if(iTempFaultCount != m_BCUFaultCount[5]){
            m_BCUFaultCount[5] = iTempFaultCount;
            bChangeFlag = true;
        }
        break;
    default:
        bChangeFlag = false;
        break;
    }

    if(bChangeFlag){
        emit sig_FaultRecordCount_Get_Update();
    }
}

void CanSignalsSlots::car_ATCData_1_Update(CAR_ID /*carId*/){
    if(masterHMIFlag){
        car_ATOModel_Update();
        if(!getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 6)){
            if(bATOModeFirstFlag && (getCanDataBitFromByte(ATCData_MC1[1], 6) || getCanDataBitFromByte(ATCData_MC1[1], 7))){
                bATODateTimeUpdateFlag = true;
            }
        }
        else if(getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 6) && !getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 6)){
            if(bATOModeFirstFlag && (getCanDataBitFromByte(ATCData_MC2[1], 6) || getCanDataBitFromByte(ATCData_MC2[1], 7))){
                bATODateTimeUpdateFlag = true;
            }
        }
        else{
            bATODateTimeUpdateFlag = false;
        }
    }
}

void CanSignalsSlots::car_ATCData_2_Update(CAR_ID /*carId*/){
    getCarDestinationID();
}

void CanSignalsSlots::car_ATCData_3_Update(CAR_ID /*carId*/){
    if(masterHMIFlag){
        if(!getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 6)){
            CarArriveData_HMI[0] = setCanDataBitFromByte(CarArriveData_HMI[0], 1, (getCanDataBitFromByte(ATCData_MC1[15], 5) ? 1 : 0));
            CarArriveData_HMI[0] = setCanDataBitFromByte(CarArriveData_HMI[0], 2, (getCanDataBitFromByte(ATCData_MC1[15], 4) ? 1 : 0));
            CarArriveData_HMI[0] = setCanDataBitFromByte(CarArriveData_HMI[0], 4, (getCanDataBitFromByte(ATCData_MC1[16], 2) ? 1 : 0));
            CarArriveData_HMI[0] = setCanDataBitFromByte(CarArriveData_HMI[0], 5, (getCanDataBitFromByte(ATCData_MC1[16], 0) ? 1 : 0));
            CarArriveData_HMI[0] = setCanDataBitFromByte(CarArriveData_HMI[0], 6, (getCanDataBitFromByte(ATCData_MC1[16], 1) ? 1 : 0));
        }
        else if(getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 6) && !getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 6)){
            CarArriveData_HMI[0] = setCanDataBitFromByte(CarArriveData_HMI[0], 1, (getCanDataBitFromByte(ATCData_MC2[15], 5) ? 1 : 0));
            CarArriveData_HMI[0] = setCanDataBitFromByte(CarArriveData_HMI[0], 2, (getCanDataBitFromByte(ATCData_MC2[15], 4) ? 1 : 0));
            CarArriveData_HMI[0] = setCanDataBitFromByte(CarArriveData_HMI[0], 4, (getCanDataBitFromByte(ATCData_MC2[16], 2) ? 1 : 0));
            CarArriveData_HMI[0] = setCanDataBitFromByte(CarArriveData_HMI[0], 5, (getCanDataBitFromByte(ATCData_MC2[16], 0) ? 1 : 0));
            CarArriveData_HMI[0] = setCanDataBitFromByte(CarArriveData_HMI[0], 6, (getCanDataBitFromByte(ATCData_MC2[16], 1) ? 1 : 0));
        }
        else {
            //do nothing
        }
        canSendCarArriveData1_Update();
    }
    getCarNowStationID();
    getCarNextStationID();
}

void CanSignalsSlots::car_ATCData_4_Update(CAR_ID /*carId*/){

}

void CanSignalsSlots::car_PISData_1_Update(CAR_ID /*carId*/){
    car_Urgent_Alert_Update();
}

void CanSignalsSlots::car_PISData_2_Update(CAR_ID /*carId*/){
    car_Urgent_Alert_Update();
}

void CanSignalsSlots::car_VVVFData_1_Update(CAR_ID carId, int vvvfIndex){
//    car_VVVF_Elec_BCU_Update(carId, vvvfIndex);
    bool bVVVFBrakeState  = false;
    switch (carId) {
    case CAR_1:
        if(bTMSDevComnErrFlag[2]){
            bVVVFBrakeState = false;
        }
        else{
            if(1 == vvvfIndex){
                bVVVFBrakeState = getCanDataBitFromByte(VVVF1Data_MC1[0], 2);
            }
            else{
                bVVVFBrakeState = getCanDataBitFromByte(VVVF2Data_MC1[0], 2);
            }
        }
        break;
    case CAR_3:
        if(bTMSDevComnErrFlag[4]){
            bVVVFBrakeState = false;
        }
        else{
            if(1 == vvvfIndex){
                bVVVFBrakeState = getCanDataBitFromByte(VVVF1Data_M[0], 2);
            }
            else{
                bVVVFBrakeState = getCanDataBitFromByte(VVVF2Data_M[0], 2);
            }
        }
        break;
    case CAR_6:
        if(bTMSDevComnErrFlag[6]){
            bVVVFBrakeState = false;
        }
        else{
            if(1 == vvvfIndex){
                bVVVFBrakeState = getCanDataBitFromByte(VVVF1Data_MC2[0], 2);
            }
            else{
                bVVVFBrakeState = getCanDataBitFromByte(VVVF2Data_MC2[0], 2);
            }
        }
        break;
    default:
        break;
    }

    emit sig_VVVF_Elec_Brake_State_Update(bVVVFBrakeState);
}

void CanSignalsSlots::car_VVVFData_2_Update(/*CAR_ID carId, int vvvfIndex*/){
    bool bHBOFFFlag = false;
    if(bTMSDevComnErrFlag[2]){
        bHBOFFFlag = false;
    }
    else{
        if((!getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 2) && getCanDataBitFromByte(VVVF1Data_MC1[8], 5)) || (!getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 3) && getCanDataBitFromByte(VVVF2Data_MC1[8], 5))){
            bHBOFFFlag = true;
            goto HB_OFFState;
        }
        else{
            bHBOFFFlag = false;
        }
    }
    if(bTMSDevComnErrFlag[4]){
        bHBOFFFlag = false;
    }
    else{
        if((!getCanDataBitFromByte(SubDev_ComnErr_M[1], 2) && getCanDataBitFromByte(VVVF1Data_M[8], 5)) || (!getCanDataBitFromByte(SubDev_ComnErr_M[1], 3) && getCanDataBitFromByte(VVVF2Data_M[8], 5))){
            bHBOFFFlag = true;
            goto HB_OFFState;
        }
        else{
            bHBOFFFlag = false;
        }
    }
    if(bTMSDevComnErrFlag[7]){
        bHBOFFFlag = false;
    }
    else{
        if((!getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 2) && getCanDataBitFromByte(VVVF1Data_MC2[8], 5)) || (!getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 3) && getCanDataBitFromByte(VVVF2Data_MC2[8], 5))){
            bHBOFFFlag = true;
            goto HB_OFFState;
        }
        else{
            bHBOFFFlag = false;
        }
    }

HB_OFFState:
    if(bHBOFFFlag != bHB_OFF_Flag_Data){
        bHB_OFF_Flag_Data = bHBOFFFlag;
        emit sig_HB_OFF_Update(bHBOFFFlag);
    }
}

void CanSignalsSlots::car_VVVFData_3_Update(/*CAR_ID carId, int vvvfIndex*/){
    //获取车辆速度值
    int tempSpeed[2] = {0};

    bool bInvalidFlag = true;
    if(!bTMSDevComnErrFlag[2] && !getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 2)){
        tempSpeed[0] = 1;
        tempSpeed[1] = VVVF1Data_MC1[14];
        bInvalidFlag = false;
    }
    else if(!bTMSDevComnErrFlag[2] && !getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 3)){
        tempSpeed[0] = 1;
        tempSpeed[1] = VVVF2Data_MC1[14];
        bInvalidFlag = false;
    }
    else if(!bTMSDevComnErrFlag[7] && !getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 2)){
        tempSpeed[0] = 1;
        tempSpeed[1] = VVVF1Data_MC2[14];
        bInvalidFlag = false;
    }
    else if(!bTMSDevComnErrFlag[7] && !getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 3)){
        tempSpeed[0] = 1;
        tempSpeed[1] = VVVF2Data_MC2[14];
        bInvalidFlag = false;
    }
    else if(!bTMSDevComnErrFlag[4] && !getCanDataBitFromByte(SubDev_ComnErr_M[1], 2)){
        tempSpeed[0] = 1;
        tempSpeed[1] = VVVF1Data_M[14];
        bInvalidFlag = false;
    }
    else if(!bTMSDevComnErrFlag[4] && !getCanDataBitFromByte(SubDev_ComnErr_M[1], 3)){
        tempSpeed[0] = 1;
        tempSpeed[1] = VVVF2Data_M[14];
        bInvalidFlag = false;
    }
    else{
        tempSpeed[0] = 0;
        tempSpeed[1] = 0;
        bInvalidFlag = true;
    }

    if(bInvalidFlag){
        carSpeedValue[0] = 0;
        carSpeedValue[1] = 0;
        Car_Data_HMI[7] = 0xFF;
    }
    else{
        float fSpeed = tempSpeed[1] * SPEED_UNIT_VALUE;
        tempSpeed[1] = qRound(fSpeed);
        carSpeedValue[0] = 1;
        if(tempSpeed[1] != carSpeedValue[1]){
            carSpeedValue[1] = tempSpeed[1];
            car_Speed_Door_State_Update();
        }
        Car_Data_HMI[7] = carSpeedValue[1];
    }
}

void CanSignalsSlots::car_VVVFData_4_Update(/*CAR_ID carId, int vvvfIndex*/){
}

void CanSignalsSlots::car_VVVFData_5_Update(CAR_ID carId, int vvvfIndex){
    bool bChangeFlag = false;
    switch (carId) {
    case CAR_1:
        if(1 == vvvfIndex){
            if(VVVF1Data_MC1[32] != m_VVVF1FaultCount[0]){
                m_VVVF1FaultCount[0] = VVVF1Data_MC1[32];
                bChangeFlag = true;
            }
        }
        else{
            if(VVVF2Data_MC1[32] != m_VVVF2FaultCount[0]){
                m_VVVF2FaultCount[0] = VVVF2Data_MC1[32];
                bChangeFlag = true;
            }
        }
        break;
    case CAR_3:
        if(1 == vvvfIndex){
            if(VVVF1Data_M[32] != m_VVVF1FaultCount[1]){
                m_VVVF1FaultCount[1] = VVVF1Data_M[32];
                bChangeFlag = true;
            }
        }
        else{
            if(VVVF2Data_M[32] != m_VVVF2FaultCount[1]){
                m_VVVF2FaultCount[1] = VVVF2Data_M[32];
                bChangeFlag = true;
            }
        }
        break;
    case CAR_6:
        if(1 == vvvfIndex){
            if(VVVF1Data_MC2[32] != m_VVVF1FaultCount[2]){
                m_VVVF1FaultCount[2] = VVVF1Data_MC2[32];
                bChangeFlag = true;
            }
        }
        else{
            if(VVVF2Data_MC2[32] != m_VVVF2FaultCount[2]){
                m_VVVF2FaultCount[2] = VVVF2Data_MC2[32];
                bChangeFlag = true;
            }
        }
        break;
    default:
        bChangeFlag = false;
        break;
    }
    if(bChangeFlag){
        emit sig_FaultRecordCount_Get_Update();
    }
}

void CanSignalsSlots::car_SIVData_1_Update(CAR_ID carId){
    car_SIV_Voltage_Freq_Update(carId);
}

void CanSignalsSlots::car_SIVData_2_Update(CAR_ID carId){
    car_SIV_Voltage_Freq_Update(carId);
}

void CanSignalsSlots::car_SIVData_3_Update(CAR_ID carId){
    bool bChangeFlag = false;
    if(CAR_2 == carId){
        if(SIVData_T[15] != m_SIVFaultCount[0]){
            m_SIVFaultCount[0] = SIVData_T[15];
            bChangeFlag = true;
        }
    }
    else if(CAR_4 == carId){
        if(SIVData_T1[15] != m_SIVFaultCount[1]){
            m_SIVFaultCount[1] = SIVData_T1[15];
            bChangeFlag = true;
        }
    }
    else{
        bChangeFlag = false;
    }
    if(bChangeFlag){
        emit sig_FaultRecordCount_Get_Update();
    }
}

void CanSignalsSlots::car_SubDev_ComnErrData_Update(){
}

//void CanSignalsSlots::car_TMSStateData_CCU_Update(){

//}

void CanSignalsSlots::car_ParaData_7B_HMI_Update(){
    if(ParaData_HMI[0] != carTempValue){
        setDDUConfigInfo(2, 0, ParaData_HMI[0]);
        carTempValue = ParaData_HMI[0];
        emit sig_ACC_SetTempValue_Update();
    }

    bool bCarWheelChangeFalg = false;
    for(int i = 0; i < 6; i++){
        if(m_CarWheel[i] != 840-ParaData_HMI[i+1]){
            m_CarWheel[i] = 840-ParaData_HMI[i+1];
            setDDUConfigInfo(3, i+1, m_CarWheel[i]);
            bCarWheelChangeFalg = true;
        }
    }
    if(bCarWheelChangeFalg){
        emit sig_Car_ParaData_Update(2);
    }
}

void CanSignalsSlots::car_ParaData_8B_HMI_Update(int dataIndex){
    if(0 == dataIndex){
        return ;
    }
    int iASFValue = 0, iASEValue = 0;
    switch (dataIndex) {
    case 1:
        // Car 1
        iASFValue = ParaData_HMI[7]<<8 | ParaData_HMI[8];
        iASEValue = ParaData_HMI[9]<<8 | ParaData_HMI[10];
        if(m_CarASFull[0] != iASFValue && 0 != iASFValue){
            m_CarASFull[0] = iASFValue;
            setDDUConfigInfo(4, 1, iASFValue);
            emit sig_Car_ParaData_Update(3);
        }
        if(m_CarASEmpty[0] != iASEValue && 0 != iASEValue){
            m_CarASEmpty[0] = iASEValue;
            setDDUConfigInfo(5, 1, iASEValue);
            emit sig_Car_ParaData_Update(4);
        }
        //Car 2
        iASFValue = ParaData_HMI[11]<<8 | ParaData_HMI[12];
        iASEValue = ParaData_HMI[13]<<8 | ParaData_HMI[14];
        if(m_CarASFull[1] != iASFValue && 0 != iASFValue){
            m_CarASFull[1] = iASFValue;
            setDDUConfigInfo(4, 2, iASFValue);
            emit sig_Car_ParaData_Update(3);
        }
        if(m_CarASEmpty[1] != iASEValue && 0 != iASEValue){
            m_CarASEmpty[1] = iASEValue;
            setDDUConfigInfo(5, 2, iASEValue);
            emit sig_Car_ParaData_Update(4);
        }
        break;
    case 2:
        // Car 3
        iASFValue = ParaData_HMI[15]<<8 | ParaData_HMI[16];
        iASEValue = ParaData_HMI[17]<<8 | ParaData_HMI[18];
        if(m_CarASFull[2] != iASFValue && 0 != iASFValue){
            m_CarASFull[2] = iASFValue;
            setDDUConfigInfo(4, 3, iASFValue);
            emit sig_Car_ParaData_Update(3);
        }
        if(m_CarASEmpty[2] != iASEValue && 0 != iASEValue){
            m_CarASEmpty[2] = iASEValue;
            setDDUConfigInfo(5, 3, iASEValue);
            emit sig_Car_ParaData_Update(4);
        }
        //Car 4
        iASFValue = ParaData_HMI[19]<<8 | ParaData_HMI[20];
        iASEValue = ParaData_HMI[21]<<8 | ParaData_HMI[22];
        if(m_CarASFull[3] != iASFValue && 0 != iASFValue){
            m_CarASFull[3] = iASFValue;
            setDDUConfigInfo(4, 4, iASFValue);
            emit sig_Car_ParaData_Update(3);
        }
        if(m_CarASEmpty[3] != iASEValue && 0 != iASEValue){
            m_CarASEmpty[3] = iASEValue;
            setDDUConfigInfo(5, 4, iASEValue);
            emit sig_Car_ParaData_Update(4);
        }
        break;
    case 3:
        // Car 5
        iASFValue = ParaData_HMI[23]<<8 | ParaData_HMI[24];
        iASEValue = ParaData_HMI[25]<<8 | ParaData_HMI[26];
        if(m_CarASFull[4] != iASFValue && 0 != iASFValue){
            m_CarASFull[4] = iASFValue;
            setDDUConfigInfo(4, 5, iASFValue);
            emit sig_Car_ParaData_Update(3);
        }
        if(m_CarASEmpty[4] != iASEValue && 0 != iASEValue){
            m_CarASEmpty[4] = iASEValue;
            setDDUConfigInfo(5, 5, iASEValue);
            emit sig_Car_ParaData_Update(4);
        }
        //Car 6
        iASFValue = ParaData_HMI[27]<<8 | ParaData_HMI[28];
        iASEValue = ParaData_HMI[29]<<8 | ParaData_HMI[30];
        if(m_CarASFull[5] != iASFValue && 0 != iASFValue){
            m_CarASFull[5] = iASFValue;
            setDDUConfigInfo(4, 6, iASFValue);
            emit sig_Car_ParaData_Update(3);
        }
        if(m_CarASEmpty[5] != iASEValue && 0 != iASEValue){
            m_CarASEmpty[5] = iASEValue;
            setDDUConfigInfo(5, 6, iASEValue);
            emit sig_Car_ParaData_Update(4);
        }
        break;
    default:
        break;
    }
}

void CanSignalsSlots::car_TempData_HMI_Update(){
    if(1 == TempData_HMI[0]){
        if(carTempValue != TempData_HMI[1]){
            carTempValue = TempData_HMI[1];
            setDDUConfigInfo(2, 0, carTempValue);
            ParaData_HMI[0] = carTempValue;
            canSendParaData1_Update();
            emit sig_ACC_SetTempValue_Update();
        }
    }
}

void CanSignalsSlots::car_TMSDevStartData_Update(){
}

void CanSignalsSlots::car_FaultDataCount_Update(){
    bool bChangeFlag = false;
    for(int i = 0; i < 6; i++){
        if(FltDataCount_ERM[i+1] != m_FaultRecordCount[i]){
            m_FaultRecordCount[i] = FltDataCount_ERM[i+1];
            bChangeFlag = true;
        }
    }
    if(bChangeFlag){
        emit sig_FaultRecordCount_Get_Update();
    }
}

void CanSignalsSlots::car_TramCode_Update(CAR_ID carId){
    int iCarCode = 0;
    if(CAR_NULL == carId){
        return;
    }

    switch (carId) {
    case CAR_1:
        iCarCode = TramCode_MC1[0]<<8 | TramCode_MC1[1];
        break;
    case CAR_2:
        iCarCode = TramCode_T[0]<<8 | TramCode_T[1];
        break;
    case CAR_3:
        iCarCode = TramCode_M[0]<<8 | TramCode_M[1];
        break;
    case CAR_4:
        iCarCode = TramCode_T1[0]<<8 | TramCode_T1[1];
        break;
    case CAR_5:
        iCarCode = TramCode_T2[0]<<8 | TramCode_T2[1];
        break;
    case CAR_6:
        iCarCode = TramCode_MC2[0]<<8 | TramCode_MC2[1];
        break;
    default:
        iCarCode = 0;
        break;
    }
    if(0 != iCarCode && iCarCode != carCodeNum[int(carId-1)]){
        carCodeNum[int(carId-1)] = iCarCode;
        if(CAR_1 == carId){
            CarArriveData_HMI[2] = iCarCode/1000*16 + (iCarCode/100)%10;
            CarArriveData_HMI[3] = (iCarCode%100)%10*16 + iCarCode%1000;
        }
    //    qDebug()<<"Car Tram Code ==  "<<carCodeNum[0] <<"    "<<carCodeNum[1]<<"    "<<carCodeNum[2]<<"    "<<carCodeNum[3]<<"    "<<carCodeNum[4]<<"    "<<carCodeNum[5];
        emit sig_Car_Code_Change();
        emit sig_Car_ParaData_Update(1);
    }
}

void CanSignalsSlots::car_VVVF_BCU_Level_Update(){
    int iState = 0, iValue = 0;
    if(CAR_NO_HEADER == carHeaderFlag){
        if(1 == deviceCarId){
            if(bTMSDevComnErrFlag[2]){
                iState = 0;
                m_CarPWMValue = 0;
            }
            else{
                m_CarPWMValue = DI_Data_MC1[6];
                iValue = DI_Data_MC1[6];
                if(!getCanDataBitFromByte(DI_Data_MC1[2], 3)){
                    iState = 1;
                }
                else if(getCanDataBitFromByte(DI_Data_MC1[2], 2)){
                    iState = 2;
                }
                else if(!getCanDataBitFromByte(DI_Data_MC1[2], 2) && getCanDataBitFromByte(DI_Data_MC1[2], 3) && !getCanDataBitFromByte(DI_Data_MC1[3], 0)){
                    iState = 3;
                }
                else if(getCanDataBitFromByte(DI_Data_MC1[3], 0)){
                    iState = 4;
                }
                else{
                    iState = 0;
                }
            }
//            qDebug()<<"#############   VVVF  BCU Level ::No Header !!!    Car Id == "<<deviceCarId<<", State == "<<iState<<"   Value ==  "<<iValue<< "   DI Data == "<<DI_Data_MC1[6];
        }
        else{
            if(bTMSDevComnErrFlag[7]){
                iState = 0;
                m_CarPWMValue = 0;
            }
            else{
                m_CarPWMValue = DI_Data_MC2[6];
                iValue = DI_Data_MC2[6];
                if(!getCanDataBitFromByte(DI_Data_MC2[2], 3)){
                    iState = 1;
                }
                else if(getCanDataBitFromByte(DI_Data_MC2[2], 2)){
                    iState = 2;
                }
                else if(!getCanDataBitFromByte(DI_Data_MC2[2], 2) && getCanDataBitFromByte(DI_Data_MC2[2], 3) && !getCanDataBitFromByte(DI_Data_MC2[3], 0)){
                    iState = 3;
                }
                else if(getCanDataBitFromByte(DI_Data_MC2[3], 0)){
                    iState = 4;
                }
                else{
                    iState = 0;
                }
            }
//            qDebug()<<"#############   VVVF  BCU Level ::No Header !!!    Car Id == "<<deviceCarId<<", State == "<<iState<<"   Value ==  "<<iValue<<"   DI Data ==  "<<DI_Data_MC2[6];
        }
    }
    else if(CAR_1_HEADER == carHeaderFlag){
        if(bTMSDevComnErrFlag[2]){
            iState = 0;
            m_CarPWMValue = 0;
        }
        else{
            m_CarPWMValue = DI_Data_MC1[6];
            iValue = DI_Data_MC1[6];
            if(!getCanDataBitFromByte(DI_Data_MC1[2], 3)){
                iState = 1;
            }
            else if(getCanDataBitFromByte(DI_Data_MC1[2], 2)){
                iState = 2;
            }
            else if(!getCanDataBitFromByte(DI_Data_MC1[2], 2) && getCanDataBitFromByte(DI_Data_MC1[2], 3) && !getCanDataBitFromByte(DI_Data_MC1[3], 0)){
                iState = 3;
            }
            else if(getCanDataBitFromByte(DI_Data_MC1[3], 0)){
                iState = 4;
            }
            else{
                iState = 0;
            }
        }
//        qDebug()<<"#############   VVVF  BCU Level ::Car 11111  Header !!!    Car Id == "<<deviceCarId<<", State == "<<iState<<"   Value ==  "<<iValue<<"   DI Data ==  "<<DI_Data_MC1[6];
    }
    else if(CAR_6_HEADER == carHeaderFlag){
        if(bTMSDevComnErrFlag[7]){
            iState = 0;
            m_CarPWMValue = 0;
        }
        else{
            m_CarPWMValue = DI_Data_MC2[6];
            iValue = DI_Data_MC2[6];
            if(!getCanDataBitFromByte(DI_Data_MC2[2], 3)){
                iState = 1;
            }
            else if(getCanDataBitFromByte(DI_Data_MC2[2], 2)){
                iState = 2;
            }
            else if(!getCanDataBitFromByte(DI_Data_MC2[2], 2) && getCanDataBitFromByte(DI_Data_MC2[2], 3) && !getCanDataBitFromByte(DI_Data_MC2[3], 0)){
                iState = 3;
            }
            else if(getCanDataBitFromByte(DI_Data_MC2[3], 0)){
                iState = 4;
            }
            else{
                iState = 0;
            }
        }
//        qDebug()<<"#############   VVVF  BCU Level ::Car 66666  Header !!!    Car Id == "<<deviceCarId<<", State == "<<iState<<"   Value ==  "<<iValue<<"   DI Data ==  "<<DI_Data_MC2[6];
    }
    else{
        iState = 0;
        iValue = 0;
    }
    if(iState != mVVVF_BCU_Level_Data[0] || iValue != mVVVF_BCU_Level_Data[1]){
        mVVVF_BCU_Level_Data[0] = iState;
        mVVVF_BCU_Level_Data[1] = iValue;
        emit sig_VVVF_BCU_Level_Update(iState, iValue);
    }
}

void CanSignalsSlots::car_VVVF_Elec_BCU_Update(CAR_ID carId, int vvvfIndex){
    int iState = 0, iValue = 0;
    switch (carId) {
    case CAR_1:
       if(bTMSDevComnErrFlag[2]){
            iState = 0;
        }
        else if(!getCanDataBitFromByte(DI_Data_MC1[1], 5)){
            iState = 1;
        }
        else{
            if(1 == vvvfIndex){
                if(getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 2)){
                    iState = 0;
                }
                else{
                    if(getCanDataBitFromByte(VVVF1Data_MC1[11], 6)){
                        iState = 6;
                    }
                    else{
                        iValue = mVVVFFirstFaultCode[0];
                        if(0 != iValue){
                            iState = 2;
                        }
                        else if(3 == mVVVF_BCU_Level_Data[0]){  //getCanDataBitFromByte(VVVF1Data_MC1[4], 6)){
                            iState = 3;
                            iValue = VVVF1Data_MC1[26];
                        }
                        else if(4 == mVVVF_BCU_Level_Data[0]){ //getCanDataBitFromByte(VVVF1Data_MC1[0], 0) || getCanDataBitFromByte(VVVF1Data_MC1[0], 1)){
                            iState = 4;
                            iValue = VVVF1Data_MC1[26];
                        }
                        else if(2 == mVVVF_BCU_Level_Data[0]){ //getCanDataBitFromByte(VVVF1Data_MC1[0], 2)){
                            iState = 5;
                            iValue = VVVF1Data_MC1[26];
                        }
                        else{
                            iState = 7;
                            iValue = 0;
                        }
                    }
                }
            }
            else{
                if(getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 3)){
                    iState = 0;
                }
                else{
                    if(getCanDataBitFromByte(VVVF2Data_MC1[11], 6)){
                        iState = 6;
                    }
                    else{
                        iValue = mVVVFFirstFaultCode[3];
                        if(0 != iValue){
                            iState = 2;
                        }
                        else if(3 == mVVVF_BCU_Level_Data[0]){ //getCanDataBitFromByte(VVVF2Data_MC1[4], 6)){
                            iState = 3;
                            iValue = VVVF2Data_MC1[26];
                        }
                        else if(4 == mVVVF_BCU_Level_Data[0]){ //getCanDataBitFromByte(VVVF2Data_MC1[0], 0) || getCanDataBitFromByte(VVVF2Data_MC1[0], 1)){
                            iState = 4;
                            iValue = VVVF2Data_MC1[26];
                        }
                        else if(2 == mVVVF_BCU_Level_Data[0]){ //getCanDataBitFromByte(VVVF2Data_MC1[0], 2)){
                            iState = 5;
                            iValue = VVVF2Data_MC1[26];
                        }
                        else{
                            iState = 7;
                            iValue = 0;
                        }
                    }
                }
            }
        }
        break;
    case CAR_3:
        if(bTMSDevComnErrFlag[4]){
            iState = 0;
        }
        else if(!getCanDataBitFromByte(DI_Data_M[1], 4)){
            iState = 1;
        }
        else{
            if(1 == vvvfIndex){
                if(getCanDataBitFromByte(SubDev_ComnErr_M[1], 2)){
                    iState = 0;
                }
                else{
                    if(getCanDataBitFromByte(VVVF1Data_M[11], 6)){
                        iState = 6;
                    }
                    else{
                        iValue = mVVVFFirstFaultCode[1];
                        if(0 != iValue){
                            iState = 2;
                        }
                        else if(3 == mVVVF_BCU_Level_Data[0]){ //getCanDataBitFromByte(VVVF1Data_M[4], 6)){
                            iState = 3;
                            iValue = VVVF1Data_M[26];
                        }
                        else if(4 == mVVVF_BCU_Level_Data[0]){ //getCanDataBitFromByte(VVVF1Data_M[0], 0) || getCanDataBitFromByte(VVVF1Data_M[0], 1)){
                            iState = 4;
                            iValue = VVVF1Data_M[26];
                        }
                        else if(2 == mVVVF_BCU_Level_Data[0]){ //getCanDataBitFromByte(VVVF1Data_M[0], 2)){
                            iState = 5;
                            iValue = VVVF1Data_M[26];
                        }
                        else{
                            iState = 7;
                            iValue = 0;
                        }
                    }
                }
            }
            else{
                if(getCanDataBitFromByte(SubDev_ComnErr_M[1], 3)){
                    iState = 0;
                }
                else{
                    if(getCanDataBitFromByte(VVVF2Data_M[11], 6)){
                        iState = 6;
                    }
                    else{
                        iValue = mVVVFFirstFaultCode[4];
                        if(0 != iValue){
                            iState = 2;
                        }
                        else if(3 == mVVVF_BCU_Level_Data[0]){ //getCanDataBitFromByte(VVVF2Data_M[4], 6)){
                            iState = 3;
                            iValue = VVVF2Data_M[26];
                        }
                        else if(4 == mVVVF_BCU_Level_Data[0]){ //getCanDataBitFromByte(VVVF2Data_M[0], 0) || getCanDataBitFromByte(VVVF2Data_M[0], 1)){
                            iState = 4;
                            iValue = VVVF2Data_M[26];
                        }
                        else if(2 == mVVVF_BCU_Level_Data[0]){ //getCanDataBitFromByte(VVVF2Data_M[0], 2)){
                            iState = 5;
                            iValue = VVVF2Data_M[26];
                        }
                        else{
                            iState = 7;
                            iValue = 0;
                        }
                    }
                }
            }
        }
        break;
    case CAR_6:
        if(bTMSDevComnErrFlag[7]){
            iState = 0;
        }
        else if(!getCanDataBitFromByte(DI_Data_MC2[1], 5)){
            iState = 1;
        }
        else{
            if(1 == vvvfIndex){
                if(getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 2)){
                    iState = 0;
                }
                else{
                    if(getCanDataBitFromByte(VVVF1Data_MC2[11], 6)){
                        iState = 6;
                    }
                    else{
                        iValue = mVVVFFirstFaultCode[2];
                        if(0 != iValue){
                            iState = 2;
                        }
                        else if(3 == mVVVF_BCU_Level_Data[0]){ //getCanDataBitFromByte(VVVF1Data_MC2[4], 6)){
                            iState = 3;
                            iValue = VVVF1Data_MC2[26];
                        }
                        else if(4 == mVVVF_BCU_Level_Data[0]){ //getCanDataBitFromByte(VVVF1Data_MC2[0], 0) || getCanDataBitFromByte(VVVF1Data_MC2[0], 1)){
                            iState = 4;
                            iValue = VVVF1Data_MC2[26];
                        }
                        else if(2 == mVVVF_BCU_Level_Data[0]){ //getCanDataBitFromByte(VVVF1Data_MC2[0], 2)){
                            iState = 5;
                            iValue = VVVF1Data_MC2[26];
                        }
                        else{
                            iState = 7;
                            iValue = 0;
                        }
                    }
                }
            }
            else{
                if(getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 3)){
                    iState = 0;
                }
                else{
                    if(getCanDataBitFromByte(VVVF2Data_MC2[11], 6)){
                        iState = 6;
                    }
                    else{
                        iValue = mVVVFFirstFaultCode[5];
                        if(0 != iValue){
                            iState = 2;
                        }
                        else if(3 == mVVVF_BCU_Level_Data[0]){ //getCanDataBitFromByte(VVVF2Data_MC2[4], 6)){
                            iState = 3;
                            iValue = VVVF2Data_MC2[26];
                        }
                        else if(4 == mVVVF_BCU_Level_Data[0]){ //getCanDataBitFromByte(VVVF2Data_MC2[0], 0) || getCanDataBitFromByte(VVVF2Data_MC2[0], 1)){
                            iState = 4;
                            iValue = VVVF2Data_MC2[26];
                        }
                        else if(2 == mVVVF_BCU_Level_Data[0]){ //getCanDataBitFromByte(VVVF2Data_MC2[0], 2)){
                            iState = 5;
                            iValue = VVVF2Data_MC2[26];
                        }
                        else{
                            iState = 7;
                            iValue = 0;
                        }
                    }
                }
            }
        }
        break;
    default:
        iState = 0;
        iValue = 0;
        break;
    }
    if(2 != iState){
        float fValue = iValue * VVVF_ELE_CURRENT_UNIT_VALUE;
        iValue = qRound(fValue);
    }

    bool bChangeFlag = false;
    switch (carId) {
    case CAR_1:
        if(1 == vvvfIndex){
            if(iState != mVVVF_Elec_BCU_Data[0][0] || iValue != mVVVF_Elec_BCU_Data[0][1]){
                mVVVF_Elec_BCU_Data[0][0] = iState;
                mVVVF_Elec_BCU_Data[0][1] = iValue;
                bChangeFlag = true;
            }
        }
        else{
            if(iState != mVVVF_Elec_BCU_Data[1][0] || iValue != mVVVF_Elec_BCU_Data[1][1]){
                mVVVF_Elec_BCU_Data[1][0] = iState;
                mVVVF_Elec_BCU_Data[1][1] = iValue;
                bChangeFlag = true;
            }
        }
        break;
    case CAR_3:
        if(1 == vvvfIndex){
            if(iState != mVVVF_Elec_BCU_Data[2][0] || iValue != mVVVF_Elec_BCU_Data[2][1]){
                mVVVF_Elec_BCU_Data[2][0] = iState;
                mVVVF_Elec_BCU_Data[2][1] = iValue;
                bChangeFlag = true;
            }
        }
        else{
            if(iState != mVVVF_Elec_BCU_Data[3][0] || iValue != mVVVF_Elec_BCU_Data[3][1]){
                mVVVF_Elec_BCU_Data[3][0] = iState;
                mVVVF_Elec_BCU_Data[3][1] = iValue;
                bChangeFlag = true;
            }
        }
        break;
    case CAR_6:
        if(1 == vvvfIndex){
            if(iState != mVVVF_Elec_BCU_Data[4][0] || iValue != mVVVF_Elec_BCU_Data[4][1]){
                mVVVF_Elec_BCU_Data[4][0] = iState;
                mVVVF_Elec_BCU_Data[4][1] = iValue;
                bChangeFlag = true;
            }
        }
        else{
            if(iState != mVVVF_Elec_BCU_Data[5][0] || iValue != mVVVF_Elec_BCU_Data[5][1]){
                mVVVF_Elec_BCU_Data[5][0] = iState;
                mVVVF_Elec_BCU_Data[5][1] = iValue;
                bChangeFlag = true;
            }
        }
        break;
    default:
        bChangeFlag = false;
        break;
    }
    if(bChangeFlag){
        emit sig_VVVF_Elec_BCU_Update(carId, vvvfIndex, iState, iValue);
    }
}

void CanSignalsSlots::car_BCU_BCKpa_Update(CAR_ID carId){
    int iState = 0, iValue = 0;
    bool bMDUComnFlag = false, bBCUComnFalg = false, bBCUPowerFalg = true, bDIFalg = false;
    int iBCKpa = 0;
    switch (carId) {
    case CAR_1:
        bMDUComnFlag = bTMSDevComnErrFlag[2];
        bBCUComnFalg = getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 1);
//        bBCUPowerFalg = getCanDataBitFromByte(DI_Data_MC1[1], 0);
        bDIFalg = getCanDataBitFromByte(DI_Data_MC1[1], 1);
        iBCKpa = BCUData_MC1[15];
        break;
    case CAR_2:
        bMDUComnFlag = bTMSDevComnErrFlag[3];
        bBCUComnFalg = getCanDataBitFromByte(SubDev_ComnErr_T[1], 1);
//        bBCUPowerFalg = getCanDataBitFromByte(DI_Data_T[1], 0);
        bDIFalg = getCanDataBitFromByte(DI_Data_T[1], 1);
        iBCKpa = BCUData_T[15];
        break;
    case CAR_3:
        bMDUComnFlag = bTMSDevComnErrFlag[4];
        bBCUComnFalg = getCanDataBitFromByte(SubDev_ComnErr_M[1], 1);
//        bBCUPowerFalg = getCanDataBitFromByte(DI_Data_M[0], 2);
        bDIFalg = getCanDataBitFromByte(DI_Data_M[0], 3);
        iBCKpa = BCUData_M[15];
        break;
    case CAR_4:
        bMDUComnFlag = bTMSDevComnErrFlag[5];
        bBCUComnFalg = getCanDataBitFromByte(SubDev_ComnErr_T1[1], 1);
//        bBCUPowerFalg = getCanDataBitFromByte(DI_Data_T1[1], 0);
        bDIFalg = getCanDataBitFromByte(DI_Data_T1[1], 1);
        iBCKpa = BCUData_T1[15];
        break;
    case CAR_5:
        bMDUComnFlag = bTMSDevComnErrFlag[6];
        bBCUComnFalg = getCanDataBitFromByte(SubDev_ComnErr_T2[1], 1);
//        bBCUPowerFalg = getCanDataBitFromByte(DI_Data_T2[0], 5);
        bDIFalg = getCanDataBitFromByte(DI_Data_T2[1], 0);
        iBCKpa = BCUData_T2[15];
        break;
    case CAR_6:
        bMDUComnFlag = bTMSDevComnErrFlag[7];
        bBCUComnFalg = getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 1);
//        bBCUPowerFalg = getCanDataBitFromByte(DI_Data_MC2[1], 0);
        bDIFalg = getCanDataBitFromByte(DI_Data_MC2[1], 1);
        iBCKpa = BCUData_MC2[15];
        break;
    default:
        bMDUComnFlag = true;
        bBCUComnFalg = true;
        bBCUPowerFalg = true;
        bDIFalg = true;
        iBCKpa = 0;
        break;
    }

    if(bMDUComnFlag){
        iState = 0;
        m_BCUBCPress[int(carId-1)] = 0;
    }
    else{
        if(!bBCUPowerFalg){
            iState = 1;
            m_BCUBCPress[int(carId-1)] = iBCKpa*BCU_BC_KPA_UNIT_VALUE;
        }
        else if(bBCUComnFalg){
            iState = 0;
            m_BCUBCPress[int(carId-1)] = 0;
        }
        else{
            m_BCUBCPress[int(carId-1)] = iBCKpa*BCU_BC_KPA_UNIT_VALUE;
            iValue = mBCUFirstFaultCode[int(carId-1)];
            if(0 != iValue){
                iState = 2;
//                qDebug()<<"BCU Fault State ::::  Car Id == "<<carId<<"   Fault Code  ==  "<<iValue;
            }
            else if(bDIFalg){
                iState = 3;
            }
            else{
                iState = 4;
                iValue = iBCKpa*BCU_BC_KPA_UNIT_VALUE;
            }
        }
    }
    if(iState != mBCU_BCKpa_Data[int(carId-1)][0] || iValue != mBCU_BCKpa_Data[int(carId-1)][1]){
        mBCU_BCKpa_Data[int(carId-1)][0] = iState;
        mBCU_BCKpa_Data[int(carId-1)][1] = iValue;
        emit sig_BCU_BC_Kpa_Update(carId, iState, iValue);
    }
}

void CanSignalsSlots::car_SIV_Voltage_Freq_Update(CAR_ID carId){
    int iState = 0, iValue1 = 0, iValue2 = 0;
    bool bMDUComnFlag = false, bSIVComnFalg = false, bDIPowerFalg = false;
    int iTempValue1 = 0, iTempValue2 = 0;
    switch (carId) {
    case CAR_2:
        bMDUComnFlag = bTMSDevComnErrFlag[3];
        bSIVComnFalg = getCanDataBitFromByte(SubDev_ComnErr_T[1], 2);
        bDIPowerFalg = getCanDataBitFromByte(DI_Data_T[0], 4);
        iTempValue1 = SIVData_T[8];
        iTempValue2 = SIVData_T[12];
        break;
    case CAR_4:
        bMDUComnFlag = bTMSDevComnErrFlag[5];
        bSIVComnFalg = getCanDataBitFromByte(SubDev_ComnErr_T1[1], 2);
        bDIPowerFalg = getCanDataBitFromByte(DI_Data_T1[0], 5);
        iTempValue1 = SIVData_T1[8];
        iTempValue2 = SIVData_T1[12];
        break;
    default:
        break;
    }
    if(bMDUComnFlag){
        iState = 0;
    }
    else{
        if(!bDIPowerFalg){
            iState = 1;
        }
        else if(bSIVComnFalg){
            iState = 0;
        }
        else{
            iValue1 = mSIVFirstFaultCode[int(carId/2-1)];
            if(0 != iValue1){
                iState = 2;
            }
            else{
                iState = 3;
                float fValue1 = iTempValue1*SIV_VOLTAGE_UNIT_VALUE;
                float fValue2 = iTempValue2*SIV_FREQ_UNIT_VALUE;
                iValue1 = qRound(fValue1);
                iValue2 = qRound(fValue2);
            }
        }
    }
    bool bChangeFlag = false;
    switch (carId) {
    case CAR_2:
        if(iState != mSIV_State_Data[0][0] || iValue1 != mSIV_State_Data[0][1] || iValue2 != mSIV_State_Data[0][2]){
            mSIV_State_Data[0][0] = iState;
            mSIV_State_Data[0][1] = iValue1;
            mSIV_State_Data[0][2] = iValue2;
            bChangeFlag = true;
        }
        break;
    case CAR_4:
        if(iState != mSIV_State_Data[1][0] || iValue1 != mSIV_State_Data[1][1] || iValue2 != mSIV_State_Data[1][2]){
            mSIV_State_Data[1][0] = iState;
            mSIV_State_Data[1][1] = iValue1;
            mSIV_State_Data[1][2] = iValue2;
            bChangeFlag = true;
        }
        break;
    default:
        bChangeFlag = false;
        break;
    }
    if(bChangeFlag){
        emit sig_SIV_Voltage_Freq_Update(carId, iState, iValue1, iValue2);
    }
}

void CanSignalsSlots::car_Expand_Elec_Update(){
    SIV_TX_DIRECTION sivDirection = SIV_NONE;
    if(getCanDataBitFromByte(DI_Data_T1[0], 1)){
        if(getCanDataBitFromByte(DI_Data_T[0], 3) && !getCanDataBitFromByte(DI_Data_T1[0], 4)){
            sivDirection = SIV_2_TO_4;
        }
        else if(!getCanDataBitFromByte(DI_Data_T[0], 3) && getCanDataBitFromByte(DI_Data_T1[0], 1)){
            sivDirection = SIV_4_TO_2;
        }
        else{
            sivDirection = SIV_NONE;
        }
    }
    if(mExpand_Elec_Data != int(sivDirection)){
        mExpand_Elec_Data = int(sivDirection);
        emit sig_SIV_Direction_Change(sivDirection);
    }
}

void CanSignalsSlots::car_Direction_Update(){
    emit sig_Car_Direction_Change();
    emit sig_VVVF_Elec_BCU_Update(CAR_1, 1, mVVVF_Elec_BCU_Data[0][0], mVVVF_Elec_BCU_Data[0][1]);
    emit sig_VVVF_Elec_BCU_Update(CAR_1, 2, mVVVF_Elec_BCU_Data[1][0], mVVVF_Elec_BCU_Data[1][1]);
    emit sig_VVVF_Elec_BCU_Update(CAR_3, 1, mVVVF_Elec_BCU_Data[2][0], mVVVF_Elec_BCU_Data[2][1]);
    emit sig_VVVF_Elec_BCU_Update(CAR_3, 2, mVVVF_Elec_BCU_Data[3][0], mVVVF_Elec_BCU_Data[3][1]);
    emit sig_VVVF_Elec_BCU_Update(CAR_6, 1, mVVVF_Elec_BCU_Data[4][0], mVVVF_Elec_BCU_Data[4][1]);
    emit sig_VVVF_Elec_BCU_Update(CAR_6, 2, mVVVF_Elec_BCU_Data[5][0], mVVVF_Elec_BCU_Data[5][1]);

    for(int i = 0; i < 6; i++){
        emit sig_BCU_BC_Kpa_Update(CAR_ID(i+1), mBCU_BCKpa_Data[i][0], mBCU_BCKpa_Data[i][1]);
        emit sig_ACC_Temp_Update(CAR_ID(i+1), mACC_Temp_Data[i][0], mACC_Temp_Data[i][1], mACC_Temp_Data[i][2]);
        emit sig_ACCData_Change(CAR_ID(i+1), mACC_DevInfo_Data[i][0], mACC_DevInfo_Data[i][1], mACC_DevInfo_Data[i][2], mACC_DevInfo_Data[i][3], mACC_DevInfo_Data[i][4]);
        emit sig_Load_Rate_Update(CAR_ID(i+1), mBCU_LoadRate_Data[i][0], mBCU_LoadRate_Data[i][1]);
        emit sig_Urgent_Alert_Update(CAR_ID(i+1), mPIS_Urgent_Alert_Data[i]);
    }
    for(int i = 0; i < 8; i++){
        emit sig_Door_State_Change(CAR_1, DOOR_ID(i+1), RUN_DOOR_STATE(doorMC1State[i]));
        emit sig_Door_State_Change(CAR_2, DOOR_ID(i+1), RUN_DOOR_STATE(doorTState[i]));
        emit sig_Door_State_Change(CAR_3, DOOR_ID(i+1), RUN_DOOR_STATE(doorMState[i]));
        emit sig_Door_State_Change(CAR_4, DOOR_ID(i+1), RUN_DOOR_STATE(doorT1State[i]));
        emit sig_Door_State_Change(CAR_5, DOOR_ID(i+1), RUN_DOOR_STATE(doorT2State[i]));
        emit sig_Door_State_Change(CAR_6, DOOR_ID(i+1), RUN_DOOR_STATE(doorMC2State[i]));
    }
    emit sig_AirPump_State_Change(CAR_1, getCanDataBitFromByte(DI_Data_MC1[1], 4));
    emit sig_AirPump_State_Change(CAR_6, getCanDataBitFromByte(DI_Data_MC2[1], 4));
    emit sig_SIV_Voltage_Freq_Update(CAR_2, mSIV_State_Data[0][0], mSIV_State_Data[0][1], mSIV_State_Data[0][2]);
    emit sig_SIV_Voltage_Freq_Update(CAR_4, mSIV_State_Data[1][0], mSIV_State_Data[1][1], mSIV_State_Data[1][2]);
    emit sig_SIV_Direction_Change(SIV_TX_DIRECTION(mExpand_Elec_Data));
    emit sig_BHB_BLB_State_Change(CAR_2, getCanDataBitFromByte(DI_Data_T[0], 0), getCanDataBitFromByte(DI_Data_T[0], 1));
    emit sig_BHB_BLB_State_Change(CAR_3, getCanDataBitFromByte(DI_Data_M[0], 0), getCanDataBitFromByte(DI_Data_M[0], 1));
    emit sig_UrgentShort(CAR_1, getCanDataBitFromByte(DI_Data_MC1[1], 3));
    emit sig_UrgentShort(CAR_6, getCanDataBitFromByte(DI_Data_MC2[1], 3));
    emit sig_BrakeIsolateDoor_Change(CAR_1, getCanDataBitFromByte(DI_Data_MC1[1], 2));
    emit sig_BrakeIsolateDoor_Change(CAR_2, getCanDataBitFromByte(DI_Data_T[1], 2));
    emit sig_BrakeIsolateDoor_Change(CAR_3, getCanDataBitFromByte(DI_Data_M[0], 4));
    emit sig_BrakeIsolateDoor_Change(CAR_4, getCanDataBitFromByte(DI_Data_T1[1], 2));
    emit sig_BrakeIsolateDoor_Change(CAR_5, getCanDataBitFromByte(DI_Data_T2[1], 1));
    emit sig_BrakeIsolateDoor_Change(CAR_6, getCanDataBitFromByte(DI_Data_MC2[1], 2));
}

void CanSignalsSlots::car_Urgent_Alert_Update(){
    bool bUrgentAlertFalg[6] = {false};
    if(1 == PISData_MC1[10] || 3 == PISData_MC1[10]){
        for(int i = 0; i < 6; i++){
            bUrgentAlertFalg[i] = getCanDataBitFromByte(PISData_MC1[1], 7-i);
        }
    }
    else if(2 == PISData_MC1[10] && 1 == PISData_MC2[10]){
        for(int i = 0; i < 6; i++){
            bUrgentAlertFalg[i] = getCanDataBitFromByte(PISData_MC2[1], 7-i);
        }
    }
    else{
        for(int i = 0; i < 6; i++){
            bUrgentAlertFalg[i] = false;
        }
    }

    for(int i = 0; i < 6; i++){
        if(bUrgentAlertFalg[i] != mPIS_Urgent_Alert_Data[i]){
            mPIS_Urgent_Alert_Data[i] = bUrgentAlertFalg[i];
            emit sig_Urgent_Alert_Update(CAR_ID(i+1), bUrgentAlertFalg[i]);
        }
    }
}

void CanSignalsSlots::car_ACC_SetInfo_Update(CAR_ID carId){
    bool bMDUComnErrFlag = false, bACCComnErrFlag = false;
    int iState = 0, acc1Mode = 0, acc2Mode = 0, acc1Err = 0, acc2Err = 0;
    switch (carId) {
    case CAR_1:
        bMDUComnErrFlag = bTMSDevComnErrFlag[2];
        bACCComnErrFlag = getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 0);
        break;
    case CAR_2:
        bMDUComnErrFlag = bTMSDevComnErrFlag[3];
        bACCComnErrFlag = getCanDataBitFromByte(SubDev_ComnErr_T[1], 0);
        break;
    case CAR_3:
        bMDUComnErrFlag = bTMSDevComnErrFlag[4];
        bACCComnErrFlag = getCanDataBitFromByte(SubDev_ComnErr_M[1], 0);
        break;
    case CAR_4:
        bMDUComnErrFlag = bTMSDevComnErrFlag[5];
        bACCComnErrFlag = getCanDataBitFromByte(SubDev_ComnErr_T1[1], 0);
        break;
    case CAR_5:
        bMDUComnErrFlag = bTMSDevComnErrFlag[6];
        bACCComnErrFlag = getCanDataBitFromByte(SubDev_ComnErr_T2[1], 0);
        break;
    case CAR_6:
        bMDUComnErrFlag = bTMSDevComnErrFlag[7];
        bACCComnErrFlag = getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 0);
        break;
    default:
        bMDUComnErrFlag = false;
        bACCComnErrFlag = false;
        break;
    }
    if(bMDUComnErrFlag || bACCComnErrFlag){
        iState = 0;
    }
    else{
        iState = 1;
        acc1Mode = getACCWorkModel(carId, 1);
        acc2Mode = getACCWorkModel(carId, 2);
        acc1Err = getACCBreakdownCode(carId, 1);
        acc2Err = getACCBreakdownCode(carId, 2);
    }
    if(iState != mACC_DevInfo_Data[int(carId-1)][0] || acc1Mode != mACC_DevInfo_Data[int(carId-1)][1] || acc2Mode != mACC_DevInfo_Data[int(carId-1)][2] || acc1Err != mACC_DevInfo_Data[int(carId-1)][3] || acc2Err != mACC_DevInfo_Data[int(carId-1)][4]){
        mACC_DevInfo_Data[int(carId-1)][0] = iState;
        mACC_DevInfo_Data[int(carId-1)][1] = acc1Mode;
        mACC_DevInfo_Data[int(carId-1)][2] = acc2Mode;
        mACC_DevInfo_Data[int(carId-1)][3] = acc1Err;
        mACC_DevInfo_Data[int(carId-1)][4] = acc2Err;
        emit sig_ACCData_Change(carId, iState, acc1Mode, acc2Mode, acc1Err, acc2Err);
    }
}

void CanSignalsSlots::car_ACC_Temp_Update(CAR_ID carId){
    int iState = 0, iValue1 = 0, iValue2 = 0;
    bool bMDUComnFlag = false, bACCComnFalg = false;
    int accTempData = 0;
    switch (carId) {
    case CAR_1:
        bMDUComnFlag = bTMSDevComnErrFlag[2];
        bACCComnFalg = getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 0);
        accTempData = ACCData_MC1[6];
        break;
    case CAR_2:
        bMDUComnFlag = bTMSDevComnErrFlag[3];
        bACCComnFalg = getCanDataBitFromByte(SubDev_ComnErr_T[1], 0);
        accTempData = ACCData_T[6];
        break;
    case CAR_3:
        bMDUComnFlag = bTMSDevComnErrFlag[4];
        bACCComnFalg = getCanDataBitFromByte(SubDev_ComnErr_M[1], 0);
        accTempData = ACCData_M[6];
        break;
    case CAR_4:
        bMDUComnFlag = bTMSDevComnErrFlag[5];
        bACCComnFalg = getCanDataBitFromByte(SubDev_ComnErr_T1[1], 0);
        accTempData = ACCData_T1[6];
        break;
    case CAR_5:
        bMDUComnFlag = bTMSDevComnErrFlag[6];
        bACCComnFalg = getCanDataBitFromByte(SubDev_ComnErr_T2[1], 0);
        accTempData = ACCData_T2[6];
        break;
    case CAR_6:
        bMDUComnFlag = bTMSDevComnErrFlag[7];
        bACCComnFalg = getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 0);
        accTempData = ACCData_MC2[6];
        break;
    default:
        bMDUComnFlag = false;
        bACCComnFalg = false;
        break;
    }
    if(bMDUComnFlag){
        iState = 0;
    }
    else{
        if(bACCComnFalg){
            iState = 0;
        }
        else{
            iValue2 = mACCFirstFaultCode[int(carId-1)];
            if(0 != iValue2){
                iState = 1;
                iValue1 = accTempData-0xD0+5;
            }
            else{
                iState = 2;
                iValue1 = accTempData-0xD0+5;
                iValue2 = 0;
            }
        }
    }

    if(mACC_Temp_Data[int(carId-1)][0] != iState || mACC_Temp_Data[int(carId-1)][1] != iValue1 || mACC_Temp_Data[int(carId-1)][2] != iValue2){
        mACC_Temp_Data[int(carId-1)][0] = iState;
        mACC_Temp_Data[int(carId-1)][1] = iValue1;
        mACC_Temp_Data[int(carId-1)][2] = iValue2;
        emit sig_ACC_Temp_Update(carId, iState, iValue1, iValue2);
    }
}

void CanSignalsSlots::car_ACC_Ctrl_Mode_Update(){
    int accCtrlMode = 0;
    if(!bTMSDevComnErrFlag[2] && !getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 0)){
        accCtrlMode = getACCCtrlMode(ACCData_MC1[8]);
    }
    else if(!bTMSDevComnErrFlag[3] && !getCanDataBitFromByte(SubDev_ComnErr_T[1], 0)){
        accCtrlMode = getACCCtrlMode(ACCData_T[8]);
    }
    else if(!bTMSDevComnErrFlag[4] && !getCanDataBitFromByte(SubDev_ComnErr_M[1], 0)){
        accCtrlMode = getACCCtrlMode(ACCData_M[8]);
    }
    else if(!bTMSDevComnErrFlag[5] && !getCanDataBitFromByte(SubDev_ComnErr_T1[1], 0)){
        accCtrlMode = getACCCtrlMode(ACCData_T1[8]);
    }
    else if(!bTMSDevComnErrFlag[6] && !getCanDataBitFromByte(SubDev_ComnErr_T2[1], 0)){
        accCtrlMode = getACCCtrlMode(ACCData_T2[8]);
    }
    else if(!bTMSDevComnErrFlag[7] && !getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 0)){
        accCtrlMode = getACCCtrlMode(ACCData_MC2[8]);
    }
    else{
        accCtrlMode = 0;
    }
    if(accCtrlMode != mACCCtrlMode){
        mACCCtrlMode = accCtrlMode;
        emit sig_ACCCtrlMode(accCtrlMode);
    }
}

void CanSignalsSlots::car_Load_Rate_Update(CAR_ID carId){
    int iState = 0, iValue = 255, iFaultValue = 0, iASValue = 0, iAS1Value = 0, iAS2Value = 0, iASAW0Value = 0, iASAW3Value = 0;
    bool bMDUComnFlag = false, bBCUComnFalg = false, bBCUPowerFalg = true, bAS1Flag = false, bAS2Flag = false;
    switch (carId) {
    case CAR_1:
        bMDUComnFlag = bTMSDevComnErrFlag[2];
        bBCUComnFalg = getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 1);
//        bBCUPowerFalg = getCanDataBitFromByte(DI_Data_MC1[1], 0);
        bAS1Flag = getCanDataBitFromByte(BCUData_MC1[3], 2);
        bAS2Flag = getCanDataBitFromByte(BCUData_MC1[3], 1);
        iAS1Value = BCUData_MC1[6];
        iAS2Value = BCUData_MC1[7];
        break;
    case CAR_2:
        bMDUComnFlag = bTMSDevComnErrFlag[3];
        bBCUComnFalg = getCanDataBitFromByte(SubDev_ComnErr_T[1], 1);
//        bBCUPowerFalg = getCanDataBitFromByte(DI_Data_T[1], 0);
        bAS1Flag = getCanDataBitFromByte(BCUData_T[3], 2);
        bAS2Flag = getCanDataBitFromByte(BCUData_T[3], 1);
        iAS1Value = BCUData_T[6];
        iAS2Value = BCUData_T[7];
        break;
    case CAR_3:
        bMDUComnFlag = bTMSDevComnErrFlag[4];
        bBCUComnFalg = getCanDataBitFromByte(SubDev_ComnErr_M[1], 1);
//        bBCUPowerFalg = getCanDataBitFromByte(DI_Data_M[0], 2);
        bAS1Flag = getCanDataBitFromByte(BCUData_M[3], 2);
        bAS2Flag = getCanDataBitFromByte(BCUData_M[3], 1);
        iAS1Value = BCUData_M[6];
        iAS2Value = BCUData_M[7];
        break;
    case CAR_4:
        bMDUComnFlag = bTMSDevComnErrFlag[5];
        bBCUComnFalg = getCanDataBitFromByte(SubDev_ComnErr_T1[1], 1);
//        bBCUPowerFalg = getCanDataBitFromByte(DI_Data_T1[1], 0);
        bAS1Flag = getCanDataBitFromByte(BCUData_T1[3], 2);
        bAS2Flag = getCanDataBitFromByte(BCUData_T1[3], 1);
        iAS1Value = BCUData_T1[6];
        iAS2Value = BCUData_T1[7];
        break;
    case CAR_5:
        bMDUComnFlag = bTMSDevComnErrFlag[6];
        bBCUComnFalg = getCanDataBitFromByte(SubDev_ComnErr_T2[1], 1);
//        bBCUPowerFalg = getCanDataBitFromByte(DI_Data_T2[0], 5);
        bAS1Flag = getCanDataBitFromByte(BCUData_T2[3], 2);
        bAS2Flag = getCanDataBitFromByte(BCUData_T2[3], 1);
        iAS1Value = BCUData_T2[6];
        iAS2Value = BCUData_T2[7];
        break;
    case CAR_6:
        bMDUComnFlag = bTMSDevComnErrFlag[7];
        bBCUComnFalg = getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 1);
//        bBCUPowerFalg = getCanDataBitFromByte(DI_Data_MC2[1], 0);
        bAS1Flag = getCanDataBitFromByte(BCUData_MC2[3], 2);
        bAS2Flag = getCanDataBitFromByte(BCUData_MC2[3], 1);
        iAS1Value = BCUData_MC2[6];
        iAS2Value = BCUData_MC2[7];
        break;
    default:
        break;
    }

    if(bMDUComnFlag){
        iState = 0;
        m_BCULoadRate[int(carId-1)] = 128;
        m_BCUASPress[int(carId-1)] = 0;
    }
    else{
        if(!bBCUPowerFalg){
            iState = 1;
            m_BCULoadRate[int(carId-1)] = 128;
            m_BCUASPress[int(carId-1)] = (iAS1Value+iAS2Value)/2*BCU_AS_KPA_UNIT_VALUE;
        }
        else if(bBCUComnFalg){
            iState = 0;
            m_BCULoadRate[int(carId-1)] = 128;
            m_BCUASPress[int(carId-1)] = 0;
        }
        else{
            m_BCUASPress[int(carId-1)] = (iAS1Value+iAS2Value)/2*BCU_AS_KPA_UNIT_VALUE;
            iFaultValue = mBCUFirstFaultCode[int(carId-1)];
            if(0 != iFaultValue){
                iState = 2;
            }
            else{
                iState = 3;
            }
        }
    }
    if(2 == iState || 3 == iState){
        bool bValidFlag = true;
        iASAW0Value = m_CarASEmpty[int(carId-1)];
        iASAW3Value = m_CarASFull[int(carId-1)];
        if(bAS1Flag && !bAS2Flag){
            iASValue = iAS2Value;
            bValidFlag = true;
        }
        else if(!bAS1Flag && bAS2Flag){
            iASValue = iAS1Value;
            bValidFlag = true;
        }
        else if(!bAS1Flag && !bAS2Flag){
            iASValue = (iAS1Value + iAS2Value)/2;
            bValidFlag = true;
        }
        else{
            iValue = 255;
            bValidFlag = false;
        }
        if(bValidFlag && iASAW3Value != iASAW0Value){
            iASValue = iASValue * BCU_AS_KPA_UNIT_VALUE;
            float fTempValue = 128.0 * (iASValue - iASAW0Value) / ((iASAW3Value - iASAW0Value)*1.0);
            iValue = qRound(fTempValue);
            m_BCULoadRate[int(carId-1)] = iValue;
        }
        else{
            m_BCULoadRate[int(carId-1)] = 128;
        }
    }
    if(2 == iState){
        iValue = iFaultValue;
    }

    if(iState != mBCU_LoadRate_Data[int(carId-1)][0] || iValue != mBCU_LoadRate_Data[int(carId-1)][1]){
        mBCU_LoadRate_Data[int(carId-1)][0] = iState;
        mBCU_LoadRate_Data[int(carId-1)][1] = iValue;
        emit sig_Load_Rate_Update(carId, iState, iValue);
    }
}

void CanSignalsSlots::car_Load_Rate_AVG_Update(){
    int tempLoadRate = 0;
    for(int i = 0; i < 6; i++){
        tempLoadRate += m_BCULoadRate[i];
    }
    float fLoadRate = tempLoadRate/6;
    tempLoadRate = qRound(fLoadRate);
    Car_Data_HMI[5] = tempLoadRate;
}

void CanSignalsSlots::car_TotalWeight_Update(){
    u_int16_t tempTotalWeight = 0;
    u_int16_t tempCarWeight[6] = {0};
    if(!bTMSDevComnErrFlag[2] && !getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 1)){
        tempCarWeight[0] = BCUData_MC1[17];
    }
    else{
        tempCarWeight[0] = m_CarASFull[0];
    }
    if(!bTMSDevComnErrFlag[3] && !getCanDataBitFromByte(SubDev_ComnErr_T[1], 1)){
        tempCarWeight[1] = BCUData_T[17];
    }
    else{
        tempCarWeight[1] = m_CarASFull[1];
    }
    if(!bTMSDevComnErrFlag[4] && !getCanDataBitFromByte(SubDev_ComnErr_M[1], 1)){
        tempCarWeight[2] = BCUData_M[17];
    }
    else{
        tempCarWeight[2] = m_CarASFull[2];
    }
    if(!bTMSDevComnErrFlag[5] && !getCanDataBitFromByte(SubDev_ComnErr_T1[1], 1)){
        tempCarWeight[3] = BCUData_T1[17];
    }
    else{
        tempCarWeight[3] = m_CarASFull[3];
    }
    if(!bTMSDevComnErrFlag[6] && !getCanDataBitFromByte(SubDev_ComnErr_T2[1], 1)){
        tempCarWeight[4] = BCUData_T2[17];
    }
    else{
        tempCarWeight[4] = m_CarASFull[4];
    }
    if(!bTMSDevComnErrFlag[7] && !getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 1)){
        tempCarWeight[5] = BCUData_MC2[17];
    }
    else{
        tempCarWeight[5] = m_CarASFull[5];
    }
    float fTotalWeight = 0;
    for(int i = 0; i < 6; i++){
        tempTotalWeight += tempCarWeight[i];
    }
    fTotalWeight = tempTotalWeight * TOTAL_WEIGHT_UNIT_VALUE;
    tempTotalWeight = qRound(fTotalWeight)+15*6;
    Car_Data_HMI[3] = tempTotalWeight >> 8;
    Car_Data_HMI[4] = tempTotalWeight << 8 >> 8;
//    qDebug()<<"%%%%%%%%%%%%%%%   Car Total Weight ==  "<<tempTotalWeight  <<"  High Value  ==  "<<Car_Data_HMI[3] << "  Low Value  ==  "<<Car_Data_HMI[4];
}

void CanSignalsSlots::car_DoorOperation_Update(CAR_ID carId){
    u_int8_t tempDIDoorData = 0;
    if(CAR_1 == carId){
        tempDIDoorData = DI_Data_MC1[0] & 0xFC;
        if(tempDIDoorData != u8DoorOperationData[0]){
            u8DoorOperationData[0] = tempDIDoorData;

            emit sig_Car_DIDoorOperation_Update(carId);
        }
    }
    else if(CAR_6 == carId){
        tempDIDoorData = DI_Data_MC2[0] & 0xFC;
        if(tempDIDoorData != u8DoorOperationData[1]){
            u8DoorOperationData[1] = tempDIDoorData;

            emit sig_Car_DIDoorOperation_Update(carId);
        }
    }
    else{
        // do nothing
    }

    bool bLeftDoorData[2] = {false}, bRightDoorData[2] = {false};
    if(CAR_1_HEADER == carHeaderFlag && CAR_1 == carId){
        for(int i = 0; i < 2; i++){
            bLeftDoorData[i] = getCanDataBitFromByte(DI_Data_MC1[0], i);
            bRightDoorData[i] = getCanDataBitFromByte(DI_Data_MC1[0], i+2);
        }
    }
    else if(CAR_6_HEADER == carHeaderFlag && CAR_6 == carId){
        for(int i = 0; i < 2; i++){
            bLeftDoorData[i] = getCanDataBitFromByte(DI_Data_MC2[0], i);
            bRightDoorData[i] = getCanDataBitFromByte(DI_Data_MC2[0], i+2);
        }
    }
    else{
        return ;
    }
    if(bLeftDoorData[1]){
        emit sig_Car_DoorOperation_Update(1, 2);
    }
    else if(bLeftDoorData[0] && !bLeftDoorData[1]){
        emit sig_Car_DoorOperation_Update(1, 1);
    }
    else{
        //do nothing
    }

    if(bRightDoorData[1]){
        emit sig_Car_DoorOperation_Update(2, 2);
    }
    else if(bRightDoorData[0] && !bRightDoorData[1]){
        emit sig_Car_DoorOperation_Update(2, 1);
    }
    else{
        //do nothing
    }
}

void CanSignalsSlots::car_Door_OpenClose_State_Update(){
    int tempDoorState = 255;
    bool bLeftOpenFlag = false;
    if((!bTMSDevComnErrFlag[2] && getCanDataBitFromByte(DI_Data_MC1[4], 2)) || (!bTMSDevComnErrFlag[7] && getCanDataBitFromByte(DI_Data_MC2[4], 2))){
        tempDoorState = 0;
    }
    else{
        if(5==doorMC1State[1] && 5==doorMC1State[3] && 5==doorMC1State[5] && 5==doorMC1State[7] && 5==doorTState[1] && 5==doorTState[3] && 5==doorTState[5] && 5==doorTState[7] &&
           5==doorMState[1] && 5==doorMState[3] && 5==doorMState[5] && 5==doorMState[7] && 5==doorT1State[6] && 5==doorT1State[4] && 5==doorT1State[2] && 5==doorT1State[0] &&
           5==doorT2State[6] && 5==doorT2State[4] && 5==doorT2State[2] && 5==doorT2State[0] && 5==doorMC2State[6] && 5==doorMC2State[4] && 5==doorMC2State[2] && 5==doorMC2State[0]){
            tempDoorState = 1;
            bLeftOpenFlag = true;
        }
        if(5==doorMC1State[0] && 5==doorMC1State[2] && 5==doorMC1State[4] && 5==doorMC1State[6] && 5==doorTState[0] && 5==doorTState[2] && 5==doorTState[4] && 5==doorTState[6] &&
           5==doorMState[0] && 5==doorMState[2] && 5==doorMState[4] && 5==doorMState[6] && 5==doorT1State[7] && 5==doorT1State[5] && 5==doorT1State[3] && 5==doorT1State[1] &&
           5==doorT2State[7] && 5==doorT2State[5] && 5==doorT2State[3] && 5==doorT2State[1] && 5==doorMC2State[7] && 5==doorMC2State[5] && 5==doorMC2State[3] && 5==doorMC2State[1]){
            if(bLeftOpenFlag){
                tempDoorState = 3;
            }
            else{
                tempDoorState = 2;
            }
        }
    }
    if(masterHMIFlag){
        if(0 == tempDoorState){
            CarArriveData_HMI[0] = setCanDataBitFromByte(CarArriveData_HMI[0], 7, 1);
        }
        else{
            CarArriveData_HMI[0] = setCanDataBitFromByte(CarArriveData_HMI[0], 7, 0);
        }
        canSendCarArriveData1_Update();
        Car_Data_HMI[2] = tempDoorState;
    }
    car_Speed_Door_State_Update();
}

void CanSignalsSlots::car_BrakePower_Up_Update(){
    if(!getCanDataBitFromByte(DI_Data_MC1[1], 0) || !getCanDataBitFromByte(DI_Data_T[1], 0) || !getCanDataBitFromByte(DI_Data_M[0], 2) || !getCanDataBitFromByte(DI_Data_T1[1], 0) || !getCanDataBitFromByte(DI_Data_T2[0], 5) || !getCanDataBitFromByte(DI_Data_MC2[1], 0)){
        Car_Data_HMI[6] = 0xFF;
    }
    else{
        Car_Data_HMI[6] = 0;
    }
}

void CanSignalsSlots::car_Speed_Door_State_Update(){
    if(0 == carSpeedValue[1] && (1 == Car_Data_HMI[2] || 2 == Car_Data_HMI[2] || 3 == Car_Data_HMI[2])){
        if(!bCarRunStopFlag){
            bCarRunStopFlag = true;
            emit sig_Speed_Door_State_Update(1);
        }
    }
    else if(carSpeedValue[1] > 0 && 0 == Car_Data_HMI[2]){
        if(bCarRunStopFlag){
            bCarRunStopFlag = false;
            emit sig_Speed_Door_State_Update(2);
        }
    }
    else{
        //do nothing
    }
}

void CanSignalsSlots::car_FaultInfo_ERM_Update(int mERMFlag){
    if(!bNowShowERMFaultFlag){
        return ;
    }
    qDebug()<<"#######  HMI Receive ERM Fault Info, Car Id == "<< FaultInfo_ERM[0] <<"    Fault Id == "<<FaultInfo_ERM[2]<< "    ERM Id == "<<mERMFlag;
    qDebug()<<"$$   HMI Receive Fault Info:  == "<< FaultInfo_ERM[0]<<" , "<<FaultInfo_ERM[1]<<" , "<<FaultInfo_ERM[2]<<" , "<<FaultInfo_ERM[13]<<" , "<<FaultInfo_ERM[14]<<" , "<<FaultInfo_ERM[16];
    CAR_ID carId = CAR_ID(FaultInfo_ERM[0]);

    int iFaultFlag = FaultInfo_ERM[1];
    int iFaultId = FaultInfo_ERM[2];
    QString strDate = ((FaultInfo_ERM[3] < 10)?QString("0%1").arg(FaultInfo_ERM[3]):QString("%1").arg(FaultInfo_ERM[3]))+"-"+((FaultInfo_ERM[4] < 10)?QString("0%1").arg(FaultInfo_ERM[4]):QString("%1").arg(FaultInfo_ERM[4]))+"-"+((FaultInfo_ERM[5] < 10)?QString("0%1").arg(FaultInfo_ERM[5]):QString("%1").arg(FaultInfo_ERM[5]));
    QString strTime = ((FaultInfo_ERM[6] < 10)?QString("0%1").arg(FaultInfo_ERM[6]):QString("%1").arg(FaultInfo_ERM[6]))+":"+((FaultInfo_ERM[7] < 10)?QString("0%1").arg(FaultInfo_ERM[7]):QString("%1").arg(FaultInfo_ERM[7]))+":"+((FaultInfo_ERM[8] < 10)?QString("0%1").arg(FaultInfo_ERM[8]):QString("%1").arg(FaultInfo_ERM[8]));
    int iStationId = FaultInfo_ERM[9]<<8|FaultInfo_ERM[10];
    int iDistance = FaultInfo_ERM[11]<<8|FaultInfo_ERM[12];
    QString strErrDevName = "", strErrCode = "";
    switch (FaultInfo_ERM[13]) {
    case 1:  //门故障
        strErrDevName = "EDCU";
        strErrCode = getDoorErrorInfo(FaultInfo_ERM[14]);
        break;
    case 2:  //空调故障
        strErrDevName = "HVAC";
        strErrCode = getACCTempErrorInfo(FaultInfo_ERM[14]);
        break;
    case 3:  //制动装置故障
        strErrDevName = "BCU";
        strErrCode = getBCUErrorInfo(FaultInfo_ERM[14]);
        break;
    case 4:  //VVVF1故障
        strErrDevName = "VVVF1";
        strErrCode = getVVVFErrorInfo(FaultInfo_ERM[14]);
        break;
    case 5:  //VVVF2故障
        strErrDevName = "VVVF2";
        strErrCode = getVVVFErrorInfo(FaultInfo_ERM[14]);
        break;
    case 6:  //SIV故障
        strErrDevName = "SIV";
        strErrCode = getSIVErrorInfo(FaultInfo_ERM[14]);
        break;
    case 7:  //空压机故障
        strErrDevName = "ACP";
        strErrCode = "ACPF";
        break;
    default:
        break;
    }
    int iVoltage = FaultInfo_ERM[18]<<8|FaultInfo_ERM[19];

    structFaultRecordData * mFaultInfo = new structFaultRecordData;
    mFaultInfo->iCarId = int(carId);
    mFaultInfo->iErrFlag = iFaultFlag;
    mFaultInfo->iErrId = iFaultId;
    mFaultInfo->strDate = strDate;
    mFaultInfo->strTime = strTime;
    mFaultInfo->iErrStationCode = iStationId;
    mFaultInfo->iErrDistance = iDistance;
    mFaultInfo->strErrDevName = strErrDevName;
    mFaultInfo->strErrCode = strErrCode;
    mFaultInfo->iTracBrakeLevel = FaultInfo_ERM[15];
    mFaultInfo->iPWMValue = FaultInfo_ERM[16];
    mFaultInfo->iSpeedValue = FaultInfo_ERM[17];
    mFaultInfo->iVoltage = iVoltage;

    switch (carId) {
    case CAR_1:
        car1FaultList.append(mFaultInfo);
        break;
    case CAR_2:
        car2FaultList.append(mFaultInfo);
        break;
    case CAR_3:
        car3FaultList.append(mFaultInfo);
        break;
    case CAR_4:
        car4FaultList.append(mFaultInfo);
        break;
    case CAR_5:
        car5FaultList.append(mFaultInfo);
        break;
    case CAR_6:
        car6FaultList.append(mFaultInfo);
        break;
    default:
        break;
    }

    //回复消息，单条信息处理完成
//    while (1){
//        usleep(5000);
//        break;
//    }
    canSendSDOData(7, mERMFlag);
    qDebug()<<"$$$$$$$  HMI Send Res to ERM for Fault Info  $$$$$$$$$$  ERM Id ==  "<<mERMFlag;

    emit sig_FaultRecordData_Get_Update();
}

void CanSignalsSlots::car_RunInfo_ERM_Update(){
    int iERMWorkState = 1;  //0：通讯正常，1：通讯异常；
    if(CAN_1 == canWorkFlag){
        iERMWorkState = bTMSDevComnErrFlag[10] ? 1: 0;
    }
    else if(CAN_2 == canWorkFlag){
        iERMWorkState = bTMSDevComnErrFlag[11] ? 1: 0;
    }
    else{
        iERMWorkState = 1;
    }
    QString strRunDate = ((RunData_ERM[0] < 10)?QString("0%1").arg(RunData_ERM[0]):QString("%1").arg(RunData_ERM[0]))+"-"+((RunData_ERM[1] < 10)?QString("0%1").arg(RunData_ERM[1]):QString("%1").arg(RunData_ERM[1]))+"-"+((RunData_ERM[2] < 10)?QString("0%1").arg(RunData_ERM[2]):QString("%1").arg(RunData_ERM[2]));
    QString strRunTime = ((RunData_ERM[3] < 10)?QString("0%1").arg(RunData_ERM[3]):QString("%1").arg(RunData_ERM[3]))+":"+((RunData_ERM[4] < 10)?QString("0%1").arg(RunData_ERM[4]):QString("%1").arg(RunData_ERM[4]))+":"+((RunData_ERM[5] < 10)?QString("0%1").arg(RunData_ERM[5]):QString("%1").arg(RunData_ERM[5]));
    u_int16_t u16RunData = RunData_ERM[6]<<8|RunData_ERM[7];
    QString strWalkDate = ((RunData_ERM[8] < 10)?QString("0%1").arg(RunData_ERM[8]):QString("%1").arg(RunData_ERM[8]))+"-"+((RunData_ERM[9] < 10)?QString("0%1").arg(RunData_ERM[9]):QString("%1").arg(RunData_ERM[9]))+"-"+((RunData_ERM[10] < 10)?QString("0%1").arg(RunData_ERM[10]):QString("%1").arg(RunData_ERM[10]));
    QString strWalkTime = ((RunData_ERM[11] < 10)?QString("0%1").arg(RunData_ERM[11]):QString("%1").arg(RunData_ERM[11]))+":"+((RunData_ERM[12] < 10)?QString("0%1").arg(RunData_ERM[12]):QString("%1").arg(RunData_ERM[12]))+":"+((RunData_ERM[13] < 10)?QString("0%1").arg(RunData_ERM[13]):QString("%1").arg(RunData_ERM[13]));
    u_int16_t u16WalkData = RunData_ERM[14]<<8|RunData_ERM[15];
    QString strLoadRateDate = ((RunData_ERM[16] < 10)?QString("0%1").arg(RunData_ERM[16]):QString("%1").arg(RunData_ERM[16]))+"-"+((RunData_ERM[17] < 10)?QString("0%1").arg(RunData_ERM[17]):QString("%1").arg(RunData_ERM[17]))+"-"+((RunData_ERM[18] < 10)?QString("0%1").arg(RunData_ERM[18]):QString("%1").arg(RunData_ERM[18]));
    QString strLoadRateTime = ((RunData_ERM[19] < 10)?QString("0%1").arg(RunData_ERM[19]):QString("%1").arg(RunData_ERM[19]))+":"+((RunData_ERM[20] < 10)?QString("0%1").arg(RunData_ERM[20]):QString("%1").arg(RunData_ERM[20]))+":"+((RunData_ERM[21] < 10)?QString("0%1").arg(RunData_ERM[21]):QString("%1").arg(RunData_ERM[21]));
    u_int16_t u16LoadRateData = RunData_ERM[22]<<8|RunData_ERM[23];
    QString strDistanceDate = ((RunData_ERM[24] < 10)?QString("0%1").arg(RunData_ERM[24]):QString("%1").arg(RunData_ERM[24]))+"-"+((RunData_ERM[25] < 10)?QString("0%1").arg(RunData_ERM[25]):QString("%1").arg(RunData_ERM[25]))+"-"+((RunData_ERM[26] < 10)?QString("0%1").arg(RunData_ERM[26]):QString("%1").arg(RunData_ERM[26]));
    QString strDistanceTime = ((RunData_ERM[27] < 10)?QString("0%1").arg(RunData_ERM[27]):QString("%1").arg(RunData_ERM[27]))+":"+((RunData_ERM[28] < 10)?QString("0%1").arg(RunData_ERM[28]):QString("%1").arg(RunData_ERM[28]))+":"+((RunData_ERM[29] < 10)?QString("0%1").arg(RunData_ERM[29]):QString("%1").arg(RunData_ERM[29]));
    u_int32_t u32DistanceData = RunData_ERM[30]<<24|RunData_ERM[31]<<16|RunData_ERM[32]<<8|RunData_ERM[33];
    QString strWHMPDate = ((RunData_ERM[34] < 10)?QString("0%1").arg(RunData_ERM[34]):QString("%1").arg(RunData_ERM[34]))+"-"+((RunData_ERM[35] < 10)?QString("0%1").arg(RunData_ERM[35]):QString("%1").arg(RunData_ERM[35]))+"-"+((RunData_ERM[36] < 10)?QString("0%1").arg(RunData_ERM[36]):QString("%1").arg(RunData_ERM[36]));
    QString strWHMPTime = ((RunData_ERM[37] < 10)?QString("0%1").arg(RunData_ERM[37]):QString("%1").arg(RunData_ERM[37]))+":"+((RunData_ERM[38] < 10)?QString("0%1").arg(RunData_ERM[38]):QString("%1").arg(RunData_ERM[38]))+":"+((RunData_ERM[39] < 10)?QString("0%1").arg(RunData_ERM[39]):QString("%1").arg(RunData_ERM[39]));
    u_int32_t u32WHMPData = RunData_ERM[40]<<24|RunData_ERM[41]<<16|RunData_ERM[42]<<8|RunData_ERM[43];
    QString strWHMBDate = ((RunData_ERM[44] < 10)?QString("0%1").arg(RunData_ERM[44]):QString("%1").arg(RunData_ERM[44]))+"-"+((RunData_ERM[45] < 10)?QString("0%1").arg(RunData_ERM[45]):QString("%1").arg(RunData_ERM[45]))+"-"+((RunData_ERM[46] < 10)?QString("0%1").arg(RunData_ERM[46]):QString("%1").arg(RunData_ERM[46]));
    QString strWHMBTime = ((RunData_ERM[47] < 10)?QString("0%1").arg(RunData_ERM[47]):QString("%1").arg(RunData_ERM[47]))+":"+((RunData_ERM[48] < 10)?QString("0%1").arg(RunData_ERM[48]):QString("%1").arg(RunData_ERM[48]))+":"+((RunData_ERM[49] < 10)?QString("0%1").arg(RunData_ERM[49]):QString("%1").arg(RunData_ERM[49]));
    u_int32_t u32WHMBData = RunData_ERM[50]<<24|RunData_ERM[51]<<16|RunData_ERM[52]<<8|RunData_ERM[53];
    QString strACPWorkDate = ((RunData_ERM[54] < 10)?QString("0%1").arg(RunData_ERM[54]):QString("%1").arg(RunData_ERM[54]))+"-"+((RunData_ERM[55] < 10)?QString("0%1").arg(RunData_ERM[55]):QString("%1").arg(RunData_ERM[55]))+"-"+((RunData_ERM[56] < 10)?QString("0%1").arg(RunData_ERM[56]):QString("%1").arg(RunData_ERM[56]));
    QString strACPWorkTime = ((RunData_ERM[57] < 10)?QString("0%1").arg(RunData_ERM[57]):QString("%1").arg(RunData_ERM[57]))+":"+((RunData_ERM[58] < 10)?QString("0%1").arg(RunData_ERM[58]):QString("%1").arg(RunData_ERM[58]))+":"+((RunData_ERM[59] < 10)?QString("0%1").arg(RunData_ERM[59]):QString("%1").arg(RunData_ERM[59]));
    u_int32_t u32ACPWorkData = RunData_ERM[60]<<24|RunData_ERM[61]<<16|RunData_ERM[62]<<8|RunData_ERM[63];

    carRunDataInfo.iErmComnState = iERMWorkState;
    carRunDataInfo.strRunDataDate = strRunDate;
    carRunDataInfo.strRunDataTime = strRunTime;
    carRunDataInfo.u16RunData = u16RunData;
    carRunDataInfo.strWalkDataDate = strWalkDate;
    carRunDataInfo.strWalkDataTime = strWalkTime;
    carRunDataInfo.u16WalkData = u16WalkData;
    carRunDataInfo.strLoadRateDataDate = strLoadRateDate;
    carRunDataInfo.strLoadRateDataTime = strLoadRateTime;
    carRunDataInfo.u16LoadRateData = u16LoadRateData;
    carRunDataInfo.strDistanceDataDate = strDistanceDate;
    carRunDataInfo.strDistanceDataTime = strDistanceTime;
    carRunDataInfo.u32DistanceData = u32DistanceData;
    carRunDataInfo.strWHMPDataDate = strWHMPDate;
    carRunDataInfo.strWHMPDataTime = strWHMPTime;
    carRunDataInfo.u32WHMPData = u32WHMPData;
    carRunDataInfo.strWHMBDataDate = strWHMBDate;
    carRunDataInfo.strWHMBDataTime = strWHMBTime;
    carRunDataInfo.u32WHMBData = u32WHMBData;
    carRunDataInfo.strACPWorkDataDate = strACPWorkDate;
    carRunDataInfo.strACPWorkDataTime = strACPWorkTime;
    carRunDataInfo.u32ACPWorkData = u32ACPWorkData;

    emit sig_RunRecordData_Get_Update();
}

void CanSignalsSlots::car_AllSpeedInfo_ERM_Update(int mERM){
    if(!bNowShowSpeedFlag){
        return ;
    }

    QString strDate = ((AllSpeedInfo_ERM[0] < 10)?QString("0%1").arg(AllSpeedInfo_ERM[0]):QString("%1").arg(AllSpeedInfo_ERM[0]))+"-"+((AllSpeedInfo_ERM[1] < 10)?QString("0%1").arg(AllSpeedInfo_ERM[1]):QString("%1").arg(AllSpeedInfo_ERM[1]))+"-"+((AllSpeedInfo_ERM[2] < 10)?QString("0%1").arg(AllSpeedInfo_ERM[2]):QString("%1").arg(AllSpeedInfo_ERM[2]));
    QString strTime = ((AllSpeedInfo_ERM[3] < 10)?QString("0%1").arg(AllSpeedInfo_ERM[3]):QString("%1").arg(AllSpeedInfo_ERM[3]))+":"+((AllSpeedInfo_ERM[4] < 10)?QString("0%1").arg(AllSpeedInfo_ERM[4]):QString("%1").arg(AllSpeedInfo_ERM[4]))+":"+((AllSpeedInfo_ERM[5] < 10)?QString("0%1").arg(AllSpeedInfo_ERM[5]):QString("%1").arg(AllSpeedInfo_ERM[5]));
    int iCheckType = AllSpeedInfo_ERM[6];
    int8_t iSpeedValue = AllSpeedInfo_ERM[7];
    int iPWMValue = AllSpeedInfo_ERM[8];
    int iRebornFlag = AllSpeedInfo_ERM[9];
    int iBrakeSpeed = AllSpeedInfo_ERM[10];
    int iBrakeDistance = AllSpeedInfo_ERM[11]<<8 | AllSpeedInfo_ERM[12];

//    qDebug()<<"@#$%@ Speed Check Data :: "<<strDate<<"  "<<strTime<<"  "<<iCheckType<<"  "<<iSpeedValue<<"  "<<iPWMValue<<"  "<<iRebornFlag<<"  "<<iBrakeSpeed<<"  "<<iBrakeDistance;

    structSpeedCheck * mSpeedCheckData = new structSpeedCheck;
    mSpeedCheckData->strDate = strDate;
    mSpeedCheckData->strTime = strTime;
    mSpeedCheckData->iCheckType = iCheckType;
    mSpeedCheckData->iSpeedValue = iSpeedValue;
    mSpeedCheckData->iPWMValue = iPWMValue;
    mSpeedCheckData->iRebornFlag = iRebornFlag;
    mSpeedCheckData->iBrakeSpeed = iBrakeSpeed;
    mSpeedCheckData->iBrakeDistance = iBrakeDistance;

    carSpeedDataList.append(mSpeedCheckData);

    emit sig_SpeedRecordData_Get_Update(1);

    //回复消息，单条信息处理完成
//    while (1){
//        usleep(2000);
//        break;
//    }
    canSendSDOData(8, mERM);
}

void CanSignalsSlots::car_ERM_DelResult_Update(){
    if(1 == DelResult_ERM[2]){
        if(1 == DelResult_ERM[0] && 1 == DelResult_ERM[1]){
            qDeleteAll(car1FaultList);
            car1FaultList.clear();
            qDeleteAll(car2FaultList);
            car2FaultList.clear();
            qDeleteAll(car3FaultList);
            car3FaultList.clear();
            qDeleteAll(car4FaultList);
            car4FaultList.clear();
            qDeleteAll(car5FaultList);
            car5FaultList.clear();
            qDeleteAll(car6FaultList);
            car6FaultList.clear();

            for(int  i = 0; i < 6; i++){
                m_FaultRecordCount[i] = 0;
            }
            emit sig_FaultRecordCount_Get_Update();
        }
        else if(2 == DelResult_ERM[0]){
            // do nothing
        }
        else if(3 == DelResult_ERM[0]){
            if(bNowShowSpeedFlag){
                qDeleteAll(carSpeedDataList);
                carSpeedDataList.clear();

                emit sig_SpeedRecordData_Get_Update(0);
            }
        }
    }
}

RUN_DOOR_STATE CanSignalsSlots::getDoor_State(int value){
    RUN_DOOR_STATE doorState = RUN_DOOR_CLOSED;
    switch (value) {
    case 0:
        doorState = RUN_DOOR_COMN_ERR;
        break;
    case 1:
        doorState = RUN_DOOR_ISOLATE;
        break;
    case 2:
        doorState = RUN_DOOR_URGENT_ACTION;
        break;
    case 3:
        doorState = RUN_DOOR_BREAKDOWN;
        break;
    case 4:
        doorState = RUN_DOOR_PREVENT_SQUEEZE;
        break;
    case 5:
        doorState = RUN_DOOR_OPENED;
        break;
    case 6:
        doorState = RUN_DOOR_OPENING_CLOSING;
        break;
    case 7:
        doorState = RUN_DOOR_CLOSED;
        break;
    default:
        doorState = RUN_DOOR_COMN_ERR;
        break;
    }
    return doorState;
}

void CanSignalsSlots::door_OneSideState_Update(CAR_ID carId, DOOR_ID doorId){
    if(CAR_NULL == carId || DOOR_NULL == doorId){
        return;
    }
    int tempDoorData[4] ={0};
    switch (carId) {
    case CAR_1:
        for(int i = 0; i < 4; i++){
            tempDoorData[i] = DoorData_MC1[int(doorId-1)*4+i];
        }
        break;
    case CAR_2:
        for(int i = 0; i < 4; i++){
            tempDoorData[i] = DoorData_T[int(doorId-1)*4+i];
        }
        break;
    case CAR_3:
        for(int i = 0; i < 4; i++){
            tempDoorData[i] = DoorData_M[int(doorId-1)*4+i];
        }
        break;
    case CAR_4:
        for(int i = 0; i < 4; i++){
            tempDoorData[i] = DoorData_T1[int(doorId-1)*4+i];
        }
        break;
    case CAR_5:
        for(int i = 0; i < 4; i++){
            tempDoorData[i] = DoorData_T2[int(doorId-1)*4+i];
        }
        break;
    case CAR_6:
        for(int i = 0; i < 4; i++){
            tempDoorData[i] = DoorData_MC2[int(doorId-1)*4+i];
        }
        break;
    default:
        break;
    }
    RUN_DOOR_STATE doorState = RUN_DOOR_COMN_ERR;
    if(getCanDataBitFromByte(tempDoorData[0], 4)){
        doorState = RUN_DOOR_ISOLATE;
    }
    else if(getCanDataBitFromByte(tempDoorData[0], 3)){
        doorState = RUN_DOOR_URGENT_ACTION;
    }
    else if(mDoorFirstFaultCode[int(carId-1)*8+int(doorId-1)] > 0){
        doorState = RUN_DOOR_BREAKDOWN;
    }
    else if(getCanDataBitFromByte(tempDoorData[2], 5) || getCanDataBitFromByte(tempDoorData[2], 6)){
        doorState = RUN_DOOR_PREVENT_SQUEEZE;
    }
    else if(getCanDataBitFromByte(tempDoorData[0], 7)){
        doorState = RUN_DOOR_OPENED;
    }
    else if(getCanDataBitFromByte(tempDoorData[0], 6)){
        doorState = RUN_DOOR_CLOSED;
    }
    else if(getCanDataBitFromByte(tempDoorData[0], 5)){
        doorState = RUN_DOOR_OPENING_CLOSING;
    }
    else{
        doorState = RUN_DOOR_COMN_ERR;
    }
    bool bChangeFlag = false;
    switch (carId) {
    case CAR_1:
        if(int(doorState) != doorMC1State[int(doorId-1)]){
            doorMC1State[int(doorId-1)] = int(doorState);
            bChangeFlag = true;
        }
        break;
    case CAR_2:
        if(int(doorState) != doorTState[int(doorId-1)]){
            doorTState[int(doorId-1)] = int(doorState);
            bChangeFlag = true;
        }
        break;
    case CAR_3:
        if(int(doorState) != doorMState[int(doorId-1)]){
            doorMState[int(doorId-1)] = int(doorState);
            bChangeFlag = true;
        }
        break;
    case CAR_4:
        if(int(doorState) != doorT1State[int(doorId-1)]){
            doorT1State[int(doorId-1)] = int(doorState);
            bChangeFlag = true;
        }
        break;
    case CAR_5:
        if(int(doorState) != doorT2State[int(doorId-1)]){
            doorT2State[int(doorId-1)] = int(doorState);
            bChangeFlag = true;
        }
        break;
    case CAR_6:
        if(int(doorState) != doorMC2State[int(doorId-1)]){
            doorMC2State[int(doorId-1)] = int(doorState);
            bChangeFlag = true;
        }
        break;
    default:
        bChangeFlag = false;
        break;
    }
    if(bChangeFlag){
        emit sig_Door_State_Change(carId, doorId, doorState);
    }
}

int CanSignalsSlots::getACCWorkModel(CAR_ID carId, int accIndex){
    int accModel = 0;
    u_int8_t accModelData = 0;
    switch (carId) {
    case CAR_1:
        if(1 == accIndex){
            accModelData = ACCData_MC1[0];
        }
        else{
            accModelData = ACCData_MC1[1];
        }
        break;
    case CAR_2:
        if(1 == accIndex){
            accModelData = ACCData_T[0];
        }
        else{
            accModelData = ACCData_T[1];
        }
        break;
    case CAR_3:
        if(1 == accIndex){
            accModelData = ACCData_M[0];
        }
        else{
            accModelData = ACCData_M[1];
        }
        break;
    case CAR_4:
        if(1 == accIndex){
            accModelData = ACCData_T1[0];
        }
        else{
            accModelData = ACCData_T1[1];
        }
        break;
    case CAR_5:
        if(1 == accIndex){
            accModelData = ACCData_T2[0];
        }
        else{
            accModelData = ACCData_T2[1];
        }
        break;
    case CAR_6:
        if(1 == accIndex){
            accModelData = ACCData_MC2[0];
        }
        else{
            accModelData = ACCData_MC2[1];
        }
        break;
    default:
        accModelData = 0;
        break;
    }
    if(getCanDataBitFromByte(accModelData, 7)){
        accModel = 1;
    }
    else if(getCanDataBitFromByte(accModelData, 6)){
        accModel = 2;
    }
    else if(getCanDataBitFromByte(accModelData, 5)){
        accModel = 3;
    }
    else if(getCanDataBitFromByte(accModelData, 4)){
        accModel = 4;
    }
    else if(getCanDataBitFromByte(accModelData, 3)){
        accModel = 5;
    }
    else if(getCanDataBitFromByte(accModelData, 2)){
        accModel = 6;
    }
    else if(getCanDataBitFromByte(accModelData, 1)){
        accModel = 7;
    }
    else if(getCanDataBitFromByte(accModelData, 0)){
        accModel = 8;
    }
    else{
        accModel = 1;
    }
    return accModel;
}

int CanSignalsSlots::getACCBreakdownCode(CAR_ID carId, int accIndex){
    int accErrCode = 0, accStateData[3] = {0};
    switch (carId) {
    case CAR_1:
        if(1 == accIndex){
            accStateData[0] = ACCData_MC1[2];
            accStateData[1] = ACCData_MC1[3];
            accStateData[2] = ACCData_MC1[7];
        }
        else{
            accStateData[0] = ACCData_MC1[4];
            accStateData[1] = ACCData_MC1[5];
            accStateData[2] = ACCData_MC1[7];
        }
        break;
    case CAR_2:
        if(1 == accIndex){
            accStateData[0] = ACCData_T[2];
            accStateData[1] = ACCData_T[3];
            accStateData[2] = ACCData_T[7];
        }
        else{
            accStateData[0] = ACCData_T[4];
            accStateData[1] = ACCData_T[5];
            accStateData[2] = ACCData_T[7];
        }
        break;
    case CAR_3:
        if(1 == accIndex){
            accStateData[0] = ACCData_M[2];
            accStateData[1] = ACCData_M[3];
            accStateData[2] = ACCData_M[7];
        }
        else{
            accStateData[0] = ACCData_M[4];
            accStateData[1] = ACCData_M[5];
            accStateData[2] = ACCData_M[7];
        }
        break;
    case CAR_4:
        if(1 == accIndex){
            accStateData[0] = ACCData_T1[2];
            accStateData[1] = ACCData_T1[3];
            accStateData[2] = ACCData_T1[7];
        }
        else{
            accStateData[0] = ACCData_T1[4];
            accStateData[1] = ACCData_T1[5];
            accStateData[2] = ACCData_T1[7];
        }
        break;
    case CAR_5:
        if(1 == accIndex){
            accStateData[0] = ACCData_T2[2];
            accStateData[1] = ACCData_T2[3];
            accStateData[2] = ACCData_T2[7];
        }
        else{
            accStateData[0] = ACCData_T2[4];
            accStateData[1] = ACCData_T2[5];
            accStateData[2] = ACCData_T2[7];
        }
        break;
    case CAR_6:
        if(1 == accIndex){
            accStateData[0] = ACCData_MC2[2];
            accStateData[1] = ACCData_MC2[3];
            accStateData[2] = ACCData_MC2[7];
        }
        else{
            accStateData[0] = ACCData_MC2[4];
            accStateData[1] = ACCData_MC2[5];
            accStateData[2] = ACCData_MC2[7];
        }
        break;
    default:
        break;
    }

    if(1 == accIndex){
        if(getCanDataBitFromByte(accStateData[2], 4)){  //PWF 空调电源故障
            accErrCode = 0x01;
        }
        else if(getCanDataBitFromByte(accStateData[2], 5)){  //EVF 紧急通风逆变器故障
            accErrCode = 0x02;
        }
        else if(getCanDataBitFromByte(accStateData[0], 4)){  //VEF11 空调机组1通风机1故障
            accErrCode = 0x03;
        }
        else if(getCanDataBitFromByte(accStateData[0], 5)){ //VEF12 空调机组1通风机2故障
            accErrCode = 0x05;
        }
        else if(getCanDataBitFromByte(accStateData[0], 6)){ //VEF13 空调机组1通风机3故障
            accErrCode = 0x07;
        }
        else if(getCanDataBitFromByte(accStateData[0], 7)){ //VEF14 空调机组1通风机4故障
            accErrCode = 0x09;
        }
        else if(getCanDataBitFromByte(accStateData[1], 1)){  //CPF11 空调机组1压缩机1过流故障
            accErrCode = 0x0B;
        }
        else if(getCanDataBitFromByte(accStateData[1], 7)){  //CPF12 空调机组1压缩机2过流故障
            accErrCode = 0x0D;
        }
        else if(getCanDataBitFromByte(accStateData[1], 0)){  //CPP11 空调机组1压缩机1压力保护
            accErrCode = 0x0F;
        }
        else if(getCanDataBitFromByte(accStateData[1], 6)){  //CPP12 空调机组1压缩机2压力保护
            accErrCode = 0x11;
        }
        else if(getCanDataBitFromByte(accStateData[0], 0)){  //COF11 空调机组1冷凝风机1故障
            accErrCode = 0x13;
        }
        else if(getCanDataBitFromByte(accStateData[0], 1)){  //COF12 空调机组1冷凝风机2故障
            accErrCode = 0x15;
        }
        else if(getCanDataBitFromByte(accStateData[2], 3)){  //FTS1 新风温度传感器1故障
            accErrCode = 0x17;
        }
        else if(getCanDataBitFromByte(accStateData[2], 1)){  //RTS1 回风温度传感器1故障
            accErrCode = 0x19;
        }
        else if(getCanDataBitFromByte(accStateData[2], 7)){  //FAN1 废排风机1故障
            accErrCode = 0x1B;
        }
        else{
            accErrCode = 0;
        }
    }
    else{
        if(getCanDataBitFromByte(accStateData[2], 4)){  //PWF 空调电源故障
            accErrCode = 0x01;
        }
        else if(getCanDataBitFromByte(accStateData[2], 5)){  //EVF 紧急通风逆变器故障
            accErrCode = 0x02;
        }
        else if(getCanDataBitFromByte(accStateData[0], 4)){  //VEF21 空调机组2通风机1故障
            accErrCode = 0x04;
        }
        else if(getCanDataBitFromByte(accStateData[0], 5)){  //VEF21 空调机组2通风机2故障
            accErrCode = 0x06;
        }
        else if(getCanDataBitFromByte(accStateData[0], 6)){  //VEF21 空调机组2通风机3故障
            accErrCode = 0x08;
        }
        else if(getCanDataBitFromByte(accStateData[0], 7)){  //VEF21 空调机组2通风机4故障
            accErrCode = 0x0A;
        }
        else if(getCanDataBitFromByte(accStateData[1], 1)){  //CPF21 空调机组2压缩机1过流故障
            accErrCode = 0x0C;
        }
        else if(getCanDataBitFromByte(accStateData[1], 7)){  //CPF22 空调机组2压缩机1过流故障
            accErrCode = 0x0E;
        }
        else if(getCanDataBitFromByte(accStateData[1], 0)){  //CPP21 空调机组2压缩机1压力保护
            accErrCode = 0x10;
        }
        else if(getCanDataBitFromByte(accStateData[1], 6)){  //CPP22 空调机组2压缩机2压力保护
            accErrCode = 0x12;
        }
        else if(getCanDataBitFromByte(accStateData[0], 0)){  //COF21 空调机组2冷凝风机1故障
            accErrCode = 0x14;
        }
        else if(getCanDataBitFromByte(accStateData[0], 1)){  //COF22 空调机组2冷凝风机2故障
            accErrCode = 0x16;
        }
        else if(getCanDataBitFromByte(accStateData[2], 2)){  //FTS2 新风温度传感器2故障
            accErrCode = 0x18;
        }
        else if(getCanDataBitFromByte(accStateData[2], 0)){  //RTS2 回风温度传感器2故障
            accErrCode = 0x1A;
        }
        else if(getCanDataBitFromByte(accStateData[2], 6)){  //FAN2 废排风机2故障
            accErrCode = 0x1C;
        }
        else{
            accErrCode = 0;
        }
    }
    return accErrCode;
}

int CanSignalsSlots::getACCCtrlMode(u_int8_t data){
    int accCtrlMode = 0;
    switch (data) {
    case 1:
        accCtrlMode = 6;
        break;
    case 2:
        accCtrlMode = 5;
        break;
    case 4:
        accCtrlMode = 2;
        break;
    case 8:
        accCtrlMode = 1;
        break;
    case 16:
        accCtrlMode = 3;
        break;
    case 32:
        accCtrlMode = 4;
        break;
    case 64:
        accCtrlMode = 7;
        break;
    case 128:
        accCtrlMode = 8;
        break;
    default:
        accCtrlMode = 0;
        break;
    }
    return accCtrlMode;
}

void CanSignalsSlots::getCarVoltage(){
    int tempVoltage[2] = {0};
    if(!bTMSDevComnErrFlag[2]){
        if(!getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 2)){
            tempVoltage[0] = 1;
            tempVoltage[1] = VVVF1Data_MC1[18];
            goto VoltageState;
        }
        else if(!getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 3)){
            tempVoltage[0] = 1;
            tempVoltage[1] = VVVF2Data_MC1[18];
            goto VoltageState;
        }
        else{
            tempVoltage[0] = 0;
            tempVoltage[1] = 0;
        }
    }

    if(!bTMSDevComnErrFlag[4]){
        if(!getCanDataBitFromByte(SubDev_ComnErr_M[1], 2)){
            tempVoltage[0] = 1;
            tempVoltage[1] = VVVF1Data_M[18];
            goto VoltageState;
        }
        else if(!getCanDataBitFromByte(SubDev_ComnErr_M[1], 3)){
            tempVoltage[0] = 1;
            tempVoltage[1] = VVVF2Data_M[18];
            goto VoltageState;
        }
        else{
            tempVoltage[0] = 0;
            tempVoltage[1] = 0;
        }
    }

    if(!bTMSDevComnErrFlag[7]){
        if(!getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 2)){
            tempVoltage[0] = 1;
            tempVoltage[1] = VVVF1Data_MC2[18];
            goto VoltageState;
        }
        else if(!getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 3)){
            tempVoltage[0] = 1;
            tempVoltage[1] = VVVF2Data_MC2[18];
            goto VoltageState;
        }
        else{
            tempVoltage[0] = 0;
            tempVoltage[1] = 0;
        }
    }

VoltageState:
    if(tempVoltage[0] != carVoltageValue[0] || tempVoltage[1] != carVoltageValue[1]){
        float fValue = tempVoltage[1]*VOLTAGE_UNIT_VALUE;
        carVoltageValue[0] = tempVoltage[0];
        carVoltageValue[1] = qRound(fValue);
    }
}

void CanSignalsSlots::getCarCurrent(){
    int8_t tempCurrent[6][2];
    for(int i = 0; i < 6; i++){
        tempCurrent[i][0] = 0;
        tempCurrent[i][1] = 0;
    }

    if(!bTMSDevComnErrFlag[2]){
        if(!getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 2)){
            tempCurrent[0][0] = 1;
            tempCurrent[0][1] = VVVF1Data_MC1[23];
        }
        if(!getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 3)){
            tempCurrent[1][0] = 1;
            tempCurrent[1][1] = VVVF2Data_MC1[23];
        }
    }

    if(!bTMSDevComnErrFlag[4]){
        if(!getCanDataBitFromByte(SubDev_ComnErr_M[1], 2)){
            tempCurrent[2][0] = 1;
            tempCurrent[2][1] = VVVF1Data_M[23];
        }
        if(!getCanDataBitFromByte(SubDev_ComnErr_M[1], 3)){
            tempCurrent[3][0] = 1;
            tempCurrent[3][1] = VVVF2Data_M[23];
        }
    }

    if(!bTMSDevComnErrFlag[7]){
        if(!getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 2)){
            tempCurrent[4][0] = 1;
            tempCurrent[4][1] = VVVF1Data_MC2[23];
        }
        if(!getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 3)){
            tempCurrent[5][0] = 1;
            tempCurrent[5][1] = VVVF2Data_MC2[23];
        }
    }

    int mValidCount = 0;
    int allCurrentCount = 0;
    for(int i = 0; i < 6; i++){
        if(1 == tempCurrent[i][0]){
            allCurrentCount = allCurrentCount+tempCurrent[i][1];
            mValidCount++;
        }
    }
    if(mValidCount > 0){
        float fCurrentCount = allCurrentCount * CURRENT_UNIT_VALUE_GT / (mValidCount * 1.0);
        allCurrentCount = qRound(fCurrentCount);
        if(0 == carCurrentValue[0] || allCurrentCount != carCurrentValue[1]){
            carCurrentValue[0] = 1;
            carCurrentValue[1] = allCurrentCount;
        }
    }
    else{
        carCurrentValue[0] = 0;
        carCurrentValue[1] = 1;
    }
}

int CanSignalsSlots::getVVVFError(CAR_ID carId, int vvvfIndex){
    int iFaultCode = 0,vvvfStateData[7] = {0};
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
    if(getCanDataBitFromByte(vvvfStateData[3], 2)){ // 1  :  01h
        iFaultCode = 0x01;
    }
    else if(getCanDataBitFromByte(vvvfStateData[3], 1) || getCanDataBitFromByte(vvvfStateData[3], 3) || getCanDataBitFromByte(vvvfStateData[3], 4) || getCanDataBitFromByte(vvvfStateData[3], 5) ||
               getCanDataBitFromByte(vvvfStateData[3], 6) || getCanDataBitFromByte(vvvfStateData[3], 7) || getCanDataBitFromByte(vvvfStateData[4], 0) || getCanDataBitFromByte(vvvfStateData[4], 1) ||
               getCanDataBitFromByte(vvvfStateData[4], 2) || getCanDataBitFromByte(vvvfStateData[4], 3) || getCanDataBitFromByte(vvvfStateData[4], 5) || getCanDataBitFromByte(vvvfStateData[4], 7)){ // 2  :  02h
        iFaultCode = 0x02;
    }
    else if(getCanDataBitFromByte(vvvfStateData[2], 2) || getCanDataBitFromByte(vvvfStateData[2], 3) || getCanDataBitFromByte(vvvfStateData[2], 4)){ // 3  :  03h
        iFaultCode = 0x03;
    }
    else if(getCanDataBitFromByte(vvvfStateData[5], 1)){ // 4  :  04h
        iFaultCode = 0x04;
    }
    else if(getCanDataBitFromByte(vvvfStateData[5], 3)){ // 5  :  05h
        iFaultCode = 0x05;
    }
    else if(getCanDataBitFromByte(vvvfStateData[5], 2)){ // 6  :  06h
        iFaultCode = 0x06;
    }
    else if(getCanDataBitFromByte(vvvfStateData[5], 4)){ // 7  :  07h
        iFaultCode = 0x07;
    }
    else if(getCanDataBitFromByte(vvvfStateData[5], 5)){ // 8  :  08h
        iFaultCode = 0x08;
    }
    else if(getCanDataBitFromByte(vvvfStateData[5], 6)){ // 9  :  09h
        iFaultCode = 0x09;
    }
    else if(getCanDataBitFromByte(vvvfStateData[5], 7)){ // 10  :  0Bh
        iFaultCode = 0x0B;
    }
    else if(getCanDataBitFromByte(vvvfStateData[6], 5)){ // 11  :  0Ch
        iFaultCode = 0x0C;
    }
    else if(getCanDataBitFromByte(vvvfStateData[6], 7)){ // 12  :  0Ah
        iFaultCode = 0x0A;
    }
    else if(getCanDataBitFromByte(vvvfStateData[4], 4) || getCanDataBitFromByte(vvvfStateData[6], 3)){ // 13  :  0Dh
        iFaultCode = 0x0D;
    }
    else if(getCanDataBitFromByte(vvvfStateData[1], 6) || getCanDataBitFromByte(vvvfStateData[1], 7)){ // 14  :  0Ch
        iFaultCode = 0x0C;
    }
    else if(getCanDataBitFromByte(vvvfStateData[0], 3)){ // 15  :  0Eh
        iFaultCode = 0x0E;
    }
    else if(getCanDataBitFromByte(vvvfStateData[6], 2)){ // 16  :  0Fh
        iFaultCode = 0x0F;
    }
    else{
        iFaultCode = 0;
    }
    return iFaultCode;
}

int CanSignalsSlots::getBCUError(CAR_ID carId){
    int iFaultCode = 0, bcuStateData[7] = {0};
    switch (carId) {
    case CAR_1:
        for(int i = 0; i < 7; i++){
            bcuStateData[i] = BCUData_MC1[i];
        }
        break;
    case CAR_2:
        for(int i = 0; i < 7; i++){
            bcuStateData[i] = BCUData_T[i];
        }
        break;
    case CAR_3:
        for(int i = 0; i < 7; i++){
            bcuStateData[i] = BCUData_M[i];
        }
        break;
    case CAR_4:
        for(int i = 0; i < 7; i++){
            bcuStateData[i] = BCUData_T1[i];
        }
        break;
    case CAR_5:
        for(int i = 0; i < 7; i++){
            bcuStateData[i] = BCUData_T2[i];
        }
        break;
    case CAR_6:
        for(int i = 0; i < 7; i++){
            bcuStateData[i] = BCUData_MC2[i];
        }
        break;
    default:
        break;
    }
    if(getCanDataBitFromByte(bcuStateData[0], 7)){  // 1  : 90h
        iFaultCode = 0x90;
    }
    else if(getCanDataBitFromByte(bcuStateData[0], 6)){  // 2  : 91h
        iFaultCode = 0x91;
    }
    else if(getCanDataBitFromByte(bcuStateData[0], 3)){  // 3  : 92h
        iFaultCode = 0x92;
    }
    else if(getCanDataBitFromByte(bcuStateData[0], 4)){  // 4  : 93h
        iFaultCode = 0x93;
    }
    else if(getCanDataBitFromByte(bcuStateData[0], 5)){  // 5  : 94h
        iFaultCode = 0x94;
    }
    else if(getCanDataBitFromByte(bcuStateData[1], 0)){  // 6  : 95h
        iFaultCode = 0x95;
    }
    else if(getCanDataBitFromByte(bcuStateData[0], 1)){  // 7  : 96h
        iFaultCode = 0x96;
    }
    else if(getCanDataBitFromByte(bcuStateData[0], 0)){  // 8  : 97h
        iFaultCode = 0x97;
    }
    else if(getCanDataBitFromByte(bcuStateData[1], 2)){  // 9  : 98h
        iFaultCode = 0x98;
    }
    else if(getCanDataBitFromByte(bcuStateData[1], 4)){  // 10  : 9Ah
        iFaultCode = 0x9A;
    }
    else if(getCanDataBitFromByte(bcuStateData[1], 5)){  // 11  : 9Bh
        iFaultCode = 0x9B;
    }
    else if(getCanDataBitFromByte(bcuStateData[1], 6)){  // 12  : 9Ch
        iFaultCode = 0x9C;
    }
    else if(getCanDataBitFromByte(bcuStateData[1], 7)){  // 13  : 9Dh
        iFaultCode = 0x9D;
    }
    else if(getCanDataBitFromByte(bcuStateData[1], 1)){  // 14  : 9Eh
        iFaultCode = 0x9E;
    }
    else if(getCanDataBitFromByte(bcuStateData[4], 1)){  // 15  : 9Fh
        iFaultCode = 0x9F;
    }
    else if(getCanDataBitFromByte(bcuStateData[3], 0)){  // 16  : A0h
        iFaultCode = 0xA0;
    }
    else if(getCanDataBitFromByte(bcuStateData[3], 2)){  // 17  : A1h
        iFaultCode = 0xA1;
    }
    else if(getCanDataBitFromByte(bcuStateData[3], 1)){  // 18  : A2h
        iFaultCode = 0xA2;
    }
    else if(getCanDataBitFromByte(bcuStateData[3], 4)){  // 19  : A3h
        iFaultCode = 0xA3;
    }
    else if(getCanDataBitFromByte(bcuStateData[3], 3)){  // 20  : A4h
        iFaultCode = 0xA4;
    }
    else if(getCanDataBitFromByte(bcuStateData[2], 5)){  // 21  : A5h
        iFaultCode = 0xA5;
    }
    else if(getCanDataBitFromByte(bcuStateData[2], 6)){  // 22  : A6h
        iFaultCode = 0xA6;
    }
    else if(getCanDataBitFromByte(bcuStateData[1], 3)){  // 23  : A7h
        iFaultCode = 0xA7;
    }
    else{
        iFaultCode = 0;
    }
    return iFaultCode;
}

int CanSignalsSlots::getSIVError(CAR_ID carId){
    int iFaultCode = 0, sivStateData[3] = {0};
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
    /*if(false){ // 1 : 01h 备用
        iFaultCode = 0x01;
    }
    else if(false){ // 2 : 02h 备用
        iFaultCode = 0x02;
    }
    else */
    if(getCanDataBitFromByte(sivStateData[2], 2)){ // 3 : 03h
        iFaultCode = 0x03;
    }
    else if(getCanDataBitFromByte(sivStateData[2], 3)){ // 4 : 04h
        iFaultCode = 0x04;
    }
    else if(getCanDataBitFromByte(sivStateData[2], 4)){ // 5 : 05h
        iFaultCode = 0x05;
    }
    else if(getCanDataBitFromByte(sivStateData[2], 5)){ // 6 : 06h
        iFaultCode = 0x06;
    }
    else if(getCanDataBitFromByte(sivStateData[2], 6)){ // 7 : 07h
        iFaultCode = 0x07;
    }
    else if(getCanDataBitFromByte(sivStateData[2], 7)){ // 8 : 08h
        iFaultCode = 0x08;
    }
    else if(getCanDataBitFromByte(sivStateData[1], 0)){ // 9 : 11h
        iFaultCode = 0x11;
    }
    else if(getCanDataBitFromByte(sivStateData[1], 1)){ // 10 : 12h
        iFaultCode = 0x12;
    }
    else if(getCanDataBitFromByte(sivStateData[1], 2)){ // 11 : 13h
        iFaultCode = 0x13;
    }
    else if(getCanDataBitFromByte(sivStateData[1], 3)){ // 12 : 14h
        iFaultCode = 0x14;
    }
    else if(getCanDataBitFromByte(sivStateData[1], 4)){ // 13 : 15h
        iFaultCode = 0x15;
    }
    else if(getCanDataBitFromByte(sivStateData[1], 5)){ // 14 : 16h
        iFaultCode = 0x16;
    }
    else if(getCanDataBitFromByte(sivStateData[1], 7)){ // 15 : 18h
        iFaultCode = 0x18;
    }
    else if(getCanDataBitFromByte(sivStateData[2], 0)){ // 16 : 21h
        iFaultCode = 0x21;
    }
    else if(getCanDataBitFromByte(sivStateData[2], 1)){ // 17 : 22h
        iFaultCode = 0x22;
    }
    else if(getCanDataBitFromByte(sivStateData[0], 7)){ // 18 : FFh
        iFaultCode = 0xFF;
    }
    else{
        iFaultCode = 0;
    }
    return iFaultCode;
}

int CanSignalsSlots::getACCError(CAR_ID carId){
    int iFaultCode = 0, accStateData[5] = {0};
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
    if(getCanDataBitFromByte(accStateData[4], 4)){   //PWF 空调电源故障
        iFaultCode = 0x01;
    }
    else if(getCanDataBitFromByte(accStateData[4], 5)){  //EVF 紧急通风逆变器故障
        iFaultCode = 0x02;
    }
    else if(getCanDataBitFromByte(accStateData[0], 4)){  //VEF11 空调机组1通风机1故障
        iFaultCode = 0x03;
    }
    else if(getCanDataBitFromByte(accStateData[2], 4)){  //VEF21 空调机组2通风机1故障
        iFaultCode = 0x04;
    }
    else if(getCanDataBitFromByte(accStateData[0], 5)){  //VEF12 空调机组1通风机2故障
        iFaultCode = 0x05;
    }
    else if(getCanDataBitFromByte(accStateData[2], 5)){  //VEF22 空调机组2通风机2故障
        iFaultCode = 0x06;
    }
    else if(getCanDataBitFromByte(accStateData[0], 6)){  //VEF13 空调机组1通风机3故障
        iFaultCode = 0x07;
    }
    else if(getCanDataBitFromByte(accStateData[2], 6)){  //VEF23 空调机组2通风机3故障
        iFaultCode = 0x08;
    }
    else if(getCanDataBitFromByte(accStateData[0], 7)){  //VEF14 空调机组1通风机4故障
        iFaultCode = 0x09;
    }
    else if(getCanDataBitFromByte(accStateData[2], 7)){  //VEF24 空调机组2通风机4故障
        iFaultCode = 0x0A;
    }
    else if(getCanDataBitFromByte(accStateData[1], 1)){  //CPF11 空调机组1压缩机1过流故障
        iFaultCode = 0x0B;
    }
    else if(getCanDataBitFromByte(accStateData[3], 1)){  //CPF21 空调机组2压缩机1过流故障
        iFaultCode = 0x0C;
    }
    else if(getCanDataBitFromByte(accStateData[1], 7)){  //CPF12 空调机组1压缩机2过流故障
        iFaultCode = 0x0D;
    }
    else if(getCanDataBitFromByte(accStateData[3], 7)){  //CPF22 空调机组2压缩机2过流故障
        iFaultCode = 0x0E;
    }
    else if(getCanDataBitFromByte(accStateData[1], 0)){  //CPP11 空调机组1压缩机1压力保护
        iFaultCode = 0x0F;
    }
    else if(getCanDataBitFromByte(accStateData[3], 0)){  //CPP21 空调机组2压缩机1压力保护
        iFaultCode = 0x10;
    }
    else if(getCanDataBitFromByte(accStateData[1], 6)){  //CPP12 空调机组1压缩机2压力保护
        iFaultCode = 0x11;
    }
    else if(getCanDataBitFromByte(accStateData[3], 6)){  //CPP22 空调机组2压缩机2压力保护
        iFaultCode = 0x12;
    }
    else if(getCanDataBitFromByte(accStateData[0], 0)){  //COF11 空调机组1冷凝风机1故障
        iFaultCode = 0x13;
    }
    else if(getCanDataBitFromByte(accStateData[2], 0)){  //COF21 空调机组2冷凝风机1故障
        iFaultCode = 0x14;
    }
    else if(getCanDataBitFromByte(accStateData[0], 1)){  //COF12 空调机组1冷凝风机2故障
        iFaultCode = 0x15;
    }
    else if(getCanDataBitFromByte(accStateData[2], 1)){  //COF22 空调机组2冷凝风机2故障
        iFaultCode = 0x16;
    }
    else if(getCanDataBitFromByte(accStateData[4], 3)){  //FTS1 新风温度传感器1故障
        iFaultCode = 0x17;
    }
    else if(getCanDataBitFromByte(accStateData[4], 2)){  //FTS2 新风温度传感器2故障
        iFaultCode = 0x18;
    }
    else if(getCanDataBitFromByte(accStateData[4], 1)){  //RTS1 回风温度传感器1故障
        iFaultCode = 0x19;
    }
    else if(getCanDataBitFromByte(accStateData[4], 0)){  //RTS2 回风温度传感器2故障
        iFaultCode = 0x1A;
    }
    else if(getCanDataBitFromByte(accStateData[4], 7)){  //FAN1 废排风机1故障
        iFaultCode = 0x1B;
    }
    else if(getCanDataBitFromByte(accStateData[4], 6)){  //FAN2 废排风机2故障
        iFaultCode = 0x1C;
    }
    else{
        iFaultCode = 0;
    }
    return iFaultCode;
}

void CanSignalsSlots::getCarDestinationID(){
    int u16DesId = 0;
    if(atoModelFlag){
        if(!getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 6)){
            u16DesId = ATCData_MC1[10] << 8 | ATCData_MC1[9];
            CarArriveData_HMI[4] = ATCData_MC1[10];
            CarArriveData_HMI[5] = ATCData_MC1[9];
        }
        else if(getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 6) && !getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 6)){
            u16DesId = ATCData_MC2[10] << 8 | ATCData_MC2[9];
            CarArriveData_HMI[4] = ATCData_MC2[10];
            CarArriveData_HMI[5] = ATCData_MC2[9];
        }
        else {
            //do nothing
        }
        if(iEndStation != u16DesId){
            iEndStation = u16DesId;
            emit sig_Car_Arrive_Station_Update(3, u16DesId);
        }
    }
    else{
        CarArriveData_HMI[4] = 0xFF;
        CarArriveData_HMI[5] = 0xFF;
    }
}

void CanSignalsSlots::getCarNowStationID(){
    int u16NowId = 0;
    if(atoModelFlag){
        if(!getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 6)){
            u16NowId = ATCData_MC1[18] << 8 | ATCData_MC1[17];
            CarArriveData_HMI[6] = ATCData_MC1[18];
            CarArriveData_HMI[7] = ATCData_MC1[17];
        }
        else if(getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 6) && !getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 6)){
            u16NowId = ATCData_MC2[18] << 8 | ATCData_MC2[17];
            CarArriveData_HMI[6] = ATCData_MC2[18];
            CarArriveData_HMI[7] = ATCData_MC2[17];
        }
        else {
            //do nothing
        }
        if(iCurrStation != u16NowId){
            iCurrStation = u16NowId;
            emit sig_Car_Arrive_Station_Update(1, u16NowId);
        }
    }
    else{
        CarArriveData_HMI[6] = 0xFF;
        CarArriveData_HMI[7] = 0xFF;
    }
}

void CanSignalsSlots::getCarNextStationID(){
    int u16NextId = 0;
    if(atoModelFlag){
        if(!getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 6)){
            u16NextId = ATCData_MC1[20] << 8 | ATCData_MC1[19];
            CarArriveData_HMI[8] = ATCData_MC1[20];
            CarArriveData_HMI[9] = ATCData_MC1[19];
        }
        else if(getCanDataBitFromByte(SubDev_ComnErr_MC1[1], 6) && !getCanDataBitFromByte(SubDev_ComnErr_MC2[1], 6)){
            u16NextId = ATCData_MC2[20] << 8 | ATCData_MC2[19];
            CarArriveData_HMI[8] = ATCData_MC2[20];
            CarArriveData_HMI[9] = ATCData_MC2[19];
        }
        else {
            //do nothing
        }
        if(iNextStation != u16NextId){
            iNextStation = u16NextId;
            emit sig_Car_Arrive_Station_Update(2, u16NextId);
        }
    }
    else{
        CarArriveData_HMI[8] = 0xFF;
        CarArriveData_HMI[9] = 0xFF;
    }
}

int CanSignalsSlots::getBCUErrorCount(u_int8_t bcuData){
    int tempCount = 0;
    u_int8_t u8Bit = 1;
    for(int i = 0; i < 6; i++){
        if((bcuData & u8Bit) > 0){
            tempCount++;
        }
        u8Bit = u8Bit * 2;
    }
    return tempCount;
}

QString CanSignalsSlots::getDoorErrorInfo(int errCode){
    QString strErrCode = "";
    switch (errCode) {
    case 0x01:
        strErrCode = "01";
        break;
    case 0x02:
        strErrCode = "02";
        break;
    case 0x03:
        strErrCode = "03";
        break;
    case 0x04:
        strErrCode = "04";
        break;
    case 0x05:
        strErrCode = "05";
        break;
    case 0x06:
        strErrCode = "06";
        break;
    case 0x07:
        strErrCode = "07";
        break;
    case 0x08:
        strErrCode = "08";
        break;
    case 0x09:
        strErrCode = "09";
        break;
//    case 0x10:
//        strErrCode = "10";
//        break;
//    case 0x11:
//        strErrCode = "11";
//        break;
    case 0x12:
        strErrCode = "12";
        break;
    case 0x13:
        strErrCode = "13";
        break;
    case 0x14:
        strErrCode = "14";
        break;
    case 0x15:
        strErrCode = "15";
        break;
    case 0x16:
        strErrCode = "16";
        break;
//    case 0x17:
//        strErrCode = "17";
//        break;
    case 0x24:
        strErrCode = "24";
        break;
    case 0x25:
        strErrCode = "25";
        break;
    default:
        strErrCode = "";
        break;
    }
    return strErrCode;
}

QString CanSignalsSlots::getVVVFErrorInfo(int errCode){
    QString strErrCode = "";
    switch (errCode) {
    case 0x01:
        strErrCode = "HBT";
        break;
    case 0x02:
        strErrCode = "FAIL";
        break;
    case 0x03:
        strErrCode = "MMOCD";
        break;
    case 0x04:
        strErrCode = "FOVD";
        break;
    case 0x05:
        strErrCode = "FLVD";
        break;
    case 0x06:
        strErrCode = "SOVD";
        break;
    case 0x07:
        strErrCode = "PGD";
        break;
    case 0x08:
        strErrCode = "BSLP";
        break;
    case 0x09:
        strErrCode = "BSLD";
        break;
    case 0x0A:
        strErrCode = "THD1";
        break;
    case 0x0B:
        strErrCode = "LGD";
        break;
    case 0x0C:
        strErrCode = "FMEF";
        break;
    case 0x0D:
        strErrCode = "BCHF";
        break;
    case 0x0E:
        strErrCode = "T_PWMF";
        break;
    case 0x0F:
        strErrCode = "TEST";
        break;
    default:
        strErrCode = "";
        break;
    }
    return strErrCode;
}

QString CanSignalsSlots::getBCUErrorInfo(int errCode){
    QString strErrCode = "";
    switch (errCode) {
    case 0x90:
        strErrCode = "VLCF";
        break;
    case 0x91:
        strErrCode = "RPCF";
        break;
    case 0x92:
        strErrCode = "MCVF";
        break;
    case 0x93:
        strErrCode = "PCVF(1)";
        break;
    case 0x94:
        strErrCode = "PCVF(2)";
        break;
    case 0x95:
        strErrCode = "RFBF";
        break;
    case 0x96:
        strErrCode = "RAME";
        break;
    case 0x97:
        strErrCode = "TRCF";
        break;
    case 0x98:
        strErrCode = "TWLF";
        break;
    case 0x9A:
        strErrCode = "AS1F";
        break;
    case 0x9B:
        strErrCode = "AS2F";
        break;
    case 0x9C:
        strErrCode = "AS3F";
        break;
    case 0x9D:
        strErrCode = "AS4F";
        break;
    case 0x9E:
        strErrCode = "ARSF";
        break;
    case 0x9F:
        strErrCode = "PWMF";
        break;
    case 0xA0:
        strErrCode = "TVLCF";
        break;
    case 0xA1:
        strErrCode = "SASF1";
        break;
    case 0xA2:
        strErrCode = "SASF2";
        break;
    case 0xA3:
        strErrCode = "SACF";
        break;
    case 0xA4:
        strErrCode = "SBCF";
        break;
    case 0xA5:
        strErrCode = "IBD";
        break;
    case 0xA6:
        strErrCode = "NRBD";
        break;
    case 0xA7:
        strErrCode = "12VF";
        break;
    default:
        strErrCode = "";
        break;
    }
    return strErrCode;
}

QString CanSignalsSlots::getSIVErrorInfo(int errCode){
    QString strErrCode = "";
    switch (errCode) {
    case 0x01:
        strErrCode = "";
        break;
    case 0x02:
        strErrCode = "";
        break;
    case 0x03:
        strErrCode = "PWF";
        break;
    case 0x04:
        strErrCode = "FRQF";
        break;
    case 0x05:
        strErrCode = "CTF";
        break;
    case 0x06:
        strErrCode = "WD_C";
        break;
    case 0x07:
        strErrCode = "MKF";
        break;
    case 0x08:
        strErrCode = "THR";
        break;
    case 0x11:
        strErrCode = "DIF_S";
        break;
    case 0x12:
        strErrCode = "FCOV";
        break;
    case 0x13:
        strErrCode = "OUTOC";
        break;
    case 0x14:
        strErrCode = "IVOC";
        break;
    case 0x15:
        strErrCode = "OUTLV";
        break;
    case 0x16:
        strErrCode = "OUTOV";
        break;
    case 0x18:
        strErrCode = "HBT";
        break;
    case 0x21:
        strErrCode = "RCDF";
        break;
    case 0x22:
        strErrCode = "FCF";
        break;
    case 0xFF:
        strErrCode = "MOGI";
        break;
    default:
        strErrCode = "";
        break;
    }
    return strErrCode;
}

QString CanSignalsSlots::getACCTempErrorInfo(int errCode){
    QString strErrCode = "";
    switch (errCode) {
    case 0x01:
        strErrCode = "PWF";  //空调电源故障
        break;
    case 0x02:
        strErrCode = "EVF";  //紧急通风逆变器故障
        break;
    case 0x03:
        strErrCode = "VEF11";  //空调机组1通风机1故障
        break;
    case 0x04:
        strErrCode = "VEF21";  //空调机组2通风机1故障
        break;
    case 0x05:
        strErrCode = "VEF12";  //空调机组1通风机2故障
        break;
    case 0x06:
        strErrCode = "VEF22";  //空调机组2通风机2故障
        break;
    case 0x07:
        strErrCode = "VEF13";  //空调机组1通风机3故障
        break;
    case 0x08:
        strErrCode = "VEF23";  //空调机组2通风机3故障
        break;
    case 0x09:
        strErrCode = "VEF14";  //空调机组1通风机4故障
        break;
    case 0x0A:
        strErrCode = "VEF24";  //空调机组2通风机4故障
        break;
    case 0x0B:
        strErrCode = "CPF11";  //空调机组1压缩机1过流故障
        break;
    case 0x0C:
        strErrCode = "CPF21";  //空调机组2压缩机1过流故障
        break;
    case 0x0D:
        strErrCode = "CPF12";  //空调机组1压缩机2过流故障
        break;
    case 0x0E:
        strErrCode = "CPF22";  //空调机组2压缩机2过流故障
        break;
    case 0x0F:
        strErrCode = "CPP11";  //空调机组1压缩机1压力保护
        break;
    case 0x10:
        strErrCode = "CPP21";  //空调机组2压缩机1压力保护
        break;
    case 0x11:
        strErrCode = "CPP12";  //空调机组1压缩机2压力保护
        break;
    case 0x12:
        strErrCode = "CPP22";  //空调机组2压缩机2压力保护
        break;
    case 0x13:
        strErrCode = "COF11";  //空调机组1冷凝风机1故障
        break;
    case 0x14:
        strErrCode = "COF21";  //空调机组2冷凝风机1故障
        break;
    case 0x15:
        strErrCode = "COF12";  //空调机组1冷凝风机2故障
        break;
    case 0x16:
        strErrCode = "COF22";  //空调机组2冷凝风机2故障
        break;
    case 0x17:
        strErrCode = "FTS1";  //新风温度传感器1故障
        break;
    case 0x18:
        strErrCode = "FTS2";  //新风温度传感器2故障
        break;
    case 0x19:
        strErrCode = "RTS1";  //回风温度传感器1故障
        break;
    case 0x1A:
        strErrCode = "RTS2";  //回风温度传感器2故障
        break;
    case 0x1B:
        strErrCode = "FAN1";  //废排风机1故障
        break;
    case 0x1C:
        strErrCode = "FAN2";  //废排风机2故障
        break;
    default:
        strErrCode = "";
        break;
    }
    return strErrCode;
}

//QString CanSignalsSlots::getACCSubDevErrorInfo(int accIndex, int errCode){
//    QString strErrCode = "";
//    switch (errCode) {
//    case 0x01:
//        strErrCode = "VEF11";
//        break;
//    case 0x02:
//        strErrCode = "VEF12";
//        break;
//    case 0x03:
//        strErrCode = "VEF13";
//        break;
//    case 0x04:
//        strErrCode = "VEF14";
//        break;
//    case 0x05:
//        strErrCode = "COF11";
//        break;
//    case 0x06:
//        strErrCode = "COF12";
//        break;
//    case 0x07:
//        strErrCode = "CPP11";
//        break;
//    case 0x08:
//        strErrCode = "CPF11";
//        break;
//    case 0x09:
//        strErrCode = "CPP12";
//        break;
//    case 0x0A:
//        strErrCode = "CPF12";
//        break;
//    case 0x0B:
//        strErrCode = "VEF21";
//        break;
//    case 0x0C:
//        strErrCode = "VEF22";
//        break;
//    case 0x0D:
//        strErrCode = "VEF23";
//        break;
//    case 0x0E:
//        strErrCode = "VEF24";
//        break;
//    case 0x0F:
//        strErrCode = "COF21";
//        break;
//    case 0x10:
//        strErrCode = "COF22";
//        break;
//    case 0x11:
//        strErrCode = "CPP21";
//        break;
//    case 0x12:
//        strErrCode = "CPF21";
//        break;
//    case 0x13:
//        strErrCode = "CPP22";
//        break;
//    case 0x14:
//        strErrCode = "CPF22";
//        break;
//    case 0x15:
//        strErrCode = "ACC1";
//        break;
//    case 0x16:
//        strErrCode = "ACC2";
//        break;
//    case 0x17:
//        strErrCode = "ACC3";
//        break;
//    case 0x18:
//        strErrCode = "ACC4";
//        break;
//    case 0x19:
//        strErrCode = "ACC5";
//        break;
//    case 0x1A:
//        strErrCode = "ACC6";
//        break;
//    case 0x1B:
//        strErrCode = "ACC7";
//        break;
//    case 0x1C:
//        strErrCode = "ACC8";
//        break;
//    default:
//        strErrCode = "";
//        break;
//    }
//    return strErrCode;
//}

void CanSignalsSlots::sendCarACCSetTempValue(){
    if(masterHMIFlag){
        ParaData_HMI[0] = carTempValue;
        canSendParaData1_Update();
    }
    else{
        TempData_HMI[0] = 1;
        TempData_HMI[1] = carTempValue;
        canSendTempData_Update();
    }
}

void CanSignalsSlots::sendCarParaData_Changed(CAR_ID carId, int dataType, int dataValue){
    if(2 == dataType){  //车辆编号更改
        SetParaData_HMI[0] = int(carId);
        SetParaData_HMI[1] = 1;
        SetParaData_HMI[2] = dataValue>>8 & 0xFF;
        SetParaData_HMI[3] = dataValue & 0xFF;
        int aaadata = SetParaData_HMI[2]<<8 | SetParaData_HMI[3];
        qDebug()<<"Car Para Set , Car Code Cahnge #####  datavalue == "<< dataValue<<"  H == "<<SetParaData_HMI[2]<<"   L == "<<SetParaData_HMI[3]<<"    Value  == "<<aaadata;
        canSendSetParaData_Update();
    }
    else{
        switch (dataType) {
        case 3:  //车轮直径更改
            if(m_CarWheel[int(carId-1) != dataValue]){
                if(masterHMIFlag){
                    m_CarWheel[int(carId-1)] = dataValue;
                    setDDUConfigInfo(3, int(carId), dataValue);
                    ParaData_HMI[int(carId)] = 840-dataValue;
                    canSendParaData1_Update();
                }
                else{
                    SetParaData_HMI[0] = int(carId);
                    SetParaData_HMI[1] = 2;
                    SetParaData_HMI[2] = dataValue>>8 & 0xFF;
                    SetParaData_HMI[3] = dataValue & 0xFF;
                    canSendSetParaData_Update();
                }
            }
            break;
        case 4:  //AS压力满更改
            if(m_CarASFull[int(carId-1) != dataValue]){
                if(masterHMIFlag){
                    m_CarASFull[int(carId-1)] = dataValue;
                    setDDUConfigInfo(4, int(carId), dataValue);
                    switch (carId) {
                    case CAR_1:
                        ParaData_HMI[7] = dataValue>>8 & 0xFF;
                        ParaData_HMI[8] = dataValue & 0xFF;
                        break;
                    case CAR_2:
                        ParaData_HMI[11] = dataValue>>8 & 0xFF;
                        ParaData_HMI[12] = dataValue & 0xFF;
                        break;
                    case CAR_3:
                        ParaData_HMI[15] = dataValue>>8 & 0xFF;
                        ParaData_HMI[16] = dataValue & 0xFF;
                        break;
                    case CAR_4:
                        ParaData_HMI[19] = dataValue>>8 & 0xFF;
                        ParaData_HMI[20] = dataValue & 0xFF;
                        break;
                    case CAR_5:
                        ParaData_HMI[23] = dataValue>>8 & 0xFF;
                        ParaData_HMI[24] = dataValue & 0xFF;
                        break;
                    case CAR_6:
                        ParaData_HMI[27] = dataValue>>8 & 0xFF;
                        ParaData_HMI[28] = dataValue & 0xFF;
                        break;
                    default:
                        break;
                    }
                    if(CAR_1 == carId || CAR_2 == carId){
                        canSendParaData2_Update();
                    }
                    else if(CAR_3 == carId || CAR_4 == carId){
                        canSendParaData3_Update();
                    }
                    else if(CAR_5 == carId || CAR_6 == carId){
                        canSendParaData4_Update();
                    }
                    else{
                        // do nothing
                    }
                }
                else{
                    SetParaData_HMI[0] = int(carId);
                    SetParaData_HMI[1] = 3;
                    SetParaData_HMI[2] = dataValue>>8 & 0xFF;
                    SetParaData_HMI[3] = dataValue & 0xFF;
                    canSendSetParaData_Update();
                }
            }
            break;
        case 5:  //AS压力空更改
            if(m_CarASEmpty[int(carId-1) != dataValue]){
                if(masterHMIFlag){
                    m_CarASEmpty[int(carId-1)] = dataValue;
                    setDDUConfigInfo(5, int(carId), dataValue);
                    switch (carId) {
                    case CAR_1:
                        ParaData_HMI[9] = dataValue>>8 & 0xFF;
                        ParaData_HMI[10] = dataValue & 0xFF;
                        break;
                    case CAR_2:
                        ParaData_HMI[13] = dataValue>>8 & 0xFF;
                        ParaData_HMI[14] = dataValue & 0xFF;
                        break;
                    case CAR_3:
                        ParaData_HMI[17] = dataValue>>8 & 0xFF;
                        ParaData_HMI[18] = dataValue & 0xFF;
                        break;
                    case CAR_4:
                        ParaData_HMI[21] = dataValue>>8 & 0xFF;
                        ParaData_HMI[22] = dataValue & 0xFF;
                        break;
                    case CAR_5:
                        ParaData_HMI[25] = dataValue>>8 & 0xFF;
                        ParaData_HMI[26] = dataValue & 0xFF;
                        break;
                    case CAR_6:
                        ParaData_HMI[29] = dataValue>>8 & 0xFF;
                        ParaData_HMI[30] = dataValue & 0xFF;
                        break;
                    default:
                        break;
                    }
                    if(CAR_1 == carId || CAR_2 == carId){
                        canSendParaData2_Update();
                    }
                    else if(CAR_3 == carId || CAR_4 == carId){
                        canSendParaData3_Update();
                    }
                    else if(CAR_5 == carId || CAR_6 == carId){
                        canSendParaData4_Update();
                    }
                    else{
                        // do nothing
                    }
                }
                else{
                    SetParaData_HMI[0] = int(carId);
                    SetParaData_HMI[1] = 4;
                    SetParaData_HMI[2] = dataValue>>8 & 0xFF;
                    SetParaData_HMI[3] = dataValue & 0xFF;
                    canSendSetParaData_Update();
                }
            }
            break;
        default:
            break;
        }
    }
}

void CanSignalsSlots::sendCarGetFaultInfoHMI(int iValue){
    if(0 != iValue){
        GetFaultInfo_HMI = iValue;

        if(3 == iValue){
            qDeleteAll(carSpeedDataList);
            carSpeedDataList.clear();
        }
        canSendGetFaultInfo_Update();
    }
}

void CanSignalsSlots::sendCarGetFaultDetailDataHMI(int errType, int carId){
    if(errType > 0){
        GetFaultData_HMI[0] = errType;
        GetFaultData_HMI[1] = carId;

        switch (errType) {
        case 1:   //故障履历
            switch (CAR_ID(carId)) {
            case CAR_1:
                qDeleteAll(car1FaultList);
                car1FaultList.clear();
                break;
            case CAR_2:
                qDeleteAll(car2FaultList);
                car2FaultList.clear();
                break;
            case CAR_3:
                qDeleteAll(car3FaultList);
                car3FaultList.clear();
                break;
            case CAR_4:
                qDeleteAll(car4FaultList);
                car4FaultList.clear();
                break;
            case CAR_5:
                qDeleteAll(car5FaultList);
                car5FaultList.clear();
                break;
            case CAR_6:
                qDeleteAll(car6FaultList);
                car6FaultList.clear();
                break;
            default:
                break;
            }
            break;
        case 2:  //VVVF1
            switch (CAR_ID(carId)) {
            case CAR_1:
                qDeleteAll(car1VVVF1ErrList);
                car1VVVF1ErrList.clear();
                break;
            case CAR_3:
                qDeleteAll(car3VVVF1ErrList);
                car3VVVF1ErrList.clear();
                break;
            case CAR_6:
                qDeleteAll(car6VVVF1ErrList);
                car6VVVF1ErrList.clear();
                break;
            default:
                break;
            }
            break;
        case 3:  //VVVF2
            switch (CAR_ID(carId)) {
            case CAR_1:
                qDeleteAll(car1VVVF2ErrList);
                car1VVVF2ErrList.clear();
                break;
            case CAR_3:
                qDeleteAll(car3VVVF2ErrList);
                car3VVVF2ErrList.clear();
                break;
            case CAR_6:
                qDeleteAll(car6VVVF2ErrList);
                car6VVVF2ErrList.clear();
                break;
            default:
                break;
            }
            break;
        case 4:  //SIV
            switch (CAR_ID(carId)) {
            case CAR_2:
                qDeleteAll(car2SIVErrList);
                car2SIVErrList.clear();
                break;
            case CAR_4:
                qDeleteAll(car4SIVErrList);
                car4SIVErrList.clear();
                break;
            default:
                break;
            }
            break;
        case 5:  //BCU
            switch (CAR_ID(carId)) {
            case CAR_1:
                qDeleteAll(car1BCUErrList);
                car1BCUErrList.clear();
                break;
            case CAR_2:
                qDeleteAll(car2BCUErrList);
                car2BCUErrList.clear();
                break;
            case CAR_3:
                qDeleteAll(car3BCUErrList);
                car3BCUErrList.clear();
                break;
            case CAR_4:
                qDeleteAll(car4BCUErrList);
                car4BCUErrList.clear();
                break;
            case CAR_5:
                qDeleteAll(car5BCUErrList);
                car5BCUErrList.clear();
                break;
            case CAR_6:
                qDeleteAll(car6BCUErrList);
                car6BCUErrList.clear();
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        canSendGetFaultData_Update();
    }
}

void CanSignalsSlots::sendCarDeleteRecordDataHMI(int iType, int iItem){
    if(0 != iType && 0 != iItem){
        DeleteData_HMI[0] = iType;
        DeleteData_HMI[1] = iItem;
        DeleteData_HMI[2] = 1;

        canSendDeleteData_Update();
    }
}

void CanSignalsSlots::sendSDOCheckData(int iFlag){
    canSendSDOData(iFlag, 1);
    canSendSDOData(iFlag, 2);
}

void CanSignalsSlots::sendBCUCheckData(){
    for(int i = 0; i < 10; i++){
        BCUCheck_Data[i*13+0] = mBCUCheckSendData[i*7+0];
        BCUCheck_Data[i*13+1] = (mBCUCheckSendData[i*7+1] >> 8) & 0xFF;
        BCUCheck_Data[i*13+2] = mBCUCheckSendData[i*7+1] & 0xFF;
        BCUCheck_Data[i*13+3] = (mBCUCheckSendData[i*7+2] >> 8) & 0xFF;
        BCUCheck_Data[i*13+4] = mBCUCheckSendData[i*7+2] & 0xFF;
        BCUCheck_Data[i*13+5] = (mBCUCheckSendData[i*7+3] >> 8) & 0xFF;
        BCUCheck_Data[i*13+6] = mBCUCheckSendData[i*7+3] & 0xFF;
        BCUCheck_Data[i*13+7] = (mBCUCheckSendData[i*7+4] >> 8) & 0xFF;
        BCUCheck_Data[i*13+8] = mBCUCheckSendData[i*7+4] & 0xFF;
        BCUCheck_Data[i*13+9] = (mBCUCheckSendData[i*7+5] >> 8) & 0xFF;
        BCUCheck_Data[i*13+10] = mBCUCheckSendData[i*7+5] & 0xFF;
        BCUCheck_Data[i*13+11] = (mBCUCheckSendData[i*7+6] >> 8) & 0xFF;
        BCUCheck_Data[i*13+12] = mBCUCheckSendData[i*7+6] & 0xFF;
    }
    sendSDOCheckData(4);
}

void CanSignalsSlots::sendSpeedCheckData(int iERMFlag){
    if(mSpeedCheckList.length() > 0){
        if(1 == iERMFlag && mSpeedCheckSendData[1] <= mSpeedCheckList.length()){
            int iSpeedIndex = mSpeedCheckSendData[1]-1;
            SpeedCheck_Data[0] = mSpeedCheckList.at(iSpeedIndex)->strDate.section("-", 0, 0).toInt();
            SpeedCheck_Data[1] = mSpeedCheckList.at(iSpeedIndex)->strDate.section("-", 1, 1).toInt();
            SpeedCheck_Data[2] = mSpeedCheckList.at(iSpeedIndex)->strDate.section("-", 2, 2).toInt();
            SpeedCheck_Data[3] = mSpeedCheckList.at(iSpeedIndex)->strDate.section(":", 0, 0).toInt();
            SpeedCheck_Data[4] = mSpeedCheckList.at(iSpeedIndex)->strDate.section(":", 1, 1).toInt();
            SpeedCheck_Data[5] = mSpeedCheckList.at(iSpeedIndex)->strDate.section(":", 2, 2).toInt();
            SpeedCheck_Data[6] = mSpeedCheckList.at(iSpeedIndex)->iCheckType;
            SpeedCheck_Data[7] = mSpeedCheckList.at(iSpeedIndex)->iSpeedValue;
            SpeedCheck_Data[8] = mSpeedCheckList.at(iSpeedIndex)->iPWMValue;
            SpeedCheck_Data[9] = mSpeedCheckList.at(iSpeedIndex)->iRebornFlag;
            SpeedCheck_Data[10] = mSpeedCheckList.at(iSpeedIndex)->iBrakeSpeed;
            SpeedCheck_Data[11] = (mSpeedCheckList.at(iSpeedIndex)->iBrakeDistance>>8) & 0xFF;
            SpeedCheck_Data[12] = mSpeedCheckList.at(iSpeedIndex)->iBrakeDistance & 0xFF;

            canSendSpeedCheckData_Update(1);
        }
        else if(2 == iERMFlag && mSpeedCheckSendData[2] <= mSpeedCheckList.length()){
            int iSpeedIndex = mSpeedCheckSendData[2]-1;
            SpeedCheck_Data[0] = mSpeedCheckList.at(iSpeedIndex)->strDate.section("-", 0, 0).toInt();
            SpeedCheck_Data[1] = mSpeedCheckList.at(iSpeedIndex)->strDate.section("-", 1, 1).toInt();
            SpeedCheck_Data[2] = mSpeedCheckList.at(iSpeedIndex)->strDate.section("-", 2, 2).toInt();
            SpeedCheck_Data[3] = mSpeedCheckList.at(iSpeedIndex)->strDate.section(":", 0, 0).toInt();
            SpeedCheck_Data[4] = mSpeedCheckList.at(iSpeedIndex)->strDate.section(":", 1, 1).toInt();
            SpeedCheck_Data[5] = mSpeedCheckList.at(iSpeedIndex)->strDate.section(":", 2, 2).toInt();
            SpeedCheck_Data[6] = mSpeedCheckList.at(iSpeedIndex)->iCheckType;
            SpeedCheck_Data[7] = mSpeedCheckList.at(iSpeedIndex)->iSpeedValue;
            SpeedCheck_Data[8] = mSpeedCheckList.at(iSpeedIndex)->iPWMValue;
            SpeedCheck_Data[9] = mSpeedCheckList.at(iSpeedIndex)->iRebornFlag;
            SpeedCheck_Data[10] = mSpeedCheckList.at(iSpeedIndex)->iBrakeSpeed;
            SpeedCheck_Data[11] = (mSpeedCheckList.at(iSpeedIndex)->iBrakeDistance>>8) & 0xFF;
            SpeedCheck_Data[12] = mSpeedCheckList.at(iSpeedIndex)->iBrakeDistance & 0xFF;

            canSendSpeedCheckData_Update(2);
        }
        else{
            // do nothing
        }
        sendSDOCheckData(5);
    }
}

QString CanSignalsSlots::getCarStationName(int stationId){
    QString strName = "";
    for(int i = 0; i < stationCodeList.length(); i++){
        if(stationCodeList.at(i) == stationId){
            strName = stationNameList.at(i);
            break;
        }
    }
    return strName;
}

void CanSignalsSlots::CanSignalsSlots::slot_1S_Timer_Out(){
    QString strDateTime = QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss");

    emit sig_1S_Timer_Out(strDateTime);
}

void CanSignalsSlots::slot_120S_Timer_Out(){
    if(masterHMIFlag && !atoModelFlag){
        QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
        int iYear = strDateTime.section("-", 0, 0).toInt()%100;
        int iMonth = strDateTime.section("-", 1, 1).toInt();
        int iDay = strDateTime.section("-", 2, 2).toInt();
        int iHour = strDateTime.section("-", 3, 3).toInt();
        int iMinute = strDateTime.section("-", 4, 4).toInt();
        int iSecond = strDateTime.section("-", 5, 5).toInt();
        sendCarDateTimeHMI(iYear, iMonth, iDay, iHour, iMinute, iSecond);
    }
}

void CanSignalsSlots::slot_CCUJudge_Time_Out(){
    bCCUJudgeFlag = true;

    if(1 == TMSStateData_CCU1[2]){  //CCU1 Master
        for(int i = 0; i < 8; i++){
            bTMSDevComnErrFlag[i] = getCanDataBitFromByte(TMSStateData_CCU1[0], i);
        }
        for(int i = 8; i < 12; i++){
            bTMSDevComnErrFlag[i] = getCanDataBitFromByte(TMSStateData_CCU1[1], i-8);
        }

        canWorkFlag = (1 == TMSStateData_CCU1[4]) ? CAN_1 : (2 == TMSStateData_CCU1[4]) ? CAN_2 : CAN_NONE;
    }
    else if(1 != TMSStateData_CCU1[2] && 1 == TMSStateData_CCU2[3]){  //CCU1 No Master && CCU2 Master
        for(int i = 0; i < 8; i++){
            bTMSDevComnErrFlag[i] = getCanDataBitFromByte(TMSStateData_CCU2[0], i);
        }
        for(int i = 8; i < 12; i++){
            bTMSDevComnErrFlag[i] = getCanDataBitFromByte(TMSStateData_CCU2[1], i-8);
        }

        canWorkFlag = (1 == TMSStateData_CCU2[4]) ? CAN_1 : (2 == TMSStateData_CCU2[4]) ? CAN_2 : CAN_NONE;
    }
    else{
        //do nothing
    }

    if(!bCCUJudgeFirstFlag){
        if(!timer_400ms->isActive()){
            timer_400ms->start(HMI_REFRESH_TIME_OUT);
        }

        mThreadSendPDO->startPDOSend();
        mThreadDistance->startThreadRun();
        mThreadFaultRecord->startThreadRun();

        bCCUJudgeFirstFlag = true;
    }

    TMSStateData_CCU1[0] = 0xFF;
    TMSStateData_CCU1[1] = 0xF0;
    TMSStateData_CCU1[2] = 0;
    TMSStateData_CCU1[3] = 0;
    TMSStateData_CCU1[4] = 0;
    TMSStateData_CCU2[0] = 0xFF;
    TMSStateData_CCU2[1] = 0xF0;
    TMSStateData_CCU2[2] = 0;
    TMSStateData_CCU2[3] = 0;
    TMSStateData_CCU2[4] = 0;

    bCCUJudgeFlag = false;
}

void CanSignalsSlots::slot_ScreenData_Refresh_400MS(){
    if(masterHMIFlag){
        car_Header_Update();
        car_ATOModel_Update();

        car_BrakePower_Up_Update();
        car_Load_Rate_AVG_Update();
        car_TotalWeight_Update();
    }
//    car_SubDev_ComnErrData_Update();

    car_DI_Data_MC1_Update();
    car_DI_Data_T_Update();
    car_DI_Data_M_Update();
    car_DI_Data_T1_Update();
    car_DI_Data_T2_Update();
    car_DI_Data_MC2_Update();

    car_Car_Data_HMI_Update();

    getCarDestinationID();
    getCarNowStationID();
    getCarNextStationID();

    car_Expand_Elec_Update();
    car_VVVF_BCU_Level_Update();

    for(int i = 0; i < 6; i++){
        car_DoorData_Update(CAR_ID(i+1));
        car_ACCData_Update(CAR_ID(i+1));
        car_BCU_BCKpa_Update(CAR_ID(i+1));
        car_Load_Rate_Update(CAR_ID(i+1));
        car_BCUData_4_Update(CAR_ID(i+1));
    }

    if(masterHMIFlag){
        car_Door_OpenClose_State_Update();
    }

    car_ACC_Ctrl_Mode_Update();

    car_ATCData_1_Update(CAR_1);
    car_ATCData_2_Update(CAR_1);
    car_ATCData_3_Update(CAR_1);

    car_Urgent_Alert_Update();

    for(int i = 0; i < 2; i++){
        car_VVVF_Elec_BCU_Update(CAR_1, i+1);
        car_VVVF_Elec_BCU_Update(CAR_3, i+1);
        car_VVVF_Elec_BCU_Update(CAR_6, i+1);
        car_VVVFData_5_Update(CAR_1, i+1);
        car_VVVFData_5_Update(CAR_3, i+1);
        car_VVVFData_5_Update(CAR_6, i+1);
    }

    car_VVVFData_2_Update();

    //获取车辆速度值
    car_VVVFData_3_Update(/*CAR_1, 1*/);
    //获取车辆电压值
    getCarVoltage();
    //获取车辆电流值
    getCarCurrent();

    car_SIV_Voltage_Freq_Update(CAR_2);
    car_SIV_Voltage_Freq_Update(CAR_4);

    car_SIVData_3_Update(CAR_2);
    car_SIVData_3_Update(CAR_4);
}

void CanSignalsSlots::slot_ACP_Check_Start(){
    emit sig_AirPump_State_Change(CAR_1, getCanDataBitFromByte(DI_Data_MC1[1], 4));
    emit sig_AirPump_State_Change(CAR_6, getCanDataBitFromByte(DI_Data_MC2[1], 4));
}

void CanSignalsSlots::slot_SIV_Check_Start(){
    emit sig_SIV_Voltage_Freq_Update(CAR_2, mSIV_State_Data[0][0], mSIV_State_Data[0][1], mSIV_State_Data[0][2]);
    emit sig_SIV_Voltage_Freq_Update(CAR_4, mSIV_State_Data[1][0], mSIV_State_Data[1][1], mSIV_State_Data[1][2]);
}

void CanSignalsSlots::slot_Speed_Check_Start(){
    emit sig_VVVF_BCU_Level_Update(mVVVF_BCU_Level_Data[0], mVVVF_BCU_Level_Data[1]);
    emit sig_VVVF_Elec_BCU_Update(CAR_1, 1, mVVVF_Elec_BCU_Data[0][0], mVVVF_Elec_BCU_Data[0][1]);
    emit sig_VVVF_Elec_BCU_Update(CAR_1, 2, mVVVF_Elec_BCU_Data[1][0], mVVVF_Elec_BCU_Data[1][1]);
    emit sig_VVVF_Elec_BCU_Update(CAR_3, 1, mVVVF_Elec_BCU_Data[2][0], mVVVF_Elec_BCU_Data[2][1]);
    emit sig_VVVF_Elec_BCU_Update(CAR_3, 2, mVVVF_Elec_BCU_Data[3][0], mVVVF_Elec_BCU_Data[3][1]);
    emit sig_VVVF_Elec_BCU_Update(CAR_6, 1, mVVVF_Elec_BCU_Data[4][0], mVVVF_Elec_BCU_Data[4][1]);
    emit sig_VVVF_Elec_BCU_Update(CAR_6, 2, mVVVF_Elec_BCU_Data[5][0], mVVVF_Elec_BCU_Data[5][1]);
    for(int i = 0; i < 6; i++){
        emit sig_BCU_BC_Kpa_Update(CAR_ID(i+1), mBCU_BCKpa_Data[i][0], mBCU_BCKpa_Data[i][1]);
    }
    emit sig_BHB_BLB_State_Change(CAR_2, getCanDataBitFromByte(DI_Data_T[0], 0), getCanDataBitFromByte(DI_Data_T[0], 1));
    emit sig_BHB_BLB_State_Change(CAR_3, getCanDataBitFromByte(DI_Data_M[0], 0), getCanDataBitFromByte(DI_Data_M[0], 1));
    emit sig_UrgentShort(CAR_1, getCanDataBitFromByte(DI_Data_MC1[1], 3));
    emit sig_UrgentShort(CAR_6, getCanDataBitFromByte(DI_Data_MC2[1], 3));
}

void CanSignalsSlots::slot_CanWorkState_Update(){
    qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$44   slot_CanWorkState_Update";
    if(can1OperationalFlag || can2OperationalFlag){
        if(!timerCCUJudge->isActive()){
            timerCCUJudge->start(CCU_JUDGE_TIME_OUT);
        }
    }
}
