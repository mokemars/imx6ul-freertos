/***************************************************************************//**
 * (c) Copyright 2012-2013 Microsemi SoC Products Group.  All rights reserved.
 *
 * Smartfusion2 MSS USB Driver Stack
 *      USB Logical Layer (USB-LL)
 *          USBD-MSC class driver.
 *
 * USBD-CDC class driver implementation:
 * This file implements CDC class Functionality.
 * Virtual COM-Port functionality is supported.
 *
 * SVN $Revision: 5468 $
 * SVN $Date: 2013-03-29 15:38:01 +0530 (Fri, 29 Mar 2013) $
 */

#include "mss_usb_device_cdc.h"
#include "mss_usb_device.h"
#include "mss_usb_std_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * Private Functions
 */
uint8_t* CDC_get_descriptor_cb(uint8_t recepient,
                               uint8_t type,
                               uint32_t* length,
                               mss_usb_device_speed_t musb_speed);

 uint8_t CDC_init_cb(uint8_t cfgidx, mss_usb_device_speed_t musb_speed);
uint8_t CDC_deinit_cb(uint8_t cfgidx);

uint8_t CDC_process_request_cb(mss_usbd_setup_pkt_t* setup_pkt,
                               uint8_t** buf_pp,
                               uint32_t* length);

uint8_t CDC_datain_cb(mss_usb_ep_num_t num,
                      uint8_t status);

uint8_t CDC_dataout_cb(mss_usb_ep_num_t num,
                       uint8_t status,
                       uint32_t rx_count);

uint8_t CDC_cep_datain_cb(uint8_t status);
uint8_t CDC_cep_dataout_cb(uint8_t status);

mss_usbd_cdc_state_t g_usbd_cdc_state = USBD_CDC_NOT_CONFIGURED;

mss_usbd_class_cb_t usbd_cdc_class_cb = {
 CDC_init_cb,
 CDC_deinit_cb,
 CDC_get_descriptor_cb,
 CDC_process_request_cb,
 CDC_datain_cb,
 CDC_dataout_cb,
 CDC_cep_datain_cb,
 CDC_cep_dataout_cb
};

mss_usbd_cdc_app_cb_t* usbd_cdc_vcp_ops;
mss_usb_device_speed_t g_usbd_cdc_user_speed;

/*Full configuration descriptor length. */
#define VCP_CONFIG_DESCR_LENGTH                         75u

uint8_t cdc_conf_descr[VCP_CONFIG_DESCR_LENGTH] = {

    /*----------------------- Configuration Descriptor -----------------------*/
    USB_STD_CONFIG_DESCR_LEN,                           /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,                  /* bDescriptorType */
    VCP_CONFIG_DESCR_LENGTH,                            /* wTotalLength LSB */
    0x00u,                                              /* wTotalLength MSB */
    0x02u,                                              /* bNumInterfaces */
                                                        /*interface number (which starts with 0) + 1
                                                         Comm Data Interface + Comm Management interface.*/
    0x01u,                                              /* bConfigurationValue */
    0x00u,                                              /* iConfiguration */
    0xC0u,                                              /* bmAttributes */
    0x32u,                                              /* bMaxPower */

    /*------------------- Interface Association Descriptor -------------------*/

    USB_STD_IA_DESCR_LEN,                               /* bLength*/
    USB_INTERFACE_ASSOCIATION_DESCRIPTOR_TYPE,          /* bDescriptorType = 11u*/
    0x00,                                               /* bFirstInterface*/
    0x02,                                               /* bInterfaceCount*/
    0x02,                                               /* bFunctionClass (Communication Class)*/
    0x02,                                               /* bFunctionSubClass (Abstract Control Model)*/
    0x01,                                               /* bFunctionProcotol (V250 (formerly V.25ter), Common AT commands)*/
    0x00,                                               /* iInterface*/

    /*----------- Interface 0 - Communications management Descriptor ---------*/
    USB_STD_INTERFACE_DESCR_LEN,                        /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                      /* bDescriptorType */
    0x00,                                               /* bInterfaceNumber: Number of this Interface */
    0x00,                                               /* bAlternateSetting: Alternate setting */
    0x01,                                               /* bNumEndpoints: One endpoints used */
    0x02,                                               /* bInterfaceClass: CDC-Control */
    0x02,                                               /* bInterfaceSubClass: Abstract Control Model */
    0x01,                                               /* bInterfaceProtocol: Common AT commands */
    0x00,                                               /* iInterface: */

    /*------------------- Header Functional Descriptor -----------------------*/
    0x05,                                               /* bLength: Endpoint Descriptor size */
    0x24,                                               /* bDescriptorType: CS_INTERFACE */
    0x00,                                               /* bDescriptorSubtype: Header Func Desc */
    0x10,                                               /* bcdCDC: spec release number */
    0x01,

    /*-------------------- Call Management Functional Descriptor -------------*/
    0x05,                                               /* bFunctionLength */
    0x24,                                               /* bDescriptorType: CS_INTERFACE */
    0x01,                                               /* bDescriptorSubtype: Call Management Func Desc */
    0x00,                                               /* bmCapabilities: D0+D1 */
    0x01,                                               /* bDataInterface: 1 */

    /*-----------------------ACM Functional Descriptor------------------------*/
    0x04,                                               /* bFunctionLength */
    0x24,                                               /* bDescriptorType: CS_INTERFACE */
    0x02,                                               /* bDescriptorSubtype: Abstract Control Management desc */
    0x02,                                               /* bmCapabilities */

    /*----------------------Union Functional Descriptor-----------------------*/
    0x05,                                               /* bFunctionLength */
    0x24,                                               /* bDescriptorType: CS_INTERFACE */
    0x06,                                               /* bDescriptorSubtype: Union func desc */
    0x00,                                               /* bMasterInterface: Communication class interface */
    0x01,                                               /* bSlaveInterface0: Data Class Interface */

    /*----------- Communications management Endpoint Descriptor --------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                         /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                       /* bDescriptorType */
    0x82u,                                              /* bEndpointAddress */
    0x03u,                                              /* bmAttributes */
    0x40u,                                              /* wMaxPacketSize LSB */
    0x00u,                                              /* wMaxPacketSize MSB */
    0x10u,                                              /* bInterval 0x10 -- corresponds to 4mSec.*/

    /*-------------- Interface 1 - Data class interface descriptor -----------*/
    USB_STD_INTERFACE_DESCR_LEN,                        /* bLength: Endpoint Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,                      /* bDescriptorType: */
    0x01,                                               /* bInterfaceNumber: Number of this Interface */
    0x00,                                               /* bAlternateSetting: Alternate setting */
    0x02,                                               /* bNumEndpoints: Two endpoints used */
    0x0A,                                               /* bInterfaceClass: CDC-Data */
    0x00,                                               /* bInterfaceSubClass: */
    0x00,                                               /* bInterfaceProtocol: */
    0x00,                                               /* iInterface: */

    /*-------------------------Endpoint OUT Descriptor-------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                         /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                       /* bDescriptorType: Endpoint */
    0x01,                                               /* bEndpointAddress */
    0x02,                                               /* bmAttributes: Bulk */
    0x40,                                               /* wMaxPacketSize: */
    0x00,
    0x00,                                               /* bInterval: ignored for Bulk transfer */

    /*-------------------------Endpoint IN Descriptor-------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                         /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                       /* bDescriptorType: Endpoint */
    0x81,                                               /* bEndpointAddress */
    0x02,                                               /* bmAttributes: Bulk */
    0x40,                                               /* wMaxPacketSize: */
    0x00,
    0x00                                                /* bInterval: ignored for Bulk transfer */
};

/***************************************************************************//**
 * Exported functions from CDC class driver.
 */

/***************************************************************************//**
* See mss_usb_device_cdc.h for details of how to use this function.
*/
void
MSS_USBD_CDC_init
(
    mss_usbd_cdc_app_cb_t* vcp_app_cb,
    mss_usb_device_speed_t speed
)
{
    usbd_cdc_vcp_ops = vcp_app_cb;
    g_usbd_cdc_user_speed = speed;
    MSS_USBD_set_class_cb_handler(&usbd_cdc_class_cb);
}

/***************************************************************************//**
* See mss_usb_device_cdc.h for details of how to use this function.
*/
void
MSS_USBD_CDC_tx
(
    uint8_t* buf,
    uint32_t length
)
{
    MSS_USBD_tx_ep_write(VCP_BULK_IN_EP, buf, length);
    g_usbd_cdc_state = USBD_CDC_TRANSMITTING;
}

/***************************************************************************//**
 See mss_usb_device_cdc.h for details of how to use this function.
 */
void
MSS_USBD_CDC_rx_prepare
(
    uint8_t* buf,
    uint32_t length
)
{
    MSS_USBD_rx_ep_read_prepare(VCP_BULK_OUT_EP, buf, length);
    g_usbd_cdc_state = USBD_CDC_RECEIVING;
}

/***************************************************************************//**
 See mss_usb_device_cdc.h for details of how to use this function.
 */
mss_usbd_cdc_state_t
MSS_USBD_CDC_get_state
(
    void
)
{
    return g_usbd_cdc_state;
}

/***************************************************************************//**
 Private functions to USBD_CDC class driver.
 ******************************************************************************/
uint8_t*
CDC_get_descriptor_cb
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
     Since Endpoint Size is wMaxpacketSize is 64, which is valid for both
     FS and HS, no need to make decision based on musb_speed.
     */

    if(recepient == USB_STD_REQ_RECIPIENT_DEVICE )
    {
        if(type == USB_CONFIGURATION_DESCRIPTOR_TYPE)
        {
            cdc_conf_descr[1] = USB_CONFIGURATION_DESCRIPTOR_TYPE;
            *length = sizeof(cdc_conf_descr);
            return(cdc_conf_descr);
        }
        else if (type == USB_OTHER_SPEED_CONFIG_DESCRIPTOR_TYPE)
        {
            cdc_conf_descr[1] = USB_OTHER_SPEED_CONFIG_DESCRIPTOR_TYPE;
            *length = sizeof(cdc_conf_descr);
            return(cdc_conf_descr);
        }
    }
    else if(recepient == USB_STD_REQ_RECIPIENT_ENDPOINT)/*Need index(EP Num)*/
    {
        /*Do nothing*/
    }
    else if(recepient == USB_STD_REQ_RECIPIENT_INTERFACE)/*Need index(intrfs)*/
    {
        /*Do nothing*/
    }
    else
    {
        /*Do nothing*/
    }

    return USB_FAIL;
}

uint8_t
CDC_init_cb
(
    uint8_t cfgidx,
    mss_usb_device_speed_t musb_speed
)
{
    uint8_t *buf_p;
    uint32_t len;

    /*
     Since Endpoint Size is wMaxpacketSize is 64, which is valid for both
     FS and HS, no need to make decision based on musb_speed.

     g_usbd_cdc_user_speed variable stores the speed selected by user.
     */
    MSS_USBD_tx_ep_configure(VCP_INTR_IN_EP,
                             VCP_INTR_IN_EP_FIFO_ADDR,
                             VCP_INTR_IN_EP_FIFO_SIZE,
                             VCP_INTR_IN_EP_MAX_PKT_SIZE,
                             1,
                             DMA_DISABLE,
                             MSS_USB_DMA_CHANNEL1,
                             MSS_USB_XFR_INTERRUPT,
                             ADD_ZLP_TO_XFR);

    MSS_USBD_tx_ep_configure(VCP_BULK_IN_EP,
                             VCP_BULK_IN_EP_FIFO_ADDR,
                             VCP_BULK_IN_EP_FIFO_SIZE,
                             VCP_BULK_IN_EP_MAX_PKT_SIZE,
                             1,
                             DMA_DISABLE,
                             MSS_USB_DMA_CHANNEL2,
                             MSS_USB_XFR_BULK,
                             ADD_ZLP_TO_XFR);

    MSS_USBD_rx_ep_configure(VCP_BULK_OUT_EP,
                             VCP_BULK_OUT_EP_FIFO_ADDR,
                             VCP_BULK_OUT_EP_FIFO_SIZE,
                             VCP_BULK_OUT_EP_MAX_PKT_SIZE,
                             1,
                             DMA_DISABLE,
                             MSS_USB_DMA_CHANNEL3,
                             MSS_USB_XFR_BULK,
                             ADD_ZLP_TO_XFR);

    g_usbd_cdc_state = USBD_CDC_CONFIGURED;
    usbd_cdc_vcp_ops->usb_cdc_init();

    usbd_cdc_vcp_ops->usb_cdc_notification(&buf_p, &len);

    if(buf_p)
    {
        MSS_USBD_tx_ep_write(VCP_INTR_IN_EP, buf_p, len);
    }
    return 1;
}

uint8_t
CDC_deinit_cb
(
    uint8_t cfgidx
)
{
    g_usbd_cdc_state = USBD_CDC_NOT_CONFIGURED;
    return 1;
}

uint8_t
CDC_process_request_cb
(
    mss_usbd_setup_pkt_t* setup_pkt,
    uint8_t** buf_pp,
    uint32_t* length
)
{
    if(USB_SUCCESS == usbd_cdc_vcp_ops->usb_cdc_process_request(setup_pkt,
                                                                buf_pp,
                                                                length))
    {
        return USB_SUCCESS;
    }
    else
    {
        return USB_FAIL;
    }
}

uint8_t
CDC_datain_cb
(
    mss_usb_ep_num_t num,
    uint8_t status
)
{
    uint8_t *buf_p;
    uint32_t len;
    if(num == VCP_INTR_IN_EP)
    {
        usbd_cdc_vcp_ops->usb_cdc_notification(&buf_p, &len);
        if(buf_p)
        {
            MSS_USBD_tx_ep_write(VCP_INTR_IN_EP, buf_p, len);
        }
    }
    else if(num ==VCP_BULK_IN_EP)
    {
        usbd_cdc_vcp_ops->usb_cdc_tx_complete(status);
    }
    else
    {
        return USB_FAIL;
    }
    return USB_SUCCESS;
}

uint8_t
CDC_dataout_cb
(
    mss_usb_ep_num_t num,
    uint8_t status,
    uint32_t rx_count
)
{
    if(num == VCP_BULK_OUT_EP)
    {
        usbd_cdc_vcp_ops->usb_cdc_rx(status, rx_count);
    }
    else
    {
        return USB_FAIL;
    }
    return USB_SUCCESS;
}

uint8_t
CDC_cep_datain_cb
(
    uint8_t status
)
{
    if(0 != usbd_cdc_vcp_ops->usb_cdc_cep_tx_complete)
        usbd_cdc_vcp_ops->usb_cdc_cep_tx_complete(status);
    return USB_SUCCESS;
}

uint8_t
CDC_cep_dataout_cb
(
    uint8_t status
)
{
    if(0 != usbd_cdc_vcp_ops->usb_cdc_cep_rx)
        usbd_cdc_vcp_ops->usb_cdc_cep_rx(status);
    return USB_SUCCESS;
}

#ifdef __cplusplus
}
#endif
