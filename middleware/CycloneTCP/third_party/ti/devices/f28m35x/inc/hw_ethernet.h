//###########################################################################
// FILE:   hw_ethernet.h
// TITLE:  Macros used when accessing the Ethernet hardware.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_ETHERNET_H__
#define __HW_ETHERNET_H__

//*****************************************************************************
// The following are defines for the Ethernet MAC register offsets.
//*****************************************************************************
#define MAC_O_RIS            0x00000000     // Ethernet MAC Raw Interrupt
                                            // Status/Acknowledge
#define MAC_O_IACK           0x00000000     // Ethernet MAC Raw Interrupt
                                            // Status/Acknowledge
#define MAC_O_IM             0x00000004     // Ethernet MAC Interrupt Mask
#define MAC_O_RCTL           0x00000008     // Ethernet MAC Receive Control
#define MAC_O_TCTL           0x0000000C     // Ethernet MAC Transmit Control
#define MAC_O_DATA           0x00000010     // Ethernet MAC Data
#define MAC_O_IA0            0x00000014     // Ethernet MAC Individual Address
                                            // 0
#define MAC_O_IA1            0x00000018     // Ethernet MAC Individual Address
                                            // 1
#define MAC_O_THR            0x0000001C     // Ethernet MAC Threshold
#define MAC_O_MCTL           0x00000020     // Ethernet MAC Management Control
#define MAC_O_MDV            0x00000024     // Ethernet MAC Management Divider
#define MAC_O_MTXD           0x0000002C     // Ethernet MAC Management Transmit
                                            // Data
#define MAC_O_MRXD           0x00000030     // Ethernet MAC Management Receive
                                            // Data
#define MAC_O_NP             0x00000034     // Ethernet MAC Number of Packets
#define MAC_O_TR             0x00000038     // Ethernet MAC Transmission
                                            // Request
#define MAC_O_TS             0x0000003C     // Ethernet MAC Timer Support

//*****************************************************************************
// The following are defines for the bit fields in the MAC_O_RIS register.
//*****************************************************************************
#define MAC_RIS_PHYINT       0x00000040     // PHY Interrupt
#define MAC_RIS_MDINT        0x00000020     // MII Transaction Complete
#define MAC_RIS_RXER         0x00000010     // Receive Error
#define MAC_RIS_FOV          0x00000008     // FIFO Overrun
#define MAC_RIS_TXEMP        0x00000004     // Transmit FIFO Empty
#define MAC_RIS_TXER         0x00000002     // Transmit Error
#define MAC_RIS_RXINT        0x00000001     // Packet Received

//*****************************************************************************
// The following are defines for the bit fields in the MAC_O_IACK register.
//*****************************************************************************
#define MAC_IACK_PHYINT      0x00000040     // Clear PHY Interrupt
#define MAC_IACK_MDINT       0x00000020     // Clear MII Transaction Complete
#define MAC_IACK_RXER        0x00000010     // Clear Receive Error
#define MAC_IACK_FOV         0x00000008     // Clear FIFO Overrun
#define MAC_IACK_TXEMP       0x00000004     // Clear Transmit FIFO Empty
#define MAC_IACK_TXER        0x00000002     // Clear Transmit Error
#define MAC_IACK_RXINT       0x00000001     // Clear Packet Received

//*****************************************************************************
// The following are defines for the bit fields in the MAC_O_IM register.
//*****************************************************************************
#define MAC_IM_PHYINTM       0x00000040     // Mask PHY Interrupt
#define MAC_IM_MDINTM        0x00000020     // Mask MII Transaction Complete
#define MAC_IM_RXERM         0x00000010     // Mask Receive Error
#define MAC_IM_FOVM          0x00000008     // Mask FIFO Overrun
#define MAC_IM_TXEMPM        0x00000004     // Mask Transmit FIFO Empty
#define MAC_IM_TXERM         0x00000002     // Mask Transmit Error
#define MAC_IM_RXINTM        0x00000001     // Mask Packet Received

//*****************************************************************************
// The following are defines for the bit fields in the MAC_O_RCTL register.
//*****************************************************************************
#define MAC_RCTL_RSTFIFO     0x00000010     // Clear Receive FIFO
#define MAC_RCTL_BADCRC      0x00000008     // Enable Reject Bad CRC
#define MAC_RCTL_PRMS        0x00000004     // Enable Promiscuous Mode
#define MAC_RCTL_AMUL        0x00000002     // Enable Multicast Frames
#define MAC_RCTL_RXEN        0x00000001     // Enable Receiver

//*****************************************************************************
// The following are defines for the bit fields in the MAC_O_TCTL register.
//*****************************************************************************
#define MAC_TCTL_DUPLEX      0x00000010     // Enable Duplex Mode
#define MAC_TCTL_CRC         0x00000004     // Enable CRC Generation
#define MAC_TCTL_PADEN       0x00000002     // Enable Packet Padding
#define MAC_TCTL_TXEN        0x00000001     // Enable Transmitter

//*****************************************************************************
// The following are defines for the bit fields in the MAC_O_DATA register.
//*****************************************************************************
#define MAC_DATA_TXDATA_M    0xFFFFFFFF     // Transmit FIFO Data
#define MAC_DATA_RXDATA_M    0xFFFFFFFF     // Receive FIFO Data
#define MAC_DATA_RXDATA_S    0
#define MAC_DATA_TXDATA_S    0

//*****************************************************************************
// The following are defines for the bit fields in the MAC_O_IA0 register.
//*****************************************************************************
#define MAC_IA0_MACOCT4_M    0xFF000000     // MAC Address Octet 4
#define MAC_IA0_MACOCT3_M    0x00FF0000     // MAC Address Octet 3
#define MAC_IA0_MACOCT2_M    0x0000FF00     // MAC Address Octet 2
#define MAC_IA0_MACOCT1_M    0x000000FF     // MAC Address Octet 1
#define MAC_IA0_MACOCT4_S    24
#define MAC_IA0_MACOCT3_S    16
#define MAC_IA0_MACOCT2_S    8
#define MAC_IA0_MACOCT1_S    0

//*****************************************************************************
// The following are defines for the bit fields in the MAC_O_IA1 register.
//*****************************************************************************
#define MAC_IA1_MACOCT6_M    0x0000FF00     // MAC Address Octet 6
#define MAC_IA1_MACOCT5_M    0x000000FF     // MAC Address Octet 5
#define MAC_IA1_MACOCT6_S    8
#define MAC_IA1_MACOCT5_S    0

//*****************************************************************************
// The following are defines for the bit fields in the MAC_O_THR register.
//*****************************************************************************
#define MAC_THR_THRESH_M     0x0000003F     // Threshold Value
#define MAC_THR_THRESH_S     0

//*****************************************************************************
// The following are defines for the bit fields in the MAC_O_MCTL register.
//*****************************************************************************
#define MAC_MCTL_REGADR_M    0x000000F8     // MII Register Address
#define MAC_MCTL_WRITE       0x00000002     // MII Register Transaction Type
#define MAC_MCTL_START       0x00000001     // MII Register Transaction Enable
#define MAC_MCTL_REGADR_S    3

//*****************************************************************************
// The following are defines for the bit fields in the MAC_O_MDV register.
//*****************************************************************************
#define MAC_MDV_DIV_M        0x000000FF     // Clock Divider
#define MAC_MDV_DIV_S        0

//*****************************************************************************
// The following are defines for the bit fields in the MAC_O_MTXD register.
//*****************************************************************************
#define MAC_MTXD_MDTX_M      0x0000FFFF     // MII Register Transmit Data
#define MAC_MTXD_MDTX_S      0

//*****************************************************************************
// The following are defines for the bit fields in the MAC_O_MRXD register.
//*****************************************************************************
#define MAC_MRXD_MDRX_M      0x0000FFFF     // MII Register Receive Data
#define MAC_MRXD_MDRX_S      0

//*****************************************************************************
// The following are defines for the bit fields in the MAC_O_NP register.
//*****************************************************************************
#define MAC_NP_NPR_M         0x0000003F     // Number of Packets in Receive
                                            // FIFO
#define MAC_NP_NPR_S         0

//*****************************************************************************
// The following are defines for the bit fields in the MAC_O_TR register.
//*****************************************************************************
#define MAC_TR_NEWTX         0x00000001     // New Transmission

//*****************************************************************************
// The following are defines for the bit fields in the MAC_O_TS register.
//*****************************************************************************
#define MAC_TS_TSEN          0x00000001     // Time Stamp Enable

//*****************************************************************************
// The following are defines for the Ethernet Controller PHY registers.
//*****************************************************************************
#define PHY_MR0              0x00000000     // Ethernet PHY Management Register
                                            // 0 - Control
#define PHY_MR1              0x00000001     // Ethernet PHY Management Register
                                            // 1 - Status
#define PHY_MR2              0x00000002     // Ethernet PHY Management Register
                                            // 2 - PHY Identifier 1
#define PHY_MR3              0x00000003     // Ethernet PHY Management Register
                                            // 3 - PHY Identifier 2
#define PHY_MR4              0x00000004     // Ethernet PHY Management Register
                                            // 4 - Auto-Negotiation
#define PHY_MR5              0x00000005     // Ethernet PHY Management Register
                                            // 5 - Auto-Negotiation Link
#define PHY_MR6              0x00000006     // Ethernet PHY Management Register
                                            // 6 - Auto-Negotiation Expansion

//*****************************************************************************
// The following are defines for the bit fields in the PHY_MR0 register.
//*****************************************************************************
#define PHY_MR0_RESET        0x00008000     // Reset Registers
#define PHY_MR0_LOOPBK       0x00004000     // Loopback Mode
#define PHY_MR0_SPEEDSL      0x00002000     // Speed Select
#define PHY_MR0_ANEGEN       0x00001000     // Auto-Negotiation Enable
#define PHY_MR0_PWRDN        0x00000800     // Power Down
#define PHY_MR0_ISO          0x00000400     // Isolate
#define PHY_MR0_RANEG        0x00000200     // Restart Auto-Negotiation
#define PHY_MR0_DUPLEX       0x00000100     // Set Duplex Mode
#define PHY_MR0_COLT         0x00000080     // Collision Test

//*****************************************************************************
// The following are defines for the bit fields in the PHY_MR1 register.
//*****************************************************************************
#define PHY_MR1_100X_F       0x00004000     // 100BASE-TX Full-Duplex Mode
#define PHY_MR1_100X_H       0x00002000     // 100BASE-TX Half-Duplex Mode
#define PHY_MR1_10T_F        0x00001000     // 10BASE-T Full-Duplex Mode
#define PHY_MR1_10T_H        0x00000800     // 10BASE-T Half-Duplex Mode
#define PHY_MR1_MFPS         0x00000040     // Management Frames with Preamble
                                            // Suppressed
#define PHY_MR1_ANEGC        0x00000020     // Auto-Negotiation Complete
#define PHY_MR1_RFAULT       0x00000010     // Remote Fault
#define PHY_MR1_ANEGA        0x00000008     // Auto-Negotiation
#define PHY_MR1_LINK         0x00000004     // Link Made
#define PHY_MR1_JAB          0x00000002     // Jabber Condition
#define PHY_MR1_EXTD         0x00000001     // Extended Capabilities

//*****************************************************************************
// The following are defines for the bit fields in the PHY_MR2 register.
//*****************************************************************************
#define PHY_MR2_OUI_M        0x0000FFFF     // Organizationally Unique
                                            // Identifier[21:6]
#define PHY_MR2_OUI_S        0

//*****************************************************************************
// The following are defines for the bit fields in the PHY_MR3 register.
//*****************************************************************************
#define PHY_MR3_OUI_M        0x0000FC00     // Organizationally Unique
                                            // Identifier[5:0]
#define PHY_MR3_MN_M         0x000003F0     // Model Number
#define PHY_MR3_RN_M         0x0000000F     // Revision Number
#define PHY_MR3_OUI_S        10
#define PHY_MR3_MN_S         4
#define PHY_MR3_RN_S         0

//*****************************************************************************
// The following are defines for the bit fields in the PHY_MR4 register.
//*****************************************************************************
#define PHY_MR4_NP           0x00008000     // Next Page
#define PHY_MR4_RF           0x00002000     // Remote Fault
#define PHY_MR4_A3           0x00000100     // Technology Ability Field [3]
#define PHY_MR4_A2           0x00000080     // Technology Ability Field [2]
#define PHY_MR4_A1           0x00000040     // Technology Ability Field [1]
#define PHY_MR4_A0           0x00000020     // Technology Ability Field [0]
#define PHY_MR4_S_M          0x0000001F     // Selector Field
#define PHY_MR4_S_S          0

//*****************************************************************************
// The following are defines for the bit fields in the PHY_MR5 register.
//*****************************************************************************
#define PHY_MR5_NP           0x00008000     // Next Page
#define PHY_MR5_ACK          0x00004000     // Acknowledge
#define PHY_MR5_RF           0x00002000     // Remote Fault
#define PHY_MR5_A_M          0x00001FE0     // Technology Ability Field
#define PHY_MR5_S_M          0x0000001F     // Selector Field
#define PHY_MR5_S_8023       0x00000001     // IEEE Std 802.3
#define PHY_MR5_S_8029       0x00000002     // IEEE Std 802.9 ISLAN-16T
#define PHY_MR5_S_8025       0x00000003     // IEEE Std 802.5
#define PHY_MR5_S_1394       0x00000004     // IEEE Std 1394
#define PHY_MR5_A_S          5

//*****************************************************************************
// The following are defines for the bit fields in the PHY_MR6 register.
//*****************************************************************************
#define PHY_MR6_PDF          0x00000010     // Parallel Detection Fault
#define PHY_MR6_LPNPA        0x00000008     // Link Partner is Next Page Able
#define PHY_MR6_PRX          0x00000002     // New Page Received
#define PHY_MR6_LPANEGA      0x00000001     // Link Partner is Auto-Negotiation
                                            // Able


#endif // __HW_ETHERNET_H__



