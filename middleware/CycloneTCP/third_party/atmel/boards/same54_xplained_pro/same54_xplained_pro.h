/**
 * @file same54_xplained_pro.h
 * @brief SAME54-Xplained-Pro evaluation kit
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

#ifndef _SAME54_XPLAINED_PRO_H
#define _SAME54_XPLAINED_PRO_H

//Dependencies
#include "same54.h"

//LED0 (PC18)
#define LED0_GROUP 2
#define LED0_PIN   18
#define LED0_MASK  (1U << 18)

//SW0 button (PB31)
#define SW0_GROUP  1
#define SW0_PIN    31
#define SW0_MASK   (1U << 31)

#endif
