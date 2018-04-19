//###########################################################################
// FILE:   hw_ucrc.h
// TITLE:  Macros and defines used when accessing the ucrc hardware.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_UCRC_H__
#define __HW_UCRC_H__

//*****************************************************************************
// The following are defines for the UCRC register offsets.
//*****************************************************************************
#define UCRC_O_CONFIG            0x00000000 // uCRC Configuration Register
#define UCRC_O_CONTROL           0x00000004 // uCRC Control Register
#define UCRC_O_RES               0x00000008 // uCRC Result Register for
                                            // Unsecured Memory

//*****************************************************************************
// The following are defines for the bit fields in the uCRC Config register.
//*****************************************************************************
#define UCRC_CONFIG_CRCTYPE_M    0x00000003 // uCRC Type 

//*****************************************************************************
// The following are defines for the bit fields in the uCRC Control register.
//*****************************************************************************
#define UCRC_CONTROL_CLEAR       0x00000001 // uCRC Clear Result Registers
#define UCRC_CONTROL_SFTRST      0x00000002 // uCRC Complete Reset


#endif // __HW_UCRC_H__



