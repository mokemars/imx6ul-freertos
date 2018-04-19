/**
 * @file stm32_p407.h
 * @brief STM32-P407 demonstration board
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

#ifndef _STM32_P407_H
#define _STM32_P407_H

//Dependencies
#include "stm32f4xx.h"

//LED1
#define LED1_PIN  GPIO_Pin_6
#define LED1_GPIO GPIOF

//LED2
#define LED2_PIN  GPIO_Pin_7
#define LED2_GPIO GPIOF

//LED3
#define LED3_PIN  GPIO_Pin_8
#define LED3_GPIO GPIOF

//LED4
#define LED4_PIN  GPIO_Pin_9
#define LED4_GPIO GPIOF

//WKUP button
#define WKUP_PIN  GPIO_Pin_0
#define WKUP_GPIO GPIOA

//TAMPER button
#define TAMPER_PIN  GPIO_Pin_13
#define TAMPER_GPIO GPIOC

//Joystick
#define JOYSTICK_LEFT_PIN  GPIO_Pin_11
#define JOYSTICK_RIGHT_PIN GPIO_Pin_6
#define JOYSTICK_UP_PIN    GPIO_Pin_7
#define JOYSTICK_DOWN_PIN  GPIO_Pin_8
#define JOYSTICK_CENT_PIN  GPIO_Pin_15
#define JOYSTICK_GPIO      GPIOG

//LCD backlight
#define LCD_BL_PIN  GPIO_Pin_0
#define LCD_BL_GPIO GPIOB

//LCD RST
#define LCD_RST_PIN  GPIO_Pin_3
#define LCD_RST_GPIO GPIOD

//LCD CS
#define LCD_CS_PIN  GPIO_Pin_6
#define LCD_CS_GPIO GPIOD

//LCD SCK
#define LCD_SCK_PIN  GPIO_Pin_5
#define LCD_SCK_GPIO GPIOA

//LCD MOSI
#define LCD_MOSI_PIN  GPIO_Pin_3
#define LCD_MOSI_GPIO GPIOC

//LCD MISO
#define LCD_MISO_PIN  GPIO_Pin_4
#define LCD_MISO_GPIO GPIOB

#endif
