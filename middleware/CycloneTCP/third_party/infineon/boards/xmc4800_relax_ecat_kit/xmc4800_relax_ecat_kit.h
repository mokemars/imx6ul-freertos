/**
 * @file xmc4800_relax_ecat_kit.h
 * @brief XMC4800 Relax EtherCAT Kit
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

#ifndef _XMC4800_RELAX_ECAT_KIT_H
#define _XMC4800_RELAX_ECAT_KIT_H

//Dependencies
#include "xmc4800.h"

//LED1
#define LED1_MASK    PORT5_OUT_P9_Msk
#define PORT_LED1    PORT5

//LED2
#define LED2_MASK    PORT5_OUT_P8_Msk
#define PORT_LED2    PORT5

//Button 1
#define BUTTON1_MASK PORT15_IN_P13_Msk
#define PORT_BUTTON1 PORT15

//Button 2
#define BUTTON2_MASK PORT15_IN_P12_Msk
#define PORT_BUTTON2 PORT15

#endif
