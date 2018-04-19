//###########################################################################
// FILE:   hw_watchdog.h
// TITLE:  Macros used when accessing the Watchdog Timer hardware.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_WATCHDOG_H__
#define __HW_WATCHDOG_H__

//*****************************************************************************
// The following are defines for the Watchdog Timer register offsets.
//*****************************************************************************
#define WDT_O_LOAD              0x00000000  // Watchdog Load
#define WDT_O_VALUE             0x00000004  // Watchdog Value
#define WDT_O_CTL               0x00000008  // Watchdog Control
#define WDT_O_ICR               0x0000000C  // Watchdog Interrupt Clear
#define WDT_O_RIS               0x00000010  // Watchdog Raw Interrupt Status
#define WDT_O_MIS               0x00000014  // Watchdog Masked Interrupt Status
#define WDT_O_TEST              0x00000418  // Watchdog Test
#define WDT_O_LOCK              0x00000C00  // Watchdog Lock

//*****************************************************************************
// The following are defines for the bit fields in the WDT_O_LOAD register.
//*****************************************************************************
#define WDT_LOAD_M              0xFFFFFFFF  // Watchdog Load Value
#define WDT_LOAD_S              0

//*****************************************************************************
// The following are defines for the bit fields in the WDT_O_VALUE register.
//*****************************************************************************
#define WDT_VALUE_M             0xFFFFFFFF  // Watchdog Value
#define WDT_VALUE_S             0

//*****************************************************************************
// The following are defines for the bit fields in the WDT_O_CTL register.
//*****************************************************************************
#define WDT_CTL_WRC             0x80000000  // Write Complete
#define WDT_CTL_RESEN           0x00000002  // Watchdog Reset Enable
#define WDT_CTL_INTEN           0x00000001  // Watchdog Interrupt Enable

//*****************************************************************************
// The following are defines for the bit fields in the WDT_O_ICR register.
//*****************************************************************************
#define WDT_ICR_M               0xFFFFFFFF  // Watchdog Interrupt Clear
#define WDT_ICR_S               0

//*****************************************************************************
// The following are defines for the bit fields in the WDT_O_RIS register.
//*****************************************************************************
#define WDT_RIS_WDTRIS          0x00000001  // Watchdog Raw Interrupt Status

//*****************************************************************************
// The following are defines for the bit fields in the WDT_O_MIS register.
//*****************************************************************************
#define WDT_MIS_WDTMIS          0x00000001  // Watchdog Masked Interrupt Status

//*****************************************************************************
// The following are defines for the bit fields in the WDT_O_TEST register.
//*****************************************************************************
#define WDT_TEST_STALL          0x00000100  // Watchdog Stall Enable

//*****************************************************************************
// The following are defines for the bit fields in the WDT_O_LOCK register.
//*****************************************************************************
#define WDT_LOCK_M              0xFFFFFFFF  // Watchdog Lock
#define WDT_LOCK_UNLOCKED       0x00000000  // Unlocked
#define WDT_LOCK_LOCKED         0x00000001  // Locked
#define WDT_LOCK_UNLOCK         0x1ACCE551  // Unlocks the watchdog timer

//*****************************************************************************
// The following are defines for the bit fields in the WDT_ISR, WDT_RIS, and
// WDT_MIS registers.
//*****************************************************************************
#define WDT_INT_TIMEOUT         0x00000001  // Watchdog timer expired


#endif // __HW_WATCHDOG_H__



