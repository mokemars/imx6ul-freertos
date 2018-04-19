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
* File Name     : rsk_switches.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Sample Program - Switch handling function
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_typedefs.h"
/* I/O register definitions */
#include "iodefine.h"
/* Device Driver common header */
#include "dev_drv.h"
/* INTC driver header */
#include "intc.h"
/* Sample Switch interface header */
#include "rsk_switches.h"
/* Low level register read/write header */
#include "rza_io_regrw.h"
/* compiler specific configuration */
#include "compiler_settings.h"

/*******************************************************************************
Typedef definitions
*******************************************************************************/

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* priority of interrupt IRQ2  */
#define INT_IRQ2_PRI               (15u)
/* priority of interrupt IRQ3  */
#define INT_IRQ3_PRI               (15u)
/* priority of interrupt IRQ5  */
#define INT_IRQ5_PRI               (15u)

/* priority of interrupt TGI4A */
#define MTU2_CH4_PRI_A             (8u)

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/


/*******************************************************************************
Private global variables and functions
*******************************************************************************/
/* IRQx interrupt handler function prototypes */
static void int_irq3 (uint32_t sense);
static void int_irq2 (uint32_t sense);
static void int_irq5 (uint32_t sense);

/* Functions used for creating the switch software debounce delay */
static void init_switch_debounce_timer (void);
static void switch_debounce_delay (void);
static void mtu_int_gfa (uint32_t int_sense);


/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/
/* Stores switch states detected via interrupts */
volatile uint8_t  g_switch_press_flg = 0u;

/* Switch press call-back pointer declaration */
void (*g_switch_press_callback_func) (void) = 0;

/* Switch release call-back pointer declaration */
void (*g_switch_release_callback_func) (void) = 0;


/*******************************************************************************
* Function Name: Init_Switches
* Description  : configures user switches (SW1,SW2,SW3) on target board
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_switches (void)
{
    /* Set SW1 (IRQ3) as input pin (P1_9) */
	rza_io_reg_write_16(&GPIO.PBDC1,   1, GPIO_PBDC1_PBDC19_SHIFT,   GPIO_PBDC1_PBDC19);
	rza_io_reg_write_16(&GPIO.PMC1,    1, GPIO_PMC1_PMC19_SHIFT,     GPIO_PMC1_PMC19);
    rza_io_reg_write_16(&GPIO.PFCAE1,  0, GPIO_PFCAE1_PFCAE19_SHIFT, GPIO_PFCAE1_PFCAE19);
    rza_io_reg_write_16(&GPIO.PFCE1,   1, GPIO_PFCE1_PFCE19_SHIFT,   GPIO_PFCE1_PFCE19);
    rza_io_reg_write_16(&GPIO.PFC1,    0, GPIO_PFC1_PFC19_SHIFT,     GPIO_PFC1_PFC19);

	/* Set SW2 (IRQ2) as input pin (P1_8) */
	rza_io_reg_write_16(&GPIO.PBDC1,   1, GPIO_PBDC1_PBDC18_SHIFT,   GPIO_PBDC1_PBDC18);
	rza_io_reg_write_16(&GPIO.PMC1,    1, GPIO_PMC1_PMC18_SHIFT,     GPIO_PMC1_PMC18);
    rza_io_reg_write_16(&GPIO.PFCAE1,  0, GPIO_PFCAE1_PFCAE18_SHIFT, GPIO_PFCAE1_PFCAE18);
    rza_io_reg_write_16(&GPIO.PFCE1,   1, GPIO_PFCE1_PFCE18_SHIFT,   GPIO_PFCE1_PFCE18);
    rza_io_reg_write_16(&GPIO.PFC1,    0, GPIO_PFC1_PFC18_SHIFT,     GPIO_PFC1_PFC18);

	/* Set SW3 (IRQ5) as input pin (P1_11) */
	rza_io_reg_write_16(&GPIO.PBDC1,   1, GPIO_PBDC1_PBDC111_SHIFT,   GPIO_PBDC1_PBDC111);
	rza_io_reg_write_16(&GPIO.PMC1,    1, GPIO_PMC1_PMC111_SHIFT,     GPIO_PMC1_PMC111);
    rza_io_reg_write_16(&GPIO.PFCAE1,  0, GPIO_PFCAE1_PFCAE111_SHIFT, GPIO_PFCAE1_PFCAE111);
    rza_io_reg_write_16(&GPIO.PFCE1,   1, GPIO_PFCE1_PFCE111_SHIFT,   GPIO_PFCE1_PFCE111);
    rza_io_reg_write_16(&GPIO.PFC1,    0, GPIO_PFC1_PFC111_SHIFT,     GPIO_PFC1_PFC111);

    /* Configure IRQs detections on falling edge */
    INTC.ICR1 |= 0x0454;

	/* Configure and disable SW1 transmit interrupts */
	R_INTC_Disable(INTC_ID_IRQ3);
	R_INTC_Regist_Int_Func(INTC_ID_IRQ3, &int_irq3);
	R_INTC_Set_Priority(INTC_ID_IRQ3, INT_IRQ3_PRI);

	/* Configure and disable SW2 transmit interrupts. */
	R_INTC_Disable(INTC_ID_IRQ2);
	R_INTC_Regist_Int_Func(INTC_ID_IRQ2, &int_irq2);
	R_INTC_Set_Priority(INTC_ID_IRQ2, INT_IRQ2_PRI);

	/* Configure and disable SW3 transmit interrupts */
	R_INTC_Disable(INTC_ID_IRQ5);
	R_INTC_Regist_Int_Func(INTC_ID_IRQ5, &int_irq5);
	R_INTC_Set_Priority(INTC_ID_IRQ5, INT_IRQ5_PRI);

	/* Enable switch interrupts */
	R_INTC_Enable(INTC_ID_IRQ3);
	R_INTC_Enable(INTC_ID_IRQ2);
	R_INTC_Enable(INTC_ID_IRQ5);

	/* Initialise MTU2 channel 4 used to debounce switches */
	init_switch_debounce_timer();
}

/*******************************************************************************
* End of Function init_switches
*******************************************************************************/

/*******************************************************************************
* Function Name: int_irq3
* Description  : IRQ3 interrupt
*              : Handler for IRQ3 (SW1) interrupts
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    : INTC_LEVEL_SENSITIVE : Level sense
*              :                    : INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : none
*******************************************************************************/
static void int_irq3 (uint32_t sense)
{
	uint16_t dummy_read;

	R_INTC_Disable(INTC_ID_IRQ3);

	/* Set the user switch flag to indicate switch was pressed */
    g_switch_press_flg |= SW1_SET_FLG;

    /* Add delay to debounce the switch */
    switch_debounce_delay();

    /* Check if switch press call-back function is not NULL */
	if(g_switch_press_callback_func)
	{
		/* Execute user call-back function */
		g_switch_press_callback_func();
	}

	/* Clearing the status flag requires a dummy read */
	dummy_read = INTC.IRQRR;

    /* Clear IRQ3 interrupt flag  */
    if (0u != (dummy_read & 0x0008))
    {
    	INTC.IRQRR &= 0xFFF7;
    }

    R_INTC_Enable(INTC_ID_IRQ3);
}

/*******************************************************************************
* End of Function int_irq3
*******************************************************************************/

/*******************************************************************************
* Function Name: int_irq2
* Description  : IRQ2 interrupt
*              : Handler for IRQ2 (SW2) interrupts
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    : INTC_LEVEL_SENSITIVE : Level sense
*              :                    : INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : none
*******************************************************************************/
static void int_irq2 (uint32_t sense)
{
	uint16_t dummy_read;

	R_INTC_Disable(INTC_ID_IRQ2);

    /* Set the user switch flag to indicate switch was pressed */
    g_switch_press_flg |= SW2_SET_FLG;

    /* Add delay to debounce the switch */
    switch_debounce_delay();

    /* Check if switch press call-back function is not NULL */
	if(g_switch_press_callback_func)
	{
		/* Execute user call-back function */
		g_switch_press_callback_func();
	}

	/* Clearing the status flag requires a dummy read */
	dummy_read = INTC.IRQRR;

    /* Clear IRQ2 interrupt flag  */
    if (0u != (dummy_read & 0x0004))
    {
    	INTC.IRQRR &= 0xFFFB;
    }

    R_INTC_Enable(INTC_ID_IRQ2);
}

/*******************************************************************************
* End of Function int_irq2
*******************************************************************************/

/*******************************************************************************
* Function Name: int_irq5
* Description  : IRQ5 interrupt
*              : Handler for IRQ5 (SW3) interrupts
* Arguments    : uint32_t int_sense : Interrupt detection
*              :                    : INTC_LEVEL_SENSITIVE : Level sense
*              :                    : INTC_EDGE_TRIGGER    : Edge trigger
* Return Value : none
*******************************************************************************/
static void int_irq5 (uint32_t sense)
{
	uint16_t dummy_read;

	R_INTC_Disable(INTC_ID_IRQ5);

    /* Set the user switch flag to indicate switch was pressed */
    g_switch_press_flg |= SW3_SET_FLG;

    /* Add delay to debounce the switch */
    switch_debounce_delay();

    /* Check if switch press call-back function is not NULL */
	if(g_switch_press_callback_func)
	{
		/* Execute user call-back function */
		g_switch_press_callback_func();
	}

	/* Clearing the status flag requires a dummy read */
	dummy_read = INTC.IRQRR;

    /* Clear IRQ5 interrupt flag  */
    if (0u != (dummy_read & 0x0020))
    {
    	INTC.IRQRR &= 0xFFD0;
    }

    R_INTC_Enable(INTC_ID_IRQ5);
}

/*******************************************************************************
* End of Function int_irq5
*******************************************************************************/

/*******************************************************************************
* Function Name: set_switch_press_callback
* Description  : Takes a pointer to a function, and sets it as the call-back
*				 function for the switch interrupts. The passed function is
*				 executed every time any switch is pressed down.
* Arguments    : void (* func)(uint32_t) : Function to be called by the IRQ
* Return Value : none
*******************************************************************************/
void set_switch_press_callback (void (* func)(void))
{
	/* Store the call-back function pointer into the global variable */
	g_switch_press_callback_func = func;
}

/*******************************************************************************
* Function Name : set_switch_release_callback
* Description 	: Takes a pointer to a function, and sets it as the call-back
*				  function for the switch interrupts. The passed function is
*				  executed every time any switch is released.
* Argument  	: pointer to call-back function (set to NULL to disable)
* Return value  : none
*******************************************************************************/
void set_switch_release_callback (void(*callback)(void))
{
	/* Store the callback function pointer into the global variable */
	g_switch_release_callback_func = callback;
}


/******************************************************************************
* Function Name: init_switch_debounce_timer
* Description  : Initialises MTU2 channel 3 to generate a 10ms delay for
*                debouncing the switches.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void init_switch_debounce_timer (void)
{
    /* Initialise the register of MTU2. */
    MTU2.TSTR &= 0x7F;

    /* Enable access to write-protected MTU2 registers */
    MTU2.TRWER = 1u;

    /* Clear the timer count */
    MTU2.TCNT_4 = 0u;

    /* MTU2 channel4 normal operation */
    MTU2.TMDR_4 = 0u;

    /* Timer output is not required */
    MTU2.TIORH_4 = 0;

    /* Clear the compare match status flag */
    MTU2.TSR_4 &= 0xFE;

    /* Enable TGRA interrupts */
    MTU2.TIER_4 = 0x01;

    /* Set the period */
    MTU2.TGRA_4 = 5000;

    /* TCNT cleared by TGRA compare match, rising edge count,
       clock source prescale P0/1024 = 32.226KHz,  */
    MTU2.TCR_4 = (0x20 | 0x05);

    /* Enable access to MTU2 write-protected registers */
    MTU2.TRWER = 0u;

    /* The setup process the interrupt IntTgfa function.*/
	R_INTC_Regist_Int_Func(INTC_ID_TGI4A , &mtu_int_gfa);
	R_INTC_Set_Priority(INTC_ID_TGI4A , MTU2_CH4_PRI_A);
	R_INTC_Enable(INTC_ID_TGI4A);
}


/******************************************************************************
* Function Name: switch_debounce_delay
* Description  : MT2 channel 4's interrupt handler. Sets flag to indicate end
*                completion of interrupt.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void switch_debounce_delay (void)
{
	/* Start the count of channel 4 of MTU2. */
	MTU2.TSTR |= 0x80;

	/* Wait for the the 10ms period to expire */
	while (0x01 != (MTU2.TSR_4 & 0x01))
    {
    	__asm__("nop");
    }

	/* Reload the period */
	MTU2.TGRA_4 = 5000;
}


/******************************************************************************
* Function Name: mtu_int_gfa
* Description  : MT2 channel 4's interrupt handler. Sets flag to indicate end
*                completion of interrupt.
* Arguments    : uint32_t int_sense (1:edge )
*                                   (0:level)
* Return Value : none
******************************************************************************/
static void mtu_int_gfa (uint32_t int_sense)
{
	uint16_t dummy_read = 0u;

	/* Stop the count of channel 4 of MTU2. */
	MTU2.TSTR &= 0x7F;

	/* Disable the MTU2 channel 4 interrupt */
	R_INTC_Disable(INTC_ID_TGI4A);

	/* Clearing the status flag requires a dummy read */
	dummy_read = MTU2.TSR_4;

    /* Clear the TGFA flag */
    if (0x01 == (dummy_read & 0x01))
    {
    	MTU2.TSR_4 &= 0xFE;
    }
}


/* End of File */

