
/* File generated by gen_cfile.py. Should not be modified. */

#ifndef _MASTER_OBJDICT_H
#define _MASTER_OBJDICT_H

#include "data.h"

/* Prototypes of function provided by object dictionnary */
UNS32 MasterDict_valueRangeTest (UNS8 typeValue, void * value);
const indextable * MasterDict_scanIndexOD (UNS16 wIndex, UNS32 * errorCode, ODCallback_t **Callback);

/* Master node data struct */
extern CO_Data MasterDict_Data;
extern UNS16 ControlWordAxis1;		            /* Mapped at index 0x6040, subindex 0x00 */
extern UNS16 StatusWordAxis1;                    /* Mapped at index 0x6041, subindex 0x00 */
extern INTEGER32 PositionDemandValueAxis1;       /* Mapped at index 0x6062, subindex 0x00 */
extern INTEGER32 InterpolationDataAxis1;		    /* Mapped at index 0x60c1, subindex 0x01 */

extern UNS16 ControlWordAxis2;		            /* Mapped at index 0x6040, subindex 0x01 */
extern UNS16 StatusWordAxis2;                    /* Mapped at index 0x6041, subindex 0x01 */
extern INTEGER32 PositionDemandValueAxis2;       /* Mapped at index 0x6062, subindex 0x01 */
extern INTEGER32 InterpolationDataAxis2;		    /* Mapped at index 0x60c1, subindex 0x03 */
#endif // MASTER_ObjDict_H
