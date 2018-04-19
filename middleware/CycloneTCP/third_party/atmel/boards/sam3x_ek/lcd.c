/******************************************************************************/
/* GLCD_16bitIF_SAM3X.c: ATSAM3X-EK low level Graphic LCD (240x320 pixels)    */
/*                       driven with 16-bit parallel interface                */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2011 Keil - An ARM Company. All rights reserved.        */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#include "sam3xa.h"
#include "lcd.h"
#include "font_6x8.h"
#include "font_16x24.h"

/************************** Orientation  configuration ************************/

#define LANDSCAPE   1                   /* 1 for landscape, 0 for portrait    */
#define ROTATE180   1                   /* 1 to rotate the screen for 180 deg */

/*********************** Hardware specific configuration **********************/

/*------------------------- Speed dependant settings -------------------------*/

/* If processor works on high frequency delay has to be increased, it can be 
   increased by factor 2^N by this constant                                   */
#define DELAY_2N    18

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

/* Note: LCD /CS is CCS2                                                      */
#define LCD_BASE    (0x62000000UL)
#define LCD_REG16   (*((volatile unsigned short *)(LCD_BASE  ))) 
#define LCD_DAT16   (*((volatile unsigned short *)(LCD_BASE+2)))

#define BG_COLOR  0                     /* Background color                   */
#define TXT_COLOR 1                     /* Text color                         */

 
/*---------------------------- Global variables ------------------------------*/

/******************************************************************************/
static volatile unsigned short Color[2] = {White, Black};


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

static __inline void wr_cmd (unsigned char cmd) {

  LCD_REG16 = cmd;
}


/*******************************************************************************
* Write data to the LCD controller                                             *
*   Parameter:    dat:    data to be written                                   *
*   Return:                                                                    *
*******************************************************************************/

static __inline void wr_dat (unsigned short dat) {

  LCD_DAT16 = dat;
}


/*******************************************************************************
* Start of data writing to the LCD controller                                  *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

static __inline void wr_dat_start (void) {

  /* only used for SPI interface */
}


/*******************************************************************************
* Stop of data writing to the LCD controller                                   *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

static __inline void wr_dat_stop (void) {

  /* only used for SPI interface */
}


/*******************************************************************************
* Data writing to the LCD controller                                           *
*   Parameter:    dat:    data to be written                                   *
*   Return:                                                                    *
*******************************************************************************/

static __inline void wr_dat_only (unsigned short dat) {

  LCD_DAT16 = dat;
}

#if 0
/*******************************************************************************
* Read data from the LCD controller                                            *
*   Parameter:                                                                 *
*   Return:               read data                                            *
*******************************************************************************/

static __inline unsigned short rd_dat (void) {

  return (LCD_DAT16);                                    /* return value */
}
#endif

/*******************************************************************************
* Write a value to the to LCD register                                         *
*   Parameter:    reg:    register to be written                               *
*                 val:    value to write to the register                       *
*******************************************************************************/

static __inline void wr_reg (unsigned char reg, unsigned short val) {

  wr_cmd(reg);
  wr_dat(val);
}

#if 0
/*******************************************************************************
* Read from the LCD register                                                   *
*   Parameter:    reg:    register to be read                                  *
*   Return:               value read from the register                         *
*******************************************************************************/

static unsigned short rd_reg (unsigned char reg) {

  wr_cmd(reg);
  return(rd_dat());
}
#endif

/************************ Exported functions **********************************/

/*******************************************************************************
* Initialize the Graphic LCD controller                                        *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

#define LCD_D0_D15      (0x0003FFFC)      /* PIO_PC2 .. PIO_PC17 Peripheral A */
#define LCD_RS          (PIO_PC22)
#define LCD_RD          (PIO_PA29B_NRD)
#define LCD_WR          (PIO_PC18A_NWE)
#define LCD_NCS2        (PIO_PB24B_NCS2)
#define LCD_BACKLIGHT   (PIO_PB27)

void GLCD_Init (void) {
  unsigned int mode, i;

  PMC->PMC_WPMR = 0x504D4300;             /* Disable write protect              */

/* Configure the LCD Control pins --------------------------------------------*/
  PMC->PMC_PCER0  = ((1 << ID_PIOA) |           /* Enable clock for PIOA      */
                     (1 << ID_PIOB) |           /* Enable clock for PIOB      */
                     (1 << ID_PIOC) );          /* Enable clock for PIOC      */

/* Configure the Data Bus pins -----------------------------------------------*/
  PIOC->PIO_PDR   =  (LCD_D0_D15);              /* Enable Peripheral function */
  PIOC->PIO_IDR   =  (LCD_D0_D15);              /* Disable interrupt          */
  PIOC->PIO_ABSR &= ~(LCD_D0_D15);              /* Select Peripheral A        */

/* Configure the Control pins ------------------------------------------------*/
  PIOC->PIO_PDR   =  (LCD_RS);                  /* Enable Peripheral function */
  PIOC->PIO_IDR   =  (LCD_RS);                  /* Disable interrupt          */
  PIOC->PIO_ABSR &= ~(LCD_RS);                  /* Select Peripheral A        */

  PIOA->PIO_PDR   =  (LCD_RD);                  /* Enable Peripheral function */
  PIOA->PIO_IDR   =  (LCD_RD);                  /* Disable interrupt          */
  PIOA->PIO_ABSR |=  (LCD_RD);                  /* Select Peripheral B        */

  PIOC->PIO_PDR   =  (LCD_WR);                  /* Enable Peripheral function */
  PIOC->PIO_IDR   =  (LCD_WR);                  /* Disable interrupt          */
  PIOC->PIO_ABSR &= ~(LCD_WR);                  /* Select Peripheral A        */

  PIOB->PIO_PDR   =  (LCD_NCS2);                /* Enable Peripheral function */
  PIOB->PIO_IDR   =  (LCD_NCS2);                /* Disable interrupt          */
  PIOB->PIO_ABSR |=  (LCD_NCS2);                /* Select Peripheral B        */

/* Configure the Backlight pins ----------------------------------------------*/
  PIOB->PIO_IDR   =                             /* Disable interrupt          */
  PIOB->PIO_OER   =                             /* Enable output              */
  PIOB->PIO_PER   = (LCD_BACKLIGHT);            /* Disable Peripheral function*/
  PIOB->PIO_CODR  = (LCD_BACKLIGHT);            /* Backlight off              */

/*-- SMC Configuration -------------------------------------------------------*/
/*--------------------------- CS2 --------------------------------------------*/
  PMC->PMC_PCER0  = (1 << ID_SMC);              /* Enable clock for SMC       */

  SMC->SMC_CS_NUMBER[2].SMC_SETUP = 0
                                 | (( 4 <<  0) & SMC_SETUP_NWE_SETUP_Msk)
                                 | (( 2 <<  8) & SMC_SETUP_NCS_WR_SETUP_Msk)
                                 | (( 4 << 16) & SMC_SETUP_NRD_SETUP_Msk)
                                 | (( 2 << 24) & SMC_SETUP_NCS_RD_SETUP_Msk)
                                 ;

  SMC->SMC_CS_NUMBER[2].SMC_PULSE = 0
                                 | ((  5 <<  0) & SMC_PULSE_NWE_PULSE_Msk)
                                 | (( 18 <<  8) & SMC_PULSE_NCS_WR_PULSE_Msk)
                                 | ((  5 << 16) & SMC_PULSE_NRD_PULSE_Msk)
                                 | (( 18 << 24) & SMC_PULSE_NCS_RD_PULSE_Msk)
                                 ;

  SMC->SMC_CS_NUMBER[2].SMC_CYCLE = 0
                                 | ((22 <<  0) & SMC_CYCLE_NWE_CYCLE_Msk)
                                 | ((22 << 16) & SMC_CYCLE_NRD_CYCLE_Msk)
                                 ;

  mode = SMC->SMC_CS_NUMBER[2].SMC_MODE;
  SMC->SMC_CS_NUMBER[2].SMC_MODE  = (mode & ~(SMC_MODE_DBW | SMC_MODE_READ_MODE | SMC_MODE_WRITE_MODE))
                                 | (SMC_MODE_READ_MODE)
                                 | (SMC_MODE_WRITE_MODE)
                                 | (SMC_MODE_DBW)
                                 ;

  delay(5);                             /* Delay 50 ms                        */

  /* Start internal OSC                                                       */
  wr_reg(0x19, 0x49);                   /* OSCADJ=10 0000, OSD_EN=1 // 60Hz   */
  wr_reg(0x93, 0x0C);                   /* RADJ=1100                          */

  /* Power on flow                                                            */
  wr_reg(0x44, 0x4D);                   /* VCM=100 1101                       */
  wr_reg(0x45, 0x11);                   /* VDV=1 0001                         */
  wr_reg(0x20, 0x40);                   /* BT=0100                            */
  wr_reg(0x1D, 0x07);                   /* VC1=111                            */
  wr_reg(0x1E, 0x00);                   /* VC3=000                            */
  wr_reg(0x1F, 0x04);                   /* VRH=0100                           */

  wr_reg(0x1C, 0x04);                   /* AP=100                             */
  wr_reg(0x1B, 0x10);                   /* GASENB=0, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0 */
  delay(5);

  wr_reg(0x43, 0x80);                   /* Set VCOMG=1                        */
  delay(5);

  /* Gamma for CMO 2.8                                                        */
  wr_reg(0x46, 0x95);
  wr_reg(0x47, 0x51);
  wr_reg(0x48, 0x00);
  wr_reg(0x49, 0x36);
  wr_reg(0x4A, 0x11);
  wr_reg(0x4B, 0x66);
  wr_reg(0x4C, 0x14);
  wr_reg(0x4D, 0x77);
  wr_reg(0x4E, 0x13);
  wr_reg(0x4F, 0x4C);
  wr_reg(0x50, 0x46);
  wr_reg(0x51, 0x46);

  /* Display Setting                                                          */
  wr_reg(0x01, 0x06);                   /* IDMON=0, INVON=1, NORON=1, PTLON=0 */
#if (LANDSCAPE == 1)
  wr_reg(0x16, 0x68);                   /* MY=0, MX=1, MV=1, BGR=1            */
#else
  wr_reg(0x16, 0xC8);                   /* MY=1, MX=1, MV=0, BGR=1            */
#endif
  wr_reg(0x23, 0x95);                   /* N_DC=1001 0101                     */
  wr_reg(0x24, 0x95);                   /* P_DC=1001 0101                     */
  wr_reg(0x25, 0xFF);                   /* I_DC=1111 1111                     */
  wr_reg(0x27, 0x06);                   /* N_BP=0000 0110                     */
  wr_reg(0x28, 0x06);                   /* N_FP=0000 0110                     */
  wr_reg(0x29, 0x06);                   /* P_BP=0000 0110                     */
  wr_reg(0x2A, 0x06);                   /* P_FP=0000 0110                     */
  wr_reg(0x2C, 0x06);                   /* I_BP=0000 0110                     */
  wr_reg(0x2D, 0x06);                   /* I_FP=0000 0110                     */
  wr_reg(0x3A, 0x01);                   /* N_RTN=0000, N_NW=001               */
  wr_reg(0x3B, 0x01);                   /* P_RTN=0000, P_NW=001               */
  wr_reg(0x3C, 0xF0);                   /* I_RTN=1111, I_NW=000               */
  wr_reg(0x3D, 0x00);                   /* DIV=00                             */
  wr_reg(0x3E, 0x38);                   /* SON=38h                            */
  wr_reg(0x40, 0x0F);                   /* GDON=0Fh                           */
  wr_reg(0x41, 0xF0);                   /* GDOF=F0h                           */

  /* Display ON Setting                                                       */
  wr_reg(0x90, 0x7F);                   /* SAP=0111 1111                      */
  wr_reg(0x26, 0x04);                   /* GON=0, DTE=0, D=01                 */
  delay(10);
  wr_reg(0x26, 0x24);                   /* GON=1, DTE=0, D=01                 */
  wr_reg(0x26, 0x2C);                   /* GON=1, DTE=0, D=11                 */
  delay(10);
  wr_reg(0x26, 0x3C);                   /* GON=1, DTE=1, D=11                 */

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

  /* Display scrolling settings ----------------------------------------------*/
  wr_reg(0x0E, 0x00);                   /* TFA MSB                            */
  wr_reg(0x0F, 0x00);                   /* TFA LSB                            */
  wr_reg(0x10, 320 >> 8);               /* VSA MSB                            */
  wr_reg(0x11, 320 &  0xFF);            /* VSA LSB                            */
  wr_reg(0x12, 0x00);                   /* BFA MSB                            */
  wr_reg(0x13, 0x00);                   /* BFA LSB                            */

  /* Turn backlight on with a certain backlight level                         */
  for (i = 0; i < 25 /* level */; i++) {
    PIOB->PIO_SODR = (LCD_BACKLIGHT);
    PIOB->PIO_SODR = (LCD_BACKLIGHT);
    PIOB->PIO_SODR = (LCD_BACKLIGHT);

    PIOB->PIO_CODR = (LCD_BACKLIGHT);
    PIOB->PIO_CODR = (LCD_BACKLIGHT);
    PIOB->PIO_CODR = (LCD_BACKLIGHT);
  }
  PIOB->PIO_SODR = (LCD_BACKLIGHT);

  PMC->PMC_WPMR = 0x504D4301;             /* Enable write protect               */
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

  xe = x+w-1;
  ye = y+h-1;

  wr_reg(0x02, x  >>    8);             /* Column address start MSB           */
  wr_reg(0x03, x  &  0xFF);             /* Column address start LSB           */
  wr_reg(0x04, xe >>    8);             /* Column address end MSB             */
  wr_reg(0x05, xe &  0xFF);             /* Column address end LSB             */
  
  wr_reg(0x06, y  >>    8);             /* Row address start MSB              */
  wr_reg(0x07, y  &  0xFF);             /* Row address start LSB              */
  wr_reg(0x08, ye >>    8);             /* Row address end MSB                */
  wr_reg(0x09, ye &  0xFF);             /* Row address end LSB                */
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

  wr_reg(0x02, x >>    8);              /* Column address start MSB           */
  wr_reg(0x03, x &  0xFF);              /* Column address start LSB           */
  wr_reg(0x04, x >>    8);              /* Column address end MSB             */
  wr_reg(0x05, x &  0xFF);              /* Column address end LSB             */
  
  wr_reg(0x06, y >>    8);              /* Row address start MSB              */
  wr_reg(0x07, y &  0xFF);              /* Row address start LSB              */
  wr_reg(0x08, y >>    8);              /* Row address end MSB                */
  wr_reg(0x09, y &  0xFF);              /* Row address end LSB                */


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

  wr_reg(0x01, 0x08);
  wr_reg(0x14, y>>8);                   /* VSP MSB                            */
  wr_reg(0x15, y&0xFF);                 /* VSP LSB                            */

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
