/*******************************************************************************
 MRF24WG Universal Driver Connection Profile

  Summary: This module contains connection profile functions.

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

#ifndef __WF_CONNECTION_PROFILE_H
#define __WF_CONNECTION_PROFILE_H

#ifdef __cplusplus
extern "C" {
#endif


INLINE uint8_t GetCpid(void);

void SsidSet(char *p_ssid);
void NetworkTypeSet(uint8_t networkType);
void ChannelListSet(uint8_t *p_channelList);
void ReconnectModeSet(t_reconnectMode *p_reconnectMode);
void SetRetryCount(uint8_t retryCount);
void SetBeaconTimeout(uint8_t beaconTimeout);
void SetDeauthAction(uint8_t deauthAction);
void SetBeaconTimeoutAction(uint8_t beaconTimeoutAction);
void SecurityOpenSet(void);
void SecurityWepSet(uint8_t wepSecurityType, t_wepContext* p_context);
void SecurityWpaSet(uint8_t securityType, uint8_t *p_key);
void StartConnectStateMachine(void);
void Connect(void);

#if defined(WF_USE_WPS)
void SecurityWpsSet(uint8_t wpsSecurityType, uint32_t wpsPin);
void WpsCredentialsGet(void);
#endif // WF_USE_WPS

#if defined(WF_USE_ADHOC)
void SetAdHocMode(uint8_t mode);
void SetAdhocSsidType(bool hidden);
void SetAdhocBeaconPeriod(uint16_t beaconPeriod);
void SetAdHocMode(uint8_t mode);
void SetHiddenSsid(bool hiddenSsid);
#endif // WF_USE_ADHOC




#ifdef __cplusplus
}
#endif


#endif /* __WF_CONNECTION_PROFILE_H */

