#include "objectdictdata.h"

//硬线采集数据
UNS8 DI_Data_MC1[7] = {0};
UNS8 DI_Data_T[2] = {0};
UNS8 DI_Data_M[2] = {0};
UNS8 DI_Data_T1[2] = {0};
UNS8 DI_Data_T2[2] = {0};
UNS8 DI_Data_MC2[7] = {0};
UNS8 DI_Data_ERM1[7] = {0};
UNS8 DI_Data_ERM2[7] = {0};
//HMI发送列车综合数据和时间数据
UNS8 Car_Data_HMI[8] = {0};
UNS8 Date_Time_HMI[7] = {0};
UNS8 SetDate_Time_HMI[7] = {0};
//门数据
UNS8 DoorData_MC1[32] = {0};
UNS8 DoorData_T[32] = {0};
UNS8 DoorData_M[32] = {0};
UNS8 DoorData_T1[32] = {0};
UNS8 DoorData_T2[32] = {0};
UNS8 DoorData_MC2[32] = {0};
//空调ACC数据
UNS8 ACCData_MC1[9] = {0};
UNS8 ACCData_T[9] = {0};
UNS8 ACCData_M[9] = {0};
UNS8 ACCData_T1[9] = {0};
UNS8 ACCData_T2[9] = {0};
UNS8 ACCData_MC2[9] = {0};
//制动控制BCU数据
UNS8 BCUData_MC1[25] = {0};
UNS8 BCUData_T[25] = {0};
UNS8 BCUData_M[25] = {0};
UNS8 BCUData_T1[25] = {0};
UNS8 BCUData_T2[25] = {0};
UNS8 BCUData_MC2[25] = {0};
//信号ATC数据
UNS8 ATCData_MC1[27] = {0};
UNS8 ATCDateTime_MC1[6] = {0};
UNS8 ATCData_MC2[27] = {0};
UNS8 ATCDateTime_MC2[6] = {0};
//广播PIS数据
UNS8 PISData_MC1[11] = {0};
UNS8 PISData_MC2[11] = {0};
//牵引VVVF数据
UNS8 VVVF1Data_MC1[36] = {0};
UNS8 VVVF2Data_MC1[36] = {0};
UNS8 VVVF1Data_M[36] = {0};
UNS8 VVVF2Data_M[36] = {0};
UNS8 VVVF1Data_MC2[36] = {0};
UNS8 VVVF2Data_MC2[36] = {0};
//扩展电源SIV数据
UNS8 SIVData_T[18] = {0};
UNS8 SIVData_T1[18] = {0};
//子设备通讯异常数据
UNS8 SubDev_ComnErr_MC1[2] = {0xFF, 0xFE};
UNS8 SubDev_ComnErr_T[2] = {0xFF, 0xE0};
UNS8 SubDev_ComnErr_M[2] = {0xFF, 0xF0};
UNS8 SubDev_ComnErr_T1[2] = {0xFF, 0xE0};
UNS8 SubDev_ComnErr_T2[2] = {0xFF, 0xC0};
UNS8 SubDev_ComnErr_MC2[2] = {0xFF, 0xFE};
//列出TMS设备状态及通讯异常数据
UNS8 TMSStateData_CCU1[5] = {0xFF, 0xF0, 0, 0, 0};
UNS8 TMSStateData_CCU2[5] = {0xFF, 0xF0, 0, 0, 0};
//激活端HMI参数设置发送数据
UNS8 SetParaData_HMI[4] = {0};
UNS8 HMISetParaData_HMI[4] = {0};
//HMI获取单车某个子设备故障信息或者ERM存储故障履历信息时发送
UNS8 GetFaultData_HMI[2] = {0};
UNS8 M_GetFaultData_HMI[2] = {0};
//列出参数数据：编号，车轮直径，AS压力空，AS压力满
UNS8 ParaData_HMI[31] = {0};
//列出空调设置温度数据值
UNS8 TempData_HMI[2] = {0};
//维修菜单操作：查询故障履历总数,列车运行记录或加减速度测定数据
UNS8 GetFaultInfo_HMI = 0;
UNS8 M_GetFaultInfo_HMI = 0;
//维修菜单操作：激活端HMI删除记录指令，HMI发送初始设定数据
UNS8 DeleteData_HMI[3] = {0};
UNS8 InitData_HMI[8] = {0};
//CCU发送所有节点设备上线状态
UNS8 TMSDevStart_CCU = 0;
//ERM发送各车故障履历总数
UNS8 FltDataCount_ERM[7] = {0};
//空压机电源数据
UNS8 AirPumpData_MC1[11] = {0};
UNS8 AirPumpData_MC2[11] = {0};
//主HMI发送列车到站信息
UNS8 CarArriveData_HMI[10] = {0};
//各车监控设备发送列车编号数据
UNS8 TramCode_MC1[2] = {0};
UNS8 TramCode_T[2] = {0};
UNS8 TramCode_M[2] = {0};
UNS8 TramCode_T1[2] = {0};
UNS8 TramCode_T2[2] = {0};
UNS8 TramCode_MC2[2] = {0};
//ERM发送删除故障履历和运行数据操作结果的信息
UNS8 DelResult_ERM[3] = {0};
//监控设备发送VVVF，SIV，BCU设备故障信息标题
UNS8 SubDevFltData_MDU[14] = {0};
//PTU软件连接时，下载状态
UNS8 PTUCMD_ERM = 0;
//维修菜单数据
UNS8 DoorCheck_Data[12] = {0};
UNS8 ACPCheck_Data[2] = {0};
UNS8 SIVCheck_Data[2] = {0};
UNS8 BCUCheck_Data[130] = {0};
UNS8 SpeedCheck_Data[13] = {0};
//故障履历数据
UNS8 FaultInfo_ERM[20] = {0};
//列车运行累积数据
UNS8 RunData_ERM[64] = {0};
//列车所有加减速度测定数据
UNS8 AllSpeedInfo_ERM[13] = {0};
//SDO发送判断标志
UNS8 nowSendSDOID = 0;
UNS8 nowSendSDOFailedID[2] = {0};
UNS8 nowResendSDOCount[2] = {0};

//界面显示状态数据值
int mVVVF_BCU_Level_Data[2] = {0};
bool bHB_OFF_Flag_Data = false;
int mVVVF_Elec_BCU_Data[6][2];
int mBCU_BCKpa_Data[6][2];
int mBCU_LoadRate_Data[6][2];
bool bACP_State_Data[2] = {false};
int mSIV_State_Data[2][3] = {-1, -1, -1, -1, -1, -1};
int mExpand_Elec_Data = -1;
bool bBHB_BLB_Data[2][2] = {-1 , -1, -1, -1};
bool bUrgent_Short_Data[2] = {false};
bool bBrakeIsolateDoor_Data[6] = {false};
int mACCCtrlMode = -1;
int mACC_Temp_Data[6][3];
int mACC_DevInfo_Data[6][5];
bool mPIS_Urgent_Alert_Data[6] = {false};
