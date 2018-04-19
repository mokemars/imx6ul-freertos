//###########################################################################
// FILE:   hw_memmap.h
// TITLE:  Macros defining the memory map of Concerto.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_MEMMAP_H__
#define __HW_MEMMAP_H__

//*****************************************************************************
// The following are defines for the base address of the memories and
// peripherals.
//*****************************************************************************
#define FLASH_BASE             0x00200000   // FLASH Memory
#define RAM_BASE               0x20000000   // RAM Memory
#define WATCHDOG0_BASE         0x40000000   // Watchdog0
#define WATCHDOG1_BASE         0x40001000   // Watchdog1
#define GPIO_PORTA_BASE        0x40004000   // GPIO Port A
#define GPIO_PORTB_BASE        0x40005000   // GPIO Port B
#define GPIO_PORTC_BASE        0x40006000   // GPIO Port C
#define GPIO_PORTD_BASE        0x40007000   // GPIO Port D
#define SSI0_BASE              0x40008000   // SSI0
#define SSI1_BASE              0x40009000   // SSI1
#define SSI2_BASE              0x4000A000   // SSI2
#define SSI3_BASE              0x4000B000   // SSI3
#define UART0_BASE             0x4000C000   // UART0
#define UART1_BASE             0x4000D000   // UART1
#define UART2_BASE             0x4000E000   // UART2
#define UART3_BASE             0x4000F000   // UART3
#define UART4_BASE             0x40010000   // UART4
#define I2C0_MASTER_BASE       0x40020000   // I2C0 Master
#define I2C0_SLAVE_BASE        0x40020800   // I2C0 Slave
#define I2C1_MASTER_BASE       0x40021000   // I2C1 Master
#define I2C1_SLAVE_BASE        0x40021800   // I2C1 Slave
#define GPIO_PORTE_BASE        0x40024000   // GPIO Port E
#define GPIO_PORTF_BASE        0x40025000   // GPIO Port F
#define GPIO_PORTG_BASE        0x40026000   // GPIO Port G
#define GPIO_PORTH_BASE        0x40027000   // GPIO Port H
#define TIMER0_BASE            0x40030000   // Timer0
#define TIMER1_BASE            0x40031000   // Timer1
#define TIMER2_BASE            0x40032000   // Timer2
#define TIMER3_BASE            0x40033000   // Timer3
#define GPIO_PORTJ_BASE        0x4003D000   // GPIO Port J
#define ETH_BASE               0x40048000   // Ethernet
#define MAC_BASE               0x40048000   // Ethernet
#define USB0_BASE              0x40050000   // USB 0 Controller
#define GPIO_PORTA_AHB_BASE    0x40058000   // GPIO Port A (high speed)
#define GPIO_PORTB_AHB_BASE    0x40059000   // GPIO Port B (high speed)
#define GPIO_PORTC_AHB_BASE    0x4005A000   // GPIO Port C (high speed)
#define GPIO_PORTD_AHB_BASE    0x4005B000   // GPIO Port D (high speed)
#define GPIO_PORTE_AHB_BASE    0x4005C000   // GPIO Port E (high speed)
#define GPIO_PORTF_AHB_BASE    0x4005D000   // GPIO Port F (high speed)
#define GPIO_PORTG_AHB_BASE    0x4005E000   // GPIO Port G (high speed)
#define GPIO_PORTH_AHB_BASE    0x4005F000   // GPIO Port H (high speed)
#define GPIO_PORTJ_AHB_BASE    0x40060000   // GPIO Port J (high speed)
#define GPIO_PORTK_AHB_BASE    0x40061000   // GPIO Port K (high speed)
#define GPIO_PORTL_AHB_BASE    0x40062000   // GPIO Port L (high speed)
#define GPIO_PORTM_AHB_BASE    0x40063000   // GPIO Port M (high speed)
#define GPIO_PORTN_AHB_BASE    0x40064000   // GPIO Port N (high speed)
#define GPIO_PORTP_AHB_BASE    0x40065000   // GPIO Port P (high speed)
#define GPIO_PORTQ_AHB_BASE    0x40066000   // GPIO Port Q (high speed)
#define GPIO_PORTR_AHB_BASE    0x40067000   // GPIO Port R (high speed)
#define GPIO_PORTS_AHB_BASE    0x40068000   // GPIO Port S (high speed)
#define CAN0_BASE              0x40070000   // CAN0
#define CAN1_BASE              0x40074000   // CAN1
#define EPI0_BASE              0x400D0000   // EPI0
#define FLASH_CONFIG_BASE      0x400FA000   // Flash Control
#define FLASH_ERROR_BASE       0x400FA600   // Flash Error
#define PBIST_BASE			   0x400FB000	// PBIST
#define RAM_CONFIG_BASE        0x400FB200   // RAM Config
#define RAM_ERROR_BASE         0x400FB300   // RAM Error (ECC/Parity/Access)
#define CSM_BASE               0x400FB400   // CSM
#define UCRC_BASE              0x400FB600   // uCRC
#define MTOCIPC_BASE           0x400FB700   // M to C IPC
#define CLOCK_CONTROL_BASE     0x400FB800   // M3 Clock Control
#define LPM_CONTROL_BASE       0x400FB880   // M3 LPM Control
#define RESET_CONTROL_BASE     0x400FB8C0   // M3 Reset Control
#define DEVICE_CONFIG_BASE     0x400FB900   // Device Config
#define WRITE_PROTECT_BASE     0x400FB980   // Write Protect
#define NMI_BASE               0x400FBA00   // NMI
#define HWBIST_BASE            0x400FBB00   // Logic BIST
#define SYSCTL_BASE            0x400FE000   // System Control
#define UDMA_BASE              0x400FF000   // uDMA Controller
#define ADC1_BASE              0x50001600   // ADC1 Result Register
#define ADC2_BASE              0x50001680   // ADC1 Result Register
#define DWT_BASE               0xE0001000   // Data Watchpoint and Trace
#define FPB_BASE               0xE0002000   // FLASH Patch and Breakpoint
#define NVIC_BASE              0xE000E000   // Nested Vectored Interrupt Ctrl

//*****************************************************************************
// The following are compatibility defines for peripherals that are available
// only on the AHB aperture.
//*****************************************************************************
#define GPIO_PORTK_BASE        GPIO_PORTK_AHB_BASE  // GPIO Port K (high speed)
#define GPIO_PORTL_BASE        GPIO_PORTL_AHB_BASE  // GPIO Port L (high speed)
#define GPIO_PORTM_BASE        GPIO_PORTM_AHB_BASE  // GPIO Port M (high speed)
#define GPIO_PORTN_BASE        GPIO_PORTN_AHB_BASE  // GPIO Port N (high speed)
#define GPIO_PORTP_BASE        GPIO_PORTP_AHB_BASE  // GPIO Port P (high speed)
#define GPIO_PORTQ_BASE        GPIO_PORTQ_AHB_BASE  // GPIO Port Q (high speed)
#define GPIO_PORTR_BASE        GPIO_PORTR_AHB_BASE  // GPIO Port R (high speed)
#define GPIO_PORTS_BASE        GPIO_PORTS_AHB_BASE  // GPIO Port S (high speed)

#endif // __HW_MEMMAP_H__




