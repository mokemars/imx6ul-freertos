/**
 * @file nm_bus_wrapper_stm32f1xx.c
 * @brief SPI bus wrapper for STM32F1xx microcontrollers
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
#if defined(USE_STDPERIPH_DRIVER)
   #include "stm32f10x.h"
#elif defined(USE_HAL_DRIVER)
   #include "stm32f1xx.h"
#endif

#include <stdio.h>
#include "bsp/include/nm_bsp.h"
#include "common/include/nm_common.h"
#include "bus_wrapper/include/nm_bus_wrapper.h"

//Maximum transfer size
#define NM_BUS_MAX_TRX_SZ 4096

//SPI instance
#if defined(USE_HAL_DRIVER)
   static SPI_HandleTypeDef SPI_Handle;
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
   uint16_t i;
   uint8_t data;

   //Pull the CS pin low
#if defined(USE_HAL_DRIVER)
   HAL_GPIO_WritePin(CONF_WILC_CS_GPIO, CONF_WILC_CS_PIN, GPIO_PIN_RESET);
#elif defined(USE_STDPERIPH_DRIVER)
   GPIO_ResetBits(CONF_WILC_CS_GPIO, CONF_WILC_CS_PIN);
#endif

   //Perform SPI
   for(i = 0; i < u16Sz; i++)
   {
      //Full-duplex transfer?
      if(pu8Mosi != NULL)
         data = pu8Mosi[i];
      else
         data = 0x00;

      //Ensure the TX buffer is empty
      while(!(CONF_WILC_SPI->SR & SPI_SR_TXE));
      //Write character
      CONF_WILC_SPI->DR = data;
      //Wait for the operation to complete
      while(!(CONF_WILC_SPI->SR & SPI_SR_RXNE));
      //Read character
      data = CONF_WILC_SPI->DR;

      //Save the received character
      if(pu8Miso != NULL)
         pu8Miso[i] = data;
   }

   //Terminate the operation by raising the CS pin
#if defined(USE_HAL_DRIVER)
   HAL_GPIO_WritePin(CONF_WILC_CS_GPIO, CONF_WILC_CS_PIN, GPIO_PIN_SET);
#elif defined(USE_STDPERIPH_DRIVER)
   GPIO_SetBits(CONF_WILC_CS_GPIO, CONF_WILC_CS_PIN);
#endif

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
#if defined(USE_STDPERIPH_DRIVER)
   SPI_InitTypeDef SPI_InitStucture;
#endif

   //Enable GPIO clocks
   CONF_WILC_SCK_GPIO_CLK_ENABLE();
   CONF_WILC_MOSI_GPIO_CLK_ENABLE();
   CONF_WILC_MISO_GPIO_CLK_ENABLE();

   //Enable SPI clock
   CONF_WILC_SPI_CLK_ENABLE();

#if defined(USE_HAL_DRIVER)
   //Configure SCK
   GPIO_InitStructure.Pin = CONF_WILC_SCK_PIN;
   GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStructure.Pull = GPIO_NOPULL;
   GPIO_InitStructure.Speed = CONF_WILC_SCK_SPEED;
   HAL_GPIO_Init(CONF_WILC_SCK_GPIO, &GPIO_InitStructure);

   //Configure MOSI
   GPIO_InitStructure.Pin = CONF_WILC_MOSI_PIN;
   GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStructure.Pull = GPIO_NOPULL;
   GPIO_InitStructure.Speed = CONF_WILC_MOSI_SPEED;
   HAL_GPIO_Init(CONF_WILC_MOSI_GPIO, &GPIO_InitStructure);

   //Configure MISO
   GPIO_InitStructure.Pin = CONF_WILC_MISO_PIN;
   GPIO_InitStructure.Mode = GPIO_MODE_AF_INPUT;
   GPIO_InitStructure.Pull = GPIO_NOPULL;
   GPIO_InitStructure.Speed = CONF_WILC_MISO_SPEED;
   HAL_GPIO_Init(CONF_WILC_MISO_GPIO, &GPIO_InitStructure);

   //Remap SPI pins
   CONF_WILC_SPI_PIN_REMAP_CONFIG();

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
   SPI_Handle.Init.TIMode = SPI_TIMODE_DISABLE;
   SPI_Handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
   SPI_Handle.Init.CRCPolynomial = 7;
   HAL_SPI_Init(&SPI_Handle);

   //Enable SPI
   __HAL_SPI_ENABLE(&SPI_Handle);

#elif defined(USE_STDPERIPH_DRIVER)
   //Configure SCK
   GPIO_InitStructure.GPIO_Pin = CONF_WILC_SCK_PIN;
   GPIO_InitStructure.GPIO_Speed = CONF_WILC_SCK_SPEED;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_Init(CONF_WILC_SCK_GPIO, &GPIO_InitStructure);

   //Configure MOSI
   GPIO_InitStructure.GPIO_Pin = CONF_WILC_MOSI_PIN;
   GPIO_InitStructure.GPIO_Speed = CONF_WILC_MOSI_SPEED;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_Init(CONF_WILC_MOSI_GPIO, &GPIO_InitStructure);

   //Configure MISO
   GPIO_InitStructure.GPIO_Pin = CONF_WILC_MISO_PIN;
   GPIO_InitStructure.GPIO_Speed = CONF_WILC_MISO_SPEED;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   GPIO_Init(CONF_WILC_MISO_GPIO, &GPIO_InitStructure);

   //Remap SPI pins
   CONF_WILC_SPI_PIN_REMAP_CONFIG();

   //Configure SPI
   SPI_InitStucture.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
   SPI_InitStucture.SPI_Mode = SPI_Mode_Master;
   SPI_InitStucture.SPI_DataSize = SPI_DataSize_8b;
   SPI_InitStucture.SPI_CPOL = SPI_CPOL_Low;
   SPI_InitStucture.SPI_CPHA = SPI_CPHA_1Edge;
   SPI_InitStucture.SPI_NSS = SPI_NSS_Soft;
   SPI_InitStucture.SPI_BaudRatePrescaler = CONF_WILC_SPI_BAUDRATE_PRE;
   SPI_InitStucture.SPI_FirstBit = SPI_FirstBit_MSB;
   SPI_InitStucture.SPI_CRCPolynomial = 7;
   SPI_Init(CONF_WILC_SPI, &SPI_InitStucture);

   //Enable SPI
   SPI_Cmd(CONF_WILC_SPI, ENABLE);
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
