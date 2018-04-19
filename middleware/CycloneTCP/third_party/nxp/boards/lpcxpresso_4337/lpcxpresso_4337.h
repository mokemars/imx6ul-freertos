/**
 * @file lpcxpresso_4337.h
 * @brief LPCXpresso4337 evaluation board
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

#ifndef _LPCXPRESSO_4337_H
#define _LPCXPRESSO_4337_H

//Dependencies
#include "lpc43xx.h"

//Red LED
#define LED_R_MASK (1 << 7)
#define LED_R_PORT 3

//Green LED
#define LED_G_MASK (1 << 7)
#define LED_G_PORT 0

//Blue LED
#define LED_B_MASK (1 << 5)
#define LED_B_PORT 3

//SW2
#define SW2_MASK  (1 << 7)
#define SW2_PORT  0

#endif
