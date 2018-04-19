/**
 * @file twr_k65f180m.h
 * @brief TWR-K65F180M module
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

#ifndef _TWR_K65F180M_H
#define _TWR_K65F180M_H

//Dependencies
#include "mk65f18.h"

//LED6
#define LED6_POS     4
#define LED6_MASK    (1 << LED6_POS)
#define PORT_LED6    PORTB
#define GPIO_LED6    PTB

//LED7
#define LED7_POS     5
#define LED7_MASK    (1 << LED7_POS)
#define PORT_LED7    PORTB
#define GPIO_LED7    PTB

//LED8
#define LED8_POS     28
#define LED8_MASK    (1 << LED8_POS)
#define PORT_LED8    PORTA
#define GPIO_LED8    PTA

//LED9
#define LED9_POS     29
#define LED9_MASK    (1 << LED9_POS)
#define PORT_LED9    PORTA
#define GPIO_LED9    PTA

//SW2
#define SW2_POS      4
#define SW2_MASK     (1 << SW2_POS)
#define PORT_SW2     PORTA
#define GPIO_SW2     PTA

//SW3
#define SW3_POS      10
#define SW3_MASK     (1 << SW3_POS)
#define PORT_SW3     PORTA
#define GPIO_SW3     PTA

#endif
