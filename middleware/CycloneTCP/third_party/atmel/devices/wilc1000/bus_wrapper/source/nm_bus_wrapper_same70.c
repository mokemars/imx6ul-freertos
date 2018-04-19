/**
 * @file nm_bus_wrapper_same70.c
 * @brief SPI bus wrapper for SAME70 microcontrollers
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
#include "same70.h"
#include "bsp/include/nm_bsp.h"
#include "common/include/nm_common.h"
#include "bus_wrapper/include/nm_bus_wrapper.h"

//Maximum transfer size
#define NM_BUS_MAX_TRX_SZ 4096

//IAR EWARM compiler?
#if defined(__ICCARM__)

//TX buffer
#pragma data_alignment = 4
#pragma location = ".ram_no_cache"
static uint8_t txBuffer[NM_BUS_MAX_TRX_SZ];
//RX buffer
#pragma data_alignment = 4
#pragma location = ".ram_no_cache"
static uint8_t rxBuffer[NM_BUS_MAX_TRX_SZ];

//Keil MDK-ARM or GCC compiler?
#else

//TX buffer
static uint8_t txBuffer[NM_BUS_MAX_TRX_SZ]
   __attribute__((aligned(4), __section__(".ram_no_cache")));
//RX buffer
static uint8_t rxBuffer[NM_BUS_MAX_TRX_SZ]
   __attribute__((aligned(4), __section__(".ram_no_cache")));

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
   volatile uint32 status;

   //Pull the CS pin low
   CONF_WILC_CS_PIO->PIO_CODR = CONF_WILC_CS_PIN;

   //Copy data to TX buffer
   if(pu8Mosi != NULL)
      m2m_memcpy(txBuffer, pu8Mosi, u16Sz);
   else
      m2m_memset(txBuffer, 0, u16Sz);

   //Clear pending interrupt status flags by reading the interrupt status
   status = XDMAC->XDMAC_CHID[1].XDMAC_CIS;
   //Set source address
   XDMAC->XDMAC_CHID[1].XDMAC_CSA = (uint32_t) txBuffer;
   //Set destination address
   XDMAC->XDMAC_CHID[1].XDMAC_CDA = (uint32_t) &(CONF_WILC_SPI->SPI_TDR);
   //Set the number of data to be transferred
   XDMAC->XDMAC_CHID[1].XDMAC_CUBC = XDMAC_CUBC_UBLEN(u16Sz);

   //Program configuration register
   XDMAC->XDMAC_CHID[1].XDMAC_CC =
      XDMAC_CC_TYPE_PER_TRAN |      //Peripheral transfer
      XDMAC_CC_MBSIZE_SINGLE |      //Memory burst size
      XDMAC_CC_SAM_INCREMENTED_AM | //Source memory addressing scheme
      XDMAC_CC_DAM_FIXED_AM |       //Destination memory addressing scheme
      XDMAC_CC_DSYNC_MEM2PER |      //Peripheral transfer direction
      XDMAC_CC_CSIZE_CHK_1 |        //Channel chunk size
      XDMAC_CC_DWIDTH_BYTE |        //Transfer data width
      XDMAC_CC_SIF_AHB_IF0 |        //Master interface used to read data
      XDMAC_CC_DIF_AHB_IF1 |        //Master interface used to write data
      XDMAC_CC_PERID(1) |           //Peripheral identifier
      XDMAC_CC_SWREQ_HWR_CONNECTED;

   //Clear the following five registers
   XDMAC->XDMAC_CHID[1].XDMAC_CNDC = 0;
   XDMAC->XDMAC_CHID[1].XDMAC_CBC = 0;
   XDMAC->XDMAC_CHID[1].XDMAC_CDS_MSP = 0;
   XDMAC->XDMAC_CHID[1].XDMAC_CSUS = 0;
   XDMAC->XDMAC_CHID[1].XDMAC_CDUS = 0;

   //Enable the microblock interrupt
   XDMAC->XDMAC_CHID[1].XDMAC_CIE = XDMAC_CIE_BIE;

   //Clear pending interrupt status flags by reading the interrupt status
   status = XDMAC->XDMAC_CHID[2].XDMAC_CIS;
   //Set source address
   XDMAC->XDMAC_CHID[2].XDMAC_CSA = (uint32_t) &(CONF_WILC_SPI->SPI_RDR);
   //Set destination address
   XDMAC->XDMAC_CHID[2].XDMAC_CDA = (uint32_t) rxBuffer;
   //Set the number of data to be transferred
   XDMAC->XDMAC_CHID[2].XDMAC_CUBC = XDMAC_CUBC_UBLEN(u16Sz);

   //Program configuration register
   XDMAC->XDMAC_CHID[2].XDMAC_CC =
      XDMAC_CC_TYPE_PER_TRAN |      //Peripheral transfer
      XDMAC_CC_MBSIZE_SINGLE |      //Memory burst size
      XDMAC_CC_SAM_FIXED_AM |       //Source memory addressing scheme
      XDMAC_CC_DAM_INCREMENTED_AM | //Destination memory addressing scheme
      XDMAC_CC_DSYNC_PER2MEM |      //Peripheral transfer direction
      XDMAC_CC_CSIZE_CHK_1 |        //Channel chunk size
      XDMAC_CC_DWIDTH_BYTE |        //Transfer data width
      XDMAC_CC_SIF_AHB_IF1 |        //Master interface used to read data
      XDMAC_CC_DIF_AHB_IF0 |        //Master interface used to write data
      XDMAC_CC_PERID(2) |           //Channel peripheral identifier
      XDMAC_CC_SWREQ_HWR_CONNECTED;

   //Clear the following five registers
   XDMAC->XDMAC_CHID[2].XDMAC_CNDC = 0;
   XDMAC->XDMAC_CHID[2].XDMAC_CBC = 0;
   XDMAC->XDMAC_CHID[2].XDMAC_CDS_MSP = 0;
   XDMAC->XDMAC_CHID[2].XDMAC_CSUS = 0;
   XDMAC->XDMAC_CHID[2].XDMAC_CDUS = 0;

   //Enable the microblock interrupt
   XDMAC->XDMAC_CHID[2].XDMAC_CIE = XDMAC_CIE_BIE;

   //Enable channel 1 and channel 2 interrupt enable bit
   XDMAC->XDMAC_GIE = XDMAC_GIE_IE1 | XDMAC_GIE_IE2;
   //Enable channel 1 (SPI0 TX) and channel 2 (SPI0 RX)
   XDMAC->XDMAC_GE = XDMAC_GE_EN1 | XDMAC_GE_EN2;

   //Wait for the DMA transfer to be complete
   while(!(XDMAC->XDMAC_CHID[1].XDMAC_CIS & XDMAC_CIS_BIS));
   while(!(XDMAC->XDMAC_CHID[2].XDMAC_CIS & XDMAC_CIS_BIS));
   while(!(CONF_WILC_SPI->SPI_SR & SPI_SR_TXEMPTY));

   //Disable channel 1 and channel 2
   XDMAC->XDMAC_GD = XDMAC_GD_DI1 | XDMAC_GD_DI2;
   //Disable channel 1 and channel 2 interrupts
   XDMAC->XDMAC_GID = XDMAC_GID_ID1 | XDMAC_GID_ID2;

   //Copy data from RX buffer
   if(pu8Miso != NULL)
      m2m_memcpy(pu8Miso, rxBuffer, u16Sz);

   //Terminate the operation by raising the CS pin
   CONF_WILC_CS_PIO->PIO_SODR = CONF_WILC_CS_PIN;

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

   //Enable PIO peripheral clock
   PMC->PMC_PCER0 = (1 << CONF_WILC_SPI_ID_PIO);
   //Enable SPI peripheral clock
   PMC->PMC_PCER0 = (1 << CONF_WILC_SPI_ID);
   //Enable XDMAC peripheral clock
   PMC->PMC_PCER1 = (1 << (ID_XDMAC - 32));

   //Disable interrupts on SPI pins
   CONF_WILC_SPI_PIO->PIO_IDR = CONF_WILC_SCK_PIN | CONF_WILC_MOSI_PIN | CONF_WILC_MISO_PIN;
   //Internal pull-up resistors
   CONF_WILC_SPI_PIO->PIO_PUER = CONF_WILC_SCK_PIN | CONF_WILC_MOSI_PIN | CONF_WILC_MISO_PIN;
   //Assign corresponding pins to Peripheral B function
   CONF_WILC_SPI_PIO->PIO_ABCDSR[0] |= CONF_WILC_SCK_PIN | CONF_WILC_MOSI_PIN | CONF_WILC_MISO_PIN;
   CONF_WILC_SPI_PIO->PIO_ABCDSR[1] &= ~(CONF_WILC_SCK_PIN | CONF_WILC_MOSI_PIN | CONF_WILC_MISO_PIN);
   //Disable the PIO from controlling the corresponding pins
   CONF_WILC_SPI_PIO->PIO_PDR = CONF_WILC_SCK_PIN | CONF_WILC_MOSI_PIN | CONF_WILC_MISO_PIN;

   //Disable SPI module
   CONF_WILC_SPI->SPI_CR = SPI_CR_SPIDIS;
   //Reset SPI module
   CONF_WILC_SPI->SPI_CR = SPI_CR_SWRST;
   CONF_WILC_SPI->SPI_CR = SPI_CR_SWRST;

   //Master mode operation
   CONF_WILC_SPI->SPI_MR = SPI_MR_MODFDIS | SPI_MR_MSTR;

   //Calculate clock divider
   div = SystemCoreClock / CONF_WILC_SPI_CLOCK;

   //SPI configuration (8-bit words, clock phase = 1, clock polarity = 0)
   CONF_WILC_SPI->SPI_CSR[0] = SPI_CSR_SCBR(div) | SPI_CSR_BITS_8_BIT | SPI_CSR_NCPHA;

   //Enable SPI module
   CONF_WILC_SPI->SPI_CR = SPI_CR_SPIEN;

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
