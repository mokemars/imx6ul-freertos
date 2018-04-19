#ifndef _SLAVE_CANOPEN_H__
#define _SLAVE_CANOPEN_H__

#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"

#include "canfestival.h"
#include "SlaveObjDict.h"

#define CANOPEN_SLAVE_QUEUE_LENGTH 5
#define CANOPEN_SLAVE_TIMES 10

enum{
	CANOPEN_SLAVE_TIMEOUT=0x1,
	CANOPEN_SLAVE_USER
};

void CanOpenSlaveBaseInit(void);
unsigned char DigitalInputHandler(CO_Data* d, unsigned char *newInput, unsigned char size);
unsigned char DigitalOutputHandler(CO_Data* d, unsigned char *newOutput, unsigned char size);
#endif
