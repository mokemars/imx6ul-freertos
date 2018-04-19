//###########################################################################
// FILE:   hw_can.h
// TITLE:  Defines and macros used when accessing the CAN controllers.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_CAN_H__
#define __HW_CAN_H__

//*****************************************************************************
// The following are defines for the CAN register offsets.
//*****************************************************************************
#define CAN_O_CTL                    0x00000000 // CAN Control
#define CAN_O_ES                     0x00000004 // CAN Status
#define CAN_O_ERRC                   0x00000008 // CAN Error Counter
#define CAN_O_BTR                    0x0000000C // CAN Bit Timing
#define CAN_O_INT                    0x00000010 // CAN Interrupt
#define CAN_O_TEST                   0x00000014 // CAN Test
#define CAN_O_PERR                   0x0000001C // CAN Parity Error
#define CAN_O_ABOTR                  0x00000080 // CAN Auto-Bus On Time
#define CAN_O_TXRQ                   0x00000088 // CAN Transmission Request
#define CAN_O_NWDAT                  0x0000009C // CAN New Data
#define CAN_O_INTPND                 0x000000B0 // CAN Interrupt Pending
#define CAN_O_MSGVAL                 0x000000C4 // CAN Valid
#define CAN_O_INTMUX                 0x000000D8 // CAN Interrupt Mux
#define CAN_O_IF1_CMD                0x00000100 // CAN Interface 1 Command
#define CAN_O_IF1_MSK                0x00000104 // CAN Interface 1 Mask
#define CAN_O_IF1_ARB                0x00000108 // CAN Interface 1 Arbitration
#define CAN_O_IF1_MCTL               0x0000010C // CAN Interface 1 Message
                                                // Control
#define CAN_O_IF1_DATA               0x00000110 // CAN Interface 1 DATA A2_A1
#define CAN_O_IF1_DATB               0x00000114 // CAN Interface 1 DATA B2_B1
#define CAN_O_IF2_CMD                0x00000120 // CAN Interface 2 Command
#define CAN_O_IF2_MSK                0x00000124 // CAN Interface 2 Mask
#define CAN_O_IF2_ARB                0x00000128 // CAN Interface 2 Arbitration
#define CAN_O_IF2_MCTL               0x0000012C // CAN Interface 2 Message
                                                // Control
#define CAN_O_IF2_DATA               0x00000130 // CAN Interface 2 DATA A2_A1
#define CAN_O_IF2_DATB               0x00000134 // CAN Interface 2 DATA B2_B1
#define CAN_O_IF3_OBS                0x00000140 // CAN Interface 3 Command
#define CAN_O_IF3_MSK                0x00000144 // CAN Interface 3 Mask
#define CAN_O_IF3_ARB                0x00000148 // CAN Interface 3 Arbitration
#define CAN_O_IF3_MCTL               0x0000014C // CAN Interface 3 Message
                                                // Control
#define CAN_O_IF3_DATA               0x00000150 // CAN Interface 3 DATA A2_A1
#define CAN_O_IF3_DATB               0x00000154 // CAN Interface 3 DATA B2_B1
#define CAN_O_IF3_UPD                0x00000160 // CAN Interface 3 Update
                                                // Enable

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_CTL register.
//*****************************************************************************
#define CAN_CTL_INIT                 0x00000001 // Initialization
#define CAN_CTL_IE0                  0x00000002 // Interrupt Enable 0
#define CAN_CTL_SIE                  0x00000004 // Status Interrupt Enable
#define CAN_CTL_EIE                  0x00000008 // Error Interrupt Enable
#define CAN_CTL_DAR                  0x00000020 // Disable
                                                // Automatic-Retransmission
#define CAN_CTL_CCE                  0x00000040 // Configuration Change Enable
#define CAN_CTL_TEST                 0x00000080 // Test Mode Enable
#define CAN_CTL_IDS                  0x00000100 // Interruption Debug Support
                                                // Enable
#define CAN_CTL_ABO                  0x00000200 // Auto-Bus On Enable
#define CAN_CTL_PMD_S                10
#define CAN_CTL_PMD_M                0x00003C00 // Parity/SECDED Enable
#define CAN_CTL_SWR                  0x00008000 // Software Reset Enable
#define CAN_CTL_INITDBG              0x00010000 // Debug Mode Status
#define CAN_CTL_IE1                  0x00020000 // Interrupt Enable 1
#define CAN_CTL_DE1                  0x00040000 // Enable DMA Request For IF1
#define CAN_CTL_DE2                  0x00080000 // Enable DMA Request For IF2
#define CAN_CTL_DE3                  0x00100000 // Enable DMA Request For IF3

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_ES register.
//*****************************************************************************
#define CAN_ES_LEC_S                 0
#define CAN_ES_LEC_M                 0x00000007 // Last Error Code
#define CAN_ES_TXOK                  0x00000008 // Transmission Status
#define CAN_ES_RXOK                  0x00000010 // Reception Status
#define CAN_ES_EPASS                 0x00000020 // Error Passive State
#define CAN_ES_EWARN                 0x00000040 // Warning State
#define CAN_ES_BOFF                  0x00000080 // Bus-Off Status
#define CAN_ES_PER                   0x00000100 // Parity Error
#define CAN_ES_WAKEUP_PND            0x00000200 // Wakeup Pending

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_ERRC register.
//*****************************************************************************
#define CAN_ERRC_TEC_S               0
#define CAN_ERRC_TEC_M               0x000000FF // Transmit Error Counter
#define CAN_ERRC_REC_S               8
#define CAN_ERRC_REC_M               0x00007F00 // Receive Error Counter
#define CAN_ERRC_RP                  0x00008000 // Received Error Passive

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_BTR register.
//*****************************************************************************
#define CAN_BTR_BRP_S                0
#define CAN_BTR_BRP_M                0x0000003F // Baud Rate Prescaler
#define CAN_BTR_SJW_S                6
#define CAN_BTR_SJW_M                0x000000C0 // (Re)Synchronization Jump
                                                // Width
#define CAN_BTR_TSEG1_S              8
#define CAN_BTR_TSEG1_M              0x00000F00 // Time Segment Before Sample
                                                // Point
#define CAN_BTR_TSEG2_S              12
#define CAN_BTR_TSEG2_M              0x00007000 // Time Segment after Sample
                                                // Point
#define CAN_BTR_BRPE_S               16
#define CAN_BTR_BRPE_M               0x000F0000 // Baud Rate Prescaler
                                                // Extension

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_INT register.
//*****************************************************************************
#define CAN_INT_INT0ID_S             0
#define CAN_INT_INT0ID_M             0x0000FFFF // Interrupt Identifier 0
#define CAN_INT_INT0ID_STATUS        0x00008000 // Status Interrupt
#define CAN_INT_INT1ID_S             16
#define CAN_INT_INT1ID_M             0x00FF0000 // Interrupt Identifier 1

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_TEST register.
//*****************************************************************************
#define CAN_TEST_SILENT              0x00000008 // Silent Mode
#define CAN_TEST_LBACK               0x00000010 // Loopback Mode
#define CAN_TEST_TX_S                5
#define CAN_TEST_TX_M                0x00000060 // CAN TX Pin Control
#define CAN_TEST_RX                  0x00000080 // CAN RX Pin Status
#define CAN_TEST_EXL                 0x00000100 // External Loopback Mode

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_PERR register.
//*****************************************************************************
#define CAN_PERR_MSGNUM_S            0
#define CAN_PERR_MSGNUM_M            0x0000003F // Message Number
#define CAN_PERR_WORDNUM_S           6
#define CAN_PERR_WORDNUM_M           0x0000007C // Word Number

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF1_CMD register.
//*****************************************************************************
#define CAN_IF1_CMD_MSGNUM_S         0
#define CAN_IF1_CMD_MSGNUM_M         0x000000FF // Message Number
#define CAN_IF1_CMD_DMAACTIVE        0x00004000 // DMA Active Flag
#define CAN_IF1_CMD_BUSY             0x00008000 // Busy Flag
#define CAN_IF1_CMD_DATAB            0x00010000 // Access Data B
#define CAN_IF1_CMD_DATAA            0x00020000 // Access Data A
#define CAN_IF1_CMD_TXRQSTNDAT       0x00040000 // Transmission Request Bit
#define CAN_IF1_CMD_CLRINTPND        0x00080000 // Clear Interrupt Pending Bit
#define CAN_IF1_CMD_CONTROL          0x00100000 // Access Control Bits
#define CAN_IF1_CMD_ARB              0x00200000 // Access Arbitration Bits
#define CAN_IF1_CMD_MASK             0x00400000 // Access Mask Bits
#define CAN_IF1_CMD_WR_RD            0x00800000 // Write and Read

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF1_MSK register.
//*****************************************************************************
#define CAN_IF1_MSK_MASK_S           0
#define CAN_IF1_MSK_MASK_M           0x1FFFFFFF // Mask
#define CAN_IF1_MSK_MDIR             0x40000000 // Mask Message Direction
#define CAN_IF1_MSK_MXTD             0x80000000 // Mask Extended Identifier

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF1_ARB register.
//*****************************************************************************
#define CAN_IF1_ARB_XTD_ID_S         0
#define CAN_IF1_ARB_XTD_ID_M         0x1FFFFFFF // Extended Message Identifier
#define CAN_IF1_ARB_STD_ID_S         18 
#define CAN_IF1_ARB_STD_ID_M         0x1FFC0000 // Standard Message Identifier
#define CAN_IF1_ARB_DIR              0x20000000 // Message Direction
#define CAN_IF1_ARB_XTD              0x40000000 // Extended Identifier
#define CAN_IF1_ARB_MSGVAL           0x80000000 // Message Valid

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF1_MCTL
// register.
//*****************************************************************************
#define CAN_IF1_MCTL_DLC_S           0
#define CAN_IF1_MCTL_DLC_M           0x0000000F // Data Length Code
#define CAN_IF1_MCTL_EOB             0x00000080 // End of Block
#define CAN_IF1_MCTL_TXRQST          0x00000100 // Transmit Request
#define CAN_IF1_MCTL_RMTEN           0x00000200 // Remote Enable
#define CAN_IF1_MCTL_RXIE            0x00000400 // Receive Interrupt Enable
#define CAN_IF1_MCTL_TXIE            0x00000800 // Transmit Interrupt Enable
#define CAN_IF1_MCTL_UMASK           0x00001000 // Use Acceptance Mask
#define CAN_IF1_MCTL_INTPND          0x00002000 // Interrupt Pending
#define CAN_IF1_MCTL_MSGLST          0x00004000 // Message Lost
#define CAN_IF1_MCTL_NEWDAT          0x00008000 // New Data

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF1_DATA
// register.
//*****************************************************************************
#define CAN_IF1_DATA_DATA0_S         0
#define CAN_IF1_DATA_DATA0_M         0x000000FF // Data Byte 0
#define CAN_IF1_DATA_DATA1_S         8
#define CAN_IF1_DATA_DATA1_M         0x0000FF00 // Data Byte 1
#define CAN_IF1_DATA_DATA2_S         16
#define CAN_IF1_DATA_DATA2_M         0x00FF0000 // Data Byte 2
#define CAN_IF1_DATA_DATA3_S         24
#define CAN_IF1_DATA_DATA3_M         0xFF000000 // Data Byte 3

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF1_DATB
// register.
//*****************************************************************************
#define CAN_IF1_DATB_DATA4_S         0
#define CAN_IF1_DATB_DATA4_M         0x000000FF // Data Byte 4
#define CAN_IF1_DATB_DATA5_S         8
#define CAN_IF1_DATB_DATA5_M         0x0000FF00 // Data Byte 5
#define CAN_IF1_DATB_DATA6_S         16
#define CAN_IF1_DATB_DATA6_M         0x00FF0000 // Data Byte 6
#define CAN_IF1_DATB_DATA7_S         24
#define CAN_IF1_DATB_DATA7_M         0xFF000000 // Data Byte 7

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF2_CMD register.
//*****************************************************************************
#define CAN_IF2_CMD_MSGNUM_S         0
#define CAN_IF2_CMD_MSGNUM_M         0x000000FF // Message Number
#define CAN_IF2_CMD_DMAACTIVE        0x00004000 // DMA Active Flag
#define CAN_IF2_CMD_BUSY             0x00008000 // Busy Flag
#define CAN_IF2_CMD_DATAB            0x00010000 // Access Data B
#define CAN_IF2_CMD_DATAA            0x00020000 // Access Data A
#define CAN_IF2_CMD_TXRQSTNDAT       0x00040000 // Transmission Request Bit
#define CAN_IF2_CMD_CLRINTPND        0x00080000 // Clear Interrupt Pending Bit
#define CAN_IF2_CMD_CONTROL          0x00100000 // Access Control Bits
#define CAN_IF2_CMD_ARB              0x00200000 // Access Arbitration Bits
#define CAN_IF2_CMD_MASK             0x00400000 // Access Mask Bits
#define CAN_IF2_CMD_WR_RD            0x00800000 // Write and Read

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF2_MSK register.
//*****************************************************************************
#define CAN_IF2_MSK_MASK_S           0
#define CAN_IF2_MSK_MASK_M           0x1FFFFFFF // Mask
#define CAN_IF2_MSK_MDIR             0x40000000 // Mask Message Direction
#define CAN_IF2_MSK_MXTD             0x80000000 // Mask Extended Identifier

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF2_ARB register.
//*****************************************************************************
#define CAN_IF2_ARB_XTD_ID_S         0
#define CAN_IF2_ARB_XTD_ID_M         0x1FFFFFFF // Extended Message Identifier
#define CAN_IF2_ARB_STD_ID_S         18 
#define CAN_IF2_ARB_STD_ID_M         0x1FFC0000 // Standard Message Identifier
#define CAN_IF2_ARB_DIR              0x20000000 // Message Direction
#define CAN_IF2_ARB_XTD              0x40000000 // Extended Identifier
#define CAN_IF2_ARB_MSGVAL           0x80000000 // Message Valid

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF2_MCTL
// register.
//*****************************************************************************
#define CAN_IF2_MCTL_DLC_S           0
#define CAN_IF2_MCTL_DLC_M           0x0000000F // Data Length Code
#define CAN_IF2_MCTL_EOB             0x00000080 // End of Block
#define CAN_IF2_MCTL_TXRQST          0x00000100 // Transmit Request
#define CAN_IF2_MCTL_RMTEN           0x00000200 // Remote Enable
#define CAN_IF2_MCTL_RXIE            0x00000400 // Receive Interrupt Enable
#define CAN_IF2_MCTL_TXIE            0x00000800 // Transmit Interrupt Enable
#define CAN_IF2_MCTL_UMASK           0x00001000 // Use Acceptance Mask
#define CAN_IF2_MCTL_INTPND          0x00002000 // Interrupt Pending
#define CAN_IF2_MCTL_MSGLST          0x00004000 // Message Lost
#define CAN_IF2_MCTL_NEWDAT          0x00008000 // New Data

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF2_DATA
// register.
//*****************************************************************************
#define CAN_IF2_DATA_DATA0_S         0
#define CAN_IF2_DATA_DATA0_M         0x000000FF // Data Byte 0
#define CAN_IF2_DATA_DATA1_S         8
#define CAN_IF2_DATA_DATA1_M         0x0000FF00 // Data Byte 1
#define CAN_IF2_DATA_DATA2_S         16
#define CAN_IF2_DATA_DATA2_M         0x00FF0000 // Data Byte 2
#define CAN_IF2_DATA_DATA3_S         24
#define CAN_IF2_DATA_DATA3_M         0xFF000000 // Data Byte 3

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF2_DATB
// register.
//*****************************************************************************
#define CAN_IF2_DATB_DATA4_S         0
#define CAN_IF2_DATB_DATA4_M         0x000000FF // Data Byte 4
#define CAN_IF2_DATB_DATA5_S         8
#define CAN_IF2_DATB_DATA5_M         0x0000FF00 // Data Byte 5
#define CAN_IF2_DATB_DATA6_S         16
#define CAN_IF2_DATB_DATA6_M         0x00FF0000 // Data Byte 6
#define CAN_IF2_DATB_DATA7_S         24
#define CAN_IF2_DATB_DATA7_M         0xFF000000 // Data Byte 7

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF3_OBS register.
//*****************************************************************************
#define CAN_IF3_OBS_MASK             0x00000001 // Mask Data Read
#define CAN_IF3_OBS_ARB              0x00000002 // Arbitration Data Read
#define CAN_IF3_OBS_CTRL             0x00000004 // Control Read
#define CAN_IF3_OBS_DATAA            0x00000008 // Data A Read
#define CAN_IF3_OBS_DATAB            0x00000010 // Data B Read

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF3_MSK register.
//*****************************************************************************
#define CAN_IF3_MSK_MASK_S           0
#define CAN_IF3_MSK_MASK_M           0x1FFFFFFF // Mask
#define CAN_IF3_MSK_MDIR             0x40000000 // Mask Message Direction
#define CAN_IF3_MSK_MXTD             0x80000000 // Mask Extended Identifier

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF3_ARB register.
//*****************************************************************************
#define CAN_IF3_ARB_ID_S             0
#define CAN_IF3_ARB_ID_M             0x1FFFFFFF // Message Identifier
#define CAN_IF3_ARB_DIR              0x20000000 // Message Direction
#define CAN_IF3_ARB_XTD              0x40000000 // Extended Identifier
#define CAN_IF3_ARB_MSGVAL           0x80000000 // Message Valid

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF3_MCTL
// register.
//*****************************************************************************
#define CAN_IF3_MCTL_DLC_S           0
#define CAN_IF3_MCTL_DLC_M           0x0000000F // Data Length Code
#define CAN_IF3_MCTL_EOB             0x00000080 // End of Block
#define CAN_IF3_MCTL_TXRQST          0x00000100 // Transmit Request
#define CAN_IF3_MCTL_RMTEN           0x00000200 // Remote Enable
#define CAN_IF3_MCTL_RXIE            0x00000400 // Receive Interrupt Enable
#define CAN_IF3_MCTL_TXIE            0x00000800 // Transmit Interrupt Enable
#define CAN_IF3_MCTL_UMASK           0x00001000 // Use Acceptance Mask
#define CAN_IF3_MCTL_INTPND          0x00002000 // Interrupt Pending
#define CAN_IF3_MCTL_MSGLST          0x00004000 // Message Lost
#define CAN_IF3_MCTL_NEWDAT          0x00008000 // New Data

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF3_DATA
// register.
//*****************************************************************************

#define CAN_IF3_DATA_DATA0_S         0
#define CAN_IF3_DATA_DATA0_M         0x000000FF // Data Byte 0
#define CAN_IF3_DATA_DATA1_S         8
#define CAN_IF3_DATA_DATA1_M         0x0000FF00 // Data Byte 1
#define CAN_IF3_DATA_DATA2_S         16
#define CAN_IF3_DATA_DATA2_M         0x00FF0000 // Data Byte 2
#define CAN_IF3_DATA_DATA3_S         24
#define CAN_IF3_DATA_DATA3_M         0xFF000000 // Data Byte 3

//*****************************************************************************
// The following are defines for the bit fields in the CAN_O_IF3_DATB
// register.
//*****************************************************************************
#define CAN_IF3_DATB_DATA4_S         0
#define CAN_IF3_DATB_DATA4_M         0x000000FF // Data Byte 4
#define CAN_IF3_DATB_DATA5_S         8
#define CAN_IF3_DATB_DATA5_M         0x0000FF00 // Data Byte 5
#define CAN_IF3_DATB_DATA6_S         16
#define CAN_IF3_DATB_DATA6_M         0x00FF0000 // Data Byte 6
#define CAN_IF3_DATB_DATA7_S         24
#define CAN_IF3_DATB_DATA7_M         0xFF000000 // Data Byte 7


#endif // __HW_CAN_H__




