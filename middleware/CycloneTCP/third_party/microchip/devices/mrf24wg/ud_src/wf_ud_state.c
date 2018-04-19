/*******************************************************************************
 MRF24WG error checking

  Summary: Functions performing run-time error checking

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
#include <stdio.h>
#include <string.h>

#include "wf_universal_driver.h"
#include "wf_global_includes.h"

//==============================================================================
//                                  DEFINES
//==============================================================================
#define MAX_RTS_THRESHOLD   ((uint16_t)2347)


//==============================================================================
//                                  LOCAL GLOBALS
//==============================================================================
t_udState g_udState;

//==============================================================================
//                                  LOCAL FUNCTION PROTOTPYES
//==============================================================================
#if defined(WF_USE_ERROR_CHECKING)
static uint32_t ValidateInfrastructureProfile(t_connectionProfile *p_connection);

#if defined(WF_USE_WPS)
static uint32_t ValidateInfrastructureWpsProfile(t_connectionProfile *p_connection);
static uint32_t ValidateWpsPin(uint32_t wpsPin);
#endif // WF_USE_WPS

#if defined(WF_USE_SOFTAP)
static uint32_t ValidateSoftApProfile(t_connectionProfile *p_connection);
#endif

#if defined(WF_USE_ADHOC)
static uint32_t ValidateAdHocProfile(t_connectionProfile *p_connection);
#endif

static uint32_t ValidateSsid(char *p_ssid);
static uint32_t ValidateInfraSecurityType(uint8_t securityType);
static uint32_t ValidateWepSecurity(t_wepContext *p_wep);
static uint32_t ValidateWpaSecurity(t_wpaContext *p_wpa);
static uint32_t ValidateChannelList(uint8_t *p_channelList);
static uint32_t ValidateReconnectMode(t_reconnectMode *p_reconnect);


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#endif /* WF_USE_ERROR_CHECKING */




//==============================================================================
void UdStateInit(void)
{

    memset(&g_udState, 0x00, sizeof(g_udState));

    UdSetConnectionState(CS_NOT_CONNECTED);
    UdDisablePsPoll();
}

//==============================================================================
void  UdEnablePsPoll(t_psPollContext *p_context)
{
    g_udState.psPollEnabled = true;
    memcpy(&g_udState.psPollContext, p_context, sizeof(t_psPollContext));
}

//==============================================================================
void UdDisablePsPoll(void)
{
    g_udState.psPollEnabled = false;
}
//==============================================================================
INLINE bool  UdisPsPollEnabled(void)
{
    return g_udState.psPollEnabled;
}

//==============================================================================
//==============================================================================
//
// All functions below this point are only used if WF_USE_ERROR_CHECKING is defined
//
//==============================================================================
//==============================================================================

#if defined(WF_USE_ERROR_CHECKING)

//==============================================================================
uint32_t UdValidateProfile(t_connectionProfile *p_profile)
{
    uint32_t errorCode;

    if (p_profile == NULL)
    {
        return UD_ERROR_CONNECTION_NULL;
    }

    if (p_profile->profileType > WF_MAX_PROFILE)
    {
        return UD_ERROR_INVALID_PROFILE;
    }

    if (p_profile->profileType == WF_INFRASTRUCTURE_PROFILE)
    {
        errorCode = ValidateInfrastructureProfile(p_profile);
    }
#if defined(WF_USE_WPS)
    else if (p_profile->profileType == WF_INFRASTRUCTURE_WPS_PROFILE)
    {
        errorCode = ValidateInfrastructureWpsProfile(p_profile);
    }
#endif // WF_USE_WPS
#if defined(WF_USE_SOFTAP)
    else if (p_profile->profileType == WF_SOFTAP_PROFILE)
    {
        errorCode = ValidateSoftApProfile(p_profile);
    }
#endif // WF_USE_SOFTAP
#if defined(WF_USE_ADHOC)
    else if (p_profile->profileType == WF_ADHOC_PROFILE)
    {
        errorCode = ValidateAdHocProfile(p_profile);
    }
#endif // WF_USE_ADHOC
    else
    {
        // unknown profile type
        errorCode = UD_ERROR_INVALID_PROFILE;
    }

    return errorCode;
}

//==============================================================================
static uint32_t ValidateInfrastructureProfile(t_connectionProfile *p_connection)
{
    uint32_t errorCode = UD_SUCCESS;
    uint8_t securityType;

    t_infrastructure *p_infra = &p_connection->profile.infrastructure;

    securityType = p_infra->securityType;  // shortcut

    errorCode = ValidateSsid(p_infra->ssid);
    if (errorCode != UD_SUCCESS)
    {
        return errorCode;
    }

    errorCode = ValidateInfraSecurityType(securityType);
    if (errorCode != UD_SUCCESS)
    {
        return errorCode;
    }

    // if WEP security
    if ((securityType == WF_SECURITY_WEP_40) || (securityType == WF_SECURITY_WEP_104))
    {
        errorCode = ValidateWepSecurity(&p_infra->security.wepContext);
        if (errorCode != UD_SUCCESS)
        {
            return errorCode;
        }
    }
    // else if WPA security
    else if ((securityType == WF_SECURITY_WPA) || (securityType == WF_SECURITY_WPA2) || (securityType == WF_SECURITY_WPA_AUTO))
    {
        errorCode = ValidateWpaSecurity(&p_infra->security.wpaContext);
        if (errorCode != UD_SUCCESS)
        {
            return errorCode;
        }
    }

    errorCode = ValidateChannelList(p_infra->channelList);
    if (errorCode != UD_SUCCESS)
    {
        return errorCode;
    }

    errorCode = ValidateReconnectMode(&p_infra->reconnectMode);
    if (errorCode != UD_SUCCESS)
    {
        return errorCode;
    }


    return UD_SUCCESS;

}

#if defined(WF_USE_WPS)
//==============================================================================
static uint32_t ValidateInfrastructureWpsProfile(t_connectionProfile *p_connection)
{
    uint32_t errorCode = UD_SUCCESS;
    t_infrastructureWps *p_wps = &p_connection->profile.infrastructureWps;

    if ((p_wps->mode != WF_SECURITY_WPS_PIN) && (p_wps->mode != WF_SECURITY_WPS_PUSH_BUTTON))
    {
        return UD_ERROR_INVALID_WPS_MODE;
    }

    errorCode = ValidateReconnectMode(&p_wps->reconnectMode);
    if (errorCode != UD_SUCCESS)
    {
        return errorCode;
    }

    if (p_connection->profile.infrastructureWps.mode == WF_SECURITY_WPS_PIN)
    {
        errorCode = ValidateWpsPin(p_wps->wpsPin);
        if (errorCode != UD_SUCCESS)
        {
            return errorCode;
        }

        errorCode = ValidateSsid(p_wps->ssid);
        if (errorCode != UD_SUCCESS)
        {
            return errorCode;
        }
    }

    return UD_SUCCESS;
}
#endif // WF_USE_WPS

#if defined(WF_USE_SOFTAP)
//==============================================================================
static uint32_t ValidateSoftApProfile(t_connectionProfile *p_connection)
{
    uint32_t errorCode;
    t_softAp *p_softAp = &p_connection->profile.softAp;     // shortcut pointer
    uint8_t  channelList[2];
    uint8_t  securityType = p_softAp->securityType;         // shortcut

    // validate SSID
    errorCode = ValidateSsid(p_softAp->ssid);
    if (errorCode != UD_SUCCESS)
    {
        return errorCode;
    }

    // validate channel
    channelList[0] = p_softAp->channel;
    channelList[1] = '\0';  // terminate channel list
    errorCode = ValidateChannelList(channelList);
    if (errorCode != UD_SUCCESS)
    {
        return errorCode;
    }

    // validate security type
    if ((securityType != WF_SECURITY_OPEN) && (securityType != WF_SECURITY_WEP_40) && (securityType != WF_SECURITY_WEP_104))
    {
        return UD_ERROR_INVALID_SOFTAP_SECURITY;
    }

    // if WEP security then validate
    if ((securityType == WF_SECURITY_WEP_40) || (securityType == WF_SECURITY_WEP_104))
    {
        errorCode = ValidateWepSecurity(&p_softAp->wepContext);
        if (errorCode != UD_SUCCESS)
        {
            return errorCode;
        }
    }

    return UD_SUCCESS;
}
#endif // WF_USE_SOFTAP

#if defined(WF_USE_ADHOC)
//==============================================================================
static uint32_t ValidateAdHocProfile(t_connectionProfile *p_connection)
{
    uint32_t errorCode = UD_SUCCESS;
    uint8_t securityType;

    t_adHoc *p_adhoc = &p_connection->profile.adHoc;

    securityType = p_adhoc->securityType;  // shortcut

    errorCode = ValidateSsid(p_adhoc->ssid);
    if (errorCode != UD_SUCCESS)
    {
        return errorCode;
    }

    if ((p_adhoc->hiddenSsid != true) && (p_adhoc->hiddenSsid != false))
    {
        return UD_ERROR_INVALID_HIDDEN_SSID;
    }

    if (p_adhoc->mode > WF_ADHOC_START_ONLY)
    {
        return UD_ERROR_INVALID_ADHOC_MODE;
    }

    errorCode = ValidateReconnectMode(&p_adhoc->reconnectMode);
    if (errorCode != UD_SUCCESS)
    {
        return errorCode;
    }

    if ((p_adhoc->mode == WF_ADHOC_CONNECT_THEN_START) && (p_adhoc->reconnectMode.retryCount == WF_RETRY_FOREVER))
    {
        return UD_ERROR_INVALID_ADHOC_RETRY_COUNT;
    }

    // if WEP security
    if ((securityType == WF_SECURITY_WEP_40) || (securityType == WF_SECURITY_WEP_104))
    {
        errorCode = ValidateWepSecurity(&p_adhoc->wepContext);
        if (errorCode != UD_SUCCESS)
        {
            return errorCode;
        }
    }

    return UD_SUCCESS;
}
#endif // WF_USE_ADHOC

//============================================================================
static uint32_t ValidateSsid(char *p_ssid)
{
    uint8_t  ssidLength = strlen(p_ssid);

    if (ssidLength > WF_MAX_SSID_LENGTH)
    {
        return UD_ERROR_SSID_TOO_LONG;
    }

    // if SSID length is 0
    if (strlen(p_ssid) == 0)
    {
        return UD_ERROR_SSID_NOT_DEFINED;
    }

    return UD_SUCCESS;
}

//============================================================================
static uint32_t ValidateInfraSecurityType(uint8_t securityType)
{
    if ((securityType == WF_SECURITY_OPEN)    || (securityType == WF_SECURITY_WEP_40) ||
        (securityType == WF_SECURITY_WEP_104) || (securityType == WF_SECURITY_WPA)    ||
        (securityType == WF_SECURITY_WPA2)    || (securityType == WF_SECURITY_WPA_AUTO))
    {
        return UD_SUCCESS;
    }
    else
    {
        return UD_ERROR_INVALID_INFRA_SECURITY_TYPE;
    }
}

//============================================================================
static uint32_t ValidateWepSecurity(t_wepContext *p_wep)
{
    if ((p_wep->keyType == WF_SECURITY_WEP_OPENKEY) || (p_wep->keyType == WF_SECURITY_WEP_SHAREDKEY))
    {
        return UD_SUCCESS;
    }
    else
    {
        return UD_ERROR_INVALID_WEP_KEY_TYPE;
    }
}

//============================================================================
static uint32_t ValidateWpaSecurity(t_wpaContext *p_wpa)
{
    uint8_t length = strlen(p_wpa->passphrase);
    uint8_t i;
    char    character;

    length = strlen(p_wpa->passphrase);
    if ((length < WF_MIN_PASSPHRASE_LENGTH) || (length > WF_MAX_PASSPHRASE_LENGTH))
    {
        return UD_ERROR_INVALID_WPA_PASSPHRASE_LENGTH;
    }

    // ASCII passphase characters must be printable (0x20 thru 0x7E)
    for (i = 0; i < length; ++i)
    {
        character = p_wpa->passphrase[i];
        if ((character < 0x20) || (character > 0x7e))
        {
            return UD_ERROR_INVALID_WPA_PASSPHRASE_CHARS;
        }
    }

    return UD_SUCCESS;
}

//============================================================================
static uint32_t ValidateChannelList(uint8_t *p_channelList)
{
    uint8_t index;
    uint8_t channel;
    uint16_t channelBitMask = 0x0000;
    uint8_t domain;
    
    domain = WF_DomainGet();

    // if channel list array is NULL
    if (p_channelList == NULL)
    {
        return UD_ERROR_NULL_CHANNEL_LIST;
    }

    index = 0;
    while (1)
    {
        // if list too long
        if (index > (WF_MAX_CHANNEL_LIST_LENGTH - 1))
        {
            return UD_ERROR_CHANNEL_LIST_TOO_LONG;
        }

        channel = p_channelList[index];
        
        // if end of channel list
        if (channel == 0)
        {
            break;
        }

        if ((domain == WF_DOMAIN_FCC) && (channel > 11))
        {
            return UD_ERROR_CHANNEL_NOT_IN_DOMAIN;
        }
        else if ((domain == WF_DOMAIN_ETSI) && (channel > 13))
        {
            return UD_ERROR_CHANNEL_NOT_IN_DOMAIN;
        }
        else if ((domain == WF_DOMAIN_JAPAN) && (channel > 14))
        {
            return UD_ERROR_CHANNEL_NOT_IN_DOMAIN;
        }

        // if this channel was already in the list
        if ( (channelBitMask & (0x01 << channel)) > 0)
        {
            return UD_ERROR_CHANNEL_DUPLICATE;
        }

        // set bit in mask
        channelBitMask |= (0x01 << channel);

        ++index;
    }
   
    // if no channels defined
    if (channelBitMask == 0)
    {
        return UD_ERROR_NO_CHANNELS_DEFINED;
    }

    return UD_SUCCESS;
}

//==============================================================================
static uint32_t ValidateReconnectMode(t_reconnectMode *p_reconnect)
{
    if ((p_reconnect->deauthAction != WF_ATTEMPT_TO_RECONNECT) && (p_reconnect->deauthAction != WF_DO_NOT_ATTEMPT_TO_RECONNECT))
    {
        return UD_ERROR_INVALID_DEAUTH_PARAM;
    }

    if ((p_reconnect->beaconTimeoutAction != WF_ATTEMPT_TO_RECONNECT) && (p_reconnect->beaconTimeoutAction != WF_DO_NOT_ATTEMPT_TO_RECONNECT))
    {
        return UD_ERROR_INVALID_BEACON_TIMEOUT_PARAM;
    }

    // if no automatic retries
    if (p_reconnect->retryCount == 0)
    {
        if ((p_reconnect->deauthAction == WF_ATTEMPT_TO_RECONNECT) || (p_reconnect->beaconTimeoutAction == WF_ATTEMPT_TO_RECONNECT))
        {
            return UD_ERROR_INVALID_RECONNECT_MODE;
        }
    }

    return UD_SUCCESS;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!





//==============================================================================
uint32_t UdSetScanContext(t_scanContext *p_context)
{
    uint32_t errorCode = UD_SUCCESS;

    // can't change scan context unless not connected
    if (UdGetConnectionState() != CS_NOT_CONNECTED)
    {
        errorCode = UD_ERROR_ONLY_VALID_WHEN_NOT_CONNECTED;
    }

    if ((p_context->scanType != WF_ACTIVE_SCAN) && (p_context->scanType != WF_PASSIVE_SCAN))
    {
        errorCode = UD_ERROR_INVALID_SCAN_TYPE;
    }

    return errorCode;
}


//==============================================================================
uint32_t UdSetBssid(uint8_t *p_bssid)
{
    p_bssid = p_bssid; // avoid warning

    // can't change BSSID unless not connected
    if (UdGetConnectionState() != CS_NOT_CONNECTED)
    {
        return UD_ERROR_ONLY_VALID_WHEN_NOT_CONNECTED;
    }
    else
    {
        return UD_SUCCESS;
    }
}

//==============================================================================
uint32_t UdSetRssi(uint8_t rssi)
{
    rssi = rssi; // avoid warning

    // can't change RSSI unless not connected
    if (UdGetConnectionState() != CS_NOT_CONNECTED)
    {
        return UD_ERROR_ONLY_VALID_WHEN_NOT_CONNECTED;
    }
    else
    {
        return UD_SUCCESS;
    }
}

//==============================================================================
uint32_t UdSetRtsThreshold(uint16_t rtsThreshold)
{
    // can't change RSSI unless not connected
    if (UdGetConnectionState() != CS_NOT_CONNECTED)
    {
        return UD_ERROR_ONLY_VALID_WHEN_NOT_CONNECTED;
    }

    if (rtsThreshold > MAX_RTS_THRESHOLD)
    {
        return UD_ERROR_INVALID_RTS_THRESHOLD;
    }

    return UD_SUCCESS;
}



//==============================================================================
uint32_t UdSetTxMode(uint8_t mode)
{
    // can't change SSID unless not connected
    if (UdGetConnectionState() != CS_NOT_CONNECTED)
    {
        return UD_ERROR_ONLY_VALID_WHEN_NOT_CONNECTED;
    }

    if (mode > WF_TXMODE_LEGACY_RATES)
    {
        return UD_ERROR_INVALID_TX_MODE;
    }
    else
    {
        return UD_SUCCESS;
    }
}

//==============================================================================
uint32_t UdScan(void)
{
    uint16_t res;

   res = isScanAllowed();
   return res;;
}

uint32_t udSetTxPowerMax(uint8_t maxTxPower, uint8_t factoryMaxTxPower)
{
    if ((maxTxPower < 9) || (maxTxPower > factoryMaxTxPower))
    {
        return UD_ERROR_INVALID_MAX_POWER;
    }
    else
    {
        return UD_SUCCESS;
    }
}

uint32_t UdSetMulticastFilter(t_multicastConfig *p_config)
{
    uint32_t errorCode = UD_SUCCESS;

    if ((p_config->filterId < WF_MULTICAST_FILTER_1) || (p_config->filterId > WF_MULTICAST_FILTER_16))
    {
        errorCode = UD_ERROR_INVALID_MULTICAST_FILTER_ID;
        goto errorExit;
    }

errorExit:
    return errorCode;
}

#if defined(WF_USE_WPS)
static uint32_t ValidateWpsPin(uint32_t wpsPin)
{
    uint32_t pin = 0;
    uint32_t accum = 0;

   

    // calculate checksum
    accum += 3 * ((pin / 10000000) % 10);
    accum += 1 * ((pin / 1000000) % 10);
    accum += 3 * ((pin / 100000) % 10);
    accum += 1 * ((pin / 10000) % 10);
    accum += 3 * ((pin / 1000) % 10);
    accum += 1 * ((pin / 100) % 10);
    accum += 3 * ((pin / 10) % 10);
    accum += 1 * ((pin / 1) % 10);

    // if checksum is valid
    if ((accum % 10) == 0)
    {
        return UD_SUCCESS;
    }
    else
    {
        return UD_ERROR_INVALID_WPS_PIN;
    }
}
#endif // WF_USE_WPS

#endif /* WF_USE_ERROR_CHECKING */
