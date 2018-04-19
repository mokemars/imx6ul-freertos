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

#include "sam3xa.h"
#include "sam3x_ek.h"
#include "sdram.h"
#include "error.h"
#include "debug.h"


/**
 * @brief SDRAM initialization
 * @param[in] coreClockFrequency Core clock frequency
 **/

void sdramInit(uint32_t coreClockFrequency)
{
   uint32_t n;

   //Enable PIO peripheral clocks
   PMC->PMC_PCER0 = (1 << ID_PIOC) | (1 << ID_PIOD);
   //Enable SMC peripheral clock
   PMC->PMC_PCER0 = (1 << ID_SMC);

   //Assign SDRAM pins to Peripheral A function
   PIOC->PIO_ABSR &= ~SDRAM_PIOC_MASK;
   //Disable the PIO from controlling the corresponding pins
   PIOC->PIO_PDR = SDRAM_PIOC_MASK;
   //Enable pull-ups
   PIOC->PIO_PUER = SDRAM_PIOC_MASK;

   //Assign SDRAM pins to Peripheral A function
   PIOD->PIO_ABSR &= ~SDRAM_PIOD_MASK;
   //Disable the PIO from controlling the corresponding pins
   PIOD->PIO_PDR = SDRAM_PIOD_MASK;
   //Enable pull-ups
   PIOD->PIO_PUER = SDRAM_PIOD_MASK;

   //Configure SDRAM enable pin as an output
   PIOD->PIO_PER = PIO_PD18;
   PIOD->PIO_OER = PIO_PD18;
   PIOD->PIO_SODR = PIO_PD18;

   //SDRAM features must be set in the Configuration Register
   SDRAMC->SDRAMC_CR = SDRAMC_CR_NC_COL9 |      //Number of columns (512)
                       SDRAMC_CR_NR_ROW13 |     //Number of rows (8192)
                       SDRAMC_CR_NB_BANK4 |     //Number of banks (4)
                       SDRAMC_CR_CAS_LATENCY2 | //CAS latency (2 cycles)
                       SDRAMC_CR_DBW |          //Data bus width (16 bits)
                       SDRAMC_CR_TWR(2) |       //Write recovery delay (2 cycles)
                       SDRAMC_CR_TRC_TRFC(9) |  //Row cycle delay (9 cycles)
                       SDRAMC_CR_TRP(3) |       //Row precharge delay (3 cycles)
                       SDRAMC_CR_TRCD(3) |      //Row to column delay (3 cycles)
                       SDRAMC_CR_TRAS(6) |      //Active to precharge delay (6 cycles)
                       SDRAMC_CR_TXSR(10);      //Exit self refresh to active delay (10 cycles)

   //For mobile SDRAM, temperature-compensated self refresh (TCSR), drive strength (DS)
   //and partial array self refresh (PASR) must be set in the Low Power Register
   SDRAMC->SDRAMC_LPR = 0;

   //The SDRAM memory type must be set in the Memory Device Register
   SDRAMC->SDRAMC_MDR = SDRAMC_MDR_MD_SDRAM;

   //A minimum pause of 200 us is provided to precede any signal toggle
   sleep(1);

   //A NOP command is issued to the SDRAM device. The application must set Mode to 1
   //in the Mode Register and perform a write access to any SDRAM address
   SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_NOP;
   *((uint16_t *)(SDRAM_BASE)) = 0x00000000;

   //An All Banks Precharge command is issued to the SDRAM devices. The application must
   //set Mode to 2 in the Mode Register and perform a write access to any SDRAM address
   SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_ALLBANKS_PRECHARGE;
   *((uint16_t *)(SDRAM_BASE)) = 0x00000000;

   //Eight auto-refresh (CBR) cycles are provided. The application must set the Mode to 4
   //in the Mode Register and perform a write access to any SDRAM location eight times
   SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_AUTO_REFRESH;
   *((uint16_t *)(SDRAM_BASE)) = 0x00000000;

   SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_AUTO_REFRESH;
   *((uint16_t *)(SDRAM_BASE)) = 0x00000000;

   SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_AUTO_REFRESH;
   *((uint16_t *)(SDRAM_BASE)) = 0x00000000;

   SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_AUTO_REFRESH;
   *((uint16_t *)(SDRAM_BASE)) = 0x00000000;

   SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_AUTO_REFRESH;
   *((uint16_t *)(SDRAM_BASE)) = 0x00000000;

   SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_AUTO_REFRESH;
   *((uint16_t *)(SDRAM_BASE)) = 0x00000000;

   SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_AUTO_REFRESH;
   *((uint16_t *)(SDRAM_BASE)) = 0x00000000;

   SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_AUTO_REFRESH;
   *((uint16_t *)(SDRAM_BASE)) = 0x00000000;

   //A Mode Register set (MRS) cycle is issued to program the parameters of the SDRAM device,
   //in particular CAS latency and burst length. The application must set Mode to 3 in the
   //Mode Register and perform a write access to the SDRAM. The write address must be chosen
   //so that BA[1:0] are set to 0
   SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_LOAD_MODEREG;
   *((uint16_t *)(SDRAM_BASE)) = 0x00000000;

   //For mobile SDRAM initialization, an Extended Mode Register set (EMRS) cycle is
   //issued to program the SDRAM parameters (TCSR, PASR, DS). The application must
   //set Mode to 5 in the Mode Register and perform a write access to the SDRAM. The
   //write address must be chosen so that BA[1] or BA[0] are set to 1
   SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_EXT_LOAD_MODEREG;
   *((uint16_t *)(SDRAM_BASE) + 0x01000000) = 0x00000000;

   //The application must go into Normal Mode, setting Mode to 0 in the Mode Register and
   //performing a write access at any location in the SDRAM
   SDRAMC->SDRAMC_MR = SDRAMC_MR_MODE_NORMAL;
   *((uint16_t *)(SDRAM_BASE)) = 0x00000000;

   //Set refresh rate (15.625us)
   n = coreClockFrequency / 1000;
   n = (n * 15625) / 1000000;

   //Write the refresh rate into the count field in the SDRAMC Refresh Timer register
   SDRAMC->SDRAMC_TR = SDRAMC_TR_COUNT(n);
}


/**
 * @brief SDRAM test routine
 * @return Error code
 **/

error_t sdramTest(void)
{
   uint_t i;

   //Point to the beginning of the memory space
   uint32_t *address = (uint32_t *) SDRAM_BASE;
   //Initialize test pattern generation
   uint32_t value = 0x12345678;

   //Write SDRAM memory contents
   for(i = 0; i < (SDRAM_SIZE / 4); i++)
   {
      //Write current location
      *(address++) = value;
      //Test pattern generation
      value = value * 0x7AB5 + 0x5E8AC93D;
   }

   //Point to the beginning of the memory space
   address = (uint32_t *) SDRAM_BASE;
   //Initialize test pattern generation
   value = 0x12345678;

   //Read back and check SDRAM memory contents
   for(i = 0; i < (SDRAM_SIZE / 4); i++)
   {
      //Read current location
      if(*(address++) != value)
         return ERROR_FAILURE;
      //Test pattern generation
      value = value * 0x7AB5 + 0x5E8AC93D;
   }

   //Successful test
   return NO_ERROR;
}
