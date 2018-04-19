//###########################################################################
// FILE:   hw_udma.h
// TITLE:  Macros for use in accessing the UDMA registers.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_UDMA_H__
#define __HW_UDMA_H__

//*****************************************************************************
// The following are defines for the Micro Direct Memory Access register
// addresses.
//*****************************************************************************
#define UDMA_STAT                       0x400FF000 // DMA Status
#define UDMA_CFG                        0x400FF004 // DMA Configuration
#define UDMA_CTLBASE                    0x400FF008 // DMA Channel Control Base
                                                   // Pointer
#define UDMA_ALTBASE                    0x400FF00C // DMA Alternate Channel
                                                   // Control Base Pointer
#define UDMA_WAITSTAT                   0x400FF010 // DMA Channel
                                                   // Wait-on-Request Status
#define UDMA_SWREQ                      0x400FF014 // DMA Channel Software
                                                   // Request
#define UDMA_USEBURSTSET                0x400FF018 // DMA Channel Useburst Set
#define UDMA_USEBURSTCLR                0x400FF01C // DMA Channel Useburst
                                                   // Clear
#define UDMA_REQMASKSET                 0x400FF020 // DMA Channel Request Mask
                                                   // Set
#define UDMA_REQMASKCLR                 0x400FF024 // DMA Channel Request Mask
                                                   // Clear
#define UDMA_ENASET                     0x400FF028 // DMA Channel Enable Set
#define UDMA_ENACLR                     0x400FF02C // DMA Channel Enable Clear
#define UDMA_ALTSET                     0x400FF030 // DMA Channel Primary
                                                   // Alternate Set
#define UDMA_ALTCLR                     0x400FF034 // DMA Channel Primary
                                                   // Alternate Clear
#define UDMA_PRIOSET                    0x400FF038 // DMA Channel Priority Set
#define UDMA_PRIOCLR                    0x400FF03C // DMA Channel Priority
                                                   // Clear
#define UDMA_ERRCLR                     0x400FF04C // DMA Bus Error Clear
#define UDMA_CHALTL                     0x400FF500 // DMA Channel Alternate
                                                   // Select (Legacy)
#define UDMA_CHMAP0                     0x400FF510 // DMA Channel Map Select 0
#define UDMA_CHMAP1                     0x400FF514 // DMA Channel Map Select 1
#define UDMA_CHMAP2                     0x400FF518 // DMA Channel Map Select 2
#define UDMA_CHMAP3                     0x400FF51C // DMA Channel Map Select 3

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_STAT register.
//*****************************************************************************
#define UDMA_STAT_DMACHANS_M            0x001F0000 // Available uDMA Channels
                                                   // Minus 1
#define UDMA_STAT_STATE_M               0x000000F0 // Control State Machine
                                                   // Status
#define UDMA_STAT_STATE_IDLE            0x00000000 // Idle
#define UDMA_STAT_STATE_RD_CTRL         0x00000010 // Reading channel
                                                   // controller data
#define UDMA_STAT_STATE_RD_SRCENDP      0x00000020 // Reading source end
                                                   // pointer
#define UDMA_STAT_STATE_RD_DSTENDP      0x00000030 // Reading destination end
                                                   // pointer
#define UDMA_STAT_STATE_RD_SRCDAT       0x00000040 // Reading source data
#define UDMA_STAT_STATE_WR_DSTDAT       0x00000050 // Writing destination data
#define UDMA_STAT_STATE_WAIT            0x00000060 // Waiting for uDMA request
                                                   // to clear
#define UDMA_STAT_STATE_WR_CTRL         0x00000070 // Writing channel
                                                   // controller data
#define UDMA_STAT_STATE_STALL           0x00000080 // Stalled
#define UDMA_STAT_STATE_DONE            0x00000090 // Done
#define UDMA_STAT_STATE_UNDEF           0x000000A0 // Undefined
#define UDMA_STAT_MASTEN                0x00000001 // Master Enable Status
#define UDMA_STAT_DMACHANS_S            16

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_CFG register.
//*****************************************************************************
#define UDMA_CFG_MASTEN                 0x00000001 // Controller Master Enable

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_CTLBASE register.
//*****************************************************************************
#define UDMA_CTLBASE_ADDR_M             0xFFFFFC00 // Channel Control Base
                                                   // Address
#define UDMA_CTLBASE_ADDR_S             10

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_ALTBASE register.
//*****************************************************************************
#define UDMA_ALTBASE_ADDR_M             0xFFFFFFFF // Alternate Channel Address
                                                   // Pointer
#define UDMA_ALTBASE_ADDR_S             0

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_WAITSTAT register.
//*****************************************************************************
#define UDMA_WAITSTAT_WAITREQ_M         0xFFFFFFFF // Channel [n] Wait Status

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_SWREQ register.
//*****************************************************************************
#define UDMA_SWREQ_M                    0xFFFFFFFF // Channel [n] Software
                                                   // Request

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_USEBURSTSET
// register.
//*****************************************************************************
#define UDMA_USEBURSTSET_SET_M          0xFFFFFFFF // Channel [n] Useburst Set

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_USEBURSTCLR
// register.
//*****************************************************************************
#define UDMA_USEBURSTCLR_CLR_M          0xFFFFFFFF // Channel [n] Useburst
                                                   // Clear

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_REQMASKSET
// register.
//*****************************************************************************
#define UDMA_REQMASKSET_SET_M           0xFFFFFFFF // Channel [n] Request Mask
                                                   // Set

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_REQMASKCLR
// register.
//*****************************************************************************
#define UDMA_REQMASKCLR_CLR_M           0xFFFFFFFF // Channel [n] Request Mask
                                                   // Clear

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_ENASET register.
//*****************************************************************************
#define UDMA_ENASET_SET_M               0xFFFFFFFF // Channel [n] Enable Set

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_ENACLR register.
//*****************************************************************************
#define UDMA_ENACLR_CLR_M               0xFFFFFFFF // Clear Channel [n] Enable
                                                   // Clear

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_ALTSET register.
//*****************************************************************************
#define UDMA_ALTSET_SET_M               0xFFFFFFFF // Channel [n] Alternate Set

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_ALTCLR register.
//*****************************************************************************
#define UDMA_ALTCLR_CLR_M               0xFFFFFFFF // Channel [n] Alternate
                                                   // Clear

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_PRIOSET register.
//*****************************************************************************
#define UDMA_PRIOSET_SET_M              0xFFFFFFFF // Channel [n] Priority Set

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_PRIOCLR register.
//*****************************************************************************
#define UDMA_PRIOCLR_CLR_M              0xFFFFFFFF // Channel [n] Priority
                                                   // Clear

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_ERRCLR register.
//*****************************************************************************
#define UDMA_ERRCLR_ERRCLR              0x00000001 // uDMA Bus Error Status

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_CHMAPL register.
//*****************************************************************************
#define UDMA_CHALTL_M                   0xFFFFFFFF // Channel [n] Alternate
                                                   // Assignment Select (Legacy)

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_CHMAPx register.
//*****************************************************************************
#define UDMA_CHMAPx_ASSIGN1_M1          0x1 // DMA First Channel Assignment
#define UDMA_CHMAPx_ASSIGN2_M2          0x2 // DMA Second Channel Assignment
#define UDMA_CHMAPx_ASSIGN3_M3          0x3 // DMA Third Channel Assignment

#define UDMA_CHMAP0_CHAN0_S             0
#define UDMA_CHMAP0_CHAN0_M             0x0000000F // DMA Channel 0 Assignment
#define UDMA_CHMAP0_CHAN1_S             4
#define UDMA_CHMAP0_CHAN1_M             0x000000F0 // DMA Channel 1 Assignment
#define UDMA_CHMAP0_CHAN2_S             8
#define UDMA_CHMAP0_CHAN2_M             0x00000F00 // DMA Channel 2 Assignment
#define UDMA_CHMAP0_CHAN3_S             12
#define UDMA_CHMAP0_CHAN3_M             0x0000F000 // DMA Channel 3 Assignment
#define UDMA_CHMAP0_CHAN4_S             16
#define UDMA_CHMAP0_CHAN4_M             0x000F0000 // DMA Channel 4 Assignment
#define UDMA_CHMAP0_CHAN5_S             20
#define UDMA_CHMAP0_CHAN5_M             0x00F00000 // DMA Channel 5 Assignment
#define UDMA_CHMAP0_CHAN6_S             24
#define UDMA_CHMAP0_CHAN6_M             0x0F000000 // DMA Channel 6 Assignment
#define UDMA_CHMAP0_CHAN7_S             28
#define UDMA_CHMAP0_CHAN7_M             0xF0000000 // DMA Channel 7 Assignment

#define UDMA_CHMAP1_CHAN8_S             0
#define UDMA_CHMAP1_CHAN8_M             0x0000000F // DMA Channel 8 Assignment
#define UDMA_CHMAP1_CHAN9_S             4
#define UDMA_CHMAP1_CHAN9_M             0x000000F0 // DMA Channel 9 Assignment
#define UDMA_CHMAP1_CHAN10_S            8
#define UDMA_CHMAP1_CHAN10_M            0x00000F00 // DMA Channel 10 Assignment
#define UDMA_CHMAP1_CHAN11_S            12
#define UDMA_CHMAP1_CHAN11_M            0x0000F000 // DMA Channel 11 Assignment
#define UDMA_CHMAP1_CHAN12_S            16
#define UDMA_CHMAP1_CHAN12_M            0x000F0000 // DMA Channel 12 Assignment
#define UDMA_CHMAP1_CHAN13_S            20
#define UDMA_CHMAP1_CHAN13_M            0x00F00000 // DMA Channel 13 Assignment
#define UDMA_CHMAP1_CHAN14_S            24
#define UDMA_CHMAP1_CHAN14_M            0x0F000000 // DMA Channel 14 Assignment
#define UDMA_CHMAP1_CHAN15_S            28
#define UDMA_CHMAP1_CHAN15_M            0xF0000000 // DMA Channel 15 Assignment

#define UDMA_CHMAP2_CHAN16_S            0
#define UDMA_CHMAP2_CHAN16_M            0x0000000F // DMA Channel 16 Assignment
#define UDMA_CHMAP2_CHAN17_S            4
#define UDMA_CHMAP2_CHAN17_M            0x000000F0 // DMA Channel 17 Assignment
#define UDMA_CHMAP2_CHAN18_S            8
#define UDMA_CHMAP2_CHAN18_M            0x00000F00 // DMA Channel 18 Assignment
#define UDMA_CHMAP2_CHAN19_S            12
#define UDMA_CHMAP2_CHAN19_M            0x0000F000 // DMA Channel 19 Assignment
#define UDMA_CHMAP2_CHAN20_S            16
#define UDMA_CHMAP2_CHAN20_M            0x000F0000 // DMA Channel 20 Assignment
#define UDMA_CHMAP2_CHAN21_S            20
#define UDMA_CHMAP2_CHAN21_M            0x00F00000 // DMA Channel 21 Assignment
#define UDMA_CHMAP2_CHAN22_S            24
#define UDMA_CHMAP2_CHAN22_M            0x0F000000 // DMA Channel 22 Assignment
#define UDMA_CHMAP2_CHAN23_S            28
#define UDMA_CHMAP2_CHAN23_M            0xF0000000 // DMA Channel 23 Assignment

#define UDMA_CHMAP3_CHAN24_S            0
#define UDMA_CHMAP3_CHAN24_M            0x0000000F // DMA Channel 24 Assignment
#define UDMA_CHMAP3_CHAN25_S            4
#define UDMA_CHMAP3_CHAN25_M            0x000000F0 // DMA Channel 25 Assignment
#define UDMA_CHMAP3_CHAN26_S            8
#define UDMA_CHMAP3_CHAN26_M            0x00000F00 // DMA Channel 26 Assignment
#define UDMA_CHMAP3_CHAN27_S            12
#define UDMA_CHMAP3_CHAN27_M            0x0000F000 // DMA Channel 27 Assignment
#define UDMA_CHMAP3_CHAN28_S            16
#define UDMA_CHMAP3_CHAN28_M            0x000F0000 // DMA Channel 28 Assignment
#define UDMA_CHMAP3_CHAN29_S            20
#define UDMA_CHMAP3_CHAN29_M            0x00F00000 // DMA Channel 29 Assignment
#define UDMA_CHMAP3_CHAN30_S            24
#define UDMA_CHMAP3_CHAN30_M            0x0F000000 // DMA Channel 30 Assignment
#define UDMA_CHMAP3_CHAN31_S            28
#define UDMA_CHMAP3_CHAN31_M            0xF0000000 // DMA Channel 31 Assignment

//*****************************************************************************
// The following are defines for the Micro Direct Memory Access (uDMA) offsets.
//*****************************************************************************
#define UDMA_O_SRCENDP                  0x00000000 // DMA Channel Source
                                                   // Address End Pointer
#define UDMA_O_DSTENDP                  0x00000004 // DMA Channel Destination
                                                   // Address End Pointer
#define UDMA_O_CHCTL                    0x00000008 // DMA Channel Control Word

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_O_SRCENDP register.
//*****************************************************************************
#define UDMA_SRCENDP_ADDR_M             0xFFFFFFFF // Source Address End
                                                   // Pointer
#define UDMA_SRCENDP_ADDR_S             0

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_O_DSTENDP register.
//*****************************************************************************
#define UDMA_DSTENDP_ADDR_M             0xFFFFFFFF // Destination Address End
                                                   // Pointer
#define UDMA_DSTENDP_ADDR_S             0

//*****************************************************************************
// The following are defines for the bit fields in the UDMA_O_CHCTL register.
//*****************************************************************************
#define UDMA_CHCTL_DSTINC_M             0xC0000000 // Destination Address
                                                   // Increment
#define UDMA_CHCTL_DSTINC_8             0x00000000 // Byte
#define UDMA_CHCTL_DSTINC_16            0x40000000 // Half-word
#define UDMA_CHCTL_DSTINC_32            0x80000000 // Word
#define UDMA_CHCTL_DSTINC_NONE          0xC0000000 // No increment
#define UDMA_CHCTL_DSTSIZE_M            0x30000000 // Destination Data Size
#define UDMA_CHCTL_DSTSIZE_8            0x00000000 // Byte
#define UDMA_CHCTL_DSTSIZE_16           0x10000000 // Half-word
#define UDMA_CHCTL_DSTSIZE_32           0x20000000 // Word
#define UDMA_CHCTL_SRCINC_M             0x0C000000 // Source Address Increment
#define UDMA_CHCTL_SRCINC_8             0x00000000 // Byte
#define UDMA_CHCTL_SRCINC_16            0x04000000 // Half-word
#define UDMA_CHCTL_SRCINC_32            0x08000000 // Word
#define UDMA_CHCTL_SRCINC_NONE          0x0C000000 // No increment
#define UDMA_CHCTL_SRCSIZE_M            0x03000000 // Source Data Size
#define UDMA_CHCTL_SRCSIZE_8            0x00000000 // Byte
#define UDMA_CHCTL_SRCSIZE_16           0x01000000 // Half-word
#define UDMA_CHCTL_SRCSIZE_32           0x02000000 // Word
#define UDMA_CHCTL_ARBSIZE_M            0x0003C000 // Arbitration Size
#define UDMA_CHCTL_ARBSIZE_1            0x00000000 // 1 Transfer
#define UDMA_CHCTL_ARBSIZE_2            0x00004000 // 2 Transfers
#define UDMA_CHCTL_ARBSIZE_4            0x00008000 // 4 Transfers
#define UDMA_CHCTL_ARBSIZE_8            0x0000C000 // 8 Transfers
#define UDMA_CHCTL_ARBSIZE_16           0x00010000 // 16 Transfers
#define UDMA_CHCTL_ARBSIZE_32           0x00014000 // 32 Transfers
#define UDMA_CHCTL_ARBSIZE_64           0x00018000 // 64 Transfers
#define UDMA_CHCTL_ARBSIZE_128          0x0001C000 // 128 Transfers
#define UDMA_CHCTL_ARBSIZE_256          0x00020000 // 256 Transfers
#define UDMA_CHCTL_ARBSIZE_512          0x00024000 // 512 Transfers
#define UDMA_CHCTL_ARBSIZE_1024         0x00028000 // 1024 Transfers
#define UDMA_CHCTL_XFERSIZE_M           0x00003FF0 // Transfer Size (minus 1)
#define UDMA_CHCTL_NXTUSEBURST          0x00000008 // Next Useburst
#define UDMA_CHCTL_XFERMODE_M           0x00000007 // uDMA Transfer Mode
#define UDMA_CHCTL_XFERMODE_STOP        0x00000000 // Stop
#define UDMA_CHCTL_XFERMODE_BASIC       0x00000001 // Basic
#define UDMA_CHCTL_XFERMODE_AUTO        0x00000002 // Auto-Request
#define UDMA_CHCTL_XFERMODE_PINGPONG    0x00000003  // Ping-Pong
#define UDMA_CHCTL_XFERMODE_MEM_SG      0x00000004 // Memory Scatter-Gather
#define UDMA_CHCTL_XFERMODE_MEM_SGA     0x00000005 // Alternate Memory
                                                   // Scatter-Gather
#define UDMA_CHCTL_XFERMODE_PER_SG      0x00000006 // Peripheral Scatter-Gather
#define UDMA_CHCTL_XFERMODE_PER_SGA     0x00000007 // Alternate Peripheral
                                                   // Scatter-Gather
#define UDMA_CHCTL_XFERSIZE_S           4


#endif // __HW_UDMA_H__



