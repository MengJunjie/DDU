#ifndef CANSENDANDRECV
#define CANSENDANDRECV

#include "gui_config.h"

#include "canfestival.h"
//#include "can1init.h"
#include "Can1ObjDict.h"
//#include "can2init.h"
#include "Can2ObjDict.h"

//发送PDO
extern void canSendCarDataUpdate();
extern void canSendCarDateTimeUpdate();
extern void canSendSetParaData_Update();
extern void canSendGetFaultData_Update();
extern void canSendParaData1_Update();
extern void canSendParaData2_Update();
extern void canSendTempData_Update();
extern void canSendGetFaultInfo_Update();
extern void canSendDeleteData_Update();
extern void canSendCarInitData_Update();
extern void canSendParaData3_Update();
extern void canSendParaData4_Update();
extern void canSendCarArriveData1_Update();
extern void canSendCarArriveData2_Update();
extern void canSendHMISetDateTime_Update();

//发送SDO
extern void canSendSDOConfig(CO_Data *d, UNS8 nodeId);
extern void canSendSDOData(int iSDOId, int iCanFlag);
extern void canSendDoorCheckData_Update(int canFlag);
extern void canSendACPCheckData_Update(int canFlag);
extern void canSendSIVCheckData_Update(int canFlag);
extern void canSendBCUCheckData_Update(int canFlag);
extern void canSendSpeedCheckData_Update(int canFlag);
extern void canSendFaultInfoOperation_Update(int canFlag);
extern void canSendFaultInfoDoFinished_Update(int canFlag);
extern void canSendSpeedInfoDoFinished_Update(int canFlag);

extern void CheckWriteSDO(CO_Data* d, UNS8 nodeid);

//接收PDO回调函数
extern UNS32 Oncan_DIData_MC1_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_DIData_T_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_DIData_M_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_DIData_T1_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_DIData_T2_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_DIData_MC2_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_CarData_HMI_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_ATCDateTime_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_DateTime_HMI_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_DoorData_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_ACCData_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_ACCData_2_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_BCUData_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_ATCData_1_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_ATCData_2_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_ATCData_3_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_ATCData_4_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_PISData_1_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_PISData_2_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_VVVFData_1_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_VVVFData_2_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_VVVFData_3_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_VVVFData_4_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_VVVFData_5_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_SIVData_1_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_SIVData_2_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_SIVData_3_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_ComnErr_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_TMSState_CCU_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_SetParaData_HMI_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_GetFaultData_HMI_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_ParaData_7B_HMI_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_ParaData_8B_HMI_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_TempData_HMI_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_GetFaultRunSpeed_HMI_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_InitSetData_HMI_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_TMSDevStart_CCU_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_FaultDataCount_ERM_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_ACPData_1_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_ACPData_2_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_TramCodeData_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_DelResult_ERM_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_SubDevFltData_MDU_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_SetDateTime_HMI_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_BCUData_4_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_PTU_CMD_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_DIData_ERM_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_SpeedCheck_HMI_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

extern UNS32 Oncan_FaultInfo_ERM_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_RunInfo_ERM_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);
extern UNS32 Oncan_AllSpeedInfo_ERM_Update(CO_Data *d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);

#endif // CANSENDANDRECV

