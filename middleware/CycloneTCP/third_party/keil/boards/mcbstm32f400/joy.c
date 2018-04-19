/*-----------------------------------------------------------------------------
 * Name:    JOY.c
 * Purpose: Low level joystick functions
 * Note(s):
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f4xx.h>                  /* STM32F4xx Definitions              */
#include "JOY.h"
#include "I2C.h"

#define IOE_I2C_ADDR 0x44               /* IO expander I2C write address      */

/* Prototypes */
static uint32_t  IOE_Read  (uint8_t reg, uint8_t *val);
static uint32_t  IOE_Write (uint8_t reg, uint8_t  val);


/*-----------------------------------------------------------------------------
 *       JOY_Init:  Initialize joystick
 *                  Joystick is connected to the STMPE811 touchscreen controler,
 *                  which is used as a GPIO expander.
 *----------------------------------------------------------------------------*/
void JOY_Init (void) {
  int i;

  IOE_Write (0x03, 0x02);               /* Reset Touch-screen controller      */
  for (i = 0; i < 180000; i++);         /* Wait minimum of 10ms               */
  IOE_Write (0x04, 0x03);               /* Enable only GPIO and temp. sensor  */
  IOE_Write (0x09, 0x03);               /* INT is active low edge interrupt   */
  IOE_Write (0x0A, 0xA0);               /* GPIO and temp. sens. int enabled   */
  IOE_Write (0x0C, 0xFF);               /* All GPIOs are generating interrupt */
  IOE_Write (0x13, 0x00);               /* GPIOs direction to input state     */
  IOE_Write (0x16, 0xFF);               /* Detect falling edge on all GPIOs   */
  IOE_Write (0x17, 0xFF);               /* Configure all pins as GPIO         */
}


/*-----------------------------------------------------------------------------
 *       JOY_Init:  Uninitialize joystick
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void JOY_UnInit (void) {
  IOE_Write (0x03, 0x02);               /* Reset Touch-screen controller      */
}


/*-----------------------------------------------------------------------------
 *       JOY_GetKeys:  Get joystick direction keys state
 *
 * Parameters: (none)
 * Return:     joystick directions bitmask
 *----------------------------------------------------------------------------*/
uint32_t JOY_GetKeys (void) {
  /* Read board keyboard inputs */
  uint8_t reg;
  uint32_t val = 0;

  if (IOE_Read (0x12, &reg) == 0) {     /* Read monitor pin state register    */
    reg = ~reg;

    if (reg & (1 << 3)) {
      val |= JOY_UP;
    }
    if (reg & (1 << 4)) {
      val |= JOY_RIGHT;
    }
    if (reg & (1 << 5)) {
      val |= JOY_LEFT;
    }
    if (reg & (1 << 6)) {
      val |= JOY_DOWN;
    }
    if (reg & (1 << 7)) {
      val |= JOY_CENTER;
    }
  }
  return (val);
}


/*-----------------------------------------------------------------------------
 *      IOE_Write:  Write value to the STMPE811 register
 *
 *  Parameter:  reg - register address
 *              val - value to be written
 *
 *  Return:   0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
static uint32_t IOE_Write (uint8_t reg, uint8_t val) {  
  return (I2C_WrData (IOE_I2C_ADDR, reg, &val, 1));
}


/*-----------------------------------------------------------------------------
 *      IOE_Read:  Read data from the STMPE811 register
 *
 *  Parameter:  reg - register address
 *              val - variable where value will be stored
 *
 *  Return: 0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
static uint32_t IOE_Read (uint8_t reg, uint8_t *val) {
  return (I2C_RdData (IOE_I2C_ADDR, reg, val, 1));
}

/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
