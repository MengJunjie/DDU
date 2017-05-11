#ifndef CAN2OPERATION_H
#define CAN2OPERATION_H

#ifdef USE_XENO
//#define eprintf(...) if(0){}
#define eprintf(...)
#elif defined USE_RTAI
#define eprintf(...)
#else
#define eprintf(...) printf (__VA_ARGS__)
#endif

#include "gui_config.h"

#include "can2init.h"
#include "canfestival.h"
#include "Can2ObjDict.h"

extern s_BOARD Can2Board;

extern void can2Connect();
extern void can2BootUp();
extern void can2RegisterCallBack();

#endif // CAN2OPERATION_H
