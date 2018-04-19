/*-----------------------------------------------------------------------------
 * Name:    ADC.h
 * Purpose: ADC definitions
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

#ifndef __ADC_H
#define __ADC_H

extern void     ADC_Init      (void);
extern void     ADC_UnInit    (void);
extern void     ADC_StartCnv  (void);
extern int32_t  ADC_GetVal    (void);
extern int32_t  ADC_Bits      (void);

#endif /* __ADC_H */
