#include "MasterObjDict.h"

/**************************************************************************/
/* Declaration of mapped variables                                        */
/**************************************************************************/
UNS16 ControlWordAxis1 = 0;			/* Mapped at index 0x6040, subindex 0x00 */
UNS16 StatusWordAxis1 = 0;			/* Mapped at index 0x6041, subindex 0x00 */
INTEGER32 PositionDemandValueAxis1 = 0;		/* Mapped at index 0x6062, subindex 0x00 */

UNS16 ControlWordAxis2 = 0;			/* Mapped at index 0x6040, subindex 0x01 */
UNS16 StatusWordAxis2 = 0;			/* Mapped at index 0x6041, subindex 0x02 */
INTEGER32 PositionDemandValueAxis2 = 0;		/* Mapped at index 0x6062, subindex 0x01 */

UNS8 InterpolationDataIndexNum = 2;		/* Mapped at index 0x60c1, subindex 0x00 */
INTEGER32 InterpolationDataAxis1 = 0;		/* Mapped at index 0x60c1, subindex 0x01 */
INTEGER32 InterpolationDataAxis2 = 0;		/* Mapped at index 0x60c1, subindex 0x03 */

UNS8 DI1 = 0x0;					/* Mapped at index 0x2000, subindex 0x00 */
UNS8 DI2 = 0x0;					/* Mapped at index 0x2001, subindex 0x00 */
UNS8 DO1 = 0x0;					/* Mapped at index 0x2100, subindex 0x00 */
UNS8 DO2 = 0x0;					/* Mapped at index 0x2101, subindex 0x00 */
/**************************************************************************/
/* Declaration of value range types                                       */
/**************************************************************************/

#define valueRange_EMC 0x9F /* Type for index 0x1003 subindex 0x00 (only set of value 0 is possible) */
UNS32 MasterDict_valueRangeTest (UNS8 typeValue, void * value)
{
	switch (typeValue) {
		case valueRange_EMC:
			if (*(UNS8*)value != (UNS8)0)
				return OD_VALUE_RANGE_EXCEEDED;
			break;
	}
	return 0;
}

/**************************************************************************/
/* The node id                                                            */
/**************************************************************************/
/* node_id default value.*/
UNS8 MasterDict_bDeviceNodeId = 0x00;

/**************************************************************************/
/* Array of message processing information */

const UNS8 MasterDict_iam_a_slave = 0;

TIMER_HANDLE MasterDict_heartBeatTimers[1] = {0xea60};
/*
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

                               OBJECT DICTIONARY

$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/

/* index 0x1000 :   Device Type. */
UNS32 MasterDict_obj1000 = 0x12D;	/* 301 */
subindex MasterDict_Index1000[] = 
{
	{ RO, uint32, sizeof (UNS32), (void*)&MasterDict_obj1000}
};

/* index 0x1001 :   Error Register. */
UNS8 MasterDict_obj1001 = 0x0;	/* 0 */
subindex MasterDict_Index1001[] = 
{
	{ RO, uint8, sizeof (UNS8), (void*)&MasterDict_obj1001}
};

/* index 0x1003 :   Pre-defined Error Field */
UNS8 MasterDict_highestSubIndex_obj1003 = 0; /* number of subindex - 1*/
UNS32 MasterDict_obj1003[] = 
{
	0x0	/* 0 */
};
subindex MasterDict_Index1003[] = 
{
	{ RW, valueRange_EMC, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj1003},
	{ RO, uint32, sizeof (UNS32), (void*)&MasterDict_obj1003[0]}
};

/* index 0x1005 :   SYNC COB ID. */
UNS32 MasterDict_obj1005 = 0x40000080;	/* 1073741952 */
subindex MasterDict_Index1005[] = 
{
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1005}
};

/* index 0x1006 :   Communication / Cycle Period. */
//UNS32 MasterDict_obj1006 = 0xC350;	/* 50000 */
//UNS32 MasterDict_obj1006 = 0xF4240;	/* 1000000 */
//UNS32 MasterDict_obj1006 = 0x4E20;	/* 20000 */
UNS32 MasterDict_obj1006 = 0;	/* 20000 */
subindex MasterDict_Index1006[] = 
{
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1006}
};
/* index 0x100C :   Guard time. */					 
UNS16 MasterDict_obj100C = 0;	
/* index 0x100D :   Life time factor. */	
UNS8 MasterDict_obj100D = 0;

/* index 0x1010 :   Store parameters. */
UNS8 MasterDict_highestSubIndex_obj1010 = 4; /* number of subindex - 1*/
UNS32 MasterDict_obj1010_Save_All_Parameters = 0x0;	/* 0 */
UNS32 MasterDict_obj1010_Save_Communication_Parameters = 0x0;	/* 0 */
UNS32 MasterDict_obj1010_Save_Application_Parameters = 0x0;	/* 0 */
UNS32 MasterDict_obj1010_Save_Manufacturer_Parameters_1 = 0x0;	/* 0 */
subindex MasterDict_Index1010[] = 
{
	{ RO, uint8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj1010},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1010_Save_All_Parameters},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1010_Save_Communication_Parameters},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1010_Save_Application_Parameters},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1010_Save_Manufacturer_Parameters_1}
};

/* index 0x1011 :   Restore Default Parameters. */
UNS8 MasterDict_highestSubIndex_obj1011 = 4; /* number of subindex - 1*/
UNS32 MasterDict_obj1011_Restore_All_Default_Parameters = 0x0;	/* 0 */
UNS32 MasterDict_obj1011_Restore_Communication_Default_Parameters = 0x0;	/* 0 */
UNS32 MasterDict_obj1011_Restore_Application_Default_Parameters = 0x0;	/* 0 */
UNS32 MasterDict_obj1011_Restore_Manufacturer_Defined_Default_Parameters_1 = 0x0;	/* 0 */
subindex MasterDict_Index1011[] = 
{
	{ RO, uint8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj1011},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1011_Restore_All_Default_Parameters},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1011_Restore_Communication_Default_Parameters},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1011_Restore_Application_Default_Parameters},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1011_Restore_Manufacturer_Defined_Default_Parameters_1}
};

/* index 0x1014 :   Emergency COB ID. */
UNS32 MasterDict_obj1014 = 0x80;	/* 128 */
subindex MasterDict_Index1014[] = 
{
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1014 }
};

/* index 0x1016 :   Consumer Heartbeat Time. */
UNS8 MasterDict_highestSubIndex_obj1016 = 1; /* number of subindex - 1*/
UNS32 MasterDict_obj1016[] = 
{
	2<<16|0xea60,
};
subindex MasterDict_Index1016[] = 
{
	{ RO, uint8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj1016},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1016[0]}
};

/* index 0x1017 :   Producer Heartbeat Time */ 
UNS16 MasterDict_obj1017 = 0x0;   /* 0 */

/* index 0x1018 :   Identity. */
UNS8 MasterDict_highestSubIndex_obj1018 = 4; /* number of subindex - 1*/
UNS32 MasterDict_obj1018_Vendor_ID = 0x0;	/* 0 */
UNS32 MasterDict_obj1018_Product_Code = 0x0;	/* 0 */
UNS32 MasterDict_obj1018_Revision_Number = 0x0;	/* 0 */
UNS32 MasterDict_obj1018_Serial_Number = 0x0;	/* 0 */
subindex MasterDict_Index1018[] = 
{
	{ RO, uint8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj1018},
	{ RO, uint32, sizeof (UNS32), (void*)&MasterDict_obj1018_Vendor_ID},
	{ RO, uint32, sizeof (UNS32), (void*)&MasterDict_obj1018_Product_Code},
	{ RO, uint32, sizeof (UNS32), (void*)&MasterDict_obj1018_Revision_Number},
	{ RO, uint32, sizeof (UNS32), (void*)&MasterDict_obj1018_Serial_Number}
};
		     
/* index 0x1200 :   Server SDO Parameter. */
UNS8 MasterDict_highestSubIndex_obj1200 = 2; /* number of subindex - 1*/
UNS32 MasterDict_obj1200_COB_ID_Client_to_Server_Receive_SDO = 0x600;	/* 1536 */
UNS32 MasterDict_obj1200_COB_ID_Server_to_Client_Transmit_SDO = 0x580;	/* 1408 */
subindex MasterDict_Index1200[] = 
{
	{ RO, uint8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj1200},
	{ RO, uint32, sizeof (UNS32), (void*)&MasterDict_obj1200_COB_ID_Client_to_Server_Receive_SDO},
	{ RO, uint32, sizeof (UNS32), (void*)&MasterDict_obj1200_COB_ID_Server_to_Client_Transmit_SDO}
};
		     
/* index 0x1280 :   Client SDO 1 Parameter. */
UNS8 MasterDict_highestSubIndex_obj1280 = 3; /* number of subindex - 1*/
UNS32 MasterDict_obj1280_COB_ID_Client_to_Server_Transmit_SDO = 0x600;	/* 1538 */
UNS32 MasterDict_obj1280_COB_ID_Server_to_Client_Receive_SDO = 0x580;	/* 1410 */
UNS8 MasterDict_obj1280_Node_ID_of_the_SDO_Server = 0x2;	/* 2 */
subindex MasterDict_Index1280[] = 
{
	{ RO, uint8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj1280},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1280_COB_ID_Client_to_Server_Transmit_SDO},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1280_COB_ID_Server_to_Client_Receive_SDO},
	{ RW, uint8, sizeof (UNS8), (void*)&MasterDict_obj1280_Node_ID_of_the_SDO_Server}
};

/* index 0x1400 :   Receive PDO 1 Parameter. */
UNS8 MasterDict_highestSubIndex_obj1400 = 5; /* number of subindex - 1*/
UNS32 MasterDict_obj1400_COB_ID_used_by_PDO = 0x200;	/* 512 */
UNS8 MasterDict_obj1400_Transmission_Type = 0x1;	/* 1 */
UNS16 MasterDict_obj1400_Inhibit_Time = 0x0;	/* 0 */
UNS8 MasterDict_obj1400_Compatibility_Entry = 0x0;	/* 0 */
UNS16 MasterDict_obj1400_Event_Timer = 0x0;	/* 0 */
subindex MasterDict_Index1400[] = 
{
	{ RO, uint8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj1400},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1400_COB_ID_used_by_PDO},
	{ RW, uint8, sizeof (UNS8), (void*)&MasterDict_obj1400_Transmission_Type},
	{ RW, uint16, sizeof (UNS16), (void*)&MasterDict_obj1400_Inhibit_Time},
	{ RW, uint8, sizeof (UNS8), (void*)&MasterDict_obj1400_Compatibility_Entry},
	{ RW, uint16, sizeof (UNS16), (void*)&MasterDict_obj1400_Event_Timer}
};
/* index 0x1401 :   Receive PDO 2 Parameter. */
UNS8 MasterDict_highestSubIndex_obj1401 = 5; /* number of subindex - 1*/
UNS32 MasterDict_obj1401_COB_ID_used_by_PDO = 0x200;	/* 512 */
UNS8 MasterDict_obj1401_Transmission_Type = 0x1;	/* 1 */
UNS16 MasterDict_obj1401_Inhibit_Time = 0x0;	/* 0 */
UNS8 MasterDict_obj1401_Compatibility_Entry = 0x0;	/* 0 */
UNS16 MasterDict_obj1401_Event_Timer = 0x0;	/* 0 */
subindex MasterDict_Index1401[] = 
{
	{ RO, uint8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj1401},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1401_COB_ID_used_by_PDO},
	{ RW, uint8, sizeof (UNS8), (void*)&MasterDict_obj1401_Transmission_Type},
	{ RW, uint16, sizeof (UNS16), (void*)&MasterDict_obj1401_Inhibit_Time},
	{ RW, uint8, sizeof (UNS8), (void*)&MasterDict_obj1401_Compatibility_Entry},
	{ RW, uint16, sizeof (UNS16), (void*)&MasterDict_obj1401_Event_Timer}
};

/* index 0x1600 :   Receive PDO 1 Mapping. */
UNS8 MasterDict_highestSubIndex_obj1600 = 2; /* number of subindex - 1*/
UNS32 MasterDict_obj1600[] = 
{
	0x60410110,	/* 536870913 */
	0x60620120,	/* 536936449 */
};
subindex MasterDict_Index1600[] = 
{
	{ RW, uint8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj1600},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1600[0]},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1600[1]},
};
/* index 0x1601 :   Receive PDO 2 Mapping. */
UNS8 MasterDict_highestSubIndex_obj1601 = 2; /* number of subindex - 1*/
UNS32 MasterDict_obj1601[] = 
{
	0x60410210,	/* 536870913 */
	0x60620220,	/* 536936449 */
};
subindex MasterDict_Index1601[] = 
{
	{ RW, uint8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj1601},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1601[0]},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1601[1]},
};
/* index 0x1800 :   Transmit PDO 1 Parameter. */
UNS8 MasterDict_highestSubIndex_obj1800 = 5; /* number of subindex - 1*/
UNS32 MasterDict_obj1800_COB_ID_used_by_PDO = 0x180;	/* 384 */
UNS8 MasterDict_obj1800_Transmission_Type = 0x0;	/* 0 */
UNS16 MasterDict_obj1800_Inhibit_Time = 0x0;	/* 0 */
UNS8 MasterDict_obj1800_Compatibility_Entry = 0x0;	/* 0 */
UNS16 MasterDict_obj1800_Event_Timer = 0x0;	/* 0 */
subindex MasterDict_Index1800[] = 
{
	{ RO, uint8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj1800},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1800_COB_ID_used_by_PDO},
	{ RW|TO_BE_SAVE, uint8, sizeof (UNS8), (void*)&MasterDict_obj1800_Transmission_Type},
	{ RW|TO_BE_SAVE, uint16, sizeof (UNS16), (void*)&MasterDict_obj1800_Inhibit_Time},
	{ RW, uint8, sizeof (UNS8), (void*)&MasterDict_obj1800_Compatibility_Entry},
	{ RW, uint16, sizeof (UNS16), (void*)&MasterDict_obj1800_Event_Timer}
};
/* index 0x1801 :   Transmit PDO 2 Parameter. */
UNS8 MasterDict_highestSubIndex_obj1801 = 5; /* number of subindex - 1*/
UNS32 MasterDict_obj1801_COB_ID_used_by_PDO = 0x190;	/* 384 */
UNS8 MasterDict_obj1801_Transmission_Type = 0x0;	/* 0 */
UNS16 MasterDict_obj1801_Inhibit_Time = 0x0;	/* 0 */
UNS8 MasterDict_obj1801_Compatibility_Entry = 0x0;	/* 0 */
UNS16 MasterDict_obj1801_Event_Timer = 0x0;	/* 0 */
subindex MasterDict_Index1801[] = 
{
	{ RO, uint8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj1801},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1801_COB_ID_used_by_PDO},
	{ RW|TO_BE_SAVE, uint8, sizeof (UNS8), (void*)&MasterDict_obj1801_Transmission_Type},
	{ RW|TO_BE_SAVE, uint16, sizeof (UNS16), (void*)&MasterDict_obj1801_Inhibit_Time},
	{ RW, uint8, sizeof (UNS8), (void*)&MasterDict_obj1801_Compatibility_Entry},
	{ RW, uint16, sizeof (UNS16), (void*)&MasterDict_obj1801_Event_Timer}
};
/* index 0x1A00 :   Transmit PDO 1 Mapping. */
UNS8 MasterDict_highestSubIndex_obj1A00 = 3; /* number of subindex - 1*/
UNS32 MasterDict_obj1A00[] = 
{
	0x60400110,
	0x60c10220,  
};
subindex MasterDict_Index1A00[] = 
{
	{ RW, uint8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj1A00},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1A00[0]},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1A00[1]},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1A00[2]},
};
/* index 0x1A01 :   Transmit PDO 2 Mapping. */
UNS8 MasterDict_highestSubIndex_obj1A01 = 3; /* number of subindex - 1*/
UNS32 MasterDict_obj1A01[] = 
{
	0x60400210,
	0x60c10320,  
};
subindex MasterDict_Index1A01[] = 
{
	{ RW, uint8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj1A01},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1A01[0]},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1A01[1]},
	{ RW, uint32, sizeof (UNS32), (void*)&MasterDict_obj1A01[2]},
};

/* index 0x2000 :   Mapped variable DI1 */
subindex MasterDict_Index2000[] = 
{
	{ RW, uint8, sizeof (UNS8), (void*)&DI1}
};

/* index 0x2001 :   Mapped variable DI2 */
subindex MasterDict_Index2001[] = 
{
	{ RW, uint8, sizeof (UNS8), (void*)&DI2}
};

/* index 0x2100 :   Mapped variable DO1 */
subindex MasterDict_Index2100[] = 
{
	{ RW, uint8, sizeof (UNS8), (void*)&DO1}
};

/* index 0x2101 :   Mapped variable DO2 */
subindex MasterDict_Index2101[] = 
{
	{ RW, uint8, sizeof (UNS8), (void*)&DO2}
};
/* index 0x6040 :   Mapped variable ControlWord */
UNS8 MasterDict_highestSubIndex_obj6040 = 2;
subindex MasterDict_Index6040[] =                                                    
{
	{ RW, int8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj6040}, 
	{ RW, uint16, sizeof (UNS16), (void*)&ControlWordAxis1},
	{ RW, uint16, sizeof (UNS16), (void*)&ControlWordAxis2},
};
/* index 0x6041 :   Mapped variable StatusWord */
UNS8 MasterDict_highestSubIndex_obj6041 = 2;
subindex MasterDict_Index6041[] =                                                    
{
	{ RW, int8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj6041}, 
	{ RW, uint16, sizeof (UNS16), (void*)&StatusWordAxis1},
	{ RW, uint16, sizeof (UNS16), (void*)&StatusWordAxis2},
};
/* index 0x6062 :   Mapped variable PositionDemandValue */
UNS8 MasterDict_highestSubIndex_obj6062 = 2;
subindex MasterDict_Index6062[] =                                                    
{
	{ RW, int8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj6062}, 
	{ RW, uint32, sizeof (UNS32), (void*)&PositionDemandValueAxis1},
	{ RW, uint32, sizeof (UNS32), (void*)&PositionDemandValueAxis2},
};
/* index 0x60c1 :   Mapped variable Interpolation Data */
UNS8 MasterDict_highestSubIndex_obj60C1 = 3;
subindex MasterDict_Index60c1[] = 
{
	{ RW, int8, sizeof (UNS8), (void*)&MasterDict_highestSubIndex_obj60C1}, 
	{ RW, int8, sizeof (UNS8), (void*)&InterpolationDataIndexNum}, 
	{ RW, int32, sizeof (UNS32), (void*)&InterpolationDataAxis1},
	{ RW, int32, sizeof (UNS32), (void*)&InterpolationDataAxis2},
};
/**************************************************************************/
/* Declaration of pointed variables                                       */
/**************************************************************************/

const indextable MasterDict_objdict[] = 
{
	{ (subindex*)MasterDict_Index1000,sizeof(MasterDict_Index1000)/sizeof(MasterDict_Index1000[0]), 0x1000},
	{ (subindex*)MasterDict_Index1001,sizeof(MasterDict_Index1001)/sizeof(MasterDict_Index1001[0]), 0x1001},
	{ (subindex*)MasterDict_Index1005,sizeof(MasterDict_Index1005)/sizeof(MasterDict_Index1005[0]), 0x1005},
	{ (subindex*)MasterDict_Index1006,sizeof(MasterDict_Index1006)/sizeof(MasterDict_Index1006[0]), 0x1006},
	{ (subindex*)MasterDict_Index1010,sizeof(MasterDict_Index1010)/sizeof(MasterDict_Index1010[0]), 0x1010},
	{ (subindex*)MasterDict_Index1011,sizeof(MasterDict_Index1011)/sizeof(MasterDict_Index1011[0]), 0x1011},
	{ (subindex*)MasterDict_Index1014,sizeof(MasterDict_Index1014)/sizeof(MasterDict_Index1014[0]), 0x1014},
	{ (subindex*)MasterDict_Index1016,sizeof(MasterDict_Index1016)/sizeof(MasterDict_Index1016[0]), 0x1016},
	{ (subindex*)MasterDict_Index1018,sizeof(MasterDict_Index1018)/sizeof(MasterDict_Index1018[0]), 0x1018},
	{ (subindex*)MasterDict_Index1200,sizeof(MasterDict_Index1200)/sizeof(MasterDict_Index1200[0]), 0x1200},
	{ (subindex*)MasterDict_Index1280,sizeof(MasterDict_Index1280)/sizeof(MasterDict_Index1280[0]), 0x1280},
	{ (subindex*)MasterDict_Index1400,sizeof(MasterDict_Index1400)/sizeof(MasterDict_Index1400[0]), 0x1400},
	{ (subindex*)MasterDict_Index1401,sizeof(MasterDict_Index1401)/sizeof(MasterDict_Index1401[0]), 0x1401},
	{ (subindex*)MasterDict_Index1600,sizeof(MasterDict_Index1600)/sizeof(MasterDict_Index1600[0]), 0x1600},
	{ (subindex*)MasterDict_Index1601,sizeof(MasterDict_Index1601)/sizeof(MasterDict_Index1601[0]), 0x1601},
	{ (subindex*)MasterDict_Index1800,sizeof(MasterDict_Index1800)/sizeof(MasterDict_Index1800[0]), 0x1800},
	{ (subindex*)MasterDict_Index1801,sizeof(MasterDict_Index1801)/sizeof(MasterDict_Index1801[0]), 0x1801},
	{ (subindex*)MasterDict_Index1A00,sizeof(MasterDict_Index1A00)/sizeof(MasterDict_Index1A00[0]), 0x1a00},
	{ (subindex*)MasterDict_Index1A01,sizeof(MasterDict_Index1A01)/sizeof(MasterDict_Index1A01[0]), 0x1a01},
	{ (subindex*)MasterDict_Index2000,sizeof(MasterDict_Index2000)/sizeof(MasterDict_Index2000[0]), 0x2000},
	{ (subindex*)MasterDict_Index2001,sizeof(MasterDict_Index2001)/sizeof(MasterDict_Index2001[0]), 0x2001},
	{ (subindex*)MasterDict_Index2100,sizeof(MasterDict_Index2100)/sizeof(MasterDict_Index2100[0]), 0x2100},
	{ (subindex*)MasterDict_Index2101,sizeof(MasterDict_Index2101)/sizeof(MasterDict_Index2101[0]), 0x2101},
	{ (subindex*)MasterDict_Index6040,sizeof(MasterDict_Index6040)/sizeof(MasterDict_Index6040[0]), 0x6040},
	{ (subindex*)MasterDict_Index6041,sizeof(MasterDict_Index6041)/sizeof(MasterDict_Index6041[0]), 0x6041},
	{ (subindex*)MasterDict_Index6062,sizeof(MasterDict_Index6062)/sizeof(MasterDict_Index6062[0]), 0x6062},
	{ (subindex*)MasterDict_Index60c1,sizeof(MasterDict_Index60c1)/sizeof(MasterDict_Index60c1[0]), 0x60c1},
};

const indextable * MasterDict_scanIndexOD (UNS16 wIndex, UNS32 * errorCode, ODCallback_t **Callback)
{
	int i;
	switch(wIndex){
		case 0x1000: i = 0;break;
		case 0x1001: i = 1;break;
		case 0x1005: i = 2;break;
		case 0x1006: i = 3;break;
		case 0x1010: i = 4;break;
		case 0x1011: i = 5;break;
		case 0x1014: i = 6;break;
		case 0x1016: i = 7;break;
		case 0x1018: i = 8;break;
		case 0x1200: i = 9;break;
		case 0x1280: i = 10;break;
		case 0x1400: i = 11;break;
		case 0x1401: i = 12;break;
		case 0x1600: i = 13;break;
		case 0x1601: i = 14;break;
		case 0x1800: i = 15;break;
		case 0x1801: i = 16;break;
		case 0x1a00: i = 17;break;
		case 0x1a01: i = 18;break;
		case 0x2000: i = 19;break;
		case 0x2001: i = 20;break;
		case 0x2100: i = 21;break;
		case 0x2101: i = 22;break;
		case 0x6040: i = 23;break;
		case 0x6041: i = 24;break;
		case 0x6062: i = 25;break;
		case 0x60c1: i = 26;break;
		default:
			*errorCode = OD_NO_SUCH_OBJECT;
			return NULL;
	}
	*errorCode = OD_SUCCESSFUL;
	*Callback=NULL;
	return &MasterDict_objdict[i];
}

/* 
 * To count at which received SYNC a PDO must be sent.
 * Even if no pdoTransmit are defined, at least one entry is computed
 * for compilations issues.
 */
s_PDO_status MasterDict_PDO_status[2] = {s_PDO_status_Initializer, s_PDO_status_Initializer};

const quick_index MasterDict_firstIndex = {
	9, /* SDO_SVR */
	10, /* SDO_CLT */
	11, /* PDO_RCV */
	13, /* PDO_RCV_MAP */
	15, /* PDO_TRS */
	17 /* PDO_TRS_MAP */
};

const quick_index MasterDict_lastIndex = {
	9, /* SDO_SVR */
	10, /* SDO_CLT */
	11, /* PDO_RCV */
	14, /* PDO_RCV_MAP */
	16, /* PDO_TRS */
	18 /* PDO_TRS_MAP */
};

const UNS16 MasterDict_ObjdictSize = sizeof(MasterDict_objdict)/sizeof(MasterDict_objdict[0]); 

CO_Data MasterDict_Data = CANOPEN_NODE_DATA_INITIALIZER(MasterDict);

