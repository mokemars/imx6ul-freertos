/**
 * @file sdram.c
 * @brief SDRAM configuration
 *
 * @section License
 *
 * Copyright (C) 2010-2017 Oryx Embedded SARL. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.0
 **/

#include "samv71.h"
#include "samv71_xplained_ultra.h"
#include "sdram.h"
#include "debug.h"


/**
 * @brief SDRAM initialization
 **/

void sdramInit(void)
{
   volatile uint32_t i;
   volatile uint8_t *pSdram = (uint8_t *) SDRAM_CS_ADDR;

   //Enable PIO peripheral clocks
   PMC->PMC_PCER0 = (1 << ID_PIOA) | (1 << ID_PIOC) | (1 << ID_PIOD) | (1 << ID_PIOE);
   //Enable SDRAMC peripheral clock
   PMC->PMC_PCER1 = (1 << (ID_SDRAMC - 32));

   //Disable pull-up resistors on SDRAM pins
   PIOA->PIO_PUDR = SDRAM_PIOA_PERIPH_A_MASK;
   //Disable interrupts-on-change
   PIOA->PIO_IDR = SDRAM_PIOA_PERIPH_A_MASK;
   //Assign RMII pins to peripheral A function
   PIOA->PIO_ABCDSR[0] &= ~SDRAM_PIOA_PERIPH_A_MASK;
   PIOA->PIO_ABCDSR[1] &= ~SDRAM_PIOA_PERIPH_A_MASK;
   //Disable the PIO from controlling the corresponding pins
   PIOA->PIO_PDR = SDRAM_PIOA_PERIPH_A_MASK;

   //Disable pull-up resistors on SDRAM pins
   PIOA->PIO_PUDR = SDRAM_PIOA_PERIPH_C_MASK;
   //Disable interrupts-on-change
   PIOA->PIO_IDR = SDRAM_PIOA_PERIPH_C_MASK;
   //Assign RMII pins to peripheral C function
   PIOA->PIO_ABCDSR[0] &= ~SDRAM_PIOA_PERIPH_C_MASK;
   PIOA->PIO_ABCDSR[1] |= SDRAM_PIOA_PERIPH_C_MASK;
   //Disable the PIO from controlling the corresponding pins
   PIOA->PIO_PDR = SDRAM_PIOA_PERIPH_C_MASK;

   //Disable pull-up resistors on SDRAM pins
   PIOC->PIO_PUDR = SDRAM_PIOC_PERIPH_A_MASK;
   //Disable interrupts-on-change
   PIOC->PIO_IDR = SDRAM_PIOC_PERIPH_A_MASK;
   //Assign RMII pins to peripheral A function
   PIOC->PIO_ABCDSR[0] &= ~SDRAM_PIOC_PERIPH_A_MASK;
   PIOC->PIO_ABCDSR[1] &= ~SDRAM_PIOC_PERIPH_A_MASK;
   //Disable the PIO from controlling the corresponding pins
   PIOC->PIO_PDR = SDRAM_PIOC_PERIPH_A_MASK;

   //Disable pull-up resistors on SDRAM pins
   PIOD->PIO_PUDR = SDRAM_PIOD_PERIPH_C_MASK;
   //Disable interrupts-on-change
   PIOD->PIO_IDR = SDRAM_PIOD_PERIPH_C_MASK;
   //Assign RMII pins to peripheral C function
   PIOD->PIO_ABCDSR[0] &= ~SDRAM_PIOD_PERIPH_C_MASK;
   PIOD->PIO_ABCDSR[1] |= SDRAM_PIOD_PERIPH_C_MASK;
   //Disable the PIO from controlling the corresponding pins
   PIOD->PIO_PDR = SDRAM_PIOD_PERIPH_C_MASK;

   //Disable pull-up resistors on SDRAM pins
   PIOE->PIO_PUDR = SDRAM_PIOE_PERIPH_A_MASK;
   //Disable interrupts-on-change
   PIOE->PIO_IDR = SDRAM_PIOE_PERIPH_A_MASK;
   //Assign RMII pins to peripheral A function
   PIOE->PIO_ABCDSR[0] &= ~SDRAM_PIOE_PERIPH_A_MASK;
   PIOE->PIO_ABCDSR[1] &= ~SDRAM_PIOE_PERIPH_A_MASK;
   //Disable the PIO from controlling the corresponding pins
   PIOE->PIO_PDR = SDRAM_PIOE_PERIPH_A_MASK;

   //Enable SDRAM support
	MATRIX->CCFG_SMCNFCS = CCFG_SMCNFCS_SDRAMEN;

	//SDRAM features must be set in the Configuration Register
	SDRAMC->SDRAMC_CR =
		  SDRAMC_CR_NC_COL8      // 8 column bits
		| SDRAMC_CR_NR_ROW11     // 12 row bits (4K)
		| SDRAMC_CR_CAS_LATENCY3 // CAS Latency 3
		| SDRAMC_CR_NB_BANK2     // 2 banks
		| SDRAMC_CR_DBW          // 16 bit
		| SDRAMC_CR_TWR(4)
		| SDRAMC_CR_TRC_TRFC(11) // 63ns   min
		| SDRAMC_CR_TRP(5)       // Command period (PRE to ACT) 21 ns min
		| SDRAMC_CR_TRCD(5)      // Active Command to read/Write Command delay time 21ns min
		| SDRAMC_CR_TRAS(8)      // Command period (ACT to PRE)  42ns min
		| SDRAMC_CR_TXSR(13U);   // Exit self-refresh to active time  70ns Min

	/* 2. For mobile SDRAM, temperature-compensated self refresh (TCSR), drive
	strength (DS) and partial array self refresh (PASR) must be set in the
	Low Power Register. */

	/* 3. The SDRAM memory type must be set in the Memory Device Register.*/
	SDRAMC->SDRAMC_MDR = SDRAMC_MDR_MD_SDRAM;

	/* 4. A minimum pause of 200 ¦Ìs is provided to precede any signal toggle.*/
	for (i = 0; i < 100000; i++);

	/* 5. (1)A NOP command is issued to the SDRAM devices. The application must
	set Mode to 1 in the Mode Register and perform a write access to
	any SDRAM address.*/
	SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_NOP;
	*pSdram = 0;
	for (i = 0; i < 100000; i++);
	/* 6. An All Banks Precharge command is issued to the SDRAM devices.
	The application must set Mode to 2 in the Mode Register and perform a write
	access to any SDRAM address. */
	SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_ALLBANKS_PRECHARGE;
	*pSdram = 0;
	for (i = 0; i < 100000; i++);
	/* 7. Eight auto-refresh (CBR) cycles are provided. The application must
	set the Mode to 4 in the Mode Register and perform a write access to any
	SDRAM location eight times.*/
	for (i = 0 ; i< 8; i++) {
		SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_AUTO_REFRESH;
		*pSdram = 0;
	}
	for (i = 0; i < 100000; i++);
	/*8. A Mode Register set (MRS) cycle is issued to program the parameters of
	the SDRAM devices, in particular CAS latency and burst length. The
	application must set Mode to 3 in the Mode Register and perform a write
	access to the SDRAM. The write address must be chosen so that BA[1:0]
	are set to 0. For example, with a 16-bit 128 MB SDRAM (12 rows, 9 columns,
	4 banks) bank address, the SDRAM write access should be done at the address
	0x70000000.*/
	SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_LOAD_MODEREG;
	*pSdram = 0;

	for (i = 0; i < 100000; i++);
	/*9. For mobile SDRAM initialization, an Extended Mode Register set (EMRS)
	cycle is issued to program the SDRAM parameters (TCSR, PASR, DS). The
	application must set Mode to 5 in the Mode Register and perform a write
	access to the SDRAM. The write address must be chosen so that BA[1] or BA[0]
	are set to 1.
	For example, with a 16-bit 128 MB SDRAM, (12 rows, 9 columns, 4 banks) bank
	address the SDRAM write access should be done at the address 0x70800000 or
	0x70400000. */
	//SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_EXT_LOAD_MODEREG;
	// *((uint8_t *)(pSdram + SDRAM_BA0)) = 0;

	/* 10. The application must go into Normal Mode, setting Mode to 0 in the
	Mode Register and performing a write access at any location in the SDRAM. */
	SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_NORMAL;
	*pSdram = 0;
	for (i = 0; i < 100000; i++);
	/* 11. Write the refresh rate into the count field in the SDRAMC Refresh
	Timer register. (Refresh rate = delay between refresh cycles).
	The SDRAM device requires a refresh every 15.625 ¦Ìs or 7.81 ¦Ìs.
	With a 100 MHz frequency, the Refresh Timer Counter Register must be set
	with the value 1562(15.625 ¦Ìs x 100 MHz) or 781(7.81 ¦Ìs x 100 MHz). */
	// For IS42S16100E, 2048 refresh cycle every 32ms, every 15.625 ¦Ìs
	/* ((32 x 10(^-3))/2048) x150 x (10^6) */
	SDRAMC->SDRAMC_TR = 2343; ;
	SDRAMC->SDRAMC_CFR1 |= SDRAMC_CFR1_UNAL;
	/* After initialization, the SDRAM devices are fully functional. */
}
