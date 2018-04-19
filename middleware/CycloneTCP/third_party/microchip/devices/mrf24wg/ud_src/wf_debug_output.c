/*******************************************************************************
 MRF24WG Universal Driver Debug Output

  Summary: This module contains debug output functions

  Description: None
*******************************************************************************/

/* MRF24WG0M Universal Driver
*
* Copyright (c) 2012-2014, Microchip <www.microchip.com>
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


#include "wf_universal_driver.h"
#include "wf_global_includes.h"
#include "wf_console_color.h"
#include "wf_console.h"

#if defined(WF_USE_DEBUG_OUTPUT) || defined(WF_USE_CONSOLE)


const char * g_authStr[] = {"Open", "WPA (PSK)", "Shared", "WPA", "WPA2", "WPA2 (PSK)"};
const char * g_encTypeStr[] = {"None", "WEP", "TKIP", "AES"};
const char *g_connectionFailureStr[] = {
                                        "NULL",                                  // 0 - not used 
                                        "NULL",                                  // 1 - not used 
                                        "CF_JOIN_FAILURE",                       // 2            
                                        "CF_AUTHENTICATION_FAILURE",             // 3            
                                        "CF_ASSOCIATION_FAILURE",                // 4            
                                        "CF_WEP_HANDSHAKE_FAILURE",              // 5            
                                        "CF_PSK_CALCULATION_FAILURE",            // 6            
                                        "CF_PSK_HANDSHAKE_FAILURE",              // 7            
                                        "CF_ADHOC_JOIN_FAILURE",                 // 8            
                                        "CF_SECURITY_MISMATCH_FAILURE",          // 9            
                                        "CF_NO_SUITABLE_AP_FOUND_FAILURE",       // 10           
                                        "CF_RETRY_FOREVER_NOT_SUPPORTED_FAILURE",// 11           
                                        "CF_LINK_LOST",                          // 12           
                                        "CF_TKIP_MIC_FAILURE",                   // 13           
                                        "CF_RSN_MIXED_MODE_NOT_SUPPORTED",       // 14           
                                        "CF_RECV_DEAUTH",                        // 15           
                                        "CF_RECV_DISASSOC",                      // 16           
                                        "CF_WPS_FAILURE",                        // 17           
                                        "CF_P2P_FAILURE",                        // 18           
                                        "CF_LINK_DOWN"                           // 19           
                                       };

const char *g_connectionLostStr[] =  {
                                        "CL_ASSOCIATION_FAILURE",               // 0 
                                        "CL_BEACON_TIMEOUT",                    // 1 
                                        "CL_DEAUTH_RECEIVED",                   // 2 
                                        "CL_DISASSOCIATE_RECEIVED",             // 3 
                                        "CL_TKIP_MIC_FAILURE",                  // 4 
                                        "CL_LINK_DOWN"                          // 5 
                                      };

const char *g_deauthDisssocReasonStr[] =
                                {
                                        "",                                     // 0 
                                        "DD_UNSPECIFIED",                       // 1 
                                        "DD_PREV_AUTH_NOT_VALID",               // 2 
                                        "DD_DEAUTH_LEAVING",                    // 3 
                                        "DD_DISASSOC_DUE_TO_INACTIVITY",        // 4 
                                        "DD_DISASSOC_AP_BUSY",                  // 5 
                                        "DD_CLASS2_FRAME_FROM_NONAUTH_STA",     // 6 
                                        "DD_CLASS3_FRAME_FROM_NONASSOC_STA",    // 7 
                                        "DD_DISASSOC_STA_HAS_LEFT",             // 8 
                                        "DD_STA_REQ_ASSOC_WITHOUT_AUTH",        // 9 
                                        "",                                     // 10 
                                        "",                                     // 11 
                                        "",                                     // 12 
                                        "DD_INVALID_IE",                        // 13 
                                        "DD_MIC_FAILURE",                       // 14 
                                        "DD_4WAY_HANDSHAKE_TIMEOUT",            // 15 
                                        "DD_GROUP_KEY_HANDSHAKE_TIMEOUT",       // 16 
                                        "DD_IE_DIFFERENT",                      // 17 
                                        "DD_INVALID_GROUP_CIPHER",              // 18 
                                        "DD_INVALID_PAIRWISE_CIPHER",           // 19 
                                        "DD_INVALID_AKMP",                      // 20 
                                        "DD_UNSUPP_RSN_VERSION",                // 21 
                                        "DD_INVALID_RSN_IE_CAP",                // 22 
                                        "DD_IEEE8021X_FAILED",                  // 23 
                                        "DD_CIPHER_SUITE_REJECTED"              // 24 
                                };

const char *g_statusCodeStr[] =
                                    {
                                        "",                                     // 0 
                                        "SC_UNSPECIFIED_FAILURE",               // 1 
                                        "",                                     // 2 
                                        "",                                     // 3 
                                        "",                                     // 4 
                                        "",                                     // 5 
                                        "",                                     // 6 
                                        "",                                     // 7 
                                        "",                                     // 8 
                                        "",                                     // 9 
                                        "SC_CAPS_UNSUPPORTED",                  // 10 
                                        "SC_REASSOC_NO_ASSOC",                  // 11 
                                        "SC_ASSOC_DENIED_UNSPEC",               // 12 
                                        "SC_NOT_SUPPORTED_AUTH_ALG",            // 13 
                                        "SC_UNKNOWN_AUTH_TRANSACTION",          // 14 
                                        "SC_CHALLENGE_FAIL",                    // 15 
                                        "SC_AUTH_TIMEOUT",                      // 16 
                                        "SC_AP_UNABLE_TO_HANDLE_NEW_STA",       // 17 
                                        "SC_ASSOC_DENIED_RATES",                // 18 
                                        "SC_ASSOC_DENIED_NOSHORTPREAMBLE",      // 19 
                                        "SC_ASSOC_DENIED_NOPBCC",               // 20 
                                        "SC_ASSOC_DENIED_NOAGILITY",            // 21 
                                        "",                                     // 22 
                                        "",                                     // 23 
                                        "",                                     // 24 
                                        "SC_ASSOC_DENIED_NOSHORTTIME",          // 25 
                                        "SC_ASSOC_DENIED_NODSSSOFDM",           // 26 
                                        "",                                     // 27 
                                        "",                                     // 28 
                                        "",                                     // 29 
                                        "",                                     // 30 
                                        "",                                     // 31 
                                        "",                                     // 32 
                                        "",                                     // 33 
                                        "",                                     // 34 
                                        "",                                     // 35 
                                        "",                                     // 36 
                                        "",                                     // 37 
                                        "",                                     // 38 
                                        "",                                     // 39 
                                        "SC_NOT_VALID_IE",                      // 40 
                                        "SC_NOT_VALID_GROUPCIPHER",             // 41 
                                        "SC_NOT_VALID_PAIRWISE_CIPHER",         // 42 
                                        "SC_NOT_VALID_AKMP",                    // 43 
                                        "SC_UNSUPPORTED_RSN_VERSION",           // 44 
                                        "SC_INVALID_RSN_IE_CAP",                // 45 
                                        "SC_CIPHER_SUITE_REJECTED",             // 46 
                                        "SC_TIMEOUT"                            // 47 
                                    };

const char *g_wpsState[] =          {
                                        "",                                     // 0
                                        "EAP_EAPOL_START",                      // 1
                                        "EAP_REQ_IDENTITY",                     // 2
                                        "EAP_RSP_IDENTITY",                     // 3
                                        "EAP_WPS_START",                        // 4
                                        "EAP_RSP_M1",                           // 5
                                        "EAP_REQ_M2",                           // 6
                                        "EAP_RSP_M3",                           // 7
                                        "EAP_REQ_M4",                           // 8
                                        "EAP_RSP_M5",                           // 9
                                        "EAP_REQ_M6",                           // 10
                                        "EAP_RSP_M7",                           // 11
                                        "EAP_REQ_M8",                           // 12
                                        "EAP_RSP_DONE",                         // 13
                                        "EAP_FAILURE"                           // 14
                                    };

const char *g_wpsConfigErr[] =      {
                                        "WPS_NOERR",                            // 0
                                        "WPS_SESSION_OVERLAPPED",               // 1
                                        "WPS_DECRYPT_CRC_FAILURE",              // 2
                                        "WPS_24G_NOT_SUPPORTED",                // 3
                                        "WPS_RETRY_FAILURE",                    // 4
                                        "WPS_INVALID_MSG",                      // 5
                                        "WPS_AUTH_FAILURE",                     // 6
                                        "WPS_ASSOC_FAILURE",                    // 7
                                        "WPS_MSG_TIMEOUT",                      // 8
                                        "WPS_SESSION_TIMEOUT",                  // 9
                                        "WPS_DEVPASSWD_AUTH_FAILURE",           // 10
                                        "WPS_NO_CONN_TOREG",                    // 11
                                        "WPS_MULTI_PBC_DETECTED",               // 12
                                        "WPS_EAP_FAILURE",                      // 13
                                        "WPS_DEV_BUSY",                         // 14
                                        "WPS_SETUP_LOCKED"                      // 15
                                    };


const char * g_eventTypeStr[] = {"WF_EVENT_INITIALIZATION",                     // 0
                                 "WF_EVENT_CONNECTION_SUCCESSFUL",              // 1
                                 "WF_EVENT_CONNECTION_FAILED",                  // 2
                                 "WF_EVENT_CONNECTION_TEMPORARILY_LOST",        // 3
                                 "WF_EVENT_CONNECTION_PERMANENTLY_LOST",        // 4
                                 "WF_EVENT_CONNECTION_REESTABLISHED",           // 5
                                 "WF_EVENT_SCAN_RESULTS_READY",                 // 6
                                 "WF_EVENT_SOFTAP_NETWORK_STARTED",             // 7
                                 "WF_EVENT_SOFTAP_CLIENT_CONNECT",              // 8
                                 "WF_EVENT_SOFTAP_CLIENT_DISCONNECT",           // 9
                                 "WF_EVENT_DISCONNECT_COMPLETE",                // 10
                                 "WF_EVENT_ERROR"                               // 11
                              };


//==============================================================================
//                                  LOCAL FUNCTION PROTOTYPES
//==============================================================================
static void OutputSsid(char *p_ssid);
static void OutputChannelList(uint8_t *p_channelList);
static void OutputSecurityType(uint8_t securityType);
static void OutputSecurityType(uint8_t securityType);
static void OutputPassPhrase(char *p_passphrase);
static void OutputWepKey(uint8_t security, uint8_t *p_key);
static void OutputReconnectMode(t_reconnectMode *p_reconnectMode);
static void OutputMac(uint8_t *p_mac);

#if defined(WF_USE_ADHOC)
static void OutputBeaconPeriod(uint16_t beaconPeriod);
static void OutputHiddenSsid(bool hiddenSsid);
static void OutputAdHocMode(uint8_t mode);
#endif // WF_USE_ADHOC

#if defined(WF_USE_WPS)
static void OutputWpsMode(uint8_t wpsMode, uint32_t wpsPin);
static uint8_t GetBitIndex(uint16_t value);
#endif 
static void OutputWepKeyType(uint8_t keyType);
static void DumpConnectionTempLostInfo(uint8_t event, uint32_t eventData);
static void DumpConnectionFailedInfo(uint8_t event, uint32_t eventData);
static void DumpConnectionPermLostInfo(uint8_t event, uint32_t eventData);
static void DumpWifiInitResult(uint8_t eventType, uint32_t eventData);
static void DumpConnectionContext(void);
static void DumpScanResultsReady(uint8_t event, uint32_t eventData);



#if defined(WF_USE_SOFTAP)
static void DumpSoftApEvent(void);
static void DumpSoftApClientTable(void);
#endif

void DumpConnectionProfile(t_connectionProfile *p_profile)
{
    uint8_t profileType = p_profile->profileType;
    t_infrastructure    *p_infra;

#if defined(WF_USE_WPS)
    t_infrastructureWps *p_wps;
#endif

#if defined(WF_USE_ADHOC)
    t_adHoc             *p_adhoc;
#endif

#if defined(WF_USE_SOFTAP)
    t_softAp            *p_softAp;
#endif

    WF_PRINTF("\r\nDemo Connection Profile\r\n");
    WF_PRINTF("-----------------------\r\n");

    WF_PRINTF("  Profile Type:    ");
    switch (profileType)
    {
        case WF_INFRASTRUCTURE_PROFILE:
            WF_PRINTF("Infrastructure\r\n");
            p_infra = &p_profile->profile.infrastructure;
            OutputSsid(p_infra->ssid);
            OutputChannelList(p_infra->channelList);
            OutputSecurityType(p_infra->securityType);
            if ((p_infra->securityType == WF_SECURITY_WPA) || (p_infra->securityType == WF_SECURITY_WPA2) || (p_infra->securityType == WF_SECURITY_WPA_AUTO))
            {
                OutputPassPhrase(p_infra->security.wpaContext.passphrase);
            }
            else if ((p_infra->securityType == WF_SECURITY_WEP_40) || (p_infra->securityType == WF_SECURITY_WEP_104))
            {
                OutputWepKey(p_infra->securityType, p_infra->security.wepContext.key);
                OutputWepKeyType(p_infra->security.wepContext.keyType);
            }
            OutputReconnectMode(&p_infra->reconnectMode);

            break;

#if defined(WF_USE_WPS)
        case WF_INFRASTRUCTURE_WPS_PROFILE:
            WF_PRINTF("Infrastructure with WPS provisioning\r\n");
            p_wps = &p_profile->profile.infrastructureWps;
            OutputWpsMode(p_wps->mode, p_wps->wpsPin);
            break;
#endif

#if defined(WF_USE_SOFTAP)
        case WF_SOFTAP_PROFILE:
            WF_PRINTF("Soft AP\r\n");
            p_softAp = &p_profile->profile.softAp;
            OutputSsid(p_softAp->ssid);

            {
                uint8_t channelList[2];
                channelList[0] = p_softAp->channel;
                channelList[1] = '\0'; // terminate list
                OutputChannelList(channelList);
            }

            OutputSecurityType(p_softAp->securityType);
            if ((p_softAp->securityType == WF_SECURITY_WEP_40) || (p_softAp->securityType == WF_SECURITY_WEP_104))
            {
                OutputWepKey(p_softAp->securityType, p_softAp->wepContext.key);
                OutputWepKeyType(p_softAp->wepContext.keyType);
            }
            break;
#endif
#if defined(WF_USE_ADHOC)
        case WF_ADHOC_PROFILE:
            WF_PRINTF("AdHoc\r\n");
            p_adhoc = &p_profile->profile.adHoc;
            OutputSsid(p_adhoc->ssid);
            OutputChannelList(p_adhoc->channelList);
            OutputSecurityType(p_adhoc->securityType);
            if ((p_adhoc->securityType == WF_SECURITY_WEP_40) || (p_adhoc->securityType == WF_SECURITY_WEP_104))
            {
                OutputWepKey(p_adhoc->securityType, p_adhoc->wepContext.key);
                OutputWepKeyType(p_adhoc->wepContext.keyType);
            }

            OutputAdHocMode(p_adhoc->mode);
            OutputBeaconPeriod(p_adhoc->beaconPeriod);
            OutputHiddenSsid(p_adhoc->hiddenSsid);
            OutputReconnectMode(&p_adhoc->reconnectMode);
            break;
#endif // WF_USE_ADHOC
    }
    WF_PRINTF("\r\n");
}


void DumpEventInfo(uint8_t eventType, uint32_t eventData)
{
#if defined(WF_CONSOLE_USE_COLOR)
    // if any of the error events
    if ((eventType == WF_EVENT_ERROR) || (eventType == WF_EVENT_CONNECTION_FAILED) ||
        (eventType == WF_EVENT_CONNECTION_TEMPORARILY_LOST) || (eventType == WF_EVENT_CONNECTION_PERMANENTLY_LOST))
    {
        SetForeground(RED);
    }
    // else if init failed
    else if ((eventType == WF_EVENT_INITIALIZATION) && (eventData != WF_INIT_SUCCESSFUL))
    {
        SetForeground(RED);
    }
    // else 'good' event
    else
    {
        SetForeground(GREEN);
    }
#endif // WF_COLOR_OUTPUT

    if (eventData != WF_NO_EVENT_DATA)
    {
        WF_PRINTF("\r\n-->WI-FI EVENT: %s  eventData = 0x%04x (%d)\r\n", g_eventTypeStr[eventType], eventData, eventData);
    }
    else
    {
        WF_PRINTF("\r\n-->WI-FI EVENT: %s  eventData = None\r\n", g_eventTypeStr[eventType]);
    }
#if defined(WF_CONSOLE_USE_COLOR)
    SetForeground(BLACK);
#endif

    switch (eventType)
    {
        case WF_EVENT_INITIALIZATION:
            DumpWifiInitResult(eventType, eventData);
            break;

        case WF_EVENT_CONNECTION_SUCCESSFUL:
            DumpConnectionContext();
            break;

        case WF_EVENT_CONNECTION_FAILED:
            DumpConnectionFailedInfo(eventType, eventData);
            break;

        case WF_EVENT_CONNECTION_TEMPORARILY_LOST:
            DumpConnectionTempLostInfo(eventType, eventData);
            break;

        case WF_EVENT_CONNECTION_PERMANENTLY_LOST:
            DumpConnectionPermLostInfo(eventType, eventData);
            break;
            
        case WF_EVENT_CONNECTION_REESTABLISHED:
            DumpConnectionContext();
            break;

        case WF_EVENT_SCAN_RESULTS_READY:
            DumpScanResultsReady(eventType, eventData);
            break;

#if defined(WF_USE_SOFTAP)

        case WF_EVENT_SOFTAP_NETWORK_STARTED:
            // no additional output
            break;

        case WF_EVENT_SOFTAP_CLIENT_CONNECT:
        case WF_EVENT_SOFTAP_CLIENT_DISCONNECT:
            DumpSoftApEvent();
            DumpSoftApClientTable();
            break;
#endif
        case WF_EVENT_DISCONNECT_COMPLETE:
            break;

        case WF_EVENT_ERROR:
            // no extra data to output
            break;
    }

}

void DumpWifiInitResult(uint8_t eventType, uint32_t eventData)
{
    uint8_t mac[WF_MAC_ADDRESS_LENGTH];
    t_deviceInfo deviceInfo;
    uint8_t domain;

    eventType = eventType;      // avoid warning

    if (eventData == WF_INIT_SUCCESSFUL)
    {
        WF_MacAddressGet(mac);
        WF_DeviceInfoGet(&deviceInfo);
        WF_PRINTF("  Init Successful\r\n");
        WF_PRINTF("    MAC Address:     ");
        OutputMac(mac);
        WF_PRINTF("\r\n");
        WF_PRINTF("    MRF24WG Version: ROM=0x%02X Patch=0x%02X\r\n", deviceInfo.romVersion, deviceInfo.patchVersion);

        domain = WF_DomainGet();
        WF_PRINTF("    Domain:          ");
        if (domain == WF_DOMAIN_FCC)
        {
            WF_PRINTF("FCC\r\n");
        }
        else if (domain == WF_DOMAIN_ETSI)
        {
            WF_PRINTF("ETSI\r\n");
        }
        else if (domain == WF_DOMAIN_JAPAN)
        {
            WF_PRINTF("JAPAN\r\n");
        }
    }
    // else init failed
    else
    {
        WF_PRINTF("Init Failed: ");
        if (eventData == WF_INIT_ERROR_SPI_NOT_CONNECTED)
        {
            WF_PRINTF("WF_SPI_NOT_CONNECTED\r\n");
        }
        else if (eventData == WF_INIT_ERROR_RESET_TIMEOUT)
        {
            WF_PRINTF("WF_RESET_TIMEOUT\r\n");
        }
        else if (eventData == WF_INIT_ERROR_INIT_TIMEOUT)
        {
            WF_PRINTF("WF_INITIALIZE_TIMEOUT\r\n");
        }
        else
        {
            WF_PRINTF("Unknown init eventData\r\n");
        }
    }
}


static void DumpScanResultsReady(uint8_t event, uint32_t eventData)
{
    event = event; // avoid compiler warning
    
    WF_PRINTF("  Number of scan results: %d\r\n", eventData);
}

#if defined(WF_USE_SOFTAP)
static void DumpSoftApEvent(void)
{
    t_softApEvent eventInfo;

    WF_SoftApEventInfoGet(&eventInfo);
    WF_PRINTF("    MAC: ");
    OutputMac(eventInfo.mac);
    WF_PRINTF("\r\n");

    if (eventInfo.event == WF_SOFTAP_CLIENT_DISCONNECTED)
    {
        if (eventInfo.reason == WF_SOFTAP_CLIENT_LINK_LOST)
        {
            WF_PRINTF("    Link Lost\r\n");
        }
        else
        {
            WF_PRINTF("    Deauth Received\r\n");
        }
    }
}

static void DumpSoftApClientTable(void)
{
   t_softApClientTable table;
   int i;
   uint8_t mask = 0x01;
   bool atLeastOne = false;

   WF_SoftApClientTableGet(&table);

   WF_PRINTF("\r\n    SoftAP Client Table\r\n");
   WF_PRINTF("    -------------------\r\n");
   for (i = 0; i < WF_MAX_SOFTAP_CLIENTS; ++i)
   {
       if ((table.slotMask & mask) > 0)
       {
           WF_PRINTF("    ");
           OutputMac(table.mac[i]);
           WF_PRINTF("\r\n");
           atLeastOne = true;
       }
       mask = mask << 1;
   }

   if (!atLeastOne)
   {
       WF_PRINTF("    Empty\r\n");
   }
}
#endif // WF_USE_SOFTAP

void DumpConnectionContext(void)
{
    t_connectionContext context;

    WF_ConnectionContextGet(&context);
    WF_PRINTF("\r\nConnection Context\r\n");
    WF_PRINTF("------------------\r\n");
    WF_PRINTF("  Channel: %d\r\n", context.channel);
    WF_PRINTF("  BSSID:   %02x:%02x:%02x:%02x:%02x:%02x\r\n\r\n", context.bssid[0], context.bssid[1], context.bssid[2],
                                                           context.bssid[3], context.bssid[4], context.bssid[5]);
#if defined(WF_USE_CONSOLE)
    OutputPrompt();
#else
    WF_PRINTF("\r\n");
#endif
}

#if defined(WF_USE_WPS)
void DumpWpsCredentials(void)
{
    t_wpsCredentials cred;
    int i;
    uint8_t bitIndex;

    WF_WpsCredentialsGet(&cred);

    WF_PRINTF("\r\nWPS Credentials\r\n");
    WF_PRINTF("----------------\r\n");

    // SSID
    WF_PRINTF("  SSID:         ");
    WF_PRINTF("\"");
    for (i = 0; i < cred.ssidLen; ++i)
    {
        WF_PRINTF("%c", cred.ssid[i]);
    }
    WF_PRINTF("\"\r\n");

    // AuthType
    WF_PRINTF("  Auth Type:    ");
    bitIndex = GetBitIndex(cred.authType);
    WF_PRINTF("%s\r\n", g_authStr[bitIndex]);

    // Encode Type
    WF_PRINTF("  Encode Type:  ");
    bitIndex = GetBitIndex(cred.encType);
    WF_PRINTF("%s\r\n", g_encTypeStr[bitIndex]);

    // Net Key
    if (cred.authType != WF_AUTH_OPEN)
    {
        WF_PRINTF("  Passphrase:   \"");
        for (i = 0; i < cred.keyLen; ++i)
        {
            WF_PRINTF("%c", (char)cred.netKey[i]);
        }
        WF_PRINTF("\"\r\n");
    }

    // BSSID
    WF_PRINTF("  BSSID:        ");
    OutputMac(cred.bssid);

#if defined(WF_USE_CONSOLE)
    OutputPrompt();
#else
    WF_PRINTF("\r\n");
#endif

}
#endif // WF_USE_WPS


static void OutputSsid(char *p_ssid)
{
    WF_PRINTF("  SSID:            \"%s\"\r\n", p_ssid);
}

static void OutputChannelList(uint8_t *p_channelList)
{
    uint8_t i;
    uint8_t length = strlen((char *)p_channelList);

    WF_PRINTF("  Channel List:    ");
    for (i = 0; i < length; ++i)
    {
        if (i < (length - 1))
        {
            WF_PRINTF("%d,", p_channelList[i]);
        }
        else
        {
            WF_PRINTF("%d", p_channelList[i]);
        }
    }
    WF_PRINTF("\r\n");
}

static void OutputSecurityType(uint8_t securityType)
{
    WF_PRINTF("  Security:        ");

    switch (securityType)
    {
        case WF_SECURITY_OPEN:
            WF_PRINTF("Open");
            break;

        case WF_SECURITY_WEP_40:
            WF_PRINTF("WEP-40");
            break;

        case WF_SECURITY_WEP_104:
            WF_PRINTF("WEP-104");
            break;

        case WF_SECURITY_WPA:
            WF_PRINTF("WPA");
            break;

        case WF_SECURITY_WPA2:
            WF_PRINTF("WPA2");
            break;

        case WF_SECURITY_WPA_AUTO:
            WF_PRINTF("WPA Auto");
            break;
    }
    WF_PRINTF("\r\n");
}

static void OutputPassPhrase(char *p_passphrase)
{
    WF_PRINTF("  WPA Passphrase:  \"%s\"\r\n", p_passphrase);
}

static void OutputWepKey(uint8_t security, uint8_t *p_key)
{
    uint8_t keyLength, i;

    WF_PRINTF("  1st Wep Key:     ");
    if (security == WF_SECURITY_WEP_40)
    {
        keyLength = WF_WEP40_KEY_LENGTH / 4;
    }
    else
    {
        keyLength = WF_WEP104_KEY_LENGTH / 4;
    }

    for (i = 0; i < keyLength; ++i)
    {
        WF_PRINTF("%02x ", p_key[i]);
    }
    WF_PRINTF("\r\n");
}

static void OutputWepKeyType(uint8_t keyType)
{
    WF_PRINTF("  WEP Key Type:    ");
    if (keyType == WF_SECURITY_WEP_OPENKEY)
    {
        WF_PRINTF("Open Key\r\n");
    }
    else
    {
        WF_PRINTF("Shared Key\r\n");
    }
}

static void OutputReconnectMode(t_reconnectMode *p_reconnectMode)
{
    WF_PRINTF("  Reconnect Settings:  \r\n");
    if (p_reconnectMode->deauthAction == WF_ATTEMPT_TO_RECONNECT)
    {
        WF_PRINTF("   Deauth Action:         Attempt to reconnect\r\n");
    }
    else
    {
        WF_PRINTF("   Deauth Action:         Do not Attempt to reconnect\r\n");
    }


    WF_PRINTF("   Beacon Timeout:        %d beacon periods\r\n", p_reconnectMode->beaconTimeout);

    if (p_reconnectMode->deauthAction == WF_ATTEMPT_TO_RECONNECT)
    {
        WF_PRINTF("   Beacon Timeout Action: Attempt to reconnect\r\n");
    }
    else
    {
        WF_PRINTF("   Beacon Timeout Action: Do not attempt to reconnect\r\n");
    }

    if (p_reconnectMode->retryCount < WF_RETRY_FOREVER)
    {
        WF_PRINTF("   Retry Count:           %d\r\n", p_reconnectMode->retryCount);
    }
    else
    {
        WF_PRINTF("   Retry Count:           Retry forever\r\n");
    }
}

#if defined(WF_USE_ADHOC)
static void OutputBeaconPeriod(uint16_t beaconPeriod)
{
    WF_PRINTF("  Beacon Period:   %d\r\n", beaconPeriod);
}

static void OutputHiddenSsid(bool hiddenSsid)
{
    WF_PRINTF("  Hidden SSID:     ");
    if (hiddenSsid)
    {
        WF_PRINTF("True\r\n");
    }
    else
    {
        WF_PRINTF("False\r\n");
    }
}

static void OutputAdHocMode(uint8_t mode)
{
    if (mode == WF_ADHOC_CONNECT_THEN_START)
    {
        WF_PRINTF("  AdHoc Mode:      Try to connect first, then start network\r\n");
    }
    else if (mode == WF_ADHOC_CONNECT_ONLY)
    {
        WF_PRINTF("  AdHoc Mode:      Connect only\r\n");
    }
    else
    {
        WF_PRINTF("  AdHoc Mode:      Start network only\r\n");
    }
}
#endif // WF_USE_ADHOC

#if defined(WF_USE_WPS)
static void OutputWpsMode(uint8_t wpsMode, uint32_t wpsPin)
{
    if (wpsMode == WF_SECURITY_WPS_PIN)
    {
        WF_PRINTF("  WPS mode:        Pin\r\n");
        WF_PRINTF("  Pin:             %8d\r\n", wpsPin);
    }
    else
    {
        WF_PRINTF("  WPS mode:        Push-Button\r\n");
    }

    WF_PRINTF("  Channel List:    All channels in domain\r\n");
    WF_PRINTF("  SSID:            Determined during WPS handshake\r\n");
    WF_PRINTF("  Security:        Determined during WPS handshake\r\n");
}
#endif // WF_USE_WPS

static void DumpConnectionTempLostInfo(uint8_t event, uint32_t eventData)
{
    if (eventData & DEAUTH_REASONCODE_MASK)
    {
        WF_PRINTF("  Deauth Occurred (%s)\r\n", g_deauthDisssocReasonStr[eventData & ~DEAUTH_REASONCODE_MASK]);
    }
    else if (eventData & DISASSOC_REASONCODE_MASK)
    {
        WF_PRINTF("  Disassoc Occurred (%s)\r\n", g_deauthDisssocReasonStr[eventData & ~DISASSOC_REASONCODE_MASK]);
    }
    else
    {
        WF_PRINTF("  %s\r\n", g_connectionLostStr[eventData]);
    }
}

static void DumpConnectionPermLostInfo(uint8_t event, uint32_t eventData)
{
    if (eventData & DEAUTH_REASONCODE_MASK)
    {
        WF_PRINTF("  Deauth Occurred (%s)\r\n", g_deauthDisssocReasonStr[eventData & ~DEAUTH_REASONCODE_MASK]);
    }
    else if (eventData & DISASSOC_REASONCODE_MASK)
    {
        WF_PRINTF("  Disassoc Occurred (%s)\r\n", g_deauthDisssocReasonStr[eventData & ~DISASSOC_REASONCODE_MASK]);
    }
    else
    {
        WF_PRINTF("  %s\r\n", g_connectionLostStr[eventData]);
    }
}

static void DumpConnectionFailedInfo(uint8_t event, uint32_t eventData)
{
    uint8_t status, reason;

    status = (uint8_t)(eventData >> 8);
    reason = (uint8_t)(eventData & 0xff);

    if ((status == CF_RECV_DEAUTH) || (status == CF_RECV_DISASSOC))
    {
        WF_PRINTF("  Status = %s,  Reason = %s\r\n", g_connectionFailureStr[status], g_deauthDisssocReasonStr[reason] );
    }
    else if ((status == CF_AUTHENTICATION_FAILURE) || (status == CF_ASSOCIATION_FAILURE))
    {
        WF_PRINTF("  Status = %s,  Reason = %s\r\n", g_connectionFailureStr[status], g_statusCodeStr[reason] );
    }
    else if (status == CF_WPS_FAILURE)
    {
        WF_PRINTF("  Status = %s,  WPS State = %s,  WPS Config Error = %s\r\n", g_connectionFailureStr[status],
                                                                    g_wpsState[reason >> 4],
                                                                    g_wpsConfigErr[reason & 0x0f]);
    }
    else
    {
        WF_PRINTF("  Status = %s\r\n", g_connectionFailureStr[status]);
    }
}

static void OutputMac(uint8_t *p_mac)
{
    WF_PRINTF("%02x:%02x:%02x:%02x:%02x:%02x", p_mac[0], p_mac[1], p_mac[2],
                                            p_mac[3], p_mac[4], p_mac[5]);
}

#if defined(WF_USE_WPS)
// gets the index of the single, set bit
static uint8_t GetBitIndex(uint16_t value)
{
    int i;
    uint16_t mask = 0x01;

    for (i = 0; i < 16; ++i)
    {
        if ((value & mask) > 0)
        {
            return i;
        }
        mask = mask << 1;
    }

    WF_PRINTF("GetBitIndex: Should never get here\r\n");
    return 0;

}
#endif // WF_USE_WPS


void OutputTxQueueFailMsg(void)
{
#if defined(WF_VT100)
    SetForeground(RED);
#endif
    WF_PRINTF("WF_QueueTxPacket failed; try increasing WF_TX_QUEUE_SIZE\r\n");
#if defined(WF_VT100)
    SetForeground(BLACK);
#endif

}


#endif // WF_USE_DEBUG_OUTPUT

