/**
 * @file lpc1830_xplorer.h
 * @brief LPC1830-Xplorer evaluation board
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

#ifndef _LPC1830_XPLORER_H
#define _LPC1830_XPLORER_H

//Dependencies
#include "lpc18xx.h"

//LED2
#define LED2_MASK (1 << 12)
#define LED2_PORT 1

//LED3
#define LED3_MASK (1 << 11)
#define LED3_PORT 1

//SW2
#define SW2_MASK  (1 << 7)
#define SW2_PORT  0

#endif
