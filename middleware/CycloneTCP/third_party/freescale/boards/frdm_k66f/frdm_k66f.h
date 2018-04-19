/**
 * @file frdm_k66f.h
 * @brief FRDM-K66F evaluation board
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

#ifndef _FRDM_K66F_H
#define _FRDM_K66F_H

//Dependencies
#include "mk66f18.h"

//Red LED
#define LED_R_POS    9
#define LED_R_MASK   (1 << LED_R_POS)
#define PORT_LED_R   PORTC
#define GPIO_LED_R   PTC

//Green LED
#define LED_G_POS    6
#define LED_G_MASK   (1 << LED_G_POS)
#define PORT_LED_G   PORTE
#define GPIO_LED_G   PTE

//Blue LED
#define LED_B_POS    11
#define LED_B_MASK   (1 << LED_B_POS)
#define PORT_LED_B   PORTA
#define GPIO_LED_B   PTA

//SW2
#define SW2_POS      11
#define SW2_MASK     (1 << SW2_POS)
#define PORT_SW2     PORTD
#define GPIO_SW2     PTD

//SW3
#define SW3_POS      10
#define SW3_MASK     (1 << SW3_POS)
#define PORT_SW3     PORTA
#define GPIO_SW3     PTA

#endif
