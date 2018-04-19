//###########################################################################
// FILE:   ram.h
// TITLE:  Defines and Macros for the IPC controller.
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

#ifndef __RAM_H__
#define __RAM_H__

//*****************************************************************************
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
// The following are values that can be passed to RamMReqSharedMemAccess() as 
// ulMask parameter to configure MSXMSEL register for master access to Sx
// RAM block.
//*****************************************************************************
#define S0_ACCESS                   0x00000001   // Master Access to S0
#define S1_ACCESS                   0x00000002   // Master Access to S1
#define S2_ACCESS                   0x00000004   // Master Access to S2
#define S3_ACCESS                   0x00000008   // Master Access to S3
#define S4_ACCESS                   0x00000010   // Master Access to S4
#define S5_ACCESS                   0x00000020   // Master Access to S5
#define S6_ACCESS                   0x00000040   // Master Access to S6
#define S7_ACCESS                   0x00000080   // Master Access to S7

//*****************************************************************************
// The following are values that can be passed to RamMReqSharedMemAccess() as 
// usMaster parameter to determine which processor to give master access to 
// Sx blocks.
//*****************************************************************************
#define SX_C28MASTER            0x0001
#define SX_M3MASTER             0x0000

//*****************************************************************************
// API Function prototypes
//*****************************************************************************
extern void RAMMReqSharedMemAccess (unsigned long ulMask, 
                                    unsigned short usMaster);
extern unsigned char RAMControlInitM1MsgRam(void);
extern unsigned char RAMControlInitL0L3Ram(void);

//*****************************************************************************
// Mark the end of the C bindings section for C++ compilers.
//*****************************************************************************
#ifdef __cplusplus
}
#endif


#endif //  __RAM_H__



