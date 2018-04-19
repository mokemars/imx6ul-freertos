//###########################################################################
// FILE:   hw_nmi.h
// TITLE:  Macros and defines used when accessing the NMI registers.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_NMI_H__
#define __HW_NMI_H__

//*****************************************************************************
// The following are defines for the NMI register offsets.
//*****************************************************************************
#define NMI_O_CFG                  0x00000000 // M3 NMI Configuration Register
#define NMI_O_FLG                  0x00000004 // M3 NMI Flag Register
#define NMI_O_FLGCLR               0x00000008 // M3 NMI Flag Clear Register
#define NMI_O_FLGFRC               0x0000000C // M3 NMI Flag Force Register
#define NMI_O_WDCNT                0x00000010 // M3 NMI Watchdog Counter
                                              // Register
#define NMI_O_WDPRD                0x00000014 // M3 NMI Watchdog Period
                                              // Register

//*****************************************************************************
// The following are defines for the bit fields in the M3 NMI Config register.
//*****************************************************************************
#define NMI_CFG_EN                 0x00000001 // NMI Enable Bit
#define NMI_CFG_AERRE              0x00000200 // AERR NMI Enable

//*****************************************************************************
// The following are defines for the bit fields in the M3 NMI Flag register.
//*****************************************************************************
#define NMI_FLG_NMIINT             0x00000001 // NMI INT Flag
#define NMI_FLG_CLOCKFAIL          0x00000002 // Clock Fail NMI Flag
#define NMI_FLG_M3BISTERR          0x00000010 // M3 HW BIST Error NMI Flag
#define NMI_FLG_C28BISTERR         0x00000020 // C28 HW BIST Timeout NMI Flag
#define NMI_FLG_EXTGPIO            0x00000040 // GPIO NMI Flag
#define NMI_FLG_C28PIENMIERR       0x00000080 // C28 PIE NMI Error Flag
#define NMI_FLG_C28NMIWDRST        0x00000100 // C28 NMI Watchdog Reset Flag
#define NMI_FLG_AERR               0x00000200 // Analog Error NMI Flag

//*****************************************************************************
// The following are defines for the bit fields in the M3 NMI Flag Clear
// register.
//*****************************************************************************
#define NMI_FLGCLR_NMIINT          0x00000001 // NMI INT Flag Clear
#define NMI_FLGCLR_CLOCKFAIL       0x00000002 // Clock Fail NMI Flag Clear
#define NMI_FLGCLR_M3BISTERR       0x00000010 // M3 HW BIST Error NMI Flag
                                              // Clear
#define NMI_FLGCLR_C28BISTERR      0x00000020 // C28 HW BIST Timeout NMI Flag
                                              // Clear
#define NMI_FLGCLR_EXTGPIO         0x00000040 // GPIO NMI Flag Clear
#define NMI_FLGCLR_C28PIENMIERR    0x00000080 // C28 PIE NMI Error Flag Clear
#define NMI_FLGCLR_C28NMIWDRST     0x00000100 // C28 NMI Watchdog Reset Flag
                                              // Clear
#define NMI_FLGCLR_AERR            0x00000200 // Analog Error NMI Flag Clear

//*****************************************************************************
// The following are defines for the bit fields in the M3 NMI Flag Force
// register.
//*****************************************************************************
#define NMI_FLGFRC_NMIINT          0x00000001 // NMI INT Flag Force
#define NMI_FLGFRC_CLOCKFAIL       0x00000002 // Clock Fail NMI Flag Force
#define NMI_FLGFRC_M3BISTERR       0x00000010 // M3 HW BIST Error NMI Flag
                                              // Force
#define NMI_FLGFRC_C28BISTERR      0x00000020 // C28 HW BIST Timeout NMI Flag
                                              // Force
#define NMI_FLGFRC_EXTGPIO         0x00000040 // GPIO NMI Flag Force
#define NMI_FLGFRC_C28PIENMIERR    0x00000080 // C28 PIE NMI Error Flag Force
#define NMI_FLGFRC_C28NMIWDRST     0x00000100 // C28 NMI Watchdog Reset Flag
                                              // Force
#define NMI_FLGFRC_AERR            0x00000200 // Analog Error Flag Force

//*****************************************************************************
// The following are defines for the bit fields in the M3 NMI WD Count
// register.
//*****************************************************************************
#define NMI_WDCNT_M                0x0000FFFF // M3 NMI WD Count

//*****************************************************************************
// The following are defines for the bit fields in the M3 NMI WD Period
// register.
//*****************************************************************************
#define NMI_WDPRD_M                0x0000FFFF // M3 NMI WD Period


#endif // __HW_NMI_H__












