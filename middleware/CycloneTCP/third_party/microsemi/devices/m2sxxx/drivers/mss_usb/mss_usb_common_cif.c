/*******************************************************************************
* (c) Copyright 2012-2013 Microsemi SoC Products Group.  All rights reserved.
 *
 * Smartfusion2 MSS USB Driver Stack
 *      USB Core Interface Layer (USB-CIFL)
 *          USB-CIF driver
 *
 * USB-CIF driver implementation:
 * This source file implements MSS USB Interrupt handler functions. This file
 * also implements core interface function for the logical layer to control
 * the MSS USB core. These interface functions are independent of the USB mode.
 *
 * SVN $Revision: 5468 $
 * SVN $Date: 2013-03-29 15:38:01 +0530 (Fri, 29 Mar 2013) $
 */

#include "../../CMSIS/mss_assert.h"
#include "mss_usb_core_regs.h"
#include "mss_usb_common_reg_io.h"
#include "mss_usb_common_cif.h"

#ifdef MSS_USB_HOST_ENABLED
#include "mss_usb_host_cif.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
* Global variables shared by mss_usb_device_cif.c and mss_usb_common_cif.c
*/
extern mss_usbd_cb_t g_mss_usbd_cb;
extern volatile mss_usbd_cep_state_t cep_state;
extern mss_usb_ep_t *gd_rx_ep[5];
extern mss_usb_ep_t *gd_tx_ep[5];

/*******************************************************************************
 * Private functions declarations of USB-CIF.
 *******************************************************************************/
#if defined(__GNUC__)
__attribute__((__interrupt__)) void USB_IRQHandler(void);
#else
void USB_IRQHandler(void);
#endif

#if defined(__GNUC__)
__attribute__((__interrupt__)) void USB_DMA_IRQHandler(void);
#else
void USB_DMA_IRQHandler(void);
#endif

static void
MSS_USB_CIF_handle_cep_irq
(
    void
);

static void
MSS_USB_CIF_handle_tx_ep_irq
(
    uint16_t irq_num
);

static void
MSS_USB_CIF_handle_rx_ep_irq
(
    uint16_t irq_num
);

/*******************************************************************************
 Read USB interrupt register to know which interrupt has occured.
 */

/*******************************************************************************
 Main USB interrupt handler. It checks for TX/RX endpoint interrupt and USB
 system level interrupts and calls the appropriate routine.
*/
#if defined(__GNUC__)
__attribute__((__interrupt__)) void USB_IRQHandler(void)
#else
void USB_IRQHandler(void)
#endif
{
    volatile uint8_t usb_irq;
    volatile uint16_t tx_ep_irq;
    volatile uint16_t rx_ep_irq;

#ifdef MSS_USB_HOST_ENABLED
    volatile uint8_t role;
#endif

    usb_irq = MSS_USB_CIF_read_irq_reg();
    tx_ep_irq = MSS_USB_CIF_read_tx_ep_irq_reg();
    rx_ep_irq = MSS_USB_CIF_read_rx_ep_irq_reg();

#ifdef MSS_USB_DEVICE_ENABLED
    if(MSS_USB_CORE_MODE_DEVICE == MSS_USB_CIF_get_mode())
    {
        if(usb_irq & RESUME_IRQ_MASK)
        {
            g_mss_usbd_cb.usb_device_resume();
        }
        if(usb_irq & SUSPEND_IRQ_MASK)
        {
            g_mss_usbd_cb.usb_device_suspend();
        }
        if(usb_irq & DISCONNECT_IRQ_MASK)
        {
            MSS_USB_CIF_disable_usbirq(0xFFu);
            NVIC_ClearPendingIRQ(USB_IRQn);
        }
        if(usb_irq & RESET_IRQ_MASK)
        {
            MSS_USB_CIF_set_index_reg(MSS_USB_CEP);
            MSS_USB_CIF_enable_usbirq(DISCONNECT_IRQ_MASK | SUSPEND_IRQ_MASK);
            cep_state = MSS_USB_CTRL_EP_IDLE;
            MSS_USB_CIF_clr_usb_irq_reg();
            MSS_USB_CIF_cep_clr_setupend();
            MSS_USB_CIF_cep_clr_stall_sent();
            g_mss_usbd_cb.usb_device_reset();
        }
/*        if(0x08u == (usb_irq & SOF_IRQ_MASK))
        {
            g_mss_usb_device_cb.usb_device_sof(0);
        }
*/

        if(tx_ep_irq & 0x0001u)
        {
            /*handle EP0 IRQ*/
            MSS_USB_CIF_handle_cep_irq();
        }
        if(tx_ep_irq & 0xFFFEu)         /*EP0 is taken care above*/
        {
            /*Handle TX EP here, pass on the EP numbers*/
            MSS_USB_CIF_handle_tx_ep_irq(tx_ep_irq);
        }

        if(rx_ep_irq & 0xFFFEu)         /*bit0 is not defined*/
        {
            /*Handle RX EP here, pass on the EP numbers*/
            MSS_USB_CIF_handle_rx_ep_irq(rx_ep_irq);
        }

    }
#endif
#ifdef MSS_USB_HOST_ENABLED
    else if(MSS_USB_CORE_MODE_HOST == MSS_USB_CIF_get_mode())
    {
        role = MSS_USB_CIF_get_role();
        if(usb_irq & RESUME_IRQ_MASK)
        {
           /*Resume interrupt in Host mode means Remote Wakeup request*/
        }
        /*Vbus_err and session request interrupts are valid only in A device*/
        if(MSS_USB_DEVICE_ROLE_DEVICE_A == role)
        {
            if(usb_irq & SESSION_REQUEST_IRQ_MASK)
            {
                /*This means SRP initiated by Target devive.*/
            }
            if(usb_irq & VBUS_ERROR_IRQ_MASK)
            {
                /*Power management*/

            }
        }
        if(usb_irq & CONNECT_IRQ_MASK)
        {
            MSS_USB_HCIF_handle_connect_irq();
        }
        if(usb_irq & DISCONNECT_IRQ_MASK)
        {
            MSS_USB_CIF_disable_usbirq(0xFFu);
            NVIC_ClearPendingIRQ(USB_IRQn);
        }
        if(usb_irq & BABBLE_IRQ_MASK)
        {

        }
#if 0   /*SOF interrupt is not processed*/
        if(usb_irq & SOF_IRQ_MASK)
        {
            g_mss_usb_device_cb.usb_device_sof(0);
        }
#endif
    }
#endif
    NVIC_ClearPendingIRQ(USB_IRQn);
}

/*******************************************************************************
 Routine to handle the interrupt on Control Endpoint.(EP0)
 */
static void
MSS_USB_CIF_handle_cep_irq
(
    void
)
{
    uint8_t status = 0u;

    MSS_USB_CIF_set_index_reg(MSS_USB_CEP);

    if(MSS_USB_CIF_cep_is_stall_sent())
    {
        status |= CTRL_EP_STALL_ERROR;
        MSS_USB_CIF_cep_clr_stall_sent();
        g_mss_usbd_cb.usb_device_cep_setup(status);
    }
    else
    {
        if(MSS_USB_CIF_cep_is_setupend())
        {
            MSS_USB_CIF_cep_clr_setupend();

            if(!MSS_USB_CIF_cep_is_rxpktrdy())
            {
                status |= CTRL_EP_SETUP_END_ERROR;
            }
            else
            {
                status &= ~CTRL_EP_SETUP_END_ERROR;
            }

            g_mss_usbd_cb.usb_device_cep_setup(status);
        }
        else
        {
            if(cep_state == MSS_USB_CTRL_EP_IDLE)
            {
                if(MSS_USB_CIF_cep_is_rxpktrdy())
                {
                    g_mss_usbd_cb.usb_device_cep_setup(status);
                }
            }
            else if (cep_state == MSS_USB_CTRL_EP_TX)
            {
                g_mss_usbd_cb.usb_device_cep_datain(status);
            }
            else if(cep_state == MSS_USB_CTRL_EP_RX)
            {
                if(MSS_USB_CIF_cep_is_rxpktrdy())
                {
                    g_mss_usbd_cb.usb_device_cep_dataout(status);
                }
            }
            else
            {
                ASSERT(0);
            }
        }
    }
}

/*******************************************************************************
 Routine to handle the interrupt on TX_EP(IN Transactions).
 */
static void MSS_USB_CIF_handle_tx_ep_irq
(
    uint16_t irq_num
)
{
    mss_usb_ep_num_t ep_num = MSS_USB_TX_EP_1;
    uint8_t status = 0u;

    while(irq_num)
    {
        irq_num >>= 1u;                                  /*EP1 starts from D1*/

        if((irq_num) && (irq_num & MSS_USB_WORD_BIT_0_MASK))
        {
            if(MSS_USB_CIF_tx_ep_is_underrun(ep_num))
            {
                /*Under-run errors should happen only for ISO endpoints.*/
                status |= TX_EP_UNDER_RUN_ERROR;
                MSS_USB_CIF_tx_ep_clr_underrun(ep_num);
            }
            if(MSS_USB_CIF_tx_ep_is_stall_sent_bit(ep_num))
            {
                status |= TX_EP_STALL_ERROR;
                MSS_USB_CIF_tx_ep_clr_stall_sent_bit(ep_num);
            }
            MSS_USB_CIF_tx_ep_disable_irq(ep_num);
            g_mss_usbd_cb.usb_device_ep_datain(ep_num,status);
        }
        status = 0u; /*resetting for next EP status*/
        ++ep_num;
    }
}

/*******************************************************************************
 Routine to handle the interrupt on RX EP (OUT transactions).
 */
static void MSS_USB_CIF_handle_rx_ep_irq
(
    uint16_t irq_num
)
{
    mss_usb_ep_num_t ep_num = MSS_USB_RX_EP_1;
    uint8_t status = 0u;

    while(irq_num)
    {
        irq_num >>= 1u;  /*EP1 starts from D1*/
        if((irq_num) && (irq_num & MSS_USB_WORD_BIT_0_MASK))
        {
            if(MSS_USB_CIF_rx_ep_is_overrun(ep_num))
            {
                status |= RX_EP_OVER_RUN_ERROR;
                MSS_USB_CIF_rx_ep_clr_overrun(ep_num);
            }
            if(MSS_USB_CIF_rx_ep_is_stall_sent_bit(ep_num))
            {
                status |= RX_EP_STALL_ERROR;
                /*
                "sent stall" bit should be cleared."Send Stall" bit is still set.
                it should be cleared via Clear feature command or reset"
                */
                MSS_USB_CIF_rx_ep_clr_stall_sent_bit(ep_num);
            }
            if(MSS_USB_CIF_rx_ep_is_dataerr(ep_num))
            {
                /*This error will be cleared when RxPktRdy bit is cleared.*/
                status |= RX_EP_DATA_ERROR;

            }
#if 0
            /*
            PID error and INCOMP error should be checked only in ISO transfers.
            This should be moved to logical layer
            */
            if(MSS_USB_CIF_rx_ep_is_piderr(ep_num))
            {
                status |= RX_EP_PID_ERROR;
                /*Data sheet doesnt mention about how this error bit is cleared
                Assuming that this will be cleared when RxPKTRdy is cleared.*/
            }
            if(MSS_USB_CIF_rx_ep_is_isoincomp(ep_num))
            {
                status |= RX_EP_ISO_INCOMP_ERROR;
                /*This error will be cleared when RxPktRdy bit is cleared.*/
            }
#endif
            if(MSS_USB_CIF_rx_ep_is_rxpktrdy(ep_num))
            {
                volatile uint32_t received_count = 0u;
                received_count = (uint32_t)MSS_USB_CIF_rx_ep_read_count(ep_num);

                if(DMA_DISABLE == (gd_rx_ep[ep_num]->dma_enable))
                {
                    /*
                    Read packet manually and call upper layer
                    received_count = 0 is also valid condition, meaning zlp received.
                    */
                    if(received_count)
                    {
                        MSS_USB_CIF_read_rx_fifo(ep_num,
                                                 gd_rx_ep[ep_num]->buf_addr,
                                                 received_count);

                        gd_rx_ep[ep_num]->txn_count = received_count;
                        gd_rx_ep[ep_num]->xfr_count += received_count;
                    }
                    MSS_USB_CIF_rx_ep_disable_irq(ep_num);
                    MSS_USB_CIF_rx_ep_clr_rxpktrdy(ep_num);
                    g_mss_usbd_cb.usb_device_ep_dataout(ep_num, status);

                }
                else
                {
                    if(MSS_USB_DMA_MODE1 == (MSS_USB_CIF_rx_ep_get_dma_mode(ep_num)))
                    {
#if 1
                        uint32_t increamented_addr;
                        /*
                        This means we are in BULK transfer with DMA mode1....
                        all the rxmaxP size pkts are received and last short pkt
                        need to be read withougt DMA or by switching to mode 0.
                        After switching mode to 0, this ISR handler is invoked
                        again. Data packet will be read then.
                        MUSB: section 16
                        */

                        /*read 'short packet' withought DMA*/
                        MSS_USB_CIF_dma_stop_xfr(gd_rx_ep[ep_num]->dma_channel);
                        MSS_USB_CIF_rx_ep_clr_autoclr(ep_num);
                        MSS_USB_CIF_rx_ep_disable_irq(ep_num);
                        increamented_addr = MSS_USB_CIF_dma_read_addr(gd_rx_ep[ep_num]->dma_channel);

                        /*Count number of bytes read so far,since DMA was
                        operating in m1 with Autoclr.*/
                        gd_rx_ep[ep_num]->xfr_count = (increamented_addr -
                                                     ((uint32_t)(gd_rx_ep[ep_num]->buf_addr)));

                        if(received_count)
                        {
                            MSS_USB_CIF_read_rx_fifo(ep_num,
                                                     (uint8_t*)increamented_addr,
                                                     received_count);

                            gd_rx_ep[ep_num]->txn_count = received_count;
                            gd_rx_ep[ep_num]->xfr_count += received_count;
                        }

                        g_mss_usbd_cb.usb_device_ep_dataout(ep_num, status);
                        MSS_USB_CIF_rx_ep_clr_rxpktrdy(ep_num);
#else
                        /*Code to switch to m0 from m1*/
                        MSS_USB_CIF_dma_stop_xfr(gd_rx_ep[ep_num]->dma_channel);
                        MSS_USB_CIF_rx_ep_disable_dma(ep_num);
                        MSS_USB_CIF_rx_ep_clr_autoclr(ep_num);
                        MSS_USB_CIF_rx_ep_set_dma_mode0(ep_num);
                        MSS_USB_CIF_rx_ep_enable_dma(ep_num);
#endif
                    }
                    else
                    {
                        MSS_USB_CIF_rx_ep_disable_irq(ep_num);
                        MSS_USB_CIF_dma_write_count(gd_rx_ep[ep_num]->dma_channel,
                                                    received_count);

                        MSS_USB_CIF_dma_start_xfr(gd_rx_ep[ep_num]->dma_channel);
                        /*Upper layer cb will be called from DMA ISR*/
                    }
                }
            }
        }
        status = 0u; /*resetting for next EP status*/
        ++ep_num;
    }
}

/*******************************************************************************
 Handler for DMA interrupt. Checks for the DMA channel on which interrupt has
 Occured and corresponding EP number and calls the upper layer to indicate the
 event.
 */
#if defined(__GNUC__)
__attribute__((__interrupt__)) void USB_DMA_IRQHandler(void)
#else
void USB_DMA_IRQHandler(void)
#endif
{
    mss_usb_dma_channel_t dma_channel= (mss_usb_dma_channel_t)0u;
    uint8_t status = 0;

    volatile mss_usb_dma_dir_t dma_dir;
    mss_usb_ep_num_t ep_num;
    volatile uint32_t dma_irq;
    volatile uint32_t increamented_addr;

    dma_irq = MSS_USB_CIF_dma_read_irq();

    while(dma_irq)
    {
        if(dma_irq & 0x01u)
        {
            /*DMA Transfer for this channel is complete.Clear Start_transfer bit*/
            MSS_USB_CIF_dma_stop_xfr(dma_channel);

            ep_num = MSS_USB_CIF_dma_get_epnum(dma_channel);
            dma_dir = MSS_USB_CIF_dma_get_dir(dma_channel);

            if(MSS_USB_CIF_dma_is_bus_err(dma_channel))
            {
                status |=DMA_XFR_ERROR;
                MSS_USB_CIF_dma_clr_bus_err(dma_channel);

                if(MSS_USB_CIF_rx_ep_is_rxpktrdy((mss_usb_ep_num_t)(ep_num)))
                    MSS_USB_CIF_rx_ep_clr_rxpktrdy((mss_usb_ep_num_t)(ep_num));
            }

            if(dma_dir == MSS_USB_DMA_READ)    /*TX EP*/
            {
                /*
                Mode0: single packet <= TxMaxPkt is loaded in Tx FIFO. send this
                packet by setting TxMaxPkt bit.
                Mode1: With Autoset in Mode1, all the TxMaxPkt size packets
                would have been sent by now. set txpktrdy to Send the last
                short packet.
                If ADD_ZLP_TO_XFR was SET then set TxPktRdy. If last packet was
                less than TxMaxPkt the last "short packet" will be sent. if
                last packet was also TxMaxPkt size then "Null packet" will be
                sent to indicate end of transfer.(VCP requirement)
                If ADD_ZLP_TO_XFR was not SET then, set TxPktRdy only for last
                "short packet". For last packet of TxMaxPkt size, "NULL packet"
                will NOT be sent. (MSD requirement)

                For both modes EP interrupt will be generated when the current
                packet is transmitted on USB bus.
                After that call the upper layer CB.
                */

                /*
                This bit was set for DMA-m1, ideally this bit should not be reset
                but with this bit set, TxEP Interrupt is not occurring.
                */

                if(MSS_USB_CIF_tx_ep_is_dma_enabled(ep_num))
                {
                    MSS_USB_CIF_tx_ep_disable_dma(ep_num);
                }

                increamented_addr = MSS_USB_CIF_dma_read_addr(gd_tx_ep[ep_num]->dma_channel);

                gd_tx_ep[ep_num]->xfr_count = (increamented_addr -
                                               ((uint32_t)(gd_tx_ep[ep_num]->buf_addr)));

                /*
                Decide to send the NULL packet or not depending on add_zlp flag
                and transfer length. Update txn_count and xfr count accordingly.
                */
                if(MSS_USB_DMA_MODE1 == MSS_USB_CIF_tx_ep_get_dma_mode(ep_num))
                {
                    if(ADD_ZLP_TO_XFR == gd_tx_ep[ep_num]->add_zlp)
                    {
                        if(gd_tx_ep[ep_num]->xfr_count >= gd_tx_ep[ep_num]->max_pkt_size)
                        {
                            gd_tx_ep[ep_num]->txn_count = (gd_tx_ep[ep_num]->xfr_count %
                                                           gd_tx_ep[ep_num]->max_pkt_size);
                        }
                        else
                        {
                            gd_tx_ep[ep_num]->txn_count = gd_tx_ep[ep_num]->xfr_count;
                        }
                        MSS_USB_CIF_tx_ep_set_txpktrdy(ep_num);
                    }
                    else
                    {
                        if(gd_tx_ep[ep_num]->xfr_count > gd_tx_ep[ep_num]->max_pkt_size)
                        {
                            if((gd_tx_ep[ep_num]->xfr_count % gd_tx_ep[ep_num]->max_pkt_size))
                                gd_tx_ep[ep_num]->txn_count = (gd_tx_ep[ep_num]->xfr_count % gd_tx_ep[ep_num]->max_pkt_size);
                            else
                                gd_tx_ep[ep_num]->txn_count = gd_tx_ep[ep_num]->max_pkt_size;
                        }
                        else
                        {
                            gd_tx_ep[ep_num]->txn_count = gd_tx_ep[ep_num]->xfr_count;
                        }

                        if((gd_tx_ep[ep_num]->txn_count % gd_tx_ep[ep_num]->max_pkt_size) ||
                           (gd_tx_ep[ep_num]->xfr_count == 0))
                        {
                            MSS_USB_CIF_tx_ep_set_txpktrdy(ep_num);
                        }
                        else
                        {
                            /* wait till last TxMaxPkt size packet is sent.*/
                            while(MSS_USB_CIF_tx_ep_is_txpktrdy(ep_num));
                            MSS_USB_CIF_tx_ep_disable_irq(ep_num);
                            g_mss_usbd_cb.usb_device_ep_datain(ep_num,status);
                        }
                    }
                }
                else
                {
                    if(gd_tx_ep[ep_num]->xfr_count > gd_tx_ep[ep_num]->max_pkt_size)
                    {
                        if((gd_tx_ep[ep_num]->xfr_count % gd_tx_ep[ep_num]->max_pkt_size))
                            gd_tx_ep[ep_num]->txn_count = (gd_tx_ep[ep_num]->xfr_count %
                                                           gd_tx_ep[ep_num]->max_pkt_size);
                        else
                            gd_tx_ep[ep_num]->txn_count = gd_tx_ep[ep_num]->max_pkt_size;
                    }
                    else
                    {
                        gd_tx_ep[ep_num]->txn_count = gd_tx_ep[ep_num]->xfr_count;
                    }
                    MSS_USB_CIF_tx_ep_set_txpktrdy(ep_num);
                }
            }
            else if(dma_dir == MSS_USB_DMA_WRITE)/*RX EP*/
            {
                 MSS_USB_CIF_rx_ep_disable_irq(ep_num);   //RX_IRQ_EN register
                /*
                 In DMA-m1 or m0 DMA_ADDR register is incremented as bytes
                 are transfered. Read dma_addr buffer to know how many bytes were
                 transfered from RX FIFO.
                 */
                increamented_addr = MSS_USB_CIF_dma_read_addr(gd_rx_ep[ep_num]->dma_channel);

                gd_rx_ep[ep_num]->xfr_count = (increamented_addr -
                                               ((uint32_t)(gd_rx_ep[ep_num]->buf_addr)));

                /*
                EP interrupt will be enabled after received data is processed
                and class driver prepares to read further data.
                RxPktRdy need not be cleared here by CPU since Autoset was
                set and control comes here only when data was exact multiple
                of wMaxPktSize. For all other cases RxPktRdy will be cleared
                in Rx EP interrupt
                */
                if(MSS_USB_DMA_MODE0 == (MSS_USB_CIF_rx_ep_get_dma_mode(ep_num)))
                {
                    gd_rx_ep[ep_num]->txn_count = gd_rx_ep[ep_num]->xfr_count;
                    MSS_USB_CIF_rx_ep_clr_rxpktrdy(ep_num);
                }

                /*Call the logical layer driver callback function*/
                g_mss_usbd_cb.usb_device_ep_dataout((mss_usb_ep_num_t)(ep_num),
                                                    status);
            }
        }
        dma_channel++;
        dma_irq >>= 1u;
    }
    NVIC_ClearPendingIRQ(USB_DMA_IRQn);
}

/*******************************************************************************
 * Writes packet on TX EP
 */
void
MSS_USB_CIF_ep_write_pkt
(
    mss_usb_ep_t* core_ep
)
{
    if((core_ep->num) && (core_ep->buf_addr != 0))
    {
        /*Make sure that address is Modulo-4.Bits D0-D1 are read only.*/
        ASSERT(!(((uint32_t)core_ep->buf_addr) & 0x00000002));

        /*TODO:Can be shifted to app.*/
        while(MSS_USB_CIF_is_txepfifo_notempty(core_ep->num));
        MSS_USB_CIF_tx_ep_enable_irq(core_ep->num);

        if((core_ep->max_pkt_size<= core_ep->fifo_size) &&
               (core_ep->txn_length <= core_ep->fifo_size))
        {
            /*
             Store the endpoint info to calculate number of bytes transfered by
             DMA and to update the txn_length and xfr_length.
            */
            gd_tx_ep[core_ep->num] = core_ep;
            if(core_ep->dma_enable == DMA_ENABLE)
            {
                MSS_USB_CIF_dma_write_addr(core_ep->dma_channel,
                                           (uint32_t)(core_ep->buf_addr));

                if(core_ep->xfr_type == MSS_USB_XFR_BULK)
                {
                    MSS_USB_CIF_tx_ep_enable_dma(core_ep->num);

                    /*
                     DMA-m1 will take care of transferring 'xfr_length' data
                     as IN packets arrive.
                     DMA interrupt will occure when all TxMaxPkt size packets
                     are transfered.
                    */
                    MSS_USB_CIF_dma_write_count(core_ep->dma_channel,
                                                core_ep->xfr_length);
                }
                else
                {
                    /*
                    DMA Enable bit in TxCSR is not needed. If set,TX EP
                    Interrupt will not occur.
                    */
                    MSS_USB_CIF_tx_ep_disable_dma(core_ep->num);

                    /* Transfer only one packet with DMA-m0 */
                    MSS_USB_CIF_dma_write_count(core_ep->dma_channel,
                                                core_ep->txn_length);
               }

                /*
                 This will start DMA transfer.
                 TODO: For Null transfer DMA is not needed, but not setting TxPktRdy
                 bit here, is not envoking EP interrupt.
                 EP interrupt does get called when Null DMA transfer is done.
               */
                MSS_USB_CIF_dma_start_xfr(core_ep->dma_channel);


                /*
                * DMA interrupt will occure when all bytes are written to FIFO
                * TxPktRdy should be set when the DMA interrupt occures.
                */
            }
            else    /*no DMA*/
            {
                MSS_USB_CIF_load_tx_fifo(core_ep->num,
                                         core_ep->buf_addr,
                                         core_ep->txn_length);

                core_ep->txn_count = core_ep->txn_length;
                core_ep->xfr_count += core_ep->txn_length;

                MSS_USB_CIF_tx_ep_set_txpktrdy(core_ep->num);
            }
        }
        else
        {
            /* Device layer should have taken care of this */
            ASSERT(0);
        }
    }
    else
    {
        ASSERT(0);
    }
}

/*******************************************************************************
 * Configures DMA for data transfer operations.
 */
void
MSS_USB_CIF_configure_ep_dma
(
    mss_usb_dma_channel_t dma_channel,
    mss_usb_dma_dir_t dma_dir,
    mss_usb_dma_mode_t dma_mode,
    mss_usb_dma_burst_mode_t burst_mode,
    mss_usb_ep_num_t ep_num,
    uint32_t buf_addr
)
{
    MSS_USB_CIF_dma_assign_to_epnum(dma_channel, ep_num);
    MSS_USB_CIF_dma_set_dir(dma_channel, dma_dir);
    MSS_USB_CIF_dma_set_mode(dma_channel, dma_mode);
    MSS_USB_CIF_dma_set_burst_mode(dma_channel, burst_mode);
    MSS_USB_CIF_dma_write_addr(dma_channel, buf_addr);
    MSS_USB_CIF_dma_enable_irq(dma_channel);
}

void
MSS_USB_CIF_tx_ep_configure

(
    mss_usb_ep_t* core_ep
)
{
    uint8_t dpb = 1u;
    mss_usb_dma_mode_t mode;

    if(core_ep->dpb_enable == DPB_ENABLE)
    {
        dpb = 2u;
    }

    MSS_USB_CIF_tx_ep_set_fifo_size(core_ep->num,
                                    ((core_ep->fifo_size) / dpb),
                                    core_ep->dpb_enable);

    MSS_USB_CIF_tx_ep_set_fifo_addr(core_ep->num, core_ep->fifo_addr);

    if(core_ep->dpb_enable == DPB_ENABLE)
    {
        MSS_USB_enable_tx_ep_dpb(core_ep->num);
    }
    else if(core_ep->dpb_enable == DPB_DISABLE)
    {
        MSS_USB_disable_tx_ep_dpb(core_ep->num);
    }
    else
    {
        ASSERT(0);
    }

    MSS_USB_CIF_tx_ep_set_max_pkt(core_ep->num,
                                  core_ep->xfr_type,
                                  core_ep->max_pkt_size,
                                  core_ep->num_usb_pkt);

    MSS_USB_CIF_tx_ep_clr_data_tog(core_ep->num);

    if(core_ep->dma_enable == DMA_ENABLE)
    {
        if((core_ep->xfr_type == MSS_USB_XFR_BULK))
        {
            MSS_USB_CIF_tx_ep_set_dma_mode1(core_ep->num);
            MSS_USB_CIF_tx_ep_enable_dma(core_ep->num);
            mode = MSS_USB_DMA_MODE1;
        }
        else
        {
            /*
            DMA_ENable bit in TXCSRL is not required to be set for m0. if it is
            set TX interrupt would not occur.
            */
            MSS_USB_CIF_tx_ep_set_dma_mode0(core_ep->num);
            MSS_USB_CIF_tx_ep_disable_dma(core_ep->num);
            mode = MSS_USB_DMA_MODE0;
        }

        MSS_USB_CIF_configure_ep_dma(core_ep->dma_channel,
                                     MSS_USB_DMA_READ,
                                     mode,
                                     MSS_USB_DMA_BURST_MODE3,
                                     core_ep->num,
                                     (uint32_t)(core_ep->buf_addr));
    }

    MSS_USB_CIF_tx_ep_enable_irq(core_ep->num);
}

/*******************************************************************************
 * Configures the RX EP for data transfer operations as per the parameters
 * provided by upper layer.
 */
void
MSS_USB_CIF_rx_ep_configure

(
    mss_usb_ep_t* core_ep
)
{
    uint8_t dpb = 1u;
    mss_usb_dma_mode_t mode;
    if(core_ep->dpb_enable == DPB_ENABLE)
    {
        dpb = 2u;
    }

    MSS_USB_CIF_rx_ep_set_fifo_size(core_ep->num,
                                     ((core_ep->fifo_size) / dpb),
                                     core_ep->dpb_enable);

    MSS_USB_CIF_rx_ep_set_fifo_addr(core_ep->num,
                                     core_ep->fifo_addr);

    if(core_ep->dpb_enable == DPB_ENABLE)
    {
        MSS_USB_CIF_enable_rx_ep_dpb(core_ep->num);
    }
    else if(core_ep->dpb_enable == DPB_DISABLE)
    {
        MSS_USB_CIF_disable_rx_ep_dpb(core_ep->num);
    }
    else
    {
        ASSERT(0);
    }

    MSS_USB_CIF_rx_ep_set_max_pkt(core_ep->num,
                                  core_ep->xfr_type,
                                  core_ep->max_pkt_size,
                                  core_ep->num_usb_pkt);

    MSS_USB_CIF_rx_ep_clr_data_tog(core_ep->num);
    MSS_USB_CIF_rx_ep_clr_rxpktrdy(core_ep->num);

    if(core_ep->dma_enable == DMA_ENABLE)
    {
        if((core_ep->xfr_type == MSS_USB_XFR_BULK))
        {
            MSS_USB_CIF_rx_ep_set_dma_mode1(core_ep->num);
            MSS_USB_CIF_rx_ep_enable_dma(core_ep->num);
            mode = MSS_USB_DMA_MODE1;
        }
        else
        {
            /*
            DMA_ENable bit in RXCSRL is not required to be set in m0. if it is
            set RX interrupt would not occur.
            */
            MSS_USB_CIF_rx_ep_set_dma_mode0(core_ep->num);
            MSS_USB_CIF_rx_ep_disable_dma(core_ep->num);
            mode = MSS_USB_DMA_MODE0;
        }

        MSS_USB_CIF_configure_ep_dma(core_ep->dma_channel,
                                     MSS_USB_DMA_WRITE,
                                     mode,
                                     MSS_USB_DMA_BURST_MODE3,
                                     core_ep->num,
                                     (uint32_t)(core_ep->buf_addr));
    }

    MSS_USB_CIF_rx_ep_enable_irq(core_ep->num);
}

#ifdef __cplusplus
}
#endif
