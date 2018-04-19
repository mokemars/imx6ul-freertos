/*-----------------------------------------------------------------------------
 * Name:    KBD.c
 * Purpose: Low level keyboard functions
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
#include "KBD.h"

#define NUM_KEYS  4                     /* Number of available keys           */

/* Keys for MCBSTM32F200/MCBSTM32F400 */
#define WAKEUP    1
#define TAMPER    2
#define USER      4


/*-----------------------------------------------------------------------------
 *       KBD_Init:  Initialize keyboard/buttons
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void KBD_Init (void) {
  /* Enable clock and init GPIO inputs */
  RCC->AHB1ENR |= (1UL << 0) |
                  (1UL << 2) |
                  (1UL << 6) ;

  GPIOA->MODER &= ~(3UL << 2* 0);
  GPIOC->MODER &= ~(3UL << 2*13);
  GPIOG->MODER &= ~(3UL << 2*15);
}


/*-----------------------------------------------------------------------------
 *       KBD_UnInit:  Uninitialize keyboard/buttons
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void KBD_UnInit (void) {
  GPIOA->MODER &= ~(3UL << 2* 0);
  GPIOC->MODER &= ~(3UL << 2*13);
  GPIOG->MODER &= ~(3UL << 2*15);
}


/*-----------------------------------------------------------------------------
 *       KBD_GetKeys:  Get keyboard state
 *
 * Parameters: (none)
 * Return:      Keys bitmask
 *----------------------------------------------------------------------------*/
uint32_t KBD_GetKeys (void) {
  /* Read board keyboard inputs */
  uint32_t val = 0;

  if ((GPIOA->IDR & (1 << 0)) != 0) {
    /* Wakeup button */
    val |= WAKEUP;
  }
  if ((GPIOC->IDR & (1 << 13)) == 0) {
    /* Tamper button */
    val |= TAMPER;
  }
  if ((GPIOG->IDR & (1 << 15)) == 0) {
    /* User button */
    val |= USER;
  }
  return (val);
}


/*-----------------------------------------------------------------------------
 *       KBD_Num:  Get number of available keys
 *
 * Parameters: (none)
 * Return:      number of keys
 *----------------------------------------------------------------------------*/
uint32_t KBD_Num (void) {
  return (NUM_KEYS);
}

/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
