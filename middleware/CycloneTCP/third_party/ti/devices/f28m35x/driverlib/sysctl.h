//###########################################################################
// FILE:   sysctl.h
// TITLE:  Prototypes for the system control driver.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright:
// Copyright (C) 2011-2015 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//###########################################################################

#ifndef __SYSCTL_H__
#define __SYSCTL_H__

//*****************************************************************************
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif


#ifndef TRUE
#define TRUE     1
#endif

#ifndef FALSE
#define FALSE    0
#endif

//*****************************************************************************
// The following define is used throughout the ControlSUITE drivers to 
// correctly set timing and delays.  Please set its definition to your external
// clock input frequency in Herz.
//*****************************************************************************
#define SYSTEM_CLOCK_SPEED                20000000      // 20MHz default

//*****************************************************************************
// The following are values that can be passed to the
// SysCtlPeripheralPresent(), SysCtlPeripheralEnable(),
// SysCtlPeripheralDisable(), and SysCtlPeripheralReset() APIs as the
// ulPeripheral parameter.
// bit  31 - 24 = peripheral index (0, 1, 2, 3, 4)
// bits 23 - 16 = shift
// bits 15 - 0  = bit value in peripheral control registers (SRCR, DC,
//                RCGC, etc) this value will be shifted as per the shift
//                stored in bits 23:16
//*****************************************************************************
#define SYSCTL_PERIPH_WDOG0               0x00000008    // Watchdog 0
#define SYSCTL_PERIPH_WDOG1               0x00101000    // Watchdog 1

#define SYSCTL_PERIPH_UART0               0x10000001    // UART 0
#define SYSCTL_PERIPH_UART1               0x10000002    // UART 1
#define SYSCTL_PERIPH_UART2               0x10000004    // UART 2
#define SYSCTL_PERIPH_UART3               0x10000008    // UART 3
#define SYSCTL_PERIPH_UART4               0x30000001    // UART4
#define SYSCTL_PERIPH_SSI0                0x10000010    // SSI 0
#define SYSCTL_PERIPH_SSI1                0x10000020    // SSI 1
#define SYSCTL_PERIPH_SSI2                0x10000040    // SSI 2
#define SYSCTL_PERIPH_SSI3                0x10000080    // SSI 3
#define SYSCTL_PERIPH_I2C0                0x10001000    // I2C 0
#define SYSCTL_PERIPH_I2C1                0x10004000    // I2C 1
#define SYSCTL_PERIPH_TIMER0              0x10100001    // Timer 0
#define SYSCTL_PERIPH_TIMER1              0x10100002    // Timer 1
#define SYSCTL_PERIPH_TIMER2              0x10100004    // Timer 2
#define SYSCTL_PERIPH_TIMER3              0x10100008    // Timer 3
#define SYSCTL_PERIPH_EPI0                0x10104000    // EPI0

#define SYSCTL_PERIPH_GPIOA               0x20000001    // GPIO A
#define SYSCTL_PERIPH_GPIOB               0x20000002    // GPIO B
#define SYSCTL_PERIPH_GPIOC               0x20000004    // GPIO C
#define SYSCTL_PERIPH_GPIOD               0x20000008    // GPIO D
#define SYSCTL_PERIPH_GPIOE               0x20000010    // GPIO E
#define SYSCTL_PERIPH_GPIOF               0x20000020    // GPIO F
#define SYSCTL_PERIPH_GPIOG               0x20000040    // GPIO G
#define SYSCTL_PERIPH_GPIOH               0x20000080    // GPIO H
#define SYSCTL_PERIPH_GPIOJ               0x20000100    // GPIO J
#define SYSCTL_PERIPH_UDMA                0x20002000    // uDMA
#define SYSCTL_PERIPH_USB0                0x20100001    // USB0
#define SYSCTL_PERIPH_ETH                 0x20101000    // ETH
#define SYSCTL_PERIPH_IEEE1588            0x20100100    // IEEE1588

#define SYSCTL_PERIPH_CAN0                0x30100100    // CAN 0
#define SYSCTL_PERIPH_CAN1                0x30100200    // CAN 1

#define SYSCTL_PERIPH_GPIOK               0x40000200    // GPIO K
#define SYSCTL_PERIPH_GPIOL               0x40000400    // GPIO L
#define SYSCTL_PERIPH_GPIOM               0x40000800    // GPIO M
#define SYSCTL_PERIPH_GPION               0x40001000    // GPIO N
#define SYSCTL_PERIPH_GPIOP               0x40002000    // GPIO P
#define SYSCTL_PERIPH_GPIOQ               0x40004000    // GPIO Q
#define SYSCTL_PERIPH_GPIOR               0x40008000    // GPIO R
#define SYSCTL_PERIPH_GPIOS               0x40100001    // GPIO S

//*****************************************************************************
// The following are values that can be passed to the SysCtlResetCauseClear()
// API or returned by the SysCtlResetCauseGet() API.
//*****************************************************************************
#define SYSCTL_CAUSE_SW                   0x00000010    // Software reset
#define SYSCTL_CAUSE_WDOG                 0x00000008    // Watchdog reset
#define SYSCTL_CAUSE_POR                  0x00000002    // Power on reset
#define SYSCTL_CAUSE_EXT                  0x00000001    // External reset

//*****************************************************************************
// The following are values that can be passed to the SysCtlClockConfigSet() 
// API as the ulConfig parameter.
//*****************************************************************************
#define SYSCTL_SYSDIV_1                   0x00000000    // C28ClkIn clock is
                                                        // osc/pll /1
#define SYSCTL_SYSDIV_2                   0x00100000    // C28ClkIn clock is
                                                        // osc/pll /2
#define SYSCTL_SYSDIV_4                   0x00200000    // C28ClkIn clock is
                                                        // osc/pll /4
#define SYSCTL_SYSDIV_8                   0x00300000    // C28ClkIn clock is
                                                        // osc/pll /8
#define SYSCTL_SYSDIV_NO_CHANGE           0xFFFFFFFF    // Leave the divider at
                                                        // it current value
#define SYSCTL_SYSDIV_M                   0x00300000    // System divider mask
#define SYSCTL_SYSDIV_M_SHIFT             20            // System divider shift

#define SYSCTL_M3SSDIV_1                  0x00000000    // M3 clock is C28ClkIn
                                                        // /1
#define SYSCTL_M3SSDIV_2                  0x00040000    // M3 clock is C28ClkIn
                                                        // /2
#define SYSCTL_M3SSDIV_4                  0x00080000    // M3 clock is C28ClkIn
                                                        // /4
#define SYSCTL_M3SSDIV_NO_CHANGE          0xFFFFFFFF    // Leave the divider at
                                                        // it current value
#define SYSCTL_M3SSDIV_M                  0x000C0000    // M3 divider mask
#define SYSCTL_M3SSDIV_M_SHIFT            18

#define SYSCTL_XCLKDIV_1                  0x00010000    // XCLK clock is
                                                        // C28ClkIn /1
#define SYSCTL_XCLKDIV_2                  0x00020000    // XCLK clock is
                                                        // C28ClkIn /2
#define SYSCTL_XCLKDIV_4                  0x00030000    // XCLK clock is
                                                        // C28ClkIn /4
#define SYSCTL_XCLKDIV_OFF                0x00000000    // XCLK clock is off
#define SYSCTL_XCLKDIV_M                  0x00030000    // XCLK divider mask
#define SYSCTL_XCLKDIV_M_SHIFT            16

#define SYSCTL_USE_PLL                    0x80000000    // System clock is the
                                                        // PLL clock
#define SYSCTL_USE_OSC                    0x00000000    // System clock is the
                                                        // osc clock
#define SYSCTL_USE_PLL_SHIFT              31

#define SYSCTL_SPLLIMULT_M                0x0000007F    // PLL Integer
                                                        // Multiplier mask
#define SYSCTL_SPLLFMULT_M                0x00000300    // PLL Fractional
                                                        // Multiplier mask
#define SYSCTL_MCLK_DETECTION_EN          0x00000001    // Enable Missing clock
                                                        // detection logic
#define SYSCTL_MCLK_DETECTION_DIS         0x00000000    // Disable Missing 
                                                        // clock detection logic


//*****************************************************************************
// The following are values that can be passed to the SysCtlUSBPLLConfigSet()
// API as the ulConfig parameter.
//*****************************************************************************
#define SYSCTL_UPLLIMULT_M                0x0000003F    // USB PLL Integer
                                                        // Multiplier mask
#define SYSCTL_UPLLFMULT_M                0x00000300    // USB PLL Fractional
                                                        // Multiplier mask

#define SYSCTL_UPLLCLKSRC_X1              0x00000000    // USB PLL sourced from
                                                        // X1 clock
#define SYSCTL_UPLLCLKSRC_XCLKIN          0x80000000    // USB PLL sourced from
                                                        // GPIO XClock IN
#define SYSCTL_UPLLCLKSRC_XCLKIN_SHIFT    31            // USB PLL sourced from
                                                        // GPIO XClock IN

#define SYSCTL_UPLLEN                     0x40000000    // USB PLL enabled


//******************************************************************************
// The following are values that can be passed to the SycCtl Subsystem reset
// functions as the ulResetConf parameter
//******************************************************************************
#define SYSCTL_CONTROL_SYSTEM_RES_CNF     0x00000001    //CRESCNF bit for
                                                        //ControlSubsystem
#define SYSCTL_ANALOG_SYSTEM_RES_CNF      0x00000002    //CRESCNF bit for
                                                        //Analog System
#define SYSCTL_RES_CNF_MASK               0x00000003
#define SYSCTL_RES_CNF_SHIFT              16

//*****************************************************************************
// The following are values that can be passed to the AnalogClockEnable(),
// AnalogClockDisable(), and ReadAnalogClockStatus()functions as the 
// AnalogConfigReg parameter.
//*****************************************************************************
#define AnalogConfig1                     1             // Choose Analog 
                                                        // Config1 Register
#define AnalogConfig2                     2             // Choose Analog 
                                                        // Config2 Register

//*****************************************************************************
// The following are values that can be passed to the AnalogClockEnable() and
// AnalogClockDisable()functions as the AnalogClockMask parameter.
//*****************************************************************************
#define ADC1_ENABLE                       0x0008        // Mask to Enable ADC1 
                                                        // (AnalogConfig1 
                                                        // Register)
#define ADC2_ENABLE                       0x8000        // Mask to Enable ADC2 
                                                        // (AnalogConfig2 
                                                        // Register)
#define COMP1_ENABLE                      0x0001        // Mask to Enable COMP1
                                                        // (AnalogConfig2 
                                                        // Register)
#define COMP2_ENABL                       0x0002        // Mask to Enable COMP2
                                                        // (AnalogConfig2 
                                                        // Register)
#define COMP3_ENABL                       0x0004        // Mask to Enable COMP3
                                                        // (AnalogConfig2 
                                                        // Register)
#define COMP4_ENABLE                      0x0008        // Mask to Enable COMP4
                                                        // (AnalogConfig2 
                                                        // Register)
#define COMP5_ENABL                       0x0010        // Mask to Enable COMP5
                                                        // (AnalogConfig2 
                                                        // Register)
#define COMP6_ENABL                       0x0020        // Mask to Enable COMP6
                                                        // (AnalogConfig2 
                                                        // Register)
#define ANALOGCONFIG2AL                   0x803F        // Mask to Enable ADC2,
                                                        // COMP1,2,3,4,5,6 
                                                        // (AnalogConfig2 
                                                        // Register)

//*****************************************************************************
// Analog System Functions
//*****************************************************************************
#define AnalogClockEnable (unsigned short   (**)(unsigned short AnalogConfigReg, unsigned short AnalogClockMask))0x006804b4
#define AnalogClockDisable (unsigned short   (**)(unsigned short AnalogConfigReg, unsigned short AnalogClockMask))0x006804b8
#define ReadAnalogClockStatus (unsigned short (**)(unsigned short AnalogConfigReg))0x006804bC


//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************
extern tBoolean SysCtlPeripheralPresent(unsigned long ulPeripheral);
extern void SysCtlPeripheralReset(unsigned long ulPeripheral);
extern void SysCtlPeripheralEnable(unsigned long ulPeripheral);
extern void SysCtlPeripheralDisable(unsigned long ulPeripheral);
extern tBoolean SysCtlPeripheralIsEnabled(unsigned long ulPeripheral);
extern void SysCtlPeripheralSleepEnable(unsigned long ulPeripheral);
extern void SysCtlPeripheralSleepDisable(unsigned long ulPeripheral);
extern void SysCtlPeripheralDeepSleepEnable(unsigned long ulPeripheral);
extern void SysCtlPeripheralDeepSleepDisable(unsigned long ulPeripheral);
extern void SysCtlPeripheralClockGating(tBoolean bEnable);
extern void SysCtlIntRegister(void (*pfnHandler)(void));
extern void SysCtlIntUnregister(void);
extern void SysCtlReset(void);
extern void SysCtlSleep(void);
extern void SysCtlDeepSleep(void);
extern unsigned long SysCtlResetCauseGet(void);
extern void SysCtlResetCauseClear(unsigned long ulCauses);
extern void SysCtlDelay(unsigned long ulCount);
extern void SysCtlClockConfigSet(unsigned long ulConfig);
extern unsigned long SysCtlClockConfigGet(void);
extern unsigned long SysCtlClockGet(unsigned long ulClockIn);
extern void SysCtlC28Enable(void);
extern void SysCtlC28Disable(void);
extern void SysCtlGPIOAHBEnable(unsigned long ulGPIOPeripheral);
extern void SysCtlGPIOAHBDisable(unsigned long ulGPIOPeripheral);
extern void SysCtlUSBPLLEnable(void);
extern void SysCtlUSBPLLDisable(void);
extern void SysCtlUSBPLLConfigSet(unsigned long ulConfig);
extern unsigned long SysCtlUSBPLLConfigGet(void);
extern void SysCtlEnableAERRNMI(void);
extern unsigned long SysCtlGetNmiConfig();
extern tBoolean SysCtlGetNmiInterruptStatus();
extern unsigned long SysCtlGetNmiFlagStatus();
extern tBoolean SysCtlNmiIsSet(unsigned long ulNmis);
extern void SysCtlNmiFlgClr(unsigned long ulNmis);
extern void SysCtlNmiFlgClrAll();
extern void SysCtlHoldSubSystemInReset(unsigned long ulResetConf);
extern void SysCtlReleaseSubSystemFromReset(unsigned long ulResetConf);
extern void SysCtlSubSystemReset();
extern void SysCtlClockDividersSet(unsigned long SysDivSelReq, unsigned long M3ClkDivReq);
extern void SysCtlXPllClockDividerSet(unsigned long XPllClkDivReq);
extern void SysCtlPowerOffPLL(void);
extern tBoolean SysCtlClockPllConfig(unsigned long ClkSrcReq, unsigned long PllMult);


//*****************************************************************************
// Mark the end of the C bindings section for C++ compilers.
//*****************************************************************************
#ifdef __cplusplus
}
#endif


#endif // __SYSCTL_H__




