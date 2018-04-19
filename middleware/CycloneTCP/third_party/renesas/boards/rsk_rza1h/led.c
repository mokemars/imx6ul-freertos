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
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : led.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Sample Program - LED setting
*               : Note
*               : LED0 is connected directly to processor
*               : LED1, LED2, LED3 are connected via port expander 1
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Default  type definition header */
#include "r_typedefs.h"
/* I/O Register root header */
#include "iodefine.h"
/* Device driver header */
#include "dev_drv.h"
/* Interface for directly connected LED header */
#include "led.h"
/* I2C RSK+RZA1H Eval-board header */
#include "riic_rskrza1h.h"
/* Low level register read/write header */
#include "rza_io_regrw.h"
/* Interchangeable compiler specific header */
#include "compiler_settings.h"


/******************************************************************************
Private global variables and functions
******************************************************************************/

/******************************************************************************
* Function Name: R_LED_Init
* Description  : Initialise port for LED.
* Arguments    : none
* Return Value : none
******************************************************************************/
void R_LED_Init (void)
{
    /* ---- P7_1 : LED0 direct connection to IP */
    rza_io_reg_write_16(&GPIO.PMC7,  0, GPIO_PMC7_PMC71_SHIFT,  GPIO_PMC7_PMC71);
    rza_io_reg_write_16(&GPIO.P7,  1, GPIO_P7_P71_SHIFT,  GPIO_P7_P71);
    rza_io_reg_write_16(&GPIO.PM7,  0, GPIO_PM7_PM71_SHIFT,  GPIO_PM7_PM71);
    rza_io_reg_write_16(&GPIO.PIPC7, 0, GPIO_PIPC7_PIPC71_SHIFT, GPIO_PIPC7_PIPC71);
}

/******************************************************************************
* End of Function R_LED_Init
******************************************************************************/

/*******************************************************************************
* Function Name: R_LED_On
* Description  : LED specified for "num" is turned on. 
* Arguments    : num desired LED to modify
* Return Value : none
*******************************************************************************/
void R_LED_On  (uint8_t num)
{
	if(LED0 == (num & LED0))
	{
        rza_io_reg_write_16(&GPIO.P7,  0, GPIO_P7_P71_SHIFT,  GPIO_P7_P71);
    }

	if((num & LED_PE) > 0)
	{
		/* Turn off the desired LED */
		Sample_R_RIIC_LED_PE_On(num & LED_PE);
    }
}

/*******************************************************************************
* Function Name: R_LED_Off
* Description  : LED specified for "num" is turned off. 
* Arguments    : num desired LED to modify
* Return Value : none
*******************************************************************************/
void R_LED_Off (uint8_t num)
{
	if(LED0 == (num & LED0))
	{
        rza_io_reg_write_16(&GPIO.P7,  1, GPIO_P7_P71_SHIFT,  GPIO_P7_P71);
    }

	if((num & LED_PE) > 0)
	{
		/* Turn on the desired port expander LED */
		Sample_R_RIIC_LED_PE_Off(num & LED_PE);
    }
}

/*******************************************************************************
* Function Name: R_LED_Toggle
* Description  : LED specified for "num" is toggled.
* Arguments    : num desired LED to modify
* Return Value : none
*******************************************************************************/
void R_LED_Toggle (uint8_t num)
{
	if(LED0 == (num & LED0))
	{
		uint16_t tmp = rza_io_reg_read_16(&GPIO.P7,
				                          GPIO_P7_P71_SHIFT,
				                          GPIO_P7_P71);

		if(tmp)
		{
			rza_io_reg_write_16(&GPIO.P7,  0, GPIO_P7_P71_SHIFT,  GPIO_P7_P71);
		}
		else
		{
			rza_io_reg_write_16(&GPIO.P7,  1, GPIO_P7_P71_SHIFT,  GPIO_P7_P71);
		}
    }

	if((num & LED_PE) > 0)
	{
		/* Toggle user LEDs LED1-LED3 on Port Expander 1 */
		Sample_R_RIIC_LED_PE_Toggle(num & LED_PE);
    }
}


/* End of File */

