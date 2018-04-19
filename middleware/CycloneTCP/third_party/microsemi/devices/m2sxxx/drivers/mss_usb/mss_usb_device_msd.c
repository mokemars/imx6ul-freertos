/*******************************************************************************
 * (c) Copyright 2012-2013 Microsemi SoC Products Group.  All rights reserved.
 *
 * Smartfusion2 MSS USB Driver Stack
 *      USB Logical Layer (USB-LL)
 *          USBD-MSC class driver.
 *
 * USBD-MSC class driver implementation:
 * This source file implements Mass storage class functionality.
 * BoT with Transparent SCSI command set is implemented.
 *
 * SVN $Revision: 5468 $
 * SVN $Date: 2013-03-29 15:38:01 +0530 (Fri, 29 Mar 2013) $
 */

#include "mss_usb_device_msd.h"
#include "mss_usb_device.h"
#include "mss_usb_std_def.h"
#include "../../CMSIS/mss_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MSC_CLASS_INTERFACE_NUM                         0x00
/*HS Operation values*/
#define MSC_TX_EP_FIFO_SIZE_HS                          512
#define MSC_RX_EP_FIFO_SIZE_HS                          512

/*Values should be same as described in ep descriptors*/
#define MSC_TX_EP_MAX_PKT_SIZE_HS                       512
#define MSC_RX_EP_MAX_PKT_SIZE_HS                       512

/*LS Operation values*/
#define MSC_TX_EP_FIFO_SIZE_FS                          64
#define MSC_RX_EP_FIFO_SIZE_FS                          64

/*Values should be same as described in ep descriptors*/
#define MSC_TX_EP_MAX_PKT_SIZE_FS                       64
#define MSC_RX_EP_MAX_PKT_SIZE_FS                       64


#define G_BOT_CBW_INIT();                           g_bot_cbw.signature = 0;\
                                                    g_bot_cbw.tag = 0;\
                                                    g_bot_cbw.xfr_length = 0;\
                                                    g_bot_cbw.flags = 0;\
                                                    g_bot_cbw.lun = 0;\
                                                    g_bot_cbw.cmd_length = 0;\
                                                    g_bot_cbw.cmd_block[0]=(uint8_t)0;\
                                                    g_bot_cbw.cmd_block[1]=(uint8_t)0;\
                                                    g_bot_cbw.cmd_block[2]=(uint8_t)0;\
                                                    g_bot_cbw.cmd_block[3]=(uint8_t)0;\
                                                    g_bot_cbw.cmd_block[4]=(uint8_t)0;\
                                                    g_bot_cbw.cmd_block[5]=(uint8_t)0;\
                                                    g_bot_cbw.cmd_block[6]=(uint8_t)0;\
                                                    g_bot_cbw.cmd_block[7]=(uint8_t)0;\
                                                    g_bot_cbw.cmd_block[8]=(uint8_t)0;\
                                                    g_bot_cbw.cmd_block[9]=(uint8_t)0;\
                                                    g_bot_cbw.cmd_block[10]=(uint8_t)0;\
                                                    g_bot_cbw.cmd_block[11]=(uint8_t)0;\
                                                    g_bot_cbw.cmd_block[12]=(uint8_t)0;\
                                                    g_bot_cbw.cmd_block[13]=(uint8_t)0;\
                                                    g_bot_cbw.cmd_block[14]=(uint8_t)0;\
                                                    g_bot_cbw.cmd_block[15]=(uint8_t)0;

/***************************************************************************//**
  Local functions used by USBD-MSC class driver.
 */
mss_usbd_msc_csw_status_t USBD_MSC_process_cbw(mss_usb_msc_cbw_t * cbw);
void USBD_MSC_prepare_sense_data(uint8_t lun, uint8_t skey, uint8_t asc);
void USBD_MSC_abort_cbw(uint8_t lun);
void USBD_MSC_BOT_fsm(uint8_t status, uint32_t rx_count);
void USBD_MSC_send_data(uint8_t* buf, uint32_t len);
void USBD_MSC_receive_data(uint8_t* buf, uint32_t len);
void USBD_MSC_send_csw(mss_usbd_msc_csw_status_t csw_status);
mss_usbd_msc_csw_status_t USBD_MSC_process_read_10(void);
mss_usbd_msc_csw_status_t USBD_MSC_process_write_10(void);

/***************************************************************************//**
  Implementations of Call-back functions used by USBD.
 */
uint8_t* USBD_MSC_get_descriptor_cb(uint8_t recepient,
                                    uint8_t type,
                                    uint32_t* length,
                                    mss_usb_device_speed_t musb_speed);

uint8_t USBD_MSC_init_cb(uint8_t cfgidx, mss_usb_device_speed_t musb_speed);
uint8_t USBD_MSC_deinit_cb(uint8_t cfgidx);
uint8_t USBD_MSC_data_in_cb(mss_usb_ep_num_t num, uint8_t status);

uint8_t USBD_MSC_data_out_cb(mss_usb_ep_num_t num,
                             uint8_t status,
                             uint32_t rx_count);

uint8_t USBD_MSC_process_request_cb(mss_usbd_setup_pkt_t* setup_pkt,
                                    uint8_t** buf_pp,
                                    uint32_t* length);

/*******************************************************************************
 Global variables used by USBD-MSC class driver.
 */

/*Buffer for responding to GET_MAX_LUN*/
uint8_t get_max_lun_response[] = {0x00u};

/*
 Buffer for responding to READ_FORMAT CAPACITIES command. Values will be provided
 by Apps.
 Driver Does not store Values for All the LUNs.
 Values of the The LUN on which last USB_MSC_SCSI_READ_FORMAT_CAPACITIES command
 was executed will be there in this buffer.
*/
uint8_t format_capacity_list[12] = {0};

/*
 Buffer for responding to READ_CAPACITY_10 command. Values will be provided by Apps.
 Driver retains values for all the supported LUN and uses them to calculate valid
 address range for that LUN.
 */
mss_usbd_msc_lun_capacity_t lun_capacity[4] = {{0},{0},{0},{0}};

/*
  Buffer to respond to USB_MSC_SCSI_MODE_SENSE_6 command.
  Values are generated by driver.Same values are returned for all supported LUNs.
*/
uint8_t mode_sense_response[12] =
{
    0x0Cu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u
};

/* Definition for Flash drive application call-backs. */
mss_usbd_msc_media_t *g_usbd_msc_media_ops;

/* USB current Speed of operation selected by user*/
mss_usb_device_speed_t g_usbd_msc_user_speed;

/*
  Global variables used for multi-transfer OUT transactions
*/
uint8_t* g_rx_buf_ptr = 0;
uint32_t g_rx_buf_len = 0u;
uint32_t g_rx_current_lba_addr = 0u;

/*
  Global variables used for multi-transfer IN transactions
*/
uint8_t* g_tx_buf_ptr = 0;
uint32_t g_tx_buf_len = 0u;
uint32_t g_tx_current_lba_addr = 0u;

/*
  The g_bot_state is used to store the current state of the driver during
   Bulk-only Transport (BOT) transaction.
  The drivers respose to the host request depends on this state.
*/
volatile mss_usbd_msc_bot_state_t g_bot_state;
/*
  This veriable track the current state of the USBD-MSC driver.
*/
mss_usbd_msc_state_t g_usbd_msc_state = USBD_MSC_NOT_CONFIGURED;
/*
 The g_sense_response is used to respond to REQUEST_SENSE request.
 Value of this variable is changed depending on the type of failure while
 responding to the host request.
 */
mss_usbd_msc_scsi_sense_resp_t g_sense_response;

/*
 The g_current_command_csw structure is used to provide the status of the command.
 The Values of the elements of this structure are updated according to the state
 of the driver,success/Failure of the command and type of failure.
*/
mss_usbd_msc_csw_t g_current_command_csw = {USB_MSC_BOT_CSW_SIGNATURE,
                                            0,
                                            0,
                                            SCSI_COMMAND_PASSED };

 /*
  The g_bot_cbw is used to store the current CBW sent by the the host as a
  BOT request.
 */
mss_usb_msc_cbw_t g_bot_cbw;

/* The g_bot_events is used to know the current event in the BOT transfer */
volatile mss_usbd_msc_bot_events_t g_botin_events = BOT_EVENT_IDLE;
volatile mss_usbd_msc_bot_events_t g_botout_events = BOT_EVENT_IDLE;

mss_usbd_class_cb_t usb_msd_class_cb = {USBD_MSC_init_cb,
                                        USBD_MSC_deinit_cb,
                                        USBD_MSC_get_descriptor_cb,
                                        USBD_MSC_process_request_cb,
                                        USBD_MSC_data_in_cb,
                                        USBD_MSC_data_out_cb,
                                        0,
                                        0 };

uint8_t msc_fs_conf_descr[FULL_CONFIG_DESCR_LENGTH] =
{
    /*----------------------- Configuration Descriptor -----------------------*/
    USB_STD_CONFIG_DESCR_LEN,                       /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,              /* bDescriptorType */
    FULL_CONFIG_DESCR_LENGTH,                       /* wTotalLength LSB */
    0x00u,                                          /* wTotalLength MSB */
    0x01u,                                          /* bNumInterfaces */
    0x01u,                                          /* bConfigurationValue */
    0x04u,                                          /* iConfiguration */
    0xC0u,                                          /* bmAttributes */
    0x32u,                                           /* bMaxPower */
        /*------------------------- Interface Descriptor -------------------------*/
    USB_STD_INTERFACE_DESCR_LEN,                    /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                  /* bDescriptorType */
    MSC_CLASS_INTERFACE_NUM,                        /* bInterfaceNumber */
    0x00u,                                          /* bAlternateSetting */
    0x02u,                                          /* bNumEndpoints */
    0x08u,                                          /* bInterfaceClass */
    0x06u,                                          /* bInterfaceSubClass */
    0x50u,                                          /* bInterfaceProtocol */
    0x05u,                                          /* bInterface */
    /*------------------------- IN Endpoint Descriptor --------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                        /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                    /* bDescriptorType */
    0x81u,                                            /* bEndpointAddress */
    0x02u,                                            /* bmAttributes */
    0x40u,                                            /* wMaxPacketSize LSB */ //22
    0x00u,                                            /* wMaxPacketSize MSB */ //23
    0xFFu,                                            /* bInterval */  //ignored by host for Bulk IN EP
    /*------------------------- OUT Endpoint Descriptor --------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                     /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    0x01u,                                          /* bEndpointAddress */
    0x02u,                                          /* bmAttributes */
    0x40u,                                          /* wMaxPacketSize LSB *///29
    0x00u,                                          /* wMaxPacketSize MSB *///30
    0xFFu                                           /* bInterval *//*Max NAK rate*/
};
/*
Configuration descriptor and sub-ordinate descriptors to enumerate the USB device
as Mass Storage class Device by host.
*/
uint8_t msc_hs_conf_descr[FULL_CONFIG_DESCR_LENGTH] =
{
    /*----------------------- Configuration Descriptor -----------------------*/
    USB_STD_CONFIG_DESCR_LEN,                       /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,              /* bDescriptorType */
    FULL_CONFIG_DESCR_LENGTH,                       /* wTotalLength LSB */
    0x00u,                                          /* wTotalLength MSB */
    0x01u,                                          /* bNumInterfaces */
    0x01u,                                          /* bConfigurationValue */
    0x04u,                                          /* iConfiguration */
    0xC0u,                                          /* bmAttributes */
    0x32u,                                          /* bMaxPower */
    /*------------------------- Interface Descriptor -------------------------*/
    USB_STD_INTERFACE_DESCR_LEN,                    /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                  /* bDescriptorType */
    MSC_CLASS_INTERFACE_NUM,                         /* bInterfaceNumber */
    0x00u,                                          /* bAlternateSetting */
    0x02u,                                          /* bNumEndpoints */
    0x08u,                                          /* bInterfaceClass */
    0x06u,                                          /* bInterfaceSubClass */
    0x50u,                                          /* bInterfaceProtocol */
    0x05u,                                          /* bInterface */
    /*------------------------- IN Endpoint Descriptor --------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                     /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    0x81u,                                          /* bEndpointAddress */
    0x02u,                                          /* bmAttributes */
    0x00u,                                          /* wMaxPacketSize LSB */ //22
    0x02u,                                          /* wMaxPacketSize MSB */ //23
    0xFFu,                                          /* bInterval *///ignored by host for Bulk IN EP
    /*------------------------- OUT Endpoint Descriptor --------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                     /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    0x01u,                                          /* bEndpointAddress */
    0x02u,                                          /* bmAttributes */
    0x00u,                                          /* wMaxPacketSize LSB *///29
    0x02u,                                          /* wMaxPacketSize MSB *///30
    0xFFu                                           /* bInterval */                /*Max NAK rate*/
};

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

mss_usbd_msc_state_t
MSS_USBD_MSC_get_state
(
    void
)
{
    return g_usbd_msc_state;
}
/***************************************************************************//**
 * returns the configuration descriptor requested by Host.
 */
uint8_t*
USBD_MSC_get_descriptor_cb
(
    uint8_t recepient,
    uint8_t type,
    uint32_t* length,
    mss_usb_device_speed_t musb_speed
)
{
    uint8_t* conf_desc=0;
    uint8_t* os_conf_desc=0;
    uint8_t conf_desc_len=0;
    uint8_t os_conf_desc_len=0;

    /*User Selected FS:
            Operate only in FS
      User Selected HS:
        Device connected to 2.0 Host(musb_speed = HS):Operate in HS
        Device connected to 1.x Host(musb_speed = FS):Operate in FS
    */
    if(g_usbd_msc_user_speed == MSS_USB_DEVICE_FS)
    {
        conf_desc = msc_fs_conf_descr;
        conf_desc[1u] = USB_CONFIGURATION_DESCRIPTOR_TYPE;
        conf_desc_len = sizeof(msc_fs_conf_descr);
        os_conf_desc = 0u;
        os_conf_desc_len = 0u;
    }
    else if(g_usbd_msc_user_speed == MSS_USB_DEVICE_HS)
    {
        if(musb_speed == MSS_USB_DEVICE_HS)
        {
            conf_desc = msc_hs_conf_descr;
            conf_desc[1u] = USB_CONFIGURATION_DESCRIPTOR_TYPE;
            conf_desc_len = sizeof(msc_hs_conf_descr);
            os_conf_desc = msc_fs_conf_descr;
            os_conf_desc[1u] = USB_OTHER_SPEED_CONFIG_DESCRIPTOR_TYPE;
            os_conf_desc_len = sizeof(msc_fs_conf_descr);
        }
        else if(musb_speed == MSS_USB_DEVICE_FS)
        {
            conf_desc = msc_fs_conf_descr;
            conf_desc[1u] = USB_CONFIGURATION_DESCRIPTOR_TYPE;
            conf_desc_len = sizeof(msc_fs_conf_descr);
            os_conf_desc = msc_hs_conf_descr;
            os_conf_desc[1u] = USB_OTHER_SPEED_CONFIG_DESCRIPTOR_TYPE;
            os_conf_desc_len = sizeof(msc_hs_conf_descr);
        }
    }
    else
        ASSERT(0);      //user must select FS or HS, nothing else.

    if(recepient == USB_STD_REQ_RECIPIENT_DEVICE)
    {
        if(type == USB_CONFIGURATION_DESCRIPTOR_TYPE)
        {
           *length = conf_desc_len;
            return(conf_desc);
        }
        else if (type == USB_OTHER_SPEED_CONFIG_DESCRIPTOR_TYPE)
        {
            ASSERT(os_conf_desc != 0u);
            *length = os_conf_desc_len;
            return(os_conf_desc);
        }
    }
    else if(recepient == USB_STD_REQ_RECIPIENT_ENDPOINT)    /*Need index(EP Num) here*/
    {
        /*Do nothing*/
    }
    else if(recepient == USB_STD_REQ_RECIPIENT_INTERFACE)     /*Need index(intrfs num) here*/
    {
        /*Do nothing*/
    }
    else
    {
        /*Do nothing*/
    }
    return USB_FAIL;
}

/***************************************************************************//**
 USBD_MSC_init_cb() call-back is called by USB Device mode driver on receiving
 SET_CONFIGURATION command. The MSC class specific configurations are performed
 by this function.
*/
uint8_t
USBD_MSC_init_cb
(
    uint8_t cfgidx,
    mss_usb_device_speed_t musb_speed
)
{
    uint16_t bulk_rxep_fifo_sz = 0u;
    uint16_t bulk_rxep_maxpktsz = 0u;

    uint16_t bulk_txep_fifo_sz = 0u;
    uint16_t bulk_txep_maxpktsz = 0u;

    g_botin_events = BOT_EVENT_IDLE;
    g_botout_events = BOT_EVENT_IDLE;
    g_bot_state = BOT_IDLE;
    g_current_command_csw.data_residue = 0u;
    g_current_command_csw.tag = 0u;
    g_current_command_csw.status = SCSI_COMMAND_FAILED;
    G_BOT_CBW_INIT();
    g_tx_buf_ptr = 0;
    g_tx_buf_len = 0u;
    g_rx_current_lba_addr = 0u;
    g_rx_buf_ptr = 0;
    g_rx_buf_len = 0u;
    g_rx_current_lba_addr = 0u;

    /*User Selected FS: Operate only in FS
      User Selected HS:
        Device connected to 2.0 Host(musb_speed = HS):Operate in HS
        Device connected to 1.x Host(musb_speed = FS):Operate in FS
    */
    if(musb_speed == MSS_USB_DEVICE_FS)
    {
        bulk_txep_fifo_sz = (uint16_t)((msc_fs_conf_descr[23u] << 8u) | (msc_fs_conf_descr[22u]));
        bulk_txep_maxpktsz = (uint16_t)((msc_fs_conf_descr[23u] << 8u) | (msc_fs_conf_descr[22u]));
        bulk_rxep_fifo_sz = (uint16_t)((msc_fs_conf_descr[30u] << 8u) | (msc_fs_conf_descr[29u]));
        bulk_rxep_maxpktsz = (uint16_t)((msc_fs_conf_descr[30u] << 8u) | (msc_fs_conf_descr[29u]));
    }
    else if(musb_speed == MSS_USB_DEVICE_HS)
    {
        bulk_txep_fifo_sz = (uint16_t)((msc_hs_conf_descr[23u] << 8u) | (msc_hs_conf_descr[22u]));
        bulk_txep_maxpktsz = (uint16_t)((msc_hs_conf_descr[23u] << 8u) | (msc_hs_conf_descr[22u]));
        bulk_rxep_fifo_sz = (uint16_t)((msc_hs_conf_descr[30u] << 8u) | (msc_hs_conf_descr[29u]));
        bulk_rxep_maxpktsz = (uint16_t)((msc_hs_conf_descr[30u] << 8u) | (msc_hs_conf_descr[29u]));
    }
    else
        ASSERT(0);

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

    MSS_USBD_tx_ep_configure(MSS_USB_TX_EP_1,
                             0x200u,
                             bulk_txep_fifo_sz,
                             bulk_txep_maxpktsz,
                             1u,
                             DMA_ENABLE,
                             MSS_USB_DMA_CHANNEL2,
                             MSS_USB_XFR_BULK,
                             NO_ZLP_TO_XFR);

    g_usbd_msc_state = USBD_MSC_CONFIGURED;
    return 1;
}

/***************************************************************************//**
 USBD_MSC_deinit_cb() call-back is called by USB Device mode driver on receiving
 a command to clear the configuration.
 */
uint8_t
USBD_MSC_deinit_cb
(
    uint8_t cfgidx
)
{
      g_usbd_msc_state = USBD_MSC_NOT_CONFIGURED;
    return 1;
}

/***************************************************************************//**
 USBD_MSC_process_request_cb() call-back function Process the MSC class requests.
 */
uint8_t
USBD_MSC_process_request_cb
(
    mss_usbd_setup_pkt_t* setup_pkt,
    uint8_t** buf_pp,
    uint32_t* length
)
{

    if((setup_pkt->index == MSC_CLASS_INTERFACE_NUM) &&
       (setup_pkt->value == 0u))
    {
        switch(setup_pkt->request)
        {
            case USB_MSC_BOT_REQ_GET_MAX_LUN:
                /*Return the max LUN index, not the actual number of LUNs*/
                ASSERT(g_usbd_msc_media_ops->media_get_max_lun !=0u);
                get_max_lun_response[0] = (g_usbd_msc_media_ops->media_get_max_lun() - 1u);
                *length = sizeof(get_max_lun_response);
                *buf_pp = (uint8_t*)&get_max_lun_response;
            return USB_SUCCESS;

            case USB_MSC_BOT_REQ_BMS_RESET:
                if(0u == setup_pkt->length)
                {
                    /*
                    LVp:62 - prepare for next CBW. Dont change the stall
                    and Data toggle bit on Bulk EPs.
                    */
                    g_botin_events = BOT_EVENT_IDLE;
                    g_botout_events = BOT_EVENT_IDLE;
                    g_bot_state = BOT_IDLE;
                    g_current_command_csw.data_residue = 0u;
                    g_current_command_csw.tag = 0u;
                    g_current_command_csw.status = SCSI_COMMAND_FAILED;
                    G_BOT_CBW_INIT();
                    g_tx_buf_ptr = 0;
                    g_tx_buf_len = 0u;
                    g_rx_current_lba_addr = 0;
                    g_rx_buf_ptr = 0;
                    g_rx_buf_len = 0u;
                    g_rx_current_lba_addr = 0;
                    MSS_USBD_rx_ep_read_prepare(MSS_USB_RX_EP_1,
                                                (uint8_t*)&g_bot_cbw,
                                                31u);
                    return USB_SUCCESS;
                }
                else
                {
                    return USB_FAIL;
                }

            default:
                length = 0u;
                return USB_FAIL;
        }
    }
    else
    {
        return USB_FAIL;
    }
}

/***************************************************************************//**
 USBD_MSC_data_in_cb() call-back function is called by USB Device mode driver
 on completion of the Current Data Transmissions (IN Transaction) which was
 previously initiated using MSS_USB_device_tx_ep_configure().
 */
uint8_t USBD_MSC_data_in_cb
(
    mss_usb_ep_num_t num,
    uint8_t status
)
{
    if((status & 0x1Fu) > 0x01u)
    {
        g_botin_events = BOT_EVENT_DATAIN_ERROR;
    }
    else
    {
        g_botin_events = BOT_EVENT_DATAIN;
    }
    USBD_MSC_BOT_fsm(status, 0u);

    return 1;
}

/***************************************************************************//**
 USBD_MSC_data_out_cb() call-back function is called by USB Device mode driver
 on completion of data receiption. USB Device mode driver must have been previously
 prepared for this data reception using MSS_USB_device_rx_ep_read_prepare()
 */
uint8_t
USBD_MSC_data_out_cb
(
    mss_usb_ep_num_t num,
    uint8_t status,
    uint32_t rx_count
)
{
    /*
    4.3.5.6
    xfr length > Allocation length field ==> Terminate with CHECK CONDITION status,
    sense key = ILLEGAL_REQUEST, additional sense code = INVALID FIELD in CDB
    when NACA bit = 0 ==> ACA condition is not set = command abort should be done
    as per sam-5, 5.8.2
    */
    if((status & 0x1Fu) > 0x01u)
    {
        g_botout_events = BOT_EVENT_DATAOUT_ERROR;
    }
    else
    {
        g_botout_events = BOT_EVENT_DATAOUT;
    }

    USBD_MSC_BOT_fsm(status, rx_count);

    return 1;
}

/***************************************************************************//**
 USBD_MSC_BOT_fsm() function is the state machine for BOT transfers.
 */
void
USBD_MSC_BOT_fsm
(
    uint8_t status,
    uint32_t rx_count
)
{
    mss_usbd_msc_csw_status_t command_status;
    switch (g_bot_state)
    {
        case BOT_IDLE:
            if(g_botout_events == BOT_EVENT_DATAOUT)
            {
                command_status = USBD_MSC_process_cbw(&g_bot_cbw);

                g_current_command_csw.status = command_status;

                if(g_current_command_csw.status != SCSI_COMMAND_PASSED)
                {
                   MSS_USBD_tx_ep_stall(MSS_USB_TX_EP_1);
                   if(g_bot_cbw.xfr_length == 0u)
                    {
                        USBD_MSC_send_csw(command_status);
                        USBD_MSC_prepare_sense_data(0u,
                                                    SC_ILLEGAL_REQUEST,
                                                    ASC_INVALID_CDB);
                    }
                    else if(g_bot_cbw.flags == 0x80u) /*In Transaction*/
                    {
                        /*Send ZLP if data was expected by Windows host*/
                        /*Dummy buffer*/
                        USBD_MSC_send_data(((uint8_t*)" "), 0u);
                    }
                    else
                    {
                        USBD_MSC_send_csw(command_status);
                        USBD_MSC_prepare_sense_data(0,
                                                    SC_ILLEGAL_REQUEST,
                                                    ASC_INVALID_CDB);

                        g_current_command_csw.data_residue = 0u;
                    }
                }
                else
                {
                    /*Command with no Data transfer phase*/
                    if(g_bot_cbw.xfr_length ==  0u)
                    {
                        USBD_MSC_send_csw(command_status);
                    }
                    else if(g_bot_cbw.xfr_length >  0u)
                    {
                        /*Command with either IN or OUT Data transfer phase*/
                        /*
                         Flags.bit7 indicates the directiont of data transport
                         phase. bit7 = 1 indicates IN transfers is expected.
                        */
                        if(g_bot_cbw.flags & 0x80u) /*In Transaction*/
                        {
                            /*READ_FORMAT_CAPABILITIES is expecting 252 bytes of data.*/
                            /*Inquiry command attemts to receive different length everytime.*/
                            if(g_tx_buf_len <= g_bot_cbw.xfr_length)
                            {
                                USBD_MSC_send_data(g_tx_buf_ptr, g_tx_buf_len);
                            }
                            else
                            {
                                MSS_USBD_tx_ep_stall(MSS_USB_TX_EP_1);
                                USBD_MSC_send_csw(SCSI_COMMAND_PHASE_ERR);
                                USBD_MSC_prepare_sense_data(0u,
                                                            SC_ILLEGAL_REQUEST,
                                                            ASC_INVALID_CDB);
                            }
                        }
                        else             /*Out transaction, prepare to receive data*/
                        {
                            if(g_rx_buf_len <= g_bot_cbw.xfr_length)
                            {
                                USBD_MSC_receive_data(g_rx_buf_ptr, g_rx_buf_len);
                            }
                            else
                            {
                                MSS_USBD_tx_ep_stall(MSS_USB_TX_EP_1);
                                USBD_MSC_send_csw(SCSI_COMMAND_PHASE_ERR);
                                USBD_MSC_prepare_sense_data(0u,
                                                            SC_ILLEGAL_REQUEST,
                                                            ASC_INVALID_CDB);
                            }
                        }
                    }
                }
            }
            else if(g_botout_events != BOT_EVENT_IDLE)
            {
                /*No other event is expected here.*/
                USBD_MSC_abort_cbw(0u);
            }
    break;
        case BOT_DATA_IN:
              if(g_botin_events == BOT_EVENT_DATAIN) /* Data TX from Device*/
            {
                if((g_current_command_csw.status == SCSI_COMMAND_FAILED) ||
                    (g_current_command_csw.status == SCSI_COMMAND_PHASE_ERR))
                {
                    g_rx_buf_ptr = 0;
                    g_rx_buf_len = 0u;
                    g_rx_current_lba_addr = 0;
                    MSS_USBD_tx_ep_stall(MSS_USB_TX_EP_1);
                    USBD_MSC_send_csw(g_current_command_csw.status);
                }
                else if(g_current_command_csw.status == SCSI_COMMAND_PASSED)
                {
                    if(g_current_command_csw.data_residue == 0u)
                    {
                        g_tx_buf_ptr = 0;
                        g_tx_buf_len = 0u;
                        g_rx_current_lba_addr = 0u;
                        USBD_MSC_send_csw(g_current_command_csw.status);
                    }
                    else
                    {
                        /*
                         Control reaches here because previous App read operation
                         returned less Data than requested by host.Read rest
                         of the data now.
                        */
                        uint32_t actual_read_len;
                        uint8_t* buf;

                        g_tx_current_lba_addr += g_tx_buf_len;
                        actual_read_len = g_usbd_msc_media_ops->media_read(g_bot_cbw.lun,
                                                                           &buf,
                                                                           g_tx_current_lba_addr,
                                                                           g_current_command_csw.data_residue);
                        /*App should not read more data than asked for*/
                        if (actual_read_len > g_current_command_csw.data_residue)
                        {
                            ASSERT(0);
                        }
                        else
                        {
                            g_tx_buf_ptr = buf;
                            g_tx_buf_len = actual_read_len;
                        }

                        USBD_MSC_send_data(buf, g_tx_buf_len);
                    }
                }
            }
            else if(g_botin_events == BOT_EVENT_DATAIN_ERROR)
            {
                /* Underrun error */
                if(status & 0x01)
                {
                    status &= ~0x01;
                    G_BOT_CBW_INIT();
                    g_current_command_csw.data_residue = 0u;
                    g_tx_buf_ptr = (uint8_t)0;
                    g_tx_buf_len = 0;
                    MSS_USBD_rx_ep_read_prepare(MSS_USB_RX_EP_1,
                                                (uint8_t*)&g_bot_cbw,
                                                31u);
                }
                /* Stall error */
                if(status & 0x02)
                {
                    status &= ~0x02;
                }
                USBD_MSC_abort_cbw(0u);
                USBD_MSC_prepare_sense_data(0u,
                                           SC_ILLEGAL_REQUEST,
                                           ASC_INVALID_CDB);
            }
            else if(g_botin_events != BOT_EVENT_IDLE)
            {
                ASSERT(0); //no other event expected
            }
        break;
        case BOT_DATA_OUT:
            if(g_botout_events == BOT_EVENT_DATAOUT)
            {
                if(g_current_command_csw.status == SCSI_COMMAND_FAILED)
                {
                    MSS_USBD_tx_ep_stall(MSS_USB_TX_EP_1);
                }
                else
                {
                    /*Write the received data on flash*/
                    if(rx_count == g_rx_buf_len)
                    {
                        /*
                        TODO make the return value as SUCCESS/Fail,
                        send status accordingly
                        */
                        g_current_command_csw.data_residue -= g_rx_buf_len;
                        g_usbd_msc_media_ops->media_write_ready(g_bot_cbw.lun,
                                                                g_rx_current_lba_addr,
                                                                g_rx_buf_len);
                    }
                    else
                    {
                        ASSERT(0);
                    }

                    /*
                    data_residue == 0 means we are done with current Data phase
                    of OUT transaction, send the status.
                    */
                    if(g_current_command_csw.data_residue == 0u)
                    {
                        g_rx_buf_ptr = 0; // Data end.
                        g_rx_buf_len = 0u;
                        g_rx_current_lba_addr = 0u;
                        command_status = SCSI_COMMAND_PASSED;
                        USBD_MSC_send_csw(SCSI_COMMAND_PASSED);
                    }
                    else
                    {
                        uint32_t app_write_len;
                        uint8_t* write_buf;

                        /*
                        Data phase of current OUT transaction is still in progress.
                        Prepare to write more data on flash
                        Additionally, make sure that the driver has read the same
                        amount of data that was requested.
                        */
                        if(rx_count == g_rx_buf_len)
                        {
                            g_rx_current_lba_addr += g_rx_buf_len;
                        }
                        else
                        {
                            ASSERT(0);
                        }

                        write_buf = g_usbd_msc_media_ops->media_acquire_write_buf(g_bot_cbw.lun,
                                                                                  g_rx_current_lba_addr,
                                                                                  &app_write_len);

                        /*
                        If application buffer is not big enough to handle all requested
                        data from Host, then read amount of data that the application
                        can handle at the moment
                        */
                        if(app_write_len < g_current_command_csw.data_residue)
                        {
                            g_rx_buf_len = app_write_len;
                        }
                        else
                        {
                            g_rx_buf_len = g_current_command_csw.data_residue;
                        }

                        g_rx_buf_ptr = write_buf;

                        USBD_MSC_receive_data (g_rx_buf_ptr, g_rx_buf_len);
                    }
                }
            }
            else if(g_botout_events == BOT_EVENT_DATAOUT_ERROR)
            {
                /* Underrun Error */
                if(status&0x01)
                {
                    status &= ~0x01;
                    G_BOT_CBW_INIT();
                    g_current_command_csw.data_residue = 0u;
                    g_tx_buf_ptr = (uint8_t*)0;
                    g_tx_buf_len = 0u;
                    MSS_USBD_rx_ep_read_prepare(MSS_USB_RX_EP_1,
                                                (uint8_t*)&g_bot_cbw,
                                                31u);
                }

                /* Stall Error*/
                if(status & 0x02u)
                {
                    status &= ~0x02u;
                }
                USBD_MSC_abort_cbw(0u);
                   USBD_MSC_prepare_sense_data(0u,
                                               SC_ILLEGAL_REQUEST,
                                               ASC_INVALID_CDB);
            }
            else if(g_botout_events != BOT_EVENT_IDLE)
            {
                ASSERT(0);
            }

        break;
        case BOT_SEND_STATUS:
            if(g_botin_events == BOT_EVENT_DATAIN)
            {
                G_BOT_CBW_INIT();
                g_current_command_csw.data_residue = 0u;
                g_tx_buf_ptr = (uint8_t*)0;
                g_tx_buf_len = 0u;
                g_rx_buf_ptr = (uint8_t*)0;
                g_rx_buf_len = 0u;
                g_bot_state = BOT_IDLE;
                /*Prepare to receive next command*/
                MSS_USBD_rx_ep_read_prepare(MSS_USB_RX_EP_1, (uint8_t*)&g_bot_cbw, 31u);
            }
            else if(g_botin_events == BOT_EVENT_DATAIN_ERROR)
            {
                /* Stall Error*/
                if(status & 0x02u)
                {
                    /*Keep EP stalled*/
                }
            }
            else if(g_botin_events != BOT_EVENT_IDLE)
            {
                ASSERT(0);
            }
        break;

        default:
            ASSERT(0);
        break;
    }

    g_botin_events = BOT_EVENT_IDLE;
    g_botout_events = BOT_EVENT_IDLE;
}

/***************************************************************************//**
 USBD_MSC_process_cbw() function decodes current command sent by host in CBW,
 and Processes the command. If the command was valid and meaningful, it asks for
 required information from application and returns the status. If the Command
 was invalid or is not processed by this driver, it returns the Failure status.
 */
mss_usbd_msc_csw_status_t
USBD_MSC_process_cbw
(
    mss_usb_msc_cbw_t * cbw
)
{
    uint8_t cbw_command = cbw->cmd_block[0];
    mss_usbd_msc_csw_status_t command_status = SCSI_COMMAND_FAILED;

    g_current_command_csw.tag = cbw->tag;

    /*
     Check for valid cbw
     1) CBW received after csw or reset
     2) cbw is 32 bytes
     3) Signature is correct*/

    /*
     CBW is meaninggful if all reserved bytes are zero
     Lun value is supported
     CBWCBLength and cbwcb are valid for the interface subclass.
     No. Max LUN index provided by App in get_max_lun reques.
    */
    if((cbw->signature != USB_MSC_BOT_CBW_SIGNATURE) ||
       (cbw->lun > get_max_lun_response[0]) ||
       (cbw->cmd_length < 1) ||
       (cbw->cmd_length > 16))
    {
        USBD_MSC_prepare_sense_data(cbw->lun,
                                    SC_ILLEGAL_REQUEST,
                                    ASC_INVALID_CDB);

        MSS_USBD_tx_ep_stall(MSS_USB_TX_EP_1);
        USBD_MSC_abort_cbw(0);
    }
    else
    {
        if(g_current_command_csw.data_residue != 0u)
            ASSERT(0);

        switch(cbw_command)
        {
            case USB_MSC_SCSI_INQUIRY:
              {
                /*TODO: needs to be updated for multiple luns*/
                if(g_usbd_msc_media_ops->media_inquiry != 0)
                {
                    g_tx_buf_ptr = g_usbd_msc_media_ops->media_inquiry(cbw->lun,
                                                                       &g_tx_buf_len);

                    g_current_command_csw.data_residue = g_tx_buf_len;
                    command_status = SCSI_COMMAND_PASSED;
                }
              }
            break;
            case USB_MSC_SCSI_READ_CAPACITY_10: /*returns last LBA and block size*/
              {
                /*
                 Returns last block addr and block size,
                 Last_blk_lba = no_of_blocks-1
                */
                if(g_usbd_msc_media_ops->media_get_capacity != 0)
                {
                      uint32_t no_of_blocks, block_size;
                      g_usbd_msc_media_ops->media_get_capacity(cbw->lun,
                                                               (uint32_t*)&no_of_blocks,
                                                               (uint32_t*)&block_size);

                      lun_capacity[cbw->lun].last_lba_msb = (uint8_t)((no_of_blocks - 1u) >> 24u);
                      lun_capacity[cbw->lun].last_lba_2 = (uint8_t)((no_of_blocks - 1u) >> 16u);
                      lun_capacity[cbw->lun].last_lba_1 = (uint8_t)((no_of_blocks - 1u) >> 8u);
                      lun_capacity[cbw->lun].last_lba_lsb = (uint8_t)((no_of_blocks - 1u));

                      lun_capacity[cbw->lun].blk_sz_len = block_size;
                      lun_capacity[cbw->lun].block_size_msb = (uint8_t)(block_size >> 24u);
                      lun_capacity[cbw->lun].block_size_2 = (uint8_t)(block_size >> 16u);
                      lun_capacity[cbw->lun].block_size_1 = (uint8_t)(block_size >> 8u);
                      lun_capacity[cbw->lun].block_size_lsb = (uint8_t)(block_size);

                      g_tx_buf_ptr = (uint8_t*)&lun_capacity[cbw->lun];
                      g_tx_buf_len = 8u;
                      command_status = SCSI_COMMAND_PASSED;

                      /*Windows expects different length of data everytime*/
                      g_current_command_csw.data_residue = g_tx_buf_len;
                  }
              }
                break;
            case USB_MSC_SCSI_READ_FORMAT_CAPACITIES:/*Returns no. of blocks and block size*/
              {
                  uint32_t no_of_blocks, block_size;

                  g_usbd_msc_media_ops->media_get_capacity(cbw->lun,
                                                           (uint32_t*)&no_of_blocks,
                                                           (uint32_t*)&block_size);

                  format_capacity_list [4u] = (uint8_t)(no_of_blocks >> 24u);
                  format_capacity_list [5u] = (uint8_t)(no_of_blocks >> 16u);
                  format_capacity_list [6u] = (uint8_t)(no_of_blocks >> 8u);
                  format_capacity_list [7u] = (uint8_t)(no_of_blocks);

                  /* format_capacity_list [8] = Descriptor Code*/
                  format_capacity_list [9u]  = (uint8_t)(block_size >> 16u);
                  format_capacity_list [10u] = (uint8_t)(block_size >> 8u);
                  format_capacity_list [11u] = (uint8_t)(block_size);

                  g_tx_buf_ptr = (uint8_t*)format_capacity_list;
                  g_tx_buf_len = sizeof(format_capacity_list);
                  g_current_command_csw.data_residue = g_tx_buf_len;
                  command_status = SCSI_COMMAND_PASSED;
              }
              break;
              case USB_MSC_SCSI_READ_10:
                  command_status = USBD_MSC_process_read_10();
              break;

            case USB_MSC_SCSI_MODE_SENSE_6: /*TODO: needs to be updated for multiple luns*/
                  g_tx_buf_ptr = mode_sense_response;
                  g_tx_buf_len = sizeof(mode_sense_response);
                  g_current_command_csw.data_residue = g_tx_buf_len;
                  command_status = SCSI_COMMAND_PASSED;
                break;

            case USB_MSC_SCSI_REQUEST_SENSE: /*TODO: needs to be updated for multiple luns*/
                /*SPC-4, table 39*/
                command_status = SCSI_COMMAND_PASSED;
                g_tx_buf_ptr = (uint8_t*)&g_sense_response;
                g_tx_buf_len = sizeof(g_sense_response);
                g_current_command_csw.data_residue = g_tx_buf_len;
                break;

            case USB_MSC_SCSI_MODE_SELECT_6:    /*TODO: needs to be updated for multiple luns*/
                break;

            case USB_MSC_SCSI_TEST_UNIT_READY:  /*TODO: needs to be updated for multiple luns*/
                command_status = SCSI_COMMAND_PASSED;
                g_tx_buf_ptr = (uint8_t*)0;
                g_tx_buf_len = 0u;
                g_current_command_csw.data_residue = g_tx_buf_len;
                break;

            case USB_MSC_SCSI_WRITE_10:
                command_status = USBD_MSC_process_write_10();
                break;

            case USB_MSC_SCSI_PREVENT_ALLW_MDM_RMVL:    /*TODO: needs to be updated for multiple luns*/
                command_status = SCSI_COMMAND_PASSED;
                g_tx_buf_ptr = (uint8_t*)0;
                g_tx_buf_len = 0u;
                g_current_command_csw.data_residue = g_tx_buf_len;
                break;

            default:
                command_status = SCSI_COMMAND_FAILED;
                USBD_MSC_prepare_sense_data(cbw->lun,
                                            SC_ILLEGAL_REQUEST,
                                            ASC_INVALID_CDB);
                g_current_command_csw.data_residue = g_bot_cbw.xfr_length;
                break;
        }
    }
    return command_status;
}

/***************************************************************************//**
 USBD_MSC_prepare_sense_data() function prepares sense information required
 by the host in case there was an error in processing CBW. This information is
 sent as response to REQUEST_SENSE command.
 */
void
USBD_MSC_prepare_sense_data
(
    uint8_t lun,
    uint8_t skey,
    uint8_t asc
)
{
    g_sense_response.sense_key = skey;
    g_sense_response.asc = asc;
}

/***************************************************************************//**
 USBD_MSC_abort_cbw() function aborts the current command CBW.and prepares
 the driver to receive new CBW from HOST.
 */
void
USBD_MSC_abort_cbw
(
    uint8_t lun
)
{
    /*Stall both Bulk endpoints, Stall will be cleared by clear feature command*/
    MSS_USBD_rx_ep_stall(MSS_USB_RX_EP_1);
    MSS_USBD_tx_ep_stall(MSS_USB_TX_EP_1);
}

/***************************************************************************//**
 USBD_MSC_send_data() function send data to the host (DATA phase of IN transactions)
 in response to current CBW command.
 */
void
USBD_MSC_send_data
(
    uint8_t* buf,
    uint32_t len
)
{
    /*TODO: check if the length can be accomodated in EP buffer*/
    g_bot_state = BOT_DATA_IN;
    MSS_USBD_tx_ep_write(MSS_USB_TX_EP_1, buf, len);
    g_current_command_csw.data_residue -= len;
}

/***************************************************************************//**
 USBD_MSC_receive_data() function prepares the driver to recieve data
 from USB host (DATA phase of OUT transactions) in response to the current CBW
 command.
 */
void
USBD_MSC_receive_data
(
    uint8_t* buf,
    uint32_t len
)
{
    g_bot_state = BOT_DATA_OUT;
    MSS_USBD_rx_ep_read_prepare(MSS_USB_RX_EP_1, g_rx_buf_ptr, len);
}

/***************************************************************************//**
 USBD_MSC_send_csw() function sends status information to USB host
 (DATA phase of IN transactions), indicating the Success/Failure status of the
 current CBW command.
 */
void
USBD_MSC_send_csw
(
    mss_usbd_msc_csw_status_t csw_status
)
{
    g_current_command_csw.status = csw_status;
    g_bot_state = BOT_SEND_STATUS;
    MSS_USBD_tx_ep_write(MSS_USB_TX_EP_1,
                         (uint8_t*)&g_current_command_csw,
                         USBD_MSC_BOT_CSW_LENGTH);
}

/***************************************************************************//**
 USBD_MSC_process_read_10() function processes read command and calls application
 call-back function to read data from media.
 */
mss_usbd_msc_csw_status_t
USBD_MSC_process_read_10
(
    void
)
{
    uint32_t actual_read_len = 0u;

    uint32_t lba;
    uint32_t lba_addr;
    uint8_t* buf = 0;

    mss_usbd_msc_csw_status_t command_status = SCSI_COMMAND_FAILED;

    /*Lba address is in command block byte 2 to 4 with MSB first.*/
    lba =  (g_bot_cbw.cmd_block[2u] << 24u) |
           (g_bot_cbw.cmd_block[3u] << 16u) |
           (g_bot_cbw.cmd_block[4u] << 8u) |
            g_bot_cbw.cmd_block[5u];

    /*Block size was received in USB_MSC_SCSI_READ_CAPACITY_10 command*/
    lba_addr = (lba * lun_capacity[g_bot_cbw.lun].blk_sz_len);

    /*
    Number of LBAs to be read is provided in command block byte 7 to 8,
    calculate the total size to be read
    */
    g_current_command_csw.data_residue = ((g_bot_cbw.cmd_block[7u] << 8u) | g_bot_cbw.cmd_block[8u]) *
                                            lun_capacity[g_bot_cbw.lun].blk_sz_len;

    actual_read_len = g_usbd_msc_media_ops->media_read(g_bot_cbw.lun,
                                                       &buf,
                                                       lba_addr,
                                                       g_current_command_csw.data_residue);

    if ((actual_read_len > g_current_command_csw.data_residue) ||
         (actual_read_len == 0u) ||
         (buf == 0u))
    {
        command_status = SCSI_COMMAND_FAILED;
    }
    else
    {
        g_tx_buf_ptr = buf;
        g_tx_buf_len = actual_read_len;

        /*
        Store the current LBA address so that it will be used to calculate new LBA
        address when remaining data is read from flash.
        */
        g_tx_current_lba_addr = lba_addr;
        command_status = SCSI_COMMAND_PASSED;
    }

    /*Data Direction must be D2H (IN)*/
    if(!(g_bot_cbw.flags & 0x80u))
        return(SCSI_COMMAND_PHASE_ERR);

    return (command_status);
}

/***************************************************************************//**
 USBD_MSC_process_write_10() function processes Write command and calls application
 call-back function to acquire a buffer where the data will be receive.
 */
mss_usbd_msc_csw_status_t
USBD_MSC_process_write_10
(
    void
)
{
    uint32_t lba;
    uint32_t lba_addr;
    uint8_t* write_buf;
    uint32_t app_write_len;
    mss_usbd_msc_csw_status_t command_status = SCSI_COMMAND_FAILED;

    /*Lba address is in command block byte 2 to 4 with MSB first.*/
    lba = (g_bot_cbw.cmd_block[2u] << 24u) |
          (g_bot_cbw.cmd_block[3u] << 16u) |
          (g_bot_cbw.cmd_block[4u] << 8u) |
           g_bot_cbw.cmd_block[5u];

    /*Block size was received in USB_MSC_SCSI_READ_CAPACITY_10 command*/
    lba_addr = (lba * lun_capacity[g_bot_cbw.lun].blk_sz_len);

    /*
    Number of LBA to be written is provided in command block byte 7 to 8,
    calculate the total size to be writen
    */
    g_current_command_csw.data_residue = ((g_bot_cbw.cmd_block[7] << 8u) | g_bot_cbw.cmd_block[8u]) *
                                           lun_capacity[g_bot_cbw.lun].blk_sz_len;

    write_buf = g_usbd_msc_media_ops->media_acquire_write_buf(g_bot_cbw.lun,
                                                              lba_addr,
                                                              &app_write_len);
    if((write_buf != 0) && (app_write_len != 0u))
    {
        /*
        Since App is not capable of handling all data at the moment.
        Store the current LBA address so that it will be used to calculate new LBA
        address when remaining data is received.
        */
        g_rx_current_lba_addr = lba_addr;

        /*
        If application buffer is not big enough to handle all requested data from Host,
        then read the amount of data that the application can handle at the moment
        */
        if(app_write_len <= g_current_command_csw.data_residue)
        {
            g_rx_buf_len = app_write_len;
        }
        else
        {
            g_rx_buf_len = g_current_command_csw.data_residue;
        }

        g_rx_buf_ptr = write_buf;
        command_status = SCSI_COMMAND_PASSED;
    }
    else
    {
        command_status = SCSI_COMMAND_FAILED;
    }

    /*Data Direction must be H2D (OUT)*/
    if(g_bot_cbw.flags & 0x80u)
        return(SCSI_COMMAND_PHASE_ERR);

    return(command_status);
}

#ifdef __cplusplus
}
#endif
