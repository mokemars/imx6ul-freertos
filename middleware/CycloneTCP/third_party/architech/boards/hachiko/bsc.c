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
* File Name     : bsc.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Sample Program - BSC initialise
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Default  type definition header */
#include "r_typedefs.h"
/* Device driver header */
#include "dev_drv.h"
/* Common Driver header */
#include "bsc_userdef.h"
/* Bus state controller system header */
#include "bsc.h"
/* Interchangeable compiler specific header */
#include "compiler_settings.h"

/*******************************************************************************
* Function Name: BSC_Init
* Description  :
* Arguments    : uint8 area
*              :   B'xxxxxxxx
*              :     |||||||+--- [0]   CS0
*              :     ||||||+---- [1]   CS1
*              :     |||||+----- [2]   CS2
*              :     ||||+------ [3]   CS3
*              :     |||+------- [4]   CS4
*              :     ||+-------- [5]   CS5
*              :     ++--------- [6-7] n/a
* Return Value : none
*******************************************************************************/
void BSC_Init (uint8_t area)
{
	/* CS0 */
    if ((area & BSC_AREA_CS0) != 0)
    {
        userdef_bsc_cs0_init();
    }

    /* CS1 */
    if ((area & BSC_AREA_CS1) != 0)
    {
        userdef_bsc_cs1_init();
    }

    /* CS2 */
    if ((area & BSC_AREA_CS2) != 0)
    {
        userdef_bsc_cs2_init();
    }

    /* CS3 */
    if ((area & BSC_AREA_CS3) != 0)
    {
        userdef_bsc_cs3_init();
    }

    /* CS4 */
    if ((area & BSC_AREA_CS4) != 0)
    {
        userdef_bsc_cs4_init();
    }

    /* CS5 */
    if ((area & BSC_AREA_CS5) != 0)
    {
        userdef_bsc_cs5_init();
    }
}

/* End of File */

