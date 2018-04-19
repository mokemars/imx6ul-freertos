//###########################################################################
// FILE:   hwbist.c
// TITLE:  M3 Driver for the HWBIST module.
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
//! \addtogroup hwbist_api_m3 M3 HWBIST API Drivers
//! @{
//*****************************************************************************

#include "driverlib/hwbist.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_hwbist.h"
#include "inc/hw_nmi.h"
#include "driverlib/debug.h"

//*****************************************************************************
//
//! \internal
//! Block of NOPs.
//
//*****************************************************************************
#define NOP_SEQUENCE()  \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");      \
    __asm(" NOP");

//*****************************************************************************
//
// Prototypes for the assembly functions located in hwbistcontext.s
//
//*****************************************************************************
extern void HWBISTASMSelfTestRun(void);          // C-callable HWBIST Assembly
                                                // Start Function
extern void HWBISTASMResetHandler;               // Address of HWBIST Reset
                                                // Routine (not C-callable)

//*****************************************************************************
//! Initializes the HWBIST module with default values.
//!
//! \param ulMicroInterval specifies the test micro interval. Valid values are
//!        20 - 22000 for Concerto F28M3Xx devices.
//! \param ulCoverage specifies the test coverage. Valid values are
//!        HWBIST_CONFIG_COV_95PERCENT and HWBIST_CONFIG_COV_99PERCENT.
//! \param ucShiftCLock specifies the scan chain shift clock divisor.
//!
//! \return None.
//*****************************************************************************
void HWBISTSelfTestInit(unsigned long ulMicroInterval,
                       unsigned long ulCoverage,
                       unsigned char ucShiftClock)
{
    unsigned long ulAllowSave;
    
    ASSERT(ulMicroInterval >= 20 && ulMicroInterval <= 22000);

    ASSERT(ulCoverage == HWBIST_CONFIG_COV_95PERCENT ||
           ulCoverage == HWBIST_CONFIG_COV_99PERCENT);

    ulAllowSave = HWREG(SYSCTL_MWRALLOW);
    
    // EALLOW 
    HWREG(SYSCTL_MWRALLOW) = 0xA5A5A5A5;

    NOP_SEQUENCE();

    // Test micro interval
    HWREG(HWBIST_BASE + HWBIST_O_MSTCGCR1) = ulMicroInterval;

    // Macro interval (always 1 for Concerto devices)
    HWREG(HWBIST_BASE + HWBIST_O_MSTCGCR2) = 0x00000001;

    // Coverage
    HWREG(HWBIST_BASE + HWBIST_O_MSTCGCR6) = ulCoverage;

    // Restart test
    HWREG(HWBIST_BASE + HWBIST_O_MSTCGCR5) = 0x0000000a;

    // MSTCGCR7 configuration (F28M3Xx Concerto specific)
    HWREG(HWBIST_BASE + HWBIST_O_MSTCGCR7) = 0x00000005;

    // MSTCGCR8 configuration (F28M3Xx Concerto specific)
    HWREG(HWBIST_BASE + HWBIST_O_MSTCGCR8) = 0x00000032;

    // MSTCPCNT configuration (F28M3Xx Concerto specific)
    HWREG(HWBIST_BASE + HWBIST_O_MSTCPCNT) = 0x0a5a0578;

    // MSTCSADDR configuration (F28M3Xx Concerto specific)
    HWREG(HWBIST_BASE + HWBIST_O_MSTCSADDR) = 0x1f400000;

    NOP_SEQUENCE();

    // Configuration complete
    HWREG(HWBIST_BASE + HWBIST_O_MSTCCONFIG) = 0x0000000a;

    // Return address after HWBIST reset (address of assembly routine)
    HWREG(HWBIST_BASE + HWBIST_O_MSTCRET) = (unsigned long)&HWBISTASMResetHandler;
    
    HWBISTShiftClockSet(ucShiftClock);

    HWREG(SYSCTL_MWRALLOW) = ulAllowSave;
}

//*****************************************************************************
//! Starts a HWBIST self-test run.  This will run one micro-interval.
//!
//! \return true on test completion, false otherwise.
//*****************************************************************************
tBoolean HWBISTSelfTestMicroRun(void)
{
    // Run the self-test assembly routine
    // A HWBIST self-test micro interval will run and return back here
    // after a HWBIST triggered CPU core reset
    HWBISTASMSelfTestRun();
    
    // Check for HWBIST test completion flags
    if(HWREG(HWBIST_BASE + HWBIST_O_MSTCGSTAT) & HWBIST_MSTCGSTAT_BISTDONE)
    {
        // Finished HWBIST test run cycle
        // Call HWBISTResultGet() to retrieve the test result
        return(true);
    }
    
    // HWBIST test not complete
    // The user should continue to call this function until it returns 1
    return(false);
}

//*****************************************************************************
//! Runs a complete HWBIST test.  This function runs until the HWBIST test cycle
//! completes with a status value.  For ease of use, this function also
//! returns a pass or fail value indicating the pass or fail state of the
//! HWBIST test.
//!
//! \return true if the CPU logic passed MISR checks, false if the CPU logic
//!         failed MISR checks.
//*****************************************************************************
tBoolean HWBISTSelfTestFullRun(void)
{
    // Run back-to-back micro tests until test completes
    while(!HWBISTSelfTestMicroRun());

    // Return simple (true/false) test result
    return HWBISTResultBasicGet();
}

//*****************************************************************************
//! Restarts the HWBIST test state.
//!
//! \return None.
//*****************************************************************************
void HWBISTSelfTestRestart(void)
{
    unsigned long ulAllowSave;

    ulAllowSave = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) = 0xA5A5A5A5;

    HWREG(HWBIST_BASE + HWBIST_O_MSTCGCR5) = 0x0000000a;    // RESTART

    HWREG(SYSCTL_MWRALLOW) = ulAllowSave;
}

//*****************************************************************************
//! Injects a logic error to test the HWBIST module.
//!
//! \param ulLogicError is used to inject a specific logic error. Set this
//!        to a non-zero number to inject a logic error.  Set to zero to
//!        disable logic error injection.
//!
//! \return None.
//*****************************************************************************
void HWBISTTestLogicSet(unsigned long ulLogicError)
{
    unsigned long ulAllowSave;

    ulAllowSave = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) = 0xA5A5A5A5;

    HWREG(HWBIST_BASE + HWBIST_O_MSTCTEST) |=
          (ulLogicError << HWBIST_MSTCTEST_TEST_S) & HWBIST_MSTCTEST_TEST_M;

    HWREG(SYSCTL_MWRALLOW) = ulAllowSave;
}

//*****************************************************************************
//! Injects a NMI error to test the HWBIST module.
//!
//! \param bEnable is used to set the NMI error mode.  Set this to true to
//!        inject a NMI error, set to false to disable NMI error injection.
//!
//! \return None.
//*****************************************************************************
void HWBISTTestNmiSet(tBoolean bEnable)
{
    unsigned long ulAllowSave;

    ulAllowSave = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) = 0xA5A5A5A5;

    if(bEnable)
    {
        HWREG(HWBIST_BASE + HWBIST_O_MSTCTEST) |= 0xA << HWBIST_MSTCTEST_TESTNMI_S;
    }
    else
    {
        HWREG(HWBIST_BASE + HWBIST_O_MSTCTEST) &= ~HWBIST_MSTCTEST_TESTNMI_M;
    }

    HWREG(SYSCTL_MWRALLOW) = ulAllowSave;
}

//*****************************************************************************
//! Injects a timeout error to test the HWBIST module.
//!
//! \param bEnable is used to set the timeout error mode.  Set this to true to
//!        inject a timeout error, set to false to disable NMI error injection.
//!
//! \return None.
//*****************************************************************************
void HWBISTTestTimeoutSet(tBoolean bEnable)
{
    unsigned long ulAllowSave;

    ulAllowSave = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) = 0xA5A5A5A5;

    if(bEnable)
    {
        HWREG(HWBIST_BASE + HWBIST_O_MSTCTEST) |= 0xA << HWBIST_MSTCTEST_TESTTO_S;
    }
    else
    {
        HWREG(HWBIST_BASE + HWBIST_O_MSTCTEST) &= ~HWBIST_MSTCTEST_TESTTO_M;
    }

    HWREG(SYSCTL_MWRALLOW) = ulAllowSave;
}

//*****************************************************************************
//! Sets the HWBIST scan chain shift clock divider.
//!
//! \param ucShiftClock is the shift clock divider to use.  Valid values are
//!        HWBIST_CONFIG_SHIFTCLOCKDIV_1, HWBIST_CONFIG_SHIFTCLOCKDIV_2, and
//!        HWBIST_CONFIG_SHIFTCLOCKDIV_4.
//!
//! \return None.
//*****************************************************************************
void HWBISTShiftClockSet(unsigned char ucShiftClock)
{
    unsigned long ulAllowSave;

    ASSERT(ucShiftClock == HWBIST_CONFIG_SHIFTCLOCKDIV_1 ||
           ucShiftClock == HWBIST_CONFIG_SHIFTCLOCKDIV_2 ||
           ucShiftClock == HWBIST_CONFIG_SHIFTCLOCKDIV_4);

    ulAllowSave = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) = 0xA5A5A5A5;

    HWREG(HWBIST_BASE + HWBIST_O_MSTCGCR7) |=
          (ucShiftClock << HWBIST_MSTCGCR7_SCD_S) & HWBIST_MSTCGCR7_SCD_M;

    HWREG(SYSCTL_MWRALLOW) = ulAllowSave;
}

//*****************************************************************************
//! Gets the HWBIST controller status register value.
//!
//! \return The HWBIST status register value.  To interpret the values, check
//!         the individual bits in the returned value.  Valid bit field values
//!         are HWBIST_RESULT_BISTDONE, HWBIST_RESULT_BISTDONE,
//!         HWBIST_RESULT_BISTFAIL, HWBIST_RESULT_INTCMPF, and
//!         HWBIST_RESULT_TOFAIL.
//*****************************************************************************
unsigned long HWBISTResultGet(void)
{
    return HWREG(HWBIST_BASE + HWBIST_O_MSTCGSTAT);
}

//*****************************************************************************
//! Gets the basic status of the HWBIST test.  This function interprets the
//! status flags and MISR value and returns a simple pass or fail indicating
//! if the CPU core passed or failed the test.
//!
//! \return true if the CPU logic passed MISR checks, false if the CPU logic
//!         failed MISR checks.
//*****************************************************************************
tBoolean HWBISTResultBasicGet(void)
{
    unsigned long ulBistResult = HWBISTResultGet();

    if(((ulBistResult & HWBIST_RESULT_BISTDONE) > 0) &&
       ((ulBistResult & HWBIST_RESULT_TOFAIL) == 0) &&
       ((ulBistResult & HWBIST_RESULT_INTCMPF) == 0) &&
       ((ulBistResult & HWBIST_RESULT_BISTFAIL) == 0) &&
       ((ulBistResult & HWBIST_RESULT_NMI) == 0))
    {
        // BIST test flags show that the test controller
        // did not detect any errors, but we still have to
        // check the MISR value
        if(HWBISTCompareMisr(HWBISTMisrGet(), HWBISTMisrGoldenGet()))
        {
            // MISR comparison passed
            return(true);
        }
    }

    // Test not complete or HWBIST failure
    return(false);
}

//*****************************************************************************
//! Gets the HWBIST test MISR value
//!
//! \return The MISR value.  Note that the value is returned as a tMisr type.
//!         Use the HWBISTCompareMisr(...) function to compare tMisr types.
//*****************************************************************************
tMisr HWBISTMisrGet(void)
{
    tMisr mResult;
    mResult.Misr0 = HWREG(HWBIST_BASE + HWBIST_O_MSTCMISR0);
    mResult.Misr1 = HWREG(HWBIST_BASE + HWBIST_O_MSTCMISR1);
    mResult.Misr2 = HWREG(HWBIST_BASE + HWBIST_O_MSTCMISR2);
    mResult.Misr3 = HWREG(HWBIST_BASE + HWBIST_O_MSTCMISR3);
    return(mResult);
}

//*****************************************************************************
//! Gets the HWBIST Golden MISR value from ROM.  This function returns either
//! the 95% coverage value or the 99% coverage value depending on the
//! selected test coverage.
//!
//! \return The MISR value.  Note that the value is returned as a tMisr type.
//!         Use the HWBISTCompareMisr(...) function to compare tMisr types.
//*****************************************************************************
tMisr HWBISTMisrGoldenGet(void)
{
    tMisr mResult;

    // Read golden MISR from ROM based on current configuration
    switch(HWREG(HWBIST_BASE + HWBIST_O_MSTCGCR6))
    {
        case HWBIST_CONFIG_COV_95PERCENT:
            mResult.Misr0 = HWREG(HWBIST_95PERCENT_MISR0);
            mResult.Misr1 = HWREG(HWBIST_95PERCENT_MISR1);
            mResult.Misr2 = HWREG(HWBIST_95PERCENT_MISR2);
            mResult.Misr3 = HWREG(HWBIST_95PERCENT_MISR3);
            break;
        case HWBIST_CONFIG_COV_99PERCENT:
            mResult.Misr0 = HWREG(HWBIST_99PERCENT_MISR0);
            mResult.Misr1 = HWREG(HWBIST_99PERCENT_MISR1);
            mResult.Misr2 = HWREG(HWBIST_99PERCENT_MISR2);
            mResult.Misr3 = HWREG(HWBIST_99PERCENT_MISR3);
            break;
        default:
            // Unrecognized value
            mResult.Misr0 = 0;
            mResult.Misr1 = 0;
            mResult.Misr2 = 0;
            mResult.Misr3 = 0;
            break;
    }

    return(mResult);
}

//*****************************************************************************
//! Compares two MISR values for equality.
//!
//! \param m1 The first MISR value.
//! \param m2 The second MISR value.
//!
//! \return true if the MISR values match, false otherwise.
//*****************************************************************************
tBoolean HWBISTCompareMisr(tMisr m1, tMisr m2)
{
    if((m1.Misr0 == m2.Misr0) &&
       (m1.Misr1 == m2.Misr1) &&
       (m1.Misr2 == m2.Misr2) &&
       (m1.Misr3 == m2.Misr3))
    {
        return(true);
    }
    else
    {
        return(false);
    }
}

//*****************************************************************************
// Close the Doxygen group.
//! @}
//*****************************************************************************
