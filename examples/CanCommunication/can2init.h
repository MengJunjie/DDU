#ifndef CAN2INIT_H
#define CAN2INIT_H

#include "Can2ObjDict.h"

extern void can2Init_heartbeatError(CO_Data* d, UNS8);
extern UNS8 can2Init_canSend(Message *);
extern void can2Init_initialisation(CO_Data* d);
extern void can2Init_preOperational(CO_Data* d);
extern void can2Init_operational(CO_Data* d);
extern void can2Init_stopped(CO_Data* d);

extern void can2Init_post_sync(CO_Data* d);
extern void can2Init_post_TPDO(CO_Data* d);
extern void can2Init_storeODSubIndex(CO_Data* d, UNS16 wIndex, UNS8 bSubindex);
extern void can2Init_post_emcy(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg);

#endif // CAN2INIT_H
