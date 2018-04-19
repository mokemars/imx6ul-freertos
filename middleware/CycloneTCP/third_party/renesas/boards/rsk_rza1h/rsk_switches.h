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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File Name     : rsk_switches.c
* Device(s)     : RZ/A1H RSK+RZA1H
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Sample Program - Switch handling function
******************************************************************************/
/******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"

/* Multiple inclusion prevention macro */
#ifndef RSK_SWITCHES_H
#define RSK_SWITCHES_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define SW1_SET_FLG        (0x01)
#define SW2_SET_FLG        (0x02)
#define SW3_SET_FLG        (0x04)
#define NMI_SET_FLG        (0x08)

#define SW1_SW2_ON         (0x03)
#define SW1_SW3_ON         (0x05)
#define SW2_SW3_ON         (0x06)

#define SW_ALL_ON          (0x07)
#define SW_ALL_OFF         (0x00)

#define SW1_SET_FLG_MASK   (0xFE)
#define SW2_SET_FLG_MASK   (0xFD)
#define SW3_SET_FLG_MASK   (0xFB)
#define NMI_SET_FLG_MASK   (0xF7)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
extern volatile uint8_t  g_switch_press_flg;

/******************************************************************************
Private global variables and functions
******************************************************************************/
/*  Switch initialisation function */
void init_switches (void);

/* Switch callback function initialisation function prototype */
void Set_Switch_Press_Callback (void(*callBack)(void));
/* Switch callback function initialisation function prototype */
void Set_Switch_Release_Callback (void(*callBack)(void));


/* RSK_SWITCHES_H */
#endif 

/* End of File */

