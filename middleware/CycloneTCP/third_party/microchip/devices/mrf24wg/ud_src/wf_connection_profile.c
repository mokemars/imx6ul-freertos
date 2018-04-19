/*******************************************************************************
 MRF24WG Connection profile functions

  Summary: Contains functions pertaining to WiFi connection profile

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
#include "wf_universal_driver.h"
#include "wf_global_includes.h"

//==============================================================================
//                                  CONSTANTS
//==============================================================================
#define DEFAULT_WEP_KEY_INDEX            (0)

/*-------------------------------------------------------------*/
/* Connection Profile Element ID?s                             */
/* Used in conjunction with the WF_CP_SET_ELEMENT_SUBTYPE and  */
/* WF_CP_GET_ELEMENT_SUBTYPE message subtypes                  */
/*-------------------------------------------------------------*/
typedef enum tCPElementIds
{
    WF_CP_ELEMENT_ALL               = 0,  /* sends all elements in CP struct (not used)*/
    WF_CP_ELEMENT_SSID              = 1,
    WF_CP_ELEMENT_BSSID             = 2,
    WF_CP_ELEMENT_SECURITY          = 3,
    WF_CP_ELEMENT_NETWORK_TYPE      = 4,
    WF_CP_ELEMENT_ADHOC_BEHAVIOR    = 5,
    WF_CP_ELEMENT_WEP_KEY_INDEX     = 6,
    WF_CP_ELEMENT_SSID_TYPE         = 7,
    WF_CP_ELEMENT_WEPKEY_TYPE       = 8,
    WF_CP_ELEMENT_UPDATE_PMK        = 9,
    WF_CP_ELEMENT_READ_WPS_CRED     = 10
} tCPElementIds;

//==============================================================================
//                                  LOCAL DATA TYPES
//==============================================================================
/* header format for response to CP Get Element message */
typedef struct cpElementResponseStruct
{
    t_mgmtMsgRxHdr   mgmtHdr;                /* normal 4-byte hdr for all mgmt responses */
    uint8_t          profileId;
    uint8_t          elementId;
    uint8_t          elementDataLength;
    /* element data follows */
} t_cPElementResponseHdr;

//==============================================================================
//                                  LOCAL FUNCTION PROTOTYPES
//==============================================================================
static void LowLevel_CPSetElement(uint8_t elementId,
                                  uint8_t *p_elementData,
                                  uint8_t elementDataLength);

static void LowLevel_CPGetElement(uint8_t elementId,
                                  uint8_t *p_elementData,
                                  uint8_t elementDataLength,
                                  bool    dataReadAction);

static void SetSecurity(uint8_t securityType,
                           uint8_t *p_securityKey,
                           uint8_t securityKeyLength);

//==============================================================================
//                                  LOCAL GLOBALS
//==============================================================================
static uint8_t g_cpid;
#if defined(WF_USE_WPS)
static t_wpsCredentials g_wpsCredentials;
#endif

INLINE uint8_t GetCpid(void)
{
    return g_cpid;
}



/*******************************************************************************
  Function:
    void CreateCP(void)

  Summary:
    Creates a Connection Profile on the MRF24W.

  Description:
    Requests the MRF24W to create a Connection Profile (CP), assign it an ID,
    and set all the elements to default values.  The ID returned by this function
    is used in other connection profile functions.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void CreateCP(void)
{
    uint8_t  hdr[2];

    g_cpid = 0xff; 

    hdr[0] = WF_MGMT_REQUEST_TYPE;
    hdr[1] = WF_CP_CREATE_PROFILE_SUBTYPE;

    SendMgmtMsg(hdr,
                sizeof(hdr),
                NULL,            /* no data */
                0);              /* no data */

    /* wait for MRF24W management response, read data, free response after read */
    WaitForMgmtResponseAndReadData(WF_CP_CREATE_PROFILE_SUBTYPE,
                                   1,                             /* num data bytes to read          */
                                   MGMT_RESP_1ST_DATA_BYTE_INDEX, /* read starting at index 4        */
                                   &g_cpid);                      /* write data here                 */
}

/*******************************************************************************
  Function:
    void SsidSet(uint8_t *p_ssid);

  Summary:
    Sets the SSID 

  Description:
    Sets the SSID and SSID Length elements in the Connection Profile.  Note that
    an Access Point can have either a visible or hidden SSID.  If an Access Point
    uses a hidden SSID then an active scan must be used (see scanType field in the
    Connection Algorithm).

  Precondition:
    None

  Parameters:
    p_ssid     - Pointer to the SSID string

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void SsidSet(char *p_ssid)
{
    uint8_t ssidLength = strlen(p_ssid);
    
    LowLevel_CPSetElement(WF_CP_ELEMENT_SSID,   // Element ID                   
                          (uint8_t *)p_ssid,    // pointer to element data
                          ssidLength);          // number of element data bytes
}

/*******************************************************************************
  Function:
    void WF_SsidGet(uint8_t *p_ssid, uint8_t *p_ssidLength)

  Summary:
    Gets the SSID for the specified Connection Profile ID.

  Description:
    Gets the SSID and SSID Length elements in the Connection Profile.

  Precondition:
    None

  Parameters:
    p_ssid - Pointer to the SSID string
    ssidLength - Pumber of bytes in the SSID

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void WF_SsidGet(uint8_t *p_ssid, uint8_t *p_ssidLength)
{
    t_cPElementResponseHdr  mgmtHdr;

    /* Request SSID, but don't have this function read data or free response buffer.       */
    LowLevel_CPGetElement( WF_CP_ELEMENT_SSID,    /* Element ID                            */
                          NULL,                   /* ptr to element data (not used here    */
                          0,                      /* num data bytes to read (not used here */
                          false);                 /* no read, leave response mounted       */

    /* At this point, management response is mounted and ready to be read.                 */
    /* Set raw index to 0, read normal 4 byte header plus the next 3 bytes, these will be: */
    /*   profile id             [4]                                                        */
    /*   element id             [5]                                                        */
    /*   element data length    [6]                                                        */
    RawRead(RAW_SCRATCH_ID, MGMT_RX_BASE, sizeof(t_cPElementResponseHdr), (uint8_t *)&mgmtHdr);

    /* extract SSID length and write to caller */
    *p_ssidLength = mgmtHdr.elementDataLength;

    /* copy SSID name to callers buffer */
    RawReadRelative(RAW_SCRATCH_ID, *p_ssidLength, p_ssid);

    /* free management buffer */
//    DeallocateMgmtRxBuffer();
}

/*******************************************************************************
  Function:
    void NetworkTypeSet(uint8_t networkType)

  Summary:
    Sets the network for the specified Connection Profile ID.

  Description:
    Sets the Network Type element a Connection Profile.  Allowable values are:
    * WF_NETWORK_TYPE_INFRASTRUCTURE
    * WF_NETWORK_TYPE_ADHOC

  Precondition:

  Parameters:
    networkType - Type of network to create (infrastructure or adhoc)

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void NetworkTypeSet(uint8_t networkType)
{
    LowLevel_CPSetElement(WF_CP_ELEMENT_NETWORK_TYPE,   /* Element ID                   */
                          &networkType,                 /* pointer to element data      */
                          1);                           /* number of element data bytes */
}

#if defined(WF_USE_ADHOC)
/*******************************************************************************
  Function:
    SetAdhocSsidType(bool hidden)

  Summary:
    Sets the SSID type

  Description:
    Sets the SSID type element in the Connection Profile.

  Precondition:
    None

  Parameters:
    hidden -- True or False

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
void SetAdhocSsidType(bool hidden)
{
    LowLevel_CPSetElement(WF_CP_ELEMENT_SSID_TYPE,  /* Element ID                   */
                          (uint8_t *)&hidden,       /* pointer to element data      */
                          1);                       /* number of element data bytes */

}
#endif // WF_USE_ADHOC

void SecurityOpenSet(void)
{
    SetSecurity(WF_SECURITY_OPEN, NULL, 0);
}

//============================================================================
void SecurityWepSet(uint8_t wepSecurityType, t_wepContext *p_context)
{
    uint8_t keyLength;

    if (wepSecurityType == WF_SECURITY_WEP_40)
    {
        keyLength = WF_WEP40_KEY_LENGTH;
    }
    else
    {
        keyLength = WF_WEP104_KEY_LENGTH;
    }

    SetSecurity(wepSecurityType,
                p_context->key,
                keyLength);
}

//============================================================================
void SecurityWpaSet(uint8_t securityType, uint8_t *p_key)
{
    SetSecurity(securityType,
                p_key,
                WF_WPA_KEY_LENGTH);
}

#if defined(WF_USE_WPS)
//============================================================================
void SecurityWpsSet(uint8_t wpsSecurityType, uint32_t wpsPin)
{
    char    wpsString[WF_WPS_PIN_LENGTH + 1];


    sprintf(wpsString, "%d", wpsPin);


    if (wpsSecurityType == WF_SECURITY_WPS_PIN)
    {
        SetSecurity(wpsSecurityType,
                    (uint8_t *)wpsString,
                    WF_WPS_PIN_LENGTH);
    }
    // push button
    else
    {
        SetSecurity(wpsSecurityType, NULL, 0);
    }
}
#endif // WF_USE_WPS


//============================================================================
void WF_BssidSet(uint8_t *p_bssid)
{
#if defined(WF_USE_ERROR_CHECKING)
    uint32_t errorCode = UdSetBssid(p_bssid);
    if (errorCode != UD_SUCCESS)
    {
        return;
    }

#endif
    LowLevel_CPSetElement(WF_CP_ELEMENT_BSSID,   // Element ID
                          p_bssid,               // pointer to element data
                          WF_BSSID_LENGTH);      // number of element data bytes

}

#if defined(WF_USE_ADHOC)
// called from SetAdhocContext().  Error checking performed there
void SetHiddenSsid(bool hiddenSsid)
{
    LowLevel_CPSetElement(WF_CP_ELEMENT_SSID_TYPE, // Element ID
                          (uint8_t *)&hiddenSsid,  // pointer to element data
                          1);                      // number of element data bytes
}

// called from SetAdhocContext().  Error checking performed there
void SetAdHocMode(uint8_t mode)
{
    LowLevel_CPSetElement(WF_CP_ELEMENT_ADHOC_BEHAVIOR,  // Element ID
                          &mode,                         // pointer to element data
                          1);                            // number of element data bytes
}
#endif // WF_USE_ADHOC

/*******************************************************************************
  Function:
    void WF_CPSetSecurity(UINT8 securityType,
                          UINT8 *p_securityKey,
                          UINT8 securityKeyLength)

  Summary:
    Sets the security for the specified Connection Profile.

  Description:
    Configures security for a Connection Profile.

    <table>
    Security                                Key         Length
    --------                                ---         ------
    WF_SECURITY_OPEN                        N/A         N/A
    WF_SECURITY_WEP_40                      hex         4, 5 byte keys
    WF_SECURITY_WEP_104                     hex         4, 13 byte keys
    WF_SECURITY_WPA_WITH_KEY                hex         32 bytes
    WF_SECURITY_WPA_WITH_PASS_PHRASE        ascii       8-63 ascii characters
    WF_SECURITY_WPA2_WITH_KEY               hex         32 bytes
    WF_SECURITY_WPA2_WITH_PASS_PHRASE       ascii       8-63 ascii characters
    WF_SECURITY_WPA_AUTO_WITH_KEY           hex         32 bytes
    WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE   ascii       8-63 ascii characters
    </table>

  Precondition:
    MACInit must be called first.

  Parameters:
    securityType      - Value corresponding to the security type desired.
    p_securityKey     - Binary key or passphrase (not used if security is
                         WF_SECURITY_OPEN)
    securityKeyLength - Number of bytes in p_securityKey (not used if security
                         is WF_SECURITY_OPEN)

  Returns:
    None.

  Remarks:
    None.
  *****************************************************************************/
static void SetSecurity(uint8_t securityType,
                        uint8_t *p_securityKey,
                        uint8_t securityKeyLength)
{
    uint8_t  hdrBuf[7];
    uint8_t  *p_key;

    /* Write out header portion of msg */
    hdrBuf[0] = WF_MGMT_REQUEST_TYPE;           // indicate this is a mgmt msg
    hdrBuf[1] = WF_CP_SET_ELEMENT_SUBTYPE;      // mgmt request subtype
    hdrBuf[2] = GetCpid();                      // Connection Profile ID
    hdrBuf[3] = WF_CP_ELEMENT_SECURITY;         // Element ID

    // Next header bytes are really part of data, but need to put them in header
    // bytes in order to prepend to security key
    hdrBuf[5] = securityType;
    hdrBuf[6] = DEFAULT_WEP_KEY_INDEX;       // WEP Key index -- always 0

    // if security is open (no key) or WPS push button method
    if ((securityType == WF_SECURITY_OPEN || securityType == WF_SECURITY_WPS_PUSH_BUTTON))
    {
        hdrBuf[4]         = 2;      // Only data is security type and wep index 
        p_key             = NULL;
        securityKeyLength = 0;
    }
    // else security is selected, so need to send key
    else
    {
        hdrBuf[4] = 2 + securityKeyLength;  // data is security type + wep index + key
        p_key     = p_securityKey;
    }

    SendMgmtMsg(hdrBuf,              // msg header which includes the security type and WEP index)
                sizeof(hdrBuf),      // msg header length
                p_key,               // msg data (security key), can be NULL
                securityKeyLength);  // msg data length (will be 0 if p_securityKey is NULL)

    // wait for mgmt response, free after it comes in, don't need data bytes
    WaitForMgmtResponse(WF_CP_SET_ELEMENT_SUBTYPE);
}

#if defined(WF_USE_WPS)
// called by WPS connection state machine to read and store the WPS credentials
void WpsCredentialsGet(void)
{
    LowLevel_CPGetElement(WF_CP_ELEMENT_READ_WPS_CRED,    // Element ID
                          (uint8_t *)&g_wpsCredentials,   // pointer to element data
                          sizeof(t_wpsCredentials),       // number of element data bytes
                          true);                          // read data, free buffer after read

    // Fix up endian for values greater than uint8_t
    g_wpsCredentials.authType = ntohs( g_wpsCredentials.authType);
    g_wpsCredentials.encType  = ntohs(g_wpsCredentials.encType);
}


void WF_WpsCredentialsGet(t_wpsCredentials *p_cred)
{
#if defined(WF_USE_ERROR_CHECKING)
    if ( !isWpsCredentialsValid() )
    {
        EventEnqueue(WF_EVENT_ERROR, UD_ERROR_INVALID_WPS_CREDENTIALS);
        return;
    }
#endif // WF_USE_ERROR_CHECKING

    if (isWpsCredentialsValid())
    {
        memcpy(p_cred, &g_wpsCredentials, sizeof(t_wpsCredentials));
    }
}
#endif // WF_USE_WPS

/*******************************************************************************
  Function:    
    void LowLevel_CPSetElement(uint8_t elementId,
                               uint8_t *p_elementData,
                               uint8_t elementDataLength)

  Summary:
    Set an element of the connection profile on the MRF24W.

  Description:
    All Connection Profile 'Set Element' functions call this function to 
    construct the management message.  The caller must fix up any endian issues 
    prior to calling this function.

  Precondition:
    MACInit must be called first.

  Parameters:
    elementId - Element that is being set
    p_elementData - Pointer to element data
    elementDataLength - Number of bytes pointed to by p_elementData

  Returns:
    None.
      
  Remarks:
    None.
  *****************************************************************************/
void LowLevel_CPSetElement(uint8_t elementId,
                           uint8_t *p_elementData,
                           uint8_t elementDataLength)
{
    uint8_t  hdrBuf[5];
    
    /* Write out header portion of msg */
    hdrBuf[0] = WF_MGMT_REQUEST_TYPE;       /* indicate this is a mgmt msg     */
    hdrBuf[1] = WF_CP_SET_ELEMENT_SUBTYPE;  /* mgmt request subtype            */     
    hdrBuf[2] = g_cpid;                     /* Connection Profile ID           */
    hdrBuf[3] = elementId;                  /* Element ID                      */
    hdrBuf[4] = elementDataLength;          /* number of bytes of element data */
    
    SendMgmtMsg(hdrBuf,              /* msg header        */
                sizeof(hdrBuf),      /* msg header length */
                p_elementData,       /* msg data          */
                elementDataLength);  /* msg data length   */
    
    WaitForMgmtResponse(WF_CP_SET_ELEMENT_SUBTYPE);

}    

/*******************************************************************************
  Function:    
    static void LowLevel_CPGetElement(uint8_t elementId,
                                      uint8_t *p_elementData,
                                      uint8_t elementDataLength,
                                      bool    dataReadAction)

  Summary:
    Get an element of the connection profile on the MRF24W.

  Description:
    All Connection Profile 'Get Element' functions call this function to 
    construct the management message.  The caller must fix up any endian issues 
    prior to calling this function.

  Precondition:
    MACInit must be called first.

  Parameters:
    elementId - Element that is being read
    p_elementData - Pointer to where element data will be written
    elementDataLength - Number of element data bytes that will be read
    dataReadAction - If TRUE then read data per paramters and free mgmt 
                      response buffer. If FALSE then return after response 
                      received, do not read any data as the caller will do that, 
                      and don't free buffer, as caller will do that as well.

  Returns:
    None.
      
  Remarks:
    None.
  *****************************************************************************/
static void LowLevel_CPGetElement(uint8_t elementId,
                                  uint8_t *p_elementData,
                                  uint8_t elementDataLength,
                                  bool    dataReadAction)
{
    uint8_t  hdrBuf[4];
      
    hdrBuf[0] = WF_MGMT_REQUEST_TYPE;          /* indicate this is a mgmt msg     */
    hdrBuf[1] = WF_CP_GET_ELEMENT_SUBTYPE;  /* mgmt request subtype            */     
    hdrBuf[2] = g_cpid;                       /* Connection Profile ID           */
    hdrBuf[3] = elementId;                  /* Element ID                      */

    SendMgmtMsg(hdrBuf,              /* msg header        */
                sizeof(hdrBuf),      /* msg header length */
                NULL,                /* msg data          */
                0);                  /* msg data length   */
  
    if (dataReadAction == (uint8_t)true)
    {
        /* wait for mgmt response, read desired data, and then free response buffer */
        WaitForMgmtResponseAndReadData(WF_CP_GET_ELEMENT_SUBTYPE, 
                                       elementDataLength,                   /* num data bytes to read                */
                                       sizeof(t_cPElementResponseHdr),      /* index of first byte of element data   */
                                       p_elementData);                      /* where to write element data           */
    }
    else
    {
        WaitForMgmtResponse(WF_CP_GET_ELEMENT_SUBTYPE);
    }                                                    
}
