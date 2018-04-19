/*-----------------------------------------------------------------------------
 * Name:    JOY.h
 * Purpose: Joystick definitions
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

#ifndef __JOY_H
#define __JOY_H

/* Joystick direction bitmasks */
#define JOY_LEFT    (1 << 0)
#define JOY_RIGHT   (1 << 1)
#define JOY_CENTER  (1 << 2)
#define JOY_UP      (1 << 3)
#define JOY_DOWN    (1 << 4)

/* Joystick Definitions */
extern void     JOY_Init    (void);
extern void     JOY_UnInit  (void);
extern uint32_t JOY_GetKeys (void);

#endif /* __JOY_H */
