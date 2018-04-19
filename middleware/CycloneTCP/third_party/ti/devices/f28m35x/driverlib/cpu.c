//###########################################################################
// FILE:   cpu.c
// TITLE:  Instruction wrappers for special CPU instructions needed by the
//         drivers.
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

#include "driverlib/cpu.h"

//*****************************************************************************
// Wrapper function for the CPSID instruction.  Returns the state of PRIMASK
// on entry.
//*****************************************************************************
inline unsigned long
CPUcpsid(void)
{
    // Read PRIMASK and disable interrupts.
    __asm("    mrs     r0, PRIMASK\n"
          "    cpsid   i\n"
          "    bx      lr\n");

    // The following keeps the compiler happy, because it wants to see a
    // return value from this function.  It will generate code to return
    // a zero.  However, the real return is the "bx lr" above, so the
    // return(0) is never executed and the function returns with the value
    // you expect in R0.
    return(0);
}

//*****************************************************************************
// Wrapper function returning the state of PRIMASK (indicating whether
// interrupts are enabled or disabled).
//*****************************************************************************
inline unsigned long
CPUprimask(void)
{
    // Read PRIMASK and disable interrupts.
    __asm("    mrs     r0, PRIMASK\n"
          "    bx      lr\n");

    // The following keeps the compiler happy, because it wants to see a
    // return value from this function.  It will generate code to return
    // a zero.  However, the real return is the "bx lr" above, so the
    // return(0) is never executed and the function returns with the value
    // you expect in R0.
    return(0);
}

//*****************************************************************************
// Wrapper function for the CPSIE instruction.  Returns the state of PRIMASK
// on entry.
//*****************************************************************************
inline unsigned long
CPUcpsie(void)
{
    // Read PRIMASK and enable interrupts.
    __asm("    mrs     r0, PRIMASK\n"
          "    cpsie   i\n"
          "    bx      lr\n");

    // The following keeps the compiler happy, because it wants to see a
    // return value from this function.  It will generate code to return
    // a zero.  However, the real return is the "bx lr" above, so the
    // return(0) is never executed and the function returns with the value
    // you expect in R0.
    return(0);
}

//*****************************************************************************
// Wrapper function for the WFI instruction.
//*****************************************************************************
inline void
CPUwfi(void)
{
    // Wait for the next interrupt.
    __asm("    wfi\n");
}

//*****************************************************************************
// Wrapper function for writing the BASEPRI register.
//*****************************************************************************
inline void
CPUbasepriSet(unsigned long ulNewBasepri)
{
    // Set the BASEPRI register
    __asm("    msr     BASEPRI, r0\n");
}

//*****************************************************************************
// Wrapper function for reading the BASEPRI register.
//*****************************************************************************
inline unsigned long
CPUbasepriGet(void)
{
    // Read BASEPRI
    __asm("    mrs     r0, BASEPRI\n"
          "    bx      lr\n");

    // The following keeps the compiler happy, because it wants to see a
    // return value from this function.  It will generate code to return
    // a zero.  However, the real return is the "bx lr" above, so the
    // return(0) is never executed and the function returns with the value
    // you expect in R0.
    return(0);
}




