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
* File Name     : peripheral_init_basic.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Sample Program - Initialise peripheral function sample
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
/* Common Driver header */
#include "bsc_userdef.h"
/* I/O Register root header */
#include "iodefine.h"
/* Peripheral startup interface header */
#include "peripheral_init_basic.h"
/* Low level register read/write header */
#include "rza_io_regrw.h"
/* Interchangeable compiler specific header */
#include "compiler_settings.h"


/*******************************************************************************
Private global variables and functions
*******************************************************************************/
static void CPG_Init(void);
static void CS0_PORT_Init(void);


/*******************************************************************************
* Function Name: Peripheral_Basic_Init
* Description  : Calls the CPG_Init and the CS0_PORT_Init of sample functions
*              : and the BSC_Init of API function. Sets the CPG, enables
*              : writing to the data-retention RAM area, and executes the 
*              : port settings for the CS0 and the CS1 spaces and the BSC 
*              : setting.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void Peripheral_Basic_Init (void)
{
    /* ==== CPG setting ====*/
    CPG_Init();

    /* ==== Port setting ==== */
    CS0_PORT_Init();

    /* ==== BSC setting ==== */
    /* Initialise rest of BSC later in startup sequence */
    BSC_Init((uint8_t)(BSC_AREA_CS0 | BSC_AREA_CS1));
}

/*******************************************************************************
* Function Name: CPG_Init
* Description  : Executes initial setting for the CPG.
*              : In the sample code, the internal clock ratio is set to be 
*              : I:G:B:P1:P0 = 30:20:10:5:5/2 in the state that the 
*              : clock mode is 0. The frequency is set to be as below when the
*              : input clock is 13.33MHz.
*              : CPU clock (I clock)              : 400MHz
*              : Image processing clock (G clock) : 266.67MHz
*              : Internal bus clock (B clock)     : 133.33MHz
*              : Peripheral clock1 (P1 clock)     : 66.67MHz
*              : Peripheral clock0 (P0 clock)     : 33.33MHz
*              : Sets the data-retention RAM area (H'2000 0000 to H'2001 FFFF)
*              : to be enabled for writing.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void CPG_Init (void)
{
    volatile uint32_t dummy = 0;

    UNUSED_VARIABLE(dummy);

    /* standby_mode_en bit of Power Control Register setting */
    (*(volatile uint32_t *)(0x3fffff80)) = 0x00000001;
    dummy = (*(volatile uint32_t *)(0x3fffff80));

    /* ==== CPG Settings ==== */

    /* PLL(x30), I:G:B:P1:P0 = 30:20:10:5:5/2 */
    CPG.FRQCR  = 0x1035u;

    /* CKIO:Output at time usually output     *
     * when bus right is opened output at     *
     * standby "L"                            *
	 * Clockin  = 13.33MHz, CKIO = 66.67MHz,  *
	 * I  Clock = 400.00MHz,                  *
	 * G  Clock = 266.67MHz,                  *
	 * B  Clock = 133.33MHz,                  *
	 * P1 Clock =  66.67MHz,                  *
	 * P0 Clock =  33.33MHz                   */
    CPG.FRQCR2 = 0x0001u;

    /* ----  Writing to On-Chip Data-Retention RAM is enabled. ---- */
    CPG.SYSCR3 = 0x0Fu;
    dummy = CPG.SYSCR3;
}

/*******************************************************************************
* Function Name: CS0_PORT_Init
* Description  : Sets the PORT multiplexed pin to use the CS0 and the CS1 
*              : spaces. In this sample code, the PORT setting is executed to 
*              : use the NOR flash memory in the CS0 and the CS1 spaces.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void CS0_PORT_Init (void)
{
    /* ==== BSC settings ==== */

	/* ---- P3_7 : CS1 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 7th multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE3, 1, GPIO_PFCAE3_PFCAE37_SHIFT, GPIO_PFCAE3_PFCAE37);
    rza_io_reg_write_16(&GPIO.PFCE3,  1, GPIO_PFCE3_PFCE37_SHIFT,   GPIO_PFCE3_PFCE37);
    rza_io_reg_write_16(&GPIO.PFC3,   0, GPIO_PFC3_PFC37_SHIFT,     GPIO_PFC3_PFC37);
    rza_io_reg_write_16(&GPIO.PMC3,   1, GPIO_PMC3_PMC37_SHIFT,     GPIO_PMC3_PMC37);
    rza_io_reg_write_16(&GPIO.PIPC3,  1, GPIO_PIPC3_PIPC37_SHIFT,   GPIO_PIPC3_PIPC37);

    /* ---- P7_0 : CS0 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE70_SHIFT, GPIO_PFCAE7_PFCAE70);
    rza_io_reg_write_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE70_SHIFT,   GPIO_PFCE7_PFCE70);
    rza_io_reg_write_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC70_SHIFT,     GPIO_PFC7_PFC70);
    rza_io_reg_write_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC70_SHIFT,     GPIO_PMC7_PMC70);
    rza_io_reg_write_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC70_SHIFT,   GPIO_PIPC7_PIPC70);

    /* ---- P7_6 : WE0# / DQMLL# ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE76_SHIFT, GPIO_PFCAE7_PFCAE76);
    rza_io_reg_write_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE76_SHIFT,   GPIO_PFCE7_PFCE76);
    rza_io_reg_write_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC76_SHIFT,     GPIO_PFC7_PFC76);
    rza_io_reg_write_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC76_SHIFT,     GPIO_PMC7_PMC76);
    rza_io_reg_write_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC76_SHIFT,   GPIO_PIPC7_PIPC76);

    /* ---- P7_8 : RD ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE78_SHIFT, GPIO_PFCAE7_PFCAE78);
    rza_io_reg_write_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE78_SHIFT,   GPIO_PFCE7_PFCE78);
    rza_io_reg_write_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC78_SHIFT,     GPIO_PFC7_PFC78);
    rza_io_reg_write_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC78_SHIFT,     GPIO_PMC7_PMC78);
    rza_io_reg_write_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC78_SHIFT,   GPIO_PIPC7_PIPC78);

#ifdef USE_SDRAM_MODE

    /* ---- P6_0 : D0 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE60_SHIFT, GPIO_PFCAE6_PFCAE60);
    rza_io_reg_write_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE60_SHIFT,   GPIO_PFCE6_PFCE60);
    rza_io_reg_write_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC60_SHIFT,     GPIO_PFC6_PFC60);
    rza_io_reg_write_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC60_SHIFT,     GPIO_PMC6_PMC60);
    rza_io_reg_write_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC60_SHIFT,   GPIO_PIPC6_PIPC60);

    /* ---- P6_1 : D1 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE61_SHIFT, GPIO_PFCAE6_PFCAE61);
    rza_io_reg_write_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE61_SHIFT,   GPIO_PFCE6_PFCE61);
    rza_io_reg_write_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC61_SHIFT,     GPIO_PFC6_PFC61);
    rza_io_reg_write_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC61_SHIFT,     GPIO_PMC6_PMC61);
    rza_io_reg_write_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC61_SHIFT,   GPIO_PIPC6_PIPC61);

    /* ---- P6_2 : D2 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE62_SHIFT, GPIO_PFCAE6_PFCAE62);
    rza_io_reg_write_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE62_SHIFT,   GPIO_PFCE6_PFCE62);
    rza_io_reg_write_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC62_SHIFT,     GPIO_PFC6_PFC62);
    rza_io_reg_write_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC62_SHIFT,     GPIO_PMC6_PMC62);
    rza_io_reg_write_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC62_SHIFT,   GPIO_PIPC6_PIPC62);

    /* ---- P6_3 : D3 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE63_SHIFT, GPIO_PFCAE6_PFCAE63);
    rza_io_reg_write_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE63_SHIFT,   GPIO_PFCE6_PFCE63);
    rza_io_reg_write_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC63_SHIFT,     GPIO_PFC6_PFC63);
    rza_io_reg_write_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC63_SHIFT,     GPIO_PMC6_PMC63);
    rza_io_reg_write_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC63_SHIFT,   GPIO_PIPC6_PIPC63);

    /* ---- P6_4 : D4 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE64_SHIFT, GPIO_PFCAE6_PFCAE64);
    rza_io_reg_write_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE64_SHIFT,   GPIO_PFCE6_PFCE64);
    rza_io_reg_write_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC64_SHIFT,     GPIO_PFC6_PFC64);
    rza_io_reg_write_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC64_SHIFT,     GPIO_PMC6_PMC64);
    rza_io_reg_write_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC64_SHIFT,   GPIO_PIPC6_PIPC64);

    /* ---- P6_5 : D5 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE65_SHIFT, GPIO_PFCAE6_PFCAE65);
    rza_io_reg_write_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE65_SHIFT,   GPIO_PFCE6_PFCE65);
    rza_io_reg_write_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC65_SHIFT,     GPIO_PFC6_PFC65);
    rza_io_reg_write_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC65_SHIFT,     GPIO_PMC6_PMC65);
    rza_io_reg_write_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC65_SHIFT,   GPIO_PIPC6_PIPC65);

    /* ---- P6_6 : D6 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE66_SHIFT, GPIO_PFCAE6_PFCAE66);
    rza_io_reg_write_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE66_SHIFT,   GPIO_PFCE6_PFCE66);
    rza_io_reg_write_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC66_SHIFT,     GPIO_PFC6_PFC66);
    rza_io_reg_write_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC66_SHIFT,     GPIO_PMC6_PMC66);
    rza_io_reg_write_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC66_SHIFT,   GPIO_PIPC6_PIPC66);

    /* ---- P6_7 : D7 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE67_SHIFT, GPIO_PFCAE6_PFCAE67);
    rza_io_reg_write_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE67_SHIFT,   GPIO_PFCE6_PFCE67);
    rza_io_reg_write_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC67_SHIFT,     GPIO_PFC6_PFC67);
    rza_io_reg_write_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC67_SHIFT,     GPIO_PMC6_PMC67);
    rza_io_reg_write_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC67_SHIFT,   GPIO_PIPC6_PIPC67);

    /* ---- P6_8 : D8 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE68_SHIFT, GPIO_PFCAE6_PFCAE68);
    rza_io_reg_write_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE68_SHIFT,   GPIO_PFCE6_PFCE68);
    rza_io_reg_write_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC68_SHIFT,     GPIO_PFC6_PFC68);
    rza_io_reg_write_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC68_SHIFT,     GPIO_PMC6_PMC68);
    rza_io_reg_write_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC68_SHIFT,   GPIO_PIPC6_PIPC68);

    /* ---- P6_9 : D9 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE69_SHIFT, GPIO_PFCAE6_PFCAE69);
    rza_io_reg_write_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE69_SHIFT,   GPIO_PFCE6_PFCE69);
    rza_io_reg_write_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC69_SHIFT,     GPIO_PFC6_PFC69);
    rza_io_reg_write_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC69_SHIFT,     GPIO_PMC6_PMC69);
    rza_io_reg_write_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC69_SHIFT,   GPIO_PIPC6_PIPC69);

    /* ---- P6_10 : D10 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE610_SHIFT, GPIO_PFCAE6_PFCAE610);
    rza_io_reg_write_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE610_SHIFT,   GPIO_PFCE6_PFCE610);
    rza_io_reg_write_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC610_SHIFT,     GPIO_PFC6_PFC610);
    rza_io_reg_write_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC610_SHIFT,     GPIO_PMC6_PMC610);
    rza_io_reg_write_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC610_SHIFT,   GPIO_PIPC6_PIPC610);

    /* ---- P6_11 : D11 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE611_SHIFT, GPIO_PFCAE6_PFCAE611);
    rza_io_reg_write_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE611_SHIFT,   GPIO_PFCE6_PFCE611);
    rza_io_reg_write_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC611_SHIFT,     GPIO_PFC6_PFC611);
    rza_io_reg_write_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC611_SHIFT,     GPIO_PMC6_PMC611);
    rza_io_reg_write_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC611_SHIFT,   GPIO_PIPC6_PIPC611);

    /* ---- P6_12 : D12 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE612_SHIFT, GPIO_PFCAE6_PFCAE612);
    rza_io_reg_write_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE612_SHIFT,   GPIO_PFCE6_PFCE612);
    rza_io_reg_write_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC612_SHIFT,     GPIO_PFC6_PFC612);
    rza_io_reg_write_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC612_SHIFT,     GPIO_PMC6_PMC612);
    rza_io_reg_write_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC612_SHIFT,   GPIO_PIPC6_PIPC612);

    /* ---- P6_13 : D13 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE613_SHIFT, GPIO_PFCAE6_PFCAE613);
    rza_io_reg_write_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE613_SHIFT,   GPIO_PFCE6_PFCE613);
    rza_io_reg_write_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC613_SHIFT,     GPIO_PFC6_PFC613);
    rza_io_reg_write_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC613_SHIFT,     GPIO_PMC6_PMC613);
    rza_io_reg_write_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC613_SHIFT,   GPIO_PIPC6_PIPC613);

    /* ---- P6_14 : D14 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE614_SHIFT, GPIO_PFCAE6_PFCAE614);
    rza_io_reg_write_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE614_SHIFT,   GPIO_PFCE6_PFCE614);
    rza_io_reg_write_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC614_SHIFT,     GPIO_PFC6_PFC614);
    rza_io_reg_write_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC614_SHIFT,     GPIO_PMC6_PMC614);
    rza_io_reg_write_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC614_SHIFT,   GPIO_PIPC6_PIPC614);

    /* ---- P6_15 : D15 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE6, 0, GPIO_PFCAE6_PFCAE615_SHIFT, GPIO_PFCAE6_PFCAE615);
    rza_io_reg_write_16(&GPIO.PFCE6,  0, GPIO_PFCE6_PFCE615_SHIFT,   GPIO_PFCE6_PFCE615);
    rza_io_reg_write_16(&GPIO.PFC6,   0, GPIO_PFC6_PFC615_SHIFT,     GPIO_PFC6_PFC615);
    rza_io_reg_write_16(&GPIO.PMC6,   1, GPIO_PMC6_PMC615_SHIFT,     GPIO_PMC6_PMC615);
    rza_io_reg_write_16(&GPIO.PIPC6,  1, GPIO_PIPC6_PIPC615_SHIFT,   GPIO_PIPC6_PIPC615);

    /* ---- P7_9 : A1 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE79_SHIFT, GPIO_PFCAE7_PFCAE79);
    rza_io_reg_write_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE79_SHIFT,   GPIO_PFCE7_PFCE79);
    rza_io_reg_write_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC79_SHIFT,     GPIO_PFC7_PFC79);
    rza_io_reg_write_16(&GPIO.PM7,    0, GPIO_PM7_PM79_SHIFT,       GPIO_PM7_PM79);
    rza_io_reg_write_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC79_SHIFT,     GPIO_PMC7_PMC79);
    rza_io_reg_write_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC79_SHIFT,   GPIO_PIPC7_PIPC79);

    /* ---- P7_10 : A2 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE710_SHIFT, GPIO_PFCAE7_PFCAE710);
    rza_io_reg_write_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE710_SHIFT,   GPIO_PFCE7_PFCE710);
    rza_io_reg_write_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC710_SHIFT,     GPIO_PFC7_PFC710);
    rza_io_reg_write_16(&GPIO.PM7,    0, GPIO_PM7_PM710_SHIFT,       GPIO_PM7_PM710);
    rza_io_reg_write_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC710_SHIFT,     GPIO_PMC7_PMC710);
    rza_io_reg_write_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC710_SHIFT,   GPIO_PIPC7_PIPC710);

    /* ---- P7_11 : A3 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE711_SHIFT, GPIO_PFCAE7_PFCAE711);
    rza_io_reg_write_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE711_SHIFT,   GPIO_PFCE7_PFCE711);
    rza_io_reg_write_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC711_SHIFT,     GPIO_PFC7_PFC711);
    rza_io_reg_write_16(&GPIO.PM7,    0, GPIO_PM7_PM711_SHIFT,       GPIO_PM7_PM711);
    rza_io_reg_write_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC711_SHIFT,     GPIO_PMC7_PMC711);
    rza_io_reg_write_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC711_SHIFT,   GPIO_PIPC7_PIPC711);

    /* ---- P7_12 : A4 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE712_SHIFT, GPIO_PFCAE7_PFCAE712);
    rza_io_reg_write_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE712_SHIFT,   GPIO_PFCE7_PFCE712);
    rza_io_reg_write_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC712_SHIFT,     GPIO_PFC7_PFC712);
    rza_io_reg_write_16(&GPIO.PM7,    0, GPIO_PM7_PM712_SHIFT,       GPIO_PM7_PM712);
    rza_io_reg_write_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC712_SHIFT,     GPIO_PMC7_PMC712);
    rza_io_reg_write_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC712_SHIFT,   GPIO_PIPC7_PIPC712);

    /* ---- P7_13 : A5 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE713_SHIFT, GPIO_PFCAE7_PFCAE713);
    rza_io_reg_write_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE713_SHIFT,   GPIO_PFCE7_PFCE713);
    rza_io_reg_write_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC713_SHIFT,     GPIO_PFC7_PFC713);
    rza_io_reg_write_16(&GPIO.PM7,    0, GPIO_PM7_PM713_SHIFT,       GPIO_PM7_PM713);
    rza_io_reg_write_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC713_SHIFT,     GPIO_PMC7_PMC713);
    rza_io_reg_write_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC713_SHIFT,   GPIO_PIPC7_PIPC713);

    /* ---- P7_14 : A6 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE714_SHIFT, GPIO_PFCAE7_PFCAE714);
    rza_io_reg_write_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE714_SHIFT,   GPIO_PFCE7_PFCE714);
    rza_io_reg_write_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC714_SHIFT,     GPIO_PFC7_PFC714);
    rza_io_reg_write_16(&GPIO.PM7,    0, GPIO_PM7_PM714_SHIFT,       GPIO_PM7_PM714);
    rza_io_reg_write_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC714_SHIFT,     GPIO_PMC7_PMC714);
    rza_io_reg_write_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC714_SHIFT,   GPIO_PIPC7_PIPC714);

    /* ---- P7_15 : A7 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE7, 0, GPIO_PFCAE7_PFCAE715_SHIFT, GPIO_PFCAE7_PFCAE715);
    rza_io_reg_write_16(&GPIO.PFCE7,  0, GPIO_PFCE7_PFCE715_SHIFT,   GPIO_PFCE7_PFCE715);
    rza_io_reg_write_16(&GPIO.PFC7,   0, GPIO_PFC7_PFC715_SHIFT,     GPIO_PFC7_PFC715);
    rza_io_reg_write_16(&GPIO.PM7,    0, GPIO_PM7_PM715_SHIFT,       GPIO_PM7_PM715);
    rza_io_reg_write_16(&GPIO.PMC7,   1, GPIO_PMC7_PMC715_SHIFT,     GPIO_PMC7_PMC715);
    rza_io_reg_write_16(&GPIO.PIPC7,  1, GPIO_PIPC7_PIPC715_SHIFT,   GPIO_PIPC7_PIPC715);

    /* ---- P8_0 : A8 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE80_SHIFT, GPIO_PFCAE8_PFCAE80);
    rza_io_reg_write_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE80_SHIFT,   GPIO_PFCE8_PFCE80);
    rza_io_reg_write_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC80_SHIFT,     GPIO_PFC8_PFC80);
    rza_io_reg_write_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC80_SHIFT,     GPIO_PMC8_PMC80);
    rza_io_reg_write_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC80_SHIFT,   GPIO_PIPC8_PIPC80);

    /* ---- P8_1 : A9 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE81_SHIFT, GPIO_PFCAE8_PFCAE81);
    rza_io_reg_write_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE81_SHIFT,   GPIO_PFCE8_PFCE81);
    rza_io_reg_write_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC81_SHIFT,     GPIO_PFC8_PFC81);
    rza_io_reg_write_16(&GPIO.PM8,    0, GPIO_PM8_PM81_SHIFT,       GPIO_PM8_PM81);
    rza_io_reg_write_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC81_SHIFT,     GPIO_PMC8_PMC81);
    rza_io_reg_write_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC81_SHIFT,   GPIO_PIPC8_PIPC81);

    /* ---- P8_2 : A10 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE82_SHIFT, GPIO_PFCAE8_PFCAE82);
    rza_io_reg_write_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE82_SHIFT,   GPIO_PFCE8_PFCE82);
    rza_io_reg_write_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC82_SHIFT,     GPIO_PFC8_PFC82);
    rza_io_reg_write_16(&GPIO.PM8,    0, GPIO_PM8_PM82_SHIFT,       GPIO_PM8_PM82);
    rza_io_reg_write_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC82_SHIFT,     GPIO_PMC8_PMC82);
    rza_io_reg_write_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC82_SHIFT,   GPIO_PIPC8_PIPC82);

    /* ---- P8_3 : A11 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE83_SHIFT, GPIO_PFCAE8_PFCAE83);
    rza_io_reg_write_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE83_SHIFT,   GPIO_PFCE8_PFCE83);
    rza_io_reg_write_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC83_SHIFT,     GPIO_PFC8_PFC83);
    rza_io_reg_write_16(&GPIO.PM8,    0, GPIO_PM8_PM83_SHIFT,       GPIO_PM8_PM83);
    rza_io_reg_write_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC83_SHIFT,     GPIO_PMC8_PMC83);
    rza_io_reg_write_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC83_SHIFT,   GPIO_PIPC8_PIPC83);

    /* ---- P8_4 : A12 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE84_SHIFT, GPIO_PFCAE8_PFCAE84);
    rza_io_reg_write_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE84_SHIFT,   GPIO_PFCE8_PFCE84);
    rza_io_reg_write_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC84_SHIFT,     GPIO_PFC8_PFC84);
    rza_io_reg_write_16(&GPIO.PM8,    0, GPIO_PM8_PM84_SHIFT,       GPIO_PM8_PM84);
    rza_io_reg_write_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC84_SHIFT,     GPIO_PMC8_PMC84);
    rza_io_reg_write_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC84_SHIFT,   GPIO_PIPC8_PIPC84);

    /* ---- P8_5 : A13 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE85_SHIFT, GPIO_PFCAE8_PFCAE85);
    rza_io_reg_write_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE85_SHIFT,   GPIO_PFCE8_PFCE85);
    rza_io_reg_write_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC85_SHIFT,     GPIO_PFC8_PFC85);
    rza_io_reg_write_16(&GPIO.PM8,    0, GPIO_PM8_PM85_SHIFT,       GPIO_PM8_PM85);
    rza_io_reg_write_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC85_SHIFT,     GPIO_PMC8_PMC85);
    rza_io_reg_write_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC85_SHIFT,   GPIO_PIPC8_PIPC85);

    /* ---- P8_6 : A14 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE86_SHIFT, GPIO_PFCAE8_PFCAE86);
    rza_io_reg_write_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE86_SHIFT,   GPIO_PFCE8_PFCE86);
    rza_io_reg_write_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC86_SHIFT,     GPIO_PFC8_PFC86);
    rza_io_reg_write_16(&GPIO.PM8,    0, GPIO_PM8_PM86_SHIFT,       GPIO_PM8_PM86);
    rza_io_reg_write_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC86_SHIFT,     GPIO_PMC8_PMC86);
    rza_io_reg_write_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC86_SHIFT,   GPIO_PIPC8_PIPC86);

    /* ---- P8_7 : A15 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE87_SHIFT, GPIO_PFCAE8_PFCAE87);
    rza_io_reg_write_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE87_SHIFT,   GPIO_PFCE8_PFCE87);
    rza_io_reg_write_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC87_SHIFT,     GPIO_PFC8_PFC87);
    rza_io_reg_write_16(&GPIO.PM8,    0, GPIO_PM8_PM87_SHIFT,       GPIO_PM8_PM87);
    rza_io_reg_write_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC87_SHIFT,     GPIO_PMC8_PMC87);
    rza_io_reg_write_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC87_SHIFT,   GPIO_PIPC8_PIPC87);

    /* ---- P8_8 : A16 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE88_SHIFT, GPIO_PFCAE8_PFCAE88);
    rza_io_reg_write_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE88_SHIFT,   GPIO_PFCE8_PFCE88);
    rza_io_reg_write_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC88_SHIFT,     GPIO_PFC8_PFC88);
    rza_io_reg_write_16(&GPIO.PM8,    0, GPIO_PM8_PM88_SHIFT,       GPIO_PM8_PM88);
    rza_io_reg_write_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC88_SHIFT,     GPIO_PMC8_PMC88);
    rza_io_reg_write_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC88_SHIFT,   GPIO_PIPC8_PIPC88);

    /* ---- P8_9 : A17 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE89_SHIFT, GPIO_PFCAE8_PFCAE89);
    rza_io_reg_write_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE89_SHIFT,   GPIO_PFCE8_PFCE89);
    rza_io_reg_write_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC89_SHIFT,     GPIO_PFC8_PFC89);
    rza_io_reg_write_16(&GPIO.PM8,    0, GPIO_PM8_PM89_SHIFT,       GPIO_PM8_PM89);
    rza_io_reg_write_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC89_SHIFT,     GPIO_PMC8_PMC89);
    rza_io_reg_write_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC89_SHIFT,   GPIO_PIPC8_PIPC89);

    /* ---- P8_10 : A18 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE810_SHIFT, GPIO_PFCAE8_PFCAE810);
    rza_io_reg_write_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE810_SHIFT,   GPIO_PFCE8_PFCE810);
    rza_io_reg_write_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC810_SHIFT,     GPIO_PFC8_PFC810);
    rza_io_reg_write_16(&GPIO.PM8,    0, GPIO_PM8_PM810_SHIFT,       GPIO_PM8_PM810);
    rza_io_reg_write_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC810_SHIFT,     GPIO_PMC8_PMC810);
    rza_io_reg_write_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC810_SHIFT,   GPIO_PIPC8_PIPC810);

    /* ---- P8_11 : A19 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE811_SHIFT, GPIO_PFCAE8_PFCAE811);
    rza_io_reg_write_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE811_SHIFT,   GPIO_PFCE8_PFCE811);
    rza_io_reg_write_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC811_SHIFT,     GPIO_PFC8_PFC811);
    rza_io_reg_write_16(&GPIO.PM8,    0, GPIO_PM8_PM811_SHIFT,       GPIO_PM8_PM811);
    rza_io_reg_write_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC811_SHIFT,     GPIO_PMC8_PMC811);
    rza_io_reg_write_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC811_SHIFT,   GPIO_PIPC8_PIPC811);

    /* ---- P8_12 : A20 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE812_SHIFT, GPIO_PFCAE8_PFCAE812);
    rza_io_reg_write_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE812_SHIFT,   GPIO_PFCE8_PFCE812);
    rza_io_reg_write_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC812_SHIFT,     GPIO_PFC8_PFC812);
    rza_io_reg_write_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC812_SHIFT,     GPIO_PMC8_PMC812);
    rza_io_reg_write_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC812_SHIFT,   GPIO_PIPC8_PIPC812);

    /* ---- P8_13 : A21 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE813_SHIFT, GPIO_PFCAE8_PFCAE813);
    rza_io_reg_write_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE813_SHIFT,   GPIO_PFCE8_PFCE813);
    rza_io_reg_write_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC813_SHIFT,     GPIO_PFC8_PFC813);
    rza_io_reg_write_16(&GPIO.PM8,    0, GPIO_PM8_PM813_SHIFT,       GPIO_PM8_PM813);
    rza_io_reg_write_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC813_SHIFT,     GPIO_PMC8_PMC813);
    rza_io_reg_write_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC813_SHIFT,   GPIO_PIPC8_PIPC813);

    /* ---- P8_14 : A22 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE814_SHIFT, GPIO_PFCAE8_PFCAE814);
    rza_io_reg_write_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE814_SHIFT,   GPIO_PFCE8_PFCE814);
    rza_io_reg_write_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC814_SHIFT,     GPIO_PFC8_PFC814);
    rza_io_reg_write_16(&GPIO.PM8,    0, GPIO_PM8_PM814_SHIFT,       GPIO_PM8_PM814);
    rza_io_reg_write_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC814_SHIFT,     GPIO_PMC8_PMC814);
    rza_io_reg_write_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC814_SHIFT,   GPIO_PIPC8_PIPC814);

    /* ---- P8_15 : A23 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE8, 0, GPIO_PFCAE8_PFCAE815_SHIFT, GPIO_PFCAE8_PFCAE815);
    rza_io_reg_write_16(&GPIO.PFCE8,  0, GPIO_PFCE8_PFCE815_SHIFT,   GPIO_PFCE8_PFCE815);
    rza_io_reg_write_16(&GPIO.PFC8,   0, GPIO_PFC8_PFC815_SHIFT,     GPIO_PFC8_PFC815);
    rza_io_reg_write_16(&GPIO.PM8,    0, GPIO_PM8_PM815_SHIFT,       GPIO_PM8_PM815);
    rza_io_reg_write_16(&GPIO.PMC8,   1, GPIO_PMC8_PMC815_SHIFT,     GPIO_PMC8_PMC815);
    rza_io_reg_write_16(&GPIO.PIPC8,  1, GPIO_PIPC8_PIPC815_SHIFT,   GPIO_PIPC8_PIPC815);

    /* ---- P9_0 : A24 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */

    rza_io_reg_write_16(&GPIO.PFCAE9, 0, GPIO_PFCAE9_PFCAE90_SHIFT, GPIO_PFCAE9_PFCAE90);
    rza_io_reg_write_16(&GPIO.PFCE9,  0, GPIO_PFCE9_PFCE90_SHIFT,   GPIO_PFCE9_PFCE90);
    rza_io_reg_write_16(&GPIO.PFC9,   0, GPIO_PFC9_PFC90_SHIFT,     GPIO_PFC9_PFC90);
    rza_io_reg_write_16(&GPIO.PM9,    0, GPIO_PM9_PM90_SHIFT,       GPIO_PM9_PM90);
    rza_io_reg_write_16(&GPIO.PMC9,   1, GPIO_PMC9_PMC90_SHIFT,     GPIO_PMC9_PMC90);
    rza_io_reg_write_16(&GPIO.PIPC9,  1, GPIO_PIPC9_PIPC90_SHIFT,   GPIO_PIPC9_PIPC90);

    /* ---- P9_1 : A25 ---- */
    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    rza_io_reg_write_16(&GPIO.PFCAE9, 0, GPIO_PFCAE9_PFCAE91_SHIFT, GPIO_PFCAE9_PFCAE91);
    rza_io_reg_write_16(&GPIO.PFCE9,  0, GPIO_PFCE9_PFCE91_SHIFT,   GPIO_PFCE9_PFCE91);
    rza_io_reg_write_16(&GPIO.PFC9,   0, GPIO_PFC9_PFC91_SHIFT,     GPIO_PFC9_PFC91);
    rza_io_reg_write_16(&GPIO.PM9,    0, GPIO_PM9_PM91_SHIFT,       GPIO_PM9_PM91);
    rza_io_reg_write_16(&GPIO.PMC9,   1, GPIO_PMC9_PMC91_SHIFT,     GPIO_PMC9_PMC91);
    rza_io_reg_write_16(&GPIO.PIPC9,  1, GPIO_PIPC9_PIPC91_SHIFT,   GPIO_PIPC9_PIPC91);
#endif

}

/* End of File */

