/*******************************************************************************
 MRF24WG WiFi connection state machine

  Summary: Contains functions pertaining MRF24WG connection process.

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


#define WPS_WAIT_TIME (20000)

// general connection state machine
enum
{
    CONNECTION_START_STATE          = 0,
    CONNECTION_INFRASTRUCTURE       = 1,
    CONNECTION_INFRASTRUCTURE_WPS   = 2,
    CONNECTION_SOFT_AP              = 3,
    CONNECTION_ADHOC                = 4,
    CONNECTION_SM_INACTIVE = 255    // connection state machine not running
};

// states for infrastructure state machine
enum
{
    INFRA_START_STATE                   = 0,
    INFRA_NETWORK_TYPE_STATE            = 1,
    INFRA_CHANNEL_LIST_STATE            = 2,
    INFRA_RETRY_COUNT_STATE             = 3,
    INFRA_BEACON_TIMEOUT_STATE          = 4,
    INFRA_DEAUTH_ACTION_STATE           = 5,
    INFRA_BEACON_TIMEOUT_ACTION_STATE   = 6,
    INFRA_SECURITY_STATE                = 7,
    INFRA_CALCULATE_WPA_KEY             = 8,
    INFRA_START_CONNECT_STATE           = 9,

    INFRA_STATE_MACHINE_SUCCESS         = 10,
    INFRA_STATE_MACHINE_FAILED          = 11,
};

// states for infrastructure/WPS state machine
enum
{
    INFRA_WPS_START_STATE                   = 0,
    INFRA_WPS_NETWORK_TYPE_STATE            = 1,
    INFRA_WPS_CHANNEL_LIST_STATE            = 2,
    INFRA_WPS_RETRY_COUNT_STATE             = 3,
    INFRA_WPS_BEACON_TIMEOUT_STATE          = 4,
    INFRA_WPS_DEAUTH_ACTION_STATE           = 5,
    INFRA_WPS_BEACON_TIMEOUT_ACTION_STATE   = 6,
    INFRA_WPS_SECURITY_STATE                = 7,
    INFRA_WPS_YIELD_PASSPHRASE_STATE        = 8,
    INFRA_WPS_START_CONNECT_STATE           = 9,
    INFRA_WPS_WAIT_FOR_CONNECT_OR_PASSPHRASE_STATE = 10,
    INFRA_WPS_CALCULATE_WPA_KEY             = 11,
    INFRA_WPS_GET_WPS_CREDENTIALS           = 12,

    INFRA_WPS_STATE_MACHINE_SUCCESS         = 13,
    INFRA_WPS_STATE_MACHINE_FAILED          = 14,
};

#if defined(WF_USE_ADHOC)
// states for AdHoc state machine
enum
{
    ADHOC_START_STATE                 = 0,
    ADHOC_SET_HIDDEN_SSID_STATE       = 1,
    ADHOC_NETWORK_TYPE_STATE          = 2,
    ADHOC_CHANNEL_LIST_STATE          = 3,
    ADHOC_RETRY_COUNT_STATE           = 4,
    ADHOC_DEAUTH_ACTION_STATE         = 5,
    ADHOC_BEACON_TIMEOUT_STATE        = 6,
    ADHOC_BEACON_TIMEOUT_ACTION_STATE = 7,
    ADHOC_SECURITY_STATE              = 8,
    ADHOC_BEACON_PERIOD_STATE         = 9,
    ADHOC_MODE_STATE                  = 10,
    ADHOC_START_CONNECT_STATE         = 11,

    ADHOC_STATE_MACHINE_SUCCESS       = 12,
    ADHOC_STATE_MACHINE_FAILED        = 13
};
#endif // WF_USE_ADHOC

#if defined(WF_USE_SOFTAP)
// states for SoftAP state machine
enum
{
    SOFTAP_START_STATE                = 0,
    SOFTAP_NETWORK_TYPE_STATE         = 1,
    SOFTAP_CHANNEL_STATE              = 2,
    SOFTAP_SECURITY_STATE             = 3,
    SOFTAP_START_NETWORK_STATE        = 4,
    
    SOFTAP_STATE_MACHINE_SUCCESS      = 5,
    SOFTAP_STATE_MACHINE_FAILURE      = 6
};
#endif

enum
{
    WF_MIN_NETWORK_TYPE            = 1,
    WF_NETWORK_TYPE_INFRASTRUCTURE = 1,
    WF_NETWORK_TYPE_ADHOC          = 2,
    WF_NETWORK_TYPE_SOFT_AP        = 4,
    WF_MAX_NETWORK_TYPE            = 4
} t_networkType;

//==============================================================================
//                                  LOCAL DATA TYPES
//==============================================================================

//==============================================================================
//                                  LOCAL MACROS
//==============================================================================
#define SetConnectionState(state)   g_connectionState = state
#define GetConnectionState()        g_connectionState
#define SetSubState(state)          g_subState = state
#define GetSubState()               g_subState


//==============================================================================
//                                  LOCAL DATA Types
//==============================================================================


//==============================================================================
//                                  LOCAL GLOBALS
//==============================================================================
static t_connectionProfile g_connectionProfile;
static uint8_t             g_connectionState = CONNECTION_SM_INACTIVE;
static uint8_t             g_subState;   // state of whatever state machine is being called by general state machine
static t_infrastructure    *g_p_infrastructure;
static uint8_t             g_broadcastBssid[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

#if defined(WF_USE_ADHOC)
static t_adHoc             *g_p_adhoc;
#endif

#if defined(WF_USE_SOFTAP)
static t_softAp            *g_p_softAp;
static t_softApClientTable g_softApClientTable;
#endif

#if defined(WF_USE_WPS)
// WPS variables
static t_infrastructureWps *g_p_infraWps;
static bool                g_wpsConnectionInProgress;
static uint8_t             g_wpsEvent;               // WPS_WAITING_FOR_EVENT, WPS_PASSPHRASE_EVENT or WPS_CONNECT_EVENT
static t_wpaKeyInfo        g_wpaKeyInfo;
static bool                g_wpsCredentialsValid;
#endif // WF_USE_WPS



#if defined(WF_USE_WPS)
t_wpaKeyInfo *  GetKeyInfoPtr(void)
{
    return &g_wpaKeyInfo;
}
#endif // WF_USE_WPS

//==============================================================================
//                                  LOCAL FUNCTIONS
//==============================================================================
static uint8_t InfrastructureStateMachine(void);
#if defined(WF_USE_WPS)
static uint8_t InfrastructureWpsStateMachine(void);
#endif
#if defined(WF_USE_ADHOC)
static uint8_t AdHocStateMachine(void);
#endif
#if defined(WF_USE_WPS)
static void GetDomainChannelList(uint8_t domain, uint8_t *p_channelList);
#endif
#if defined(WF_USE_SOFTAP)
static uint8_t SoftApStateMachine(void);
static uint8_t SoftApFindEmptySlot(void);
void WF_SoftApEventInfoGet(t_softApEvent *p_softApEvent);
#endif



// resets the general connection state machine
void    ResetConnectionSm(void)
{
    SetConnectionState(CONNECTION_START_STATE);
}

// save connection profile locally
void WF_ConnectionProfileSet(t_connectionProfile *p_connectionProfile)
{
#if defined(WF_USE_ERROR_CHECKING)
  uint32_t errorCode = UdValidateProfile(p_connectionProfile);
  if (errorCode != UD_SUCCESS)
  {
      EventEnqueue(WF_EVENT_ERROR, errorCode);
      return;
  }
#endif

    memcpy(&g_connectionProfile, p_connectionProfile, sizeof(t_connectionProfile));
}

// copies connection profile to caller's structure
void WF_ConnectionProfileGet(t_connectionProfile *p_connectionProfile)
{
    memcpy(p_connectionProfile, &g_connectionProfile, sizeof(t_connectionProfile));
}

// internal to driver; gets pointer to current connection profile
t_connectionProfile * GetConnectionProfile(void)
{
    return &g_connectionProfile;
}

#if defined(WF_USE_SOFTAP)
// called by event handler when there is a successful connection to distinguish
// between a real connection and the SoftAP network starting
bool isSoftApProfile(void)
{
    return (g_connectionProfile.profileType == WF_SOFTAP_PROFILE);
}
#else
bool isSoftApProfile(void)
{
    return false;
}
#endif // WF_USE_SOFTAP

#if defined(WF_USE_ADHOC)
bool isAdHocProfile(void)
{
    return (g_connectionProfile.profileType == WF_ADHOC_PROFILE);
}
#else
bool isAdHocProfile(void)
{
    return false;
}

#endif


#if defined(WF_USE_WPS)
bool isWpsConnection(void)
{
    return g_wpsConnectionInProgress;
}

void SetWpsConnectionEvent(uint8_t event)
{
    g_wpsEvent = event;
}

bool isWpsCredentialsValid(void)
{
    return g_wpsCredentialsValid;
}
#endif // WF_USE_WPS

// general connection state machine
uint8_t ConnectionSm(void)
{
    uint8_t res = SM_IN_PROGRESS;

    switch ( GetConnectionState() )
    {
        //----------------------------
        case CONNECTION_START_STATE:
        //----------------------------
#if defined(WF_USE_WPS)
            g_wpsConnectionInProgress = false;
            g_wpsCredentialsValid     = false;
#endif

            // put default bssid back to broadcast because after WPS the bssid is not restored back to broadcast
            WF_BssidSet(g_broadcastBssid);

#if defined(WF_USE_DEBUG_OUTPUT)
            WF_PRINTF("Wi-Fi connection state machine started\r\n");
#endif
            // if the app is connecting in infrastructure mode
            if (g_connectionProfile.profileType == WF_INFRASTRUCTURE_PROFILE)
            {
                // prepare the infrastructure state machine to run
                SetSubState(INFRA_START_STATE);
                g_p_infrastructure = &g_connectionProfile.profile.infrastructure;
                SetConnectionState(CONNECTION_INFRASTRUCTURE);
            }
#if defined(WF_USE_WPS)
            // else if app is connecting in infrastructure mode, but with WPS provisioning
            else if (g_connectionProfile.profileType == WF_INFRASTRUCTURE_WPS_PROFILE)
            {
                SetSubState(INFRA_WPS_START_STATE);
                g_p_infraWps = &g_connectionProfile.profile.infrastructureWps;
                g_wpsConnectionInProgress = true;
                SetConnectionState(CONNECTION_INFRASTRUCTURE_WPS);
            }
#endif // WF_USE_WPS
#if defined(WF_USE_SOFTAP)
            // else if app is connecting in SoftAP mode
            else if (g_connectionProfile.profileType == WF_SOFTAP_PROFILE)
            {
                SetSubState(SOFTAP_START_STATE);
                g_p_softAp = &g_connectionProfile.profile.softAp;
                SetConnectionState(CONNECTION_SOFT_AP);
            }
#endif // WF_USE_SOFT_AP
#if defined(WF_USE_ADHOC)
            // else if app is connecting in AdHoc mode
            else if (g_connectionProfile.profileType == WF_ADHOC_PROFILE)
            {
                SetSubState(ADHOC_START_STATE);
                g_p_adhoc = &g_connectionProfile.profile.adHoc;
                SetConnectionState(CONNECTION_ADHOC);
            }
#endif // WF_USE_ADHOC
            else
            {
                // unknown profile type
                EventEnqueue(WF_EVENT_ERROR, UD_ERROR_CONNECTION_INTERNAL_1);
            }
            break;

        //----------------------------
        case CONNECTION_INFRASTRUCTURE:
        //----------------------------
            res = InfrastructureStateMachine();
            break;

#if defined(WF_USE_WPS)
        //----------------------------
        case CONNECTION_INFRASTRUCTURE_WPS:
        //----------------------------
            res = InfrastructureWpsStateMachine();
            break;
#endif

#if defined(WF_USE_SOFTAP)
        case CONNECTION_SOFT_AP:
            res = SoftApStateMachine();
            break;
#endif

#if defined(WF_USE_ADHOC)
        //----------------------------
        case CONNECTION_ADHOC:
        //----------------------------
            res = AdHocStateMachine();
            break;
#endif // WF_USE_ADHOC
    } // end switch

    return res;
}

//------------------------------------------------
// sub-state machine for infrastructure network
//------------------------------------------------
static uint8_t InfrastructureStateMachine(void)
{
    uint8_t  res = SM_IN_PROGRESS;
    uint8_t  wpaCalcRes;
   
    switch (GetSubState() )
    {
        case INFRA_START_STATE:
            SsidSet(g_p_infrastructure->ssid);
            SetSubState(INFRA_NETWORK_TYPE_STATE);
            break;

        case INFRA_NETWORK_TYPE_STATE:
            NetworkTypeSet(WF_NETWORK_TYPE_INFRASTRUCTURE);
            SetSubState(INFRA_CHANNEL_LIST_STATE);
            break;

        case INFRA_CHANNEL_LIST_STATE:
            ChannelListSet(g_p_infrastructure->channelList);
            SetSubState(INFRA_RETRY_COUNT_STATE);
            break;

        case INFRA_RETRY_COUNT_STATE:
            SetRetryCount(g_p_infrastructure->reconnectMode.retryCount);
            SetSubState(INFRA_DEAUTH_ACTION_STATE);
            break;

        case INFRA_DEAUTH_ACTION_STATE:
            SetDeauthAction(g_p_infrastructure->reconnectMode.deauthAction);
            SetSubState(INFRA_BEACON_TIMEOUT_STATE);
            break;

        case INFRA_BEACON_TIMEOUT_STATE:
            SetBeaconTimeout(g_p_infrastructure->reconnectMode.beaconTimeout);
            SetSubState(INFRA_BEACON_TIMEOUT_ACTION_STATE);
            break;

        case INFRA_BEACON_TIMEOUT_ACTION_STATE:
            SetBeaconTimeoutAction(g_p_infrastructure->reconnectMode.beaconTimeoutAction);
            SetSubState(INFRA_SECURITY_STATE);
            break;

        case INFRA_SECURITY_STATE:
            // if open security
            if (g_p_infrastructure->securityType == WF_SECURITY_OPEN)
            {
                SecurityOpenSet();
                SetSubState(INFRA_START_CONNECT_STATE);
            }
            // else if WPA security
            else if ((g_p_infrastructure->securityType == WF_SECURITY_WPA)     ||
                     (g_p_infrastructure->securityType == WF_SECURITY_WPA2)    ||
                     (g_p_infrastructure->securityType == WF_SECURITY_WPA_AUTO))
            {
                // if using passphrase to calculate binary key
                if (g_p_infrastructure->security.wpaContext.useBinaryKey == false)
                {
                    // set the context for the WPA key calculation state machine
                    SetSha1Context(g_p_infrastructure->ssid, g_p_infrastructure->security.wpaContext.passphrase);
#if defined(WF_USE_DEBUG_OUTPUT)
                    WF_PRINTF("Starting WPA key calculation...\r\n");
#endif
                    SetSubState(INFRA_CALCULATE_WPA_KEY);
                }
                // else using binary key directly
                else
                {
#if defined(WF_USE_DEBUG_OUTPUT)
                    WF_PRINTF("Starting Connection...\r\n");
#endif
                    SecurityWpaSet(g_p_infrastructure->securityType, g_p_infrastructure->security.wpaContext.binaryKey);
                    SetSubState(INFRA_START_CONNECT_STATE);
                }
            }
            // else if WEP security
            else if ((g_p_infrastructure->securityType == WF_SECURITY_WEP_40)  ||
                     (g_p_infrastructure->securityType == WF_SECURITY_WEP_104))
            {
                SecurityWepSet(g_p_infrastructure->securityType, &g_p_infrastructure->security.wepContext);
                SetSubState(INFRA_START_CONNECT_STATE);
            }
            else
            {
                EventEnqueue(WF_EVENT_ERROR, UD_ERROR_INVALID_INFRA_SECURITY_TYPE);
                SetSubState(INFRA_STATE_MACHINE_FAILED);
            }
            break;

        case INFRA_CALCULATE_WPA_KEY:
            wpaCalcRes = WPA2_StateMachine();
            if (wpaCalcRes == SM_SUCCESSFUL)
            {
                uint8_t *p_key;
#if defined(WF_USE_DEBUG_OUTPUT)
                WF_PRINTF("  Key calculation complete\r\n");
#endif
                p_key = GetComputedWPAKey();

                // copy binary key to connection profile
                memcpy(g_p_infrastructure->security.wpaContext.binaryKey, p_key, WF_WPA_KEY_LENGTH);
#if defined(WF_USE_DEBUG_OUTPUT)
                WF_PRINTF("Starting connection...\r\n");
#endif
                SecurityWpaSet(g_p_infrastructure->securityType, p_key);

                SetSubState(INFRA_START_CONNECT_STATE);
            }
            break;

        case INFRA_START_CONNECT_STATE:
            Connect();
            SetSubState(INFRA_STATE_MACHINE_SUCCESS);
            break;

        case INFRA_STATE_MACHINE_SUCCESS:
            res = SM_SUCCESSFUL;
            break;

        case INFRA_STATE_MACHINE_FAILED:
            res = SM_FAIL;
            break;
    }

    return res;
}

#if defined(WF_USE_WPS)
//-------------------------------------------------------
// sub-state machine for infrastructure network using WPS
//-------------------------------------------------------
static uint8_t InfrastructureWpsStateMachine(void)
{
    uint8_t  res = SM_IN_PROGRESS;
    uint8_t  wpaCalcRes;
    uint8_t  channelList[WF_MAX_CHANNEL_LIST_LENGTH + 1];
    static uint32_t startTime = 0; // avoid compile warning

    switch ( GetSubState() )
    {
        case INFRA_WPS_START_STATE:
            if (g_p_infraWps->mode == WF_SECURITY_WPS_PIN)
            {
                SsidSet(g_p_infraWps->ssid);
            }
            // push button requires empty SSID
            else
            {
                SsidSet("");            // must be empty SSID for WPS-PushButton
            }
            SetSubState(INFRA_WPS_NETWORK_TYPE_STATE);
            break;

        case INFRA_WPS_NETWORK_TYPE_STATE:
            NetworkTypeSet(WF_NETWORK_TYPE_INFRASTRUCTURE);
            SetSubState(INFRA_WPS_CHANNEL_LIST_STATE);
            break;

        case INFRA_WPS_CHANNEL_LIST_STATE:
            memset(channelList, 0x00, sizeof(channelList));            // make sure channel list is terminated
            GetDomainChannelList(WF_DomainGet(), channelList);
            ChannelListSet(channelList);
            SetSubState(INFRA_WPS_RETRY_COUNT_STATE);
            break;

        case INFRA_WPS_RETRY_COUNT_STATE:
            SetRetryCount(g_p_infraWps->reconnectMode.retryCount);
            SetSubState(INFRA_WPS_DEAUTH_ACTION_STATE);
            break;

        case INFRA_WPS_DEAUTH_ACTION_STATE:
            SetDeauthAction(g_p_infraWps->reconnectMode.deauthAction);
            SetSubState(INFRA_WPS_BEACON_TIMEOUT_STATE);
            break;

        case INFRA_WPS_BEACON_TIMEOUT_STATE:
            SetBeaconTimeout(g_p_infraWps->reconnectMode.beaconTimeout);
            SetSubState(INFRA_WPS_BEACON_TIMEOUT_ACTION_STATE);
            break;

        case INFRA_WPS_BEACON_TIMEOUT_ACTION_STATE:
            SetBeaconTimeoutAction(g_p_infraWps->reconnectMode.beaconTimeoutAction);
            SetSubState(INFRA_WPS_SECURITY_STATE);
            break;

        case INFRA_WPS_SECURITY_STATE:
            if (g_p_infraWps->mode == WF_SECURITY_WPS_PIN)
            {
                SecurityWpsSet(g_p_infraWps->mode, g_p_infraWps->wpsPin);
            }
            // else pushbutton
            else
            {
                SecurityWpsSet(g_p_infraWps->mode, 0); // pin is ignored
            }
            SetSubState(INFRA_WPS_YIELD_PASSPHRASE_STATE);
            break;

        case INFRA_WPS_YIELD_PASSPHRASE_STATE:
            YieldPassPhraseToHost();   // have MRF send us the passphrase so we can convert to binary key
            SetSubState(INFRA_WPS_START_CONNECT_STATE);
            break;

        case INFRA_WPS_START_CONNECT_STATE:
#if defined(WF_USE_DEBUG_OUTPUT)
            if (g_p_infraWps->mode == WF_SECURITY_WPS_PUSH_BUTTON)
            {
                WF_PRINTF("Starting WPS handshake [Push Button on AP]...\r\n");
            }
            else
            {
                WF_PRINTF("Starting WPS handshake...\r\n");
            }
#endif
            Connect();
            g_wpsEvent = WPS_WAITING_FOR_EVENT;
            startTime = WF_TimerRead();
            SetSubState(INFRA_WPS_WAIT_FOR_CONNECT_OR_PASSPHRASE_STATE);
            break;

        case INFRA_WPS_WAIT_FOR_CONNECT_OR_PASSPHRASE_STATE:
            // When in this state, we are waiting for either a connect event, a
            // connect fail event, or, the MRF giving the host the WPA ASCII passphrase
            // so the host can calculate the binary key.

            // if AP sent us WPA passphrase
            if (g_wpsEvent == WPS_PASSPHRASE_EVENT)
            {
                // terminate the strings (the MRF does not do this)
                g_wpaKeyInfo.passphrase[g_wpaKeyInfo.passphraseLength] = '\0';
                g_wpaKeyInfo.ssid[g_wpaKeyInfo.ssidLen] = '\0';

                // read WPS info to get ssid and passphrase so we can calculate binary key
#if defined(WF_USE_DEBUG_OUTPUT)
                WF_PRINTF("Starting WPA key calculation...\r\n");
#endif
                SetSha1Context(g_wpaKeyInfo.ssid, g_wpaKeyInfo.passphrase);
                SetSubState(INFRA_WPS_CALCULATE_WPA_KEY);
            }
            // else AP connected/failed to connect (and MRF didn't need to send WPA passphrase)
            else if (g_wpsEvent == WPS_CONNECT_EVENT)
            {
                if (UdGetConnectionState() == CS_CONNECTED)
                {
                    SetSubState(INFRA_WPS_GET_WPS_CREDENTIALS);
                }
                else
                {
                    SetSubState(INFRA_WPS_STATE_MACHINE_FAILED);
                }
            }
            else if (GetElapsedTime(startTime, WF_TimerRead()) > WPS_WAIT_TIME)
            {
                EventEnqueue(WF_EVENT_ERROR, UD_ERROR_WPS_EVENT_TIMEOUT);
                SetSubState(INFRA_WPS_STATE_MACHINE_FAILED);
            }
            // else still waiting (g_wpsEvent == WPS_WAITING_FOR_EVENT)
            else
            {
                // keep waiting
            }
            break;

        case INFRA_WPS_CALCULATE_WPA_KEY:
            wpaCalcRes = WPA2_StateMachine();
            // if binary key calculated
            if (wpaCalcRes == SM_SUCCESSFUL)
            {
#if defined(WF_USE_DEBUG_OUTPUT)
                WF_PRINTF("  Key calculation complete\r\n");
#endif
                // send binary WPA key to MRF
                SetPSK(GetComputedWPAKey());
                SetSubState(INFRA_WPS_GET_WPS_CREDENTIALS);
            }
            break;

        case INFRA_WPS_GET_WPS_CREDENTIALS:
            WpsCredentialsGet();
            g_wpsCredentialsValid = true;
#if defined(WF_USE_DEBUG_OUTPUT)
            DumpWpsCredentials();
#endif
            SetSubState(INFRA_WPS_STATE_MACHINE_SUCCESS);
            break;

        case INFRA_WPS_STATE_MACHINE_SUCCESS:
            res = SM_SUCCESSFUL;
            break;

        case INFRA_WPS_STATE_MACHINE_FAILED:
            res = SM_FAIL;
            break;
    }

    return res;
}

// channel list is initialized to all 0's before this function is called
static void GetDomainChannelList(uint8_t domain, uint8_t *p_channelList)
{
    int i;
    uint8_t stopChannel = 0;  // avoid warning

    if (domain == WF_DOMAIN_FCC)
    {
        stopChannel = 11;
    }
    else if (domain == WF_DOMAIN_ETSI)
    {
        stopChannel = 13;
    }
    else if ((domain == WF_DOMAIN_JAPAN) || (domain == WF_DOMAIN_OTHER))
    {
        stopChannel = 14;
    }

    for (i = 0; i < stopChannel; ++i)
    {
        p_channelList[i] = i + 1;
    }
}
#endif // WF_USE_WPS

#if defined(WF_USE_SOFTAP)

static uint8_t SoftApStateMachine(void)
{
    uint8_t res = SM_IN_PROGRESS;
    uint8_t channelList[2];

    switch ( GetSubState() )
    {
        case SOFTAP_START_STATE:
            memset(&g_softApClientTable, 0x00, sizeof(t_softApClientTable));
            SsidSet(g_p_softAp->ssid);
            SetSubState(SOFTAP_NETWORK_TYPE_STATE);
            break;

        case SOFTAP_NETWORK_TYPE_STATE:
            NetworkTypeSet(WF_NETWORK_TYPE_SOFT_AP);
            SetSubState(SOFTAP_CHANNEL_STATE);
            break;

        case SOFTAP_CHANNEL_STATE:
            channelList[0] = g_p_softAp->channel;
            channelList[1] = '\0'; // terminate the list
            ChannelListSet(channelList);
            SetSubState(SOFTAP_SECURITY_STATE);
            break;

        case SOFTAP_SECURITY_STATE:
            if (g_p_softAp->securityType == WF_SECURITY_OPEN)
            {
                SecurityOpenSet();
            }
            else // WEP security
            {
                SecurityWepSet(g_p_softAp->securityType, &g_p_softAp->wepContext);
            }
            SetSubState(SOFTAP_START_NETWORK_STATE);
            break;

        case SOFTAP_START_NETWORK_STATE:
            Connect();
            SetSubState(SOFTAP_STATE_MACHINE_SUCCESS);
            break;

        case SOFTAP_STATE_MACHINE_SUCCESS:
            res = SM_SUCCESSFUL;
            break;

        case SOFTAP_STATE_MACHINE_FAILURE:
            res = SM_FAIL;
            break;
    }

    return res;
}

void WF_SoftApClientTableGet(t_softApClientTable *p_table)
{
    memcpy(p_table, &g_softApClientTable, sizeof(t_softApClientTable));
}

// mark the corresponding index as used
static void SoftApMarkSlotUsed(uint8_t index)
{
    uint8_t mask = (0x01 << index);
    g_softApClientTable.slotMask |= mask;
}

// mark the corresponding index as free
static void SoftApMarkSlotFree(uint8_t index)
{
    uint8_t mask = (0x01 << index);
    g_softApClientTable.slotMask &= ~mask;
}

// find a specific MAC within client table
static uint8_t SoftApFindMacSlot(uint8_t *p_mac)
{
    int i;
    for (i = 0; i < WF_MAX_SOFTAP_CLIENTS; ++i)
    {
        // if we found a match
        if (memcmp(g_softApClientTable.mac[i], p_mac, WF_MAC_ADDRESS_LENGTH) == 0)
        {
            return i;
        }
    }

    EventEnqueue(WF_EVENT_ERROR, UD_ERROR_SOFTAP_INTERNAL_2);
    return 0xff;
}

static uint8_t SoftApFindEmptySlot(void)
{
    uint8_t i;
    uint8_t mask = 0x0001;
    for (i = 0; i < WF_MAX_SOFTAP_CLIENTS; ++i)
    {
        // if slot is empty
        if ((g_softApClientTable.slotMask & mask) == 0)
        {
            return i;
        }
        // else keep looking
        else
        {
            mask = mask << 1;
        }
    }

    // if made it here, table is full, which should never happen
    EventEnqueue(WF_EVENT_ERROR, UD_ERROR_SOFTAP_INTERNAL_1);
    return 0xff;
}

void SoftApAddClient(uint8_t *p_mac)
{
    uint8_t index = SoftApFindEmptySlot();

    if (index != 0xff)
    {
        memcpy(g_softApClientTable.mac[index], p_mac, WF_MAC_ADDRESS_LENGTH);
        SoftApMarkSlotUsed(index);
    }
}

void SoftApRemoveClient(uint8_t *p_mac)
{
    uint8_t index = SoftApFindMacSlot(p_mac);
    if (index != 0xff)
    {
        SoftApMarkSlotFree(index);
    }
}

#endif // WF_USE_SOFTAP


#if defined(WF_USE_ADHOC)
//------------------------------------------------
// sub-state machine for AdHoc network
//------------------------------------------------
static uint8_t AdHocStateMachine(void)
{
    uint8_t  res = SM_IN_PROGRESS;

    switch ( GetSubState() )
    {
        case ADHOC_START_STATE:
            SsidSet(g_p_adhoc->ssid);
            SetSubState(ADHOC_SET_HIDDEN_SSID_STATE);
            break;

        case ADHOC_SET_HIDDEN_SSID_STATE:
            SetAdhocSsidType(g_p_adhoc->hiddenSsid);
            SetSubState(ADHOC_NETWORK_TYPE_STATE);
            break;

        case ADHOC_NETWORK_TYPE_STATE:
            NetworkTypeSet(WF_NETWORK_TYPE_ADHOC);
            SetSubState(ADHOC_CHANNEL_LIST_STATE);
            break;

        case ADHOC_CHANNEL_LIST_STATE:
            ChannelListSet(g_p_adhoc->channelList);
            SetSubState(ADHOC_RETRY_COUNT_STATE);
            break;

        case ADHOC_RETRY_COUNT_STATE:
            SetRetryCount(g_p_adhoc->reconnectMode.retryCount);
            SetSubState(ADHOC_DEAUTH_ACTION_STATE);
            break;

        case ADHOC_DEAUTH_ACTION_STATE:
            SetDeauthAction(g_p_adhoc->reconnectMode.deauthAction);
            SetSubState(ADHOC_BEACON_TIMEOUT_STATE);
            break;

        case ADHOC_BEACON_TIMEOUT_STATE:
            SetBeaconTimeout(g_p_adhoc->reconnectMode.beaconTimeout);
            SetSubState(ADHOC_BEACON_TIMEOUT_ACTION_STATE);
            break;

        case ADHOC_BEACON_TIMEOUT_ACTION_STATE:
            SetBeaconTimeoutAction(g_p_adhoc->reconnectMode.beaconTimeoutAction);
            SetSubState(ADHOC_SECURITY_STATE);
            break;

        case ADHOC_SECURITY_STATE:
            // if open security
            if (g_p_adhoc->securityType == WF_SECURITY_OPEN)
            {
                SecurityOpenSet();
                SetSubState(ADHOC_BEACON_PERIOD_STATE);
            }
            // else if WEP-40 or WEP-104
            else if ((g_p_adhoc->securityType == WF_SECURITY_WEP_40)  ||
                     (g_p_adhoc->securityType == WF_SECURITY_WEP_104))
            {
                SecurityWepSet(g_p_adhoc->securityType, &g_p_adhoc->wepContext);
                SetSubState(ADHOC_BEACON_PERIOD_STATE);
            }
            // else invalid security for AdHoc
            else
            {
                EventEnqueue(WF_EVENT_ERROR, UD_ERROR_INVALID_ADHOC_SECURITY_TYPE);
                SetSubState(ADHOC_STATE_MACHINE_FAILED);
            }
            break;

        case ADHOC_BEACON_PERIOD_STATE:
            SetAdhocBeaconPeriod(g_p_adhoc->beaconPeriod);
            SetSubState(ADHOC_MODE_STATE);
            break;

        case ADHOC_MODE_STATE:
            SetAdHocMode(g_p_adhoc->mode);
            SetSubState(ADHOC_START_CONNECT_STATE);
            break;

        case ADHOC_START_CONNECT_STATE:
            Connect();
            SetSubState(ADHOC_STATE_MACHINE_SUCCESS);
            break;

        case ADHOC_STATE_MACHINE_SUCCESS:
            res = SM_SUCCESSFUL;
            break;

        case ADHOC_STATE_MACHINE_FAILED:
            res = SM_FAIL;
            break;
    }
    return res;
}
#endif // WF_USE_ADHOC

