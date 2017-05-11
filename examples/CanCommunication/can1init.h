#ifndef CAN1INIT_H
#define CAN1INIT_H

#include "Can1ObjDict.h"

extern void can1Init_heartbeatError(CO_Data* d, UNS8);
extern UNS8 can1Init_canSend(Message *);
extern void can1Init_initialisation(CO_Data* d);
extern void can1Init_preOperational(CO_Data* d);
extern void can1Init_operational(CO_Data* d);
extern void can1Init_stopped(CO_Data* d);

extern void can1Init_post_sync(CO_Data* d);
extern void can1Init_post_TPDO(CO_Data* d);
extern void can1Init_storeODSubIndex(CO_Data* d, UNS16 wIndex, UNS8 bSubindex);
extern void can1Init_post_emcy(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg);

#endif // CAN1INIT_H
