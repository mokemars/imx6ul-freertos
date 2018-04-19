/*----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: Low level LED functions
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
#include "LED.h"

#define NUM_LEDS  8                     /* Number of LEDs                     */

const uint32_t led_mask[] = { 1UL <<  3,
                              1UL <<  6,
                              1UL <<  7,
                              1UL << 10,
                              1UL <<  6,
                              1UL <<  7,
                              1UL <<  8,
                              1UL <<  2 };

/*-----------------------------------------------------------------------------
 *      LED_Init:  Initialize LEDs
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Init (void) {
  /* Enable clock for GPIOG, GPIOH and GPIOI                                  */
  RCC->AHB1ENR |= (1UL << 6) | (1UL << 7) | (1UL << 8) ;

  /* Configure Port G pins PG6, PG7, PG8:                                     */
  /* Pins in Push-pull output mode, 50 MHz Fast Speed with Pull-up resistors  */
  GPIOG->MODER   &= ~((3UL << 2*6) | (3UL << 2*7) | (3UL << 2*8));
  GPIOG->MODER   |=  ((1UL << 2*6) | (1UL << 2*7) | (1UL << 2*8));
  GPIOG->OTYPER  &= ~((1UL <<   6) | (1UL <<   7) | (1UL <<   8));
  GPIOG->OSPEEDR &= ~((3UL << 2*6) | (3UL << 2*7) | (3UL << 2*8));
  GPIOG->OSPEEDR |=  ((2UL << 2*6) | (2UL << 2*7) | (2UL << 2*8));
  GPIOG->PUPDR   &= ~((3UL << 2*6) | (3UL << 2*7) | (3UL << 2*8));
  GPIOG->PUPDR   |=  ((1UL << 2*6) | (1UL << 2*7) | (1UL << 2*8));

  /* Configure Port H: PH2, PH3, PH6, PH7                                     */
  /* Pins in Push-pull output mode, 50 MHz Fast Speed with Pull-up resistors  */
  GPIOH->MODER   &= ~((3UL << 2*2) | (3UL << 2*3) | (3UL << 2*6) | (3UL << 2*7));
  GPIOH->MODER   |=  ((1UL << 2*2) | (1UL << 2*3) | (1UL << 2*6) | (1UL << 2*7));
  GPIOH->OTYPER  &= ~((1UL <<   2) | (1UL <<   3) | (1UL <<   6) | (1UL <<   7));
  GPIOH->OSPEEDR &= ~((3UL << 2*2) | (3UL << 2*3) | (3UL << 2*6) | (3UL << 2*7));
  GPIOH->OSPEEDR |=  ((2UL << 2*2) | (2UL << 2*3) | (2UL << 2*6) | (2UL << 2*7));
  GPIOH->PUPDR   &= ~((3UL << 2*2) | (3UL << 2*3) | (3UL << 2*6) | (3UL << 2*7));
  GPIOH->PUPDR   |=  ((1UL << 2*2) | (1UL << 2*3) | (1UL << 2*6) | (1UL << 2*7));

  /* Configure Port I pin PI10:                                               */
  /* Pins in Push-pull output mode, 50 MHz Fast Speed with Pull-up resistors  */
  GPIOI->MODER   &= ~(3UL << 2*10);
  GPIOI->MODER   |=  (1UL << 2*10);
  GPIOI->OTYPER  &= ~(1UL <<   10);
  GPIOI->OSPEEDR &= ~(3UL << 2*10);
  GPIOI->OSPEEDR |=  (2UL << 2*10);
  GPIOI->PUPDR   &= ~(3UL << 2*10);
  GPIOI->PUPDR   |=  (1UL << 2*10);
}


/*-----------------------------------------------------------------------------
 *      LED_UnInit:  Uninitialize LEDs; pins are set to reset state
 *
 * Parameters: (none)
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_UnInit(void) {
  /* Configure Port G pins PG6, PG7, PG8:                                     */
  GPIOG->MODER   &= ~((3UL << 2*6) | (3UL << 2*7) | (3UL << 2*8));
  GPIOG->OTYPER  &= ~((1UL <<   6) | (1UL <<   7) | (1UL <<   8));
  GPIOG->OSPEEDR &= ~((3UL << 2*6) | (3UL << 2*7) | (3UL << 2*8));
  GPIOG->PUPDR   &= ~((3UL << 2*6) | (3UL << 2*7) | (3UL << 2*8));

  /* Reset Port H: PH2, PH3, PH6, PH7                                         */
  GPIOH->MODER   &= ~((3UL << 2*2) | (3UL << 2*3) | (3UL << 2*6) | (3UL << 2*7));
  GPIOH->OTYPER  &= ~((1UL <<   2) | (1UL <<   3) | (1UL <<   6) | (1UL <<   7));
  GPIOH->OSPEEDR &= ~((3UL << 2*2) | (3UL << 2*3) | (3UL << 2*6) | (3UL << 2*7));
  GPIOH->PUPDR   &= ~((3UL << 2*2) | (3UL << 2*3) | (3UL << 2*6) | (3UL << 2*7));

  /* Reset Port I: PI10                                                       */
  GPIOI->MODER   &= ~(3UL << 2*10);
  GPIOI->OTYPER  &= ~(1UL <<   10);
  GPIOI->OSPEEDR &= ~(3UL << 2*10);
  GPIOI->PUPDR   &= ~(3UL << 2*10);
}

/*-----------------------------------------------------------------------------
 *      LED_On: Turns on requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num) {

  if (num < 3) {
    GPIOH->BSRRL = led_mask[num];
  }
  else if (num < 4) {
    GPIOI->BSRRL = led_mask[num];
  }
  else if (num < 7){
    GPIOG->BSRRL = led_mask[num];
  }
  else {
    GPIOH->BSRRL = led_mask[num];
  }
}

/*-----------------------------------------------------------------------------
 *       LED_Off: Turns off requested LED
 *
 * Parameters:  num - LED number
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Off (uint32_t num) {

  if (num < 3) {
    GPIOH->BSRRH = led_mask[num];
  }
  else if (num < 4) {
    GPIOI->BSRRH = led_mask[num];
  }
  else if (num < 7){
    GPIOG->BSRRH = led_mask[num];
  }
  else {
    GPIOH->BSRRH = led_mask[num];
  }
}

/*-----------------------------------------------------------------------------
 *       LED_Val: Write value to LEDs
 *
 * Parameters:  val - value to be displayed on LEDs
 * Return:     (none)
 *----------------------------------------------------------------------------*/
void LED_Val (uint32_t val) {
  int i;

  for (i = 0; i < NUM_LEDS; i++) {
    if (val & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}

/*-----------------------------------------------------------------------------
 *       LED_Num: Get number of available LEDs
 *
 * Parameters: (none)
 * Return:      number of available LEDs
 *----------------------------------------------------------------------------*/
uint32_t LED_Num (void) {
  return (NUM_LEDS);
}

/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
