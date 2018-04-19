/**
 * @file bcm4xxxx_driver.h
 * @brief BCM4XXXX Wi-Fi controller
 *
 * @section License
 *
 * Copyright (C) 2010-2017 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Open.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.0
 **/

#ifndef _BCM4XXXX_DRIVER_H
#define _BCM4XXXX_DRIVER_H

//Dependencies
#include "core/nic.h"

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

//BCM4xxxx driver (STA mode)
extern const NicDriver bcm4xxxxStaDriver;
//BCM4xxxx driver (AP mode)
extern const NicDriver bcm4xxxxApDriver;

//BCM4xxxx related functions
error_t bcm4xxxxInit(NetInterface *interface);

void bcm4xxxxTick(NetInterface *interface);

void bcm4xxxxEnableIrq(NetInterface *interface);
void bcm4xxxxDisableIrq(NetInterface *interface);
bool_t bcm4xxxxIrqHandler(void);
void bcm4xxxxEventHandler(NetInterface *interface);

error_t bcm4xxxxSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset);

error_t bcm4xxxxSetMulticastFilter(NetInterface *interface);

void bcm4xxxxAppWifiEvent(uint8_t msgType, void *msg);
void bcm4xxxxAppEthEvent(uint8_t msgType, void *msg, void *ctrlBuf);

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
