/**
 * @file twr_kv58f220m.h
 * @brief TWR-KV58F220M module
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

#ifndef _TWR_KV58F220M_H
#define _TWR_KV58F220M_H

//Dependencies
#include "mkv58f22.h"

//LED1
#define LED1_POS     11
#define LED1_MASK    (1 << LED1_POS)
#define PORT_LED1    PORTE
#define GPIO_LED1    GPIOE

//LED2
#define LED2_POS     12
#define LED2_MASK    (1 << LED2_POS)
#define PORT_LED2    PORTE
#define GPIO_LED2    GPIOE

//LED3
#define LED3_POS     29
#define LED3_MASK    (1 << LED3_POS)
#define PORT_LED3    PORTE
#define GPIO_LED3    GPIOE

//LED4
#define LED4_POS     30
#define LED4_MASK    (1 << LED4_POS)
#define PORT_LED4    PORTE
#define GPIO_LED4    GPIOE

//SW2
#define SW2_POS      4
#define SW2_MASK     (1 << SW2_POS)
#define PORT_SW2     PORTA
#define GPIO_SW2     GPIOA

//SW3
#define SW3_POS      4
#define SW3_MASK     (1 << SW3_POS)
#define PORT_SW3     PORTE
#define GPIO_SW3     GPIOE

//SW4
#define SW4_POS      5
#define SW4_MASK     (1 << SW4_POS)
#define PORT_SW4     PORTB
#define GPIO_SW4     GPIOB

//SW5
#define SW5_POS      4
#define SW5_MASK     (1 << SW5_POS)
#define PORT_SW5     PORTB
#define GPIO_SW5     GPIOB

#endif
