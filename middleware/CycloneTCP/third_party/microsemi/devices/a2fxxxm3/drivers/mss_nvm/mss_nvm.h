/*******************************************************************************
 * (c) Copyright 2009-2011 Actel Corporation.  All rights reserved.
 *
 * SmartFusion Microcontroller Subsystem ENVM bare metal software driver public API.
 *
 * SVN $Revision: 3764 $
 * SVN $Date: 2011-07-29 11:21:30 +0530 (Fri, 29 Jul 2011) $
 */
/*=========================================================================*//**
  @mainpage SmartFusion MSS NVM Bare Metal Driver.

  The SmartFusion MSS NVM driver provides the ability to program the content of
  the SmartFusion NVM.

  @section intro_sec Introduction
  The SmartFusion™ microcontroller subsystem (MSS) includes non-volatile memory
  (NVM). This driver provides a set of functions for controlling the MSS NVM as
  part of a bare metal system where no operating system is available. This
  driver can be adapted for use as part of an operating system, but the
  implementation of the adaptation layer between this driver and the operating
  system’s driver model is outside the scope of this driver.

  @section hw_dependencies Hardware Flow Dependencies
  The size of the MSS NVM varies with different SmartFusion device types. You must
  call the NVM_init() function to configure the driver for the required device
  type. The driver supports the A2F060, A2F200 and A2F500 SmartFusion devices.
  We recommend that you do not write to the spare pages region of the MSS NVM
  as it contains system boot code and configuration data.
  The SmartFusion MSS configurator also reserves a variable number of address
  locations, for configuration data, at the top of the MSS NVM’s user array
  address space. The MSS NVM configuration user’s guide describes how to
  calculate the address range of these reserved locations. We recommend that
  you do not write to these reserved locations.

  @section theory_op Theory of Operation
  The MSS NVM software driver is designed to provide write access from the
  application code to the SmartFusion nonvolatile memory (NVM).

  Initialization
  The NVM size varies with different SmartFusion device types. You must call the
  NVM_init() function to specify the device type before calling any other driver
  functions. If the NVM_init() function is not called, the driver will assume that
  the SmartFusion device is A2F200 device.

  Writing to the NVM
  The NVM_write() function uses a set of commands understood by the MSS NVM
  hardware block to set the NVM into write mode. It then writes the
  requested data into the NVM before setting the NVM back into read mode.
 *//*=========================================================================*/

#ifndef MSS_NVM_H_
#define MSS_NVM_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif 

/*-------------------------------------------------------------------------*//**
  The nvm_dev_type_t enumeration is used to specify the SmartFusion device
  type. It provides the allowed values for the NVM_init() function’s
  device_type parameter.
 */
typedef enum nvm_dev_type
{
    NVM_A2F060_DEVICE = 0,
    NVM_A2F200_DEVICE,
    NVM_A2F500_DEVICE
} nvm_dev_type_t ;

/*-------------------------------------------------------------------------*//**
  The nvm_status_t enumeration specifies the possible return values from the
  NVM_write() function.
 */
typedef enum nvm_status
{
    NVM_SUCCESS = 0,
    NVM_PROTECTION_ERROR,
    NVM_WRITE_ERROR,
    NVM_INVALID_ADDRESS
} nvm_status_t;

/*-------------------------------------------------------------------------*//**
   The NVM_init() function is used to specify which type of SmartFusion device
   is used in the hardware design. The NVM size varies with different
   SmartFusion device types. The driver is configured with the correct NVM
   memory boundaries based on the specified device type.
   The supported device types are:
     NVM_A2F060_DEVICE
     NVM_A2F200_DEVICE
     NVM_A2F500_DEVICE

   @param device_type
    The device_type parameter specifies the SmartFusion device type.
    The possible device types are:
      NVM_A2F060_DEVICE
      NVM_A2F200_DEVICE
      NVM_A2F500_DEVICE

   @return
     none
 */
void
NVM_init
(
    nvm_dev_type_t device_type
);

/*-------------------------------------------------------------------------*//**
   This function writes the content of the buffer passed as the p_data
   parameter to the SmartFusion MSS NVM. The data is written to the memory
   location specified by the start_addr parameter. This address is the absolute
   address in the processor's memory space at which the NVM is located. Any
   address within the NVM address space that is valid for the selected
   SmartFusion device is permitted as a write address.
   The NVM is logically organized as 4 address spaces: the NVM array, NVM spare
   pages, NVM array auxiliary block (array AUX block), and NVM spare pages
   auxiliary block (spare pages AUX block). The NVM_write() function supports
   the writing of a data buffer to contiguous addresses within each of these 4
   NVM address spaces, but not across the boundaries between these address spaces.
   Some regions of the NVM address spaces are either write-protected or are not
   recommended for write operations. The eNVM chapter of the SmartFusion MSS
   User’s Guide describes these write restrictions in detail, and they are
   summarized as follows:
   - NVM array address space:
     The SmartFusion MSS configurator reserves a variable number of address
     locations, for configuration data, at the top of the NVM user array address
     space. The MSS NVM configuration user’s guide describes how to calculate the
     address range of these reserved locations. We recommend that you do not write
     to these reserved locations.
   - NVM spare pages address space:
     We recommend that you do not write to the spare pages region of the MSS NVM
     as it contains system boot code and configuration data. Furthermore, the first
     2048 address locations (pages 0-15) at the bottom of the spare pages address
     space are write-protected.
   - NVM array AUX block address space:
     There are no write restrictions in this address space.

   - NVM spare pages AUX block address space:
     The first 64 address locations (pages 0-15) at the bottom of the spare pages
     AUX block address space are write-protected. The last 4 address locations
     (page 63) at the top of both the eNVM0 and the eNVM1 spare pages AUX block
     address spaces are read only. For an A2F500 device, a contiguous write that
     crosses the boundary between eNVM0 and eNVM1 is not possible in this address
     space,because of these read-only locations at the top of eNVM0.

   @param start_addr
     This is the start address to which data will be written. This address is the
     absolute address in the processor's memory space at which the NVM is located.
     Any address within the NVM address space that is valid for the selected
     SmartFusion device is permitted as a write address.

   @param p_data
     This is a pointer to the buffer holding the data to be written into NVM.

   @param nb_bytes
     This is the number of bytes to be written into NVM.

   @return
     The return value indicates if the write was successful.
     Possible values are:
     - NVM_SUCCESS
     - NVM_PROTECTION_ERROR
     - NVM_WRITE_ERROR
     - NVM_INVALID_ADDRESS
  */
nvm_status_t
NVM_write
(
     uint32_t start_addr,
     const uint8_t * p_data,
     size_t nb_bytes
);

#ifdef __cplusplus
}
#endif

#endif /*MSS_NVM_H_*/
