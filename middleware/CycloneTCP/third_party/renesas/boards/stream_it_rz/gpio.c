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
* File Name     : gpio.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Initialise peripheral function sample - PORT initialise
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
/* Common Driver header */
#include "bsc_userdef.h"
/* I/O Register root header */
#include "iodefine.h"
/* Initialise peripheral function header */
#include "gpio.h"
/* Low level register read/write header */
#include "rza_io_regrw.h"
/* Interchangeable compiler specific header */
#include "compiler_settings.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
* Function Name: Port_Init
* Description  : Executes initial setting for the PORT.
*              : In this sample code, the PORT initial setting is executed to 
*              : connect the SDRAM to the CS2 and the CS3 spaces.
* Arguments    : none
* Return Value : none
******************************************************************************/
void Port_Init(void)
{
    /* ==== BSC settings ==== */

    /* ---- P5_8 : CS2 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 6th multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE5, 1, GPIO_PFCAE5_PFCAE58_SHIFT, GPIO_PFCAE5_PFCAE58);
    rza_io_reg_write_16(&GPIO.PFCE5,  0, GPIO_PFCE5_PFCE58_SHIFT,   GPIO_PFCE5_PFCE58);
    rza_io_reg_write_16(&GPIO.PFC5,   1, GPIO_PFC5_PFC58_SHIFT,     GPIO_PFC5_PFC58);
    rza_io_reg_write_16(&GPIO.PMC5,   1, GPIO_PMC5_PMC58_SHIFT,     GPIO_PMC5_PMC58);
    rza_io_reg_write_16(&GPIO.PIPC5,  1, GPIO_PIPC5_PIPC58_SHIFT,   GPIO_PIPC5_PIPC58);

    /* ---- P7_1 : CS3 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE71_SHIFT, GPIO_PFCAE7_PFCAE71);
    rza_io_reg_write_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE71_SHIFT,   GPIO_PFCE7_PFCE71);
    rza_io_reg_write_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC71_SHIFT,     GPIO_PFC7_PFC71);
    rza_io_reg_write_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC71_SHIFT,     GPIO_PMC7_PMC71);
    rza_io_reg_write_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC71_SHIFT,   GPIO_PIPC7_PIPC71);

    /* ---- P7_2 : RAS# ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE72_SHIFT, GPIO_PFCAE7_PFCAE72);
    rza_io_reg_write_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE72_SHIFT,   GPIO_PFCE7_PFCE72);
    rza_io_reg_write_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC72_SHIFT,     GPIO_PFC7_PFC72);
    rza_io_reg_write_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC72_SHIFT,     GPIO_PMC7_PMC72);
    rza_io_reg_write_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC72_SHIFT,   GPIO_PIPC7_PIPC72);

    /* ---- P7_3 : CAS# ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE73_SHIFT, GPIO_PFCAE7_PFCAE73);
    rza_io_reg_write_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE73_SHIFT,   GPIO_PFCE7_PFCE73);
    rza_io_reg_write_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC73_SHIFT,     GPIO_PFC7_PFC73);
    rza_io_reg_write_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC73_SHIFT,     GPIO_PMC7_PMC73);
    rza_io_reg_write_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC73_SHIFT,   GPIO_PIPC7_PIPC73);

    /* ---- P7_4 : CKE ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE74_SHIFT, GPIO_PFCAE7_PFCAE74);
    rza_io_reg_write_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE74_SHIFT,   GPIO_PFCE7_PFCE74);
    rza_io_reg_write_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC74_SHIFT,     GPIO_PFC7_PFC74);
    rza_io_reg_write_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC74_SHIFT,     GPIO_PMC7_PMC74);
    rza_io_reg_write_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC74_SHIFT,   GPIO_PIPC7_PIPC74);

    /* ---- P7_5 : RD/WR# ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE75_SHIFT, GPIO_PFCAE7_PFCAE75);
    rza_io_reg_write_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE75_SHIFT,   GPIO_PFCE7_PFCE75);
    rza_io_reg_write_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC75_SHIFT,     GPIO_PFC7_PFC75);
    rza_io_reg_write_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC75_SHIFT,     GPIO_PMC7_PMC75);
    rza_io_reg_write_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC75_SHIFT,   GPIO_PIPC7_PIPC75);

    /* ---- P7_6 : DQMLL# ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE76_SHIFT, GPIO_PFCAE7_PFCAE76);
    rza_io_reg_write_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE76_SHIFT,   GPIO_PFCE7_PFCE76);
    rza_io_reg_write_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC76_SHIFT,     GPIO_PFC7_PFC76);
    rza_io_reg_write_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC76_SHIFT,     GPIO_PMC7_PMC76);
    rza_io_reg_write_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC76_SHIFT,   GPIO_PIPC7_PIPC76);

    /* ---- P7_7 : DQMLU# ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE77_SHIFT, GPIO_PFCAE7_PFCAE77);
    rza_io_reg_write_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE77_SHIFT,   GPIO_PFCE7_PFCE77);
    rza_io_reg_write_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC77_SHIFT,     GPIO_PFC7_PFC77);
    rza_io_reg_write_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC77_SHIFT,     GPIO_PMC7_PMC77);
    rza_io_reg_write_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC77_SHIFT,   GPIO_PIPC7_PIPC77);
}

/* End of File */

