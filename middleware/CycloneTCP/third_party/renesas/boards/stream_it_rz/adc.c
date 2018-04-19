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
* File Name     : adc.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Sample Program - ADC channel A7 user code
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
/* Default  type definition header */
#include "r_typedefs.h"
/* I/O Register root header */
#include "iodefine.h"
/* Device driver header */
#include "dev_drv.h"
/* INTC Driver Header */
#include "intc.h"
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
#define ADC_DELAY       (10000)
#define ADC_PRI_ADI     (5uL)      /* ADI interrupt priority                 */

#define ADCSR_SET       (0x4002u)  /* ADCSR setting value                    */
#define STBCR6_7_SET    (0x80uL)   /* STBCR6  setting value                  */
#define PMC1_8_SET      (0x0100uL) /* P1 PMC  setting value                  */
#define PFCAE1_8_SET    (0xFEFFuL) /* P1 PFCAE  setting value                */
#define PFCE1_8_SET     (0xFEFFuL) /* P1 PFCE  setting value                 */
#define PFC1_8_SET      (0xFEFFuL) /* P1 PFC  setting value                  */
#define PM1_8_SET       (0x0100uL) /* P1 PM  setting value                   */
#define ADST_ON         (0x2000uL) /* ADC start                              */
#define ADST_OFF        (0xDFFFuL) /* ADC stop                               */
#define ADC_10BIT_SET   (0x03FFuL) /* 15-10bit 0 write ADCDRA                */
#define ADC_BIT_SHIFT_6 (0x0006uL) /* 6bit right shift ADCDRA                */
#define ADIE_ON         (0x4000uL) /* A/D conversion end interrupt enble     */
#define ADIE_OFF        (0xBFFFuL) /* A/D conversion end interrupt disabled  */
#define ADF_CLEAR       (0x7FFFuL) /* A/D conversion end interrupt ADF clear */
#define ADC_SET_FLG     (1u)       /* A/D conversion end interrupt ADF set   */
#define ADC_READ_ERR    ((uint32_t)-1)  /* Timeout error                    */

#define DELAY_TIME      (2000000uL)/* Sampling loop                          */
#define TIME_OUT        (2000000uL)/* Timeout loop                           */


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

static void     adc_start (void);
static void     adc_stop (void);
static uint32_t adc_read_single (void);
static void     adc_initialise (void);
static void     adc_uninitialise (void);
static void     int_adc_end (uint32_t sense);

/* Declare volatile to prevent it from being optimised by the compiler */
volatile uint8_t  g_adc_endflg = 0u;

uint8_t  g_adc_complete_flag = 0u;
uint32_t g_adc_result        = 0u;


/******************************************************************************
* Function Name: adc_initialise
* Description  : ADC Initialise (Single mode, Analog input AN7)
* Arguments    : none
* Return Value : none
******************************************************************************/
static void adc_initialise (void)
{
    uint8_t  stbcr6_tmp;

    /* Configure ADC
    Channel AN0, Single mode, AD conversion end interrupt enable,
    Conversion time =  256tcyc(10bit)
    ADCSR   - A/D  control/status register
    b15     - A/D end flag              - Write 0 to clear flag
    b14     - A/D interrupt enable      - AD conversion end interrupt enable
    b13     - A/D start                 - Stop
    b12-9   - Timer Trigger set         - Disables start A/D conversion by external trigger
    b8-6    - Clock Select              - Conversion time = 256tcyc(10bit)
    b5-3    - Multi-scan mode           - Single Mode
    b2-0    - Channel Select            - AN0 */
    ADC.ADCSR = ADCSR_SET;

    /* IRQ interrupt disabled */
    __disable_fiq();

    /* FIQ interrupt disabled */
    __disable_irq();

    /* Configure Low-power mode register
    ADC clock supply,Other settings are saved
    STBCR6  - Standby control register 6
    b7      - Module Stop 67            - ADC clock supply
    b6-0    - Module Stop 66-63         - No setting */
    stbcr6_tmp = CPG.STBCR6;
    CPG.STBCR6 = (uint8_t)((uint32_t)stbcr6_tmp & STBCR6_7_SET);

    /* Configure P1_10 as AN2 */
    rza_io_reg_write_16(&GPIO.PMC1,  1, GPIO_PMC1_PMC110_SHIFT,  GPIO_PMC1_PMC110);
    rza_io_reg_write_16(&GPIO.PFCAE1,0, GPIO_PFCAE1_PFCAE110_SHIFT,GPIO_PFCAE1_PFCAE110);
    rza_io_reg_write_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE110_SHIFT, GPIO_PFCE1_PFCE110);
    rza_io_reg_write_16(&GPIO.PFC1,  0, GPIO_PFC1_PFC110_SHIFT,  GPIO_PFC1_PFC110);

    /* static variable initialisation */
    g_adc_endflg = 0u;

    /* IRQ interrupt Enable */
    __enable_irq();

    /* FIQ interrupt Enable */
    __enable_fiq();

}

/******************************************************************************
* Function Name: adc_start
* Description  : ADC start
* Arguments    : none
* Return Value : none
******************************************************************************/
static void adc_start (void)
{
    uint16_t adcsr_tmp;

    /* Configure ADC
    ADC Start, Other settings are saved
    ADCSR   - A/D  control/status register
    b15     - A/D end flag              - No setting
    b14     - A/D interrupt enable      - No setting
    b13     - A/D start                 - Start
    b12-9   - Timer Trigger set         - No setting
    b8-6    - Clock Select              - No setting
    b5-3    - Multi-scan mode           - No setting
    b2-0    - Channel Select            - 111 */
    adcsr_tmp = ADC.ADCSR;
    ADC.ADCSR = (uint16_t)((uint32_t)adcsr_tmp | ADST_ON);
}


/******************************************************************************
* Function Name: adc_stop
* Description  : ADC stop
* Arguments    : none
* Return Value : none
******************************************************************************/
static void adc_stop (void)
{
    uint16_t adcsr_tmp;

    /* Configure ADC
    ADC Stop, Other settings are saved
    ADCSR     - A/D  control/status register
    b15       - A/D end flag            - No setting
    b14       - A/D interrupt enable    - No setting
    b13       - A/D start               - Stop
    b12-9     - Timer Trigger set       - No setting
    b8-6      - Clock Select            - No setting
    b5-3      - Multi-scan mode         - No setting
    b2-0      - Channel Select          - No setting */
    adcsr_tmp = ADC.ADCSR;
    ADC.ADCSR = (uint16_t)((uint32_t)adcsr_tmp & ADST_OFF);
}


/******************************************************************************
* Function Name: adc_read_single
* Description  : AN7 A/D conversion
* Arguments    : none
* Return Value : A/D conversion value
*              : Time out error
******************************************************************************/
static uint32_t adc_read_single (void)
{
    uint32_t adc_time_count;
    uint32_t  adc_data_a7;

    adc_time_count = 0u;

    /* Wait */
    do
    {
        if (TIME_OUT <= adc_time_count)
        {
            /* Time out. This is intentional. */
           break;
        }

        /* Time count up */
        adc_time_count++;
    }
    while (ADC_SET_FLG != (uint32_t)g_adc_endflg);

    if(TIME_OUT <= adc_time_count)
    {
        /* ADC stop */
        adc_stop();

        /* Timeout error */
        adc_data_a7 = ADC_READ_ERR;
    }
    else
    {
        /* Normal processing 6bit right shift ADCDR  */
        adc_data_a7 = (int32_t)(((uint32_t)ADC.ADDRC >> ADC_BIT_SHIFT_6) & ADC_10BIT_SET);
    }

    /* ADI interrupt disabled */
    R_INTC_Disable(INTC_ID_ADI);

    /* g_adc_end_flg clear */
    g_adc_endflg = 0u;

    /* ADI interrupt Enable */
    R_INTC_Enable(INTC_ID_ADI);

    return (adc_data_a7);
}

/******************************************************************************
* Function Name: adc_uninitialise
* Description  : ADC Uninitialise
* Arguments    : none
* Return Value : none
******************************************************************************/
static void adc_uninitialise (void)
{
    /* Configure ADC
    ADC All clear
    ADCSR     - A/D  control/status register */
    ADC.ADCSR = 0u;
}

/******************************************************************************
* Function Name: int_adc_end
* Description  : A/D conversion end interrupt
*              : Set g_adc_end_flg of Inform conversion end
* Arguments    : Type of interrupt request (unused)
* Return Value : none
******************************************************************************/
static void int_adc_end (uint32_t int_sense)
{
    uint16_t adcsr_tmp;

    UNUSED_PARAM(int_sense);


    g_adc_endflg = ADC_SET_FLG;

   	/* Clear ADC flag */
	if (0x8000u == (ADC.ADCSR & 0x8000u))
	{
		/* Clear ADC complete flag */
	    ADC.ADCSR &= 0x7FFFu;
	}

	/* Read the result register for AN2 */
	g_adc_result = ((uint32_t)ADC.ADDRC >> 4u);

	/* Check if the upper and lower limits have been read */
	if ((uint32_t)0u == g_adc_result)
	{
		g_adc_complete_flag |= 0x80u;
	}

	if ((uint32_t)4090u < g_adc_result)
	{
		g_adc_complete_flag |= 0x01u;
	}

    /* -> MISRA 10.6 */
    /* Configure ADC
    Clear of A/D end flag, Other settings are saved
    ADCSR   - A/D control/status register
    b15     - A/D end flag              - CLEAR
    b14     - A/D interrupt enable      - No setting
    b13     - A/D start                 - No setting
    b12-9   - Timer Trigger set         - No setting
    b8-6    - Clock Select              - No setting
    b5-3    - Multiple-scan mode           - No setting
    b2-0    - Channel Select            - No setting */
    adcsr_tmp = ADC.ADCSR;
    ADC.ADCSR = (uint16_t)((uint32_t)adcsr_tmp & ADF_CLEAR);
}


/******************************************************************************
* Interface Functions
******************************************************************************/

/******************************************************************************
* Function Name: R_ADC_Open
* Description  : Enables and configures AN0 ADC channel
* Arguments    : none
* Return Value : 0
******************************************************************************/
void R_ADC_Open (void)
{
    /* Registration interrupt function  */
    R_INTC_Regist_Int_Func(INTC_ID_ADI, &int_adc_end);

    /* Set interrupt priority */
    R_INTC_Set_Priority(INTC_ID_ADI, ADC_PRI_ADI);

    /* A/D end interrupt enable */
    R_INTC_Enable(INTC_ID_ADI);

    /* ADC initialise */
    adc_initialise();

}

/******************************************************************************
* End of Function R_ADC_Open
******************************************************************************/

/******************************************************************************
* Function Name: R_ADC_Close
* Description  : Disables the ADC
* Arguments    : none
* Return Value : 0
******************************************************************************/
void R_ADC_Close (void)
{
    /* A/D end interrupt disable */
    R_INTC_Disable(INTC_ID_ADI);

    /* ADC Uninitialised */
    adc_uninitialise();

}

/******************************************************************************
* Function Name: R_ADC_Read
* Description  : Read value from AN0
* Arguments    : none
* Return Value : AN0
******************************************************************************/
uint32_t R_ADC_Read (void)
{
	/* ADC start */
	adc_start();

	/* Wait for conversion to end */
	while (ADC_SET_FLG != g_adc_endflg)
	{
		__asm__("nop");
	}

    g_adc_result = adc_read_single();

	/* ADC start */
	adc_stop();

    return (g_adc_result);
}


/* End of File */

