/**
 * @file sam3x_ek.h
 * @brief SAM3X-EK evaluation kit
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

#ifndef _SAM3X_EK_H
#define _SAM3X_EK_H

//Dependencies
#include "sam3xa.h"

//LED1
#define LED1            PIO_PA12
#define PIO_LED1        PIOA
#define ID_PIO_LED1     ID_PIOA

//LED2
#define LED2            PIO_PB12
#define PIO_LED2        PIOB
#define ID_PIO_LED2     ID_PIOB

//LED3
#define LED3            PIO_PB13
#define PIO_LED3        PIOB
#define ID_PIO_LED3     ID_PIOB

//LED4
#define LED4            PIO_PA13
#define PIO_LED4        PIOA
#define ID_PIO_LED4     ID_PIOA

//Left button
#define BT_LEFT         PIO_PE7
#define PIO_BT_LEFT     PIOE
#define ID_PIO_BT_LEFT  ID_PIOE

//Right button
#define BT_RIGHT        PIO_PB23
#define PIO_BT_RIGHT    PIOB
#define ID_PIO_BT_RIGHT ID_PIOB

#endif
