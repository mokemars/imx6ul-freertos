/**************************************************************************//**
 * @file     LPC17xx.h
 * @brief    CMSIS Cortex-M3 Device Peripheral Access Layer Header File for
 *           NXP LPC17xx Device Series
 * @version  V1.10
 * @date     06. April 2011
 *
 * @note
 * Copyright (C) 2009-2011 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *                                                            modified by Keil
******************************************************************************/


#ifndef __LPC17xx_H__
#define __LPC17xx_H__

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

/** @addtogroup LPC17xx_System
 * @{
 */

/** @brief IRQ interrupt source definition */
typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
  Reset_IRQn                    = -15,      /*!< 1 Reset Vector, invoked on PowerUp and warm reset*/
  NonMaskableInt_IRQn           = -14,      /*!< 2 Non Maskable Interrupt                         */
  HardFault_IRQn                = -13,      /*!< 3  Hard Fault, all classes of Fault              */
  MemoryManagement_IRQn         = -12,      /*!< 4 Cortex-M3 Memory Management Interrupt          */
  BusFault_IRQn                 = -11,      /*!< 5 Cortex-M3 Bus Fault Interrupt                  */
  UsageFault_IRQn               = -10,      /*!< 6 Cortex-M3 Usage Fault Interrupt                */
  SVCall_IRQn                   = -5,       /*!< 11 Cortex-M3 SV Call Interrupt                   */
  DebugMonitor_IRQn             = -4,       /*!< 12 Cortex-M3 Debug Monitor Interrupt             */
  PendSV_IRQn                   = -2,       /*!< 14 Cortex-M3 Pend SV Interrupt                   */
  SysTick_IRQn                  = -1,       /*!< 15 Cortex-M3 System Tick Interrupt               */

/******  LPC17xx Specific Interrupt Numbers *******************************************************/
  WDT_IRQn                      = 0,        /*!< Watchdog Timer Interrupt                         */
  TIMER0_IRQn                   = 1,        /*!< Timer0 Interrupt                                 */
  TIMER1_IRQn                   = 2,        /*!< Timer1 Interrupt                                 */
  TIMER2_IRQn                   = 3,        /*!< Timer2 Interrupt                                 */
  TIMER3_IRQn                   = 4,        /*!< Timer3 Interrupt                                 */
  UART0_IRQn                    = 5,        /*!< UART0 Interrupt                                  */
  UART1_IRQn                    = 6,        /*!< UART1 Interrupt                                  */
  UART2_IRQn                    = 7,        /*!< UART2 Interrupt                                  */
  UART3_IRQn                    = 8,        /*!< UART3 Interrupt                                  */
  PWM1_IRQn                     = 9,        /*!< PWM1 Interrupt                                   */
  I2C0_IRQn                     = 10,       /*!< I2C0 Interrupt                                   */
  I2C1_IRQn                     = 11,       /*!< I2C1 Interrupt                                   */
  I2C2_IRQn                     = 12,       /*!< I2C2 Interrupt                                   */
  SPI_IRQn                      = 13,       /*!< SPI Interrupt                                    */
  SSP0_IRQn                     = 14,       /*!< SSP0 Interrupt                                   */
  SSP1_IRQn                     = 15,       /*!< SSP1 Interrupt                                   */
  PLL0_IRQn                     = 16,       /*!< PLL0 Lock (Main PLL) Interrupt                   */
  RTC_IRQn                      = 17,       /*!< Real Time Clock Interrupt                        */
  EINT0_IRQn                    = 18,       /*!< External Interrupt 0 Interrupt                   */
  EINT1_IRQn                    = 19,       /*!< External Interrupt 1 Interrupt                   */
  EINT2_IRQn                    = 20,       /*!< External Interrupt 2 Interrupt                   */
  EINT3_IRQn                    = 21,       /*!< External Interrupt 3 Interrupt                   */
  ADC_IRQn                      = 22,       /*!< A/D Converter Interrupt                          */
  BOD_IRQn                      = 23,       /*!< Brown-Out Detect Interrupt                       */
  USB_IRQn                      = 24,       /*!< USB Interrupt                                    */
  CAN_IRQn                      = 25,       /*!< CAN Interrupt                                    */
  DMA_IRQn                      = 26,       /*!< General Purpose DMA Interrupt                    */
  I2S_IRQn                      = 27,       /*!< I2S Interrupt                                    */
  ENET_IRQn                     = 28,       /*!< Ethernet Interrupt                               */
  RIT_IRQn                      = 29,       /*!< Repetitive Interrupt Timer Interrupt             */
  MCPWM_IRQn                    = 30,       /*!< Motor Control PWM Interrupt                      */
  QEI_IRQn                      = 31,       /*!< Quadrature Encoder Interface Interrupt           */
  PLL1_IRQn                     = 32,       /*!< PLL1 Lock (USB PLL) Interrupt                    */
  USBActivity_IRQn              = 33,       /*!< USB Activity Interrupt 						  */
  CANActivity_IRQn              = 34,       /*!< CAN Activity Interrupt 						  */
} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __MPU_PRESENT             1         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          5         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */


#include "core_cm3.h"                       /* Cortex-M3 processor and core peripherals           */
#include "system_LPC17xx.h"                 /* System Header                                      */


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/*------------- System Control (SC) ------------------------------------------*/
/** @brief System Control (SC) register structure definition */
typedef struct
{
  __IO uint32_t FLASHCFG;               /* Flash Accelerator Module           */
       uint32_t RESERVED0[31];
  __IO uint32_t PLL0CON;                /* Clocking and Power Control         */
  __IO uint32_t PLL0CFG;
  __I  uint32_t PLL0STAT;
  __O  uint32_t PLL0FEED;
       uint32_t RESERVED1[4];
  __IO uint32_t PLL1CON;
  __IO uint32_t PLL1CFG;
  __I  uint32_t PLL1STAT;
  __O  uint32_t PLL1FEED;
       uint32_t RESERVED2[4];
  __IO uint32_t PCON;
  __IO uint32_t PCONP;
       uint32_t RESERVED3[15];
  __IO uint32_t CCLKCFG;
  __IO uint32_t USBCLKCFG;
  __IO uint32_t CLKSRCSEL;
  __IO uint32_t	CANSLEEPCLR;
  __IO uint32_t	CANWAKEFLAGS;
       uint32_t RESERVED4[10];
  __IO uint32_t EXTINT;                 /* External Interrupts                */
       uint32_t RESERVED5;
  __IO uint32_t EXTMODE;
  __IO uint32_t EXTPOLAR;
       uint32_t RESERVED6[12];
  __IO uint32_t RSID;                   /* Reset                              */
       uint32_t RESERVED7[7];
  __IO uint32_t SCS;                    /* Syscon Miscellaneous Registers     */
  __IO uint32_t IRCTRIM;                /* Clock Dividers                     */
  __IO uint32_t PCLKSEL0;
  __IO uint32_t PCLKSEL1;
       uint32_t RESERVED8[4];
  __IO uint32_t USBIntSt;               /* USB Device/OTG Interrupt Register  */
  __IO uint32_t DMAREQSEL;
  __IO uint32_t CLKOUTCFG;              /* Clock Output Configuration         */
 } LPC_SC_TypeDef;

/*------------- Pin Connect Block (PINCON) -----------------------------------*/
/** @brief Pin Connect Block (PINCON) register structure definition */
typedef struct
{
  __IO uint32_t PINSEL0;
  __IO uint32_t PINSEL1;
  __IO uint32_t PINSEL2;
  __IO uint32_t PINSEL3;
  __IO uint32_t PINSEL4;
  __IO uint32_t PINSEL5;
  __IO uint32_t PINSEL6;
  __IO uint32_t PINSEL7;
  __IO uint32_t PINSEL8;
  __IO uint32_t PINSEL9;
  __IO uint32_t PINSEL10;
       uint32_t RESERVED0[5];
  __IO uint32_t PINMODE0;
  __IO uint32_t PINMODE1;
  __IO uint32_t PINMODE2;
  __IO uint32_t PINMODE3;
  __IO uint32_t PINMODE4;
  __IO uint32_t PINMODE5;
  __IO uint32_t PINMODE6;
  __IO uint32_t PINMODE7;
  __IO uint32_t PINMODE8;
  __IO uint32_t PINMODE9;
  __IO uint32_t PINMODE_OD0;
  __IO uint32_t PINMODE_OD1;
  __IO uint32_t PINMODE_OD2;
  __IO uint32_t PINMODE_OD3;
  __IO uint32_t PINMODE_OD4;
  __IO uint32_t I2CPADCFG;
} LPC_PINCON_TypeDef;

/*------------- General Purpose Input/Output (GPIO) --------------------------*/
/** @brief General Purpose Input/Output (GPIO) register structure definition */
typedef struct
{
  union {
    __IO uint32_t FIODIR;
    struct {
      __IO uint16_t FIODIRL;
      __IO uint16_t FIODIRH;
    };
    struct {
      __IO uint8_t  FIODIR0;
      __IO uint8_t  FIODIR1;
      __IO uint8_t  FIODIR2;
      __IO uint8_t  FIODIR3;
    };
  };
  uint32_t RESERVED0[3];
  union {
    __IO uint32_t FIOMASK;
    struct {
      __IO uint16_t FIOMASKL;
      __IO uint16_t FIOMASKH;
    };
    struct {
      __IO uint8_t  FIOMASK0;
      __IO uint8_t  FIOMASK1;
      __IO uint8_t  FIOMASK2;
      __IO uint8_t  FIOMASK3;
    };
  };
  union {
    __IO uint32_t FIOPIN;
    struct {
      __IO uint16_t FIOPINL;
      __IO uint16_t FIOPINH;
    };
    struct {
      __IO uint8_t  FIOPIN0;
      __IO uint8_t  FIOPIN1;
      __IO uint8_t  FIOPIN2;
      __IO uint8_t  FIOPIN3;
    };
  };
  union {
    __IO uint32_t FIOSET;
    struct {
      __IO uint16_t FIOSETL;
      __IO uint16_t FIOSETH;
    };
    struct {
      __IO uint8_t  FIOSET0;
      __IO uint8_t  FIOSET1;
      __IO uint8_t  FIOSET2;
      __IO uint8_t  FIOSET3;
    };
  };
  union {
    __O  uint32_t FIOCLR;
    struct {
      __O  uint16_t FIOCLRL;
      __O  uint16_t FIOCLRH;
    };
    struct {
      __O  uint8_t  FIOCLR0;
      __O  uint8_t  FIOCLR1;
      __O  uint8_t  FIOCLR2;
      __O  uint8_t  FIOCLR3;
    };
  };
} LPC_GPIO_TypeDef;

/** @brief General Purpose Input/Output interrupt (GPIOINT) register structure definition */
typedef struct
{
  __I  uint32_t IntStatus;
  __I  uint32_t IO0IntStatR;
  __I  uint32_t IO0IntStatF;
  __O  uint32_t IO0IntClr;
  __IO uint32_t IO0IntEnR;
  __IO uint32_t IO0IntEnF;
       uint32_t RESERVED0[3];
  __I  uint32_t IO2IntStatR;
  __I  uint32_t IO2IntStatF;
  __O  uint32_t IO2IntClr;
  __IO uint32_t IO2IntEnR;
  __IO uint32_t IO2IntEnF;
} LPC_GPIOINT_TypeDef;

/*------------- Timer (TIM) --------------------------------------------------*/
/** @brief Timer (TIM) register structure definition */
typedef struct
{
  __IO uint32_t IR;
  __IO uint32_t TCR;
  __IO uint32_t TC;
  __IO uint32_t PR;
  __IO uint32_t PC;
  __IO uint32_t MCR;
  __IO uint32_t MR0;
  __IO uint32_t MR1;
  __IO uint32_t MR2;
  __IO uint32_t MR3;
  __IO uint32_t CCR;
  __I  uint32_t CR0;
  __I  uint32_t CR1;
       uint32_t RESERVED0[2];
  __IO uint32_t EMR;
       uint32_t RESERVED1[12];
  __IO uint32_t CTCR;
} LPC_TIM_TypeDef;

/*------------- Pulse-Width Modulation (PWM) ---------------------------------*/
/** @brief Pulse-Width Modulation (PWM) register structure definition */
typedef struct
{
  __IO uint32_t IR;
  __IO uint32_t TCR;
  __IO uint32_t TC;
  __IO uint32_t PR;
  __IO uint32_t PC;
  __IO uint32_t MCR;
  __IO uint32_t MR0;
  __IO uint32_t MR1;
  __IO uint32_t MR2;
  __IO uint32_t MR3;
  __IO uint32_t CCR;
  __I  uint32_t CR0;
  __I  uint32_t CR1;
  __I  uint32_t CR2;
  __I  uint32_t CR3;
       uint32_t RESERVED0;
  __IO uint32_t MR4;
  __IO uint32_t MR5;
  __IO uint32_t MR6;
  __IO uint32_t PCR;
  __IO uint32_t LER;
       uint32_t RESERVED1[7];
  __IO uint32_t CTCR;
} LPC_PWM_TypeDef;

/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/
/** @brief  Universal Asynchronous Receiver Transmitter (UART) register structure definition */
typedef struct
{
  union {
  __I  uint8_t  RBR;
  __O  uint8_t  THR;
  __IO uint8_t  DLL;
       uint32_t RESERVED0;
  };
  union {
  __IO uint8_t  DLM;
  __IO uint32_t IER;
  };
  union {
  __I  uint32_t IIR;
  __O  uint8_t  FCR;
  };
  __IO uint8_t  LCR;
       uint8_t  RESERVED1[7];
  __I  uint8_t  LSR;
       uint8_t  RESERVED2[7];
  __IO uint8_t  SCR;
       uint8_t  RESERVED3[3];
  __IO uint32_t ACR;
  __IO uint8_t  ICR;
       uint8_t  RESERVED4[3];
  __IO uint8_t  FDR;
       uint8_t  RESERVED5[7];
  __IO uint8_t  TER;
} LPC_UART_TypeDef;

/** @brief  Universal Asynchronous Receiver Transmitter 1 (UART1) register structure definition */
typedef struct
{
  union {
  __I  uint8_t  RBR;
  __O  uint8_t  THR;
  __IO uint8_t  DLL;
       uint32_t RESERVED0;
  };
  union {
  __IO uint8_t  DLM;
  __IO uint32_t IER;
  };
  union {
  __I  uint32_t IIR;
  __O  uint8_t  FCR;
  };
  __IO uint8_t  LCR;
       uint8_t  RESERVED1[3];
  __IO uint8_t  MCR;
       uint8_t  RESERVED2[3];
  __I  uint8_t  LSR;
       uint8_t  RESERVED3[3];
  __I  uint8_t  MSR;
       uint8_t  RESERVED4[3];
  __IO uint8_t  SCR;
       uint8_t  RESERVED5[3];
  __IO uint32_t ACR;
       uint32_t RESERVED6;
  __IO uint32_t FDR;
       uint32_t RESERVED7;
  __IO uint8_t  TER;
       uint8_t  RESERVED8[27];
  __IO uint8_t  RS485CTRL;
       uint8_t  RESERVED9[3];
  __IO uint8_t  ADRMATCH;
       uint8_t  RESERVED10[3];
  __IO uint8_t  RS485DLY;
} LPC_UART1_TypeDef;

/*------------- Serial Peripheral Interface (SPI) ----------------------------*/
/** @brief  Serial Peripheral Interface (SPI) register structure definition */
typedef struct
{
  __IO uint32_t SPCR;
  __I  uint32_t SPSR;
  __IO uint32_t SPDR;
  __IO uint32_t SPCCR;
       uint32_t RESERVED0[3];
  __IO uint32_t SPINT;
} LPC_SPI_TypeDef;

/*------------- Synchronous Serial Communication (SSP) -----------------------*/
/** @brief  Synchronous Serial Communication (SSP) register structure definition */
typedef struct
{
  __IO uint32_t CR0;
  __IO uint32_t CR1;
  __IO uint32_t DR;
  __I  uint32_t SR;
  __IO uint32_t CPSR;
  __IO uint32_t IMSC;
  __IO uint32_t RIS;
  __IO uint32_t MIS;
  __IO uint32_t ICR;
  __IO uint32_t DMACR;
} LPC_SSP_TypeDef;

/*------------- Inter-Integrated Circuit (I2C) -------------------------------*/
/** @brief  Inter-Integrated Circuit (I2C) register structure definition */
typedef struct
{
  __IO uint32_t I2CONSET;
  __I  uint32_t I2STAT;
  __IO uint32_t I2DAT;
  __IO uint32_t I2ADR0;
  __IO uint32_t I2SCLH;
  __IO uint32_t I2SCLL;
  __O  uint32_t I2CONCLR;
  __IO uint32_t MMCTRL;
  __IO uint32_t I2ADR1;
  __IO uint32_t I2ADR2;
  __IO uint32_t I2ADR3;
  __I  uint32_t I2DATA_BUFFER;
  __IO uint32_t I2MASK0;
  __IO uint32_t I2MASK1;
  __IO uint32_t I2MASK2;
  __IO uint32_t I2MASK3;
} LPC_I2C_TypeDef;

/*------------- Inter IC Sound (I2S) -----------------------------------------*/
/** @brief  Inter IC Sound (I2S) register structure definition */
typedef struct
{
  __IO uint32_t I2SDAO;
  __IO uint32_t I2SDAI;
  __O  uint32_t I2STXFIFO;
  __I  uint32_t I2SRXFIFO;
  __I  uint32_t I2SSTATE;
  __IO uint32_t I2SDMA1;
  __IO uint32_t I2SDMA2;
  __IO uint32_t I2SIRQ;
  __IO uint32_t I2STXRATE;
  __IO uint32_t I2SRXRATE;
  __IO uint32_t I2STXBITRATE;
  __IO uint32_t I2SRXBITRATE;
  __IO uint32_t I2STXMODE;
  __IO uint32_t I2SRXMODE;
} LPC_I2S_TypeDef;

/*------------- Repetitive Interrupt Timer (RIT) -----------------------------*/
/** @brief  Repetitive Interrupt Timer (RIT) register structure definition */
typedef struct
{
  __IO uint32_t RICOMPVAL;
  __IO uint32_t RIMASK;
  __IO uint8_t  RICTRL;
       uint8_t  RESERVED0[3];
  __IO uint32_t RICOUNTER;
} LPC_RIT_TypeDef;

/*------------- Real-Time Clock (RTC) ----------------------------------------*/
/** @brief  Real-Time Clock (RTC) register structure definition */
typedef struct
{
  __IO uint8_t  ILR;
       uint8_t  RESERVED0[7];
  __IO uint8_t  CCR;
       uint8_t  RESERVED1[3];
  __IO uint8_t  CIIR;
       uint8_t  RESERVED2[3];
  __IO uint8_t  AMR;
       uint8_t  RESERVED3[3];
  __I  uint32_t CTIME0;
  __I  uint32_t CTIME1;
  __I  uint32_t CTIME2;
  __IO uint8_t  SEC;
       uint8_t  RESERVED4[3];
  __IO uint8_t  MIN;
       uint8_t  RESERVED5[3];
  __IO uint8_t  HOUR;
       uint8_t  RESERVED6[3];
  __IO uint8_t  DOM;
       uint8_t  RESERVED7[3];
  __IO uint8_t  DOW;
       uint8_t  RESERVED8[3];
  __IO uint16_t DOY;
       uint16_t RESERVED9;
  __IO uint8_t  MONTH;
       uint8_t  RESERVED10[3];
  __IO uint16_t YEAR;
       uint16_t RESERVED11;
  __IO uint32_t CALIBRATION;
  __IO uint32_t GPREG0;
  __IO uint32_t GPREG1;
  __IO uint32_t GPREG2;
  __IO uint32_t GPREG3;
  __IO uint32_t GPREG4;
  __IO uint8_t  RTC_AUXEN;
       uint8_t  RESERVED12[3];
  __IO uint8_t  RTC_AUX;
       uint8_t  RESERVED13[3];
  __IO uint8_t  ALSEC;
       uint8_t  RESERVED14[3];
  __IO uint8_t  ALMIN;
       uint8_t  RESERVED15[3];
  __IO uint8_t  ALHOUR;
       uint8_t  RESERVED16[3];
  __IO uint8_t  ALDOM;
       uint8_t  RESERVED17[3];
  __IO uint8_t  ALDOW;
       uint8_t  RESERVED18[3];
  __IO uint16_t ALDOY;
       uint16_t RESERVED19;
  __IO uint8_t  ALMON;
       uint8_t  RESERVED20[3];
  __IO uint16_t ALYEAR;
       uint16_t RESERVED21;
} LPC_RTC_TypeDef;

/*------------- Watchdog Timer (WDT) -----------------------------------------*/
/** @brief  Watchdog Timer (WDT) register structure definition */
typedef struct
{
  __IO uint8_t  WDMOD;
       uint8_t  RESERVED0[3];
  __IO uint32_t WDTC;
  __O  uint8_t  WDFEED;
       uint8_t  RESERVED1[3];
  __I  uint32_t WDTV;
  __IO uint32_t WDCLKSEL;
} LPC_WDT_TypeDef;

/*------------- Analog-to-Digital Converter (ADC) ----------------------------*/
/** @brief  Analog-to-Digital Converter (ADC) register structure definition */
typedef struct
{
  __IO uint32_t ADCR;
  __IO uint32_t ADGDR;
       uint32_t RESERVED0;
  __IO uint32_t ADINTEN;
  __I  uint32_t ADDR0;
  __I  uint32_t ADDR1;
  __I  uint32_t ADDR2;
  __I  uint32_t ADDR3;
  __I  uint32_t ADDR4;
  __I  uint32_t ADDR5;
  __I  uint32_t ADDR6;
  __I  uint32_t ADDR7;
  __I  uint32_t ADSTAT;
  __IO uint32_t ADTRM;
} LPC_ADC_TypeDef;

/*------------- Digital-to-Analog Converter (DAC) ----------------------------*/
/** @brief  Digital-to-Analog Converter (DAC) register structure definition */
typedef struct
{
  __IO uint32_t DACR;
  __IO uint32_t DACCTRL;
  __IO uint16_t DACCNTVAL;
} LPC_DAC_TypeDef;

/*------------- Motor Control Pulse-Width Modulation (MCPWM) -----------------*/
/** @brief  Motor Control Pulse-Width Modulation (MCPWM) register structure definition */
typedef struct
{
  __I  uint32_t MCCON;
  __O  uint32_t MCCON_SET;
  __O  uint32_t MCCON_CLR;
  __I  uint32_t MCCAPCON;
  __O  uint32_t MCCAPCON_SET;
  __O  uint32_t MCCAPCON_CLR;
  __IO uint32_t MCTIM0;
  __IO uint32_t MCTIM1;
  __IO uint32_t MCTIM2;
  __IO uint32_t MCPER0;
  __IO uint32_t MCPER1;
  __IO uint32_t MCPER2;
  __IO uint32_t MCPW0;
  __IO uint32_t MCPW1;
  __IO uint32_t MCPW2;
  __IO uint32_t MCDEADTIME;
  __IO uint32_t MCCCP;
  __IO uint32_t MCCR0;
  __IO uint32_t MCCR1;
  __IO uint32_t MCCR2;
  __I  uint32_t MCINTEN;
  __O  uint32_t MCINTEN_SET;
  __O  uint32_t MCINTEN_CLR;
  __I  uint32_t MCCNTCON;
  __O  uint32_t MCCNTCON_SET;
  __O  uint32_t MCCNTCON_CLR;
  __I  uint32_t MCINTFLAG;
  __O  uint32_t MCINTFLAG_SET;
  __O  uint32_t MCINTFLAG_CLR;
  __O  uint32_t MCCAP_CLR;
} LPC_MCPWM_TypeDef;

/*------------- Quadrature Encoder Interface (QEI) ---------------------------*/
/** @brief  Quadrature Encoder Interface (QEI) register structure definition */
typedef struct
{
  __O  uint32_t QEICON;
  __I  uint32_t QEISTAT;
  __IO uint32_t QEICONF;
  __I  uint32_t QEIPOS;
  __IO uint32_t QEIMAXPOS;
  __IO uint32_t CMPOS0;
  __IO uint32_t CMPOS1;
  __IO uint32_t CMPOS2;
  __I  uint32_t INXCNT;
  __IO uint32_t INXCMP;
  __IO uint32_t QEILOAD;
  __I  uint32_t QEITIME;
  __I  uint32_t QEIVEL;
  __I  uint32_t QEICAP;
  __IO uint32_t VELCOMP;
  __IO uint32_t FILTER;
       uint32_t RESERVED0[998];
  __O  uint32_t QEIIEC;
  __O  uint32_t QEIIES;
  __I  uint32_t QEIINTSTAT;
  __I  uint32_t QEIIE;
  __O  uint32_t QEICLR;
  __O  uint32_t QEISET;
} LPC_QEI_TypeDef;

/*------------- Controller Area Network (CAN) --------------------------------*/
/** @brief  Controller Area Network Acceptance Filter RAM (CANAF_RAM)structure definition */
typedef struct
{
  __IO uint32_t mask[512];              /* ID Masks                           */
} LPC_CANAF_RAM_TypeDef;

/** @brief  Controller Area Network Acceptance Filter(CANAF) register structure definition */
typedef struct                          /* Acceptance Filter Registers        */
{
  __IO uint32_t AFMR;
  __IO uint32_t SFF_sa;
  __IO uint32_t SFF_GRP_sa;
  __IO uint32_t EFF_sa;
  __IO uint32_t EFF_GRP_sa;
  __IO uint32_t ENDofTable;
  __I  uint32_t LUTerrAd;
  __I  uint32_t LUTerr;
  __IO uint32_t FCANIE;
  __IO uint32_t FCANIC0;
  __IO uint32_t FCANIC1;
} LPC_CANAF_TypeDef;

/** @brief  Controller Area Network Central (CANCR) register structure definition */
typedef struct                          /* Central Registers                  */
{
  __I  uint32_t CANTxSR;
  __I  uint32_t CANRxSR;
  __I  uint32_t CANMSR;
} LPC_CANCR_TypeDef;

/** @brief  Controller Area Network Controller (CAN) register structure definition */
typedef struct                          /* Controller Registers               */
{
  __IO uint32_t MOD;
  __O  uint32_t CMR;
  __IO uint32_t GSR;
  __I  uint32_t ICR;
  __IO uint32_t IER;
  __IO uint32_t BTR;
  __IO uint32_t EWL;
  __I  uint32_t SR;
  __IO uint32_t RFS;
  __IO uint32_t RID;
  __IO uint32_t RDA;
  __IO uint32_t RDB;
  __IO uint32_t TFI1;
  __IO uint32_t TID1;
  __IO uint32_t TDA1;
  __IO uint32_t TDB1;
  __IO uint32_t TFI2;
  __IO uint32_t TID2;
  __IO uint32_t TDA2;
  __IO uint32_t TDB2;
  __IO uint32_t TFI3;
  __IO uint32_t TID3;
  __IO uint32_t TDA3;
  __IO uint32_t TDB3;
} LPC_CAN_TypeDef;

/*------------- General Purpose Direct Memory Access (GPDMA) -----------------*/
/** @brief  General Purpose Direct Memory Access (GPDMA) register structure definition */
typedef struct                          /* Common Registers                   */
{
  __I  uint32_t DMACIntStat;
  __I  uint32_t DMACIntTCStat;
  __O  uint32_t DMACIntTCClear;
  __I  uint32_t DMACIntErrStat;
  __O  uint32_t DMACIntErrClr;
  __I  uint32_t DMACRawIntTCStat;
  __I  uint32_t DMACRawIntErrStat;
  __I  uint32_t DMACEnbldChns;
  __IO uint32_t DMACSoftBReq;
  __IO uint32_t DMACSoftSReq;
  __IO uint32_t DMACSoftLBReq;
  __IO uint32_t DMACSoftLSReq;
  __IO uint32_t DMACConfig;
  __IO uint32_t DMACSync;
} LPC_GPDMA_TypeDef;

/** @brief  General Purpose Direct Memory Access Channel (GPDMACH) register structure definition */
typedef struct                          /* Channel Registers                  */
{
  __IO uint32_t DMACCSrcAddr;
  __IO uint32_t DMACCDestAddr;
  __IO uint32_t DMACCLLI;
  __IO uint32_t DMACCControl;
  __IO uint32_t DMACCConfig;
} LPC_GPDMACH_TypeDef;

/*------------- Universal Serial Bus (USB) -----------------------------------*/
/** @brief  Universal Serial Bus (USB) register structure definition */
typedef struct
{
  __I  uint32_t HcRevision;             /* USB Host Registers                 */
  __IO uint32_t HcControl;
  __IO uint32_t HcCommandStatus;
  __IO uint32_t HcInterruptStatus;
  __IO uint32_t HcInterruptEnable;
  __IO uint32_t HcInterruptDisable;
  __IO uint32_t HcHCCA;
  __I  uint32_t HcPeriodCurrentED;
  __IO uint32_t HcControlHeadED;
  __IO uint32_t HcControlCurrentED;
  __IO uint32_t HcBulkHeadED;
  __IO uint32_t HcBulkCurrentED;
  __I  uint32_t HcDoneHead;
  __IO uint32_t HcFmInterval;
  __I  uint32_t HcFmRemaining;
  __I  uint32_t HcFmNumber;
  __IO uint32_t HcPeriodicStart;
  __IO uint32_t HcLSTreshold;
  __IO uint32_t HcRhDescriptorA;
  __IO uint32_t HcRhDescriptorB;
  __IO uint32_t HcRhStatus;
  __IO uint32_t HcRhPortStatus1;
  __IO uint32_t HcRhPortStatus2;
       uint32_t RESERVED0[40];
  __I  uint32_t Module_ID;

  __I  uint32_t OTGIntSt;               /* USB On-The-Go Registers            */
  __IO uint32_t OTGIntEn;
  __O  uint32_t OTGIntSet;
  __O  uint32_t OTGIntClr;
  __IO uint32_t OTGStCtrl;
  __IO uint32_t OTGTmr;
       uint32_t RESERVED1[58];

  __I  uint32_t USBDevIntSt;            /* USB Device Interrupt Registers     */
  __IO uint32_t USBDevIntEn;
  __O  uint32_t USBDevIntClr;
  __O  uint32_t USBDevIntSet;

  __O  uint32_t USBCmdCode;             /* USB Device SIE Command Registers   */
  __I  uint32_t USBCmdData;

  __I  uint32_t USBRxData;              /* USB Device Transfer Registers      */
  __O  uint32_t USBTxData;
  __I  uint32_t USBRxPLen;
  __O  uint32_t USBTxPLen;
  __IO uint32_t USBCtrl;
  __O  uint32_t USBDevIntPri;

  __I  uint32_t USBEpIntSt;             /* USB Device Endpoint Interrupt Regs */
  __IO uint32_t USBEpIntEn;
  __O  uint32_t USBEpIntClr;
  __O  uint32_t USBEpIntSet;
  __O  uint32_t USBEpIntPri;

  __IO uint32_t USBReEp;                /* USB Device Endpoint Realization Reg*/
  __O  uint32_t USBEpInd;
  __IO uint32_t USBMaxPSize;

  __I  uint32_t USBDMARSt;              /* USB Device DMA Registers           */
  __O  uint32_t USBDMARClr;
  __O  uint32_t USBDMARSet;
       uint32_t RESERVED2[9];
  __IO uint32_t USBUDCAH;
  __I  uint32_t USBEpDMASt;
  __O  uint32_t USBEpDMAEn;
  __O  uint32_t USBEpDMADis;
  __I  uint32_t USBDMAIntSt;
  __IO uint32_t USBDMAIntEn;
       uint32_t RESERVED3[2];
  __I  uint32_t USBEoTIntSt;
  __O  uint32_t USBEoTIntClr;
  __O  uint32_t USBEoTIntSet;
  __I  uint32_t USBNDDRIntSt;
  __O  uint32_t USBNDDRIntClr;
  __O  uint32_t USBNDDRIntSet;
  __I  uint32_t USBSysErrIntSt;
  __O  uint32_t USBSysErrIntClr;
  __O  uint32_t USBSysErrIntSet;
       uint32_t RESERVED4[15];

  union {
  __I  uint32_t I2C_RX;                 /* USB OTG I2C Registers              */
  __O  uint32_t I2C_TX;
  };
  __I  uint32_t I2C_STS;
  __IO uint32_t I2C_CTL;
  __IO uint32_t I2C_CLKHI;
  __O  uint32_t I2C_CLKLO;
       uint32_t RESERVED5[824];

  union {
  __IO uint32_t USBClkCtrl;             /* USB Clock Control Registers        */
  __IO uint32_t OTGClkCtrl;
  };
  union {
  __I  uint32_t USBClkSt;
  __I  uint32_t OTGClkSt;
  };
} LPC_USB_TypeDef;

/*------------- Ethernet Media Access Controller (EMAC) ----------------------*/
/** @brief  Ethernet Media Access Controller (EMAC) register structure definition */
typedef struct
{
  __IO uint32_t MAC1;                   /* MAC Registers                      */
  __IO uint32_t MAC2;
  __IO uint32_t IPGT;
  __IO uint32_t IPGR;
  __IO uint32_t CLRT;
  __IO uint32_t MAXF;
  __IO uint32_t SUPP;
  __IO uint32_t TEST;
  __IO uint32_t MCFG;
  __IO uint32_t MCMD;
  __IO uint32_t MADR;
  __O  uint32_t MWTD;
  __I  uint32_t MRDD;
  __I  uint32_t MIND;
       uint32_t RESERVED0[2];
  __IO uint32_t SA0;
  __IO uint32_t SA1;
  __IO uint32_t SA2;
       uint32_t RESERVED1[45];
  __IO uint32_t Command;                /* Control Registers                  */
  __I  uint32_t Status;
  __IO uint32_t RxDescriptor;
  __IO uint32_t RxStatus;
  __IO uint32_t RxDescriptorNumber;
  __I  uint32_t RxProduceIndex;
  __IO uint32_t RxConsumeIndex;
  __IO uint32_t TxDescriptor;
  __IO uint32_t TxStatus;
  __IO uint32_t TxDescriptorNumber;
  __IO uint32_t TxProduceIndex;
  __I  uint32_t TxConsumeIndex;
       uint32_t RESERVED2[10];
  __I  uint32_t TSV0;
  __I  uint32_t TSV1;
  __I  uint32_t RSV;
       uint32_t RESERVED3[3];
  __IO uint32_t FlowControlCounter;
  __I  uint32_t FlowControlStatus;
       uint32_t RESERVED4[34];
  __IO uint32_t RxFilterCtrl;           /* Rx Filter Registers                */
  __IO uint32_t RxFilterWoLStatus;
  __IO uint32_t RxFilterWoLClear;
       uint32_t RESERVED5;
  __IO uint32_t HashFilterL;
  __IO uint32_t HashFilterH;
       uint32_t RESERVED6[882];
  __I  uint32_t IntStatus;              /* Module Control Registers           */
  __IO uint32_t IntEnable;
  __O  uint32_t IntClear;
  __O  uint32_t IntSet;
       uint32_t RESERVED7;
  __IO uint32_t PowerDown;
       uint32_t RESERVED8;
  __IO uint32_t Module_ID;
} LPC_EMAC_TypeDef;


#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif


/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* Base addresses                                                             */
#define LPC_FLASH_BASE        (0x00000000UL)
#define LPC_RAM_BASE          (0x10000000UL)
#ifdef __LPC17XX_REV00
#define LPC_AHBRAM0_BASE      (0x20000000UL)
#define LPC_AHBRAM1_BASE      (0x20004000UL)
#else
#define LPC_AHBRAM0_BASE      (0x2007C000UL)
#define LPC_AHBRAM1_BASE      (0x20080000UL)
#endif
#define LPC_GPIO_BASE         (0x2009C000UL)
#define LPC_APB0_BASE         (0x40000000UL)
#define LPC_APB1_BASE         (0x40080000UL)
#define LPC_AHB_BASE          (0x50000000UL)
#define LPC_CM3_BASE          (0xE0000000UL)

/* APB0 peripherals                                                           */
#define LPC_WDT_BASE          (LPC_APB0_BASE + 0x00000)
#define LPC_TIM0_BASE         (LPC_APB0_BASE + 0x04000)
#define LPC_TIM1_BASE         (LPC_APB0_BASE + 0x08000)
#define LPC_UART0_BASE        (LPC_APB0_BASE + 0x0C000)
#define LPC_UART1_BASE        (LPC_APB0_BASE + 0x10000)
#define LPC_PWM1_BASE         (LPC_APB0_BASE + 0x18000)
#define LPC_I2C0_BASE         (LPC_APB0_BASE + 0x1C000)
#define LPC_SPI_BASE          (LPC_APB0_BASE + 0x20000)
#define LPC_RTC_BASE          (LPC_APB0_BASE + 0x24000)
#define LPC_GPIOINT_BASE      (LPC_APB0_BASE + 0x28080)
#define LPC_PINCON_BASE       (LPC_APB0_BASE + 0x2C000)
#define LPC_SSP1_BASE         (LPC_APB0_BASE + 0x30000)
#define LPC_ADC_BASE          (LPC_APB0_BASE + 0x34000)
#define LPC_CANAF_RAM_BASE    (LPC_APB0_BASE + 0x38000)
#define LPC_CANAF_BASE        (LPC_APB0_BASE + 0x3C000)
#define LPC_CANCR_BASE        (LPC_APB0_BASE + 0x40000)
#define LPC_CAN1_BASE         (LPC_APB0_BASE + 0x44000)
#define LPC_CAN2_BASE         (LPC_APB0_BASE + 0x48000)
#define LPC_I2C1_BASE         (LPC_APB0_BASE + 0x5C000)

/* APB1 peripherals                                                           */
#define LPC_SSP0_BASE         (LPC_APB1_BASE + 0x08000)
#define LPC_DAC_BASE          (LPC_APB1_BASE + 0x0C000)
#define LPC_TIM2_BASE         (LPC_APB1_BASE + 0x10000)
#define LPC_TIM3_BASE         (LPC_APB1_BASE + 0x14000)
#define LPC_UART2_BASE        (LPC_APB1_BASE + 0x18000)
#define LPC_UART3_BASE        (LPC_APB1_BASE + 0x1C000)
#define LPC_I2C2_BASE         (LPC_APB1_BASE + 0x20000)
#define LPC_I2S_BASE          (LPC_APB1_BASE + 0x28000)
#define LPC_RIT_BASE          (LPC_APB1_BASE + 0x30000)
#define LPC_MCPWM_BASE        (LPC_APB1_BASE + 0x38000)
#define LPC_QEI_BASE          (LPC_APB1_BASE + 0x3C000)
#define LPC_SC_BASE           (LPC_APB1_BASE + 0x7C000)

/* AHB peripherals                                                            */
#define LPC_EMAC_BASE         (LPC_AHB_BASE  + 0x00000)
#define LPC_GPDMA_BASE        (LPC_AHB_BASE  + 0x04000)
#define LPC_GPDMACH0_BASE     (LPC_AHB_BASE  + 0x04100)
#define LPC_GPDMACH1_BASE     (LPC_AHB_BASE  + 0x04120)
#define LPC_GPDMACH2_BASE     (LPC_AHB_BASE  + 0x04140)
#define LPC_GPDMACH3_BASE     (LPC_AHB_BASE  + 0x04160)
#define LPC_GPDMACH4_BASE     (LPC_AHB_BASE  + 0x04180)
#define LPC_GPDMACH5_BASE     (LPC_AHB_BASE  + 0x041A0)
#define LPC_GPDMACH6_BASE     (LPC_AHB_BASE  + 0x041C0)
#define LPC_GPDMACH7_BASE     (LPC_AHB_BASE  + 0x041E0)
#define LPC_USB_BASE          (LPC_AHB_BASE  + 0x0C000)

/* GPIOs                                                                      */
#define LPC_GPIO0_BASE        (LPC_GPIO_BASE + 0x00000)
#define LPC_GPIO1_BASE        (LPC_GPIO_BASE + 0x00020)
#define LPC_GPIO2_BASE        (LPC_GPIO_BASE + 0x00040)
#define LPC_GPIO3_BASE        (LPC_GPIO_BASE + 0x00060)
#define LPC_GPIO4_BASE        (LPC_GPIO_BASE + 0x00080)

/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
#define LPC_SC                ((LPC_SC_TypeDef        *) LPC_SC_BASE       )
#define LPC_GPIO0             ((LPC_GPIO_TypeDef      *) LPC_GPIO0_BASE    )
#define LPC_GPIO1             ((LPC_GPIO_TypeDef      *) LPC_GPIO1_BASE    )
#define LPC_GPIO2             ((LPC_GPIO_TypeDef      *) LPC_GPIO2_BASE    )
#define LPC_GPIO3             ((LPC_GPIO_TypeDef      *) LPC_GPIO3_BASE    )
#define LPC_GPIO4             ((LPC_GPIO_TypeDef      *) LPC_GPIO4_BASE    )
#define LPC_WDT               ((LPC_WDT_TypeDef       *) LPC_WDT_BASE      )
#define LPC_TIM0              ((LPC_TIM_TypeDef       *) LPC_TIM0_BASE     )
#define LPC_TIM1              ((LPC_TIM_TypeDef       *) LPC_TIM1_BASE     )
#define LPC_TIM2              ((LPC_TIM_TypeDef       *) LPC_TIM2_BASE     )
#define LPC_TIM3              ((LPC_TIM_TypeDef       *) LPC_TIM3_BASE     )
#define LPC_RIT               ((LPC_RIT_TypeDef       *) LPC_RIT_BASE      )
#define LPC_UART0             ((LPC_UART_TypeDef      *) LPC_UART0_BASE    )
#define LPC_UART1             ((LPC_UART1_TypeDef     *) LPC_UART1_BASE    )
#define LPC_UART2             ((LPC_UART_TypeDef      *) LPC_UART2_BASE    )
#define LPC_UART3             ((LPC_UART_TypeDef      *) LPC_UART3_BASE    )
#define LPC_PWM1              ((LPC_PWM_TypeDef       *) LPC_PWM1_BASE     )
#define LPC_I2C0              ((LPC_I2C_TypeDef       *) LPC_I2C0_BASE     )
#define LPC_I2C1              ((LPC_I2C_TypeDef       *) LPC_I2C1_BASE     )
#define LPC_I2C2              ((LPC_I2C_TypeDef       *) LPC_I2C2_BASE     )
#define LPC_I2S               ((LPC_I2S_TypeDef       *) LPC_I2S_BASE      )
#define LPC_SPI               ((LPC_SPI_TypeDef       *) LPC_SPI_BASE      )
#define LPC_RTC               ((LPC_RTC_TypeDef       *) LPC_RTC_BASE      )
#define LPC_GPIOINT           ((LPC_GPIOINT_TypeDef   *) LPC_GPIOINT_BASE  )
#define LPC_PINCON            ((LPC_PINCON_TypeDef    *) LPC_PINCON_BASE   )
#define LPC_SSP0              ((LPC_SSP_TypeDef       *) LPC_SSP0_BASE     )
#define LPC_SSP1              ((LPC_SSP_TypeDef       *) LPC_SSP1_BASE     )
#define LPC_ADC               ((LPC_ADC_TypeDef       *) LPC_ADC_BASE      )
#define LPC_DAC               ((LPC_DAC_TypeDef       *) LPC_DAC_BASE      )
#define LPC_CANAF_RAM         ((LPC_CANAF_RAM_TypeDef *) LPC_CANAF_RAM_BASE)
#define LPC_CANAF             ((LPC_CANAF_TypeDef     *) LPC_CANAF_BASE    )
#define LPC_CANCR             ((LPC_CANCR_TypeDef     *) LPC_CANCR_BASE    )
#define LPC_CAN1              ((LPC_CAN_TypeDef       *) LPC_CAN1_BASE     )
#define LPC_CAN2              ((LPC_CAN_TypeDef       *) LPC_CAN2_BASE     )
#define LPC_MCPWM             ((LPC_MCPWM_TypeDef     *) LPC_MCPWM_BASE    )
#define LPC_QEI               ((LPC_QEI_TypeDef       *) LPC_QEI_BASE      )
#define LPC_EMAC              ((LPC_EMAC_TypeDef      *) LPC_EMAC_BASE     )
#define LPC_GPDMA             ((LPC_GPDMA_TypeDef     *) LPC_GPDMA_BASE    )
#define LPC_GPDMACH0          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH0_BASE )
#define LPC_GPDMACH1          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH1_BASE )
#define LPC_GPDMACH2          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH2_BASE )
#define LPC_GPDMACH3          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH3_BASE )
#define LPC_GPDMACH4          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH4_BASE )
#define LPC_GPDMACH5          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH5_BASE )
#define LPC_GPDMACH6          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH6_BASE )
#define LPC_GPDMACH7          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH7_BASE )
#define LPC_USB               ((LPC_USB_TypeDef       *) LPC_USB_BASE      )

//PINSEL0 register
#define PINSEL0_P0_0_MASK           0x00000003
#define PINSEL0_P0_0_GPIO           0x00000000
#define PINSEL0_P0_0_RD1            0x00000001
#define PINSEL0_P0_0_TXD3           0x00000002
#define PINSEL0_P0_0_SDA1           0x00000003

#define PINSEL0_P0_1_MASK           0x0000000C
#define PINSEL0_P0_1_GPIO           0x00000000
#define PINSEL0_P0_1_TD1            0x00000004
#define PINSEL0_P0_1_RXD3           0x00000008
#define PINSEL0_P0_1_SCL1           0x0000000C

#define PINSEL0_P0_2_MASK           0x00000030
#define PINSEL0_P0_2_GPIO           0x00000000
#define PINSEL0_P0_2_TXD0           0x00000010
#define PINSEL0_P0_2_AD0_7          0x00000020

#define PINSEL0_P0_3_MASK           0x000000C0
#define PINSEL0_P0_3_GPIO           0x00000000
#define PINSEL0_P0_3_RXD0           0x00000040
#define PINSEL0_P0_3_AD0_6          0x00000080

#define PINSEL0_P0_4_MASK           0x00000300
#define PINSEL0_P0_4_GPIO           0x00000000
#define PINSEL0_P0_4_I2SRX_CLK      0x00000100
#define PINSEL0_P0_4_RD2            0x00000200
#define PINSEL0_P0_4_CAP2_0         0x00000300

#define PINSEL0_P0_5_MASK           0x00000C00
#define PINSEL0_P0_5_GPIO           0x00000000
#define PINSEL0_P0_5_I2SRX_WS       0x00000400
#define PINSEL0_P0_5_TD2            0x00000800
#define PINSEL0_P0_5_CAP2_1         0x00000C00

#define PINSEL0_P0_6_MASK           0x00003000
#define PINSEL0_P0_6_GPIO           0x00000000
#define PINSEL0_P0_6_I2SRX_SDA      0x00001000
#define PINSEL0_P0_6_SSEL1          0x00002000
#define PINSEL0_P0_6_MAT2_0         0x00003000

#define PINSEL0_P0_7_MASK           0x0000C000
#define PINSEL0_P0_7_GPIO           0x00000000
#define PINSEL0_P0_7_I2STX_CLK      0x00004000
#define PINSEL0_P0_7_SCK1           0x00008000
#define PINSEL0_P0_7_MAT2_1         0x0000C000

#define PINSEL0_P0_8_MASK           0x00030000
#define PINSEL0_P0_8_GPIO           0x00000000
#define PINSEL0_P0_8_I2STX_WS       0x00010000
#define PINSEL0_P0_8_MISO1          0x00020000
#define PINSEL0_P0_8_MAT2_2         0x00030000

#define PINSEL0_P0_9_MASK           0x000C0000
#define PINSEL0_P0_9_GPIO           0x00000000
#define PINSEL0_P0_9_I2STX_SDA      0x00040000
#define PINSEL0_P0_9_MOSI1          0x00080000
#define PINSEL0_P0_9_MAT2_3         0x000C0000

#define PINSEL0_P0_10_MASK          0x00300000
#define PINSEL0_P0_10_GPIO          0x00000000
#define PINSEL0_P0_10_TXD2          0x00100000
#define PINSEL0_P0_10_SDA2          0x00200000
#define PINSEL0_P0_10_MAT3_0        0x00300000

#define PINSEL0_P0_11_MASK          0x00C00000
#define PINSEL0_P0_11_GPIO          0x00000000
#define PINSEL0_P0_11_RXD2          0x00400000
#define PINSEL0_P0_11_SCL2          0x00800000
#define PINSEL0_P0_11_MAT3_1        0x00C00000

#define PINSEL0_P0_15_MASK          0xC0000000
#define PINSEL0_P0_15_GPIO          0x00000000
#define PINSEL0_P0_15_TXD1          0x40000000
#define PINSEL0_P0_15_SCK0          0x80000000
#define PINSEL0_P0_15_SCK           0xC0000000

//PINSEL1 register
#define PINSEL1_P0_16_MASK          0x00000003
#define PINSEL1_P0_16_GPIO          0x00000000
#define PINSEL1_P0_16_RXD1          0x00000001
#define PINSEL1_P0_16_SSEL0         0x00000002
#define PINSEL1_P0_16_SSEL          0x00000003

#define PINSEL1_P0_17_MASK          0x0000000C
#define PINSEL1_P0_17_GPIO          0x00000000
#define PINSEL1_P0_17_CTS1          0x00000004
#define PINSEL1_P0_17_MISO0         0x00000008
#define PINSEL1_P0_17_MISO          0x0000000C

#define PINSEL1_P0_18_MASK          0x00000030
#define PINSEL1_P0_18_GPIO          0x00000000
#define PINSEL1_P0_18_DCD1          0x00000010
#define PINSEL1_P0_18_MOSI0         0x00000020
#define PINSEL1_P0_18_MOSI          0x00000030

#define PINSEL1_P0_19_MASK          0x000000C0
#define PINSEL1_P0_19_GPIO          0x00000000
#define PINSEL1_P0_19_DSR1          0x00000040
#define PINSEL1_P0_19_SDA1          0x000000C0

#define PINSEL1_P0_20_MASK          0x00000300
#define PINSEL1_P0_20_GPIO          0x00000000
#define PINSEL1_P0_20_DTR1          0x00000100
#define PINSEL1_P0_20_SCL1          0x00000300

#define PINSEL1_P0_21_MASK          0x00000C00
#define PINSEL1_P0_21_GPIO          0x00000000
#define PINSEL1_P0_21_RI1           0x00000400
#define PINSEL1_P0_21_RD1           0x00000C00

#define PINSEL1_P0_22_MASK          0x00003000
#define PINSEL1_P0_22_GPIO          0x00000000
#define PINSEL1_P0_22_RTS1          0x00001000
#define PINSEL1_P0_22_TD1           0x00003000

#define PINSEL1_P0_23_MASK          0x0000C000
#define PINSEL1_P0_23_GPIO          0x00000000
#define PINSEL1_P0_23_AD0_0         0x00004000
#define PINSEL1_P0_23_I2SRX_CLK     0x00008000
#define PINSEL1_P0_23_CAP3_0        0x0000C000

#define PINSEL1_P0_24_MASK          0x00030000
#define PINSEL1_P0_24_GPIO          0x00000000
#define PINSEL1_P0_24_AD0_1         0x00010000
#define PINSEL1_P0_24_I2SRX_WS      0x00020000
#define PINSEL1_P0_24_CAP3_1        0x00030000

#define PINSEL1_P0_25_MASK          0x000C0000
#define PINSEL1_P0_25_GPIO          0x00000000
#define PINSEL1_P0_25_AD0_2         0x00040000
#define PINSEL1_P0_25_I2SRX_SDA     0x00080000
#define PINSEL1_P0_25_TXD3          0x000C0000

#define PINSEL1_P0_26_MASK          0x00300000
#define PINSEL1_P0_26_GPIO          0x00000000
#define PINSEL1_P0_26_AD0_3         0x00100000
#define PINSEL1_P0_26_AOUT          0x00200000
#define PINSEL1_P0_26_RXD3          0x00300000

#define PINSEL1_P0_27_MASK          0x00C00000
#define PINSEL1_P0_27_GPIO          0x00000000
#define PINSEL1_P0_27_SDA0          0x00400000
#define PINSEL1_P0_27_USB_SDA       0x00800000

#define PINSEL1_P0_28_MASK          0x03000000
#define PINSEL1_P0_28_GPIO          0x00000000
#define PINSEL1_P0_28_SCL0          0x01000000
#define PINSEL1_P0_28_USB_SCL       0x02000000

#define PINSEL1_P0_29_MASK          0x0C000000
#define PINSEL1_P0_29_GPIO          0x00000000
#define PINSEL1_P0_29_USB_DP        0x04000000

#define PINSEL1_P0_30_MASK          0x30000000
#define PINSEL1_P0_30_GPIO          0x00000000
#define PINSEL1_P0_30_USB_DM        0x10000000

//PINSEL2 register
#define PINSEL2_P1_0_MASK           0x00000003
#define PINSEL2_P1_0_GPIO           0x00000000
#define PINSEL2_P1_0_ENET_TXD0      0x00000001

#define PINSEL2_P1_1_MASK           0x0000000C
#define PINSEL2_P1_1_GPIO           0x00000000
#define PINSEL2_P1_1_ENET_TXD1      0x00000004

#define PINSEL2_P1_4_MASK           0x00000300
#define PINSEL2_P1_4_GPIO           0x00000000
#define PINSEL2_P1_4_ENET_TX_EN     0x00000100

#define PINSEL2_P1_8_MASK           0x00030000
#define PINSEL2_P1_8_GPIO           0x00000000
#define PINSEL2_P1_8_ENET_CRS       0x00010000

#define PINSEL2_P1_9_MASK           0x000C0000
#define PINSEL2_P1_9_GPIO           0x00000000
#define PINSEL2_P1_9_ENET_RXD0      0x00040000

#define PINSEL2_P1_10_MASK          0x00300000
#define PINSEL2_P1_10_GPIO          0x00000000
#define PINSEL2_P1_10_ENET_RXD1     0x00100000

#define PINSEL2_P1_14_MASK          0x30000000
#define PINSEL2_P1_14_GPIO          0x00000000
#define PINSEL2_P1_14_ENET_RX_ER    0x10000000

#define PINSEL2_P1_15_MASK          0xC0000000
#define PINSEL2_P1_15_GPIO          0x00000000
#define PINSEL2_P1_15_ENET_REF_CLK  0x40000000

//PINSEL3 register
#define PINSEL3_P1_16_MASK          0x00000003
#define PINSEL3_P1_16_GPIO          0x00000000
#define PINSEL3_P1_16_ENET_MDC      0x00000001

#define PINSEL3_P1_17_MASK          0x0000000C
#define PINSEL3_P1_17_GPIO          0x00000000
#define PINSEL3_P1_17_ENET_MDIO     0x00000004

#define PINSEL3_P1_18_MASK          0x00000030
#define PINSEL3_P1_18_GPIO          0x00000000
#define PINSEL3_P1_18_USB_UP_LED    0x00000010
#define PINSEL3_P1_18_PWM1_1        0x00000020
#define PINSEL3_P1_18_CAP1_0        0x00000030

#define PINSEL3_P1_19_MASK          0x000000C0
#define PINSEL3_P1_19_GPIO          0x00000000
#define PINSEL3_P1_19_MCOA0         0x00000040
#define PINSEL3_P1_19_USB_PPWR      0x00000080
#define PINSEL3_P1_19_CAP1_1        0x000000C0

#define PINSEL3_P1_20_MASK          0x00000300
#define PINSEL3_P1_20_GPIO          0x00000000
#define PINSEL3_P1_20_MCI0          0x00000100
#define PINSEL3_P1_20_PWM1_2        0x00000200
#define PINSEL3_P1_20_SCK0          0x00000300

#define PINSEL3_P1_21_MASK          0x00000C00
#define PINSEL3_P1_21_GPIO          0x00000000
#define PINSEL3_P1_21_MCABORT       0x00000400
#define PINSEL3_P1_21_PWM1_3        0x00000800
#define PINSEL3_P1_21_SSEL0         0x00000C00

#define PINSEL3_P1_22_MASK          0x00003000
#define PINSEL3_P1_22_GPIO          0x00000000
#define PINSEL3_P1_22_MCOB0         0x00001000
#define PINSEL3_P1_22_USB_PWRD      0x00002000
#define PINSEL3_P1_22_MAT1_0        0x00003000

#define PINSEL3_P1_23_MASK          0x0000C000
#define PINSEL3_P1_23_GPIO          0x00000000
#define PINSEL3_P1_23_MCI1          0x00004000
#define PINSEL3_P1_23_PWM1_4        0x00008000
#define PINSEL3_P1_23_MISO0         0x0000C000

#define PINSEL3_P1_24_MASK          0x00030000
#define PINSEL3_P1_24_GPIO          0x00000000
#define PINSEL3_P1_24_MCI2          0x00010000
#define PINSEL3_P1_24_PWM1_5        0x00020000
#define PINSEL3_P1_24_MOSI0         0x00030000

#define PINSEL3_P1_25_MASK          0x000C0000
#define PINSEL3_P1_25_GPIO          0x00000000
#define PINSEL3_P1_25_MCOA1         0x00040000
#define PINSEL3_P1_25_RESERVED      0x00080000
#define PINSEL3_P1_25_MAT1_1        0x000C0000

#define PINSEL3_P1_26_MASK          0x00300000
#define PINSEL3_P1_26_GPIO          0x00000000
#define PINSEL3_P1_26_MCOB1         0x00100000
#define PINSEL3_P1_26_PWM1_6        0x00200000
#define PINSEL3_P1_26_CAP0_0        0x00300000

#define PINSEL3_P1_27_MASK          0x00C00000
#define PINSEL3_P1_27_GPIO          0x00000000
#define PINSEL3_P1_27_CLKOUT        0x00400000
#define PINSEL3_P1_27_USB_OVRCR     0x00800000
#define PINSEL3_P1_27_CAP0_1        0x00C00000

#define PINSEL3_P1_28_MASK          0x03000000
#define PINSEL3_P1_28_GPIO          0x00000000
#define PINSEL3_P1_28_MCOA2         0x01000000
#define PINSEL3_P1_28_PCAP1_0       0x02000000
#define PINSEL3_P1_28_MAT0_0        0x03000000

#define PINSEL3_P1_29_MASK          0x0C000000
#define PINSEL3_P1_29_GPIO          0x00000000
#define PINSEL3_P1_29_MCOB2         0x04000000
#define PINSEL3_P1_29_PCAP1_1       0x08000000
#define PINSEL3_P1_29_MAT0_1        0x0C000000

#define PINSEL3_P1_30_MASK          0x30000000
#define PINSEL3_P1_30_GPIO          0x00000000
#define PINSEL3_P1_30_RESERVED      0x10000000
#define PINSEL3_P1_30_VBUS          0x20000000
#define PINSEL3_P1_30_AD0_4         0x30000000

#define PINSEL3_P1_31_MASK          0xC0000000
#define PINSEL3_P1_31_GPIO          0x00000000
#define PINSEL3_P1_31_RESERVED      0x40000000
#define PINSEL3_P1_31_SCK1          0x80000000
#define PINSEL3_P1_31_AD0_5         0xC0000000

//PINSEL4 register
#define PINSEL4_P2_0_MASK           0x00000003
#define PINSEL4_P2_0_GPIO           0x00000000
#define PINSEL4_P2_0_PWM1_1         0x00000001
#define PINSEL4_P2_0_TXD1           0x00000002

#define PINSEL4_P2_1_MASK           0x0000000C
#define PINSEL4_P2_1_GPIO           0x00000000
#define PINSEL4_P2_1_PWM1_2         0x00000004
#define PINSEL4_P2_1_RXD1           0x00000008

#define PINSEL4_P2_2_MASK           0x00000030
#define PINSEL4_P2_2_GPIO           0x00000000
#define PINSEL4_P2_2_PWM1_3         0x00000010
#define PINSEL4_P2_2_CTS1           0x00000020

#define PINSEL4_P2_3_MASK           0x000000C0
#define PINSEL4_P2_3_GPIO           0x00000000
#define PINSEL4_P2_3_PWM1_4         0x00000040
#define PINSEL4_P2_3_DCD1           0x00000080

#define PINSEL4_P2_4_MASK           0x00000300
#define PINSEL4_P2_4_GPIO           0x00000000
#define PINSEL4_P2_4_PWM1_5         0x00000100
#define PINSEL4_P2_4_DSR1           0x00000200

#define PINSEL4_P2_5_MASK           0x00000C00
#define PINSEL4_P2_5_GPIO           0x00000000
#define PINSEL4_P2_5_PWM1_6         0x00000400
#define PINSEL4_P2_5_DTR1           0x00000800

#define PINSEL4_P2_6_MASK           0x00003000
#define PINSEL4_P2_6_GPIO           0x00000000
#define PINSEL4_P2_6_PCAP1_0        0x00001000
#define PINSEL4_P2_6_RI1            0x00002000

#define PINSEL4_P2_7_MASK           0x0000C000
#define PINSEL4_P2_7_GPIO           0x00000000
#define PINSEL4_P2_7_RD2            0x00004000
#define PINSEL4_P2_7_RTS1           0x00008000

#define PINSEL4_P2_8_MASK           0x00030000
#define PINSEL4_P2_8_GPIO           0x00000000
#define PINSEL4_P2_8_TD2            0x00010000
#define PINSEL4_P2_8_TXD2           0x00020000
#define PINSEL4_P2_8_ENET_MDC       0x00030000

#define PINSEL4_P2_9_MASK           0x000C0000
#define PINSEL4_P2_9_GPIO           0x00000000
#define PINSEL4_P2_9_USB_CONNECT    0x00040000
#define PINSEL4_P2_9_RXD2           0x00080000
#define PINSEL4_P2_9_ENET_MDIO      0x000C0000

#define PINSEL4_P2_10_MASK          0x00300000
#define PINSEL4_P2_10_GPIO          0x00000000
#define PINSEL4_P2_10_EINT0         0x00100000
#define PINSEL4_P2_10_NMI           0x00200000

#define PINSEL4_P2_11_MASK          0x00C00000
#define PINSEL4_P2_11_GPIO          0x00000000
#define PINSEL4_P2_11_EINT1         0x00400000
#define PINSEL4_P2_11_I2STX_CLK     0x00C00000

#define PINSEL4_P2_12_MASK          0x03000000
#define PINSEL4_P2_12_GPIO          0x00000000
#define PINSEL4_P2_12_EINT2         0x01000000
#define PINSEL4_P2_12_I2STX_WS      0x03000000

#define PINSEL4_P2_13_MASK          0x0C000000
#define PINSEL4_P2_13_GPIO          0x00000000
#define PINSEL4_P2_13_EINT3         0x04000000
#define PINSEL4_P2_13_I2STX_SDA     0x0C000000

//PINSEL7 register
#define PINSEL7_P3_25_MASK          0x000C0000
#define PINSEL7_P3_25_GPIO          0x00000000
#define PINSEL7_P3_25_MAT0_0        0x00080000
#define PINSEL7_P3_25_PWM1_2        0x000C0000

#define PINSEL7_P3_26_MASK          0x00300000
#define PINSEL7_P3_26_GPIO          0x00000000
#define PINSEL7_P3_26_STCLK         0x00100000
#define PINSEL7_P3_26_MAT0_1        0x00200000
#define PINSEL7_P3_26_PWM1_3        0x00300000

//PINSEL9 register
#define PINSEL9_P4_28_MASK          0x03000000
#define PINSEL9_P4_28_GPIO          0x00000000
#define PINSEL9_P4_28_RX_MCLK       0x01000000
#define PINSEL9_P4_28_MAT2_0        0x02000000
#define PINSEL9_P4_28_TXD3          0x03000000

#define PINSEL9_P4_29_MASK          0x0C000000
#define PINSEL9_P4_29_GPIO          0x00000000
#define PINSEL9_P4_29_TX_MCLK       0x04000000
#define PINSEL9_P4_29_MAT2_1        0x08000000
#define PINSEL9_P4_29_RXD3          0x0C000000

//FLASHCFG register
#define FLASHCFG_RESET_VALUE        0x0000003A
#define FLASHCFG_FLASHTIM           0x0000F000
#define FLASHCFG_FLASHTIM_1CLK      0x00000000
#define FLASHCFG_FLASHTIM_2CLK      0x00001000
#define FLASHCFG_FLASHTIM_3CLK      0x00002000
#define FLASHCFG_FLASHTIM_4CLK      0x00003000
#define FLASHCFG_FLASHTIM_5CLK      0x00004000
#define FLASHCFG_FLASHTIM_6CLK      0x00005000

//CLKSRCSEL register
#define CLKSRCSEL_CLKSRC            0x00000003
#define CLKSRCSEL_CLKSRC_IRC        0x00000000
#define CLKSRCSEL_CLKSRC_OSC        0x00000001
#define CLKSRCSEL_CLKSRC_RTC        0x00000002

//SCS register
#define SCS_OSCSTAT                 0x00000040
#define SCS_OSCEN                   0x00000020
#define SCS_OSCRANGE                0x00000010

//PLL0CON register
#define PLL0CON_PLLC0               0x00000002
#define PLL0CON_PLLE0               0x00000001

//PLL0CFG register
#define PLL0CFG_NSEL0               0x00FF0000
#define PLL0CFG_MSEL0               0x00007FFF

//PLL0STAT register
#define PLL0STAT_PLOCK0             0x04000000
#define PLL0STAT_PLLC0_STAT         0x02000000
#define PLL0STAT_PLLE0_STAT         0x01000000
#define PLL0STAT_NSEL0              0x00FF0000
#define PLL0STAT_MSEL0              0x00007FFF

//CLKOUTCFG register
#define CLKOUTCFG_CLKOUT_ACT        0x00000200
#define CLKOUTCFG_CLKOUT_EN         0x00000100
#define CLKOUTCFG_CLKOUTDIV         0x000000F0
#define CLKOUTCFG_CLKOUTSEL         0x0000000F
#define CLKOUTCFG_CLKOUTSEL_CCLK    0x00000000
#define CLKOUTCFG_CLKOUTSEL_OSC     0x00000001
#define CLKOUTCFG_CLKOUTSEL_IRC     0x00000002
#define CLKOUTCFG_CLKOUTSEL_USB     0x00000003
#define CLKOUTCFG_CLKOUTSEL_RTC     0x00000004

//PCLKSEL0 register
#define PCLKSEL0_PCLK_UART0         0x000000C0
#define PCLKSEL0_PCLK_UART0_DIV4    0x00000000
#define PCLKSEL0_PCLK_UART0_DIV1    0x00000040
#define PCLKSEL0_PCLK_UART0_DIV2    0x00000080
#define PCLKSEL0_PCLK_UART0_DIV8    0x000000C0

#define PCLKSEL0_PCLK_CAN1          0x0C000000
#define PCLKSEL0_PCLK_CAN1_DIV4     0x00000000
#define PCLKSEL0_PCLK_CAN1_DIV1     0x04000000
#define PCLKSEL0_PCLK_CAN1_DIV2     0x08000000
#define PCLKSEL0_PCLK_CAN1_DIV6     0x0C000000

#define PCLKSEL0_PCLK_CAN2          0x30000000
#define PCLKSEL0_PCLK_CAN2_DIV4     0x00000000
#define PCLKSEL0_PCLK_CAN2_DIV1     0x10000000
#define PCLKSEL0_PCLK_CAN2_DIV2     0x20000000
#define PCLKSEL0_PCLK_CAN2_DIV6     0x30000000

#define PCLKSEL0_PCLK_ACF           0xC0000000
#define PCLKSEL0_PCLK_ACF_DIV4      0x00000000
#define PCLKSEL0_PCLK_ACF_DIV1      0x40000000
#define PCLKSEL0_PCLK_ACF_DIV2      0x80000000
#define PCLKSEL0_PCLK_ACF_DIV6      0xC0000000

//PCLKSEL1 register
#define PCLKSEL1_PCLK_SSP0          0x00000C00
#define PCLKSEL1_PCLK_SSP0_DIV4     0x00000000
#define PCLKSEL1_PCLK_SSP0_DIV1     0x00000400
#define PCLKSEL1_PCLK_SSP0_DIV2     0x00000800
#define PCLKSEL1_PCLK_SSP0_DIV8     0x00000C00

#define PCLKSEL1_PCLK_I2C2          0x00300000
#define PCLKSEL1_PCLK_I2C2_DIV4     0x00000000
#define PCLKSEL1_PCLK_I2C2_DIV1     0x00100000
#define PCLKSEL1_PCLK_I2C2_DIV2     0x00200000
#define PCLKSEL1_PCLK_I2C2_DIV8     0x00300000

#define PCLKSEL1_PCLK_I2S           0x00C00000
#define PCLKSEL1_PCLK_I2S_DIV4      0x00000000
#define PCLKSEL1_PCLK_I2S_DIV1      0x00400000
#define PCLKSEL1_PCLK_I2S_DIV2      0x00800000
#define PCLKSEL1_PCLK_I2S_DIV8      0x00C00000

#define PCLKSEL1_PCLK_RIT           0x0C000000
#define PCLKSEL1_PCLK_RIT_DIV4      0x00000000
#define PCLKSEL1_PCLK_RIT_DIV1      0x04000000
#define PCLKSEL1_PCLK_RIT_DIV2      0x08000000
#define PCLKSEL1_PCLK_RIT_DIV8      0x0C000000

#define PCLKSEL1_PCLK_SYSCON        0x30000000
#define PCLKSEL1_PCLK_SYSCON_DIV4   0x00000000
#define PCLKSEL1_PCLK_SYSCON_DIV1   0x10000000
#define PCLKSEL1_PCLK_SYSCON_DIV2   0x20000000
#define PCLKSEL1_PCLK_SYSCON_DIV8   0x30000000

#define PCLKSEL1_PCLK_MC            0xC0000000
#define PCLKSEL1_PCLK_MC_DIV4       0x00000000
#define PCLKSEL1_PCLK_MC_DIV1       0x40000000
#define PCLKSEL1_PCLK_MC_DIV2       0x80000000
#define PCLKSEL1_PCLK_MC_DIV8       0xC0000000

//PCONP register
#define PCONP_PCUSB                 0x80000000
#define PCONP_PCENET                0x40000000
#define PCONP_PCGPDMA               0x20000000
#define PCONP_PCI2S                 0x08000000
#define PCONP_PCI2C2                0x04000000
#define PCONP_PCUART3               0x02000000
#define PCONP_PCUART2               0x01000000
#define PCONP_PCTIM3                0x00800000
#define PCONP_PCTIM2                0x00400000
#define PCONP_PCSSP0                0x00200000
#define PCONP_PCI2C1                0x00080000
#define PCONP_PCQEI                 0x00040000
#define PCONP_PCMCPWM               0x00020000
#define PCONP_PCRIT                 0x00010000
#define PCONP_PCGPIO                0x00008000
#define PCONP_PCCAN2                0x00004000
#define PCONP_PCCAN1                0x00002000
#define PCONP_PCADC                 0x00001000
#define PCONP_PCSSP1                0x00000400
#define PCONP_PCRTC                 0x00000200
#define PCONP_PCSPI                 0x00000100
#define PCONP_PCI2C0                0x00000080
#define PCONP_PCPWM1                0x00000040
#define PCONP_PCUART1               0x00000010
#define PCONP_PCUART0               0x00000008
#define PCONP_PCTIM1                0x00000004
#define PCONP_PCTIM0                0x00000002

//FCR register
#define FCR_RX_TRIGGER_LEVEL        0x000000C0
#define FCR_RX_TRIGGER_LEVEL_0      0x00000000
#define FCR_RX_TRIGGER_LEVEL_1      0x00000040
#define FCR_RX_TRIGGER_LEVEL_2      0x00000080
#define FCR_RX_TRIGGER_LEVEL_3      0x000000C0

#define FCR_DMA_MODE_SELECT         0x00000008
#define FCR_TX_FIFO_RESET           0x00000004
#define FCR_RX_FIFO_RESET           0x00000002
#define FCR_FIFO_ENABLE             0x00000001

//LCR register
#define LCR_DLAB                    0x00000080
#define LCR_BREAK_CONTROL           0x00000040

#define LCR_PARITY_SELECT           0x00000030
#define LCR_PARITY_SELECT_ODD       0x00000000
#define LCR_PARITY_SELECT_EVEN      0x00000010
#define LCR_PARITY_SELECT_1         0x00000020
#define LCR_PARITY_SELECT_0         0x00000030

#define LCR_PARITY_ENABLE           0x00000008

#define LCR_STOP_BIT_SELECT         0x00000004
#define LCR_STOP_BIT_SELECT_1       0x00000000
#define LCR_STOP_BIT_SELECT_2       0x00000004

#define LCR_WORD_LENGTH_SELECT      0x00000003
#define LCR_WORD_LENGTH_SELECT_5    0x00000000
#define LCR_WORD_LENGTH_SELECT_6    0x00000001
#define LCR_WORD_LENGTH_SELECT_7    0x00000002
#define LCR_WORD_LENGTH_SELECT_8    0x00000003

//LSR register
#define LSR_RXFE                    0x00000080
#define LSR_TEMT                    0x00000040
#define LSR_THRE                    0x00000020
#define LSR_BI                      0x00000010
#define LSR_FE                      0x00000008
#define LSR_PE                      0x00000004
#define LSR_OE                      0x00000002
#define LSR_RDR                     0x00000001

//CR0 register
#define CR0_DSS                     0x0000000F
#define CR0_DSS_4                   0x00000003
#define CR0_DSS_5                   0x00000004
#define CR0_DSS_6                   0x00000005
#define CR0_DSS_7                   0x00000006
#define CR0_DSS_8                   0x00000007
#define CR0_DSS_9                   0x00000008
#define CR0_DSS_10                  0x00000009
#define CR0_DSS_11                  0x0000000A
#define CR0_DSS_12                  0x0000000B
#define CR0_DSS_13                  0x0000000C
#define CR0_DSS_14                  0x0000000D
#define CR0_DSS_15                  0x0000000E
#define CR0_DSS_16                  0x0000000F

#define CR0_FRF                     0x00000030
#define CR0_FRF_SPI                 0x00000000
#define CR0_FRF_TI                  0x00000010
#define CR0_FRF_MICROWIRE           0x00000020

#define CR0_CPOL                    0x00000040
#define CR0_CPHA                    0x00000080
#define CR0_SCR                     0x0000FF00

//CR1 register
#define CR1_LBM                     0x00000001
#define CR1_SSE                     0x00000002
#define CR1_MS                      0x00000004
#define CR1_SOD                     0x00000008

//DR register
#define DR_DATA                     0x0000FFFF

//SR register
#define SR_TFE                      0x00000001
#define SR_TNF                      0x00000002
#define SR_RNE                      0x00000004
#define SR_RFF                      0x00000008
#define SR_BSY                      0x00000010

//CPSR register
#define CPSR_CPSDVSR                0x000000FF

//IMSC register
#define IMSC_RORIM                  0x00000001
#define IMSC_RTIM                   0x00000002
#define IMSC_RXIM                   0x00000004
#define IMSC_TXIM                   0x00000008

//RIS register
#define RIS_RORRIS                  0x00000001
#define RIS_RTRIS                   0x00000002
#define RIS_RXRIS                   0x00000004
#define RIS_TXRIS                   0x00000008

//MIS register
#define MIS_RORMIS                  0x00000001
#define MIS_RTMIS                   0x00000002
#define MIS_RXMIS                   0x00000004
#define MIS_TXMIS                   0x00000008

//ICR register
#define ICR_RORIC                   0x00000001
#define ICR_RTIC                    0x00000002

//DMACR register
#define DMACR_RXDMAE                0x00000001
#define DMACR_TXDMAE                0x00000002

//I2CONSET register
#define I2CONSET_AA                 0x00000004
#define I2CONSET_SI                 0x00000008
#define I2CONSET_STO                0x00000010
#define I2CONSET_STA                0x00000020
#define I2CONSET_I2EN               0x00000040

//I2CONCLR register
#define I2CONCLR_AAC                0x00000004
#define I2CONCLR_SIC                0x00000008
#define I2CONCLR_STAC               0x00000020
#define I2CONCLR_I2ENC              0x00000040

//I2STAT register
#define I2STAT_STATUS               0x000000F8

//I2DAT register
#define I2DAT_DATA                  0x000000FF

//I2MMCTRL register
#define I2MMCTRL_MM_ENA             0x00000001
#define I2MMCTRL_ENA_SCL            0x00000002
#define I2MMCTRL_MATCH_ALL          0x00000004

//I2DATA_BUFFER register
#define I2DATA_BUFFER_DATA          0x000000FF

//I2ADR0 register
#define I2ADR0_GC                   0x00000001
#define I2ADR0_ADDRESS              0x000000FE

//I2ADR1 register
#define I2ADR1_GC                   0x00000001
#define I2ADR1_ADDRESS              0x000000FE

//I2ADR2 register
#define I2ADR2_GC                   0x00000001
#define I2ADR2_ADDRESS              0x000000FE

//I2ADR3 register
#define I2ADR3_GC                   0x00000001
#define I2ADR3_ADDRESS              0x000000FE

//I2MASK0 register
#define I2MASK0_MASK                0x000000FE

//I2MASK1 register
#define I2MASK1_MASK                0x000000FE

//I2MASK2 register
#define I2MASK2_MASK                0x000000FE

//I2MASK3 register
#define I2MASK3_MASK                0x000000FE

//I2SCLH register
#define I2SCLH_SCLH                 0x0000FFFF

//I2SCLL register
#define I2SCLL_SCLL                 0x0000FFFF


/**
 * @}
 */

#endif  // __LPC17xx_H__
