/* Adapted for use with IAR Embedded Workbench */

/*******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. 

* No other uses are authorized.
*
* This software is owned by Renesas Electronics Corp. and is protected under
* all applicable laws, including copyright laws.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
*
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/******************************************************************************
* File Name     : lcd.h
* Version       : 1.0
* Device(s)     : R5F563N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : Provides variable and function declarations for LCD.C file
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 22.Sep.2011     1.00        First release
*******************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef LCD_H
#define LCD_H

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines standard integer variable types used in this file */
#include <stdint.h>

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
#define LCD_LINE1       0
#define LCD_LINE2       1
#define LCD_LINE3       2
#define LCD_LINE4       3
#define LCD_LINE5       4
#define LCD_LINE6       5
#define LCD_LINE7       6
#define LCD_LINE8       7

/******************************************************************************
* Global Function Prototypes
******************************************************************************/
/* LCD initialisation function declaration */
void lcd_initialize (void);

/* Update display function declaration */
void lcd_display(uint8_t x, uint8_t y, const uint8_t * string);

/* Clear LCD function delcaration */
void lcd_clear (void);

/* End of multiple inclusion prevention macro */
#endif
