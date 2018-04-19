/******************************************************************************/
/* GLCD_16bitIF_STM32F4xx.c: STM32F4xx low level Graphic LCD (320x240 pixels) */
/*                           with 16-bit parallel interface                   */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2011 Keil - An ARM Company. All rights reserved.        */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/


#include <stm32f4xx.h>                  /* STM32F4xx Definitions              */
#include "GLCD.h"
#include "Font_6x8.h"
#include "Font_16x24.h"

/************************** Orientation  configuration ************************/

#define LANDSCAPE   1                   /* 1 for landscape, 0 for portrait    */
#define ROTATE180   0                   /* 1 to rotate the screen for 180 deg */

/*********************** Hardware specific configuration **********************/

/*------------------------- Speed dependant settings -------------------------*/

/* If processor works on high frequency delay has to be increased, it can be 
   increased by factor 2^N by this constant                                   */
#define DELAY_2N    16

/*---------------------- Graphic LCD size definitions ------------------------*/

#if (LANDSCAPE == 1)
#define WIDTH       320                 /* Screen Width (in pixels)           */
#define HEIGHT      240                 /* Screen Hight (in pixels)           */
#else
#define WIDTH       240                 /* Screen Width (in pixels)           */
#define HEIGHT      320                 /* Screen Hight (in pixels)           */
#endif
#define BPP         16                  /* Bits per pixel                     */
#define BYPP        ((BPP+7)/8)         /* Bytes per pixel                    */

/*--------------- Graphic LCD interface hardware definitions -----------------*/

#ifdef __STM_EVAL                       /* STM3220G-EVAL and STM3240G-EVAL    */
/* LCD /CS is NE3 - Bank 3 of NOR/SRAM Bank 1~4                               */
#define LCD_BASE   (0x60000000UL | 0x08000000UL)
#else                                   /* MCBSTM32F200 and MCBSTMF400 board  */
/* LCD /CS is NE4 - Bank 4 of NOR/SRAM Bank 1~4                               */
#define LCD_BASE   (0x60000000UL | 0x0C000000UL)
#endif

#define LCD_REG16  (*((volatile unsigned short *)(LCD_BASE  )))
#define LCD_DAT16  (*((volatile unsigned short *)(LCD_BASE+2)))

#define BG_COLOR  0                     /* Background color                   */
#define TXT_COLOR 1                     /* Text color                         */

 
/*---------------------------- Global variables ------------------------------*/

/******************************************************************************/
static volatile unsigned short Color[2] = {White, Black};
static unsigned char Himax;

/************************ Local auxiliary functions ***************************/

/*******************************************************************************
* Delay in while loop cycles                                                   *
*   Parameter:    cnt:    number of while cycles to delay                      *
*   Return:                                                                    *
*******************************************************************************/

static void delay (int cnt) {
  cnt <<= DELAY_2N;
  while (cnt--);
}


/*******************************************************************************
* Write a command the LCD controller                                           *
*   Parameter:    cmd:    command to be written                                *
*   Return:                                                                    *
*******************************************************************************/

__STATIC_INLINE void wr_cmd (unsigned char cmd) {

  LCD_REG16 = cmd;
}


/*******************************************************************************
* Write data to the LCD controller                                             *
*   Parameter:    dat:    data to be written                                   *
*   Return:                                                                    *
*******************************************************************************/

__STATIC_INLINE void wr_dat (unsigned short dat) {

  LCD_DAT16 = dat;
}


/*******************************************************************************
* Start of data writing to the LCD controller                                  *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

__STATIC_INLINE void wr_dat_start (void) {

  /* only used for SPI interface */
}


/*******************************************************************************
* Stop of data writing to the LCD controller                                   *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

__STATIC_INLINE void wr_dat_stop (void) {

  /* only used for SPI interface */
}


/*******************************************************************************
* Data writing to the LCD controller                                           *
*   Parameter:    dat:    data to be written                                   *
*   Return:                                                                    *
*******************************************************************************/

__STATIC_INLINE void wr_dat_only (unsigned short dat) {

  LCD_DAT16 = dat;
}


/*******************************************************************************
* Read data from the LCD controller                                            *
*   Parameter:                                                                 *
*   Return:               read data                                            *
*******************************************************************************/

__STATIC_INLINE unsigned short rd_dat (void) {

  return (LCD_DAT16);                                    /* return value */
}


/*******************************************************************************
* Write a value to the to LCD register                                         *
*   Parameter:    reg:    register to be written                               *
*                 val:    value to write to the register                       *
*******************************************************************************/

__STATIC_INLINE void wr_reg (unsigned char reg, unsigned short val) {

  wr_cmd(reg);
  wr_dat(val);
}


/*******************************************************************************
* Read from the LCD register                                                   *
*   Parameter:    reg:    register to be read                                  *
*   Return:               value read from the register                         *
*******************************************************************************/

static unsigned short rd_reg (unsigned char reg) {

  wr_cmd(reg);
  return(rd_dat());
}


/************************ Exported functions **********************************/

/*******************************************************************************
* Initialize the Graphic LCD controller                                        *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_Init (void) {
  unsigned long temp;
  unsigned short driverCode;

/* Configure the LCD Control pins --------------------------------------------*/
  RCC->AHB1ENR    |=((1UL <<  0) |      /* Enable GPIOA clock                 */
#ifndef __STM_EVAL                      /* MCBSTM32F200 and MCBSTMF400 board  */
                    (1UL <<  2)  |      /* Enable GPIOC clock                 */
#endif
                     (1UL <<  3) |      /* Enable GPIOD clock                 */
                     (1UL <<  4) |      /* Enable GPIOE clock                 */
                     (1UL <<  5) |      /* Enable GPIOF clock                 */
                     (1UL <<  6));      /* Enable GPIOG clock                 */

  /* PD.00(D2),  PD.01(D3),  PD.04(NOE), PD.05(NWE)                           */
  /* PD.08(D13), PD.09(D14), PD.10(D15), PD.14(D0), PD.15(D1)                 */
  temp = GPIOD->MODER & ~0xF03F0F0F;      /* Clear Bits                         */
  GPIOD->MODER = temp | 0xA02A0A0A;       /* Alternate Function mode            */
  temp = GPIOD->OSPEEDR & ~0xF03F0F0F;    /* Clear Bits                         */
  GPIOD->OSPEEDR = temp | 0xA02A0A0A;     /* 50 MHz Fast speed                  */
  temp = GPIOD->AFR[0] & ~0x00FF00FF;     /* Clear Bits                         */
  GPIOD->AFR[0] = temp | 0x00CC00CC;      /* Alternate Function mode AF12       */
  temp = GPIOD->AFR[1] & ~0xFF000FFF;     /* Clear Bits                         */
  GPIOD->AFR[1] = temp | 0xCC000CCC;      /* Alternate Function mode AF12       */

  /* PE.07(D4), PE.08(D5),  PE.09(D6),  PE.10(D7), PE.11(D8)                  */
  /* PE.12(D9), PE.13(D10), PE.14(D11), PE.15(D12)                            */
  temp = GPIOE->MODER & ~0xFFFFC000;      /* Clear Bits                         */
  GPIOE->MODER = temp | 0xAAAA8000;       /* Alternate Function mode            */
  temp = GPIOE->OSPEEDR & ~0xFFFFC000;    /* Clear Bits                         */
  GPIOE->OSPEEDR = temp | 0xAAAA8000;     /* 50 MHz Fast speed                  */
  temp = GPIOE->AFR[0] & ~0xF0000000;     /* Clear Bits                         */
  GPIOE->AFR[0] = temp | 0xC0000000;      /* Alternate Function mode AF12       */
  temp = GPIOE->AFR[1] & ~0xFFFFFFFF;     /* Clear Bits                         */
  GPIOE->AFR[1] = temp | 0xCCCCCCCC;      /* Alternate Function mode AF12       */

  /* PF.00(A0 (RS)) */ 
  temp = GPIOF->MODER & ~0x00000003;      /* Clear Bits                         */
  GPIOF->MODER = temp | 0x00000002;       /* Alternate Function mode            */
  temp = GPIOF->OSPEEDR & ~0x00000003;    /* Clear Bits                         */
  GPIOF->OSPEEDR = temp | 0x00000002;     /* 50 MHz Fast speed                  */
  temp = GPIOF->AFR[0] & ~0x0000000F;     /* Clear Bits                         */
  GPIOF->AFR[0] = temp | 0x0000000C;      /* Alternate Function mode AF12       */

#ifdef __STM_EVAL                         /* STM3220G-EVAL and STM3240G-EVAL    */
  /* PG.10(NE4 (LCD/CS)) - CE1(LCD /CS)                                       */
  temp = GPIOG->MODER & ~0x00300000;      /* Clear Bits                         */
  GPIOG->MODER = temp | 0x00200000;       /* Alternate Function mode            */
  temp = GPIOG->OSPEEDR = ~0x00300000;    /* Clear Bits                         */
  GPIOG->OSPEEDR = temp | 0x00200000;     /* 50 MHz Fast speed                  */
  temp = GPIOG->AFR[1] & ~0x00000F00;     /* Clear Bits                         */
  GPIOG->AFR[1] = temp | 0x00000C00;      /* Alternate Function mode AF12       */

  /* PA.08(LCD Backlight                                                      */
  GPIOA->BSRRH |= (1UL << 8);             /* Backlight off                      */
  temp = GPIOA->MODER & ~(3UL << 2*8);    /* Clear Bits                         */
  GPIOA->MODER = temp | (1UL << 2*8);     /* PA.9 is output                     */
  GPIOA->OTYPER &= ~(1UL << 8);           /* PA.9 is output Push-Pull           */
  temp = GPIOA->OSPEEDR & ~(3UL << 2*8);  /* Clear Bits                         */
  GPIOA->OSPEEDR = temp | (2UL << 2*8);   /* PA.9 is 50MHz Fast Speed           */

#else                                   /* MCBSTM32F200 and MCBSTMF400 board  */
  /* PG.12(NE4 (LCD/CS)) - CE1(LCD /CS)                                       */
  temp = GPIOG->MODER & ~0x03000000;      /* Clear Bits                         */
  GPIOG->MODER = temp | 0x02000000;       /* Alternate Function mode            */
  temp = GPIOG->OSPEEDR & ~0x03000000;    /* Clear Bits                         */
  GPIOG->OSPEEDR = temp | 0x02000000;     /* 50 MHz Fast speed                  */
  temp = GPIOG->AFR[1] & ~0x000F0000;     /* Clear Bits                         */
  GPIOG->AFR[1] = temp | 0x000C0000;      /* Alternate Function mode AF12       */

  /* PC.07(LCD Backlight                                                      */
  GPIOC->BSRRH |= (1UL << 7);             /* Backlight off                      */
  temp = GPIOC->MODER & ~(3UL << 2*7);    /* Clear Bits                         */
  GPIOC->MODER = temp | (1UL << 2*7);     /* PC.7 is output                     */
  GPIOC->OTYPER &= ~(1UL << 7);           /* PC.7 is output Push-Pull           */
  temp = GPIOC->OSPEEDR & ~(3UL << 2*7);  /* Clear Bits                         */
  GPIOC->OSPEEDR = temp | (2UL << 2*7);   /* PC.7 is 50MHz Fast Speed           */
#endif


/*-- FSMC Configuration ------------------------------------------------------*/
  RCC->AHB3ENR  |= (1UL << 0);          /* Enable FSMC clock                  */

#ifdef __STM_EVAL                       /* STM3220G-EVAL and STM3240G-EVAL    */
  FSMC_Bank1->BTCR[(3-1)*2 + 1] =       /* Bank3 NOR/SRAM timing register     */
                                        /* configuration                      */
#else                                   /* MCBSTM32F200 and MCBSTMF400 board  */
  FSMC_Bank1->BTCR[(4-1)*2 + 1] =       /* Bank4 NOR/SRAM timing register     */
                                        /* configuration                      */
#endif
                          (0 << 28) |   /* FSMC AccessMode A                  */
                          (0 << 24) |   /* Data Latency                       */
                          (0 << 20) |   /* CLK Division                       */
                          (0 << 16) |   /* Bus Turnaround Duration            */
                         (11 <<  8) |   /* Data SetUp Time                    */
                          (0 <<  4) |   /* Address Hold Time                  */
                          (1 <<  0);    /* Address SetUp Time                 */

#ifdef __STM_EVAL                       /* STM3220G-EVAL and STM3240G-EVAL    */
  FSMC_Bank1->BTCR[(3-1)*2 + 0] =
#else                                   /* MCBSTM32F200 and MCBSTMF400 board  */
  FSMC_Bank1->BTCR[(4-1)*2 + 0] =       /* Control register                   */
#endif
                          (0 << 19) |   /* Write burst disabled               */
                          (0 << 15) |   /* Async wait disabled                */
                          (0 << 14) |   /* Extended mode disabled             */
                          (0 << 13) |   /* NWAIT signal is disabled           */ 
                          (1 << 12) |   /* Write operation enabled            */
                          (0 << 11) |   /* NWAIT signal is active one data    */
                                        /* cycle before wait state            */
                          (0 << 10) |   /* Wrapped burst mode disabled        */
                          (0 <<  9) |   /* Wait signal polarity active low    */
                          (0 <<  8) |   /* Burst access mode disabled         */
                          (1 <<  4) |   /* Memory data  bus width is 16 bits  */
                          (0 <<  2) |   /* Memory type is SRAM                */
                          (0 <<  1) |   /* Address/Data Multiplexing disable  */
                          (1 <<  0);    /* Memory Bank enable                 */

  delay(5);                             /* Delay 50 ms                        */
  driverCode = rd_reg(0x00);

  if (driverCode == 0x47) {             /* LCD with HX8347-D LCD Controller   */
    Himax = 1;                          /* Set Himax LCD controller flag      */
    /* Driving ability settings ----------------------------------------------*/
    wr_reg(0xEA, 0x00);                 /* Power control internal used (1)    */
    wr_reg(0xEB, 0x20);                 /* Power control internal used (2)    */
    wr_reg(0xEC, 0x0C);                 /* Source control internal used (1)   */
    wr_reg(0xED, 0xC7);                 /* Source control internal used (2)   */
    wr_reg(0xE8, 0x38);                 /* Source output period Normal mode   */
    wr_reg(0xE9, 0x10);                 /* Source output period Idle mode     */
    wr_reg(0xF1, 0x01);                 /* RGB 18-bit interface ;0x0110       */
    wr_reg(0xF2, 0x10);

    /* Adjust the Gamma Curve ------------------------------------------------*/
    wr_reg(0x40, 0x01);
    wr_reg(0x41, 0x00);
    wr_reg(0x42, 0x00);
    wr_reg(0x43, 0x10);
    wr_reg(0x44, 0x0E);
    wr_reg(0x45, 0x24);
    wr_reg(0x46, 0x04);
    wr_reg(0x47, 0x50);
    wr_reg(0x48, 0x02);
    wr_reg(0x49, 0x13);
    wr_reg(0x4A, 0x19);
    wr_reg(0x4B, 0x19);
    wr_reg(0x4C, 0x16);

    wr_reg(0x50, 0x1B);
    wr_reg(0x51, 0x31);
    wr_reg(0x52, 0x2F);
    wr_reg(0x53, 0x3F);
    wr_reg(0x54, 0x3F);
    wr_reg(0x55, 0x3E);
    wr_reg(0x56, 0x2F);
    wr_reg(0x57, 0x7B);
    wr_reg(0x58, 0x09);
    wr_reg(0x59, 0x06);
    wr_reg(0x5A, 0x06);
    wr_reg(0x5B, 0x0C);
    wr_reg(0x5C, 0x1D);
    wr_reg(0x5D, 0xCC);

    /* Power voltage setting -------------------------------------------------*/
    wr_reg(0x1B, 0x1B);
    wr_reg(0x1A, 0x01);
    wr_reg(0x24, 0x2F);
    wr_reg(0x25, 0x57);
    wr_reg(0x23, 0x88);

    /* Power on setting ------------------------------------------------------*/
    wr_reg(0x18, 0x36);                 /* Internal oscillator frequency adj  */
    wr_reg(0x19, 0x01);                 /* Enable internal oscillator         */
    wr_reg(0x01, 0x00);                 /* Normal mode, no scrool             */
    wr_reg(0x1F, 0x88);                 /* Power control 6 - DDVDH Off        */
    delay(20);
    wr_reg(0x1F, 0x82);                 /* Power control 6 - Step-up: 3 x VCI */
    delay(5);
    wr_reg(0x1F, 0x92);                 /* Power control 6 - Step-up: On      */
    delay(5);
    wr_reg(0x1F, 0xD2);                 /* Power control 6 - VCOML active     */
    delay(5);

    /* Color selection -------------------------------------------------------*/
    wr_reg(0x17, 0x55);                 /* RGB, System interface: 16 Bit/Pixel*/
    wr_reg(0x00, 0x00);                 /* Scrolling off, no standby          */

    /* Interface config ------------------------------------------------------*/
    wr_reg(0x2F, 0x11);                 /* LCD Drive: 1-line inversion        */
    wr_reg(0x31, 0x00);
    wr_reg(0x32, 0x00);                 /* DPL=0, HSPL=0, VSPL=0, EPL=0       */

    /* Display on setting ----------------------------------------------------*/
    wr_reg(0x28, 0x38);                 /* PT(0,0) active, VGL/VGL            */
    delay(20);
    wr_reg(0x28, 0x3C);                 /* Display active, VGL/VGL            */

   #if (LANDSCAPE == 1)
    #if (ROTATE180 == 0)
     wr_reg (0x16, 0xA8);
    #else
     wr_reg (0x16, 0x68);
    #endif
   #else
    #if (ROTATE180 == 0)
     wr_reg (0x16, 0x08);
    #else
     wr_reg (0x16, 0xC8);
    #endif
   #endif

    /* Display scrolling settings --------------------------------------------*/
    wr_reg(0x0E, 0x00);                 /* TFA MSB                            */
    wr_reg(0x0F, 0x00);                 /* TFA LSB                            */
    wr_reg(0x10, 320 >> 8);             /* VSA MSB                            */
    wr_reg(0x11, 320 &  0xFF);          /* VSA LSB                            */
    wr_reg(0x12, 0x00);                 /* BFA MSB                            */
    wr_reg(0x13, 0x00);                 /* BFA LSB                            */
  }
  else {
    Himax = 0;                          /* This is not Himax LCD controller   */
    /* Start Initial Sequence ------------------------------------------------*/
   #if (ROTATE180 == 1)
    wr_reg(0x01, 0x0000);               /* Clear SS bit                       */
   #else
    wr_reg(0x01, 0x0100);               /* Set SS bit                         */
   #endif
    wr_reg(0x02, 0x0700);               /* Set 1 line inversion               */
    wr_reg(0x04, 0x0000);               /* Resize register                    */
    wr_reg(0x08, 0x0207);               /* 2 lines front, 7 back porch        */
    wr_reg(0x09, 0x0000);               /* Set non-disp area refresh cyc ISC  */
    wr_reg(0x0A, 0x0000);               /* FMARK function                     */
    wr_reg(0x0C, 0x0000);               /* RGB interface setting              */
    wr_reg(0x0D, 0x0000);               /* Frame marker Position              */
    wr_reg(0x0F, 0x0000);               /* RGB interface polarity             */

    /* Power On sequence -----------------------------------------------------*/
    wr_reg(0x10, 0x0000);               /* Reset Power Control 1              */
    wr_reg(0x11, 0x0000);               /* Reset Power Control 2              */
    wr_reg(0x12, 0x0000);               /* Reset Power Control 3              */
    wr_reg(0x13, 0x0000);               /* Reset Power Control 4              */
    delay(20);                          /* Discharge cap power voltage (200ms)*/
    wr_reg(0x10, 0x12B0);               /* SAP, BT[3:0], AP, DSTB, SLP, STB   */
    wr_reg(0x11, 0x0007);               /* DC1[2:0], DC0[2:0], VC[2:0]        */
    delay(5);                           /* Delay 50 ms                        */
    wr_reg(0x12, 0x01BD);               /* VREG1OUT voltage                   */
    delay(5);                           /* Delay 50 ms                        */
    wr_reg(0x13, 0x1400);               /* VDV[4:0] for VCOM amplitude        */
    wr_reg(0x29, 0x000E);               /* VCM[4:0] for VCOMH                 */
    delay(5);                           /* Delay 50 ms                        */
    wr_reg(0x20, 0x0000);               /* GRAM horizontal Address            */
    wr_reg(0x21, 0x0000);               /* GRAM Vertical Address              */

    /* Adjust the Gamma Curve ------------------------------------------------*/
    switch (driverCode) {
      case 0x5408:                      /* LCD with SPFD5408 LCD Controller   */
        wr_reg(0x30, 0x0B0D);
        wr_reg(0x31, 0x1923);
        wr_reg(0x32, 0x1C26);
        wr_reg(0x33, 0x261C);
        wr_reg(0x34, 0x2419);
        wr_reg(0x35, 0x0D0B);
        wr_reg(0x36, 0x1006);
        wr_reg(0x37, 0x0610);
        wr_reg(0x38, 0x0706);
        wr_reg(0x39, 0x0304);
        wr_reg(0x3A, 0x0E05);
        wr_reg(0x3B, 0x0E01);
        wr_reg(0x3C, 0x010E);
        wr_reg(0x3D, 0x050E);
        wr_reg(0x3E, 0x0403);
        wr_reg(0x3F, 0x0607);
        break;

      case 0x9325:                      /* LCD with RM68050 LCD Controller    */
        wr_reg(0x0030,0x0000);
        wr_reg(0x0031,0x0607);
        wr_reg(0x0032,0x0305);
        wr_reg(0x0035,0x0000);
        wr_reg(0x0036,0x1604);
        wr_reg(0x0037,0x0204);
        wr_reg(0x0038,0x0001);
        wr_reg(0x0039,0x0707);
        wr_reg(0x003C,0x0000);
        wr_reg(0x003D,0x000F);
        break;

      case 0x9320:                      /* LCD with ILI9320 LCD Controller    */
      default:                          /* LCD with other LCD Controller      */
        wr_reg(0x30, 0x0006);
        wr_reg(0x31, 0x0101);
        wr_reg(0x32, 0x0003);
        wr_reg(0x35, 0x0106);
        wr_reg(0x36, 0x0B02);
        wr_reg(0x37, 0x0302);
        wr_reg(0x38, 0x0707);
        wr_reg(0x39, 0x0007);
        wr_reg(0x3C, 0x0600);
        wr_reg(0x3D, 0x020B);
        break;
    }

    /* Set GRAM area ---------------------------------------------------------*/
    wr_reg(0x50, 0x0000);               /* Horizontal GRAM Start Address      */
    wr_reg(0x51, (HEIGHT-1));           /* Horizontal GRAM End   Address      */
    wr_reg(0x52, 0x0000);               /* Vertical   GRAM Start Address      */
    wr_reg(0x53, (WIDTH-1));            /* Vertical   GRAM End   Address      */

    /* Set Gate Scan Line ----------------------------------------------------*/
    switch (driverCode) {
      case 0x5408:                      /* LCD with SPFD5408 LCD Controller   */
      case 0x9325:                      /* LCD with RM68050 LCD Controller    */
       #if (LANDSCAPE ^ ROTATE180)
        wr_reg(0x60, 0x2700);
       #else
        wr_reg(0x60, 0xA700);
       #endif
        break;

      case 0x9320:                      /* LCD with ILI9320 LCD Controller    */
      default:                          /* LCD with other LCD Controller      */
       #if (LANDSCAPE ^ ROTATE180)
        wr_reg(0x60, 0xA700);
       #else
        wr_reg(0x60, 0x2700);
       #endif
        break;
    }
    wr_reg(0x61, 0x0001);               /* NDL,VLE, REV                       */
    wr_reg(0x6A, 0x0000);               /* Set scrolling line                 */

    /* Partial Display Control -----------------------------------------------*/
    wr_reg(0x80, 0x0000);
    wr_reg(0x81, 0x0000);
    wr_reg(0x82, 0x0000);
    wr_reg(0x83, 0x0000);
    wr_reg(0x84, 0x0000);
    wr_reg(0x85, 0x0000);

    /* Panel Control ---------------------------------------------------------*/
    wr_reg(0x90, 0x0010);
    wr_reg(0x92, 0x0000);
    wr_reg(0x93, 0x0003);
    wr_reg(0x95, 0x0110);
    wr_reg(0x97, 0x0000);
    wr_reg(0x98, 0x0000);

    /* Set GRAM write direction
       I/D=11 (Horizontal : increment, Vertical : increment)                  */
  #if (LANDSCAPE == 1)
    /* AM=1   (address is updated in vertical writing direction)              */
    wr_reg(0x03, 0x1038);
  #else
    /* AM=0   (address is updated in horizontal writing direction)            */
    wr_reg(0x03, 0x1030);
  #endif

    wr_reg(0x07, 0x0137);               /* 262K color and display ON          */
  }

#ifdef __STM_EVAL                       /* STM3220G-EVAL and STM3240G-EVAL    */
  GPIOA->BSRRL |= (1UL << 8);           /* Backlight on                       */
#else                                   /* MCBSTM32F200 and MCBSTMF400 board  */
  GPIOC->BSRRL |= (1UL << 7);           /* Backlight on                       */
#endif
}


/*******************************************************************************
* Set draw window region                                                       *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*                   w:        window width in pixel                            *
*                   h:        window height in pixels                          *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_SetWindow (unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
  unsigned int xe, ye;

  if (Himax) {
    xe = x+w-1;
    ye = y+h-1;

    wr_reg(0x02, x  >>    8);           /* Column address start MSB           */
    wr_reg(0x03, x  &  0xFF);           /* Column address start LSB           */
    wr_reg(0x04, xe >>    8);           /* Column address end MSB             */
    wr_reg(0x05, xe &  0xFF);           /* Column address end LSB             */
  
    wr_reg(0x06, y  >>    8);           /* Row address start MSB              */
    wr_reg(0x07, y  &  0xFF);           /* Row address start LSB              */
    wr_reg(0x08, ye >>    8);           /* Row address end MSB                */
    wr_reg(0x09, ye &  0xFF);           /* Row address end LSB                */
  }
  else {
   #if (LANDSCAPE == 1)
    wr_reg(0x50, y);                    /* Vertical   GRAM Start Address      */
    wr_reg(0x51, y+h-1);                /* Vertical   GRAM End   Address (-1) */
    wr_reg(0x52, x);                    /* Horizontal GRAM Start Address      */
    wr_reg(0x53, x+w-1);                /* Horizontal GRAM End   Address (-1) */
    wr_reg(0x20, y);
    wr_reg(0x21, x);
   #else
    wr_reg(0x50, x);                    /* Horizontal GRAM Start Address      */
    wr_reg(0x51, x+w-1);                /* Horizontal GRAM End   Address (-1) */
    wr_reg(0x52, y);                    /* Vertical   GRAM Start Address      */
    wr_reg(0x53, y+h-1);                /* Vertical   GRAM End   Address (-1) */
    wr_reg(0x20, x);
    wr_reg(0x21, y);
   #endif
  }
}


/*******************************************************************************
* Set draw window region to whole screen                                       *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_WindowMax (void) {
  GLCD_SetWindow (0, 0, WIDTH, HEIGHT);
}


/*******************************************************************************
* Draw a pixel in foreground color                                             *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_PutPixel (unsigned int x, unsigned int y) {

  if (Himax) {
    wr_reg(0x02, x >>    8);            /* Column address start MSB           */
    wr_reg(0x03, x &  0xFF);            /* Column address start LSB           */
    wr_reg(0x04, x >>    8);            /* Column address end MSB             */
    wr_reg(0x05, x &  0xFF);            /* Column address end LSB             */
  
    wr_reg(0x06, y >>    8);            /* Row address start MSB              */
    wr_reg(0x07, y &  0xFF);            /* Row address start LSB              */
    wr_reg(0x08, y >>    8);            /* Row address end MSB                */
    wr_reg(0x09, y &  0xFF);            /* Row address end LSB                */
  }
  else {
   #if (LANDSCAPE == 1)
    wr_reg(0x20, y);
    wr_reg(0x21, x);
   #else
    wr_reg(0x20, x);
    wr_reg(0x21, y);
   #endif
  }

  wr_cmd(0x22);
  wr_dat(Color[TXT_COLOR]);
}


/*******************************************************************************
* Set foreground color                                                         *
*   Parameter:      color:    foreground color                                 *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_SetTextColor (unsigned short color) {

  Color[TXT_COLOR] = color;
}


/*******************************************************************************
* Set background color                                                         *
*   Parameter:      color:    background color                                 *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_SetBackColor (unsigned short color) {

  Color[BG_COLOR] = color;
}


/*******************************************************************************
* Clear display                                                                *
*   Parameter:      color:    display clearing color                           *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_Clear (unsigned short color) {
  unsigned int i;

  GLCD_WindowMax();
  wr_cmd(0x22);
  wr_dat_start();

  for(i = 0; i < (WIDTH*HEIGHT); i++)
    wr_dat_only(color);
  wr_dat_stop();
}


/*******************************************************************************
* Draw character on given position                                             *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*                   cw:       character width in pixel                         *
*                   ch:       character height in pixels                       *
*                   c:        pointer to character bitmap                      *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_DrawChar (unsigned int x, unsigned int y, unsigned int cw, unsigned int ch, unsigned char *c) {
  unsigned int i, j, k, pixs;

  GLCD_SetWindow(x, y, cw, ch);

  wr_cmd(0x22);
  wr_dat_start();

  k  = (cw + 7)/8;

  if (k == 1) {
    for (j = 0; j < ch; j++) {
      pixs = *(unsigned char  *)c;
      c += 1;
      
      for (i = 0; i < cw; i++) {
        wr_dat_only (Color[(pixs >> i) & 1]);
      }
    }
  }
  else if (k == 2) {
    for (j = 0; j < ch; j++) {
      pixs = *(unsigned short *)c;
      c += 2;
      
      for (i = 0; i < cw; i++) {
        wr_dat_only (Color[(pixs >> i) & 1]);
      }
    }
  }
  wr_dat_stop();
}


/*******************************************************************************
* Disply character on given line                                               *
*   Parameter:      ln:       line number                                      *
*                   col:      column number                                    *
*                   fi:       font index (0 = 6x8, 1 = 16x24)                  *
*                   c:        ascii character                                  *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_DisplayChar (unsigned int ln, unsigned int col, unsigned char fi, unsigned char c) {

  c -= 32;
  switch (fi) {
    case 0:  /* Font 6 x 8 */
      GLCD_DrawChar(col *  6, ln *  8,  6,  8, (unsigned char *)&Font_6x8_h  [c * 8]);
      break;
    case 1:  /* Font 16 x 24 */
      GLCD_DrawChar(col * 16, ln * 24, 16, 24, (unsigned char *)&Font_16x24_h[c * 24]);
      break;
  }
}


/*******************************************************************************
* Disply string on given line                                                  *
*   Parameter:      ln:       line number                                      *
*                   col:      column number                                    *
*                   fi:       font index (0 = 6x8, 1 = 16x24)                  *
*                   s:        pointer to string                                *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_DisplayString (unsigned int ln, unsigned int col, unsigned char fi, unsigned char *s) {

  while (*s) {
    GLCD_DisplayChar(ln, col++, fi, *s++);
  }
}


/*******************************************************************************
* Clear given line                                                             *
*   Parameter:      ln:       line number                                      *
*                   fi:       font index (0 = 6x8, 1 = 16x24)                  *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_ClearLn (unsigned int ln, unsigned char fi) {
  unsigned char i;
  unsigned char buf[60];

  GLCD_WindowMax();
  switch (fi) {
    case 0:  /* Font 6 x 8 */
      for (i = 0; i < (WIDTH+5)/6; i++)
        buf[i] = ' ';
      buf[i+1] = 0;
      break;
    case 1:  /* Font 16 x 24 */
      for (i = 0; i < (WIDTH+15)/16; i++)
        buf[i] = ' ';
      buf[i+1] = 0;
      break;
  }
  GLCD_DisplayString (ln, 0, fi, buf);
}

/*******************************************************************************
* Draw bargraph                                                                *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*                   w:        maximum width of bargraph (in pixels)            *
*                   h:        bargraph height                                  *
*                   val:      value of active bargraph (in 1/1024)             *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_Bargraph (unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int val) {
  int i,j;

  val = (val * w) >> 10;                /* Scale value                        */
  GLCD_SetWindow(x, y, w, h);
  wr_cmd(0x22);
  wr_dat_start();
  for (i = 0; i < h; i++) {
    for (j = 0; j <= w-1; j++) {
      if(j >= val) {
        wr_dat_only(Color[BG_COLOR]);
      } else {
        wr_dat_only(Color[TXT_COLOR]);
      }
    }
  }
  wr_dat_stop();
}


/*******************************************************************************
* Display graphical bitmap image at position x horizontally and y vertically   *
* (This function is optimized for 16 bits per pixel format, it has to be       *
*  adapted for any other bits per pixel format)                                *
*   Parameter:      x:        horizontal position                              *
*                   y:        vertical position                                *
*                   w:        width of bitmap                                  *
*                   h:        height of bitmap                                 *
*                   bitmap:   address at which the bitmap data resides         *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_Bitmap (unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char *bitmap) {
  int i, j;
  unsigned short *bitmap_ptr = (unsigned short *)bitmap;

  GLCD_SetWindow (x, y, w, h);

  wr_cmd(0x22);
  wr_dat_start();
  for (i = (h-1)*w; i > -1; i -= w) {
    for (j = 0; j < w; j++) {
      wr_dat_only (bitmap_ptr[i+j]);
    }
  }
  wr_dat_stop();
}



/*******************************************************************************
* Scroll content of the whole display for dy pixels vertically                 *
*   Parameter:      dy:       number of pixels for vertical scroll             *
*   Return:                                                                    *
*******************************************************************************/

void GLCD_ScrollVertical (unsigned int dy) {
#if (LANDSCAPE == 0)
  static unsigned int y = 0;

  y = y + dy;
  while (y >= HEIGHT)
    y -= HEIGHT;

  if (Himax) {
    wr_reg(0x01, 0x08);
    wr_reg(0x14, y>>8);                 /* VSP MSB                            */
    wr_reg(0x15, y&0xFF);               /* VSP LSB                            */
  }
  else {
    wr_reg(0x6A, y);
    wr_reg(0x61, 3);
  }
#endif
}


/*******************************************************************************
* Write a command to the LCD controller                                        *
*   Parameter:      cmd:      command to write to the LCD                      *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_WrCmd (unsigned char cmd) {
  wr_cmd (cmd);
}


/*******************************************************************************
* Write a value into LCD controller register                                   *
*   Parameter:      reg:      lcd register address                             *
*                   val:      value to write into reg                          *
*   Return:                                                                    *
*******************************************************************************/
void GLCD_WrReg (unsigned char reg, unsigned short val) {
  wr_reg (reg, val);
}
/******************************************************************************/
