/*******************************************************************************
 * (c) Copyright 2012-2013 Microsemi SoC Products Group.  All rights reserved.
 *
 * Smartfusion2 MSS USB Driver Stack.
 *      USB Logical Layer (USB-LL)
 *          USBD-MSC class driver.
 *
 *  USBD-MSC class driver public API.
 *
 * SVN $Revision: 5468 $
 * SVN $Date: 2013-03-29 15:38:01 +0530 (Fri, 29 Mar 2013) $
 */

/*=========================================================================*//**
  @mainpage Smartfusion2 MSS USB Driver Stack 
                USB Logical Layer (USB-LL)
                    USBD-MSC driver
                
  @section intro_sec Introduction
  The Mass Storage Class device driver implements the USB MSC Device specified by 
  USB-IF. This driver enables easy implementation of Mass storage drive 
  functionality on Smarfusion2 devices. 

  This driver implements MSD class using Bulk Only transport. One BULK IN and one
  BULK OUT endpoints are used to implement BOT. This driver is independent of the
  storage medium used for storing data and the number of LUN it supports.

  This driver uses the USBD-Class driver template to implement the USB MSC Device.

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
  MSC class driver must be initialized using MSS_USBD_MSC_init() function. Once 
  initialized, this driver will get configured by USBD Driver during the process
  of Enumeration. The Call-back function MSS_USBD_MSC_Init_cb() is implemented by
  this driver which will be called by USBD Driver when Host configures the device.
  The MSS_USBD_MSC_get_descriptor_cb() is implemented to provide class specific 
  descriptor table to the USBD Driver.

  For successful enumeration as a MSC class device, this driver defines descriptor
  table which contains a configuration descriptor, an interface descriptor, two 
  endpoint descriptors.

  Note: For successful enumeration, device specific descriptors must also be 
  provided by application using MSS_USBD_device_set_desc_cb_handler() function to
  the USBD Driver. Since Device descriptor, string descriptors etc. are not Class
  specific, they are not part of MSC Class driver.

  Class Specific requests 
  The MSS_USBD_MSC_process_request_cb() call-back function is implemented by this 
  driver which processes the MSC class specific requests. This driver may not have
  all the information related to storage medium e.g. storage capacity etc. To 
  process such requests; they are passed on to the application by calling appropriate
  call-back functions. The application must implement these call-back functions for
  the driver to successfully respond to the host requests. Refer Data Structures 
  section for more information.

  Data transfer
  The MSC class driver performs the data transfers using one BULK IN endpoint and 
  one BULK OUT endpoint. The data transfers follow the BoT protocol. To get the 
  information on data transfer events on the USB bus, this driver implements 
  MSS_USBD_MSC_datain_cb()  and MSS_USBD_MSC_dataout_cb() which are called by 
  USBD Driver.

  BoT read/write operations happen on Logical Units of fixed block sizes. This 
  driver translates these read write operation into memory read/write operation 
  and calls the call-back function where application can perform the memory 
  read/write operations. This method makes MSC class driver independent of the 
  storage medium used. The call-Back functions elements media_acquire_write_buf, 
  media_write_ready and media_read (as part of structure of type
  mss_usbd_msc_media_t) must be implemented by the application.

 *//*=========================================================================*/  
  
#ifndef __MSS_USB_DEVICE_MSD_H_
#define __MSS_USB_DEVICE_MSD_H_

#include <stdint.h>
#include <mss_usb_device.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * User Descriptor lengths
 */

/* Full configuration descriptor length*/
#define FULL_CONFIG_DESCR_LENGTH                    (USB_STD_CONFIG_DESCR_LEN + \
                                                     USB_STD_INTERFACE_DESCR_LEN + \
                                                     USB_STD_ENDPOINT_DESCR_LEN + \
                                                     USB_STD_ENDPOINT_DESCR_LEN )

/***************************************************************************//**
 Exported Types from USBD-MSC class driver
 */
/***************************************************************************//**
 The mss_usbd_msc_state_t provides a type to identify the current state of the 
 MSC class driver.

 USBD_MSC_NOT_CONFIGURED -- USB MSC class driver is not configured and it 
                            cannot perform data transfers.

 USBD_MSC_CONFIGURED    --  USB MSC class driver is configured by Host and it 
                            can perform data transfers.

 Note:    Application must not access the Flash media (on board SPI Flash in this 
 case) while the MSC class driver is in USBD_MSC_CONFIGURED state because the USB
 host might be performing read/write operation on the Flash media. If application
 also attempts to access Flash media at the same time, the data would be 
 corrupted and host will get misleading information from Flash Media.
 */
typedef enum mss_usbd_msc_state {
    USBD_MSC_NOT_CONFIGURED,
    USBD_MSC_CONFIGURED,
}mss_usbd_msc_state_t;

/***************************************************************************//**
 mss_usbd_setup_pkt_t
    The mss_usbd_msc_media_t provides the prototype for all the call-back 
    functions which should be implemented by the user application. User 
    application must define and initialize a structure of this type and provide
    the address of that structure as parameter to MSS_USBD_MSC_init() function.

    media_init
    The function pointed by the media_init function pointer will be called to 
    indicate that the MSC driver is configured by the USB host. The application 
    can use this function to prepare itself for data exchange with the host. This
    driver cannot perform data transfers unless it is configured by the USB host
    to do so.

    media_get_capacity
    The function pointed to by the media_get_capacity function pointer is called 
    to know the capacity of the LUN on the storage medium. The parameter lun 
    indicates the Logical unit on which the current operation is being performed.
    The Application must return address of the last logical block and size of 
    logical block in parameters last_block_lba and block_size.

    media_is_ready 
    The function pointed by the media_is_ready function pointer is called to know
    if the LUN on media is ready for data transfers. The parameter lun indicates 
    the Logical unit on which the current operation is being performed.

    media_is_write_protected
    The function pointed by the media_is_write_protected function pointer is 
    called to know if the LUN on the media is write-protected, before a write 
    operation can be performed on the LUN. The parameter lun indicates the 
    Logical unit on which the current operation is being performed.

    msd_media_read
    The function pointed by the msd_media_read function pointer is called when 
    the host wants to read the data from the storage medium. Application must 
    provide a buffer and its length which can be sent to the host. The parameter
    lun indicates the Logical unit on which the current operation is being 
    performed. The blk_addr parameter provides the logical block address and the
    length parameter provides the number of bytes to be read. Appplication must 
    provide a buffer in return parameter buf and the length of this buffer must 
    be provided as the return value of this function.

    msd_media_acquire_write_buffer
    The function pointed by the msd_media_acquire_write_buffer function pointer 
    is called when the host wants to write data on to the storage medium. 
    Application must provide a buffer where the data sent by host can be stored.
    The parameter lun indicates the Logical unit on which the current operation
    is being performed. The blk_addr parameter provides the logical block 
    address where data is to be written. Application must provide a buffer as a
    return value of this function and the length of the buffer must be provided
    in the return parameter length.

    media_write_ready
    The function pointed by the media_write_ready function pointer is called when
    the data to be written is received from the host and is ready to be written 
    on the storage medium. The data is stored in the previously provided write 
    buffer using media_acquire_write_buffer. The parameter lun indicates the 
    Logical unit on which the current operation is being performed. The blk_addr
    parameter provides the logical block address where data is to be written. 
    The parameter length provides the number of bytes to be written.

    media_get_max_lun
    The function pointed by the media_get_max_lun function pointer is called to
    know the maximum number of logical units supported by the application or 
    storage medium. 

    media_inquiry 
    The function pointed by the media_inquiry function pointer is called to get
    the response for the INQUIRY command request from MSD host. The parameter 
    lun indicates the Logical unit on which the current operation is being 
    performed. Application must provide a buffer as a return value of this 
    function and must provide the length of this buffer in parameter length.
 */

typedef struct mss_usbd_msc_media {
    uint8_t (*media_init)(uint8_t lun);
    uint8_t (*media_get_capacity)(uint8_t lun,
                                  uint32_t *last_block_lba,
                                  uint32_t *block_size);

    uint8_t (*media_is_ready)(uint8_t lun);
    uint8_t (*media_is_write_protected)(uint8_t lun);
    uint32_t(*media_read)(uint8_t lun,
                          uint8_t **buf,
                          uint32_t blk_addr,
                          uint32_t len);

    uint8_t*(*media_acquire_write_buf)(uint8_t lun,
                                       uint32_t blk_addr,
                                       uint32_t *len);

    uint32_t(*media_write_ready)(uint8_t lun,
                                 uint32_t blk_addr,
                                 uint32_t len);

    uint8_t (*media_get_max_lun)(void);
    uint8_t*(*media_inquiry)(uint8_t lun, uint32_t *len);

} mss_usbd_msc_media_t;

/***************************************************************************//**
 Types Internal to the USBD-MSC class driver
 */
typedef enum mss_usbd_msc_bot_events {
    BOT_EVENT_IDLE,
    BOT_EVENT_DATAIN,
    BOT_EVENT_DATAOUT,
    BOT_EVENT_DATAIN_ERROR,
    BOT_EVENT_DATAOUT_ERROR
} mss_usbd_msc_bot_events_t;

typedef enum mss_usbd_msc_csw_status {
    SCSI_COMMAND_PASSED,
    SCSI_COMMAND_FAILED,
    SCSI_COMMAND_PHASE_ERR
} mss_usbd_msc_csw_status_t;

typedef enum mss_usbd_msc_bot_state {
    BOT_IDLE,
    BOT_DATA_OUT,
    BOT_DATA_IN,
    BOT_SEND_STATUS
} mss_usbd_msc_bot_state_t;

/*  Command Block Warpper (CBW) */
typedef struct mss_usb_msc_cbw {
    uint32_t signature;
    uint32_t tag;
    uint32_t xfr_length;
    uint8_t flags;
    uint8_t lun;
    uint8_t cmd_length;
    uint8_t cmd_block[16];
} mss_usb_msc_cbw_t;

/* Command Status Wrapper (CSW) */
typedef struct mss_usbd_msc_csw {
    uint32_t signature;
    uint32_t tag;
    uint32_t data_residue;
    mss_usbd_msc_csw_status_t status;
} mss_usbd_msc_csw_t;

/* SCSI inquiry response */
typedef struct mss_usbd_msc_scsi_inq_resp {
    uint8_t peripheral;
    uint8_t removable;
    uint8_t version;
    uint8_t resp_data_format;
    uint8_t additional_length;
    uint8_t sccstp;
    uint8_t bqueetc;
    uint8_t cmd_que;
    uint8_t vendor_id[8];
    uint8_t product_id[16];
    uint8_t product_rev[4];
} mss_usbd_msc_scsi_inq_resp_t;

typedef struct mss_usbd_msc_scsi_sense_resp {
    const uint8_t response_code;
    const uint8_t obsolete;
    uint8_t sense_key;
    const uint8_t info[4];
    const uint8_t additional_length;
    const uint8_t command_info[4];
    uint8_t asc;
    const uint8_t unused[5];
} mss_usbd_msc_scsi_sense_resp_t;

typedef struct mss_usbd_msc_lun_capacity {
    uint8_t last_lba_msb;
    uint8_t last_lba_2;
    uint8_t last_lba_1;
    uint8_t last_lba_lsb;
    uint8_t block_size_msb;
    uint8_t block_size_2;
    uint8_t block_size_1;
    uint8_t block_size_lsb;
    uint32_t blk_sz_len;
} mss_usbd_msc_lun_capacity_t;

/***************************************************************************//**
 Exported functions from USBD-MSC class driver
 */

/***************************************************************************//**
  @brief MSS_USBD_MSC_init()
    The MSS_USBD_MSC_init() API must be used by the application to initialize the
    MSC class driver. A pointer to the structure of type mss_usbd_msc_media_t must
    be passed as a parameter to this function.

  @param media_ops
    The media_ops parameter is a pointer to the structure of type 
    mss_usbd_msc_media_t. This will be used by the MSC class driver to call the
    call-back functions implemented by application.

  @param speed
    The speed parameter indicates the USB speed at which this class driver should 
    operate.
  
  @return
    This function does not return a value.

  Example:
  @code
        //Assign call-back function handler structure needed by MSD class driver
        MSS_USBD_MSC_init(&usb_flash_media, MSS_USB_DEVICE_HS);

        //Assign call-back function handler structure needed by USB Device Core driver
        MSS_USBD_set_desc_cb_handler(&flash_drive_descr_cb);

        //Initialize USB driver HS device mode
        MSS_USB_device_init(MSS_USB_DEVICE_HS);

  @endcode
*/
void
MSS_USBD_MSC_init
(
    mss_usbd_msc_media_t* media_ops,
    mss_usb_device_speed_t speed
);

/***************************************************************************//**
  @brief USBD_get_state()
    The MSS_USBD_MSC_get_state() API can be used by the application to know the
    current state of the MSC class driver.

  @param media_ops
    This function does not take a parameter.

  @return
    This function returns a value of type mss_usbd_msc_state_t indicating the 
    current state of the MSC class driver. 

  Example:
  @code
        uint8_t
        usb_flash_media_get_capacity
        (
            uint8_t lun,
            uint32_t *no_of_blocks,
            uint32_t *block_size
        )
        {
           if(USBD_MSC_CONFIGURED == MSS_USBD_MSC_get_state())
           {
              *no_of_blocks = lun_data[lun].number_of_blocks;
              *block_size = lun_data[lun].lba_block_size;
              return 1;
           }
           Else
           {
              Return 0;
           }
        }

  @endcode
*/
mss_usbd_msc_state_t
MSS_USBD_MSC_get_state
(
    void
);

#ifdef __cplusplus
}
#endif

#endif  /* __MSS_USB_DEVICE_MSD_H_ */
