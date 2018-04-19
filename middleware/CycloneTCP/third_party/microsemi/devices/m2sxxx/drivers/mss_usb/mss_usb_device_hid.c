/*******************************************************************************
 * (c) Copyright 2012-2013 Microsemi SoC Products Group.  All rights reserved.
 *
 * Microsemi Smartfusion2 MSS USB Driver Stack
 *      USB Logical Layer (USB-LL)
 *      USBD-HID class driver
 *
 * This file implements HID class functionality.
 * HID Mouse report is supported.
 *
 * SVN $Revision: 5468 $
 * SVN $Date: 2013-03-29 15:38:01 +0530 (Fri, 29 Mar 2013) $
 */

#include "mss_usb_device_hid.h"
#include "mss_usb_device.h"
#include "mss_usb_std_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
* Local functions.
*/
uint8_t* USBD_HID_get_descriptor_cb(uint8_t recepient,
                                    uint8_t type,
                                    uint32_t* length,
                                    mss_usb_device_speed_t musb_speed);

uint8_t USBD_HID_init_cb(uint8_t cfgidx, mss_usb_device_speed_t musb_speed);
uint8_t USBD_HID_deinit_cb(uint8_t cfgidx);

uint8_t USBD_HID_process_request_cb(mss_usbd_setup_pkt_t * setup_pkt,
                                    uint8_t** buf_pp,
                                    uint32_t* length);

uint8_t USBD_HID_data_in_cb(mss_usb_ep_num_t num, uint8_t status);
uint8_t USBD_HID_dataout_cb(mss_usb_ep_num_t num,
                            uint8_t status,
                            uint32_t rx_count);

uint8_t USBD_HID_cep_datain_cb(uint8_t status);
uint8_t USBD_HID_cep_dataout_cb(uint8_t status);

/* Call-back function structure definition needed by USB Device Core Driver */
mss_usbd_class_cb_t usbd_hid_cb = {
    USBD_HID_init_cb,
    USBD_HID_deinit_cb,
    USBD_HID_get_descriptor_cb,
    USBD_HID_process_request_cb,
    USBD_HID_data_in_cb,
    USBD_HID_dataout_cb,
    USBD_HID_cep_datain_cb,
    USBD_HID_cep_dataout_cb
};

uint32_t g_tx_complete_status = 1u;

/* USB current Speed of operation selected by user*/
mss_usb_device_speed_t g_usbd_hid_user_speed;

mss_usbd_hid_state_t g_hid_state = USBD_HID_NOT_CONFIGURED;

uint8_t hid_conf_descr[HID_CONFIG_DESCR_LENGTH] = {
    /*----------------------- Configuration Descriptor -----------------------*/
    USB_STD_CONFIG_DESCR_LEN,                           /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,                  /* bDescriptorType */
    0x22u,                                              /* wTotalLength LSB */
    0x00u,                                              /* wTotalLength MSB */
    0x01u,                                              /* bNumInterfaces */
    0x01u,                                              /* bConfigurationValue */
    0x04u,                                              /* iConfiguration */
    0xC0u,                                              /* bmAttributes */
    0x32u,                                              /* bMaxPower */
    /*------------------------- Interface Descriptor -------------------------*/
    USB_STD_INTERFACE_DESCR_LEN,                        /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                      /* bDescriptorType */
    0x00u,                                              /* bInterfaceNumber */
    0x00u,                                              /* bAlternateSetting */
    0x01u,                                              /* bNumEndpoints */
    0x03u,                                              /* bInterfaceClass */
    0x00u,                                              /* bInterfaceSubClass */
    0x02u,                                              /* bInterfaceProtocol */
    0x00u,                                              /* bInterface */
    /*---------------------------- HID Descriptor ----------------------------*/
    USB_HID_DESCR_LENGTH,                               /* bLength */
    0x21u,                                              /* bDescriptorType */
    0x10u,                                              /* bcdHID */
    0x01u,
    0x00u,                                              /* bCountryCode */
    0x01u,                                              /* bNumDescriptor */
    0x22u,                                              /* bDescriptorType */
    0x34u,                                              /* wDescriptorLength */
    0x00u,
    /*------------------------- Endpoint Descriptor --------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                         /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                       /* bDescriptorType */
    0x81u,                                              /* bEndpointAddress */
    0x03u,                                              /* bmAttributes */
    0x08u,                                              /* wMaxPacketSize LSB */
    0x00u,                                              /* wMaxPacketSize MSB */
    0x06u                                               /* bInterval */
};

/*
Report descriptor: sent to the host during enumeration process.
This descriptors defines the format of the HID report.
*/
uint8_t report_descr[] = {
    0x05u, 0x01u,
    0x09u, 0x02u,
    0xA1u, 0x01u,
    0x09u, 0x01u,
    0xA1u, 0x00u,
    0x05u, 0x09u,
    0x19u, 0x01u,
    0x29u, 0x03u,
    0x15u, 0x00u,
    0x25u, 0x01u,
    0x75u, 0x01u,
    0x95u, 0x03u,
    0x81u, 0x02u,
    0x75u, 0x05u,
    0x95u, 0x01u,
    0x81u, 0x01u,
    0x05u, 0x01u,
    0x09u, 0x30u,
    0x09u, 0x31u,
    0x09u, 0x38u,
    0x15u, 0x81u,
    0x25u, 0x7Fu,
    0x75u, 0x08u,
    0x95u, 0x03u,
    0x81u, 0x06u,
    0xC0u, 0xC0u
};

uint8_t*
USBD_HID_get_descriptor_cb
(
    uint8_t recepient,
    uint8_t type,
    uint32_t* length,
    mss_usb_device_speed_t musb_speed
)
{
    /*User Selected FS:
            Operate only in FS
      User Selected HS:
        Device connected to 2.0 Host(musb_speed = HS):Operate in HS
        Device connected to 1.x Host(musb_speed = FS):Operate in FS
    */

    /*
    Since Endpoint Size is wMaxpacketSize is 8, which is valid for both
    FS and HS, no need to make decision based on musb_speed.

    bInterval value is 6.
    For FS it results in 6mSec polling period
    For HS it results in 4mSec polling period
    Since this is OK for mouse app, we will return same configuration for
    Other Speed Config as well.
    */

    if(recepient == USB_STD_REQ_RECIPIENT_DEVICE )
    {
        if(type == USB_CONFIGURATION_DESCRIPTOR_TYPE)
        {
            hid_conf_descr[1u] = USB_CONFIGURATION_DESCRIPTOR_TYPE;
            *length = sizeof(hid_conf_descr);
            return(hid_conf_descr);
        }
        else if (type == USB_OTHER_SPEED_CONFIG_DESCRIPTOR_TYPE)
        {
            hid_conf_descr[1u] = USB_OTHER_SPEED_CONFIG_DESCRIPTOR_TYPE;
            *length = sizeof(hid_conf_descr);
            return(hid_conf_descr);
        }
    }
    else if( recepient == USB_STD_REQ_RECIPIENT_ENDPOINT )    /*Need index(EP Num) here*/
    {
        /*Do nothing*/
    }
    else if(recepient == USB_STD_REQ_RECIPIENT_INTERFACE)     /*Need index(intrfs num) here*/
    {
        if(type == USB_REPORT_DESCRIPTOR_TYPE)
        {
            *length = sizeof(report_descr);
            return(report_descr);
        }
        else if (type == USB_HID_DESCRIPTOR_TYPE)
        {
            *length = 9u;
            return(&hid_conf_descr[18u]);
        }
    }
    else
    {
        /*Do nothing*/
    }

    return USB_FAIL;
}

uint8_t
USBD_HID_init_cb
(
    uint8_t cfgidx,
    mss_usb_device_speed_t musb_speed
)
{
    /*
    Since Endpoint Size is wMaxpacketSize is 8, which is valid for both
    FS and HS, no need to make decision based on musb_speed
    */
    g_tx_complete_status = 1u;
    MSS_USBD_tx_ep_configure(HID_INTR_IN_EP,
                             HID_INTR_IN_EP_FIFO_ADDR,
                             HID_INTR_IN_EP_MAX_PKT_SIZE,
                             HID_INTR_IN_EP_MAX_PKT_SIZE,
                             1u,
                             DMA_DISABLE,
                             MSS_USB_DMA_CHANNEL1,
                             MSS_USB_XFR_INTERRUPT,
                             NO_ZLP_TO_XFR);

    g_hid_state = USBD_HID_CONFIGURED;
    return USB_SUCCESS;
}

uint8_t
USBD_HID_deinit_cb
(
    uint8_t cfgidx
)
{
    g_hid_state = USBD_HID_NOT_CONFIGURED;
    return 1;
}

uint8_t
USBD_HID_process_request_cb
(
    mss_usbd_setup_pkt_t * setup_pkt,
    uint8_t** buf_pp,
    uint32_t* length
)
{
    return USB_FAIL;
}

uint8_t
USBD_HID_data_in_cb
(
    mss_usb_ep_num_t num,
    uint8_t status
)
{
    if(status > 1u)
    {
        MSS_USBD_tx_ep_flush_fifo(HID_INTR_IN_EP);
    }
    else
    {
        g_tx_complete_status = 1u;
    }
    return 1;
}

uint8_t
USBD_HID_dataout_cb
(
    mss_usb_ep_num_t num,
    uint8_t status,
    uint32_t rx_count
)
{
    return USB_SUCCESS;
}

uint8_t
USBD_HID_cep_datain_cb
(
    uint8_t status
)
{
    return USB_SUCCESS;
}
uint8_t
USBD_HID_cep_dataout_cb
(
    uint8_t status
)
{
    return USB_SUCCESS;
}

/***************************************************************************//**
* Exported function from HID Class driver.
*******************************************************************************/

/***************************************************************************//**
* See mss_usb_device_hid.h for details of how to use this function.
*/
uint32_t
MSS_USBD_HID_tx_report
(
    uint8_t * buf,
    uint32_t length
)
{
    if(g_hid_state == USBD_HID_CONFIGURED)
    {
        g_tx_complete_status = 0u;
        MSS_USBD_tx_ep_write(HID_INTR_IN_EP, buf, length);
        return (1u);
    }
    else
    {
        return (0u);
    }
}

/***************************************************************************//**
* See mss_usb_device_hid.h for details of how to use this function.
*/
uint8_t
MSS_USBD_HID_tx_done
(
    void
)
{
    return(g_tx_complete_status);
}

/***************************************************************************//**
* See mss_usb_device_hid.h for details of how to use this function.
*/
void
MSS_USBD_HID_init
(
    mss_usb_device_speed_t speed
)
{
    g_usbd_hid_user_speed = speed;
    MSS_USBD_set_class_cb_handler(&usbd_hid_cb);
}

#ifdef __cplusplus
}
#endif
