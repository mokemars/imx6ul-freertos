//###########################################################################
// FILE:   hw_pbist.h
// TITLE:  Register defines for accessing the PBIST controller
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef HW_PBIST_H_
#define HW_PBIST_H_

//*****************************************************************************
// The following are defines for the PBIST register offsets.
//*****************************************************************************
#define PBIST_RAMT							0x00000160	// RAM Configuration Register
#define PBIST_DLR							0x00000164	// Datalogger Register
#define PBIST_STR							0x0000016C	// Program Control Register
#define PBIST_PACT          				0x00000180	// PBIST Activate/ROM Clock Enable Register
#define PBIST_OVER							0x00000188	// Override Register
#define PBIST_FSRF0							0x00000190	// Fail Status Fail Register 0
#define PBIST_FSRF1							0x00000194	// Fail Status Fail Register 1
#define PBIST_FSRC0							0x00000198	// Fail Status Count Register 0
#define PBIST_FSRC1							0x0000019C	// Fail Status Count Register 1
#define PBIST_FSRA0 						0x000001A0	// Fail Status Address Register 0
#define PBIST_FSRA1 						0x000001A4	// Fail Status Address Register 1
#define PBIST_FSRDL0 						0x000001A8	// Fail Status Data Register 0
#define PBIST_FSRDL1						0x000001B0	// Fail Status Data Register 1
#define PBIST_ALGO		    				0x000001C4	// Algorithm Mask Register
#define PBIST_RINFOL						0x000001C8	// RAM Info Mask Lower Register
#define PBIST_RINFOU						0x000001CC	// RAM Info Mask Upper Register

//*****************************************************************************
// The following are defines for the bit fields in the PBIST_RAMT register.
//*****************************************************************************
#define PBIST_RAMT_RGS_M					0xFF000000	// RAM Group Select Mask
#define PBIST_RAMT_RGS_S					24
#define PBIST_RAMT_RDS_M					0x00FF0000	// Return Data Select Mask
#define PBIST_RAMT_RDS_S					16


//*****************************************************************************
// The following are defines for the bit fields in the PBIST_DLR register.
//*****************************************************************************
#define PBIST_DLR_ROM						0x00000004	// ROM-Based Testing Mode
#define PBIST_DLR_CPUAM						0x00000010	// CPU Access Mode
#define PBIST_DLR_GNG						0x00000200	// GO/NO-GO Testing Mode

//*****************************************************************************
// The following are defines for the bit fields in the PBIST_STR register.
//*****************************************************************************
#define PBIST_STR_RESUME					0x00000002	// Resume PBIST until complete
#define PBIST_STR_RESUME_S					1
#define PBIST_STR_STOP						0x00000004	// Stop PBIST
#define PBIST_STR_STOP_S					2

//*****************************************************************************
// The following are defines for the bit fields in the PBIST_PACT register.
//*****************************************************************************
#define PBIST_PACT_CLKEN					0x00000001	// PBIST controller Clock Enable

//*****************************************************************************
// The following are defines for the bit fields in the PBIST_OVER register.
//*****************************************************************************
#define PBIST_OVER_RINFO					0x00000001	// RINFO Override
#define PBIST_OVER_ALGO						0x00000008	// Algorithm Override

//*****************************************************************************
// The following are defines for the bit fields in the PBIST_FSRF0 register.
//*****************************************************************************
#define PBIST_FSRF0_FS0						0x00000001	// Fail Status Port 0

//*****************************************************************************
// The following are defines for the bit fields in the PBIST_FSRF1 register.
//*****************************************************************************
#define PBIST_FSRF1_FS1						0x00000001	// Fail Status Port 1

//*****************************************************************************
// The following are defines for the bit fields in the PBIST_FSRC0 register.
//*****************************************************************************
#define PBIST_FSRC0_C0						0x000000FF	// Fail Count Port 0
#define PBIST_FSRC0_C0_S					0

//*****************************************************************************
// The following are defines for the bit fields in the PBIST_FSRC1 register.
//*****************************************************************************
#define PBIST_FSRC1_C1						0x000000FF	// Fail Count Port 1
#define PBIST_FSRC1_C1_S					0

//*****************************************************************************
// The following are defines for the bit fields in the PBIST_FSRA0 register.
//*****************************************************************************
#define PBIST_FSRA0_FSA0					0x0000FFFF	// Fail Status Address Port 0
#define PBIST_FSRA0_FSA0_S					0

//*****************************************************************************
// The following are defines for the bit fields in the PBIST_FSRA1 register.
//*****************************************************************************
#define PBIST_FSRA1_FSA1					0x0000FFFF	// Fail Status Address Port 1
#define PBIST_FSRA1_FSA1_S					0

//*****************************************************************************
// The following are defines for the bit fields in the PBIST_FSRDL0 register.
//*****************************************************************************
#define PBIST_FSRDL0_FD0					0xFFFFFFFF	// Failure Data Port 0
#define PBIST_FSRDL0_FD0_S					0

//*****************************************************************************
// The following are defines for the bit fields in the PBIST_FSRDL1 register.
//*****************************************************************************
#define PBIST_FSRDL1_FD1					0xFFFFFFFF	// Failure Data Port 1
#define PBIST_FSRDL1_FD1_S					0


#endif /* HW_PBIST_H_ */
