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
* File Name     : scif.h
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Sample Program - SCIF UART device driver header
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
*******************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef SCIF_H
#define SCIF_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "iodefine.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define SCIF_UART_MODE_W        (1)
#define SCIF_UART_MODE_R        (2)
#define SCIF_UART_MODE_RW       (3)

typedef enum scif_cks_division
{
    SCIF_CKS_DIVISION_1,
    SCIF_CKS_DIVISION_4,
    SCIF_CKS_DIVISION_16,
    SCIF_CKS_DIVISION_64
} scif_cks_division_t;

/*******************************************************************************
Functions Prototypes
*******************************************************************************/
/* ==== API function ==== */
int32_t R_SCIF_UART_Init (uint32_t channel, uint8_t mode, 
		                                          uint16_t cks, uint8_t scbrr);

/* ==== User-defined function ==== */
void Userdef_SCIF2_UART_Init (uint8_t mode, uint16_t cks, uint8_t scbrr);

/* SCIF_H */
#endif

/* End of File */
