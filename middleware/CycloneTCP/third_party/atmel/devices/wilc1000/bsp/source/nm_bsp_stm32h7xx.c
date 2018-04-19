/**
 * @file nm_bsp_stm32h7xx.c
 * @brief BSP for STM32H7xx microcontrollers
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
#include "stm32h7xx.h"
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
   GPIO_InitTypeDef GPIO_InitStructure;

   //Enable GPIO clocks
   CONF_WILC_RST_GPIO_CLK_ENABLE();
   CONF_WILC_CE_GPIO_CLK_ENABLE();
   CONF_WILC_WAKE_GPIO_CLK_ENABLE();
   CONF_WILC_CS_GPIO_CLK_ENABLE();
   CONF_WILC_IRQ_GPIO_CLK_ENABLE();

   //Enable SYSCFG clock
   __HAL_RCC_SYSCFG_CLK_ENABLE();

   //Configure RST pin
   GPIO_InitStructure.Pin = CONF_WILC_RST_PIN;
   GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStructure.Pull = GPIO_NOPULL;
   GPIO_InitStructure.Speed = CONF_WILC_RST_SPEED;
   HAL_GPIO_Init(CONF_WILC_RST_GPIO, &GPIO_InitStructure);

   HAL_GPIO_WritePin(CONF_WILC_RST_GPIO, CONF_WILC_RST_PIN, GPIO_PIN_RESET);

   //Configure CE pin
   GPIO_InitStructure.Pin = CONF_WILC_CE_PIN;
   GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStructure.Pull = GPIO_NOPULL;
   GPIO_InitStructure.Speed = CONF_WILC_CE_SPEED;
   HAL_GPIO_Init(CONF_WILC_CE_GPIO, &GPIO_InitStructure);

   HAL_GPIO_WritePin(CONF_WILC_CE_GPIO, CONF_WILC_CE_PIN, GPIO_PIN_RESET);

   //Configure WAKE pin
   GPIO_InitStructure.Pin = CONF_WILC_WAKE_PIN;
   GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStructure.Pull = GPIO_NOPULL;
   GPIO_InitStructure.Speed = CONF_WILC_WAKE_SPEED;
   HAL_GPIO_Init(CONF_WILC_WAKE_GPIO, &GPIO_InitStructure);

   HAL_GPIO_WritePin(CONF_WILC_WAKE_GPIO, CONF_WILC_WAKE_PIN, GPIO_PIN_RESET);

   //Configure CS pin
   GPIO_InitStructure.Pin = CONF_WILC_CS_PIN;
   GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStructure.Pull = GPIO_NOPULL;
   GPIO_InitStructure.Speed = CONF_WILC_CS_SPEED;
   HAL_GPIO_Init(CONF_WILC_CS_GPIO, &GPIO_InitStructure);

   HAL_GPIO_WritePin(CONF_WILC_CS_GPIO, CONF_WILC_CS_PIN, GPIO_PIN_SET);

   //Configure IRQ pin
   GPIO_InitStructure.Pin = CONF_WILC_IRQ_PIN;
   GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
   GPIO_InitStructure.Pull = GPIO_PULLUP;
   HAL_GPIO_Init(CONF_WILC_IRQ_GPIO, &GPIO_InitStructure);
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
   HAL_GPIO_WritePin(CONF_WILC_CE_GPIO, CONF_WILC_CE_PIN, GPIO_PIN_RESET);
   HAL_GPIO_WritePin(CONF_WILC_RST_GPIO, CONF_WILC_RST_PIN, GPIO_PIN_RESET);
   nm_bsp_sleep(100);

   //Set CE pin high
   HAL_GPIO_WritePin(CONF_WILC_CE_GPIO, CONF_WILC_CE_PIN, GPIO_PIN_SET);
   nm_bsp_sleep(100);

   //Set RST pin high
   HAL_GPIO_WritePin(CONF_WILC_RST_GPIO, CONF_WILC_RST_PIN, GPIO_PIN_SET);
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

   //Enter interrupt service routine
   osEnterIsr();

   //This flag will be set if a higher priority task must be woken
   flag = FALSE;

   //Check interrupt status
   if(__HAL_GPIO_EXTI_GET_IT(CONF_WILC_IRQ_PIN) != RESET)
   {
      //Clear interrupt flag
      __HAL_GPIO_EXTI_CLEAR_IT(CONF_WILC_IRQ_PIN);
   }

   //WILC1000 interrupt processing
   if(gpfIsr != NULL)
      gpfIsr();

   //Call interrupt handler
   flag = wilc1000IrqHandler();

   //Leave interrupt service routine
   osExitIsr(flag);
}
