/**
 * @file nm_bsp_sama5d2.c
 * @brief BSP for SAMA5D2 microcontrollers
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
#include "chip.h"
#include "peripherals/aic.h"
#include "peripherals/pio.h"
#include "peripherals/pmc.h"
#include "peripherals/matrix.h"
#include "bsp/include/nm_bsp.h"
#include "common/include/nm_common.h"
#include "core/net.h"
#include "drivers/wifi/wilc1000_driver.h"

//IRQ handler
static tpfNmBspIsr gpfIsr;

//Forward declaration of functions
void nm_bsp_irq_handler(void);


/**
 * @brief GPIO initialization
 **/

static void init_chip_pins(void)
{
   const struct _pin wilcRstPin = CONF_WILC_RST_PIN;
   const struct _pin wilcCePin = CONF_WILC_CE_PIN;
   const struct _pin wilcWakePin = CONF_WILC_WAKE_PIN;
#ifdef CONF_WILC_USE_SPI
   const struct _pin wilcCsPin = CONF_WILC_CS_PIN;
#endif
   const struct _pin wilcIrqPin = CONF_WILC_IRQ_PIN;

   //Configure RST pin
   pio_configure(&wilcRstPin, 1);
   pio_clear(&wilcRstPin);

   //Configure CE pin
   pio_configure(&wilcCePin, 1);
   pio_clear(&wilcCePin);

   //Configure WAKE pin
   pio_configure(&wilcWakePin, 1);
   pio_clear(&wilcWakePin);

#ifdef CONF_WILC_USE_SPI
   //Configure CS pin
   pio_configure(&wilcCsPin, 1);
   pio_set(&wilcCsPin);
#endif

   //Configure IRQ pin
   pio_configure(&wilcIrqPin, 1);
   pio_configure_it(&wilcIrqPin);
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
   const struct _pin wilcRstPin = CONF_WILC_RST_PIN;
   const struct _pin wilcCePin = CONF_WILC_CE_PIN;

   //Debug message
   M2M_PRINT("nm_bsp_reset\r\n");

   //Set CE and RST pins low
   pio_clear(&wilcCePin);
   pio_clear(&wilcRstPin);
   nm_bsp_sleep(100);

   //Set CE pin high
   pio_set(&wilcCePin);
   nm_bsp_sleep(100);

   //Set RST pin high
   pio_set(&wilcRstPin);
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
   const struct _pin wilcIrqPin = CONF_WILC_IRQ_PIN;

   //Save ISR handler
   gpfIsr = pfIsr;

   //Enable PIO interrupt
   pio_enable_it(&wilcIrqPin);

   //Register interrupt handler
   aic_set_source_vector(CONF_WILC_IRQ_ID_PIO, nm_bsp_irq_handler);

   //Configure interrupt priority
   aic_configure(CONF_WILC_IRQ_ID_PIO, AIC_SMR_SRCTYPE_INT_LEVEL_SENSITIVE |
      AIC_SMR_PRIOR(CONF_WILC_IRQ_PRIORITY));

   //Enable external interrupt
   aic_enable(CONF_WILC_IRQ_ID_PIO);
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
      aic_enable(CONF_WILC_IRQ_ID_PIO);
   }
   else
   {
      //Disable external interrupt
      aic_disable(CONF_WILC_IRQ_ID_PIO);
   }
}


/**
 * @brief IRQ handler
 **/

void nm_bsp_irq_handler(void)
{
   bool_t flag;
   volatile uint32_t status;
   const struct _pin wilcIrqPin = CONF_WILC_IRQ_PIN;

   //Enter interrupt service routine
   osEnterIsr();

   //This flag will be set if a higher priority task must be woken
   flag = FALSE;

   //Read status register to clear interrupt
   if(matrix_is_peripheral_secured(MATRIX1, CONF_WILC_IRQ_ID_PIO))
      status = PIOA->PIO_PIO_[wilcIrqPin.group].S_PIO_ISR;
   else
      status = PIOA->PIO_IO_GROUP[wilcIrqPin.group].PIO_ISR;

   //Ensure the IRQ pin is asserted
   if(!pio_get(&wilcIrqPin))
   {
      //WILC1000 interrupt processing
      if(gpfIsr != NULL)
         gpfIsr();

      //Call interrupt handler
      flag = wilc1000IrqHandler();
   }

   //Write AIC_EOICR register before exiting
   AIC->AIC_EOICR = 0;

   //Leave interrupt service routine
   osExitIsr(flag);
}
