/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : glcd_ll.h
 *    Description : GLCD low level include file
 *
 *    History :
 *    1. Date        : December 2, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: #2 $
 **************************************************************************/
#ifndef __GLCD_LL_H
#define __GLCD_LL_H

#include "arm_comm.h"

#define BACKLIGHT_OFF     0x00
#define BACKLIGHT_ON      0x80
#define SSP_FIFO_SIZE     8

#define GLCD_SPI_CLK      400    // [kHz]

// LCD controller reset pin - PD3
#define LCD_RST_PORT      GPIOD
#define LCD_RST_MASK      GPIO_Pin_3
#define LCD_RST_CLK       RCC_AHB1Periph_GPIOD

// LCD controller CS pin - PD6
#define LCD_CS_PORT       GPIOD
#define LCD_CS_MASK       GPIO_Pin_6
#define LCD_CS_CLK        RCC_AHB1Periph_GPIOD

// LCD controller BACK LIGHT pin
#define LCD_BL_PORT         GPIOB
#define LCD_BL_MASK         GPIO_Pin_0
#define LCD_BL_CLK          RCC_AHB1Periph_GPIOB
#define LCD_BL_PIN_SOURCE   GPIO_PinSource0
#define LCD_BL_PIN_AF       GPIO_AF_TIM3

// LCD controller SPI pins - PA5, PC3, PB4
#define LCD_SPI_SCLK_PORT   GPIOA
#define LCD_SPI_SCLK_MASK   GPIO_Pin_5
#define LCD_SPI_SCLK_CLK    RCC_AHB1Periph_GPIOA

#define LCD_SPI_MOSI_PORT   GPIOC
#define LCD_SPI_MOSI_MASK   GPIO_Pin_3
#define LCD_SPI_MOSI_CLK    RCC_AHB1Periph_GPIOC

/* multiplexed with JTAG do not use */
#define LCD_SPI_MISO_PORT   GPIOB
#define LCD_SPI_MISO_MASK   GPIO_Pin_4
#define LCD_SPI_MISO_CLK    RCC_AHB1Periph_GPIOB

#define GLCD_SPI_MOSI_H() LCD_SPI_MOSI_PORT->BSRRL = LCD_SPI_MOSI_MASK
#define GLCD_SPI_MOSI_L() LCD_SPI_MOSI_PORT->BSRRH  = LCD_SPI_MOSI_MASK
#define GLCD_SPI_CLK_H()  LCD_SPI_SCLK_PORT->BSRRL = LCD_SPI_SCLK_MASK
#define GLCD_SPI_CLK_L()  LCD_SPI_SCLK_PORT->BSRRH  = LCD_SPI_SCLK_MASK
#define GLCD_SPI_MISO()   (0 != (LCD_SPI_MISO_PORT->IDR & LCD_SPI_MISO_MASK))

/*************************************************************************
 * Function Name: GLCD_SetReset
 * Parameters: Boolean State
 * Return: none
 *
 * Description: Set reset pin state
 *
 *************************************************************************/
void GLCD_SetReset (Boolean State);

/*************************************************************************
 * Function Name: GLCD_SetBacklight
 * Parameters: Int8U Light
 * Return: none
 *
 * Description: Set backlight pin state
 *
 *************************************************************************/
void GLCD_Backlight (Int8U Light);

/*************************************************************************
 * Function Name: GLCD_LLInit
 * Parameters: none
 * Return: none
 *
 * Description: Init Reset and Backlight control outputs
 *
 *************************************************************************/
void GLCD_LLInit (void);

/*************************************************************************
 * Function Name: LcdSpiChipSelect
 * Parameters: Boolean Select
 * Return: none
 *
 * Description: SPI Chip select control
 * Select = true  - Chip is enable
 * Select = false - Chip is disable
 *
 *************************************************************************/
void GLCD_SPI_ChipSelect (Boolean Select);

/*************************************************************************
 * Function Name: LcdSpiSetWordWidth
 * Parameters: Int32U Width
 * Return: Boolean
 *
 * Description: Set SPI word width
 *
 *************************************************************************/
Boolean GLCD_SPI_SetWordWidth (Int32U Width);

/*************************************************************************
 * Function Name: LcdSpiSetClockFreq
 * Parameters: Int32U Frequency
 * Return: Int32U
 *
 * Description: Set SPI clock
 *
 *************************************************************************/
Int32U GLCD_SPI_SetClockFreq (Int32U Frequency);

/*************************************************************************
 * Function Name: GLCD_SPI_Init
 * Parameters: Int32U Clk, Int32U Width
 * Return: none
 *
 * Description: Init SPI
 *
 *************************************************************************/
void GLCD_SPI_Init(Int32U Clk, Int32U Width);

/*************************************************************************
 * Function Name: GLCD_SPI_TranserByte
 * Parameters: Int32U Data
 * Return: Int32U
 *
 * Description: Transfer byte from SPI
 *
 *************************************************************************/
Int32U GLCD_SPI_TranserByte (Int32U Data);

/*************************************************************************
 * Function Name: GLCD_SPI_SendBlock
 * Parameters: pInt8U pData, Int32U Size
 *
 * Return: void
 *
 * Description: Write block of data to SPI
 *
 *************************************************************************/
void GLCD_SPI_SendBlock (pInt8U pData, Int32U Size);

/*************************************************************************
 * Function Name: GLCD_SPI_ReceiveBlock
 * Parameters: pInt8U pData, Int32U Size
 *
 * Return: void
 *
 * Description: Read block of data from SPI
 *
 *************************************************************************/
void GLCD_SPI_ReceiveBlock (pInt8U pData, Int32U Size);

#endif // __GLCD_LL_H
