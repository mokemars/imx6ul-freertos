/**
 * @file twr_mcf5225x.h
 * @brief TWR-MCF5225X module
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

#ifndef _TWR_MCF5225X_H
#define _TWR_MCF5225X_H

//Dependencies
#include "mcf52259.h"

//System bus frequency
#define SYSTEM_CLOCK_HZ 80000000

//LED1
#define LED1_MASK    (1 << 0)
//LED2
#define LED2_MASK    (1 << 1)
//LED3
#define LED3_MASK    (1 << 2)
//LED4
#define LED4_MASK    (1 << 3)

//SW1
#define SW1_MASK     (1 << 0)
//SW3
#define SW3_MASK     (1 << 1)

#endif
