//###########################################################################
// FILE:   hw_ram.h
// TITLE:  Macros and defines used when accessing the RAM registers.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_RAM_H__
#define __HW_RAM_H__

//***************************************************************************
// The following are defines for the RAM Config register offsets.
//***************************************************************************
#define RAM_O_CXDRCR1                               0x00000000 // Cx Dedicated
                                                               // RAM Config 1
#define RAM_O_CXSRCR1                               0x00000008 // Cx Shared RAM
                                                               // (CPU and
                                                               // uDMA) Config
                                                               // 1
#define RAM_O_CXSRCR2                               0x0000000C // Cx Shared RAM
                                                               // (CPU and
                                                               // uDMA) Config
                                                               // 2
#define RAM_O_MSXMSEL                               0x00000010 // Sx SHRAM
                                                               // Master Select
#define RAM_O_MSXSRCR1                              0x00000020 // Sx SHRAM
                                                               // Config 1
#define RAM_O_MSXSRCR2                              0x00000024 // Sx SHRAM
                                                               // Config 2
#define RAM_O_MTOCMSGRCR                            0x00000030 // M to C
                                                               // Message RAM
                                                               // Config
#define RAM_O_CXSRCR3                               0x00000034 // Cx Shared RAM
                                                               // (CPU and
                                                               // uDMA) Config
                                                               // 3
#define RAM_O_CXSRCR4                               0x00000038 // Cx Shared RAM
                                                               // (CPU and
                                                               // uDMA) Config
                                                               // 4
#define RAM_O_CXRTESTINIT1                          0x00000040 // Cx RAM Test
                                                               // and Init 1
#define RAM_O_MSXRTESTINIT1                         0x00000050 // Sx RAM Test
                                                               // and Init 1
#define RAM_O_MTOCCRTESTINIT1                       0x00000060 // M to C
                                                               // Message RAM
                                                               // Test and Init
#define RAM_O_CXRINITDONE1                          0x00000070 // Cx RAM Init
                                                               // Done 1
#define RAM_O_MSXRINITDONE1                         0x00000078 // Sx RAM Init
                                                               // Done 1
#define RAM_O_MTOCRINITDONE                         0x00000088 // M to C
                                                               // Message RAM
                                                               // Init Done

//*****************************************************************************
// The following are defines for the RAM Error register offsets.
//*****************************************************************************
#define RAM_O_MCUNCWEADDR                           0x00000000 // CPU
                                                               // Uncorrectable
                                                               // Write Error
                                                               // Address
#define RAM_O_MDUNCWEADDR                           0x00000004 // uDMA
                                                               // Uncorrectable
                                                               // Write Error
                                                               // Address
#define RAM_O_MCUNCREADDR                           0x00000008 // CPU
                                                               // Uncorrectable
                                                               // Read Error
                                                               // Address
#define RAM_O_MDUNCREADDR                           0x0000000C // uDMA
                                                               // Uncorrectable
                                                               // Read Error
                                                               // Address
#define RAM_O_MCPUCREADDR                           0x00000010 // CPU Corrected
                                                               // Read Error
                                                               // Address
#define RAM_O_MDMACREADDR                           0x00000014 // uDMA
                                                               // Corrected
                                                               // Read Error
                                                               // Address
#define RAM_O_MUEFLG                                0x00000020 // Uncorrectable
                                                               // Error Flag
                                                               // Register
#define RAM_O_MUEFRC                                0x00000024 // Uncorrectable
                                                               // Error Force
#define RAM_O_MUECLR                                0x00000028 // Uncorrectable
                                                               // Error Flag
                                                               // Clear
#define RAM_O_MCECNTR                               0x0000002C // Corrected
                                                               // Error Counter
#define RAM_O_MCETRES                               0x00000030 // Corrected
                                                               // Error
                                                               // Threshold
#define RAM_O_MCEFLG                                0x00000038 // Corrected
                                                               // Error
                                                               // Threshold
                                                               // Exceeded Flag
#define RAM_O_MCEFRC                                0x0000003C // Corrected
                                                               // Error
                                                               // Threshold
                                                               // Exceeded
                                                               // Force
#define RAM_O_MCECLR                                0x00000040 // Corrected
                                                               // Error
                                                               // Threshold
                                                               // Exceeded Flag
                                                               // Clear
#define RAM_O_MCEIE                                 0x00000044 // Single Error
                                                               // Interrupt
                                                               // Enable
#define RAM_O_NMAVFLG                               0x00000050 // Non-Master
                                                               // Access
                                                               // Violation
                                                               // Flag
#define RAM_O_NMAVCLR                               0x00000058 // Non-Master
                                                               // Access
                                                               // Violation
                                                               // Flag Clear
#define RAM_O_MAVFLG                                0x00000060 // Master Access
                                                               // Violation
                                                               // Flag
#define RAM_O_MAVCLR                                0x00000068 // Master Access
                                                               // Violation
                                                               // Flag Clear
#define RAM_O_NMWRAVADDR                            0x00000070 // Non-Master
                                                               // CPU Write
                                                               // Access
                                                               // Violation
                                                               // Address
#define RAM_O_NMDMAWRAVADDR                         0x00000074 // Non-Master
                                                               // DMA Write
                                                               // Access
                                                               // Violation
                                                               // Address
#define RAM_O_NMFAVADDR                             0x00000078 // Non-Master
                                                               // CPU Fetch
                                                               // Access
                                                               // Violation
                                                               // Address
#define RAM_O_MWRAVADDR                             0x00000080 // Master CPU
                                                               // Write Access
                                                               // Violation
                                                               // Address
#define RAM_O_MDMAWRAVADDR                          0x00000084 // Master DMA
                                                               // Write Access
                                                               // Violation
                                                               // Address
#define RAM_O_MFAVADDR                              0x00000088 // Master CPU
                                                               // Fetch Access
                                                               // Violation
                                                               // Address

//*****************************************************************************
// The following are defines for the bit fields in the CXDRCR1 register.
//*****************************************************************************
#define RAM_CXDRCR1_FETCHPROTC0                     0x00000001 // Fetch
                                                               // Protection
                                                               // for M3
                                                               // Dedicated RAM
                                                               // C0
#define RAM_CXDRCR1_CPUWRPROTC0                     0x00000004 // CPU WR
                                                               // Protection
                                                               // for M3
                                                               // Dedicated RAM
                                                               // C0
#define RAM_CXDRCR1_FETCHPROTC1                     0x00000100 // Fetch
                                                               // Protection
                                                               // for M3
                                                               // Dedicated RAM
                                                               // C1
#define RAM_CXDRCR1_CPUWRPROTC1                     0x00000400 // CPU WR
                                                               // Protection
                                                               // for M3
                                                               // Dedicated RAM
                                                               // C1

//*****************************************************************************
// The following are defines for the bit fields in the CXSRCR1 register.
//*****************************************************************************
#define RAM_CXSRCR1_FETCHPROTC2                     0x00000001 // Fetch
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C2
#define RAM_CXSRCR1_DMAWRPROTC2                     0x00000002 // DMA WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C2
#define RAM_CXSRCR1_CPUWRPROTC2                     0x00000004 // CPU WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C2
#define RAM_CXSRCR1_FETCHPROTC3                     0x00000100 // Fetch
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C3
#define RAM_CXSRCR1_DMAWRPROTC3                     0x00000200 // DMA WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C3
#define RAM_CXSRCR1_CPUWRPROTC3                     0x00000400 // CPU WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C3

//*****************************************************************************
// The following are defines for the bit fields in the CXSRCR2 register.
//*****************************************************************************
#define RAM_CXSRCR2_FETCHPROTC4                     0x00000001 // Fetch
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C4
#define RAM_CXSRCR2_DMAWRPROTC4                     0x00000002 // DMA WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C4
#define RAM_CXSRCR2_CPUWRPROTC4                     0x00000004 // CPU WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C4
#define RAM_CXSRCR2_FETCHPROTC5                     0x00000100 // Fetch
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C5
#define RAM_CXSRCR2_DMAWRPROTC5                     0x00000200 // DMA WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C5
#define RAM_CXSRCR2_CPUWRPROTC5                     0x00000400 // CPU WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C5
#define RAM_CXSRCR2_FETCHPROTC6                     0x00010000 // Fetch
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C6
#define RAM_CXSRCR2_DMAWRPROTC6                     0x00020000 // DMA WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C6
#define RAM_CXSRCR2_CPUWRPROTC6                     0x00040000 // CPU WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C6
#define RAM_CXSRCR2_FETCHPROTC7                     0x01000000 // Fetch
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C7
#define RAM_CXSRCR2_DMAWRPROTC7                     0x02000000 // DMA WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C7
#define RAM_CXSRCR2_CPUWRPROTC7                     0x04000000 // CPU WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C7

//*****************************************************************************
// The following are defines for the bit fields in the CXSRCR3 register.
//*****************************************************************************
#define RAM_CXSRCR3_FETCHPROTC8                     0x00000001 // Fetch
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C8
#define RAM_CXSRCR3_DMAWRPROTC8                     0x00000002 // DMA WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C8
#define RAM_CXSRCR3_CPUWRPROTC8                     0x00000004 // CPU WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C8
#define RAM_CXSRCR3_FETCHPROTC9                     0x00000100 // Fetch
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C9
#define RAM_CXSRCR3_DMAWRPROTC9                     0x00000200 // DMA WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C9
#define RAM_CXSRCR3_CPUWRPROTC9                     0x00000400 // CPU WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C9
#define RAM_CXSRCR3_FETCHPROTC10                    0x00010000 // Fetch
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C10
#define RAM_CXSRCR3_DMAWRPROTC10                    0x00020000 // DMA WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C10
#define RAM_CXSRCR3_CPUWRPROTC10                    0x00040000 // CPU WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C10
#define RAM_CXSRCR3_FETCHPROTC11                    0x01000000 // Fetch
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C11
#define RAM_CXSRCR3_DMAWRPROTC11                    0x02000000 // DMA WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C11
#define RAM_CXSRCR3_CPUWRPROTC11                    0x04000000 // CPU WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C11

//*****************************************************************************
// The following are defines for the bit fields in the CXSRCR4 register.
//*****************************************************************************
#define RAM_CXSRCR4_FETCHPROTC12                    0x00000001 // Fetch
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C12
#define RAM_CXSRCR4_DMAWRPROTC12                    0x00000002 // DMA WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C12
#define RAM_CXSRCR4_CPUWRPROTC12                    0x00000004 // CPU WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C12
#define RAM_CXSRCR4_FETCHPROTC13                    0x00000100 // Fetch
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C13
#define RAM_CXSRCR4_DMAWRPROTC13                    0x00000200 // DMA WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C13
#define RAM_CXSRCR4_CPUWRPROTC13                    0x00000400 // CPU WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C13
#define RAM_CXSRCR4_FETCHPROTC14                    0x00010000 // Fetch
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C14
#define RAM_CXSRCR4_DMAWRPROTC14                    0x00020000 // DMA WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C14
#define RAM_CXSRCR4_CPUWRPROTC14                    0x00040000 // CPU WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C14
#define RAM_CXSRCR4_FETCHPROTC15                    0x01000000 // Fetch
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C15
#define RAM_CXSRCR4_DMAWRPROTC15                    0x02000000 // DMA WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C15
#define RAM_CXSRCR4_CPUWRPROTC15                    0x04000000 // CPU WR
                                                               // Protection
                                                               // for M3 Shared
                                                               // RAM (CPU and
                                                               // uDMA) C15
                                                               
//*****************************************************************************
// The following are defines for the bit fields in the MSXMSEL register.
//*****************************************************************************
#define RAM_MSXMSEL_S0MSEL                          0x00000001 // Master Select
                                                               // Status for
                                                               // Shared RAM S0
#define RAM_MSXMSEL_S1MSEL                          0x00000002 // Master Select
                                                               // Status for
                                                               // Shared RAM S1
#define RAM_MSXMSEL_S2MSEL                          0x00000004 // Master Select
                                                               // Status for
                                                               // Shared RAM S2
#define RAM_MSXMSEL_S3MSEL                          0x00000008 // Master Select
                                                               // Status for
                                                               // Shared RAM S3
#define RAM_MSXMSEL_S4MSEL                          0x00000010 // Master Select
                                                               // Status for
                                                               // Shared RAM S4
#define RAM_MSXMSEL_S5MSEL                          0x00000020 // Master Select
                                                               // Status for
                                                               // Shared RAM S5
#define RAM_MSXMSEL_S6MSEL                          0x00000040 // Master Select
                                                               // Status for
                                                               // Shared RAM S6
#define RAM_MSXMSEL_S7MSEL                          0x00000080 // Master Select
                                                               // Status for
                                                               // Shared RAM S7

//*****************************************************************************
// The following are defines for the bit fields in the MSXSRCR1 register.
//*****************************************************************************
#define RAM_MSXSRCR1_FETCHPROTS0                    0x00000001 // Fetch
                                                               // Protection
                                                               // for Shared
                                                               // RAM S0
#define RAM_MSXSRCR1_DMAWRPROTS0                    0x00000002 // DMA WR
                                                               // Protection
                                                               // for Shared
                                                               // RAM S0
#define RAM_MSXSRCR1_CPUWRPROTS0                    0x00000004 // CPU WR
                                                               // Protection
                                                               // for Shared
                                                               // RAM S0
#define RAM_MSXSRCR1_FETCHPROTS1                    0x00000100 // Fetch
                                                               // Protection
                                                               // for Shared
                                                               // RAM S1
#define RAM_MSXSRCR1_DMAWRPROTS1                    0x00000200 // DMA WR
                                                               // Protection
                                                               // for Shared
                                                               // RAM S1
#define RAM_MSXSRCR1_CPUWRPROTS1                    0x00000400 // CPU WR
                                                               // Protection
                                                               // for Shared
                                                               // RAM S1
#define RAM_MSXSRCR1_FETCHPROTS2                    0x00010000 // Fetch
                                                               // Protection
                                                               // for Shared
                                                               // RAM S2
#define RAM_MSXSRCR1_DMAWRPROTS2                    0x00020000 // DMA WR
                                                               // Protection
                                                               // for Shared
                                                               // RAM S2
#define RAM_MSXSRCR1_CPUWRPROTS2                    0x00040000 // CPU WR
                                                               // Protection
                                                               // for Shared
                                                               // RAM S2
#define RAM_MSXSRCR1_FETCHPROTS3                    0x01000000 // Fetch
                                                               // Protection
                                                               // for Shared
                                                               // RAM S3
#define RAM_MSXSRCR1_DMAWRPROTS3                    0x02000000 // DMA WR
                                                               // Protection
                                                               // for Shared
                                                               // RAM S3
#define RAM_MSXSRCR1_CPUWRPROTS3                    0x04000000 // CPU WR
                                                               // Protection
                                                               // for Shared
                                                               // RAM S3

//*****************************************************************************
// The following are defines for the bit fields in the MSXSRCR2 register.
//*****************************************************************************
#define RAM_MSXSRCR2_FETCHPROTS4                    0x00000001 // Fetch
                                                               // Protection
                                                               // for Shared
                                                               // RAM S4
#define RAM_MSXSRCR2_DMAWRPROTS4                    0x00000002 // DMA WR
                                                               // Protection
                                                               // for Shared
                                                               // RAM S4
#define RAM_MSXSRCR2_CPUWRPROTS4                    0x00000004 // CPU WR
                                                               // Protection
                                                               // for Shared
                                                               // RAM S4
#define RAM_MSXSRCR2_FETCHPROTS5                    0x00000100 // Fetch
                                                               // Protection
                                                               // for Shared
                                                               // RAM S5
#define RAM_MSXSRCR2_DMAWRPROTS5                    0x00000200 // DMA WR
                                                               // Protection
                                                               // for Shared
                                                               // RAM S5
#define RAM_MSXSRCR2_CPUWRPROTS5                    0x00000400 // CPU WR
                                                               // Protection
                                                               // for Shared
                                                               // RAM S5
#define RAM_MSXSRCR2_FETCHPROTS6                    0x00010000 // Fetch
                                                               // Protection
                                                               // for Shared
                                                               // RAM S6
#define RAM_MSXSRCR2_DMAWRPROTS6                    0x00020000 // DMA WR
                                                               // Protection
                                                               // for Shared
                                                               // RAM S6
#define RAM_MSXSRCR2_CPUWRPROTS6                    0x00040000 // CPU WR
                                                               // Protection
                                                               // for Shared
                                                               // RAM S6
#define RAM_MSXSRCR2_FETCHPROTS7                    0x01000000 // Fetch
                                                               // Protection
                                                               // for Shared
                                                               // RAM S7
#define RAM_MSXSRCR2_DMAWRPROTS7                    0x02000000 // DMA WR
                                                               // Protection
                                                               // for Shared
                                                               // RAM S7
#define RAM_MSXSRCR2_CPUWRPROTS7                    0x04000000 // CPU WR
                                                               // Protection
                                                               // for Shared
                                                               // RAM S7

//*****************************************************************************
// The following are defines for the bit fields in the MTOCMSGRCR register.
//*****************************************************************************
#define RAM_MTOCMSGRCR_DMAWRPROT                    0x00000002 // DMA WR
                                                               // Protect for
                                                               // Message RAM

//*****************************************************************************
// The following are defines for the bit fields in the CXRTESTINIT1 register.
//*****************************************************************************
#define RAM_CXRTESTINIT1_RAMINITC0                  0x00000001 // RAM Init
                                                               // Status for C0
                                                               // RAM
#define RAM_CXRTESTINIT1_ECCPARTESTC0               0x00000002 // RAM Test for
                                                               // C0 RAM
#define RAM_CXRTESTINIT1_RAMINITC1                  0x00000004 // RAM Init
                                                               // Status for C1
                                                               // RAM
#define RAM_CXRTESTINIT1_ECCPARTESTC1               0x00000008 // RAM Test for
                                                               // C1 RAM
#define RAM_CXRTESTINIT1_RAMINITC2                  0x00000010 // RAM Init
                                                               // Status for C2
                                                               // RAM
#define RAM_CXRTESTINIT1_ECCPARTESTC2               0x00000020 // RAM Test for
                                                               // C2 RAM
#define RAM_CXRTESTINIT1_RAMINITC3                  0x00000040 // RAM Init
                                                               // Status for C3
                                                               // RAM
#define RAM_CXRTESTINIT1_ECCPARTESTC3               0x00000080 // RAM Test for
                                                               // C3 RAM

//*****************************************************************************
// The following are defines for the bit fields in the MSXRTESTINIT1 register.
//*****************************************************************************
#define RAM_MSXRTESTINIT1_RAMINITS0                 0x00000001 // RAM Init
                                                               // Status for S0
                                                               // RAM
#define RAM_MSXRTESTINIT1_ECCPARTESTS0              0x00000002 // RAM Test for
                                                               // S0 RAM
#define RAM_MSXRTESTINIT1_RAMINITS1                 0x00000004 // RAM Init
                                                               // Status for S1
                                                               // RAM
#define RAM_MSXRTESTINIT1_ECCPARTESTS1              0x00000008 // RAM Test for
                                                               // S1 RAM
#define RAM_MSXRTESTINIT1_RAMINITS2                 0x00000010 // RAM Init
                                                               // Status for S2
                                                               // RAM
#define RAM_MSXRTESTINIT1_ECCPARTESTS2              0x00000020 // RAM Test for
                                                               // S2 RAM
#define RAM_MSXRTESTINIT1_RAMINITS3                 0x00000040 // RAM Init
                                                               // Status for S3
                                                               // RAM
#define RAM_MSXRTESTINIT1_ECCPARTESTS3              0x00000080 // RAM Test for
                                                               // S3 RAM
#define RAM_MSXRTESTINIT1_RAMINITS4                 0x00000001 // RAM Init
                                                               // Status for S4
                                                               // RAM
#define RAM_MSXRTESTINIT1_ECCPARTESTS4              0x00000002 // RAM Test for
                                                               // S4 RAM
#define RAM_MSXRTESTINIT1_RAMINITS5                 0x00000004 // RAM Init
                                                               // Status for S5
                                                               // RAM
#define RAM_MSXRTESTINIT1_ECCPARTESTS5              0x00000008 // RAM Test for
                                                               // S5 RAM
#define RAM_MSXRTESTINIT1_RAMINITS6                 0x00000010 // RAM Init
                                                               // Status for S6
                                                               // RAM
#define RAM_MSXRTESTINIT1_ECCPARTESTS6              0x00000020 // RAM Test for
                                                               // S6 RAM
#define RAM_MSXRTESTINIT1_RAMINITS7                 0x00000040 // RAM Init
                                                               // Status for S7
                                                               // RAM
#define RAM_MSXRTESTINIT1_ECCPARTESTS7              0x00000080 // RAM Test for
                                                               // S7 RAM

//*****************************************************************************
// The following are defines for the bit fields in the MTOCCRTESTINIT1
// register.
//*****************************************************************************
#define RAM_MTOCCRTESTINIT1_RAMINITMTOCMSGRAM       0x00000001  // RAM Init
                                                                // Status for M
                                                                // to C Message
                                                                // RAM
#define RAM_MTOCCRTESTINIT1_ECCPARTESTMTOCMSGRAM    0x00000002  // RAM Test for
                                                                // M to C
                                                                // Message RAM

//*****************************************************************************
// The following are defines for the bit fields in the CXRINITDONE1 register.
//*****************************************************************************
#define RAM_CXRINITDONE1_RAMINITDONEC0              0x00000001 // RAM Test
                                                               // Complete for
                                                               // C0 RAM
#define RAM_CXRINITDONE1_RAMINITDONEC1              0x00000004 // RAM Test
                                                               // Complete for
                                                               // C1 RAM
#define RAM_CXRINITDONE1_RAMINITDONEC2              0x00000010 // RAM Test
                                                               // Complete for
                                                               // C2 RAM
#define RAM_CXRINITDONE1_RAMINITDONEC3              0x00000040 // RAM Test
                                                               // Complete for
                                                               // C3 RAM

//*****************************************************************************
// The following are defines for the bit fields in the MSXRINITDONE1 register.
//*****************************************************************************
#define RAM_MSXRINITDONE1_RAMINITDONES0             0x00000001 // RAM Test
                                                               // Complete for
                                                               // S0 RAM
#define RAM_MSXRINITDONE1_RAMINITDONES1             0x00000004 // RAM Test
                                                               // Complete for
                                                               // S1 RAM
#define RAM_MSXRINITDONE1_RAMINITDONES2             0x00000010 // RAM Test
                                                               // Complete for
                                                               // S2 RAM
#define RAM_MSXRINITDONE1_RAMINITDONES3             0x00000040 // RAM Test
                                                               // Complete for
                                                               // S3 RAM
#define RAM_MSXRINITDONE1_RAMINITDONES4             0x00000100 // RAM Test
                                                               // Complete for
                                                               // S4 RAM
#define RAM_MSXRINITDONE1_RAMINITDONES5             0x00000400 // RAM Test
                                                               // Complete for
                                                               // S5 RAM
#define RAM_MSXRINITDONE1_RAMINITDONES6             0x00001000 // RAM Test
                                                               // Complete for
                                                               // S6 RAM
#define RAM_MSXRINITDONE1_RAMINITDONES7             0x00004000 // RAM Test
                                                               // Complete for
                                                               // S7 RAM

//*****************************************************************************
// The following are defines for the bit fields in the MTOCRINITDONE register.
//*****************************************************************************
#define RAM_MTOCRINITDONE_RAMINITDONEMTOCMSGRAM     0x00000001 // RAM Test
                                                               // Complete for
                                                               // M to C
                                                               // Message RAM

//*****************************************************************************
// The following are defines for the bit fields in the MUEFLG, MUEFRC, and
// MUECLR registers.
//*****************************************************************************
#define RAM_MUE_CPUWE                               0x00000001 // M3 CPU
                                                               // Uncorrectable
                                                               // Write Error
                                                               // Bit
#define RAM_MUE_UDMAWE                              0x00000002 // M3 uDMA
                                                               // Uncorrectable
                                                               // Write Error
                                                               // Bit
#define RAM_MUE_CPURE                               0x00000004 // M3 CPU
                                                               // Uncorrectable
                                                               // Read Error
                                                               // Bit
#define RAM_MUE_UDMARE                              0x00000008 // M3 uDMA
                                                               // Uncorrectbale
                                                               // Read Error
                                                               // Bit

//*****************************************************************************
// The following are defines for the bit fields in the MCECNTR register.
//*****************************************************************************
#define RAM_MCECNTR_MCECNTR_S                       0
#define RAM_MCECNTR_MCECNTR_M                       0x0000FFFF // CPU
                                                               // Corrected
                                                               // Error Counter

//*****************************************************************************
// The following are defines for the bit fields in the MCETRES register.
//*****************************************************************************
#define RAM_MCETRES_MCETRES_S                       0
#define RAM_MCETRES_MCETRES_M                       0x0000FFFF // CPU
                                                               // Corrected
                                                               // Error
                                                               // Threshold

//*****************************************************************************
// The following are defines for the bit fields in the MCEFLG, MCEFRC, and
// MCECLR registers.
//*****************************************************************************
#define RAM_MCE_BIT                                 0x00000001 // CPU
                                                               // Corrected
                                                               // Error Count
                                                               // Exceeded Bit

//*****************************************************************************
// The following are defines for the bit fields in the MCEIE register.
//*****************************************************************************
#define RAM_MCEIE_MCEIE                             0x00000001 // CPU
                                                               // Corrected
                                                               // Error
                                                               // Interrupt
                                                               // Enable

//*****************************************************************************
// The following are defines for the bit fields in the NMAVFLG and NMAVCLR
// registers.
//*****************************************************************************
#define RAM_NMAVFLG_CPUFETCH                        0x00000001 // Non-master
                                                               // CPU Fetch
                                                               // Access
                                                               // Violation Bit
#define RAM_NMAVFLG_DMAWRITE                        0x00000002 // Non-master
                                                               // DMA Write
                                                               // Access
                                                               // Violation Bit
#define RAM_NMAVFLG_CPUWRITE                        0x00000004 // Non-master
                                                               // CPU Write
                                                               // Access
                                                               // Violation Bit

//*****************************************************************************
// The following are defines for the bit fields in the MAVFLG and MAVCLR
// registers.
//*****************************************************************************
#define RAM_MAVFLG_CPUFETCH                         0x00000001 // Master CPU
                                                               // Fetch Access
                                                               // Violation Bit
#define RAM_MAVFLG_DMAWRITE                         0x00000002 // Master DMA
                                                               // Write Access
                                                               // Violation Bit
#define RAM_MAVFLG_CPUWRITE                         0x00000004 // Master CPU
                                                               // Write Access
                                                               // Violation Bit



#endif // __HW_RAM_H__









