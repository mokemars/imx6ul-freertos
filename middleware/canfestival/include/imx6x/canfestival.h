#ifndef _CANFESTIVAL_H__
#define _CANFESTIVAL_H__

#include "applicfg.h"
#include "data.h"

void initTimer(void);
unsigned char canSend(CAN_PORT CANx, Message *m);
unsigned char canInit(CO_Data * d, CAN_PORT* CANx,unsigned short int adrCAN,uint32_t bitrate,unsigned char master);
unsigned char canChangeBaudRate( CAN_HANDLE CANx, char* baud);
#endif
