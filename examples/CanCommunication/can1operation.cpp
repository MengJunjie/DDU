#include "can1operation.h"
#include "can2operation.h"
#include "Can1ObjDict.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "cansendandrecv.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

s_BOARD Can1Board = {"0", "125k"};

void can1Connect(){
    char* LibraryPath="/lib/libcanfestival_can_socket.so";

    if (LoadCanDriver(LibraryPath) == NULL){
        eprintf("CAN1  CAN1  Unable to load library: %s\n",LibraryPath);
    }

    TimerInit();

    Can1ObjDict_Data.heartbeatError = can1Init_heartbeatError;
    Can1ObjDict_Data.initialisation = can1Init_initialisation;
    Can1ObjDict_Data.preOperational = can1Init_preOperational;
    Can1ObjDict_Data.operational = can1Init_operational;
    Can1ObjDict_Data.stopped = can1Init_stopped;
    Can1ObjDict_Data.post_sync = can1Init_post_sync;
    Can1ObjDict_Data.post_TPDO = can1Init_post_TPDO;
    Can1ObjDict_Data.storeODSubIndex = can1Init_storeODSubIndex;
    Can1ObjDict_Data.post_emcy = can1Init_post_emcy;

    if(1 == deviceCarId){
        if(!canOpen(&Can1Board, &Can1ObjDict_Data)){
        }
    }
    else if(6 == deviceCarId){
        if(!canOpen(&Can2Board, &Can1ObjDict_Data)){
        }
    }
    else{
        qDebug()<<"%%%%%%%%%%%%  Car ID Get Failed ,Can't Up The Can1 ";
        return;
    }

    can1RegisterCallBack();

    can1BootUp();
}

void can1BootUp(){
    if(1 == deviceCarId){
        setNodeId(&Can1ObjDict_Data, NODEID_HMI1);
        setState(&Can1ObjDict_Data, Initialisation);
    }
    else{
        setNodeId(&Can1ObjDict_Data, NODEID_HMI2);
        setState(&Can1ObjDict_Data, Initialisation);
    }
}

void can1RegisterCallBack(){
    qDebug()<<"can1RegisterCallBack";
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2000, 7, &Oncan_DIData_MC1_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2001, 2, &Oncan_DIData_T_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2002, 2, &Oncan_DIData_M_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2003, 2, &Oncan_DIData_T1_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2004, 2, &Oncan_DIData_T2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2005, 7, &Oncan_DIData_MC2_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2006, 8, &Oncan_CarData_HMI_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2007, 6, &Oncan_ATCDateTime_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2008, 7, &Oncan_DateTime_HMI_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2009, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x200A, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x200B, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x200C, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x200D, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x200E, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x206F, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2070, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2071, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2072, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2073, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2074, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x20A1, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x20A2, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x20A3, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x20A4, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x20A5, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x20A6, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x20A7, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x20A8, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x20A9, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x20AA, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x20AB, 8, &Oncan_DoorData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x20AC, 8, &Oncan_DoorData_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x200F, 8, &Oncan_ACCData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2010, 8, &Oncan_ACCData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2011, 8, &Oncan_ACCData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2012, 8, &Oncan_ACCData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2013, 8, &Oncan_ACCData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2014, 8, &Oncan_ACCData_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2063, 0, &Oncan_ACCData_2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2064, 0, &Oncan_ACCData_2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2065, 0, &Oncan_ACCData_2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2066, 0, &Oncan_ACCData_2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2067, 0, &Oncan_ACCData_2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2068, 0, &Oncan_ACCData_2_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2015, 8, &Oncan_BCUData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2016, 8, &Oncan_BCUData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2017, 8, &Oncan_BCUData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2018, 8, &Oncan_BCUData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2019, 8, &Oncan_BCUData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x201A, 8, &Oncan_BCUData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x201B, 8, &Oncan_BCUData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x201C, 8, &Oncan_BCUData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x201D, 8, &Oncan_BCUData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x201E, 8, &Oncan_BCUData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x201F, 8, &Oncan_BCUData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2020, 8, &Oncan_BCUData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2021, 8, &Oncan_BCUData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2022, 8, &Oncan_BCUData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2023, 8, &Oncan_BCUData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2024, 8, &Oncan_BCUData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2025, 8, &Oncan_BCUData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2026, 8, &Oncan_BCUData_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2027, 7, &Oncan_ATCData_1_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2028, 8, &Oncan_ATCData_2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2029, 8, &Oncan_ATCData_3_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x202A, 4, &Oncan_ATCData_4_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x202B, 7, &Oncan_ATCData_1_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x202C, 8, &Oncan_ATCData_2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x202D, 8, &Oncan_ATCData_3_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x202E, 4, &Oncan_ATCData_4_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x202F, 8, &Oncan_PISData_1_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2030, 3, &Oncan_PISData_2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2031, 8, &Oncan_PISData_1_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2032, 3, &Oncan_PISData_2_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2033, 8, &Oncan_VVVFData_1_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2034, 6, &Oncan_VVVFData_2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2035, 8, &Oncan_VVVFData_3_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2036, 8, &Oncan_VVVFData_4_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2037, 6, &Oncan_VVVFData_5_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2038, 8, &Oncan_VVVFData_1_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2039, 6, &Oncan_VVVFData_2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x203A, 8, &Oncan_VVVFData_3_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x203B, 8, &Oncan_VVVFData_4_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x203C, 6, &Oncan_VVVFData_5_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x203D, 8, &Oncan_VVVFData_1_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x203E, 6, &Oncan_VVVFData_2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x203F, 8, &Oncan_VVVFData_3_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2040, 8, &Oncan_VVVFData_4_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2041, 6, &Oncan_VVVFData_5_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2042, 8, &Oncan_VVVFData_1_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2043, 6, &Oncan_VVVFData_2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2044, 8, &Oncan_VVVFData_3_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2045, 8, &Oncan_VVVFData_4_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2046, 6, &Oncan_VVVFData_5_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2047, 8, &Oncan_VVVFData_1_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2048, 6, &Oncan_VVVFData_2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2049, 8, &Oncan_VVVFData_3_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x204A, 8, &Oncan_VVVFData_4_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x204B, 6, &Oncan_VVVFData_5_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x204C, 8, &Oncan_VVVFData_1_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x204D, 6, &Oncan_VVVFData_2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x204E, 8, &Oncan_VVVFData_3_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x204F, 8, &Oncan_VVVFData_4_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2050, 6, &Oncan_VVVFData_5_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2051, 7, &Oncan_SIVData_1_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2052, 8, &Oncan_SIVData_2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2053, 3, &Oncan_SIVData_3_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2054, 7, &Oncan_SIVData_1_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2055, 8, &Oncan_SIVData_2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2056, 3, &Oncan_SIVData_3_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2057, 2, &Oncan_ComnErr_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2058, 2, &Oncan_ComnErr_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2059, 2, &Oncan_ComnErr_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x205A, 2, &Oncan_ComnErr_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x205B, 2, &Oncan_ComnErr_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x205C, 2, &Oncan_ComnErr_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x205D, 5, &Oncan_TMSState_CCU_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2093, 5, &Oncan_TMSState_CCU_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x205E, 4, &Oncan_SetParaData_HMI_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x205F, 2, &Oncan_GetFaultData_HMI_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2060, 7, &Oncan_ParaData_7B_HMI_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2061, 8, &Oncan_ParaData_8B_HMI_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x206C, 8, &Oncan_ParaData_8B_HMI_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x206D, 8, &Oncan_ParaData_8B_HMI_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2062, 2, &Oncan_TempData_HMI_Update);

//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2069, 0, &Oncan_GetFaultRunSpeed_HMI_Update);

//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x206A, 3, &Oncan_XXXX);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x206B, 8, &Oncan_InitSetData_HMI_Update);

//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x206E, 0, &Oncan_TMSDevStart_CCU_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2075, 6, &Oncan_ATCDateTime_Update);

//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2076, 0, &Oncan_XXXX);
//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2077, 0, &Oncan_XXXX);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2078, 7, &Oncan_FaultDataCount_ERM_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2079, 8, &Oncan_ACPData_1_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x207A, 3, &Oncan_ACPData_2_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x207B, 8, &Oncan_ACPData_1_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x207C, 3, &Oncan_ACPData_2_Update);

//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x207D, 2, &Oncan_XXXX);
//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x207E, 8, &Oncan_XXXX);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x207F, 2, &Oncan_TramCodeData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2080, 2, &Oncan_TramCodeData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2081, 2, &Oncan_TramCodeData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2082, 2, &Oncan_TramCodeData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2083, 2, &Oncan_TramCodeData_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2084, 2, &Oncan_TramCodeData_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2085, 3, &Oncan_DelResult_ERM_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2086, 7, &Oncan_SubDevFltData_MDU_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2087, 7, &Oncan_SubDevFltData_MDU_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2088, 7, &Oncan_SetDateTime_HMI_Update);

//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2089, 4, &Oncan_GetFaultData_ERM_Update);

//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x208A, 6, &Oncan_XXX
//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x208B, 6, &Oncan_XXXX);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x208C, 0, &Oncan_BCUData_4_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x208D, 0, &Oncan_BCUData_4_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x208E, 0, &Oncan_BCUData_4_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x208F, 0, &Oncan_BCUData_4_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2090, 0, &Oncan_BCUData_4_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2091, 0, &Oncan_BCUData_4_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2092, 0, &Oncan_PTU_CMD_Update);

//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2094, 8, &Oncan_XXXXX);
//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2095, 7, &Oncan_XXXXX);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2096, 2, &Oncan_GetFaultData_HMI_Update);

//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2097, 7, &Oncan_XXXXX);
//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2098, 8, &Oncan_XXXXX);

//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x2099, 0, &Oncan_GetFaultRunSpeed_HMI_Update);

//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x209A, 3, &Oncan_XXXXX);
//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x209B, 8, &Oncan_XXXXX);
//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x209C, 8, &Oncan_XXXXX);
//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x209D, 8, &Oncan_XXXXX);
//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x209E, 2, &Oncan_XXXXX);
//    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x209F, 8, &Oncan_XXXXX);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x20A0, 7, &Oncan_DIData_ERM_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x4004, 2, &Oncan_SpeedCheck_HMI_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x4005, 1, &Oncan_FaultInfo_ERM_Update);
    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x4006, 1, &Oncan_RunInfo_ERM_Update);

    RegisterSetODentryCallBack(&Can1ObjDict_Data, 0x400A, 1, &Oncan_AllSpeedInfo_ERM_Update);
}
