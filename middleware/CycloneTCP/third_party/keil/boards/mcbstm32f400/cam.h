/*-----------------------------------------------------------------------------
 * Name:    CAM.h
 * Purpose: Digital camera driver definitions
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

#ifndef __CAM_H
#define __CAM_H

#define TEST_PAT_OFF 0                  /* Test pattern OFF                   */
#define TEST_PAT_BAR 1                  /* Color bar test pattern             */

extern void     CAM_Init  (void);
extern uint32_t CAM_WrReg (uint8_t reg, uint8_t byte);
extern uint32_t CAM_RdReg (uint8_t reg);

extern uint32_t CAM_On    (void);
extern uint32_t CAM_Off   (void);

extern uint32_t CAM_Test  (uint32_t pattern);

#endif /* __CAM_H */
