/*-----------------------------------------------------------------------------
 * Name:    CAM.c
 * Purpose: Digital camera driver
 * Note(s):
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f4xx.h>                  /* STM32F4xx Definitions              */
#include "I2C.h"
#include "CAM.h"

/* I2C Camera address */
#define CAM_I2C_ADDR    0x21            /* 7-bit accelerator I2C address      */

/* DMA Source and destination addresses */
#define DCMI_SRC_ADDR   0x50050028
#define LCD_DST_ADDR    0x6C000002

/* Local bitmasks and settings definitions */
#define PH_MODER_MASK  (GPIO_MODER_MODER8       | GPIO_MODER_MODER9       | GPIO_MODER_MODER10      | \
                        GPIO_MODER_MODER11      | GPIO_MODER_MODER12      | GPIO_MODER_MODER14      )
#define PH_MODER_SET   (GPIO_MODER_MODER8_1     | GPIO_MODER_MODER9_1     | GPIO_MODER_MODER10_1    | \
                        GPIO_MODER_MODER11_1    | GPIO_MODER_MODER12_1    | GPIO_MODER_MODER14_1    )
#define PH_OTYPER_MASK (GPIO_OTYPER_OT_8        | GPIO_OTYPER_OT_9        | GPIO_OTYPER_OT_10       | \
                        GPIO_OTYPER_OT_11       | GPIO_OTYPER_OT_12       | GPIO_OTYPER_OT_14       )
#define PH_OSPEEDR_SET (GPIO_OSPEEDER_OSPEEDR8  | GPIO_OSPEEDER_OSPEEDR9  | GPIO_OSPEEDER_OSPEEDR10 | \
                        GPIO_OSPEEDER_OSPEEDR11 | GPIO_OSPEEDER_OSPEEDR12 | GPIO_OSPEEDER_OSPEEDR14 )
#define PH_PUPDR_MASK  (GPIO_PUPDR_PUPDR8       | GPIO_PUPDR_PUPDR9       | GPIO_PUPDR_PUPDR10      | \
                        GPIO_PUPDR_PUPDR11      | GPIO_PUPDR_PUPDR12      | GPIO_PUPDR_PUPDR14      )
#define PH_PUPDR_SET   (GPIO_PUPDR_PUPDR8_0     | GPIO_PUPDR_PUPDR9_0     | GPIO_PUPDR_PUPDR10_0    | \
                        GPIO_PUPDR_PUPDR11_0    | GPIO_PUPDR_PUPDR12_0    | GPIO_PUPDR_PUPDR14_0    )

#define PI_MODER_MASK  (GPIO_MODER_MODER4       | GPIO_MODER_MODER5       | GPIO_MODER_MODER6       | GPIO_MODER_MODER7      )
#define PI_MODER_SET   (GPIO_MODER_MODER4_1     | GPIO_MODER_MODER5_1     | GPIO_MODER_MODER6_1     | GPIO_MODER_MODER7_1    )
#define PI_OTYPER_MASK (GPIO_OTYPER_OT_4        | GPIO_OTYPER_OT_5        | GPIO_OTYPER_OT_6        | GPIO_OTYPER_OT_7       )
#define PI_OSPEEDR_SET (GPIO_OSPEEDER_OSPEEDR4  | GPIO_OSPEEDER_OSPEEDR5  | GPIO_OSPEEDER_OSPEEDR6  | GPIO_OSPEEDER_OSPEEDR7 )
#define PI_PUPDR_MASK  (GPIO_PUPDR_PUPDR4       | GPIO_PUPDR_PUPDR5       | GPIO_PUPDR_PUPDR6       | GPIO_PUPDR_PUPDR7      )
#define PI_PUPDR_SET   (GPIO_PUPDR_PUPDR4_0     | GPIO_PUPDR_PUPDR5_0     | GPIO_PUPDR_PUPDR6_0     | GPIO_PUPDR_PUPDR7_0    )

/* Calculate array size */
#define ARR_SZ(x) (sizeof (x) / sizeof(x[0]))

/* Register value */
typedef struct {
  uint8_t Addr;
  uint8_t Val;
} REG_VAL;


/* OmniVision recommended settings based on OVM7690 Setting V2.2              */
/* Modified for RGB QVGA settings                                             */
REG_VAL CAM_RegInit[] = {
  {0x0E, 0x00},                         /* Normal operation mode              */
  {0x0C, 0x46},
  {0x81, 0xFF},
  {0x21, 0x44},
  {0x16, 0x03},
  {0x39, 0x80},
  {0x1E, 0xB1},
  
  /* Format */
  {0x12, 0x06},                         /* Output format control: RGB         */
  {0x82, 0x03},
  {0xD0, 0x48},
  {0x80, 0x7F},
  {0x3E, 0x30},
  {0x22, 0x00},
  
  /* Resolution */
  {0x17, 0x69},                         /* Horizontal window start point      */
  {0x18, 0xA4},                         /* Horizontal senzor size             */
  {0x19, 0x0C},                         /* Vertical Window start line         */
  {0x1A, 0xF6},                         /* Vertical sensor size               */
  
  {0xC8, 0x02},
  {0xC9, 0x80},
  {0xCA, 0x01},
  {0xCB, 0xE0},  
  {0xCC, 0x02},
  {0xCD, 0x80},
  {0xCE, 0x01},
  {0xCF, 0xE0},
  
  /* Lens Correction */
  {0x85, 0x90},
  {0x86, 0x00},
  {0x87, 0x00},
  {0x88, 0x10},
  {0x89, 0x30},
  {0x8A, 0x29},
  {0x8B, 0x26},
  
  /* Color Matrix */
  {0xBB, 0x80},
  {0xBC, 0x62},
  {0xBD, 0x1E},
  {0xBE, 0x26},
  {0xBF, 0x7B},
  {0xC0, 0xAC},
  {0xC1, 0x1E},
  
  /* Edge + Denoise */
  {0xB7, 0x05},
  {0xB8, 0x09},
  {0xB9, 0x00},
  {0xBA, 0x18},
  
  /* UVAdjust */
  {0x5A, 0x4A},
  {0x5B, 0x9F},
  {0x5C, 0x48},
  {0x5D, 0x32},
  
  /* AEC/AGC target */
  {0x24, 0x78},
  {0x25, 0x68},
  {0x26, 0xB3},
  
  /* Gamma */
  {0xA3, 0x0B},
  {0xA4, 0x15},
  {0xA5, 0x2A},
  {0xA6, 0x51},
  {0xA7, 0x63},
  {0xA8, 0x74},
  {0xA9, 0x83},
  {0xAA, 0x91},
  {0xAB, 0x9E},
  {0xAC, 0xAA},
  {0xAD, 0xBE},
  {0xAE, 0xCE},
  {0xAF, 0xE5},
  {0xB0, 0xF3},
  {0xB1, 0xFB},
  {0xB2, 0x06},
  
  /* Advance */
  {0x8C, 0x5D},
  {0x8D, 0x11},
  {0x8E, 0x12},
  {0x8F, 0x11},
  {0x90, 0x50},
  {0x91, 0x22},
  {0x92, 0xD1},
  {0x93, 0xA7},
  {0x94, 0x23},
  {0x95, 0x3B},
  {0x96, 0xFF},
  {0x97, 0x00},
  {0x98, 0x4A},
  {0x99, 0x46},
  {0x9A, 0x3D},
  {0x9B, 0x3A},
  {0x9C, 0xF0},
  {0x9D, 0xF0},
  {0x9E, 0xF0},
  {0x9F, 0xFF},
  {0xA0, 0x56},
  {0xA1, 0x55},
  {0xA2, 0x13},
  
  /* General Control */
  {0x50, 0x9A},
  {0x51, 0x80},
  {0x21, 0x23},
  
  {0x14, 0x29},
  {0x13, 0xF7},
  {0x11, 0x01},
  
  {0x0E, 0x00} 
};

/* Camera module register settings for QVGA/RGB565 capture.                   */
REG_VAL CAM_RegSz[] = {
  {0xC8, 0x02},
  {0xC9, 0x80},
  {0xCA, 0x01},
  {0xCB, 0xE0},
  {0xCC, 0x01},
  {0xCd, 0x40},
  {0xCE, 0x00},
  {0xCF, 0xF0},
};

/* Prototypes */
static void CAM_SetQVGA (void);

/*-----------------------------------------------------------------------------
 *      CAM_Init:  Initialize digital camera interface
 *
 *  Parameters: (none)
 *  Return:     (none)
 *----------------------------------------------------------------------------*/
void CAM_Init (void) {
  uint32_t val;

  /* Configure camera size */
  CAM_SetQVGA ();

  val = CAM_RdReg (0x6F) & ~(1 << 7);
  CAM_WrReg (0x6F, val);


  /* Enable GPIOA, GPIOH and GPIOI clocks */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN |
                  RCC_AHB1ENR_GPIOHEN |
                  RCC_AHB1ENR_GPIOIEN ;

  RCC->AHB2ENR |= RCC_AHB2ENR_DCMIEN;

  /* Configure PA6: push-pull output, 100 MHz, with pull-up, AF is DCMI       */
  GPIOA->MODER   &= ~GPIO_MODER_MODER6;
  GPIOA->MODER   |=  GPIO_MODER_MODER6_1;
  GPIOA->OTYPER  &=  GPIO_OTYPER_OT_6;
  GPIOA->OSPEEDR |=  GPIO_OSPEEDER_OSPEEDR6;
  GPIOA->PUPDR   &= ~GPIO_PUPDR_PUPDR6;
  GPIOA->PUPDR   |=  GPIO_PUPDR_PUPDR6_0;
  GPIOA->AFR[0]  &= ~0x0F000000;
  GPIOA->AFR[0]  |=  0x0D000000;
  
  /* Configure PH8, PH9, PH10, PH11, PH12, PH14                               */
  /* Pins are push-pull outputs, 100MHz, with pull-up, AF is DCMI             */
  GPIOH->MODER   &= ~PH_MODER_MASK;
  GPIOH->MODER   |=  PH_MODER_SET;
  GPIOH->OTYPER  &= ~PH_OTYPER_MASK;
  GPIOH->OSPEEDR |=  PH_OSPEEDR_SET;
  GPIOH->PUPDR   &= ~PH_PUPDR_MASK;
  GPIOH->PUPDR   |=  PH_PUPDR_SET;
  GPIOH->AFR[1]  &= ~0x0F0FFFFF;
  GPIOH->AFR[1]  |=  0x0D0DDDDD;

  /* Configure PI4, PI5, PI6, PI7                                             */
  /* Pins are push-pull outputs, 100MHz, with pull-up, AF is DCMI             */
  GPIOI->MODER   &= ~PI_MODER_MASK;
  GPIOI->MODER   |=  PI_MODER_SET;
  GPIOI->OTYPER  &= ~PI_OTYPER_MASK;
  GPIOI->OSPEEDR |=  PI_OSPEEDR_SET;
  GPIOI->PUPDR   &= ~PI_PUPDR_MASK;
  GPIOI->PUPDR   |=  PI_PUPDR_SET;
  GPIOI->AFR[0]  &= ~0xFFFF0000;
  GPIOI->AFR[0]  |=  0xDDDD0000;

  /* Configure DCMI peripheral */
  DCMI->CR &= ~(DCMI_CR_ENABLE | DCMI_CR_CAPTURE);  
  
  DCMI->CR  =  DCMI_CR_PCKPOL |         /* Rising polarity                    */
               DCMI_CR_VSPOL;           /* VS Polarity high                   */
                                        /* HS Polarity low                    */
                                        /* Continuous mode                    */
                                        /* Hardware sync                      */
                                        /* Capture rate all frames            */
                                        /* Extended data mode 8b              */
  /* Configure DMA */
  RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;   /* Enable DMA2 clock                  */

  DMA2_Stream1->CR = DMA_SxCR_CHSEL_0 | /* Select channel 1                   */
                     DMA_SxCR_MSIZE_0 | /* Memory data size is half-word      */
                     DMA_SxCR_PSIZE_1 | /* Peripheral data size is word       */
                     DMA_SxCR_PL_1    | /* DMA priority high                  */
                     DMA_SxCR_CIRC    ; /* Curcular mode enabled              */

  DMA2_Stream1->FCR  = DMA_SxFCR_FTH  | /* FIFO threshold: full FIFO          */
                       DMA_SxFCR_DMDIS; /* Direct mode disabled               */
  DMA2_Stream1->NDTR = 1;               /* Transfer 1 byte                    */
  DMA2_Stream1->PAR  = DCMI_SRC_ADDR;   /* Set DCMI data source address       */
  DMA2_Stream1->M0AR = LCD_DST_ADDR;    /* Set LCD data destination address   */
}


/*-----------------------------------------------------------------------------
 *      CAM_WrReg:  Write a value to camera register
 *
 *  Parameters:  reg - register address
 *               num - number of bytes to write
 *               val - value to be written
 *
 *  Return: 0 on success, nonzero on error
 *----------------------------------------------------------------------------*/
uint32_t CAM_WrReg (uint8_t reg, uint8_t byte) {
  uint32_t err = 0;

  if (I2C_WrData (CAM_I2C_ADDR, reg, &byte, 1)) {
    err |= (1UL << 31);
  }
  return (err);
}


/*-----------------------------------------------------------------------------
 *      CAM_RdReg:  Read a value from camera register
 *
 *  Parameters:  reg - register address
 *
 *  Return: 8-bit register value, >8-bit value on error
 *----------------------------------------------------------------------------*/
uint32_t CAM_RdReg (uint8_t reg) {
  uint32_t err = 0;
  uint8_t  val = 0;

  if (I2C_RdData (CAM_I2C_ADDR, reg, &val, 1)) {
    err |= (1UL << 31);
  }
  return (err | val);
}


/*-----------------------------------------------------------------------------
 *      CAM_SetQVGA:  Configure display size to QVGA (240*320)
 *
 *  Parameters: (none)
 *  Return:     (none)
 *----------------------------------------------------------------------------*/
static void CAM_SetQVGA (void) {
  uint32_t i;

  for (i = 0; i < ARR_SZ(CAM_RegInit); i++) {
    CAM_WrReg (CAM_RegInit[i].Addr, CAM_RegInit[i].Val);
  }

  for (i = 0; i < ARR_SZ(CAM_RegSz); i++) {
    CAM_WrReg (CAM_RegSz[i].Addr, CAM_RegSz[i].Val);
  }
}


/*-----------------------------------------------------------------------------
 *      CAM_On:  Turn camera ON
 *
 *  Parameters: (none)
 *  Return:     0 on success, nonzero otherwise
 *----------------------------------------------------------------------------*/
uint32_t CAM_On (void) {
  uint32_t val;
  
  val  = CAM_RdReg (0x0E);
  val &= ~(1 << 3);
  CAM_WrReg (0x0E, val);                /* Put camera into normal mode        */

  DMA2_Stream1->CR |= DMA_SxCR_EN;      /* Enable DMA                         */
  DCMI->CR |= DCMI_CR_ENABLE;           /* Enable DCMI                        */
  DCMI->CR |= DCMI_CR_CAPTURE;          /* Start image capture                */
  return (0);
}


/*-----------------------------------------------------------------------------
 *      CAM_On:  Turn camera OFF
 *
 *  Parameters: (none)
 *  Return:     0 on success, nonzero otherwise
 *----------------------------------------------------------------------------*/
uint32_t CAM_Off (void) {
  uint32_t val;
 
  DMA2_Stream1->CR &= ~DMA_SxCR_EN;     /* Disable DMA                        */
  DCMI->CR &= ~DCMI_CR_CAPTURE;         /* Disable image capture              */
  DCMI->CR &= ~DCMI_CR_ENABLE;          /* Disable DCMI                       */
  
  val = CAM_RdReg (0x0E);
  CAM_WrReg (0x0E, val | (1 << 3));     /* Put camera into sleep mode         */
  return (0);
}

/*-----------------------------------------------------------------------------
 *      CAM_Test:  Enable camera test mode
 *
 *  Parameters: mode - type of test pattern
 *  Return:     0 on success, nonzero otherwise
 *----------------------------------------------------------------------------*/
uint32_t CAM_Test (uint32_t mode) {
  uint32_t val;

  val = CAM_RdReg (0x0C);
  if (mode == TEST_PAT_OFF) {    
    val &= ~1;                          /* Color bar OFF                      */
  }
  else {    
    val |= 1;                           /* Color bar enable                   */
  }
  CAM_WrReg (0x0C, val);
  return (0);
}

/*-----------------------------------------------------------------------------
 * End of file
 *----------------------------------------------------------------------------*/
