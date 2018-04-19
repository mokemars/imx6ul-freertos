//###########################################################################
// FILE:   flash.c
// TITLE:  Driver for the flash
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

#include "inc/hw_ints.h"
#include "inc/hw_ipc.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_flash.h"
#include "driverlib/cpu.h"
#include "driverlib/debug.h"
#include "driverlib/flash.h"
#include "driverlib/ipc.h"
#include "driverlib/sysctl.h"
#include "driverlib/device.h"

//*****************************************************************************
//! \addtogroup flash_api
//! @{
//*****************************************************************************

// Functions that will be run from RAM need to be assigned to
// a different section.  This section will then be mapped to a load and
// run address using the linker cmd file.

#pragma CODE_SECTION(FlashInit, "ramfuncs");
#pragma CODE_SECTION(FlashSetup, "ramfuncs");

//*****************************************************************************
//! Initialize Flash waitstates and caches
//!
//! This function initializes the Flash Control registers
//!                   
//! CAUTION: This function MUST be executed out of RAM. Executing it
//! out of OTP/Flash will yield unpredictable results.
//!
//! \return None
//*****************************************************************************
void FlashInit()
{
    unsigned long ulMWRAllow;
    unsigned int i = 0;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    // At reset bank and pump are in sleep
    // A Flash access will power up the bank and pump automatically
    // After a Flash access, bank and pump go to low power mode (configurable
    // in FBFALLBACK/FPAC1 registers)-
    // if there is no further access to flash
    // Power up Flash bank and pump and this also sets the fall back mode of
    // flash and pump as active
    HWREG(FLASH_CONFIG_BASE + FLASHCTRL_O_FPAC1) |= FLASHCTRL_FPAC1_PUMPPWR;
    HWREG(FLASH_CONFIG_BASE +
          FLASHCTRL_O_FBFALLBACK) |= FLASHCTRL_FBFALLBACK_BANKPWR0_M;

    //Disable Cache and prefetch mechanism before changing wait states
    HWREG(FLASH_CONFIG_BASE + FLASHCTRL_O_FRD_INTF_CTRL) = 0;

    // Set waitstates according to frequency
    //                CAUTION
    // Minimum waitstates required for the flash operating
    // at a given CPU rate must be characterized by TI.
    // Refer to the datasheet for the latest information.
    #if CPU_FRQ_125MHZ
    HWREG(FLASH_CONFIG_BASE +
          FLASHCTRL_O_FRDCNTL) = (3 << FLASHCTRL_FRDCNTL_RWAIT_S);
    #endif

    #if CPU_FRQ_100MHZ
    HWREG(FLASH_CONFIG_BASE +
          FLASHCTRL_O_FRDCNTL) = (2 << FLASHCTRL_FRDCNTL_RWAIT_S);
    #endif

    #if CPU_FRQ_75MHZ
    HWREG(FLASH_CONFIG_BASE +
          FLASHCTRL_O_FRDCNTL) = (1 << FLASHCTRL_FRDCNTL_RWAIT_S);
    #endif

    #if CPU_FRQ_60MHZ
    HWREG(FLASH_CONFIG_BASE +
          FLASHCTRL_O_FRDCNTL) = (1 << FLASHCTRL_FRDCNTL_RWAIT_S);
    #endif

    // Enable Cache and prefetch mechanism to improve performance
    // of code executed from Flash.
    HWREG(FLASH_CONFIG_BASE +
          FLASHCTRL_O_FRD_INTF_CTRL) |= FLASHCTRL_FRD_INTF_CTRL_PRG_CACHE_EN;
    HWREG(FLASH_CONFIG_BASE +
          FLASHCTRL_O_FRD_INTF_CTRL) |= FLASHCTRL_FRD_INTF_CTRL_DAT_CACHE_EN;

    // At reset, ECC is enabled
    // If it is disabled by application software and if application again
    // wants to enable ECC
    HWREG(FLASH_ERROR_BASE + FLASHERR_O_ECC_ENABLE) = 0xA;

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;

    // Force a pipeline flush to ensure that the write to
    // the last register configured occurs before returning.
    for(i=0; i<7; i++)
    {
        __asm(" NOP");
    }
}

//*****************************************************************************
//! Setup flash waitstates and caches
//!
//! \param ucFlashWaitState
//!
//! This function sets up the Flash Control registers
//!                   
//! CAUTION: This function MUST be executed out of RAM. Executing it
//! out of OTP/Flash will yield unpredictable results.
//!
//! CAUTION: The flash wait state variable must be a valid value given the 
//! current M3 CPU frequency! Setting a value too low will yield unpredictable 
//! results.
//!
//! \return None
//*****************************************************************************
void FlashSetup(unsigned char ucFlashWaitState)
{
    unsigned long ulMWRAllow;
    unsigned int i = 0;

    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) =  0xA5A5A5A5;

    // At reset bank and pump are in sleep
    // A Flash access will power up the bank and pump automatically
    // After a Flash access, bank and pump go to low power mode (configurable
    // in FBFALLBACK/FPAC1 registers)-
    // if there is no further access to flash
    // Power up Flash bank and pump and this also sets the fall back mode of
    // flash and pump as active
    HWREG(FLASH_CONFIG_BASE + FLASHCTRL_O_FPAC1) |= FLASHCTRL_FPAC1_PUMPPWR;
    HWREG(FLASH_CONFIG_BASE +
          FLASHCTRL_O_FBFALLBACK) |= FLASHCTRL_FBFALLBACK_BANKPWR0_M;

    //Disable Cache and prefetch mechanism before changing wait states
    HWREG(FLASH_CONFIG_BASE + FLASHCTRL_O_FRD_INTF_CTRL) = 0;

    // Set waitstates according to user input
    //                CAUTION
    // Minimum waitstates required for the flash operating
    // at a given CPU rate must be characterized by TI.
    // Refer to the datasheet for the latest information.
    HWREG(FLASH_CONFIG_BASE +
          FLASHCTRL_O_FRDCNTL) = (ucFlashWaitState <<
                                  FLASHCTRL_FRDCNTL_RWAIT_S);


    //Enable Cache and prefetch mechanism to improve performance
    //of code executed from Flash.
    HWREG(FLASH_CONFIG_BASE +
          FLASHCTRL_O_FRD_INTF_CTRL) |= FLASHCTRL_FRD_INTF_CTRL_PRG_CACHE_EN;
    HWREG(FLASH_CONFIG_BASE +
          FLASHCTRL_O_FRD_INTF_CTRL) |= FLASHCTRL_FRD_INTF_CTRL_DAT_CACHE_EN;

    // At reset, ECC is enabled
    // If it is disabled by application software and if application again
    // wants to enable ECC
    HWREG(FLASH_ERROR_BASE + FLASHERR_O_ECC_ENABLE) = 0xA;

    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;

    // Force a pipeline flush to ensure that the write to
    // the last register configured occurs before returning.
    for(i=0; i<7; i++) {
        __asm(" NOP");
    }
}

//*****************************************************************************
//! Master subsystem requests control of the Flash pump.
//!
//! This function allows master subsystem to take control of the Flash pump.
//!
//! CAUTION: This function MUST be executed out of RAM. Executing it
//! out of OTP/Flash will yield unpredictable results.
//!
//!
//! \return None
//*****************************************************************************
void FlashGainPump(void)
{
    // Wait for flashpump to be idle
    while(HWREG(MTOCIPC_BASE + IPC_O_MPUMPREQUEST) != 0) { }

    // Take control of flash pump
    HWREG(MTOCIPC_BASE + IPC_O_MPUMPREQUEST) = IPC_MPUMPREQUEST_KEY | 0x2;
}

//*****************************************************************************
//! Master subsystem releases control of the Flash pump.
//!
//! This function allows master subsystem to release control of the Flash pump.
//!
//! CAUTION: This function MUST be executed out of RAM. Executing it
//! out of OTP/Flash will yield unpredictable results.
//!
//!
//! \return None
//*****************************************************************************
void FlashLeavePump(void)
{
    // Release control of flash pump
    HWREG(MTOCIPC_BASE + IPC_O_MPUMPREQUEST) = IPC_MPUMPREQUEST_KEY | 0x0;
}

//*****************************************************************************
// Close the Doxygen group.
//! @}
//*****************************************************************************





