/*----------------------------------------------------------------------------
 * Name:    Gyro.h
 * Purpose: Low level gyroscope driver definitions
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

#ifndef __GYRO_H
#define __GYRO_H

typedef struct {
  int16_t x;
  int16_t y;
  int16_t z;
} ANGLE_RATE;

extern uint32_t GYRO_Init    (void);
extern uint32_t GYRO_GetData (ANGLE_RATE *ang);

#endif /* __ACCEL_H */
