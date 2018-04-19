/**
 * @file nm_bus_wrapper_stm32h7xx.c
 * @brief SPI bus wrapper for STM32H7xx microcontrollers
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
#include <string.h>
#include "stm32h7xx.h"
#include "bsp/include/nm_bsp.h"
#include "common/include/nm_common.h"
#include "bus_wrapper/include/nm_bus_wrapper.h"

//Maximum transfer size
#define NM_BUS_MAX_TRX_SZ 4096

//SPI instance
static SPI_HandleTypeDef SPI_Handle;

//TX buffer
static uint8_t txBuffer[NM_BUS_MAX_TRX_SZ];
//RX buffer
static uint8_t rxBuffer[NM_BUS_MAX_TRX_SZ];


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
   uint16_t i;
   size_t j;
   size_t n;

   //Pull the CS pin low
   __DSB();
   HAL_GPIO_WritePin(CONF_WILC_CS_GPIO, CONF_WILC_CS_PIN, GPIO_PIN_RESET);
   __DSB();

#if 0
   //Copy data to TX buffer
   if(pu8Mosi != NULL)
      memcpy(txBuffer, pu8Mosi, u16Sz);
   else
      memset(txBuffer, 0, u16Sz);

   //Perform SPI transfer
   HAL_SPI_TransmitReceive(&SPI_Handle, txBuffer, rxBuffer, u16Sz, HAL_MAX_DELAY);

   //Copy data from RX buffer
   if(pu8Miso != NULL)
      memcpy(pu8Miso, rxBuffer, u16Sz);
#else
   //Transfer the requested number of bytes
   for(i = 0; i < u16Sz; i += n)
   {
      //Limit the number of bytes to transfer at a time
      if((u16Sz - i) <= 16)
         n = u16Sz - i;
      else
         n = 16;

      //Prepare TX buffer
      if(pu8Mosi != NULL)
         memcpy(txBuffer, pu8Mosi + i, n);
      else
         memset(txBuffer, 0, n);

      //Wait for the transmitter to be ready
      while(!(CONF_WILC_SPI->SR & SPI_SR_TXC));

      //Send data
      for(j = 0; j < n; j++)
          *((uint8_t *) &CONF_WILC_SPI->TXDR) = txBuffer[j];

      //Wait for the transfer to complete
      while(!(CONF_WILC_SPI->SR & SPI_SR_TXC));

      //Receive data
      for(j = 0; j < n; j++)
          rxBuffer[j] = *((uint8_t *) &CONF_WILC_SPI->RXDR);

      //Copy received data to the user buffer
      if(pu8Miso != NULL)
         memcpy(pu8Miso + i, rxBuffer, n);
   }

   //Wait for the SPI transfer to complete
   while(!(CONF_WILC_SPI->SR & SPI_SR_TXC));
#endif

   //Terminate the operation by raising the CS pin
   __DSB();
   HAL_GPIO_WritePin(CONF_WILC_CS_GPIO, CONF_WILC_CS_PIN, GPIO_PIN_SET);
   __DSB();

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
   GPIO_InitTypeDef GPIO_InitStructure;

   //Enable GPIO clocks
   CONF_WILC_SCK_GPIO_CLK_ENABLE();
   CONF_WILC_MOSI_GPIO_CLK_ENABLE();
   CONF_WILC_MISO_GPIO_CLK_ENABLE();

   //Enable SPI clock
   CONF_WILC_SPI_CLK_ENABLE();

   //Configure SCK
   GPIO_InitStructure.Pin = CONF_WILC_SCK_PIN;
   GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStructure.Pull = GPIO_NOPULL;
   GPIO_InitStructure.Speed = CONF_WILC_SCK_SPEED;
   GPIO_InitStructure.Alternate = CONF_WILC_SPI_ALTERNATE;
   HAL_GPIO_Init(CONF_WILC_SCK_GPIO, &GPIO_InitStructure);

   //Configure MOSI
   GPIO_InitStructure.Pin = CONF_WILC_MOSI_PIN;
   GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStructure.Pull = GPIO_NOPULL;
   GPIO_InitStructure.Speed = CONF_WILC_MOSI_SPEED;
   GPIO_InitStructure.Alternate = CONF_WILC_SPI_ALTERNATE;
   HAL_GPIO_Init(CONF_WILC_MOSI_GPIO, &GPIO_InitStructure);

   //Configure MISO
   GPIO_InitStructure.Pin = CONF_WILC_MISO_PIN;
   GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStructure.Pull = GPIO_NOPULL;
   GPIO_InitStructure.Speed = CONF_WILC_MISO_SPEED;
   GPIO_InitStructure.Alternate = CONF_WILC_SPI_ALTERNATE;
   HAL_GPIO_Init(CONF_WILC_MISO_GPIO, &GPIO_InitStructure);

#if 0
   //Configure SPI
   SPI_Handle.Instance = CONF_WILC_SPI;
   SPI_Handle.Init.Mode = SPI_MODE_MASTER;
   SPI_Handle.Init.Direction = SPI_DIRECTION_2LINES;
   SPI_Handle.Init.DataSize = SPI_DATASIZE_8BIT;
   SPI_Handle.Init.CLKPolarity = SPI_POLARITY_LOW;
   SPI_Handle.Init.CLKPhase = SPI_PHASE_1EDGE;
   SPI_Handle.Init.NSS = SPI_NSS_SOFT;
   SPI_Handle.Init.BaudRatePrescaler = CONF_WILC_SPI_BAUDRATE_PRE;
   SPI_Handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
   SPI_Handle.Init.TIMode = SPI_TIMODE_DISABLED;
   SPI_Handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
   SPI_Handle.Init.CRCPolynomial = 7;
   HAL_SPI_Init(&SPI_Handle);

   //Enable SPI
   __HAL_SPI_ENABLE(&SPI_Handle);
#else
   //Disable SPI
   CONF_WILC_SPI->CR1 &= ~SPI_CR1_SPE;

   //Configure SPI
   CONF_WILC_SPI->CFG1 = ((3 << 28) & SPI_CFG1_MBR) | ((7 << 16) & SPI_CFG1_CRCSIZE) | (7 & SPI_CFG1_DSIZE);
   CONF_WILC_SPI->CFG2 = SPI_CFG2_MASTER | SPI_CFG2_SSOE;
   CONF_WILC_SPI->I2SCFGR = 0;
   CONF_WILC_SPI->CR2 = 0;

   //Enable SPI
   CONF_WILC_SPI->CR1 = SPI_CR1_SPE;
   CONF_WILC_SPI->CR1 |= SPI_CR1_CSTART;
#endif

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
