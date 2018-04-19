/*******************************************************************************
 * (c) Copyright 2012-2013 Microsemi SoC Products Group.  All rights reserved.
 *
 * Smartfusion2 MSS USB Driver Stack.
 *      USB Logical Layer (USB-LL)
 *          USBD-CDC class driver.
 *
 * USBD-CDC class driver public API
 *
 * SVN $Revision: 5468 $
 * SVN $Date: 2013-03-29 15:38:01 +0530 (Fri, 29 Mar 2013) $
 */

/*=========================================================================*//**
  @mainpage Smartfusion2 MSS USB Driver Stack.
       USB Logical Layer (USB-LL)
           USBD-CDC class driver.

  @section intro_sec Introduction
  The Communications Device Class driver implements the USB CDC class specified
  by USB-IF. This driver enables Virtual COM port (VCP) emulation on Smarfusion2
  devices. VCP functionality falls under PSTN devices subclass of the CDC class.
  With this driver, when connected to USB host, the Smartfusion2 device appears as
  a COM port. Unlike other USB classes, USB CDC devices need multiple USB
  interfaces to achieve the complete device functionality. All these interfaces
  should be associated with a single device driver on the USB host side. This
  driver uses interface association descriptor (IAD) along with other descriptors
  for enumeration.

  To implement VCP, this driver implements two USB interfaces, Communications class
  interface and Data class interface. The Communications Class interface is a
  communications management interface and is required for all communications devices.
  The Data Class interface is used to transport data between the host and the device.

  Communications class interface uses two USB endpoints: Control endpoint for
  enumeration and communication management and an interrupt IN endpoint for
  providing CDC class notifications to the USB host.

  Data class interface uses one BULK IN endpoint and one BULK OUT endpoint for data
  transfers between host and the device.

  This driver uses USBD-Class driver template to implement CDC class VCP
  functionality.

  @section theory_op Theory of Operation
  The following steps are involved in the operation of MSS USB:
    •	Configuration
    •	Initialization
    •	Enumeration
    •	Class Specific requests
    •	data transfer
  Configuration
  To use this driver, the MSS USB Driver stack must first be configured in USB
  Device mode using the MSS_USB_PERIPHERAL_MODE. No other configuration is
  necessary.

  Initialization
  CDC class driver must be initialized using MSS_USBD_CDC_init() function. Once
  initialized, this driver will get configured by USBD Device Driver during the
  process of Enumeration. The Call-back function MSS_USBD_CDC_Init_cb() is
  implemented by this driver which will be called by USBD Driver when Host
  configures this device. The MSS_USBD_CDC_get_descriptor() is implemented to
  provide class specific descriptor table to the USBD Core driver.

  For successful enumeration as a CDC class device, this driver defines descriptor
  table which contains a configuration descriptor, interface association
  descriptor, two interface descriptors, CDC class descriptors and three endpoint
  descriptors.

  Note:	For successful enumeration device specific descriptors must be provided
  by application using MSS_USBD_set_desc_cb_handler() function to the USBD Driver.
  Since Device, string descriptors etc. are not Class specific, they are not part
  of CDC Class driver.

  Class Specific requests
  The MSS_USBD_CDC_process_request_cb() call-back function is implemented by this
  driver which processes the CDC class specific requests. The class driver must
  implement this call-back function to successfully respond to the host requests.

  Data transfer
  The CDC class driver performs the Data transfers on data class interface using
  one BULK IN endpoint and one BULK OUT endpoint. The data transferred on these
  endpoints is raw data (no protocol abstraction on top of USB transfers).
  Application must use MSS_USBD_CDC_tx()  and MSS_USBD_CDC_rx_prepare()
  functions to transmit and receive data on VCP respectively. To get the
  information on data transfer events on the USB bus, this driver implements
  MSS_USBD_CDC_datain_cb() and MSS_USBD_CDC_dataout_cb() which are called by
  USBD Driver.

  Note: The VCP functionality is the emulation of serial data transfer on USB bus.
  Though the received data is presented to the application software as raw serial
  data, the underlying USB protocol must be followed. In USB protocol, a USB
  Device cannot transfer data unless USB host requests for it. Though
  MSS_USBD_CDC_tx() keeps the data ready in the USB TX FIFO, data transmission
  will take place only when a IN packet is received from the host. This behavior
  is different than UART serial transmission where both the connected devices
  can send messages own their own (When no flow is control used).

 *//*=========================================================================*/

#ifndef __MSS_USB_DEVICE_CDC_H_
#define __MSS_USB_DEVICE_CDC_H_

#include <stdint.h>
#include "mss_usb_device.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Public Types from USBD-CDC class driver
 */
/*
 The mss_usbd_cdc_state_t is used to provide information of current state of
  the USBD-CDC class driver.
  USBD_CDC_NOT_CONFIGURED -- USB CDC class driver is not configured and it can
                            not perform    data transfers.
  USBD_CDC_CONFIGURED    --  USB CDC class driver is configured by Host and it
                            can perform data data transfers.
  USBD_CDC_TRANSMITTING  --  USB CDC class driver is configured by Host and is
                            transmitting data.
  USBD_CDC_RECEIVING  --     USB CDC class driver is configured by Host and is
                            receiving data.
*/

typedef enum mss_usbd_cdc_state {
    USBD_CDC_NOT_CONFIGURED,
    USBD_CDC_CONFIGURED,
    USBD_CDC_TRANSMITTING,
    USBD_CDC_RECEIVING
}mss_usbd_cdc_state_t;

/*******************************************************************************
 * Public Constants from USBD-CDC class driver
 */
/*******************************************************************************
 CDC Class configuration definitions: These values will be used by CDC driver
 to configure the MSS USB core endpoints. These values are alos used for
 generating the descriptor tables which are sent to the USB host durring
 enumeration.

 Note1:
 You are allwed to modify these vallues according to requirement.
 Make sure that the values here are in accordance with the user descriptors
 device descriptor etc), USB 2.0 standard and USB Core Driver operation speed
 (MSS_USB_DEVICE_HS or MSS_USB_DEVICE_FS)

 Note2:
 Endpoint MaxPktSize limits must be followed according to USB specification and
 speed of operation. Speed dependent descriptor should be updated accordingly.
 */

/*MSS USB core Endpoints used by CDC class*/
#define VCP_BULK_OUT_EP                                 MSS_USB_RX_EP_1
#define VCP_BULK_IN_EP                                  MSS_USB_TX_EP_1
#define VCP_INTR_IN_EP                                  MSS_USB_TX_EP_2

/*MSS USB core FIFO address for endpoints used by CDC class*/
#define VCP_BULK_IN_EP_FIFO_ADDR                        0x100u
#define VCP_BULK_OUT_EP_FIFO_ADDR                       0x400u
#define VCP_INTR_IN_EP_FIFO_ADDR                        0x50u

/*MSS USB core FIFO size for endpoints used by CDC class*/
#define VCP_BULK_IN_EP_FIFO_SIZE                        64u
#define VCP_BULK_OUT_EP_FIFO_SIZE                       64u
#define VCP_INTR_IN_EP_FIFO_SIZE                        64u

/*Maximum packet size for endpoints used by CDC class*/
#define VCP_BULK_IN_EP_MAX_PKT_SIZE                     64u
#define VCP_BULK_OUT_EP_MAX_PKT_SIZE                    64u
#define VCP_INTR_IN_EP_MAX_PKT_SIZE                     64u

/*******************************************************************************
 * Public data strucures from CDC class driver.
 ******************************************************************************/

/*******************************************************************************
 mss_usbd_cdc_app_cb_t
  The mss_usbd_cdc_app_cb_t type provides the prototype for all call-back function
  handlers which should be implemented by the user application. User application
  must define and initialize a structure of this type and provide the address of
  that structure as parameter in MSS_USBD_CDC_init() function.

  usb_cdc_init
  The function pointed by the usbd_cdc_init element will be called to indicate
  that the CDC driver is configured by the USB host. The application can use this
  function to prepare itself for data exchange with the host. This driver cannot
  perform data transfers unless it is configured by the USB host to do so.

  usb_cdc_release
  The function pointed by the usbd_cdc_release element will be called when the
  host wants to un-configure this CDC device.

  usb_cdc_process_request
  The function pointed by the usbd_cdc_process_request element will be called
  when this driver receives a CDC class specific request to be processed. The
  parameter setup_pkt is the pointer to the setup packet sent by the host. The
  parameters buf_p and length are used for the data which might be associated
  with the current setup packet. If Host requests data from the device, the
  application should provide the address of the buffer containing the data in
  buf_p parameter and the length of this buffer in bytes in length parameter.
  If the host wants to send data to the device then application should provide
  the address of the buffer where the data should be placed in buf_p parameter
  and the size of the buffer in bytes in the length parameter. For zero data
  length requests the buf_p and the length parameters are not meaningful.

  usb_cdc_tx_complete
  The function pointed by the usbd_cdc_tx_complete element will be called to
  indicate completion of data transmission operation. The status parameter
  provides the error status of this operation. A non-zero value indicates that
  there was an error in last operation.

  usb_cdc_rx
  The function pointed by the usbd_cdc_rx element will be called to indicate
  data is available to read from RX EP FIFO. The status parameter provides
  the error status of this operation. A non-zero value indicates that there
  was an error in last operation. Number of bytes received during this
  operation is indicated in rx_count parameter.

  usb_cdc_notification
  This driver needs to transmit CDC class Notification data on the Interrupt
  endpoint on a periodic basis. The period of transmission of data should be
  selected in endpoint descriptor. Host gets this information during the
  enumeration process. Application should provide the notification data to
  this driver by implementing usb_cdc_notification element of usbd_cdc_app_cb_t
  type structure. This callback function is called periodically when IN token
  is received on CDC interrupt endpoint.

  usb_cdc_cep_tx_complete
  The function pointed by the usb_cdc_cep_tx_complete element will be called
  when data packet is transmitted on previously configured control endpoint.
  The data from the buffer provided with usb_cdc_process_requests call-back
  would be transmitted. The parameter status indicates error status of the
  transmit transaction. A non-zero status value indicates that there was error
  in last transmit transaction.

  usb_cdc_cep_rx
  The function pointed by the usb_cdc_cep_rx element will be called when data
  packet is received on previously configured control endpoint. The data would
  be placed in the buffer which was previously provided with
  usb_cdc_process_requests call-back. The parameter status indicates error
  status of the receive transaction. A non-zero status value indicates that
  there was error in last receive transaction.

*/
typedef struct mss_usbd_cdc_app_cb {
    void (*usb_cdc_init)(void);
    void (*usb_cdc_release)(void);
    uint8_t (*usb_cdc_process_request)(mss_usbd_setup_pkt_t* setup_pkt,
                                       uint8_t** buf,
                                       uint32_t* length);

    uint8_t(*usb_cdc_tx_complete)(uint8_t status);
    uint8_t(*usb_cdc_rx)(uint8_t status, uint32_t rx_count);
    void(*usb_cdc_notification)(uint8_t** buf_p, uint32_t* length_p);
    uint8_t(*usb_cdc_cep_tx_complete)(uint8_t status);
    uint8_t(*usb_cdc_cep_rx)(uint8_t status);

} mss_usbd_cdc_app_cb_t;

/*******************************************************************************
 * Exported APIs from USBD-CDC driver
 ******************************************************************************/

/***************************************************************************//**
  @brief MSS_USBD_CDC_init()
    The MSS_USBD_CDC_init() API must be used by the application to initialize the
    CDC class driver. A pointer to the structure of type mss_usbd_cdc_app_cb_t
    must be passed as a parameter to this function.

  @param vcp_app_cb
    The vcp_app_cb parameter is a pointer to the structure of type
    mss_usbd_cdc_app_cb_t. This will be used by the USBD-CDC class driver to call
    the call-back functions implemented by application.

  @param speed
    The speed parameter specifies the USB speed at which the USB driver and the
    MSS USB core should operate.

    Valid values for this parameter are
        MSS_USB_DEVICE_HS
        MSS_USB_DEVICE_FS

  @return
    This function does not return a value.

  Example:
  @code
        //Assign call-back function handler structure needed by USBD-CDC driver
        MSS_USBD_CDC_init(&g_vcp_app_cb, MSS_USB_DEVICE_HS);

        //Assign call-back function handler structure needed by USB Device Core driver
        MSS_USBD_set_desc_cb_handler(&vcp_descr_cb);

        //Initialize USB Device Core driver
        MSS_USBD_init(MSS_USB_DEVICE_HS);

  @endcode
*/
void
MSS_USBD_CDC_init
(
    mss_usbd_cdc_app_cb_t* vcp_app_cb,
    mss_usb_device_speed_t speed
);

/***************************************************************************//**
  @brief MSS_USBD_CDC_tx()
    The MSS_USBD_CDC_tx() API can be used by the application to start transmission
    on the CDC VCP. A pointer to the buffer which needs to be transmitted should
    be provided in the parameter buf. Length of the buffer should be provided in
    the parameter length.

  @param buf
    The buf parameter is a pointer to the buffer from which the data to be
    transmitted.

  @param length
    The length parameter indicates the number of data byte to be transmitted.

  @return
    This function does not return a value.

  Example:
  @code
        if(USBD_CDC_CONFIGURED == MSS_USBD_CDC_get_state())
        {
        MSS_USBD_CDC_tx(data_buf, length);
        }
  @endcode
*/
void
MSS_USBD_CDC_tx
(
    uint8_t* buf,
    uint32_t length
);

/***************************************************************************//**
  @brief MSS_USBD_CDC_rx_prepare()
    The MSS_USBD_CDC_rx_prepare() API can be used by the application to prepare
    CDC VCP to receive data. Once prepared, this driver will read data into the
    provided buffer when it receives data from the Host.

  @param buf
    The buf parameter is a pointer to the buffer in which the received data will
    be stored.

  @param length
    The length parameter indicates the length of the buffer in which the data
    received from the host is stored.

  @return
    This function does not return a value.

  Example:
  @code
        if(USBD_CDC_CONFIGURED == MSS_USBD_CDC_get_state())
        {
            MSS_USBD_CDC_rx_prepare(buf, length);
        }

  @endcode
*/
void
MSS_USBD_CDC_rx_prepare
(
    uint8_t* buf,
    uint32_t length
);

/***************************************************************************//**
  @brief MSS_USBD_CDC_get_state()
    The MSS_USBD_CDC_get_state() API can be used by the application to know the
    current state of the CDC Class driver.

  @param
    This function does not take any parameter.

  @return
    This function returns the status of the CDC Class driver of type
    mss_usbd_cdc_state_t.

  Example:
  @code
        if(USBD_CDC_CONFIGURED == MSS_USBD_CDC_get_state())
        {
            MSS_USBD_CDC_rx_prepare(buf, length);
        }
  @endcode
*/
mss_usbd_cdc_state_t
MSS_USBD_CDC_get_state
(
    void
);

#ifdef __cplusplus
}
#endif

#endif  /* __MSS_USB_DEVICE_CDC_H_ */
