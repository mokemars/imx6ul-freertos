/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : 91x_fmi.h
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : This file contains all the functions prototypes for the
*                      FMI firmware library.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS
* A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


/* Define to prevent recursive inclusion ------------------------------------ */

#ifndef __91x_FMI_H
#define __91x_FMI_H



/* Includes ------------------------------------------------------------------*/

#include "91x_map.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* FMI banks */

#ifdef Boot_Bank_1            /* Boot from Bank 1 */


#define    FMI_BANK_0     ((*(vu32*)0x54000010) << 2)   /* FMI Bank 0 */
#define    FMI_BANK_1     ((*(vu32*)0x5400000C) << 2)   /* FMI Bank 1 */

#endif

#ifdef Boot_Bank_0            /* Boot from Bank 0 */

#define    FMI_BANK_0     ((*(vu32*)0x5400000C) << 2)   /* FMI Bank 0 */
#define    FMI_BANK_1     ((*(vu32*)0x54000010) << 2)   /* FMI Bank 1 */

#endif


#define    FMI_B0S0     0x00000000 + FMI_BANK_0     /* Bank 0 sector 0*/
#define    FMI_B0S1     0x00010000 + FMI_BANK_0     /* Bank 0 sector 1*/
#define    FMI_B0S2     0x00020000 + FMI_BANK_0     /* Bank 0 sector 2*/
#define    FMI_B0S3	0x00030000 + FMI_BANK_0     /* Bank 0 sector 3*/
#define    FMI_B0S4     0x00040000 + FMI_BANK_0     /* Bank 0 sector 4*/
#define    FMI_B0S5     0x00050000 + FMI_BANK_0     /* Bank 0 sector 5*/
#define    FMI_B0S6     0x00060000 + FMI_BANK_0     /* Bank 0 sector 6*/
#define    FMI_B0S7     0x00070000 + FMI_BANK_0     /* Bank 0 sector 7*/
#define    FMI_B0S8     0x00080000 + FMI_BANK_0     /* Bank 0 sector 8*/
#define    FMI_B0S9     0x00090000 + FMI_BANK_0     /* Bank 0 sector 9*/
#define    FMI_B0S10    0x000A0000 + FMI_BANK_0     /* Bank 0 sector 10*/
#define    FMI_B0S11	0x000B0000 + FMI_BANK_0     /* Bank 0 sector 11*/
#define    FMI_B0S12    0x000C0000 + FMI_BANK_0     /* Bank 0 sector 12*/
#define    FMI_B0S13    0x000D0000 + FMI_BANK_0     /* Bank 0 sector 13*/
#define    FMI_B0S14    0x000E0000 + FMI_BANK_0     /* Bank 0 sector 14*/
#define    FMI_B0S15    0x000F0000 + FMI_BANK_0     /* Bank 0 sector 15*/
#define    FMI_B0S16    0x00100000 + FMI_BANK_0     /* Bank 0 sector 16*/
#define    FMI_B0S17    0x00110000 + FMI_BANK_0     /* Bank 0 sector 17*/
#define    FMI_B0S18    0x00120000 + FMI_BANK_0     /* Bank 0 sector 18*/
#define    FMI_B0S19    0x00130000 + FMI_BANK_0     /* Bank 0 sector 19*/
#define    FMI_B0S20    0x00140000 + FMI_BANK_0     /* Bank 0 sector 20*/
#define    FMI_B0S21    0x00150000 + FMI_BANK_0     /* Bank 0 sector 21*/
#define    FMI_B0S22    0x00160000 + FMI_BANK_0     /* Bank 0 sector 22*/
#define    FMI_B0S23    0x00170000 + FMI_BANK_0     /* Bank 0 sector 23*/
#define    FMI_B0S24    0x00180000 + FMI_BANK_0     /* Bank 0 sector 24*/
#define    FMI_B0S25    0x00190000 + FMI_BANK_0     /* Bank 0 sector 25*/
#define    FMI_B0S26    0x001A0000 + FMI_BANK_0     /* Bank 0 sector 26*/
#define    FMI_B0S27	0x001B0000 + FMI_BANK_0     /* Bank 0 sector 27*/
#define    FMI_B0S28    0x001C0000 + FMI_BANK_0     /* Bank 0 sector 28*/
#define    FMI_B0S29    0x001D0000 + FMI_BANK_0     /* Bank 0 sector 29*/
#define    FMI_B0S30    0x001E0000 + FMI_BANK_0     /* Bank 0 sector 30*/
#define    FMI_B0S31    0x001F0000 + FMI_BANK_0     /* Bank 0 sector 31*/


#ifdef Flash_2MB_1MB

#define    FMI_B1S0     0x00000000 + FMI_BANK_1     /* Bank 1 sector 0 */
#define    FMI_B1S1     0x00004000 + FMI_BANK_1     /* Bank 1 sector 1 */
#define    FMI_B1S2     0x00008000 + FMI_BANK_1     /* Bank 1 sector 2 */
#define    FMI_B1S3     0x0000C000 + FMI_BANK_1     /* Bank 1 sector 3 */
#define    FMI_B1S4     0x00010000 + FMI_BANK_1     /* Bank 1 sector 4 */
#define    FMI_B1S5     0x00014000 + FMI_BANK_1     /* Bank 1 sector 5 */
#define    FMI_B1S6     0x00018000 + FMI_BANK_1     /* Bank 1 sector 6 */
#define    FMI_B1S7     0x0001C000 + FMI_BANK_1     /* Bank 1 sector 7 */

#endif

#ifdef Flash_512KB_256KB

#define    FMI_B1S0     0x00000000 + FMI_BANK_1     /* Bank 1 sector 0 */
#define    FMI_B1S1     0x00002000 + FMI_BANK_1     /* Bank 1 sector 1 */
#define    FMI_B1S2     0x00004000 + FMI_BANK_1     /* Bank 1 sector 2 */
#define    FMI_B1S3     0x00006000 + FMI_BANK_1     /* Bank 1 sector 3 */

#endif



/* FMI Flags */

#define    FMI_FLAG_SPS        0x02       /* Sector Protection Status Flag */
#define    FMI_FLAG_PSS        0x04       /* Program Suspend Status Flag   */
#define    FMI_FLAG_PS         0x10       /* Program Status Flag           */
#define    FMI_FLAG_ES         0x20       /* Erase Status Flag             */
#define    FMI_FLAG_ESS        0x40       /* Erase Suspend Status Flag     */
#define    FMI_FLAG_PECS       0x80       /* FPEC Status Flag              */

/* FMI read wait states */

#define    FMI_READ_WAIT_STATE_1     0x0000    /* One read wait state    */
#define    FMI_READ_WAIT_STATE_2     0x2000    /* Two read wait states   */
#define    FMI_READ_WAIT_STATE_3     0x4000    /* Three read wait states */

/* FMI write wait states */

#define    FMI_WRITE_WAIT_STATE_0     0xFFFFFEFF    /* Zero wait state */
#define    FMI_WRITE_WAIT_STATE_1     0x00000100    /* One wait state  */

/* FMI power down configuration */

#define    FMI_PWD_ENABLE       0x1000    /* FMI Power Down Enable  */
#define    FMI_PWD_DISABLE      0x0000    /* FMI Power Down Disable */

/* FMI low voltage detector */

#define    FMI_LVD_ENABLE       0x0000    /* FMI Low Voltage Detector Enable  */
#define    FMI_LVD_DISABLE      0x0800    /* FMI Low Voltage Detector Disable */

/* FMI frequency range */

#define    FMI_FREQ_LOW         0x0000    /* FMI Low bus working frequency   */
#define    FMI_FREQ_HIGH        0x0040    /* FMI High bus working gfrequency */
                                          /* Above 66 MHz*/
/* FMI OTP word addresses */      

#define    FMI_OTP_WORD_0       0x00   /* OTP word 0 */
#define    FMI_OTP_WORD_1       0x04   /* OTP word 1 */
#define    FMI_OTP_WORD_2       0x08   /* OTP word 2 */
#define    FMI_OTP_WORD_3       0x0C   /* OTP word 3 */
#define    FMI_OTP_WORD_4       0x10   /* OTP word 4 */
#define    FMI_OTP_WORD_5       0x14   /* OTP word 5 */
#define    FMI_OTP_WORD_6       0x18   /* OTP word 6 */
#define    FMI_OTP_WORD_7       0x1C   /* OTP word 7 */
                                    
/* FMI OTP halfword addresses */

#define    FMI_OTP_LOW_HALFWORD_0       0x00   /* OTP Low halfword 0  */
#define    FMI_OTP_HIGH_HALFWORD_0      0x02   /* OTP High halfword 0 */
#define    FMI_OTP_LOW_HALFWORD_1       0x04   /* OTP Low halfword 1  */
#define    FMI_OTP_HIGH_HALFWORD_1      0x06   /* OTP High halfword 1 */
#define    FMI_OTP_LOW_HALFWORD_2       0x08   /* OTP Low halfword 2  */
#define    FMI_OTP_HIGH_HALFWORD_2      0x0A   /* OTP High halfword 2 */
#define    FMI_OTP_LOW_HALFWORD_3       0x0C   /* OTP Low halfword 3  */
#define    FMI_OTP_HIGH_HALFWORD_3      0x0E   /* OTP High halfword 3 */
#define    FMI_OTP_LOW_HALFWORD_4       0x10   /* OTP Low halfword 4  */
#define    FMI_OTP_HIGH_HALFWORD_4      0x12   /* OTP High halfword 4 */
#define    FMI_OTP_LOW_HALFWORD_5       0x14   /* OTP Low halfword 5  */
#define    FMI_OTP_HIGH_HALFWORD_5      0x16   /* OTP High halfword 5 */
#define    FMI_OTP_LOW_HALFWORD_6       0x18   /* OTP Low halfword 6  */
#define    FMI_OTP_HIGH_HALFWORD_6      0x1A   /* OTP High halfword 6 */
#define    FMI_OTP_LOW_HALFWORD_7       0x1C   /* OTP Low halfword 7  */
#define    FMI_OTP_HIGH_HALFWORD_7      0x1E   /* OTP High halfword 7 */

/*FMI LSB RSIG Address*/

#define    FMI_ReadRSIGData_0           0x00    /*Manufacturer Code*/
#define    FMI_ReadRSIGData_1           0x01    /*Device Code*/
#define    FMI_ReadRSIGData_2           0x02    /*Die Revision Code*/
#define    FMI_ReadRSIGData_3           0x03    /*Protection Level 2 Register for 512KB Flash */
                                                /*or Protection Level 1 Register (sectors of bank0)*/
                                                /* for 2MB flash*/

#define    FMI_ReadRSIGData_4           0x04    /*Protection Level 1 Register for 512KB Flash*/
                                                /* or Protection Level 1 Register (sectors of bank1)*/
                                                /* for 2MB flash*/

#define    FMI_ReadRSIGData_5           0x05    /*Protection Status Register(sectors of bank0)*/
                                                /*for 2MB flash or Flash Configuration Register*/
                                                /*for 512KB flash*/
#define    FMI_ReadRSIGData_6           0x06    /*Protection Status Register (sectors of bank1)*/
                                                /*available only for 2MB flash*/
#define    FMI_ReadRSIGData_7           0x07    /*Flash Configuration Register*/
                                                /*available only for 2MB flash*/
                    

/* Timeout error */

#define FMI_TIME_OUT_ERROR      0x00       /* Timeout error    */     
#define FMI_NO_TIME_OUT_ERROR   0x01       /* No Timeout error */

/* Module private variables --------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void FMI_BankRemapConfig(u8 FMI_BootBankSize, u8 FMI_NonBootBankSize, \
                         u32 FMI_BootBankAddress, u32 FMI_NonBootBankAddress);
void FMI_Config(u16 FMI_ReadWaitState, u32 FMI_WriteWaitState, u16 FMI_PWD,\
                u16 FMI_LVDEN, u16 FMI_FreqRange);
void FMI_EraseSector(vu32 FMI_Sector);
void FMI_EraseBank(vu32 FMI_Bank);
void FMI_WriteHalfWord(u32 FMI_Address, u16 FMI_Data);
void FMI_WriteOTPHalfWord(u8 FMI_OTPHWAddress, u16 FMI_OTPData);
u32 FMI_ReadWord(u32 FMI_Address);
u32 FMI_ReadOTPData(u8 FMI_OTPAddress);
FlagStatus FMI_GetFlagStatus(u8 FMI_Flag, vu32 FMI_Bank);
u16 FMI_GetReadWaitStateValue(void);
u16 FMI_GetWriteWaitStateValue(void);
void FMI_SuspendEnable(vu32 FMI_Bank);
void FMI_ResumeEnable(vu32 FMI_Bank);
void FMI_ClearFlag(vu32 FMI_Bank);
void FMI_WriteProtectionCmd(vu32 FMI_Sector, FunctionalState FMI_NewState);
u8 FMI_WaitForLastOperation(vu32 FMI_Bank);
u32 FMI_ReadRSIGData(u8 FMI_LSB_RSIGAddress);

#endif /* __91x_FMI_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

