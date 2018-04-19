/*******************************************************************************
 MRF24WG Universal Driver Init

  Summary: This module contains the WiFi connection state machine

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

#ifndef __WF_CONNECTION_SM_H
#define __WF_CONNECTION_SM_H

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    WPS_WAITING_FOR_EVENT = 0,
    WPS_PASSPHRASE_EVENT  = 1,
    WPS_CONNECT_EVENT     = 2
};

#if defined(WF_USE_WPS)
typedef struct
{
    uint8_t passphraseLength;                      // number of bytes in binary key (always 32) or passphrase
    char    passphrase[WF_MAX_PASSPHRASE_LENGTH];  // passphrase (must be 64 bytes)
    uint8_t ssidLen;                               // number of bytes in SSID
    char    ssid[WF_MAX_SSID_LENGTH];              // ssid (must be 32 bytes)
} t_wpaKeyInfo;
#endif // WF_USE_WPS


void    ResetConnectionSm(void);
uint8_t ConnectionSm(void);
void    SaveDomain(uint8_t domain);
t_connectionProfile * GetConnectionProfile(void);

#if defined(WF_USE_WPS)
bool    isWpsConnection(void);
void    SetWpsConnectionEvent(uint8_t status);
t_wpaKeyInfo *  GetKeyInfoPtr(void);
bool isWpsCredentialsValid(void);
#endif // WF_USE_WPS

#if defined(WF_USE_SOFTAP)
void SoftApAddClient(uint8_t *p_mac);
void SoftApRemoveClient(uint8_t *p_mac);
#endif // WF_USE_SOFTAP
bool isSoftApProfile(void);
bool isAdHocProfile(void);
bool isConnectionOperational(void);
void NotifyConnectionLost(void);
uint8_t isScanAllowed(void);

#ifdef __cplusplus
}
#endif

#endif // __WF_CONNECTION_SM_H
