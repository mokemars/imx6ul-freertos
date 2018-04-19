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
* File Name     : ostm_userdef.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board

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
/* Device Driver common header */
#include "dev_drv.h"                
/* OSTM Driver header */
#include "ostm.h"
/* INTC Driver Header */
#include "intc.h"            
/* definition of I/O registers */
#include "iodefine.h"
/* Low level register read/write header */
#include "rza_io_regrw.h"


/******************************************************************************
Macro definitions
******************************************************************************/
/* ---- P0 clock (Unit : kHz) ---- */
#define P0_CLOCK_FREQUENCY_kHz  (33.333 * 1000)     /* Assume 33.333MHz */

/* ---- Timer cycle specification(Argument cycle)(Unit : ms) maximum value ---- */
/* The maximum value is H'FFFF_FFFF because CMP is the register which has 32 bits */
/* 128,850msec * 33.333MHz = 4,294,957,050 (H'FFFF_D7FA)   : No Error             */
/* 128,851msec * 33.333MHz = 4,294,990,383 (H'1_0000_5A2F) : Error                */
/* Timer cycle specification > Error when the cycle is 128850ms                   */
#define MAX_CYCLE_msec          (0xFFFFFFFF / P0_CLOCK_FREQUENCY_kHz)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
#ifdef USE_CH_OSTM0
extern void Sample_OSTM0_Interrupt(uint32_t int_sense);

#endif /* USE_CH_OSTM0 */

#ifdef USE_CH_OSTM1
extern void Sample_OSTM1_Interrupt(uint32_t int_sense);

#endif /* USE_CH_OSTM1 */

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/


#ifdef USE_CH_OSTM0
/******************************************************************************
* Function Name: Userdef_OSTM0_Init
* Description  : This function is a user-defined function. Run the processing to
*              : execute initial setting for the count value of the OSTM channel 0 
*              : using the timer cycle specified by the argument cycle in the 
*              : operating mode specified by the argument mode. In this sample code,
*              : the timer count value is set assuming that the P0 clock is set to
*              : 33.33MH. The initial setting for the use of the OSTM0 interrupt
*              : is also executed.
* Arguments    : uint32_t mode    : OSTM operating mode
*              :                  :   OSTM_MODE_INTERVAL : Interval timer mode
*              :                  :   OSTM_MODE_COMPARE  : Free running compare mode
*              : uint32_t cycle   : Timer cycle (msec)
* Return Value : DEVDRV_SUCCESS   : Success to initialise OSTM
*              : DEVDRV_ERROR     : Failure to initialise OSTM
******************************************************************************/
int32_t Userdef_OSTM0_Init (uint32_t mode, uint32_t cycle)
{
    volatile uint8_t dummy_buf = 0;

    UNUSED_VARIABLE(dummy_buf);

    /* ==== Argument check ==== */
    if (cycle > MAX_CYCLE_msec)
    {
        return DEVDRV_ERROR;
    }

    /* ==== Module standby clear ==== */
    /* ---- Supply clock to the OSTM(channel 0) ---- */
    rza_io_reg_write_8((uint8_t *)&CPG.STBCR5, 0, CPG_STBCR5_MSTP51_SHIFT, CPG_STBCR5_MSTP51);
    dummy_buf = CPG.STBCR5;     /* Dummy read */

    /* ==== Start counting with OS timer 500ms ===== */
    /* ---- OSTM count stop trigger register (TT) setting ---- */
    OSTM0.OSTMnTT = 0x01u;      /* Stop counting */

    /* ---- OSTM control register (CTL) setting ---- */
    /* Counter operating mode               */
    /* Enable interrupt when start counting */
    rza_io_reg_write_8(&OSTM0.OSTMnCTL, (uint8_t)mode, OSTM0_OSTMnCTL_MD1_SHIFT, OSTM0_OSTMnCTL_MD1);
    rza_io_reg_write_8(&OSTM0.OSTMnCTL,             1, OSTM0_OSTMnCTL_MD0_SHIFT, OSTM0_OSTMnCTL_MD0);

    /* ---- OSTM compare register (CMP) setting ---- */
    OSTM0.OSTMnCMP = (uint32_t)(P0_CLOCK_FREQUENCY_kHz * cycle);

        /* Set timer in the cycle specified by the cycle */
        /* CMP = P0 clock (kHz) * cycle (msec)           */
        /* (Assume that P0 clock has 33.333MHz)          */

    /* ==== Register OS timer interrupt handler ==== */
    R_INTC_Regist_Int_Func(INTC_ID_OSTM0TINT, Sample_OSTM0_Interrupt);

    /* ==== Set priority of OS timer interrupt to 5 ==== */
    R_INTC_Set_Priority(INTC_ID_OSTM0TINT, 5);

    /* ==== Validate OS timer interrupt ==== */
    R_INTC_Enable(INTC_ID_OSTM0TINT);

    return DEVDRV_SUCCESS;
}

#endif /* USE_CH_OSTM0 */

#ifdef USE_CH_OSTM1
/******************************************************************************
* Function Name: Userdef_OSTM1_Init
* Description  : This function is a user-defined function. Run the processing 
*              : to execute initial setting for the count value of the OSTM 
*              : channel 1 using the timer cycle specified by the argument cycle
*              : in the operating mode specified by the argument mode.
* Arguments    : uint32_t mode    : OSTM operating mode
*              :                  :   OSTM_MODE_INTERVAL : Interval timer mode
*              :                  :   OSTM_MODE_COMPARE  : Free running compare mode
*              : uint32_t cycle   : Timer cycle
* Return Value : DEVDRV_SUCCESS   : Success to initialise OSTM
*              : DEVDRV_ERROR     : Failure to initialise OSTM
******************************************************************************/
int32_t Userdef_OSTM1_Init (uint32_t mode, uint32_t cycle)
{
    volatile uint8_t dummy_buf = 0;

    UNUSED_VARIABLE(dummy_buf);

    /* ==== Argument check ==== */
    if (cycle > MAX_CYCLE_msec)
    {
        return DEVDRV_ERROR;
    }

    /* ==== Module standby clear ==== */
    /* ---- Supply clock to the OSTM(channel 1) ---- */
    rza_io_reg_write_8((uint8_t *)&CPG.STBCR5, 0, CPG_STBCR5_MSTP50_SHIFT, CPG_STBCR5_MSTP50);
    dummy_buf = CPG.STBCR5;     /* Dummy read */

    /* ==== Start counting with OS timer 500ms ===== */
    /* ---- OSTM count stop trigger register (TT) setting ---- */
    OSTM1.OSTMnTT = 0x01u;      /* Stop counting */

    /* ---- OSTM control register (CTL) setting ---- */
    /* Counter operating mode               */
    /* Enable interrupt when start counting */
    rza_io_reg_write_8(&OSTM1.OSTMnCTL, (uint8_t)mode, OSTM1_OSTMnCTL_MD1_SHIFT, OSTM1_OSTMnCTL_MD1);
    rza_io_reg_write_8(&OSTM1.OSTMnCTL,             1, OSTM1_OSTMnCTL_MD0_SHIFT, OSTM1_OSTMnCTL_MD0);

    /* ---- OSTM compare register (CMP) setting ---- */
    OSTM1.OSTMnCMP = (uint32_t)(P0_CLOCK_FREQUENCY_kHz * cycle);

        /* Set timer in the cycle specified by the cycle */
        /* CMP = P0 clock (kHz) * cycle (msec)           */
        /* (Assume that P0 clock has 33.333MHz)          */

    /* ==== Register OS timer interrupt handler ==== */
    R_INTC_Regist_Int_Func(INTC_ID_OSTM1TINT, Sample_OSTM1_Interrupt);

    /* ==== Set priority of OS timer interrupt to 6 ==== */
    R_INTC_Set_Priority(INTC_ID_OSTM1TINT, 6);

    /* ==== Validate OS timer interrupt ==== */
    R_INTC_Enable(INTC_ID_OSTM1TINT);

    return DEVDRV_SUCCESS;
}

#endif /* USE_CH_OSTM1 */


/* End of File */

