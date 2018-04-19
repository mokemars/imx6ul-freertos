/*******************************************************************************
 MRF24WG management message processing

  Summary: Contains functions pertaining MRF24WG management messages.
           initialization.

  Description: Same as summary.
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

#if defined(WF_USE_CONSOLE)
#include "wf_console_cmds.h"
#endif

//==============================================================================
//                                  MACROS
//==============================================================================
#define SignalMgmtResponseReceived()    g_mgmtResponseReceived = true
#define ClearMgmtResponseReceived()     g_mgmtResponseReceived = false
#define isMgmtResponseReceived()        g_mgmtResponseReceived

//==============================================================================
//                                  LOCAL DATA TYPES
//==============================================================================


//==============================================================================
//                                  LOCAL GLOBALS
//==============================================================================
uint16_t             g_mgmtBase;         // index within scratch memory where mgmt messages start
static bool volatile g_mgmtRxMsgReady;   // true if new Rx msgmt message to process (response or indicate)
#if defined(WF_USE_SOFTAP)
static t_softApEvent g_softApEvent;      // data for SoftAp connection or disconnection event

#endif 
static bool          g_mgmtResponseReceived;  // true if mgmt response message received

//==============================================================================
//                                  LOCAL FUNCTION PROTOTYPES
//==============================================================================
void MgmtRxTask(void);
static bool isMgmtResponseMsg(void);
static bool isMgmtIndicateMsg(void);

static void WFProcessMgmtIndicateMsg(void);

#if defined(WF_USE_SOFTAP)
void WF_SoftApEventInfoGet(t_softApEvent *p_softApEvent)
{
    memcpy(p_softApEvent, &g_softApEvent, sizeof(t_softApEvent));
}


#endif // WF_USE_SOFTAP

// called during init to determine the base location for mgmt tx and rx messages
void InitMgmtBase(void)
{
    // mgmt base index is located in ScratchPad 0 register after MRF is taken out of reset
    Write16BitWFRegister(WF_INDEX_ADDR_REG, WF_SCRATCHPAD_0_REG);
    g_mgmtBase = Read16BitWFRegister(WF_INDEX_DATA_REG);

    g_mgmtRxMsgReady = false;  // no mgmt rx messages yet
}

/*****************************************************************************
 * FUNCTION: SendMgmtMsg
 *
 * RETURNS:  error code
 *
 * PARAMS:   p_header      -- pointer to mgmt message header data
 *           headerLength  -- number of bytes in the header
 *                              will be written
 *           p_data        -- pointer to mgmt message data
 *           dataLength    -- number of byte of data
 *
 *  NOTES:   Sends a management message
 *****************************************************************************/
void SendMgmtMsg(uint8_t *p_header, uint8_t headerLength, uint8_t *p_data, uint8_t dataLength)
{
    // Ensure not trying to send mgmt msg while in hibernate state
    if (WF_PowerStateGet() == WF_PS_HIBERNATE)
    {
        EventEnqueue(WF_EVENT_ERROR, UD_ERROR_MGMT_TX_HIBERNATE);
        return;
    }
    
    EnsureWFisAwake();

    // write out management header
    RawSetIndex(RAW_SCRATCH_ID, MGMT_TX_BASE);
    RawSetByte(RAW_SCRATCH_ID, p_header, headerLength);

    // write out data (if any)
    if (dataLength > 0)
    {
        RawSetByte(RAW_SCRATCH_ID, p_data, dataLength);
    }

    ClearMgmtResponseReceived();

    // Signal MRF24WG that mgmt message ready to process 
    Write16BitWFRegister(WF_HOST_MAIL_BOX_0_MSW_REG, 0x0400);
    Write16BitWFRegister(WF_HOST_MAIL_BOX_0_LSW_REG, 0x0000);
}


/*****************************************************************************
 * FUNCTION: WaitForMgmtResponse
 *
 * RETURNS:  True if mgmt response received and is valid
 *
 * PARAMS:   expectedSubtype -- The expected subtype of the mgmt response
 *           freeAction      -- FREE_MGMT_BUFFER or DO_NOT_FREE_MGMT_BUFFER
 *
 *  NOTES:   Called after sending a mgmt request.  This function waits for a mgmt
 *           response.  
 *****************************************************************************/
void WaitForMgmtResponse(uint8_t expectedSubtype)
{
    uint32_t startTime, elapsedTime;
    t_mgmtMsgRxHdr  hdr;
    
    // Wait until mgmt response is received
    startTime = WF_TimerRead();
    while ( !isMgmtResponseReceived()  )
    {
        MgmtRxTask();
        
        elapsedTime = GetElapsedTime(startTime, WF_TimerRead());
        if (elapsedTime > 50)
        {
            EventEnqueue(WF_EVENT_ERROR, UD_ERROR_MGMT_RESPONSE_TIMEOUT);
            return;
        }
    }

    // if made it here then a valid management response has been received
    ClearMgmtResponseReceived();

    // validate the mgmt msg response header to ensure the mgmt tx was successful
    RawRead(RAW_SCRATCH_ID, MGMT_RX_BASE, (uint16_t)(sizeof(t_mgmtMsgRxHdr)), (uint8_t *)&hdr);

    if (hdr.subtype != expectedSubtype)
    {
        EventEnqueue(WF_EVENT_ERROR, UD_ERROR_INVALID_MGMT_SUBTYPE);
    }

    if (hdr.result != MGMT_RESP_SUCCESS)
    {
        EventEnqueue(WF_EVENT_ERROR, hdr.result);
     }
}

/*****************************************************************************
 * FUNCTION: WaitForMgmtRespAndReadData
 *
 * RETURNS:  None
 *
 * PARAMS:   expectedSubtype -- management message subtype that we are expecting
 *
 *           numDataBytes    -- Number of data bytes from mgmt response to write to
 *                              p_data.  Data always starts at index 4 of mgmt response.
 *           startIndex      -- index within mgmt data block to start read 
 *
 *           p_data          -- pointer where any desired management data bytes
 *                              will be written
 *
 *  NOTES:   Waits for the mgmt response message and validates it by:
 *             1) checking the result field
 *             2) verifying that the received subtype matches the execpted subtype
 *
 *            In addition, this function reads the desired number of data bytes from
 *            the mgmt response, copies them to p_data, and then frees the mgmt buffer.
 *****************************************************************************/
void WaitForMgmtResponseAndReadData(uint8_t expectedSubtype,
                                    uint8_t numDataBytes,
                                    uint8_t startIndex,
                                    uint8_t *p_data)
{
    // wait for mgmt response and if received valid response, read data bytes
    WaitForMgmtResponse(expectedSubtype);

    /* if caller wants to read data from this mgmt response */
    if (numDataBytes > 0)
    {
        RawRead(RAW_SCRATCH_ID, MGMT_RX_BASE + startIndex, numDataBytes, p_data);
    }
}


// called from interrupt routine when mgmt message is ready; could be a response or a indicate msg.
// There are two possiblities:
//   1) In WaitForMgmtResponse() because we just sent a mgmt msg
//   2) A mgmt indicate occurred, and we could be anywhere in the code
void SignalMgmtMsgRx(void)
{
    g_mgmtRxMsgReady = true;
}

// checks if interrupt has signalled a mgmt message ready to process
INLINE bool isMgmtRx(void)
{
    return g_mgmtRxMsgReady;
}

INLINE void ClearMgmtRx(void)
{
    g_mgmtRxMsgReady = false;
}

// returns true if a mgmt response or indicate msg is ready to process
void MgmtRxTask(void)
{
    // if a mgmt rx msg is ready to read
    if ( isMgmtRx() )
    {
        ClearMgmtRx();  // clear flag

        // if mgmt response
        if (isMgmtResponseMsg())
        {
            SignalMgmtResponseReceived();
        }
        // else if mgmt indicate
        else if (isMgmtIndicateMsg())
        {
            WFProcessMgmtIndicateMsg();
        }
        else
        {
            EventEnqueue(WF_EVENT_ERROR, UD_ERROR_INVALID_MGMT_TYPE);
        }
    }
}


static bool isMgmtResponseMsg(void)
{
    uint8_t flag;

    // read the 'mailbox' for the mgmt response and return true if mgmt response msg is present
    RawRead(RAW_SCRATCH_ID, MGMT_RX_ACK_BASE, 1, &flag);

    // if 'mailbox' flag set
    if (flag == MGMT_RESPONSE_SET_FLAG)
    {
        // clear the 'mailbox' and return true
        RawSetIndex(RAW_SCRATCH_ID, MGMT_RX_ACK_BASE);
        flag = MGMT_RESPONSE_CLEAR_FLAG;
        RawSetByte(RAW_SCRATCH_ID, &flag, 1);
        return true;
    }
    // else no mgmt response msg
    else
    {
        return false;
    }
}

static bool isMgmtIndicateMsg(void)
{
    uint8_t flag;

    // read the 'mailbox' for the mgmt indicate and return true if mgmt indicate msg is present
    RawRead(RAW_SCRATCH_ID, MGMT_INDICATE_ACK_BASE, 1, &flag);

    // if 'mailbox' flag set
    if (flag == MGMT_INDICATE_SET_FLAG)
    {
        // clear the 'mailbox' and return true
        RawSetIndex(RAW_SCRATCH_ID, MGMT_INDICATE_ACK_BASE);
        flag = MGMT_INDICATE_CLEAR_FLAG;
        RawSetByte(RAW_SCRATCH_ID, &flag, 1);
        return true;
    }
    // else not mgmt indicate msg
    else
    {
         return false;
    }
}

static void WFProcessMgmtIndicateMsg(void)
{
    t_mgmtIndicateHdr  hdr;
    uint8_t buf[6];
    uint8_t eventType = 0xff;
    uint32_t eventData = WF_NO_EVENT_DATA;
    bool reportEventToApp = true;

    // read mgmt indicate header (2 bytes)
    RawRead(RAW_SCRATCH_ID, MGMT_INDICATE_BASE, sizeof(t_mgmtIndicateHdr), (uint8_t *)&hdr);

    /* Determine which event occurred and handle it */
    switch (hdr.subType)
    {
        //----------------------------------------------
        case WF_EVENT_CONNECTION_ATTEMPT_STATUS_SUBTYPE:
        //----------------------------------------------
            RawReadRelative(RAW_SCRATCH_ID, 2, buf); // read first 2 bytes after header 
            // if connection attempt successful
            if (buf[0] == CONNECTION_ATTEMPT_SUCCESSFUL)
            {
                // if a real connection (not simply starting SoftAp network)
                if (!isSoftApProfile() )
                {
#if defined(WF_USE_CONSOLE)
                    IncrementConnectionSuccessful();
#endif
                    GetConnectionContext();
                    eventType = WF_EVENT_CONNECTION_SUCCESSFUL;
                    eventData = WF_NO_EVENT_DATA;
                    UdSetConnectionState(CS_CONNECTED);
                }
                // else SoftAP network started
                else
                {
                    eventType = WF_EVENT_SOFTAP_NETWORK_STARTED;
                    eventData = WF_NO_EVENT_DATA;
                }
            }
            /* else connection attempt failed */
            else
            {
#if defined(WF_USE_CONSOLE)
                IncrementConnectionFailed();
#endif
                eventType = WF_EVENT_CONNECTION_FAILED;
                eventData = ((uint32_t)buf[0] << 8) | (uint32_t)buf[1]; // contains connection failure code
                NotifyConnectionLost();
                UdSetConnectionState(CS_NOT_CONNECTED);
            }
#if defined(WF_USE_WPS)
            // if this was a connection using WPS
            if (isWpsConnection())
            {
                // let state machine know that connection event occurred
                SetWpsConnectionEvent(WPS_CONNECT_EVENT);
            }
#endif // WF_USE_WPS
            break;               

        //------------------------------------
        case WF_EVENT_CONNECTION_LOST_SUBTYPE:
        //------------------------------------
            /* read index 2 and 3 from message and store in buf[0] and buf[1]
               buf[0] -- 1: Connection temporarily lost  2: Connection permanently lost 3: Connection Reestablished
               buf[1] -- 0: Beacon Timeout  1: Deauth from AP  */
            RawReadRelative(RAW_SCRATCH_ID, 2, buf);

            if (buf[0] == CONNECTION_TEMPORARILY_LOST)
            {
#if defined(WF_USE_CONSOLE)
                IncrementConnectionTempLost();
#endif
                eventType     = WF_EVENT_CONNECTION_TEMPORARILY_LOST;
                eventData = (uint32_t)buf[1];    // lost due to beacon timeout or deauth 
                UdSetConnectionState(CS_CONNECTION_IN_PROGRESS);
            }
            else if (buf[0] == CONNECTION_PERMANENTLY_LOST)
            {
#if defined(WF_USE_CONSOLE)
                IncrementConnectionPermLost();
#endif
                eventType     = WF_EVENT_CONNECTION_PERMANENTLY_LOST;
                eventData = (uint32_t)buf[1];   // lost due to beacon timeout or deauth 
                NotifyConnectionLost();         // notify WF_Task that connection is lost
                UdSetConnectionState(CS_NOT_CONNECTED);
            }
            else if (buf[0] == CONNECTION_REESTABLISHED)
            {
#if defined(WF_USE_CONSOLE)
                IncrementConnectionRestab();
#endif
                GetConnectionContext();
                eventType     = WF_EVENT_CONNECTION_REESTABLISHED;
                eventData = (uint32_t)buf[1];    /* originally lost due to beacon timeout or deauth */
                UdSetConnectionState(CS_CONNECTED);
            }
            else
            {
                // invalid parameter in received mgmt indicate message
                EventEnqueue(WF_EVENT_ERROR, UD_ERROR_BAD_PARAM_IN_CONN_LOST_EVENT);
            }
            break;

        //---------------------------------------
        case WF_EVENT_SCAN_RESULTS_READY_SUBTYPE:
        //---------------------------------------
            // read index 2 of mgmt indicate to get the number of scan results
            RawReadRelative(RAW_SCRATCH_ID, 2, buf);
            eventType = WF_EVENT_SCAN_RESULTS_READY;
            eventData = (uint32_t)buf[0];          // number of scan results
#if defined(WF_USE_CONSOLE)
            EnableScanResultDisplayStateMachine(eventData);
#endif
            break;

#if 0
        /*-----------------------------------------------------------------*/
        case WF_EVENT_SCAN_IE_RESULTS_READY_SUBTYPE:
        /*-----------------------------------------------------------------*/
            eventType = WF_EVENT_IE_RESULTS_READY;
            /* read indexes 2 and 3 containing the 16-bit value of IE bytes */
            RawReadRelative(RAW_SCRATCH_ID, 2, (uint8_t *)&eventInfo);
            eventData = WFSTOHS(eventInfo);     /* fix endianess of 16-bit value */
            break;
#endif

#if defined(WF_USE_WPS)
        //--------------------------------------------
        case WF_EVENT_KEY_CALCULATION_REQUEST_SUBTYPE:
        //--------------------------------------------
            // this is an internal event and will not be passed to application event callback
            reportEventToApp = false;                    // do not report this event to application

            // read the passphrase data into the structure used by WPS state machine
            RawReadRelative(RAW_SCRATCH_ID, sizeof(t_wpaKeyInfo), (uint8_t *)GetKeyInfoPtr());

            SetWpsConnectionEvent(WPS_PASSPHRASE_EVENT); // notify WPS connection state machine
            break;
#endif // WF_USE_WPS

#if defined(WF_USE_SOFTAP)
        //-----------------------------------------------------------------
        case WF_EVENT_SOFT_AP_EVENT_SUBTYPE:    // Valid only with 3108 or the later module FW version
        //-----------------------------------------------------------------
            
            // read the connection information
            RawReadRelative(RAW_SCRATCH_ID, sizeof(t_softApEvent), (uint8_t *)&g_softApEvent);
            // if SoftAP client joining the network
            if (g_softApEvent.event == WF_SOFTAP_CLIENT_CONNECTED)
            {
                eventType = WF_EVENT_SOFTAP_CLIENT_CONNECT;
                // add to the UD client table
                SoftApAddClient(g_softApEvent.mac);
            }
            // else SoftAP client leaving the network
            else
            {
                eventType = WF_EVENT_SOFTAP_CLIENT_DISCONNECT;
                // remove from UD client table
                SoftApRemoveClient(g_softApEvent.mac);
            }
            break;
#endif // WF_USE_SOFTAP

        //-----------------------------------------------------------------
        case WF_EVENT_DISCONNECT_DONE_SUBTYPE:
        //-----------------------------------------------------------------
            eventType =  WF_EVENT_DISCONNECT_COMPLETE;
            UdSetConnectionState(CS_NOT_CONNECTED);
            NotifyConnectionLost();  // notify WF_Task that connection is lost
            break;

        //------
        default:
        //------
            eventType = WF_EVENT_ERROR;
            eventData = UD_ERROR_UNKNOWN_EVENT_TYPE;
            break;
    }

    if (reportEventToApp)
    {
        EventEnqueue(eventType, eventData);
    }
}

