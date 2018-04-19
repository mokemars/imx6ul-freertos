/*******************************************************************************
 MRF24WG External Interrupt

  Summary: Contains functions pertaining MRF24WG external interrupt

  Description: MRF24WG interrupts the host CPU to signal data Rx messages, events,
               and management response messages.
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
#include "wf_universal_driver.h"
#include "wf_global_includes.h"


/*****************************************************************************
  Function:
    void WF_EintHandler(void);

  Summary:
    MRF24WG interrupt handler, called directly from the the interrupt routine,
    _WFInterrupt() in wf_eint_stub.c.

  Description:
    Must be called, once, for each MRF24WG interrupt.

 Parameters:
    None

  Returns:
    None

  Remarks:
    None
*****************************************************************************/
void WF_EintHandler(void)
{
    uint8_t  hostInt;
    uint16_t hostInt2 = 0; // avoid warning
    uint32_t mgmtEventInfo;
    
    // read hostInt register and hostIntMask register to determine cause of interrupt
    // TODO: [NOTE: Stellaris requires two reads to get proper SPI read?]
    hostInt      = Read8BitWFRegister(WF_HOST_INTR_REG);
    //hostInt      = Read8BitWFRegister(WF_HOST_INTR_REG);

    // read the active int bits and mask with enabled interrupts to get the current active, enabled interrupts
    hostInt = hostInt & Read8BitWFRegister(WF_HOST_MASK_REG);

    //------------------------------------------------------------------------------------------------
    // if level two interrupt occurred then either mgmt response or mgmt indicate msg ready to process
    //------------------------------------------------------------------------------------------------
    if ((hostInt & WF_HOST_INT_MASK_INT2) == WF_HOST_INT_MASK_INT2)
    {
        // read level 2 cause bits and clear level 2 interrupt sources
        hostInt2 = Read16BitWFRegister(WF_HOST_INTR2_REG);
        Write16BitWFRegister(WF_HOST_INTR2_REG, hostInt2);

        // read the mailbox register and verify that mgmt response or indicate is ready to read
        mgmtEventInfo =  ((uint32_t)Read16BitWFRegister(WF_HOST_MAIL_BOX_1_MSW_REG)) << 16;  // MS 16 bits
        mgmtEventInfo |= ((uint32_t)Read16BitWFRegister(WF_HOST_MAIL_BOX_1_LSW_REG));        // LS 16 bits
        if ((mgmtEventInfo >> 24) == 0x60) // if mgmt rx msg
        {
            // notify management message received and needs to be processed (either response or indicate)
            SignalMgmtMsgRx();
        }
    }
    
    // if got a Raw Move complete for Data Rx raw window
    if ((hostInt & WF_HOST_INT_MASK_RAW_0_INT_0) == WF_HOST_INT_MASK_RAW_0_INT_0)
    {
        // and were waiting for a data rx raw move complete
        if (RawMoveState[RAW_DATA_RX_ID].waitingForRawMoveCompleteInterrupt)
        {
            RawMoveState[RAW_DATA_RX_ID].rawInterrupt = hostInt;
            RawMoveState[RAW_DATA_RX_ID].waitingForRawMoveCompleteInterrupt = false;
        }
        else
        {
            // should never happen
            EventEnqueue(WF_EVENT_ERROR, UD_ERROR_EINT_INTERNAL_1);
        }
    }

    //-----------------------------------------------------------------------------------
    // if tx raw move complete (signal for allocating a tx packet or sending a tx packet)
    //-----------------------------------------------------------------------------------
    // if got a Raw Move complete for Data Tx window
    if ((hostInt & WF_HOST_INT_MASK_RAW_1_INT_0) == WF_HOST_INT_MASK_RAW_1_INT_0)
    {
        if (RawMoveState[RAW_DATA_TX_ID].waitingForRawMoveCompleteInterrupt)
        {
            RawMoveState[RAW_DATA_TX_ID].rawInterrupt = hostInt;
            RawMoveState[RAW_DATA_TX_ID].waitingForRawMoveCompleteInterrupt = false;
        }
        else
        {
            // should never happen
            EventEnqueue(WF_EVENT_ERROR, UD_ERROR_EINT_INTERNAL_2);
        }
    }

    //----------------------------------------------------------
    // if got a FIFO 0 Threshold Interrupt (Data Fifo Rx signal)
    //----------------------------------------------------------
    if((hostInt & WF_HOST_INT_MASK_FIFO_0_THRESHOLD) == WF_HOST_INT_MASK_FIFO_0_THRESHOLD)
    {
        // Notify Rx state machine that Rx packet is ready
        SignalRxPacketAvailable();
    }

    WF_EintEnable();

    // clear host int sources
    Write8BitWFRegister(WF_HOST_INTR_REG, hostInt);
}
