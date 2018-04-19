/*******************************************************************************
 * (c) Copyright 2010 Actel Corporation.  All rights reserved.
 *
 * SmartFusion Microcontroller Subsystem RTC bare metal software driver
 * implementation.
 *
 * SVN $Revision: 2433 $
 * SVN $Date: 2010-03-05 16:45:32 +0000 (Fri, 05 Mar 2010) $
 */
#include "mss_rtc.h"
#include "../../CMSIS/a2fxxxm3.h"
#include "../../CMSIS/mss_assert.h"

/***************************************************************************//**
  Maximum possible value of the RTC.
 */
#define RTC_MAX_VALUE   0x000000FFFFFFFFFFuLL

/***************************************************************************//**
  Register bit masks.
 */
#define CTRL_STAT_RTC_RST           0x80u
#define CTRL_STAT_CNTR_EN           0x40u
#define CTRL_STAT_VR_EN_MAT         0x20u
#define CTRL_STAT_RST_CNT_OMAT      0x04u
#define CTRL_STAT_RSTB_CNT          0x02u
#define CTRL_STAT_XTAL_EN           0x01u

#define RTC_CONFIG_BITS_MASK    ( CTRL_STAT_VR_EN_MAT | CTRL_STAT_RST_CNT_OMAT )

#define CLR_MSS_SR_RTC_MATCH_IRQ    0x01u

/***************************************************************************//**
  See "mss_rtc.h" for details of how to use this function.
 */
void MSS_RTC_init( void )
{
    SYSREG->CLR_MSS_SR |= CLR_MSS_SR_RTC_MATCH_IRQ;
    NVIC_ClearPendingIRQ( RTC_Match_IRQn );
    
    RTC->CTRL_STAT_REG |= CTRL_STAT_XTAL_EN;
    RTC->CTRL_STAT_REG |= CTRL_STAT_RSTB_CNT;
}

/***************************************************************************//**
  See "mss_rtc.h" for details of how to use this function.
 */
void MSS_RTC_configure( uint8_t configuration )
{
    uint32_t hw_config;
    
    hw_config = RTC->CTRL_STAT_REG;
    configuration &= (uint8_t)RTC_CONFIG_BITS_MASK;
    hw_config = hw_config & ~(uint32_t)RTC_CONFIG_BITS_MASK;
    hw_config = hw_config | (uint32_t)configuration;
    RTC->CTRL_STAT_REG = hw_config;
}

/***************************************************************************//**
  See "mss_rtc.h" for details of how to use this function.
 */
void MSS_RTC_start( void )
{
    RTC->CTRL_STAT_REG |= CTRL_STAT_CNTR_EN;
}

/***************************************************************************//**
  See "mss_rtc.h" for details of how to use this function.
 */
void MSS_RTC_stop( void )
{
    RTC->CTRL_STAT_REG &= ~(uint32_t)CTRL_STAT_CNTR_EN;
}

/***************************************************************************//**
  See "mss_rtc.h" for details of how to use this function.
 */
void MSS_RTC_set_rtc_count( uint64_t new_rtc_value )
{
    uint32_t ctrl_stat;
    
    ASSERT( new_rtc_value <=  RTC_MAX_VALUE );
    
    /* Stop RTC. */
    ctrl_stat = RTC->CTRL_STAT_REG;
    RTC->CTRL_STAT_REG &= ~(uint32_t)CTRL_STAT_CNTR_EN;
    
    /* Set new RTC count value */
    RTC->COUNTER0_REG = (uint32_t)new_rtc_value;
    RTC->COUNTER1_REG = (uint32_t)(new_rtc_value >> 8u);
    RTC->COUNTER2_REG = (uint32_t)(new_rtc_value >> 16u);
    RTC->COUNTER3_REG = (uint32_t)(new_rtc_value >> 24u);
    RTC->COUNTER4_REG = (uint32_t)(new_rtc_value >> 32u);
    
    /* Restore RTC configuration, whether running or stopped. */
    RTC->CTRL_STAT_REG = ctrl_stat;
}

/***************************************************************************//**
  See "mss_rtc.h" for details of how to use this function.
 */
uint64_t MSS_RTC_get_rtc_count( void )
{
    uint64_t rtc_count;
    uint32_t irq_disabled;

    /*
     * SAR 17079. No interrupt must be allowed to occur between reading the least
     * significant byte of the RTC count and reading the second byte in order to
     * preserve 40 bit counter value coherency.
     */
    irq_disabled = __get_PRIMASK();
    __disable_irq();
    
    rtc_count = RTC->COUNTER0_REG;
    rtc_count = rtc_count | (RTC->COUNTER1_REG << 8u);
     
    /* Only re-enable interrupts if they were enabled before the call to this function. */
    if ( !irq_disabled)
    {
        __enable_irq();
    }
    
    rtc_count = rtc_count | (RTC->COUNTER2_REG << 16u);
    rtc_count = rtc_count | (RTC->COUNTER3_REG << 24u);
    rtc_count = rtc_count | ((uint64_t)RTC->COUNTER4_REG << 32u);
    
    return rtc_count;
}

/***************************************************************************//**
  See "mss_rtc.h" for details of how to use this function.
 */
void MSS_RTC_set_rtc_match( uint64_t match_value)
{
    ASSERT( match_value <=  RTC_MAX_VALUE );
    
    /* Set new RTC count value */
    RTC->MATCHREG4_REG = (uint32_t)(match_value >> 32u);
    RTC->MATCHREG3_REG = (uint32_t)(match_value >> 24u);
    RTC->MATCHREG2_REG = (uint32_t)(match_value >> 16u);
    RTC->MATCHREG1_REG = (uint32_t)(match_value >> 8u);
    RTC->MATCHREG0_REG = (uint32_t)match_value;
}

/***************************************************************************//**
  See "mss_rtc.h" for details of how to use this function.
 */
void MSS_RTC_set_seconds_count( uint32_t new_seconds_count )
{
    uint32_t ctrl_stat;
    
    /* Stop RTC. */
    ctrl_stat = RTC->CTRL_STAT_REG;
    RTC->CTRL_STAT_REG &= ~(uint32_t)CTRL_STAT_CNTR_EN;
    
    /* Set new RTC count value */
    RTC->COUNTER0_REG = 0u;
    RTC->COUNTER1_REG = (uint32_t)new_seconds_count;
    RTC->COUNTER2_REG = (uint32_t)(new_seconds_count >> 8u);
    RTC->COUNTER3_REG = (uint32_t)(new_seconds_count >> 16u);
    RTC->COUNTER4_REG = (uint32_t)(new_seconds_count >> 24u);
    
    /* Restore RTC configuration, whether running or stopped. */
    RTC->CTRL_STAT_REG = ctrl_stat;
}

/***************************************************************************//**
  See "mss_rtc.h" for details of how to use this function.
 */
uint32_t MSS_RTC_get_seconds_count( void )
{
    uint32_t seconds;
    
    /* Dummy read of COUNTER0_REG to latch counter value. */
    seconds = RTC->COUNTER0_REG;
    ++seconds;
    
    /* Read number of seconds elapsed by ignoring lowest 8 bits of counter. */
    seconds = RTC->COUNTER1_REG;
    seconds = seconds | (RTC->COUNTER2_REG << 8u);
    seconds = seconds | (RTC->COUNTER3_REG << 16u);
    seconds = seconds | (RTC->COUNTER4_REG << 24u);
    
    return seconds;
}

/***************************************************************************//**
  See "mss_rtc.h" for details of how to use this function.
 */
void MSS_RTC_enable_irq( void )
{
    NVIC_EnableIRQ( RTC_Match_IRQn );
}

/***************************************************************************//**
  See "mss_rtc.h" for details of how to use this function.
 */
void MSS_RTC_disable_irq( void )
{
    NVIC_DisableIRQ( RTC_Match_IRQn );
}

/***************************************************************************//**
  See "mss_rtc.h" for details of how to use this function.
 */
void MSS_RTC_clear_irq( void )
{
	SYSREG->CLR_MSS_SR |= CLR_MSS_SR_RTC_MATCH_IRQ;
    NVIC_ClearPendingIRQ( RTC_Match_IRQn );
}
