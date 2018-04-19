/*******************************************************************************
 MRF24WG Universal Driver Parameter Messages

  Summary: This module contains parameter message defintions

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


#ifndef __WF_PARAM_MSG_H
#define __WF_PARAM_MSG_H

#ifdef __cplusplus
extern "C" {
#endif


#define MSG_PARAM_START_DATA_INDEX          (6)
#define MULTICAST_ADDRESS                   (6)
#define ADDRESS_FILTER_DEACTIVATE           (0)

#define ENABLE_MRF24WB0M                    (1)

void DeviceInfoGet(t_deviceInfo *p_info);
void GetMacAddress(uint8_t *p_macAddress);
uint8_t GetDomain(void);
void EnableMRF24WB0MMode(void);
void SetFactoryTxPower(uint8_t maxTxPower);
void SetTxDataConfirm(uint8_t state);
void YieldPassPhraseToHost(void);
void SetPSK(uint8_t *psk);
void GetConnectionContext(void);
void EnableSWMulticastFilter(void);



#ifdef __cplusplus
}
#endif


#endif /* __WF_PARAM_MSG_H */

