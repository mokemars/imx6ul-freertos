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
* File Name     : riic_userdef.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : RIIC driver (User define function)
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
/* I/O Register root header */
#include "iodefine.h"
/* Device driver header */
#include "dev_drv.h"
/* RIIC Driver header */
#include "riic.h"
/* INTC Driver Header */
#include "intc.h"
/* I2C RSK+RZA1H Eval-board header */
#include "riic_rskrza1h.h"
/* Interchangeable compiler specific header */
#include "compiler_settings.h"
/* Low level register read/write header */
#include "rza_io_regrw.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
#define ICC0_MAX_TIMEOUT (20000)
#define ICC1_MAX_TIMEOUT (20000)
#define ICC2_MAX_TIMEOUT (20000)
#define ICC3_MAX_TIMEOUT (20000)

/******************************************************************************
Private global variables and functions
******************************************************************************/
static volatile uint8_t riic0_receive_full_flg   = DEVDRV_FLAG_OFF;
static volatile uint8_t riic0_transmit_empty_flg = DEVDRV_FLAG_OFF;
static volatile uint8_t riic0_transmit_end_flg   = DEVDRV_FLAG_OFF;

/* Channel 1 Not in use
static volatile uint8_t riic1_receive_full_flg   = DEVDRV_FLAG_OFF;
static volatile uint8_t riic1_transmit_empty_flg = DEVDRV_FLAG_OFF;
static volatile uint8_t riic1_transmit_end_flg   = DEVDRV_FLAG_OFF;
*/

/* Channel 2 Not in use
static volatile uint8_t riic2_receive_full_flg   = DEVDRV_FLAG_OFF;
static volatile uint8_t riic2_transmit_empty_flg = DEVDRV_FLAG_OFF;
static volatile uint8_t riic2_transmit_end_flg   = DEVDRV_FLAG_OFF;
*/
static volatile uint8_t riic3_receive_full_flg   = DEVDRV_FLAG_OFF;
static volatile uint8_t riic3_transmit_empty_flg = DEVDRV_FLAG_OFF;
static volatile uint8_t riic3_transmit_end_flg   = DEVDRV_FLAG_OFF;

/******************************************************************************
* Function Name: userdef_riic0_init
* Description  : User-defined function.
*              : Called to initialise the RIIC channel 0 peripheral
*              : Uses the following interrupts:
*              : Receive data full interrupt
*              : Transmit data empty interrupt
*              : Transmission complete interrupt
* Arguments    : uint32_t cks  : I2c Clock Selection
*              :               : (CKS bit setting in RIICnMR1 register)
*              :               :   RIIC_CKS_DIVISION_1   : P0φ/1 clock
*              :               :   RIIC_CKS_DIVISION_2   : P0φ/2 clock
*              :               :   RIIC_CKS_DIVISION_4   : P0φ/4 clock
*              :               :   RIIC_CKS_DIVISION_8   : P0φ/8 clock
*              :               :   RIIC_CKS_DIVISION_16  : P0φ/16 clock
*              :               :   RIIC_CKS_DIVISION_32  : P0φ/32 clock
*              :               :   RIIC_CKS_DIVISION_64  : P0φ/64 clock
*              :               :   RIIC_CKS_DIVISION_128 : P0φ/128 clock
*              : uint32_t brl  : Lower byte value of the width of the SCL clock
*              : uint32_t brh  : High byte value of the width of the SCL clock
* Return Value : none
******************************************************************************/
void userdef_riic0_init (uint32_t cks, uint32_t brl, uint32_t brh)
{
    volatile uint8_t dummy_buf = 0u;

    UNUSED_VARIABLE(dummy_buf);

    /* ==== Module standby clear ==== */
    /* ---- Supply clock to the RIIC(channel 0) ---- */
    rza_io_reg_write_8((uint8_t *)&CPG.STBCR9,
    		          0,
    		          CPG_STBCR9_MSTP97_SHIFT,
    		          CPG_STBCR9_MSTP97);

    dummy_buf = rza_io_reg_read_8((uint8_t *)&CPG.STBCR9,
    		                     CPG_STBCR9_MSTP97_SHIFT,
    		                     CPG_STBCR9_MSTP97);

    /* ==== RIIC initial setting ==== */
    /* Clear the RIICn reset bit */
    rza_io_reg_write_8(&(RIIC0.RIICnCR1.UINT8[0]),
    		            0,
    		            RIICn_RIICnCR1_ICE_SHIFT,
    		            RIICn_RIICnCR1_ICE);

    /* Set I2C Bus Interface Internal Reset */
    rza_io_reg_write_8(&(RIIC0.RIICnCR1.UINT8[0]),
    		            1,
    		            RIICn_RIICnCR1_IICRST_SHIFT,
    		            RIICn_RIICnCR1_IICRST);

    /* Set the RIICn reset bit */
    rza_io_reg_write_8(&(RIIC0.RIICnCR1.UINT8[0]),
    		            1,
    		            RIICn_RIICnCR1_ICE_SHIFT,
    		            RIICn_RIICnCR1_ICE);

    /* Set Internal Reference Clock (IICφ) Selection */
    rza_io_reg_write_8(&(RIIC0.RIICnMR1.UINT8[0]),
    		            (uint8_t)cks,
    		            RIICn_RIICnMR1_CKS_SHIFT,
    		            RIICn_RIICnMR1_CKS);

    /* Set Bit Rate Low-Level Period */
    rza_io_reg_write_8(&(RIIC0.RIICnBRL.UINT8[0]),
    		            (uint8_t)brl,
    		            RIICn_RIICnBRL_BRL_SHIFT,
    		            RIICn_RIICnBRL_BRL);

    /* Set Bit Rate High-Level Period */
    rza_io_reg_write_8(&(RIIC0.RIICnBRH.UINT8[0]),
                        (uint8_t)brh,
    		            RIICn_RIICnBRH_BRH_SHIFT,
    		            RIICn_RIICnBRH_BRH);

    /* Set NACK Reception Transfer Suspension Enable */
    rza_io_reg_write_8(&(RIIC0.RIICnFER.UINT8[0]),
    		            1,
    		            RIICn_RIICnFER_NACKE_SHIFT,
    		            RIICn_RIICnFER_NACKE);

    /* Set Receive Data Full Interrupt Enable */
    rza_io_reg_write_8(&(RIIC0.RIICnIER.UINT8[0]),
    		            1,
    		            RIICn_RIICnIER_RIE_SHIFT,
    		            RIICn_RIICnIER_RIE);

    /* Set Transmit Data Empty Interrupt Enable */
    rza_io_reg_write_8(&(RIIC0.RIICnIER.UINT8[0]),
    		            1,
    		            RIICn_RIICnIER_TIE_SHIFT,
    		            RIICn_RIICnIER_TIE);

    /*Set Transmit End Interrupt Enable */
    rza_io_reg_write_8(&(RIIC0.RIICnIER.UINT8[0]),
    		            1,
    		            RIICn_RIICnIER_TEIE_SHIFT,
    		            RIICn_RIICnIER_TEIE);

    /* Clear I2C Bus Interface Internal Reset */
    rza_io_reg_write_8(&(RIIC0.RIICnCR1.UINT8[0]),
    		            0,
    		            RIICn_RIICnCR1_IICRST_SHIFT,
    		            RIICn_RIICnCR1_IICRST);

    /* Initialise flags */
    riic0_receive_full_flg   = DEVDRV_FLAG_OFF;
    riic0_transmit_empty_flg = DEVDRV_FLAG_OFF;
    riic0_transmit_end_flg   = DEVDRV_FLAG_OFF;

    /* Register active interrupts */
    R_INTC_Regist_Int_Func(INTC_ID_INTIICRI0,  Sample_RIIC_Ri0_Interrupt);
    R_INTC_Regist_Int_Func(INTC_ID_INTIICTI0,  Sample_RIIC_Ti0_Interrupt);
    R_INTC_Regist_Int_Func(INTC_ID_INTIICTEI0, Sample_RIIC_Tei0_Interrupt);

    /* Set active interrupt priorities */
    R_INTC_Set_Priority(INTC_ID_INTIICRI0,  9);
    R_INTC_Set_Priority(INTC_ID_INTIICTI0,  9);
    R_INTC_Set_Priority(INTC_ID_INTIICTEI0, 9);

    /* Enable active interrupts */
    R_INTC_Enable(INTC_ID_INTIICRI0);
    R_INTC_Enable(INTC_ID_INTIICTI0);
    R_INTC_Enable(INTC_ID_INTIICTEI0);
}

/******************************************************************************
* Function Name: userdef_riic1_Init
* Description  : User-defined function.
*              : Called to initialise the RIIC channel 1 peripheral
*              : Uses the following interrupts:
*              : Receive data full interrupt
*              : Transmit data empty interrupt
*              : Transmission complete interrupt
* Arguments    : uint32_t cks  : I2c Clock Selection
*              :               : (Settings in the CKS bit of RIICnMR1 register)
*              :               :   RIIC_CKS_DIVISION_1   : P0φ/1 clock
*              :               :   RIIC_CKS_DIVISION_2   : P0φ/2 clock
*              :               :   RIIC_CKS_DIVISION_4   : P0φ/4 clock
*              :               :   RIIC_CKS_DIVISION_8   : P0φ/8 clock
*              :               :   RIIC_CKS_DIVISION_16  : P0φ/16 clock
*              :               :   RIIC_CKS_DIVISION_32  : P0φ/32 clock
*              :               :   RIIC_CKS_DIVISION_64  : P0φ/64 clock
*              :               :   RIIC_CKS_DIVISION_128 : P0φ/128 clock
*              : uint32_t brl  : Lower byte value of the width of the SCL clock
*              : uint32_t brh  : High byte value of the width of the SCL clock
* Return Value : none
******************************************************************************/
void userdef_riic1_init (uint32_t cks, uint32_t brl, uint32_t brh)
{
	/* Channel 1 is unused */
}

/******************************************************************************
* Function Name: userdef_riic2_Init
* Description  : User-defined function.
*              : Called to initialise the RIIC channel 2 peripheral
*              : Uses the following interrupts:
*              : Receive data full interrupt
*              : Transmit data empty interrupt
*              : Transmission complete interrupt
* Arguments    : uint32_t cks  : I2c Clock Selection
*              :               : (Settings in the CKS bit of RIICnMR1 register)
*              :               :   RIIC_CKS_DIVISION_1   : P0φ/1 clock
*              :               :   RIIC_CKS_DIVISION_2   : P0φ/2 clock
*              :               :   RIIC_CKS_DIVISION_4   : P0φ/4 clock
*              :               :   RIIC_CKS_DIVISION_8   : P0φ/8 clock
*              :               :   RIIC_CKS_DIVISION_16  : P0φ/16 clock
*              :               :   RIIC_CKS_DIVISION_32  : P0φ/32 clock
*              :               :   RIIC_CKS_DIVISION_64  : P0φ/64 clock
*              :               :   RIIC_CKS_DIVISION_128 : P0φ/128 clock
*              : uint32_t brl  : Lower byte value of the width of the SCL clock
*              : uint32_t brh  : High byte value of the width of the SCL clock
* Return Value : none
******************************************************************************/
void userdef_riic2_init (uint32_t cks, uint32_t brl, uint32_t brh)
{
	/* Channel 2 is unused */
}

/******************************************************************************
* Function Name: userdef_riic3_init
* Description  : User-defined function.
*              : Called to initialise the RIIC channel 3 peripheral
*              : Uses the following interrupts:
*              : Receive data full interrupt
*              : Transmit data empty interrupt
*              : Transmission complete interrupt
* Arguments    : uint32_t cks  : I2c Clock Selection
*              :               : (Settings in the CKS bit of RIICnMR1 register)
*              :               :   RIIC_CKS_DIVISION_1   : P0φ/1 clock
*              :               :   RIIC_CKS_DIVISION_2   : P0φ/2 clock
*              :               :   RIIC_CKS_DIVISION_4   : P0φ/4 clock
*              :               :   RIIC_CKS_DIVISION_8   : P0φ/8 clock
*              :               :   RIIC_CKS_DIVISION_16  : P0φ/16 clock
*              :               :   RIIC_CKS_DIVISION_32  : P0φ/32 clock
*              :               :   RIIC_CKS_DIVISION_64  : P0φ/64 clock
*              :               :   RIIC_CKS_DIVISION_128 : P0φ/128 clock
*              : uint32_t brl  : Lower byte value of the width of the SCL clock
*              : uint32_t brh  : High byte value of the width of the SCL clock
* Return Value : none
******************************************************************************/
void userdef_riic3_init (uint32_t cks, uint32_t brl, uint32_t brh)
{
    volatile uint8_t dummy_buf = 0u;

    UNUSED_VARIABLE(dummy_buf);

    /* ==== Module standby clear ==== */

    /* Clear the RIICn reset bit */
    rza_io_reg_write_8(&(RIIC3.RIICnCR1.UINT8[0]),
    		            0,
    		            RIICn_RIICnCR1_ICE_SHIFT,
    		            RIICn_RIICnCR1_ICE);

    /* Set I2C Bus Interface Internal Reset */
    rza_io_reg_write_8(&(RIIC3.RIICnCR1.UINT8[0]),
    		            1,
    		            RIICn_RIICnCR1_IICRST_SHIFT,
    		            RIICn_RIICnCR1_IICRST);

    /* Set the RIICn reset bit */
    rza_io_reg_write_8(&(RIIC3.RIICnCR1.UINT8[0]),
    		            1,
    		            RIICn_RIICnCR1_ICE_SHIFT,
    		            RIICn_RIICnCR1_ICE);

    /* Set Internal Reference Clock (IICφ) Selection */
    rza_io_reg_write_8(&(RIIC3.RIICnMR1.UINT8[0]),
                        (uint8_t)cks,
    		            RIICn_RIICnMR1_CKS_SHIFT,
    		            RIICn_RIICnMR1_CKS);

    /* Set Bit Rate Low-Level Period */
    rza_io_reg_write_8(&(RIIC3.RIICnBRL.UINT8[0]),
    		            (uint8_t)brl,
    		            RIICn_RIICnBRL_BRL_SHIFT,
    		            RIICn_RIICnBRL_BRL);

    /* Set Bit Rate High-Level Period */
    rza_io_reg_write_8(&(RIIC3.RIICnBRH.UINT8[0]),
                        (uint8_t)brh,
    		            RIICn_RIICnBRH_BRH_SHIFT,
    		            RIICn_RIICnBRH_BRH);

    /* Set NACK Reception Transfer Suspension Enable */
    rza_io_reg_write_8(&(RIIC3.RIICnFER.UINT8[0]),
    		            1,
    		            RIICn_RIICnFER_NACKE_SHIFT,
    		            RIICn_RIICnFER_NACKE);

    /* Set Receive Data Full Interrupt Enable */
    rza_io_reg_write_8(&(RIIC3.RIICnIER.UINT8[0]),
    		            1,
    		            RIICn_RIICnIER_RIE_SHIFT,
    		            RIICn_RIICnIER_RIE);

    /* Set Transmit Data Empty Interrupt Enable */
    rza_io_reg_write_8(&(RIIC3.RIICnIER.UINT8[0]),
    		            1,
    		            RIICn_RIICnIER_TIE_SHIFT,
    		            RIICn_RIICnIER_TIE);

    /*Set Transmit End Interrupt Enable */
    rza_io_reg_write_8(&(RIIC3.RIICnIER.UINT8[0]),
    		            1,
    		            RIICn_RIICnIER_TEIE_SHIFT,
    		            RIICn_RIICnIER_TEIE);

    /* Clear I2C Bus Interface Internal Reset */
    rza_io_reg_write_8(&(RIIC3.RIICnCR1.UINT8[0]),
    		            0,
    		            RIICn_RIICnCR1_IICRST_SHIFT,
    		            RIICn_RIICnCR1_IICRST);

    /* Register active interrupts */
    R_INTC_Regist_Int_Func(INTC_ID_INTIICRI3,  Sample_RIIC_Ri3_Interrupt);
    R_INTC_Regist_Int_Func(INTC_ID_INTIICTI3,  Sample_RIIC_Ti3_Interrupt);
    R_INTC_Regist_Int_Func(INTC_ID_INTIICTEI3, Sample_RIIC_Tei3_Interrupt);

    /* Set active interrupt priorities */
    R_INTC_Set_Priority(INTC_ID_INTIICRI3,  9);
    R_INTC_Set_Priority(INTC_ID_INTIICTI3,  9);
    R_INTC_Set_Priority(INTC_ID_INTIICTEI3, 9);

    /* Enable active interrupts */
    R_INTC_Enable(INTC_ID_INTIICRI3);
    R_INTC_Enable(INTC_ID_INTIICTI3);
    R_INTC_Enable(INTC_ID_INTIICTEI3);
}

/******************************************************************************
* Function Name: userdef_riic0_init_rx_full
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICRI for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic0_init_rx_full (void)
{
    riic0_receive_full_flg = DEVDRV_FLAG_OFF;
}

/******************************************************************************
* Function Name: userdef_riic1_init_rx_full
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICRI for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic1_init_rx_full (void)
{
	/* Channel 1 is unused */
}

/******************************************************************************
* Function Name: userdef_riic2_init_rx_full
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICRI for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic2_init_rx_full (void)
{
	/* Channel 2 is unused */
}

/******************************************************************************
* Function Name: userdef_riic3_init_rx_full
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICRI for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic3_init_rx_full (void)
{
	riic3_receive_full_flg = DEVDRV_FLAG_OFF;
}

/******************************************************************************
* Function Name: userdef_riic0_init_tx_empty
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICTI for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic0_init_tx_empty (void)
{
    riic0_transmit_empty_flg = DEVDRV_FLAG_OFF;
}

/******************************************************************************
* Function Name: userdef_riic1_init_tx_empty
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICTI for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic1_init_tx_empty (void)
{
	/* Channel 1 is unused */
}

/******************************************************************************
* Function Name: userdef_riic2_init_tx_empty
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICTI for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic2_init_tx_empty (void)
{
	/* Channel 2 is unused */
}

/******************************************************************************
* Function Name: userdef_riic3_init_tx_empty
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICTI for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic3_init_tx_empty (void)
{
	riic3_transmit_empty_flg = DEVDRV_FLAG_OFF;
}

/******************************************************************************
* Function Name: userdef_riic0_init_tx_end
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICTEI for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic0_init_tx_end (void)
{
    riic0_transmit_end_flg = DEVDRV_FLAG_OFF;
}

/******************************************************************************
* Function Name: userdef_riic1_init_tx_end
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICTEI for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic1_init_tx_end (void)
{
	/* Channel 1 is unused */
}

/******************************************************************************
* Function Name: userdef_riic2_init_tx_end
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICTEI for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic2_init_tx_end (void)
{
	/* Channel 2 is unused */
}

/******************************************************************************
* Function Name: userdef_riic3_init_tx_end
* Description  : User-defined function.
*              : Called to process INTC_ID_INTIICTEI for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic3_init_tx_end (void)
{
	riic3_transmit_end_flg = DEVDRV_FLAG_OFF;
}

/******************************************************************************
* Function Name: userdef_riic0_set_rx_full
* Description  : User-defined function.
*              : Set receive full flag for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic0_set_rx_full (void)
{
    riic0_receive_full_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: userdef_riic1_set_rx_full
* Description  : User-defined function.
*              : Set receive full flag for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic1_set_rx_full (void)
{
	/* Channel 1 is unused */
}

/******************************************************************************
* Function Name: userdef_riic2_set_rx_full
* Description  : User-defined function.
*              : Set receive full flag for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic2_set_rx_full (void)
{
	/* Channel 2 is unused */
}

/******************************************************************************
* Function Name: userdef_riic3_set_rx_full
* Description  : User-defined function.
*              : Set receive full flag for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic3_set_rx_full (void)
{
	riic3_receive_full_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: userdef_riic0_set_tx_empty
* Description  : User-defined function.
*              : Set transmit empty flag for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic0_set_tx_empty (void)
{
    riic0_transmit_empty_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: userdef_riic1_set_tx_empty
* Description  : User-defined function.
*              : Set transmit empty flag for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic1_set_tx_empty (void)
{
	/* Channel 1 is unused */
}

/******************************************************************************
* Function Name: userdef_riic2_set_tx_empty
* Description  : User-defined function.
*              : Set transmit empty flag for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic2_set_tx_empty (void)
{
	/* Channel 2 is unused */
}

/******************************************************************************
* Function Name: userdef_riic3_set_tx_empty
* Description  : User-defined function.
*              : Set transmit empty flag for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic3_set_tx_empty (void)
{
	riic3_transmit_empty_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: userdef_riic0_set_tx_end
* Description  : User-defined function.
*              : Set transmit end flag for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic0_set_tx_end (void)
{
    riic0_transmit_end_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: userdef_riic1_set_tx_end
* Description  : User-defined function.
*              : Set transmit end flag for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic1_set_tx_end (void)
{
	/* Channel 1 is unused */
}

/******************************************************************************
* Function Name: userdef_riic2_set_tx_end
* Description  : User-defined function.
*              : Set transmit end flag for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic2_set_tx_end (void)
{
	/* Channel 2 is unused */
}

/******************************************************************************
* Function Name: userdef_riic3_set_tx_end
* Description  : User-defined function.
*              : Set transmit end flag for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic3_set_tx_end (void)
{
	riic3_transmit_end_flg = DEVDRV_FLAG_ON;
}

/******************************************************************************
* Function Name: userdef_riic0_wait_rx_full
* Description  : User-defined function.
*              : Wait in function till receive full flag set for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic0_wait_rx_full (void)
{
	/* Wait */
    while ( (DEVDRV_FLAG_OFF == riic0_receive_full_flg))
    {
    	__asm__("nop");
    }
}

/******************************************************************************
* Function Name: userdef_riic1_wait_rx_full
* Description  : User-defined function.
*              : Wait in function till receive full flag set for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic1_wait_rx_full (void)
{
	/* Channel 1 is unused */
}

/******************************************************************************
* Function Name: userdef_riic2_wait_rx_full
* Description  : User-defined function.
*              : Wait in function till receive full flag set for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic2_wait_rx_full (void)
{
	/* Channel 2 is unused */
}

/******************************************************************************
* Function Name: userdef_riic3_wait_rx_full
* Description  : User-defined function.
*              : Wait in function till receive full flag set for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic3_wait_rx_full (void)
{
	/* Wait */
    while ( (DEVDRV_FLAG_OFF == riic3_receive_full_flg))
    {
    	__asm__("nop");
    }
}

/******************************************************************************
* Function Name: userdef_riic0_wait_tx_empty
* Description  : User-defined function.
*              : Wait in function till transmit empty flag set for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic0_wait_tx_empty (void)
{
	/* Wait */
    while ( (DEVDRV_FLAG_OFF == riic0_transmit_empty_flg))
    {
    	__asm__("nop");
    }
}

/******************************************************************************
* Function Name: userdef_riic1_wait_tx_empty
* Description  : User-defined function.
*              : Wait in function till transmit empty flag set for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic1_wait_tx_empty (void)
{
	/* Channel 1 is unused */
}

/******************************************************************************
* Function Name: userdef_riic2_wait_tx_empty
* Description  : User-defined function.
*              : Wait in function till transmit empty flag set for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic2_wait_tx_empty (void)
{
	/* Channel 2 is unused */
}

/******************************************************************************
* Function Name: userdef_riic3_wait_tx_empty
* Description  : User-defined function.
*              : Wait in function till transmit empty flag set for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic3_wait_tx_empty (void)
{
	/* Wait */
    while (DEVDRV_FLAG_OFF == riic3_transmit_empty_flg)
    {
    	__asm__("nop");
    }
}

/******************************************************************************
* Function Name: userdef_riic0_wait_tx_end
* Description  : User-defined function.
*              : Wait in function till transmit end flag set for channel 0
* Arguments    : uint32_t mode : RIIC_TEND_WAIT_TRANSMIT : Transmission Mode
*              :               : RIIC_TEND_WAIT_RECEIVE  : Receive Mode
* Return Value : none
******************************************************************************/
void userdef_riic0_wait_tx_end (uint32_t mode)
{
	int32_t timeout = ICC0_MAX_TIMEOUT;

    if (RIIC_TEND_WAIT_TRANSMIT == mode)
    {
    	/* Wait */
        while ( (DEVDRV_FLAG_OFF == riic0_transmit_end_flg))
        {
        	__asm__("nop");
        }
    }
    else
    {
        /* Wait */
        while ( (DEVDRV_FLAG_OFF == riic0_receive_full_flg))
        {
        	__asm__("nop");
        }
    }
}

/******************************************************************************
* Description  : User-defined function.
*              : Wait in function till transmit end flag set for channel 1
* Arguments    : uint32_t mode : RIIC_TEND_WAIT_TRANSMIT : Transmission Mode
*              :               : RIIC_TEND_WAIT_RECEIVE  : Receive Mode
* Return Value : none
******************************************************************************/
void userdef_riic1_wait_tx_end (uint32_t mode)
{
	/* Channel 1 is unused */
}

/******************************************************************************
* Function Name: userdef_riic2_wait_tx_end
* Description  : User-defined function.
*              : Wait in function till transmit end flag set for channel 1
* Arguments    : uint32_t mode : RIIC_TEND_WAIT_TRANSMIT : Transmission Mode
*              :               : RIIC_TEND_WAIT_RECEIVE  : Receive Mode
* Return Value : none
******************************************************************************/
void userdef_riic2_wait_tx_end (uint32_t mode)
{
	/* Channel 2 is unused */
}

/******************************************************************************
* Function Name: userdef_riic3_wait_tx_end
* Description  : User-defined function.
*              : Wait in function till transmit end flag set for channel 3
* Arguments    : uint32_t mode : RIIC_TEND_WAIT_TRANSMIT : Transmission Mode
*              :               : RIIC_TEND_WAIT_RECEIVE  : Receive Mode
* Return Value : none
******************************************************************************/
void userdef_riic3_wait_tx_end (uint32_t mode)
{
	int32_t timeout = ICC3_MAX_TIMEOUT;

    if (RIIC_TEND_WAIT_TRANSMIT == mode)
    {
    	/* Wait */
        while ( (DEVDRV_FLAG_OFF == riic3_transmit_end_flg))
        {
        	__asm__("nop");

        }
    }
    else
    {
        /* Wait */
        while ( (DEVDRV_FLAG_OFF == riic3_receive_full_flg))
        {
        	__asm__("nop");
        }
    }
}

/******************************************************************************
* Function Name: userdef_riic0_wait_bus_master
* Description  : User-defined function.
*              : Wait for bus busy or bus free state for channel 0
* Arguments    : uint32_t mode
*              :        : RIIC_BUS_MASTERSHIP_WAIT_FREE : wait till bus free
*              :        : RIIC_BUS_MASTERSHIP_WAIT_BUSY : wait till bus busy
* Return Value : none
******************************************************************************/
void userdef_riic0_wait_bus_master (uint32_t mode)
{
	int32_t timeout = ICC0_MAX_TIMEOUT;
    if (RIIC_BUS_MASTERSHIP_WAIT_FREE == mode)
    {
        while (1 == rza_io_reg_read_8(&(RIIC0.RIICnCR2.UINT8[0]),
                                       RIICn_RIICnCR2_BBSY_SHIFT,
                                       RIICn_RIICnCR2_BBSY))
        {
            /* Wait */
        	__asm__("nop");
        }
    }
    else
    {
        while (0 == rza_io_reg_read_8(&(RIIC0.RIICnCR2.UINT8[0]),
            		                   RIICn_RIICnCR2_BBSY_SHIFT,
            		                   RIICn_RIICnCR2_BBSY))
        {
            /* Wait */
        	__asm__("nop");
        }
    }
}

/******************************************************************************
* Function Name: userdef_riic1_wait_bus_master
* Description  : User-defined function.
*              : Wait for bus busy or bus free state for channel 1
* Arguments    : uint32_t mode
*              :        : RIIC_BUS_MASTERSHIP_WAIT_FREE : wait till bus free
*              :        : RIIC_BUS_MASTERSHIP_WAIT_BUSY : wait till bus busy
* Return Value : none
******************************************************************************/
void userdef_riic1_wait_bus_master (uint32_t mode)
{
	/* Channel 1 is unused */
}

/******************************************************************************
* Function Name: userdef_riic2_wait_bus_master
* Description  : User-defined function.
*              : Wait for bus busy or bus free state for channel 2
* Arguments    : uint32_t mode
*              :        : RIIC_BUS_MASTERSHIP_WAIT_FREE : wait till bus free
*              :        : RIIC_BUS_MASTERSHIP_WAIT_BUSY : wait till bus busy
* Return Value : none
******************************************************************************/
void userdef_riic2_wait_bus_master (uint32_t mode)
{
	/* Channel 2 is unused */
}

/******************************************************************************
* Function Name: userdef_riic3_wait_bus_master
* Description  : User-defined function.
*              : Wait for bus busy or bus free state for channel 3
* Arguments    : uint32_t mode
*              :        : RIIC_BUS_MASTERSHIP_WAIT_FREE : wait till bus free
*              :        : RIIC_BUS_MASTERSHIP_WAIT_BUSY : wait till bus busy
* Return Value : none
******************************************************************************/
void userdef_riic3_wait_bus_master (uint32_t mode)
{
	if (RIIC_BUS_MASTERSHIP_WAIT_FREE == mode)
    {
    	/* Wait till bus free */
        while (1 == rza_io_reg_read_8(&(RIIC3.RIICnCR2.UINT8[0]),
            		                   RIICn_RIICnCR2_BBSY_SHIFT,
            		                   RIICn_RIICnCR2_BBSY))
        {
            /* Wait */
        	__asm__("nop");
        }
    }
    else
    {
    	/* Wait till bus busy */
        while (0 == rza_io_reg_read_8(&(RIIC3.RIICnCR2.UINT8[0]),
                                       RIICn_RIICnCR2_BBSY_SHIFT,
                                       RIICn_RIICnCR2_BBSY))
        {
            /* Wait */
        	__asm__("nop");
        }
    }
}

/******************************************************************************
* Function Name: userdef_riic0_wait_stop
* Description  : User-defined function.
*              : Wait for stop bit state for channel 0
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic0_wait_stop (void)
{
	/* Wait */
    while (0 == rza_io_reg_read_8(&(RIIC0.RIICnSR2.UINT8[0]),
                                   RIICn_RIICnSR2_STOP_SHIFT,
                                   RIICn_RIICnSR2_STOP))
    {
    	__asm__("nop");
    }
}

/******************************************************************************
* Function Name: userdef_riic1_wait_stop
* Description  : User-defined function.
*              : Wait for stop bit state for channel 1
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic1_wait_stop (void)
{
	/* Channel 1 is unused */
}

/******************************************************************************
* Function Name: userdef_riic2_wait_stop
* Description  : User-defined function.
*              : Wait for stop bit state for channel 2
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic2_wait_stop (void)
{
	/* Channel 2 is unused */
}

/******************************************************************************
* Function Name: userdef_riic3_wait_stop
* Description  : User-defined function.
*              : Wait for stop bit state for channel 3
* Arguments    : none
* Return Value : none
******************************************************************************/
void userdef_riic3_wait_stop (void)
{
	/* Wait */
    while (0 == rza_io_reg_read_8(&(RIIC3.RIICnSR2.UINT8[0]),
        		                   RIICn_RIICnSR2_STOP_SHIFT,
        		                   RIICn_RIICnSR2_STOP))
    {
    	__asm__("nop");
    }
}


/* End of File */

