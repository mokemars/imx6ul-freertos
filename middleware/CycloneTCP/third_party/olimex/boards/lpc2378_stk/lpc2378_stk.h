/**
 * @file lpc2378_stk.h
 * @brief LPC2378-STK demonstration board
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

#ifndef _LPC2378_STK_H
#define _LPC2378_STK_H

//Dependencies
#include "lpc23xx.h"

//CPU clock frequency
#define CPU_CLOCK_FREQ           72000000

//USB link LED
#define LED_USB_LINK_MASK        (1 << 13)
#define LED_USB_LINK_FIODIR      FIO0DIR
#define LED_USB_LINK_FIOSET      FIO0SET
#define LED_USB_LINK_FIOCLR      FIO0CLR

//USB connect LED
#define LED_USB_CONNECT_MASK     (1 << 14)
#define LED_USB_CONNECT_FIODIR   FIO0DIR
#define LED_USB_CONNECT_FIOSET   FIO0SET
#define LED_USB_CONNECT_FIOCLR   FIO0CLR

//Button 1
#define BUTTON1_MASK             (1 << 29)
#define BUTTON1_FIODIR           FIO0DIR
#define BUTTON1_FIOPIN           FIO0PIN

//Button 2
#define BUTTON2_MASK             (1 << 18)
#define BUTTON2_FIODIR           FIO0DIR
#define BUTTON2_FIOPIN           FIO0PIN

//Joystick
#define JOYSTICK_UP_MASK         (1 << 18)
#define JOYSTICK_DOWN_MASK       (1 << 19)
#define JOYSTICK_RIGHT_MASK      (1 << 22)
#define JOYSTICK_CENTER_MASK     (1 << 25)
#define JOYSTICK_LEFT_MASK       (1 << 27)
#define JOYSTICK_FIODIR          FIO1DIR
#define JOYSTICK_FIOPIN          FIO1PIN

//LCD backlight
#define LCD_BL_MASK              (1 << 26)
#define LCD_BL_FIODIR            FIO1DIR
#define LCD_BL_FIOSET            FIO1SET
#define LCD_BL_FIOCLR            FIO1CLR

//LCD reset
#define LCD_RST_MASK             (1 << 25)
#define LCD_RST_FIODIR           FIO3DIR
#define LCD_RST_FIOSET           FIO3SET
#define LCD_RST_FIOCLR           FIO3CLR

//LCD CS
#define LCD_CS_MASK              (1 << 21)
#define LCD_CS_FIODIR            FIO1DIR
#define LCD_CS_FIOSET            FIO1SET
#define LCD_CS_FIOCLR            FIO1CLR

#endif
