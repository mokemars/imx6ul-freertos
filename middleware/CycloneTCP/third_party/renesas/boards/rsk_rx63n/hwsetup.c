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
* File Name     : hwsetup.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines the hardware initialisation routines used each time 
*                  the MCU is restarted. 
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Includes assembley level definitions */
#include <machine.h>
/* Following header file provides C99 type definitions. */
#include <stdint.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines RX63N port registers */
#include "iorx63n.h"
/* Defines macros relating to the RX63N user LEDs and switches */
#include "rskrx63ndef.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Switch handler function definitions */
#include "switch.h"
/* Contains declarations for the functions defined in this file */
#include "hwsetup.h"

/*******************************************************************************
* Outline       : HardwareSetup
* Description   : Contains all the setup functions called at device restart
* Argument      : none
* Return value  : none
*******************************************************************************/
void HardwareSetup(void)
{
    ConfigureOperatingFrequency();
    ConfigureOutputPorts();
    ConfigureInterrupts();
    EnablePeripheralModules();
}
/*******************************************************************************
* End of function HardwareSetup
*******************************************************************************/

/*******************************************************************************
* Outline       : ConfigureOperatingFrequency
* Description   : Configures the clock settings for each of the device clocks
* Argument      : none
* Return value  : none
*******************************************************************************/
void ConfigureOperatingFrequency(void)
{                    
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503u;            
    
    /* Stop sub-clock */
    SYSTEM.SOSCCR.BYTE = 0x01u;            
                                        
    /* Set main oscillator settling time to 10ms (131072 cycles @ 12MHz) */ 
    SYSTEM.MOSCWTCR.BYTE = 0x0Du;        
        
    /* Set PLL circuit settling time to 10ms (2097152 cycles @ 192MHz) */                        
    SYSTEM.PLLWTCR.BYTE = 0x0Eu;            
    
    /* Set PLL circuit to x16 */
    SYSTEM.PLLCR.WORD = 0x0F00u;        

    /* Start the external 12Mhz oscillator */
    SYSTEM.MOSCCR.BYTE = 0x00u;    
            
    /* Turn on the PLL */
    SYSTEM.PLLCR2.BYTE = 0x00u;    
    
    /* Wait over 12ms (~2075op/s @ 125KHz) */
    for(volatile uint16_t i = 0; i < 2075u; i++)
    {
        nop();
    }
        
    /* Configure the clocks as follows -
    Clock Description              Frequency
    ----------------------------------------
    PLL Clock frequency...............192MHz
    System Clock Frequency.............96MHz
    Peripheral Module Clock B..........48MHz     
    FlashIF Clock......................48MHz
    External Bus Clock.................48MHz */                 
    SYSTEM.SCKCR.LONG = 0x21821211u;    
    
    /* Configure the clocks as follows -
    Clock Description              Frequency
    ----------------------------------------
    USB Clock..........................48MHz      
    IEBus Clock........................24MHz */ 
    SYSTEM.SCKCR2.WORD = 0x0033u;
    
    /* Set the clock source to PLL */
    SYSTEM.SCKCR3.WORD = 0x0400u;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500u;        
}
/*******************************************************************************
* End of function ConfigureOperatingFrequency
*******************************************************************************/

/*******************************************************************************
* Outline       : ConfigureOutputPorts
* Description   : Configures the port and pin direction settings, and sets the
*                  pin outputs to a safe level.
* Argument      : none
* Return value  : none
*******************************************************************************/
void ConfigureOutputPorts(void)
{
    /* Configure the LED pins (LED0-LED3) as outputs */
    LED0_PIN_DIR = OUTPUT_PIN;
    LED1_PIN_DIR = OUTPUT_PIN;
    LED2_PIN_DIR = OUTPUT_PIN;
    LED3_PIN_DIR = OUTPUT_PIN;

    /* Set LED pin outputs as high (all LEDs off) */
    LED0 = LED_OFF;
    LED1 = LED_OFF;
    LED2 = LED_OFF;
    LED3 = LED_OFF;
    
    /* Configure the debug LCD data pins (DLCD7-DLCD4) as outputs */
    PORT8.PDR.BYTE |= 0xF0u;
            
    /* Set debug LCD data pin (DLCD7-DLCD4) outputs as low */
    PORT8.PODR.BYTE &= 0x0Fu;
    
    /* Configure the debug LCD control pin (DLCDRS) and enable pin (DLCDE)
       as outputs */
    RS_PIN_DIR = OUTPUT_PIN;
    E_PIN_DIR  = OUTPUT_PIN;
            
    /* Set the debug LCD control pin (DLCDRS) and enable pin (DLCDE) as output
       low */
    RS_PIN = 0x0u;
    E_PIN  = 0x0u;    
}
/*******************************************************************************
* End of function ConfigureOutputPorts
*******************************************************************************/

/*******************************************************************************
* Outline       : ConfigureInterrupts
* Description   : Configures the interrupts used
* Argument      : none
* Return value  : none
*******************************************************************************/
void ConfigureInterrupts(void)
{
    /* Configure switch interrupts */
    InitialiseSwitchInterrupts();
}
/*******************************************************************************
* End of function ConfigureInterrupts
*******************************************************************************/

/*******************************************************************************
* Outline       : EnablePeripheralModules
* Description   : Enables and configures peripheral devices on the MCU
* Argument      : none
* Return value  : none
*******************************************************************************/
void EnablePeripheralModules(void)
{
    /* Peripherals initialised in specific initialisation functions */
}
/*******************************************************************************
* End of function EnablePeripheralModules
*******************************************************************************/
