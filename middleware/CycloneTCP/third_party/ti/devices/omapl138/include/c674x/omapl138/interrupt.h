/**
 * \file  interrupt.h
 *
 * \brief Contains DSP interrupt related API declarations and system interrupt
 *        event numbers. (Note: event numbers are device-specific.)
 */

/*
* Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/ 
*
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
** 							INTERNAL TYPE DEFINITIONS
******************************************************************************/
typedef void (*c674xISR)(void);


/******************************************************************************
** 							EXTERNAL MACRO DEFINITIONS
******************************************************************************/
/* API Parameter: cpuINT, ecmINTx
 *
 * Brief: Used to select DSP CPU maskable interrupt.
 *
 * Functions:
 *		IntRegister
 *		IntUnRegister
 *		IntEventMap
 *		IntEventCombineInit
 *		IntEnable
 *		IntDisable */
#define C674X_MASK_INT4 4
#define C674X_MASK_INT5 5
#define C674X_MASK_INT6 6
#define C674X_MASK_INT7 7
#define C674X_MASK_INT8 8
#define C674X_MASK_INT9 9
#define C674X_MASK_INT10 10
#define C674X_MASK_INT11 11
#define C674X_MASK_INT12 12
#define C674X_MASK_INT13 13
#define C674X_MASK_INT14 14
#define C674X_MASK_INT15 15
/* Functions:
 *		IntRegister
 *		IntUnRegister  */
#define C674X_MASK_NMI 1
#define C674X_MASK_EXC 1
#define C674X_MASK_RSV2 2
#define C674X_MASK_RSV3 3

/* API Parameter: ecmINTx ONLY
 *
 * Brief: Used to specify unused Event Combiner system events
 *
 * Functions:
 * 		IntEventCombineInit */
#define ECM0_UNUSED -1
#define ECM1_UNUSED -1
#define ECM2_UNUSED -1
#define ECM3_UNUSED -1

/* API Parameter: sysINT
 *
 * Brief: Used to select system interrupts.
 *
 * Functions:
 * 		IntEventMap
 * 		IntEventSet
 * 		IntEventClear
 * 		IntEventCombineAdd
 * 		IntEventCombineRemove
 * 		IntEventCombineRegister
 * 		ExcCombineAdd
 * 		ExcCombineRemove */
 
/*			Event Combiners */
#define SYS_INT_EVT0 				0
#define SYS_INT_EVT1 				1
#define SYS_INT_EVT2 				2
#define SYS_INT_EVT3 				3

/*			Timer Interrupts */
#define SYS_INT_T64P0_TINT12 		4
#define SYS_INT_T64P0_TINT34 		64
#define SYS_INT_T64P1_TINT12 		40
#define SYS_INT_T64P1_TINT34 		48
#define SYS_INT_T64P2_TINTALL 		25
#define SYS_INT_T64P3_TINTALL 		86

#define SYS_INT_T64P2_CMPINT0 		78
#define SYS_INT_T64P2_CMPINT1 		79
#define SYS_INT_T64P2_CMPINT2 		80
#define SYS_INT_T64P2_CMPINT3 		81
#define SYS_INT_T64P2_CMPINT4 		82
#define SYS_INT_T64P2_CMPINT5 		83
#define SYS_INT_T64P2_CMPINT6 		84
#define SYS_INT_T64P2_CMPINT7 		85

/*			Syscfg Interrupts */
#define SYS_INT_SYSCFG_CHIPINT2 	5
#define SYS_INT_SYSCFG_CHIPINT3 	67
#define SYS_INT_PROTERR 			74

/*			PRU Interrupts */
#define SYS_INT_PRU_EVTOUT0 		6
#define SYS_INT_PRU_EVTOUT1 		17
#define SYS_INT_PRU_EVTOUT2 		22
#define SYS_INT_PRU_EVTOUT3 		35
#define SYS_INT_PRU_EVTOUT4 		66
#define SYS_INT_PRU_EVTOUT5 		39
#define SYS_INT_PRU_EVTOUT6 		44
#define SYS_INT_PRU_EVTOUT7 		50

/*			EHRPWM Interrupts */
#define SYS_INT_EHRPWM0 			7
#define SYS_INT_EHRPWM0TZ 			10
#define SYS_INT_EHRPWM1 			18
#define SYS_INT_EHRPWM1TZ 			23

/*			EDMA Interrupts */
#define SYS_INT_EDMA3_0_CC0_INT1 	8
#define SYS_INT_EDMA3_0_CC0_ERRINT 	56
#define SYS_INT_EDMA3_0_TC0_ERRINT 	57
#define SYS_INT_EDMA3_0_TC1_ERRINT 	58
#define SYS_INT_EDMA3_1_CC0_INT1 	91
#define SYS_INT_EDMA3_1_CC0_ERRINT 	92 
#define SYS_INT_EDMA3_1_TC0_ERRINT 	93

/*			EMU Interrupts */
#define SYS_INT_EMU_DTDMA 			9
#define SYS_INT_EMU_RTDXRX 			11
#define SYS_INT_EMU_RTDXTX 			12

/*			IDMA Interrupts */
#define SYS_INT_IDMAINT0 			13
#define SYS_INT_IDMAINT1 			14

/*			MMCSD Interrupts */
#define SYS_INT_MMCSD0_INT0 		15
#define SYS_INT_MMCSD0_INT1 		16
#define SYS_INT_MMCSD1_INT0 		68
#define SYS_INT_MMCSD1_INT1 		53

/*			USB Interrupts */
#define SYS_INT_USB0     			19
#define SYS_INT_USB1_HCINT 			20
#define SYS_INT_USB1_RWAKEUP 		21

/*			SATA Interrupts */
#define SYS_INT_SATA_INT 			24

/*			EMAC Interrupts */
#define SYS_INT_EMAC_C0RXTHRESH 	26
#define SYS_INT_EMAC_C0RX 			27
#define SYS_INT_EMAC_C0TX 			28
#define SYS_INT_EMAC_C0MISC 		29
#define SYS_INT_EMAC_C1RXTHRESH 	30
#define SYS_INT_EMAC_C1RX 			31
#define SYS_INT_EMAC_C1TX 			32
#define SYS_INT_EMAC_C1MISC 		33

/*			HPI Interrupts */
#define SYS_INT_UHPI_DSPINT 		34

/*			I2C Interrupts */
#define SYS_INT_I2C0_INT 			36
#define SYS_INT_I2C1_INT 			42

/*			SPI Interrupts */
#define SYS_INT_SPI0_INT 			37
#define SYS_INT_SPI1_INT 			43

/*			UART Interrupts */
#define SYS_INT_UART0_INT 			38
#define SYS_INT_UART1_INT 			46
#define SYS_INT_UART2_INT 			69

/*			GPIO Interrupts */
#define SYS_INT_GPIO_B0INT 			65
#define SYS_INT_GPIO_B1INT 			41
#define SYS_INT_GPIO_B2INT 			49
#define SYS_INT_GPIO_B3INT 			52
#define SYS_INT_GPIO_B4INT 			54
#define SYS_INT_GPIO_B5INT 			59
#define SYS_INT_GPIO_B6INT 			62
#define SYS_INT_GPIO_B7INT 			72
#define SYS_INT_GPIO_B8INT 			75

/*			ECAP Interrupts */
#define SYS_INT_ECAP0 				45
#define SYS_INT_ECAP1 				47
#define SYS_INT_ECAP2 				51

/*			EMIF Interrupts */
#define SYS_INT_EMIFA_INT 			55

/*			DDR Interrupts */
#define SYS_INT_DDR2_MEMERR 		60

/*			MCASP Interrupts */
#define SYS_INT_MCASP0_INT 			61

/*			RTC Interrupts */
#define SYS_INT_RTC_IRQS 			63

/*			PSC Interrupts */
#define SYS_INT_PSC0_ALLINT 		70
#define SYS_INT_PSC1_ALLINT 		71

/*			LCD Interrupts */
#define SYS_INT_LCDC_INT 			73

/*			MCBSP Interrupts */
#define SYS_INT_MCBSP0_RINT 		87
#define SYS_INT_MCBSP0_XINT 		88
#define SYS_INT_MCBSP1_RINT 		89
#define SYS_INT_MCBSP1_XINT 		90

/*			UPP Interrupts */
#define SYS_INT_UPP_INT 			94

/*			VPIF Interrupts */
#define SYS_INT_VPIF_INT 			95

/*			INTC Interrupts */
#define SYS_INT_INTERR 				96

/*			EMC Interrupts */
#define SYS_INT_EMC_IDMAERR 		97
#define SYS_INT_EMC_CMPA 			126
#define SYS_INT_EMC_BUSERR 			127

/*			PMC Interrupts */
#define SYS_INT_PMC_ED 				113
#define SYS_INT_PMC_CMPA 			120
#define SYS_INT_PMC_DMPA 			121

/*			UMC Interrupts */
#define SYS_INT_UMC_ED1 			116
#define SYS_INT_UMC_ED2 			117
#define SYS_INT_UMC_CMPA 			124
#define SYS_INT_UMC_DMPA 			125

/*			PDC Interrupts */
#define SYS_INT_PDC_INT 			118

/*			SYS Interrupts */
#define SYS_INT_SYS_CMPA 			119

/*			DMC Interrupts */
#define SYS_INT_DMC_CMPA 			122
#define SYS_INT_DMC_DMPA 			123

#ifndef SYS_INT_USBSSINT
#define SYS_INT_USBSSINT    255
#endif


/******************************************************************************
** 							FUNCTION DEFINITIONS
******************************************************************************/
extern void IntDSPINTCInit (void);
extern void IntRegister (unsigned int cpuINT, void (*userISR)(void));
extern void IntUnRegister (unsigned int cpuINT);
extern void IntEventMap (unsigned int cpuINT, unsigned int sysINT);
extern void IntEventSet (unsigned int sysINT);
extern void IntEventClear(unsigned int sysINT);
extern void IntEventCombineInit(int ecmINT0, int ecmINT1, int ecmINT2, int ecmINT3);
extern void IntEventCombineAdd(unsigned int sysINT);
extern void IntEventCombineRemove(unsigned int sysINT);
extern void IntEventCombineRegister(unsigned int sysINT, void (*userISR)(void));
extern void IntEnable (unsigned int cpuINT);
extern void IntDisable (unsigned int cpuINT);
extern void IntReset (void);
extern void IntGlobalEnable (void);
extern unsigned int IntGlobalDisable (void);
extern void IntGlobalRestore (unsigned int restoreValue);
extern void ExcGlobalEnable (void);
extern void ExcCombineAdd(unsigned int sysINT);
extern void ExcCombineRemove(unsigned int sysINT);

#ifdef __cplusplus
}
#endif

#endif /* INTERRUPT_H_ */
