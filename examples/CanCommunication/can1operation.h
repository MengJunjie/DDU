#ifndef CAN1OPERATION_H
#define CAN1OPERATION_H

#ifdef USE_XENO
//#define eprintf(...) if(0){}
#define eprintf(...)
#elif defined USE_RTAI
#define eprintf(...)
#else
#define eprintf(...) printf (__VA_ARGS__)
#endif

#include "gui_config.h"

#include "can1init.h"
#include "canfestival.h"
#include "Can1ObjDict.h"

extern s_BOARD Can1Board;

extern void can1Connect();
extern void can1BootUp();
extern void can1RegisterCallBack();

#endif // CAN1OPERATION_H
