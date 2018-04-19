/* Adapted for use with IAR Embedded Workbench */
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
* File Name     : lcd.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : LCD Module utility functions.
*                  Written for KS0066u compatible LCD Module.
*                  (8 characters by 2 lines)
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines RX63N port registers */
#include "iorx63n.h"
/* Defines RX63N LEDs and switches */
#include "rskrx63ndef.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Declare LCD nibble write function prototype */
static void LCD_nibble_write(uint8_t, uint8_t);

/*******************************************************************************
* Outline       : Init_LCD
* Description   : Initialises the LCD display. 
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_LCD(void)
{   
    /* Power-up Delay for LCD Module */
    E_PIN = SET_BIT_HIGH;

    /* Add delay */
    Delay_us(14000u);

    /* Set the enable pin low */
    E_PIN = SET_BIT_LOW;

    /* Display initialises in 8 bit mode - so send one write (seen as 8 bit)
    to set to 4 bit mode. */

    /* Function Set */
    LCD_nibble_write(CTRL_WR,0x03);
    LCD_nibble_write(CTRL_WR,0x03);

    /* Add delay */
    Delay_us(8000u);

    /* Configure display */
    LCD_nibble_write(CTRL_WR,0x03);
    LCD_nibble_write(CTRL_WR,0x02);
    LCD_nibble_write(CTRL_WR,(LCD_DISPLAY_ON | LCD_TWO_LINE ));
    LCD_nibble_write(CTRL_WR,(LCD_DISPLAY_ON | LCD_TWO_LINE ));

    /* Add delay */
    Delay_us(8000u);

    /* Display ON/OFF control */
    LCD_write(CTRL_WR,LCD_CURSOR_OFF);
    Delay_us(8000u);

    /* Display Clear */
    LCD_write(CTRL_WR,LCD_CLEAR);
    Delay_us(30600u);

    /* Entry Mode Set */
    LCD_write(CTRL_WR,0x06);
    LCD_write(CTRL_WR,LCD_HOME_L1);
    Delay_us(10000u);
}
/*******************************************************************************
* End of function Init_LCD
*******************************************************************************/

/******************************************************************************
* Outline       : Display_LCD
* Description   : This function controls LCD writes to line 1 or 2 of the LCD.
*                 You need to use the defines LCD_LINE1 and LCD_LINE2 in order
*                  to specify the starting position.
*                  For example, to start at the 2nd position on line 1...
*                           Display_LCD(LCD_LINE1 + 1, "Hello") 
* Argument      : (uint8_t)position -    Line number of display 
*                  (const uint8_t *) string -Pointer to data to be written to
*                                    display. Last character should be null.
* Return value  : none
*******************************************************************************/
void Display_LCD(uint8_t position,  uint8_t* string)
{
    /* Declare static local variable to store next position */
    static uint8_t next_pos = 0xFF;

    /* Set a line position if needed. If current cursor position is outside of
    the NUMB_CHARS_PER_LINE value, the current data writes are skipped.
    If data is written byte to byte, no CTRL writes are written to LCD module
    (CTRL write takes more time) */
    if (next_pos != position)
    {
        /* Check if the next write should be 
           placed on the second line of the LCD */
        if(position < LCD_LINE2)
        {
            /* Configure LCD to write on Line 1 */
            LCD_write(CTRL_WR, (uint8_t)(LCD_HOME_L1 + position));
        }
        else
        {
            /* Configure LCD to write on Line 2 */
            LCD_write(CTRL_WR, (uint8_t)((LCD_HOME_L2 + position) - LCD_LINE2));
        }
    
        /* Set position index to known value */
        next_pos = position;
    }

    do
    {
        /* Write the display data to the LCD */
        LCD_write(DATA_WR,*string++);

        /* Add a delay */
        Delay_us(45);

        /* Increment position index */
        next_pos++;
    }
    while (*string);
}
/*******************************************************************************
* End of function Display_LCD
*******************************************************************************/

/*******************************************************************************
* Outline       : LCD_write
* Description   : This function controls LCD writes to line 1 or 2 of the LCD.
*                 You need to use the defines LCD_LINE1 and LCD_LINE2 in order
*                  to specify the starting position.
*                  For example, to start at the 2nd position on line 1...
*                           DisplayString(LCD_LINE1 + 1, "Hello") 
* Argument      : (uint8_t)value - the value to write
*                  (uint8_t) data_or_ctrl - To write value 
*                    as DATA or CONTROL
*                        1 = DATA
*                        0 = CONTROL
* Return value  : none
*******************************************************************************/
void LCD_write(uint8_t data_or_ctrl, uint8_t value)
{
    /* Write upper nibble first */
    LCD_nibble_write(data_or_ctrl, (uint8_t)((value & 0xF0) >> 4));
    
    /* Write lower nibble second */
    LCD_nibble_write(data_or_ctrl, (uint8_t)(value & 0x0F));
}
/*******************************************************************************
* End of function LCD_write
*******************************************************************************/

/******************************************************************************
* Outline       : LCD_nibble_write
* Description   : Writes data to display. Sends command to display. 
* Argument          : (uint8_t) value - the value to write
*                  (uint8_t) data_or_ctrl - To write value  
*                    as DATA or CONTROL
*                        1 = DATA
*                        0 = CONTROL
* Return value  : None
*******************************************************************************/
static void LCD_nibble_write(uint8_t data_or_ctrl, uint8_t value)
{
    uint8_t ucStore = 0x00;

    /* Check for the type of data and set RS_PIN */
    if (data_or_ctrl == DATA_WR)
    {
        RS_PIN = SET_BIT_HIGH;
    }
    else
    {
        RS_PIN = SET_BIT_LOW;
    }

    /* There must be 40ns between RS write and EN write */
    Delay_us(50);
    
    /* EN enable chip (HIGH) */
    E_PIN = SET_BIT_HIGH;
    
    /* Tiny delay */
    Delay_us(250);

    /* Write upper 4 bits of nibble */
    LCD_DATA_PORT =  (uint8_t)((ucStore << 4) & 0xF0);

    /* Clear port bits used */    
    ucStore = LCD_DATA_PORT;
    ucStore &= (uint8_t)~DATA_PORT_MASK;
    
    /* OR in data */    
    ucStore |= (uint8_t)((value << DATA_PORT_SHIFT) & DATA_PORT_MASK );
    
    /* Write data to port */    
    LCD_DATA_PORT = ucStore;

    /* Write delay while En High */
    Delay_us(250);

    /* Latch data by dropping EN */
    E_PIN = SET_BIT_LOW;

    /* Write delay while EN High */
    Delay_us(250);

    /* Latch data by dropping EN */
    E_PIN = SET_BIT_LOW;
      
    /* Extra delay for write controls */
    Delay_us(250);
}
/******************************************************************************
* End of Function LCD_nibble_write
******************************************************************************/

/******************************************************************************
* Outline        : Delay_us
* Description    : Delay routine for LCD or any other devices.
* Argument       : (uint32_t) units - time in, approximately, microseconds
* Return value   : None
******************************************************************************/
void Delay_us(const uint32_t usec)
{
    /* Declare counter value variable based on usec input and DELAY_TIMING */
    volatile uint32_t counter = usec * DELAY_TIMING_US;
    
    /* Decrement the counter and wait */
    while (counter--)
    {
        ;
    }
}
/******************************************************************************
* End of Function Delay_us
******************************************************************************/

/******************************************************************************
* Outline        : Delay_s
* Description    : Delay routine for LCD or any other devices.
* Argument       : (uint8_t) units - time in, approximately, seconds
* Return value   : None
******************************************************************************/
void Delay_s(const uint8_t sec)
{
    /* Declare counter value variable based on usec input and DELAY_TIMING */
    volatile uint32_t counter = ((uint32_t)sec) * DELAY_TIMING_S;
    
    /* Decrement the counter and wait */
    while (counter--)
    {
        ;
    }
}
/******************************************************************************
* End of Function Delay_s
******************************************************************************/

/*******************************************************************************
* Outline      : u16ToString
* Description  : Function converts a 16 bit integer into a character string, 
*                 inserts it into the array via the pointer passed at execution.
* Argument     : * output_string : Pointer to uint8_t array that will hold 
*                                   character string.
*                             pos : uint8_t number, element number to begin 
*                                   inserting the character string from (offset).
*                    input_number : 16 bit integer to convert into a string.
* Return value : none
* Note         : No input validation is used, so output data can overflow the
*                 array passed.
*******************************************************************************/
void u16ToString(uint8_t *output_string, uint8_t pos, uint16_t input_number)
{    
    /* Declare bit_shift variable */    
    uint8_t bit_shift = 12u;
    
    /* Declare loop iteration variable */
    uint8_t i = (uint8_t)((bit_shift/4u) + 1u);
    
    /* Declare 16bit mask variable */
    uint16_t mask = 0xF000;
    
    /* Loop through until each hex digit is converted to an ASCII character */
    while(i)
    {            
        /* Mask and shift the hex digit, and store in temporary variable, a */ 
        uint8_t a = (uint8_t)((input_number & mask) >> bit_shift);
        
        /* Convert the hex digit into an ASCII character, and store in output
           string */
        output_string[pos] = (uint8_t)((a < 0x0A) ? (a + 0x30) : (a + 0x37));
        
        /* Shift the bit mask 4 bits to the right, to convert the next digit */
        mask = (uint16_t) (mask >> 4u);
        
        /* Decrement the bit_shift counter by 4 (bits in a each digit) */
        bit_shift -= 4u;
        
        /* Increment the output string location */
        pos++;
        
        /* Decrement the iteration counter */
        i--;
    }
}
/*******************************************************************************
* End of function u16ToString
*******************************************************************************/

/******************************************************************************
* Outline      : u32ToString
* Description  : Function converts a 32 bit integer into a character string, 
*                 inserts it into the array via the pointer passed at execution.
* Argument     : * output_string : Pointer to uint8_t array that will hold 
*                                   character string.
*                             pos : uint8_t number, element number to begin 
*                                   inserting the character string from (offset).
*                    input_number : 32 bit integer to convert into a string.
* Return value : none
* Note         : No input validation is used, so output data can overflow the
*                 array passed.
******************************************************************************/
void u32ToString(uint8_t* output_string, uint8_t pos, uint32_t input_number)
{
    /* Declare temporary charcter storage variable, and bit_shift variable */    
    uint8_t a = 0x00, bit_shift = 28u;
    
    /* Declare 32bit mask variable */
    uint32_t mask = 0xF0000000;
    
    /* Loop through until each hex digit is converted to an ASCII character */
    while(bit_shift < 30u)
    {
        /* Mask and shift the hex digit, and store in temporary variable, a */ 
        a = (uint8_t)((input_number & mask) >> bit_shift);
        
        /* Convert the hex digit into an ASCII character, and store in output
           string */
        output_string[pos] = (uint8_t)((a < 0x0A) ? (a + 0x30) : (a + 0x37));
        
        /* Shift the bit mask 4 bits to the right, to convert the next digit */
        mask =(uint32_t)( mask >> 4u);
        
        /* Decrement the bit_shift counter by 4 (bits in a each digit) */
        bit_shift -= (uint8_t)(4u);
        
        /* Increment the output string location */
        pos++;
    }
}
/******************************************************************************
* End of function u32ToString
******************************************************************************/
