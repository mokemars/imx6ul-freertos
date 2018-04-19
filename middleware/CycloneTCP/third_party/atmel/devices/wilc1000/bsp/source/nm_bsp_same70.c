/**
 * @file nm_bsp_same70.c
 * @brief BSP for SAME70 microcontrollers
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
#include "same70.h"
#include "bsp/include/nm_bsp.h"
#include "common/include/nm_common.h"
#include "core/net.h"
#include "drivers/wifi/wilc1000_driver.h"

//IRQ handler
static tpfNmBspIsr gpfIsr;


/**
 * @brief GPIO initialization
 **/

static void init_chip_pins(void)
{
   //Enable PIO peripheral clocks
   PMC->PMC_PCER0 = (1 << CONF_WILC_RST_ID_PIO);
   PMC->PMC_PCER0 = (1 << CONF_WILC_CE_ID_PIO);
   PMC->PMC_PCER0 = (1 << CONF_WILC_WAKE_ID_PIO);
   PMC->PMC_PCER0 = (1 << CONF_WILC_CS_ID_PIO);
   PMC->PMC_PCER0 = (1 << CONF_WILC_IRQ_ID_PIO);

   //Configure RST pin
   CONF_WILC_RST_PIO->PIO_PER = CONF_WILC_RST_PIN;
   CONF_WILC_RST_PIO->PIO_OER = CONF_WILC_RST_PIN;
   CONF_WILC_RST_PIO->PIO_CODR = CONF_WILC_RST_PIN;

   //Configure CE pin
   CONF_WILC_CE_PIO->PIO_PER = CONF_WILC_CE_PIN;
   CONF_WILC_CE_PIO->PIO_OER = CONF_WILC_CE_PIN;
   CONF_WILC_CE_PIO->PIO_CODR = CONF_WILC_CE_PIN;

   //Configure WAKE pin
   CONF_WILC_WAKE_PIO->PIO_PER = CONF_WILC_WAKE_PIN;
   CONF_WILC_WAKE_PIO->PIO_OER = CONF_WILC_WAKE_PIN;
   CONF_WILC_WAKE_PIO->PIO_CODR = CONF_WILC_WAKE_PIN;

   //Configure CS pin
   CONF_WILC_CS_PIO->PIO_PER = CONF_WILC_CS_PIN;
   CONF_WILC_CS_PIO->PIO_OER = CONF_WILC_CS_PIN;
   CONF_WILC_CS_PIO->PIO_SODR = CONF_WILC_CS_PIN;

   //Configure IRQ pin
   CONF_WILC_IRQ_PIO->PIO_PUER = CONF_WILC_IRQ_PIN;
   CONF_WILC_IRQ_PIO->PIO_ODR = CONF_WILC_IRQ_PIN;
   CONF_WILC_IRQ_PIO->PIO_PER = CONF_WILC_IRQ_PIN;
}


/**
 * @brief BSP initialization
 * @return Status code
 **/

sint8 nm_bsp_init(void)
{
   //Initialize IRQ handler
   gpfIsr = NULL;

   //Initialize WILC1000 pins
   init_chip_pins();

   //Reset WILC1000 chip
   nm_bsp_reset();

   //No error to report
   return M2M_SUCCESS;
}


/**
 * @brief Reset chip
 **/

void nm_bsp_reset(void)
{
   //Debug message
   M2M_PRINT("nm_bsp_reset\r\n");

   //Set CE and RST pins low
   CONF_WILC_CE_PIO->PIO_CODR = CONF_WILC_CE_PIN;
   CONF_WILC_RST_PIO->PIO_CODR = CONF_WILC_RST_PIN;
   nm_bsp_sleep(100);

   //Set CE pin high
   CONF_WILC_CE_PIO->PIO_SODR = CONF_WILC_CE_PIN;
   nm_bsp_sleep(100);

   //Set RST pin high
   CONF_WILC_RST_PIO->PIO_SODR = CONF_WILC_RST_PIN;
   nm_bsp_sleep(100);
}


/**
 * @brief Delay routine
 * @param[in] u32TimeMsec Time interval in milliseconds
 **/

void nm_bsp_sleep(uint32 u32TimeMsec)
{
   while(u32TimeMsec--)
   {
      sleep(1);
   }
}

/**
 * @brief Register interrupt service routine
 * @param[in] pfIsr ISR handler
 **/

void nm_bsp_register_isr(tpfNmBspIsr pfIsr)
{
   //Save ISR handler
   gpfIsr = pfIsr;

   //Enable level detection
   CONF_WILC_IRQ_PIO->PIO_LSR = CONF_WILC_IRQ_PIN;
   CONF_WILC_IRQ_PIO->PIO_FELLSR = CONF_WILC_IRQ_PIN;
   CONF_WILC_IRQ_PIO->PIO_IER = CONF_WILC_IRQ_PIN;

   //Set priority grouping
   NVIC_SetPriorityGrouping(CONF_WILC_IRQ_PRIORITY_GROUPING);

   //Configure PIO interrupt priority
   NVIC_SetPriority(CONF_WILC_IRQn, NVIC_EncodePriority(CONF_WILC_IRQ_PRIORITY_GROUPING,
      CONF_WILC_IRQ_GROUP_PRIORITY, CONF_WILC_IRQ_SUB_PRIORITY));

   //Enable PIO interrupt
   NVIC_EnableIRQ(CONF_WILC_IRQn);
   NVIC_SetPendingIRQ(CONF_WILC_IRQn);
}


/**
 * @brief Enable/disable interrupts
 * @param[in] u8Enable Interrupt enable state
 **/

void nm_bsp_interrupt_ctrl(uint8 u8Enable)
{
   if(u8Enable)
   {
      //Enable external interrupt
      NVIC_EnableIRQ(CONF_WILC_IRQn);
   }
   else
   {
      //Disable external interrupt
      NVIC_DisableIRQ(CONF_WILC_IRQn);
   }
}


/**
 * @brief IRQ handler
 **/

void CONF_WILC_IRQHandler(void)
{
   bool_t flag;
   volatile uint32_t status;

   //Enter interrupt service routine
   osEnterIsr();

   //This flag will be set if a higher priority task must be woken
   flag = FALSE;

   //Read status register to clear interrupt
   status = CONF_WILC_IRQ_PIO->PIO_ISR;

   //Ensure the IRQ pin is asserted
   if(!(CONF_WILC_IRQ_PIO->PIO_PDSR & CONF_WILC_IRQ_PIN))
   {
      //WILC1000 interrupt processing
      if(gpfIsr != NULL)
         gpfIsr();

      //Call interrupt handler
      flag = wilc1000IrqHandler();
   }

   //Leave interrupt service routine
   osExitIsr(flag);
}
