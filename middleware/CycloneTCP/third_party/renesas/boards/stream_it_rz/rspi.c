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
* File Name     : rspi.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : This file implements device driver for RSPI module.
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
*******************************************************************************/


/******************************************************************************
Includes
******************************************************************************/
/* Default  type definition header */
#include "r_typedefs.h"
/* I/O Register root header */
#include "iodefine.h"
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
/* RSPI1 transmit buffer address */
uint32_t * g_prspi1_tx_address;

/* RSPI1 transmit data number */
uint16_t   g_rspi1_tx_count;

/* RSPI1 receive buffer address */
uint32_t * g_prspi1_rx_address;

/* RSPI1 receive data number */
uint16_t   g_rspi1_rx_count;

/* RSPI1 receive data length */
uint16_t   g_rspi1_rx_length;

/* SCI5 transmit buffer address */
uint8_t * g_pspi_tx_address;

/* SCI5 transmit data number */
uint16_t  g_spi_tx_count;

/*******************************************************************************
* Function Name: R_RSPI1_Create
* Description  : This function initialises the RSPI1 module.
* Arguments    : None
* Return Value : None
*******************************************************************************/
void R_RSPI1_Create (void)
{
	uint16_t dummy_word  = 0u;
	uint8_t  dummy_byte  = 0u;

    UNUSED_VARIABLE(dummy_word);
    UNUSED_VARIABLE(dummy_byte);

	/* Configure and disable SPI0 transmit interrupts */
	R_INTC_Disable(INTC_ID_SPTI4);
	R_INTC_Regist_Int_Func(INTC_ID_SPTI4, &R_RSPI1_TransmitInterrupt);
	R_INTC_Set_Priority(INTC_ID_SPTI4, ADC_PRI_SPTI1);

	RSPI1.SPPCR = 0u;
	dummy_byte = RSPI1.SPPCR;

    /* P1 clock = 66.67MHz, SPI bit rate = 11.11Mbits/s Check Table 16.3 */
    RSPI1.SPBR  = 2u;
	dummy_byte = RSPI1.SPBR;
    RSPI1.SPDCR = 0x20u;
    dummy_byte = RSPI1.SPDCR;
    RSPI1.SPSCR = 0u;
	dummy_byte = RSPI1.SPSCR;
    RSPI1.SPCKD = 0;
	dummy_byte = RSPI1.SPCKD;
    RSPI1.SSLND = 0u;
    dummy_byte =  RSPI1.SSLND;
    RSPI1.SPND  = 0u;
    dummy_byte = RSPI1.SPND;
    RSPI1.SSLP  = 0u;
    dummy_byte = RSPI1.SSLP;
    RSPI1.SPSSR  = 0u;
    dummy_byte = RSPI1.SPSSR;
    RSPI1.SPBFCR  |= 0x30u;
    dummy_byte = RSPI1.SPBFCR;

    /* LSB first, use 16 bit data length */
    RSPI1.SPCMD0 = 0x0703u;
    dummy_word = RSPI1.SPCMD0;

    /* Set P6_13 as CS pin for PMOD */
    rza_io_reg_write_16(&GPIO.PM6,   0, GPIO_PM6_PM613_SHIFT,    GPIO_PM6_PM613);
    rza_io_reg_write_16(&GPIO.PMC6,  0, GPIO_PMC6_PMC613_SHIFT,  GPIO_PMC6_PMC613);
    rza_io_reg_write_16(&GPIO.PFCAE6,0, GPIO_PFCAE6_PFCAE613_SHIFT,GPIO_PFCAE6_PFCAE613);
    rza_io_reg_write_16(&GPIO.PFCE6, 0, GPIO_PFCE6_PFCE613_SHIFT, GPIO_PFCE6_PFCE613);
    rza_io_reg_write_16(&GPIO.PFC6,  0, GPIO_PFC6_PFC613_SHIFT,  GPIO_PFC6_PFC613);
    rza_io_reg_write_16(&GPIO.PIPC6, 0, GPIO_PIPC6_PIPC613_SHIFT, GPIO_PIPC6_PIPC613);

    /* Set RSPCK1 as output pin (P6_12) */
    rza_io_reg_write_16(&GPIO.PM6,   0, GPIO_PM6_PM612_SHIFT,    GPIO_PM6_PM612);
    rza_io_reg_write_16(&GPIO.PMC6,  1, GPIO_PMC6_PMC612_SHIFT,  GPIO_PMC6_PMC612);
    rza_io_reg_write_16(&GPIO.PFCAE6,0, GPIO_PFCAE6_PFCAE612_SHIFT,GPIO_PFCAE6_PFCAE612);
    rza_io_reg_write_16(&GPIO.PFCE6, 1, GPIO_PFCE6_PFCE612_SHIFT, GPIO_PFCE6_PFCE612);
    rza_io_reg_write_16(&GPIO.PFC6,  0, GPIO_PFC6_PFC612_SHIFT,  GPIO_PFC6_PFC612);
    rza_io_reg_write_16(&GPIO.PIPC6, 1, GPIO_PIPC6_PIPC612_SHIFT, GPIO_PIPC6_PIPC612);

	/* Set MOSI1 as output pin (P6_14) */
    rza_io_reg_write_16(&GPIO.PM6,   0, GPIO_PM6_PM614_SHIFT,    GPIO_PM6_PM614);
    rza_io_reg_write_16(&GPIO.PMC6,  1, GPIO_PMC6_PMC614_SHIFT,  GPIO_PMC6_PMC614);
    rza_io_reg_write_16(&GPIO.PFCAE6,0, GPIO_PFCAE6_PFCAE614_SHIFT,GPIO_PFCAE6_PFCAE614);
    rza_io_reg_write_16(&GPIO.PFCE6, 1, GPIO_PFCE6_PFCE614_SHIFT, GPIO_PFCE6_PFCE614);
    rza_io_reg_write_16(&GPIO.PFC6,  0, GPIO_PFC6_PFC614_SHIFT,  GPIO_PFC6_PFC614);
    rza_io_reg_write_16(&GPIO.PIPC6, 1, GPIO_PIPC6_PIPC614_SHIFT, GPIO_PIPC6_PIPC614);

    /* Set MISO1 as input pin (P6_15) */
    rza_io_reg_write_16(&GPIO.PM6,   1, GPIO_PM6_PM615_SHIFT,    GPIO_PM6_PM615);
    rza_io_reg_write_16(&GPIO.PMC6,  1, GPIO_PMC6_PMC615_SHIFT,  GPIO_PMC6_PMC615);
    rza_io_reg_write_16(&GPIO.PFCAE6,0, GPIO_PFCAE6_PFCAE615_SHIFT,GPIO_PFCAE6_PFCAE615);
    rza_io_reg_write_16(&GPIO.PFCE6, 1, GPIO_PFCE6_PFCE615_SHIFT, GPIO_PFCE6_PFCE615);
    rza_io_reg_write_16(&GPIO.PFC6,  0, GPIO_PFC6_PFC615_SHIFT,  GPIO_PFC6_PFC615);
    rza_io_reg_write_16(&GPIO.PIPC6, 1, GPIO_PIPC6_PIPC615_SHIFT, GPIO_PIPC6_PIPC615);

	/* Set P1_9 as PMOD_IRQ pin (input) */
    //rza_io_reg_write_16(&GPIO.PM1,   1, GPIO_PM1_PM19_SHIFT,    GPIO_PM1_PM19);
    rza_io_reg_write_16(&GPIO.PMC1,  1, GPIO_PMC1_PMC19_SHIFT,  GPIO_PMC1_PMC19);
    rza_io_reg_write_16(&GPIO.PFCAE1,0, GPIO_PFCAE1_PFCAE19_SHIFT,GPIO_PFCAE1_PFCAE19);
    rza_io_reg_write_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE19_SHIFT, GPIO_PFCE1_PFCE19);
    rza_io_reg_write_16(&GPIO.PFC1,  1, GPIO_PFC1_PFC19_SHIFT,  GPIO_PFC1_PFC19);
    //rza_io_reg_write_16(&GPIO.PIPC1, 1, GPIO_PIPC1_PIPC19_SHIFT, GPIO_PIPC1_PIPC19);

	/* Set P9_5 as PMOD pin9 (output) */
    rza_io_reg_write_16(&GPIO.PM9,   0, GPIO_PM9_PM95_SHIFT,    GPIO_PM9_PM95);
    rza_io_reg_write_16(&GPIO.PMC9,  0, GPIO_PMC9_PMC95_SHIFT,  GPIO_PMC9_PMC95);
    rza_io_reg_write_16(&GPIO.PFCAE9,0, GPIO_PFCAE9_PFCAE95_SHIFT,GPIO_PFCAE9_PFCAE95);
    rza_io_reg_write_16(&GPIO.PFCE9, 0, GPIO_PFCE9_PFCE95_SHIFT, GPIO_PFCE9_PFCE95);
    rza_io_reg_write_16(&GPIO.PFC9,  0, GPIO_PFC9_PFC95_SHIFT,  GPIO_PFC9_PFC95);
    rza_io_reg_write_16(&GPIO.PIPC9, 0, GPIO_PIPC9_PIPC95_SHIFT, GPIO_PIPC9_PIPC95);

   	/* Enable master mode */
	RSPI1.SPCR |= 0x28u;
	dummy_byte = RSPI1.SPCR;
}

/*******************************************************************************
* Function Name: R_RSPI1_Start
* Description  : This function starts the RSPI1 module operation.
* Arguments    : None
* Return Value : None
*******************************************************************************/
void R_RSPI1_Start (void)
{
    volatile uint8_t dummy = 0u;

    UNUSED_VARIABLE(dummy);

    /* Enable RSPI interrupts */
	R_INTC_Enable(INTC_ID_SPTI4);
	R_INTC_Enable(INTC_ID_SPRI4);

    /* Clear error sources */
    dummy = RSPI1.SPSR;
    RSPI1.SPSR = 0x00U;

    if (0 == rza_io_reg_read_8( &(RSPI1.SPSR),
                               RSPIn_SPSR_MODF_SHIFT,
    		                   RSPIn_SPSR_MODF))
	{
		/* test */
		RSPI1.SPCR |= 0x40U;
		dummy = RSPI1.SPCR;
	}
	else
	{
		/* clear the MODF flag then set the SPE bit */
		dummy = RSPI1.SPSR;

		RSPI1.SPCR |= 0x40U;
		dummy = RSPI1.SPCR;
	}
}

/*******************************************************************************
* Function Name: R_RSPI1_Stop
* Description  : This function stops the RSPI1 module operation.
* Arguments    : None
* Return Value : None
*******************************************************************************/
void R_RSPI1_Stop (void)
{
    /* Disable RSPI interrupts */
	R_INTC_Disable(INTC_ID_SPTI4);

    /* Disable RSPI function */
	rza_io_reg_write_8( &(RSPI1.SPCR),
			           0,
			           RSPIn_SPCR_SPE_SHIFT,
			           RSPIn_SPCR_SPE);

}


/*******************************************************************************
* Function Name: R_RSPI1_SendReceive
* Description  : This function sends and receives CSI#n1 data.
* Arguments    : tx_buf -
*                 transfer buffer pointer (not used when data is handled by DTC)
*                tx_num -
*                    buffer size
*                rx_buf -
*                 receive buffer pointer (not used when data is handled by DTC)
* Return Value : status -
*                    MD_OK or MD_ARGERROR
*******************************************************************************/
uint8_t R_RSPI1_SendReceive (uint32_t * const tx_buf,
		                              uint16_t tx_num, uint32_t * const rx_buf)
{
	uint8_t status = 0u;

    if (tx_num < 1U)
    {
        status = 1u;
    }
    else
    {
		g_prspi1_tx_address = tx_buf;
        g_rspi1_tx_count = tx_num;

        g_prspi1_rx_address = rx_buf;
        g_rspi1_rx_length = tx_num;
        g_rspi1_rx_count = 0U;

        /* Enable transmit interrupt */
    	rza_io_reg_write_8( &(RSPI1.SPCR),
    			           1,
    			           RSPIn_SPCR_SPTIE_SHIFT,
    			           RSPIn_SPCR_SPTIE);

        /* Enable receive interrupt */
    	rza_io_reg_write_8( &(RSPI1.SPCR),
    			           1,
    			           RSPIn_SPCR_SPRIE_SHIFT,
    			           RSPIn_SPCR_SPRIE);

        /* Enable RSPI function */
    	rza_io_reg_write_8( &(RSPI1.SPCR),
    			           1,
    			           RSPIn_SPCR_SPE_SHIFT,
    			           RSPIn_SPCR_SPE);
    }

    return (status);
}

/*******************************************************************************
* Function Name: R_RSPI1_LoopBackReversed
* Description  : This function .
* Arguments    : None
* Return Value : None
*******************************************************************************/
void R_RSPI1_LoopBackReversed (void)
{
	rza_io_reg_write_8( &(RSPI1.SPPCR),
			           (uint8_t)1,
			           RSPIn_SPPCR_SPLP_SHIFT,
			           RSPIn_SPPCR_SPLP);
}

/*******************************************************************************
* Function Name: R_RSPI1_LoopBackDisable
* Description  : This function disable loop-back mode.
* Arguments    : None
* Return Value : None
*******************************************************************************/
void R_RSPI1_LoopBackDisable (void)
{
	rza_io_reg_write_8( &(RSPI1.SPPCR),
			           0,
			           RSPIn_SPPCR_SPLP_SHIFT,
			           RSPIn_SPPCR_SPLP);
}
