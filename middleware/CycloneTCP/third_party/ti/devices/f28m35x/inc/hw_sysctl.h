//###########################################################################
// FILE:   hw_sysctl.h
// TITLE:  Macros used when accessing the system control hardware.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_SYSCTL_H__
#define __HW_SYSCTL_H__

//*****************************************************************************
// The following are defines for the System Control Registers addresses.
//*****************************************************************************
#define SYSCTL_SYSPLLCTL                    0x400FB800 // Device PLL
                                                       // Configuration
#define SYSCTL_SYSPLLMULT                   0x400FB804 // Device PLL Multiplier
#define SYSCTL_SYSPLLLOCKS                  0x400FB808 // Device PLL Lock
                                                       // Status
#define SYSCTL_SYSDIVSEL                    0x400FB80C // Device Clock Divider
#define SYSCTL_M3SSDIVSEL                   0x400FB810 // M3 Sub System Clock
                                                       // Divider
#define SYSCTL_CCLKCTL                      0x400FB814 // Analog System Clock
                                                       // Divider
#define SYSCTL_UPLLCTL                      0x400FB820 // USB PLL Configuration
#define SYSCTL_UPLLMULT                     0x400FB824 // USB PLL Multiplier
#define SYSCTL_UPLLSTS                      0x400FB828 // USB PLL Lock Status
#define SYSCTL_MCLKSTS                      0x400FB830 // Missing Clock Status
#define SYSCTL_MCLKFRCCLR                   0x400FB838 // Missing Clock Force
#define SYSCTL_MCLKEN                       0x400FB83C // Missing Clock Enable
#define SYSCTL_MREFCLKLIM                   0x400FB840 // Missing Clock
                                                       // Reference Limit
#define SYSCTL_XPLLCLKCFG                   0x400FB850 // XPLL CLK Out Control
#define SYSCTL_CCLKOFF                      0x400FB860 // C28 Clock Disable
#define SYSCTL_CAN0BCLKSEL                  0x400FB870 // CAN0 Bit Clock Source
                                                       // Selection
#define SYSCTL_CAN1BCLKSEL                  0x400FB874 // CAN1 Bit Clock Source
                                                       // Selection
#define SYSCTL_CLPMSTAT                     0x400FB880 // C28 Low Power Mode
                                                       // Status
#define SYSCTL_CRESCNF                      0x400FB8C0 // C28 Reset
                                                       // Configuration
#define SYSCTL_CRESSTS                      0x400FB8C4 // C28 Reset Status
#define SYSCTL_FUSEERR                      0x400FB8C8 // Efuse Error Status
#define SYSCTL_MWIR                         0x400FB8CC // WIR Decode Latch
                                                       // Register
#define SYSCTL_MCNF                         0x400FB900 // M3 Sub System
                                                       // Configuration
#define SYSCTL_SERPLOOP                     0x400FB908 // Serial Port Loop Back
                                                       // Control
#define SYSCTL_MCIBSTATUS                   0x400FB90C // Analog System
                                                       // Status Register
#define SYSCTL_CCNF0                        0x400FB910 // C28 Peripheral
                                                       // Configuration 0
#define SYSCTL_CCNF1                        0x400FB914 // C28 Peripheral
                                                       // Configuration 1
#define SYSCTL_CCNF2                        0x400FB918 // C28 Peripheral
                                                       // Configuration 2
#define SYSCTL_CCNF3                        0x400FB91C // C28 Peripheral
                                                       // Configuration 3
#define SYSCTL_CCNF4                        0x400FB920 // C28 Peripheral
                                                       // Configuration 4
#define SYSCTL_MEMCNF                       0x400FB930 // M3 Memory
                                                       // Configuration
#define SYSCTL_MWRALLOW                     0x400FB980 // M3 Configuration
                                                       // Write Allow
#define SYSCTL_MLOCK                        0x400FB984 // M3 Configuration Lock
                                                       // Register

//*****************************************************************************
// The following are defines for the System Control register addresses.
//*****************************************************************************
#define SYSCTL_DID0                         0x400FE000 // Device Identification
                                                       // 0
#define SYSCTL_DID1                         0x400FE004 // Device Identification
                                                       // 1
#define SYSCTL_DC1                          0x400FE010 // Device Capabilities 1
#define SYSCTL_DC2                          0x400FE014 // Device Capabilities 2
#define SYSCTL_DC4                          0x400FE01C // Device Capabilities 4
#define SYSCTL_DC6                          0x400FE024 // Device Capabilities 6
#define SYSCTL_DC7                          0x400FE028 // Device Capabilities 7
#define SYSCTL_SRCR0                        0x400FE040 // Software Reset
                                                       // Control 0
#define SYSCTL_SRCR1                        0x400FE044 // Software Reset
                                                       // Control 1
#define SYSCTL_SRCR2                        0x400FE048 // Software Reset
                                                       // Control 2
#define SYSCTL_SRCR3                        0x400FE04C // Software Reset
                                                       // Control 3
#define SYSCTL_RESC                         0x400FE05C // Reset Cause
#define SYSCTL_RCC                          0x400FE060 // Run Mode Clock
#define SYSCTL_GPIOHBCTL                    0x400FE06C // GPIO High Performance
                                                       // Bus Control
#define SYSCTL_RCGC0                        0x400FE100 // Run Mode Clock Gating
                                                       // Control - Register 0
#define SYSCTL_RCGC1                        0x400FE104 // Run Mode Clock Gating
                                                       // Control - Register 1
#define SYSCTL_RCGC2                        0x400FE108 // Run Mode Clock Gating
                                                       // Control - Register 2
#define SYSCTL_RCGC3                        0x400FE10C // Run Mode Clock Gating
                                                       // Control - Register 3
#define SYSCTL_SCGC0                        0x400FE110 // Sleep Mode Clock
                                                       // Gating Control -
                                                       // Register 0
#define SYSCTL_SCGC1                        0x400FE114 // Sleep Mode Clock
                                                       // Gating Control -
                                                       // Register 1
#define SYSCTL_SCGC2                        0x400FE118 // Sleep Mode Clock
                                                       // Gating Control -
                                                       // Register 2
#define SYSCTL_SCGC3                        0x400FE11C // Sleep Mode Clock
                                                       // Gating Control -
                                                       // Register 3
#define SYSCTL_DCGC0                        0x400FE120 // Deep Sleep Mode Clock
                                                       // Gating - Control
                                                       // Register 0
#define SYSCTL_DCGC1                        0x400FE124 // Deep Sleep Mode Clock
                                                       // Gating - Control
                                                       // Register 1
#define SYSCTL_DCGC2                        0x400FE128 // Deep Sleep Mode Clock
                                                       // Gating - Control
                                                       // Register 2
#define SYSCTL_DCGC3                        0x400FE12C // Deep Sleep Mode Clock
                                                       // Gating - Control
                                                       // Register 3
#define SYSCTL_DSLPCLKCFG                   0x400FE144 // Deep Sleep Clock
                                                       // Configuration
#define SYSCTL_DC10                         0x400FE194 // Device Configuration
                                                       // 10
#define SYSCTL_NVMSTAT                      0x400FE1A0 // Non-Volatile Memory
                                                       // Status
#define SYSCTL_PPGPIO                       0x400FE308 // GPIO Peripheral
                                                       // Present
#define SYSCTL_SRGPIO                       0x400FE508 // GPIO Software Reset
#define SYSCTL_RCGCGPIO                     0x400FE608 // GPIO Run Mode Clock
                                                       // Gating Control
#define SYSCTL_SCGCGPIO                     0x400FE708 // GPIO Sleep Mode Clock
                                                       // Gating Control
#define SYSCTL_DCGCGPIO                     0x400FE808 // GPIO Deep-Sleep Mode
                                                       // Clock Gating Control

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_DID0 register.
//*****************************************************************************
#define SYSCTL_DID0_REVID_S                 0
#define SYSCTL_DID0_REVID_M                 0x0000FFFF // REVID
#define SYSCTL_DID0_CLASS_S                 16
#define SYSCTL_DID0_CLASS_M                 0x00FF0000 // Device Class
#define SYSCTL_DID0_VER_S                   28
#define SYSCTL_DID0_VER_M                   0x70000000 // DID0 Version

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_DID1 register.
//*****************************************************************************
#define SYSCTL_DID1_QUAL_S                  0
#define SYSCTL_DID1_QUAL_M                  0x00000003 // Qual Status
#define SYSCTL_DID1_ROHS                    0x00000004 // ROHS Compliance
#define SYSCTL_DID1_PACKAGE_S               3
#define SYSCTL_DID1_PACKAGE_M               0x00000018 // Package Type
#define SYSCTL_DID1_TEMP_S                  5
#define SYSCTL_DID1_TEMP_M                  0x000000E0 // Temperature Range
#define SYSCTL_DID1_PINCOUNT_S              13
#define SYSCTL_DID1_PINCOUNT_M              0x0000E000 // Pin Count
#define SYSCTL_DID1_PARTNO_S                16
#define SYSCTL_DID1_PARTNO_M                0x00FF0000 // Part Number
#define SYSCTL_DID1_FAM_S                   24
#define SYSCTL_DID1_FAM_M                   0x0F000000 // Device Family
#define SYSCTL_DID1_VER_S                   28
#define SYSCTL_DID1_VER_M                   0xF0000000 // DID1 Version


//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_DC1 register.
//*****************************************************************************
#define SYSCTL_DC1_WDT1                     0x10000000 // Watchdog Timer1
                                                       // Present
#define SYSCTL_DC1_PLL                      0x00000010 // PLL Present
#define SYSCTL_DC1_WDT0                     0x00000008 // Watchdog Timer 0
                                                       // Present
#define SYSCTL_DC1_SWO                      0x00000004 // SWO Trace Port
                                                       // Present
#define SYSCTL_DC1_SWD                      0x00000002 // SWD Present
#define SYSCTL_DC1_JTAG                     0x00000001 // JTAG Present

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_DC2 register.
//*****************************************************************************
#define SYSCTL_DC2_EPI0                     0x40000000 // EPI Module 0 Present
#define SYSCTL_DC2_TIMER3                   0x00080000 // Timer Module 3
                                                       // Present
#define SYSCTL_DC2_TIMER2                   0x00040000 // Timer Module 2
                                                       // Present
#define SYSCTL_DC2_TIMER1                   0x00020000 // Timer Module 1
                                                       // Present
#define SYSCTL_DC2_TIMER0                   0x00010000 // Timer Module 0
                                                       // Present
#define SYSCTL_DC2_I2C1                     0x00004000 // I2C Module 1 Present
#define SYSCTL_DC2_I2C0                     0x00001000 // I2C Module 0 Present
#define SYSCTL_DC2_SSI3                     0x00000080 // SSI Module 3 Present
#define SYSCTL_DC2_SSI2                     0x00000040 // SSI Module 2 Present
#define SYSCTL_DC2_SSI1                     0x00000020 // SSI Module 1 Present
#define SYSCTL_DC2_SSI0                     0x00000010 // SSI Module 0 Present
#define SYSCTL_DC2_UART3                    0x00000008 // UART Module 3 Present
#define SYSCTL_DC2_UART2                    0x00000004 // UART Module 2 Present
#define SYSCTL_DC2_UART1                    0x00000002 // UART Module 1 Present
#define SYSCTL_DC2_UART0                    0x00000001 // UART Module 0 Present

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_DC4 register.
//*****************************************************************************
#define SYSCTL_DC4_EMAC0                    0x10000000 // Ethernet MAC Layer 0
                                                       // Present
#define SYSCTL_DC4_E1588                    0x01000000 // 1588 Capable
#define SYSCTL_DC4_USB0                     0x00010000 // USB0 Software Enable
                                                       // Bit
#define SYSCTL_DC4_UDMA                     0x00002000 // Micro-DMA Module
                                                       // Present
#define SYSCTL_DC4_ROM                      0x00001000 // Internal Code ROM
                                                       // Present
#define SYSCTL_DC4_GPIOJ                    0x00000100 // GPIO Port J Present
#define SYSCTL_DC4_GPIOH                    0x00000080 // GPIO Port H Present
#define SYSCTL_DC4_GPIOG                    0x00000040 // GPIO Port G Present
#define SYSCTL_DC4_GPIOF                    0x00000020 // GPIO Port F Present
#define SYSCTL_DC4_GPIOE                    0x00000010 // GPIO Port E Present
#define SYSCTL_DC4_GPIOD                    0x00000008 // GPIO Port D Present
#define SYSCTL_DC4_GPIOC                    0x00000004 // GPIO Port C Present
#define SYSCTL_DC4_GPIOB                    0x00000002 // GPIO Port B Present
#define SYSCTL_DC4_GPIOA                    0x00000001 // GPIO Port A Present

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_DC6 register.
//*****************************************************************************
#define SYSCTL_DC6_USB0PHY                  0x00000010 // USB Module 0 PHY
                                                       // Present
#define SYSCTL_DC6_USB0_M                   0x00000003 // USB Module 0 Present
#define SYSCTL_DC6_USB0_DEV                 0x00000001 // USB0 is Device Only
#define SYSCTL_DC6_USB0_HOSTDEV             0x00000002 // USB is Device or Host
#define SYSCTL_DC6_USB0_OTG                 0x00000003 // USB0 is OTG

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_DC10 register.
//*****************************************************************************
#define SYSCTL_DC10_CAN1                    0x02000000 // CAN1 is present
#define SYSCTL_DC10_CAN0                    0x01000000 // CAN0 is present
#define SYSCTL_DC10_UART4                   0x00000001 // UART4 is present

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_SRCR0 register.
//*****************************************************************************
#define SYSCTL_SRCR0_WDT1                   0x10000000 // WDT1 Reset Control
#define SYSCTL_SRCR0_WDT0                   0x00000008 // WDT0 Reset Control

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_SRCR1 register.
//*****************************************************************************
#define SYSCTL_SRCR1_EPI0                   0x40000000 // EPI0 Reset Control
#define SYSCTL_SRCR1_TIMER3                 0x00080000 // Timer 3 Reset Control
#define SYSCTL_SRCR1_TIMER2                 0x00040000 // Timer 2 Reset Control
#define SYSCTL_SRCR1_TIMER1                 0x00020000 // Timer 1 Reset Control
#define SYSCTL_SRCR1_TIMER0                 0x00010000 // Timer 0 Reset Control
#define SYSCTL_SRCR1_I2C1                   0x00004000 // I2C1 Reset Control
#define SYSCTL_SRCR1_I2C0                   0x00001000 // I2C0 Reset Control
#define SYSCTL_SRCR1_SSI3                   0x00000080 // SSI3 Reset Control
#define SYSCTL_SRCR1_SSI2                   0x00000040 // SSI2 Reset Control
#define SYSCTL_SRCR1_SSI1                   0x00000020 // SSI1 Reset Control
#define SYSCTL_SRCR1_SSI0                   0x00000010 // SSI0 Reset Control
#define SYSCTL_SRCR1_UART3                  0x00000008 // UART3 Reset Control
#define SYSCTL_SRCR1_UART2                  0x00000004 // UART2 Reset Control
#define SYSCTL_SRCR1_UART1                  0x00000002 // UART1 Reset Control
#define SYSCTL_SRCR1_UART0                  0x00000001 // UART0 Reset Control

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_SRCR2 register.
//*****************************************************************************
#define SYSCTL_SRCR2_EMAC0                  0x10000000 // MAC0 Reset Control
#define SYSCTL_SRCR2_USB0                   0x00010000 // USB0 Reset Control
#define SYSCTL_SRCR2_UDMA                   0x00002000 // Micro-DMA Reset
                                                       // Control
#define SYSCTL_SRCR2_GPIOJ                  0x00000100 // Port J Reset Control
#define SYSCTL_SRCR2_GPIOH                  0x00000080 // Port H Reset Control
#define SYSCTL_SRCR2_GPIOG                  0x00000040 // Port G Reset Control
#define SYSCTL_SRCR2_GPIOF                  0x00000020 // Port F Reset Control
#define SYSCTL_SRCR2_GPIOE                  0x00000010 // Port E Reset Control
#define SYSCTL_SRCR2_GPIOD                  0x00000008 // Port D Reset Control
#define SYSCTL_SRCR2_GPIOC                  0x00000004 // Port C Reset Control
#define SYSCTL_SRCR2_GPIOB                  0x00000002 // Port B Reset Control
#define SYSCTL_SRCR2_GPIOA                  0x00000001 // Port A Reset Control

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_SRCR3 register.
//*****************************************************************************
#define SYSCTL_SRCR3_CAN0                   0x02000000 // CAN0 Reset Control
#define SYSCTL_SRCR3_CAN1                   0x01000000 // CAN1 Reset Control
#define SYSCTL_SRCR3_UART4                  0x00000001 // UART4 Reset Control

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_RESC register.
//*****************************************************************************
#define SYSCTL_RESC_AERRNMI                 0x80000000 // Analog Error
                                                       // Unserviced
#define SYSCTL_RESC_C28NMIWDRST             0x40000000 // C28 NMI WD Reset NMI
                                                       // Unserviced
#define SYSCTL_RESC_M3BISTERRNMI            0x20000000 // M3 HW BIST Error NMI
                                                       // Unserviced
#define SYSCTL_RESC_C28BISTERR              0x10000000 // C28 HW BIST Error NMI
                                                       // Unserviced
#define SYSCTL_RESC_C28PIENMI               0x08000000 // C28 PIE NMI Error
                                                       // Unserviced
#define SYSCTL_RESC_HWBIST_S                25
#define SYSCTL_RESC_HWBIST_M                0x06000000 // M3 HWBIST Reset
#define SYSCTL_RESC_EXTGPIO                 0x01000000 // External GPIO NMI
                                                       // Unserviced
#define SYSCTL_RESC_MCLKNMI                 0x00010000 // Missing Clock
                                                       // Condition NMI
                                                       // Unserviced
#define SYSCTL_RESC_WDT1                    0x00000020 // Watchdog Timer 1
                                                       // Reset
#define SYSCTL_RESC_SW                      0x00000010 // Software Reset
#define SYSCTL_RESC_WDT0                    0x00000008 // Watchdog Timer 0
                                                       // Reset
#define SYSCTL_RESC_POR                     0x00000002 // Power-On Reset
#define SYSCTL_RESC_XRSn                    0x00000001 // External Reset

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_RCC register.
//*****************************************************************************
#define SYSCTL_RCC_ACG                      0x08000000 // Auto Clock Gating

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_GPIOHBCTL
// register.
//*****************************************************************************
#define SYSCTL_GPIOHBCTL_PORTJ              0x00000100 // Port J - AHB or APB
                                                       // Bus
#define SYSCTL_GPIOHBCTL_PORTH              0x00000080 // Port H - AHB or APB
                                                       // Bus
#define SYSCTL_GPIOHBCTL_PORTG              0x00000040 // Port G - AHB or APB
                                                       // Bus
#define SYSCTL_GPIOHBCTL_PORTF              0x00000020 // Port F - AHB or APB
                                                       // Bus
#define SYSCTL_GPIOHBCTL_PORTE              0x00000010 // Port E - AHB or APB
                                                       // Bus
#define SYSCTL_GPIOHBCTL_PORTD              0x00000008 // Port D - AHB or APB
                                                       // Bus
#define SYSCTL_GPIOHBCTL_PORTC              0x00000004 // Port C - AHB or APB
                                                       // Bus
#define SYSCTL_GPIOHBCTL_PORTB              0x00000002 // Port B - AHB or APB
                                                       // Bus
#define SYSCTL_GPIOHBCTL_PORTA              0x00000001 // Port A - AHB or APB
                                                       // Bus

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_RCGC0 register.
//*****************************************************************************
#define SYSCTL_RCGC0_WDT1                   0x10000000 // WDT1 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC0_WDT0                   0x00000008 // WDT0 Clock Gating
                                                       // Control Run Mode

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_RCGC1 register.
//*****************************************************************************
#define SYSCTL_RCGC1_EPI0                   0x40000000 // EPI0 Clock Gating Run
                                                       // Mode
#define SYSCTL_RCGC1_TIMER3                 0x00080000 // Timer 3 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC1_TIMER2                 0x00040000 // Timer 2 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC1_TIMER1                 0x00020000 // Timer 1 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC1_TIMER0                 0x00010000 // Timer 0 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC1_I2C1                   0x00004000 // I2C1 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC1_I2C0                   0x00001000 // I2C0 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC1_SSI3                   0x00000080 // SSI3 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC1_SSI2                   0x00000040 // SSI2 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC1_SSI1                   0x00000020 // SSI1 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC1_SSI0                   0x00000010 // SSI0 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC1_UART3                  0x00000008 // UART3 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC1_UART2                  0x00000004 // UART2 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC1_UART1                  0x00000002 // UART1 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC1_UART0                  0x00000001 // UART0 Clock Gating
                                                       // Control Run Mode

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_RCGC2 register.
//*****************************************************************************
#define SYSCTL_RCGC2_EMAC0                  0x10000000 // MAC0 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC2_USB0                   0x00010000 // USB0 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC2_UDMA                   0x00002000 // Micro-DMA Clock
                                                       // Gating Control Run
                                                       // Mode
#define SYSCTL_RCGC2_GPIOJ                  0x00000100 // Port J Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC2_GPIOH                  0x00000080 // Port H Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC2_GPIOG                  0x00000040 // Port G Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC2_GPIOF                  0x00000020 // Port F Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC2_GPIOE                  0x00000010 // Port E Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC2_GPIOD                  0x00000008 // Port D Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC2_GPIOC                  0x00000004 // Port C Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC2_GPIOB                  0x00000002 // Port B Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC2_GPIOA                  0x00000001 // Port A Clock Gating
                                                       // Control Run Mode

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_RCGC3
// register.
//*****************************************************************************
#define SYSCTL_RCGC3_CAN1                   0x02000000 // CAN1 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC3_CAN0                   0x01000000 // CAN0 Clock Gating
                                                       // Control Run Mode
#define SYSCTL_RCGC3_UART4                  0x00000001 // UART4 Clock Gating
                                                       // Control Run Mode

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_SCGC0 register.
//*****************************************************************************
#define SYSCTL_SCGC0_WDT1                   0x10000000 // WDT1 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC0_WDT0                   0x00000008 // WDT0 Clock Gating
                                                       // Control Sleep Mode

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_SCGC1 register.
//*****************************************************************************
#define SYSCTL_SCGC1_EPI0                   0x40000000 // EPI0 Clock Gating
                                                       // Sleep Mode
#define SYSCTL_SCGC1_TIMER3                 0x00080000 // Timer 3 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC1_TIMER2                 0x00040000 // Timer 2 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC1_TIMER1                 0x00020000 // Timer 1 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC1_TIMER0                 0x00010000 // Timer 0 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC1_I2C1                   0x00004000 // I2C1 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC1_I2C0                   0x00001000 // I2C0 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC1_SSI3                   0x00000080 // SSI3 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC1_SSI2                   0x00000040 // SSI2 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC1_SSI1                   0x00000020 // SSI1 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC1_SSI0                   0x00000010 // SSI0 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC1_UART3                  0x00000008 // UART3 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC1_UART2                  0x00000004 // UART2 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC1_UART1                  0x00000002 // UART1 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC1_UART0                  0x00000001 // UART0 Clock Gating
                                                       // Control Sleep Mode

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_SCGC2 register.
//*****************************************************************************
#define SYSCTL_SCGC2_EMAC0                  0x10000000 // MAC0 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC2_USB0                   0x00010000 // USB0 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC2_UDMA                   0x00002000 // Micro-DMA Clock
                                                       // Gating Control Sleep
                                                       // Mode
#define SYSCTL_SCGC2_GPIOJ                  0x00000100 // Port J Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC2_GPIOH                  0x00000080 // Port H Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC2_GPIOG                  0x00000040 // Port G Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC2_GPIOF                  0x00000020 // Port F Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC2_GPIOE                  0x00000010 // Port E Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC2_GPIOD                  0x00000008 // Port D Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC2_GPIOC                  0x00000004 // Port C Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC2_GPIOB                  0x00000002 // Port B Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC2_GPIOA                  0x00000001 // Port A Clock Gating
                                                       // Control Sleep Mode

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_SCGC3
// register.
//*****************************************************************************
#define SYSCTL_SCGC3_CAN1                   0x02000000 // CAN1 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC3_CAN0                   0x01000000 // CAN0 Clock Gating
                                                       // Control Sleep Mode
#define SYSCTL_SCGC3_UART4                  0x00000001 // UART4 Clock Gating
                                                       // Control Sleep Mode

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_DCGC0 register.
//*****************************************************************************
#define SYSCTL_DCGC0_WDT1                   0x10000000 // WDT1 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC0_WDT0                   0x00000008 // WDT0 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_DCGC1 register.
//*****************************************************************************
#define SYSCTL_DCGC1_EPI0                   0x40000000 // EPI0 Clock Gating
                                                       // Deep Sleep Mode
#define SYSCTL_DCGC1_TIMER3                 0x00080000 // Timer 3 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC1_TIMER2                 0x00040000 // Timer 2 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC1_TIMER1                 0x00020000 // Timer 1 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC1_TIMER0                 0x00010000 // Timer 0 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC1_I2C1                   0x00004000 // I2C1 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC1_I2C0                   0x00001000 // I2C0 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC1_SSI3                   0x00000080 // SSI3 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC1_SSI2                   0x00000040 // SSI2 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC1_SSI1                   0x00000020 // SSI1 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC1_SSI0                   0x00000010 // SSI0 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC1_UART3                  0x00000008 // UART3 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC1_UART2                  0x00000004 // UART2 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC1_UART1                  0x00000002 // UART1 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC1_UART0                  0x00000001 // UART0 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_DCGC2 register.
//*****************************************************************************
#define SYSCTL_DCGC2_EMAC0                  0x10000000 // MAC0 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC2_USB0                   0x00010000 // USB0 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC2_UDMA                   0x00002000 // Micro-DMA Clock
                                                       // Gating Control Deep
                                                       // Sleep Mode
#define SYSCTL_DCGC2_GPIOJ                  0x00000100 // Port J Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC2_GPIOH                  0x00000080 // Port H Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC2_GPIOG                  0x00000040 // Port G Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC2_GPIOF                  0x00000020 // Port F Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC2_GPIOE                  0x00000010 // Port E Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC2_GPIOD                  0x00000008 // Port D Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC2_GPIOC                  0x00000004 // Port C Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC2_GPIOB                  0x00000002 // Port B Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC2_GPIOA                  0x00000001 // Port A Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_DCGC3
// register.
//*****************************************************************************
#define SYSCTL_DCGC3_CAN1                   0x02000000 // CAN1 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC3_CAN0                   0x01000000 // CAN0 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode
#define SYSCTL_DCGC3_UART4                  0x00000001 // UART4 Clock Gating
                                                       // Control Deep Sleep
                                                       // Mode

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_DSLPCLKCFG
// register.
//*****************************************************************************
#define SYSCTL_DSLPCLKCFG_D_M               0x1F800000 // Deep Sleep Divider
                                                       // Field Override
#define SYSCTL_DSLPCLKCFG_D_2               0x00800000 // System clock /2
#define SYSCTL_DSLPCLKCFG_D_3               0x01000000 // System clock /3
#define SYSCTL_DSLPCLKCFG_D_4               0x01800000 // System clock /4
#define SYSCTL_DSLPCLKCFG_D_5               0x02000000 // System clock /5
#define SYSCTL_DSLPCLKCFG_D_6               0x02800000 // System clock /6
#define SYSCTL_DSLPCLKCFG_D_7               0x03000000 // System clock /7
#define SYSCTL_DSLPCLKCFG_D_8               0x03800000 // System clock /8
#define SYSCTL_DSLPCLKCFG_D_9               0x04000000 // System clock /9
#define SYSCTL_DSLPCLKCFG_D_10              0x04800000 // System clock /10
#define SYSCTL_DSLPCLKCFG_D_11              0x05000000 // System clock /11
#define SYSCTL_DSLPCLKCFG_D_12              0x05800000 // System clock /12
#define SYSCTL_DSLPCLKCFG_D_13              0x06000000 // System clock /13
#define SYSCTL_DSLPCLKCFG_D_14              0x06800000 // System clock /14
#define SYSCTL_DSLPCLKCFG_D_15              0x07000000 // System clock /15
#define SYSCTL_DSLPCLKCFG_D_16              0x07800000 // System clock /16
#define SYSCTL_DSLPCLKCFG_D_S               23
#define SYSCTL_DSLPCLKCFG_S_M               0x00000070 // Deep Sleep Mode Clock
                                                       // Source Select
#define SYSCTL_DSLPCLKCFG_S_OSC             0x00000000 // OSCCLK as Clock
                                                       // Source
#define SYSCTL_DSLPCLKCFG_S_32              0x00000010 // 32KHz Clock Source
#define SYSCTL_DSLPCLKCFG_S_10              0x00000020 // 10MHz AtoB Clock
                                                       // Source

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_NVMSTAT register.
//*****************************************************************************
#define SYSCTL_NVMSTAT_TPSW                 0x00000010 // Third Party Software
                                                       // in M3 ROM

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_PPGPIO register.
//*****************************************************************************
#define SYSCTL_PPGPIO_GPIOA                 0x00000001 // GPIOA Present
#define SYSCTL_PPGPIO_GPIOB                 0x00000002 // GPIOB Present
#define SYSCTL_PPGPIO_GPIOC                 0x00000004 // GPIOC Present
#define SYSCTL_PPGPIO_GPIOD                 0x00000008 // GPIOD Present
#define SYSCTL_PPGPIO_GPIOE                 0x00000010 // GPIOE Present
#define SYSCTL_PPGPIO_GPIOF                 0x00000020 // GPIOF Present
#define SYSCTL_PPGPIO_GPIOG                 0x00000040 // GPIOG Present
#define SYSCTL_PPGPIO_GPIOH                 0x00000080 // GPIOH Present
#define SYSCTL_PPGPIO_GPIOJ                 0x00000100 // GPIOJ Present
#define SYSCTL_PPGPIO_GPIOK                 0x00000200 // GPIOK Present
#define SYSCTL_PPGPIO_GPIOL                 0x00000400 // GPIOL Present
#define SYSCTL_PPGPIO_GPIOM                 0x00000800 // GPIOM Present
#define SYSCTL_PPGPIO_GPION                 0x00001000 // GPION Present
#define SYSCTL_PPGPIO_GPIOP                 0x00002000 // GPIOP Present
#define SYSCTL_PPGPIO_GPIOQ                 0x00004000 // GPIOQ Present
#define SYSCTL_PPGPIO_GPIOR                 0x00008000 // GPIOR Present
#define SYSCTL_PPGPIO_GPIOS                 0x00010000 // GPIOS Present

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_SRGPIO register.
//*****************************************************************************
#define SYSCTL_SRGPIO_GPIOA                 0x00000001 // GPIOA SW Reset
#define SYSCTL_SRGPIO_GPIOB                 0x00000002 // GPIOB SW Reset
#define SYSCTL_SRGPIO_GPIOC                 0x00000004 // GPIOC SW Reset
#define SYSCTL_SRGPIO_GPIOD                 0x00000008 // GPIOD SW Reset
#define SYSCTL_SRGPIO_GPIOE                 0x00000010 // GPIOE SW Reset
#define SYSCTL_SRGPIO_GPIOF                 0x00000020 // GPIOF SW Reset
#define SYSCTL_SRGPIO_GPIOG                 0x00000040 // GPIOG SW Reset
#define SYSCTL_SRGPIO_GPIOH                 0x00000080 // GPIOH SW Reset
#define SYSCTL_SRGPIO_GPIOJ                 0x00000100 // GPIOJ SW Reset
#define SYSCTL_SRGPIO_GPIOK                 0x00000200 // GPIOK SW Reset
#define SYSCTL_SRGPIO_GPIOL                 0x00000400 // GPIOL SW Reset
#define SYSCTL_SRGPIO_GPIOM                 0x00000800 // GPIOM SW Reset
#define SYSCTL_SRGPIO_GPION                 0x00001000 // GPION SW Reset
#define SYSCTL_SRGPIO_GPIOP                 0x00002000 // GPIOP SW Reset
#define SYSCTL_SRGPIO_GPIOQ                 0x00004000 // GPIOQ SW Reset
#define SYSCTL_SRGPIO_GPIOR                 0x00008000 // GPIOR SW Reset
#define SYSCTL_SRGPIO_GPIOS                 0x00010000 // GPIOS SW Reset

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_RCGCGPIO
// register.
//*****************************************************************************
#define SYSCTL_RCGCGPIO_GPIOA               0x00000001 // GPIOA Run Mode
                                                       // Clock Gating
#define SYSCTL_RCGCGPIO_GPIOB               0x00000002 // GPIOB Run Mode
                                                       // Clock Gating
#define SYSCTL_RCGCGPIO_GPIOC               0x00000004 // GPIOC Run Mode
                                                       // Clock Gating
#define SYSCTL_RCGCGPIO_GPIOD               0x00000008 // GPIOD Run Mode
                                                       // Clock Gating
#define SYSCTL_RCGCGPIO_GPIOE               0x00000010 // GPIOE Run Mode
                                                       // Clock Gating
#define SYSCTL_RCGCGPIO_GPIOF               0x00000020 // GPIOF Run Mode
                                                       // Clock Gating
#define SYSCTL_RCGCGPIO_GPIOG               0x00000040 // GPIOG Run Mode
                                                       // Clock Gating
#define SYSCTL_RCGCGPIO_GPIOH               0x00000080 // GPIOH Run Mode
                                                       // Clock Gating
#define SYSCTL_RCGCGPIO_GPIOJ               0x00000100 // GPIOJ Run Mode
                                                       // Clock Gating
#define SYSCTL_RCGCGPIO_GPIOK               0x00000200 // GPIOK Run Mode
                                                       // Clock Gating
#define SYSCTL_RCGCGPIO_GPIOL               0x00000400 // GPIOL Run Mode
                                                       // Clock Gating
#define SYSCTL_RCGCGPIO_GPIOM               0x00000800 // GPIOM Run Mode
                                                       // Clock Gating
#define SYSCTL_RCGCGPIO_GPION               0x00001000 // GPION Run Mode
                                                       // Clock Gating
#define SYSCTL_RCGCGPIO_GPIOP               0x00002000 // GPIOP Run Mode
                                                       // Clock Gating
#define SYSCTL_RCGCGPIO_GPIOQ               0x00004000 // GPIOQ Run Mode
                                                       // Clock Gating
#define SYSCTL_RCGCGPIO_GPIOR               0x00008000 // GPIOR Run Mode
                                                       // Clock Gating
#define SYSCTL_RCGCGPIO_GPIOS               0x00010000 // GPIOS Run Mode
                                                       // Clock Gating

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_SCGCGPIO
// register.
//*****************************************************************************
#define SYSCTL_SCGCGPIO_GPIOA               0x00000001 // GPIOA Sleep Mode
                                                       // Clock Gating
#define SYSCTL_SCGCGPIO_GPIOB               0x00000002 // GPIOB Sleep Mode
                                                       // Clock Gating
#define SYSCTL_SCGCGPIO_GPIOC               0x00000004 // GPIOC Sleep Mode
                                                       // Clock Gating
#define SYSCTL_SCGCGPIO_GPIOD               0x00000008 // GPIOD Sleep Mode
                                                       // Clock Gating
#define SYSCTL_SCGCGPIO_GPIOE               0x00000010 // GPIOE Sleep Mode
                                                       // Clock Gating
#define SYSCTL_SCGCGPIO_GPIOF               0x00000020 // GPIOF Sleep Mode
                                                       // Clock Gating
#define SYSCTL_SCGCGPIO_GPIOG               0x00000040 // GPIOG Sleep Mode
                                                       // Clock Gating
#define SYSCTL_SCGCGPIO_GPIOH               0x00000080 // GPIOH Sleep Mode
                                                       // Clock Gating
#define SYSCTL_SCGCGPIO_GPIOJ               0x00000100 // GPIOJ Sleep Mode
                                                       // Clock Gating
#define SYSCTL_SCGCGPIO_GPIOK               0x00000200 // GPIOK Sleep Mode
                                                       // Clock Gating
#define SYSCTL_SCGCGPIO_GPIOL               0x00000400 // GPIOL Sleep Mode
                                                       // Clock Gating
#define SYSCTL_SCGCGPIO_GPIOM               0x00000800 // GPIOM Sleep Mode
                                                       // Clock Gating
#define SYSCTL_SCGCGPIO_GPION               0x00001000 // GPION Sleep Mode
                                                       // Clock Gating
#define SYSCTL_SCGCGPIO_GPIOP               0x00002000 // GPIOP Sleep Mode
                                                       // Clock Gating
#define SYSCTL_SCGCGPIO_GPIOQ               0x00004000 // GPIOQ Sleep Mode
                                                       // Clock Gating
#define SYSCTL_SCGCGPIO_GPIOR               0x00008000 // GPIOR Sleep Mode
                                                       // Clock Gating
#define SYSCTL_SCGCGPIO_GPIOS               0x00010000 // GPIOS Sleep Mode
                                                       // Clock Gating                                                       

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_DCGCGPIO
// register.
//*****************************************************************************
#define SYSCTL_DCGCGPIO_GPIOA               0x00000001 // GPIOA Deep-Sleep Mode
                                                       // Clock Gating
#define SYSCTL_DCGCGPIO_GPIOB               0x00000002 // GPIOB Deep-Sleep Mode
                                                       // Clock Gating
#define SYSCTL_DCGCGPIO_GPIOC               0x00000004 // GPIOC Deep-Sleep Mode
                                                       // Clock Gating
#define SYSCTL_DCGCGPIO_GPIOD               0x00000008 // GPIOD Deep-Sleep Mode
                                                       // Clock Gating
#define SYSCTL_DCGCGPIO_GPIOE               0x00000010 // GPIOE Deep-Sleep Mode
                                                       // Clock Gating
#define SYSCTL_DCGCGPIO_GPIOF               0x00000020 // GPIOF Deep-Sleep Mode
                                                       // Clock Gating
#define SYSCTL_DCGCGPIO_GPIOG               0x00000040 // GPIOG Deep-Sleep Mode
                                                       // Clock Gating
#define SYSCTL_DCGCGPIO_GPIOH               0x00000080 // GPIOH Deep-Sleep Mode
                                                       // Clock Gating
#define SYSCTL_DCGCGPIO_GPIOJ               0x00000100 // GPIOJ Deep-Sleep Mode
                                                       // Clock Gating
#define SYSCTL_DCGCGPIO_GPIOK               0x00000200 // GPIOK Deep-Sleep Mode
                                                       // Clock Gating
#define SYSCTL_DCGCGPIO_GPIOL               0x00000400 // GPIOL Deep-Sleep Mode
                                                       // Clock Gating
#define SYSCTL_DCGCGPIO_GPIOM               0x00000800 // GPIOM Deep-Sleep Mode
                                                       // Clock Gating
#define SYSCTL_DCGCGPIO_GPION               0x00001000 // GPION Deep-Sleep Mode
                                                       // Clock Gating
#define SYSCTL_DCGCGPIO_GPIOP               0x00002000 // GPIOP Deep-Sleep Mode
                                                       // Clock Gating
#define SYSCTL_DCGCGPIO_GPIOQ               0x00004000 // GPIOQ Deep-Sleep Mode
                                                       // Clock Gating
#define SYSCTL_DCGCGPIO_GPIOR               0x00008000 // GPIOR Deep-Sleep Mode
                                                       // Clock Gating
#define SYSCTL_DCGCGPIO_GPIOS               0x00010000 // GPIOS Deep-Sleep Mode
                                                       // Clock Gating
                                                       
//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_SYSPLLCTL
// register.
//*****************************************************************************
#define SYSCTL_SYSPLLCTL_SPLLEN             0x00000001  // M3 System PLL Enable
#define SYSCTL_SYSPLLCTL_SPLLCLKEN          0x00000002  // M3 System PLL Clock
                                                        // Enable

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_SYSPLLMULT
// register.
//*****************************************************************************
#define SYSCTL_SYSPLLMULT_SPLLIMULT_S       0
#define SYSCTL_SYSPLLMULT_SPLLIMULT_M       0x0000007F // M3 System PLL Integer
                                                       // Multiplier
#define SYSCTL_SYSPLLMULT_SPLLFMULT_S       8
#define SYSCTL_SYSPLLMULT_SPLLFMULT_M       0x00000300 // M3 System PLL
                                                       // Fractional Multiplier

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_SYSPLLLOCKS
// register.
//*****************************************************************************
#define SYSCTL_SYSPLLLOCKS_SYSPLLLOCKS      0x00000001  // System PLL Lock
                                                        // Status
#define SYSCTL_SYSPLLLOCKS_SPLLSLIPS        0x00000002  // System PLL Out of
                                                        // Range Status

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_DIVSEL register.
//*****************************************************************************
#define SYSCTL_SYSDIVSEL_SYSDIVSEL_S        0
#define SYSCTL_SYSDIVSEL_SYSDIVSEL_M        0x00000003 // M3 System Clock
                                                       // Divider
#define SYSCTL_SYSDIVSEL_SPR_FAMOUNT_S      6
#define SYSCTL_SYSDIVSEL_SPR_FAMOUNT_M      0x001FFFC0 // System PLL Modulation
                                                       // (Fractional Part)
#define SYSCTL_SYSDIVSEL_SPR_IAMOUNT_S      21
#define SYSCTL_SYSDIVSEL_SPR_IAMOUNT_M      0xFFE00000 // System PLL Modulation
                                                       // (Integer Part)

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_M3SSDIVSEL
// register.
//*****************************************************************************
#define SYSCTL_M3SSDIVSEL_S                 0
#define SYSCTL_M3SSDIVSEL_M                 0x00000003 // M3 Sub System Clock
                                                       // Divide

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_CCLKCTL
// register.
//*****************************************************************************
#define SYSCTL_CCLKCTL_S                    0
#define SYSCTL_CCLKCTL_M                    0x00000007 // Analog System Divisor
                                                       // Ratio                                                       

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_UPLLCTL register.
//*****************************************************************************
#define SYSCTL_UPLLCTL_CLKSRSCEL            0x00000001 // USB PLL Clock Source
                                                       // Select
#define SYSCTL_UPLLCTL_EN                   0x00000002 // USB PLL Enable
#define SYSCTL_UPLLCTL_CLKEN                0x00000004 // USB PLL Clock Enable

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_UPLLMULT
// register.
//*****************************************************************************
#define SYSCTL_UPLLMULT_PLLIMULT_S          0
#define SYSCTL_UPLLMULT_PLLIMULT_M          0x0000001F // USB PLL Integer
                                                       // Multiplier
#define SYSCTL_UPLLMULT_PLLFMULT_S          8
#define SYSCTL_UPLLMULT_PLLFMULT_M          0x00000300 // USB PLL Fractional
                                                       // Multiplier

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_UPLLSTS register.
//*****************************************************************************
#define SYSCTL_UPLLSTS_UPLLLOCKS            0x00000001 // USB PLL Lock Status
#define SYSCTL_UPLLSTS_UPLLSLIPS            0x00000002 // USB PLL Out of Range
                                                       // Status

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_MCLKSTS register.
//*****************************************************************************
#define SYSCTL_MCLKSTS_REFCLKCNT_S          0
#define SYSCTL_MCLKSTS_REFCLKCNT_M          0x000000FF // Reference Clock Count
#define SYSCTL_MCLKSTS_MCLKFLG              0x00010000 // Missing Clock Status
                                                       // Flag

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_MCLKFRCCLR
// register.
//*****************************************************************************
#define SYSCTL_MCLKFRCCLR_REFCLKOFF         0x00000001 // Reference Clock Off
#define SYSCTL_MCLKFRCCLR_MCLKCLR           0x00010000 // Missing Clock Status
                                                       // Clear

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_MCLKEN register.
//*****************************************************************************
#define SYSCTL_MCLKEN_MCLKNMIEN             0x00000100 // Missing Clock NMI
                                                       // Enable

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_MREFCLKLIM
// register.
//*****************************************************************************
#define SYSCTL_MREFCLKLIM_REFCLKLOLIM_S     0
#define SYSCTL_MREFCLKLIM_REFCLKLOLIM_M     0x000000FF // Reference Clock Low
                                                       // Limit
#define SYSCTL_MREFCLKLIM_REFCLKHILIM_S     8
#define SYSCTL_MREGCLKLIM_REFCLKHILIM_M     0x0000FF00 // Reference Clock High
                                                       // Limit

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_XPLLCLKCFG
// register.
//*****************************************************************************
#define SYSCTL_XCLKCFG_XPLLCLKOUTDIV_S      0
#define SYSCTL_XCLKCFG_XPLLCLKOUTDIV_M      0x00000003 // XCLKOUT Divide Ration

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_CCLKOFF register.
//*****************************************************************************
#define SYSCTL_CCLKOFF_C28CLKINDIS          0x00000001 // M3 C28 CLKIN Disable

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_CAN0BCLKSEL
// register.
//*****************************************************************************
#define SYSCTL_CAN0BCLKSEL_BCLKSEL_S        0
#define SYSCTL_CAN0BCLKSEL_BCLKSEL_M        0x00000003 // Bit Clock Source
                                                       // Select

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_CAN1BCLKSEL
// register.
//*****************************************************************************
#define SYSCTL_CAN1BCLKSEL_BCLKSEL_S        0
#define SYSCTL_CAN1BCLKSEL_BCLKSEL_M        0x00000003 // Bit Clock Source
                                                       // Select

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_CLPMSTAT
// register.
//*****************************************************************************
#define SYSCTL_BC28LPMSTAT_S                0
#define SYSCTL_BC28LPMSTAT_M                0x00000003 // M3 C28 Low Power Mode
                                                       // Status

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_CRESCNF register.
//*****************************************************************************
#define SYSCTL_CRESCNF_MTOCRESDIS           0x00000001 // M3 Software Reset
                                                       // Disable to C28
#define SYSCTL_CRESCNF_M3RSNIN              0x00010000 // M3 Reset to C28 CPU
#define SYSCTL_CRESCNF_ANALOGRESET          0x00020000 // M3 Reset to Analog 
                                                       // Subsystem

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_CRESSTS register.
//*****************************************************************************
#define SYSCTL_CRESSTS_CRES                 0x00000001 // C28 Reset Status
#define SYSCTL_CRESSTS_CNMIWDRST            0x00010000 // C28 NMI WD Reset
#define SYSCTL_CRESSTS_CHWBISTRST_S         17
#define SYSCTL_CRESSTS_CHWBISTRST_M         0x00060000 // C28 HW BIST Reset

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_FUSEERR register.
//*****************************************************************************
#define SYSCTL_FUSEERR_ALERR_S              0
#define SYSCTL_FUSEERR_ALERR_M              0x0000001F // Autoload Error Status
#define SYSCTL_FUSEERR_ERR                  0x00010000 // Self Test Error
                                                       // Status

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_MWIR register.
//*****************************************************************************
#define SYSCTL_MWIR_EMU0                    0x00000001 // Latched State of EMU0
                                                       // Pin
#define SYSCTL_MWIR_EMU1                    0x00000002 // Latched State of EMU1
                                                       // Pin
#define SYSCTL_MWIR_SAMPLE                  0x00000004 // Re-Sample EMU0 and
                                                       // EMU1 Pins

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_MCNF register.
//*****************************************************************************
#define SYSCTL_MCNF_FLASH_S                 0
#define SYSCTL_MCNF_FLASH_M                 0x00000007 // M3 Flash Size
                                                       // Configuratioin
#define SYSCTL_MCNF_UCRC                    0x00000100 // uCRC Configuration
#define SYSCTL_MCNF_M3HWBIST                0x00010000 // M3 HW BIST
                                                       // Configuration

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_SERPLOOP
// register.
//*****************************************************************************
#define SYSCTL_SERPLOOP_SSI3TOSPIA_S        0
#define SYSCTL_SERPLOOP_SSI3TOSPIA_M        0x00000003 // SSI3 to SPIA loopback
#define SYSCTL_SERPLOOP_UART4TOSCIA         0x00000100 // UART4 to SCIA
                                                       // loopback

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_CCNF0 register.
//*****************************************************************************
#define SYSCTL_CCNF0_MCBSP                  0x00001000 // McBSP Module
                                                       // Configuration
#define SYSCTL_CCNF0_SCI                    0x00000400 // SCI Module
                                                       // Configuration
#define SYSCTL_CCNF0_SPI                    0x00000100 // SPI Module
                                                       // Configuration
#define SYSCTL_CCNF0_I2C                    0x00000010 // I2C Module
                                                       // Configuration
#define SYSCTL_CCNF0_HRPWM                  0x00000001 // HRPWM Module
                                                       // Configuration

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_CCNF1 register.
//*****************************************************************************
#define SYSCTL_CCNF1_EQEP2                  0x00008000 // EQEP2 Module
                                                       // Configuration
#define SYSCTL_CCNF1_EQEP1                  0x00004000 // EQEP1 Module
                                                       // Configuration
#define SYSCTL_CCNF1_ECAP6                  0x00002000 // ECAP6 Module
                                                       // Configuration
#define SYSCTL_CCNF1_ECAP5                  0x00001000 // ECAP5 Module
                                                       // Configuration
#define SYSCTL_CCNF1_ECAP4                  0x00000800 // ECAP4 Module
                                                       // Configuration
#define SYSCTL_CCNF1_ECAP3                  0x00000400 // ECAP3 Module
                                                       // Configuration
#define SYSCTL_CCNF1_ECAP2                  0x00000200 // ECAP2 Module
                                                       // Configuration
#define SYSCTL_CCNF1_ECAP1                  0x00000100 // ECAP1 Module
                                                       // Configuration
#define SYSCTL_CCNF1_EPWM8                  0x00000080 // EPWM8 Module
                                                       // Configuration
#define SYSCTL_CCNF1_EPWM7                  0x00000040 // EPWM7 Module
                                                       // Configuration
#define SYSCTL_CCNF1_EPWM6                  0x00000020 // EPWM6 Module
                                                       // Configuration
#define SYSCTL_CCNF1_EPWM5                  0x00000010 // EPWM5 Module
                                                       // Configuration
#define SYSCTL_CCNF1_EPWM4                  0x00000008 // EPWM4 Module
                                                       // Configuration
#define SYSCTL_CCNF1_EPWM3                  0x00000004 // EPWM3 Module
                                                       // Configuration
#define SYSCTL_CCNF1_EPWM2                  0x00000002 // EPWM2 Module
                                                       // Configuration
#define SYSCTL_CCNF1_EPWM1                  0x00000001 // EPWM1 Module
                                                       // Configuration

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_CCNF2 register.
//*****************************************************************************
#define SYSCTL_CCNF2_EQEP3                  0x00000100 // EQEP3 Module
                                                       // Configuration
#define SYSCTL_CCNF2_EPWM9                  0x00000001 // EPWM9 Module
                                                       // Configuration
#define SYSCTL_CCNF2_EPWM10                 0x00000002 // EPWM10 Module
                                                       // Configuration
#define SYSCTL_CCNF2_EPWM11                 0x00000004 // EPWM11 Module
                                                       // Configuration
#define SYSCTL_CCNF2_EPWM12                 0x00000008 // EPWM12 Module
                                                       // Configuration
                                                       
//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_CCNF3 register.
//*****************************************************************************
#define SYSCTL_CCNF3_C28DMA                 0x00000800 // C28DMA Module
                                                       // Configuration

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_CCNF4 register.
//*****************************************************************************
#define SYSCTL_CCNF4_C28HWBIST              0x00010000 // C28 HWBIST
                                                       // Configuration
#define SYSCTL_CCNF4_FLASH_M                0x00000007 // Flash configuration

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_MCIBSTATUS
// register.
//*****************************************************************************
#define SYSCTL_MCIBSTATUS_APGOODSTS         0x00000001 // Analog System Power
                                                       // Good Status
#define SYSCTL_MCIBSTATUS_READY             0x00000002 // Ready Signal State
#define SYSCTL_MCIBSTATUS_INT               0x00000004 // INTS Signal State
#define SYSCTL_MCIBSTATUS_ANALOGBUSCLKCNT_S 8
#define SYSCTL_MCIBSTATUS_ANALOGBUSCLKCNT_M 0x0000FF00 // 8-Bit Analog 
                                                       // Bus Clock 
                                                       // Counter

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_MEMCNF register.
//*****************************************************************************
#define SYSCTL_MEMCNF_S0                    0x00000001 // S0 Shared Memory
                                                       // Config
#define SYSCTL_MEMCNF_S1                    0x00000002 // S1 Shared Memory
                                                       // Config
#define SYSCTL_MEMCNF_S2                    0x00000004 // S2 Shared Memory
                                                       // Config
#define SYSCTL_MEMCNF_S3                    0x00000008 // S3 Shared Memory
                                                       // Config
#define SYSCTL_MEMCNF_S4                    0x00000010 // S4 Shared Memory
                                                       // Config
#define SYSCTL_MEMCNF_S5                    0x00000020 // S5 Shared Memory
                                                       // Config
#define SYSCTL_MEMCNF_S6                    0x00000040 // S6 Shared Memory
                                                       // Config
#define SYSCTL_MEMCNF_S7                    0x00000080 // S7 Shared Memory
                                                       // Config

//*****************************************************************************
// The following are defines for the bit fields in the SYSCTL_MLOCK register.
//*****************************************************************************
#define SYSCTL_MSXMSELLOCK                  0x00000001 // Lock Writes to
                                                       // MSxMSEL Register


#endif // __HW_SYSCTL_H__






