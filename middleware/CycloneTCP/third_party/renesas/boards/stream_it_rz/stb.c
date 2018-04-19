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
/******************************************************************************
* File Name     : stb.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Sample Program - Initialise system CPG register
******************************************************************************/
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
/* Common Driver header */
#include "bsc_userdef.h"
/* System CPG register configuration header */
#include "stb.h"
/* Interchangeable compiler specific header */
#include "compiler_settings.h"

/******************************************************************************
* Function Name: STB_Init
* Description  : Configures the standby control register for each peripheral's 
*                channels. It either supplies clock signals to a channel or 
*                not; enabling or disabling it.
* Arguments    : none
* Return Value : none
******************************************************************************/
void STB_Init (void)
{
    volatile uint8_t dummy_buf = 0u;

    UNUSED_VARIABLE(dummy_buf);

    /* ---- Enable all module clocks ---- */

    /* Port level is keep in standby mode, [1], [1], [0],           */
    /* [1], [0], [1], CoreSight                                     */
    CPG.STBCR2 = 0x6Au;

    /* (Dummy read)                                                 */
    dummy_buf = CPG.STBCR2;

    /* IEBus, IrDA, LIN0, LIN1, MTU2, RSCAN2, [0], PWM              */
    CPG.STBCR3 = 0x00u;

    /* (Dummy read)                                                 */
    dummy_buf = CPG.STBCR3;

    /* SCIF0, SCIF1, SCIF2, SCIF3, SCIF4, SCIF5, SCIF6, SCIF7       */
    CPG.STBCR4 = 0x00u;

    /* (Dummy read)                                                 */
    dummy_buf = CPG.STBCR4;

    /* SCIM0, SCIM1, SDG0, SDG1, SDG2, SDG3, OSTM0, OSTM1           */
    CPG.STBCR5 = 0x00u;

    /* (Dummy read)                                                 */
    dummy_buf = CPG.STBCR5;

    /* A/D, CEU, DISCOM0, DISCOM1, DRC0, DRC1, JCU, RTClock         */
    CPG.STBCR6 = 0x00u;

    /* (Dummy read)                                                 */
    dummy_buf = CPG.STBCR6;

    /* DVDEC0, DVDEC1, [1], ETHER, FLCTL, [1], USB0, USB1           */
    CPG.STBCR7 = 0x24u;

    /* (Dummy read)                                                 */
    dummy_buf = CPG.STBCR7;

    /* IMR-LS20, IMR-LS21, IMR-LSD, MMCIF, MOST50, [1], SCUX, [1]   */
    CPG.STBCR8 = 0x05u;

    /* (Dummy read)                                                 */
    dummy_buf = CPG.STBCR8;

    /* I2C0, I2C1, I2C2, I2C3, SPIBSC0, SPIBSC1, VDC50, VDC51       */
    CPG.STBCR9 = 0x00u;

    /* (Dummy read)                                                 */
    dummy_buf = CPG.STBCR9;

    /* RSPI0, RSPI1, RSPI2, RSPI3, RSPI4, CD-ROMDEC, RSPDIF, RGPVG  */
    CPG.STBCR10 = 0x00u;

    /* (Dummy read)                                                 */
    dummy_buf = CPG.STBCR10;

    /* [1], [1], SSIF0, SSIF1, SSIF2, SSIF3, SSIF4, SSIF5           */
    CPG.STBCR11 = 0xC0u;

    /* (Dummy read)                                                 */
    dummy_buf = CPG.STBCR11;

    /* [1], [1], [1], [1], SDHI00, SDHI01, SDHI10, SDHI11           */
    CPG.STBCR12 = 0xF0u;

    /* (Dummy read)                                                 */
    dummy_buf = CPG.STBCR12;
}


/* End of File */

