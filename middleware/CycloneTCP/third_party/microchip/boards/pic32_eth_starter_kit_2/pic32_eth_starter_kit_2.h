/**
 * @file pic32_eth_starter_kit_2.h
 * @brief PIC32 Ethernet Starter Kit II
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

#ifndef _PIC32_ETH_STARTER_KIT_2_H
#define _PIC32_ETH_STARTER_KIT_2_H

//Dependencies
#include <p32xxxx.h>

//LEDs
#define LED1_MASK (1 << 0)
#define LED2_MASK (1 << 1)
#define LED3_MASK (1 << 2)

//Push buttons
#define SW1_MASK (1 << 6)
#define SW2_MASK (1 << 7)
#define SW3_MASK (1 << 13)

//MEB LEDs
#define MEB_LED1_MASK (1 << 1)
#define MEB_LED2_MASK (1 << 2)
#define MEB_LED3_MASK (1 << 3)
#define MEB_LED4_MASK (1 << 2)
#define MEB_LED5_MASK (1 << 1)

//MEB joystick
#define MEB_JOY_LEFT_MASK  (1 << 0)
#define MEB_JOY_RIGHT_MASK (1 << 4)
#define MEB_JOY_UP_MASK    (1 << 1)
#define MEB_JOY_DOWN_MASK  (1 << 3)
#define MEB_JOY_FIRE_MASK  (1 << 15)

#endif
