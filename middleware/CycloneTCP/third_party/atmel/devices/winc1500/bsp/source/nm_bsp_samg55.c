/**
 * @file nm_bsp_samg55.c
 * @brief BSP for SAMG55 microcontrollers
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
#include "samg55.h"
#include "bsp/include/nm_bsp.h"
#include "common/include/nm_common.h"
#include "core/net.h"
#include "drivers/wifi/winc1500_driver.h"

//IRQ handler
static tpfNmBspIsr gpfIsr;


/**
 * @brief GPIO initialization
 **/

static void init_chip_pins(void)
{
   //Enable PIO peripheral clocks
   PMC->PMC_PCER0 = (1 << CONF_WINC_RST_ID_PIO);
   PMC->PMC_PCER0 = (1 << CONF_WINC_CE_ID_PIO);
   PMC->PMC_PCER0 = (1 << CONF_WINC_WAKE_ID_PIO);
   PMC->PMC_PCER0 = (1 << CONF_WINC_CS_ID_PIO);
   PMC->PMC_PCER0 = (1 << CONF_WINC_IRQ_ID_PIO);

   //Configure RST pin
   CONF_WINC_RST_PIO->PIO_PER = CONF_WINC_RST_PIN;
   CONF_WINC_RST_PIO->PIO_OER = CONF_WINC_RST_PIN;
   CONF_WINC_RST_PIO->PIO_CODR = CONF_WINC_RST_PIN;

   //Configure CE pin
   CONF_WINC_CE_PIO->PIO_PER = CONF_WINC_CE_PIN;
   CONF_WINC_CE_PIO->PIO_OER = CONF_WINC_CE_PIN;
   CONF_WINC_CE_PIO->PIO_CODR = CONF_WINC_CE_PIN;

   //Configure WAKE pin
   CONF_WINC_WAKE_PIO->PIO_PER = CONF_WINC_WAKE_PIN;
   CONF_WINC_WAKE_PIO->PIO_OER = CONF_WINC_WAKE_PIN;
   CONF_WINC_WAKE_PIO->PIO_CODR = CONF_WINC_WAKE_PIN;

   //Configure CS pin
   CONF_WINC_CS_PIO->PIO_PER = CONF_WINC_CS_PIN;
   CONF_WINC_CS_PIO->PIO_OER = CONF_WINC_CS_PIN;
   CONF_WINC_CS_PIO->PIO_SODR = CONF_WINC_CS_PIN;

   //Configure IRQ pin
   CONF_WINC_IRQ_PIO->PIO_PUER = CONF_WINC_IRQ_PIN;
   CONF_WINC_IRQ_PIO->PIO_ODR = CONF_WINC_IRQ_PIN;
   CONF_WINC_IRQ_PIO->PIO_PER = CONF_WINC_IRQ_PIN;
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
   CONF_WINC_CE_PIO->PIO_CODR = CONF_WINC_CE_PIN;
   CONF_WINC_RST_PIO->PIO_CODR = CONF_WINC_RST_PIN;
   nm_bsp_sleep(100);

   //Set CE pin high
   CONF_WINC_CE_PIO->PIO_SODR = CONF_WINC_CE_PIN;
   nm_bsp_sleep(100);

   //Set RST pin high
   CONF_WINC_RST_PIO->PIO_SODR = CONF_WINC_RST_PIN;
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
   CONF_WINC_IRQ_PIO->PIO_LSR = CONF_WINC_IRQ_PIN;
   CONF_WINC_IRQ_PIO->PIO_FELLSR = CONF_WINC_IRQ_PIN;
   CONF_WINC_IRQ_PIO->PIO_IER = CONF_WINC_IRQ_PIN;

   //Set priority grouping
   NVIC_SetPriorityGrouping(CONF_WINC_IRQ_PRIORITY_GROUPING);

   //Configure PIO interrupt priority
   NVIC_SetPriority(CONF_WINC_IRQn, NVIC_EncodePriority(CONF_WINC_IRQ_PRIORITY_GROUPING,
      CONF_WINC_IRQ_GROUP_PRIORITY, CONF_WINC_IRQ_SUB_PRIORITY));

   //Enable PIO interrupt
   NVIC_EnableIRQ(CONF_WINC_IRQn);
   NVIC_SetPendingIRQ(CONF_WINC_IRQn);
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
      NVIC_EnableIRQ(CONF_WINC_IRQn);
   }
   else
   {
      //Disable external interrupt
      NVIC_DisableIRQ(CONF_WINC_IRQn);
   }
}


/**
 * @brief IRQ handler
 **/

void CONF_WINC_IRQHandler(void)
{
   bool_t flag;
   volatile uint32_t status;

   //Enter interrupt service routine
   osEnterIsr();

   //This flag will be set if a higher priority task must be woken
   flag = FALSE;

   //Read status register to clear interrupt
   status = CONF_WINC_IRQ_PIO->PIO_ISR;

   //Ensure the IRQ pin is asserted
   if(!(CONF_WINC_IRQ_PIO->PIO_PDSR & CONF_WINC_IRQ_PIN))
   {
      //WILC1000 interrupt processing
      if(gpfIsr != NULL)
         gpfIsr();

      //Call interrupt handler
      flag = winc1500IrqHandler();
   }

   //Leave interrupt service routine
   osExitIsr(flag);
}
