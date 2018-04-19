/**
 * @file xmc4500_relax_kit.h
 * @brief XMC4500 Relax Kit
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

#ifndef _XMC4500_RELAX_KIT_H
#define _XMC4500_RELAX_KIT_H

//Dependencies
#include "xmc4500.h"

//LED1
#define LED1_MASK    PORT1_OUT_P1_Msk
#define PORT_LED1    PORT1

//LED2
#define LED2_MASK    PORT1_OUT_P0_Msk
#define PORT_LED2    PORT1

//Button 1
#define BUTTON1_MASK PORT1_IN_P14_Msk
#define PORT_BUTTON1 PORT1

//Button 2
#define BUTTON2_MASK PORT1_IN_P15_Msk
#define PORT_BUTTON2 PORT1

#endif
