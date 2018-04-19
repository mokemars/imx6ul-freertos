/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : 91x_emi.h
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : This file contains all the functions prototypes for the
*                      EMI firmware library.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __91x_EMI_H
#define __91x_EMI_H

/* Includes ------------------------------------------------------------------*/
#include "91x_map.h"

/* Exported types ------------------------------------------------------------*/
 typedef struct
 {
  u32 EMI_Bank_IDCY;

  u32 EMI_Bank_WSTRD;

  u32 EMI_Bank_WSTWR;

  u32 EMI_Bank_WSTROEN;

  u32 EMI_Bank_WSTWEN;
  
  u32 EMI_Bank_BRDCR;
  
  u32 EMI_Bank_MemWidth ;

  u32  EMI_Bank_WriteProtection;

  u32 EMI_Burst_and_PageModeRead_TransferLength;
  
  u32 EMI_Burst_and_PageModeRead_Selection;
  
  u32 EMI_BurstModeWrite_TransferLength;
  
  u32 EMI_BurstModeWrite_Selection;
 
  u32 EMI_AccessRead_Support;
  
  u32 EMI_AccessWrite_Support;
  
  u32 EMI_ByteLane_Selection;
  
 } EMI_InitTypeDef;

/* Exported constants --------------------------------------------------------*/




/*Transfer length  valid for both burst and page mode*/
#define EMI_Read_4Data	                 0x00000000   /*4 transfers burst*/
#define EMI_Read_8Data	                 0x00000400   /*8 transfers burst*/

/*Transfer length  valid only for burst mode*/
#define EMI_Read_16Data	                 0x00000800   /*16 transfers burst*/
#define EMI_Read_Continuous              0x00000C00   /* Continuous (synchron-*/
                                                      /*ous only)*/

/*Select or deselect the page mode read*/
#define EMI_NormalMode	                 0x00000000   /*Normal Mode*/
#define EMI_Burst_and_PageModeRead       0x00000100   /*Page and burst Mode Read*/


/* Write transfer length for Burst  mode Write */
#define EMI_Write_4Data	                 0x00000000   /*4 transfers burst*/
#define EMI_Write_8Data	                 0x00040000   /*8 transfers burst*/
#define EMI_Write_Continuous             0x000C0000   /* Continuous (synchron-*/
                                                      /*ous only)*/

/*Select or deselect the Burst(or page) mode Write*/
#define EMI_NonBurstModeWrite            0x00000000   /*Non Burst Mode Write*/
#define EMI_BurstModeWrite               0x00010000   /*Burst Mode Write*/

/*Byte Lane feature */
#define EMI_Byte_Select_disabled	 0x00000000   /*Byte select disabled*/
#define EMI_Byte_Select_enabled		 0x00000001   /*Byte select enabled*/

/*Read access feature*/
#define EMI_Read_Asyn	                 0x00000000   /*Asynch access for read default)*/
#define EMI_Read_Syn	                 0x00000200   /*Synch access for read*/
                                     

/*Write access feature*/
#define EMI_Write_Asyn	                 0x00000000   /*Asynch access for Write(default)*/
#define EMI_Write_Syn	                 0x00020000   /*Synch access for Write*/
                                      

/*Memory width*/
#define EMI_Width_Byte	                 0x00000000   /*8 bits width*/
#define EMI_Width_HalfWord	         0x00000010   /*16 bits width*/

/*Write protection feature */
#define EMI_Bank_NonWriteProtect	 0x00000000    /*No write protection*/
#define EMI_Bank_WriteProtect	         0x00000008    /*bank is write protected*/


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void EMI_DeInit(void);
void EMI_Init( EMI_Bank_TypeDef* EMI_Bankx, EMI_InitTypeDef* EMI_InitStruct);
void EMI_StructInit(EMI_InitTypeDef* EMI_InitStruct);
void EMI_BCLKCmd(FunctionalState NewState);

#endif /* __EMI_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
