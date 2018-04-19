/*******************************************************************************
 MRF24WG register access

  Summary: Functions to access registers on MRF24WG

  Description: Same as summary
*******************************************************************************/

/* MRF24WG0M Universal Driver
*
* Copyright (c) 2012-2014, Microchip Technology, Inc. <www.microchip.com>
* Contact Microchip for the latest version.
*
* This program is free software; distributed under the terms of BSD
* license:
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* 1.    Redistributions of source code must retain the above copyright notice, this
*        list of conditions and the following disclaimer.
* 2.    Redistributions in binary form must reproduce the above copyright notice,
*        this list of conditions and the following disclaimer in the documentation
*        and/or other materials provided with the distribution.
* 3.    Neither the name(s) of the above-listed copyright holder(s) nor the names
*        of its contributors may be used to endorse or promote products derived
*        from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//==============================================================================
//                                  INCLUDES
//==============================================================================
#include <stdio.h>
#include "wf_universal_driver.h"
#include "wf_global_includes.h"
#include "wf_demos.h"


//==============================================================================
//                                  LOCAL GLOBALS
//==============================================================================

/*****************************************************************************
 * FUNCTION: Read8BitWFRegister
 *
 * RETURNS: register value
 *
 * PARAMS:
 *      regId -- ID of 8-bit register being read
 *
 *  NOTES: Reads WF 8-bit register
 *****************************************************************************/
uint8_t Read8BitWFRegister(uint8_t regId)
{
    uint8_t txBuf[3];
    uint8_t rxBuf[3];

    txBuf[0] = regId | WF_READ_REGISTER_MASK;


    WF_SpiEnableChipSelect();

    WF_SpiTxRx(txBuf,
               1,
               rxBuf,
               2);

    WF_SpiDisableChipSelect();

    return rxBuf[1];   /* register value returned in the second byte clocking */
}

/*****************************************************************************
 * FUNCTION: Write8BitWFRegister
 *
 * RETURNS: None
 *
 * PARAMS:
 *      regId -- ID of 8-bit register being written to
 *      value -- value to write
 *
 *  NOTES: Writes WF 8-bit register
 *****************************************************************************/
void Write8BitWFRegister(uint8_t regId, uint8_t value)
{
    uint8_t txBuf[3];
    uint8_t rxBuf[3];

    txBuf[0] = regId | WF_WRITE_REGISTER_MASK;
    txBuf[1] = value;

    WF_SpiEnableChipSelect();

    WF_SpiTxRx(txBuf,
               2,
               rxBuf,
               1);

    WF_SpiDisableChipSelect();
}

/*****************************************************************************
 * FUNCTION: Write16BitWFRegister
 *
 * RETURNS: None
 *
 * PARAMS:
 *      regId -- ID of 16-bit register being written to
 *      value -- value to write
 *
 *  NOTES: Writes WF 16-bit register
 *****************************************************************************/
void Write16BitWFRegister(uint8_t regId, uint16_t value)
{
    uint8_t txBuf[3];
    uint8_t rxBuf[3];

    txBuf[0] = regId | WF_WRITE_REGISTER_MASK;
    txBuf[1] = (uint8_t)(value >> 8);       /* MS byte being written     */
    txBuf[2] = (uint8_t)(value & 0x00ff);   /* LS byte being written     */

    WF_SpiEnableChipSelect();

    WF_SpiTxRx(txBuf,
               3,
               rxBuf,
               1);

    WF_SpiDisableChipSelect();
}

/*****************************************************************************
 * FUNCTION: Read16BitWFRegister
 *
 * RETURNS: register value
 *
 * PARAMS:
 *      regId -- ID of 16-bit register being read
 *
 *  NOTES: Reads WF 16-bit register
 *****************************************************************************/
uint16_t Read16BitWFRegister(uint8_t regId)
{
    uint8_t txBuf[3];
    uint8_t rxBuf[3];

    txBuf[0] = regId | WF_READ_REGISTER_MASK;
    WF_SpiEnableChipSelect();

    WF_SpiTxRx(txBuf,
               1,
               rxBuf,
               3);

    WF_SpiDisableChipSelect();

    return (((uint16_t)rxBuf[1]) << 8) | ((uint16_t)(rxBuf[2]));
}

/*****************************************************************************
 * FUNCTION: WriteWFArray
 *
 * RETURNS: None
 *
 * PARAMS:
 *      regId  -- Raw register being written to
 *      pBuf   -- pointer to array of bytes being written
 *      length -- number of bytes in pBuf
 *
 *  NOTES: Writes a data block to specified raw register
 *****************************************************************************/
void WriteWFArray(uint8_t regId, uint8_t *p_Buf, uint16_t length)
{
    uint8_t txBuf[3];
    uint8_t rxBuf[3];

    txBuf[0] = regId;

    WF_SpiEnableChipSelect();

    // output single command byte that tells MRF24WG we are about to do a write
    WF_SpiTxRx(txBuf,
               1,
               rxBuf,
               1);
    
    /* output data array bytes */
    WF_SpiTxRx(p_Buf,
               length,
               rxBuf,
               1);

    WF_SpiDisableChipSelect();
}

/*****************************************************************************
 * FUNCTION: ReadWFArray
 *
 * RETURNS: None
 *
 * PARAMS:
 *      regId  -- Raw register being read from
 *      pBuf   -- pointer where to write out bytes
 *      length -- number of bytes to read
 *
 *  NOTES: Reads a block of data from a raw register
 *****************************************************************************/
void ReadWFArray(uint8_t  regId, uint8_t *p_Buf, uint16_t length)
{
    uint8_t txBuf[3];
    uint8_t rxBuf[3];

    WF_SpiEnableChipSelect();

    /* output command byte */
    txBuf[0] = regId | WF_READ_REGISTER_MASK;

    // output single command byte that tells MRF24WG we are about to do a read
    WF_SpiTxRx(txBuf,
               1,
               rxBuf,
               1);

    /* read data array */
    WF_SpiTxRx(txBuf,
               1,   /* garbage tx byte */
               p_Buf,
               length);
    
    WF_SpiDisableChipSelect();
}



