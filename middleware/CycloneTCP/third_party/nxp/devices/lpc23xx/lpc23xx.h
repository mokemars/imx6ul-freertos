/******************************************************************************
 *   LPC23xx.h:  Header file for NXP LPC23xx Family Microprocessors
 *   The header file is the super set of all hardware definition of the 
 *   peripherals for the LPC23xx family microprocessor.
 *
 *   Copyright(C) 2006-2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2005.10.01  ver 1.00    Prelimnary version, first Release
 *   2007.05.17  ver 1.01    several corrections
 *   2007.09.05  ver 1.02    added VICVectPriorityx symbols
 *   2007.09.05  ver 1.03    FIO1PIN1 value corrected
 *   2008.04.23  ver 1.04    EMC_DYN_CFGx,EMC_DYN_RASCASx value corrected
 *   2008.08.12  ver 1.05    EMC_STA_EXT_WAIT value corrected
 *   2009.03.03  ver 1.06    Removed LPC24xx register references
 *   2009.03.13  ver 1.07    FIO1MASK1,FIO1SET1,FIO1CLR1 value corrected
 *
******************************************************************************/

#ifndef __LPC23xx_H
#define __LPC23xx_H

/* Vectored Interrupt Controller (VIC) */
#define VIC_BASE_ADDR	0xFFFFF000
#define VICIRQStatus   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x000))
#define VICFIQStatus   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x004))
#define VICRawIntr     (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x008))
#define VICIntSelect   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x00C))
#define VICIntEnable   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x010))
#define VICIntEnClr    (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x014))
#define VICSoftInt     (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x018))
#define VICSoftIntClr  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x01C))
#define VICProtection  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x020))
#define VICSWPrioMask  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x024))

#define VICVectAddr0   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x100))
#define VICVectAddr1   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x104))
#define VICVectAddr2   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x108))
#define VICVectAddr3   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x10C))
#define VICVectAddr4   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x110))
#define VICVectAddr5   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x114))
#define VICVectAddr6   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x118))
#define VICVectAddr7   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x11C))
#define VICVectAddr8   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x120))
#define VICVectAddr9   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x124))
#define VICVectAddr10  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x128))
#define VICVectAddr11  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x12C))
#define VICVectAddr12  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x130))
#define VICVectAddr13  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x134))
#define VICVectAddr14  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x138))
#define VICVectAddr15  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x13C))
#define VICVectAddr16  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x140))
#define VICVectAddr17  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x144))
#define VICVectAddr18  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x148))
#define VICVectAddr19  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x14C))
#define VICVectAddr20  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x150))
#define VICVectAddr21  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x154))
#define VICVectAddr22  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x158))
#define VICVectAddr23  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x15C))
#define VICVectAddr24  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x160))
#define VICVectAddr25  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x164))
#define VICVectAddr26  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x168))
#define VICVectAddr27  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x16C))
#define VICVectAddr28  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x170))
#define VICVectAddr29  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x174))
#define VICVectAddr30  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x178))
#define VICVectAddr31  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x17C))

/* The name convention below is from previous LPC2000 family MCUs, in LPC23xx/24xx,
these registers are known as "VICVectPriority(x)". */
#define VICVectCntl0   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x200))
#define VICVectCntl1   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x204))
#define VICVectCntl2   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x208))
#define VICVectCntl3   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x20C))
#define VICVectCntl4   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x210))
#define VICVectCntl5   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x214))
#define VICVectCntl6   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x218))
#define VICVectCntl7   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x21C))
#define VICVectCntl8   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x220))
#define VICVectCntl9   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x224))
#define VICVectCntl10  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x228))
#define VICVectCntl11  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x22C))
#define VICVectCntl12  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x230))
#define VICVectCntl13  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x234))
#define VICVectCntl14  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x238))
#define VICVectCntl15  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x23C))
#define VICVectCntl16  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x240))
#define VICVectCntl17  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x244))
#define VICVectCntl18  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x248))
#define VICVectCntl19  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x24C))
#define VICVectCntl20  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x250))
#define VICVectCntl21  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x254))
#define VICVectCntl22  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x258))
#define VICVectCntl23  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x25C))
#define VICVectCntl24  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x260))
#define VICVectCntl25  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x264))
#define VICVectCntl26  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x268))
#define VICVectCntl27  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x26C))
#define VICVectCntl28  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x270))
#define VICVectCntl29  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x274))
#define VICVectCntl30  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x278))
#define VICVectCntl31  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x27C))

/* LPC23xx/24xx VICVectPriority(x)". */
#define VICVectPriority0   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x200))
#define VICVectPriority1   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x204))
#define VICVectPriority2   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x208))
#define VICVectPriority3   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x20C))
#define VICVectPriority4   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x210))
#define VICVectPriority5   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x214))
#define VICVectPriority6   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x218))
#define VICVectPriority7   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x21C))
#define VICVectPriority8   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x220))
#define VICVectPriority9   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x224))
#define VICVectPriority10  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x228))
#define VICVectPriority11  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x22C))
#define VICVectPriority12  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x230))
#define VICVectPriority13  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x234))
#define VICVectPriority14  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x238))
#define VICVectPriority15  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x23C))
#define VICVectPriority16  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x240))
#define VICVectPriority17  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x244))
#define VICVectPriority18  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x248))
#define VICVectPriority19  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x24C))
#define VICVectPriority20  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x250))
#define VICVectPriority21  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x254))
#define VICVectPriority22  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x258))
#define VICVectPriority23  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x25C))
#define VICVectPriority24  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x260))
#define VICVectPriority25  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x264))
#define VICVectPriority26  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x268))
#define VICVectPriority27  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x26C))
#define VICVectPriority28  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x270))
#define VICVectPriority29  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x274))
#define VICVectPriority30  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x278))
#define VICVectPriority31  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x27C))

#define VICVectAddr    (*(volatile unsigned long *)(VIC_BASE_ADDR + 0xF00))


/* Pin Connect Block */
#define PINSEL_BASE_ADDR	0xE002C000
#define PINSEL0        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x00))
#define PINSEL1        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x04))
#define PINSEL2        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x08))
#define PINSEL3        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x0C))
#define PINSEL4        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x10))
#define PINSEL5        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x14))
#define PINSEL6        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x18))
#define PINSEL7        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x1C))
#define PINSEL8        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x20))
#define PINSEL9        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x24))
#define PINSEL10       (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x28))

#define PINMODE0        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x40))
#define PINMODE1        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x44))
#define PINMODE2        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x48))
#define PINMODE3        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x4C))
#define PINMODE4        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x50))
#define PINMODE5        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x54))
#define PINMODE6        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x58))
#define PINMODE7        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x5C))
#define PINMODE8        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x60))
#define PINMODE9        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x64))

/* General Purpose Input/Output (GPIO) */
#define GPIO_BASE_ADDR		0xE0028000
#define IOPIN0         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x00))
#define IOSET0         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x04))
#define IODIR0         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x08))
#define IOCLR0         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x0C))
#define IOPIN1         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x10))
#define IOSET1         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x14))
#define IODIR1         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x18))
#define IOCLR1         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x1C))

/* GPIO Interrupt Registers */
#define IO0_INT_EN_R    (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x90)) 
#define IO0_INT_EN_F    (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x94))
#define IO0_INT_STAT_R  (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x84))
#define IO0_INT_STAT_F  (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x88))
#define IO0_INT_CLR     (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x8C))

#define IO2_INT_EN_R    (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0xB0)) 
#define IO2_INT_EN_F    (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0xB4))
#define IO2_INT_STAT_R  (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0xA4))
#define IO2_INT_STAT_F  (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0xA8))
#define IO2_INT_CLR     (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0xAC))

#define IO_INT_STAT     (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x80))

#define PARTCFG_BASE_ADDR		0x3FFF8000
#define PARTCFG        (*(volatile unsigned long *)(PARTCFG_BASE_ADDR + 0x00)) 

/* Fast I/O setup */
#define FIO_BASE_ADDR		0x3FFFC000
#define FIO0DIR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x00)) 
#define FIO0MASK       (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x10))
#define FIO0PIN        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x14))
#define FIO0SET        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x18))
#define FIO0CLR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x1C))

#define FIO1DIR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x20)) 
#define FIO1MASK       (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x30))
#define FIO1PIN        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x34))
#define FIO1SET        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x38))
#define FIO1CLR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x3C))

#define FIO2DIR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x40)) 
#define FIO2MASK       (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x50))
#define FIO2PIN        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x54))
#define FIO2SET        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x58))
#define FIO2CLR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x5C))

#define FIO3DIR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x60)) 
#define FIO3MASK       (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x70))
#define FIO3PIN        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x74))
#define FIO3SET        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x78))
#define FIO3CLR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x7C))

#define FIO4DIR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x80)) 
#define FIO4MASK       (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x90))
#define FIO4PIN        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x94))
#define FIO4SET        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x98))
#define FIO4CLR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x9C))

/* FIOs can be accessed through WORD, HALF-WORD or BYTE. */
#define FIO0DIR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x00)) 
#define FIO1DIR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x20)) 
#define FIO2DIR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x40)) 
#define FIO3DIR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x60)) 
#define FIO4DIR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x80)) 

#define FIO0DIR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x01)) 
#define FIO1DIR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x21)) 
#define FIO2DIR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x41)) 
#define FIO3DIR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x61)) 
#define FIO4DIR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x81)) 

#define FIO0DIR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x02)) 
#define FIO1DIR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x22)) 
#define FIO2DIR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x42)) 
#define FIO3DIR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x62)) 
#define FIO4DIR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x82)) 

#define FIO0DIR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x03)) 
#define FIO1DIR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x23)) 
#define FIO2DIR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x43)) 
#define FIO3DIR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x63)) 
#define FIO4DIR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x83)) 

#define FIO0DIRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x00)) 
#define FIO1DIRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x20)) 
#define FIO2DIRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x40)) 
#define FIO3DIRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x60)) 
#define FIO4DIRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x80)) 

#define FIO0DIRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x02)) 
#define FIO1DIRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x22)) 
#define FIO2DIRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x42)) 
#define FIO3DIRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x62)) 
#define FIO4DIRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x82)) 

#define FIO0MASK0      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x10)) 
#define FIO1MASK0      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x30)) 
#define FIO2MASK0      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x50)) 
#define FIO3MASK0      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x70)) 
#define FIO4MASK0      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x90)) 

#define FIO0MASK1      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x11)) 
#define FIO1MASK1      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x31)) 
#define FIO2MASK1      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x51)) 
#define FIO3MASK1      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x71)) 
#define FIO4MASK1      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x91)) 

#define FIO0MASK2      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x12)) 
#define FIO1MASK2      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x32)) 
#define FIO2MASK2      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x52)) 
#define FIO3MASK2      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x72)) 
#define FIO4MASK2      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x92)) 

#define FIO0MASK3      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x13)) 
#define FIO1MASK3      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x33)) 
#define FIO2MASK3      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x53)) 
#define FIO3MASK3      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x73)) 
#define FIO4MASK3      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x93)) 

#define FIO0MASKL      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x10)) 
#define FIO1MASKL      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x30)) 
#define FIO2MASKL      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x50)) 
#define FIO3MASKL      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x70)) 
#define FIO4MASKL      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x90)) 

#define FIO0MASKU      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x12)) 
#define FIO1MASKU      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x32)) 
#define FIO2MASKU      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x52)) 
#define FIO3MASKU      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x72)) 
#define FIO4MASKU      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x92)) 

#define FIO0PIN0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x14)) 
#define FIO1PIN0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x34)) 
#define FIO2PIN0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x54)) 
#define FIO3PIN0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x74)) 
#define FIO4PIN0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x94)) 

#define FIO0PIN1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x15)) 
#define FIO1PIN1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x35)) 
#define FIO2PIN1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x55)) 
#define FIO3PIN1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x75)) 
#define FIO4PIN1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x95)) 

#define FIO0PIN2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x16)) 
#define FIO1PIN2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x36)) 
#define FIO2PIN2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x56)) 
#define FIO3PIN2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x76)) 
#define FIO4PIN2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x96)) 

#define FIO0PIN3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x17)) 
#define FIO1PIN3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x37)) 
#define FIO2PIN3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x57)) 
#define FIO3PIN3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x77)) 
#define FIO4PIN3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x97)) 

#define FIO0PINL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x14)) 
#define FIO1PINL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x34)) 
#define FIO2PINL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x54)) 
#define FIO3PINL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x74)) 
#define FIO4PINL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x94)) 

#define FIO0PINU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x16)) 
#define FIO1PINU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x36)) 
#define FIO2PINU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x56)) 
#define FIO3PINU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x76)) 
#define FIO4PINU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x96)) 

#define FIO0SET0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x18)) 
#define FIO1SET0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x38)) 
#define FIO2SET0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x58)) 
#define FIO3SET0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x78)) 
#define FIO4SET0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x98)) 

#define FIO0SET1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x19)) 
#define FIO1SET1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x39)) 
#define FIO2SET1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x59)) 
#define FIO3SET1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x79)) 
#define FIO4SET1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x99)) 

#define FIO0SET2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1A)) 
#define FIO1SET2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3A)) 
#define FIO2SET2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5A)) 
#define FIO3SET2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7A)) 
#define FIO4SET2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9A)) 

#define FIO0SET3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1B)) 
#define FIO1SET3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3B)) 
#define FIO2SET3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5B)) 
#define FIO3SET3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7B)) 
#define FIO4SET3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9B)) 

#define FIO0SETL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x18)) 
#define FIO1SETL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x38)) 
#define FIO2SETL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x58)) 
#define FIO3SETL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x78)) 
#define FIO4SETL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x98)) 

#define FIO0SETU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x1A)) 
#define FIO1SETU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x3A)) 
#define FIO2SETU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x5A)) 
#define FIO3SETU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x7A)) 
#define FIO4SETU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x9A)) 

#define FIO0CLR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1C)) 
#define FIO1CLR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3C)) 
#define FIO2CLR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5C)) 
#define FIO3CLR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7C)) 
#define FIO4CLR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9C)) 

#define FIO0CLR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1D)) 
#define FIO1CLR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3D)) 
#define FIO2CLR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5D)) 
#define FIO3CLR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7D)) 
#define FIO4CLR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9D)) 

#define FIO0CLR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1E)) 
#define FIO1CLR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3E)) 
#define FIO2CLR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5E)) 
#define FIO3CLR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7E)) 
#define FIO4CLR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9E)) 

#define FIO0CLR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1F)) 
#define FIO1CLR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3F)) 
#define FIO2CLR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5F)) 
#define FIO3CLR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7F)) 
#define FIO4CLR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9F)) 

#define FIO0CLRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x1C)) 
#define FIO1CLRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x3C)) 
#define FIO2CLRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x5C)) 
#define FIO3CLRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x7C)) 
#define FIO4CLRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x9C)) 

#define FIO0CLRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x1E)) 
#define FIO1CLRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x3E)) 
#define FIO2CLRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x5E)) 
#define FIO3CLRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x7E)) 
#define FIO4CLRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x9E)) 


/* System Control Block(SCB) modules include Memory Accelerator Module,
Phase Locked Loop, VPB divider, Power Control, External Interrupt, 
Reset, and Code Security/Debugging */
#define SCB_BASE_ADDR	0xE01FC000

/* Memory Accelerator Module (MAM) */
#define MAMCR          (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x000))
#define MAMTIM         (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x004))
#define MEMMAP         (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x040))

/* Phase Locked Loop (PLL) */
#define PLLCON         (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x080))
#define PLLCFG         (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x084))
#define PLLSTAT        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x088))
#define PLLFEED        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x08C))

/* Power Control */
#define PCON           (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x0C0))
#define PCONP          (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x0C4))

/* Clock Divider */
// #define APBDIV         (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x100))
#define CCLKCFG        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x104))
#define USBCLKCFG      (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x108))
#define CLKSRCSEL      (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x10C))
#define PCLKSEL0       (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x1A8))
#define PCLKSEL1       (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x1AC))
	
/* External Interrupts */
#define EXTINT         (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x140))
#define INTWAKE        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x144))
#define EXTMODE        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x148))
#define EXTPOLAR       (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x14C))

/* Reset, reset source identification */
#define RSIR           (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x180))

/* RSID, code security protection */
#define CSPR           (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x184))

/* AHB configuration */
#define AHBCFG1        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x188))
#define AHBCFG2        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x18C))

/* System Controls and Status */
#define SCS            (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x1A0))	

/* MPMC(EMC) registers */
#define STATIC_MEM0_BASE		0x80000000
#define STATIC_MEM1_BASE		0x81000000
#define STATIC_MEM2_BASE		0x82000000
#define STATIC_MEM3_BASE		0x83000000

#define DYNAMIC_MEM0_BASE		0xA0000000
#define DYNAMIC_MEM1_BASE		0xB0000000
#define DYNAMIC_MEM2_BASE		0xC0000000
#define DYNAMIC_MEM3_BASE		0xD0000000

/* External Memory Controller (EMC) */
#define EMC_BASE_ADDR		0xFFE08000
#define EMC_CTRL       (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x000))
#define EMC_STAT       (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x004))
#define EMC_CONFIG     (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x008))

/* Dynamic RAM access registers */
#define EMC_DYN_CTRL     (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x020))
#define EMC_DYN_RFSH     (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x024))
#define EMC_DYN_RD_CFG   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x028))
#define EMC_DYN_RP       (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x030))
#define EMC_DYN_RAS      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x034))
#define EMC_DYN_SREX     (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x038))
#define EMC_DYN_APR      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x03C))
#define EMC_DYN_DAL      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x040))
#define EMC_DYN_WR       (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x044))
#define EMC_DYN_RC       (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x048))
#define EMC_DYN_RFC      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x04C))
#define EMC_DYN_XSR      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x050))
#define EMC_DYN_RRD      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x054))
#define EMC_DYN_MRD      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x058))

#define EMC_DYN_CFG0     (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x100))
#define EMC_DYN_RASCAS0  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x104))
#define EMC_DYN_CFG1     (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x120))
#define EMC_DYN_RASCAS1  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x124))
#define EMC_DYN_CFG2     (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x140))
#define EMC_DYN_RASCAS2  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x144))
#define EMC_DYN_CFG3     (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x160))
#define EMC_DYN_RASCAS3  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x164))

/* static RAM access registers */
#define EMC_STA_CFG0      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x200))
#define EMC_STA_WAITWEN0  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x204))
#define EMC_STA_WAITOEN0  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x208))
#define EMC_STA_WAITRD0   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x20C))
#define EMC_STA_WAITPAGE0 (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x210))
#define EMC_STA_WAITWR0   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x214))
#define EMC_STA_WAITTURN0 (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x218))

#define EMC_STA_CFG1      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x220))
#define EMC_STA_WAITWEN1  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x224))
#define EMC_STA_WAITOEN1  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x228))
#define EMC_STA_WAITRD1   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x22C))
#define EMC_STA_WAITPAGE1 (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x230))
#define EMC_STA_WAITWR1   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x234))
#define EMC_STA_WAITTURN1 (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x238))

#define EMC_STA_CFG2      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x240))
#define EMC_STA_WAITWEN2  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x244))
#define EMC_STA_WAITOEN2  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x248))
#define EMC_STA_WAITRD2   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x24C))
#define EMC_STA_WAITPAGE2 (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x250))
#define EMC_STA_WAITWR2   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x254))
#define EMC_STA_WAITTURN2 (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x258))

#define EMC_STA_CFG3      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x260))
#define EMC_STA_WAITWEN3  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x264))
#define EMC_STA_WAITOEN3  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x268))
#define EMC_STA_WAITRD3   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x26C))
#define EMC_STA_WAITPAGE3 (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x270))
#define EMC_STA_WAITWR3   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x274))
#define EMC_STA_WAITTURN3 (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x278))

#define EMC_STA_EXT_WAIT  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x080))

	
/* Timer 0 */
#define TMR0_BASE_ADDR		0xE0004000
#define T0IR           (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x00))
#define T0TCR          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x04))
#define T0TC           (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x08))
#define T0PR           (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x0C))
#define T0PC           (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x10))
#define T0MCR          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x14))
#define T0MR0          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x18))
#define T0MR1          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x1C))
#define T0MR2          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x20))
#define T0MR3          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x24))
#define T0CCR          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x28))
#define T0CR0          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x2C))
#define T0CR1          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x30))
#define T0CR2          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x34))
#define T0CR3          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x38))
#define T0EMR          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x3C))
#define T0CTCR         (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x70))

/* Timer 1 */
#define TMR1_BASE_ADDR		0xE0008000
#define T1IR           (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x00))
#define T1TCR          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x04))
#define T1TC           (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x08))
#define T1PR           (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x0C))
#define T1PC           (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x10))
#define T1MCR          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x14))
#define T1MR0          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x18))
#define T1MR1          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x1C))
#define T1MR2          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x20))
#define T1MR3          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x24))
#define T1CCR          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x28))
#define T1CR0          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x2C))
#define T1CR1          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x30))
#define T1CR2          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x34))
#define T1CR3          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x38))
#define T1EMR          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x3C))
#define T1CTCR         (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x70))

/* Timer 2 */
#define TMR2_BASE_ADDR		0xE0070000
#define T2IR           (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x00))
#define T2TCR          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x04))
#define T2TC           (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x08))
#define T2PR           (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x0C))
#define T2PC           (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x10))
#define T2MCR          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x14))
#define T2MR0          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x18))
#define T2MR1          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x1C))
#define T2MR2          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x20))
#define T2MR3          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x24))
#define T2CCR          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x28))
#define T2CR0          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x2C))
#define T2CR1          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x30))
#define T2CR2          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x34))
#define T2CR3          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x38))
#define T2EMR          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x3C))
#define T2CTCR         (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x70))

/* Timer 3 */
#define TMR3_BASE_ADDR		0xE0074000
#define T3IR           (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x00))
#define T3TCR          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x04))
#define T3TC           (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x08))
#define T3PR           (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x0C))
#define T3PC           (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x10))
#define T3MCR          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x14))
#define T3MR0          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x18))
#define T3MR1          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x1C))
#define T3MR2          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x20))
#define T3MR3          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x24))
#define T3CCR          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x28))
#define T3CR0          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x2C))
#define T3CR1          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x30))
#define T3CR2          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x34))
#define T3CR3          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x38))
#define T3EMR          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x3C))
#define T3CTCR         (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x70))


/* Pulse Width Modulator (PWM) */
#define PWM0_BASE_ADDR		0xE0014000
#define PWM0IR          (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x00))
#define PWM0TCR         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x04))
#define PWM0TC          (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x08))
#define PWM0PR          (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x0C))
#define PWM0PC          (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x10))
#define PWM0MCR         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x14))
#define PWM0MR0         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x18))
#define PWM0MR1         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x1C))
#define PWM0MR2         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x20))
#define PWM0MR3         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x24))
#define PWM0CCR         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x28))
#define PWM0CR0         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x2C))
#define PWM0CR1         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x30))
#define PWM0CR2         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x34))
#define PWM0CR3         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x38))
#define PWM0MR4         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x40))
#define PWM0MR5         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x44))
#define PWM0MR6         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x48))
#define PWM0PCR         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x4C))
#define PWM0LER         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x50))
#define PWM0CTCR        (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x70))

#define PWM1_BASE_ADDR		0xE0018000
#define PWM1IR          (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x00))
#define PWM1TCR         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x04))
#define PWM1TC          (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x08))
#define PWM1PR          (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x0C))
#define PWM1PC          (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x10))
#define PWM1MCR         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x14))
#define PWM1MR0         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x18))
#define PWM1MR1         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x1C))
#define PWM1MR2         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x20))
#define PWM1MR3         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x24))
#define PWM1CCR         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x28))
#define PWM1CR0         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x2C))
#define PWM1CR1         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x30))
#define PWM1CR2         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x34))
#define PWM1CR3         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x38))
#define PWM1MR4         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x40))
#define PWM1MR5         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x44))
#define PWM1MR6         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x48))
#define PWM1PCR         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x4C))
#define PWM1LER         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x50))
#define PWM1CTCR        (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x70))


/* Universal Asynchronous Receiver Transmitter 0 (UART0) */
#define UART0_BASE_ADDR		0xE000C000
#define U0RBR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x00))
#define U0THR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x00))
#define U0DLL          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x00))
#define U0DLM          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x04))
#define U0IER          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x04))
#define U0IIR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x08))
#define U0FCR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x08))
#define U0LCR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x0C))
#define U0LSR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x14))
#define U0SCR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x1C))
#define U0ACR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x20))
#define U0ICR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x24))
#define U0FDR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x28))
#define U0TER          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x30))

/* Universal Asynchronous Receiver Transmitter 1 (UART1) */
#define UART1_BASE_ADDR		0xE0010000
#define U1RBR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x00))
#define U1THR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x00))
#define U1DLL          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x00))
#define U1DLM          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x04))
#define U1IER          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x04))
#define U1IIR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x08))
#define U1FCR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x08))
#define U1LCR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x0C))
#define U1MCR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x10))
#define U1LSR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x14))
#define U1MSR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x18))
#define U1SCR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x1C))
#define U1ACR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x20))
#define U1FDR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x28))
#define U1TER          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x30))

/* Universal Asynchronous Receiver Transmitter 2 (UART2) */
#define UART2_BASE_ADDR		0xE0078000
#define U2RBR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x00))
#define U2THR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x00))
#define U2DLL          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x00))
#define U2DLM          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x04))
#define U2IER          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x04))
#define U2IIR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x08))
#define U2FCR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x08))
#define U2LCR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x0C))
#define U2LSR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x14))
#define U2SCR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x1C))
#define U2ACR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x20))
#define U2ICR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x24))
#define U2FDR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x28))
#define U2TER          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x30))

/* Universal Asynchronous Receiver Transmitter 3 (UART3) */
#define UART3_BASE_ADDR		0xE007C000
#define U3RBR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x00))
#define U3THR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x00))
#define U3DLL          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x00))
#define U3DLM          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x04))
#define U3IER          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x04))
#define U3IIR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x08))
#define U3FCR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x08))
#define U3LCR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x0C))
#define U3LSR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x14))
#define U3SCR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x1C))
#define U3ACR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x20))
#define U3ICR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x24))
#define U3FDR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x28))
#define U3TER          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x30))

/* I2C Interface 0 */
#define I2C0_BASE_ADDR		0xE001C000
#define I20CONSET      (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x00))
#define I20STAT        (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x04))
#define I20DAT         (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x08))
#define I20ADR         (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x0C))
#define I20SCLH        (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x10))
#define I20SCLL        (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x14))
#define I20CONCLR      (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x18))

/* I2C Interface 1 */
#define I2C1_BASE_ADDR		0xE005C000
#define I21CONSET      (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x00))
#define I21STAT        (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x04))
#define I21DAT         (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x08))
#define I21ADR         (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x0C))
#define I21SCLH        (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x10))
#define I21SCLL        (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x14))
#define I21CONCLR      (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x18))

/* I2C Interface 2 */
#define I2C2_BASE_ADDR		0xE0080000
#define I22CONSET      (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x00))
#define I22STAT        (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x04))
#define I22DAT         (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x08))
#define I22ADR         (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x0C))
#define I22SCLH        (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x10))
#define I22SCLL        (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x14))
#define I22CONCLR      (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x18))

/* SPI0 (Serial Peripheral Interface 0) */
#define SPI0_BASE_ADDR		0xE0020000
#define S0SPCR         (*(volatile unsigned long *)(SPI0_BASE_ADDR + 0x00))
#define S0SPSR         (*(volatile unsigned long *)(SPI0_BASE_ADDR + 0x04))
#define S0SPDR         (*(volatile unsigned long *)(SPI0_BASE_ADDR + 0x08))
#define S0SPCCR        (*(volatile unsigned long *)(SPI0_BASE_ADDR + 0x0C))
#define S0SPINT        (*(volatile unsigned long *)(SPI0_BASE_ADDR + 0x1C))

/* SSP0 Controller */
#define SSP0_BASE_ADDR		0xE0068000
#define SSP0CR0        (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x00))
#define SSP0CR1        (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x04))
#define SSP0DR         (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x08))
#define SSP0SR         (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x0C))
#define SSP0CPSR       (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x10))
#define SSP0IMSC       (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x14))
#define SSP0RIS        (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x18))
#define SSP0MIS        (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x1C))
#define SSP0ICR        (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x20))
#define SSP0DMACR      (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x24))

/* SSP1 Controller */
#define SSP1_BASE_ADDR		0xE0030000
#define SSP1CR0        (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x00))
#define SSP1CR1        (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x04))
#define SSP1DR         (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x08))
#define SSP1SR         (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x0C))
#define SSP1CPSR       (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x10))
#define SSP1IMSC       (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x14))
#define SSP1RIS        (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x18))
#define SSP1MIS        (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x1C))
#define SSP1ICR        (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x20))
#define SSP1DMACR      (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x24))


/* Real Time Clock */
#define RTC_BASE_ADDR		0xE0024000
#define RTC_ILR         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x00))
#define RTC_CTC         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x04))
#define RTC_CCR         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x08))
#define RTC_CIIR        (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x0C))
#define RTC_AMR         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x10))
#define RTC_CTIME0      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x14))
#define RTC_CTIME1      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x18))
#define RTC_CTIME2      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x1C))
#define RTC_SEC         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x20))
#define RTC_MIN         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x24))
#define RTC_HOUR        (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x28))
#define RTC_DOM         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x2C))
#define RTC_DOW         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x30))
#define RTC_DOY         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x34))
#define RTC_MONTH       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x38))
#define RTC_YEAR        (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x3C))
#define RTC_CISS        (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x40))
#define RTC_ALSEC       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x60))
#define RTC_ALMIN       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x64))
#define RTC_ALHOUR      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x68))
#define RTC_ALDOM       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x6C))
#define RTC_ALDOW       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x70))
#define RTC_ALDOY       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x74))
#define RTC_ALMON       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x78))
#define RTC_ALYEAR      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x7C))
#define RTC_PREINT      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x80))
#define RTC_PREFRAC     (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x84))


/* A/D Converter 0 (AD0) */
#define AD0_BASE_ADDR		0xE0034000
#define AD0CR          (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x00))
#define AD0GDR         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x04))
#define AD0INTEN       (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x0C))
#define AD0DR0         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x10))
#define AD0DR1         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x14))
#define AD0DR2         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x18))
#define AD0DR3         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x1C))
#define AD0DR4         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x20))
#define AD0DR5         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x24))
#define AD0DR6         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x28))
#define AD0DR7         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x2C))
#define AD0STAT        (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x30))


/* D/A Converter */
#define DAC_BASE_ADDR		0xE006C000
#define DACR           (*(volatile unsigned long *)(DAC_BASE_ADDR + 0x00))


/* Watchdog */
#define WDG_BASE_ADDR		0xE0000000
#define WDMOD          (*(volatile unsigned long *)(WDG_BASE_ADDR + 0x00))
#define WDTC           (*(volatile unsigned long *)(WDG_BASE_ADDR + 0x04))
#define WDFEED         (*(volatile unsigned long *)(WDG_BASE_ADDR + 0x08))
#define WDTV           (*(volatile unsigned long *)(WDG_BASE_ADDR + 0x0C))
#define WDCLKSEL       (*(volatile unsigned long *)(WDG_BASE_ADDR + 0x10))

/* CAN CONTROLLERS AND ACCEPTANCE FILTER */
#define CAN_ACCEPT_BASE_ADDR		0xE003C000
#define CAN_AFMR		(*(volatile unsigned long *)(CAN_ACCEPT_BASE_ADDR + 0x00))  	
#define CAN_SFF_SA 		(*(volatile unsigned long *)(CAN_ACCEPT_BASE_ADDR + 0x04))  	
#define CAN_SFF_GRP_SA 	(*(volatile unsigned long *)(CAN_ACCEPT_BASE_ADDR + 0x08))
#define CAN_EFF_SA 		(*(volatile unsigned long *)(CAN_ACCEPT_BASE_ADDR + 0x0C))
#define CAN_EFF_GRP_SA 	(*(volatile unsigned long *)(CAN_ACCEPT_BASE_ADDR + 0x10))  	
#define CAN_EOT 		(*(volatile unsigned long *)(CAN_ACCEPT_BASE_ADDR + 0x14))
#define CAN_LUT_ERR_ADR (*(volatile unsigned long *)(CAN_ACCEPT_BASE_ADDR + 0x18))  	
#define CAN_LUT_ERR 	(*(volatile unsigned long *)(CAN_ACCEPT_BASE_ADDR + 0x1C))

#define CAN_CENTRAL_BASE_ADDR		0xE0040000  	
#define CAN_TX_SR 	(*(volatile unsigned long *)(CAN_CENTRAL_BASE_ADDR + 0x00))  	
#define CAN_RX_SR 	(*(volatile unsigned long *)(CAN_CENTRAL_BASE_ADDR + 0x04))  	
#define CAN_MSR 	(*(volatile unsigned long *)(CAN_CENTRAL_BASE_ADDR + 0x08))

#define CAN1_BASE_ADDR		0xE0044000
#define CAN1MOD 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x00))  	
#define CAN1CMR 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x04))  	
#define CAN1GSR 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x08))  	
#define CAN1ICR 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x0C))  	
#define CAN1IER 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x10))
#define CAN1BTR 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x14))  	
#define CAN1EWL 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x18))  	
#define CAN1SR 		(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x1C))  	
#define CAN1RFS 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x20))  	
#define CAN1RID 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x24))
#define CAN1RDA 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x28))  	
#define CAN1RDB 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x2C))
  	
#define CAN1TFI1 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x30))  	
#define CAN1TID1 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x34))  	
#define CAN1TDA1 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x38))
#define CAN1TDB1 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x3C))  	
#define CAN1TFI2 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x40))  	
#define CAN1TID2 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x44))  	
#define CAN1TDA2 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x48))  	
#define CAN1TDB2 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x4C))
#define CAN1TFI3 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x50))  	
#define CAN1TID3 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x54))  	
#define CAN1TDA3 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x58))  	
#define CAN1TDB3 	(*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x5C))

#define CAN2_BASE_ADDR		0xE0048000
#define CAN2MOD 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x00))  	
#define CAN2CMR 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x04))  	
#define CAN2GSR 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x08))  	
#define CAN2ICR 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x0C))  	
#define CAN2IER 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x10))
#define CAN2BTR 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x14))  	
#define CAN2EWL 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x18))  	
#define CAN2SR 		(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x1C))  	
#define CAN2RFS 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x20))  	
#define CAN2RID 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x24))
#define CAN2RDA 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x28))  	
#define CAN2RDB 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x2C))
  	
#define CAN2TFI1 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x30))  	
#define CAN2TID1 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x34))  	
#define CAN2TDA1 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x38))
#define CAN2TDB1 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x3C))  	
#define CAN2TFI2 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x40))  	
#define CAN2TID2 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x44))  	
#define CAN2TDA2 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x48))  	
#define CAN2TDB2 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x4C))
#define CAN2TFI3 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x50))  	
#define CAN2TID3 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x54))  	
#define CAN2TDA3 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x58))  	
#define CAN2TDB3 	(*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x5C))


/* MultiMedia Card Interface(MCI) Controller */
#define MCI_BASE_ADDR		0xE008C000
#define MCI_POWER      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x00))
#define MCI_CLOCK      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x04))
#define MCI_ARGUMENT   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x08))
#define MCI_COMMAND    (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x0C))
#define MCI_RESP_CMD   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x10))
#define MCI_RESP0      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x14))
#define MCI_RESP1      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x18))
#define MCI_RESP2      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x1C))
#define MCI_RESP3      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x20))
#define MCI_DATA_TMR   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x24))
#define MCI_DATA_LEN   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x28))
#define MCI_DATA_CTRL  (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x2C))
#define MCI_DATA_CNT   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x30))
#define MCI_STATUS     (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x34))
#define MCI_CLEAR      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x38))
#define MCI_MASK0      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x3C))
#define MCI_MASK1      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x40))
#define MCI_FIFO_CNT   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x48))
#define MCI_FIFO       (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x80))


/* I2S Interface Controller (I2S) */
#define I2S_BASE_ADDR		0xE0088000
#define I2S_DAO        (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x00))
#define I2S_DAI        (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x04))
#define I2S_TX_FIFO    (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x08))
#define I2S_RX_FIFO    (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x0C))
#define I2S_STATE      (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x10))
#define I2S_DMA1       (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x14))
#define I2S_DMA2       (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x18))
#define I2S_IRQ        (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x1C))
#define I2S_TXRATE     (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x20))
#define I2S_RXRATE     (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x24))


/* General-purpose DMA Controller */
#define DMA_BASE_ADDR		0xFFE04000
#define GPDMA_INT_STAT         (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x000))
#define GPDMA_INT_TCSTAT       (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x004))
#define GPDMA_INT_TCCLR        (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x008))
#define GPDMA_INT_ERR_STAT     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x00C))
#define GPDMA_INT_ERR_CLR      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x010))
#define GPDMA_RAW_INT_TCSTAT   (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x014))
#define GPDMA_RAW_INT_ERR_STAT (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x018))
#define GPDMA_ENABLED_CHNS     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x01C))
#define GPDMA_SOFT_BREQ        (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x020))
#define GPDMA_SOFT_SREQ        (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x024))
#define GPDMA_SOFT_LBREQ       (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x028))
#define GPDMA_SOFT_LSREQ       (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x02C))
#define GPDMA_CONFIG           (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x030))
#define GPDMA_SYNC             (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x034))

/* DMA channel 0 registers */
#define GPDMA_CH0_SRC      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x100))
#define GPDMA_CH0_DEST     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x104))
#define GPDMA_CH0_LLI      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x108))
#define GPDMA_CH0_CTRL     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x10C))
#define GPDMA_CH0_CFG      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x110))

/* DMA channel 1 registers */
#define GPDMA_CH1_SRC      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x120))
#define GPDMA_CH1_DEST     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x124))
#define GPDMA_CH1_LLI      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x128))
#define GPDMA_CH1_CTRL     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x12C))
#define GPDMA_CH1_CFG      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x130))


/* USB Controller */
#define USB_INT_BASE_ADDR	0xE01FC1C0
#define USB_BASE_ADDR		0xFFE0C200		/* USB Base Address */

#define USB_INT_STAT    (*(volatile unsigned long *)(USB_INT_BASE_ADDR + 0x00))

/* USB Device Interrupt Registers */
#define DEV_INT_STAT    (*(volatile unsigned long *)(USB_BASE_ADDR + 0x00))
#define DEV_INT_EN      (*(volatile unsigned long *)(USB_BASE_ADDR + 0x04))
#define DEV_INT_CLR     (*(volatile unsigned long *)(USB_BASE_ADDR + 0x08))
#define DEV_INT_SET     (*(volatile unsigned long *)(USB_BASE_ADDR + 0x0C))
#define DEV_INT_PRIO    (*(volatile unsigned long *)(USB_BASE_ADDR + 0x2C))

/* USB Device Endpoint Interrupt Registers */
#define EP_INT_STAT     (*(volatile unsigned long *)(USB_BASE_ADDR + 0x30))
#define EP_INT_EN       (*(volatile unsigned long *)(USB_BASE_ADDR + 0x34))
#define EP_INT_CLR      (*(volatile unsigned long *)(USB_BASE_ADDR + 0x38))
#define EP_INT_SET      (*(volatile unsigned long *)(USB_BASE_ADDR + 0x3C))
#define EP_INT_PRIO     (*(volatile unsigned long *)(USB_BASE_ADDR + 0x40))

/* USB Device Endpoint Realization Registers */
#define REALIZE_EP      (*(volatile unsigned long *)(USB_BASE_ADDR + 0x44))
#define EP_INDEX        (*(volatile unsigned long *)(USB_BASE_ADDR + 0x48))
#define MAXPACKET_SIZE  (*(volatile unsigned long *)(USB_BASE_ADDR + 0x4C))

/* USB Device Command Reagisters */
#define CMD_CODE        (*(volatile unsigned long *)(USB_BASE_ADDR + 0x10))
#define CMD_DATA        (*(volatile unsigned long *)(USB_BASE_ADDR + 0x14))

/* USB Device Data Transfer Registers */
#define RX_DATA         (*(volatile unsigned long *)(USB_BASE_ADDR + 0x18))
#define TX_DATA         (*(volatile unsigned long *)(USB_BASE_ADDR + 0x1C))
#define RX_PLENGTH      (*(volatile unsigned long *)(USB_BASE_ADDR + 0x20))
#define TX_PLENGTH      (*(volatile unsigned long *)(USB_BASE_ADDR + 0x24))
#define USB_CTRL        (*(volatile unsigned long *)(USB_BASE_ADDR + 0x28))

/* USB Device DMA Registers */
#define DMA_REQ_STAT        (*(volatile unsigned long *)(USB_BASE_ADDR + 0x50))
#define DMA_REQ_CLR         (*(volatile unsigned long *)(USB_BASE_ADDR + 0x54))
#define DMA_REQ_SET         (*(volatile unsigned long *)(USB_BASE_ADDR + 0x58))
#define UDCA_HEAD           (*(volatile unsigned long *)(USB_BASE_ADDR + 0x80))
#define EP_DMA_STAT         (*(volatile unsigned long *)(USB_BASE_ADDR + 0x84))
#define EP_DMA_EN           (*(volatile unsigned long *)(USB_BASE_ADDR + 0x88))
#define EP_DMA_DIS          (*(volatile unsigned long *)(USB_BASE_ADDR + 0x8C))
#define DMA_INT_STAT        (*(volatile unsigned long *)(USB_BASE_ADDR + 0x90))
#define DMA_INT_EN          (*(volatile unsigned long *)(USB_BASE_ADDR + 0x94))
#define EOT_INT_STAT        (*(volatile unsigned long *)(USB_BASE_ADDR + 0xA0))
#define EOT_INT_CLR         (*(volatile unsigned long *)(USB_BASE_ADDR + 0xA4))
#define EOT_INT_SET         (*(volatile unsigned long *)(USB_BASE_ADDR + 0xA8))
#define NDD_REQ_INT_STAT    (*(volatile unsigned long *)(USB_BASE_ADDR + 0xAC))
#define NDD_REQ_INT_CLR     (*(volatile unsigned long *)(USB_BASE_ADDR + 0xB0))
#define NDD_REQ_INT_SET     (*(volatile unsigned long *)(USB_BASE_ADDR + 0xB4))
#define SYS_ERR_INT_STAT    (*(volatile unsigned long *)(USB_BASE_ADDR + 0xB8))
#define SYS_ERR_INT_CLR     (*(volatile unsigned long *)(USB_BASE_ADDR + 0xBC))
#define SYS_ERR_INT_SET     (*(volatile unsigned long *)(USB_BASE_ADDR + 0xC0))

/* USB Host Controller */
#define USBHC_BASE_ADDR		0xFFE0C000
#define HC_REVISION         (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x00))
#define HC_CONTROL          (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x04))
#define HC_CMD_STAT         (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x08))
#define HC_INT_STAT         (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x0C))
#define HC_INT_EN           (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x10))
#define HC_INT_DIS          (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x14))
#define HC_HCCA             (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x18))
#define HC_PERIOD_CUR_ED    (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x1C))
#define HC_CTRL_HEAD_ED     (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x20))
#define HC_CTRL_CUR_ED      (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x24))
#define HC_BULK_HEAD_ED     (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x28))
#define HC_BULK_CUR_ED      (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x2C))
#define HC_DONE_HEAD        (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x30))
#define HC_FM_INTERVAL      (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x34))
#define HC_FM_REMAINING     (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x38))
#define HC_FM_NUMBER        (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x3C))
#define HC_PERIOD_START     (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x40))
#define HC_LS_THRHLD        (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x44))
#define HC_RH_DESCA         (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x48))
#define HC_RH_DESCB         (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x4C))
#define HC_RH_STAT          (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x50))
#define HC_RH_PORT_STAT1    (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x54))
#define HC_RH_PORT_STAT2    (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x58))

/* USB OTG Controller */
#define USBOTG_BASE_ADDR	0xFFE0C100
#define OTG_INT_STAT        (*(volatile unsigned long *)(USBOTG_BASE_ADDR + 0x00))
#define OTG_INT_EN          (*(volatile unsigned long *)(USBOTG_BASE_ADDR + 0x04))
#define OTG_INT_SET         (*(volatile unsigned long *)(USBOTG_BASE_ADDR + 0x08))
#define OTG_INT_CLR         (*(volatile unsigned long *)(USBOTG_BASE_ADDR + 0x0C))
#define OTG_TIMER           (*(volatile unsigned long *)(USBOTG_BASE_ADDR + 0x14))

#define USBOTG_I2C_BASE_ADDR	0xFFE0C300
#define OTG_I2C_RX          (*(volatile unsigned long *)(USBOTG_I2C_BASE_ADDR + 0x00))
#define OTG_I2C_TX          (*(volatile unsigned long *)(USBOTG_I2C_BASE_ADDR + 0x00))
#define OTG_I2C_STS         (*(volatile unsigned long *)(USBOTG_I2C_BASE_ADDR + 0x04))
#define OTG_I2C_CTL         (*(volatile unsigned long *)(USBOTG_I2C_BASE_ADDR + 0x08))
#define OTG_I2C_CLKHI       (*(volatile unsigned long *)(USBOTG_I2C_BASE_ADDR + 0x0C))
#define OTG_I2C_CLKLO       (*(volatile unsigned long *)(USBOTG_I2C_BASE_ADDR + 0x10))

#define USBOTG_CLK_BASE_ADDR	0xFFE0CFF0
#define USBPortSel          (*(volatile unsigned long *)(USBOTG_BASE_ADDR + 0x10))
#define USBClkCtrl          (*(volatile unsigned long *)(USBOTG_CLK_BASE_ADDR + 0x04))
#define USBClkSt            (*(volatile unsigned long *)(USBOTG_CLK_BASE_ADDR + 0x08))

/* Ethernet MAC (32 bit data bus) -- all registers are RW unless indicated in parentheses */
#define MAC_BASE_ADDR		0xFFE00000 /* AHB Peripheral # 0 */
#define MAC_MAC1            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x000)) /* MAC config reg 1 */
#define MAC_MAC2            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x004)) /* MAC config reg 2 */
#define MAC_IPGT            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x008)) /* b2b InterPacketGap reg */
#define MAC_IPGR            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x00C)) /* non b2b InterPacketGap reg */
#define MAC_CLRT            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x010)) /* CoLlision window/ReTry reg */
#define MAC_MAXF            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x014)) /* MAXimum Frame reg */
#define MAC_SUPP            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x018)) /* PHY SUPPort reg */
#define MAC_TEST            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x01C)) /* TEST reg */
#define MAC_MCFG            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x020)) /* MII Mgmt ConFiG reg */
#define MAC_MCMD            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x024)) /* MII Mgmt CoMmanD reg */
#define MAC_MADR            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x028)) /* MII Mgmt ADdRess reg */
#define MAC_MWTD            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x02C)) /* MII Mgmt WriTe Data reg (WO) */
#define MAC_MRDD            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x030)) /* MII Mgmt ReaD Data reg (RO) */
#define MAC_MIND            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x034)) /* MII Mgmt INDicators reg (RO) */

#define MAC_SA0             (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x040)) /* Station Address 0 reg */
#define MAC_SA1             (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x044)) /* Station Address 1 reg */
#define MAC_SA2             (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x048)) /* Station Address 2 reg */

#define MAC_COMMAND         (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x100)) /* Command reg */
#define MAC_STATUS          (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x104)) /* Status reg (RO) */
#define MAC_RXDESCRIPTOR    (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x108)) /* Rx descriptor base address reg */
#define MAC_RXSTATUS        (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x10C)) /* Rx status base address reg */
#define MAC_RXDESCRIPTORNUM (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x110)) /* Rx number of descriptors reg */
#define MAC_RXPRODUCEINDEX  (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x114)) /* Rx produce index reg (RO) */
#define MAC_RXCONSUMEINDEX  (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x118)) /* Rx consume index reg */
#define MAC_TXDESCRIPTOR    (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x11C)) /* Tx descriptor base address reg */
#define MAC_TXSTATUS        (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x120)) /* Tx status base address reg */
#define MAC_TXDESCRIPTORNUM (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x124)) /* Tx number of descriptors reg */
#define MAC_TXPRODUCEINDEX  (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x128)) /* Tx produce index reg */
#define MAC_TXCONSUMEINDEX  (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x12C)) /* Tx consume index reg (RO) */

#define MAC_TSV0            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x158)) /* Tx status vector 0 reg (RO) */
#define MAC_TSV1            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x15C)) /* Tx status vector 1 reg (RO) */
#define MAC_RSV             (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x160)) /* Rx status vector reg (RO) */

#define MAC_FLOWCONTROLCNT  (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x170)) /* Flow control counter reg */
#define MAC_FLOWCONTROLSTS  (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x174)) /* Flow control status reg */

#define MAC_RXFILTERCTRL    (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x200)) /* Rx filter ctrl reg */
#define MAC_RXFILTERWOLSTS  (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x204)) /* Rx filter WoL status reg (RO) */
#define MAC_RXFILTERWOLCLR  (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x208)) /* Rx filter WoL clear reg (WO) */

#define MAC_HASHFILTERL     (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x210)) /* Hash filter LSBs reg */
#define MAC_HASHFILTERH     (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x214)) /* Hash filter MSBs reg */

#define MAC_INTSTATUS       (*(volatile unsigned long *)(MAC_BASE_ADDR + 0xFE0)) /* Interrupt status reg (RO) */
#define MAC_INTENABLE       (*(volatile unsigned long *)(MAC_BASE_ADDR + 0xFE4)) /* Interrupt enable reg  */
#define MAC_INTCLEAR        (*(volatile unsigned long *)(MAC_BASE_ADDR + 0xFE8)) /* Interrupt clear reg (WO) */
#define MAC_INTSET          (*(volatile unsigned long *)(MAC_BASE_ADDR + 0xFEC)) /* Interrupt set reg (WO) */

#define MAC_POWERDOWN       (*(volatile unsigned long *)(MAC_BASE_ADDR + 0xFF4)) /* Power-down reg */
#define MAC_MODULEID        (*(volatile unsigned long *)(MAC_BASE_ADDR + 0xFFC)) /* Module ID reg (RO) */


//PINSEL0 register
#define PINSEL0_P0_0_MASK           0x00000003
#define PINSEL0_P0_0_GPIO           0x00000000
#define PINSEL0_P0_0_RD1            0x00000001
#define PINSEL0_P0_0_TXD3           0x00000002
#define PINSEL0_P0_0_SDA1           0x00000003

#define PINSEL0_P0_1_MASK           0x0000000C
#define PINSEL0_P0_1_GPIO           0x00000000
#define PINSEL0_P0_1_TD1            0x00000004
#define PINSEL0_P0_1_RXD3           0x00000008
#define PINSEL0_P0_1_SCL1           0x0000000C

#define PINSEL0_P0_2_MASK           0x00000030
#define PINSEL0_P0_2_GPIO           0x00000000
#define PINSEL0_P0_2_TXD0           0x00000010
#define PINSEL0_P0_2_AD0_7          0x00000020

#define PINSEL0_P0_3_MASK           0x000000C0
#define PINSEL0_P0_3_GPIO           0x00000000
#define PINSEL0_P0_3_RXD0           0x00000040
#define PINSEL0_P0_3_AD0_6          0x00000080

#define PINSEL0_P0_4_MASK           0x00000300
#define PINSEL0_P0_4_GPIO           0x00000000
#define PINSEL0_P0_4_I2SRX_CLK      0x00000100
#define PINSEL0_P0_4_RD2            0x00000200
#define PINSEL0_P0_4_CAP2_0         0x00000300

#define PINSEL0_P0_5_MASK           0x00000C00
#define PINSEL0_P0_5_GPIO           0x00000000
#define PINSEL0_P0_5_I2SRX_WS       0x00000400
#define PINSEL0_P0_5_TD2            0x00000800
#define PINSEL0_P0_5_CAP2_1         0x00000C00

#define PINSEL0_P0_6_MASK           0x00003000
#define PINSEL0_P0_6_GPIO           0x00000000
#define PINSEL0_P0_6_I2SRX_SDA      0x00001000
#define PINSEL0_P0_6_SSEL1          0x00002000
#define PINSEL0_P0_6_MAT2_0         0x00003000

#define PINSEL0_P0_7_MASK           0x0000C000
#define PINSEL0_P0_7_GPIO           0x00000000
#define PINSEL0_P0_7_I2STX_CLK      0x00004000
#define PINSEL0_P0_7_SCK1           0x00008000
#define PINSEL0_P0_7_MAT2_1         0x0000C000

#define PINSEL0_P0_8_MASK           0x00030000
#define PINSEL0_P0_8_GPIO           0x00000000
#define PINSEL0_P0_8_I2STX_WS       0x00010000
#define PINSEL0_P0_8_MISO1          0x00020000
#define PINSEL0_P0_8_MAT2_2         0x00030000

#define PINSEL0_P0_9_MASK           0x000C0000
#define PINSEL0_P0_9_GPIO           0x00000000
#define PINSEL0_P0_9_I2STX_SDA      0x00040000
#define PINSEL0_P0_9_MOSI1          0x00080000
#define PINSEL0_P0_9_MAT2_3         0x000C0000

#define PINSEL0_P0_10_MASK          0x00300000
#define PINSEL0_P0_10_GPIO          0x00000000
#define PINSEL0_P0_10_TXD2          0x00100000
#define PINSEL0_P0_10_SDA2          0x00200000
#define PINSEL0_P0_10_MAT3_0        0x00300000

#define PINSEL0_P0_11_MASK          0x00C00000
#define PINSEL0_P0_11_GPIO          0x00000000
#define PINSEL0_P0_11_RXD2          0x00400000
#define PINSEL0_P0_11_SCL2          0x00800000
#define PINSEL0_P0_11_MAT3_1        0x00C00000

#define PINSEL0_P0_15_MASK          0xC0000000
#define PINSEL0_P0_15_GPIO          0x00000000
#define PINSEL0_P0_15_TXD1          0x40000000
#define PINSEL0_P0_15_SCK0          0x80000000
#define PINSEL0_P0_15_SCK           0xC0000000

//PINSEL1 register
#define PINSEL1_P0_16_MASK          0x00000003
#define PINSEL1_P0_16_GPIO          0x00000000
#define PINSEL1_P0_16_RXD1          0x00000001
#define PINSEL1_P0_16_SSEL0         0x00000002
#define PINSEL1_P0_16_SSEL          0x00000003

#define PINSEL1_P0_17_MASK          0x0000000C
#define PINSEL1_P0_17_GPIO          0x00000000
#define PINSEL1_P0_17_CTS1          0x00000004
#define PINSEL1_P0_17_MISO0         0x00000008
#define PINSEL1_P0_17_MISO          0x0000000C

#define PINSEL1_P0_18_MASK          0x00000030
#define PINSEL1_P0_18_GPIO          0x00000000
#define PINSEL1_P0_18_DCD1          0x00000010
#define PINSEL1_P0_18_MOSI0         0x00000020
#define PINSEL1_P0_18_MOSI          0x00000030

#define PINSEL1_P0_19_MASK          0x000000C0
#define PINSEL1_P0_19_GPIO          0x00000000
#define PINSEL1_P0_19_DSR1          0x00000040
#define PINSEL1_P0_19_SDA1          0x000000C0

#define PINSEL1_P0_20_MASK          0x00000300
#define PINSEL1_P0_20_GPIO          0x00000000
#define PINSEL1_P0_20_DTR1          0x00000100
#define PINSEL1_P0_20_SCL1          0x00000300

#define PINSEL1_P0_21_MASK          0x00000C00
#define PINSEL1_P0_21_GPIO          0x00000000
#define PINSEL1_P0_21_RI1           0x00000400
#define PINSEL1_P0_21_RD1           0x00000C00

#define PINSEL1_P0_22_MASK          0x00003000
#define PINSEL1_P0_22_GPIO          0x00000000
#define PINSEL1_P0_22_RTS1          0x00001000
#define PINSEL1_P0_22_TD1           0x00003000

#define PINSEL1_P0_23_MASK          0x0000C000
#define PINSEL1_P0_23_GPIO          0x00000000
#define PINSEL1_P0_23_AD0_0         0x00004000
#define PINSEL1_P0_23_I2SRX_CLK     0x00008000
#define PINSEL1_P0_23_CAP3_0        0x0000C000

#define PINSEL1_P0_24_MASK          0x00030000
#define PINSEL1_P0_24_GPIO          0x00000000
#define PINSEL1_P0_24_AD0_1         0x00010000
#define PINSEL1_P0_24_I2SRX_WS      0x00020000
#define PINSEL1_P0_24_CAP3_1        0x00030000

#define PINSEL1_P0_25_MASK          0x000C0000
#define PINSEL1_P0_25_GPIO          0x00000000
#define PINSEL1_P0_25_AD0_2         0x00040000
#define PINSEL1_P0_25_I2SRX_SDA     0x00080000
#define PINSEL1_P0_25_TXD3          0x000C0000

#define PINSEL1_P0_26_MASK          0x00300000
#define PINSEL1_P0_26_GPIO          0x00000000
#define PINSEL1_P0_26_AD0_3         0x00100000
#define PINSEL1_P0_26_AOUT          0x00200000
#define PINSEL1_P0_26_RXD3          0x00300000

#define PINSEL1_P0_27_MASK          0x00C00000
#define PINSEL1_P0_27_GPIO          0x00000000
#define PINSEL1_P0_27_SDA0          0x00400000
#define PINSEL1_P0_27_USB_SDA       0x00800000

#define PINSEL1_P0_28_MASK          0x03000000
#define PINSEL1_P0_28_GPIO          0x00000000
#define PINSEL1_P0_28_SCL0          0x01000000
#define PINSEL1_P0_28_USB_SCL       0x02000000

#define PINSEL1_P0_29_MASK          0x0C000000
#define PINSEL1_P0_29_GPIO          0x00000000
#define PINSEL1_P0_29_USB_DP        0x04000000

#define PINSEL1_P0_30_MASK          0x30000000
#define PINSEL1_P0_30_GPIO          0x00000000
#define PINSEL1_P0_30_USB_DM        0x10000000

//PINSEL2 register
#define PINSEL2_P1_0_MASK           0x00000003
#define PINSEL2_P1_0_GPIO           0x00000000
#define PINSEL2_P1_0_ENET_TXD0      0x00000001

#define PINSEL2_P1_1_MASK           0x0000000C
#define PINSEL2_P1_1_GPIO           0x00000000
#define PINSEL2_P1_1_ENET_TXD1      0x00000004

#define PINSEL2_P1_4_MASK           0x00000300
#define PINSEL2_P1_4_GPIO           0x00000000
#define PINSEL2_P1_4_ENET_TX_EN     0x00000100

#define PINSEL2_P1_8_MASK           0x00030000
#define PINSEL2_P1_8_GPIO           0x00000000
#define PINSEL2_P1_8_ENET_CRS       0x00010000

#define PINSEL2_P1_9_MASK           0x000C0000
#define PINSEL2_P1_9_GPIO           0x00000000
#define PINSEL2_P1_9_ENET_RXD0      0x00040000

#define PINSEL2_P1_10_MASK          0x00300000
#define PINSEL2_P1_10_GPIO          0x00000000
#define PINSEL2_P1_10_ENET_RXD1     0x00100000

#define PINSEL2_P1_14_MASK          0x30000000
#define PINSEL2_P1_14_GPIO          0x00000000
#define PINSEL2_P1_14_ENET_RX_ER    0x10000000

#define PINSEL2_P1_15_MASK          0xC0000000
#define PINSEL2_P1_15_GPIO          0x00000000
#define PINSEL2_P1_15_ENET_REF_CLK  0x40000000

//PINSEL3 register
#define PINSEL3_P1_16_MASK          0x00000003
#define PINSEL3_P1_16_GPIO          0x00000000
#define PINSEL3_P1_16_ENET_MDC      0x00000001

#define PINSEL3_P1_17_MASK          0x0000000C
#define PINSEL3_P1_17_GPIO          0x00000000
#define PINSEL3_P1_17_ENET_MDIO     0x00000004

#define PINSEL3_P1_18_MASK          0x00000030
#define PINSEL3_P1_18_GPIO          0x00000000
#define PINSEL3_P1_18_USB_UP_LED    0x00000010
#define PINSEL3_P1_18_PWM1_1        0x00000020
#define PINSEL3_P1_18_CAP1_0        0x00000030

#define PINSEL3_P1_19_MASK          0x000000C0
#define PINSEL3_P1_19_GPIO          0x00000000
#define PINSEL3_P1_19_MCOA0         0x00000040
#define PINSEL3_P1_19_USB_PPWR      0x00000080
#define PINSEL3_P1_19_CAP1_1        0x000000C0

#define PINSEL3_P1_20_MASK          0x00000300
#define PINSEL3_P1_20_GPIO          0x00000000
#define PINSEL3_P1_20_MCI0          0x00000100
#define PINSEL3_P1_20_PWM1_2        0x00000200
#define PINSEL3_P1_20_SCK0          0x00000300

#define PINSEL3_P1_21_MASK          0x00000C00
#define PINSEL3_P1_21_GPIO          0x00000000
#define PINSEL3_P1_21_MCABORT       0x00000400
#define PINSEL3_P1_21_PWM1_3        0x00000800
#define PINSEL3_P1_21_SSEL0         0x00000C00

#define PINSEL3_P1_22_MASK          0x00003000
#define PINSEL3_P1_22_GPIO          0x00000000
#define PINSEL3_P1_22_MCOB0         0x00001000
#define PINSEL3_P1_22_USB_PWRD      0x00002000
#define PINSEL3_P1_22_MAT1_0        0x00003000

#define PINSEL3_P1_23_MASK          0x0000C000
#define PINSEL3_P1_23_GPIO          0x00000000
#define PINSEL3_P1_23_MCI1          0x00004000
#define PINSEL3_P1_23_PWM1_4        0x00008000
#define PINSEL3_P1_23_MISO0         0x0000C000

#define PINSEL3_P1_24_MASK          0x00030000
#define PINSEL3_P1_24_GPIO          0x00000000
#define PINSEL3_P1_24_MCI2          0x00010000
#define PINSEL3_P1_24_PWM1_5        0x00020000
#define PINSEL3_P1_24_MOSI0         0x00030000

#define PINSEL3_P1_25_MASK          0x000C0000
#define PINSEL3_P1_25_GPIO          0x00000000
#define PINSEL3_P1_25_MCOA1         0x00040000
#define PINSEL3_P1_25_RESERVED      0x00080000
#define PINSEL3_P1_25_MAT1_1        0x000C0000

#define PINSEL3_P1_26_MASK          0x00300000
#define PINSEL3_P1_26_GPIO          0x00000000
#define PINSEL3_P1_26_MCOB1         0x00100000
#define PINSEL3_P1_26_PWM1_6        0x00200000
#define PINSEL3_P1_26_CAP0_0        0x00300000

#define PINSEL3_P1_27_MASK          0x00C00000
#define PINSEL3_P1_27_GPIO          0x00000000
#define PINSEL3_P1_27_CLKOUT        0x00400000
#define PINSEL3_P1_27_USB_OVRCR     0x00800000
#define PINSEL3_P1_27_CAP0_1        0x00C00000

#define PINSEL3_P1_28_MASK          0x03000000
#define PINSEL3_P1_28_GPIO          0x00000000
#define PINSEL3_P1_28_MCOA2         0x01000000
#define PINSEL3_P1_28_PCAP1_0       0x02000000
#define PINSEL3_P1_28_MAT0_0        0x03000000

#define PINSEL3_P1_29_MASK          0x0C000000
#define PINSEL3_P1_29_GPIO          0x00000000
#define PINSEL3_P1_29_MCOB2         0x04000000
#define PINSEL3_P1_29_PCAP1_1       0x08000000
#define PINSEL3_P1_29_MAT0_1        0x0C000000

#define PINSEL3_P1_30_MASK          0x30000000
#define PINSEL3_P1_30_GPIO          0x00000000
#define PINSEL3_P1_30_RESERVED      0x10000000
#define PINSEL3_P1_30_VBUS          0x20000000
#define PINSEL3_P1_30_AD0_4         0x30000000

#define PINSEL3_P1_31_MASK          0xC0000000
#define PINSEL3_P1_31_GPIO          0x00000000
#define PINSEL3_P1_31_RESERVED      0x40000000
#define PINSEL3_P1_31_SCK1          0x80000000
#define PINSEL3_P1_31_AD0_5         0xC0000000

//PINSEL4 register
#define PINSEL4_P2_0_MASK           0x00000003
#define PINSEL4_P2_0_GPIO           0x00000000
#define PINSEL4_P2_0_PWM1_1         0x00000001
#define PINSEL4_P2_0_TXD1           0x00000002

#define PINSEL4_P2_1_MASK           0x0000000C
#define PINSEL4_P2_1_GPIO           0x00000000
#define PINSEL4_P2_1_PWM1_2         0x00000004
#define PINSEL4_P2_1_RXD1           0x00000008

#define PINSEL4_P2_2_MASK           0x00000030
#define PINSEL4_P2_2_GPIO           0x00000000
#define PINSEL4_P2_2_PWM1_3         0x00000010
#define PINSEL4_P2_2_CTS1           0x00000020

#define PINSEL4_P2_3_MASK           0x000000C0
#define PINSEL4_P2_3_GPIO           0x00000000
#define PINSEL4_P2_3_PWM1_4         0x00000040
#define PINSEL4_P2_3_DCD1           0x00000080

#define PINSEL4_P2_4_MASK           0x00000300
#define PINSEL4_P2_4_GPIO           0x00000000
#define PINSEL4_P2_4_PWM1_5         0x00000100
#define PINSEL4_P2_4_DSR1           0x00000200

#define PINSEL4_P2_5_MASK           0x00000C00
#define PINSEL4_P2_5_GPIO           0x00000000
#define PINSEL4_P2_5_PWM1_6         0x00000400
#define PINSEL4_P2_5_DTR1           0x00000800

#define PINSEL4_P2_6_MASK           0x00003000
#define PINSEL4_P2_6_GPIO           0x00000000
#define PINSEL4_P2_6_PCAP1_0        0x00001000
#define PINSEL4_P2_6_RI1            0x00002000

#define PINSEL4_P2_7_MASK           0x0000C000
#define PINSEL4_P2_7_GPIO           0x00000000
#define PINSEL4_P2_7_RD2            0x00004000
#define PINSEL4_P2_7_RTS1           0x00008000

#define PINSEL4_P2_8_MASK           0x00030000
#define PINSEL4_P2_8_GPIO           0x00000000
#define PINSEL4_P2_8_TD2            0x00010000
#define PINSEL4_P2_8_TXD2           0x00020000
#define PINSEL4_P2_8_ENET_MDC       0x00030000

#define PINSEL4_P2_9_MASK           0x000C0000
#define PINSEL4_P2_9_GPIO           0x00000000
#define PINSEL4_P2_9_USB_CONNECT    0x00040000
#define PINSEL4_P2_9_RXD2           0x00080000
#define PINSEL4_P2_9_ENET_MDIO      0x000C0000

#define PINSEL4_P2_10_MASK          0x00300000
#define PINSEL4_P2_10_GPIO          0x00000000
#define PINSEL4_P2_10_EINT0         0x00100000
#define PINSEL4_P2_10_NMI           0x00200000

#define PINSEL4_P2_11_MASK          0x00C00000
#define PINSEL4_P2_11_GPIO          0x00000000
#define PINSEL4_P2_11_EINT1         0x00400000
#define PINSEL4_P2_11_I2STX_CLK     0x00C00000

#define PINSEL4_P2_12_MASK          0x03000000
#define PINSEL4_P2_12_GPIO          0x00000000
#define PINSEL4_P2_12_EINT2         0x01000000
#define PINSEL4_P2_12_I2STX_WS      0x03000000

#define PINSEL4_P2_13_MASK          0x0C000000
#define PINSEL4_P2_13_GPIO          0x00000000
#define PINSEL4_P2_13_EINT3         0x04000000
#define PINSEL4_P2_13_I2STX_SDA     0x0C000000

//PINSEL7 register
#define PINSEL7_P3_25_MASK          0x000C0000
#define PINSEL7_P3_25_GPIO          0x00000000
#define PINSEL7_P3_25_MAT0_0        0x00080000
#define PINSEL7_P3_25_PWM1_2        0x000C0000

#define PINSEL7_P3_26_MASK          0x00300000
#define PINSEL7_P3_26_GPIO          0x00000000
#define PINSEL7_P3_26_STCLK         0x00100000
#define PINSEL7_P3_26_MAT0_1        0x00200000
#define PINSEL7_P3_26_PWM1_3        0x00300000

//PINSEL9 register
#define PINSEL9_P4_28_MASK          0x03000000
#define PINSEL9_P4_28_GPIO          0x00000000
#define PINSEL9_P4_28_RX_MCLK       0x01000000
#define PINSEL9_P4_28_MAT2_0        0x02000000
#define PINSEL9_P4_28_TXD3          0x03000000

#define PINSEL9_P4_29_MASK          0x0C000000
#define PINSEL9_P4_29_GPIO          0x00000000
#define PINSEL9_P4_29_TX_MCLK       0x04000000
#define PINSEL9_P4_29_MAT2_1        0x08000000
#define PINSEL9_P4_29_RXD3          0x0C000000

//FLASHCFG register
#define FLASHCFG_RESET_VALUE        0x0000003A
#define FLASHCFG_FLASHTIM           0x0000F000
#define FLASHCFG_FLASHTIM_1CLK      0x00000000
#define FLASHCFG_FLASHTIM_2CLK      0x00001000
#define FLASHCFG_FLASHTIM_3CLK      0x00002000
#define FLASHCFG_FLASHTIM_4CLK      0x00003000
#define FLASHCFG_FLASHTIM_5CLK      0x00004000
#define FLASHCFG_FLASHTIM_6CLK      0x00005000

//CLKSRCSEL register
#define CLKSRCSEL_CLKSRC            0x00000003
#define CLKSRCSEL_CLKSRC_IRC        0x00000000
#define CLKSRCSEL_CLKSRC_OSC        0x00000001
#define CLKSRCSEL_CLKSRC_RTC        0x00000002

//SCS register
#define SCS_OSCSTAT                 0x00000040
#define SCS_OSCEN                   0x00000020
#define SCS_OSCRANGE                0x00000010
#define SCS_GPIOM                   0x00000001

//PLL0CON register
#define PLL0CON_PLLC0               0x00000002
#define PLL0CON_PLLE0               0x00000001

//PLL0CFG register
#define PLL0CFG_NSEL0               0x00FF0000
#define PLL0CFG_MSEL0               0x00007FFF

//PLL0STAT register
#define PLL0STAT_PLOCK0             0x04000000
#define PLL0STAT_PLLC0_STAT         0x02000000
#define PLL0STAT_PLLE0_STAT         0x01000000
#define PLL0STAT_NSEL0              0x00FF0000
#define PLL0STAT_MSEL0              0x00007FFF

//CLKOUTCFG register
#define CLKOUTCFG_CLKOUT_ACT        0x00000200
#define CLKOUTCFG_CLKOUT_EN         0x00000100
#define CLKOUTCFG_CLKOUTDIV         0x000000F0
#define CLKOUTCFG_CLKOUTSEL         0x0000000F
#define CLKOUTCFG_CLKOUTSEL_CCLK    0x00000000
#define CLKOUTCFG_CLKOUTSEL_OSC     0x00000001
#define CLKOUTCFG_CLKOUTSEL_IRC     0x00000002
#define CLKOUTCFG_CLKOUTSEL_USB     0x00000003
#define CLKOUTCFG_CLKOUTSEL_RTC     0x00000004

//PCLKSEL0 register
#define PCLKSEL0_PCLK_UART0         0x000000C0
#define PCLKSEL0_PCLK_UART0_DIV4    0x00000000
#define PCLKSEL0_PCLK_UART0_DIV1    0x00000040
#define PCLKSEL0_PCLK_UART0_DIV2    0x00000080
#define PCLKSEL0_PCLK_UART0_DIV8    0x000000C0

#define PCLKSEL0_PCLK_UART1         0x00000300
#define PCLKSEL0_PCLK_UART1_DIV4    0x00000000
#define PCLKSEL0_PCLK_UART1_DIV1    0x00000100
#define PCLKSEL0_PCLK_UART1_DIV2    0x00000200
#define PCLKSEL0_PCLK_UART1_DIV8    0x00000300

#define PCLKSEL0_PCLK_CAN1          0x0C000000
#define PCLKSEL0_PCLK_CAN1_DIV4     0x00000000
#define PCLKSEL0_PCLK_CAN1_DIV1     0x04000000
#define PCLKSEL0_PCLK_CAN1_DIV2     0x08000000
#define PCLKSEL0_PCLK_CAN1_DIV6     0x0C000000

#define PCLKSEL0_PCLK_CAN2          0x30000000
#define PCLKSEL0_PCLK_CAN2_DIV4     0x00000000
#define PCLKSEL0_PCLK_CAN2_DIV1     0x10000000
#define PCLKSEL0_PCLK_CAN2_DIV2     0x20000000
#define PCLKSEL0_PCLK_CAN2_DIV6     0x30000000

#define PCLKSEL0_PCLK_ACF           0xC0000000
#define PCLKSEL0_PCLK_ACF_DIV4      0x00000000
#define PCLKSEL0_PCLK_ACF_DIV1      0x40000000
#define PCLKSEL0_PCLK_ACF_DIV2      0x80000000
#define PCLKSEL0_PCLK_ACF_DIV6      0xC0000000

//PCLKSEL1 register
#define PCLKSEL1_PCLK_SSP0          0x00000C00
#define PCLKSEL1_PCLK_SSP0_DIV4     0x00000000
#define PCLKSEL1_PCLK_SSP0_DIV1     0x00000400
#define PCLKSEL1_PCLK_SSP0_DIV2     0x00000800
#define PCLKSEL1_PCLK_SSP0_DIV8     0x00000C00

#define PCLKSEL1_PCLK_I2C2          0x00300000
#define PCLKSEL1_PCLK_I2C2_DIV4     0x00000000
#define PCLKSEL1_PCLK_I2C2_DIV1     0x00100000
#define PCLKSEL1_PCLK_I2C2_DIV2     0x00200000
#define PCLKSEL1_PCLK_I2C2_DIV8     0x00300000

#define PCLKSEL1_PCLK_I2S           0x00C00000
#define PCLKSEL1_PCLK_I2S_DIV4      0x00000000
#define PCLKSEL1_PCLK_I2S_DIV1      0x00400000
#define PCLKSEL1_PCLK_I2S_DIV2      0x00800000
#define PCLKSEL1_PCLK_I2S_DIV8      0x00C00000

#define PCLKSEL1_PCLK_RIT           0x0C000000
#define PCLKSEL1_PCLK_RIT_DIV4      0x00000000
#define PCLKSEL1_PCLK_RIT_DIV1      0x04000000
#define PCLKSEL1_PCLK_RIT_DIV2      0x08000000
#define PCLKSEL1_PCLK_RIT_DIV8      0x0C000000

#define PCLKSEL1_PCLK_SYSCON        0x30000000
#define PCLKSEL1_PCLK_SYSCON_DIV4   0x00000000
#define PCLKSEL1_PCLK_SYSCON_DIV1   0x10000000
#define PCLKSEL1_PCLK_SYSCON_DIV2   0x20000000
#define PCLKSEL1_PCLK_SYSCON_DIV8   0x30000000

#define PCLKSEL1_PCLK_MC            0xC0000000
#define PCLKSEL1_PCLK_MC_DIV4       0x00000000
#define PCLKSEL1_PCLK_MC_DIV1       0x40000000
#define PCLKSEL1_PCLK_MC_DIV2       0x80000000
#define PCLKSEL1_PCLK_MC_DIV8       0xC0000000

//PCONP register
#define PCONP_PCUSB                 0x80000000
#define PCONP_PCENET                0x40000000
#define PCONP_PCGPDMA               0x20000000
#define PCONP_PCI2S                 0x08000000
#define PCONP_PCI2C2                0x04000000
#define PCONP_PCUART3               0x02000000
#define PCONP_PCUART2               0x01000000
#define PCONP_PCTIM3                0x00800000
#define PCONP_PCTIM2                0x00400000
#define PCONP_PCSSP0                0x00200000
#define PCONP_PCI2C1                0x00080000
#define PCONP_PCQEI                 0x00040000
#define PCONP_PCMCPWM               0x00020000
#define PCONP_PCRIT                 0x00010000
#define PCONP_PCGPIO                0x00008000
#define PCONP_PCCAN2                0x00004000
#define PCONP_PCCAN1                0x00002000
#define PCONP_PCADC                 0x00001000
#define PCONP_PCSSP1                0x00000400
#define PCONP_PCRTC                 0x00000200
#define PCONP_PCSPI                 0x00000100
#define PCONP_PCI2C0                0x00000080
#define PCONP_PCPWM1                0x00000040
#define PCONP_PCUART1               0x00000010
#define PCONP_PCUART0               0x00000008
#define PCONP_PCTIM1                0x00000004
#define PCONP_PCTIM0                0x00000002

//FCR register
#define FCR_RX_TRIGGER_LEVEL        0x000000C0
#define FCR_RX_TRIGGER_LEVEL_0      0x00000000
#define FCR_RX_TRIGGER_LEVEL_1      0x00000040
#define FCR_RX_TRIGGER_LEVEL_2      0x00000080
#define FCR_RX_TRIGGER_LEVEL_3      0x000000C0

#define FCR_DMA_MODE_SELECT         0x00000008
#define FCR_TX_FIFO_RESET           0x00000004
#define FCR_RX_FIFO_RESET           0x00000002
#define FCR_FIFO_ENABLE             0x00000001

//LCR register
#define LCR_DLAB                    0x00000080
#define LCR_BREAK_CONTROL           0x00000040

#define LCR_PARITY_SELECT           0x00000030
#define LCR_PARITY_SELECT_ODD       0x00000000
#define LCR_PARITY_SELECT_EVEN      0x00000010
#define LCR_PARITY_SELECT_1         0x00000020
#define LCR_PARITY_SELECT_0         0x00000030

#define LCR_PARITY_ENABLE           0x00000008

#define LCR_STOP_BIT_SELECT         0x00000004
#define LCR_STOP_BIT_SELECT_1       0x00000000
#define LCR_STOP_BIT_SELECT_2       0x00000004

#define LCR_WORD_LENGTH_SELECT      0x00000003
#define LCR_WORD_LENGTH_SELECT_5    0x00000000
#define LCR_WORD_LENGTH_SELECT_6    0x00000001
#define LCR_WORD_LENGTH_SELECT_7    0x00000002
#define LCR_WORD_LENGTH_SELECT_8    0x00000003

//LSR register
#define LSR_RXFE                    0x00000080
#define LSR_TEMT                    0x00000040
#define LSR_THRE                    0x00000020
#define LSR_BI                      0x00000010
#define LSR_FE                      0x00000008
#define LSR_PE                      0x00000004
#define LSR_OE                      0x00000002
#define LSR_RDR                     0x00000001

//CR0 register
#define CR0_DSS                     0x0000000F
#define CR0_DSS_4                   0x00000003
#define CR0_DSS_5                   0x00000004
#define CR0_DSS_6                   0x00000005
#define CR0_DSS_7                   0x00000006
#define CR0_DSS_8                   0x00000007
#define CR0_DSS_9                   0x00000008
#define CR0_DSS_10                  0x00000009
#define CR0_DSS_11                  0x0000000A
#define CR0_DSS_12                  0x0000000B
#define CR0_DSS_13                  0x0000000C
#define CR0_DSS_14                  0x0000000D
#define CR0_DSS_15                  0x0000000E
#define CR0_DSS_16                  0x0000000F

#define CR0_FRF                     0x00000030
#define CR0_FRF_SPI                 0x00000000
#define CR0_FRF_TI                  0x00000010
#define CR0_FRF_MICROWIRE           0x00000020

#define CR0_CPOL                    0x00000040
#define CR0_CPHA                    0x00000080
#define CR0_SCR                     0x0000FF00

//CR1 register
#define CR1_LBM                     0x00000001
#define CR1_SSE                     0x00000002
#define CR1_MS                      0x00000004
#define CR1_SOD                     0x00000008

//DR register
#define DR_DATA                     0x0000FFFF

//SR register
#define SR_TFE                      0x00000001
#define SR_TNF                      0x00000002
#define SR_RNE                      0x00000004
#define SR_RFF                      0x00000008
#define SR_BSY                      0x00000010

//CPSR register
#define CPSR_CPSDVSR                0x000000FF

//IMSC register
#define IMSC_RORIM                  0x00000001
#define IMSC_RTIM                   0x00000002
#define IMSC_RXIM                   0x00000004
#define IMSC_TXIM                   0x00000008

//RIS register
#define RIS_RORRIS                  0x00000001
#define RIS_RTRIS                   0x00000002
#define RIS_RXRIS                   0x00000004
#define RIS_TXRIS                   0x00000008

//MIS register
#define MIS_RORMIS                  0x00000001
#define MIS_RTMIS                   0x00000002
#define MIS_RXMIS                   0x00000004
#define MIS_TXMIS                   0x00000008

//ICR register
#define ICR_RORIC                   0x00000001
#define ICR_RTIC                    0x00000002

//DMACR register
#define DMACR_RXDMAE                0x00000001
#define DMACR_TXDMAE                0x00000002

//I2CONSET register
#define I2CONSET_AA                 0x00000004
#define I2CONSET_SI                 0x00000008
#define I2CONSET_STO                0x00000010
#define I2CONSET_STA                0x00000020
#define I2CONSET_I2EN               0x00000040

//I2CONCLR register
#define I2CONCLR_AAC                0x00000004
#define I2CONCLR_SIC                0x00000008
#define I2CONCLR_STAC               0x00000020
#define I2CONCLR_I2ENC              0x00000040

//I2STAT register
#define I2STAT_STATUS               0x000000F8

//I2DAT register
#define I2DAT_DATA                  0x000000FF

//I2MMCTRL register
#define I2MMCTRL_MM_ENA             0x00000001
#define I2MMCTRL_ENA_SCL            0x00000002
#define I2MMCTRL_MATCH_ALL          0x00000004

//I2DATA_BUFFER register
#define I2DATA_BUFFER_DATA          0x000000FF

//I2ADR0 register
#define I2ADR0_GC                   0x00000001
#define I2ADR0_ADDRESS              0x000000FE

//I2ADR1 register
#define I2ADR1_GC                   0x00000001
#define I2ADR1_ADDRESS              0x000000FE

//I2ADR2 register
#define I2ADR2_GC                   0x00000001
#define I2ADR2_ADDRESS              0x000000FE

//I2ADR3 register
#define I2ADR3_GC                   0x00000001
#define I2ADR3_ADDRESS              0x000000FE

//I2MASK0 register
#define I2MASK0_MASK                0x000000FE

//I2MASK1 register
#define I2MASK1_MASK                0x000000FE

//I2MASK2 register
#define I2MASK2_MASK                0x000000FE

//I2MASK3 register
#define I2MASK3_MASK                0x000000FE

//I2SCLH register
#define I2SCLH_SCLH                 0x0000FFFF

//I2SCLL register
#define I2SCLL_SCLL                 0x0000FFFF

//Interrupt sources bit allocation table
#define VIC_INT_I2S                 0x80000000
#define VIC_INT_I2C2                0x40000000
#define VIC_INT_UART3               0x20000000
#define VIC_INT_UART2               0x10000000
#define VIC_INT_TIMER3              0x08000000
#define VIC_INT_TIMER2              0x04000000
#define VIC_INT_GPDMA               0x02000000
#define VIC_INT_SD_MMC              0x01000000
#define VIC_INT_CAN1_CAN2           0x00800000
#define VIC_INT_USB                 0x00400000
#define VIC_INT_ETHERNET            0x00200000
#define VIC_INT_BOD                 0x00100000
#define VIC_INT_I2C1                0x00080000
#define VIC_INT_AD0                 0x00040000
#define VIC_INT_EINT3               0x00020000
#define VIC_INT_EINT2               0x00010000
#define VIC_INT_EINT1               0x00008000
#define VIC_INT_EINT0               0x00004000
#define VIC_INT_RTC                 0x00002000
#define VIC_INT_PLL                 0x00001000
#define VIC_INT_SSP1                0x00000800
#define VIC_INT_SPI_SSP0            0x00000400
#define VIC_INT_I2C0                0x00000200
#define VIC_INT_PWM1                0x00000100
#define VIC_INT_UART1               0x00000080
#define VIC_INT_UART0               0x00000040
#define VIC_INT_TIMER1              0x00000020
#define VIC_INT_TIMER0              0x00000010
#define VIC_INT_ARM_CORE1           0x00000008
#define VIC_INT_ARM_CORE0           0x00000004
#define VIC_INT_WDT                 0x00000001

#endif  // __LPC23xx_H

