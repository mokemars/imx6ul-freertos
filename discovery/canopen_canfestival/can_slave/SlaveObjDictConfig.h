#ifndef __SLAVE_OBJDICT_CONFIG__
#define __SLAVE_OBJDICT_CONFIG__

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "canfestival.h"	 
#include "MasterObjDict.h"

extern CO_Data* CANOpenSlaveObject;
void CanopenSlaveInit(void);
	 
#ifdef __cplusplus
}
#endif

#endif /* __SLAVE_OBJDICT_CONFIG__ */
