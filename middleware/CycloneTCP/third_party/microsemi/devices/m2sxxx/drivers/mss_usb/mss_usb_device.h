/*******************************************************************************
 * (c) Copyright 2012-2013 Microsemi SoC Products Group.  All rights reserved.
 *
 * Smartfusion2 MSS USB Driver Stack
 *      USB Logical Layer (USB-LL)
 *          USBD driver
 *
 *  USBD driver public API.
 *
 * SVN $Revision: 5468 $
 * SVN $Date: 2013-03-29 15:38:01 +0530 (Fri, 29 Mar 2013) $
 */

/*=========================================================================*//**
  @mainpage Smartfusion2 MSS USB Driver Stack
                USBD driver

  @section intro_sec Introduction
  USB Device Driver (USBD) falls in USB-LL layer of the MSS USB Driver stack.
  The USBD driver implements the core functionality of the USB Device mode
  operations. The USBD driver is responsible for following functionalities.
    •	USB Device Enumeration
    •	USB Standard request handling
    •	USB Suspend, Resume and Reset handling
    •	Endpoint management
    •	USB transfers management
    •	USBD-Class call-back interface
    •	Application call-back interface

  @section theory_op Theory of Operation
  The following steps are involved in the operation of MSS USB:
    •	Configuration
    •	Initialization
    •	USBD-Class Interface
    •	Application Interface
    •	Data transfer
  USBD driver operation depends on user configuration provided in mss_usb_config.h.
  Configuration
  To operate the MSS USB block in USB Device mode following parameter must be
  defined in the mss_usb_config.h file.

  MSS_USB_PERIPHERAL_MODE  	Configures the MSS USB Driver Stack to operate in USB
  Device mode.
  Initialization
  The USBD driver must be initialized by calling initialization function
  MSS_USBD_init() with a parameter to select Hi-Speed (HS) mode or Full Speed (FS)
  mode. The USB enumeration process is handled internally by this driver.
  Application only needs to provide required application specific information
  (descriptor) by using MSS_USBD_set_desc_cb_handler() and
  MSS_USBD_set_class_cb_handler() functions.

  Note: USB Low speed operation in Device mode is not supported by MSS USB.

  USBD-Class Call-back Interface
  This driver encapsulates the generic USB protocol functionality from the USB
  class functionality. To pass on the control to the USBD-Class driver for
  handling the USB class specific functionality, this driver needs the
  USBD-Class driver to implement a set of call-back functions. This driver
  provides a data type mss_usbd_user_class_cb_t which must be implemented by
  USBD-Class driver. This data type is the collection of call-back functions
  which should be implemented by USBD-Class driver. Implementing all the
  elements of this structure may not be necessary for a specific USB class.
  The USBD-Class driver must define a structure of type mss_usbd_user_class_cb_t
  and implement its required elements. The USBD-Class driver must pass a pointer
  to this structure to USBD driver using MSS_USBD_set_class_cb_handler() function.

  Application Call-back Interface
  Application is required to provide the Device specific descriptors during
  enumeration process, namely; Device descriptor, Device Qualifier descriptor and
  Strings descriptors.

  These functions are elements of data structure of type mss_usbd_user_descr_cb_t.
  This structure must be defined and all the required elements of this structure
  must be implemented in the user application source code. The address of this
  structure must be passed to the USBD Driver using MSS_USBD_set_desc_cb_handler()
  function.

  Data transfer
  USB Device uses ‘Endpoint’ for data transfer. The transmit endpoint and receive
  endpoint need to be configured for their respective configuration parameters
  before using them for data transfer. The functions MSS_USBD_tx_ep_configure()
  and MSS_USBD_rx_ep_configure() should be used to configure transmit endpoint
  and receive endpoint respectively.

  To start data transmission on a transmit endpoint, application should use
  MSS_USBD_tx_ep_write() function. This function prepares the driver to transmit
  data to the Host. However, actual transmission happens on receiving IN packet
  from the USB Host. This function is non-blocking. When the data transmission
  is complete or if there were error during the operation, the USBD-Class driver
  will be informed about it by calling a call-back function. Please refer
  USBD-Class Interface section above for more details.

  To receive data from USB Host, this driver must first be prepared for it.
  Application must use MSS_USBD_rx_ep_read_prepare() function for this purpose.
  This function is non-blocking.  On receiving data or in case of errors during
  this operation the USBD-Class driver will be informed about it by calling a
  call-back function. Please refer USBD-Class Interface section above for more
  details.

  The functions MSS_USBD_tx_ep_stall() or MSS_USBD_rx_ep_stall() should be used
  when USBD-Class driver needs to stall a particular endpoint. The stall condition
  on an endpoint can be cleared using MSS_USBD_rx_ep_clr_error() and
  MSS_USBD_rx_ep_clr_error() functions on respective endpoint.

  Control endpoint is significantly different in its operations compared to a
  transmit endpoint or a receive endpoint. Control endpoint can be used for both
  transmitting data to the Host or receiving data from the Host. To configure and
  perform data transfers over control endpoint, separate functions are provided.
  The MSS_USBD_cep_configure() function should be used to configure the control
  endpoint. MSS_USBD_cep_write() and MSS_USBD_cep_read_prepare() function should
  be used to transmit and receive data over control endpoint respectively.

  Since Control endpoint is shared by the USBD driver and the USBD-Class driver,
  the stall condition on Control endpoint is handled by USBD driver internally.
  The USBD-Class driver must provide meaningful return value in the call-back
  functions related to Control endpoint. Depending on these return values USBD
  driver will appropriately handle the stall condition on Control endpoint.

 *//*=========================================================================*/

#ifndef __MSS_USB_DEVICE_H_
#define __MSS_USB_DEVICE_H_

#include "mss_usb_common_cif.h"
#include "mss_usb_device_cif.h"

/***************************************************************************//**
  Constant values exported from USBD driver
 ******************************************************************************/
/*
 The following constants are used to specify whether or not internal DMA should
 be used for data transfers. These values should be used as parameter to
 MSS_USBD_tx_ep_configure() and MSS_USBD_rx_ep_configure() functions.
*/
#define DMA_DISABLE                                     0u
#define DMA_ENABLE                                      1u

/*
 The following constants are used to specify whether or not a bulk transfers
 ends with a zero length packet when transfer size is exact multiple of
 wMaxPacketSize. These values should be used as parameter to the
 MSS_USBD_tx_ep_configure() and MSS_USBD_rx_ep_configure() functions.
 */
#define NO_ZLP_TO_XFR                                   0u
#define ADD_ZLP_TO_XFR                                  1u

/*
 The following constants are used as return values for the call-back functions,
 implemented by USBD-Class driver as part of mss_usbd_class_cb_t type, which
 are called by USBD driver. USBD driver uses these values to infer whether or
 not the call-back function was successfully executed. The current control
 transfer is stalled if the return value is USB_FAIL.
 */
#define USB_FAIL                                        0u
#define USB_SUCCESS                                     1u

/***************************************************************************//**
  Data structures exported by the USBD driver
 ******************************************************************************/

/***************************************************************************//**
 mss_usbd_user_descr_cb
    The mss_usbd_user_descr_cb_t provides the prototype of the structure
    that should be implemented by the application to provide the call-back
    functions which will be called by the driver to receive the user descriptor
    information.

    usb_device_descriptor
    The function pointed by the usb_device_descriptor element will be called
	when USB device receives the GET_DESCRIPTOR command requesting device
	descriptor from USB host. This function should return a pointer to the
	device descriptor and provide the length of the descriptor in the return
	parameter.

    usb_device_qual_descriptor
    The function pointed by the usb_device_qual_descriptor element will be called
	when USB device receives the GET_DESCRIPTOR command requesting device qualifier
	descriptor from USB host. This function should return a pointer to the device
	qualifier descriptor and provide the length of the descriptor in the return
	parameter.

    usb_string_descriptor
    The function pointed by the usb_string_descriptor element will be called
	when USB device receives the GET_DESCRIPTOR command requesting specific
	string descriptor from USB host. Requested string descriptor number is
	provided in parameter index. This function should return a pointer to the
	requested string descriptor and provide the length of the descriptor in
	the return parameter length.
*/
typedef struct mss_usbd_user_descr_cb {
    uint8_t* (*usb_device_descriptor)(uint32_t* length);

    uint8_t* (*usb_device_qual_descriptor)(mss_usb_device_speed_t speed,
                                           uint32_t* length);

    uint8_t* (*usb_string_descriptor)(uint8_t index, uint32_t* length);
} mss_usbd_user_descr_cb_t;

 /***************************************************************************//**
 mss_usbd_setup_pkt_t
    The mss_usbd_setup_pkt_t provides the prototype of the structure for the setup
    packet sent by the host. All the parameters of this structure are as per the
    standard setup packet defined in the USB2.0 specification.

    request_type
    The request_type parameter provides the information on the type of the request
    sent by the host. The meaning of this parameter is exactly as defined by the
    USB2.0 specification.

    request
    The request parameter is the actual request sent by the host. The meaning of
    this parameter is exactly as defined by the USB2.0 specification.

    value
    The value parameter is the value associated with the request. The meaning of
    this parameter is exactly as defined by the USB2.0 specification.

    index
    The index parameter is the index information associated with the request.
    The meaning of this parameter is exactly as defined by the USB2.0 specification.

    length
    The length parameter provides the length of the data associated with the request.
    The meaning of this parameter is exactly as defined by the USB2.0 specification.
  */
typedef struct {
    uint8_t request_type;
    uint8_t request;
    uint16_t value;
    uint16_t index;
    uint16_t length;
} mss_usbd_setup_pkt_t;

 /***************************************************************************//**
 mss_usb_class_cb
    The mss_usb_user_class_cb_t provides the prototype of the structure that
    should be implemented by the application/class specific component to provide
    the call-back functions which will be called by the USBD driver on specific
	events on the USB bus.

    usb_class_init
    The function pointed by the usb_class_init element will be called when USB
	device receives SET_CONFIGURATION request from USB host with a non-zero
	cfgidx number. The parameter cfgidx indicates the configuration number that
	should be set by the USB device. USB device goes into the
	MSS_USB_CONFIGURED_STATE on receiving this command. The musb_speed parameter
	indicates the USB speed at which the MSS USB is operating. The speed of
	operation is determined during enumeration process.

    usb_class_deinit
    The function pointed by the usb_class_deinit element will be called when USB
	device receives SET_CONFIGURATION request from USB host with a cfgidx = 0.
	The parameter cfgidx indicates the configuration number that should be set
	by the USB device. USB device goes into the MSS_USB_ADDRESS_STATE on
	receiving this command.

    usb_class_get_descriptor
    The function pointed by the usb_class_get_descriptor element will be called
	when USB device receives GET_DESCRIPTOR request from USB host requesting a
	class specific descriptor. The parameter recipient indicates the intended
	recipient by the USB host (endpoint, interface or device). The parameter type
	indicates the type of descriptor requested. The musb_speed parameter
	indicates the USB speed at which the MSS USB is operating. The speed of
	operation is determined during enumeration process. The musb_speed parameter
	should be used to provide speed dependent descriptors.

    usb_class_process_request
    The function pointed by the usb_class_process_requests element will be
	called when USB device receives class specific request from USB host. The
	parameter setup_pkt is the pointer to the setup packet sent by the host.
	The parameters buf_p and length are used for the data which might be
	associated with the current setup packet. If Host requests data from the
	device, the USBD-Class driver should provide the address of the buffer
	containing the data in buf_p parameter and the length of this buffer in
	bytes in length parameter. If the host wants to send data to the device
	then USBD-Class driver should provide the the address of the buffer where
	the data should be placed in buf_p parameter and the size of the buffer
	in bytes in the length parameter. For zero data length request the buf_p
	and the length parameters are not meaningful.

    usb_class_datain
    The function pointed by the usb_class_datain element will be called when
	data is transferred on previously configured transmit endpoint. The
	endpoint on which data is transmitted is indicated by parameter num.
	The parameter status indicates error status of the transmit transaction.
	A non-zero status value indicates that there was error in last receive
	transaction.

    usb_class_dataout
    The function pointed by the usb_class_dataout element will be called
	when data is received on previously configured receive endpoint. The
	endpoint on which data is received is indicated by parameter num. The
	parameter status indicates error status of the receive transaction.
	A non-zero status value indicates that there was error in last receive
	transaction. The rx_count parameter indicates the number of bytes
	received in the last receive transaction.

    usb_class_cep_datain
    The function pointed by the usb_class_cep_datain element will be called
	when data packet is transmitted on previously configured control endpoint.
	The data from the buffer provided with usb_class_process_requests
	call-back would be transmitted. The parameter status indicates error
	status of the transmit transaction. A non-zero status value indicates
	that there was error in last transmit transaction.

    usb_class_cep_dataout
    The function pointed by the usb_class_cep_dataout element will be called
	when data packet is received on previously configured control endpoint.
	The data would be placed in the buffer which was previously provided with
	usb_class_process_requests call-back. The parameter status indicates error
	status of the receive transaction. A non-zero status value indicates that
	there was error in last receive transaction.
*/
typedef struct mss_usbd_class_cb {
    uint8_t(*usb_class_init)(uint8_t cfgidx, mss_usb_device_speed_t musb_speed);
    uint8_t(*usb_class_deinit)(uint8_t cfgidx);

    uint8_t*(*usb_class_get_descriptor)(uint8_t recepient,
                                        uint8_t type,
                                        uint32_t* length,
                                        mss_usb_device_speed_t musb_speed);

    uint8_t (*usb_class_process_request)(mss_usbd_setup_pkt_t* setup_pkt,
                                         uint8_t** buf_p,
                                         uint32_t* length);

    uint8_t(*usb_class_datain)(mss_usb_ep_num_t num, uint8_t status);
    uint8_t(*usb_class_dataout)(mss_usb_ep_num_t num,
                                uint8_t status,
                                uint32_t rx_count);

    uint8_t(*usb_class_cep_datain)(uint8_t status);
    uint8_t(*usb_class_cep_dataout)(uint8_t status);

} mss_usbd_class_cb_t;

/***************************************************************************//**
  Data structures which are internally used by USBD driver
 ******************************************************************************/
#ifdef MSS_USB_DEVICE_TEST_MODE
typedef struct mss_usb_user_device_test_cb {
    void(*test_ep_dataout)(mss_usb_ep_num_t num, uint8_t status, uint32_t rx_count);
    void(*test_ep_datain)(mss_usb_ep_num_t num, uint8_t status);

    void(*test_cep_setup)(uint8_t status);
    void(*test_cep_dataout)(uint8_t status);
    void(*test_cep_datain)(uint8_t status);

    void(*test_sof)(uint8_t status);
    void(*test_reset)(void);
    void(*test_suspend)(void);
    void(*test_resume)(void);
    void(*test_disconnect)(void);

} mss_usbd_user_test_cb_t;
#endif

/*******************************************************************************
  Exported functions from USBD driver
 *******************************************************************************/

/***************************************************************************//**
  @brief MSS_USBD_init()
    The MSS_USBD_init() API must be used to initialize the USB driver in device
    mode. Appropriate value of the parameter speed must be provided to make the
    driver operate at desired USB speed.

    Note: When MSS_USB_DEVICE_HS is selected, the MSS USB will try to negotiate
    for High speed during enumeration process. The actual speed of operation
    depends on the Host it is connected to. If the Host is only a FS host, then
    MSS USB will appear as a FS device. This will be the case when MSS USB is
    connected to a host through USB 1.x hub.

    When MSS_USB_DEVICE_FS is selected, MSS USB will never negotiate for High
    speed. It will always appear as a FS device only.

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
    MSS_USBD_init(MSS_USB_DEVICE_FS);
    MSS_USBD_set_descr_cb_handler(&flash_drive_descriptors_cb);

  @endcode
*/
void
MSS_USBD_init
(
    mss_usb_device_speed_t speed
);

/***************************************************************************//**
  @brief MSS_USBD_set_descr_cb_handler()
    The MSS_USBD_set_desc_cb_handler() API must be used to provide the
    Application call-back interface functions to the USBD driver which will be
    called by this driver to get the USB user descriptor.

  @param user_desc_cb
    The user_desc_cb parameter provides the address of the structure of type
    mss_usbd_user_descriptors_cb_t which is implemented by the class driver or
    application.

  @return
    This function does not return a value.

  Example:
  @code

    MSS_USBD_init(MSS_USB_DEVICE_FS);

    MSS_USBD_set_descr_cb_handler(&flash_drive_descriptors_cb);

    @endcode
*/
void
MSS_USBD_set_descr_cb_handler
(
    mss_usbd_user_descr_cb_t* user_desc_cb
);

/***************************************************************************//**
  @brief MSS_USBD_set_class_cb_handler()
    The MSS_USBD_set_class_cb_handler() API must be used to provide the call-back
    functions to the driver which will be called by this driver to indicate the
    specific events happening on the USB Bus.

  @param class_cb
    The class_cb parameter provides the address of the structure of type
    mss_usbd_class_cb_t which is implemented by the USBD-Class driver.

  @return
    This function does not return a value.

  Example:
  @code
        void
        MSS_USBD_MSC_init
        (
            mss_usbd_msc_media_t* media_ops,
            mss_usb_device_speed_t speed
        )
        {
            g_usbd_msc_media_ops = media_ops;
            g_usbd_msc_user_speed = speed;

            MSS_USBD_set_class_cb_handler(&usb_msd_class_cb);
        }
  @endcode
*/
void
MSS_USBD_set_class_cb_handler
(
    mss_usbd_class_cb_t* class_cb
);

/***************************************************************************//**
  @brief MSS_USBD_cep_configure()
    The MSS_USBD_cep_configure() API is used to configure the Control Endpoint
    (EP0) for the Control transactions and enables the Control endpoint interrupt.
    Other Properties of the Control endpoint are fixed as listed below.
        Transfer type 		– Control Transfer.
        FIFO address 		– 0x00
        FIFO Size 		    - 0x40 (64 decimal)

    This API must be called before any other API for the control endpoint.

  @param max_pkt_size
    The max_pkt_size parameter is the maximum packet size used for control transfer.
    This value must match with the bMaxPacketSize0 value in the Device Descriptor.

  @return
    This function does not return a value.

  Example:
  @code
    MSS_USBD_cep_configure(64u);
    MSS_USBD_cep_read_prepare((uint8_t*)&g_setup_pkt, 8u);
  @endcode
*/
void
MSS_USBD_cep_configure
(
    uint8_t max_pkt_size
);

/***************************************************************************//**
  @brief MSS_USBD_cep_read_prepare()
    The MSS_USBD_cep_read_prepare() function prepares the previously configured
    Control endpoint for receiving data. After calling this function, USBD driver
    is ready to read data received on the next OUT packet. This function will
    store the data read from endpoint FIFO into the provided buffer. If the data
    in receive FIFO exceeds in length than the length parameter value then this
    function reads data equal to length value.

  @param addr
    The addr parameter specifies the address of the receive buffer where the data
    from RX FIFO is stored by this driver.

  @param length
    The length parameter specifies the length of the receive buffer in bytes.

  @return
    This function does not return a value.

  Example:
  @code
    MSS_USBD_cep_configure(64u);
    MSS_USBD_cep_read_prepare((uint8_t*)&g_setup_pkt, 8u);
  @endcode
*/
void
MSS_USBD_cep_read_prepare
(
    uint8_t * addr,
    uint32_t length
);

/***************************************************************************//**
  @brief MSS_USBD_cep_write()
    The MSS_USBD_cep_write() API loads the Control Endpoint FIFO with the data
    from provided buffer. On receiving the next IN token, the data will be
    transmitted over USB. If the length parameter value is more than the size
    of the FIFO then this functions loads number of bytes equal to the length
    parameter value.

  @param addr
    The addr parameter specifies the address of the buffer from which the data
    is to be copied to the Control endpoint buffer.

  @param length
    The length parameter specifies the length of the provided buffer in bytes.

  @return
    This function does not return a value.

  Example:
  @code
    MSS_USBD_cep_write(buf, length);
  @endcode
*/
void
MSS_USBD_cep_write
(
    uint8_t * addr,
    uint32_t length
);

/***************************************************************************//**
  @brief MSS_USBD_cep_flush_fifo()
    The MSS_USBD_cep_flush_fifo() API is used to flush the content of the control
    endpoint FIFO.

  @param
    This function does not take any parameter.

  @return
    This function does not return a value.

  Example:
  @code
    MSS_USBD_cep_flush_fifo();
  @endcode
*/
void
MSS_USBD_cep_flush_fifo
(
    void
);

/***************************************************************************//**
  @brief MSS_USBD_cep_enable_irq()
    The MSS_USBD_cep_enable_irq() API is used to enable the control endpoint interrupt.

  @param
    This function does not take any parameter.

  @return
    This function does not return a value.

  Example:
  @code
    MSS_USBD_cep_enable_irq();
  @endcode
*/
void
MSS_USBD_cep_enable_irq
(
    void
);

/***************************************************************************//**
  @brief MSS_USBD_cep_disable_irq()
    The MSS_USBD_cep_disable_irq() API is used to disable the control endpoint
    interrupt.

  @param
    This function does not take any parameter.

  @return
    This function does not return a value.

  Example:
  @code
    MSS_USBD_cep_disable_irq();
  @endcode
*/
void
MSS_USBD_cep_disable_irq
(
    void
);

/***************************************************************************//**
  @brief MSS_USBD_tx_ep_configure()
    The MSS_USBD_tx_ep_configure() API configures the transmit endpoint with the
    configuration values provided as parameters. After initializing the core,
    this API must be called before any other API for the desired transmit
    endpoint.

    Note:  To provide complete flexibility in configuring the FIFOs, this driver
    does not make any assumptions on the FIFO size and FIFO address. User is
    responsible to make sure that the endpoint FIFO does not overlap with other
    endpoint FIFO space.

  @param ep_num
    The ep_num parameter is the endpoint number which is to be configured.
    The endpoint number must be of type mss_usb_ep_num_t.

  @param fifo_addr
    The fifo_addr parameter is the address of the FIFO in the MSS_USB internal
    RAM. Valid FIFO address values are from 0x00 to 0xFFF8. FIFO Address must
    be multiple of 8. If the value provided is not multiple of 8, then the
    immediate lower value which is multiple of 8 is taken as the FIFO address.
    E.g. If the provided value is 0x09, the actual value taken by driver is 0x08.
    If the provided value is less than 0x08 then FIFO address is taken as 0x00.

  @param fifo_size
    The fifo_size parameter is the endpoint FIFO size in the MSS USB core
    internal RAM. Valid FIFO size values are 8, 16, 32, 64, 128, 512, 1024,
    2048, 4096. MSS USB core by default assigns 8 byte FIFO if the FIFO size
    is not configured.

  @param max_pkt_size
    The max_pkt_size parameter is the maximum packet size of the USB transfer
    on this endpoint. This value must be equal to the maximum packet size as
    mentioned in the endpoint descriptor for this endpoint which is used
    during enumeration process.

    Note: This value must be less than or equal to the FIFO size value.

  @param num_usb_pkt
    The num_usb_pkt parameter has different meanings for different types of
    transfers.

    Low bandwidth ISO/interrupt transfers – This parameter must always be ‘1u’.
    This parameter represents the number of packets transferred in one (micro)
    frame.

    High bandwidth ISO transfers – This parameter represents the number of
    packets transferred in one (Micro) frame. In this case, this parameter can
    have a value of 1, 2 or 3. High bandwidth ISO transfers are yet implemented.

    Bulk transfers. – For Bulk transfer this value must always be ‘1u’. This
    parameter will be used with the auto-amalgamation/auto-split feature where
    it indicates number of bulk packets to be auto-amalgamated/auto-split in
    bulk transfer. The auto-amalgamation/auto-split feature is not available
    yet.

  @param dma_enable
    The dma_enable parameter specifies whether or not the internal DMA should
    be used for the data transfer from the provided buffer to the USB FIFO.

  @param dma_channel
    The dma_channel parameter specifies the internal DMA channel to be used
    for the data transfers. DMA channel will be associated with selected
    endpoint. Unique DMA channel must be selected to transfer data on individual
    endpoints. This parameter is ignored when dma_enable parameter indicates
    that the DMA should not be used.

  @param xfr_type
   The xfr_type parameter specifies the type of transfer to be performed on the
   selected endpoint. Except control transfer, all other types of transfers
   (Interrupt, Isochronous, and Bulk) can be selected.

   @param add_zlp
    The add_zlp parameter indicates whether a Zero length packet (ZLP) should be
    sent if the transfer is exact multiple of wMaxPacketSize. This parameter is
    only applicable for Bulk transfers. For all other transfer types this
    parameter is ignored.


  @return
    This function does not return a value.

  Example:
  @code
        uint8_t
        USBD_HID_init_cb
        (
            uint8_t cfgidx,
            mss_usb_device_speed_t musb_speed
        )
        {
            g_tx_complete_status = 1;
            MSS_USB_device_tx_ep_configure(HID_INTR_IN_EP,
                                           HID_INTR_IN_EP_FIFO_ADDR,
                                           HID_INTR_IN_EP_MAX_PKT_SIZE,
                                           HID_INTR_IN_EP_MAX_PKT_SIZE,
                                           1,
                                           DMA_DISABLE,
                                           MSS_USB_DMA_CHANNEL1,
                                           MSS_USB_XFR_INTERRUPT,
                                           NO_ZLP_TO_XFR);

            g_usb_hid_state = USBD_HID_CONFIGURED;
            return SUCCESS;
        }
  @endcode
*/
void
MSS_USBD_tx_ep_configure
(
    mss_usb_ep_num_t ep_num,
    uint16_t fifo_addr,
    uint16_t fifo_size,
    uint16_t max_pkt_size,
    uint8_t num_usb_pkt,
    uint8_t dma_enable,
    mss_usb_dma_channel_t dma_channel,
    mss_usb_xfr_type_t xfr_type,
    uint32_t add_zlp
);

/***************************************************************************//**
  @brief MSS_USBD_tx_ep_write()
    The MSS_USBD_tx_ep_write() API writes the data provided by user into the
    previously configured transmit endpoint FIFO. After calling this API, the
    data in EP FIFO is transmitted on USB bus on receiving the next IN token
    from USB Host. After complete data has been transferred callback function
    will be called to indicate successful data-in transaction.

  @param ep_num
    The ep_num parameter specifies the endpoint number on which the data is to
    be transmitted.

  @param addr
    The addr parameter is the address of the buffer provided by user from which
    the data is copied to the selected endpoint buffer.

  @param Length
    The length parameter specifies the length of the data buffer in bytes.

  @return
    This function does not return a value.

  Example:
  @code
        void
        USBD_MSC_send_data
        (
            uint8_t* buf,
            uint32_t len
        )
        {

            g_bot_state = BOT_DATA_IN;
            MSS_USBD_tx_ep_write(MSS_USB_TX_EP_1, buf, len);
            g_current_command_csw.data_residue -= len;
        }
  @endcode
*/
void
MSS_USBD_tx_ep_write
(
    mss_usb_ep_num_t ep_num,
    uint8_t * addr,
    uint32_t length
);

/***************************************************************************//**
  @brief MSS_USBD_tx_ep_stall()
    The MSS_USBD_tx_ep_stall() API is used to send a stall condition on selected
    transmit endpoint.

  @param ep_num
    The ep_num parameter specifies the transmit endpoint number which is to be
    stalled.

  @return
    This function does not return a value.

  Example:
  @code
        if(g_current_command_csw.status == SCSI_COMMAND_FAILED)
        {
            MSS_USBD_tx_ep_stall(MSS_USB_TX_EP_1);
        }
  @endcode
*/
void
MSS_USBD_tx_ep_stall
(
    mss_usb_ep_num_t ep_num
);

/***************************************************************************//**
  @brief MSS_USBD_tx_ep_clr_stall()
    The MSS_USBD_tx_ep_clr_stall() API is used to clear a stall condition on
    previously stalled transmit endpoint.

  @param ep_num
    The ep_num parameter specifies the transmit endpoint number on which the
    stall condition is to be cleared.

  @return
    This function does not return a value.

  Example:
  @code
        MSS_USBD_tx_ep_clr_stall(MSS_USB_TX_EP_1);
  @endcode
*/
void
MSS_USBD_tx_ep_clr_stall
(
    mss_usb_ep_num_t ep_num
);

/***************************************************************************//**
  @brief MSS_USBD_tx_ep_flush_fifo()
    The MSS_USBD_tx_ep_flush_fifo() API is used to flush the content of the
    selected transmit endpoint FIFO.

  @param ep_num
    The ep_num parameter specifies the transmit endpoint number whose content
    is to be flushed.

  @return
    This function does not return a value.

  Example:
  @code
        MSS_USBD_tx_ep_flush_fifo(MSS_USB_TX_EP_1);
  @endcode
*/
void
MSS_USBD_tx_ep_flush_fifo
(
    mss_usb_ep_num_t ep_num
);

/***************************************************************************//**
  @brief MSS_USBD_tx_ep_is_fifo_notempty()
    The MSS_USBD_tx_ep_fifo_notempty() API is used to know whether or not the
    transmit endpoint FIFO is empty.

  @param ep_num
    The ep_num parameter specifies the transmit endpoint number whose FIFO is
    to be checked.

  @return
    A non zero return value indicates that there is at least one packet in the
    transmit endpoint FIFO.

  Example:
  @code
  @endcode
*/
uint8_t
MSS_USBD_tx_ep_is_fifo_notempty
(
    mss_usb_ep_num_t epnum
);

/***************************************************************************//**
  @brief MSS_USBD_tx_ep_enable_irq()
    The MSS_USBD_tx_ep_enable_irq() API is used to enable interrupt on the
    selected transmit endpoint.

  @param ep_num
    The ep_num parameter specifies the transmit endpoint number on which the
    interrupt is to be enabled.

  @return
    This function does not return a value.

  Example:
  @code
        MSS_USBD_tx_ep_enable_irq(MSS_USB_TX_EP_1);
  @endcode
*/
void
MSS_USBD_tx_ep_enable_irq
(
    mss_usb_ep_num_t epnum
);

/***************************************************************************//**
  @brief MSS_USBD_tx_ep_disable_irq()
    The MSS_USBD_tx_ep_disable_irq() API is used to disable interrupt on the
    selected transmit endpoint.

  @param ep_num
    The ep_num parameter specifies the transmit endpoint number on which the
    interrupt is to be disabled.

  @return
    This function does not return a value.

  Example:
  @code
        MSS_USBD_tx_ep_disable_irq(MSS_USB_TX_EP_1);
  @endcode
*/
void
MSS_USBD_tx_ep_disable_irq
(
    mss_usb_ep_num_t epnum
);

/***************************************************************************//**
  @brief MSS_USBD_set_isoupdate()
    The MSS_USBD_set_isoupdate() API is used to make the transmit endpoint wait
    for transmitting the data till a SOF packet is received. After calling this
    function, from the time data is deemed ready, the Isochronous transmit
    endpoint is forced to wait for an SOF token before IN token is received.
    If IN token is received before an SOF token, then a zero length data packet
    will be sent.

    Note: All the transmit endpoint which use Isochronous transfers are affected
    by this function.

  @param
    This API does not take any parameters.

  @return
    This function does not return a value.

  Example:
  @code
        MSS_USBD_set_isoupdate();
  @endcode
*/
void
MSS_USBD_set_isoupdate
(
    void
);

/***************************************************************************//**
  @brief MSS_USBD_clr_isoupdate()
    The MSS_USBD_clr_isoupdate() API is used to configure all the Isochronous
    transmit endpoint such that, once the data is deemed ready, Isochronous
    endpoint will not wait for SOF token to arrive before IN token. Data will
    be transmitted on the next received IN token.

    This is the default behavior of an Isochronous transmit endpoint. This API
    has effect only if the endpoint was previously configured by
    MSS_USBD_set_isoupdate() API.

    Note: All the transmit endpoint which use Isochronous transfers are affected
    by this function.

  @param
    This API does not take any parameters.

  @return
    This function does not return a value.

  Example:
  @code
        MSS_USBD_clr_isoupdate();
  @endcode
*/
void
MSS_USBD_clr_isoupdate
(
    void
);

/***************************************************************************//**
  @brief MSS_USBD_rx_ep_configure()
    The MSS_USBD_rx_ep_configure() API configures the receive endpoint with the
    configuration values provided as parameters. After initializing the core,
    this API must be called before any other API for the desired receive endpoint.

    Note:  To provide complete flexibility in configuring the FIFOs, this driver
    does not make any assumptions on the FIFO size and FIFO address. User is
    responsible to make sure that the endpoint FIFO does not overlap with other
    endpoint FIFO space.

  @param ep_num
    The ep_num parameter is the endpoint number on which USB transfers are to be
    performed. The endpoint number must be provided using the mss_usb_ep_num_t
    type.

  @param fifo_addr
    The fifo_addr parameter is the address of the FIFO in the internal RAM in the
    USB core. Valid FIFO address values are from 0x00 to 0xFFF8. FIFO Address must
    be multiple of 8. If the value provided is not multiple of 8, then the
    immediate lower value which is multiple of 8 is taken as the FIFO address.
    E.g. If the provided value is 0x09, the actual value taken by driver is 0x08.
    If the provided value is less than 0x08 then FIFO address is taken as 0x00.

  @param fifo_size
    The fifo_size parameter provides the endpoint FIFO size in the USB core
    internal RAM. Valid FIFO size values are 8, 16, 32, 64, 128, 512, 1024,
    2048, 4096. MSS USB core by default assigns 8 byte FIFO if the FIFO size
    is not configured.

  @param max_pkt_size
    The max_pkt_size parameter provides the maximum packet size of the USB
    transfer. This value must be equal to the maximum packet size as
    mentioned in the endpoint descriptor which is used during enumeration
    process.

    Note: This value must be less than or equal to the FIFO size value.

  @param num_usb_pkt
    The num_usb_pkt parameter has different meanings for different types of
    transfers.

    Low bandwidth ISO/interrupt transfers – This parameter must always be ‘1u’.
    This parameter represents the number of packets transferred in one (micro)
    frame.

    High bandwidth ISO transfers – This parameter represents the number of
    packets transferred in one (Micro) frame. In this case, this parameter can
    have a value of 1 2 or 3. High bandwidth ISO transfers are yet implemented.

    Bulk transfers. – For Bulk transfer this value must always be ‘1u’. This
    parameter will be used with the auto-amalgamation/auto-split feature where
    it indicates number of bulk packets to be auto-amalgamated/auto-split in
    bulk transfer. The auto-amalgamation/auto-split feature is implemented but
    not yet tested.

  @param dma_enable
    The dma_enable parameter specifies whether or not the internal DMA should be
    used for the data transfer from the provided buffer to the USB FIFO.

  @param dma_channel
    The dma_channel parameter specifies the internal DMA channel to be used for
    the data transfers. DMA channel will be associated with selected endpoint.
    Unique DMA channel must be selected to transfer data on individual endpoints.
    This parameter is ignored when dma_enable parameter indicates that the DMA
    should not be used.

  @param xfr_type
   The xfr_type parameter specifies the type of transfer to be performed on the
   selected endpoint. Except control transfer, all other types of transfers
   (Interrupt, Isochronous, and Bulk) can be selected.

   @param add_zlp
    The add_zlp parameter indicates whether to expect a Zero length packet (ZLP)
    if the transfer is exact multiple of wMaxPacketSize. This parameter is only
    applicable for Bulk transfers. For all other transfer types this parameter
    is ignored

  @return
    This function does not return a value.


  Example:
  @code
        MSS_USBD_rx_ep_configure(MSS_USB_RX_EP_1,
                         0x100u,
                         bulk_rxep_fifo_sz,
                         bulk_rxep_maxpktsz,
                         1u,
                         DMA_ENABLE,
                         MSS_USB_DMA_CHANNEL1,
                         MSS_USB_XFR_BULK,
                         NO_ZLP_TO_XFR);

        MSS_USBD_rx_ep_read_prepare(MSS_USB_RX_EP_1,
                                    (uint8_t*)&g_bot_cbw,
                                    31u);
  @endcode
*/

void
MSS_USBD_rx_ep_configure
(
    mss_usb_ep_num_t ep_num,
    uint16_t fifo_addr,
    uint16_t fifo_size,
    uint16_t max_pkt_size,
    uint8_t num_usb_pkt,
    uint8_t dma_enable,
    mss_usb_dma_channel_t dma_channel,
    mss_usb_xfr_type_t xfr_type,
    uint32_t add_zlp
);

/***************************************************************************//**
  @brief MSS_USBD_rx_ep_read_prepare()
    The MSS_USBD_rx_ep_read_prepare() API prepares the previously configured
    receive endpoint for receiving data over USB. After calling this function,
    data will be received on the selected endpoint when the data arrives with
    the next OUT packet.

    Note: Make sure that MSS_USBD_rx_ep_configure() is called before this API
    is called.

  @param ep_num
    The ep_num parameter specifies the endpoint number on which the data is to be
    received.

  @param addr
    The addr parameter is the address of the buffer to which the received data
    is copied from the selected endpoint buffer.

  @param length
    The length parameter specifies the length of the buffer in bytes.

  @return
    This function does not return a value.

  Example:
  @code
        MSS_USBD_rx_ep_configure(MSS_USB_RX_EP_1,
                             0x100u,
                             bulk_rxep_fifo_sz,
                             bulk_rxep_maxpktsz,
                             1u,
                             DMA_ENABLE,
                             MSS_USB_DMA_CHANNEL1,
                             MSS_USB_XFR_BULK,
                             NO_ZLP_TO_XFR);

    MSS_USBD_rx_ep_read_prepare(MSS_USB_RX_EP_1,
                               (uint8_t*)&g_bot_cbw,
                                31u);

  @endcode
*/
void
MSS_USBD_rx_ep_read_prepare
(
    mss_usb_ep_num_t ep_num,
    uint8_t* addr,
    uint32_t length
);

/***************************************************************************//**
  @brief MSS_USBD_rx_ep_stall()
    The MSS_USBD_rx_ep_stall() API is used to send a stall condition on selected
    receive endpoint.

  @param ep_num
    The ep_num parameter specifies the receive endpoint number which is to be
    stalled.

  @return
    This function does not return a value.

  Example:
  @code
        MSS_USBD_rx_ep_stall(MSS_USB_RX_EP_1);
  @endcode
*/
void
MSS_USBD_rx_ep_stall
(
    mss_usb_ep_num_t ep_num
);

/***************************************************************************//**
  @brief MSS_USBD_rx_ep_clr_stall()
    The MSS_USBD_rx_ep_clr_stall() API is used to clear a stall condition on
    previously stalled receive endpoint.

  @param ep_num
    The ep_num parameter specifies the receive endpoint number on which the
    stall condition is to be cleared.

  @return
    This function does not return a value.

  Example:
  @code
        MSS_USBD_rx_ep_clr_stall(MSS_USB_RX_EP_1);
  @endcode
*/
void
MSS_USBD_rx_ep_clr_stall
(
    mss_usb_ep_num_t ep_num
);

/***************************************************************************//**
  @brief MSS_USBD_rx_ep_flush_fifo()
    The MSS_USBD_rx_ep_flush_fifo() API is used to flush the content of the
    selected receive endpoint FIFO.

  @param ep_num
    The ep_num parameter specifies the receive endpoint number whose content is
    to be flushed.

  @return
    This function does not return a value.

  Example:
  @code
        MSS_USBD_rx_ep_flush_fifo(MSS_USB_RX_EP_1);
  @endcode
*/
void
MSS_USBD_rx_ep_flush_fifo
(
    mss_usb_ep_num_t ep_num
);

/***************************************************************************//**
  @brief MSS_USBD_rx_ep_is_fifo_full()
    The MSS_USBD_rx_ep_is_fifo_full() API is used to know whether or not
    selected receive endpoint FIFO is full.

  @param ep_num
    The ep_num parameter specifies the receive endpoint number whose FIFO needs
    to be checked.

  @return
    This function does not return a value.

  Example:
  @code
        MSS_USBD_rx_ep_is_fifo_full(MSS_USB_RX_EP_1);
  @endcode
*/
uint8_t
MSS_USBD_rx_ep_is_fifo_full
(
    mss_usb_ep_num_t epnum
);

/***************************************************************************//**
  @brief MSS_USBD_rx_ep_enable_irq()
    The MSS_USBD_rx_ep_enable_irq() API is used to enable interrupt on the
    selected receive endpoint.

  @param ep_num
    The ep_num parameter specifies the receive endpoint number on which the
    interrupt is to be enabled.

  @return
    This function does not return a value.

  Example:
  @code
        MSS_USBD_rx_ep_enable_irq(MSS_USB_RX_EP_1);
  @endcode
*/
void
MSS_USBD_rx_ep_enable_irq
(
    mss_usb_ep_num_t epnum
);

/***************************************************************************//**
  @brief MSS_USBD_rx_ep_disable_irq()
    The MSS_USBD_rx_ep_disable_irq() API is used to disable interrupt on the
    selected receive endpoint.

  @param ep_num
    The ep_num parameter specifies the receive endpoint number on which the
    interrupt is to be disabled.

  @return
    This function does not return a value.

  Example:
  @code
        MSS_USBD_rx_ep_disable_irq(MSS_USB_RX_EP_1);
  @endcode
*/
void
MSS_USBD_rx_ep_disable_irq
(
    mss_usb_ep_num_t epnum
);

/***************************************************************************//**
  @brief MSS_USBD_get_dev_address()
    The MSS_USBD_get_dev_address() API is used to read the current USB address
    by which the host addresses this device. By default this address is 0x00.
    This address gets updated during enumeration process when the USB Host
    assigns a non-zero address to this device using SET_ADDRESS request.

  @param
    This function does not take any parameter.

  @return
    This function returns 8-bit value indicating the current USB address to which
    this device responds over USB.

  Example:
  @code
        MSS_USBD_get_dev_address();
  @endcode
*/
uint8_t
MSS_USBD_get_dev_address
(
    void
);

#endif  /* __MSS_USB_DEVICE_H_ */
