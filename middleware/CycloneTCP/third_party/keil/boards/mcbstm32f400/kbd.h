/*-----------------------------------------------------------------------------
 * Name:    KBD.h
 * Purpose: Keyboard definitions
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

#ifndef __KBD_H
#define __KBD_H

/* Keyboard Definitions */
extern void     KBD_Init    (void);
extern void     KBD_UnInit  (void);
extern uint32_t KBD_GetKeys (void);
extern uint32_t KBD_Num     (void);

#endif /* __KBD_H */
