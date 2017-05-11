#ifndef CANSIGNALSSLOTS_H
#define CANSIGNALSSLOTS_H

#include <QObject>
#include <QDateTime>
#include <QTimer>
#include <QSettings>

#include "gui_config.h"
#include "threadsendpdo.h"
#include "threaddistance.h"
#include "threadfaultrecord.h"

class CanSignalsSlots : public QObject
{
    Q_OBJECT
public:
    explicit CanSignalsSlots(QObject *parent = 0);

    void tempValueInit();
    void getDDUConfigInfo();
    void setDDUConfigInfo(int dataFlag, int carId, int iValue);
    void getCarStationInfo();
    bool getCanDataBitFromByte(u_int8_t byteValue, int indexValue);
    u_int8_t setCanDataBitFromByte(u_int8_t byteValue, int indexValue, int iValue);
    int getRealValueFromByte(u_int8_t data);

    void masterHMIUpdate();
    void canWorkStateUpdate();

    void car_DI_Data_MC1_Update();
    void car_DI_Data_T_Update();
    void car_DI_Data_M_Update();
    void car_DI_Data_T1_Update();
    void car_DI_Data_T2_Update();
    void car_DI_Data_MC2_Update();
    void car_Car_Data_HMI_Update();
    void car_DateTime_HMI_Update();
    void car_DoorData_Update(CAR_ID carId);
    void car_ACCData_Update(CAR_ID carId);
    void car_ACCData_2_Update();
    void car_BCUData_Update(CAR_ID carId, int dataIndex);
    void car_BCUData_4_Update(CAR_ID carId);
    void car_ATCData_1_Update(CAR_ID carId);
    void car_ATCData_2_Update(CAR_ID carId);
    void car_ATCData_3_Update(CAR_ID carId);
    void car_ATCData_4_Update(CAR_ID carId);
    void car_ATCDateTime_Update();
    void car_PISData_1_Update(CAR_ID carId);
    void car_PISData_2_Update(CAR_ID carId);
    void car_VVVFData_1_Update(CAR_ID carId, int vvvfIndex);
    void car_VVVFData_2_Update(/*CAR_ID carId, int vvvfIndex*/);
    void car_VVVFData_3_Update(/*CAR_ID carId, int vvvfIndex*/);
    void car_VVVFData_4_Update(/*CAR_ID carId, int vvvfIndex*/);
    void car_VVVFData_5_Update(CAR_ID carId, int vvvfIndex);
    void car_SIVData_1_Update(CAR_ID carId);
    void car_SIVData_2_Update(CAR_ID carId);
    void car_SIVData_3_Update(CAR_ID carId);
    void car_SubDev_ComnErrData_Update();
//    void car_TMSStateData_CCU_Update();
    void car_ParaData_7B_HMI_Update();
    void car_ParaData_8B_HMI_Update(int dataIndex);
    void car_TempData_HMI_Update();
    void car_TMSDevStartData_Update();
    void car_FaultDataCount_Update();
    void car_TramCode_Update(CAR_ID carId);
    void car_SetParaData_HMI_Update();
    void car_SubDev_FaultData_Sort(int iDevFalg);
    void car_SubDev_FaultData_Update(int iDataIndex);
    void car_PTU_CMD_Update();

    void car_Header_Update();
    void car_ATOModel_Update();
    void car_VVVF_BCU_Level_Update();
    void car_VVVF_Elec_BCU_Update(CAR_ID carId, int vvvfIndex);
    void car_BCU_BCKpa_Update(CAR_ID carId);
    void car_SIV_Voltage_Freq_Update(CAR_ID carId);
    void car_Expand_Elec_Update();
    void car_Direction_Update();
    void car_Urgent_Alert_Update();
    void car_ACC_SetInfo_Update(CAR_ID carId);
    void car_ACC_Temp_Update(CAR_ID carId);
    void car_ACC_Ctrl_Mode_Update();
    void car_Load_Rate_Update(CAR_ID carId);
    void car_Load_Rate_AVG_Update();
    void car_TotalWeight_Update();
    void car_DoorOperation_Update(CAR_ID carId);
    void car_Door_OpenClose_State_Update();
    void car_BrakePower_Up_Update();
    void car_Speed_Door_State_Update();
    void car_FaultInfo_ERM_Update(int mERMFlag);
    void car_RunInfo_ERM_Update();
    void car_AllSpeedInfo_ERM_Update(int mERM);

    void car_ERM_DelResult_Update();

    RUN_DOOR_STATE getDoor_State(int value);
    void door_OneSideState_Update(CAR_ID carId, DOOR_ID doorId);

    int getACCWorkModel(CAR_ID carId, int accIndex);
    int getACCBreakdownCode(CAR_ID carId, int accIndex);
    int getACCCtrlMode(u_int8_t data);
    void getCarVoltage();
    void getCarCurrent();
    int getVVVFError(CAR_ID carId, int vvvfIndex);
    int getBCUError(CAR_ID carId);
    int getSIVError(CAR_ID carId);
    int getACCError(CAR_ID carId);
    void getCarDestinationID();
    void getCarNowStationID();
    void getCarNextStationID();
    int getBCUErrorCount(u_int8_t bcuData);

    QString getDoorErrorInfo(int errCode);
    QString getVVVFErrorInfo(int errCode);
    QString getBCUErrorInfo(int errCode);
    QString getSIVErrorInfo(int errCode);
    QString getACCTempErrorInfo(int errCode);
//    QString getACCSubDevErrorInfo(int accIndex, int errCode);
    QString getCarStationName(int stationId);

    void setCarStationManual(int curStaId, int endStaId);

    void car_FaultRecordOperation_Update(CAR_ID carId, int devCode, int errCode, bool bFlag);

    void car_SysDateTime_Update();
    void car_HMI_2_SetDateTime();

    void car_AnotherHMIGetFaultData_Update();
    void car_AnotherHMIGetSpeedData_Update();

    void sendCarDateTimeHMI(int iYear, int iMonth, int iDay, int iHour, int iMinute, int iSecond);
    void sendCarACCSetTempValue();
    void sendCarParaData_Changed(CAR_ID carId, int dataType, int dataValue);
    void sendCarGetFaultInfoHMI(int iValue);  //0:默认,1:查询各车故障履历条数,2:查询列车运行记录数据
    void sendCarGetFaultDetailDataHMI(int errType, int carId);
    void sendCarDeleteRecordDataHMI(int iType, int iItem);

    void sendSDOCheckData(int iFlag);  //iFlag :: 0:默认,1:门检查结果数据,2:空压机检查结果数据,3:SIV检查结果数据,4:制动试验结果数据,5:加减速度测定数据;
    void sendBCUCheckData();
    void sendSpeedCheckData(int iERMFlag);

signals:
    void sig_1S_Timer_Out(QString strDateTime);
    void sig_CanWorkState_Update();

    void sig_Car_Code_Change();
    void sig_Car_Direction_Change();
    void sig_Car_ATOModel_Change(int model);
    void sig_Door_State_Change(CAR_ID carId, DOOR_ID doorId, RUN_DOOR_STATE doorState);
    void sig_ACC_SetTempValue_Update();
    void sig_ACCData_Change(CAR_ID carId, int iState, int acc1Mode, int acc2Mode, int acc1Err, int acc2Err);
    void sig_ACCCtrlMode(int ctrlMode);
    void sig_AirPump_State_Change(CAR_ID carId, bool bState);
    void sig_SIV_Direction_Change(SIV_TX_DIRECTION sivDirection);
    void sig_BHB_BLB_State_Change(CAR_ID carId, bool bBHBFlag, bool bBLBFlag);
    void sig_Urgent_Alert_Update(CAR_ID carId, bool bState);
    void sig_UrgentShort(CAR_ID carId, bool bState);
    void sig_BrakeIsolateDoor_Change(CAR_ID carId, bool bState);
    void sig_HB_OFF_Update(bool bState);
    void sig_VVVF_BCU_Level_Update(int iState, int iValue);
    void sig_VVVF_Elec_BCU_Update(CAR_ID carId, int iIndex, int iState, int iValue);
    void sig_BCU_BC_Kpa_Update(CAR_ID carId, int iState, int iValue);
    void sig_SIV_Voltage_Freq_Update(CAR_ID carId, int iState, int iValue1, int iValue2);
    void sig_ACC_Temp_Update(CAR_ID carId, int iState, int iValue1, int iValue2);
    void sig_Load_Rate_Update(CAR_ID carId, int iState, int iValue);
    void sig_Car_Arrive_Station_Update(int iFalg, int stationId);
    void sig_Car_ParaData_Update(int dataIndex);
    void sig_Car_DoorOperation_Update(int doorFlag, int operation);   //doorFlag 0:默认;1:左侧门;2:右侧门;    operation 0:默认;1:门开;2:门关
    void sig_Car_DIDoorOperation_Update(CAR_ID carid);
    void sig_Speed_Door_State_Update(int iValue);
    void sig_FaultRecordCount_Get_Update();
    void sig_FaultRecordData_Get_Update();
    void sig_VVVF_Elec_Brake_State_Update(bool bState);
    void sig_RunRecordData_Get_Update();
    void sig_SpeedRecordData_Get_Update(int flag);
    void sig_PTUDownloadState_Update(bool bFlag);

public slots:
    void slot_1S_Timer_Out();
    void slot_120S_Timer_Out();
    void slot_CCUJudge_Time_Out();
    void slot_ScreenData_Refresh_400MS();

    void slot_ACP_Check_Start();
    void slot_SIV_Check_Start();
    void slot_Speed_Check_Start();

    void slot_CanWorkState_Update();

public:
    int m_CarWheel[6];
    int m_CarASFull[6];
    int m_CarASEmpty[6];
    int m_BCULoadRate[6];
    int m_BCUASPress[6];
    int m_BCUBCPress[6];
    int m_CarPWMValue;
    QList<int> stationCodeList;
    QStringList stationNameList;
    int iCurrStation, iNextStation, iEndStation;
    int doorMC1State[8];
    int doorTState[8];
    int doorMState[8];
    int doorT1State[8];
    int doorT2State[8];
    int doorMC2State[8];
    bool bATODateTimeUpdateFlag, bATOModeFirstFlag;
    bool bCarRunStopFlag;   //true: 列车停止，false:列车运行；
    float fCurStationDistance;   //列车启动后与当前站的距离

    int m_FaultRecordCount[6];
    int m_VVVF1FaultCount[3];
    int m_VVVF2FaultCount[3];
    int m_SIVFaultCount[2];
    int m_BCUFaultCount[6];

    int mVVVFFirstFaultCode[6];
    int mBCUFirstFaultCode[6];
    int mACCFirstFaultCode[6];
    int mSIVFirstFaultCode[2];
    int mDoorFirstFaultCode[48];

    int mBCUCheckSendData[70];

    int mSpeedCheckSendData[3];
    QList<structSpeedCheck*> mSpeedCheckList;

    QList<structFaultRecordData*> car1FaultList;
    QList<structFaultRecordData*> car2FaultList;
    QList<structFaultRecordData*> car3FaultList;
    QList<structFaultRecordData*> car4FaultList;
    QList<structFaultRecordData*> car5FaultList;
    QList<structFaultRecordData*> car6FaultList;
    QList<structSubDevErr*> car1VVVF1ErrList;
    QList<structSubDevErr*> car1VVVF2ErrList;
    QList<structSubDevErr*> car3VVVF1ErrList;
    QList<structSubDevErr*> car3VVVF2ErrList;
    QList<structSubDevErr*> car6VVVF1ErrList;
    QList<structSubDevErr*> car6VVVF2ErrList;
    QList<structSubDevErr*> car2SIVErrList;
    QList<structSubDevErr*> car4SIVErrList;
    QList<structSubDevErr*> car1BCUErrList;
    QList<structSubDevErr*> car2BCUErrList;
    QList<structSubDevErr*> car3BCUErrList;
    QList<structSubDevErr*> car4BCUErrList;
    QList<structSubDevErr*> car5BCUErrList;
    QList<structSubDevErr*> car6BCUErrList;
    QList<structSpeedCheck*> carSpeedDataList;
    structRunDataInfo carRunDataInfo;

    bool bTMSDevComnErrFlag[12];  //CCU发送TMS设备通讯状态，默认为通讯异常

    u_int8_t u8DoorOperationData[2];   //门开闭操作状态数据，脉冲数据

    bool bNowShowERMFaultFlag;
    bool bNowShowSubDevErrFlag;
    bool bNowShowSpeedFlag;

private:
    QTimer * timer_1S;
    QTimer * timerCCUJudge;
    QTimer * timer_400ms;

    ThreadSendPDO * mThreadSendPDO;
    ThreadDistance * mThreadDistance;
    ThreadFaultRecord * mThreadFaultRecord;

    bool bCCUJudgeFirstFlag;
};

#endif // CANSIGNALSSLOTS_H
