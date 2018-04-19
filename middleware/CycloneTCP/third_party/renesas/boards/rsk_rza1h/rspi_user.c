/******************************************************************************
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
******************************************************************************/
/******************************************************************************
* File Name     : rspi_user.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : This file implements user driver for RSPI module.
******************************************************************************/
/******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
/* Default  type definition header */
#include "r_typedefs.h"
/* I/O Register root header */
#include "iodefine.h"
/* Device Driver common header */
#include "dev_drv.h"
/* INTC Driver Header */
#include "intc.h"
/* Device driver for RSPI header */
#include "rspi.h"
/* Low level register read/write header */
#include "rza_io_regrw.h"
/* Interchangeable compiler specific header */
#include "compiler_settings.h"

/******************************************************************************
Global variables and functions
******************************************************************************/

/* Stores the data to be transmitted */
volatile uint32_t g_tx_data;
volatile uint32_t g_trans_data;

void uint16_to_string (uint8_t * const output_string, uint8_t pos,
                                                const uint16_t input_number);

/* RSPI1 transmit buffer address */
extern uint32_t * g_prspi1_tx_address;

/* RSPI1 transmit data number */
extern uint16_t   g_rspi1_tx_count;

/* RSPI1 receive buffer address */
extern uint32_t * g_prspi1_rx_address;

/* RSPI1 receive data number */
extern uint16_t   g_rspi1_rx_count;

/* RSPI1 receive data length */
extern uint16_t   g_rspi1_rx_length;

/* Declare storage variable for received data */
volatile uint32_t g_rx_data = 0;


/******************************************************************************
* Function Name: R_RSPI1_TransmitInterrupt
* Description  : None
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : None
******************************************************************************/
void R_RSPI1_TransmitInterrupt (uint32_t int_sense)
{
	UNUSED_PARAM(int_sense);

    if (g_rspi1_tx_count > 0U)
    {
        g_prspi1_tx_address++;
        g_rspi1_tx_count--;
    }
}

/******************************************************************************
* Function Name: R_RSPI1_ReceiveInterrupt
* Description  : None
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : None
******************************************************************************/
void R_RSPI1_ReceiveInterrupt (uint32_t int_sense)
{
	UNUSED_PARAM(int_sense);

    if (g_rspi1_rx_length > g_rspi1_rx_count)
    {
        (*(uint16_t *)g_prspi1_rx_address) =
        		                       (uint16_t)(RSPI1.SPDR.UINT16[0] >> 16u);
        g_prspi1_rx_address++;
        g_rspi1_rx_count++;

        if (g_rspi1_rx_length == g_rspi1_rx_count)
        {
            /* Disable receive interrupt */
        	rza_io_reg_write_8( &(RSPI1.SPCR),
        			           0,
        			           RSPIn_SPCR_SPRIE_SHIFT,
        			           RSPIn_SPCR_SPRIE);

            R_RSPI1_CallBackReceiveEnd();
        }
    }
}

/******************************************************************************
* Function Name: R_RSPI1_ErrorInterrupt
* Description  : None
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : None
******************************************************************************/
void R_RSPI1_ErrorInterrupt (uint32_t int_sense)
{
    uint8_t err_type;

	UNUSED_PARAM(int_sense);

    /* Disable RSPI function */
	rza_io_reg_write_8( &(RSPI1.SPCR),
			           0,
			           RSPIn_SPCR_SPE_SHIFT,
			           RSPIn_SPCR_SPE);

    /* Disable transmit interrupt */
	rza_io_reg_write_8( &(RSPI1.SPCR),
			           0,
			           RSPIn_SPCR_SPTIE_SHIFT,
			           RSPIn_SPCR_SPTIE);

    /* Disable receive interrupt */
	rza_io_reg_write_8( &(RSPI1.SPCR),
			           0,
			           RSPIn_SPCR_SPRIE_SHIFT,
			           RSPIn_SPCR_SPRIE);

    /* Disable error interrupt */
	rza_io_reg_write_8( &(RSPI1.SPCR),
			           0,
			           RSPIn_SPCR_SPEIE_SHIFT,
			           RSPIn_SPCR_SPEIE);

    /* Clear error sources */
    err_type = RSPI1.SPSR;
    RSPI1.SPSR = 0xA0U;

    if (0u != err_type)
    {
        R_RSPI1_CallBackError(err_type);
    }
}

/******************************************************************************
* Function Name: R_RSPI1_IdleInterrupt
* Description  : None
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    :   INTC_LEVEL_SENSITIVE : Level sense
*              :                    :   INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : None
******************************************************************************/
void R_RSPI1_IdleInterrupt (uint32_t int_sense)
{
	UNUSED_PARAM(int_sense);

    /* Disable RSPI function */
	rza_io_reg_write_8( &(RSPI1.SPCR),
			           0,
			           RSPIn_SPCR_SPE_SHIFT,
			           RSPIn_SPCR_SPE);

    R_RSPI1_CallBackTransmitEnd();
}

/******************************************************************************
* Function Name: r_rspi1_callback_transmit_end
* Description  : This function is a callback_ function when RSPI0 finishes
*                transmission.
* Arguments    : None
* Return Value : None
******************************************************************************/
void R_RSPI1_CallBackTransmitEnd (void)
{
	/* Do nothing */
}

/******************************************************************************
* Function Name: R_RSPI1_CallBackReceiveEnd
* Description  : This function is a callback_ function when RSPI0 finishes
*                reception.
* Arguments    : None
* Return Value : None
******************************************************************************/
void R_RSPI1_CallBackReceiveEnd (void)
{
	/* Do nothing */
}

/******************************************************************************
* Function Name: r_rspi1_callback__error
* Description  : This function is a callback_ function when RSPI0 error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
******************************************************************************/
void R_RSPI1_CallBackError (uint8_t err_type)
{
	/* Do nothing */
}

/******************************************************************************
* Function Name : R_SPI_Init
* Description   : Initialises the SPI functions for this sample
* Argument      : none
* Return value  : none
******************************************************************************/
void R_SPI_Init (void)
{
	/* Initialise the rSPI */
	R_RSPI1_Create();

    /* start rSPI */
    R_RSPI1_Start();
}

/******************************************************************************
* End of function R_SPI_Init
******************************************************************************/

/*******************************************************************************
* Function Name: uint16_to_string
* Description  : Function converts a 16 bit integer into a character string,
*                inserts it into the array via the pointer passed at execution.
* Arguments    : (char *)output_string        - Pointer to char array that will
*                                               hold character string.
*                (uint8_t)pos                 - uint8_t number, element number
*                                               to begin inserting the character
*                                               string from (offset).
*                (const uint16_t)input_number - 16 bit integer to convert into
*                                               a string.
* Return Value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
*******************************************************************************/
void uint16_to_string (uint8_t * const output_string, uint8_t pos,
                                                   const uint16_t input_number)
{
    /* Declare temporary character storage variable, and bit_shift variable */
    uint8_t  a = 0x00;

    /* Number of bits in 3 hex characters */
    uint8_t  bit_shift = 12u;

    /* Declare 16bit mask variable */
    uint16_t mask = 0xF000u;

    /* Loop through until each hex digit is converted to an ASCII character */
    while (30u > bit_shift)
    {
        /* Mask and shift the hex digit, and store in temporary variable, a */
        a = (uint8_t)((input_number & mask) >> bit_shift);

        /* Convert the hex digit into an ASCII character, and store in output
           string */
        output_string[pos] = (uint8_t)((a < 0x0Au) ? (a + 0x30u) : (a + 0x37u));

        /* Shift the bit mask 4 bits to the right, to convert the next digit */
        mask = (uint16_t) (mask >> 4u);

        /* Decrement the bit_shift counter by 4 (bits in each digit) */
        bit_shift = (uint8_t)(bit_shift - 4u);

        /* Increment the output string location */
        pos++;
    }
}

/*******************************************************************************
* End of function uint16_to_string
*******************************************************************************/


/* End of file */
