/*******************************************************************************
 * (c) Copyright 2010 Actel Corporation.  All rights reserved.
 *
 * SmartFusion Microcontroller Subsystem RTC bare metal software driver public API.
 *
 * SVN $Revision: 2445 $
 * SVN $Date: 2010-03-05 23:45:42 +0000 (Fri, 05 Mar 2010) $
 */
/*=========================================================================*//**
  @mainpage SmartFusion MSS RTC Bare Metal Driver.

  @section intro_sec Introduction
  The SmartFusion Microcontroller Subsystem (MSS) includes a real time counter
  (RTC) allowing timing delays using a low power 32kHZ oscillator.
  This software driver provides a set of functions for controlling the MSS ACE
  block as part of a bare metal system where no operating system is available.
  This driver can be adapted for use as part of an operating system but the
  implementation of the adaptation layer between this driver and the operating
  system's driver model is outside the scope of this driver.
  
  @section hw_dependencies Hardware Flow Dependencies
  The configuration of all features of the MSS RTC is covered by this driver.
  The SmartFusion MSS configurator in the hardware design must be configured as
  follows, when using the RTC for SmartFusion sleep mode control:
    • The SmartFusion analog block must be powered up by enabling the ACE block
    • The SmartFusion internal voltage regulator must be powered up by configuring
      the Reset Management block to have the Voltage Regulator output at power up
      set to On.
  When the SmartFusion device is put into sleep mode, an RTC match event can be
  configured to switch the SmartFusion internal voltage regulator back on. This
  powers up to the SmartFusion device and brings it back out of sleep mode.

  
  @section theory_op Theory of Operation
  The MSS RTC driver uses the SmartFusion "Cortex Microcontroler Software
  Interface Standard - Peripheral Access Layer" (CMSIS-PAL) to access MSS hardware
  registers. You must ensure that the SmartFusion CMSIS-PAL is either included
  in the software toolchain used to build your project or is included in your
  project. The most up-to-date SmartFusion CMSIS-PAL files can be obtained using
  the Actel Firmware Catalog.
  
  The MSS RTC driver functions are grouped into the following categories:
    - Initiliazation
    - Configuration
    - Setting and reading the RTC current value
    - Setting the RTC match value
  
  The MSS RTC driver is initialized through a call to the MSS_RTC_init() function.
  The MSS_RTC_init() function must be called before any other RTC driver functions
  can be called. The MSS_RTC_init() function does not reset the RTC hardware so
  that the RTC counter value is preserved when the system is initialized after
  SmartFusion resets as a result of coming out of sleep mode.
  
  The SmartFusion RTC is configured through a call to the MSS_RTC_configure()
  function. This function is used to select:
    - Whether or not the RTC counter value should be reset when a RTC match
      event occurs.
    - Whether or not the SmartFusion internal voltage regulator should be powered
      back up when a RTC match event occurs.
  
  The RTC counter can be started and stopped using the follwoing functions:
    - MSS_RTC_start()
    - MSS_RTC_stop()
  
  The RTC current value can be set or read using the following functions:
    - MSS_RTC_set_rtc_count()
    - MSS_RTC_get_rtc_count()
    - MSS_RTC_set_seconds_count()
    - MSS_RTC_get_seconds_count()
    
  The RTC match value can be set using the following function:
    - MSS_RTC_set_rtc_match()
  
  The RTC interrupts are controlled uisng the following functions:
    - MSS_RTC_enable_irq() 
    - MSS_RTC_disable_irq()
    - MSS_RTC_clear_irq()
  
 *//*=========================================================================*/
#ifndef __MSS_RTC_H_
#define __MSS_RTC_H_    1

#ifdef __cplusplus
extern "C" {
#endif 

#include <stdint.h>

/***************************************************************************//**
  The MSS_RTC_NO_COUNTER_RESET constant is used to specify the configuration
  parameter to the MSS_RTC_configure() function. It is used to specify that the
  RTC count value must not be reset when an RTC match occurs.
 */
#define MSS_RTC_NO_COUNTER_RESET                    0x00u

/***************************************************************************//**
  The MSS_RTC_RESET_COUNTER_ON_MATCH constant is used to specify the
  configuration parameter to the MSS_RTC_configure() function. It is used to
  specify that the RTC count value must be reset when an RTC match occurs.
 */
#define MSS_RTC_ENABLE_VOLTAGE_REGULATOR_ON_MATCH   0x20u

/***************************************************************************//**
  The MSS_RTC_ENABLE_VOLTAGE_REGULATOR_ON_MATCH constant is used to specify the
  configuration parameter to the MSS_RTC_configure() function. It is used to
  specify that the SmartFusion voltage regulator must be enabled when an RTC
  match occurs. This is typically used to wake the Smartfusion device from sleep
  mode after a delay timed using the RTC.
 */
#define MSS_RTC_RESET_COUNTER_ON_MATCH              0x04u
  
/***************************************************************************//**
  The MSS_RTC_init() function initializes the RTC driver without resetting the
  RTC hardware. This function clears any pended RTC match interrupts. The
  MSS_RTC_init() function does not reset the RTC hardware so that the RTC
  counter value is preserved when the system is initialized after SmartFusion
  resets as a result of coming out of sleep mode.
 */
void MSS_RTC_init( void );

/***************************************************************************//**
  The MSS_RTC_configure() function configures the real time counter behavior. It
  is used to select whether the voltage regulator should be enabled or not when
  an RTC match occurs and whether the RTC counter should reset to zero when a
  RTC match occurs.
  
  @param configuration
    The configuration parameter specifies the configuration to be applied to the
    RTC. It is a logical OR of the counter value behavior when a RTC match
    occurs and the effect of an RTC match on the voltage regulator. 
    The RTC count value can be reset or not when a RTC match occurs by using one
    of the following defined constants:
        - MSS_RTC_NO_COUNTER_RESET
        - MSS_RTC_RESET_COUNTER_ON_MATCH
    The SmartFusion voltage regulator can be enabled when a RTC match occurs.
    This is typically used to wake the Smartfusion device from sleep mode after
    a delay timed using the RTC. The following defined constant can be used as
    parameter to enable this behavior:
        - MSS_RTC_ENABLE_VOLTAGE_REGULATOR_ON_MATCH

  
  Example:
  The following call to MSS_RTC_configure() will cause the RTC count to keep
  incrementing when an RTC match occurs. It will also cause the voltage
  regulator to become enabled causing SmartFusion to be powered back up if it
  was in sleep mode.
  
  @code
    MSS_RTC_configure( MSS_RTC_NO_COUNTER_RESET | MSS_RTC_ENABLE_VOLTAGE_REGULATOR_ON_MATCH );
  @endcode
 */
void MSS_RTC_configure( uint8_t configuration );

/***************************************************************************//**
  The MSS_RTC_start() function starts the RTC incrementing.
 */
void MSS_RTC_start( void );

/***************************************************************************//**
  The MSS_RTC_stop(() function stops the RTC from incrementing.
 */
void MSS_RTC_stop( void );

/***************************************************************************//**
  The MSS_RTC_set_rtc_count() function sets the current value of the RTC counter.
  
  @param new_rtc_value
    The new_rtc_value parameter contains the 40 bit value that will be used as
    the new RTC counter value.
 */
void MSS_RTC_set_rtc_count( uint64_t new_rtc_value);

/***************************************************************************//**
  The MSS_RTC_set_rtc_match() function sets the match value of the RTC. The RTC
  match value is the value the RTC count must reach for the RTC to generate a
  RTC match event. The RTC event can be used to generate an RTC match interrupt
  or turn on the voltage regulator.
  Note: You should ensure that the match value is greater than the current count
        value. Otherwise you may trigger an unintended RTC match event, as the
        new match value is written by 5 individual match register writes in the
        order MSByte to LSByte of the new match value.
  
  @param match_value
    The new_rtc_value parameter contains the 40 bit value that will be used as
    the new RTC counter macth value.
 */
void MSS_RTC_set_rtc_match( uint64_t match_value);

/***************************************************************************//**
  The MSS_RTC_get_rtc_count() function returns the current value of th RTC
  count.
  
  @return
    This function returns the 40 bit current value of the RTC counter.
  
  Note:
    This function disables interrupts for a brief period of time by modifying
    the Cortex-M3 PRIMASK register. This results in this function not being
    callable in Cortex-M3 user access level. This may be an issue if using this
    driver as part of an operating system. It is recommended to use the
    MSS_RTC_get_seconds_count() function instead of MSS_RTC_get_rtc_count()
    unless the value of the least significant 8 bits of the RTC counter are
    required.
 */
uint64_t MSS_RTC_get_rtc_count( void );

/***************************************************************************//**
  The MSS_RTC_set_seconds_count() function sets the current value of the RTC
  counter to the number of seconds passed as parameter.
  Note: This function assumes a 32.768KHz external crystal.
  
  @param new_seconds_count
    The new_seconds_count parameter contains the number of seconds that will be
    used to set the new current value of the RTC counter.
 */
void MSS_RTC_set_seconds_count( uint32_t new_seconds_count );

/***************************************************************************//**
  The MSS_RTC_get_seconds_count() function returns the number of seconds
  equivalent to the current RTC counter value.
  Note: This function assumes a 32.768KHz external crystal.
  
  @return
    This function returns the number of seconds equivalent to the RTC current
    value.
 */
uint32_t MSS_RTC_get_seconds_count( void );

/***************************************************************************//**
  The MSS_RTC_enable_irq() function enables the RTC match interrupt
  (RTC_Match_IRQn) in the Cortex-M3 interrupt controller.
  The RTC_Match_IRQHandler() function will be called when an RTC match interrupt
  occurs.
  Note: A RTC_Match_IRQHandler() default implementation is defined, with weak
        linkage, in the SmartFusion CMSIS-PAL. You must provide your own
        implementation of the RTC_Match_IRQHandler() function, that will override
        the default implementation, to suit your application.
 */
void MSS_RTC_enable_irq( void );

/***************************************************************************//**
  The MSS_RTC_disable_irq() function disables the RTC match interrupt
  (RTC_Match_IRQn) in the Cortex-M3 interrupt controller.
 */
void MSS_RTC_disable_irq( void );

/***************************************************************************//**
  The MSS_RTC_clear_irq() function clears pending RTC match interrupts. This
  function also clears the interrupt in the Cortex-M3 interrupt controller.
  Note: You must call the MSS_RTC_clear_irq() function as part of your
        implementation of the RTC_Match_IRQHandler() RTC match interrupt service
        routine (ISR) in order to prevent the same interrupt event retriggering
        a call to the ISR.
 */
void MSS_RTC_clear_irq( void );

#ifdef __cplusplus
}
#endif


#endif  /* __MSS_RTC_H_ */
