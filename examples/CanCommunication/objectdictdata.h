#ifndef OBJECTDICTDATA_H
#define OBJECTDICTDATA_H

#include "applicfg.h"

//将所有数据转换为数组变量存储
//硬线采集数据
extern UNS8 DI_Data_MC1[7];
extern UNS8 DI_Data_T[2];
extern UNS8 DI_Data_M[2];
extern UNS8 DI_Data_T1[2];
extern UNS8 DI_Data_T2[2];
extern UNS8 DI_Data_MC2[7];
extern UNS8 DI_Data_ERM1[7];
extern UNS8 DI_Data_ERM2[7];
//HMI发送列车综合数据和时间数据
extern UNS8 Car_Data_HMI[8];
extern UNS8 Date_Time_HMI[7];
extern UNS8 SetDate_Time_HMI[7];
//门数据
extern UNS8 DoorData_MC1[32];
extern UNS8 DoorData_T[32];
extern UNS8 DoorData_M[32];
extern UNS8 DoorData_T1[32];
extern UNS8 DoorData_T2[32];
extern UNS8 DoorData_MC2[32];
//空调ACC数据
extern UNS8 ACCData_MC1[9];
extern UNS8 ACCData_T[9];
extern UNS8 ACCData_M[9];
extern UNS8 ACCData_T1[9];
extern UNS8 ACCData_T2[9];
extern UNS8 ACCData_MC2[9];
//制动控制BCU数据
extern UNS8 BCUData_MC1[25];
extern UNS8 BCUData_T[25];
extern UNS8 BCUData_M[25];
extern UNS8 BCUData_T1[25];
extern UNS8 BCUData_T2[25];
extern UNS8 BCUData_MC2[25];
//信号ATC数据
extern UNS8 ATCData_MC1[27];
extern UNS8 ATCDateTime_MC1[6];
extern UNS8 ATCData_MC2[27];
extern UNS8 ATCDateTime_MC2[6];
//广播PIS数据
extern UNS8 PISData_MC1[11];
extern UNS8 PISData_MC2[11];
//牵引VVVF数据
extern UNS8 VVVF1Data_MC1[36];
extern UNS8 VVVF2Data_MC1[36];
extern UNS8 VVVF1Data_M[36];
extern UNS8 VVVF2Data_M[36];
extern UNS8 VVVF1Data_MC2[36];
extern UNS8 VVVF2Data_MC2[36];
//扩展电源SIV数据
extern UNS8 SIVData_T[18];
extern UNS8 SIVData_T1[18];
//子设备通讯异常数据
extern UNS8 SubDev_ComnErr_MC1[2];
extern UNS8 SubDev_ComnErr_T[2];
extern UNS8 SubDev_ComnErr_M[2];
extern UNS8 SubDev_ComnErr_T1[2];
extern UNS8 SubDev_ComnErr_T2[2];
extern UNS8 SubDev_ComnErr_MC2[2];
//列出TMS设备状态及通讯异常数据
extern UNS8 TMSStateData_CCU1[5];
extern UNS8 TMSStateData_CCU2[5];
//激活端HMI参数设置发送数据
extern UNS8 SetParaData_HMI[4];
extern UNS8 HMISetParaData_HMI[4];
//HMI获取单车某个子设备故障信息或者ERM存储故障履历信息时发送
extern UNS8 GetFaultData_HMI[2];
extern UNS8 M_GetFaultData_HMI[2];
//列出参数数据：编号，车轮直径，AS压力空，AS压力满
extern UNS8 ParaData_HMI[31];
//列出空调设置温度数据值
extern UNS8 TempData_HMI[2];
//维修菜单操作：查询故障履历总数，列车运行记录或加减速度测定数据
extern UNS8 GetFaultInfo_HMI;
extern UNS8 M_GetFaultInfo_HMI;
//维修菜单操作：激活端HMI删除记录指令，HMI发送初始设定数据
extern UNS8 DeleteData_HMI[3];
extern UNS8 InitData_HMI[8];
//CCU发送所有节点设备上线状态
extern UNS8 TMSDevStart_CCU;
//ERM发送各车故障履历总数
extern UNS8 FltDataCount_ERM[7];
//空压机电源数据
extern UNS8 AirPumpData_MC1[11];
extern UNS8 AirPumpData_MC2[11];
//主HMI发送列车到站信息
extern UNS8 CarArriveData_HMI[10];
//各车监控设备发送列车编号数据
extern UNS8 TramCode_MC1[2];
extern UNS8 TramCode_T[2];
extern UNS8 TramCode_M[2];
extern UNS8 TramCode_T1[2];
extern UNS8 TramCode_T2[2];
extern UNS8 TramCode_MC2[2];
//ERM发送删除故障履历和运行数据操作结果的信息
extern UNS8 DelResult_ERM[3];
//监控设备发送VVVF，SIV，BCU设备故障信息标题
extern UNS8 SubDevFltData_MDU[14];
//PTU软件连接时，下载状态
extern UNS8 PTUCMD_ERM;
//维修菜单数据
extern UNS8 DoorCheck_Data[12];
extern UNS8 ACPCheck_Data[2];
extern UNS8 SIVCheck_Data[2];
extern UNS8 BCUCheck_Data[130];
extern UNS8 SpeedCheck_Data[13];
//故障履历数据
extern UNS8 FaultInfo_ERM[20];
//列车运行累积数据
extern UNS8 RunData_ERM[64];
//列车所有加减速度测定数据
extern UNS8 AllSpeedInfo_ERM[13];
//SDO发送判断标志
extern UNS8 nowSendSDOID;
extern UNS8 nowSendSDOFailedID[2];
extern UNS8 nowResendSDOCount[2];

//界面显示状态数据值
extern int mVVVF_BCU_Level_Data[2];
extern bool bHB_OFF_Flag_Data;
extern int mVVVF_Elec_BCU_Data[6][2];
extern int mBCU_BCKpa_Data[6][2];
extern int mBCU_LoadRate_Data[6][2];
extern bool bACP_State_Data[2];
extern int mSIV_State_Data[2][3];
extern int mExpand_Elec_Data;
extern bool bBHB_BLB_Data[2][2];
extern bool bUrgent_Short_Data[2];
extern bool bBrakeIsolateDoor_Data[6];
extern int mACCCtrlMode;
extern int mACC_Temp_Data[6][3];
extern int mACC_DevInfo_Data[6][5];
extern bool mPIS_Urgent_Alert_Data[6];

#endif // OBJECTDICTDATA_H

