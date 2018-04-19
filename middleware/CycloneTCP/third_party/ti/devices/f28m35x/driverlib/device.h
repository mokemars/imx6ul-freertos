//###########################################################################
// FILE:   device.h
// TITLE:  Device Specific Definitions
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

#ifndef __DEVICE_H__
#define __DEVICE_H__

//*****************************************************************************
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif


#define   TARGET                     1
//---------------------------------------------------------------------------
// User To Select Target Device:


#define   DSP28_F28M35Ex             0
#define   DSP28_F28M35Mx             0
#define   DSP28_F28M35Hx             TARGET


/*-----------------------------------------------------------------------------
      Target device determines CPU frequency
      (for examples) - either 100 MHz (for H devices) 75 MHz
      (for M devices), and 60 (for E devices). User does not have to 
      change anything here.
-----------------------------------------------------------------------------*/
#if DSP28_F28M35Mx                // F28M36Hx device only
  #define CPU_FRQ_60MHZ     0     // 75 MHz CPU Freq (20 MHz input freq)
  #define CPU_FRQ_75MHZ     1
#elif DSP28_F28M35Ex              // F28M36Px device only
  #define CPU_FRQ_60MHZ     1     // 60 MHz CPU Freq (20 MHz input freq)
  #define CPU_FRQ_75MHZ     0
#else                             // CUSTOM CONFIGURATION
  #define CPU_FRQ_60MHZ     0     // User defined Setup
  #define CPU_FRQ_75MHZ     1
  #define CPU_FRQ_100MHZ    0
  #endif

//*****************************************************************************
// Mark the end of the C bindings section for C++ compilers.
//*****************************************************************************
#ifdef __cplusplus
}
#endif


#endif // __DEVICE_H__



