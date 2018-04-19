/**
 * @file sam7_ex256.h
 * @brief SAM7-EX256 demonstration board
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

#ifndef _SAM7_EX256_H
#define _SAM7_EX256_H

//Dependencies
#include "at91sam7x256.h"

//Master clock
#define AT91B_MCK (18432000 * 73 / 14 / 2)

//Push buttons
#define SW1_MASK           AT91C_PIO_PB24
#define SW2_MASK           AT91C_PIO_PB25
#define SW_BASE_PIO        AT91C_BASE_PIOB
#define SW_ID_PIO          AT91C_ID_PIOB

//LCD backlight
#define LCD_BL_MASK        AT91C_PIO_PB20
#define LCD_BL_BASE_PIO    AT91C_BASE_PIOB
#define LCD_BL_ID_PIO      AT91C_ID_PIOB

//LCD reset
#define LCD_RST_MASK       AT91C_PIO_PA2
#define LCD_RST_BASE_PIO   AT91C_BASE_PIOA
#define LCD_RST_ID_PIO     AT91C_ID_PIOA

//LCD CS
#define LCD_CS_MASK        AT91C_PIO_PA12
#define LCD_CS_BASE_PIO    AT91C_BASE_PIOA
#define LCD_CS_ID_PIO      AT91C_ID_PIOA

#endif
