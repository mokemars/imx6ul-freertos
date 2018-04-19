/**
 * @file sam7se_ek.h
 * @brief SAM7SE-EK evaluation kit
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

#ifndef _SAM7SE_EK_H
#define _SAM7SE_EK_H

//Dependencies
#include "at91sam7se512.h"

//Macro definitions
#define FALSE 0
#define TRUE  1

#define LSB(x) ((x) & 0xFF)
#define MSB(x) (((x) >> 8) & 0xFF)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define arraysize(a) (sizeof(a) / sizeof(a[0]))

//Master clock
#define AT91B_MCK (18432000 * 73 / 14 / 2)

//Push buttons
#define AT91B_SW1      AT91C_PIO_PB25
#define AT91B_SW2      AT91C_PIO_PB22
#define AT91B_SW_UP    AT91C_PIO_PB23
#define AT91B_SW_DOWN  AT91C_PIO_PB24
#define AT91B_SW_LEFT  AT91C_PIO_PB27
#define AT91B_SW_RIGHT AT91C_PIO_PB26
#define AT91B_SW_OK    AT91C_PIO_PB25

#define AT91B_SW_MASK (AT91B_SW1 | AT91B_SW2 | AT91B_SW_UP | \
   AT91B_SW_DOWN | AT91B_SW_LEFT | AT91B_SW_RIGHT | AT91B_SW_OK)

#define AT91D_BASE_PIO_SW AT91C_BASE_PIOB
#define AT91D_ID_PIO_SW AT91C_ID_PIOB

//LEDs
#define AT91B_LED1 AT91C_PIO_PA1 //PA1/PWM1/A1/NBS2
#define AT91B_LED2 AT91C_PIO_PA2 //PA2/PWM2/A2
#define AT91B_LED3 AT91C_PIO_PA0 //PA0/PWM0/A0/NBS0

#define AT91B_LED_MASK     (AT91B_LED1 | AT91B_LED2 | AT91B_LED3)
#define AT91D_BASE_PIO_LED AT91C_BASE_PIOA
#define AT91D_ID_PIO_LED   AT91C_ID_PIOA

//Additional macro definitions
#define isButtonPressed(n) ((AT91D_BASE_PIO_SW->PIO_PDSR & (n)) ? 0 : 1)
#define setLed(n)          AT91D_BASE_PIO_LED->PIO_CODR = (n)
#define clearLed(n)        AT91D_BASE_PIO_LED->PIO_SODR = (n)

#endif
