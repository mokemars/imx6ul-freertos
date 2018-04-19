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

#include "sam7se_ek.h"
#include "sdram.h"
#include "error.h"
#include "debug.h"


/**
 * @brief SDRAM initialization
 **/

void sdramInit(void)
{
   //Enable the peripheral clock of PIO controllers
   AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PIOA;
   AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PIOB;
   AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_PIOC;

   //Chip select 1 is assigned to the SDRAM controller
   AT91C_BASE_EBI->EBI_CSA |= AT91C_EBI_CS1A;

   //Assign SDRAM control pins to Peripheral B function
   AT91C_BASE_PIOA->PIO_BSR = AT91C_SDRAM_CTRL_MASK;
   //Disable the PIO from controlling the corresponding pins
   AT91C_BASE_PIOA->PIO_PDR = AT91C_SDRAM_CTRL_MASK;

   //Assign SDRAM address bus to Peripheral B function
   AT91C_BASE_PIOB->PIO_BSR = AT91C_SDRAM_ADDR_MASK;
   //Disable the PIO from controlling the corresponding pins
   AT91C_BASE_PIOB->PIO_PDR = AT91C_SDRAM_ADDR_MASK;

   //Assign SDRAM data bus to Peripheral A function
   AT91C_BASE_PIOC->PIO_ASR = AT91C_SDRAM_DATA_MASK;
   //Disable the PIO from controlling the corresponding pins
   AT91C_BASE_PIOC->PIO_PDR = AT91C_SDRAM_DATA_MASK;

   //SDRAM features must be set in the configuration register
   AT91C_BASE_SDRC->SDRC_CR = AT91C_SDRC_NC_9 |       //Number of columns (512)
                              AT91C_SDRC_NR_13 |      //Number of rows (8192)
                              AT91C_SDRC_NB_4_BANKS | //Number of banks (4)
                              AT91C_SDRC_CAS_2 |      //CAS latency (2 cycles)
                              AT91C_SDRC_TWR_2 |      //Write recovery delay (2 cycles)
                              AT91C_SDRC_TRC_4 |      //Row cycle delay (4 cycles)
                              AT91C_SDRC_TRP_4 |      //Row precharge delay (4 cycles)
                              AT91C_SDRC_TRCD_2 |     //Row to column delay (2 cycles)
                              AT91C_SDRC_TRAS_3 |     //Active to precharge delay (3 cycles)
                              AT91C_SDRC_TXSR_4;      //Exit self refresh to active delay (4 cycles)

   //A minimum pause of 200us is executed to precede any signal toggle
   sleep(1);

   //A NOP command is issued to the SDRAM device. The application must set Mode to 1
   //in the Mode Register and perform a write access to any SDRAM address
   AT91C_BASE_SDRC->SDRC_MR = AT91C_SDRC_DBW_16_BITS | AT91C_SDRC_MODE_NOP_CMD;
   *AT91C_SDRAM_BASE = 0x00000000;

   //An All Banks Precharge command is issued to the SDRAM devices. The application must
   //set Mode to 2 in the Mode Register and perform a write access to any SDRAM address
   AT91C_BASE_SDRC->SDRC_MR = AT91C_SDRC_DBW_16_BITS | AT91C_SDRC_MODE_PRCGALL_CMD;
   *AT91C_SDRAM_BASE = 0x00000000;

   //Eight auto-refresh (CBR) cycles are provided. The application must set the Mode to 4
   //in the Mode Register and perform a write access to any SDRAM location eight times
   AT91C_BASE_SDRC->SDRC_MR = AT91C_SDRC_DBW_16_BITS | AT91C_SDRC_MODE_RFSH_CMD;
   *AT91C_SDRAM_BASE = 0x00000000;

   AT91C_BASE_SDRC->SDRC_MR = AT91C_SDRC_DBW_16_BITS | AT91C_SDRC_MODE_RFSH_CMD;
   *AT91C_SDRAM_BASE = 0x00000000;

   AT91C_BASE_SDRC->SDRC_MR = AT91C_SDRC_DBW_16_BITS | AT91C_SDRC_MODE_RFSH_CMD;
   *AT91C_SDRAM_BASE = 0x00000000;

   AT91C_BASE_SDRC->SDRC_MR = AT91C_SDRC_DBW_16_BITS | AT91C_SDRC_MODE_RFSH_CMD;
   *AT91C_SDRAM_BASE = 0x00000000;

   AT91C_BASE_SDRC->SDRC_MR = AT91C_SDRC_DBW_16_BITS | AT91C_SDRC_MODE_RFSH_CMD;
   *AT91C_SDRAM_BASE = 0x00000000;

   AT91C_BASE_SDRC->SDRC_MR = AT91C_SDRC_DBW_16_BITS | AT91C_SDRC_MODE_RFSH_CMD;
   *AT91C_SDRAM_BASE = 0x00000000;

   AT91C_BASE_SDRC->SDRC_MR = AT91C_SDRC_DBW_16_BITS | AT91C_SDRC_MODE_RFSH_CMD;
   *AT91C_SDRAM_BASE = 0x00000000;

   AT91C_BASE_SDRC->SDRC_MR = AT91C_SDRC_DBW_16_BITS | AT91C_SDRC_MODE_RFSH_CMD;
   *AT91C_SDRAM_BASE = 0x00000000;

   //A Mode Register set (MRS) cycle is issued to program the parameters of the SDRAM device,
   //in particular CAS latency and burst length. The application must set Mode to 3 in the
   //Mode Register and perform a write access to the SDRAM
   AT91C_BASE_SDRC->SDRC_MR = AT91C_SDRC_DBW_16_BITS | AT91C_SDRC_MODE_LMR_CMD;
   *AT91C_SDRAM_BASE = 0x00000000;

   //The application must go into Normal Mode, setting Mode to 0 in the Mode Register and
   //performing a write access at any location in the SDRAM
   AT91C_BASE_SDRC->SDRC_MR= AT91C_SDRC_DBW_16_BITS | AT91C_SDRC_MODE_NORMAL_CMD;
   *AT91C_SDRAM_BASE = 0x00000000;

   //Write the refresh rate into the count field in the SDRAM Refresh Timer Register
   AT91C_BASE_SDRC->SDRC_TR = AT91C_SDRC_TR_TIME;
}


/**
 * @brief SDRAM test routine
 * @return Error code
 * @retval #NO_ERROR Successful test
 * @retval #ERROR_FAILURE Failure when reading back the data
 **/

error_t sdramTest(void)
{
   uint_t i;

   //Point to the beginning of the memory space
   uint32_t *address = (uint32_t *) AT91C_SDRAM_BASE;
   //Initialize test pattern generation
   uint32_t value = 0x12345678;

   //Write SDRAM memory contents
   for(i = 0; i < (AT91C_SDRAM_SIZE / 4); i++)
   {
      //Write current location
      *(address++) = value;
      //Test pattern generation
      value = value * 0x7AB5 + 0x5E8AC93D;
   }

   //Point to the beginning of the memory space
   address = (uint32_t *) AT91C_SDRAM_BASE;
   //Initialize test pattern generation
   value = 0x12345678;

   //Read back and check SDRAM memory contents
   for(i = 0; i < (AT91C_SDRAM_SIZE / 4); i++)
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
