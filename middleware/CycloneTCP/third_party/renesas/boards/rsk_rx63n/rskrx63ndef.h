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
/*******************************************************************************
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    */
/*******************************************************************************
* File Name     : rskrx63ndef.h
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines macros relating to the RX63N user LEDs and switches
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef RSKRX63NDEF_H
#define RSKRX63NDEF_H

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines RX63N port registers */
#include "iorx63n.h"

/* General Values */
#define LED_ON           (0)
#define LED_OFF          (1)
#define SET_BIT_HIGH     (1)
#define SET_BIT_LOW      (0)
#define SET_BYTE_HIGH    (0xFF)
#define SET_BYTE_LOW     (0x00)
#define OUTPUT_PIN       (1)
#define INPUT_PIN        (0)

/* Switch port pins data direction */
#define SW1_PIN_DIR      PORT0.PDR.BIT.B2
#define SW2_PIN_DIR      PORT0.PDR.BIT.B0
#define SW3_PIN_DIR      PORT0.PDR.BIT.B7

/* Button input pin settings */
#define SW1              PORT0.PIDR.BIT.B2
#define SW2              PORT0.PIDR.BIT.B0
#define SW3              PORT0.PIDR.BIT.B7

/* LED data direction */
#define LED0_PIN_DIR     PORT0.PDR.BIT.B3
#define LED1_PIN_DIR     PORT0.PDR.BIT.B5
#define LED2_PIN_DIR     PORT1.PDR.BIT.B0
#define LED3_PIN_DIR     PORT1.PDR.BIT.B1

/* LED output pin settings */
#define LED0             PORT0.PODR.BIT.B3
#define LED1             PORT0.PODR.BIT.B5
#define LED2             PORT1.PODR.BIT.B0
#define LED3             PORT1.PODR.BIT.B1

/* End of multiple inclusion prevention macro */
#endif
