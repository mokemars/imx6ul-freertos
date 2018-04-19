//###########################################################################
// FILE:   hw_cram.h
// TITLE:  Macros and defines used when accessing the Control Subsystem
//         RAM registers through IPC
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_CRAM_H__
#define __HW_CRAM_H__

//*****************************************************************************
// The following are the address for the Control Subsystem RAM registers
//*****************************************************************************

#define CRAM_CRTESTINIT                   0x4920     // M0, M1 AND C28 to M3
                                                     // Message RAM TEST and
                                                     // INIT Register
#define CRAM_CRINITDONE                   0x4930     // M0, M1 AND C28T0M3
                                                     // MSGRAM INIT Done
                                                     // Register
#define CRAM_CLxRTESTINIT1                0x4922     // C28 Lx RAM_TEST and
                                                     // INIT Register 
#define CRAM_CLxRINITDONE                 0x4932     // C28 Lx RAM_INIT_DONE
                                                     // Register

//*****************************************************************************
// The following are defines for the bit fields in the C28 CRTESTINIT register
//*****************************************************************************
#define CRTESTINIT_RAMINIT_M0             0x0001     // RAM Init M0 Memory
#define CRTESTINIT_RAMINIT_M1             0x0004     // RAM Init M1 Memory
#define CRTESTINIT_RAMINIT_CTOMMSGRAM     0x0010     // RAM Init CtoM Message
                                                     // RAM

//*****************************************************************************
// The following are defines for the bit fields in the C28 CRINITDONE register
//*****************************************************************************
#define CRINITDONE_RAMINITDONE_M0         0x0001     // RAM Init M0 Memory Done
#define CRINITDONE_RAMINITDONE_M1         0x0004     // RAM Init M1 Memory Done
#define CRINITDONE_RAMINITDONE_CTOMMSGRAM 0x0010     // RAM Init CtoM Message
                                                     // RAM Done

//*****************************************************************************
// The following are defines for the bit fields in the C28 CLxRTESTINIT1
// register
//*****************************************************************************
#define CLxRTESTINIT1_RAMINIT_L0          0x0001     // RAM Init L0 Memory
#define CLxRTESTINIT1_RAMINIT_L1          0x0004     // RAM Init L1 Memory
#define CLxRTESTINIT1_RAMINIT_L2          0x0010     // RAM Init L2 Memory
#define CLxRTESTINIT1_RAMINIT_L3          0x0040     // RAM Init L3 Memory

//*****************************************************************************
// The following are defines for the bit fields in the C28 CLxRINITDONE
// register
//*****************************************************************************
#define CLxRINITDONE_RAMINITDONE_L0          0x0001     // RAM Init L0 Memory 
                                                        // Done
#define CLxRINITDONE_RAMINITDONE_L1          0x0004     // RAM Init L1 Memory 
                                                        // Done
#define CLxRINITDONE_RAMINITDONE_L2          0x0010     // RAM Init L2 Memory 
                                                        // Done
#define CLxRINITDONE_RAMINITDONE_L3          0x0040     // RAM Init L3 Memory 
                                                        // Done

#endif // __HW_CRAM_H__

