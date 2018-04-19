/*******************************************************************************
 MRF24WG Universal Driver Error Checking

  Summary: This module contains error checking definitions

  Description: This module is an extension of wf_events.h, but specific to
               WiFi connection-related events
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


#ifndef __WF_UD_STATE_H
#define __WF_UD_STATE_H

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
//                                  CONSTANTS
//==============================================================================



typedef enum t_internalConnectionState
{
    CS_NOT_CONNECTED           = 0,
    CS_CONNECTION_IN_PROGRESS  = 1,
    CS_CONNECTED               = 2,
} t_internalConnectionState;

//==============================================================================
//                                  DATA TYPES
//==============================================================================
typedef struct udStateStruct
{
    uint8_t         connectionState;
    bool            psPollEnabled;
    t_psPollContext psPollContext;
} t_udState;

//==============================================================================
//                                  GLOBALS
//==============================================================================
extern t_udState g_udState;

//==============================================================================
//                                  MACROS
//==============================================================================
#define UdSetConnectionState(s)         g_udState.connectionState = s
#define UdGetConnectionState()          g_udState.connectionState


//==============================================================================
//                                  ERROR BITMAP MACROS
//==============================================================================


//==============================================================================
//                                  FUNCTION PROTOTYPES
//==============================================================================

void  UdStateInit(void);

void  UdEnablePsPoll(t_psPollContext *p_context);
void  UdDisablePsPoll(void);
INLINE bool  UdisPsPollEnabled(void);

#if defined(WF_USE_ERROR_CHECKING)
    uint32_t UdValidateProfile(t_connectionProfile *p_connection);
    
    uint32_t UdSetScanContext(t_scanContext *p_context);
    uint32_t UdSetTxMode(uint8_t mode);
    uint32_t UdSetBssid(uint8_t *p_bssid);
    uint32_t UdSetRssi(uint8_t rssi);
    uint32_t UdSetRtsThreshold(uint16_t rtsThreshold);
    uint32_t UdScan(void);
    uint32_t udSetTxPowerMax(uint8_t maxTxPower, uint8_t factoryMaxTxPower);
    uint32_t UdSetMulticastFilter(t_multicastConfig *p_config);


#endif /* WF_USE_ERROR_CHECKING */

#ifdef __cplusplus
extern "C" {
#endif


#endif /* __WF_UD_STATE_H */
