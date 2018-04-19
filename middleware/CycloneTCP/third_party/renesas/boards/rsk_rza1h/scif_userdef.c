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
* File Name     : scif_userdef.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : SCIF UART driver (User define function)
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
********************************************************************************/
/* Standard I/O definitions header */
#include <stdio.h>
#include "r_typedefs.h"
/* Device Driver common header */
#include "dev_drv.h"
/* UART Driver header */
#include "scif.h"
/* INTC Driver Header */
#include "intc.h"
/* I/O register definitions */
#include "iodefine.h"
/* Low level register read/write header */
#include "rza_io_regrw.h"


/*******************************************************************************
* Function Name: Userdef_SCIF2_UART_Init
* Description  : Executes the setting for initialisation of the UART function
*              : of channel 2 SCIF. Specifies the mode for transmission,
*              : reception, or transmission/reception to the argument mode.
*              : The values to be set to the CKS bit of the serial mode register
*              : (SCSMR) and the bit rate register (SCBRR) are specified to the
*              : argument csk and scbrr according to the UAART baud rate used
*              : in the system.
* Arguments    : uint8_t  mode    : Mode : 
*              :                  :    B'x..xxx
*              :                  :      |  ||+-- [0] 1=Use transmit,
*              :                  :                       0=Do not use transmit
*              :                  :      |  |+--- [1] 1=Use receive,
*              :                  :                        0=Do not use receive
*              :                  :      |  +---- [2] (Reserved)
*              :                  :      |         :     :  
*              :                  :      +------- [7] (Reserved)
*              : uint16_t cks     : Select SCIF clock source
*              :                  : (Setting value of SCSMR CKS bit)
*              :                  :   SCIF_CKS_DIVISION_1  : P1 clock
*              :                  :   SCIF_CKS_DIVISION_4  : P1 clock / 4
*              :                  :   SCIF_CKS_DIVISION_16 : P1 clock / 16
*              :                  :   SCIF_CKS_DIVISION_64 : P1 clock / 64
*              : uint8_t  scbrr   : Setting value of SCIF bit rate (SCBRR)
* Return Value : none
*******************************************************************************/
void Userdef_SCIF2_UART_Init (uint8_t mode, uint16_t cks, uint8_t scbrr)
{
    volatile uint8_t dummy_buf = 0u;

    /* Used to suppress the variable declared but unused warning */
    UNUSED_VARIABLE(dummy_buf);

    /* Module standby clear */
    /* Supply clock to the SCIF(channel 2) */
    rza_io_reg_write_8((uint8_t *)&CPG.STBCR4,
    		          0,
    		          CPG_STBCR4_MSTP45_SHIFT,
    		          CPG_STBCR4_MSTP45);

    /* Dummy read */
    dummy_buf = CPG.STBCR4;

    /* SCIF initial setting */
    /* Serial control register (SCSCR2) setting */
    /* SCIF transmitting and receiving operations stop */
    SCIF2.SCSCR = 0x0000u;

    /* FIFO control register (SCFCR2) setting */
    if (SCIF_UART_MODE_W == (mode & SCIF_UART_MODE_W))
    {
        /* Transmit FIFO reset */
        rza_io_reg_write_16((uint16_t *)&SCIF2.SCFCR,
        		           1,
        		           SCIF2_SCFCR_TFRST_SHIFT,
        		           SCIF2_SCFCR_TFRST);
    }

    if (SCIF_UART_MODE_R == (mode & SCIF_UART_MODE_R))
    {
        /* Receive FIFO data register reset */
        rza_io_reg_write_16((uint16_t *)&SCIF2.SCFCR,
        		            1,
        		            SCIF2_SCFCR_RFRST_SHIFT,
        		            SCIF2_SCFCR_RFRST);
    }

    /* Serial status register (SCFSR2) setting */
    /* ER,BRK,DR bit clear */
    SCIF2.SCFSR &= 0xFF6Eu;

    /* Line status register (SCLSR2) setting */
    /* ORER bit clear */
    rza_io_reg_write_16((uint16_t *)&SCIF2.SCLSR,
    		           0,
    		           SCIF2_SCLSR_ORER_SHIFT,
    		           SCIF2_SCLSR_ORER);

    /* Serial control register (SCSCR2) setting */
    /* B'00 : Internal CLK */
    rza_io_reg_write_16((uint16_t *)&SCIF2.SCSCR,
    		           0,
    		           SCIF2_SCSCR_CKE_SHIFT,
    		           SCIF2_SCSCR_CKE);

    /* Serial mode register (SCSMR2) setting
    b7    C/A  - Communication mode : Asynchronous mode
    b6    CHR  - Character length   : 8-bit data
    b5    PE   - Parity enable      : Add and check are disabled
    b3    STOP - Stop bit length    : 1 stop bit
    b1:b0 CKS  - Clock select       : cks(argument) */
    SCIF2.SCSMR = (cks & 0x0003u);

    /* Serial extension mode register (SCEMR2) setting
    b7 BGDM - Baud rate generator double-speed mode  : Normal mode
    b0 ABCS - Base clock select in asynchronous mode : Base clock is 16
    times the bit rate */
    SCIF2.SCEMR = 0x0000u;

    /* Bit rate register (SCBRR2) setting */
    SCIF2.SCBRR = scbrr;

    /* FIFO control register (SCFCR2) setting */
    if (SCIF_UART_MODE_RW == mode)
    {
	   /*
	    b10:b8 RSTRG - RTS output active trigger         : Initial value
	    b7:b6  RTRG  - Receive FIFO data trigger         : 1-data
	    b5:b4  TTRG  - Transmit FIFO data trigger        : 0-data
	    b3     MCE   - Modem control enable              : Disabled
	    b2     TFRST - Transmit FIFO data register reset : Disabled
	    b1     RFRST - Receive FIFO data register reset  : Disabled
	    b0     LOOP  - Loop-back test                    : Disabled */
        SCIF2.SCFCR = 0x0030u;
    }
    else
    {
        if (SCIF_UART_MODE_W == (mode & SCIF_UART_MODE_W))
        {
		   /*
		    b10:b8 RSTRG - RTS output active trigger         : Initial value
		    b7:b6  RTRG  - Receive FIFO data trigger         : 1-data
		    b5:b4  TTRG  - Transmit FIFO data trigger        : 0-data
		    b3     MCE   - Modem control enable              : Disabled
		    b2     TFRST - Transmit FIFO data register reset : Disabled
		    b1     RFRST - Receive FIFO data register reset  : Enabled
		    b0     LOOP  - Loop-back test                    : Disabled */
            SCIF2.SCFCR = 0x0032u;
        }
        else if (SCIF_UART_MODE_R == (mode & SCIF_UART_MODE_R))
        {
           /*
		    b10:b8 RSTRG - RTS output active trigger         : Initial value
		    b7:b6  RTRG  - Receive FIFO data trigger         : 1-data
		    b5:b4  TTRG  - Transmit FIFO data trigger        : 0-data
		    b3     MCE   - Modem control enable              : Disabled
		    b2     TFRST - Transmit FIFO data register reset : Enabled
		    b1     RFRST - Receive FIFO data register reset  : Disabled
		    b0     LOOP  - Loop-back test                    : Disabled */
            SCIF2.SCFCR = 0x0034u;
        }
        else
        {
           /*
		    b10:b8 RSTRG - RTS output active trigger         : Initial value
		    b7:b6  RTRG  - Receive FIFO data trigger         : 1-data
		    b5:b4  TTRG  - Transmit FIFO data trigger        : 0-data
		    b3     MCE   - Modem control enable              : Disabled
		    b2     TFRST - Transmit FIFO data register reset : Enabled
		    b1     RFRST - Receive FIFO data register reset  : Enabled
		    b0     LOOP  - Loop-back test                    : Disabled */
            SCIF2.SCFCR = 0x0036u;
        }
    }

    /* Serial port register (SCSPTR2) setting
    b1 SPB2IO - Serial port break output : Enabled
    b0 SPB2DT - Serial port break data   : High-level */
    SCIF2.SCSPTR |= 0x0003u;
}


/* End of File */

