/*----------------------------------------------------------------------------
 * Name:    Accel_LIS3DH.c
 * Purpose: Low level LIS3DH accelerator functions
 * Note(s):
 *----------------------------------------------------------------------------
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
#include "Accel.h"
#include "I2C.h"

#define ACC_I2C_ADDR    0x18            /* Accelerometer 7-bit I2C address    */
#define ACC_ID          0x33            /* Accelerometer ID (Who am I value)  */

/* Register addresses */
#define WHO_AM_I        0x0F
#define CTRL_REG1       0x20
#define CTRL_REG4       0x23
#define CTRL_REG5       0x24
#define OUT_X_L         0x28
#define OUT_X_H         0x29
#define OUT_Y_L         0x2A
#define OUT_Y_H         0x2B
#define OUT_Z_L         0x2C
#define OUT_Z_H         0x2D

/* Prototypes */
static uint32_t ACC_WrReg (uint8_t reg, uint8_t  val);
static uint32_t ACC_RdReg (uint8_t reg, uint8_t *val);


/*-----------------------------------------------------------------------------
 *      ACC_Init:  Initialize accelerator
 *
 * Parameters: (none)
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t ACC_Init (void) {
  uint8_t who = 0;
  
  ACC_RdReg (WHO_AM_I, &who);
  if (who != ACC_ID) {
    return (1);                         /* Wrong ID                           */
  }
  ACC_WrReg (CTRL_REG1, 0x77); 
  ACC_WrReg (CTRL_REG4, 0x00);
  ACC_WrReg (CTRL_REG5, 0x40);          /* Normal mode, FIFO Enable           */
  return (0);
}

/*-----------------------------------------------------------------------------
 *      ACC_GetData:  Read acceleration data from registers and write them
 *                    into AXIS_DATA structure. Values are expressed in
 *                    two's complement.
 *
 * Parameters: axd - pointer to AXIS_DATA structure
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t ACC_GetData (AXIS_DATA *axd) {
  uint32_t rtv = 0;
  uint8_t valL, valH;
  
  rtv |= ACC_RdReg (OUT_X_L, &valL);
  rtv |= ACC_RdReg (OUT_X_H, &valH);
  axd->x = (int16_t)((valH << 8) | valL);
  
  rtv |= ACC_RdReg (OUT_Y_L, &valL);
  rtv |= ACC_RdReg (OUT_Y_H, &valH);
  axd->y = (int16_t)((valH << 8) | valL);
  
  rtv |= ACC_RdReg (OUT_Z_L, &valL);
  rtv |= ACC_RdReg (OUT_Z_H, &valH);
  
  axd->z = (int16_t)((valH << 8) | valL);
  return (rtv);
}


/*-----------------------------------------------------------------------------
 *      ACC_WrReg:  Write a value to accelerator register
 *
 * Parameters:  reg - register to write
 *              val - value to write
 *
 * Return:      0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
static uint32_t ACC_WrReg (uint8_t reg, uint8_t val) {
  return I2C_WrData (ACC_I2C_ADDR, reg, &val, 1);
}


/*-----------------------------------------------------------------------------
 *      ACC_RdReg:  Read a value from accelerator register
 *
 * Parameters:  reg - register to read
 *              val - variable where value will be stored
 *
 * Return:      0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
static uint32_t ACC_RdReg (uint8_t reg, uint8_t *val) {
  return I2C_RdData (ACC_I2C_ADDR, reg, val, 1);
}


/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
