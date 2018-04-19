/**
 * @file mimxrt1050_evk.h
 * @brief MIMXRT1050-EVK evaluation board
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

#ifndef _MIMXRT1050_EVK_H
#define _MIMXRT1050_EVK_H

//Dependencies
#include "fsl_device_registers.h"

//User LED
#define USER_LED_GPIO    GPIO1
#define USER_LED_PIN     9

//User button
#define USER_BUTTON_GPIO GPIO5
#define USER_BUTTON_PIN  0

#endif
