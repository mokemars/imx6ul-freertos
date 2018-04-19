/**
 * @file nm_bus_wrapper_pic32mz.c
 * @brief SPI bus wrapper for PIC32MZ microcontrollers
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

//Dependencies
#include <stdio.h>
#include <p32xxxx.h>
#include "bsp/include/nm_bsp.h"
#include "common/include/nm_common.h"
#include "bus_wrapper/include/nm_bus_wrapper.h"

//Maximum transfer size
#define NM_BUS_MAX_TRX_SZ 4096

//SPI instance
#if (CONF_WILC_SPI_INSTANCE == 1)
   #define SPIxCON    SPI1CON
   #define SPIxCONSET SPI1CONSET
   #define SPIxSTAT   SPI1STAT
   #define SPIxBUF    SPI1BUF
   #define SPIxBRG    SPI1BRG
#elif (CONF_WILC_SPI_INSTANCE == 2)
   #define SPIxCON    SPI2CON
   #define SPIxCONSET SPI2CONSET
   #define SPIxSTAT   SPI2STAT
   #define SPIxBUF    SPI2BUF
   #define SPIxBRG    SPI2BRG
#elif (CONF_WILC_SPI_INSTANCE == 3)
   #define SPIxCON    SPI3CON
   #define SPIxCONSET SPI3CONSET
   #define SPIxSTAT   SPI3STAT
   #define SPIxBUF    SPI3BUF
   #define SPIxBRG    SPI3BRG
#elif (CONF_WILC_SPI_INSTANCE == 4)
   #define SPIxCON    SPI4CON
   #define SPIxCONSET SPI4CONSET
   #define SPIxSTAT   SPI4STAT
   #define SPIxBUF    SPI4BUF
   #define SPIxBRG    SPI4BRG
#elif (CONF_WILC_SPI_INSTANCE == 5)
   #define SPIxCON    SPI5CON
   #define SPIxCONSET SPI5CONSET
   #define SPIxSTAT   SPI5STAT
   #define SPIxBUF    SPI5BUF
   #define SPIxBRG    SPI5BRG
#elif (CONF_WILC_SPI_INSTANCE == 6)
   #define SPIxCON    SPI6CON
   #define SPIxCONSET SPI6CONSET
   #define SPIxSTAT   SPI6STAT
   #define SPIxBUF    SPI6BUF
   #define SPIxBRG    SPI6BRG
#endif


/**
 * @brief Bus capabilities information
 **/

tstrNmBusCapabilities egstrNmBusCapabilities =
{
   NM_BUS_MAX_TRX_SZ
};


/**
 * @brief SPI transfer
 * @param[in] pu8Mosi The data to be written to the slave device
 * @param[out] pu8Miso The data received from the slave device
 * @param[in] u16Sz Number of bytes to be transferred
 * @return Status code (M2M_SUCCESS or M2M_ERR_BUS_FAIL)
 **/

static sint8 spi_rw(uint8 *pu8Mosi, uint8 *pu8Miso, uint16 u16Sz)
{
   uint16 i;
   uint8 data;

   //Pull the CS pin low
   CONF_WILC_CS_CLR();

   //Perform SPI
   for(i = 0; i < u16Sz; i++)
   {
      //Full-duplex transfer?
      if(pu8Mosi != NULL)
         data = pu8Mosi[i];
      else
         data = 0x00;

      //Ensure the TX buffer is empty
      while(!(SPIxSTAT & _SPI1STAT_SPITBE_MASK));
      //Write character
      SPIxBUF = data;
      //Wait for the operation to complete
      while(!(SPIxSTAT & _SPI1STAT_SPIRBF_MASK));
      //Read character
      data = SPIxBUF;

      //Save the received character
      if(pu8Miso != NULL)
         pu8Miso[i] = data;
   }

   //Terminate the operation by raising the CS pin
   CONF_WILC_CS_SET();

   //Successful operation
   return M2M_SUCCESS;
}


/**
 * @brief SPI bus initialization
 * @param[in] pvinit Unused parameter
 * @return Status code (M2M_SUCCESS or M2M_ERR_BUS_FAIL)
 **/

sint8 nm_bus_init(void *pvinit)
{
   uint32 div;

   //Configure SCK pin
   CONF_WILC_SCK_INIT();
   //Configure MOSI pin
   CONF_WILC_MOSI_INIT();
   //Configure MISO pin
   CONF_WILC_MISO_INIT();

   //Select master mode operation
   SPIxCON = _SPI1CON_CKE_MASK | _SPI1CON_MSTEN_MASK;
   //Set SCK clock frequency
   SPIxBRG = CONF_WILC_SPI_BRG;
   //Enable SPI module
   SPIxCONSET = _SPI1CON_ON_MASK;

   //Reset WILC1000
   nm_bsp_reset();

   //Successful operation
   return M2M_SUCCESS;
}


/**
 * @brief IOCTL command processing
 * @param[in] u8Cmd Command opcode
 * @param[in] pvParameter Command parameters
 * @return Status code (M2M_SUCCESS or M2M_ERR_BUS_FAIL)
 **/

sint8 nm_bus_ioctl(uint8 u8Cmd, void *pvParameter)
{
   sint8 ret;
#ifdef CONF_WILC_USE_SPI
   tstrNmSpiRw *spiRwParams;
#endif

   //Check commande opcode
   switch(u8Cmd)
   {
#ifdef CONF_WILC_USE_SPI
   //Read/write command?
   case NM_BUS_IOCTL_RW:
      //Retrieve command parameters
      spiRwParams = (tstrNmSpiRw *) pvParameter;
      //Perform SPI transfer
      ret = spi_rw(spiRwParams->pu8InBuf, spiRwParams->pu8OutBuf, spiRwParams->u16Sz);
      break;
#endif
   //Invalid command?
   default:
      //Debug message
      M2M_ERR("Invalid IOCTL command!\r\n");
      //Report an error
      ret = M2M_ERR_BUS_FAIL;
      break;
   }

   //Return status code
   return ret;
}


/**
 * @brief SPI bus deinitialization
 * @param[in] pvinit Unused parameter
 * @return Status code (M2M_SUCCESS or M2M_ERR_BUS_FAIL)
 **/

sint8 nm_bus_deinit(void)
{
   //Not implemented
   return M2M_SUCCESS;
}
