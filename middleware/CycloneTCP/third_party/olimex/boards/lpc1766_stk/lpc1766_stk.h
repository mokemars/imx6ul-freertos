/**
 * @file lpc1766_stk.h
 * @brief LPC1766-STK demonstration board
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

#ifndef _LPC1766_STK_H
#define _LPC1766_STK_H

//Dependencies
#include "lpc17xx.h"

//LED 1
#define LED1_MASK                (1 << 25)
#define LED1_FIODIR              LPC_GPIO1->FIODIR
#define LED1_FIOSET              LPC_GPIO1->FIOSET
#define LED1_FIOCLR              LPC_GPIO1->FIOCLR

//LED 2
#define LED2_MASK                (1 << 4)
#define LED2_FIODIR              LPC_GPIO0->FIODIR
#define LED2_FIOSET              LPC_GPIO0->FIOSET
#define LED2_FIOCLR              LPC_GPIO0->FIOCLR

//USB link LED
#define LED_USB_LINK_MASK        (1 << 18)
#define LED_USB_LINK_FIODIR      LPC_GPIO1->FIODIR
#define LED_USB_LINK_FIOSET      LPC_GPIO1->FIOSET
#define LED_USB_LINK_FIOCLR      LPC_GPIO1->FIOCLR

//USB connect LED
#define LED_USB_CONNECT_MASK     (1 << 9)
#define LED_USB_CONNECT_FIODIR   LPC_GPIO2->FIODIR
#define LED_USB_CONNECT_FIOSET   LPC_GPIO2->FIOSET
#define LED_USB_CONNECT_FIOCLR   LPC_GPIO2->FIOCLR

//Button 1
#define BUTTON1_MASK             (1 << 23)
#define BUTTON1_FIODIR           LPC_GPIO0->FIODIR
#define BUTTON1_FIOPIN           LPC_GPIO0->FIOPIN

//Button 2
#define BUTTON2_MASK             (1 << 13)
#define BUTTON2_FIODIR           LPC_GPIO2->FIODIR
#define BUTTON2_FIOPIN           LPC_GPIO2->FIOPIN

//Wake-up button
#define BUTTON_WUP_MASK          (1 << 12)
#define BUTTON_WUP_FIODIR        LPC_GPIO2->FIODIR
#define BUTTON_WUP_FIOPIN        LPC_GPIO2->FIOPIN

//Joystick
#define JOYSTICK_UP_MASK         (1 << 0)
#define JOYSTICK_DOWN_MASK       (1 << 1)
#define JOYSTICK_LEFT_MASK       (1 << 7)
#define JOYSTICK_RIGHT_MASK      (1 << 8)
#define JOYSTICK_FIODIR          LPC_GPIO2->FIODIR
#define JOYSTICK_FIOPIN          LPC_GPIO2->FIOPIN

#define JOYSTICK_CENTER_MASK     (1 << 5)
#define JOYSTICK_CENTER_FDIR     LPC_GPIO0->FIODIR
#define JOYSTICK_CENTER_FIOPIN   LPC_GPIO0->FIOPIN

//LCD backlight
#define LCD_BL_MASK              (1 << 26)
#define LCD_BL_FIODIR            LPC_GPIO3->FIODIR
#define LCD_BL_FIOSET            LPC_GPIO3->FIOSET
#define LCD_BL_FIOCLR            LPC_GPIO3->FIOCLR

//LCD reset
#define LCD_RST_MASK             (1 << 25)
#define LCD_RST_FIODIR           LPC_GPIO3->FIODIR
#define LCD_RST_FIOSET           LPC_GPIO3->FIOSET
#define LCD_RST_FIOCLR           LPC_GPIO3->FIOCLR

//LCD CS
#define LCD_CS_MASK              (1 << 21)
#define LCD_CS_FIODIR            LPC_GPIO1->FIODIR
#define LCD_CS_FIOSET            LPC_GPIO1->FIOSET
#define LCD_CS_FIOCLR            LPC_GPIO1->FIOCLR

//SD card presence
#define SD_PRES_MASK             (1 << 28)
#define SD_PRES_FIODIR           LPC_GPIO1->FIODIR
#define SD_PRES_FIOPIN           LPC_GPIO1->FIOPIN

//SD card write protect
#define SD_WP_MASK               (1 << 29)
#define SD_WP_FIODIR             LPC_GPIO1->FIODIR
#define SD_WP_FIOPIN             LPC_GPIO1->FIOPIN

//SD card power
#define SD_PWR_MASK              (1 << 21)
#define SD_PWR_FIODIR            LPC_GPIO0->FIODIR
#define SD_PWR_FIOSET            LPC_GPIO0->FIOSET
#define SD_PWR_FIOCLR            LPC_GPIO0->FIOCLR

//SD card CS
#define SD_CS_MASK               (1 << 6)
#define SD_CS_FIODIR             LPC_GPIO0->FIODIR
#define SD_CS_FIOSET             LPC_GPIO0->FIOSET
#define SD_CS_FIOCLR             LPC_GPIO0->FIOCLR

#endif
