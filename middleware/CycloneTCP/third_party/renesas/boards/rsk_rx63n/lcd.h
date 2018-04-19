/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : lcd.h
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : This Header file contains the Macro Definitions & prototypes  
*                  for the functions used in lcd.c
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef LCD_H
#define LCD_H

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines standard variable types used in this file */
#include <stdint.h>
    
/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* RS register select pin */
#define RS_PIN                  PORTJ.PODR.BIT.B5
/* RS register select pin */
#define RS_PIN_DIR              PORTJ.PDR.BIT.B5
/* Display enable pin */    
#define E_PIN                   PORTF.PODR.BIT.B5
/* Display enable pin */    
#define E_PIN_DIR               PORTF.PDR.BIT.B5
/* Data bus port */
#define LCD_DATA_PORT           PORT8.PODR.BYTE
/* Data bus direction port */
#define LCD_DATA_DIR_PORT       PORT8.PDR.BYTE

/******************************************************************************
* Macro Definitions
******************************************************************************/
/* Bit mask from entire port */
#define DATA_PORT_MASK          (0xF0u)
/* Number of bits data needs to shift */    
#define DATA_PORT_SHIFT         (4u)
/* Data write definition */
#define DATA_WR                 (1u)
/* Control write definition */
#define CTRL_WR                 (0)

/* Set to ensure base delay */
#define DELAY_TIMING_US         (0x000B)
#define DELAY_TIMING_S          (0x00840000)
/* Maximum characters per line of LCD display. */
#define NUMB_CHARS_PER_LINE     (8u)
/* Number of lines on the LCD display */
#define MAXIMUM_LINES           (2u)
/* Starting position for line 1 */
#define LCD_LINE1               (0)
/* Starting position for line 2 */
#define LCD_LINE2               (16u)

/*****************************************************************************
* LCD commands - use LCD_write function to write these commands to the LCD. 
*****************************************************************************/
/* Clear LCD display and home cursor */
#define LCD_CLEAR               (0x01u)
/* Move cursor to line 1 */
#define LCD_HOME_L1             (0x80u)
/* Move cursor to line 2 */
#define LCD_HOME_L2             (0xC0u)
/* Cursor auto decrement after R/W */
#define CURSOR_MODE_DEC         (0x04u)
/* Cursor auto increment after R/W */
#define CURSOR_MODE_INC         (0x06u)
/* Setup, 4 bits, 2 lines, 5X7 */
#define FUNCTION_SET            (0x28u)
/* Display on with cursor */
#define LCD_CURSOR_ON           (0x0Eu)
/* Display on with Cursor off */
#define LCD_CURSOR_OFF          (0x0Cu)
/* Display on with blinking cursor */
#define LCD_CURSOR_BLINK        (0x0Du)
/* Move cursor left one position */
#define LCD_CURSOR_LEFT         (0x10u)
/* Move cursor right one position */
#define LCD_CURSOR_RIGHT        (0x14u)
/* Turn ON the display */
#define LCD_DISPLAY_ON          (0x04u)
/* Initialise 2 line display */
#define LCD_TWO_LINE            (0x08u)

/******************************************************************************
* Function Prototypes
******************************************************************************/
/* Initialises the debug LCD */
void Init_LCD(void);
/* This function writes to line 1 or 2 of the LCD */
void Display_LCD(uint8_t, uint8_t*);
/* Writes data to display or sends command to display */
void LCD_write(uint8_t, uint8_t);
/* Converts 16bit integer into a string */
void u16ToString(uint8_t *, uint8_t, uint16_t);
/* Converts 32bit integer into a string */
void u32ToString(uint8_t *, uint8_t, uint32_t);
/* Delay routine for LCD or any other devices */
void Delay_us(const uint32_t);
/* Delay routine for LCD or any other devices */
void Delay_s(const uint8_t);

/* End of multiple inclusion prevention macro */
#endif
