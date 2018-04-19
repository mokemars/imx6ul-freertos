/**
 * @file iot_ethernet_kit.h
 * @brief IoT Ethernet Kit
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

#ifndef _IOT_ETHERNET_KIT_H
#define _IOT_ETHERNET_KIT_H

//Dependencies
#include <p32xxxx.h>

//LED1 (RB13)
#define LED1_MASK (1 << 13)
//LED2 (RB12)
#define LED2_MASK (1 << 12)
//LED3 (RF1)
#define LED3_MASK (1 << 1)
//LED4 (RC15)
#define LED4_MASK (1 << 15)

//SW1 (RB10)
#define SW1_MASK (1 << 10)
//SW2 (RB8)
#define SW2_MASK (1 << 8)
//SW3 (RF0)
#define SW3_MASK (1 << 0)
//SW4 (RD5)
#define SW4_MASK (1 << 5)

#endif
