/*-----------------------------------------------------------------------------
 * Name:    TSC_STMPE811.c
 * Purpose: Touchscreen controller routines for the STMPE811
 * Note(s):
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdint.h> 
#include "TSC.h"
#include "I2C.h"

#define TSC_I2C_ADDR    0x41            /* Touchscreen 7-bit I2C address      */

/* Register addresses */

#define SYS_CTRL1       0x03
#define SYS_CTRL2       0x04
#define INT_CTRL        0x09
#define INT_EN          0x0A
#define INT_STA         0x0B
#define GPIO_ALT_FUNCT  0x17
#define ADC_CTRL1       0x20
#define ADC_CTRL2       0x21
#define TSC_CTRL        0x40
#define TSC_CFG         0x41
#define FIFO_TH         0x4A
#define FIFO_STA        0x4B
#define FIFO_SIZE       0x4C
#define DATA_X          0x4D
#define DATA_Y          0x4F
#define DATA_Z          0x51
#define TSC_FRACTION_Z  0x56
#define TSC_I_DRIVE     0x58
#define TSC_SHIELD      0x59
#define DATA_XYZ        0xD7

/* Prototypes */
static uint32_t TSC_WrReg (uint8_t reg, uint8_t  val);
static uint32_t TSC_RdReg (uint8_t reg, uint8_t *val);


/*-----------------------------------------------------------------------------
 *      TSC_Init:  Initialize touchscreen controller
 *
 *  Parameter:  (none)
 *  Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t TSC_Init (void) {
  int i;

  TSC_WrReg (SYS_CTRL1,       0x02);    /* Reset Touch-screen controller      */
  for (i = 0; i < 180000; i++);         /* Wait a bit                         */
  TSC_WrReg (SYS_CTRL2,       0x0C);    /* Enable TSC and ADC                 */
  TSC_WrReg (INT_EN,          0x07);    /* Enable Touch detect, FIFO          */
  TSC_WrReg (ADC_CTRL1,       0x69);    /* Set sample time , 12-bit mode      */
  for (i = 0; i < 36000; i++);          /* Wait a bit                         */
  TSC_WrReg (ADC_CTRL2,       0x01);    /* ADC frequency 3.25 MHz             */
  TSC_WrReg (TSC_CFG,         0xC2);    /* Detect delay 10us Settle time 500us*/
  TSC_WrReg (FIFO_TH,         0x01);    /* Threshold for FIFO                 */
  TSC_WrReg (FIFO_STA,        0x01);    /* FIFO reset                         */
  TSC_WrReg (FIFO_STA,        0x00);    /* FIFO not reset                     */
  TSC_WrReg (TSC_FRACTION_Z,  0x07);    /* Fraction z                         */
  TSC_WrReg (TSC_I_DRIVE,     0x01);    /* Drive 50 mA typical                */
  TSC_WrReg (GPIO_ALT_FUNCT,  0x00);    /* Pins are used for touchscreen      */
  TSC_WrReg (TSC_CTRL,        0x01);    /* Enable TSC                         */
  TSC_WrReg (INT_STA,         0xFF);    /* Clear interrupt status             */
  return (0);
}

/*-----------------------------------------------------------------------------
 *      TSC_TouchDet: Check if touch of the screen is detected
 *
 * Parameters: (none)
 *
 * Return:     0 if touch not detected, nonzero otherwise
 *----------------------------------------------------------------------------*/
uint32_t TSC_TouchDet (void) {
  uint8_t  val;
  uint32_t rtv = 0;
  
  if (TSC_RdReg (TSC_CTRL, &val) == 0) {
    if (val & (1 << 7)) {
      rtv = 1;
    }
  }
  return (rtv);
}

/*-----------------------------------------------------------------------------
 *      TSC_GetData: Read coordinates from registers and write them into
 *                   TSC_DATA structure.
 *
 * Parameters: tscd - pointer to TSC_DATA structure
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t TSC_GetData (TSC_DATA *tscd) {
  uint8_t  num, xyz[4];
  uint32_t rtv;

  /* If FIFO overflow, read all samples except the last one */
  TSC_RdReg (FIFO_SIZE, &num);
  while (--num) {
    if (I2C_RdData (TSC_I2C_ADDR, DATA_XYZ, xyz, 4)) {
      /* Registers cannot be read */
      return (1);
    }
  }

  /* Retreive last taken sample */
  rtv = I2C_RdData (TSC_I2C_ADDR, DATA_XYZ, xyz, 4);
  tscd->x = (xyz[0] << 4) | ((xyz[1] & 0xF0) >> 4);
  tscd->y = ((xyz[1] & 0x0F) << 8) | xyz[2];
  tscd->z =  xyz[3];

  /* Clear interrupt flags */
  TSC_WrReg (INT_STA, 0x1F);

  return (rtv);
}

/*-----------------------------------------------------------------------------
 *      TSC_Write:  Write a value to the touchscreen controller register
 *
 * Parameters:  reg - register to write
 *              val - value to write
 *
 * Return:      0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t TSC_WrReg (uint8_t reg, uint8_t  val) {
  return I2C_WrData (TSC_I2C_ADDR, reg, &val, 1);
}


/*-----------------------------------------------------------------------------
 *      TSC_Read:  Read a value from the touchscreen controller register
 *
 * Parameters:  reg - register to read
 *              val - variable where value will be stored
 *
 * Return:      0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t TSC_RdReg (uint8_t reg, uint8_t *val) {
  return I2C_RdData (TSC_I2C_ADDR, reg, val, 1);
}


/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
