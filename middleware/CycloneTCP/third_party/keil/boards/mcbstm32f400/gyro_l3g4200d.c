/*----------------------------------------------------------------------------
 * Name:    Gyro_L3G4200D.c
 * Purpose: Low level L3G4200D gyroscope functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f4xx.h>                  /* STM32F4xx Definitions              */
#include "Gyro.h"
#include "I2C.h"

#define GYRO_I2C_ADDR   0x68            /* Gyroscope 7-bit I2C address        */
#define GYRO_ID         0xD3            /* Gyroscope ID (Who am I value)      */

#define GYRO_I2C_ADDR_2 0x6A            /* Gyroscope 7-bit I2C address 2      */
#define GYRO_ID_2       0xD4            /* This driver also works with L3GD20 */

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
static uint32_t GYRO_WrReg (uint8_t reg, uint8_t  val);
static uint32_t GYRO_RdReg (uint8_t reg, uint8_t *val);

static uint8_t DeviceAddr;


/*-----------------------------------------------------------------------------
 *      GYRO_Init:  Initialize gyroscope
 *
 * Parameters: (none)
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t GYRO_Init (void) {
  uint8_t who = 0;

  /* Init L3G4200D device */
  DeviceAddr = GYRO_I2C_ADDR;
  if (GYRO_RdReg (WHO_AM_I, &who) == 0) {
    if (who != GYRO_ID) {
      return (1);                       /* Wrong ID                           */
    }
  }
  else {
    /* Check if this is L3GD20 device (L3G4200D compatible) */
    DeviceAddr = GYRO_I2C_ADDR_2;
    if (GYRO_RdReg (WHO_AM_I, &who) == 0) {
      if (who != GYRO_ID_2) {
        return (1);                     /* Wrong ID                           */
      }
    }
  }

  GYRO_WrReg (CTRL_REG1, 0x0F);         /* ODR: 100Hz, Cut-Off: 12.5,         */
  GYRO_WrReg (CTRL_REG4, 0x00);         /* Continous update,full scale 250dps */
  GYRO_WrReg (CTRL_REG5, 0x40);         /* FIFO Enable                        */
  return (0);
}

/*-----------------------------------------------------------------------------
 *      GYRO_GetData: Read angular rate data from registers and write them
 *                    into ANGLE_RATE structure. Values are expressed in
 *                    two's complement.
 *
 * Parameters: ang - pointer to ANGLE_RATE structure
 *
 * Return:     0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t GYRO_GetData (ANGLE_RATE *ang) {
  uint32_t rtv;
  uint8_t valL, valH;
  
  rtv  = GYRO_RdReg (OUT_X_L, &valL);
  rtv |= GYRO_RdReg (OUT_X_H, &valH);
  ang->x = (int16_t)((valH << 8) | valL);
  
  rtv |= GYRO_RdReg (OUT_Y_L, &valL);
  rtv |= GYRO_RdReg (OUT_Y_H, &valH);
  ang->y = (int16_t)((valH << 8) | valL);
  
  rtv |= GYRO_RdReg (OUT_Z_L, &valL);
  rtv |= GYRO_RdReg (OUT_Z_H, &valH);
  
  ang->z = (int16_t)((valH << 8) | valL);
  return (rtv);
}


/*-----------------------------------------------------------------------------
 *      GYRO_WrReg:  Write a value to gyroscope register
 *
 * Parameters:  reg - register to write
 *              val - value to write
 *
 * Return:      0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
static uint32_t GYRO_WrReg (uint8_t reg, uint8_t val) {
  return I2C_WrData (DeviceAddr, reg, &val, 1);
}


/*-----------------------------------------------------------------------------
 *      GYRO_RdReg:  Read a value from accelerator register
 *
 * Parameters:  reg - register to read
 *              val - variable where value will be stored
 *
 * Return:      0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
static uint32_t GYRO_RdReg (uint8_t reg, uint8_t *val) {
  return I2C_RdData (DeviceAddr, reg, val, 1);
}


/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
