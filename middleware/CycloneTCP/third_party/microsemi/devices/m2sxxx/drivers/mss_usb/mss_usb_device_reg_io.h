/*******************************************************************************
 * (c) Copyright 2012-2013 Microsemi SoC Products Group.  All rights reserved.
 *
 *   * Microsemi Smartfusion2 MSS USB Driver Stack
 *      USB Core Interface Layer (USB-CIFL)
 *          USBD-CIF
 *
 *  This file provides interfaces to perform register bit level
 *  I\O operations in USB Device mode
 *
 * SVN $Revision: 5468 $
 * SVN $Date: 2013-03-29 15:38:01 +0530 (Fri, 29 Mar 2013) $
 */

#ifndef __MSS_USB_DEVICE_REG_IO_H_
#define __MSS_USB_DEVICE_REG_IO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../CMSIS/m2sxxx.h"
#include "mss_usb_core_regs.h"


/*******************************************************************************
* Device mode functions
*/

/*******************************************************************************
 * POWER register related APIs
 */

static __INLINE uint8_t
MSS_USBD_CIF_is_suspend_mode
(
    void
)
{
    return(((USB->POWER & POWER_REG_SUSPEND_MODE_MASK) ?
            MSS_USB_BOOLEAN_TRUE : MSS_USB_BOOLEAN_FALSE));
}

/*To force out of the Suspend while in Device mode.*/
static __INLINE void
MSS_USBD_CIF_resume_gen
(
    void
)
{
    USB->POWER |= POWER_REG_RESUME_SIGNAL_MASK;
    /*clear after 10ms*/
}

/*******************************************************************************
 * DEVCTL register related APIs
 */
static __INLINE uint8_t
MSS_USB_OTG_is_session_on
(
    void
)
{
    return (((USB->DEV_CTRL & DEV_CTRL_SESSION_MASK) ?
            MSS_USB_BOOLEAN_TRUE : MSS_USB_BOOLEAN_FALSE));
}

static __INLINE void
MSS_USB_OTG_initiate_srp
(
    void
)
{
    /*
    TODO:make sure that the device is suspended and is a Bdevice when this
    function is called
    */
    USB->DEV_CTRL |= DEV_CTRL_SESSION_MASK;
}

static __INLINE void
MSS_USB_OTG_initiate_hnp
(
    void
)
{
    /*TODO:make sure that the device is Bdevice when this function is called*/
    USB->DEV_CTRL |= DEV_CTRL_HOST_REQ_MASK;
}

#ifdef __cplusplus
}
#endif

#endif  /* __MSS_USB_DEVICE_REG_IO_H_ */
