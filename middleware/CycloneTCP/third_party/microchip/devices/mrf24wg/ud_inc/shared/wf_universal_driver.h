/*******************************************************************************
 MRF24WG primary header file

  Summary: Function prototypes and defines for Universal Driver API.

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

#ifndef __MRF24WG_UNIVERSAL_DRIVER_API_H
#define __MRF24WG_UNIVERSAL_DRIVER_API_H

//==============================================================================
//                                  INCLUDES
//==============================================================================
#include <stdint.h>
#include <stdbool.h>

#include "wf_events.h"
#include "wf_connection_event_codes.h"
#include "wf_stubs.h"
#include "wf_customize.h"


#ifdef __cplusplus
extern "C" {
#endif

#define UD_VERSION  "v2.0"

// Profile types (see WF_ConnectionProfileSet)
enum
{
    WF_INFRASTRUCTURE_PROFILE     = 0,
    WF_INFRASTRUCTURE_WPS_PROFILE = 1,
    WF_SOFTAP_PROFILE            = 2,
    WF_ADHOC_PROFILE              = 3,

    WF_MAX_PROFILE       = WF_ADHOC_PROFILE
};

//==============================================================================
//                                  DEFINES/CONSTANTS
//==============================================================================
#define WF_MAC_ADDRESS_LENGTH               (6)         // MAC addresses are always 6 bytes
#define WF_BSSID_LENGTH                     (6)         // BSSID is always 6 bytes
#define WF_MAX_SSID_LENGTH                  (32)        // does not include string terminator
#define WF_MIN_PASSPHRASE_LENGTH            (8)         // must include string terminator
#define WF_MAX_PASSPHRASE_LENGTH            (64)        // must include string terminator
#define WF_MAX_CHANNEL_LIST_LENGTH          (14)        // max channel list (for Japan) is 1-14
#if defined(WF_USE_WPS)
#define WF_WPS_PIN_LENGTH                   (8)         // 7 digits + checksum byte
#endif
#define WF_WPA_KEY_LENGTH                   (32)        // WPA binary key always 32 bytes
#define WF_WEP40_KEY_LENGTH                 (20)        // 4 keys of 5 bytes each
#define WF_WEP104_KEY_LENGTH                (52)        // 4 keys of 13 bytes each
#define WF_MAX_WEP_KEY_LENGTH               (WF_WEP104_KEY_LENGTH)
#define WF_NO_EVENT_DATA                    ((uint32_t)0xffffffff)
#define WF_RETRY_FOREVER                    (0xff)
#define WF_MAX_NUM_RATES                    (8)
#define WF_MAX_TIMER_COUNT                  ((uint32_t)0xffffffff)  // count at which 32-bit counter rolls back to 0
#define WF_MAX_SOFTAP_CLIENTS               (4)                     // MRF24WG supports 4 SoftAP clients

//==============================================================================
// DEFAULTS - After a reset, the MRF24WG has the following default settings.  These
//            can also be used in various API calls.
//==============================================================================
// see t_scanContext
#define WF_DEFAULT_SCAN_TYPE                WF_ACTIVE_SCAN
#define WF_DEFAULT_SCAN_COUNT               (1)
#define WF_DEFAULT_SCAN_MIN_CHANNEL_TIME    (200)   // ms
#define WF_DEFAULT_SCAN_MAX_CHANNEL_TIME    (400)   // ms
#define WF_DEFAULT_SCAN_PROBE_DELAY         (20)    // us

#define WF_DEFAULT_ADHOC_HIDDEN_SSID        false
#define WF_DEFAULT_ADHOC_BEACON_PERIOD      (100)   // ms
#define WF_DEFAULT_ADHOC_MODE               WF_ADHOC_CONNECT_THEN_START

// see t_psPollContext
#define WF_DEFAULT_PS_LISTEN_INTERVAL       ((uint16_t)1)       // 100ms multiplier, e.g. 1 * 100ms = 100ms
#define WF_DEFAULT_PS_DTIM_INTERVAL         ((uint16_t)2)       // number of beacon periods
#define WF_DEFAULT_PS_DTIM_ENABLED          true                // DTIM wake-up enabled (normally the case)

// see t_txmode
#define WF_DEFAULT_TX_MODE                  WF_TXMODE_G_RATES   // full 802.11 g rates

// see WF_RssiSet()
#define WF_DEFAULT_RSSI                     (255)               // connect to highest RSSI found

// see WF_RtsThresholdSet()
#define WF_DEFAULT_RTS_THRESHOLD            ((uint16_t)2347)

// see WF_SetSecurityWep() and t_WepContext
#define WF_DEFAULT_WEP_KEY_TYPE             WF_SECURITY_WEP_OPENKEY

#define WF_DEFAULT_BEACON_TIMEOUT           (20)               // 20 beacons missed signals a lost connection

//===================
// End DEFAULTS block
//===================

// used in wf_gpio_stub.c
typedef enum
{
    WF_LOW  = 0,
    WF_HIGH = 1
} t_gpioStateType;

// See WF_DeviceInfoGet() and t_mrf24wgDeviceInfo
typedef enum
{
    WF_UNKNOWN_DEVICE = 0,
    WF_MRF24WB_DEVICE = 1,
    WF_MRF24WG_DEVICE = 2
} t_mrfDeviceType;

// Regional domains, see WF_DomainGet()
typedef enum
{
    WF_DOMAIN_FCC   = 0,    // Available Channels: 1 - 11
    WF_DOMAIN_ETSI  = 2,    // Available Channels: 1 - 13
    WF_DOMAIN_JAPAN = 7,    // Available Channels: 1 - 14
    WF_DOMAIN_OTHER = 7    // Available Channels: 1 - 14
} t_domain;

// WiFi security modes (do NOT renumber)
typedef enum
{
    WF_SECURITY_OPEN              = 0,
    WF_SECURITY_WEP_40            = 1,
    WF_SECURITY_WEP_104           = 2,
    WF_SECURITY_WPA               = 3,
    WF_SECURITY_WPA2              = 5,
    WF_SECURITY_WPA_AUTO          = 7,
    WF_SECURITY_WPS_PUSH_BUTTON   = 9,
    WF_SECURITY_WPS_PIN           = 10,
} t_securityMode;

// see WF_SetReconnectMode
typedef enum
{
    WF_DO_NOT_ATTEMPT_TO_RECONNECT = 0,
    WF_ATTEMPT_TO_RECONNECT        = 1
} t_retry;

// WF_ScanContextSet()
typedef enum
{
    WF_ACTIVE_SCAN  = 1,
    WF_PASSIVE_SCAN = 2
} t_scanType;

typedef enum
{
    WF_SECURITY_WEP_SHAREDKEY = 0,
    WF_SECURITY_WEP_OPENKEY   = 1
} t_wepKey;

#if defined(WF_USE_ADHOC)
typedef enum
{
    WF_ADHOC_CONNECT_THEN_START = 0,
    WF_ADHOC_CONNECT_ONLY       = 1,
    WF_ADHOC_START_ONLY         = 2
} t_adhocMode;
#endif // WF_USE_ADHOC

// see WF_PowerStateGet()
typedef enum
{
    WF_PS_HIBERNATE             = 1,
    WF_PS_PS_POLL_DTIM_ENABLED  = 2,
    WF_PS_PS_POLL_DTIM_DISABLED = 3,
    WF_PS_OFF                   = 4
} t_powerSaveStates;

// See WF_TxModeSet()
typedef enum
{
    WF_TXMODE_G_RATES       = 0,
    WF_TXMODE_B_RATES       = 1,
    WF_TXMODE_LEGACY_RATES  = 2
} t_txMode;

// see WF_ConnectionStateGet()
typedef enum
{
    WF_CSTATE_NOT_CONNECTED               = 1,
    WF_CSTATE_CONNECTION_IN_PROGRESS      = 2,
    WF_CSTATE_CONNECTED_INFRASTRUCTURE    = 3,
    WF_CSTATE_CONNECTED_ADHOC             = 4,
    WF_CSTATE_RECONNECTION_IN_PROGRESS    = 5,
    WF_CSTATE_CONNECTION_PERMANENTLY_LOST = 6
} t_connectionState;

//------------------------------------------------------------------------------
// Multicast Filter ID's (see t_multicastConfig() and WF_MulticastConfigure().
// AdHoc can only use WF_MULTICAST_FILTER_1 or WF_MULTICAST_FILTER_2.
//------------------------------------------------------------------------------
typedef enum
{
    WF_MULTICAST_FILTER_1       = 4,
    WF_MULTICAST_FILTER_2       = 5,
    WF_MULTICAST_FILTER_3       = 6,
    WF_MULTICAST_FILTER_4       = 7,
    WF_MULTICAST_FILTER_5       = 8,
    WF_MULTICAST_FILTER_6       = 9,
    WF_MULTICAST_FILTER_7       = 10,
    WF_MULTICAST_FILTER_8       = 11,
    WF_MULTICAST_FILTER_9       = 12,
    WF_MULTICAST_FILTER_10      = 13,
    WF_MULTICAST_FILTER_11      = 14,
    WF_MULTICAST_FILTER_12      = 15,
    WF_MULTICAST_FILTER_13      = 16,
    WF_MULTICAST_FILTER_14      = 17,
    WF_MULTICAST_FILTER_15      = 18,
    WF_MULTICAST_FILTER_16      = 19
} t_multicastFilterId;


#if defined(WF_USE_WPS)
// see WF_WpsCredentialsGet()
typedef enum
{
    WF_AUTH_OPEN       = 0x01,
    WF_AUTH_WPA_PSK    = 0x02,
    WF_AUTH_SHARED     = 0x04,
    WF_AUTH_WPA        = 0x08,
    WF_AUTH_WPA2       = 0x10,
    WF_AUTH_WPA2_PSK   = 0x20
} t_wps_auth;

// see WF_WpsCredentialsGet()
typedef enum
{
    WF_ENC_NONE        = 0x01,
    WF_ENC_WEP         = 0x02,
    WF_ENC_TKIP        = 0x04,
    WF_ENC_AES         = 0x08
} t_wps_enc;
#endif // WF_USE_WPS

#if defined(WF_USE_SOFTAP)
// SoftAP event code -- see t_softAp structure
typedef enum
{
    WF_SOFTAP_CLIENT_CONNECTED    = 0,
    WF_SOFTAP_CLIENT_DISCONNECTED = 1
} t_softApEventCode;

// SoftAP reason code -- see t_softAp structure
typedef enum
{
    WF_SOFTAP_CLIENT_LINK_LOST   = 0,
    WF_SOFTAP_CLIENT_SENT_DEAUTH = 1
} t_softApReasonCode;
#endif // WF_USE_SOFTAP

//==============================================================================
//                                  DATA TYPES
//==============================================================================

typedef struct wpaContext
{
    char passphrase[WF_MAX_PASSPHRASE_LENGTH];

    bool    useBinaryKey;                   // if true, use binary key instead of passphrase
    uint8_t binaryKey[WF_WPA_KEY_LENGTH];   // always 32 bytes

} t_wpaContext;

typedef struct wepContext
{
    uint8_t keyType;                     // WF_SECURITY_WEP_OPENKEY (default) or WF_SECURITY_WEP_SHAREDKEY
    uint8_t key[WF_MAX_WEP_KEY_LENGTH];  // array containing WEP binary security key (4 5-byte keys for WEP-40, 4 13-byte keys for WEP-104)
} t_wepContext;


typedef union
{
    t_wpaContext wpaContext;
    t_wepContext wepContext;
} t_securityContext;

// Configures how the MRF24WG should reconnect after losing a connection.
// For infrastructure mode it is recommended to use the following values:
//      deauthAction        = WF_ATTEMP_TO_RECONNECT
//      beaconTimeout       = 20
//      beaconTimeoutAction = WF_ATTEMP_TO_RECONNECT
//      retryCount          = WF_RETRY_FOREVER (255)
//
// For AdHoc mode it is recommended to use the following values:
//      deauthAction        = WF_ATTEMP_TO_RECONNECT
//      beaconTimeout       = 20
//      beaconTimeoutAction = WF_ATTEMP_TO_RECONNECT
//      retryCount          = 3 (if adHoc mode is WF_ADHOC_CONNECT_THEN_START then will try 3 times to connect and then
//                               and then start its own AdHoc network)
typedef struct
{
    uint8_t deauthAction;           // In the event the MRF receives a deauth from the AP, the MRF can
                                    // be directed to do one of the following:
                                    //   WF_ATTEMP_TO_RECONNECT or WF_DO_NOT_ATTEMP_TO_RECONNECT

    uint8_t beaconTimeout;          // Number of missed beacons before the MRF designates the connection
                                    // is lost.
                                    //   0:     MRF will not monitor the beacon timeout condition
                                    //   1-255: Number of missed beacons before the MRF will designate the
                                    //           connection has been lost

    uint8_t beaconTimeoutAction;    // The action the MRF should take if a beacon timeout occurs:
                                    //   WF_ATTEMPT_TO_RECONNECT or WF_DO_NOT_ATTEMPT_TO_RECONNECT

    uint8_t retryCount;             // 0:     MRF should not try to regain a connection; it will
                                    //         only report disconnect to host
                                    // 1-254: Number of times MRF should try to regain connection
                                    // 255:   MRF will retry forever (do not to use for AdHoc mode)

} t_reconnectMode;

// connection profile for Infrastructure mode where SSID and security passphrases are provided by application
// (if using WPA or WEP security)
typedef struct
{
    uint8_t           securityType;                                // WF_SECURITY_OPEN, WF_SECURITY_WEP_40, etc.
    char              ssid[WF_MAX_SSID_LENGTH + 1];                // ASCII string, terminated by 0
    uint8_t           channelList[WF_MAX_CHANNEL_LIST_LENGTH + 1]; // first 0 value in array terminates the list
    t_securityContext security;                                    // use security-matching struct within union
    t_reconnectMode   reconnectMode;                               // MRF224WG reconnection modes
} t_infrastructure;

#if defined(WF_USE_WPS)
// connection profile for Infrastructure mode where WPS is being used in Pin mode
// (SSID and security info provided by AP)
typedef struct
{
    uint8_t           mode;                          // WF_SECURITY_WPS_PUSH_BUTTON or WF_SECURITY_WPS_PIN
    t_reconnectMode   reconnectMode;                 // MRF224WG reconnection modes
    uint8_t           handshakeTimeout;              // number of seconds to wait for WPS handshake (
    uint32_t          wpsPin;                        // 8-digit pin (only used if mode=WF_SECURITY_WPS_PIN)
    char              ssid[WF_MAX_SSID_LENGTH + 1];  // Required for wps-pin, not used for wps-pushbutton
} t_infrastructureWps;
#endif // WF_USE_WPS

#if defined(WF_USE_SOFTAP)
// connection profile for SoftAP mode (MRF24WG acts like AP and clients connect to it)
typedef struct
{
    char         ssid[WF_MAX_SSID_LENGTH + 1];   // ASCII string, terminated by 0
    uint8_t      channel;                        // channel to use
    uint8_t      securityType;                   // WF_SECURITY_OPEN, WF_SECURITY_WEP_40, or WF_SECURITY_WEP_104
    t_wepContext wepContext;                     // only need to fill in if securityType is WEP
} t_softAp;
#endif // WF_USE_SOFTAP

#if defined(WF_USE_ADHOC)
// connection profile for AdHoc mode (MRF24WG can act like AP (or client)
typedef struct
{
    uint8_t          securityType;                  // WF_SECURITY_OPEN, WF_SECURITY_WEP_40, or WF_SECURITY_WEP_104
    char             ssid[WF_MAX_SSID_LENGTH + 1];  // ASCII string, terminated by 0
    bool             hiddenSsid;                    // True if SSID should be hidden, else False (normally False)
    uint8_t          channelList[WF_MAX_CHANNEL_LIST_LENGTH + 1]; // first 0 value in array terminates the list
    uint16_t         beaconPeriod;                  // beacon period, in ms, recommend 100
    uint8_t          mode;                          // see t_adhoc mode
    t_reconnectMode  reconnectMode;                 // MRF224WG reconnection modes (only used when MRF is client)
    t_wepContext     wepContext;
} t_adHoc;
#endif // WF_USE_ADHOC

// Union of the connection profile types
typedef union
{
    t_infrastructure      infrastructure;
#if defined(WF_USE_WPS)
    t_infrastructureWps   infrastructureWps;
#endif
#if defined(WF_USE_SOFTAP)
    t_softAp              softAp;
#endif
#if defined(WF_USE_ADHOC)
    t_adHoc               adHoc;
#endif
} t_profile;

// See WF_ConnectionProfileSet()
typedef struct
{
    uint8_t   profileType;    // WF_INFRASTRUCTURE, WF_INFRASTRUCTURE_WPS, WF_SOFT_AP, WF_ADHOC
    t_profile profile;
} t_connectionProfile;


// See WF_ScanContextSet()
typedef struct scanContext
{
    uint8_t  scanType;                  // see t_scanType
    uint8_t  scanCount;
    uint16_t minChannelTime;            // ms
    uint16_t maxChannelTime;            // ms
    uint16_t probeDelay;                // us
} t_scanContext;

// used in WF_DeviceInfoGet()
typedef struct mrf24wgDeviceInfo
{
    uint8_t  deviceType;    // see t_deviceType
    uint8_t  romVersion;    // ROM version number
    uint8_t  patchVersion;  // Patch version number
} t_deviceInfo;


// see WF_ConnectionContextGet()
typedef struct 
{
    uint8_t  channel;                       // channel number of current connection
    uint8_t  bssid[WF_MAC_ADDRESS_LENGTH];  // bssid of connected AP
} t_connectionContext;




// See WF_PsPollEnable()
typedef struct psPollContext
{
    uint16_t listenInterval;  // Number of 100ms intervals between instances when
                              // the MRF24W wakes up to receive buffered messages
                              // from the network (1 = 100ms, 2 = 200ms, etc.)
    uint16_t dtimInterval;    // Number of DTIM intervals between instances when
                              // the MRF24W wakes up to receive buffered messages
                              // from the network.
    bool     useDtim;         // true if dtimInterval is being used, else false
} t_psPollContext;

// See WF_MacStatsGet()
typedef struct WFMacStatsStruct
{
    uint32_t MibWEPExcludeCtr;      // Number of frames received with the Protected Frame subfield of the Frame
                                    //  Control field set to zero and the value of dot11ExcludeUnencrypted causes
                                    //  that frame to be discarded
    uint32_t MibTxBytesCtr;         // Total number of Tx bytes that have been transmitted
    uint32_t MibTxMulticastCtr;     // Number of frames successfully transmitted that had the multicast bit set
                                    //  in the destination MAC address.
    uint32_t MibTxFailedCtr;        // Number of Tx frames that failed due to the number of transmits exceeding the retry count
    uint32_t MibTxRtryCtr;          // Number of times a transmitted frame needed to be retried
    uint32_t MibTxMultRtryCtr;      // Number of times a frame was successfully transmitted after more than one retransmission.
    uint32_t MibTxSuccessCtr;       // Number of Tx frames successfully transmitted.
    uint32_t MibRxDupCtr;           // Number of frames received where the Sequence Control field indicates a duplicate.
    uint32_t MibRxCtsSuccCtr;       // Number of CTS frames received in response to an RTS frame.
    uint32_t MibRxCtsFailCtr;       // Number of times an RTS frame was not received in response to a CTS frame.
    uint32_t MibRxAckFailCtr;       // Number of times an Ack was not received in response to a Tx frame.
    uint32_t MibRxBytesCtr;         // Total number of Rx bytes received.
    uint32_t MibRxFragCtr;          // Number of successful received frames (management or data)
    uint32_t MibRxMultCtr;          // Number of frames received with the multicast bit set in the destination MAC address.
    uint32_t MibRxFCSErrCtr;        // Number of frames received with an invalid Frame Checksum (FCS).
    uint32_t MibRxWEPUndecryptCtr;  // Number of frames received where the Protected Frame subfield of the Frame Control Field is set to
                                    //  one and the WEPOn value for the key mapped to the transmitter's MAC address indicates the frame
                                    //  should not have been encrypted.
    uint32_t MibRxFragAgedCtr;      // Number of times that fragments 'aged out', or were not received in the allowable time.
    uint32_t MibRxMICFailureCtr;    // Number of MIC failures that have occurred.
} t_macStats;

// See WF_ScanResultGet()
typedef struct
{
    uint8_t      bssid[WF_BSSID_LENGTH];    // Network BSSID value
    uint8_t      ssid[WF_MAX_SSID_LENGTH];  // Network SSID value [NOT TERMINATED BY A 0]

    /**
      Access point configuration
      <table>
        Bit 7       Bit 6       Bit 5       Bit 4       Bit 3       Bit 2       Bit 1       Bit 0
        -----       -----       -----       -----       -----       -----       -----       -----
        WPA2        WPA         Preamble    Privacy     Reserved    Reserved    Reserved    IE
      </table>

      <table>
      IE         1 if AP broadcasting one or more Information Elements, else 0
      Privacy    0 : AP is open (no security)
                 1: AP using security,  if neither WPA and WPA2 set then security is WEP.
      Preamble   0: AP transmitting with short preamble
                 1: AP transmitting with long preamble
      WPA        Only valid if Privacy is 1.
                   0: AP does not support WPA
                   1: AP supports WPA
      WPA2       Only valid if Privacy is 1.
                   0: AP does not support WPA2
                   1: AP supports WPA2
      </table>
      */
    uint8_t    apConfig;
    uint8_t    reserved;
    uint16_t   beaconPeriod; // Network beacon interval
    uint16_t   atimWindow; // Only valid if bssType = WF_INFRASTRUCTURE

    /*
      List of Network basic rates.  Each rate has the following format:

      Bit 7
        * 0: rate is not part of the basic rates set
        * 1: rate is part of the basic rates set

      Bits 6:0
        Multiple of 500kbps giving the supported rate.  For example, a value of 2
        (2 * 500kbps) indicates that 1mbps is a supported rate.  A value of 4 in
        this field indicates a 2mbps rate (4 * 500kbps).
      */
    uint8_t      basicRateSet[WF_MAX_NUM_RATES];
    uint8_t      rssi;                                  // Signal strength of received frame beacon or probe response
    uint8_t      numRates;                              // Number of valid rates in basicRates
    uint8_t      dtimPeriod;                            // Part of TIM element
    uint8_t      bssType;                               // WF_INFRASTRUCTURE or WF_ADHOC
    uint8_t      channel;                               // Channel number
    uint8_t      ssidLen;                               // Number of valid characters in ssid

} t_scanResult;


// see WF_MulticastConfigure
typedef struct multicastConfigStruct
{
    uint8_t filterId;
    uint8_t macAddress[WF_MAC_ADDRESS_LENGTH];
    uint8_t macBitMask;
} t_multicastConfig;


#if defined(WF_USE_WPS)
// See WF_WpsCredentialsGet()
typedef struct wpsCredentialsStruct
{
    uint8_t  ssid[WF_MAX_SSID_LENGTH];          // network SSID         [must be 32 bytes]
    uint8_t  netKey[64];                        // binary security key
    uint16_t authType;                          // see t_wps_auth
    uint16_t encType;                           // see t_wps_enc
    uint8_t  netIdx;                            // not used
    uint8_t  ssidLen;                           // SSID length
    uint8_t  keyIdx;                            // WEP key index (only valid if encType = WF_ENC_WEP)
    uint8_t  keyLen;                            // key length, in bytes
    uint8_t  bssid[WF_MAC_ADDRESS_LENGTH];      // BSSID
} t_wpsCredentials;
#endif // WF_USE_WPS

#if defined(WF_USE_SOFTAP)
// See WF_SoftApEventInfoGet()
typedef struct
{
    /* event code */
    uint8_t event;

    /* reason code */
    uint8_t reason;

    /* MAC address */
    uint8_t mac[WF_MAC_ADDRESS_LENGTH];
} t_softApEvent;

// table of currently connected SoftAp Clients
typedef struct
{
    // bit position corresponds to index in mac table.
    //   0: index in mac table is unused
    //   1: index in mac table is occupied with a currently connected SoftAP client
    uint8_t slotMask;       // bits 3:0 are used as we support 4 clients

    // An array of MAC addresses
    uint8_t  mac[WF_MAX_SOFTAP_CLIENTS][WF_MAC_ADDRESS_LENGTH];
} t_softApClientTable;


#endif // WF_USE_SOFTAP

//==============================================================================
//                                  MRF24WG API
//==============================================================================

// WiFi init and task functions
//-----------------------------
void WF_Init(void);
void WF_Task(void);

// WiFi Connection functions
//--------------------------
void WF_ConnectionProfileSet(t_connectionProfile *p_profile);
void WF_ConnectionProfileGet(t_connectionProfile *p_profile);
void WF_Connect(void);
void WF_Disconnect(void);
void WF_ConnectionStateGet(uint8_t *p_state);
void WF_ConnectionContextGet(t_connectionContext *p_context);

// WiFi scanning functions
//------------------------
void WF_Scan(void);
void WF_ScanResultGet(uint8_t listIndex, t_scanResult *p_scanResult);

// Power-save mode (PS-Poll)
//--------------------------
#if defined(WF_USE_PSPOLL)
void WF_PsPollEnable(t_psPollContext *p_context);
void WF_PsPollDisable(void);
#endif // WF_USE_PSPOLL
void WF_Hibernate(void);
INLINE uint8_t WF_PowerStateGet(void);

// WPS Credentials
//----------------
#if defined(WF_USE_WPS)
void WF_WpsCredentialsGet(t_wpsCredentials *p_cred);
#endif

#if defined(WF_USE_SOFTAP)
// SoftAP
//-------
void WF_SoftApEventInfoGet(t_softApEvent *p_softApEvent);
void WF_SoftApClientTableGet(t_softApClientTable *p_table);
#endif

// Maximum Tx power functions
//---------------------------
uint8_t WF_TxPowerFactoryMaxGet(void);
void WF_TxPowerMaxSet(uint8_t maxTxPower);

// WiFi status functions
//----------------------
void WF_DeviceInfoGet(t_deviceInfo *p_deviceInfo);
void WF_MacStatsGet(t_macStats *p_stats);

// multicast filter functions
void WF_MulticastForwardAll(void);
void WF_MulticastDiscardAll(void);
void WF_MulticastConfigure(t_multicastConfig *p_config);

// Data Tx functions
//------------------
bool WF_QueueTxPacket(uint8_t *p_packet, uint16_t packetSize);
void WF_TxComplete(uint8_t *p_packet);        // callback function (only called by Universal Driver)

// Data Rx functions
//------------------
void WF_RxPacketReady(void);                 // callback function (only called by Universal Driver)
uint16_t WF_RxPacketLengthGet(void);
void WF_RxPacketCopy(uint8_t *p_buf, uint16_t len);
void WF_RxPacketDeallocate(void);

// Miscellaneous functions
//------------------------
void WF_MacAddressGet(uint8_t *p_macAddress);
uint8_t WF_DomainGet(void);

// Advanced functions.  These are not typically needed because the MRF24WG
// defaults suffice for most applications.
//------------------------------------------------------------------------
void WF_ScanContextSet(t_scanContext *p_context);
void WF_TxModeSet(uint8_t mode);
void WF_BssidSet(uint8_t *p_bssid);
void WF_RssiSet(uint8_t rssi);
void WF_RtsThresholdSet(uint16_t rstThreshold);
void WF_LinkDownThresholdSet(uint8_t threshold);

#ifdef __cplusplus
}
#endif


#endif /* __MRF24WG_UNIVERSAL_DRIVER_API_H */

