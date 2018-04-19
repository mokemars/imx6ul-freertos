/*******************************************************************************
 Universal Driver Console Cmds

  Summary: This module processes console cmds

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
//==============================================================================
//                                  INCLUDES
//==============================================================================
#include <stdio.h>
#include <string.h>

#include <ctype.h>

#include "wf_universal_driver.h"
#include "wf_global_includes.h"
#include "wf_console_cmds.h"
#include "wf_console.h"
#include "wf_demos.h"

#if defined(WF_USE_CONSOLE)

//==============================================================================
//                                  DEFINES
//==============================================================================
// Cmd List (numbers MUST MATCH indexes in g_cmds
enum
{
    CLS_CMD        = 0,
    CONNECT_CMD    = 1,
    CP_CMD         = 2,
    DISCONNECT_CMD = 3,
    DUMP_CMD       = 4,
    HELP_CMD       = 5,
    RESET_CMD      = 6,
    SCAN_CMD       = 7,
    UDP_CLIENT_CMD = 8,
    UDP_SERVER_CMD = 9,
    TCP_CLIENT_CMD = 10,
};

// dump values (numbers MUST MATCH indexes in g_dumpValues
enum
{
    CP_DUMP_PARAM         = 0,
    MAC_DUMP_PARAM        = 1,
    HOST_STATS_DUMP_PARAM = 2,
    MAC_STATS_DUMP_PARAM  = 3
};

// value must match index in g_cpValues[]
enum
{
    CP_TYPE       = 0,        // "type"
    CP_SSID       = 1,        // "ssid"
    CP_CHANNEL    = 2,        // "channel"
    CP_SECURITY   = 3,        // "security"
    CP_PASSPHRASE = 4         // "passphrase"
};

// value must match index in g_typeValues[]
enum
{
    CP_INFRA        = 0,
    CP_WPS_PUSH     = 1,
    CP_WPS_PIN      = 2,
    CP_SOFTAP       = 3,
    CP_ADHOC        = 4
};

// value must match index in g_securityValues[]
enum
{
    CP_OPEN      = 0,
    CP_WPA       = 1,
    CP_WPA2      = 2,
    CP_WPA_AUTO  = 3
};

// bits in apConfig field of scan result
#define WPA2_MASK       0x10
#define WPA_MASK        0x40
#define PRIVACY_MASK    0x10

//==============================================================================
//                                  LOCAL DATATYPES
//==============================================================================
typedef struct
{
    bool    displayScanResults;
    uint8_t scanIndex;
    uint8_t numScanResults;
} t_scanDisplayContext;

typedef struct
{
    uint32_t  txPackets;
    uint32_t  rxPackets;
    uint16_t  connectionSuccessful;
    uint16_t  connectionFailed;
    uint16_t  connectionTempLost;
    uint16_t  connectionPermLost;
    uint16_t  connectionRestab;

} t_hostStats;

//==============================================================================
//                                  LOCAL GLOBALS
//==============================================================================
char    *g_p_token;
uint8_t g_numCmds;
t_scanDisplayContext g_scanDisplayContext;
t_hostStats g_hostStats;


// command list -- indexes MUST MATCH cmd constants above.  Commands must not be
//                 longer than 12 characters. List must end with NULL entry.
const char *g_cmds[] = {"cls",              // 0
                        "connect",          // 1
                        "cp",               // 2
                        "disconnect",       // 3
                        "dump",             // 4
                        "help",             // 5
                        "reset",            // 6
                        "scan",             // 7
                        "udpclient",        // 8
                        "udpserver",        // 9
                        "tcpclient",        // 10

                        NULL
};

const char *g_cmdHelp[] = {"Clear the screen",
                           "Start Wi-Fi connection",
                           "Change values in connection profile",
                           "Force Wi-Fi disconnect",
                           "Dump value(s) [cp] [mac] [hoststats]",
                           "Display help",
                           "Reset Host MCU",
                           "Start a Wi-Fi scan",
                           "Start/stop UDP Client test",
                           "Start/stop UDP Server Test",
                           "Start/stop TCP Client Test",
                           NULL
};

// dump value param list
const char *g_dumpValues[] = {"cp",         // 0
                              "mac",        // 1
                              "hoststats",  // 2
                              NULL};

const char *g_cpValues[] = {"type",
                            "ssid",
                            "channel",
                            "security",
                            "passphrase",
                             NULL};

const char *g_typeValues[] = {"infra", "wpspushbutton", "wpspin", "softap", "adhoc", NULL};
const char *g_securityValues[] = {"open", "wpa", "wpa2", "wpaauto", NULL};

const char g_channelErrorString[] = {"Invalid channel list, should be: cp chan 1,6,11 or cp chan 1\n"};

//==============================================================================
//                                  LOCAL FUNCTIONS
//==============================================================================
static char * GetFirstToken(char *p_string);
static char * GetNextToken(void);
static int  GetStringFromList(char *p_searchString, const char *p_list[]);
static void ConnectCmd(void);
static void CpCmd(void);
static void DisconnectCmd(void);
static void DumpCmd(void);
static void HelpCmd(void);
static void ScanCmd(void);
static void SetCpType(char *p_type, t_connectionProfile *p_profile);
static void SetCpSsid(char *p_ssid, t_connectionProfile *p_profile);
static void SetCpChannel(char *p_channel, t_connectionProfile *p_profile);
static void SetCpPassphrase(char *p_passphrase, t_connectionProfile *p_profile);
static void SetCpSecurity(char *p_security, t_connectionProfile *p_profile);
static void DumpHostStats(void);
#if 0
static void DumpMacStats(void);
#endif
static void UdpClientCmd(void);
static void UdpServerCmd(void);
static void TcpClientCmd(void);


extern void SoftwareReset(void);

void InitConsoleCmd(void)
{
    // go through list of commands until reach end of list so we can get the
    // total number of commands
    g_numCmds = 0;
    while (g_cmds[g_numCmds] != NULL)
    {
        ++g_numCmds;
    }

    // not currently displaying any scan results
    g_scanDisplayContext.displayScanResults = false;
}

void IncrementRxPackets(void)
{
    ++g_hostStats.rxPackets;
}

void IncrementTxPackets(void)
{
    ++g_hostStats.txPackets;
}

void IncrementConnectionSuccessful(void)
{
    ++g_hostStats.connectionSuccessful;
}

void IncrementConnectionFailed(void)
{
    ++g_hostStats.connectionFailed;
}

void IncrementConnectionTempLost(void)
{
    ++g_hostStats.connectionTempLost;
}

void IncrementConnectionPermLost(void)
{
    ++g_hostStats.connectionPermLost;
}

void IncrementConnectionRestab(void)
{
    ++g_hostStats.connectionRestab;
}



void ProcessConsoleCmd(char *p_cmd)
{
    char *p_token;
    int  cmdIndex;

    p_token = GetFirstToken(p_cmd);

    cmdIndex = GetStringFromList(p_token, g_cmds);

    switch (cmdIndex)
    {
        case CLS_CMD:
            ClearScreen();
            break;

        case CONNECT_CMD:
            ConnectCmd();
            break;

        case CP_CMD:
            CpCmd();
            break;

        case DISCONNECT_CMD:
            DisconnectCmd();
            break;

        case DUMP_CMD:
            DumpCmd();
            break;

        case HELP_CMD:
            HelpCmd();
            break;

        case RESET_CMD:
            SoftwareReset();
            break;

        case SCAN_CMD:
            ScanCmd();
            break;

        case UDP_CLIENT_CMD:
            UdpClientCmd();
            break;

        case UDP_SERVER_CMD:
            UdpServerCmd();
            break;

        case TCP_CLIENT_CMD:
            TcpClientCmd();
            break;

        default:
            WF_PRINTF("Unknown command\r\n");
            break;
    }
}


static void ConnectCmd(void)
{
    if (UdGetConnectionState() == CS_NOT_CONNECTED)
    {
        WF_Connect();
    }
    else
    {
        WF_PRINTF("Already connected\r\n");
    }
}

static void CpCmd(void)
{
    char *p_value;
    int index;
    t_connectionProfile *p_profile = GetConnectionProfile();

    // get value string
    p_value = GetNextToken();

    if (p_value == NULL)
    {
        WF_PRINTF(" Values for cp command:\r\n");
        WF_PRINTF("   type       - infra, wpspushbutton, wpspin, softap, or adhoc\r\n");
        WF_PRINTF("   ssid       - ssid string in quotes (e.g. \"myssid\")\r\n");
        WF_PRINTF("   channel    - single channel or comma-separated channel list (e.g. 1,6,11)\r\n");
        WF_PRINTF("   security   - open, wpa, wpa2, wpaauto\r\n");
        WF_PRINTF("   passphrase - wpa passphrase in quotes or wps key(e.g. \"passphrase\" or 92255326)\r\n");
        return;
    }

    index = GetStringFromList(p_value, g_cpValues);

    switch (index)
    {
        case CP_TYPE:
            p_value = GetNextToken();  // get third token
            SetCpType(p_value, p_profile);
            break;

        case CP_SSID:
            p_value = GetNextToken();  // get third token
            SetCpSsid(p_value, p_profile);
            break;

        case CP_CHANNEL:
            p_value = GetNextToken();
            SetCpChannel(p_value, p_profile);
            break;

        case CP_SECURITY:
            p_value = GetNextToken();
            SetCpSecurity(p_value, p_profile);
            break;

        case CP_PASSPHRASE:
            p_value = GetNextToken();
            SetCpPassphrase(p_value, p_profile);
            break;

        default:
            WF_PRINTF("unknown value for cp command\r\n");
            break;
    }

}

static void SetCpType(char *p_type, t_connectionProfile *p_profile)
{
    int index;
    uint8_t   oldType = p_profile->profileType;  // get current profile type
#if defined(WF_USE_WPS)
    uint8_t   oldMode = 0;                       // avoid warning
#endif 
    if (UdGetConnectionState() == CS_CONNECTED)
    {
        WF_PRINTF("Must disconnect before changing profile type");
        return;
    }

#if defined(WF_USE_WPS)
    if (oldType == WF_INFRASTRUCTURE_WPS_PROFILE)
    {
        oldMode = p_profile->profile.infrastructureWps.mode;
    }
#endif // WF_USE_WPS

    if (p_type == NULL)
    {
        WF_PRINTF("Missing type (e.g. cp type infra)");
        return;
    }


    index = GetStringFromList(p_type, g_typeValues);
    switch (index)
    {
        case CP_INFRA:
            p_profile->profileType = WF_INFRASTRUCTURE_PROFILE;
            // if user is changing connection profile type
            if (oldType != WF_INFRASTRUCTURE_PROFILE)
            {
                // put reasonable default values in profile
                int i;
                for (i = 1; i <= 11; ++i)
                {
                    p_profile->profile.infrastructure.channelList[i - 1] = i;
                }
                p_profile->profile.infrastructure.channelList[11] = 0;
                p_profile->profile.infrastructure.securityType = WF_SECURITY_OPEN;
                strcpy(p_profile->profile.infrastructure.ssid, "ssid");
                p_profile->profile.infrastructure.reconnectMode.beaconTimeout = WF_DEFAULT_BEACON_TIMEOUT;
                p_profile->profile.infrastructure.reconnectMode.retryCount = WF_RETRY_FOREVER;
                p_profile->profile.infrastructure.reconnectMode.beaconTimeoutAction = WF_ATTEMPT_TO_RECONNECT;
                p_profile->profile.infrastructure.reconnectMode.deauthAction = WF_ATTEMPT_TO_RECONNECT;
            }
            break;

#if defined(WF_USE_WPS)
        case CP_WPS_PIN:
            p_profile->profileType = WF_INFRASTRUCTURE_WPS_PROFILE;
            p_profile->profile.infrastructureWps.mode = WF_SECURITY_WPS_PIN;
            if ((oldType != WF_INFRASTRUCTURE_WPS_PROFILE) || (oldMode != WF_SECURITY_WPS_PIN))
            {
                // put reasonable default values in profile
                p_profile->profile.infrastructureWps.handshakeTimeout = 20;
                strcpy(p_profile->profile.infrastructureWps.ssid, "ssid");
                p_profile->profile.infrastructureWps.wpsPin = 92255326;
                p_profile->profile.infrastructureWps.reconnectMode.beaconTimeout = WF_DEFAULT_BEACON_TIMEOUT;
                p_profile->profile.infrastructureWps.reconnectMode.retryCount = WF_RETRY_FOREVER;
                p_profile->profile.infrastructureWps.reconnectMode.beaconTimeoutAction = WF_ATTEMPT_TO_RECONNECT;
                p_profile->profile.infrastructureWps.reconnectMode.deauthAction = WF_ATTEMPT_TO_RECONNECT;
            }
            break;

        case CP_WPS_PUSH:
            p_profile->profileType = WF_INFRASTRUCTURE_WPS_PROFILE;
            p_profile->profile.infrastructureWps.mode = WF_SECURITY_WPS_PUSH_BUTTON;
            if ((oldType != WF_INFRASTRUCTURE_WPS_PROFILE) || (oldMode != WF_SECURITY_WPS_PUSH_BUTTON))
            {
                // put reasonable default values in profile
                p_profile->profile.infrastructureWps.handshakeTimeout = 20;
                strcpy(p_profile->profile.infrastructureWps.ssid, "");
                p_profile->profile.infrastructureWps.reconnectMode.beaconTimeout = WF_DEFAULT_BEACON_TIMEOUT;
                p_profile->profile.infrastructureWps.reconnectMode.retryCount = WF_RETRY_FOREVER;
                p_profile->profile.infrastructureWps.reconnectMode.beaconTimeoutAction = WF_ATTEMPT_TO_RECONNECT;
                p_profile->profile.infrastructureWps.reconnectMode.deauthAction = WF_ATTEMPT_TO_RECONNECT;
            }
            break;
#endif // WF_USE_WPS

#if defined(WF_USE_SOFTAP)
        case CP_SOFTAP:
            p_profile->profileType = WF_SOFTAP_PROFILE;
            if (oldType != WF_SOFTAP_PROFILE)
            {
                // put reasonable default values in profile
                p_profile->profile.softAp.channel = 1;
                p_profile->profile.softAp.securityType = WF_SECURITY_OPEN;
                strcpy(p_profile->profile.softAp.ssid, "ssid");
            }
            break;
#endif // WF_USE_SOFTAP

#if defined(WF_USE_ADHOC)
        case CP_ADHOC:
            p_profile->profileType = WF_ADHOC_PROFILE;
            if (oldType != WF_ADHOC_PROFILE)
            {
                // put reasonable default values in profile
                strcpy(p_profile->profile.adHoc.ssid, "ssid");
                p_profile->profile.adHoc.hiddenSsid = false;
                p_profile->profile.adHoc.channelList[0] = 1;
                p_profile->profile.adHoc.channelList[1] = 0;
                p_profile->profile.adHoc.securityType = WF_SECURITY_OPEN;
                p_profile->profile.adHoc.beaconPeriod = WF_DEFAULT_ADHOC_BEACON_PERIOD;
                p_profile->profile.adHoc.mode = WF_ADHOC_CONNECT_THEN_START;
                p_profile->profile.adHoc.reconnectMode.beaconTimeout = WF_DEFAULT_BEACON_TIMEOUT;
                p_profile->profile.adHoc.reconnectMode.retryCount = 3;
                p_profile->profile.adHoc.reconnectMode.beaconTimeoutAction = WF_ATTEMPT_TO_RECONNECT;
                p_profile->profile.adHoc.reconnectMode.deauthAction = WF_ATTEMPT_TO_RECONNECT;
            }
            break;
#endif // WF_USE_ADHOC
            
        default:
            WF_PRINTF("Unknown type\r\n");
            break;
    }
}


void SetCpSsid(char *p_ssid, t_connectionProfile *p_profile)
{
    if (p_ssid == NULL)
    {
        WF_PRINTF("Missing SSID (e.g. cp ssid \"myssid\")\r\n");
        return;
    }

    // if quotes are present (needed to get entire ssid as single token if spaces are in ssid).  First
    // quote stripped off below.
    if ((p_ssid[0] == '\"') && (p_ssid[strlen(p_ssid) - 1]) == '\"')
    {
        // strip off end quote
        p_ssid[strlen(p_ssid) - 1] = '\0';

        if (strlen(&p_ssid[1]) <= WF_MAX_SSID_LENGTH)
        {
            if (p_profile->profileType == WF_INFRASTRUCTURE_PROFILE)
            {
                strcpy(p_profile->profile.infrastructure.ssid, &p_ssid[1]);
            }
#if defined(WF_USE_WPS)
            else if (p_profile->profileType == WF_INFRASTRUCTURE_WPS_PROFILE)
            {
                strcpy(p_profile->profile.infrastructureWps.ssid, &p_ssid[1]);
            }
#endif // WF_USE_WPS
#if defined(WF_USE_SOFTAP)
            else if (p_profile->profileType == WF_SOFTAP_PROFILE)
            {
                strcpy(p_profile->profile.softAp.ssid, &p_ssid[1]);
            }
#endif // WF_USE_SOFTAP
#if defined(WF_USE_ADHOC)
            else if (p_profile->profileType == WF_ADHOC_PROFILE)
            {
                strcpy(p_profile->profile.adHoc.ssid, &p_ssid[1]);
            }
#endif // WF_USE_ADHOC
        }
        else
        {
            WF_PRINTF("SSID must be 32 or less characters\r\n");
        }
    }
    // else quotes are not present
    else
    {
        WF_PRINTF("SSID must be in quotes (e.g. \"myssid\")\r\n");
    }
}

static void SetCpChannel(char *p_channel, t_connectionProfile *p_profile)
{
    int     i;
    int     len;
    uint8_t channelList[WF_MAX_CHANNEL_LIST_LENGTH + 1];
    int     channelNumber;
    int     chanIndex = 0;
    char    *p_token;
    char    chanString[40];  // need bigger string so can add comma after last channel

    memset(channelList, 0x00, sizeof(channelList));

    // if no channel list
    if (p_channel == NULL)
    {
        WF_PRINTF("%s\r\n", g_channelErrorString);
        return;
    }

    // string should have nothing but ascii digits and commas
    for (i = 0; i < strlen(p_channel); ++i)
    {
        if ( (!isdigit(p_channel[i])) && (p_channel[i] != ',') )
        {
            WF_PRINTF("%s\r\n", g_channelErrorString);
            return;
        }
    }

    // if string does not start with ascii digit
    if (!isdigit(p_channel[0]))
    {
        WF_PRINTF("%s\r\n", g_channelErrorString);
        return;
    }

    // copy channel string into local buffer and add comma at the end
    strcpy(chanString, p_channel);
    len = strlen(chanString);
    chanString[len] = ',';      // add comma
    chanString[len+1] = '\0';   // terminate string

    // get first channel
    p_token = strtok(chanString, ",");

    while(1)
    {
        if (strlen(p_token) <=2)
        {
            sscanf(p_token, "%d", &channelNumber);
            if (channelList[chanIndex] > 14)
            {
                WF_PRINTF("%d is not a valid channel number\r\n");
                return;
            }

            channelList[chanIndex++] = (uint8_t)channelNumber;

            // get next channel string
            p_token = strtok(NULL, ",");
            if (p_token == NULL)
            {
                if (p_profile->profileType == WF_INFRASTRUCTURE_PROFILE)
                {
                    memcpy(p_profile->profile.infrastructure.channelList, channelList, sizeof(channelList));
                }
                else if (p_profile->profileType == WF_INFRASTRUCTURE_WPS_PROFILE)
                {
                    WF_PRINTF("WPS will automatically use all channels in domain\r\n");
                }
#if defined(WF_USE_SOFTAP)
                else if (p_profile->profileType == WF_SOFTAP_PROFILE)
                {
                    if (chanIndex == 1)
                    {
                        p_profile->profile.softAp.channel = channelList[0];
                    }
                    else
                    {
                        WF_PRINTF("SoftAP can only have one channel\r\n");
                    }
                }
#endif // WF_USE_SOFTAP
#if defined(WF_USE_ADHOC)
                else if (p_profile->profileType == WF_ADHOC_PROFILE)
                {
                    memcpy(p_profile->profile.adHoc.channelList, channelList, sizeof(channelList));
                }
#endif // WF_USE_ADHOC

                return;  // end of list
            }
        }
        else
        {
            WF_PRINTF("Invalid channel number\r\n");
            return;
        }
    }
}

 static void SetCpSecurity(char *p_security, t_connectionProfile *p_profile)
{
    uint8_t index;

    index = GetStringFromList(p_security, g_securityValues);

    switch (index)
    {
        case CP_OPEN:
            if (p_profile->profileType == WF_INFRASTRUCTURE_PROFILE)
            {
                p_profile->profile.infrastructure.securityType = WF_SECURITY_OPEN;
            }
            else if (p_profile->profileType == WF_INFRASTRUCTURE_WPS_PROFILE)
            {
                WF_PRINTF("For WPS profile, security will be automatically detected and set\r\n");
            }
#if defined(WF_USE_SOFTAP)
            else if (p_profile->profileType == WF_SOFTAP_PROFILE)
            {
                p_profile->profile.softAp.securityType = WF_SECURITY_OPEN;
            }
#endif // WF_USE_SOFTAP
#if defined(WF_USE_ADHOC)
            else if (p_profile->profileType == WF_ADHOC_PROFILE)
            {
                p_profile->profile.adHoc.securityType = WF_SECURITY_OPEN;
            }
#endif // WF_USE_ADHOC
            break;

        case CP_WPA:
            if (p_profile->profileType == WF_INFRASTRUCTURE_PROFILE)
            {
                p_profile->profile.infrastructure.securityType = WF_SECURITY_WPA;
            }
            else if (p_profile->profileType == WF_INFRASTRUCTURE_WPS_PROFILE)
            {
                WF_PRINTF("For WPS profile, security will be automatically detected and set\r\n");
            }
            else if (p_profile->profileType == WF_SOFTAP_PROFILE)
            {
                WF_PRINTF("WPA not supported for SoftAP\r\n");
            }
            else if (p_profile->profileType == WF_ADHOC_PROFILE)
            {
                WF_PRINTF("WPA not supported for AdHoc\r\n");
            }
            break;

        case CP_WPA2:
            if (p_profile->profileType == WF_INFRASTRUCTURE_PROFILE)
            {
                p_profile->profile.infrastructure.securityType = WF_SECURITY_WPA2;
            }
            else if (p_profile->profileType == WF_INFRASTRUCTURE_WPS_PROFILE)
            {
                WF_PRINTF("For WPS profile, security will be automatically detected and set\r\n");
            }
            else if (p_profile->profileType == WF_SOFTAP_PROFILE)
            {
                WF_PRINTF("WPA2 not supported for SoftAP\r\n");
            }
            else if (p_profile->profileType == WF_ADHOC_PROFILE)
            {
                WF_PRINTF("WPA2 not supported for AdHoc\r\n");
            }
            break;

        case CP_WPA_AUTO:
            if (p_profile->profileType == WF_INFRASTRUCTURE_PROFILE)
            {
                p_profile->profile.infrastructure.securityType = WF_SECURITY_WPA_AUTO;
            }
            else if (p_profile->profileType == WF_INFRASTRUCTURE_WPS_PROFILE)
            {
                WF_PRINTF("For WPS profile, security will be automatically detected and set\r\n");
            }
            else if (p_profile->profileType == WF_SOFTAP_PROFILE)
            {
                WF_PRINTF("WPA Auto not supported for SoftAP\r\n");
            }
            else if (p_profile->profileType == WF_ADHOC_PROFILE)
            {
                WF_PRINTF("WPA Auto not supported for AdHoc\r\n");
            }
            break;
        default:
            WF_PRINTF("Unknown security type\r\n");
            break;

    }
}

static void SetCpPassphrase(char *p_passphrase, t_connectionProfile *p_profile)
{
    if (p_passphrase == NULL)
    {
        WF_PRINTF("Missing passphrase or pin\r\n");
        return;
    }

    if (p_profile->profileType == WF_INFRASTRUCTURE_PROFILE)
    {
        // if quotes are present (needed to get entire ssid as single token if spaces are in ssid).  First
        // quote stripped off below.
        if ((p_passphrase[0] == '\"') && (p_passphrase[strlen(p_passphrase) - 1]) == '\"')
        {
            // strip off end quote
            p_passphrase[strlen(p_passphrase) - 1] = '\0';

            if (strlen(&p_passphrase[1]) <= WF_MAX_PASSPHRASE_LENGTH)
            {
                strcpy(p_profile->profile.infrastructure.security.wpaContext.passphrase, &p_passphrase[1]);
            }
            else
            {
                WF_PRINTF("Passphrase must be %d characters or less\r\n", WF_MAX_PASSPHRASE_LENGTH);
            }
        }
        else
        {
             WF_PRINTF("Passphrase must be in quotes (e.g. \"my passphrase\")\r\n");
        }
    }
#if defined(WF_USE_WPS)
    else if (p_profile->profileType == WF_INFRASTRUCTURE_WPS_PROFILE)
    {
        if (p_profile->profile.infrastructureWps.mode == WF_SECURITY_WPS_PIN)
        {
            if (strlen(p_passphrase) == WF_WPS_PIN_LENGTH)
            {
                int i;
                for (i = 0; i < WF_WPS_PIN_LENGTH; ++i)
                {
                    if (!isdigit(p_passphrase[i]))
                    {
                        WF_PRINTF("WPS Pin must be all digits\r\n");
                        return;
                    }
                }

                sscanf(p_passphrase, "%d", &p_profile->profile.infrastructureWps.wpsPin);
            }
            else
            {
                WF_PRINTF("WPS Pin must be 8 digits in length\r\n");
            }
        }
        else
        {
            WF_PRINTF("WPS Pin not needed for WPS Pushbutton mode\r\n");
        }
    }
#endif // WF_USE_WPS
#if defined(WF_USE_SOFTAP)
    else if (p_profile->profileType == WF_SOFTAP_PROFILE)
    {
        WF_PRINTF("SoftAp does not support WPA\r\n");
    }
#endif // WF_USE_SOFTAP
#if defined(WF_USE_ADHOC)
    else if (p_profile->profileType == WF_ADHOC_PROFILE)
    {
        WF_PRINTF("AdHoc does not support WPA\r\n");
    }
#endif // WF_USE_ADHOC
}

static void DisconnectCmd(void)
{
   if ((UdGetConnectionState() == CS_CONNECTED) || (isSoftApProfile()))
   {
       WF_Disconnect();
   }
   else
   {
       WF_PRINTF("Not currently connected\r\n");
   }
}

static void DumpCmd(void)
{
    char *p_value;
    int  dumpIndex;
    uint8_t macAddress[WF_MAC_ADDRESS_LENGTH];

    // get dump value string
    p_value = GetNextToken();

    if (p_value == NULL)
    {
        WF_PRINTF(" Values for dump command:\r\n");
        WF_PRINTF("   cp        - dump connnection profile\r\n");
        WF_PRINTF("   mac       - dump mac address\r\n");
        WF_PRINTF("   hoststats - dump host stats\r\n");
#if 0
        WF_PRINTF("   macstats  - dump MAC stats\r\n");
#endif
        return;
    }

    // get the value to be dumped
    dumpIndex = GetStringFromList(p_value, g_dumpValues);

    switch (dumpIndex)
    {
        case CP_DUMP_PARAM:
            DumpConnectionProfile(GetConnectionProfile());
            break;

        case MAC_DUMP_PARAM:
            GetMacAddress(macAddress);
            WF_PRINTF("MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\r\n", macAddress[0], macAddress[1], macAddress[2],
                                                                   macAddress[3], macAddress[4], macAddress[5]);
            break;

        case HOST_STATS_DUMP_PARAM:
            DumpHostStats();
            break;
#if 0
        case MAC_STATS_DUMP_PARAM:
            DumpMacStats();
            break;
#endif
        default:
            WF_PRINTF("Unknown value for dump command\r\n");
            break;
    }

}


static void HelpCmd(void)
{
    int i, j;
    int length;
    int spacer;

    for (i = 0; i < g_numCmds; ++i)
    {
        length = strlen(g_cmds[i]);
        spacer = 13 - length;

        WF_PRINTF("%s", g_cmds[i]);
        for (j = 0; j < spacer; ++j)
        {
            WF_PRINTF(" ");
        }
        WF_PRINTF("- %s\r\n", g_cmdHelp[i]);
    }
}

static void ScanCmd(void)
{
    uint16_t res;
    res = isScanAllowed();
    if (res == UD_SUCCESS)
    {
        WF_Scan();
    }
    else
    {
        WF_PRINTF("Scan not allowed, see t_udEventErrors error code %d", res);
    }
}

static void UdpClientCmd(void)
{
    char *p_value;

    // get action string
    p_value = GetNextToken();

    if (p_value == NULL)
    {
        WF_PRINTF(" Values for udpclient command:\r\n");
        WF_PRINTF("   start - start UDP client demo\r\n");
        WF_PRINTF("   stop  - stop UDP client demo\r\n");
        return;
    }

    if (strcmp(p_value, "start") == 0)
    {
        if (isUdpClientActive())
        {
            WF_PRINTF("udp client is already started\r\n");
        }
        else
        {
            UdpClientStart();
        }
    }
    else if (strcmp(p_value, "stop") == 0)
    {
         if (!isUdpClientActive())
         {
             WF_PRINTF("udp client is already stopped\r\n");

         }
         else
         {
             UdpClientStop();
         }
    }
    else
    {
        WF_PRINTF("Invalid command -- only start and stop are allowed\r\n");
    }
}

static void UdpServerCmd(void)
{
    char *p_value;

    // get action string
    p_value = GetNextToken();

    if (p_value == NULL)
    {
        WF_PRINTF(" Values for udpserver command:\r\n");
        WF_PRINTF("   start - start UDP server demo\r\n");
        WF_PRINTF("   stop  - stop UDP server demo\r\n");
        return;
    }

    if (strcmp(p_value, "start") == 0)
    {
        if (isUdpServerActive())
        {
            WF_PRINTF("udp server is already started\r\n");
        }
        else
        {
            UdpServerStart();
        }
    }
    else if (strcmp(p_value, "stop") == 0)
    {
         if (!isUdpServerActive())
         {
             WF_PRINTF("udp server is already stopped\r\n");

         }
         else
         {
             UdpServerStop();
         }
    }
    else
    {
        WF_PRINTF("Invalid command -- only start and stop are allowed\r\n");
    }
}

static void TcpClientCmd(void)
{
    char *p_value;

    // get action string
    p_value = GetNextToken();

    if (p_value == NULL)
    {
        WF_PRINTF(" Values for tcpclient command:\r\n");
        WF_PRINTF("   start - start TCP Client demo\r\n");
        WF_PRINTF("   stop  - stop TCP Client demo\r\n");
        return;
    }

    if (strcmp(p_value, "start") == 0)
    {
        if (isTcpClientActive())
        {
            WF_PRINTF("tcp client is already started\r\n");
        }
        else
        {
            TcpClientStart();
        }
    }
    else if (strcmp(p_value, "stop") == 0)
    {
         if (!isTcpClientActive())
         {
             WF_PRINTF("tcp client is already stopped\r\n");

         }
         else
         {
             TcpClientStop();
         }
    }
    else
    {
        WF_PRINTF("Invalid command -- only start and stop are allowed\r\n");
    }
}

// called by WFProcessMgmtIndicateMsg() when scan results are ready
void EnableScanResultDisplayStateMachine(uint8_t numResults)
{
    g_scanDisplayContext.scanIndex = 0;
    g_scanDisplayContext.displayScanResults = true;
    g_scanDisplayContext.numScanResults = numResults;
}

// called by DisplayScanResults() after last scan result has been displayed
void DisableScanResultDisplayStateMachine(void)
{
    g_scanDisplayContext.displayScanResults = false;
}

// called by ConsoleStateMachine() to check if scan results are currently being displayed
bool isScanResultsDisplayEnabled(void)
{
    return g_scanDisplayContext.displayScanResults;
}

// called by ConsoleStateMachine() to display the next scan result
void DisplayScanResults(void)
{
    t_scanResult result;
    char buf[40];
    int i;
    uint8_t rate;

    // if this is the first scan result
    if (g_scanDisplayContext.scanIndex == 0)
    {
        WF_PRINTF("\r\nSSID                              Chan  RSSI  Sec    Rates (MHz)\r\n");
        WF_PRINTF("--------------------------------  ----  ----  ---    --------------\r\n");
    }

    // get next sca result
    WF_ScanResultGet(g_scanDisplayContext.scanIndex, &result);

    // if SSID is not hidden
    if (isprint(result.ssid[0]))
    {
        // output SSID
        memcpy(buf, result.ssid, result.ssidLen);
        buf[result.ssidLen] = '\0';
        WF_PRINTF("%s", buf);
    }
    // else hidden SSID
    else
    {
        WF_PRINTF(" ");
        result.ssidLen = 1;
    }

    // output pad spaces after SSID
    for (i = 0; i < (35 - result.ssidLen); ++i)
    {
        buf[i] = ' ';
    }
    buf[i] = '\0';
    WF_PRINTF("%s", buf);

    // output channel and pad spaces
    WF_PRINTF("%2d   ", result.channel);

    // output RSSI
    WF_PRINTF("%3d   ", result.rssi);

    // output security
    if (result.apConfig & PRIVACY_MASK)
    {
        if ((result.apConfig & WPA_MASK) && (result.apConfig & WPA2_MASK))
        {
            WF_PRINTF("WPA/2  ");
        }
        else if (result.apConfig & WPA_MASK)
        {
            WF_PRINTF("WPA    ");
        }
        else if (result.apConfig & WPA2_MASK)
        {
            WF_PRINTF("WPA2   ");
        }
        else
        {
            WF_PRINTF("WEP    ");
        }
    }
    else
    {
        WF_PRINTF("Open   ");
    }

    // output rates
    for (i = 0; i < result.numRates; ++i)
    {
        rate = result.basicRateSet[i] & 0x7f;

        WF_PRINTF("%d", rate / 2);

        if ((rate % 2) > 0)
        {
            WF_PRINTF(".5");
        }

        if (i < (result.numRates - 1))
        {
            WF_PRINTF(",");
        }
    }

    // terminate line
    WF_PRINTF("\r\n");

    // if done with scan results than let console state machine that scan result output complete.
    ++g_scanDisplayContext.scanIndex;
    if (g_scanDisplayContext.scanIndex >= g_scanDisplayContext.numScanResults)
    {
        DisableScanResultDisplayStateMachine();
        OutputPrompt();
    }


}

static void DumpHostStats(void)
{
    WF_PRINTF("\r\nHost Stats\r\n");
    WF_PRINTF("----------\r\n");
    WF_PRINTF("  Tx Packets:         %d\r\n", g_hostStats.txPackets);
    WF_PRINTF("  Rx Packets:         %d\r\n", g_hostStats.rxPackets);
    WF_PRINTF("  Connections:\r\n");
    WF_PRINTF("    Successful:       %d\r\n", g_hostStats.connectionSuccessful);
    WF_PRINTF("    Failed            %d\r\n", g_hostStats.connectionFailed);
    WF_PRINTF("    Perm. Lost        %d\r\n", g_hostStats.connectionPermLost);
    WF_PRINTF("    Temp. Lost        %d\r\n", g_hostStats.connectionTempLost);
    WF_PRINTF("    Reestablished:    %d\r\n", g_hostStats.connectionRestab);
}

#if 0
static void DumpMacStats(void)
{
    t_macStats stats;

    WF_MacStatsGet(&stats);

    WF_PRINTF("\r\nMAC Stats\r\n");
    WF_PRINTF("---------\r\n");
    WF_PRINTF("  Tx frames:\r\n");
    WF_PRINTF("    MibTxBytesCtr:     %d\r\n", stats.MibTxBytesCtr);
    WF_PRINTF("    MibTxSuccessCtr:   %d\r\n", stats.MibTxSuccessCtr);
    WF_PRINTF("    MibTxRtryCtr:      %d\r\n", stats.MibTxRtryCtr);
    WF_PRINTF("    MibTxMultRtryCtr:  %d\r\n", stats.MibTxMultRtryCtr);
    WF_PRINTF("    MibTxFailedCtr:    %d\r\n", stats.MibTxFailedCtr);
    WF_PRINTF("    MibRxAckFailCtr:   %d\r\n", stats.MibRxAckFailCtr);
    WF_PRINTF("  Rx frames:\r\n");
    WF_PRINTF("    MibRxBytesCtr:     %d\r\n", stats.MibRxBytesCtr);
    WF_PRINTF("    MibRxFragCtr:      %d\r\n", stats.MibRxFragCtr);
    WF_PRINTF("    MibRxCtsSuccCtr:   %d\r\n", stats.MibRxCtsSuccCtr);
    WF_PRINTF("    MibRxCtsFailCtr:   %d\r\n", stats.MibRxCtsFailCtr);
    WF_PRINTF("    MibRxDupCtr:       %d\r\n", stats.MibRxDupCtr);
    WF_PRINTF("    MibRxMultCtr:      %d\r\n", stats.MibRxMultCtr);
    WF_PRINTF("    MibRxFCSErrCtr:    %d\r\n", stats.MibRxFCSErrCtr);
}
#endif

// searches thru an array of strings looking for a match to the input string
static int GetStringFromList(char *p_searchString, const char *p_list[])
{
    int i = 0;

    // while not at end of list
    while (p_list[i] != NULL)
    {
        if (strcmp(p_searchString, p_list[i]) == 0)
        {
            return i;
        }
        
        ++i;
    }

    return 0xffff;  // no match found

}

// gets first token from command string
static char * GetFirstToken(char *p_string)
{
    g_p_token = strtok(p_string, " ");
    return (g_p_token);
}

// gets the next token in the command string.  The first token must already be
// extracted.  Returns NULL if no more tokens.
static char * GetNextToken(void)
{
    g_p_token = strtok(NULL, " ");
    return g_p_token;
}


#endif // WF_USE_CONSOLE

