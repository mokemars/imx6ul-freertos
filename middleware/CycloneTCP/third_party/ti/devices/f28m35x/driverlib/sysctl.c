//###########################################################################
// FILE:   sysctl.c
// TITLE:  Driver for the system controller.
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

//*****************************************************************************
//! \addtogroup sysctl_api
//! @{
//*****************************************************************************

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_nmi.h"
#include "driverlib/cpu.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/watchdog.h"

//*****************************************************************************
// This macro extracts the array index out of the peripheral number.
//*****************************************************************************
#define SYSCTL_PERIPH_INDEX(a)   (((a) >> 28) & 0xf)

//*****************************************************************************
// This macro constructs the peripheral bit mask from the peripheral number.
//*****************************************************************************
#define SYSCTL_PERIPH_MASK(a)    (((a) & 0xffff) << (((a) & 0x001f0000) >> 16))

//*****************************************************************************
// These macros are used for PLL clock configuration
//*****************************************************************************
#define SYSCTL_SYSDIV_STEP  0x00100000
#define SYSCTL_M3SSDIV_STEP 0x00040000

//*****************************************************************************
// An array that maps the "peripheral set" number (which is stored in the upper
// nibble of the SYSCTL_PERIPH_* defines) to the SYSCTL DC? register that
// contains the peripheral present bit for that peripheral.
//*****************************************************************************
static const unsigned long g_pulDCRegs[] =
{
    SYSCTL_DC1,
    SYSCTL_DC2,
    SYSCTL_DC4,
    SYSCTL_DC6,
    SYSCTL_PPGPIO
};

//*****************************************************************************
// An array that maps the "peripheral set" number (which is stored in the upper
// nibble of the SYSCTL_PERIPH_* defines) to the SYSCTL_SRCR? register that
// controls the software reset for that peripheral.
//*****************************************************************************
static const unsigned long g_pulSRCRRegs[] =
{
    SYSCTL_SRCR0,
    SYSCTL_SRCR1,
    SYSCTL_SRCR2,
    SYSCTL_SRCR3,
    SYSCTL_SRGPIO
};

//*****************************************************************************
// An array that maps the "peripheral set" number (which is stored in the upper
// nibble of the SYSCTL_PERIPH_* defines) to the SYSCTL_RCGC? register that
// controls the run-mode enable for that peripheral.
//*****************************************************************************
static const unsigned long g_pulRCGCRegs[] =
{
    SYSCTL_RCGC0,
    SYSCTL_RCGC1,
    SYSCTL_RCGC2,
    SYSCTL_RCGC3,
    SYSCTL_RCGCGPIO
};

//*****************************************************************************
// An array that maps the "peripheral set" number (which is stored in the upper
// nibble of the SYSCTL_PERIPH_* defines) to the SYSCTL_SCGC? register that
// controls the sleep-mode enable for that peripheral.
//*****************************************************************************
static const unsigned long g_pulSCGCRegs[] =
{
    SYSCTL_SCGC0,
    SYSCTL_SCGC1,
    SYSCTL_SCGC2,
    SYSCTL_SCGC3,
    SYSCTL_SCGCGPIO
};

//*****************************************************************************
// An array that maps the "peripheral set" number (which is stored in the upper
// nibble of the SYSCTL_PERIPH_* defines) to the SYSCTL_DCGC? register that
// controls the deep-sleep-mode enable for that peripheral.
//*****************************************************************************
static const unsigned long g_pulDCGCRegs[] =
{
    SYSCTL_DCGC0,
    SYSCTL_DCGC1,
    SYSCTL_DCGC2,
    SYSCTL_DCGC3,
    SYSCTL_DCGCGPIO
};

//*****************************************************************************
//! Gets the size of the shared RAM
//!
//! This function determines the size of the shared RAM on the Concerto
//! devices.
//!
//! \return The total number of bytes of shared RAM.
//*****************************************************************************
unsigned long
SysCtlSRAMSizeGet(void)
{
    unsigned long ulTemp;
    unsigned long ulSize;
    unsigned char i;

    // Read shared RAM configuration.
    ulTemp = HWREG(SYSCTL_MEMCNF);

    // Compute the size of the Shared RAM.
    for(i=0; i<32; i++) {
        if(ulTemp & 0x01)
            ulSize += 0x2000;
        ulTemp = ulTemp >> 1;
    }
    // Return the size
    return ulSize;
}

//*****************************************************************************
//! Gets the size of the flash.
//!
//! This function determines the size of the flash on the Concerto device.
//!
//! \return The total number of bytes of flash.
//*****************************************************************************
unsigned long
SysCtlFlashSizeGet(void)
{
    // Compute the size of the flash.
    //TODO: Add math to calculate number of bytes
    return(HWREG(SYSCTL_MCNF) & SYSCTL_MCNF_FLASH_M);
}

//*****************************************************************************
//! \internal
//! Checks a peripheral identifier.
//!
//! \param ulPeripheral is the peripheral identifier.
//!
//! This function determines if a peripheral identifier is valid.
//!
//! \return Returns \b true if the peripheral identifier is valid and \b false
//! otherwise.
//*****************************************************************************
#ifdef DEBUG
static tBoolean
SysCtlPeripheralValid(unsigned long ulPeripheral)
{
    return((ulPeripheral == SYSCTL_PERIPH_CAN0) ||
           (ulPeripheral == SYSCTL_PERIPH_CAN1) ||
           (ulPeripheral == SYSCTL_PERIPH_EPI0) ||
           (ulPeripheral == SYSCTL_PERIPH_ETH) ||
           (ulPeripheral == SYSCTL_PERIPH_GPIOA) ||
           (ulPeripheral == SYSCTL_PERIPH_GPIOB) ||
           (ulPeripheral == SYSCTL_PERIPH_GPIOC) ||
           (ulPeripheral == SYSCTL_PERIPH_GPIOD) ||
           (ulPeripheral == SYSCTL_PERIPH_GPIOE) ||
           (ulPeripheral == SYSCTL_PERIPH_GPIOF) ||
           (ulPeripheral == SYSCTL_PERIPH_GPIOG) ||
           (ulPeripheral == SYSCTL_PERIPH_GPIOH) ||
           (ulPeripheral == SYSCTL_PERIPH_GPIOJ) ||
           (ulPeripheral == SYSCTL_PERIPH_I2C0) ||
           (ulPeripheral == SYSCTL_PERIPH_I2C1) ||
           (ulPeripheral == SYSCTL_PERIPH_IEEE1588) ||
           (ulPeripheral == SYSCTL_PERIPH_SSI0) ||
           (ulPeripheral == SYSCTL_PERIPH_SSI1) ||
           (ulPeripheral == SYSCTL_PERIPH_SSI2) ||
           (ulPeripheral == SYSCTL_PERIPH_SSI3) ||
           (ulPeripheral == SYSCTL_PERIPH_TIMER0) ||
           (ulPeripheral == SYSCTL_PERIPH_TIMER1) ||
           (ulPeripheral == SYSCTL_PERIPH_TIMER2) ||
           (ulPeripheral == SYSCTL_PERIPH_TIMER3) ||
           (ulPeripheral == SYSCTL_PERIPH_UART0) ||
           (ulPeripheral == SYSCTL_PERIPH_UART1) ||
           (ulPeripheral == SYSCTL_PERIPH_UART2) ||
           (ulPeripheral == SYSCTL_PERIPH_UART3) ||
           (ulPeripheral == SYSCTL_PERIPH_UART4) ||
           (ulPeripheral == SYSCTL_PERIPH_UDMA) ||
           (ulPeripheral == SYSCTL_PERIPH_USB0) ||
           (ulPeripheral == SYSCTL_PERIPH_WDOG0) ||
           (ulPeripheral == SYSCTL_PERIPH_WDOG1) ||
           (ulPeripheral == SYSCTL_PERIPH_GPIOK) ||
           (ulPeripheral == SYSCTL_PERIPH_GPIOL) ||
           (ulPeripheral == SYSCTL_PERIPH_GPIOM) ||
           (ulPeripheral == SYSCTL_PERIPH_GPION) ||
           (ulPeripheral == SYSCTL_PERIPH_GPIOP) ||
           (ulPeripheral == SYSCTL_PERIPH_GPIOQ) ||
           (ulPeripheral == SYSCTL_PERIPH_GPIOR) ||
           (ulPeripheral == SYSCTL_PERIPH_GPIOS));
}

#endif

//*****************************************************************************
//! Determines if a peripheral is present.
//!
//! \param ulPeripheral is the peripheral in question.
//!
//! Determines if a particular peripheral is present in the device.  Each
//! member of the Concerto family has a different peripheral set; this will
//! determine which are present on this device.
//!
//! The \e ulPeripheral parameter must be only one of the following values:
//! \b SYSCTL_PERIPH_CAN0,   \b SYSCTL_PERIPH_CAN1,   \b SYSCTL_PERIPH_EPI0,
//! \b SYSCTL_PERIPH_ETH,    \b SYSCTL_PERIPH_GPIOA,  \b SYSCTL_PERIPH_GPIOB,
//! \b SYSCTL_PERIPH_GPIOC,  \b SYSCTL_PERIPH_GPIOD,  \b SYSCTL_PERIPH_GPIOE,
//! \b SYSCTL_PERIPH_GPIOF,  \b SYSCTL_PERIPH_GPIOG,  \b SYSCTL_PERIPH_GPIOH,
//! \b SYSCTL_PERIPH_GPIOJ,  \b SYSCTL_PERIPH_I2C0,   \b SYSCTL_PERIPH_I2C1,
//! \b SYSCTL_PERIPH_IEEE1588, \b SYSCTL_PERIPH_SSI0, \b SYSCTL_PERIPH_SSI1,
//! \b SYSCTL_PERIPH_SSI2,   \b SYSCTL_PERIPH_SSI3    \b SYSCTL_PERIPH_TIMER0,
//! \b SYSCTL_PERIPH_TIMER1, \b SYSCTL_PERIPH_TIMER2, \b SYSCTL_PERIPH_TIMER3,
//! \b SYSCTL_PERIPH_UART0,  \b SYSCTL_PERIPH_UART1,  \b SYSCTL_PERIPH_UART2,
//! \b SYSCTL_PERIPH_UART3, \b SYSCTL_PERIPH_UART4,  \b SYSCTL_PERIPH_UDMA,
//! \b SYSCTL_PERIPH_USB0,  \b SYSCTL_PERIPH_WDOG0, \b SYSCTL_PERIPH_WDOG1,
//! \b SYSCTL_PERIPH_GPIOK, \b SYSCTL_PERIPH_GPIOL, \b SYSCTL_PERIPH_GPIOM,
//! \b SYSCTL_PERIPH_GPION, \b SYSCTL_PERIPH_GPIOP, \b SYSCTL_PERIPH_GPIOQ,
//! \b SYSCTL_PERIPH_GPIOR, or \b SYSCTL_PERIPH_GPIOS.
//!
//! \return Returns \b true if the specified peripheral is present and \b false
//! if it is not.
//*****************************************************************************
tBoolean
SysCtlPeripheralPresent(unsigned long ulPeripheral)
{
    // Check the arguments.
    ASSERT(SysCtlPeripheralValid(ulPeripheral));

    // Read the correct DC register and determine if this peripheral exists.
    if(ulPeripheral == SYSCTL_PERIPH_USB0)
    {
        // USB is a special case since the DC bit is missing for USB0.
        if(HWREG(SYSCTL_DC6) & SYSCTL_DC6_USB0_M)
        {
            return(true);
        }
        else
        {
            return(false);
        }
    }
    else if(ulPeripheral == SYSCTL_PERIPH_CAN0)
    {
        if(HWREG(SYSCTL_DC10) & SYSCTL_DC10_CAN0)
        {
            return(true);
        }
        else
        {
            return(false);
        }

    }
    else if (ulPeripheral == SYSCTL_PERIPH_CAN1)
    {
        if(HWREG(SYSCTL_DC10) & SYSCTL_DC10_CAN1)
        {
            return(true);
        }
        else
        {
            return(false);
        }
    }
    else if (ulPeripheral == SYSCTL_PERIPH_UART4)
    {
        if(HWREG(SYSCTL_DC10) & SYSCTL_DC10_UART4)
        {
            return(true);
        }
        else
        {
            return(false);
        }
    }
    else if(HWREG(g_pulDCRegs[SYSCTL_PERIPH_INDEX(ulPeripheral)]) &
            SYSCTL_PERIPH_MASK(ulPeripheral))
    {
        return(true);
    }
    else
    {
        return(false);
    }
}

//*****************************************************************************
//! Performs a software reset of a peripheral.
//!
//! \param ulPeripheral is the peripheral to reset.
//!
//! This function performs a software reset of the specified peripheral.  An
//! individual peripheral reset signal is asserted for a brief period and then
//! deasserted, leaving the peripheral in a operating state but in its reset
//! condition.
//!
//! The \e ulPeripheral parameter must be only one of the following values:
//! \b SYSCTL_PERIPH_CAN0,   \b SYSCTL_PERIPH_CAN1,   \b SYSCTL_PERIPH_EPI0,
//! \b SYSCTL_PERIPH_ETH,    \b SYSCTL_PERIPH_GPIOA,  \b SYSCTL_PERIPH_GPIOB,
//! \b SYSCTL_PERIPH_GPIOC,  \b SYSCTL_PERIPH_GPIOD,  \b SYSCTL_PERIPH_GPIOE,
//! \b SYSCTL_PERIPH_GPIOF,  \b SYSCTL_PERIPH_GPIOG,  \b SYSCTL_PERIPH_GPIOH,
//! \b SYSCTL_PERIPH_GPIOJ,  \b SYSCTL_PERIPH_I2C0,   \b SYSCTL_PERIPH_I2C1,
//! \b SYSCTL_PERIPH_IEEE1588, \b SYSCTL_PERIPH_SSI0, \b SYSCTL_PERIPH_SSI1,
//! \b SYSCTL_PERIPH_SSI2,   \b SYSCTL_PERIPH_SSI3    \b SYSCTL_PERIPH_TIMER0,
//! \b SYSCTL_PERIPH_TIMER1, \b SYSCTL_PERIPH_TIMER2, \b SYSCTL_PERIPH_TIMER3,
//! \b SYSCTL_PERIPH_UART0,  \b SYSCTL_PERIPH_UART1,  \b SYSCTL_PERIPH_UART2,
//! \b SYSCTL_PERIPH_UART3, \b SYSCTL_PERIPH_UART4,  \b SYSCTL_PERIPH_UDMA,
//! \b SYSCTL_PERIPH_USB0,  \b SYSCTL_PERIPH_WDOG0, \b SYSCTL_PERIPH_WDOG1,
//! \b SYSCTL_PERIPH_GPIOK, \b SYSCTL_PERIPH_GPIOL, \b SYSCTL_PERIPH_GPIOM,
//! \b SYSCTL_PERIPH_GPION, \b SYSCTL_PERIPH_GPIOP, \b SYSCTL_PERIPH_GPIOQ,
//! \b SYSCTL_PERIPH_GPIOR, or \b SYSCTL_PERIPH_GPIOS.
//!
//! \return None.
//*****************************************************************************
void
SysCtlPeripheralReset(unsigned long ulPeripheral)
{
    volatile unsigned long ulDelay;
    unsigned long ulMWRAllow;

    // Check the arguments.
    ASSERT(SysCtlPeripheralValid(ulPeripheral));

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    if(ulPeripheral == SYSCTL_PERIPH_CAN0)
    {
        /*because CAN0 and CAN1 bits are flipped in the design in SRCR3
           register*/
        // Put the peripheral into the reset state.
        HWREG(g_pulSRCRRegs[SYSCTL_PERIPH_INDEX(SYSCTL_PERIPH_CAN1)]) |=
            SYSCTL_PERIPH_MASK(SYSCTL_PERIPH_CAN1);
        // Delay for a little bit.
        for(ulDelay = 0; ulDelay < 16; ulDelay++)
        {
        }

        // Take the peripheral out of the reset state.
        HWREG(g_pulSRCRRegs[SYSCTL_PERIPH_INDEX(SYSCTL_PERIPH_CAN1)]) &=
            ~SYSCTL_PERIPH_MASK(SYSCTL_PERIPH_CAN1);

    }
    else if (ulPeripheral == SYSCTL_PERIPH_CAN1)
    {
        /*because CAN0 and CAN1 bits are flipped in the design in SRCR3
           register*/

        // Put the peripheral into the reset state.
        HWREG(g_pulSRCRRegs[SYSCTL_PERIPH_INDEX(SYSCTL_PERIPH_CAN0)]) |=
            SYSCTL_PERIPH_MASK(SYSCTL_PERIPH_CAN0);
        // Delay for a little bit.
        for(ulDelay = 0; ulDelay < 16; ulDelay++)
        {
        }

        // Take the peripheral out of the reset state.
        HWREG(g_pulSRCRRegs[SYSCTL_PERIPH_INDEX(SYSCTL_PERIPH_CAN0)]) &=
            ~SYSCTL_PERIPH_MASK(SYSCTL_PERIPH_CAN0);

    }
    else if (ulPeripheral == SYSCTL_PERIPH_USB0)
    {
        // Put the peripheral into the reset state.
        HWREG(g_pulSRCRRegs[SYSCTL_PERIPH_INDEX(SYSCTL_PERIPH_USB0)]) |=
            SYSCTL_PERIPH_MASK(SYSCTL_PERIPH_USB0);
        
        // Delay for a little bit.
        for(ulDelay = 0; ulDelay < 16; ulDelay++)
        {
        }
        
        // Take the peripheral out of the reset state.
        HWREG(g_pulSRCRRegs[SYSCTL_PERIPH_INDEX(SYSCTL_PERIPH_USB0)]) &=
            ~SYSCTL_PERIPH_MASK(SYSCTL_PERIPH_USB0);

    }
    else
    {
        // Put the peripheral into the reset state.
        HWREG(g_pulSRCRRegs[SYSCTL_PERIPH_INDEX(ulPeripheral)]) |=
            SYSCTL_PERIPH_MASK(ulPeripheral);

        // Delay for a little bit.
        for(ulDelay = 0; ulDelay < 16; ulDelay++)
        {
        }

        // Take the peripheral out of the reset state.
        HWREG(g_pulSRCRRegs[SYSCTL_PERIPH_INDEX(ulPeripheral)]) &=
            ~SYSCTL_PERIPH_MASK(ulPeripheral);

    }

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;
}

//*****************************************************************************
//! Enables a peripheral.
//!
//! \param ulPeripheral is the peripheral to enable.
//!
//! Peripherals are enabled with this function.  At power-up, all peripherals
//! are disabled; they must be enabled in order to operate or respond to
//! register reads/writes.
//!
//! The \e ulPeripheral parameter must be only one of the following values:
//! \b SYSCTL_PERIPH_CAN0,   \b SYSCTL_PERIPH_CAN1,   \b SYSCTL_PERIPH_EPI0,
//! \b SYSCTL_PERIPH_ETH,    \b SYSCTL_PERIPH_GPIOA,  \b SYSCTL_PERIPH_GPIOB,
//! \b SYSCTL_PERIPH_GPIOC,  \b SYSCTL_PERIPH_GPIOD,  \b SYSCTL_PERIPH_GPIOE,
//! \b SYSCTL_PERIPH_GPIOF,  \b SYSCTL_PERIPH_GPIOG,  \b SYSCTL_PERIPH_GPIOH,
//! \b SYSCTL_PERIPH_GPIOJ,  \b SYSCTL_PERIPH_I2C0,   \b SYSCTL_PERIPH_I2C1,
//! \b SYSCTL_PERIPH_IEEE1588, \b SYSCTL_PERIPH_SSI0, \b SYSCTL_PERIPH_SSI1,
//! \b SYSCTL_PERIPH_SSI2,   \b SYSCTL_PERIPH_SSI3    \b SYSCTL_PERIPH_TIMER0,
//! \b SYSCTL_PERIPH_TIMER1, \b SYSCTL_PERIPH_TIMER2, \b SYSCTL_PERIPH_TIMER3,
//! \b SYSCTL_PERIPH_UART0,  \b SYSCTL_PERIPH_UART1,  \b SYSCTL_PERIPH_UART2,
//! \b SYSCTL_PERIPH_UART3, \b SYSCTL_PERIPH_UART4,  \b SYSCTL_PERIPH_UDMA,
//! \b SYSCTL_PERIPH_USB0,  \b SYSCTL_PERIPH_WDOG0, \b SYSCTL_PERIPH_WDOG1,
//! \b SYSCTL_PERIPH_GPIOK, \b SYSCTL_PERIPH_GPIOL, \b SYSCTL_PERIPH_GPIOM,
//! \b SYSCTL_PERIPH_GPION, \b SYSCTL_PERIPH_GPIOP, \b SYSCTL_PERIPH_GPIOQ,
//! \b SYSCTL_PERIPH_GPIOR, or \b SYSCTL_PERIPH_GPIOS.
//!
//! \note It takes five clock cycles after the write to enable a peripheral
//! before the the peripheral is actually enabled.  During this time, attempts
//! to access the peripheral will result in a bus fault.  Care should be taken
//! to ensure that the peripheral is not accessed during this brief time
//! period.
//!
//! \return None.
//*****************************************************************************
void
SysCtlPeripheralEnable(unsigned long ulPeripheral)
{
    unsigned long ulMWRAllow;

    // Check the arguments.
    ASSERT(SysCtlPeripheralValid(ulPeripheral));

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    // Enable this peripheral.
    HWREG(g_pulRCGCRegs[SYSCTL_PERIPH_INDEX(ulPeripheral)]) |=
        SYSCTL_PERIPH_MASK(ulPeripheral);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;
}

//*****************************************************************************
//! Disables a peripheral.
//!
//! \param ulPeripheral is the peripheral to disable.
//!
//! Peripherals are disabled with this function.  Once disabled, they will not
//! operate or respond to register reads/writes.
//!
//! The \e ulPeripheral parameter must be only one of the following values:
//! \b SYSCTL_PERIPH_CAN0,   \b SYSCTL_PERIPH_CAN1,   \b SYSCTL_PERIPH_EPI0,
//! \b SYSCTL_PERIPH_ETH,    \b SYSCTL_PERIPH_GPIOA,  \b SYSCTL_PERIPH_GPIOB,
//! \b SYSCTL_PERIPH_GPIOC,  \b SYSCTL_PERIPH_GPIOD,  \b SYSCTL_PERIPH_GPIOE,
//! \b SYSCTL_PERIPH_GPIOF,  \b SYSCTL_PERIPH_GPIOG,  \b SYSCTL_PERIPH_GPIOH,
//! \b SYSCTL_PERIPH_GPIOJ,  \b SYSCTL_PERIPH_I2C0,   \b SYSCTL_PERIPH_I2C1,
//! \b SYSCTL_PERIPH_IEEE1588, \b SYSCTL_PERIPH_SSI0, \b SYSCTL_PERIPH_SSI1,
//! \b SYSCTL_PERIPH_SSI2,   \b SYSCTL_PERIPH_SSI3    \b SYSCTL_PERIPH_TIMER0,
//! \b SYSCTL_PERIPH_TIMER1, \b SYSCTL_PERIPH_TIMER2, \b SYSCTL_PERIPH_TIMER3,
//! \b SYSCTL_PERIPH_UART0,  \b SYSCTL_PERIPH_UART1,  \b SYSCTL_PERIPH_UART2,
//! \b SYSCTL_PERIPH_UART3, \b SYSCTL_PERIPH_UART4,  \b SYSCTL_PERIPH_UDMA,
//! \b SYSCTL_PERIPH_USB0,  \b SYSCTL_PERIPH_WDOG0, \b SYSCTL_PERIPH_WDOG1,
//! \b SYSCTL_PERIPH_GPIOK, \b SYSCTL_PERIPH_GPIOL, \b SYSCTL_PERIPH_GPIOM,
//! \b SYSCTL_PERIPH_GPION, \b SYSCTL_PERIPH_GPIOP, \b SYSCTL_PERIPH_GPIOQ,
//! \b SYSCTL_PERIPH_GPIOR, or \b SYSCTL_PERIPH_GPIOS.
//!
//! \return None.
//*****************************************************************************
void
SysCtlPeripheralDisable(unsigned long ulPeripheral)
{
    unsigned long ulMWRAllow;

    // Check the arguments.
    ASSERT(SysCtlPeripheralValid(ulPeripheral));

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    // Disable this peripheral.
    HWREG(g_pulRCGCRegs[SYSCTL_PERIPH_INDEX(ulPeripheral)]) &=
        ~SYSCTL_PERIPH_MASK(ulPeripheral);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;

}

//*****************************************************************************
//! Checks if a peripheral is enabled.
//!
//! \param ulPeripheral is the peripheral to check on.
//!
//! Peripheral clock gating can be checked during runtime with this function.
//!
//! The \e ulPeripheral parameter must be only one of the following values:
//! \b SYSCTL_PERIPH_CAN0,   \b SYSCTL_PERIPH_CAN1,   \b SYSCTL_PERIPH_EPI0,
//! \b SYSCTL_PERIPH_ETH,    \b SYSCTL_PERIPH_GPIOA,  \b SYSCTL_PERIPH_GPIOB,
//! \b SYSCTL_PERIPH_GPIOC,  \b SYSCTL_PERIPH_GPIOD,  \b SYSCTL_PERIPH_GPIOE,
//! \b SYSCTL_PERIPH_GPIOF,  \b SYSCTL_PERIPH_GPIOG,  \b SYSCTL_PERIPH_GPIOH,
//! \b SYSCTL_PERIPH_GPIOJ,  \b SYSCTL_PERIPH_I2C0,   \b SYSCTL_PERIPH_I2C1,
//! \b SYSCTL_PERIPH_IEEE1588, \b SYSCTL_PERIPH_SSI0, \b SYSCTL_PERIPH_SSI1,
//! \b SYSCTL_PERIPH_SSI2,   \b SYSCTL_PERIPH_SSI3    \b SYSCTL_PERIPH_TIMER0,
//! \b SYSCTL_PERIPH_TIMER1, \b SYSCTL_PERIPH_TIMER2, \b SYSCTL_PERIPH_TIMER3,
//! \b SYSCTL_PERIPH_UART0,  \b SYSCTL_PERIPH_UART1,  \b SYSCTL_PERIPH_UART2,
//! \b SYSCTL_PERIPH_UART3, \b SYSCTL_PERIPH_UART4,  \b SYSCTL_PERIPH_UDMA,
//! \b SYSCTL_PERIPH_USB0,  \b SYSCTL_PERIPH_WDOG0, \b SYSCTL_PERIPH_WDOG1,
//! \b SYSCTL_PERIPH_GPIOK, \b SYSCTL_PERIPH_GPIOL, \b SYSCTL_PERIPH_GPIOM,
//! \b SYSCTL_PERIPH_GPION, \b SYSCTL_PERIPH_GPIOP, \b SYSCTL_PERIPH_GPIOQ,
//! \b SYSCTL_PERIPH_GPIOR, or \b SYSCTL_PERIPH_GPIOS.
//!
//!
//! \return True if the peripheral is enabled, False if disabled.
//*****************************************************************************
tBoolean
SysCtlPeripheralIsEnabled(unsigned long ulPeripheral)
{
    // Check the arguments.
    ASSERT(SysCtlPeripheralValid(ulPeripheral));

    // Check if the peripheral is enabled.
    if(HWREG(g_pulRCGCRegs[SYSCTL_PERIPH_INDEX(ulPeripheral)]) &
       SYSCTL_PERIPH_MASK(ulPeripheral))
        return true;
    else
        return false;

}

//*****************************************************************************
//! Enables a peripheral in sleep mode. 
//!
//! \param ulPeripheral is the peripheral to enable in sleep mode.
//!
//! This function allows a peripheral to continue operating when the processor
//! goes into sleep mode.  Since the clocking configuration of the device does
//! not change, any peripheral can safely continue operating while the
//! processor is in sleep mode, and can therefore wake the processor from sleep
//! mode.
//!
//! Sleep mode clocking of peripherals must be enabled via
//! SysCtlPeripheralClockGating(); if disabled, the peripheral sleep mode
//! configuration is maintained but has no effect when sleep mode is entered.
//!
//! The \e ulPeripheral parameter must be only one of the following values:
//! \b SYSCTL_PERIPH_CAN0,   \b SYSCTL_PERIPH_CAN1,   \b SYSCTL_PERIPH_EPI0,
//! \b SYSCTL_PERIPH_ETH,    \b SYSCTL_PERIPH_GPIOA,  \b SYSCTL_PERIPH_GPIOB,
//! \b SYSCTL_PERIPH_GPIOC,  \b SYSCTL_PERIPH_GPIOD,  \b SYSCTL_PERIPH_GPIOE,
//! \b SYSCTL_PERIPH_GPIOF,  \b SYSCTL_PERIPH_GPIOG,  \b SYSCTL_PERIPH_GPIOH,
//! \b SYSCTL_PERIPH_GPIOJ,  \b SYSCTL_PERIPH_I2C0,   \b SYSCTL_PERIPH_I2C1,
//! \b SYSCTL_PERIPH_IEEE1588, \b SYSCTL_PERIPH_SSI0, \b SYSCTL_PERIPH_SSI1,
//! \b SYSCTL_PERIPH_SSI2,   \b SYSCTL_PERIPH_SSI3    \b SYSCTL_PERIPH_TIMER0,
//! \b SYSCTL_PERIPH_TIMER1, \b SYSCTL_PERIPH_TIMER2, \b SYSCTL_PERIPH_TIMER3,
//! \b SYSCTL_PERIPH_UART0,  \b SYSCTL_PERIPH_UART1,  \b SYSCTL_PERIPH_UART2,
//! \b SYSCTL_PERIPH_UART3, \b SYSCTL_PERIPH_UART4,  \b SYSCTL_PERIPH_UDMA,
//! \b SYSCTL_PERIPH_USB0,  \b SYSCTL_PERIPH_WDOG0, \b SYSCTL_PERIPH_WDOG1,
//! \b SYSCTL_PERIPH_GPIOK, \b SYSCTL_PERIPH_GPIOL, \b SYSCTL_PERIPH_GPIOM,
//! \b SYSCTL_PERIPH_GPION, \b SYSCTL_PERIPH_GPIOP, \b SYSCTL_PERIPH_GPIOQ,
//! \b SYSCTL_PERIPH_GPIOR, or \b SYSCTL_PERIPH_GPIOS.
//!
//! \return None.
//*****************************************************************************
void
SysCtlPeripheralSleepEnable(unsigned long ulPeripheral)
{
    unsigned long ulMWRAllow;

    // Check the arguments.
    ASSERT(SysCtlPeripheralValid(ulPeripheral));

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    // Enable this peripheral in sleep mode.
    HWREG(g_pulSCGCRegs[SYSCTL_PERIPH_INDEX(ulPeripheral)]) |=
        SYSCTL_PERIPH_MASK(ulPeripheral);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;
}

//*****************************************************************************
//! Disables a peripheral in sleep mode.  
//!
//! \param ulPeripheral is the peripheral to disable in sleep mode.
//!
//! This function causes a peripheral to stop operating when the processor goes
//! into sleep mode.  Disabling peripherals while in sleep mode helps to lower
//! the current draw of the device.  If enabled (via SysCtlPeripheralEnable()),
//! the peripheral will automatically resume operation when the processor
//! leaves sleep mode, maintaining its entire state from before sleep mode was
//! entered.
//!
//! Sleep mode clocking of peripherals must be enabled via
//! SysCtlPeripheralClockGating(); if disabled, the peripheral sleep mode
//! configuration is maintained but has no effect when sleep mode is entered.
//!
//! The \e ulPeripheral parameter must be only one of the following values:
//! \b SYSCTL_PERIPH_CAN0,   \b SYSCTL_PERIPH_CAN1,   \b SYSCTL_PERIPH_EPI0,
//! \b SYSCTL_PERIPH_ETH,    \b SYSCTL_PERIPH_GPIOA,  \b SYSCTL_PERIPH_GPIOB,
//! \b SYSCTL_PERIPH_GPIOC,  \b SYSCTL_PERIPH_GPIOD,  \b SYSCTL_PERIPH_GPIOE,
//! \b SYSCTL_PERIPH_GPIOF,  \b SYSCTL_PERIPH_GPIOG,  \b SYSCTL_PERIPH_GPIOH,
//! \b SYSCTL_PERIPH_GPIOJ,  \b SYSCTL_PERIPH_I2C0,   \b SYSCTL_PERIPH_I2C1,
//! \b SYSCTL_PERIPH_IEEE1588, \b SYSCTL_PERIPH_SSI0, \b SYSCTL_PERIPH_SSI1,
//! \b SYSCTL_PERIPH_SSI2,   \b SYSCTL_PERIPH_SSI3    \b SYSCTL_PERIPH_TIMER0,
//! \b SYSCTL_PERIPH_TIMER1, \b SYSCTL_PERIPH_TIMER2, \b SYSCTL_PERIPH_TIMER3,
//! \b SYSCTL_PERIPH_UART0,  \b SYSCTL_PERIPH_UART1,  \b SYSCTL_PERIPH_UART2,
//! \b SYSCTL_PERIPH_UART3, \b SYSCTL_PERIPH_UART4,  \b SYSCTL_PERIPH_UDMA,
//! \b SYSCTL_PERIPH_USB0,  \b SYSCTL_PERIPH_WDOG0, \b SYSCTL_PERIPH_WDOG1,
//! \b SYSCTL_PERIPH_GPIOK, \b SYSCTL_PERIPH_GPIOL, \b SYSCTL_PERIPH_GPIOM,
//! \b SYSCTL_PERIPH_GPION, \b SYSCTL_PERIPH_GPIOP, \b SYSCTL_PERIPH_GPIOQ,
//! \b SYSCTL_PERIPH_GPIOR, or \b SYSCTL_PERIPH_GPIOS.
//!
//! \return None.
//*****************************************************************************
void
SysCtlPeripheralSleepDisable(unsigned long ulPeripheral)
{
    unsigned long ulMWRAllow;

    // Check the arguments.
    ASSERT(SysCtlPeripheralValid(ulPeripheral));

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    // Disable this peripheral in sleep mode.
    HWREG(g_pulSCGCRegs[SYSCTL_PERIPH_INDEX(ulPeripheral)]) &=
        ~SYSCTL_PERIPH_MASK(ulPeripheral);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;
}

//*****************************************************************************
//! Enables a peripheral in deep-sleep mode.  
//!
//! \param ulPeripheral is the peripheral to enable in deep-sleep mode.
//!
//! This function allows a peripheral to continue operating when the processor
//! goes into deep-sleep mode.  Since the clocking configuration of the device
//! may change, not all peripherals can safely continue operating while the
//! processor is in sleep mode.  Those that must run at a particular frequency
//! (such as a UART) will not work as expected if the clock changes.  It is the
//! responsibility of the caller to make sensible choices.
//!
//! Deep-sleep mode clocking of peripherals must be enabled via
//! SysCtlPeripheralClockGating(); if disabled, the peripheral deep-sleep mode
//! configuration is maintained but has no effect when deep-sleep mode is
//! entered.
//!
//! The \e ulPeripheral parameter must be only one of the following values:
//! \b SYSCTL_PERIPH_CAN0,   \b SYSCTL_PERIPH_CAN1,   \b SYSCTL_PERIPH_EPI0,
//! \b SYSCTL_PERIPH_ETH,    \b SYSCTL_PERIPH_GPIOA,  \b SYSCTL_PERIPH_GPIOB,
//! \b SYSCTL_PERIPH_GPIOC,  \b SYSCTL_PERIPH_GPIOD,  \b SYSCTL_PERIPH_GPIOE,
//! \b SYSCTL_PERIPH_GPIOF,  \b SYSCTL_PERIPH_GPIOG,  \b SYSCTL_PERIPH_GPIOH,
//! \b SYSCTL_PERIPH_GPIOJ,  \b SYSCTL_PERIPH_I2C0,   \b SYSCTL_PERIPH_I2C1,
//! \b SYSCTL_PERIPH_IEEE1588, \b SYSCTL_PERIPH_SSI0, \b SYSCTL_PERIPH_SSI1,
//! \b SYSCTL_PERIPH_SSI2,   \b SYSCTL_PERIPH_SSI3    \b SYSCTL_PERIPH_TIMER0,
//! \b SYSCTL_PERIPH_TIMER1, \b SYSCTL_PERIPH_TIMER2, \b SYSCTL_PERIPH_TIMER3,
//! \b SYSCTL_PERIPH_UART0,  \b SYSCTL_PERIPH_UART1,  \b SYSCTL_PERIPH_UART2,
//! \b SYSCTL_PERIPH_UART3, \b SYSCTL_PERIPH_UART4,  \b SYSCTL_PERIPH_UDMA,
//! \b SYSCTL_PERIPH_USB0,  \b SYSCTL_PERIPH_WDOG0, \b SYSCTL_PERIPH_WDOG1,
//! \b SYSCTL_PERIPH_GPIOK, \b SYSCTL_PERIPH_GPIOL, \b SYSCTL_PERIPH_GPIOM,
//! \b SYSCTL_PERIPH_GPION, \b SYSCTL_PERIPH_GPIOP, \b SYSCTL_PERIPH_GPIOQ,
//! \b SYSCTL_PERIPH_GPIOR, or \b SYSCTL_PERIPH_GPIOS.
//!
//! \return None.
//*****************************************************************************
void
SysCtlPeripheralDeepSleepEnable(unsigned long ulPeripheral)
{
    unsigned long ulMWRAllow;

    // Check the arguments.
    ASSERT(SysCtlPeripheralValid(ulPeripheral));

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    // Enable this peripheral in deep-sleep mode.
    HWREG(g_pulDCGCRegs[SYSCTL_PERIPH_INDEX(ulPeripheral)]) |=
        SYSCTL_PERIPH_MASK(ulPeripheral);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;
}

//*****************************************************************************
//! Disables a peripheral in deep-sleep mode.  
//!
//! \param ulPeripheral is the peripheral to disable in deep-sleep mode.
//!
//! This function causes a peripheral to stop operating when the processor goes
//! into deep-sleep mode.  Disabling peripherals while in deep-sleep mode helps
//! to lower the current draw of the device, and can keep peripherals that
//! require a particular clock frequency from operating when the clock changes
//! as a result of entering deep-sleep mode.  If enabled (via
//! SysCtlPeripheralEnable()), the peripheral will automatically resume
//! operation when the processor leaves deep-sleep mode, maintaining its entire
//! state from before deep-sleep mode was entered.
//!
//! Deep-sleep mode clocking of peripherals must be enabled via
//! SysCtlPeripheralClockGating(); if disabled, the peripheral deep-sleep mode
//! configuration is maintained but has no effect when deep-sleep mode is
//! entered.
//!
//! The \e ulPeripheral parameter must be only one of the following values:
//! \b SYSCTL_PERIPH_CAN0,   \b SYSCTL_PERIPH_CAN1,   \b SYSCTL_PERIPH_EPI0,
//! \b SYSCTL_PERIPH_ETH,    \b SYSCTL_PERIPH_GPIOA,  \b SYSCTL_PERIPH_GPIOB,
//! \b SYSCTL_PERIPH_GPIOC,  \b SYSCTL_PERIPH_GPIOD,  \b SYSCTL_PERIPH_GPIOE,
//! \b SYSCTL_PERIPH_GPIOF,  \b SYSCTL_PERIPH_GPIOG,  \b SYSCTL_PERIPH_GPIOH,
//! \b SYSCTL_PERIPH_GPIOJ,  \b SYSCTL_PERIPH_I2C0,   \b SYSCTL_PERIPH_I2C1,
//! \b SYSCTL_PERIPH_IEEE1588, \b SYSCTL_PERIPH_SSI0, \b SYSCTL_PERIPH_SSI1,
//! \b SYSCTL_PERIPH_SSI2,   \b SYSCTL_PERIPH_SSI3    \b SYSCTL_PERIPH_TIMER0,
//! \b SYSCTL_PERIPH_TIMER1, \b SYSCTL_PERIPH_TIMER2, \b SYSCTL_PERIPH_TIMER3,
//! \b SYSCTL_PERIPH_UART0,  \b SYSCTL_PERIPH_UART1,  \b SYSCTL_PERIPH_UART2,
//! \b SYSCTL_PERIPH_UART3, \b SYSCTL_PERIPH_UART4,  \b SYSCTL_PERIPH_UDMA,
//! \b SYSCTL_PERIPH_USB0,  \b SYSCTL_PERIPH_WDOG0, \b SYSCTL_PERIPH_WDOG1,
//! \b SYSCTL_PERIPH_GPIOK, \b SYSCTL_PERIPH_GPIOL, \b SYSCTL_PERIPH_GPIOM,
//! \b SYSCTL_PERIPH_GPION, \b SYSCTL_PERIPH_GPIOP, \b SYSCTL_PERIPH_GPIOQ,
//! \b SYSCTL_PERIPH_GPIOR, or \b SYSCTL_PERIPH_GPIOS.
//!
//! \return None.
//*****************************************************************************
void
SysCtlPeripheralDeepSleepDisable(unsigned long ulPeripheral)
{
    unsigned long ulMWRAllow;

    // Check the arguments.
    ASSERT(SysCtlPeripheralValid(ulPeripheral));

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    // Disable this peripheral in deep-sleep mode.
    HWREG(g_pulDCGCRegs[SYSCTL_PERIPH_INDEX(ulPeripheral)]) &=
        ~SYSCTL_PERIPH_MASK(ulPeripheral);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;
}

//*****************************************************************************
//! Controls peripheral clock gating in sleep and deep-sleep mode.  
//!
//! \param bEnable is a boolean that is \b true if the sleep and deep-sleep
//! peripheral configuration should be used and \b false if not.
//!
//! This function controls how peripherals are clocked when the processor goes
//! into sleep or deep-sleep mode.  By default, the peripherals are clocked the
//! same as in run mode; if peripheral clock gating is enabled they are clocked
//! according to the configuration set by SysCtlPeripheralSleepEnable(),
//! SysCtlPeripheralSleepDisable(), SysCtlPeripheralDeepSleepEnable(), and
//! SysCtlPeripheralDeepSleepDisable().
//!
//! \return None.
//*****************************************************************************
void
SysCtlPeripheralClockGating(tBoolean bEnable)
{
    unsigned long ulMWRAllow;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;
    // Enable peripheral clock gating as requested.
    if(bEnable)
    {
        HWREG(SYSCTL_RCC) |= SYSCTL_RCC_ACG;
    }
    else
    {
        HWREG(SYSCTL_RCC) &= ~(SYSCTL_RCC_ACG);
    }

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;
}

//*****************************************************************************
//! Resets the device.
//!
//! This function will perform a software reset of the entire device.  The
//! processor and all peripherals will be reset and all device registers will
//! return to their default values (with the exception of the reset cause
//! register, which will maintain its current value but have the software reset
//! bit set as well).
//!
//! \return This function does not return.
//*****************************************************************************
void
SysCtlReset(void)
{
    // Perform a software reset request.  This will cause the device to reset,
    // no further code will be executed.
    HWREG(NVIC_APINT) = NVIC_APINT_VECTKEY | NVIC_APINT_SYSRESETREQ;

    // The device should have reset, so this should never be reached.  Just in
    // case, loop forever.
    while(1)
    {
    }
}

//*****************************************************************************
//! Puts the processor into sleep mode.  
//!
//! This function places the processor into sleep mode; it will not return
//! until the processor returns to run mode.  The peripherals that are enabled
//! via SysCtlPeripheralSleepEnable() continue to operate and can wake up the
//! processor (if automatic clock gating is enabled with
//! SysCtlPeripheralClockGating(), otherwise all peripherals continue to
//! operate).
//!
//! \return None.
//*****************************************************************************
void
SysCtlSleep(void)
{
    // Wait for an interrupt.
    CPUwfi();
}

//*****************************************************************************
//! Puts the processor into deep-sleep mode.  
//!
//! This function places the processor into deep-sleep mode; it will not return
//! until the processor returns to run mode.  The peripherals that are enabled
//! via SysCtlPeripheralDeepSleepEnable() continue to operate and can wake up
//! the processor (if automatic clock gating is enabled with
//! SysCtlPeripheralClockGating(), otherwise all peripherals continue to
//! operate).
//!
//! \return None.
//*****************************************************************************
void
SysCtlDeepSleep(void)
{
    // Enable deep-sleep.
    HWREG(NVIC_SYS_CTRL) |= NVIC_SYS_CTRL_SLEEPDEEP;

    // Wait for an interrupt.
    CPUwfi();

    // Disable deep-sleep so that a future sleep will work correctly.
    HWREG(NVIC_SYS_CTRL) &= ~(NVIC_SYS_CTRL_SLEEPDEEP);
}

//*****************************************************************************
//! Gets the reason for a reset.
//!
//! This function will return the reason(s) for a reset.  Since the reset
//! reasons are sticky until either cleared by software or an external reset,
//! multiple reset reasons may be returned if multiple resets have occurred.
//! The reset reason will be a logical OR of \b SYSCTL_CAUSE_LDO,
//! \b SYSCTL_CAUSE_SW, \b SYSCTL_CAUSE_WDOG, \b SYSCTL_CAUSE_POR,
//! \b and/or \b SYSCTL_CAUSE_EXT.
//!
//! \return Returns the reason(s) for a reset.
//*****************************************************************************
unsigned long
SysCtlResetCauseGet(void)
{
    // Return the reset reasons.
    return(HWREG(SYSCTL_RESC));
}

//*****************************************************************************
//! Clears reset reasons.
//!
//! \param ulCauses are the reset causes to be cleared; must be a logical OR of
//! \b SYSCTL_CAUSE_LDO, \b SYSCTL_CAUSE_SW, \b SYSCTL_CAUSE_WDOG,
//! \b SYSCTL_CAUSE_POR, and/or \b SYSCTL_CAUSE_EXT.
//!
//! This function clears the specified sticky reset reasons.  Once cleared,
//! another reset for the same reason can be detected, and a reset for a
//! different reason can be distinguished (instead of having two reset causes
//! set).  If the reset reason is used by an application, all reset causes
//! should be cleared after they are retrieved with SysCtlResetCauseGet().
//!
//! \return None.
//*****************************************************************************
void
SysCtlResetCauseClear(unsigned long ulCauses)
{
    // Clear the given reset reasons.
    HWREG(SYSCTL_RESC) &= ~(ulCauses);
}

//*****************************************************************************
//! Provides a small delay.
//!
//! \param ulCount is the number of delay loop iterations to perform.
//!
//! This function provides a means of generating a constant length delay.  It
//! is written in assembly to keep the delay consistent across tool chains,
//! avoiding the need to tune the delay based on the tool chain in use.
//!
//! The loop takes 3 cycles/loop when residing in flash and 4 cycles/loop 
//! when residing in RAM.
//!
//! \return None.
//*****************************************************************************
__asm("    .sect \".text:SysCtlDelay\"\n"
      "    .clink\n"
      "    .thumbfunc SysCtlDelay\n"
      "    .thumb\n"
      "    .align 4\n"
      "    .global SysCtlDelay\n"
      "SysCtlDelay:\n"
      "    subs r0, #1\n"
      "    bne.n SysCtlDelay\n"
      "    bx lr\n");

//*****************************************************************************
//! Configure XPLL CLK OUT divider.
//!
//! \param XPllClkDivReq XPLL CLK OUT divider value, must be one of:
//! - SYSCTL_XCLKDIV_1
//! - SYSCTL_XCLKDIV_2
//! - SYSCTL_XCLKDIV_4
//! - SYSCTL_XCLKDIV_OFF
//!
//! Sets up device clocking according to the parameter passed in.
//!
//! \return None.
//*****************************************************************************
void
SysCtlXPllClockDividerSet(unsigned long XPllClkDivReq)
{
    unsigned long ulMWRAllow;
    unsigned long cur_xpll_val = 0x0;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    //get the current value of M3SSDIV SEL in req. format for ease of
    // comaprision below
    cur_xpll_val =
        ((HWREG(SYSCTL_XPLLCLKCFG)&
          (SYSCTL_XCLKDIV_M >>
           SYSCTL_XCLKDIV_M_SHIFT)) << SYSCTL_XCLKDIV_M_SHIFT);

    //Set the xclock out divider
    while((XPllClkDivReq & SYSCTL_XCLKDIV_M) != cur_xpll_val)
    {
        if(cur_xpll_val == SYSCTL_XCLKDIV_4)
        {
            //increase the freq. of operation in steps to avoid any VDD
            // fluctuations
            HWREG(SYSCTL_XPLLCLKCFG) =
                ((SYSCTL_XCLKDIV_2) >> SYSCTL_XCLKDIV_M_SHIFT) |
                (HWREG(SYSCTL_XPLLCLKCFG) &
                 ~(SYSCTL_XCLKDIV_M >> SYSCTL_XCLKDIV_M_SHIFT));

            cur_xpll_val = SYSCTL_XCLKDIV_2;

            if((XPllClkDivReq & SYSCTL_XCLKDIV_M) != cur_xpll_val)
                SysCtlDelay(15);    /*delay some where around 50 clock cycles*/
        }
        else
        {
            //increase the freq. of operation in steps to avoid any VDD
            // fluctuations
            HWREG(SYSCTL_XPLLCLKCFG) =
                ((XPllClkDivReq &
                  SYSCTL_XCLKDIV_M) >> SYSCTL_XCLKDIV_M_SHIFT) |
                (HWREG(SYSCTL_XPLLCLKCFG) &
                 ~(SYSCTL_XCLKDIV_M >> SYSCTL_XCLKDIV_M_SHIFT));

            cur_xpll_val =
                ((HWREG(SYSCTL_XPLLCLKCFG)&
                  (SYSCTL_XCLKDIV_M >>
                   SYSCTL_XCLKDIV_M_SHIFT)) << SYSCTL_XCLKDIV_M_SHIFT);
        }
    }

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;
}

//*****************************************************************************
//! Configure SYS CLK and M3SS Clock dividers.
//!
//! \param SysDivSelReq SYSCLK divider value, must be one of:
//! - SYSCTL_SYSDIV_1
//! - SYSCTL_SYSDIV_2
//! - SYSCTL_SYSDIV_4
//! - SYSCTL_SYSDIV_8
//! - SYSCTL_SYSDIV_NO_CHANGE
//! \param M3ClkDivReq M3SSCLK divider value, must be one of:
//! - SYSCTL_M3SSDIV_1
//! - SYSCTL_M3SSDIV_2
//! - SYSCTL_M3SSDIV_4
//! - SYSCTL_M3SSDIV_NO_CHANGE
//!
//! Sets up device clocking according to the parameter passed in.
//!
//! \return None.
//*****************************************************************************
void
SysCtlClockDividersSet(unsigned long SysDivSelReq, unsigned long M3ClkDivReq)
{
    unsigned long ulMWRAllow;
    unsigned long cur_m3ss_val = 0x0;
    unsigned long cur_sys_div_val = 0x0;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    //get the current value of M3SSDIV SEL in req. format for ease of
    // comparison below
    cur_m3ss_val =
        ((HWREG(SYSCTL_M3SSDIVSEL)&
          (SYSCTL_M3SSDIV_M >>
           SYSCTL_M3SSDIV_M_SHIFT)) << SYSCTL_M3SSDIV_M_SHIFT);

    //check if M3 SS CLK DIV should not be changed
    if(M3ClkDivReq != SYSCTL_M3SSDIV_NO_CHANGE)
    {
        while((M3ClkDivReq & SYSCTL_M3SSDIV_M) != cur_m3ss_val)
        {
            if(cur_m3ss_val == SYSCTL_M3SSDIV_4)
            {
                //increase the freq. of operation in steps to avoid any VDD
                // fluctuations
                HWREG(SYSCTL_M3SSDIVSEL) =
                    ((SYSCTL_M3SSDIV_2) >> SYSCTL_M3SSDIV_M_SHIFT) |
                    (HWREG(SYSCTL_M3SSDIVSEL) &
                     ~(SYSCTL_M3SSDIV_M >> SYSCTL_M3SSDIV_M_SHIFT));

                cur_m3ss_val = SYSCTL_M3SSDIV_2;

                if((M3ClkDivReq & SYSCTL_M3SSDIV_M) != cur_m3ss_val)
                    SysCtlDelay(15);    /*delay some where around 50 clock
                                          cycles*/
            }
            else
            {
                HWREG(SYSCTL_M3SSDIVSEL) =
                    (((M3ClkDivReq &
                       SYSCTL_M3SSDIV_M) >> SYSCTL_M3SSDIV_M_SHIFT) |
                     (HWREG(SYSCTL_M3SSDIVSEL) &
                      ~(SYSCTL_M3SSDIV_M >> SYSCTL_M3SSDIV_M_SHIFT)));

                cur_m3ss_val =
                    ((HWREG(SYSCTL_M3SSDIVSEL)&
                      (SYSCTL_M3SSDIV_M >>
                       SYSCTL_M3SSDIV_M_SHIFT)) << SYSCTL_M3SSDIV_M_SHIFT);
            }
        }

    }

    //get the current value of M3SSDIV SEL in req. format for ease of
    // comparison below
    cur_sys_div_val =
        (HWREG(SYSCTL_SYSDIVSEL)&
         (SYSCTL_SYSDIV_M >> SYSCTL_SYSDIV_M_SHIFT)) << SYSCTL_SYSDIV_M_SHIFT;

    //check if SYS CLK DIV should not be changed
    if(SysDivSelReq != SYSCTL_SYSDIV_NO_CHANGE)
    {
        while((SysDivSelReq & SYSCTL_SYSDIV_M) != cur_sys_div_val)
        {
            if(cur_sys_div_val == SYSCTL_SYSDIV_8)
            {
                HWREG(SYSCTL_SYSDIVSEL) =
                    ((SYSCTL_SYSDIV_4) >> SYSCTL_SYSDIV_M_SHIFT) |
                    (HWREG(SYSCTL_SYSDIVSEL) &
                     ~(SYSCTL_SYSDIV_M >> SYSCTL_SYSDIV_M_SHIFT));

                cur_sys_div_val = SYSCTL_SYSDIV_4;

                if((SysDivSelReq & SYSCTL_SYSDIV_M) != cur_sys_div_val)
                    SysCtlDelay(15);    /*delay some where around 50 clock
                                          cycles*/
            }
            else if(cur_sys_div_val == SYSCTL_SYSDIV_4)
            {
                HWREG(SYSCTL_SYSDIVSEL) =
                    ((SYSCTL_SYSDIV_2) >> SYSCTL_SYSDIV_M_SHIFT) |
                    (HWREG(SYSCTL_SYSDIVSEL) &
                     ~(SYSCTL_SYSDIV_M >> SYSCTL_SYSDIV_M_SHIFT));

                cur_sys_div_val = SYSCTL_SYSDIV_2;

                if((SysDivSelReq & SYSCTL_SYSDIV_M) != cur_sys_div_val)
                    SysCtlDelay(15);    /*delay some where around 50 clock
                                          cycles*/
            }
            else
            {
                HWREG(SYSCTL_SYSDIVSEL) =
                    ((SysDivSelReq &
                      SYSCTL_SYSDIV_M) >> SYSCTL_SYSDIV_M_SHIFT) |
                    (HWREG(SYSCTL_SYSDIVSEL) &
                     ~(SYSCTL_SYSDIV_M >> SYSCTL_SYSDIV_M_SHIFT));

                cur_sys_div_val =
                    (HWREG(SYSCTL_SYSDIVSEL)&
                     (SYSCTL_SYSDIV_M >>
                 SYSCTL_SYSDIV_M_SHIFT)) << SYSCTL_SYSDIV_M_SHIFT;
            }
        }
    }

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;
}

//*****************************************************************************
//! Power OFF PLL
//!
//! This function turns OFF PLL completely
//!
//! \return None.
//*****************************************************************************
void
SysCtlPowerOffPLL(void)
{
    unsigned long ulMWRAllow;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    HWREG(SYSCTL_SYSPLLCTL) &=
        ~(SYSCTL_SYSPLLCTL_SPLLEN | SYSCTL_SYSPLLCTL_SPLLCLKEN);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;
}

//*****************************************************************************
//! Configure PLL
//!
//! \param ClkSrcReq Must be one of:
//! - SYSCTL_USE_PLL
//! - SYSCTL_USE_OSC
//! \param PllMult bits 0-6 must be PLLIMULT and bit 8:9 must be PLLFMULT;
//!  bits 7, 31:10 must be 0 (reserved)
//!
//! Sets up PLL and associated multipliers according to the parameter passed in.
//!
//! \return \b true if the PLL is configured as requested and \b false
//! if it is not.
//*****************************************************************************
tBoolean
SysCtlClockPllConfig(unsigned long ClkSrcReq, unsigned long PllMult)
{
	int i;
    unsigned long ulMWRAllow;
    unsigned long CurPllSrc = 0, CurPllIMult = 0, CurPllFMult = 0;
    unsigned long CurSysDiv = 0, CurM3SSDiv = 0, CurClockConfig = 0;

    CurClockConfig = SysCtlClockConfigGet();

    CurPllSrc   = CurClockConfig & SYSCTL_USE_PLL;
    CurPllIMult = CurClockConfig & SYSCTL_SPLLIMULT_M;
    CurPllFMult = CurClockConfig & SYSCTL_SPLLFMULT_M;
    CurSysDiv   = CurClockConfig & SYSCTL_SYSDIV_M;
    CurM3SSDiv  = CurClockConfig & SYSCTL_M3SSDIV_M;

    if((CurPllSrc == (ClkSrcReq >> SYSCTL_USE_PLL_SHIFT)) &&
       (CurPllIMult == (PllMult & SYSCTL_SPLLIMULT_M)) &&
       (CurPllFMult == (PllMult & SYSCTL_SPLLFMULT_M)))
    {
        //everything is set as required, so just return true;
        return true;
    }

    //if request is to bypass PLL then just do that and return;
    if((ClkSrcReq & SYSCTL_USE_PLL) == SYSCTL_USE_OSC)
    {
        //ensure the PLL is bypassed
        HWREG(SYSCTL_SYSPLLCTL) &= ~SYSCTL_SYSPLLCTL_SPLLCLKEN;
        return true;
    }

    //Check if missing clock logic is set, because PLL can be in LIMP mode
    if(HWREG(SYSCTL_MCLKSTS) & SYSCTL_MCLKSTS_MCLKFLG)
    {
        //missing clock status is set, so no good to change PLL
        //clear missing clock status and call back the function
        return false;
    }

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    // first modify the PLL multipliers
    if(CurPllIMult != (PllMult & SYSCTL_SPLLIMULT_M) || CurPllFMult != (PllMult & SYSCTL_SPLLFMULT_M))
    {
    	//ensure the PLL is bypassed
    	HWREG(SYSCTL_SYSPLLCTL) &= ~SYSCTL_SYSPLLCTL_SPLLCLKEN;

    	// Set dividers to maximum speed
    	SysCtlClockDividersSet((unsigned long)SYSCTL_SYSDIV_1,
    	                           (unsigned long)SYSCTL_M3SSDIV_1);

    	if(CurPllIMult != (PllMult & SYSCTL_SPLLIMULT_M))
    	{
            HWREG(SYSCTL_SYSPLLMULT) = (PllMult & SYSCTL_SPLLIMULT_M) |
                                       (HWREG(SYSCTL_SYSPLLMULT) &
                                        ~SYSCTL_SPLLIMULT_M);
    	}
    	if(CurPllFMult != (PllMult & SYSCTL_SPLLFMULT_M))
    	{
    		HWREG(SYSCTL_SYSPLLMULT) = (PllMult & SYSCTL_SPLLFMULT_M) |
    		                           (HWREG(SYSCTL_SYSPLLMULT) &
    		                            ~SYSCTL_SPLLFMULT_M);
    	}

        //Turn on the PLL, but don't clock the processor off of it
        HWREG(SYSCTL_SYSPLLCTL) = SYSCTL_SYSPLLCTL_SPLLEN |
                                  (HWREG(SYSCTL_SYSPLLCTL) &
                                   ~(SYSCTL_SYSPLLCTL_SPLLEN |
                                     SYSCTL_SYSPLLCTL_SPLLCLKEN));

        // Delay loop for PLL initialization timing
        SysCtlDelay(0x76);

        // Rewrite one of the multipliers to reinit PLL sequence
        HWREG(SYSCTL_SYSPLLMULT) = (PllMult & SYSCTL_SPLLIMULT_M) |
                                   (HWREG(SYSCTL_SYSPLLMULT) &
                                    ~SYSCTL_SPLLIMULT_M);
    }

    if(ClkSrcReq & SYSCTL_USE_PLL)
    {
        // Wait for PLL to lock
        while(!(HWREG(SYSCTL_SYSPLLLOCKS) & SYSCTL_SYSPLLLOCKS_SYSPLLLOCKS))
        {
            //if WDT is running then keep reloading the max counter value
            if(SysCtlPeripheralIsEnabled(SYSCTL_PERIPH_WDOG0))
            {
                if(WatchdogRunning(WATCHDOG0_BASE))
                {
                    WatchdogReloadSet(WATCHDOG0_BASE, 0xFFFFFFFF);
                }
            }
            if(SysCtlPeripheralIsEnabled(SYSCTL_PERIPH_WDOG1))
            {
                if(WatchdogRunning(WATCHDOG1_BASE))
                {
                    WatchdogReloadSet(WATCHDOG1_BASE, 0xFFFFFFFF);
                }
            }
        };

        // Set PLL output frequency to half of what they will be to limit in-rush current
        if(CurSysDiv == SYSCTL_SYSDIV_8 && CurM3SSDiv == SYSCTL_M3SSDIV_4)
        {
            SysCtlClockDividersSet((unsigned long)SYSCTL_SYSDIV_8,
                                   (unsigned long)SYSCTL_M3SSDIV_4);
        }
        else
        {
        	if(CurSysDiv == SYSCTL_SYSDIV_8)
        	{
        		SysCtlClockDividersSet((unsigned long)SYSCTL_SYSDIV_8,
        		                       (unsigned long)(CurM3SSDiv + SYSCTL_M3SSDIV_STEP));
        	}
            else if(CurM3SSDiv == SYSCTL_M3SSDIV_4)
        	{
        		SysCtlClockDividersSet((unsigned long)(CurSysDiv + SYSCTL_SYSDIV_STEP),
        		        		       (unsigned long)CurM3SSDiv);
        	}
            else
        	{
        		SysCtlClockDividersSet((unsigned long)(CurSysDiv + SYSCTL_SYSDIV_STEP),
                                       (unsigned long)(CurM3SSDiv  + SYSCTL_M3SSDIV_STEP));
        	}
        }

        //enable PLL clock to CPU
        HWREG(SYSCTL_SYSPLLCTL) |= SYSCTL_SYSPLLCTL_SPLLCLKEN;
    }

    // Small delay after switching to PLL to allow device to stabilize after current increase
    SysCtlDelay(34);	// Delay is approximately 34*3 = 102 cycles

    //put sysclock and m3ssclock dividers back to what they were
    SysCtlClockDividersSet(CurSysDiv, CurM3SSDiv);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;
    return true;
}

//*****************************************************************************
//! Configure device clocking
//!
//! \param ulConfig Clock configuration as defined below: \n
//! bits 21,20 of ulConfig param represent bit 1,0 of SYSDIVSEL register \n
//! bits 19,18 of ulConfig param represent bit 1,0 of M3SSDIVSEL register \n
//! bits 17,16 of ulConfig param represent bit 1,0 of XPLLCFG register \n
//! bits 9-0 of ulConfig param represent bit 9-0 of SYSPLLMULT register \n
//! bit 31 of ulConfig param represent bit 0 of SYSPLLCTL register
//!
//! Sets up device clocking according to the parameter passed in.
//!
//! \return None.
//*****************************************************************************
void
SysCtlClockConfigSet(unsigned long ulConfig)
{
    unsigned long ulMWRAllow;
    unsigned long CurPllSrc = 0, CurPllIMult = 0, CurPllFMult = 0;
    unsigned long CurClockConfig = 0;
    unsigned long ClkSrcReq= 0, PllMult = 0;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    CurClockConfig = SysCtlClockConfigGet();

    CurPllSrc   = CurClockConfig & SYSCTL_USE_PLL;
    CurPllIMult = CurClockConfig & SYSCTL_SPLLIMULT_M;
    CurPllFMult = CurClockConfig & SYSCTL_SPLLFMULT_M;
    ClkSrcReq = ulConfig & SYSCTL_USE_PLL;
    PllMult = ulConfig & (SYSCTL_SPLLIMULT_M | SYSCTL_SPLLFMULT_M);

    // If any PLL Settings changed, reconfigure PLL
    if((CurPllSrc != (ClkSrcReq >> SYSCTL_USE_PLL_SHIFT)) ||
          (CurPllIMult != (PllMult & SYSCTL_SPLLIMULT_M)) ||
          (CurPllFMult != (PllMult & SYSCTL_SPLLFMULT_M)))
    {
    	// Bypass PLL
    	HWREG(SYSCTL_SYSPLLCTL) &= ~SYSCTL_SYSPLLCTL_SPLLCLKEN;

    	//Set the main system divider
		//Set the M3 system divider
		SysCtlClockDividersSet(ulConfig & SYSCTL_SYSDIV_M,
							   (ulConfig & SYSCTL_M3SSDIV_M));

        //Set the PLL integer multiplier
        //Set the PLL fractional multiplier
        //Enable/disable the PLL
        SysCtlClockPllConfig(ClkSrcReq, (PllMult));
    }
    else
    {
        //Set the main system divider
        //Set the M3 system divider
        SysCtlClockDividersSet(ulConfig & SYSCTL_SYSDIV_M,
                               (ulConfig & SYSCTL_M3SSDIV_M));
    }

    //Set the xclock out divider
    SysCtlXPllClockDividerSet(ulConfig & SYSCTL_XCLKDIV_M);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;

}

//*****************************************************************************
//! Gets the clocking value of device.
//!
//! Read the current clock configuration and pass it back in the same format
//! as SysCtlClockConfigSet().
//!
//! \return Clock configuration in the following format: <br>
//! bits 21,20 of return value represent bit 1,0 of SYSDIVSEL register <br>
//! bits 19,18 of return value represent bit 1,0 of M3SSDIVSEL register <br>
//! bits 17,16 of return value represent bit 1,0 of XPLLCFG register <br>
//! bits 9-0 of return value represent bit 9-0 of SYSPLLMULT register <br>
//! bit 31 of return value represent bit 0 of SYSPLLCTL register
//*****************************************************************************
unsigned long
SysCtlClockConfigGet(void)
{
    unsigned long ulMWRAllow;
    unsigned long ulSysClk = 0;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    //read the main system divider
    ulSysClk |=
        ((HWREG(SYSCTL_SYSDIVSEL)&
          (SYSCTL_SYSDIV_M >> SYSCTL_SYSDIV_M_SHIFT)) << (SYSCTL_SYSDIV_M_SHIFT));

    //Set the M3 system divider
    ulSysClk |=
        ((HWREG(SYSCTL_M3SSDIVSEL)&
          (SYSCTL_M3SSDIV_M >>
           SYSCTL_M3SSDIV_M_SHIFT)) << (SYSCTL_M3SSDIV_M_SHIFT));

    //Set the xclock out divider
    ulSysClk |=
        ((HWREG(SYSCTL_XPLLCLKCFG)&
          (SYSCTL_XCLKDIV_M >>
           SYSCTL_XCLKDIV_M_SHIFT)) << (SYSCTL_XCLKDIV_M_SHIFT));

    //Set the PLL integer multiplier
    ulSysClk |= (HWREG(SYSCTL_SYSPLLMULT)& SYSCTL_SPLLIMULT_M);

    //Set the PLL fractional multiplier
    ulSysClk |= (HWREG(SYSCTL_SYSPLLMULT)& SYSCTL_SPLLFMULT_M);

    //Enable/disable value the PLL
    ulSysClk |= ((HWREG(SYSCTL_SYSPLLCTL)&
                  (SYSCTL_USE_PLL >>
                   SYSCTL_USE_PLL_SHIFT)) << SYSCTL_USE_PLL_SHIFT);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;

    return ulSysClk;
}

//*****************************************************************************
//! Returns the current M3 Clock frequency (in Hertz)
//!
//! \param ulClockIn Frequency of the external clock
//!
//! \return M3 System clock frequency (in Hertz)
//*****************************************************************************
unsigned long
SysCtlClockGet(unsigned long ulClockIn)
{
    //If the pll is enabled calculate its effect on the clock
    if((HWREG(SYSCTL_SYSPLLCTL) &
        (SYSCTL_SYSPLLCTL_SPLLEN | SYSCTL_SYSPLLCTL_SPLLCLKEN)) == 3)
    {
        //Calculate integer multiplier and fixed divide by 2
        ulClockIn = ulClockIn *
                    (HWREG(SYSCTL_SYSPLLMULT) & SYSCTL_SYSPLLMULT_SPLLIMULT_M);
        ulClockIn = ulClockIn / 2;

        //Calculate fractional multiplier
        switch((HWREG(SYSCTL_SYSPLLMULT) & SYSCTL_SYSPLLMULT_SPLLFMULT_M) >>
                SYSCTL_SYSPLLMULT_SPLLFMULT_S)
        {
        default:
        case 0:
            break;

        case 1:
            ulClockIn += ulClockIn / 4;
            break;

        case 2:
            ulClockIn += ulClockIn / 2;
            break;

        case 3:
            ulClockIn += (ulClockIn * 3) / 4;
            break;
        }
    }

    //Divide clock by the system clock divider
    switch(HWREG(SYSCTL_SYSDIVSEL) & SYSCTL_SYSDIVSEL_SYSDIVSEL_M)
    {
    default:
    case 0:
        break;

    case 1:
        ulClockIn /= 2;
        break;

    case 2:
        ulClockIn /= 4;
        break;

    case 3:
        ulClockIn /= 8;
        break;
    }

    //Divide clock by the M3 clock divider
    switch(HWREG(SYSCTL_M3SSDIVSEL) & SYSCTL_M3SSDIVSEL_M)
    {
    default:
    case 0:
        break;

    case 1:
        ulClockIn /= 2;
        break;

    case 2:
        ulClockIn /= 4;
        break;
    }

    return ulClockIn;
}

//*****************************************************************************
//! Enable the Control Subsystem Clock
//!
//! Enable the clock to the control subsystem.
//!
//! \return None.
//*****************************************************************************
void
SysCtlC28Enable(void)
{
    unsigned long ulMWRAllow;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    //Enable 28x clock
    HWREG(SYSCTL_CCLKOFF) &= ~SYSCTL_CCLKOFF_C28CLKINDIS;

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;

}

//*****************************************************************************
//! Disable the Control Subsystem clock.
//!
//! Disable the clock to the control subsystem.
//!
//! \return None.
//*****************************************************************************
void
SysCtlC28Disable(void)
{
    unsigned long ulMWRAllow;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    //Disable 28x clock
    HWREG(SYSCTL_CCLKOFF) |= SYSCTL_CCLKOFF_C28CLKINDIS;

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;

}

//*****************************************************************************
//! Enable GPIO port to be addresses from AHB memory space
//!
//! \param ulGPIOPeripheral GPIO port to put on the AHB bus, can be
//! SYSCTL_PERIPH_GPIOA to SYSCTL_PERIPH_GPIOJ.
//!
//! Switch accesses to a GPIO address space from the APB bus to the AHB bus.
//!
//! \return None.
//*****************************************************************************
void
SysCtlGPIOAHBEnable(unsigned long ulGPIOPeripheral)
{
    unsigned long ulMWRAllow;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    //Disable 28x clock
    HWREG(SYSCTL_GPIOHBCTL) |= (ulGPIOPeripheral & 0x1FF);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;
}

//*****************************************************************************
//! Disable GPIO port access from AHB memory space
//!
//! \param ulGPIOPeripheral GPIO port to remove from the AHB bus, can be
//! SYSCTL_PERIPH_GPIOA to SYSCTL_PERIPH_GPIOJ
//!
//! Switch accesses to a GPIO address space from the AHB bus to the APB bus.
//!
//! \return None.
//*****************************************************************************
void
SysCtlGPIOAHBDisable(unsigned long ulGPIOPeripheral)
{
    unsigned long ulMWRAllow;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    //Disable 28x clock
    HWREG(SYSCTL_GPIOHBCTL) &= ~(ulGPIOPeripheral & 0x1FF);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;
}

//*****************************************************************************
//! Power up the USB PLL.
//!
//! Enable the USB controller's PLL. This call is necessary before connecting
//! to any external devices.
//!
//! \return None.
//*****************************************************************************
void
SysCtlUSBPLLEnable(void)
{
    unsigned long ulMWRAllow;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    //Turn on the PLL, but don't enable the clock yet
    HWREG(SYSCTL_UPLLCTL) = SYSCTL_UPLLCTL_EN |
                            (HWREG(SYSCTL_UPLLCTL) & ~SYSCTL_UPLLCTL_CLKEN);
    //wait for PLL to lock
    while(!(HWREG(SYSCTL_UPLLSTS) & SYSCTL_UPLLSTS_UPLLLOCKS)) ;
    //enable PLL clock to USB
    HWREG(SYSCTL_UPLLCTL) |= SYSCTL_UPLLCTL_CLKEN;

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;

}

//*****************************************************************************
//! Power down the USB PLL.
//!
//! Disable the USB controller's PLL. The USB registers are still accessible,
//! but the physical layer will no longer function.
//!
//! \return None.
//*****************************************************************************
void
SysCtlUSBPLLDisable(void)
{
    unsigned long ulMWRAllow;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    //ensure the PLL is bypassed
    HWREG(SYSCTL_UPLLCTL) &= ~(SYSCTL_UPLLCTL_CLKEN | SYSCTL_UPLLCTL_EN);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;
}

//*****************************************************************************
//! Set the configuration of the USBPLL.
//!
//! Configure the USBPLL for operation with passed in configuration.
//!
//! \param ulConfig unsigned long integer value formatted as below: \n
//!  bits  7-0 of ulConfig represent bits 7-0 of UPLLMULT register \n
//!  bits  9-8 of ulConfig represent bits 9-8 of UPLLMULT register \n
//!  bit 31 of ulConfig represents bit 0 of UPLLCTL register \n
//!  bit 30 of ulConfig represents bit 1 of UPLLCTL register
//!
//! \return None.
//*****************************************************************************
void
SysCtlUSBPLLConfigSet(unsigned long ulConfig)
{
    unsigned long ulMWRAllow;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    //Set the USB PLL integer multiplier
    if((ulConfig & SYSCTL_UPLLIMULT_M) !=
       (HWREG(SYSCTL_UPLLMULT)& SYSCTL_UPLLMULT_PLLIMULT_M))
    {
        //Write requested value
        HWREG(SYSCTL_UPLLMULT) =
            (ulConfig &
             SYSCTL_UPLLIMULT_M) |
            (HWREG(SYSCTL_UPLLMULT) & ~SYSCTL_UPLLMULT_PLLIMULT_M);
    }
    //Set the USB PLL fractional multiplier
    if((ulConfig & SYSCTL_UPLLFMULT_M) !=
       (HWREG(SYSCTL_UPLLMULT)& SYSCTL_UPLLMULT_PLLFMULT_M))
    {
        //Write requested value
        HWREG(SYSCTL_UPLLMULT) =
            (ulConfig &
             SYSCTL_UPLLFMULT_M) |
            (HWREG(SYSCTL_UPLLMULT) & ~SYSCTL_UPLLMULT_PLLFMULT_M);
    }

    //Set USB PLL clock source
    if(((ulConfig &
         SYSCTL_UPLLCLKSRC_XCLKIN) >> SYSCTL_UPLLCLKSRC_XCLKIN_SHIFT) !=
       (HWREG(SYSCTL_UPLLCTL)& SYSCTL_UPLLCTL_CLKSRSCEL))
    {
        //Write requested value
        HWREG(SYSCTL_UPLLCTL) =
            ((ulConfig &
              SYSCTL_UPLLCLKSRC_XCLKIN) >>
             SYSCTL_UPLLCLKSRC_XCLKIN_SHIFT) |
            (HWREG(SYSCTL_UPLLCTL) & ~SYSCTL_UPLLCTL_CLKSRSCEL);
    }

    //Enable/disable the PLL
    if(ulConfig & SYSCTL_UPLLEN)
    {
        //Turn on the PLL, but don't enable the clock yet
        HWREG(SYSCTL_UPLLCTL) = SYSCTL_UPLLCTL_EN |
                                (HWREG(SYSCTL_UPLLCTL) & ~SYSCTL_UPLLCTL_CLKEN);
        //wait for PLL to lock
        while(!(HWREG(SYSCTL_UPLLSTS) & SYSCTL_UPLLSTS_UPLLLOCKS)) ;
        //enable PLL clock to CPU
        HWREG(SYSCTL_UPLLCTL) |= SYSCTL_UPLLCTL_CLKEN;
    }
    else
    {
        //ensure the PLL is bypassed
        HWREG(SYSCTL_UPLLCTL) &= ~(SYSCTL_UPLLCTL_CLKEN | SYSCTL_UPLLCTL_EN);
    }

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;

}

//*****************************************************************************
//! Get the configuration of the USBPLL.
//!
//! Read the configuration of the USBPLL.
//!
//! \return Configuration of the USBPLL in the following format: \n
//!  bits  7-0 of return value represent bits 7-0 of UPLLMULT register \n
//!  bits  9-8 of return value represent bits 9-8 of UPLLMULT register \n
//!  bit 31 of return value represents bit 0 of UPLLCTL register \n
//!  bit 30 of return value represents bit 1 of UPLLCTL register
//*****************************************************************************
unsigned long
SysCtlUSBPLLConfigGet(void)
{
    unsigned long ulReturn;

    //Get the Integer and Fractional USB PLL multipliers
    ulReturn = HWREG(SYSCTL_UPLLMULT) &
               (SYSCTL_UPLLMULT_PLLIMULT_M | SYSCTL_UPLLMULT_PLLFMULT_M);
    //Get the USB PLL clock source
    ulReturn |= (HWREG(SYSCTL_UPLLCTL) & SYSCTL_UPLLCTL_CLKSRSCEL) << 31;
    //Get the USB PLL enable status
    ulReturn |= (HWREG(SYSCTL_UPLLCTL) & SYSCTL_UPLLCTL_EN) << 29;
    return ulReturn;
}

//*****************************************************************************
//! Enable the AERR NMI.
//!
//! Enable the AERR NMI in MNMICFG register.
//!
//! \return None.
//*****************************************************************************
void SysCtlEnableAERRNMI(void)
{
    unsigned long ulMWRAllow;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    HWREG(NMI_BASE + NMI_O_CFG) = (NMI_CFG_AERRE);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;

}

//*****************************************************************************
//! Read the current value of NMI Config register.
//!
//! Read the current value of NMI Config register.
//!
//! \return - Value of MNMICFG register as defined in the user guide
//*****************************************************************************
unsigned long SysCtlGetNmiConfig()
{
    return HWREG(NMI_BASE + NMI_O_CFG);
}

//*****************************************************************************
//! Read NMI interrups.
//!
//! Read the current state of NMI interrupt.
//!
//! \return \b TRUE if NMI interrupt is triggered, \b FALSE if not.
//*****************************************************************************
tBoolean SysCtlGetNmiInterruptStatus()
{
    return (HWREG(NMI_BASE + NMI_O_FLG) && NMI_FLG_NMIINT);
}

//*****************************************************************************
//! Read NMI Flags.
//!
//! Read the current state of individual NMI interrupts
//!
//! \return Value of MNMIFLG register.
//*****************************************************************************
unsigned long SysCtlGetNmiFlagStatus()
{
    return HWREG(NMI_BASE + NMI_O_FLG);
}

//*****************************************************************************
//! Check if the individual NMI interrupts are set
//!
//! \param ulNmis Bit mask of the NMI interrupts that user wants  to check if
//! set.  The bit format of this parameter is same as of the MNMIFLG register.
//!
//! Check if interrupt flags corresponding to the passed in bit mask are
//! asserted.
//!
//! \return \b TRUE if any of the NMI asked for in the parameter bit mask
//! is set. \b FALSE if none of the NMI requested in the parameter bit mask are
//! set.
//*****************************************************************************
tBoolean SysCtlNmiIsSet(unsigned long ulNmis)
{
    if(HWREG(NMI_BASE + NMI_O_FLG) & ulNmis)
        return TRUE;
    else
        return FALSE;
}

//*****************************************************************************
//! Function to clear individual NMI interrupts
//!
//! \param ulNmis Bit mask of the NMI interrupts that user wants  to clear.
//! The bit format of this parameter is same as of the MNMIFLG register.
//!
//! Clear NMI interrupt flags that correspond with the passed in bit mask.
//!
//! \return None.
//*****************************************************************************
void SysCtlNmiFlgClr(unsigned long ulNmis)
{
    unsigned long ulMWRAllow;

    // Check the arguments.
    // make sure if reserved bits are not set in the input param
    ASSERT((ulNmis & 0xFFFFF80C) == 0);

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    HWREG(NMI_BASE + NMI_O_FLGCLR) = (ulNmis | NMI_FLG_NMIINT);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;
}

//*****************************************************************************
//! Clear all the NMI that are set.
//!
//! Clear every NMI flag that is currently set.
//!
//! \return None.
//*****************************************************************************
void SysCtlNmiFlgClrAll()
{

    unsigned long ulMWRAllow;
    unsigned long ulNmis;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    ulNmis = SysCtlGetNmiFlagStatus();
    HWREG(NMI_BASE + NMI_O_FLGCLR) = (ulNmis | NMI_FLG_NMIINT);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;

}

//*****************************************************************************
//! Hold Control system and/or Analog subsystem in reset
//!
//! \param ulResetConf System to be held in reset \n
//! bit 0 - Hold Control system in reset \n
//! bit 1 - Hold Analog System in reset
//!
//! Hold the selected subsystem(s) in reset indefinitely.
//!
//! \return None.
//*****************************************************************************
void SysCtlHoldSubSystemInReset(unsigned long ulResetConf)
{

    unsigned long ulMWRAllow;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    HWREG(SYSCTL_CRESCNF) &=
        ~((ulResetConf & SYSCTL_RES_CNF_MASK) << SYSCTL_RES_CNF_SHIFT);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;

}

//*****************************************************************************
//! Release Control system and/or Analog subsystem from reset
//!
//! \param ulResetConf System to be released from reset \n
//! bit 0 - Release Control system from reset \n
//! bit 1 - Release Analog System from reset
//!
//! Release the selected subsystem(s) from reset immediately.
//!
//! \return - None
//*****************************************************************************
void SysCtlReleaseSubSystemFromReset(unsigned long ulResetConf)
{
    unsigned long ulMWRAllow;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    HWREG(SYSCTL_CRESCNF) |=
        ((ulResetConf & SYSCTL_RES_CNF_MASK) << SYSCTL_RES_CNF_SHIFT);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;

}

//*****************************************************************************
//! Reset Control and/or analog system
//!
//! \param ulResetConf System to be reset \n
//! bit 0 - Control system reset \n
//! bit 1 - Analog system reset
//!
//! Reset the selected subsystem(s).
//!
//! \return - None
//*****************************************************************************
void SysCtlSubSystemReset(unsigned long ulResetConf)
{
    unsigned long ulMWRAllow;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    HWREG(SYSCTL_CRESCNF) &=
        ~((ulResetConf & SYSCTL_RES_CNF_MASK) << SYSCTL_RES_CNF_SHIFT);
    SysCtlDelay(10);    /*get around 30 clock cycles delay*/
    HWREG(SYSCTL_CRESCNF) |=
        ((ulResetConf & SYSCTL_RES_CNF_MASK) << SYSCTL_RES_CNF_SHIFT);

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;

}

//*****************************************************************************
// Close the Doxygen group.
//! @}
//*****************************************************************************




