//###########################################################################
// FILE:   hwbist.h
// TITLE:  Prototypes for the HWBIST driver.
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

#ifndef __HWBIST_H__
#define __HWBIST_H__

#include "inc/hw_types.h"

//*****************************************************************************
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
// Coverage configuration options for HWBISTSelfTestInit(...)
//*****************************************************************************
#define HWBIST_CONFIG_COV_99PERCENT   0x00000000
#define HWBIST_CONFIG_COV_95PERCENT   0x00000001

//*****************************************************************************
// Memory addresses for the 95% coverage MISR
//*****************************************************************************
#define HWBIST_95PERCENT_MISR   0x01000238
#define HWBIST_95PERCENT_MISR0  (HWBIST_95PERCENT_MISR)
#define HWBIST_95PERCENT_MISR1  (HWBIST_95PERCENT_MISR + 4)
#define HWBIST_95PERCENT_MISR2  (HWBIST_95PERCENT_MISR + 8)
#define HWBIST_95PERCENT_MISR3  (HWBIST_95PERCENT_MISR + 12)

//*****************************************************************************
// Memory addresses for the 99% coverage MISR
//*****************************************************************************
#define HWBIST_99PERCENT_MISR   0x01000248
#define HWBIST_99PERCENT_MISR0  (HWBIST_99PERCENT_MISR)
#define HWBIST_99PERCENT_MISR1  (HWBIST_99PERCENT_MISR + 4)
#define HWBIST_99PERCENT_MISR2  (HWBIST_99PERCENT_MISR + 8)
#define HWBIST_99PERCENT_MISR3  (HWBIST_99PERCENT_MISR + 12)

//*****************************************************************************
// Shift clock divider configuration options for HWBISTShiftClockSet(...)
//*****************************************************************************
#define HWBIST_CONFIG_SHIFTCLOCKDIV_1   0x0
#define HWBIST_CONFIG_SHIFTCLOCKDIV_2   0x1
#define HWBIST_CONFIG_SHIFTCLOCKDIV_4   0x2

//*****************************************************************************
// Return bit masks for HWBISTResultGet()
//*****************************************************************************
#define HWBIST_RESULT_BISTDONE   0x1     // HWBIST Test Complete
#define HWBIST_RESULT_NMI        0x4     // Early exit due to NMI
#define HWBIST_RESULT_BISTFAIL   0x8     // HWBIST Failure
#define HWBIST_RESULT_INTCMPF    0x10    // Intermediate Comparison Failure
#define HWBIST_RESULT_TOFAIL     0x20    // Time Out Failure

//*****************************************************************************
// Multiple Input Signature Register 128-bit Type
// Use this struct to pass MISR signatures between functions and the app
//*****************************************************************************
typedef struct
{
    unsigned long Misr0;
    unsigned long Misr1;
    unsigned long Misr2;
    unsigned long Misr3;
}
tMisr;

void HWBISTSelfTestInit(unsigned long ulMicroInterval,
                       unsigned long ulCoverage,
                       unsigned char ucShiftClock);
tBoolean HWBISTSelfTestMicroRun(void);
tBoolean HWBISTSelfTestFullRun(void);
void HWBISTSelfTestRestart(void);
void HWBISTTestLogicSet(unsigned long ulLogicError);
void HWBISTTestNmiSet(tBoolean bEnable);
void HWBISTTestTimeoutSet(tBoolean bEnable);
void HWBISTShiftClockSet(unsigned char ucShiftClock);
unsigned long HWBISTResultGet(void);
tBoolean HWBISTResultBasicGet(void);
tMisr HWBISTMisrGet(void);
tMisr HWBISTMisrGoldenGet(void);
tBoolean HWBISTCompareMisr(tMisr m1, tMisr m2);

//*****************************************************************************
// Mark the end of the C bindings section for C++ compilers.
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __HWBIST_H__
