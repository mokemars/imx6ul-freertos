/*******************************************************************************
 * (c) Copyright 2012-2013 Microsemi SoC Products Group.  All rights reserved.
 *
 * Smartfusion2 MSS USB Driver Stack
 *
 * MSS USB Driver stack configuration parameters.
 * User must choose the constant definitions in this file to select the mode
 * operation.
 * The constants defined in this file are used by MSS USB driver stack to
 * function as per configuration.
 *
 * SVN $Revision: 5468 $
 * SVN $Date: 2013-03-29 15:38:01 +0530 (Fri, 29 Mar 2013) $
 */

#ifndef __MSS_USB_CONFIG_H_
#define __MSS_USB_CONFIG_H_

/***************************************************************************//**
  User should choose the Mode in which Smartfusion2 MSS USB should operate
*/
/* #define MSS_USB_OTG_DUAL_ROLE_MODE */
/* #define MSS_USB_OTG_PERIPHERAL_MODE*/
/* #define MSS_USB_OTG_HOST_MODE */
   #define MSS_USB_PERIPHERAL_MODE
/* #define MSS_USB_DEVICE_TEST_MODE */

/***************************************************************************//**
   Definitions Internally generated for use in the Core and logical layer.
*/
#ifdef MSS_USB_OTG_DUAL_ROLE_MODE
    #define MSS_USB_HOST_ENABLED
    #define MSS_USB_DEVICE_ENABLED
    #define MSS_USB_OTG_SRP_ENABLED
    #define MSS_USB_OTG_HNP_ENABLED
#endif

#ifdef MSS_USB_OTG_PERIPHERAL_MODE
    #define MSS_USB_DEVICE_ENABLED
    #define MSS_USB_OTG_SRP_ENABLED
#endif

#ifdef MSS_USB_PERIPHERAL_MODE
    #define MSS_USB_DEVICE_ENABLED
#endif

#ifdef MSS_USB_OTG_HOST_MODE
    #define MSS_USB_HOST_ENABLED
    #define MSS_USB_OTG_SRP_ENABLED
#endif

#endif  /* __MSS_USB_CONFIG_H_ */
