//###########################################################################
// FILE:   hw_ints.h
// TITLE:  Macros that define the interrupt assignment on Concerto.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_INTS_H__
#define __HW_INTS_H__

//*****************************************************************************
// The following are defines for the fault assignments.
//*****************************************************************************
#define FAULT_NMI                    2   // NMI fault
#define FAULT_HARD                   3   // Hard fault
#define FAULT_MPU                    4   // MPU fault
#define FAULT_BUS                    5   // Bus fault
#define FAULT_USAGE                  6   // Usage fault
#define FAULT_SVCALL                 11  // SVCall
#define FAULT_DEBUG                  12  // Debug monitor
#define FAULT_PENDSV                 14  // PendSV
#define FAULT_SYSTICK                15  // System Tick

//*****************************************************************************
// The following are defines for the interrupt assignments.
//*****************************************************************************
#define INT_GPIOA                    16  // GPIO Port A
#define INT_GPIOB                    17  // GPIO Port B
#define INT_GPIOC                    18  // GPIO Port C
#define INT_GPIOD                    19  // GPIO Port D
#define INT_GPIOE                    20  // GPIO Port E
#define INT_UART0                    21  // UART0 Rx and Tx
#define INT_UART1                    22  // UART1 Rx and Tx
#define INT_SSI0                     23  // SSI0 Rx and Tx
#define INT_I2C0                     24  // I2C0 Master and Slave
#define INT_WATCHDOG                 34  // Watchdog timer
#define INT_TIMER0A                  35  // Timer 0 subtimer A
#define INT_TIMER0B                  36  // Timer 0 subtimer B
#define INT_TIMER1A                  37  // Timer 1 subtimer A
#define INT_TIMER1B                  38  // Timer 1 subtimer B
#define INT_TIMER2A                  39  // Timer 2 subtimer A
#define INT_TIMER2B                  40  // Timer 2 subtimer B
#define INT_SYSCTL                   44  // System Control (PLL, OSC, BO)
#define INT_FLASH                    45  // FLASH FSM
#define INT_GPIOF                    46  // GPIO Port F
#define INT_GPIOG                    47  // GPIO Port G
#define INT_GPIOH                    48  // GPIO Port H
#define INT_UART2                    49  // UART2 Rx and Tx
#define INT_SSI1                     50  // SSI1 Rx and Tx
#define INT_TIMER3A                  51  // Timer 3 subtimer A
#define INT_TIMER3B                  52  // Timer 3 subtimer B
#define INT_I2C1                     53  // I2C1 Master and Slave
#define INT_ETH                      58  // EMAC
#define INT_USB0                     60  // USB Controller
#define INT_UDMA                     62  // uDMA Software
#define INT_UDMAERR                  63  // uDMA Error
#define INT_EPI0                     69  // EPI
#define INT_GPIOJ                    70  // GPIO Port J
#define INT_GPIOK                    71  // GPIO Port K
#define INT_GPIOL                    72  // GPIO Port L
#define INT_SSI2                     73  // SSI2
#define INT_SSI3                     74  // SSI3
#define INT_UART3                    75  // UART3
#define INT_UART4                    76  // UART4
#define INT_CAN0INT0                 80  // CAN 0 INT 0
#define INT_CAN0INT1                 81  // CAN 0 INT 1
#define INT_CAN1INT0                 82  // CAN 1 INT 0
#define INT_CAN1INT1                 83  // CAN 1 INT 1
#define INT_ADC1                     88  // ADC Interrupt 1
#define INT_ADC2                     89  // ADC Interrupt 2
#define INT_ADC3                     90  // ADC Interrupt 3
#define INT_ADC4                     91  // ADC Interrupt 4
#define INT_ADC5                     92  // ADC Interrupt 5
#define INT_ADC6                     93  // ADC Interrupt 6
#define INT_ADC7                     94  // ADC Interrupt 7
#define INT_ADC8                     95  // ADC Interrupt 8
#define INT_CTOMPIC1                 96  // CtoM IPC1
#define INT_CTOMPIC2                 97  // CtoM IPC2
#define INT_CTOMPIC3                 98  // CtoM IPC3
#define INT_CTOMPIC4                 99  // CtoM IPC4
#define INT_RAM_SINGLE_ERROR         104 // RAM Single Error
#define INT_USB_PLL_LOCK             105 // USB PLL Lock
#define INT_M3_FLASH_SINGLE_ERROR    106 // M3 Flash Single Error
#define INT_PBIST                    107 // PBIST Done
#define INT_GPIOM                    127 // GPIO Port M
#define INT_GPION                    128 // GPIO Port N
#define INT_GPIOP                    132 // GPIO Port P
#define INT_GPIOQ                    140 // GPIO Port Q
#define INT_GPIOR                    148 // GPIO Port R
#define INT_GPIOS                    149 // GPIO Port S

//*****************************************************************************
// The following are defines for the total number of interrupts.
//*****************************************************************************
#define NUM_INTERRUPTS               150

//*****************************************************************************
// The following are defines for the total number of priority levels.
//*****************************************************************************
#define NUM_PRIORITY                 8
#define NUM_PRIORITY_BITS            3


#endif // __HW_INTS_H__



