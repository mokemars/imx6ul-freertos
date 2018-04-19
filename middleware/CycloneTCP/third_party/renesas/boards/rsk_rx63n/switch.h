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
* File Name     : switch.h
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Provides declarations for functions defined in switch.c
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines standard boolean variable types used in this file */
#include <stdbool.h>
/* Defines standard variable types used in this file */
#include <stdint.h>

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef SWITCH_H
#define SWITCH_H

/* Switch 1 pressed flag mask */
#define SWITCHPRESS_1       0x80u
/* Switch 1 held down flag mask */
#define SWITCHHOLD_1        0x08u
/* Switch 2 pressed flag mask */
#define SWITCHPRESS_2       0x40u
/* Switch 2 held down flag mask */
#define SWITCHHOLD_2        0x04u
/* Switch 3 pressed flag mask */
#define SWITCHPRESS_3       0x20u
/* Switch 3 held down flag mask */
#define SWITCHHOLD_3        0x02u
/* Any switch pressed flag mask */
#define SWITCHPRESS_ALL     0xE0u
/* Any switch held down flag mask */
#define SWITCHHOLD_ALL      0x0Eu

/* Defines the CMT compare match value for the short switch debounce */
#define DEBOUNCE_SHORT      0x0800u
/* Defines the CMT compare match value for the long switch debounce */
#define DEBOUNCE_LONG       0x5000u

/* Define switch interrupt priority level */
#define SWITCH_IPL          6u
    
/* Check if NULL has been defined, and define as 0 if undefined */
#ifndef NULL
    #define NULL (0)
#endif
    
    
/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Switch standby ready global flag */
extern volatile bool gSwitchStandbyReady;
/* Switch flag global variable. Switch status bits:
        b7 : Switch 1 press complete flag*
        b6 : Switch 2 press complete flag*
        b5 : Switch 3 press complete flag*
        b4 : Unused
        b3 : Switch 1 held-down status flag
        b2 : Switch 2 held-down status flag
        b1 : Switch 3 held-down status flag
        b0 : Unused    
         * Switch press complete flags must be cleared manually     */
extern volatile uint8_t gSwitchFlag;

/*******************************************************************************
* Global Function Prototypes
*******************************************************************************/
/* Switch initialisation function prototype */
void InitialiseSwitchInterrupts(void);
/* Switch enable/disable function prototype */
void ControlSwitchInterrupts(uint8_t);
/* Switch callback function initialisation function prototype */
void SetSwitchPressCallback(void(*callBack)(void));
/* Switch callback function initialisation function prototype */
void SetSwitchReleaseCallback(void(*callBack)(void));

/* End of multiple inclusion prevention macro */
#endif
