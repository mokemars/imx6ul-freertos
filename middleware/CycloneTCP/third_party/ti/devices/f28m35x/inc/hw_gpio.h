//###########################################################################
// FILE:   hw_gpio.h
// TITLE:  Defines and Macros for GPIO hardware.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_GPIO_H__
#define __HW_GPIO_H__

//*****************************************************************************
// The following are defines for the GPIO register offsets.
//*****************************************************************************
#define GPIO_O_DATA                  0x00000000 // GPIO Data
#define GPIO_O_DATA_ALL              0x000003FC // GPIO Data Represented by Bit
#define GPIO_O_DIR                   0x00000400 // GPIO Direction
#define GPIO_O_IS                    0x00000404 // GPIO Interrupt Sense
#define GPIO_O_IBE                   0x00000408 // GPIO Interrupt Both Edges
#define GPIO_O_IEV                   0x0000040C // GPIO Interrupt Event
#define GPIO_O_IM                    0x00000410 // GPIO Interrupt Mask
#define GPIO_O_RIS                   0x00000414 // GPIO Raw Interrupt Status
#define GPIO_O_MIS                   0x00000418 // GPIO Masked Interrupt Status
#define GPIO_O_ICR                   0x0000041C // GPIO Interrupt Clear
#define GPIO_O_AFSEL                 0x00000420 // GPIO Alternate Function
                                                // Select
#define GPIO_O_ODR                   0x0000050C // GPIO Open Drain Select
#define GPIO_O_PUR                   0x00000510 // GPIO Pull-Up Select
#define GPIO_O_DEN                   0x0000051C // GPIO Digital Enable
#define GPIO_O_LOCK                  0x00000520 // GPIO Lock
#define GPIO_O_CR                    0x00000524 // GPIO Commit
#define GPIO_O_AMSEL                 0x00000528 // GPIO Analog Mode Select
#define GPIO_O_PCTL                  0x0000052C // GPIO Port Control
#define GPIO_O_APSEL                 0x00000530 // GPIO Alternate Port Select
#define GPIO_O_CSEL                  0x00000534 // GPIO Core Select

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_O_LOCK register.
//*****************************************************************************
#define GPIO_LOCK_M                  0xFFFFFFFF // GPIO Lock
#define GPIO_LOCK_UNLOCKED           0x00000000 // The GPIOCR register is
                                                // unlocked and may be modified
#define GPIO_LOCK_LOCKED             0x00000001 // The GPIOCR register is
                                                // locked and may not be
                                                // modified
#define GPIO_LOCK_KEY_DD             0x4C4F434B // Unlocks the GPIO_CR register

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_PCTL register for
// port A.
//*****************************************************************************
#define GPIO_PCTL_PA0_M              0x0000000F // PA0 mask
#define GPIO_PCTL_PA0_U0RX           0x00000001 // U0RX on PA0
#define GPIO_PCTL_PA0_I2C1SCL        0x00000008 // I2C1SCL on PA0
#define GPIO_PCTL_PA0_U1RX           0x00000009 // U1RX on PA0
#define GPIO_PCTL_PA1_M              0x000000F0 // PA1 mask
#define GPIO_PCTL_PA1_U0TX           0x00000010 // U0TX on PA1
#define GPIO_PCTL_PA1_I2C1SDA        0x00000080 // I2C1SDA on PA1
#define GPIO_PCTL_PA1_U1TX           0x00000090 // U1TX on PA1
#define GPIO_PCTL_PA1_SSI1FSS        0x000000F0 // SSI1FSS on PA1
#define GPIO_PCTL_PA2_M              0x00000F00 // PA2 mask
#define GPIO_PCTL_PA2_SSI0CLK        0x00000100 // SSI0CLK on PA2
#define GPIO_PCTL_PA2_MIITXD2        0x00000300 // EMAC MIITXD2 on PA2
#define GPIO_PCTL_PA2_U1CTS          0x00000E00 // U1CTS on PA2
#define GPIO_PCTL_PA3_M              0x0000F000 // PA3 mask
#define GPIO_PCTL_PA3_SSI0FSS        0x00001000 // SSI0FSS on PA3
#define GPIO_PCTL_PA3_MIITXD1        0x00003000 // EMAC MIITXD1 on PA3
#define GPIO_PCTL_PA3_U1DCD          0x0000E000 // U1DCD on PA3
#define GPIO_PCTL_PA3_SSI1CLK        0x0000F000 // SSI1CLK on PA3
#define GPIO_PCTL_PA4_M              0x000F0000 // PA4 mask
#define GPIO_PCTL_PA4_SSI0RX         0x00010000 // SSI0RX on PA4
#define GPIO_PCTL_PA4_MIITXD0        0x00030000 // EMAC MIITXD0 on PA4
#define GPIO_PCTL_PA4_CAN0RX         0x00050000 // CAN0RX on PA4
#define GPIO_PCTL_PA4_U1DSR          0x000E0000 // U1DSR on PA4
#define GPIO_PCTL_PA5_M              0x00F00000 // PA5 mask
#define GPIO_PCTL_PA5_SSI0TX         0x00100000 // SSI0TX on PA5
#define GPIO_PCTL_PA5_MIIRXDV        0x00300000 // EMAC MIIRXDV on PA5
#define GPIO_PCTL_PA5_CAN0TX         0x00500000 // CAN0TX on PA5
#define GPIO_PCTL_PA5_U1RTS          0x00E00000 // U1RTS on PA5
#define GPIO_PCTL_PA6_M              0x0F000000 // PA6 mask
#define GPIO_PCTL_PA6_I2C1SCL        0x01000000 // I2C1SCL on PA6
#define GPIO_PCTL_PA6_CCP1           0x02000000 // CCP1 on PA6
#define GPIO_PCTL_PA6_MIIRXCK        0x03000000 // EMAC MIIRXCK on PA6
#define GPIO_PCTL_PA6_CAN0RX         0x06000000 // CAN0RX on PA6
#define GPIO_PCTL_PA6_USB0EPEN       0x08000000 // USB0EPEN on PA6
#define GPIO_PCTL_PA6_U1CTS          0x09000000 // U1CTS on PA6
#define GPIO_PCTL_PA6_U1DTR          0x0E000000 // U1DTR on PA6
#define GPIO_PCTL_PA7_M              0xF0000000 // PA7 mask
#define GPIO_PCTL_PA7_I2C1SDA        0x10000000 // I2C1SDA on PA7
#define GPIO_PCTL_PA7_CCP4           0x20000000 // CCP4 on PA7
#define GPIO_PCTL_PA7_MIIRXER        0x30000000 // EMAC MIIRXER on PA7
#define GPIO_PCTL_PA7_CAN0TX         0x60000000 // CAN0TX on PA7
#define GPIO_PCTL_PA7_CCP3           0x70000000 // CCP3 on PA7
#define GPIO_PCTL_PA7_USB0PFLT       0x80000000 // USB0PFLT on PA7
#define GPIO_PCTL_PA7_U1DCD          0x90000000 // U1DCD on PA7
#define GPIO_PCTL_PA7_U1RI           0xE0000000 // E1RI on PA7

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_PCTL register for
// port B.
//*****************************************************************************
#define GPIO_PCTL_PB0_M              0x0000000F // PB0 mask
#define GPIO_PCTL_PB0_CCP0           0x00000001 // CCP0 on PB0
#define GPIO_PCTL_PB0_U1RX           0x00000005 // U1RX on PB0
#define GPIO_PCTL_PB0_SSI2TX         0x0000000D // SSI2TX on PB0
#define GPIO_PCTL_PB0_CAN1TX         0x0000000E // CAN1TX on PB0
#define GPIO_PCTL_PB0_U4TX           0x0000000F // U4TX on PB0
#define GPIO_PCTL_PB1_M              0x000000F0 // PB1 mask
#define GPIO_PCTL_PB1_CCP2           0x00000010 // CCP2 on PB1
#define GPIO_PCTL_PB1_CCP1           0x00000040 // CCP1 on PB1
#define GPIO_PCTL_PB1_U1TX           0x00000050 // U1TX on PB1
#define GPIO_PCTL_PB1_SSI2RX         0x000000D0 // SSI2RX on PB1
#define GPIO_PCTL_PB2_M              0x00000F00 // PB2 mask
#define GPIO_PCTL_PB2_I2C0SCL        0x00000100 // I2C0SCL on PB2
#define GPIO_PCTL_PB2_CCP3           0x00000400 // CCP3 on PB2
#define GPIO_PCTL_PB2_CCP0           0x00000500 // CCP0 on PB2
#define GPIO_PCTL_PB2_USB0EPEN       0x00000800 // USB0EPEN on PB2
#define GPIO_PCTL_PB2_SSI2CLK        0x00000D00 // SSI2CLK on PB2
#define GPIO_PCTL_PB2_CAN1RX         0x00000E00 // CAN1RX on PB2
#define GPIO_PCTL_PB2_U4RX           0x00000F00 // U4RX on PB2
#define GPIO_PCTL_PB3_M              0x0000F000 // PB3 mask
#define GPIO_PCTL_PB3_I2C0SDA        0x00001000 // I2C0SDA on PB3
#define GPIO_PCTL_PB3_USB0PFLT       0x00008000 // USB0PFLT on PB3
#define GPIO_PCTL_PB3_SSI2FSS        0x0000D000 // SSI2TX on PB3
#define GPIO_PCTL_PB3_U1RX           0x0000E000 // U1RX on PB3
#define GPIO_PCTL_PB4_M              0x000F0000 // PB4 mask
#define GPIO_PCTL_PB4_U2RX           0x00040000 // U2RX on PB4
#define GPIO_PCTL_PB4_CAN0RX         0x00050000 // CAN0RX on PB4
#define GPIO_PCTL_PB4_U1RX           0x00070000 // U1RX on PB4
#define GPIO_PCTL_PB4_EPI0S23        0x00080000 // EPI0S23 on PB4
#define GPIO_PCTL_PB4_CAN1TX         0x000E0000 // CAN1TX on PB4
#define GPIO_PCTL_PB4_SSI1TX         0x000F0000 // SSI1TX on PB4
#define GPIO_PCTL_PB5_M              0x00F00000 // PB5 mask
#define GPIO_PCTL_PB5_CCP5           0x00200000 // CCP5 on PB5
#define GPIO_PCTL_PB5_CCP6           0x00300000 // CCP6 on PB5
#define GPIO_PCTL_PB5_CCP0           0x00400000 // CCP0 on PB5
#define GPIO_PCTL_PB5_CAN0TX         0x00500000 // CAN0TX on PB5
#define GPIO_PCTL_PB5_CCP2           0x00600000 // CCP2 on PB5
#define GPIO_PCTL_PB5_U1TX           0x00700000 // U1TX on PB5
#define GPIO_PCTL_PB5_EPI0S22        0x00800000 // EPI0S22 on PB5
#define GPIO_PCTL_PB5_CAN1RX         0x00E00000 // CAN1RX on PB5
#define GPIO_PCTL_PB5_SSI1RX         0x00F00000 // SSI1RX on PB5
#define GPIO_PCTL_PB6_M              0x0F000000 // PB6 mask
#define GPIO_PCTL_PB6_CCP1           0x01000000 // CCP1 on PB6
#define GPIO_PCTL_PB6_CCP7           0x02000000 // CCP7 on PB6
#define GPIO_PCTL_PB6_CCP5           0x06000000 // CCP5 on PB6
#define GPIO_PCTL_PB6_MIICRC         0x0C000000 // MIICRC on PB6
#define GPIO_PCTL_PB6_I2C0SDA        0x0D000000 // I2C0SDA on PB6
#define GPIO_PCTL_PB6_U1TX           0x0E000000 // U1TX on PB6
#define GPIO_PCTL_PB6_SSI1CLK        0x0F000000 // SSI1CLK on PB6
#define GPIO_PCTL_PB7_M              0xF0000000 // PB7 mask
#define GPIO_PCTL_PB7_NMI            0x40000000 // NMI on PB7
#define GPIO_PCTL_PB7_MIIRXD1        0x70000000 // EMAC MIIRXD1 on PB7
#define GPIO_PCTL_PB7_I2C0SCL        0xD0000000 // I2C0SCL on PB7
#define GPIO_PCTL_PB7_U1RX           0xE0000000 // U1RX on PB7
#define GPIO_PCTL_PB7_SSI1FSS        0xF0000000 // SSI1FSS on PB7

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_PCTL register for
// port C.
//*****************************************************************************
#define GPIO_PCTL_PC4_M              0x000F0000 // PC4 mask
#define GPIO_PCTL_PC4_CCP5           0x00010000 // CCP5 on PC4
#define GPIO_PCTL_PC4_MIITXD3        0x00030000 // EMAC MIITXD3 on PC4
#define GPIO_PCTL_PC4_CCP2           0x00050000 // CCP2 on PC4
#define GPIO_PCTL_PC4_CCP4           0x00060000 // CCP4 on PC4
#define GPIO_PCTL_PC4_EPI0S2         0x00080000 // EPI0S2 on PC4
#define GPIO_PCTL_PC4_CCP1           0x00090000 // CCP1 on PC4
#define GPIO_PCTL_PC5_M              0x00F00000 // PC5 mask
#define GPIO_PCTL_PC5_CCP1           0x00100000 // CCP1 on PC5
#define GPIO_PCTL_PC5_CCP3           0x00500000 // CCP3 on PC5
#define GPIO_PCTL_PC5_USB0EPEN       0x00600000 // USB0EPEN on PC5
#define GPIO_PCTL_PC5_EPI0S3         0x00800000 // EPI0S3 on PC5
#define GPIO_PCTL_PC6_M              0x0F000000 // PC6 mask
#define GPIO_PCTL_PC6_CCP3           0x01000000 // CCP3 on PC6
#define GPIO_PCTL_PC6_U1RX           0x05000000 // U1RX on PC6
#define GPIO_PCTL_PC6_CCP0           0x06000000 // CCP0 on PC6
#define GPIO_PCTL_PC6_USB0PFLT       0x07000000 // USB0PFLT on PC6
#define GPIO_PCTL_PC6_EPI0S4         0x08000000 // EPI0S4 on PC6
#define GPIO_PCTL_PC7_M              0xF0000000 // PC7 mask
#define GPIO_PCTL_PC7_CCP4           0x10000000 // CCP4 on PC7
#define GPIO_PCTL_PC7_CCP0           0x40000000 // CCP0 on PC7
#define GPIO_PCTL_PC7_U1TX           0x50000000 // U1TX on PC7
#define GPIO_PCTL_PC7_USB0PFLT       0x60000000 // USB0PFLT on PC7
#define GPIO_PCTL_PC7_EPI0S5         0x80000000 // EPI0S5 on PC7

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_PCTL register for
// port D.
//*****************************************************************************
#define GPIO_PCTL_PD0_M              0x0000000F // PD0 mask
#define GPIO_PCTL_PD0_CAN0RX         0x00000002 // CAN0RX on PD0
#define GPIO_PCTL_PD0_U2RX           0x00000004 // U2RX on PD0
#define GPIO_PCTL_PD0_U1RX           0x00000005 // U1RX on PD0
#define GPIO_PCTL_PD0_CCP6           0x00000006 // CCP6 on PD0
#define GPIO_PCTL_PD0_U1CTS          0x00000009 // U1CTS on PD0
#define GPIO_PCTL_PD0_MIIRXD2        0x0000000C // MIIRXD2 on PD0
#define GPIO_PCTL_PD0_SSI0TX         0x0000000D // SSI0TX on PD0
#define GPIO_PCTL_PD0_CAN1TX         0x0000000E // CAN1TX on PD0
#define GPIO_PCTL_PD0_USB0EPEN       0x0000000F // USB0EPEN on PDO
#define GPIO_PCTL_PD1_M              0x000000F0 // PD1 mask
#define GPIO_PCTL_PD1_CAN0TX         0x00000020 // CAN0TX on PD1
#define GPIO_PCTL_PD1_U2TX           0x00000040 // U2TX on PD1
#define GPIO_PCTL_PD1_U1TX           0x00000050 // U1TX on PD1
#define GPIO_PCTL_PD1_CCP7           0x00000060 // CCP7 on PD1
#define GPIO_PCTL_PD1_U1DCD          0x00000090 // U1DCD on PD1
#define GPIO_PCTL_PD1_CCP2           0x000000A0 // CCP2 on PD1
#define GPIO_PCTL_PD1_MIICOL         0x000000C0 // MIICOL on PD1
#define GPIO_PCTL_PD1_SSI0RX         0x000000D0 // SSI0RX on PD1
#define GPIO_PCTL_PD1_CAN1RX         0x000000E0 // CAN1RX on PD1
#define GPIO_PCTL_PD1_USB0PFLT       0x000000F0 // USB0PFLT on PD1
#define GPIO_PCTL_PD2_M              0x00000F00 // PD2 mask
#define GPIO_PCTL_PD2_U1RX           0x00000100 // U1RX on PD2
#define GPIO_PCTL_PD2_CCP6           0x00000200 // CCP6 on PD2
#define GPIO_PCTL_PD2_CCP5           0x00000400 // CCP5 on PD2
#define GPIO_PCTL_PD2_MIIRXDV        0x00000700 // EMAC MIIRXDV on PD2
#define GPIO_PCTL_PD2_EPI0S20        0x00000800 // EPI0S20 on PD2
#define GPIO_PCTL_PD2_SSI0CLK        0x00000D00 // SSI0CLK on PD2
#define GPIO_PCTL_PD2_U1TX           0x00000E00 // U1TX on PD2
#define GPIO_PCTL_PD2_CAN0RX         0x00000F00 // CAN0RX on PD2
#define GPIO_PCTL_PD3_M              0x0000F000 // PD3 mask
#define GPIO_PCTL_PD3_U1TX           0x00001000 // U1TX on PD3
#define GPIO_PCTL_PD3_CCP7           0x00002000 // CCP7 on PD3
#define GPIO_PCTL_PD3_CCP0           0x00004000 // CCP0 on PD3
#define GPIO_PCTL_PD3_MIITXER        0x00007000 // EMAC MIITXER on Pd3
#define GPIO_PCTL_PD3_EPI0S21        0x00008000 // EPI0S21 on PD3
#define GPIO_PCTL_PD3_SSI0FSS        0x0000D000 // SSI0FSS on PD3
#define GPIO_PCTL_PD3_U1RX           0x0000E000 // U1RX on PD3
#define GPIO_PCTL_PD3_CAN0TX         0x0000F000 // CAN0TX on PD3
#define GPIO_PCTL_PD4_M              0x000F0000 // PD4 mask
#define GPIO_PCTL_PD4_CCP0           0x00010000 // CCP0 on PD4
#define GPIO_PCTL_PD4_CCP3           0x00020000 // CCP3 on PD4
#define GPIO_PCTL_PD4_MIITXD3        0x00040000 // EMAC MIITXD3 on PD4
#define GPIO_PCTL_PD4_U1RI           0x00090000 // U1RI on PD4
#define GPIO_PCTL_PD4_EPI0S19        0x000A0000 // EPI0S19 on PD4
#define GPIO_PCTL_PD4_U3TX           0x000E0000 // U3TX on PD4
#define GPIO_PCTL_PD4_CAN1TX         0x000F0000 // CAN1TX on PD4
#define GPIO_PCTL_PD5_M              0x00F00000 // PD5 mask
#define GPIO_PCTL_PD5_CCP2           0x00100000 // CCP2 on PD5
#define GPIO_PCTL_PD5_CCP4           0x00200000 // CCP4 on PD5
#define GPIO_PCTL_PD5_MIITXD2        0x00400000 // EMAC MIITXD2 on PD5
#define GPIO_PCTL_PD5_U2RX           0x00900000 // U2RX on PD5
#define GPIO_PCTL_PD5_EPI0S28        0x00A00000 // EPI0S28 on PD5
#define GPIO_PCTL_PD5_U3RX           0x00E00000 // U3RX on PD5
#define GPIO_PCTL_PD5_CAN1RX         0x00F00000 // CAN1RX on PD5
#define GPIO_PCTL_PD6_M              0x0F000000 // PD6 mask
#define GPIO_PCTL_PD6_MIITXD1        0x04000000 // EMAC MIITXD1 on PD6
#define GPIO_PCTL_PD6_U2TX           0x09000000 // U2TX on PD6
#define GPIO_PCTL_PD6_EPI0S29        0x0A000000 // EPI0S29 on PD6
#define GPIO_PCTL_PD6_I2C1SDA        0x0E000000 // I2C1SDA on PD6
#define GPIO_PCTL_PD6_U1TX           0x0F000000 // U1TX
#define GPIO_PCTL_PD7_M              0xF0000000 // PD7 mask
#define GPIO_PCTL_PD7_CCP1           0x30000000 // CCP1 on PD7
#define GPIO_PCTL_PD7_MIITXD0        0x40000000 // EMAC MIITXD0 on PD7
#define GPIO_PCTL_PD7_U1DTR          0x90000000 // U1DTR on PD7
#define GPIO_PCTL_PD7_EPI0S30        0xA0000000 // EPI0S30 on PD7
#define GPIO_PCTL_PD7_I2C1SCL        0xE0000000 // I2C1SCL on PD7
#define GPIO_PCTL_PD7_U1RX           0xF0000000 // U1RX on PD7

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_PCTL register for
// port E.
//*****************************************************************************
#define GPIO_PCTL_PE0_M              0x0000000F // PE0 mask
#define GPIO_PCTL_PE0_SSI1CLK        0x00000002 // SSI1CLK on PE0
#define GPIO_PCTL_PE0_CCP3           0x00000003 // CCP3 on PE0
#define GPIO_PCTL_PE0_EPI0S8         0x00000008 // EPI0S8 on PE0
#define GPIO_PCTL_PE0_USB0PFLT       0x00000009 // USB0PFLT on PE0
#define GPIO_PCTL_PE0_SSI3TX         0x0000000D // SSI3TX on PE0
#define GPIO_PCTL_PE0_CAN0RX         0x0000000E // CAN0RX on PE0
#define GPIO_PCTL_PE0_SSI1TX         0x0000000F // SSI1TX on PE0
#define GPIO_PCTL_PE1_M              0x000000F0 // PE1 mask
#define GPIO_PCTL_PE1_SSI1FSS        0x00000020 // SSI1FSS on PE1
#define GPIO_PCTL_PE1_CCP2           0x00000040 // CCP2 on PE1
#define GPIO_PCTL_PE1_CCP6           0x00000050 // CCP6 on PE1
#define GPIO_PCTL_PE1_EPI0S9         0x00000080 // EPI0S9 on PE1
#define GPIO_PCTL_PE1_SSI3RX         0x000000D0 // SSI3RX on PE1
#define GPIO_PCTL_PE1_CAN0TX         0x000000E0 // CAN0TX on PE1
#define GPIO_PCTL_PE1_SSI1RX         0x000000F0 // SSI1RX on PE1
#define GPIO_PCTL_PE2_M              0x00000F00 // PE2 mask
#define GPIO_PCTL_PE2_CCP4           0x00000100 // CCP4 on PE2
#define GPIO_PCTL_PE2_SSI1RX         0x00000200 // SSI1RX on PE2
#define GPIO_PCTL_PE2_CCP2           0x00000500 // CCP2 on PE2
#define GPIO_PCTL_PE2_EPI0S24        0x00000800 // EPI0S24 on PE2
#define GPIO_PCTL_PE2_SSI3CLK        0x00000D00 // SSI3CLK on PE2
#define GPIO_PCTL_PE2_U2RX           0x00000E00 // U2RX on PE2
#define GPIO_PCTL_PE2_SSI1CLK        0x00000F00 // SSI1CLK on PE2
#define GPIO_PCTL_PE3_M              0x0000F000 // PE3 mask
#define GPIO_PCTL_PE3_CCP1           0x00001000 // CCP1 on PE3
#define GPIO_PCTL_PE3_SSI1TX         0x00002000 // SSI1TX on PE3
#define GPIO_PCTL_PE3_CCP7           0x00005000 // CCP7 on PE3
#define GPIO_PCTL_PE3_EPI0S25        0x00008000 // EPI0S25 on PE3
#define GPIO_PCTL_PE3_SSI3FSS        0x0000D000 // SSI3FSS on PE3
#define GPIO_PCTL_PE3_U2TX           0x0000E000 // U2TX on PE3
#define GPIO_PCTL_PE3_SSI1FSS        0x0000F000 // SSI1FSS on PE3
#define GPIO_PCTL_PE4_M              0x000F0000 // PE4 mask
#define GPIO_PCTL_PE4_CCP3           0x00010000 // CCP3 on PE4
#define GPIO_PCTL_PE4_U2TX           0x00050000 // U2TX on PE4
#define GPIO_PCTL_PE4_CCP2           0x00060000 // CCP2 on PE4
#define GPIO_PCTL_PE4_MIIRXD0        0x00070000 // EMAC MIIRXD0 on PE4
#define GPIO_PCTL_PE4_U0RX           0x000D0000 // U0RX on PE4
#define GPIO_PCTL_PE4_USB0EPEN       0x000F0000 // USB0EPEN on PE4
#define GPIO_PCTL_PE5_M              0x00F00000 // PE5 mask
#define GPIO_PCTL_PE5_CCP5           0x00100000 // CCP5 on PE5
#define GPIO_PCTL_PE5_MIITXER        0x00C00000 // MIITXER on PE5
#define GPIO_PCTL_PE5_U0TX           0x00D00000 // U0TX on PE5
#define GPIO_PCTL_PE5_USB0PFLT       0x00F00000 // USB0PFLT on PE5
#define GPIO_PCTL_PE6_M              0x0F000000 // PE6 mask
#define GPIO_PCTL_PE6_U1CTS          0x09000000 // U1CTS on PE6
#define GPIO_PCTL_PE6_MIIMDIO        0x0C000000 // MIIMDIO on PE6
#define GPIO_PCTL_PE6_CAN0RX         0x0D000000 // CAN0RX on PE6
#define GPIO_PCTL_PE7_M              0xF0000000 // PE7 mask
#define GPIO_PCTL_PE7_U1DCD          0x90000000 // U1DCD on PE7
#define GPIO_PCTL_PE7_MIIRXD3        0xC0000000 // MIIRXD3 on PE7
#define GPIO_PCTL_PE7_CAN0TX         0xD0000000 // CAN0TX on PE7

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_PCTL register for
// port F.
//*****************************************************************************
#define GPIO_PCTL_PF0_M              0x0000000F // PF0 mask
#define GPIO_PCTL_PF0_CAN1RX         0x00000001 // CAN1RX on PF0
#define GPIO_PCTL_PFO_MIIRXCK        0x00000004 // EMAC MIIRXCK on PF0
#define GPIO_PCTL_PF0_U1DSR          0x00000009 // U1DSR on PF0
#define GPIO_PCTL_PF0_I2C0SDA        0x0000000D // I2C0SDA on PF0
#define GPIO_PCTL_PF1_M              0x000000F0 // PF1 mask
#define GPIO_PCTL_PF1_CAN1TX         0x00000010 // CAN1TX on PF1
#define GPIO_PCTL_PF1_MIIRXER        0x00000040 // EMAC MIIRXER on PF1
#define GPIO_PCTL_PF1_U1RTS          0x00000090 // U1RTS on PF1
#define GPIO_PCTL_PF1_CCP3           0x000000A0 // CCP3 on PF1
#define GPIO_PCTL_PF1_I2C0SCL        0x000000D0 // I2C0SCL on PF1
#define GPIO_PCTL_PF2_M              0x00000F00 // PF2 mask
#define GPIO_PCTL_PF2_MIIPHYINTRn    0x00000300 // EMAC MIIPHYINTRn on PF2
#define GPIO_PCTL_PF2_SSI1CLK        0x00000900 // SSI1CLK on PF2
#define GPIO_PCTL_PF2_XCLKOUT0       0x00000F00 // XCLKOUT0 on PF2
#define GPIO_PCTL_PF3_M              0x0000F000 // PF3 mask
#define GPIO_PCTL_PF3_MIIMDC         0x00003000 // EMAC MIIMDC on PF3
#define GPIO_PCTL_PF3_SSI1FSS        0x00009000 // SSI1FSS on PF3
#define GPIO_PCTL_PF3_U0TX           0x0000D000 // U0TX on PF3
#define GPIO_PCTL_PF4_M              0x000F0000 // PF4 mask
#define GPIO_PCTL_PF4_CCP0           0x00010000 // CCP0 on PF4
#define GPIO_PCTL_PF4_MIIMDIO        0x00030000 // EMAC MIIMDIO on PF4
#define GPIO_PCTL_PF4_EPI0S12        0x00080000 // EPI0S12 on PF4
#define GPIO_PCTL_PF4_SSI1RX         0x00090000 // SSI1RX on PF4
#define GPIO_PCTL_PF4_U0RX           0x000D0000 // U0RX on PF4
#define GPIO_PCTL_PF5_M              0x00F00000 // PF5 mask
#define GPIO_PCTL_PF5_CCP2           0x00100000 // CCP2 on PF5
#define GPIO_PCTL_PF5_MIIRXD3        0x00300000 // EMAC MIIRXD3 on PF5
#define GPIO_PCTL_PF5_EPI0S15        0x00800000 // EPI0S15 on PF5
#define GPIO_PCTL_PF5_SSI1TX         0x00900000 // SSI1TX on PF5
#define GPIO_PCTL_PF6_M              0x0F000000 // PF6 mask
#define GPIO_PCTL_PF6_CCP1           0x01000000 // CCP1 on PF6
#define GPIO_PCTL_PF6_MIIRXD2        0x03000000 // EMAC MIIRXD2 on PF6
#define GPIO_PCTL_PF6_U1RTS          0x0A000000 // U1RTS on PF6

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_PCTL register for
// port G.
//*****************************************************************************
#define GPIO_PCTL_PG0_M              0x0000000F // PG0 mask
#define GPIO_PCTL_PG0_U2RX           0x00000001 // U2RX on PG0
#define GPIO_PCTL_PG0_I2C1SCL        0x00000003 // I2C1SCL on PG0
#define GPIO_PCTL_PG0_USB0EPEN       0x00000007 // USB0EPEN on PG0
#define GPIO_PCTL_PG0_EPI0S13        0x00000008 // EPI0S13 on PG0
#define GPIO_PCTL_PG0_MIIRXD2        0x0000000C // MIIRXD2 on PG0
#define GPIO_PCTL_PG0_U4RX           0x0000000D // U4RX on PG0
#define GPIO_PCTL_PG1_M              0x000000F0 // PG1 mask
#define GPIO_PCTL_PG1_U2TX           0x00000010 // U2TX on PG1
#define GPIO_PCTL_PG1_I2C1SDA        0x00000030 // I2C1SDA on PG1
#define GPIO_PCTL_PG1_EPI0S14        0x00000080 // EPI0S14 on PG1
#define GPIO_PCTL_PG1_MIIRXD1        0x000000C0 // MIIRXD1 on PG1
#define GPIO_PCTL_PG1_U4TX           0x000000D0 // U4TX on PG1
#define GPIO_PCTL_PG2_M              0x00000F00 // PG2 mask
#define GPIO_PCTL_PG2_MIICOL         0x00000300 // EMAC MIICOL on PG2
#define GPIO_PCTL_PG3_M              0x0000F000 // PG3 mask
#define GPIO_PCTL_PG3_MIICRS         0x00003000 // EMAC MIICRS on PG3
#define GPIO_PCTL_PG3_MIIRXDV        0x0000C000 // MIIRXDV on PG3
#define GPIO_PCTL_PG5_M              0x00F00000 // PG5 mask
#define GPIO_PCTL_PG5_CCP5           0x00100000 // CCP5 on PG5
#define GPIO_PCTL_PG5_MIITXEN        0x00300000 // EMAC MIITXEN on PG5
#define GPIO_PCTL_PG5_U1DTR          0x00A00000 // U1DTR on PG5
#define GPIO_PCTL_PG6_M              0x0F000000 // PG6 mask
#define GPIO_PCTL_PG6_MIITXCLK       0x03000000 // EMAC MIITXCK on PG6
#define GPIO_PCTL_PG6_U1RI           0x0A000000 // U1RI on PG6
#define GPIO_PCTL_PG7_M              0xF0000000 // PG7 mask
#define GPIO_PCTL_PG7_MIITXER        0x30000000 // EMAC MIITXER on PG7
#define GPIO_PCTL_PG7_CCP5           0x80000000 // CCP5 on PG7
#define GPIO_PCTL_PG7_EPI0S31        0x90000000 // EPI0S31 on PG7

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_PCTL register for
// port H.
//*****************************************************************************
#define GPIO_PCTL_PH0_M              0x0000000F // PH0 mask
#define GPIO_PCTL_PH0_CCP6           0x00000001 // CCP6 on PH0
#define GPIO_PCTL_PH0_MIIPHYRSTn     0x00000003 // EMAC MIIPHYRSTn on PH0
#define GPIO_PCTL_PH0_EPI0S6         0x00000008 // EPI0S6 on PH0
#define GPIO_PCTL_PH0_SSI3TX         0x0000000D // SSI3TX on PH0
#define GPIO_PCTL_PH1_M              0x000000F0 // PH1 mask
#define GPIO_PCTL_PH1_CCP7           0x00000010 // CCP7 on PH1
#define GPIO_PCTL_PH1_EPI0S7         0x00000080 // EPI0S7 on PH1
#define GPIO_PCTL_PH1_MIIRXD0        0x000000C0 // MIIRXD0 on PH1
#define GPIO_PCTL_PH1_SSI3RX         0x000000D0 // SSI3RX on PH1
#define GPIO_PCTL_PH2_M              0x00000F00 // PH2 mask
#define GPIO_PCTL_PH2_EPI0S1         0x00000800 // EPI0S1 on PH2
#define GPIO_PCTL_PH2_MIITXD3        0x00000900 // EMAC MIITXD3 on PH2
#define GPIO_PCTL_PH2_SSI3CLK        0x00000D00 // SSI3CLK on PH2
#define GPIO_PCTL_PH3_M              0x0000F000 // PH3 mask
#define GPIO_PCTL_PH3_USB0EPEN       0x00004000 // USB0EPEN on PH3
#define GPIO_PCTL_PH3_EPI0S0         0x00008000 // EPI0S0 on PH3
#define GPIO_PCTL_PH3_MIITXD2        0x00009000 // EMAC MIITXD2 on PH3
#define GPIO_PCTL_PH3_SSI3FSS        0x0000C000 // SSI3FSS on PH3
#define GPIO_PCTL_PH4_M              0x000F0000 // PH4 mask
#define GPIO_PCTL_PH4_USB0PFLT       0x00040000 // USB0PFLT on PH4
#define GPIO_PCTL_PH4_EPI0S10        0x00080000 // EPI0S10 on PH4
#define GPIO_PCTL_PH4_MIITXD1        0x00090000 // EMAC MIITXD1 on PH4
#define GPIO_PCTL_PH4_SSI1CLK        0x000B0000 // SSI1CLK on PH4
#define GPIO_PCTL_PH4_U3TX           0x000D0000 // U3TX on PH4
#define GPIO_PCTL_PH5_M              0x00F00000 // PH5 mask
#define GPIO_PCTL_PH5_EPI0S11        0x00800000 // EPI0S11 on PH5
#define GPIO_PCTL_PH5_MIITXD0        0x00900000 // EMAC MIITXD0 on PH5
#define GPIO_PCTL_PH5_SSI1FSS        0x00B00000 // SSI1FSS on PH5
#define GPIO_PCTL_PH5_U3RX           0x00D00000 // U3RX on PH5
#define GPIO_PCTL_PH6_M              0x0F000000 // PH6 mask
#define GPIO_PCTL_PH6_EPI0S26        0x08000000 // EPI0S26 on PH6
#define GPIO_PCTL_PH6_MIIRXDV        0x09000000 // EMAC MIIRXDV pn PH6
#define GPIO_PCTL_PH6_SSI1RX         0x0B000000 // SSI1RX on PH6
#define GPIO_PCTL_PH6_MIITXEN        0x0C000000 // MIITXEN on PH6
#define GPIO_PCTL_PH6_SSI0TX         0x0D000000 // SSI0TX on PH6
#define GPIO_PCTL_PH7_M              0xF0000000 // PH7 mask
#define GPIO_PCTL_PH7_MIIRXCK        0x30000000 // EMAC MIIRXCK on PH7
#define GPIO_PCTL_PH7_EPI0S27        0x80000000 // EPI0S27 on PH7
#define GPIO_PCTL_PH7_SSI1TX         0xB0000000 // SSI1TX on PH7
#define GPIO_PCTL_PH7_MIITXCK        0xC0000000 // MIITXCK on PH7
#define GPIO_PCTL_PH7_SSI0RX         0xD0000000 // SSI0RX on PH7

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_PCTL register for
// port J.
//*****************************************************************************
#define GPIO_PCTL_PJ0_M              0x0000000F // PJ0 mask
#define GPIO_PCTL_Pj0_MIIRXER        0x00000003 // EMAC MIIRXER on PJ0
#define GPIO_PCTL_PJ0_EPI0S16        0x00000008 // EPI0S16 on PJ0
#define GPIO_PCTL_PJ0_I2C1SCL        0x0000000B // I2C1SCL on PJ0
#define GPIO_PCTL_PJ0_SSI0CLK        0x0000000D // SSI0CLK on PJ0
#define GPIO_PCTL_PJ1_M              0x000000F0 // PJ1 mask
#define GPIO_PCTL_PJ1_EPI0S17        0x00000080 // EPI0S17 on PJ1
#define GPIO_PCTL_PJ1_USB0PFLT       0x00000090 // USB0PFLT on PJ1
#define GPIO_PCTL_PJ1_I2C1SDA        0x000000B0 // I2C1SDA on PJ1
#define GPIO_PCTL_PJ1_MIIRXDV        0x000000C0 // MIIRXDV on PJ1
#define GPIO_PCTL_PJ1_SSI0FSS        0x000000D0 // SSI0FSS on PJ1
#define GPIO_PCTL_PJ2_M              0x00000F00 // PJ2 mask
#define GPIO_PCTL_PJ2_EPI0S18        0x00000800 // EPI0S18 on PJ2
#define GPIO_PCTL_PJ2_CCP0           0x00000900 // CCP0 on PJ2
#define GPIO_PCTL_PJ2_MIIRXCK        0x00000C00 // MIIRXCK on PJ2
#define GPIO_PCTL_PJ2_SSI0CLK        0x00000D00 // SSI0CLK on PJ2
#define GPIO_PCTL_PJ2_U0TX           0x00000E00 // U0TX on PJ2
#define GPIO_PCTL_PJ3_M              0x0000F000 // PJ3 mask
#define GPIO_PCTL_PJ3_EPI0S19        0x00008000 // EPI0S19 on PJ3
#define GPIO_PCTL_PJ3_U1CTS          0x00009000 // U1CTS on PJ3
#define GPIO_PCTL_PJ3_CCP6           0x0000A000 // CCP6 on PJ3
#define GPIO_PCTL_PJ3_MIIMDC         0x0000C000 // MIIMDC on PJ3
#define GPIO_PCTL_PJ3_SSI0FSS        0x0000D000 // SSI0FSS on PJ3
#define GPIO_PCTL_PJ3_U0RX           0x0000E000 // U0RX on PJ3
#define GPIO_PCTL_PJ4_M              0x000F0000 // PJ4 mask
#define GPIO_PCTL_PJ4_EPI0S28        0x00080000 // EPI0S28 on PJ4
#define GPIO_PCTL_PJ4_U1DCD          0x00090000 // U1DCD on PJ4
#define GPIO_PCTL_PJ4_CCP4           0x000A0000 // CCP4 on PJ4
#define GPIO_PCTL_PJ4_MIICOL         0x000C0000 // MIICOL on PJ4
#define GPIO_PCTL_PJ4_SSI1CLK        0x000D0000 // SSI1CLK on PJ4
#define GPIO_PCTL_PJ5_M              0x00F00000 // PJ5 mask
#define GPIO_PCTL_PJ5_EPI0S29        0x00800000 // EPI0S29 on PJ5
#define GPIO_PCTL_PJ5_U1DSR          0x00900000 // U1DSR on PJ5
#define GPIO_PCTL_PJ5_CCP2           0x00A00000 // CCP2 on PJ5
#define GPIO_PCTL_PJ5_MIICRS         0x00C00000 // MIICRS on PJ5
#define GPIO_PCTL_PJ5_SSI1FSS        0x00D00000 // SSI1FSS on PJ5
#define GPIO_PCTL_PJ6_M              0x0F000000 // PJ6 mask
#define GPIO_PCTL_PJ6_EPI0S30        0x08000000 // EPI0S30 on PJ6
#define GPIO_PCTL_PJ6_U1RTS          0x09000000 // U1RTS on PJ6
#define GPIO_PCTL_PJ6_CCP1           0x0A000000 // CCP1 on PJ6
#define GPIO_PCTL_PJ6_MIIPHYINTRn    0x0C000000 // MIIPHYINTRn on PJ6
#define GPIO_PCTL_PJ6_U2RX           0x0D000000 // U2RX on PJ6
#define GPIO_PCTL_PJ7_M              0xF0000000 // PJ7 mask
#define GPIO_PCTL_PJ7_U1DTR          0x90000000 // U1DTR on PJ7
#define GPIO_PCTL_PJ7_CCP0           0xA0000000 // CCP0 on PJ7
#define GPIO_PCTL_PJ7_MIIPHYRSTn     0xC0000000 // MIIPHYRSTn on PJ7
#define GPIO_PCTL_PJ7_U2TX           0xD0000000 // U2TX on PJ7

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_APSEL register for
// port A.
//*****************************************************************************
#define GPIO_APSEL_PA_APSEL0         0x00000001 // Port A Pin 0 Alternate
                                                // Mapping
#define GPIO_APSEL_PA_APSEL1         0x00000002 // Port A Pin 1 Alternate
                                                // Mapping
#define GPIO_APSEL_PA_APSEL2         0x00000004 // Port A Pin 2 Alternate
                                                // Mapping
#define GPIO_APSEL_PA_APSEL3         0x00000008 // Port A Pin 3 Alternate
                                                // Mapping
#define GPIO_APSEL_PA_APSEL4         0x00000010 // Port A Pin 4 Alternate
                                                // Mapping
#define GPIO_APSEL_PA_APSEL5         0x00000020 // Port A Pin 5 Alternate
                                                // Mapping
#define GPIO_APSEL_PA_APSEL6         0x00000040 // Port A Pin 6 Alternate
                                                // Mapping
#define GPIO_APSEL_PA_APSEL7         0x00000080 // Port A Pin 7 Alternate
                                                // Mapping

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_APSEL register for
// port B.
//*****************************************************************************
#define GPIO_APSEL_PB_APSEL0         0x00000001 // Port B Pin 0 Alternate
                                                // Mapping
#define GPIO_APSEL_PB_APSEL1         0x00000002 // Port B Pin 1 Alternate
                                                // Mapping
#define GPIO_APSEL_PB_APSEL2         0x00000004 // Port B Pin 2 Alternate
                                                // Mapping
#define GPIO_APSEL_PB_APSEL3         0x00000008 // Port B Pin 3 Alternate
                                                // Mapping
#define GPIO_APSEL_PB_APSEL4         0x00000010 // Port B Pin 4 Alternate
                                                // Mapping
#define GPIO_APSEL_PB_APSEL5         0x00000020 // Port B Pin 5 Alternate
                                                // Mapping
#define GPIO_APSEL_PB_APSEL6         0x00000040 // Port B Pin 6 Alternate
                                                // Mapping
#define GPIO_APSEL_PB_APSEL7         0x00000080 // Port B Pin 7 Alternate
                                                // Mapping

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_APSEL register for
// port C.
//*****************************************************************************
#define GPIO_APSEL_PC_APSEL0         0x00000001 // Port C Pin 0 Alternate
                                                // Mapping
#define GPIO_APSEL_PC_APSEL1         0x00000002 // Port C Pin 1 Alternate
                                                // Mapping
#define GPIO_APSEL_PC_APSEL2         0x00000004 // Port C Pin 2 Alternate
                                                // Mapping
#define GPIO_APSEL_PC_APSEL3         0x00000008 // Port C Pin 3 Alternate
                                                // Mapping
#define GPIO_APSEL_PC_APSEL4         0x00000010 // Port C Pin 4 Alternate
                                                // Mapping
#define GPIO_APSEL_PC_APSEL5         0x00000020 // Port C Pin 5 Alternate
                                                // Mapping
#define GPIO_APSEL_PC_APSEL6         0x00000040 // Port C Pin 6 Alternate
                                                // Mapping
#define GPIO_APSEL_PC_APSEL7         0x00000080 // Port C Pin 7 Alternate
                                                // Mapping

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_APSEL register for
// port D.
//*****************************************************************************
#define GPIO_APSEL_PD_APSEL0         0x00000001 // Port D Pin 0 Alternate
                                                // Mapping
#define GPIO_APSEL_PD_APSEL1         0x00000002 // Port D Pin 1 Alternate
                                                // Mapping
#define GPIO_APSEL_PD_APSEL2         0x00000004 // Port D Pin 2 Alternate
                                                // Mapping
#define GPIO_APSEL_PD_APSEL3         0x00000008 // Port D Pin 3 Alternate
                                                // Mapping
#define GPIO_APSEL_PD_APSEL4         0x00000010 // Port D Pin 4 Alternate
                                                // Mapping
#define GPIO_APSEL_PD_APSEL5         0x00000020 // Port D Pin 5 Alternate
                                                // Mapping
#define GPIO_APSEL_PD_APSEL6         0x00000040 // Port D Pin 6 Alternate
                                                // Mapping
#define GPIO_APSEL_PD_APSEL7         0x00000080 // Port D Pin 7 Alternate
                                                // Mapping

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_APSEL register for
// port E.
//*****************************************************************************
#define GPIO_APSEL_PE_APSEL0         0x00000001 // Port E Pin 0 Alternate
                                                // Mapping
#define GPIO_APSEL_PE_APSEL1         0x00000002 // Port E Pin 1 Alternate
                                                // Mapping
#define GPIO_APSEL_PE_APSEL2         0x00000004 // Port E Pin 2 Alternate
                                                // Mapping
#define GPIO_APSEL_PE_APSEL3         0x00000008 // Port E Pin 3 Alternate
                                                // Mapping
#define GPIO_APSEL_PE_APSEL4         0x00000010 // Port E Pin 4 Alternate
                                                // Mapping
#define GPIO_APSEL_PE_APSEL5         0x00000020 // Port E Pin 5 Alternate
                                                // Mapping
#define GPIO_APSEL_PE_APSEL6         0x00000040 // Port E Pin 6 Alternate
                                                // Mapping
#define GPIO_APSEL_PE_APSEL7         0x00000080 // Port E Pin 7 Alternate
                                                // Mapping

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_APSEL register for
// port F.
//*****************************************************************************
#define GPIO_APSEL_PF_APSEL0         0x00000001 // Port F Pin 0 Alternate
                                                // Mapping
#define GPIO_APSEL_PF_APSEL1         0x00000002 // Port F Pin 1 Alternate
                                                // Mapping
#define GPIO_APSEL_PF_APSEL2         0x00000004 // Port F Pin 2 Alternate
                                                // Mapping
#define GPIO_APSEL_PF_APSEL3         0x00000008 // Port F Pin 3 Alternate
                                                // Mapping
#define GPIO_APSEL_PF_APSEL4         0x00000010 // Port F Pin 4 Alternate
                                                // Mapping
#define GPIO_APSEL_PF_APSEL5         0x00000020 // Port F Pin 5 Alternate
                                                // Mapping
#define GPIO_APSEL_PF_APSEL6         0x00000040 // Port F Pin 6 Alternate
                                                // Mapping
#define GPIO_APSEL_PF_APSEL7         0x00000080 // Port F Pin 7 Alternate
                                                // Mapping

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_APSEL register for
// port G.
//*****************************************************************************
#define GPIO_APSEL_PG_APSEL0         0x00000001 // Port G Pin 0 Alternate
                                                // Mapping
#define GPIO_APSEL_PG_APSEL1         0x00000002 // Port G Pin 1 Alternate
                                                // Mapping
#define GPIO_APSEL_PG_APSEL2         0x00000004 // Port G Pin 2 Alternate
                                                // Mapping
#define GPIO_APSEL_PG_APSEL3         0x00000008 // Port G Pin 3 Alternate
                                                // Mapping
#define GPIO_APSEL_PG_APSEL4         0x00000010 // Port G Pin 4 Alternate
                                                // Mapping
#define GPIO_APSEL_PG_APSEL5         0x00000020 // Port G Pin 5 Alternate
                                                // Mapping
#define GPIO_APSEL_PG_APSEL6         0x00000040 // Port G Pin 6 Alternate
                                                // Mapping
#define GPIO_APSEL_PG_APSEL7         0x00000080 // Port G Pin 7 Alternate
                                                // Mapping

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_APSEL register for
// port H.
//*****************************************************************************
#define GPIO_APSEL_PH_APSEL0         0x00000001 // Port H Pin 0 Alternate
                                                // Mapping
#define GPIO_APSEL_PH_APSEL1         0x00000002 // Port H Pin 1 Alternate
                                                // Mapping
#define GPIO_APSEL_PH_APSEL2         0x00000004 // Port H Pin 2 Alternate
                                                // Mapping
#define GPIO_APSEL_PH_APSEL3         0x00000008 // Port H Pin 3 Alternate
                                                // Mapping
#define GPIO_APSEL_PH_APSEL4         0x00000010 // Port H Pin 4 Alternate
                                                // Mapping
#define GPIO_APSEL_PH_APSEL5         0x00000020 // Port H Pin 5 Alternate
                                                // Mapping
#define GPIO_APSEL_PH_APSEL6         0x00000040 // Port H Pin 6 Alternate
                                                // Mapping
#define GPIO_APSEL_PH_APSEL7         0x00000080 // Port H Pin 7 Alternate
                                                // Mapping

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_APSEL register for
// port J.
//*****************************************************************************
#define GPIO_APSEL_PJ_APSEL0         0x00000001 // Port J Pin 0 Alternate
                                                // Mapping
#define GPIO_APSEL_PJ_APSEL1         0x00000002 // Port J Pin 1 Alternate
                                                // Mapping
#define GPIO_APSEL_PJ_APSEL2         0x00000004 // Port J Pin 2 Alternate
                                                // Mapping
#define GPIO_APSEL_PJ_APSEL3         0x00000008 // Port J Pin 3 Alternate
                                                // Mapping
#define GPIO_APSEL_PJ_APSEL4         0x00000010 // Port J Pin 4 Alternate
                                                // Mapping
#define GPIO_APSEL_PJ_APSEL5         0x00000020 // Port J Pin 5 Alternate
                                                // Mapping
#define GPIO_APSEL_PJ_APSEL6         0x00000040 // Port J Pin 6 Alternate
                                                // Mapping
#define GPIO_APSEL_PJ_APSEL7         0x00000080 // Port J Pin 7 Alternate
                                                // Mapping

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_CSEL register for
// port A.
//*****************************************************************************
#define GPIO_CSEL_PA_CSEL0           0x00000001 // Port A Pin 0 Core Select
#define GPIO_CSEL_PA_CSEL1           0x00000002 // Port A Pin 1 Core Select
#define GPIO_CSEL_PA_CSEL2           0x00000004 // Port A Pin 2 Core Select
#define GPIO_CSEL_PA_CSEL3           0x00000008 // Port A Pin 3 Core Select
#define GPIO_CSEL_PA_CSEL4           0x00000010 // Port A Pin 4 Core Select
#define GPIO_CSEL_PA_CSEL5           0x00000020 // Port A Pin 5 Core Select
#define GPIO_CSEL_PA_CSEL6           0x00000040 // Port A Pin 6 Core Select
#define GPIO_CSEL_PA_CSEL7           0x00000080 // Port A Pin 7 Core Select

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_CSEL register for
// port B.
//*****************************************************************************
#define GPIO_CSEL_PB_CSEL0           0x00000001 // Port B Pin 0 Core Select
#define GPIO_CSEL_PB_CSEL1           0x00000002 // Port B Pin 1 Core Select
#define GPIO_CSEL_PB_CSEL2           0x00000004 // Port B Pin 2 Core Select
#define GPIO_CSEL_PB_CSEL3           0x00000008 // Port B Pin 3 Core Select
#define GPIO_CSEL_PB_CSEL4           0x00000010 // Port B Pin 4 Core Select
#define GPIO_CSEL_PB_CSEL5           0x00000020 // Port B Pin 5 Core Select
#define GPIO_CSEL_PB_CSEL6           0x00000040 // Port B Pin 6 Core Select
#define GPIO_CSEL_PB_CSEL7           0x00000080 // Port B Pin 7 Core Select

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_CSEL register for
// port C.
//*****************************************************************************
#define GPIO_CSEL_PC_CSEL0           0x00000001 // Port C Pin 0 Core Select
#define GPIO_CSEL_PC_CSEL1           0x00000002 // Port C Pin 1 Core Select
#define GPIO_CSEL_PC_CSEL2           0x00000004 // Port C Pin 2 Core Select
#define GPIO_CSEL_PC_CSEL3           0x00000008 // Port C Pin 3 Core Select
#define GPIO_CSEL_PC_CSEL4           0x00000010 // Port C Pin 4 Core Select
#define GPIO_CSEL_PC_CSEL5           0x00000020 // Port C Pin 5 Core Select
#define GPIO_CSEL_PC_CSEL6           0x00000040 // Port C Pin 6 Core Select
#define GPIO_CSEL_PC_CSEL7           0x00000080 // Port C Pin 7 Core Select

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_CSEL register for
// port D.
//*****************************************************************************
#define GPIO_CSEL_PD_CSEL0           0x00000001 // Port D Pin 0 Core Select
#define GPIO_CSEL_PD_CSEL1           0x00000002 // Port D Pin 1 Core Select
#define GPIO_CSEL_PD_CSEL2           0x00000004 // Port D Pin 2 Core Select
#define GPIO_CSEL_PD_CSEL3           0x00000008 // Port D Pin 3 Core Select
#define GPIO_CSEL_PD_CSEL4           0x00000010 // Port D Pin 4 Core Select
#define GPIO_CSEL_PD_CSEL5           0x00000020 // Port D Pin 5 Core Select
#define GPIO_CSEL_PD_CSEL6           0x00000040 // Port D Pin 6 Core Select
#define GPIO_CSEL_PD_CSEL7           0x00000080 // Port D Pin 7 Core Select

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_CSEL register for
// port E.
//*****************************************************************************
#define GPIO_CSEL_PE_CSEL0           0x00000001 // Port E Pin 0 Core Select
#define GPIO_CSEL_PE_CSEL1           0x00000002 // Port E Pin 1 Core Select
#define GPIO_CSEL_PE_CSEL2           0x00000004 // Port E Pin 2 Core Select
#define GPIO_CSEL_PE_CSEL3           0x00000008 // Port E Pin 3 Core Select
#define GPIO_CSEL_PE_CSEL4           0x00000010 // Port E Pin 4 Core Select
#define GPIO_CSEL_PE_CSEL5           0x00000020 // Port E Pin 5 Core Select
#define GPIO_CSEL_PE_CSEL6           0x00000040 // Port E Pin 6 Core Select
#define GPIO_CSEL_PE_CSEL7           0x00000080 // Port E Pin 7 Core Select

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_CSEL register for
// port F.
//*****************************************************************************
#define GPIO_CSEL_PF_CSEL0           0x00000001 // Port F Pin 0 Core Select
#define GPIO_CSEL_PF_CSEL1           0x00000002 // Port F Pin 1 Core Select
#define GPIO_CSEL_PF_CSEL2           0x00000004 // Port F Pin 2 Core Select
#define GPIO_CSEL_PF_CSEL3           0x00000008 // Port F Pin 3 Core Select
#define GPIO_CSEL_PF_CSEL4           0x00000010 // Port F Pin 4 Core Select
#define GPIO_CSEL_PF_CSEL5           0x00000020 // Port F Pin 5 Core Select
#define GPIO_CSEL_PF_CSEL6           0x00000040 // Port F Pin 6 Core Select
#define GPIO_CSEL_PF_CSEL7           0x00000080 // Port F Pin 7 Core Select

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_CSEL register for
// port G.
//*****************************************************************************
#define GPIO_CSEL_PG_CSEL0           0x00000001 // Port G Pin 0 Core Select
#define GPIO_CSEL_PG_CSEL1           0x00000002 // Port G Pin 1 Core Select
#define GPIO_CSEL_PG_CSEL2           0x00000004 // Port G Pin 2 Core Select
#define GPIO_CSEL_PG_CSEL3           0x00000008 // Port G Pin 3 Core Select
#define GPIO_CSEL_PG_CSEL4           0x00000010 // Port G Pin 4 Core Select
#define GPIO_CSEL_PG_CSEL5           0x00000020 // Port G Pin 5 Core Select
#define GPIO_CSEL_PG_CSEL6           0x00000040 // Port G Pin 6 Core Select
#define GPIO_CSEL_PG_CSEL7           0x00000080 // Port G Pin 7 Core Select

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_CSEL register for
// port H.
//*****************************************************************************
#define GPIO_CSEL_PH_CSEL0           0x00000001 // Port H Pin 0 Core Select
#define GPIO_CSEL_PH_CSEL1           0x00000002 // Port H Pin 1 Core Select
#define GPIO_CSEL_PH_CSEL2           0x00000004 // Port H Pin 2 Core Select
#define GPIO_CSEL_PH_CSEL3           0x00000008 // Port H Pin 3 Core Select
#define GPIO_CSEL_PH_CSEL4           0x00000010 // Port H Pin 4 Core Select
#define GPIO_CSEL_PH_CSEL5           0x00000020 // Port H Pin 5 Core Select
#define GPIO_CSEL_PH_CSEL6           0x00000040 // Port H Pin 6 Core Select
#define GPIO_CSEL_PH_CSEL7           0x00000080 // Port H Pin 7 Core Select

//*****************************************************************************
// The following are defines for the bit fields in the GPIO_CSEL register for
// port J.
//*****************************************************************************
#define GPIO_CSEL_PJ_CSEL0           0x00000001 // Port J Pin 0 Core Select
#define GPIO_CSEL_PJ_CSEL1           0x00000002 // Port J Pin 1 Core Select
#define GPIO_CSEL_PJ_CSEL2           0x00000004 // Port J Pin 2 Core Select
#define GPIO_CSEL_PJ_CSEL3           0x00000008 // Port J Pin 3 Core Select
#define GPIO_CSEL_PJ_CSEL4           0x00000010 // Port J Pin 4 Core Select
#define GPIO_CSEL_PJ_CSEL5           0x00000020 // Port J Pin 5 Core Select
#define GPIO_CSEL_PJ_CSEL6           0x00000040 // Port J Pin 6 Core Select
#define GPIO_CSEL_PJ_CSEL7           0x00000080 // Port J Pin 7 Core Select


#endif // __HW_GPIO_H__



