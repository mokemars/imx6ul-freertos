/***************************************************************************//**
 * (c) Copyright 2012-2013 Microsemi SoC Products Group.  All rights reserved.
 *
 * Smartfusion2 MSS USB Driver Stack
 *      USB Logical Layer (USB-LL)
 *          USBD driver
 *
 * USBD driver implementation:
 * This source file implements the common functinality of USB device mode,
 * which includes initialization of MSS USB in device mode, USB standard request
 * handling, distribution of requests to specific class, interface or endpoints.
 *
 * SVN $Revision: 5468 $
 * SVN $Date: 2013-03-29 15:38:01 +0530 (Fri, 29 Mar 2013) $
 */

#include "mss_usb_device.h"
#include "mss_usb_config.h"
#include "mss_usb_common_cif.h"
#include "mss_usb_device_cif.h"
#include "mss_usb_std_def.h"
#include "../../CMSIS/mss_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SETUP_PKT_INIT();                         g_setup_pkt.request_type = 0;\
                                                  g_setup_pkt.request = 0;\
                                                  g_setup_pkt.value = 0;\
                                                  g_setup_pkt.index = 0;\
                                                  g_setup_pkt.length = 0;

/***************************************************************************//**
 Global variables used within this file.
 */
/* This structure must be implemented by user application */
mss_usbd_user_descr_cb_t *g_usbd_user_descr_cb;

/* This structure must be implemented by USBD-Class driver*/
mss_usbd_class_cb_t *g_usbd_class_cb;

/* USB current Speed of operation selected by user*/
static mss_usb_device_speed_t g_usbd_user_speed;

/*Status information for SET_FEATURE,GET_FEATURE requests.*/
static uint16_t g_usbd_status;

/*Device configuration information*/
static mss_usbd_dev_conf_t g_usbd_dev_conf;

/*This structure is used to hold the setup packet received on control endpoint*/
static mss_usbd_setup_pkt_t g_setup_pkt;

/*Structure representing each endpoint on MSS USB*/
static mss_usb_ep_t device_tx_ep[5u];       /*idx = 0: Control Endpoint*/
static mss_usb_ep_t device_rx_ep[5u];

/*
 This structure should be implemented for bare testing of the USB transfers,
 when in the test mode, no other class specific components or enumeration
 related APIs are called.
 This mode is only provided for internal customers for testing purpose.
 */
#ifdef MSS_USB_DEVICE_TEST_MODE
extern mss_usbd_user_test_cb_t usbd_test_cb;
#endif

/***************************************************************************//**
 Private function declarations for this file (USBD Driver).
*/
static void MSS_USBD_cep_setup_cb(uint8_t status);
static void MSS_USBD_cep_dataout_cb(uint8_t status);
static void MSS_USBD_cep_datain_cb(uint8_t status);
static void MSS_USBD_reset_cb(void);
static void MSS_USBD_sof_cb(uint8_t status);
static void MSS_USBD_suspend_cb(void);
static void MSS_USBD_resume_cb(void);
static void MSS_USBD_ep_dataout_cb(mss_usb_ep_num_t num, uint8_t status);
static void MSS_USBD_ep_datain_cb(mss_usb_ep_num_t num, uint8_t status);

static uint8_t MSS_USBD_class_requests(uint8_t** buf_pp,
                                       uint32_t* len_p);

static uint8_t MSS_USBD_vendor_requests(uint8_t** buf_pp,
                                        uint32_t* len_p);

static uint8_t MSS_USBD_get_descriptor(uint8_t** buf_pp, uint32_t* len_p);
static uint8_t MSS_USBD_set_address(void);
static uint8_t MSS_USBD_set_config(void);
static uint8_t MSS_USBD_get_config(uint8_t** buf_pp, uint32_t* len_p);
static uint8_t MSS_USBD_get_status(uint8_t** buf_pp, uint32_t* len_p);
static uint8_t MSS_USBD_set_feature(void);
static uint8_t MSS_USBD_clr_feature(void);
static uint8_t MSS_USBD_set_descriptor(void);
static uint8_t MSS_USBD_std_requests(uint8_t** buf,uint32_t* length);

void MSS_USBD_get_hwcore_info(mss_usb_core_info_t* hw_info);
void MSS_USBD_dev_force_resume(void);
void MSS_USBD_enable_usbirq(uint8_t irq_mask);
void MSS_USBD_disable_usbirq(uint8_t irq_mask);
void MSS_USBD_set_dev_address(uint8_t dev_addr);

/*
 This structure implements the callback functions which will by the USBD-CIFL
 layer.
 */
mss_usbd_cb_t g_mss_usbd_cb =
{
    MSS_USBD_ep_dataout_cb,
    MSS_USBD_ep_datain_cb,

    MSS_USBD_cep_setup_cb,
    MSS_USBD_cep_dataout_cb,
    MSS_USBD_cep_datain_cb,

    MSS_USBD_sof_cb,
    MSS_USBD_reset_cb,
    MSS_USBD_suspend_cb,
    MSS_USBD_resume_cb
};

/***************************************************************************//**
 Exported functions from this file (USBD Driver)
 */

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
 */
void
MSS_USBD_init
(
    mss_usb_device_speed_t speed
)
{
    g_usbd_dev_conf.device_addr = 0x00u;
    g_usbd_dev_conf.device_total_interfaces = 0x00u;
    g_usbd_dev_conf.device_total_ep = 0x00u;
    g_usbd_dev_conf.device_state = MSS_USB_NOT_ATTACHED_STATE;

    /*store this for usage with DevQual, OtherSpeed requests*/
    g_usbd_user_speed = speed;
    g_usbd_dev_conf.device_speed = speed;

    device_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_IDLE;

    MSS_USBD_CIF_init(g_usbd_dev_conf.device_speed);

    MSS_USBD_CIF_dev_connect();

    g_usbd_dev_conf.device_state = MSS_USB_POWERED_STATE;
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
 */
void
MSS_USBD_set_descr_cb_handler
(
    mss_usbd_user_descr_cb_t* user_desc_cb
)
{
    g_usbd_user_descr_cb = user_desc_cb;
}
/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
 */
void
MSS_USBD_set_class_cb_handler
(
    mss_usbd_class_cb_t* class_cb
)
{
    g_usbd_class_cb = class_cb;
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
 */
void
MSS_USBD_tx_ep_stall
(
    mss_usb_ep_num_t ep_num
)
{
    device_tx_ep[ep_num].stall = 1u;
    MSS_USBD_CIF_tx_ep_stall(ep_num);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
 */
void
MSS_USBD_tx_ep_clr_stall
(
    mss_usb_ep_num_t ep_num
)
{
    MSS_USBD_CIF_tx_ep_clr_stall(ep_num);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
 */
void
MSS_USBD_tx_ep_flush_fifo
(
    mss_usb_ep_num_t ep_num
)
{
    MSS_USB_CIF_tx_ep_flush_fifo(ep_num);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
 */
void
MSS_USBD_rx_ep_stall
(
    mss_usb_ep_num_t ep_num
)
{
    device_rx_ep[ep_num].stall = 1u;
    MSS_USBD_CIF_rx_ep_stall(ep_num);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
 */
void
MSS_USBD_rx_ep_clr_stall
(
    mss_usb_ep_num_t ep_num
)
{
    MSS_USBD_CIF_rx_ep_clr_stall(ep_num);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
 */
void
MSS_USBD_rx_ep_flush_fifo
(
    mss_usb_ep_num_t ep_num
)
{
    MSS_USB_CIF_rx_ep_flush_fifo(ep_num);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
 */
void
MSS_USBD_cep_flush_fifo
(
    void
)
{
    MSS_USB_CIF_cep_flush_fifo();
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
 */
void
MSS_USBD_cep_configure
(
    uint8_t max_pkt_size
)
{
    device_tx_ep[MSS_USB_CEP].num = MSS_USB_CEP;
    device_tx_ep[MSS_USB_CEP].stall = 0u;
    device_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_IDLE;
    device_tx_ep[MSS_USB_CEP].xfr_type = MSS_USB_XFR_CONTROL;

    /*FIFO arress */
    device_tx_ep[MSS_USB_CEP].buf_addr = 0u;

    device_tx_ep[MSS_USB_CEP].max_pkt_size = max_pkt_size;
    device_tx_ep[MSS_USB_CEP].txn_length = SETUP_PKT_SIZE;

    MSS_USBD_CIF_cep_configure();
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
 */
void
MSS_USBD_cep_read_prepare
(
    uint8_t * addr,
    uint32_t length
)
{
    device_tx_ep[MSS_USB_CEP].buf_addr = addr;
    device_tx_ep[MSS_USB_CEP].xfr_length = length;
    device_tx_ep[MSS_USB_CEP].xfr_count = 0u;
    device_tx_ep[MSS_USB_CEP].txn_count = 0u;

    if(device_tx_ep[MSS_USB_CEP].xfr_length >
       device_tx_ep[MSS_USB_CEP].max_pkt_size)
    {
        device_tx_ep[MSS_USB_CEP].txn_length =
            device_tx_ep[MSS_USB_CEP].max_pkt_size;
    }
    else
    {
        device_tx_ep[MSS_USB_CEP].txn_length = length;
    }
    MSS_USBD_CIF_cep_rx_prepare(&device_tx_ep[MSS_USB_CEP]);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
 */
void
MSS_USBD_cep_write
(
    uint8_t * addr,
    uint32_t length
)
{
    device_tx_ep[MSS_USB_CEP].buf_addr = addr;
    device_tx_ep[MSS_USB_CEP].xfr_length = length;

    device_tx_ep[MSS_USB_CEP].xfr_count = 0u;
    device_tx_ep[MSS_USB_CEP].txn_count = 0u;

    if(device_tx_ep[MSS_USB_CEP].xfr_length >
       device_tx_ep[MSS_USB_CEP].max_pkt_size)
    {
        device_tx_ep[MSS_USB_CEP].txn_length =
            device_tx_ep[MSS_USB_CEP].max_pkt_size;
    }
    else
    {
        device_tx_ep[MSS_USB_CEP].txn_length = length;
    }
    MSS_USBD_CIF_cep_write_pkt(&device_tx_ep[MSS_USB_CEP]);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
 */
void
MSS_USBD_tx_ep_configure
(
    mss_usb_ep_num_t ep_num,
    uint16_t fifo_addr,
    uint16_t fifo_size,       /*This is actual size of FIFO independent of DPB*/
    uint16_t max_pkt_size,
    uint8_t num_usb_pkt,
    uint8_t dma_enable,
    mss_usb_dma_channel_t dma_channel,
    mss_usb_xfr_type_t xfr_type,
    uint32_t add_zlp
)
{
    uint8_t err_check = USB_SUCCESS;
    uint16_t std_max_pkt_sz = USB_HS_BULK_MAX_PKT_SIZE;

    if(MSS_USB_DEVICE_HS == g_usbd_dev_conf.device_speed)
    {
        switch(xfr_type)
        {
            case MSS_USB_XFR_BULK:
                    std_max_pkt_sz = USB_HS_BULK_MAX_PKT_SIZE;
                break;
                case MSS_USB_XFR_INTERRUPT:
                    std_max_pkt_sz = USB_HS_INTERRUPT_MAX_PKT_SIZE;
                break;
                case MSS_USB_XFR_ISO:
                std_max_pkt_sz = USB_HS_ISO_MAX_PKT_SIZE;
                break;
            default:
                err_check = USB_FAIL;
        }
    }
    else if(MSS_USB_DEVICE_FS == g_usbd_dev_conf.device_speed)
    {
        switch(xfr_type)
        {
            case MSS_USB_XFR_BULK:
                std_max_pkt_sz = USB_FS_BULK_MAX_PKT_SIZE;
                break;
            case MSS_USB_XFR_INTERRUPT:
                std_max_pkt_sz = USB_FS_INTERRUPT_MAX_PKT_SIZE;
                break;
            case MSS_USB_XFR_ISO:
                std_max_pkt_sz = USB_FS_ISO_MAX_PKT_SIZE;
                break;
            default:
                err_check = USB_FAIL;
        }
    }

    if(max_pkt_size > std_max_pkt_sz)
    {
        err_check = USB_FAIL;
    }
    if(fifo_size < max_pkt_size)
    {
        err_check = USB_FAIL;
    }

    ASSERT(err_check == USB_SUCCESS);

    if(err_check == USB_SUCCESS)
    {
        if((max_pkt_size * 2) <= fifo_size)
        {
            device_tx_ep[ep_num].dpb_enable = DPB_ENABLE;
        }
        else
        {
            device_tx_ep[ep_num].dpb_enable = DPB_DISABLE;
        }

        device_tx_ep[ep_num].num = ep_num;
        device_tx_ep[ep_num].fifo_addr = fifo_addr;
        device_tx_ep[ep_num].fifo_size = fifo_size;
        device_tx_ep[ep_num].max_pkt_size = max_pkt_size;
        device_tx_ep[ep_num].num_usb_pkt = num_usb_pkt;
        device_tx_ep[ep_num].dma_enable = dma_enable;
        device_tx_ep[ep_num].dma_channel = dma_channel;
        device_tx_ep[ep_num].stall = 0;
        device_tx_ep[ep_num].state = MSS_USB_EP_VALID;
        device_tx_ep[ep_num].xfr_type = xfr_type;
        device_tx_ep[ep_num].buf_addr = 0u;
        device_tx_ep[ep_num].add_zlp = add_zlp;
        MSS_USBD_CIF_tx_ep_configure(&device_tx_ep[ep_num]);
    }
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
void
MSS_USBD_rx_ep_configure
(
    mss_usb_ep_num_t ep_num,
    uint16_t fifo_addr,
    uint16_t fifo_size,      /*This is actual size of FIFO independent of DPB*/
    uint16_t max_pkt_size,
    uint8_t num_usb_pkt,
    uint8_t dma_enable,
    mss_usb_dma_channel_t dma_channel,
    mss_usb_xfr_type_t xfr_type,
    uint32_t add_zlp
)
{
    uint8_t err_check = USB_SUCCESS;
    uint16_t std_max_pkt_sz = USB_HS_BULK_MAX_PKT_SIZE;

    if(MSS_USB_DEVICE_HS == g_usbd_dev_conf.device_speed)
    {
        switch(xfr_type)
        {
            case MSS_USB_XFR_BULK:
                std_max_pkt_sz = USB_HS_BULK_MAX_PKT_SIZE;
                break;
            case MSS_USB_XFR_INTERRUPT:
                std_max_pkt_sz = USB_HS_INTERRUPT_MAX_PKT_SIZE;
                break;
            case MSS_USB_XFR_ISO:
                std_max_pkt_sz = USB_HS_ISO_MAX_PKT_SIZE;
                break;
            default:
                err_check = USB_FAIL;
        }
    }
    else if(MSS_USB_DEVICE_FS == g_usbd_dev_conf.device_speed)
    {
        switch(xfr_type)
        {
            case MSS_USB_XFR_BULK:
                std_max_pkt_sz = USB_FS_BULK_MAX_PKT_SIZE;
                break;
            case MSS_USB_XFR_INTERRUPT:
                std_max_pkt_sz = USB_FS_INTERRUPT_MAX_PKT_SIZE;
                break;
            case MSS_USB_XFR_ISO:
                std_max_pkt_sz = USB_FS_ISO_MAX_PKT_SIZE;
                break;
            default:
                err_check = USB_FAIL;
        }
    }

    if(max_pkt_size > std_max_pkt_sz)
    {
        err_check = USB_FAIL;
    }
    if(fifo_size < max_pkt_size)
    {
        err_check = USB_FAIL;
    }

    ASSERT(err_check == USB_SUCCESS);

    if(err_check == USB_SUCCESS)
    {
        if((max_pkt_size * 2 ) <= fifo_size)
        {
            device_rx_ep[ep_num].dpb_enable = DPB_ENABLE;
        }
        else
        {
            device_rx_ep[ep_num].dpb_enable = DPB_DISABLE;
        }
        device_rx_ep[ep_num].num = ep_num;
        device_rx_ep[ep_num].fifo_addr = fifo_addr;
        device_rx_ep[ep_num].fifo_size = fifo_size;
        device_rx_ep[ep_num].max_pkt_size = max_pkt_size;
        device_rx_ep[ep_num].num_usb_pkt = num_usb_pkt;
        device_rx_ep[ep_num].dma_enable = dma_enable;
        device_rx_ep[ep_num].dma_channel = dma_channel;
        device_rx_ep[ep_num].stall = 0;
        device_rx_ep[ep_num].state = MSS_USB_EP_VALID;
        device_rx_ep[ep_num].xfr_type = xfr_type;
        device_rx_ep[ep_num].buf_addr = 0u;
        device_rx_ep[ep_num].add_zlp = add_zlp;

        MSS_USBD_CIF_rx_ep_configure(&device_rx_ep[ep_num]);
    }
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
void
MSS_USBD_rx_ep_read_prepare
(
    mss_usb_ep_num_t ep_num,
    uint8_t * addr,
    uint32_t length
)
{
    device_rx_ep[ep_num].buf_addr = addr;
    device_rx_ep[ep_num].xfr_length = length;
    device_rx_ep[ep_num].xfr_count = 0u;
    device_rx_ep[ep_num].txn_count = 0u;

    /*ASSERT when length is 0, address is null or ep number is wrong*/
    if(length > 0)
    {
        /*
        TODO:    While using DMA, RxMaxP payload value MUST be an even number
        when dealing with Multi packet Bulk transaction for
        proper interrupt generation. This needs to be taken care.
        */
        /*
         section 8.4.2.3 -- No support for HB in Interrupt transfers.
         i.e. maximum one packet(num_usb_pkt=1)can be transffered in one frame.
        */
        if(length >= device_rx_ep[ep_num].max_pkt_size)
        {
            device_rx_ep[ep_num].txn_length = device_rx_ep[ep_num].max_pkt_size;
        }
        else
        {
            device_rx_ep[ep_num].txn_length = length;
        }
    }
    MSS_USBD_CIF_rx_ep_read_prepare(&device_rx_ep[ep_num]);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
void
MSS_USBD_tx_ep_write
(
    mss_usb_ep_num_t ep_num,
    uint8_t * addr,
    uint32_t length
)
{
    uint8_t dpb = 1u;

    /* assert if uneven value for BULK transfers */
    ASSERT(ep_num);
    ASSERT(addr != 0);

    if((ep_num) && (addr != 0))
    {
        if(device_tx_ep[ep_num].dpb_enable == DPB_ENABLE)
        {
            dpb = 2u;
        }
        device_tx_ep[ep_num].num = ep_num;
        device_tx_ep[ep_num].buf_addr = addr;
        device_tx_ep[ep_num].xfr_length = length;
        device_tx_ep[ep_num].xfr_count = 0u;
        device_tx_ep[ep_num].txn_count = 0u;

        if(device_tx_ep[ep_num].xfr_type == MSS_USB_XFR_BULK)
        {
            /*
            TODO: TxMaxP payload value MUST be an even number when dealing with
            Multi packet Bulk transaction for proper interrupt generation.
            This needs to be taken care.
            */
            if(length <= device_tx_ep[ep_num].fifo_size)
            {
                device_tx_ep[ep_num].txn_length = length;
            }
            else
            {
                device_tx_ep[ep_num].txn_length= device_tx_ep[ep_num].fifo_size;
            }
        }

        /*
         section 8.4.2.3 -- No support for HB in Interrupt transfers.
         i.e. maximum one packet(num_usb_pkt=1)can be transffered in one frame.
         */
        else if(device_tx_ep[ep_num].xfr_type == MSS_USB_XFR_INTERRUPT)
        {
            if(length >= (device_tx_ep[ep_num].max_pkt_size * dpb))
            {
                device_tx_ep[ep_num].txn_length =
                    (device_tx_ep[ep_num].max_pkt_size * dpb);
            }
            else
            {
                device_tx_ep[ep_num].txn_length = length;
            }
        }
        else if(device_tx_ep[ep_num].xfr_type == MSS_USB_XFR_ISO)
        {
            if(length >= (device_tx_ep[ep_num].max_pkt_size * dpb))
            {
                device_tx_ep[ep_num].txn_length =
                    (device_tx_ep[ep_num].max_pkt_size * dpb);
            }
            else
            {
                device_tx_ep[ep_num].txn_length = length;
            }
        }
        MSS_USB_CIF_ep_write_pkt(&device_tx_ep[ep_num]);
    }
    else
    {
         ASSERT(0);
    }
}


/***************************************************************************//**
 Private functions to this file (USBD Driver)
 */

/***************************************************************************//**
 This call-back function is executed on receiving SOF interrupt in Device mode.
 */
static void
MSS_USBD_sof_cb
(
    uint8_t status
)
{
#ifndef MSS_USB_DEVICE_TEST_MODE
#else
    usbd_test_cb.test_sof(status);
#endif

}

/******************************************************************************
 This Callback function is executed when reset interrupt is received when
 the MSS USB is in device mode.
 */
static void
MSS_USBD_reset_cb
(
    void
)
{
#ifndef MSS_USB_DEVICE_TEST_MODE
    g_usbd_dev_conf.device_addr = 0x00u;
    g_usbd_dev_conf.device_total_interfaces = 0x00u;
    g_usbd_dev_conf.device_total_ep = 0x00u;
    g_usbd_dev_conf.device_state = MSS_USB_DEFAULT_STATE;

    device_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_IDLE;
    g_usbd_dev_conf.remote_wakeup = 0x00u;

    if(MSS_USBD_CIF_is_hs_mode())
    {
        g_usbd_dev_conf.device_speed = MSS_USB_DEVICE_HS;
    }
    else
    {
        g_usbd_dev_conf.device_speed = MSS_USB_DEVICE_FS;
    }

    SETUP_PKT_INIT();

    MSS_USBD_cep_configure(64u);
    MSS_USBD_cep_read_prepare((uint8_t*)&g_setup_pkt, 8u);
#else
    usbd_test_cb.test_reset();
#endif

}

/*******************************************************************************
 This Callback function is executed when Setup packet is received on Control EP
 */
static void
MSS_USBD_cep_setup_cb
(
    uint8_t status
)
{
#ifndef MSS_USB_DEVICE_TEST_MODE
    uint8_t* buf = 0;
    uint32_t length = 0u;
    uint8_t result = USB_FAIL;

    if(status)
    {
        /*
         EP0 was previously stalled, clear the error condition and
         prepare for next transaction
         */
        device_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_IDLE;

        SETUP_PKT_INIT();

        MSS_USBD_cep_read_prepare((uint8_t*)&g_setup_pkt, 8u);
    }
    else
    {
        MSS_USBD_CIF_cep_read_pkt(&device_tx_ep[MSS_USB_CEP]);

        if(device_tx_ep[MSS_USB_CEP].txn_length == SETUP_PKT_SIZE)
        {
            device_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_SETUP;

            if(USB_STANDARD_REQUEST ==
              (g_setup_pkt.request_type & USB_STD_REQ_TYPE_MASK))
            {
                result = MSS_USBD_std_requests(&buf, &length);
            }
            else if(USB_CLASS_REQUEST ==
                   (g_setup_pkt.request_type & USB_STD_REQ_TYPE_MASK))
            {
                result = MSS_USBD_class_requests(&buf, &length);
            }
            else if(USB_VENDOR_REQUEST ==
                   (g_setup_pkt.request_type & USB_STD_REQ_TYPE_MASK))
            {
                result = MSS_USBD_vendor_requests(&buf, &length);
            }
        }
        else
        {
            ASSERT(0);
        }
        if(result)  //USB_SUCCESS
        {
            MSS_USBD_CIF_reset_index_reg();
            if(g_setup_pkt.length == 0u)
            {
                volatile uint32_t delay = 0;
                MSS_USBD_CIF_cep_end_zdr();                 //zdl complete
                for(delay = 0; delay < 500 ; delay ++);
                device_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_IDLE;
                if(g_setup_pkt.request == USB_STD_REQ_SET_ADDRESS)
                {
                    //special case SetAddress Request
                    MSS_USBD_CIF_set_dev_addr(g_usbd_dev_conf.device_addr);
                }
                MSS_USBD_cep_read_prepare((uint8_t*)&g_setup_pkt, 8u);
                SETUP_PKT_INIT();
            }
            else
            {
                //end of setup phase for Read/Write Req
                MSS_USBD_CIF_cep_clr_rxpktrdy();
                if((g_setup_pkt.request_type & USB_STD_REQ_DATA_DIR_MASK))
                {
                    device_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_DATAIN;
                    if(buf == (uint8_t*)0)
                        ASSERT(0);
                    if(length > g_setup_pkt.length)
                    {
                        length = g_setup_pkt.length;
                    }
                    MSS_USBD_cep_write(buf,length);
                }
                else
                {
                    device_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_DATAOUT;
                    MSS_USBD_cep_read_prepare(buf,length);
                }
            }
        }
        else
        {
            device_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_IDLE;
            SETUP_PKT_INIT();
            /*both servicedrxpktrdy and Sendstall should be set in setupphase*/
            MSS_USBD_CIF_cep_stall();
        }
    }

#else /*MSS_USB_DEVICE_TEST_MODE*/
    usbd_test_cb.usb_user_device_test_cep_setup(status);
#endif
}

/*******************************************************************************
 This Callback function is called when Datapacket is received on Control EP.
 DATAOUT Phase.
 */
static void
MSS_USBD_cep_dataout_cb
(
    uint8_t status
)
{
#ifndef MSS_USB_DEVICE_TEST_MODE

    uint32_t rem_length;

    /*
     xfr_rem_length should have been setup by the Specific request which needed
     data to arriave in the Data phase
     */
    if(status & 0x03u)
    {
        SETUP_PKT_INIT();
        if(0 != g_usbd_class_cb->usb_class_cep_dataout)
            g_usbd_class_cb->usb_class_cep_dataout(status);

        device_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_IDLE;
        MSS_USBD_cep_read_prepare((uint8_t*)&g_setup_pkt, 8u);
    }
    else
    {
        MSS_USBD_CIF_cep_read_pkt(&device_tx_ep[MSS_USB_CEP]);

        if(MSS_USB_CEP_DATAOUT == device_tx_ep[MSS_USB_CEP].state)
        {
            if(device_tx_ep[MSS_USB_CEP].xfr_count ==
               device_tx_ep[MSS_USB_CEP].xfr_length)
            {
                /*Call USBD-Class indicating that the desired data is arrived*/
                if(0 != g_usbd_class_cb->usb_class_cep_dataout)
                    g_usbd_class_cb->usb_class_cep_dataout(status);

                MSS_USBD_CIF_reset_index_reg();
                SETUP_PKT_INIT();
                device_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_IDLE;
                MSS_USBD_CIF_cep_end_wdr();                 //WriteReq complete
                MSS_USBD_cep_read_prepare((uint8_t*)&g_setup_pkt, 8u);

            }
            else if(device_tx_ep[MSS_USB_CEP].xfr_count <
                    device_tx_ep[MSS_USB_CEP].xfr_length)
            {
                MSS_USBD_CIF_cep_clr_rxpktrdy();       //get more data from host

                /*Continue to read data on the CEP*/
                rem_length = device_tx_ep[MSS_USB_CEP].xfr_length -
                             device_tx_ep[MSS_USB_CEP].xfr_count;

                if(rem_length >= device_tx_ep[MSS_USB_CEP].max_pkt_size)
                {
                    device_tx_ep[MSS_USB_CEP].txn_length =
                        device_tx_ep[MSS_USB_CEP].max_pkt_size;
                }
                else
                {
                    device_tx_ep[MSS_USB_CEP].txn_length = rem_length;
                }
                device_tx_ep[MSS_USB_CEP].buf_addr +=
                    device_tx_ep[MSS_USB_CEP].txn_count;

                device_tx_ep[MSS_USB_CEP].txn_count = 0u;
                MSS_USBD_cep_read_prepare(device_tx_ep[MSS_USB_CEP].buf_addr,
                                          device_tx_ep[MSS_USB_CEP].txn_length);
            }
            else
            {
                SETUP_PKT_INIT();
                device_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_IDLE;
                MSS_USBD_CIF_cep_stall();               //Send stall
            }
        }
        else
        {
            ASSERT(0);
        }
    }
#else /*MSS_USB_DEVICE_TEST_MODE*/
    usbd_test_cb.usb_user_device_test_cep_dataout(status);
#endif
}

/*******************************************************************************
 This Callback function is executed when Data packet is sent from control EP.
 DATAIN phase.
 */
static void
MSS_USBD_cep_datain_cb
(
    uint8_t status
)
{
#ifndef MSS_USB_DEVICE_TEST_MODE
    /*
     xfr_rem_length should have been setup by the Specific request which needed
     data to arriave in the Data phase
     */
    if(status & 0x03u)
    {
        SETUP_PKT_INIT();
        if(0 !=  g_usbd_class_cb->usb_class_cep_datain)
             g_usbd_class_cb->usb_class_cep_datain(status);
        device_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_IDLE;
        MSS_USBD_cep_read_prepare((uint8_t*)&g_setup_pkt, 8u);
    }
    else
    {
        /*Device should be in DATAIN phase.*/
        if(MSS_USB_CEP_DATAIN == device_tx_ep[MSS_USB_CEP].state)
        {
            if(device_tx_ep[MSS_USB_CEP].xfr_count <
               device_tx_ep[MSS_USB_CEP].xfr_length)
            {
                /*Continue to transmit more data*/
                device_tx_ep[MSS_USB_CEP].buf_addr +=
                    device_tx_ep[MSS_USB_CEP].txn_count;

                if((device_tx_ep[MSS_USB_CEP].xfr_length - device_tx_ep[MSS_USB_CEP].xfr_count) >=
                   device_tx_ep[MSS_USB_CEP].max_pkt_size)
                {
                    device_tx_ep[MSS_USB_CEP].txn_length =
                        device_tx_ep[MSS_USB_CEP].max_pkt_size;
                }
                else
                {
                    device_tx_ep[MSS_USB_CEP].txn_length =
                                        (device_tx_ep[MSS_USB_CEP].xfr_length -
                                         device_tx_ep[MSS_USB_CEP].xfr_count);
                }

                /*
                 Reset the txn_count since one transaction out of the transfer
                 is completed now
                 */
                device_tx_ep[MSS_USB_CEP].txn_count = 0u;
                MSS_USBD_CIF_cep_write_pkt(&device_tx_ep[MSS_USB_CEP]);
            }
            else
            {
                /*Call USBD-Class indicating that the desired data is sent*/
                if(0 !=  g_usbd_class_cb->usb_class_cep_datain)
                    g_usbd_class_cb->usb_class_cep_datain(status);

                device_tx_ep[MSS_USB_CEP].txn_count = 0u;
                device_tx_ep[MSS_USB_CEP].xfr_count = 0u;
                device_tx_ep[MSS_USB_CEP].xfr_length = 0u;
                device_tx_ep[MSS_USB_CEP].txn_length = 0u;
                device_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_IDLE;
                SETUP_PKT_INIT();
                MSS_USBD_CIF_reset_index_reg();
                MSS_USBD_cep_read_prepare((uint8_t*)&g_setup_pkt, 8u);
            }
        }
    }

#else /*MSS_USB_DEVICE_TEST_MODE*/
        usbd_test_cb.test_cep_datain(status);
#endif
}
/*******************************************************************************
 This Call-back function is executed when Data packet is received on RX EP
 */
static void
MSS_USBD_ep_dataout_cb
(
    mss_usb_ep_num_t num,
    uint8_t status
)
{
    uint8_t transfer_complete = 0;

    /*
     While receiving data, xfr_length might be known to application or unknown.
     When xfr_length is not known, application is expected to provide
     sizeof(Receive_buffer) as xfr_length which is big enough to receive
     unknown data. End of data reception in this case is flagged by
     reception of short packet or ZLP(when xfr_length = n*maxpktsz).
     When xfr_length is known(via transport protocol), end of data reception
     is concluded when xfr_count = xfr_length. No zlp is expected to be
     received for the case of xfr_length = n*maxpktsz.
     When DMA-m1 is enabled, the control will return here only once after
     completion of transfer.
     */
    if((status & 0x1Fu) > 1)
    {
        transfer_complete = 1;
    }
    else
    {
        if(device_rx_ep[num].dma_enable== DMA_ENABLE)
        {
            transfer_complete = 1;
        }
        else // no dma
        {
            if(device_rx_ep[num].xfr_type == MSS_USB_XFR_BULK)
            {
                if(device_rx_ep[num].add_zlp == ADD_ZLP_TO_XFR)
                {
                    if(device_rx_ep[num].xfr_count < device_rx_ep[num].xfr_length)
                    {
                        if(device_rx_ep[num].txn_count < device_rx_ep[num].max_pkt_size)
                        {
                            transfer_complete = 1;
                        }
                        else
                        {
                           /*receive short pkt.zlp when xfr_length is multiple of wMaxPktsz*/
                            transfer_complete = 0;
                        }
                    }
                    else if(device_rx_ep[num].xfr_count == device_rx_ep[num].xfr_length)
                    {
                        /*buffer is full*/
                        transfer_complete = 1;
                    }
                    else
                        ASSERT(0);
                }
                else // no zlp
                {
                    if(device_rx_ep[num].xfr_count == device_rx_ep[num].xfr_length)
                    {
                        transfer_complete = 1;
                    }
                    else if(device_rx_ep[num].xfr_count < device_rx_ep[num].xfr_length)
                    {
                        transfer_complete = 0;
                    }
                    else
                        ASSERT(0);
                }
            }
            else
            {
                /*
                 For ISO and interrupt transfers, LB-expects only one packet in
                 one transaction HB expects at most 3 packets in one transaction
                 */
                if(device_rx_ep[num].txn_count <= device_rx_ep[num].max_pkt_size)
                {
                    transfer_complete = 1;
                }
                else
                {
                    ASSERT(0);/*TODO: replace this with stall EP*/
                }
            }
        }
    }
    if(transfer_complete == 1)
    {
        #ifndef MSS_USB_DEVICE_TEST_MODE
            g_usbd_class_cb->usb_class_dataout(num,
                                              status,
                                              device_rx_ep[num].xfr_count);
        #else
            usbd_test_cb.test_ep_dataout(num,
                                         status,
                                         device_rx_ep[num].xfr_count);
        #endif
    }
    else
    {
        device_rx_ep[num].buf_addr += device_rx_ep[num].txn_count;
        ASSERT(device_rx_ep[num].xfr_length >= device_rx_ep[num].xfr_count);
        if((device_rx_ep[num].xfr_length - device_rx_ep[num].xfr_count) >=
            device_rx_ep[num].max_pkt_size)
        {
            device_rx_ep[num].txn_length = device_rx_ep[num].max_pkt_size;
        }
        else
        {
            device_rx_ep[num].txn_length = (device_rx_ep[num].xfr_length -
                                            device_rx_ep[num].xfr_count);
        }

        /*
         Reset the txn_count since one transaction out of the
         transfer is completed.
         */
        device_rx_ep[num].txn_count = 0u;
        MSS_USBD_CIF_rx_ep_read_prepare(&device_rx_ep[num]);
    }
}

/******************************************************************************
 This Callback function is executed on transmission complete interrupt even when
 the MSS USB is in device mode.
*/
static void
MSS_USBD_ep_datain_cb
(
    mss_usb_ep_num_t num,
    uint8_t status
)
{
    uint8_t transfer_complete = 0;
    if(status > 1)
    {
        transfer_complete = 1u;
    }
    else
    {
        /*
         While transmitting data we always know the xfr_length, use it to
         check if the transfer has ended.

         DMA Enabled:
         Mode-m1 is used for Bulk transfer.In this case all the data
         single or multiple packets is handled by DMA since Autoset bit is set.
         Control will return here only once when complete data is transmitted.
         Mode-m0 is used for ISO/Interrupt transfers. In this case single packet
         should have been given by the application and control will return here
         only once after the single packet is transmitted.

         DMA Disabled:
         For multi-packet bulk transfers, control will reach here after every
         single packet is transfered. Provide next packet for tramission till
         the end of data.
         */
        if(DMA_ENABLE == device_tx_ep[num].dma_enable)
        {
            if(device_tx_ep[num].xfr_count != device_tx_ep[num].xfr_length)
            {
                ASSERT(0);
            }
            /*TODO: Move the decision to transmit ZLP from CIFL to here*/
            transfer_complete = 1u;
        }
        else    /*DMA_DISABLE*/
        {
            if(device_tx_ep[num].xfr_type == MSS_USB_XFR_BULK)
            {
                if(device_tx_ep[num].xfr_count < device_tx_ep[num].xfr_length)
                {
                    transfer_complete = 0u;
                }
                else if(device_tx_ep[num].xfr_count == device_tx_ep[num].xfr_length)
                {
                    if(device_tx_ep[num].add_zlp == ADD_ZLP_TO_XFR)
                    {
                        if(device_tx_ep[num].txn_count == 0)
                        {
                            transfer_complete = 1u;
                        }
                        else
                        {
                            if(device_tx_ep[num].txn_count == device_tx_ep[num].max_pkt_size)
                            {
                                transfer_complete = 0u;
                            }
                            else if(device_tx_ep[num].txn_count < device_tx_ep[num].max_pkt_size)
                            {
                                transfer_complete = 1u;
                            }
                        }
                    }
                    else        //no zlp
                    {
                        transfer_complete = 1u;
                    }
                }
                else
                {
                    ASSERT(0);
                }
            }
            else    /*ISO/INTERRUPT XRF*/
            {
                if(device_tx_ep[num].txn_count != device_tx_ep[num].txn_length)
                {
                    ASSERT(0);
                }
                transfer_complete = 1u;
            }
        }
    }
    if(transfer_complete == 1)
    {
        #ifndef MSS_USB_DEVICE_TEST_MODE
            g_usbd_class_cb->usb_class_datain(num, status);
        #else   /*MSS_USB_DEVICE_TEST_MODE*/
            usbd_test_cb.test_ep_datain(num, status);
        #endif
    }
    else
    {
        device_tx_ep[num].buf_addr += device_tx_ep[num].txn_count;

        /*
         Reset the txn_count since one transaction out of the transfer
         is completed now
         */
        device_tx_ep[num].txn_count = 0u;
        ASSERT(device_tx_ep[num].xfr_length >= device_tx_ep[num].xfr_count);
        if((device_tx_ep[num].xfr_length - device_tx_ep[num].xfr_count) >=
            device_tx_ep[num].max_pkt_size)
        {
            device_tx_ep[num].txn_length = device_tx_ep[num].max_pkt_size;
        }
        else
        {
            device_tx_ep[num].txn_length = (device_tx_ep[num].xfr_length -
                                         device_tx_ep[num].xfr_count);
        }
        MSS_USB_CIF_ep_write_pkt(&device_tx_ep[num]);
    }
}

/*******************************************************************************
 This Callback function is executed when suspend interrupt is received when
 the MSS USB is in device mode.
 */
static void
MSS_USBD_suspend_cb
(
    void
)
{
    #ifndef MSS_USB_DEVICE_TEST_MODE
        MSS_USB_CIF_enable_usbirq(RESUME_IRQ_MASK);
        g_usbd_dev_conf.device_state_at_suspend = g_usbd_dev_conf.device_state;
        g_usbd_dev_conf.device_state = MSS_USB_SUSPENDED_STATE;
    #else
    /*usbd_test_cb.test_suspend();*/
    #endif
}

/*******************************************************************************
 This Callback function is executed when resume interrupt is received when the
 MSS USB is in device mode.
*/
static void
MSS_USBD_resume_cb
(
    void
)
{
    #ifndef MSS_USB_DEVICE_TEST_MODE
        g_usbd_dev_conf.device_state = g_usbd_dev_conf.device_state_at_suspend;
    #else
        usbd_test_cb.test_resume();
    #endif
}

/*******************************************************************************
 This function processes standard USB requests.
*/
static uint8_t
MSS_USBD_std_requests
(
    uint8_t** buf,
    uint32_t* length
)
{
    uint8_t result = USB_FAIL;
    /*TODO:result should be used for all the functions*/
    switch(g_setup_pkt.request)
    {
        case USB_STD_REQ_GET_DESCRIPTOR:
            result = MSS_USBD_get_descriptor(buf, length);
        return result;

        case USB_STD_REQ_SET_ADDRESS:
            MSS_USBD_set_address();
        break;

        case USB_STD_REQ_SET_CONFIG:
            MSS_USBD_set_config();
        break;

        case USB_STD_REQ_GET_CONFIG:
            MSS_USBD_get_config(buf, length);
        break;

        case USB_STD_REQ_GET_STATUS:
            MSS_USBD_get_status(buf, length);
        break;

        case USB_STD_REQ_SET_FEATURE:
            result = MSS_USBD_set_feature();
        return result;

        case USB_STD_REQ_CLEAR_FEATURE:
            result = MSS_USBD_clr_feature();
        return result;

        case USB_STD_REQ_SET_DESCRIPTOR:
            MSS_USBD_set_descriptor();
        break;

        case USB_STD_REQ_SET_INTERFACE:
            g_usbd_dev_conf.active_interface_num = (uint8_t)g_setup_pkt.value;
        return USB_SUCCESS;

         case USB_STD_REQ_GET_INTERFACE:
            *buf = &g_usbd_dev_conf.active_interface_num;
            *length = 1u;
        return USB_SUCCESS;

        default:
            return USB_FAIL;
    }

    return USB_SUCCESS;
}

/*******************************************************************************
 This function processes USB class requests.
*/
static uint8_t
MSS_USBD_class_requests
(
    uint8_t** buf_pp,
    uint32_t* len_p
)
{
    if((g_usbd_class_cb->usb_class_process_request !=0))
    {
        if(USB_SUCCESS ==
            g_usbd_class_cb->usb_class_process_request(&g_setup_pkt,
                                                       buf_pp,
                                                       len_p))
        {
            return USB_SUCCESS;
        }
        else
        {
            return USB_FAIL;
        }
    }
    else
    {
        return USB_FAIL;
    }
}

/*******************************************************************************
 This function processes vendor defined USB requests.
*/
static uint8_t
MSS_USBD_vendor_requests
(
    uint8_t** buf_pp,
    uint32_t* len_p
)
{
    return USB_FAIL;
}

static uint8_t
MSS_USBD_get_descriptor
(
    uint8_t** buf_pp,
    uint32_t* len_p
)
{
    if((g_usbd_dev_conf.device_state >= MSS_USB_DEFAULT_STATE) &&
        (USB_STD_REQ_DATA_DIR_IN == (g_setup_pkt.request_type & USB_STD_REQ_DATA_DIR_MASK)))
    {
        if(USB_STD_REQ_RECIPIENT_DEVICE ==
           (g_setup_pkt.request_type & USB_STD_REQ_RECIPIENT_MASK))
        {
            switch(g_setup_pkt.value >> 8)
            {
                case USB_DEVICE_DESCRIPTOR_TYPE:

                    /*descriptor Index and Index field should be zero*/
                    if((((uint8_t)(g_setup_pkt.value)) == 0) &&
                        (g_setup_pkt.index == 0) &&
                        (g_usbd_user_descr_cb->usb_device_descriptor !=0))
                    {
                        *buf_pp = g_usbd_user_descr_cb->usb_device_descriptor(len_p);
                        return(USB_SUCCESS);
                    }
                    else
                    {
                        return(USB_FAIL);
                    }

                case USB_STRING_DESCRIPTOR_TYPE:

                    /*
                    * When descriptor index is 0, index field must be 0.
                    * When descriptor index is >0, index field indicates Lang ID
                    */
                    if(g_usbd_user_descr_cb->usb_string_descriptor != 0)
                    {
                        *buf_pp = g_usbd_user_descr_cb->usb_string_descriptor((uint8_t)g_setup_pkt.value, len_p);
                        return(USB_SUCCESS);
                    }
                    else
                    {
                        return(USB_FAIL);
                    }

                case USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE:
                    if(g_usbd_user_speed == MSS_USB_DEVICE_HS)
                    {
                        /*descriptor Index should be zero*/
                        if((((uint8_t)(g_setup_pkt.value)) == 0) &&
                            (g_setup_pkt.index == 0) &&
                            (g_usbd_user_descr_cb->usb_device_qual_descriptor)!=0)
                        {
                            *buf_pp = g_usbd_user_descr_cb->usb_device_qual_descriptor
                                            (g_usbd_dev_conf.device_speed, len_p);
                            return(USB_SUCCESS);
                        }
                        else
                        {
                            return(USB_FAIL);
                        }
                    }
                    else
                    {
                        /*Since User operates USBD at FS, Stall this request*/
                        return(USB_FAIL);
                    }

                case USB_CONFIGURATION_DESCRIPTOR_TYPE:
                    if(g_usbd_class_cb->usb_class_get_descriptor != 0)
                    {
                        *buf_pp = g_usbd_class_cb->usb_class_get_descriptor(USB_STD_REQ_RECIPIENT_DEVICE,
                                                                           USB_CONFIGURATION_DESCRIPTOR_TYPE,
                                                                           len_p,
                                                                           g_usbd_dev_conf.device_speed);
                        return(USB_SUCCESS);
                    }
                    else
                    {
                        return(USB_FAIL);
                    }

                case USB_OTHER_SPEED_CONFIG_DESCRIPTOR_TYPE:
                    if(g_usbd_user_speed == MSS_USB_DEVICE_HS)
                    {
                        /*descriptor Index should be zero and Index field should be zero*/
                        if((((uint8_t)(g_setup_pkt.value)) == 0u) &&
                           (g_setup_pkt.index == 0u))
                        {
                            if(g_usbd_class_cb->usb_class_get_descriptor != 0)
                            {
                                *buf_pp = g_usbd_class_cb->usb_class_get_descriptor(USB_STD_REQ_RECIPIENT_DEVICE,
                                                                                   USB_OTHER_SPEED_CONFIG_DESCRIPTOR_TYPE,
                                                                                   len_p,
                                                                                   g_usbd_dev_conf.device_speed);
                                return(USB_SUCCESS);
                            }
                            else
                            {
                                return(USB_FAIL);
                            }
                        }
                    }
                    else
                    {
                        /*Since User operates USBD at FS, Stall this request*/
                        return(USB_FAIL);
                    }
                break;
                default:
                    return(USB_FAIL);
            }

        }
        else if(USB_STD_REQ_RECIPIENT_INTERFACE ==
                (g_setup_pkt.request_type & USB_STD_REQ_RECIPIENT_MASK))
        {
            if(g_usbd_class_cb->usb_class_get_descriptor != 0)
            {
                *buf_pp = g_usbd_class_cb->usb_class_get_descriptor(USB_STD_REQ_RECIPIENT_INTERFACE,
                                                                   g_setup_pkt.value >> 8,
                                                                   len_p,
                                                                   g_usbd_dev_conf.device_speed);
                return(USB_SUCCESS);
            }
            else
            {
                return(USB_FAIL);
            }
        }
        else if(USB_STD_REQ_RECIPIENT_ENDPOINT ==
                (g_setup_pkt.request_type & USB_STD_REQ_RECIPIENT_MASK))
        {
            if((g_setup_pkt.value >> 8) && (g_usbd_class_cb->usb_class_get_descriptor != 0))
            {
                *buf_pp = g_usbd_class_cb->usb_class_get_descriptor(USB_STD_REQ_RECIPIENT_ENDPOINT,
                                                                   (g_setup_pkt.value >> 8),
                                                                   len_p,
                                                                   g_usbd_dev_conf.device_speed);
                return(USB_SUCCESS);
            }
            else
            {
                return(USB_FAIL);
            }
        }
    }
    return USB_FAIL;
}

/******************************************************************************
 This function sets the Device address sent by host in the MSS USB register.
*/
static uint8_t
MSS_USBD_set_address
(
    void
)
{
    uint8_t addr;

    addr = (uint8_t)(g_setup_pkt.value & 0x7Fu);
    g_usbd_dev_conf.device_addr = addr;

    addr = (uint8_t)(g_setup_pkt.value & 0x7Fu);

    /*USB2.0 section 9.4.6*/
    if(g_usbd_dev_conf.device_state == MSS_USB_CONFIGURED_STATE)
    {
        /*Behavior not defined by USB2.0, May raise error here*/
        return USB_FAIL;
    }
    else
    {
        if(g_usbd_dev_conf.device_state == MSS_USB_DEFAULT_STATE)
        {
            if(addr != 0u)
            {
                g_usbd_dev_conf.device_state = MSS_USB_ADDRESS_STATE;
            }
        }
        else if(g_usbd_dev_conf.device_state == MSS_USB_ADDRESS_STATE)
        {
            if(addr == 0u)
            {
                g_usbd_dev_conf.device_state = MSS_USB_DEFAULT_STATE;
            }
        }
        /*FADDR register will be updated after Status phase of this Request*/
    }
    return USB_SUCCESS;
}

/******************************************************************************
 This function processes SET_DESCRIPTOR request.
*/
static uint8_t
MSS_USBD_set_descriptor
(
    void
)
{
    return USB_FAIL;
}

/******************************************************************************
 This function processes SET_CONFIGURATION request.
*/
static uint8_t
MSS_USBD_set_config
(
    void
)
{
    uint8_t cfgidx;

    cfgidx = (uint8_t)g_setup_pkt.value;

    /*USB2.0 section 9.4.6*/
    if(g_usbd_dev_conf.device_state == MSS_USB_DEFAULT_STATE)
    {
        /*Undefined behavior*/
        return USB_FAIL;
    }
    else
    {
        /*This value will be returned in Get_config command*/
        g_usbd_dev_conf.active_config_num = cfgidx;
        if(g_usbd_dev_conf.device_state == MSS_USB_ADDRESS_STATE)
        {
            if(cfgidx)
            {
                g_usbd_dev_conf.device_state = MSS_USB_CONFIGURED_STATE;
                g_usbd_class_cb->usb_class_init(cfgidx,
                                                g_usbd_dev_conf.device_speed);
            }
        }
        else if(g_usbd_dev_conf.device_state == MSS_USB_CONFIGURED_STATE)
        {
            if(cfgidx == 0)
            {
                g_usbd_dev_conf.device_state = MSS_USB_ADDRESS_STATE;
                g_usbd_class_cb->usb_class_deinit(cfgidx);
            }
            else
            {
                g_usbd_class_cb->usb_class_init(cfgidx,
                                                g_usbd_dev_conf.device_speed);
            }
        }
    }
    return USB_SUCCESS;
}

/******************************************************************************
 This function processes GET_CONFIGURATION requests
*/
static uint8_t
MSS_USBD_get_config
(
    uint8_t** buf_pp,
    uint32_t* len_p
)
{
    if((g_setup_pkt.value == 0u) &&
       (g_setup_pkt.index == 0u) &&
       (g_setup_pkt.length == 1u))
    {
        /*This value was set in Set_config command*/
        *buf_pp = &g_usbd_dev_conf.active_config_num ;
        *len_p = 1u;
        return USB_SUCCESS;
    }
    else
    {
        return USB_FAIL;
    }
}

/******************************************************************************
 This function processes GET_STATUS request.
*/
static uint8_t
MSS_USBD_get_status
(
    uint8_t** buf_pp,
    uint32_t* len_p
)
{
    if(g_usbd_dev_conf.device_state == MSS_USB_ADDRESS_STATE)
    {
        if((g_setup_pkt.value == 0u) &&
           (g_setup_pkt.index == 0u) &&
           (g_setup_pkt.length == 2u))
        {
            switch (g_setup_pkt.request_type & USB_STD_REQ_RECIPIENT_MASK)
            {
            case USB_STD_REQ_RECIPIENT_DEVICE:
                /*SF2 device is always self powered. RemoteWakeup NotSupported*/
                    g_usbd_status = 0x0001;
                    *buf_pp = (uint8_t*)&g_usbd_status;
                    break;
            case USB_STD_REQ_RECIPIENT_INTERFACE:        /*Reserved by USB2.0*/
                    g_usbd_status = 0x0000u;
                    *buf_pp = (uint8_t*)&g_usbd_status;
                break;

            case USB_STD_REQ_RECIPIENT_ENDPOINT:/*Endpoint halt (stall) status*/
                    g_usbd_status = ((device_tx_ep[g_setup_pkt.index].stall) ? 0x0001:0x0000);
                    *buf_pp = (uint8_t*)&g_usbd_status;
                break;
            default:
                return USB_FAIL;
            }
            * len_p = sizeof(g_usbd_status);
        }
        else
        {
            return USB_FAIL;
        }
    }
    else if(g_usbd_dev_conf.device_state == MSS_USB_CONFIGURED_STATE)
    {
        if((g_setup_pkt.value == 0) && (g_setup_pkt.length == 2))
        {
            switch (g_setup_pkt.request_type & USB_STD_REQ_RECIPIENT_MASK)
            {
             case USB_STD_REQ_RECIPIENT_DEVICE: /*SF2 device is self powered*/
                g_usbd_status = 0x0001;
                g_usbd_status |= (g_usbd_dev_conf.remote_wakeup << 0x0001u);
                *buf_pp = (uint8_t*)&g_usbd_status;

             break;

             case USB_STD_REQ_RECIPIENT_INTERFACE:
                if(g_setup_pkt.index <= g_usbd_dev_conf.device_total_interfaces)
                {
                    g_usbd_status = 0x0000u;        /*Reserved by USB2.0*/
                    *buf_pp = (uint8_t*)&g_usbd_status;
                }
                else
                {
                    return USB_FAIL;
                }
            break;
            case USB_STD_REQ_RECIPIENT_ENDPOINT:
                if(((uint8_t)(g_setup_pkt.index) & 0x80u)) /* IN,TX endpoint*/
                {
                    uint8_t idx = (((uint8_t)(g_setup_pkt.index))& 0x7fu);
                    g_usbd_status = ((device_tx_ep[idx].stall) ? 0x0001:0x0000) ;
                    *buf_pp = (uint8_t*)&g_usbd_status;
                }
                else    /*out, rx endpoint*/
                {
                    g_usbd_status = ((device_rx_ep[g_setup_pkt.index].stall) ? 0x0001:0x0000) ;
                    *buf_pp = (uint8_t*)&g_usbd_status;
                }
             break;

             default:
                return USB_FAIL;
            }
            * len_p = sizeof(g_usbd_status);
        }
        else
        {
            return USB_FAIL;
        }

    }
    else if(g_usbd_dev_conf.device_state == MSS_USB_DEFAULT_STATE)
    {
        return USB_FAIL;
    }
    return USB_SUCCESS;
}

/******************************************************************************
 This function processes SET_FEATURE request.
*/
static uint8_t
MSS_USBD_set_feature
(
    void
)
{
    uint8_t result = USB_SUCCESS;
    if(g_setup_pkt.length == 0)
    {
        switch(g_setup_pkt.value)
        {
        case USB_STD_FEATURE_REMOTE_WAKEUP:
            if(((g_setup_pkt.request_type & USB_STD_REQ_RECIPIENT_MASK) == USB_STD_REQ_RECIPIENT_DEVICE) &&
                (g_usbd_dev_conf.device_state > MSS_USB_DEFAULT_STATE))
            {
                /*Enable Remote wakeup capability for the device*/
                g_usbd_dev_conf.remote_wakeup = 0x01u;
                ASSERT(0);  //RemoteWakeupNot enabled by Config Descr.
            }
            else
            {
                result = USB_FAIL;
            }
            break;
        case USB_STD_FEATURE_EP_HALT:
            if(((g_setup_pkt.request_type & USB_STD_REQ_RECIPIENT_MASK) == USB_STD_REQ_RECIPIENT_ENDPOINT) &&
                ((uint8_t)(g_setup_pkt.index)) &&
                (g_usbd_dev_conf.device_state == MSS_USB_CONFIGURED_STATE))
            {
                /*8.5.3.4 Control EP should not be implementing HALT feature*/
                if(((uint8_t)(g_setup_pkt.index) & 0x80u)) /* IN,TX endpoint*/
                {
                    device_tx_ep[(((uint8_t)(g_setup_pkt.index))& 0x7fu)].stall = 0x01;     /*Enable HALT*/
                    MSS_USBD_CIF_tx_ep_stall((mss_usb_ep_num_t)((((uint8_t)(g_setup_pkt.index)) & 0x7fu)));
                }
                else    /*out, rx endpoint*/
                {
                    device_rx_ep[(uint8_t)(g_setup_pkt.index)].stall = 0x01;     /*Enable HALT*/
                    MSS_USBD_CIF_rx_ep_stall((mss_usb_ep_num_t)(g_setup_pkt.index));
                }
            }
            else
            {
                result = USB_FAIL;
            }
            break;
        case USB_STD_FEATURE_TEST_MODE:
            if((g_setup_pkt.request_type & USB_STD_REQ_RECIPIENT_MASK) ==
               USB_STD_REQ_RECIPIENT_DEVICE)
            {
                switch(g_setup_pkt.index >> 8u)
                {
                    case USB_TEST_MODE_SELECTOR_TEST_J:
                    break;
                    case USB_TEST_MODE_SELECTOR_TEST_K:
                    break;
                    case USB_TEST_MODE_SELECTOR_TEST_SE0NAK:
                    break;
                    case USB_TEST_MODE_SELECTOR_TEST_PACKET:
                    break;
                    case USB_TEST_MODE_SELECTOR_TEST_FORCE_ENA:
                    break;
                    default:
                    break;
                }
            }
            else
            {
                result = USB_FAIL;
            }
            break;
        default:
            result = USB_FAIL;
            break;
        }
    }
    else
    {
        result = USB_FAIL;
    }
    return result;
}

/******************************************************************************
 This function processes CLEAR_FEATURE.
*/
static uint8_t
MSS_USBD_clr_feature
(
    void
)
{
    uint8_t result = USB_SUCCESS;

    if((g_setup_pkt.length == 0) &&
       (g_usbd_dev_conf.device_state > MSS_USB_DEFAULT_STATE))
    {
        switch(g_setup_pkt.value)
        {
        case USB_STD_FEATURE_REMOTE_WAKEUP:
            if((g_setup_pkt.request_type & USB_STD_REQ_RECIPIENT_MASK) ==
               USB_STD_REQ_RECIPIENT_DEVICE)
            {
                /*Disable Remote wakeup capability for the device*/
                g_usbd_dev_conf.remote_wakeup = 0x00u;
            }
            break;
        case USB_STD_FEATURE_EP_HALT:
            if(((g_setup_pkt.request_type & USB_STD_REQ_RECIPIENT_MASK) ==
                USB_STD_REQ_RECIPIENT_ENDPOINT))
            {
                /*8.5.3.4 HALT clear can be performed on CEP and Data EP*/
                if((uint8_t)(g_setup_pkt.index)) /*Not a control endpoint*/
                {
                    if(((uint8_t)(g_setup_pkt.index) & 0x80u)) /* IN,TX endpoint*/
                    {
                        /*Disable HALT*/
                        device_tx_ep[(((uint8_t)(g_setup_pkt.index))& 0x7fu)].stall = 0x00u;
                        MSS_USBD_CIF_tx_ep_clr_stall((mss_usb_ep_num_t)((((uint8_t)(g_setup_pkt.index)) & 0x7fu)));
                    }
                    else    /* out, rx endpoint */
                    {
                        /*Enable HALT*/
                        device_rx_ep[(uint8_t)(g_setup_pkt.index)].stall = 0x00u;
                        MSS_USBD_CIF_rx_ep_clr_stall((mss_usb_ep_num_t)(g_setup_pkt.index));
                    }
                }
                else
                {
                    result = USB_SUCCESS;
                }
            }
            break;
        case USB_STD_FEATURE_TEST_MODE:
            if((g_setup_pkt.request_type & USB_STD_REQ_RECIPIENT_MASK) ==
               USB_STD_REQ_RECIPIENT_DEVICE)
            {
                switch(g_setup_pkt.index >> 8)
                {
                    case USB_TEST_MODE_SELECTOR_TEST_J:
                    break;
                    case USB_TEST_MODE_SELECTOR_TEST_K:
                    break;
                    case USB_TEST_MODE_SELECTOR_TEST_SE0NAK:
                    break;
                    case USB_TEST_MODE_SELECTOR_TEST_PACKET:
                    break;
                    case USB_TEST_MODE_SELECTOR_TEST_FORCE_ENA:
                    break;
                    default:
                    break;
                }
            }
            break;
        default:
            result = USB_FAIL;
            break;
        }
    }
    else
    {
        result = USB_FAIL;
    }
    return result;
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
void
MSS_USBD_get_hwcore_info
(
    mss_usb_core_info_t* hw_info
)
{
    MSS_USBD_CIF_get_hwcore_info(hw_info);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
void
MSS_USBD_set_dev_address
(
    uint8_t dev_addr
)
{
    MSS_USBD_CIF_set_dev_addr(dev_addr);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
uint8_t
MSS_USBD_get_dev_address
(
    void
)
{
    uint8_t temp;
    temp = MSS_USBD_CIF_get_dev_addr();
    return(temp);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
void
MSS_USBD_dev_force_resume
(
    void
)
{
    MSS_USBD_CIF_force_resume();
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
void
MSS_USBD_set_isoupdate
(
    void
)
{
    MSS_USBD_CIF_set_isoupdate();
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
void
MSS_USBD_tx_ep_clr_isoupdate
(
    void
)
{
    MSS_USBD_CIF_clr_isoupdate();
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
uint8_t
MSS_USBD_tx_ep_is_fifo_notempty
(
    mss_usb_ep_num_t epnum
)
{
    uint8_t temp;
    temp = MSS_USB_CIF_is_txepfifo_notempty(epnum);
    return(temp);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
uint8_t
MSS_USBD_rx_ep_is_fifo_full
(
    mss_usb_ep_num_t epnum
)
{
    uint8_t temp;
    temp = MSS_USB_CIF_rx_ep_is_fifo_full(epnum);
    return(temp);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
void
MSS_USBD_enable_usbirq
(
    uint8_t irq_mask
)
{
    MSS_USB_CIF_enable_usbirq(irq_mask);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
void
MSS_USBD_disable_usbirq
(
    uint8_t irq_mask
)
{
    MSS_USB_CIF_disable_usbirq(irq_mask);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
void
MSS_USBD_tx_ep_disable_irq
(
    mss_usb_ep_num_t epnum
)
{
    MSS_USB_CIF_tx_ep_disable_irq(epnum);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
void
MSS_USBD_rx_ep_disable_irq
(
    mss_usb_ep_num_t epnum
)
{
    MSS_USB_CIF_rx_ep_disable_irq(epnum);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
void
MSS_USBD_tx_ep_enable_irq
(
    mss_usb_ep_num_t epnum
)
{
    MSS_USB_CIF_tx_ep_enable_irq(epnum);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
void
MSS_USBD_rx_ep_enable_irq
(
    mss_usb_ep_num_t epnum
)
{
    MSS_USB_CIF_rx_ep_enable_irq(epnum);
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
void
MSS_USBD_cep_enable_irq
(
    void
)
{
    MSS_USB_CIF_cep_enable_irq();
}

/***************************************************************************//**
 See mss_usb_device.h for details of how to use this function.
*/
void
MSS_USBD_cep_disable_irq
(
    void
)
{
    MSS_USB_CIF_cep_disable_irq();
}


#ifdef __cplusplus
}
#endif
