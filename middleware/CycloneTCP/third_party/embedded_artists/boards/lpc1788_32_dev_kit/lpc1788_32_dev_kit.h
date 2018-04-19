/**
 * @file lpc1788_32_dev_kit.h
 * @brief LPC1788-32 Developer's Kit
 *
 * @section License
 *
 * Copyright (C) 2010-2017 Oryx Embedded SARL. All rights reserved.
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

#ifndef _LPC1788_32_DEV_KIT_H
#define _LPC1788_32_DEV_KIT_H

//Dependencies
#include "lpc177x_8x.h"

//LED3
#define LED3_MASK                (1 << 26)
#define LED3_FIODIR              LPC_GPIO2->DIR
#define LED3_FIOSET              LPC_GPIO2->SET
#define LED3_FIOCLR              LPC_GPIO2->CLR

//LED4
#define LED4_MASK                (1 << 27)
#define LED4_FIODIR              LPC_GPIO2->DIR
#define LED4_FIOSET              LPC_GPIO2->SET
#define LED4_FIOCLR              LPC_GPIO2->CLR

//SW6
#define SW6_MASK                 (1 << 10)
#define SW6_FIODIR               LPC_GPIO2->DIR
#define SW6_FIOPIN               LPC_GPIO2->PIN

//Joystick
#define JOYSTICK_UP_MASK         (1 << 25)
#define JOYSTICK_DOWN_MASK       (1 << 27)
#define JOYSTICK_LEFT_MASK       (1 << 23)
#define JOYSTICK_RIGHT_MASK      (1 << 26)
#define JOYSTICK_FIODIR          LPC_GPIO2->DIR
#define JOYSTICK_FIOPIN          LPC_GPIO2->PIN

#endif
