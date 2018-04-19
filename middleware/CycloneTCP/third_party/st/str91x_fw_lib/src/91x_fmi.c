/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : 91x_fmi.c
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : This file provides all the FMI firmware functions.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS
* A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


/* Standard include ----------------------------------------------------------*/
#include "91x_fmi.h"

/* Include of other module interface headers ---------------------------------*/
/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define TIMEOUT      0xFFFFFF     /* Timeout value */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Interface functions -------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : FMI_BankRemapConfig
* Description    : Configure the addresses and sizes of bank 0 and bank 1. 
* Input1         : FMI_BootBankSize: specifies the boot bank size.
*                  This parameter can be one of the following values:
*                     - 0x0: 32KBytes.
*                     - 0x1: 64KBytes.
*                     - 0x2: 128KBytes.
*                     - 0x3: 256KBytes.
*                     - 0x4: 512KBytes.
*                     ....
*                     - 0xB: 64MBytes.
* Input2         : FMI_NonBootBankSize: specifies the non boot bank size.
*                  This parameter can be one of the following values:
*                     - 0x0: 8KBytes.
*                     - 0x1: 16KBytes.
*                     - 0x2: 32KBytes.
*                     - 0x3: 64KBytes.
*                     ....
*                     - 0xD: 64MBytes.
* Input3         : FMI_BootBankAddress: specifies the address of the boot bank.
* Input4         : FMI_NonBootBankAddress: specifies the address of the non 
*                  boot bank.
* Output         : None
* Return         : None
*******************************************************************************/
void FMI_BankRemapConfig(u8 FMI_BootBankSize, u8 FMI_NonBootBankSize, \
                         u32 FMI_BootBankAddress, u32 FMI_NonBootBankAddress)
{
    FMI->BBSR   = FMI_BootBankSize;
    FMI->NBBSR  = FMI_NonBootBankSize;
    FMI->BBADR  = (FMI_BootBankAddress >> 2);
    FMI->NBBADR = (FMI_NonBootBankAddress >> 2);
    FMI->CR     |= 0x18; /* Enable bank 1 */
}

/*******************************************************************************
* Function Name  : FMI_Config
* Description    : Configure the FMI.
* Input1         : FMI_ReadWaitState: specifies the needed read wait states.
*                  This parameter can be one of the following values:
*                     - FMI_READ_WAIT_STATE_1: One read wait state.
*                     - FMI_READ_WAIT_STATE_2: Two read wait states.
*                     - FMI_READ_WAIT_STATE_3: Three read wait states.
* Input2         : FMI_WriteWaitState: specifies the needed write wait states.
*                  This parameter can be one of the following values:
*                     - FMI_WRITE_WAIT_STATE_1: One write wait state.
*                     - FMI_WRITE_WAIT_STATE_2: Two write wait states.
* Input3         : FMI_PWD: specifies the power down mode status.
*                  This parameter can be one of the following values:
*                     - FMI_PWD_ENABLE:  Enable the PWD.
*                     - FMI_PWD_DISABLE: Disable the PWD.
* Input4         : FMI_LVDEN: specifies the low voltage detector status.
*                  This parameter can be one of the following values:
*                     - FMI_LVD_ENABLE:  Enable the LVD.
*                     - FMI_LVD_DISABLE: Disable the LVD.
* Input5         : FMI_FreqRange: specifies the working frequency range.
*                  This parameter can be one of the following values:
*                     - FMI_FREQ_LOW:  Low working frequency (up to 66MHz).
*                     - FMI_FREQ_HIGH: High working frequency (above 66MHz) .
* Output         : None
* Return         : None
*
*NOTE:
*This function should be executed from SRAM when booting from bank1 
*to avoid any conflicts(reading and writing at the same time in bank1)
*
*******************************************************************************/
void FMI_Config(u16 FMI_ReadWaitState, u32 FMI_WriteWaitState, u16 FMI_PWD,\
                u16 FMI_LVDEN, u16 FMI_FreqRange)
{
  /* Configure the write wait state value */
  if (FMI_WriteWaitState == FMI_WRITE_WAIT_STATE_1)
  {
    FMI->CR |= FMI_WRITE_WAIT_STATE_1;
  }
  else
  {
    FMI->CR &= FMI_WRITE_WAIT_STATE_0;
  }

  /* Write a write flash configuration register command */
  *(vu16 *)FMI_BANK_1 = 0x60;

  /* Configure the flash configuration register */
  *(vu16 *)(FMI_BANK_1|FMI_ReadWaitState|FMI_PWD|FMI_LVDEN|FMI_FreqRange) = 0x03;
}

/*******************************************************************************
* Function Name  : FMI_EraseSector
* Description    : Erase the needed sector.
* Input          : FMI_Sector: specifies the sector to be erased.  
*                  This parameter can be one of the following values:
*                     - FMI_B0S0: FMI bank 0 sector 0. 
*                       ...
*                     - FMI_B0S31: FMI bank 0 sector 31.
*
*
*                     - FMI_B1S0: FMI bank 1 sector 0.
*                      ...
*                     - FMI_B1S7: FMI bank 1 sector 7.
* Output         : None
* Return         : None
*******************************************************************************/
void FMI_EraseSector(vu32 FMI_Sector)
{ 
  /* Write an erase set-up command to the sector */
  *(vu16 *)FMI_Sector = 0x20;

  /* Write an erase confirm command to the sector */
  *(vu16 *)FMI_Sector = 0xD0;
}

/*******************************************************************************
* Function Name  : FMI_EraseBank
* Description    : Erase the needed bank.
* Input          : FMI_Bank: specifies the bank to be erased.
*                  This parameter can be one of the following values:
*                     - FMI_BANK_0: FMI bank 0.
*                     - FMI_BANK_1: FMI bank 1.
* Output         : None
* Return         : None
*******************************************************************************/
void FMI_EraseBank(vu32 FMI_Bank)
{
  /* Write a bank erase set-up command to the bank */
  *(vu16 *)FMI_Bank = 0x80;

  /* Write an erase confirm command to the sector */
  *(vu16 *)FMI_Bank = 0xD0;
}

/*******************************************************************************
* Function Name  : FMI_WriteHalfWord
* Description    : Write a halfword to the needed Flash memory address.
* Input 1        : FMI_Address: specifies the address offset where the data will 
*                  be written.
* Input 2        : FMI_Data: the needed data.
* Output         : None
* Return         : None
*******************************************************************************/
void FMI_WriteHalfWord(u32 FMI_Address, u16 FMI_Data)
{
  /* Write a program command to the sector to be written */
  *(vu16 *)(FMI_Address & 0xFFFFFFFC) = 0x40;
  
  /* Write the halfword to the destination address */
  *(vu16 *)FMI_Address = FMI_Data;
}

/*******************************************************************************
* Function Name  : FMI_WriteOTPHalfWord
* Description    : Write a halfword to the needed OTP sector address.
* Input 1        : FMI_OTPHWAddress: specifies the halfword address offset  
*                  where the data will be written.
*                  This parameter can be one of the following values:
*                     - FMI_OTP_LOW_HALFWORD_0: OTP Low halfword 0.
*                     - FMI_OTP_HIGH_HALFWORD_0: OTP High halfword 0.
*                     - FMI_OTP_LOW_HALFWORD_1: OTP Low halfword 1.
*                     - FMI_OTP_HIGH_HALFWORD_1: OTP High halfword 1.
*                     - FMI_OTP_LOW_HALFWORD_2: OTP Low halfword 2.
*                     - FMI_OTP_HIGH_HALFWORD_2: OTP High halfword 2.
*                     - FMI_OTP_LOW_HALFWORD_3: OTP Low halfword 3.
*                     - FMI_OTP_HIGH_HALFWORD_3: OTP High halfword 3.
*                     - FMI_OTP_LOW_HALFWORD_4: OTP Low halfword 4.
*                     - FMI_OTP_HIGH_HALFWORD_4: OTP High halfword 4.
*                     - FMI_OTP_LOW_HALFWORD_5: OTP Low halfword 5.
*                     - FMI_OTP_HIGH_HALFWORD_5: OTP High halfword 5.
*                     - FMI_OTP_LOW_HALFWORD_6: OTP Low halfword 6.
*                     - FMI_OTP_HIGH_HALFWORD_6: OTP High halfword 6.
*                     - FMI_OTP_LOW_HALFWORD_7: OTP Low halfword 7.
*                     - FMI_OTP_HIGH_HALFWORD_7: OTP High halfword 7.
* Input 2        : FMI_OTPData: The needed OTP data.
* Output         : None
* Return         : None
*******************************************************************************/
void FMI_WriteOTPHalfWord(u8 FMI_OTPHWAddress, u16 FMI_OTPData)
{
  /* Write a write OTP command to the needed address */
  *(vu16 *)(FMI_BANK_1) = 0xC0;
  
  /* Write the halfword to the destination address */
  *(vu16 *)(FMI_BANK_1 + FMI_OTPHWAddress) = FMI_OTPData;
}

/*******************************************************************************
* Function Name  : FMI_ReadWord
* Description    : Read the correspondent data.
* Input          : FMI_Address: specifies the needed address.
* Output         : None
* Return         : The data contained in the specified address.
*******************************************************************************/
u32 FMI_ReadWord(u32 FMI_Address)
{
  return(*(u32*)FMI_Address);
}

/*******************************************************************************
* Function Name  : FMI_ReadOTPData
* Description    : Read data from the OTP sector.
* Input          : FMI_OTPAddress: specifies the address of the data to be read.
*                  This parameter can be one of the following values:
*                     - FMI_OTP_WORD_0:   OTP word 0 . 
*                     - FMI_OTP_WORD_1:   OTP word 1 .
*                     - FMI_OTP_WORD_2:   OTP word 2 .
*                     - FMI_OTP_WORD_3:   OTP word 3 .
*                     - FMI_OTP_WORD_4:   OTP word 4 .
*                     - FMI_OTP_WORD_5:   OTP word 5 .
*                     - FMI_OTP_WORD_6:   OTP word 6 . 
*                     - FMI_OTP_WORD_7:   OTP word 7 .
* Output         : None
* Return         : The needed OTP words.
*******************************************************************************/
u32 FMI_ReadOTPData(u8 FMI_OTPAddress)
{
  u32 OTP_Data = 0x0;
  /* write a read OTP sector command */
  *(vu16 *)(FMI_BANK_1) = 0x98;
  
  /* Read the correspondent data */
  OTP_Data = (*(vu32*)(FMI_BANK_1 + FMI_OTPAddress));

  /* Write a read array command */
  *(vu16 *)(FMI_BANK_1) = 0xFF;

  return OTP_Data;
}

/*******************************************************************************
* Function Name  : FMI_GetFlagStatus
* Description    : Check whether the specified FMI flag is set or not.
* Input1         : FMI_Flag: flag to check.
*                  This parameter can be one of the following values:
*                     - FMI_FLAG_SPS: Sector Protection Status Flag.
*                     - FMI_FLAG_PSS: Program Suspend Status Flag.
*                     - FMI_FLAG_PS: Program Status Flag.
*                     - FMI_FLAG_ES: Erase Status Flag.
*                     - FMI_FLAG_ESS: Erase Suspend Status Flag.
*                     - FMI_FLAG_PECS: FPEC Status Flag.
* Input2         : FMI_Bank: specifies the needed bank.
*                  This parameter can be one of the following values:
*                     - FMI_BANK_0: FMI bank 0.
*                     - FMI_BANK_1: FMI bank 1.
* Output         : None
* Return         : None
*******************************************************************************/
FlagStatus FMI_GetFlagStatus(u8 FMI_Flag, vu32 FMI_Bank)
{    
  u16 FMI_Status_Register = 0;

  /* Write a read status register command */
  *(vu16 *)FMI_Bank = 0x70;

  /* Wait until operation completion */
  while(!((*(vu16 *)FMI_Bank) & 0x80));

  /* Read the status register */
  FMI_Status_Register = *(vu16 *)FMI_Bank;

  /* Write a read array command */
  *(vu16 *)FMI_Bank = 0xFF;
   
  if((FMI_Status_Register & FMI_Flag) != RESET)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/*******************************************************************************
* Function Name  : FMI_GetReadWaitStateValue
* Description    : Get the current Read wait state value.
* Input          : None
* Output         : None
* Return         : The current read wait states value.
*******************************************************************************/
u16 FMI_GetReadWaitStateValue(void)
{ 
  u16 FMI_Configuration_Register = 0;
  /*Write a read RSIG command to any word address in Bank1*/
  *(vu16 *)FMI_BANK_1 = 0x90;

  /* Read the flash configuration register */
#ifdef Flash_512KB_256KB
  FMI_Configuration_Register = *(vu16 *)(FMI_BANK_1 + 0x14);
#endif

#ifdef  Flash_2MB_1MB
  FMI_Configuration_Register = *(vu16 *)(FMI_BANK_1 + 0x1C);
#endif
  /* Write a read array command */
  *(vu16 *)FMI_BANK_1 = 0xFF;

  FMI_Configuration_Register = ((FMI_Configuration_Register >> 11) + 1) & 0x3;

  /* Return the wait states value */
  return FMI_Configuration_Register;
}

/*******************************************************************************
* Function Name  : FMI_GetWriteWaitStateValue
* Description    : Get the current write wait state value.
* Input          : None
* Output         : None
* Return         : The current write wait states value.
*******************************************************************************/
u16 FMI_GetWriteWaitStateValue(void)
{
  return ((u16)((FMI->CR & 0x100) >> 8));
}

/*******************************************************************************
* Function Name  : FMI_SuspendEnable
* Description    : Suspend command enable.
* Input          : FMI_Bank: specifies the bank to be suspended.
*                  This parameter can be one of the following values:
*                     - FMI_BANK_0: FMI bank 0.
*                     - FMI_BANK_1: FMI bank 1.
* Output         : None
* Return         : None
*******************************************************************************/
void FMI_SuspendEnable(vu32 FMI_Bank)
{
  /* Write a suspend command to the bank */
  *(vu16 *)FMI_Bank = 0xB0;
}

/*******************************************************************************
* Function Name  : FMI_ResumeEnable
* Description    : Resume the suspended command.
* Input          : FMI_Bank: specifies the suspended bank.
*                  This parameter can be one of the following values:
*                     - FMI_BANK_0: FMI bank 0.
*                     - FMI_BANK_1: FMI bank 1.
* Output         : None
* Return         : None
*******************************************************************************/
void FMI_ResumeEnable(vu32 FMI_Bank)
{
  /* Write a resume command to the bank */
  *(vu16 *)FMI_Bank = 0xD0;
}

/*******************************************************************************
* Function Name  : FMI_ClearFlag
* Description    : Clear the FMI Flags on the correspondent bank.
* Input          : FMI_Bank: specifies the needed bank.
*                  This parameter can be one of the following values:
*                     - FMI_BANK_0: FMI bank 0.
*                     - FMI_BANK_1: FMI bank 1.
* Output         : None
* Return         : None
*******************************************************************************/
void FMI_ClearFlag(vu32 FMI_Bank)
{
  /* Write a clear status register command */
  *(vu16 *)FMI_Bank = 0x50;
}

/*******************************************************************************
* Function Name  : FMI_WriteProtectionCmd
* Description    : Enable or disable the write protection for the needed sector.
* Input1         : FMI_Sector: specifies the sector to be protected or   
*                  unprotected. 
*                  This parameter can be one of the following values:
*
*                     - FMI_B0S0:  FMI bank 0 sector 0. 
*                       ...
*                     - FMI_B0S31: FMI bank 0 sector 31.
*
*
*                     - FMI_B1S0: FMI bank 1 sector 0.
*                      ...
*                     - FMI_B1S7: FMI bank 1 sector 7.
*
* Input2         : FMI_NewState: specifies the protection status.
*                  This parameter can be one of the following values:
*                     - ENABLE:  Enable the protection.
*                     - DISABLE: Disable the protection.
* Output         : None
* Return         : None
*******************************************************************************/
void FMI_WriteProtectionCmd(vu32 FMI_Sector, FunctionalState FMI_NewState)
{
  if (FMI_NewState == ENABLE)
  {
    *(vu16*)FMI_Sector = 0x60;
    *(vu16*)FMI_Sector = 0x01;
    *(vu16*)FMI_Sector = 0xFF;
  }
  else /* DISABLE */
  {
    *(vu16*)FMI_Sector = 0x60;
    *(vu16*)FMI_Sector = 0xD0;
    *(vu16*)FMI_Sector = 0xFF;
  }
}


/*******************************************************************************
* Function Name  : FMI_WaitForLastOperation
* Description    : Wait until the last operation (Write halfword, Write OTP 
*                  halfword, Erase sector and Erase bank) completion.
* Input          : FMI_Bank: specifies the bank where the operation is on going.
*                  This parameter can be one of the following values:
*                     - FMI_BANK_0: FMI bank 0.
*                     - FMI_BANK_1: FMI bank 1.
* Output         : None
* Return         : The timeout status.
*                  This parameter can be one of the following values:
*                     - FMI_TIME_OUT_ERROR: Timeout error occurred.
*                     - FMI_NO_TIME_OUT_ERROR: No timeout error.
*******************************************************************************/
u8 FMI_WaitForLastOperation(vu32 FMI_Bank)
{
  u32 Time_Out = 0;
 
  /* Write a read status register command */
  *(vu16 *)(FMI_Bank) = 0x70;

  /* Wait until operation compeletion */
  while((!((*(vu16 *)FMI_Bank) & 0x80))&&(Time_Out < TIMEOUT ))
  {
    Time_Out ++;  /* Time Out */
  }

  /* Write a read array command */
  *(vu16 *)FMI_Bank = 0xFF;
 
  if (Time_Out == TIMEOUT)
  {
    return FMI_TIME_OUT_ERROR;
  }
  else
  {
    return FMI_NO_TIME_OUT_ERROR;
  }
}

/*******************************************************************************
* Function Name  : FMI_ReadRSIGData
* Description    : Read the Electronic Signature stored in the user configuration 
*                  sector of Bank 1.
* Input          :  FMI_LSB_RSIGAddress: specifies the low byte of the address
*                   to select the register.
*                  This parameter can be one of the following values:
*                     - FMI_ReadRSIGData_0. 
*                     - FMI_ReadRSIGData_1. 
*                     - FMI_ReadRSIGData_2. 
*                     - FMI_ReadRSIGData_3. 
*                     - FMI_ReadRSIGData_4. 
*                     - FMI_ReadRSIGData_5.
*                     - FMI_ReadRSIGData_6. 
*                     - FMI_ReadRSIGData_7. 
*                     
* Output         : None
* Return         : The needed RSIG data.
*******************************************************************************/

u32 FMI_ReadRSIGData(u8 FMI_LSB_RSIGAddress)
{
  u32 RSIG_Data = 0x0;
  
  /*Write a read RSIG command to any word address in Bank1*/ 
  *(vu16 *)(FMI_BANK_1) = 0x90; 
  
  /*Read any RSIG register from any address in Bank1*/ 
  RSIG_Data = (*(vu32*)(FMI_BANK_1 + (FMI_LSB_RSIGAddress<<2))); 

  /*write a Read Array command (FFh) to any word address in Bank 1 to*/
  /*return it to Read Array mode.*/
  *(vu16 *)FMI_BANK_1 = 0xFF; 

  return RSIG_Data;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
