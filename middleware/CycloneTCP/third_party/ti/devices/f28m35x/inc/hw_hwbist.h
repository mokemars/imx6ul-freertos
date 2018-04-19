//###########################################################################
// FILE:   hw_hwbist.h
// TITLE:  Macros used when accessing the HWBIST controller
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_HWBIST_H__
#define __HW_HWBIST_H__

//*****************************************************************************
// The following are defines for the HWBIST register offsets.
//*****************************************************************************
#define HWBIST_O_MSTCGCR0       0x00000000   // STC Global Control Register 0
#define HWBIST_O_MSTCGCR1       0x00000004   // STC Global Control Register 1
#define HWBIST_O_MSTCGCR2       0x00000008   // STC Global Control Register 2
#define HWBIST_O_MSTCGCR3       0x0000000C   // STC Global Control Register 3
#define HWBIST_O_MSTCGCR4       0x00000010   // STC Global Control Register 4
#define HWBIST_O_MSTCGCR5       0x00000014   // STC Global Control Register 5
#define HWBIST_O_MSTCGCR6       0x00000018   // STC Global Control Register 6
#define HWBIST_O_MSTCGCR7       0x0000001C   // STC Global Control Register 7
#define HWBIST_O_MSTCGCR8       0x00000020   // STC Global Control Register 8
#define HWBIST_O_MSTCPCNT       0x00000024   // STC Pattern Count Register
#define HWBIST_O_MSTCCONFIG     0x00000028   // STC Registers Configuration
                                            // Status
#define HWBIST_O_MSTCSADDR      0x0000002C   // STC ROM Start Address Register
#define HWBIST_O_MSTCTEST       0x00000030   // M3 HWBIST Test Register
#define HWBIST_O_MSTCRET        0x00000034   // M3 Return PC Address
#define HWBIST_O_MSTCCRD        0x00000038   // M3 Context Restore Done
                                            // Register
#define HWBIST_O_MSTCGSTAT      0x00000040   // STC Global Status Register
#define HWBIST_O_MSTCCPCR       0x00000048   // STC Current Pattern Count
                                            // Register
#define HWBIST_O_MSTCCADDR      0x0000004C   // STC Current ROM Address
                                            // Register
#define HWBIST_O_MSTCMISR       0x00000050   // MISR Result Register Base
#define HWBIST_O_MSTCMISR0      0x00000050   // MISR Result Register 0
#define HWBIST_O_MSTCMISR1      0x00000054   // MISR Result Register 1
#define HWBIST_O_MSTCMISR2      0x00000058   // MISR Result Register 2
#define HWBIST_O_MSTCMISR3      0x0000005C   // MISR Result Register 3

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCGCR0
// register.
//*****************************************************************************
#define HWBIST_MSTCGCR0_BISTDISABLE_M   0x0000000F   // HWBIST Disable
#define HWBIST_MSTCGCR0_BISTDISABLE_S   0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCGCR1
// register.
//*****************************************************************************
#define HWBIST_MSTCGCR1_MICROCFG_M      0xFFFFFFFF   // Micro Configuration
#define HWBIST_MSTCGCR1_MICROCFG_S      0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCGCR2
// register.
//*****************************************************************************
#define HWBIST_MSTCGCR2_MACROCFG_M      0x0000FFFF   // Macro Configuration
#define HWBIST_MSTCGCR2_MACROCFG_S      0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCGCR3
// register.
//*****************************************************************************
#define HWBIST_MSTCGCR3_ILS_M           0x0000000F   // Interrupt Logging Start
#define HWBIST_MSTCGCR3_ILS_S           0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCGCR4
// register.
//*****************************************************************************
#define HWBIST_MSTCGCR4_BISTGO_M        0x0000000F   // BIST Start
#define HWBIST_MSTCGCR4_BISTGO_S        0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCGCR5
// register.
//*****************************************************************************
#define HWBIST_MSTCGCR5_RESTART_M       0x0000000F   // Restart Enable
#define HWBIST_MSTCGCR5_RESTART_S       0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCGCR6
// register.
//*****************************************************************************
#define HWBIST_MSTCGCR6_COV_M           0x00000003   // Coverage
#define HWBIST_MSTCGCR6_COV_S           0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCGCR7
// register.
//*****************************************************************************
#define HWBIST_MSTCGCR7_SCD_M           0x000C0000   // Shift Clock Divison
#define HWBIST_MSTCGCR7_SCD_S           18
#define HWBIST_MSTCGCR7_PST_M           0x00030000   // Pattern Set Type
#define HWBIST_MSTCGCR7_PST_S           16
#define HWBIST_MSTCGCR7_NP_M            0x0000F000   // Num of Pipeline Stages
#define HWBIST_MSTCGCR7_NP_S            12
#define HWBIST_MSTCGCR7_DC_M            0x00000F00   // Dead Cycles
#define HWBIST_MSTCGCR7_DC_S            8
#define HWBIST_MSTCGCR7_MCL_M           0x000000FF   // Max Chain Length
#define HWBIST_MSTCGCR7_MCL_S           0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCGCR8
// register.
//*****************************************************************************
#define HWBIST_MSTCGCR8_BMCPC_M         0x0000FFFF   // MISR Compare Pattern Cnt
#define HWBIST_MSTCGCR8_BMCPC_S         0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCPCNT
// register.
//*****************************************************************************
#define HWBIST_MSTCPCNT_PCNT99_M        0xFFFF0000   // Patterns for 99%
                                                    // Coverage
#define HWBIST_MSTCPCNT_PCNT99_S        16
#define HWBIST_MSTCPCNT_PCNT95_M        0x0000FFFF   // Patterns for 95%
                                                    // Coverage
#define HWBIST_MSTCPCNT_PCNT95_S        0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCCONFIG
// register.
//*****************************************************************************
#define HWBIST_MSTCCONFIG_CFGDONE_M     0x0000000F   // Configuration Done
#define HWBIST_MSTCCONFIG_CFGDONE_S     0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCSADDR
// register.
//*****************************************************************************
#define HWBIST_MSTCSADDR_SAMISR_M       0xFFFF0000   // MISR ROM Start Address
#define HWBIST_MSTCSADDR_SAMISR_S       16
#define HWBIST_MSTCSADDR_SAPAT_M        0x0000FFFF   // Pattern ROM Start
                                                    // Address
#define HWBIST_MSTCSADDR_SAPAT_S        0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCTEST
// register.
//*****************************************************************************
#define HWBIST_MSTCTEST_TEST_M          0xFFFFF000   // Test Bits
#define HWBIST_MSTCTEST_TEST_S          12
#define HWBIST_MSTCTEST_TESTNMI_M       0x00000F00   // Test NMI
#define HWBIST_MSTCTEST_TESTNMI_S       8
#define HWBIST_MSTCTEST_TESTCMPFAIL_M   0x000000F0   // Test MISR Compare Fail
#define HWBIST_MSTCTEST_TESTCMPFAIL_S   4
#define HWBIST_MSTCTEST_TESTTO_M        0x0000000F   // Test Time Out
#define HWBIST_MSTCTEST_TESTTO_S        0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCRET
// register.
//*****************************************************************************
#define HWBIST_MSTCRET_RETADDR_M        0xFFFFFFFF   // Return Address
#define HWBIST_MSTCRET_RETADDR_S        0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCCRD
// register.
//*****************************************************************************
#define HWBIST_MSTCCRD_RESTOREDONE_M    0x0000001F   // Context Restore Done
#define HWBIST_MSTCCRD_RESTOREDONE_S    0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCGSTAT
// register.
//*****************************************************************************
#define HWBIST_MSTCGSTAT_TOFAIL         0x00000020   // Time Out Failure
#define HWBIST_MSTCGSTAT_INTCMPF        0x00000010   // Intermediate Comparison
                                                    // Failure
#define HWBIST_MSTCGSTAT_BISTFAIL       0x00000008   // HWBIST Failure
#define HWBIST_MSTCGSTAT_NMI            0x00000004   // Exit Due to NMI
#define HWBIST_MSTCGSTAT_MACRODONE      0x00000002   // Macro Test Slot Complete
#define HWBIST_MSTCGSTAT_BISTDONE       0x00000001   // HWBIST Complete

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCCPCR
// register.
//*****************************************************************************
#define HWBIST_MSTCCPCR_PATCNT_M        0x0000FFFF   // Current Pattern Coutn
#define HWBIST_MSTCCPCR_PATCNT_S        0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCCADDR
// register.
//*****************************************************************************
#define HWBIST_MSTCCADDR_MISRADDR_M     0xFFFF0000   // Current MISR ROM Address
#define HWBIST_MSTCCADDR_MISRADDR_S     16
#define HWBIST_MSTCCADDR_PATADDR_M      0x0000FFFF   // Current Pattern ROM
                                                    // Address
#define HWBIST_MSTCCADDR_PATADDR_S      0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCMISR0
// register.
//*****************************************************************************
#define HWBIST_MSTCMISR0_MISR0_M        0xFFFFFFFF   // Result MISR 0
#define HWBIST_MSTCMISR0_MISR0_S        0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCMISR1
// register.
//*****************************************************************************
#define HWBIST_MSTCMISR1_MISR1_M        0xFFFFFFFF   // Result MISR 1
#define HWBIST_MSTCMISR1_MISR1_S        0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCMISR2
// register.
//*****************************************************************************
#define HWBIST_MSTCMISR2_MISR2_M        0xFFFFFFFF   // Result MISR 2
#define HWBIST_MSTCMISR2_MISR2_S        0

//*****************************************************************************
// The following are defines for the bit fields in the HWBIST_O_MSTCMISR3
// register.
//*****************************************************************************
#define HWBIST_MSTCMISR3_MISR3_M        0xFFFFFFFF   // Result MISR 3
#define HWBIST_MSTCMISR3_MISR3_S        0

#endif // __HW_HWBIST_H__
