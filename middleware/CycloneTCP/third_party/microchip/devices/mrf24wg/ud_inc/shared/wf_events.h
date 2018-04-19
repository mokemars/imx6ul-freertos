/*******************************************************************************
 MRF24WG event definitions

  Summary: This module contains defines used to define WiFi events

  Description: None
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

#ifndef __WF_EVENTS_H
#define __WF_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------
// Event Types -- corresponds to eventType parameter in WF_ProcessEvent()
//-----------------------------------------------------------------------
typedef enum t_eventType
{
    // initialization event
    WF_EVENT_INITIALIZATION              = 0, 

    // connection events
    WF_EVENT_CONNECTION_SUCCESSFUL       = 1,
    WF_EVENT_CONNECTION_FAILED           = 2,
    WF_EVENT_CONNECTION_TEMPORARILY_LOST = 3,
    WF_EVENT_CONNECTION_PERMANENTLY_LOST = 4,
    WF_EVENT_CONNECTION_REESTABLISHED    = 5,

    // WiFi scan event
    WF_EVENT_SCAN_RESULTS_READY          = 6,

    // SoftAP events
    WF_EVENT_SOFTAP_NETWORK_STARTED      = 7,
    WF_EVENT_SOFTAP_CLIENT_CONNECT       = 8,
    WF_EVENT_SOFTAP_CLIENT_DISCONNECT    = 9,

    // disconnect complete (app requested disconnect)
    WF_EVENT_DISCONNECT_COMPLETE         = 10,

    // error events
    WF_EVENT_ERROR                       = 11,  // Universal Driver error has occurred

} t_eventType;


// WF_INITIALIZATION_EVENT -- eventData values
//--------------------------------------------
typedef enum t_initEventErrors
{
    WF_INIT_SUCCESSFUL                 = 0,    // MRF24WG initialization successful
    WF_INIT_ERROR_SPI_NOT_CONNECTED    = 1,    // appears that the MRF24WG SPI interface is not connected
    WF_INIT_ERROR_RESET_TIMEOUT        = 2,    // timed out waiting for MRF24WG to come out of reset
    WF_INIT_ERROR_INIT_TIMEOUT         = 3,    // timed out waiting for MRF24WG to initialize after reset
    WF_INIT_RAW_INIT_FAIL              = 4     // timed out waiting for Raw init to complete
} t_initEventErrors;


// WF_EVENT_ERROR -- eventData values
//-----------------------------------
typedef enum t_udEventErrors
{
    UD_SUCCESS = 0,

    // this block of error codes are returned from the MRF24WG in the result field
    // of a managment response message and must not be changed.
    MGMT_RESP_SUCCESS                                              = 1,
    MGMT_RESP_ERROR_INVALID_SUBTYPE                                = 2,
    MGMT_RESP_ERROR_OPERATION_CANCELLED                            = 3,
    MGMT_RESP_ERROR_FRAME_END_OF_LINE_OCCURRED                     = 4,
    MGMT_RESP_ERROR_FRAME_RETRY_LIMIT_EXCEEDED                     = 5,
    MGMT_RESP_ERROR_EXPECTED_BSS_VALUE_NOT_IN_FRAME                = 6,
    MGMT_RESP_ERROR_FRAME_SIZE_EXCEEDS_BUFFER_SIZE                 = 7,
    MGMT_RESP_ERROR_FRAME_ENCRYPT_FAILED                           = 8,
    MGMT_RESP_ERROR_INVALID_PARAM                                  = 9,
    MGMT_RESP_ERROR_AUTH_REQ_ISSUED_WHILE_IN_AUTH_STATE            = 10,
    MGMT_RESP_ERROR_ASSOC_REQ_ISSUED_WHILE_IN_ASSOC_STATE          = 11,
    MGMT_RESP_ERROR_INSUFFICIENT_RESOURCES                         = 12,
    MGMT_RESP_ERROR_TIMEOUT_OCCURRED                               = 13,
    MGMT_RESP_ERROR_BAD_EXCHANGE_ENCOUNTERED_IN_FRAME_RECEPTION    = 14,
    MGMT_RESP_ERROR_AUTH_REQUEST_REFUSED                           = 15,
    MGMT_RESP_ERROR_ASSOCIATION_REQUEST_REFUSED                    = 16,
    MGMT_RESP_ERROR_PRIOR_MGMT_REQUEST_IN_PROGRESS                 = 17,
    MGMT_RESP_ERROR_NOT_IN_JOINED_STATE                            = 18,
    MGMT_RESP_ERROR_NOT_IN_ASSOCIATED_STATE                        = 19,
    MGMT_RESP_ERROR_NOT_IN_AUTHENTICATED_STATE                     = 20,
    MGMT_RESP_ERROR_SUPPLICANT_FAILED                              = 21,
    MGMT_RESP_ERROR_UNSUPPORTED_FEATURE                            = 22,
    MGMT_RESP_ERROR_REQUEST_OUT_OF_SYNC                            = 23,
    MGMT_RESP_ERROR_CP_INVALID_ELEMENT_TYPE                        = 24,
    MGMT_RESP_ERROR_CP_INVALID_PROFILE_ID                          = 25,
    MGMT_RESP_ERROR_CP_INVALID_DATA_LENGTH                         = 26,
    MGMT_RESP_ERROR_CP_INVALID_SSID_LENGTH                         = 27,
    MGMT_RESP_ERROR_CP_INVALID_SECURITY_TYPE                       = 28,
    MGMT_RESP_ERROR_CP_INVALID_SECURITY_KEY_LENGTH                 = 29,
    MGMT_RESP_ERROR_CP_INVALID_WEP_KEY_ID                          = 30,
    MGMT_RESP_ERROR_CP_INVALID_NETWORK_TYPE                        = 31,
    MGMT_RESP_ERROR_CP_INVALID_ADHOC_MODE                          = 32,
    MGMT_RESP_ERROR_CP_INVALID_SCAN_TYPE                           = 33,
    MGMT_RESP_ERROR_CP_INVALID_CP_LIST                             = 34,
    MGMT_RESP_ERROR_CP_INVALID_CHANNEL_LIST_LENGTH                 = 35,
    MGMT_RESP_ERROR_NOT_CONNECTED                                  = 36,
    MGMT_RESP_ERROR_ALREADY_CONNECTING                             = 37,
    MGMT_RESP_ERROR_DISCONNECT_FAILED                              = 38,
    MGMT_RESP_ERROR_NO_STORED_BSS_DESCRIPTOR                       = 39,
    MGMT_RESP_ERROR_INVALID_MAX_POWER                              = 40,
    MGMT_RESP_ERROR_CONNECTION_TERMINATED                          = 41,
    MGMT_RESP_ERROR_HOST_SCAN_NOT_ALLOWED                          = 42,
    MGMT_RESP_ERROR_INVALID_WPS_PIN                                = 44,
    // end of mgmt response error code block

            
    UD_ERROR_MGMT_RESPONSE_TIMEOUT           = 50,     // timed out waiting for mgmt response msg
    UD_ERROR_SCRATCH_UNMOUNT_TIMEOUT         = 51,     // timed out wating for scratch unmount
    UD_ERROR_SCRATCH_MOUNT_TIMEOUT           = 52,     // timed out wating for scratch unmount
    UD_ERROR_RAW_RX_MOUNT_FAILED             = 53,     // notified of Rx msg (mgmt or data), but length was 0
    UD_ERROR_RAW_SET_INDEX_OUT_OF_BOUNDS     = 54,     // trying to set raw index past end of raw window
    UD_ERROR_RAW_GET_BYTE_OUT_OF_BOUNDS      = 55,     // trying to read past end of raw window
    UD_ERROR_RAW_SET_BYTE_OUT_OF_BOUNDS      = 56,     // trying to write past end of raw window
    UD_ERROR_INVALID_MGMT_TYPE               = 57,     // unknown mgmt msg type was received
    UD_ERROR_INVALID_MGMT_SUBTYPE            = 58,     // unexpected mgmt confirm subtype received
    UD_ERROR_UNKNOWN_DEVICE                  = 59,     // Unknown device (not an MRF24WG)
    UD_ERROR_MRF24WB_NOT_SUPPORTED           = 60,     // MRF24WB not supported, only MRF24WG supported
    UD_ERROR_RAW_INTERRUPT_TIMEOUT           = 61,     // Timed out waiting for raw move complete interrupt
    UD_ERROR_UNKNOWN_EVENT_TYPE              = 62,     // Unknown event type received from MRF24WG
    UD_ERROR_BAD_PARAM_IN_CONN_LOST_EVENT    = 63,     // Unknown param in WF_EVENT_CONNECTION_LOST_SUBTYPE event


    UD_ERROR_INIT_NOT_COMPLETE               = 100,     // Cannot connect until initialization complete.
    UD_ERROR_CONNECTION_NULL                 = 101,     // pointer to t_connectionProfile in WF_ConnectionProfileSet() is NULL
    UD_ERROR_INVALID_PROFILE                 = 102,     // invalid profileType in t_connectionProfile
    UD_ERROR_SSID_NOT_DEFINED                = 103,     // An SSID is required for this connection mode
    UD_ERROR_SSID_TOO_LONG                   = 104,     // SSID cannot be longer than 32 characters
    UD_ERROR_INVALID_INFRA_SECURITY_TYPE     = 105,     // Invalid infrastructure security type
    UD_ERROR_INVALID_WEP_KEY_TYPE            = 106,     // Invalid WEP keyType in t_wepContext
    UD_ERROR_INVALID_WPA_PASSPHRASE_LENGTH   = 107,     // WPA/WPA2 ASCII passphrase must be between 8 and 63 characters
    UD_ERROR_INVALID_WPA_PASSPHRASE_CHARS    = 108,     // Passphrase characters must be printable
    UD_ERROR_NULL_CHANNEL_LIST               = 109,     // channel list array is NULL
    UD_ERROR_CHANNEL_LIST_TOO_LONG           = 110,     // channel list too long (perhaps not terminated by 0)
    UD_ERROR_CHANNEL_DUPLICATE               = 111,     // a channel number appears multiple times in the channel list
    UD_ERROR_NO_CHANNELS_DEFINED             = 112,     // no channels in list
    UD_ERROR_INVALID_DEAUTH_PARAM            = 113,     // UdSetReconnectMode(), deauthAction param invalid
    UD_ERROR_INVALID_BEACON_TIMEOUT_PARAM    = 114,     // UdSetReconnectMode(), beaconTimeoutAction param invalid
    UD_ERROR_INVALID_RECONNECT_MODE          = 115,     // UdSetReconnectMode(), can't have retry count of 0 and attempt to reconnect
    UD_ERROR_CHANNEL_NOT_IN_DOMAIN           = 116,     // channel number not in regional domain
    UD_ERROR_INVALID_ADHOC_SECURITY_TYPE     = 117,     // invalid security type for Adhoc connection
    UD_ERROR_INVALID_HIDDEN_SSID             = 118,     // hiddenSsid field must be true or false
    UD_ERROR_INVALID_ADHOC_MODE              = 119,     // mode field in t_adHoc structure invalid
    UD_ERROR_INVALID_ADHOC_RETRY_COUNT       = 120,     // should not retry forever if adhoc mode is WF_ADHOC_CONNECT_THEN_START
    UD_ERROR_WPS_EVENT_TIMEOUT               = 121,     // timed out waiting for WPS event (connection or key)
    UD_ERROR_INVALID_WPS_PIN                 = 122,     // WPS Pin is not valid
    UD_ERROR_INVALID_WPS_MODE                = 123,     // mode field in t_infrastructureWps is not valid
    UD_ERROR_MISMATCH_ROM_VERSION            = 124,     // ROM version must be 0x31
    UD_ERROR_MISMATCH_PATCH_VERSION          = 125,     // Patch version must be 0x08 or greater
    UD_ERROR_INVALID_WPS_CREDENTIALS         = 126,     // Invalid to call WF_WpsCredentialsGet() without WPS connection
    UD_ERROR_INVALID_SOFTAP_SECURITY         = 127,     // SoftAp only supports open WEP-40, or WEP-104 security
    UD_ERROR_BEACON_PERIOD_IS_ZERO           = 128,     // Cannot use beacon period of 0 (use 100)
    UD_ERROR_SOFTAP_INTERNAL_1               = 129,     // Internal driver error (should never happen)
    UD_ERROR_SOFTAP_INTERNAL_2               = 130,     // Internal driver error (should never happen)
    UD_ERROR_PS_POLL_ADHOC_NOT_ALLOWED       = 131,     // PS-Poll not allowed in AdHoc mode
    UD_ERROR_PS_POLL_SOFTAP_NOT_ALLOWED      = 132,     // PS-Poll not allowed in SoftAP mode
    UD_ERROR_SPI_INTERFACE_FAILED            = 133,     // SPI interface is not working, write-read test failed during init
    UD_ERROR_NO_SCAN_IN_SOFTAP               = 134,     // Scan not allowed after creating a SoftAP network.
    UD_ERROR_DISCONNECT_NOT_ALLOWED          = 135,     // WF_Disconnect() not allowed, MRF24WG either trying to reconnect or has no connection
    UD_ERROR_ONLY_VALID_WHEN_NOT_CONNECTED   = 136,     // Can only perform this action when not connected
    UD_ERROR_TX_QUEUE_FULL                   = 137,     // WF_QueueTxPacket() failed because queue is full
    UD_ERROR_INVALID_SCAN_TYPE               = 138,     // WF_ScanContextSet(), scanType parameter is invalid
    UD_ERROR_INVALID_TX_MODE                 = 139,     // WF_TxModeSet(), invalid mode param
    UD_ERROR_INVALID_RTS_THRESHOLD           = 140,     // WF_SetRtsTheshold(), invalid rtsThreshold param
    UD_ERROR_SCAN_NOT_ALLOWED                = 141,     // WF_Scan() not allowed in midst of connection or connection retry
    UD_ERROR_INVALID_SCAN_MODE               = 142,     // WF_Scan(), invalid scanMode param
    UD_ERROR_INVALID_MAX_POWER               = 143,     // WF_SetTxPowerMax, maxTxPower param out of range
    UD_ERROR_INVALID_MULTICAST_FILTER_ID     = 144,     // WF_MulticastConfigure(); invalid Multicast filter ID parameter
    UD_ERROR_RAW_TX_ALLOC_FAILED             = 151,     // AllocateDataTxBuffer(); unable to allocate memory for tx packet
    UD_ERROR_PS_POLL_INVALID                 = 152,     // Cannot enable PS-Poll mode until after WiFi connection complete.
    UD_ERROR_INVALID_DATA_MSG                = 153,     // Internal driver error (should never happen).
    UD_ERROR_MGMT_TX_HIBERNATE               = 155,     // Can't send mgmt msg while in hibernate mode
    UD_ERROR_TX_INTERNAL_1                   = 157,     // Internal driver error (should never happen)
    UD_ERROR_TX_INTERNAL_2                   = 158,     // Internal driver error (should never happen)
    UD_ERROR_TX_INTERNAL_3                   = 159,     // Internal driver error (should never happen)
    UD_ERROR_TX_INTERNAL_4                   = 160,     // Internal driver error (should never happen)
    UD_ERROR_TX_INTERNAL_5                   = 161,     // Internal driver error (should never happen)
    UD_ERROR_TX_INTERNAL_6                   = 162,     // Internal driver error (should never happen)
    UD_ERROR_RX_INTERNAL_1                   = 163,     // Internal driver error (should never happen)
    UD_ERROR_RX_INTERNAL_2                   = 164,     // Internal driver error (should never happen)
    UD_ERROR_RX_INTERNAL_3                   = 165,     // Internal driver error (should never happen)
    UD_ERROR_RX_INTERNAL_4                   = 166,     // Internal driver error (should never happen)
    UD_ERROR_RX_INTERNAL_5                   = 167,     // Internal driver error (should never happen)
    UD_ERROR_RX_INTERNAL_6                   = 168,     // Internal driver error (should never happen)
    UD_ERROR_EINT_INTERNAL_1                 = 169,     // Internal driver error (should never happen)
    UD_ERROR_EINT_INTERNAL_2                 = 170,     // Internal driver error (should never happen)
    UD_ERROR_CONNECTION_INTERNAL_1           = 180,     // Internal driver error (should never happen)

} t_udEventErrors;

#ifdef __cplusplus
}
#endif



#endif /* __WF_EVENTS_H */


