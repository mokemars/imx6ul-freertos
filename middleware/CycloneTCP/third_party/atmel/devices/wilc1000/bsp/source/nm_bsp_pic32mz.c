/**
 * @file nm_bsp_pic32mz.c
 * @brief BSP for PIC32MZ microcontrollers
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
#include <p32xxxx.h>
#include "bsp/include/nm_bsp.h"
#include "common/include/nm_common.h"
#include "core/net.h"
#include "drivers/wifi/wilc1000_driver.h"

//IRQ handler
static tpfNmBspIsr gpfIsr;

//External interrupt instance
#if (CONF_WILC_INT_INSTANCE == 0)
   #define _INTCON_INTxEP_MASK   _INTCON_INT0EP_MASK
   #define _IFS0_INTxIF_MASK     _IFS0_INT0IF_MASK
   #define _IEC0_INTxIE_MASK     _IEC0_INT0IE_MASK
   #define IPCxCLR               IPC0CLR
   #define IPCxSET               IPC0SET
   #define _IPCx_INTxIP_MASK     _IPC0_INT0IP_MASK
   #define _IPCx_INTxIP_POSITION _IPC0_INT0IP_POSITION
   #define _IPCx_INTxIS_MASK     _IPC0_INT0IS_MASK
   #define _IPCx_INTxIS_POSITION _IPC0_INT0IS_POSITION
#elif (CONF_WILC_INT_INSTANCE == 1)
   #define _INTCON_INTxEP_MASK   _INTCON_INT1EP_MASK
   #define _IFS0_INTxIF_MASK     _IFS0_INT1IF_MASK
   #define _IEC0_INTxIE_MASK     _IEC0_INT1IE_MASK
   #define IPCxCLR               IPC2CLR
   #define IPCxSET               IPC2SET
   #define _IPCx_INTxIP_MASK     _IPC2_INT1IP_MASK
   #define _IPCx_INTxIP_POSITION _IPC2_INT1IP_POSITION
   #define _IPCx_INTxIS_MASK     _IPC2_INT1IS_MASK
   #define _IPCx_INTxIS_POSITION _IPC2_INT1IS_POSITION
#elif (CONF_WILC_INT_INSTANCE == 2)
   #define _INTCON_INTxEP_MASK   _INTCON_INT2EP_MASK
   #define _IFS0_INTxIF_MASK     _IFS0_INT2IF_MASK
   #define _IEC0_INTxIE_MASK     _IEC0_INT2IE_MASK
   #define IPCxCLR               IPC3CLR
   #define IPCxSET               IPC3SET
   #define _IPCx_INTxIP_MASK     _IPC3_INT2IP_MASK
   #define _IPCx_INTxIP_POSITION _IPC3_INT2IP_POSITION
   #define _IPCx_INTxIS_MASK     _IPC3_INT2IS_MASK
   #define _IPCx_INTxIS_POSITION _IPC3_INT2IS_POSITION
#elif (CONF_WILC_INT_INSTANCE == 3)
   #define _INTCON_INTxEP_MASK   _INTCON_INT3EP_MASK
   #define _IFS0_INTxIF_MASK     _IFS0_INT3IF_MASK
   #define _IEC0_INTxIE_MASK     _IEC0_INT3IE_MASK
   #define IPCxCLR               IPC4CLR
   #define IPCxSET               IPC4SET
   #define _IPCx_INTxIP_MASK     _IPC4_INT3IP_MASK
   #define _IPCx_INTxIP_POSITION _IPC4_INT3IP_POSITION
   #define _IPCx_INTxIS_MASK     _IPC4_INT3IS_MASK
   #define _IPCx_INTxIS_POSITION _IPC4_INT3IS_POSITION
#elif (CONF_WILC_INT_INSTANCE == 4)
   #define _INTCON_INTxEP_MASK   _INTCON_INT4EP_MASK
   #define _IFS0_INTxIF_MASK     _IFS0_INT4IF_MASK
   #define _IEC0_INTxIE_MASK     _IEC0_INT4IE_MASK
   #define IPCxCLR               IPC5CLR
   #define IPCxSET               IPC5SET
   #define _IPCx_INTxIP_MASK     _IPC5_INT4IP_MASK
   #define _IPCx_INTxIP_POSITION _IPC5_INT4IP_POSITION
   #define _IPCx_INTxIS_MASK     _IPC5_INT4IS_MASK
   #define _IPCx_INTxIS_POSITION _IPC5_INT4IS_POSITION
#endif


/**
 * @brief GPIO initialization
 **/

static void init_chip_pins(void)
{
   //Configure RST pin
   CONF_WILC_RST_INIT();
   CONF_WILC_RST_CLR();

   //Configure WAKE pin
   CONF_WILC_WAKE_INIT();
   CONF_WILC_WAKE_CLR();

   //Configure CE pin
   CONF_WILC_CE_INIT();
   CONF_WILC_CE_CLR();

   //Configure CS pin
   CONF_WILC_CS_INIT();
   CONF_WILC_CS_SET();

   //Configure IRQ pin
   CONF_WILC_IRQ_INIT();
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
   CONF_WILC_CE_CLR();
   CONF_WILC_RST_CLR();
   nm_bsp_sleep(100);

   //Set CE pin high
   CONF_WILC_CE_SET();
   nm_bsp_sleep(100);

   //Set RST pin high
   CONF_WILC_RST_SET();
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

   //Configure edge polarity (falling edge)
   INTCONCLR = _INTCON_INTxEP_MASK;

   //Set interrupt priority
   IPCxCLR = _IPCx_INTxIP_MASK;
   IPCxSET = (CONF_WILC_IRQ_PRIORITY << _IPCx_INTxIP_POSITION);

   //Set interrupt subpriority
   IPCxCLR = _IPCx_INTxIS_MASK;
   IPCxSET = (CONF_WILC_IRQ_SUB_PRIORITY << _IPCx_INTxIS_POSITION);

   //Enable interrupt
   IEC0SET = _IEC0_INTxIE_MASK;
   //Set interrupt flag
   IFS0SET = _IFS0_INTxIF_MASK;
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
      IEC0SET = _IEC0_INTxIE_MASK;
   }
   else
   {
      //Disable external interrupt
      IEC0CLR = _IEC0_INTxIE_MASK;
   }
}


/**
 * @brief IRQ handler
 **/

void CONF_WILC_IRQ_HANDLER(void)
{
   bool_t flag;

   //Enter interrupt service routine
   osEnterIsr();

   //This flag will be set if a higher priority task must be woken
   flag = FALSE;

   //Check interrupt status
   if(IFS0 & _IFS0_INTxIF_MASK)
   {
      //Clear interrupt flag
      IFS0CLR = _IFS0_INTxIF_MASK;

      //WILC1000 special processing
      if(gpfIsr != NULL)
         gpfIsr();

      //Call interrupt handler
      flag = wilc1000IrqHandler();
   }

   //Leave interrupt service routine
   osExitIsr(flag);
}
