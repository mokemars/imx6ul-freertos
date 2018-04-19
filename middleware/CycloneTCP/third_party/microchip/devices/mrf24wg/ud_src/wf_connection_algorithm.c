/*******************************************************************************
 MRF24WG Connection algorithm functions

  Summary: Contains functions pertaining to WiFi Connection algorithms

  Description: Contains functions that connect, disconnect, get connection status,
               set reconnection mode, and convert a WPA passphrase to a binary key.
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
#include <string.h>

#include "wf_universal_driver.h"
#include "wf_global_includes.h"


//==============================================================================
//                                  LOCAL DATA TYPES
//==============================================================================

/* header format for response to CA Get Element message */
typedef struct caElementResponseStruct
{
    t_mgmtMsgRxHdr    mgmtHdr;                /* normal 4-byte hdr for all mgmt responses */
    uint8_t           elementId;              /* index 4 */
    uint8_t           elementDataLength;      /* index 5 */
    /* element data follows */
} tCAElementResponseHdr;

//==============================================================================
//                                  LOCAL FUNCTION PROTOTYPES
//==============================================================================
static void LowLevel_CASetElement(uint8_t elementId,
                                  uint8_t *p_elementData,
                                  uint8_t elementDataLength);

#if 0
static void LowLevel_CAGetElement(uint8_t elementId,
                                  uint8_t *p_elementData,
                                  uint8_t elementDataLength,
                                  bool    dataReadAction);
#endif

//==============================================================================
//                                  GLOBAL FUNCTION PROTOTYPES
//==============================================================================

/*******************************************************************************
  Function:
    void Connect()

  Summary:
    Commands the MRF24WB0MA/B or MRF24WG0MA/B to start a connection.

  Description:
    Directs the Connection Manager to scan for and connect to a WiFi network.
    This function does not wait until the connection attempt is successful, but
    returns immediately.  See WF_ProcessEvent for events that can occur as a
    result of a connection attempt being successful or not.

    Note that if the Connection Profile being used has WPA or WPA2 security
    enabled and is using a passphrase, the connection manager will first
    calculate the PSK key, and then start the connection process.  The key
    calculation can take up to 30 seconds.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void Connect(void)
{
    uint8_t  hdrBuf[4];

    /* Write out header portion of msg (which is whole msg, there is no data) */
    hdrBuf[0] = WF_MGMT_REQUEST_TYPE;    /* indicate this is a mgmt msg     */
    hdrBuf[1] = WF_CM_CONNECT_SUBYTPE;   /* mgmt request subtype            */
    hdrBuf[2] = GetCpid();
    hdrBuf[3] = 0;

    SendMgmtMsg(hdrBuf,
                sizeof(hdrBuf),
                NULL,
                0);

    WaitForMgmtResponse(WF_CM_CONNECT_SUBYTPE);
}



void SetRetryCount(uint8_t retryCount)
{
    // since retryCount can be between 0 and 255 there is no error check
    
    LowLevel_CASetElement(WF_CA_ELEMENT_LIST_RETRY_COUNT,       // Element ID
                          &retryCount,                          // pointer to element data
                          1);                                   // number of element data bytes
}


void SetBeaconTimeout(uint8_t beaconTimeout)
{
    LowLevel_CASetElement(WF_CA_ELEMENT_BEACON_TIMEOUT,         // Element ID
                          &beaconTimeout,                       // pointer to element data
                          1);                                   // number of element data bytes
}

void SetDeauthAction(uint8_t deauthAction)
{
    LowLevel_CASetElement(WF_CA_ELEMENT_DEAUTH_ACTION,          // Element ID
                          &deauthAction,                        // pointer to element data
                          1);                                   // number of element data bytes
}

void SetBeaconTimeoutAction(uint8_t beaconTimeoutAction)
{
    LowLevel_CASetElement(WF_CA_ELEMENT_BEACON_TIMEOUT_ACTION,  // Element ID
                          &beaconTimeoutAction,                 // pointer to element data
                          sizeof(beaconTimeoutAction));         // number of element data bytes
}



void WF_Connect(void)
{
#if defined(WF_USE_DEBUG_OUTPUT)
    WF_PRINTF("\r\nWF_Connect() called\r\n");
#endif
    if (isInitComplete())
    {
        StartConnectStateMachine();
    }
    else
    {
        EventEnqueue(WF_EVENT_ERROR, UD_ERROR_INIT_NOT_COMPLETE);
    }
}

/*******************************************************************************
  Function:
    void WF_Disconnect(void)

  Summary:
    Commands the MRF24WG to close any open connections and/or to cease attempting
    to connect.

  Description:
    Directs the Connection Manager to close any open connection or connection
    attempt in progress.  No further attempts to connect are taken until
    Connect() is called.

  Precondition:
    

  Parameters:
    None.

  Returns:
    None

  Remarks:
    None.
  *****************************************************************************/
void WF_Disconnect(void)
{
    uint8_t  hdrBuf[2];
    uint8_t   connectionState; // not used, but required for function call

    /* WARNING !!! :
    * Disconnect is allowed only in connected state.
    * If module FW is in the midst of connection (or reconnection) process, then
    * disconnect can hammer connection process, and furthermore it may cause
    * fatal failure in module FW operation. 
    */

    // verify it is OK to issue a disconnect command
    WF_ConnectionStateGet(&connectionState);
    if ((connectionState != WF_CSTATE_CONNECTED_INFRASTRUCTURE) && (connectionState != WF_CSTATE_CONNECTED_ADHOC))
    {
        EventEnqueue(WF_EVENT_ERROR, UD_ERROR_DISCONNECT_NOT_ALLOWED);
        return;
    }

    hdrBuf[0] = WF_MGMT_REQUEST_TYPE;
    hdrBuf[1] = WF_CM_DISCONNECT_SUBYTPE;

    SendMgmtMsg(hdrBuf,
                sizeof(hdrBuf),
                NULL,
                0);

    WaitForMgmtResponse(WF_CM_DISCONNECT_SUBYTPE);

    UdSetConnectionState(CS_NOT_CONNECTED);
}


/*******************************************************************************
  Function:
    void ChannelListSet(uint8_t *p_channelList)

  Summary:
    Sets the channel list.

  Description:
    Sets the Channel List used by the Connection Algorithm.

  Precondition:
    None

  Parameters:
    p_channelList - Pointer to channel list.

  Returns:
    None.

  Remarks:
    None.
 *****************************************************************************/
void ChannelListSet(uint8_t *p_channelList)
{
    uint8_t length = 0; // used as index and length

    while (1)
    {
        // if found end of list
        if (p_channelList[length] == 0)
        {
            break;
        }
        ++length;

        // safety check in case error checking disabled
        if (length > 14)
        {
            break;
        }
    }

    LowLevel_CASetElement(WF_CA_ELEMENT_CHANNEL_LIST,  // Element ID                   
                          (uint8_t *)p_channelList,    // pointer to element data
                          length);                     // number of element data bytes
}

void WF_ScanContextSet(t_scanContext *p_context)
{
    uint16_t tmp;

#if defined(WF_USE_ERROR_CHECKING)
    uint32_t errorCode = UdSetScanContext(p_context);
    if (errorCode != UD_SUCCESS)
    {
        EventEnqueue(WF_EVENT_ERROR, errorCode);
        return;
    }
#endif

    LowLevel_CASetElement(WF_CA_ELEMENT_SCANTYPE,           // Element ID
                          &p_context->scanType,             // pointer to element data
                          sizeof(p_context->scanType));     // number of element data bytes

    LowLevel_CASetElement(WF_CA_ELEMENT_SCAN_COUNT,
                          &p_context->scanCount,
                          sizeof(p_context->scanCount));

    tmp = htons(p_context->minChannelTime);
    LowLevel_CASetElement(WF_CA_ELEMENT_MIN_CHANNEL_TIME,
                          (uint8_t *)&tmp,
                          sizeof(tmp));

    tmp = htons(p_context->maxChannelTime);
    LowLevel_CASetElement(WF_CA_ELEMENT_MAX_CHANNEL_TIME,
                          (uint8_t *)&tmp,
                          sizeof(tmp));

    tmp = htons(p_context->probeDelay);
    LowLevel_CASetElement(WF_CA_ELEMENT_PROBE_DELAY,
                          (uint8_t *)&tmp,
                          sizeof(tmp));
}

#if defined(WF_USE_ADHOC)
void SetAdhocBeaconPeriod(uint16_t beaconPeriod)
{
    beaconPeriod = htons(beaconPeriod);
    LowLevel_CASetElement(WF_CA_ELEMENT_BEACON_PERIOD,  // Element ID
                          (uint8_t *)&beaconPeriod,     // pointer to element data
                          sizeof(beaconPeriod));        // number of element data bytes
}
#endif // WF_USE_ADHOC

void WF_RssiSet(uint8_t rssi)
{
#if defined(WF_USE_ERROR_CHECKING)
    uint32_t errorCode = UdSetRssi(rssi);
    if (errorCode != UD_SUCCESS)
    {
        return;
    }
#endif
    LowLevel_CASetElement(WF_CA_ELEMENT_RSSI,   // Element ID
                          &rssi,                // pointer to element data
                          sizeof(rssi));        // number of element data bytes 
}


/*******************************************************************************
  Function:
    void WF_ConnectionStateGet(UINT8 *p_state)

  Summary:
    Returns the current connection state.

  Description:
    Returns the current connection state.

  Parameters:
    p_state - Pointer to location where connection state will be written

  Returns:
    None.

  Remarks:
    Response msg is actually two bytes, the second byte being the Connection Profile ID.
    Since this is not being used, set msgData to a 1-byte array.  
  *****************************************************************************/
void WF_ConnectionStateGet(uint8_t *p_state)
{
    uint8_t  hdrBuf[2];
    uint8_t  msgData[1];

    hdrBuf[0] = WF_MGMT_REQUEST_TYPE;
    hdrBuf[1] = WF_CM_GET_CONNECTION_STATUS_SUBYTPE;

    SendMgmtMsg(hdrBuf,
                sizeof(hdrBuf),
                NULL,
                0);

    // wait for mgmt response, read data, free after read
    WaitForMgmtResponseAndReadData(WF_CM_GET_CONNECTION_STATUS_SUBYTPE,
                                   sizeof(msgData),                  // num data bytes to read          
                                   MGMT_RESP_1ST_DATA_BYTE_INDEX,    // only used if num data bytes > 0
                                   msgData);                         // only used if num data bytes > 0

    *p_state = msgData[0]; // connection state
}


void SetListenInterval(uint16_t listenInterval)
{

    /* correct endianness before sending message */
    listenInterval = htons(listenInterval);
    LowLevel_CASetElement(WF_CA_ELEMENT_LISTEN_INTERVAL,    /* Element ID                   */
                         (uint8_t *)&listenInterval,        /* pointer to element data      */
                          sizeof(listenInterval));          /* number of element data bytes */
}

void SetDtimInterval(uint16_t dtimInterval)
{
    /* correct endianness before sending message */
    dtimInterval = htons(dtimInterval);

    LowLevel_CASetElement(WF_CA_ELEMENT_DTIM_INTERVAL,    /* Element ID                   */
                          (uint8_t *)&dtimInterval,       /* pointer to element data      */
                          sizeof(dtimInterval));          /* number of element data bytes */
}


/*******************************************************************************
  Function:
    static void LowLevel_CASetElement(uint8_t elementId,
                                      uint8_t *p_elementData,
                                      uint8_t elementDataLength)

  Summary:
    Set an element of the connection algorithm on the MRF24W.

  Description:
    Low-level function to send the appropriate management message to the
    MRF24W to set the Connection Algorithm element.

  Precondition:
    MACInit must be called first.

  Parameters:
    elementId - Element that is being set
    p_elementData - Pointer to element data
    elementDataLength - Number of bytes pointed to by p_elementData

  Returns:
    None.

  Remarks:
    All Connection Algorithm 'Set Element' functions call this function
    to construct the management message.  The caller must fix up any endian
    issues prior to calling this function.
 *****************************************************************************/
static void LowLevel_CASetElement(uint8_t elementId,
                                  uint8_t *p_elementData,
                                  uint8_t elementDataLength)
{
    uint8_t  hdrBuf[4];

    hdrBuf[0] = WF_MGMT_REQUEST_TYPE;           /* indicate this is a mgmt msg     */
    hdrBuf[1] = WF_CA_SET_ELEMENT_SUBTYPE;      /* mgmt request subtype            */
    hdrBuf[2] = elementId;                      /* Element ID                      */
    hdrBuf[3] = elementDataLength;              /* number of bytes of element data */

    SendMgmtMsg(hdrBuf,
                sizeof(hdrBuf),
                p_elementData,
                elementDataLength);

    WaitForMgmtResponse(WF_CA_SET_ELEMENT_SUBTYPE);
}

#if 0
/*******************************************************************************
  Function:
    static void LowLevel_CAGetElement(uint8_t elementId,
                                      uint8_t *p_elementData,
                                      uint8_t elementDataLength,
                                      uint8_t dataReadAction)

  Summary:
    Get an element of the connection algorithm on the MRF24W.

  Description:
    Low-level function to send the appropriate management message to the
    MRF24W to get the Connection Algorithm element.

  Precondition:
    MACInit must be called first.

  Parameters:
    elementId - Element that is being read
    p_elementData - Pointer to where element data will be written
    elementDataLength - Number of element data bytes that will be read
    dataReadAction - If TRUE then read data per paramters and free mgmt response buffer.
                      If FALSE then return after response received, do not read any data as the
                      caller will do that, and don't free buffer, as caller will do that as well.

  Returns:
    None.

  Remarks:
    All Connection Algorithm 'Get Element' functions call this function to
    construct the management message.  The caller must fix up any endian issues
    after getting the data from this function.
 *****************************************************************************/
void LowLevel_CAGetElement(uint8_t elementId,
                                  uint8_t *p_elementData,
                                  uint8_t elementDataLength,
                                  bool    dataReadAction)
{
    uint8_t  hdrBuf[4];

    hdrBuf[0] = WF_MGMT_REQUEST_TYPE;       /* indicate this is a mgmt msg     */
    hdrBuf[1] = WF_CA_GET_ELEMENT_SUBTYPE;  /* mgmt request subtype            */
    hdrBuf[2] = elementId;                  /* Element ID                      */
    hdrBuf[3] = 0;                          /* not used                        */

    SendMgmtMsg(hdrBuf,
                sizeof(hdrBuf),
                NULL,
                0);

    if (dataReadAction == true)
    {
        /* wait for mgmt response, read desired data, and then free response buffer */
        WaitForMgmtResponseAndReadData(WF_CA_GET_ELEMENT_SUBTYPE,
                                       elementDataLength,                   /* num data bytes to read                */
                                       sizeof(tCAElementResponseHdr),       /* index of first byte of element data   */
                                       p_elementData);                      /* where to write element data           */
    }
    else
    {
        /* wait for mgmt response, don't read any data bytes */
        WaitForMgmtResponse(WF_CA_GET_ELEMENT_SUBTYPE);
    }
}

#endif
