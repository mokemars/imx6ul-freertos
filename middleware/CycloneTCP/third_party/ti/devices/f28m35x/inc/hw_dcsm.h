//###########################################################################
// FILE:   hw_dcsm.h
// TITLE:  Macros and defines used when accessing the dcsm hardware.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_DCSM_H__
#define __HW_DCSM_H__

//*****************************************************************************
// The following are defines for the DCSM register offsets.
//*****************************************************************************
#define DCSM_O_Z1_CSMKEY0             0x00000000 // 128-Bit Register for
                                                 // M3-Zone 1
#define DCSM_O_Z1_CSMKEY1             0x00000004 // 128-Bit Register for
                                                 // M3-Zone 1
#define DCSM_O_Z1_CSMKEY2             0x00000008 // 128-Bit Register for
                                                 // M3-Zone 1
#define DCSM_O_Z1_CSMKEY3             0x0000000C // 128-Bit Register for
                                                 // M3-Zone 1
#define DCSM_O_Z1_ECSLKEY0            0x00000010 // 64-Bit Register for M3-Zone
                                                 // 1
#define DCSM_O_Z1_ECSLKEY1            0x00000014 // 64-Bit Register for M3-Zone
                                                 // 1
#define DCSM_O_Z2_CSMKEY0             0x00000018 // 128-Bit Register for
                                                 // M3-Zone 2
#define DCSM_O_Z2_CSMKEY1             0x0000001C // 128-Bit Register for
                                                 // M3-Zone 2
#define DCSM_O_Z2_CSMKEY2             0x00000020 // 128-Bit Register for
                                                 // M3-Zone 2
#define DCSM_O_Z2_CSMKEY3             0x00000024 // 128-Bit Register for
                                                 // M3-Zone 2
#define DCSM_O_Z2_ECSLKEY0            0x00000028 // 64-Bit Register for M3-Zone
                                                 // 2
#define DCSM_O_Z2_ECSLKEY1            0x0000002C // 64-Bit Register for M3-Zone
                                                 // 2
#define DCSM_O_Z1_CR                  0x00000080 // Status and Control for
                                                 // M3-Zone 1
#define DCSM_O_Z2_CR                  0x00000084 // Status and Control for
                                                 // M3-Zone 2
#define DCSM_O_Z1_GRABSECTR           0x00000090 // Grab Flash Sectors for
                                                 // M3-Zone 1
#define DCSM_O_Z1_GRABRAMR            0x00000094 // Grab RAM Blocks for M3-Zone
                                                 // 1
#define DCSM_O_Z2_GRABSECTR           0x00000098 // Grab Flash Sectors for
                                                 // M3-Zone 2
#define DCSM_O_Z2_GRABRAMR            0x0000009C // Grab RAM Blocks for M3-Zone
                                                 // 2
#define DCSM_O_Z1_EXEONLYR            0x000000B0 // Execute Only Select for
                                                 // Flash M3-Zone 1
#define DCSM_O_Z2_EXEONLYR            0x000000B4 // Execute Only Select for
                                                 // Flash M3-Zone 2

//*****************************************************************************
// The following are defines for the bit fields in the M3 Zx ECSLKEYx register.
//*****************************************************************************
#define DCSM_M3Zx_ECSLKEYx_S          0
#define DCSM_M3Zx_ECSLKEYx_M          0x0000FFFF // DCSM ECSL Password Register

//*****************************************************************************
// The following are defines for the bit fields in the M3 Zx CR register.
//*****************************************************************************
#define DCSM_CR_CSMALLZERO            0x00000020        // M3 Zx CSM contains 
                                                        // all 0's
#define DCSM_CR_ECSLALLZERO           0x00000040        // M3 Zx ECSL contains 
                                                        // all 0's
#define DCSM_CR_CSMALLONE             0x00000080        // M3 Zx CSM contains 
                                                        // all 1's
#define DCSM_CR_ECSLALLONE            0x00000100        // M3 Zx ECSL contains 
                                                        // all 1's
#define DCSM_CR_CSMMATCH              0x00000200        // M3 Zx CSM key 
                                                        // matches pswd Register
#define DCSM_CR_ECSLMATCH             0x00000400        // M3 Zx ECSL key 
                                                        // matches pswd Lock
                                                        // Register
#define DCSM_CR_CSMARMED              0x00000800        // M3 Zx CSM is in Armed
                                                        // State
#define DCSM_CR_ECSLARMED             0x00001000        // M3 Zx ECSL is in Armed
                                                        // State
#define DCSM_CR_FORCESEC              0x00008000        // Forces Zx secure State

//*******************************************************************************
// The following are defines for the bit fields in the M3 Zx GRABSECTR
// register.
//*******************************************************************************
#define DCSM_CR_GRABSECTRM_S          0
#define DCSM_CR_GRABSECTRL_S          2
#define DCSM_CR_GRABSECTRK_S          4
#define DCSM_CR_GRABSECTRJ_S          6
#define DCSM_CR_GRABSECTRI_S          8
#define DCSM_CR_GRABSECTRH_S          10
#define DCSM_CR_GRABSECTRG_S          12
#define DCSM_CR_GRABSECTRF_S          14
#define DCSM_CR_GRABSECTRE_S          16
#define DCSM_CR_GRABSECTRD_S          18
#define DCSM_CR_GRABSECTRC_S          20
#define DCSM_CR_GRABSECTRB_S          22

#define DCSM_CR_GRABSECTRM_M          0x00000003 // Requesting to grab SectM
                                                 // for use
#define DCSM_CR_GRABSECTRL_M          0x0000000C // Requesting to grab SectL
                                                 // for use
#define DCSM_CR_GRABSECTRK_M          0x00000030 // Requesting to grab SectK
                                                 // for use
#define DCSM_CR_GRABSECTRJ_M          0x000000C0 // Requesting to grab SectJ
                                                 // for use
#define DCSM_CR_GRABSECTRI_M          0x00000300 // Requesting to grab SectI
                                                 // for use
#define DCSM_CR_GRABSECTRH_M          0x00000C00 // Requesting to grab SectH
                                                 // for use
#define DCSM_CR_GRABSECTRG_M          0x00003000 // Requesting to grab SectG
                                                 // for use
#define DCSM_CR_GRABSECTRF_M          0x0000C000 // Requesting to grab SectF
                                                 // for use
#define DCSM_CR_GRABSECTRE_M          0x00030000 // Requesting to grab SectE
                                                 // for use
#define DCSM_CR_GRABSECTRD_M          0x000C0000 // Requesting to grab SectD
                                                 // for use
#define DCSM_CR_GRABSECTRC_M          0x00300000 // Requesting to grab SectC
                                                 // for use
#define DCSM_CR_GRABSECTRB_M          0x00C00000 // Requesting to grab SectB
                                                 // for use

//*******************************************************************************
// The following are defines for the bit fields in the M3 Zx GRABRAMR register.
//*******************************************************************************
#define DCSM_CR_GRABRAMRx_C0_S        0
#define DCSM_CR_GRABRAMRx_C1_S        2
#define DCSM_CR_GRABRAMRx_C0_M        0x00000003 // Requesting to grab RAM
                                                 // block C0
#define DCSM_CR_GRABRAMRx_C1_M        0x0000000C // Requesting to grab RAM
                                                 // block C1

//*******************************************************************************
// The following are defines for the bit fields in the M3 Zx EXEONLYR register.
//*******************************************************************************
#define DCSM_EXEONLYR_SECTN           0x00000001 // M3 CSM does not turn on Exe
                                                 // Only Sect N Flash
#define DCSM_EXEONLYR_SECTM           0x00000002 // M3 CSM does not turn on Exe
                                                 // Only Sect M Flash
#define DCSM_EXEONLYR_SECTL           0x00000004 // M3 CSM does not turn on Exe
                                                 // Only Sect L Flash
#define DCSM_EXEONLYR_SECTK           0x00000008 // M3 CSM does not turn on Exe
                                                 // Only Sect K Flash
#define DCSM_EXEONLYR_SECTJ           0x00000010 // M3 CSM does not turn on Exe
                                                 // Only Sect J Flash
#define DCSM_EXEONLYR_SECTI           0x00000020 // M3 CSM does not turn on Exe
                                                 // Only Sect I Flash
#define DCSM_EXEONLYR_SECTH           0x00000040 // M3 CSM does not turn on Exe
                                                 // Only Sect H Flash
#define DCSM_EXEONLYR_SECTG           0x00000080 // M3 CSM does not turn on Exe
                                                 // Only Sect G Flash
#define DCSM_EXEONLYR_SECTF           0x00000100 // M3 CSM does not turn on Exe
                                                 // Only Sect F Flash
#define DCSM_EXEONLYR_SECTE           0x00000200 // M3 CSM does not turn on Exe
                                                 // Only Sect E Flash
#define DCSM_EXEONLYR_SECTD           0x00000400 // M3 CSM does not turn on Exe
                                                 // Only Sect D Flash
#define DCSM_EXEONLYR_SECTC           0x00000800 // M3 CSM does not turn on Exe
                                                 // Only Sect C Flash
#define DCSM_EXEONLYR_SECTB           0x00001000 // M3 CSM does not turn on Exe
                                                 // Only Sect B Flash
#define DCSM_EXEONLYR_SECTA           0x00002000 // M3 CSM does not turn on Exe
                                                 // Only Sect A Flash


#endif // __HW_DCSM_H__







