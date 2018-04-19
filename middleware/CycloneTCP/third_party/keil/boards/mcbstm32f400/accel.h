/*----------------------------------------------------------------------------
 * Name:    Accel.h
 * Purpose: Low level accelerator driver definitions
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef __ACCEL_H
#define __ACCEL_H

typedef struct {
  int16_t x;
  int16_t y;
  int16_t z;
} AXIS_DATA;

extern uint32_t ACC_Init    (void);
extern uint32_t ACC_GetData (AXIS_DATA *axd);

#endif /* __ACCEL_H */
