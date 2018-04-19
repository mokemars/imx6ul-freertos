/**
 * @file sam4e_ek.h
 * @brief SAM4E-EK evaluation kit
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

#ifndef _SAM4E_EK_H
#define _SAM4E_EK_H

//Dependencies
#include "sam4e.h"

//LED1
#define LED1            PIO_PA0
#define PIO_LED1        PIOA

//LED2
#define LED2            PIO_PD20
#define PIO_LED2        PIOD

//LED3
#define LED3            PIO_PD21
#define PIO_LED3        PIOD

//LED4
#define LED4            PIO_PD22
#define PIO_LED4        PIOD

//Up button
#define BT_UP          PIO_PA1
#define PIO_BT_UP      PIOA

//Down button
#define BT_DOWN        PIO_PA2
#define PIO_BT_DOWN    PIOA

//WAKU button
#define BT_WAKU        PIO_PA19
#define PIO_BT_WAKU    PIOA

//TAMP button
#define BT_TAMP        PIO_PA20
#define PIO_BT_TAMP    PIOA

//LCD data bus
#define LCD_DATA       (PIO_PC7 | PIO_PC6 | PIO_PC5 | PIO_PC4 | PIO_PC3 | PIO_PC2 | PIO_PC1 | PIO_PC0)
#define PIO_LCD_DATA   PIOC

//LCD control pins
#define LCD_RS         PIO_PC19
#define PIO_LCD_RS     PIOC

#define LCD_RD         PIO_PC11
#define PIO_LCD_RD     PIOC

#define LCD_WR         PIO_PC8
#define PIO_LCD_WR     PIOC

#define LCD_NCS1       PIO_PD18
#define PIO_LCD_NCS1   PIOD

//LCD backlight pin
#define LCD_BL        PIO_PC13
#define PIO_LCD_BL    PIOC

#endif
