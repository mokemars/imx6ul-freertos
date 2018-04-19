//###########################################################################
// FILE:   ucrc.c
// TITLE:  Driver for the uCRC module.
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

#include "inc/hw_ucrc.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/ucrc.h"
#include "driverlib/debug.h"

//*****************************************************************************
//! \addtogroup ucrc_api
//! @{
//*****************************************************************************

//*****************************************************************************
//! Clear the uCRC result register.
//!
//! \param ulBase is the base address of the uCRC peripheral.
//!
//! Resets the result register back to initial state
//!
//*****************************************************************************
void UCRCClear(unsigned long ulBase){
    // Check the arguments.
    ASSERT(ulBase == UCRC_BASE);

    HWREG(ulBase + UCRC_O_CONTROL) |= UCRC_CONTROL_CLEAR;

}

//*****************************************************************************
//! Configure the uCRC module for a particular type of CRC.
//!
//! \param ulBase is the base address of the uCRC peripheral.
//! \param ulType is the type of CRC to perform.
//!
//! Configures the UCRC peripheral for a given type of CRC.  The \e ucType
//! parameter may be any of the following:
//! - \b UCRC_CONFIG_CRC8 - perform CRC8 (Polynomial = 0x03)
//! - \b UCRC_CONFIG_CRC16_1 - perform CRC16 (Polynomial = 0x8005)
//! - \b UCRC_CONFIG_CRC16_2 - perform CRC16 (Polynomial = 0x1021)
//! - \b UCRC_CONFIG_CRC32 - perform CRC32 (Polynomial = 0x04C11DB7)
//!
//*****************************************************************************
void UCRCConfig(unsigned long ulBase, unsigned long ulType){
    // Check the arguments.
    ASSERT(ulBase == UCRC_BASE);
    ASSERT(ulType &
           (UCRC_CONFIG_CRC8 | UCRC_CONFIG_CRC16_1 | UCRC_CONFIG_CRC16_2 |
    UCRC_CONFIG_CRC32));

    HWREG(ulBase + UCRC_O_CONFIG) = ulType;

}

//*****************************************************************************
//! Perform a CRC calculation
//!
//! \param ulBase is the base address of the uCRC peripheral.
//! \param ulType is the type of CRC to perform.
//! \param pucBuffer is the buffer to perform CRC calculation on.
//! \param ulNumBytes is the number of bytes to perform CRC on.
//!
//! Configures the uCRC module and performs the requested CRC operation.
//!
//! \return Requested CRC value for the memory given.
//!
//*****************************************************************************
unsigned long UCRCCalculation(unsigned long ulBase, unsigned long ulType,
                              unsigned char * pucBuffer,
                              unsigned long ulNumBytes){

    unsigned long ulTemp;
    volatile unsigned long ulPreviousCRC;
    volatile unsigned long ulPreviousMode;
    unsigned char *pucBufPtr;

    // Check the arguments.
    ASSERT(ulBase == UCRC_BASE);
    ASSERT(ulType &
           (UCRC_CONFIG_CRC8 | UCRC_CONFIG_CRC16_1 | UCRC_CONFIG_CRC16_2 |
    UCRC_CONFIG_CRC32));
    
    // Save previous CRC calculation value incase a calculation was already in
    // progress
    ulPreviousCRC = HWREG(ulBase + UCRC_O_RES);
    ulPreviousMode = HWREG(ulBase + UCRC_O_CONFIG);

    UCRCClear(ulBase);
    UCRCConfig(ulBase, ulType);

    // uCRC only calculates on reads from a re-mapped space of memory,
    // so we need to remap the address before we start calculation.
    pucBufPtr = (unsigned char *)UCRC_REMAP_ADDRESS(pucBuffer);

    // Read the buffer while calculating CRC
    while(pucBufPtr <
          (unsigned char *)UCRC_REMAP_ADDRESS(pucBuffer + ulNumBytes)) {
        ulTemp = HWREGB(pucBufPtr++);
    }

    // Read calculated CRC
    ulTemp = HWREG(ulBase + UCRC_O_RES);

    // Restore previous CRC setup and value
    UCRCClear(ulBase);
    HWREG(ulBase + UCRC_O_CONFIG) = ulPreviousMode;
    HWREG(ulBase + UCRC_O_RES) = ulPreviousCRC;

    return ulTemp;

}

//*****************************************************************************
// Close the Doxygen group.
//! @}
//*****************************************************************************



