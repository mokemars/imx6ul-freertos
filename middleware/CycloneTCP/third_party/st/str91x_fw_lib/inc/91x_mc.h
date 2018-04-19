/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : 91x_mc.h
* Author             : MCD Application Team
* Version            : V2.1
* Date               : 12/22/2008
* Description        : This file contains all the functions prototypes for the
*                      MC firmware library.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS
* A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __91x_MC_H
#define __91x_MC_H

/* Includes ------------------------------------------------------------------*/
#include "91x_map.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  u16 MC_OperatingMode;
  u16 MC_TachoMode;
  u16 MC_TachoEvent_Mode;
  u8  MC_Prescaler;
  u8  MC_ForcedPWMState;
  u16 MC_TachoPrescaler;
  u16 MC_PWMMode;
  u16 MC_Complementary;
  u16 MC_Emergency;
  u16 MC_Period;
  u16 MC_TachoPeriod;
  u16 MC_Channel;
  u16 MC_PulseU;
  u16 MC_PulseV;
  u16 MC_PulseW;
  u16 MC_PolarityUL;
  u16 MC_PolarityUH;
  u16 MC_PolarityVL;
  u16 MC_PolarityVH;
  u16 MC_PolarityWL;
  u16 MC_PolarityWH;
  u16 MC_TachoPolarity;
  u16 MC_DeadTime;
  u8  MC_RepetitionCounter;
} MC_InitTypeDef;

typedef enum { DOWN = 0, UP  = !DOWN} CountingStatus;

/* Exported constants --------------------------------------------------------*/

/* MC enable/disable complementary feature */
#define MC_Complementary_Enable   0x0001          /*MC Complementary Mode Enable*/
#define MC_Complementary_Disable  0x0002	  /*MC Complementary Mode Disable*/

/* MC operating modes */
#define MC_HardwareOperating_Mode 0x0001	  /* Hardware operating Mode*/
#define MC_SoftwareOperating_Mode 0x0002	  /* Software operating Mode*/

/* MC PWM modes */
#define MC_PWMClassical_Mode    0x0001		  /*Classical PWM Mode*/
#define MC_PWMZeroCentered_Mode 0x0002            /*Zero Centered PWM Mode*/


/* MC enable/disable emergency */
#define MC_Emergency_Enable  0x0001		 /*MC Emergency Enable*/
#define MC_Emergency_Disable 0x0002		 /*MC Emergency Disable*/


/* MC forced PWM state */
#define MC_Polarity_Inverted     0x0001		/*PWM signal polarity inverted*/
#define MC_Polarity_NonInverted  0x0002		/*PWM signal polarity non-inverted*/


/* MC tacho modes */
#define MC_TachoOneShot_Mode     0x0001		/*One Shot Tacho Mode*/
#define MC_TachoContinuous_Mode  0x0002		/*Continuous Tacho Mode*/

 
/* MC tacho polarity */
#define MC_TachoEventEdge_None           0x0000		/* Edge None */
#define MC_TachoEventEdge_Falling        0x0001		/* Falling Edge  */
#define MC_TachoEventEdge_Rising         0x0002		/* Rising Edge  */
#define MC_TachoEventEdge_RisingFalling  0x0003		/* Rising and falling Edge  */

/* MC channels */
#define MC_Channel_U           0x0001          /* Channel U */
#define MC_Channel_V           0x0002	       /* Channel V */
#define MC_Channel_W           0x0003	       /* Channel W */
#define MC_Channel_ALL         0x0004	      /* All Channels */

/* MC tacho event mode */
#define MC_TachoEvent_Hardware_Mode  0x0001	/*Hardware operating Mode*/
#define MC_TachoEvent_Software_Mode  0x0002     /*Software operating Mode*/

/* MC interrupts */
#define MC_IT_CMPW  0x0001  		/* Compare W interrupt */
#define MC_IT_CMPV  0x0002		/* Compare V interrupt */
#define MC_IT_CMPU  0x0004		/* Compare U interrupt */
#define MC_IT_ZPC   0x0008		/* Zero of PWM counter interrupt */
#define MC_IT_ADT   0x0010		/* Automatic data transfer interrupt */
#define MC_IT_OTC   0x0020		/* Overflow of tacho counter interrupt */
#define MC_IT_CPT   0x0040		/* Capture of tacho counter interrupt */
#define MC_IT_CM0   0x0080		/* Compare 0 interrupt */

/* MC flags */
#define MC_FLAG_CMPW  0x0001		/* Compare W pending bit */
#define MC_FLAG_CMPV  0x0002		/* Compare V pending bit */
#define MC_FLAG_CMPU  0x0004		/* Compare U pending bit */
#define MC_FLAG_ZPC   0x0008		/* Zero of PWM counter pending bit */
#define MC_FLAG_ADT   0x0010		/* Automatic data transfer pending bit */
#define MC_FLAG_OTC   0x0020		/* Overflow of tacho counter pending bit */
#define MC_FLAG_CPT   0x0040		/* Capture of tacho counter pending bit */
#define MC_FLAG_CM0   0x0080		/* Compare 0 pending bit */
#define MC_FLAG_EST   0x0100		/* Emergency stop pending bit */

/* MC lock levels */
#define MC_LockLevel0   0x0001		/* Level0 */
#define MC_LockLevel1   0x0002		/* Level1 */
#define MC_LockLevel2   0x0004		/* Level2 */
#define MC_LockLevel3   0x0008		/* Level3 */
#define MC_LockLevel4   0x0010		/* Level4 */

/* MC counters */
#define  MC_DT_Counter   0x0010    /*Dead time counter*/
#define  MC_PWM_Counter  0x0020	   /*PWM counter*/


/* MC double update mode feature */
#define  MC_DUM          0x0001    /*Double update mode */


/* MC Trigger events to ADC conversion */
#define  MC_ZPC          0x0004		/*Zero of PWM counter trigger event*/
#define  MC_CM0          0x0008		/*Compare 0 of PWM trigger event*/
#define  MC_ADT          0x000C		/*Automatic Data Transfer trigger event */




/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void MC_Init(MC_InitTypeDef* MC_InitStruct);
void MC_DeInit(void);
void MC_StructInit(MC_InitTypeDef* MC_InitStruct);
void MC_Cmd(FunctionalState NewState);
void MC_CtrlPWMOutputs(FunctionalState Newstate);
void MC_ITConfig(u16 MC_IT, FunctionalState NewState);
void MC_SetPrescaler(u8 Prescaler);
void MC_SetPeriod(u16 MC_Period);
void MC_SetPulseU(u16 MC_PulseU);
void MC_SetPulseV(u16 MC_PulseV);
void MC_SetPulseW(u16 MC_PulseW);
void MC_SetTachoCompare(u8 MC_Compare);
void MC_PWMModeConfig(u16 MC_PWMMode);
void MC_SetDeadTime(u16 MC_DeadTime);
void MC_EmergencyCmd(FunctionalState NewState);
void MC_EmergencyClear(void);
u16 MC_GetPeriod(void);
u16 MC_GetPulseU(void);
u16 MC_GetPulseV(void);
u16 MC_GetPulseW(void);
u16 MC_GetTachoCapture(void);
void MC_ClearOnTachoCapture(FunctionalState NewState);
void MC_ForceDataTransfer(u8 MC_ForcedData);
void MC_SoftwarePreloadConfig(void);
void MC_SoftwareTachoCapture(void);
CountingStatus MC_GetCountingStatus(void);
FlagStatus MC_GetFlagStatus(u16 MC_FLAG);
void MC_ClearFlag(u16 MC_FLAG);
ITStatus MC_GetITStatus(u16 MC_IT);
void MC_ClearITPendingBit(u16 MC_IT);
void MC_ClearPWMCounter(void);
void MC_ClearTachoCounter(void);
void MC_Lock(u16 MC_LockLevel);
void MC_CounterModeConfig(u16 MC_Counter);
void MC_DoubleUpdateMode(FunctionalState NewState);
void MC_ADCTrigger(u16 IMC_Event, FunctionalState NewState);
void MC_EnhancedStop(FunctionalState NewState);
void MC_EmergencyStopPolarity(FunctionalState NewState);
void MC_DebugOutputProtection(FunctionalState NewState);

#endif /* __91x_MC_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

