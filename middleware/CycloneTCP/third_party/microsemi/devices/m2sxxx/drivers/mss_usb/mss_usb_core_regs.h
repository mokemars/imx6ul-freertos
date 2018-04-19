/*******************************************************************************
 * (c) Copyright 2012-2013 Microsemi SoC Products Group.  All rights reserved.
 *
 * Smartfusion2 MSS USB Driver Stack
 *      USB Core Interface Layer (USB-CIFL)
 *          USB-CIF driver
 *
 *
 * Register bit offset and mask defintions for Smartfusion2 MSS USB.
 *
 * SVN $Revision: 5468 $
 * SVN $Date: 2013-03-29 15:38:01 +0530 (Fri, 29 Mar 2013) $
 */

#ifndef __MSS_USB_CORE_REGS_H_
#define __MSS_USB_CORE_REGS_H_

/******************************************************************************
 * Power register
 */
#define POWER_REG_ENABLE_SUSPENDM_MASK                  0x01u
#define POWER_REG_SUSPEND_MODE_MASK                     0x02u
#define POWER_REG_RESUME_SIGNAL_MASK                    0x04u
#define POWER_REG_BUS_RESET_SIGNAL_MASK                 0x08u
#define POWER_REG_HS_MODE_MASK                          0x10u
#define POWER_REG_ENABLE_HS_MASK                        0x20u
#define POWER_REG_SOFT_CONN_MASK                        0x40u
#define POWER_REG_ISO_UPDATE_MASK                       0x80u

/******************************************************************************
 * Soft_reset_mask
 */
#define SOFT_RESET_REG_MASK                             0x03u

/******************************************************************************
 * DevCTL register bit masks
 */
#define DEV_CTRL_SESSION_MASK                           0x01u
#define DEV_CTRL_HOST_REQ_MASK                          0x02u
#define DEV_CTRL_HOST_MODE_MASK                         0x04u
#define DEV_CTRL_VBUS_MASK                              0x18u
#define DEV_CTRL_LS_DEV_MASK                            0x20u
#define DEV_CTRL_FS_DEV_MASK                            0x40u
#define DEV_CTRL_B_DEVICE_MASK                          0x80u

#define VBUS_BELOW_SESSION_END                          0x00u
#define VBUS_ABOVE_SESSION_END                          0x08u
#define VBUS_ABOVE_AVALID                               0x10u
#define VBUS_ABOVE_VBUS_VALID                           0x18u

/******************************************************************************
 * CSR0L bit masks (peripheral mode)
 */
#define CSR0L_DEV_RX_PKT_RDY_MASK                       0x0001u
#define CSR0L_DEV_TX_PKT_RDY_MASK                       0x0002u
#define CSR0L_DEV_STALL_SENT_MASK                       0x0004u
#define CSR0L_DEV_DATA_END_MASK                         0x0008u
#define CSR0L_DEV_SETUP_END_MASK                        0x0010u
#define CSR0L_DEV_SEND_STALL_MASK                       0x0020u
#define CSR0L_DEV_SERVICED_RX_PKT_RDY_MASK              0x0040u
#define CSR0L_DEV_SERVICED_SETUP_END_MASK               0x0080u

/******************************************************************************
 * CSR0H bit masks (peripheral mode)
 */
#define CSR0H_DEV_FLUSH_FIFO_MASK                       0x0100u

/******************************************************************************
 * COUNT0 register masks
 */
#define COUNT0_REG_MASK                                 0x7fu

/******************************************************************************
 * Endpoint TxMAXP register bit masks
 */
#define TX_MAX_P_REG_NUM_USB_PKT_SHIFT                  11u

/******************************************************************************
 * Endpoint TxCSRL register bit masks
 */
#define TxCSRL_REG_EPN_TX_PKT_RDY_MASK                  0x0001u
#define TxCSRL_REG_EPN_TX_FIFO_NE_MASK                  0x0002u
#define TxCSRL_REG_EPN_UNDERRUN_MASK                    0x0004u
#define TxCSRL_REG_EPN_FLUSH_FIFO_MASK                  0x0008u
#define TxCSRL_REG_EPN_SEND_STALL_MASK                  0x0010u
#define TxCSRL_REG_EPN_STALL_SENT_MASK                  0x0020u
#define TxCSRL_REG_EPN_CLR_DATA_TOG_MASK                0x0040u
#define TxCSRL_REG_EPN_ISO_INCOMP_TX_MASK               0x0080u

/******************************************************************************
 * Endpoint TxCSRH register bit masks
 */
/*D0,D1 are un-used*/
#define TxCSRH_REG_EPN_DMA_MODE_MASK                    0x0400u
#define TxCSRH_REG_EPN_FRC_DAT_TOG_MASK                 0x0800u
#define TxCSRH_REG_EPN_ENABLE_DMA_MASK                  0x1000u
#define TxCSRH_REG_EPN_TXRX_MODE_MASK                   0x2000u
#define TxCSRH_REG_EPN_ENABLE_ISO_MASK                  0x4000u
#define TxCSRH_REG_EPN_ENABLE_AUTOSET_MASK              0x8000u

/******************************************************************************
 * Endpoint TxMAXP register bit masks
 */
#define RX_MAX_P_REG_NUM_USB_PKT_SHIFT                  11u

/******************************************************************************
 * Endpoint RxCSRL register bit masks
 */
#define RxCSRL_REG_EPN_RX_PKT_RDY_MASK                  0x0001u
#define RxCSRL_REG_EPN_RX_FIFO_FULL_MASK                0x0002u
#define RxCSRL_REG_EPN_OVERRUN_MASK                     0x0004u
#define RxCSRL_REG_EPN_DATA_ERR_MASK                    0x0008u
#define RxCSRL_REG_EPN_FLUSH_FIFO_MASK                  0x0010u
#define RxCSRL_REG_EPN_SEND_STALL_MASK                  0x0020u
#define RxCSRL_REG_EPN_STALL_SENT_MASK                  0x0040u
#define RxCSRL_REG_EPN_CLR_DAT_TOG_MASK                 0x0080u

/******************************************************************************
 * Endpoint RxCSRH register bit masks
 */
#define RxCSRL_REG_EPN_RX_ISO_INCOMP                    0x0100u
/*D1,D2 are unused*/
#define RxCSRL_REG_EPN_DMA_MODE_MASK                    0x0800u

#define RxCSRL_REG_EPN_ISO_PID_ERR_MASK                 0x1000u
#define RxCSRL_REG_EPN_BI_DIS_NYET_MASK                 0x1000u

#define RxCSRL_REG_EPN_ENABLE_DMA_MASK                  0x2000u
#define RxCSRL_REG_EPN_ENABLE_ISO_MASK                  0x4000u
#define RxCSRL_REG_EPN_ENABLE_AUTOCLR_MASK              0x8000u


/******************************************************************************
 * Endpoint DMA_CNTL register bit masks
 */
#define DMA_CNTL_REG_START_XFR_MASK                     0x00000001u
#define DMA_CNTL_REG_DMA_DIR_MASK                       0x00000002u
#define DMA_CNTL_REG_DMA_MODE_MASK                      0x00000004u
#define DMA_CNTL_REG_ENABLE_DMA_IRQ_MASK                0x00000008u
#define DMA_CNTL_REG_DMA_EP_NUM_MASK                    0x000000F0u
#define DMA_CNTL_REG_DMA_BUS_ERR_MASK                   0x00000100u
#define DMA_CNTL_REG_DMA_BURST_MODE_MASK                0x00000600u

#define DMA_CNTL_REG_DMA_BURST_MODE_SHIFT               9u
#define DMA_CNTL_REG_DMA_EP_NUM_SHIFT                   4u
#define DMA_CNTL_REG_DMA_DIR_SHIFT                      1u
#define DMA_CNTL_REG_DMA_MODE_SHIFT                     2u

/******************************************************************************
 * TX Endpoint Fifo size masks
 */
#define TXFIFOSZ_REG_DPB_SHIFT                          4u

/******************************************************************************
 * RX Endpoint Fifo size masks
 */
#define RXFIFOSZ_REG_DPB_SHIFT                          4u

/******************************************************************************
 * TX_IRQ_ENABLE register masks
 */
#define TX_IRQ_ENABLE_REG_CEP_MASK                      0x0001u


/******************************************************************************
* Host Side register definitions
*/

/******************************************************************************
 * CSR0L bit masks
 */
#define CSR0L_HOST_RX_PKT_RDY_MASK                      0x0001u
#define CSR0L_HOST_TX_PKT_RDY_MASK                      0x0002u
#define CSR0L_HOST_STALL_RCVD_MASK                      0x0004u
#define CSR0L_HOST_SETUP_PKT_MASK                       0x0008u
#define CSR0L_HOST_RETRY_ERR_MASK                       0x0010u
#define CSR0L_HOST_IN_PKT_REQ_MASK                      0x0020u
#define CSR0L_HOST_STATUS_PKT_MASK                      0x0040u
#define CSR0L_HOST_NAK_TIMEOUT_MASK                     0x0080u

/******************************************************************************
 * CSR0H bit masks
 */
#define CSR0H_HOST_FLUSH_FIFO_MASK                      0x0100u     /*Self Clearing*/
#define CSR0H_HOST_DATA_TOG_MASK                        0x0200u
#define CSR0H_HOST_DATA_TOG_WE_MASK                     0x0400u     /*Self Clearing*/
#define CSR0H_HOST_DISABLE_PING_MASK                    0x0800u

/******************************************************************************
* Type0 register bit masks
*/
#define TYPE0_HOST_MP_TARGET_SPEED_MASK                 0xC0u

#define TYPE0_HOST_MP_TARGET_SPEED_HIGH                 0x40u
#define TYPE0_HOST_MP_TARGET_SPEED_FULL                 0x80u
#define TYPE0_HOST_MP_TARGET_SPEED_LOW                  0xC0u
#define TYPE0_HOST_MP_TARGET_SPEED_SELF                 0x00u

#define TYPE0_HOST_MP_TARGET_SPEED_SHIFT                6u

/******************************************************************************
* NAKLIMIT0 register bit masks
*/
#define NAKLIMIT0_REG_MASK                              0x00u

/******************************************************************************
 * Endpoint TxCSRL register bit masks
 */
#define TxCSRL_HOST_EPN_TX_PKT_RDY_MASK                 0x0001u
#define TxCSRL_HOST_EPN_TX_FIFO_NE_MASK                 0x0002u
#define TxCSRL_HOST_EPN_RESPONSE_ERR_MASK               0x0004u
#define TxCSRL_HOST_EPN_FLUSH_FIFO_MASK                 0x0008u
#define TxCSRL_HOST_EPN_SETUP_PKT_MASK                  0x0010u
#define TxCSRL_HOST_EPN_STALL_RCVD_MASK                 0x0020u
#define TxCSRL_HOST_EPN_CLR_DATA_TOG_MASK               0x0040u
#define TxCSRL_HOST_EPN_NAK_TIMEOUT_MASK                0x0080u

/******************************************************************************
 * Endpoint TxCSRH register bit masks
 */
#define TxCSRH_HOST_EPN_DATA_TOG_MASK                   0x0100u
#define TxCSRH_HOST_EPN_DATA_TOG_WE_MASK                0x0200u
#define TxCSRH_HOST_EPN_DMA_MODE_MASK                   0x0400u
#define TxCSRH_HOST_EPN_FRC_DATA_TOG_MASK               0x0800u
#define TxCSRH_HOST_EPN_ENABLE_DMA_MASK                 0x1000u
#define TxCSRH_HOST_EPN_TXRX_MODE_MASK                  0x2000u
/*D6 is unused*/
#define TxCSRH_HOST_EPN_ENABLE_AUTOSET_MASK             0x8000u

/******************************************************************************
 * Endpoint RxCSRL register bit masks
 */
#define RXCSRL_HOST_EPN_RX_PKT_RDY_MASK                 0x0001u
#define RXCSRL_HOST_EPN_RX_FIFO_FULL_MASK               0x0002u
#define RXCSRL_HOST_EPN_RESPONSE_ERR_MASK               0x0004u
#define RXCSRL_HOST_EPN_NAK_TIMEOUT_ERR_MASK            0x0008u
#define RXCSRL_HOST_EPN_FLUSH_FIFO_MASK                 0x0010u
#define RXCSRL_HOST_EPN_IN_PKT_REQ_MASK                 0x0020u
#define RXCSRL_HOST_EPN_STALL_RCVD_MASK                 0x0040u
#define RXCSRL_HOST_EPN_CLR_DATA_TOG_MASK               0x0080u

/******************************************************************************
 * Endpoint RxCSRH register bit masks
 */
#define RXCSRH_HOST_EPN_RX_ISO_INCOMP                   0x0100u
#define RXCSRH_HOST_EPN_DATA_TOG_MASK                   0x0200u
#define RXCSRH_HOST_EPN_DATA_TOG_WE_MASK                0x0400u
#define RXCSRH_HOST_EPN_DMA_MODE_MASK                   0x0800u
#define RXCSRH_HOST_EPN_PID_ERR_MASK                    0x1000u
#define RXCSRH_HOST_EPN_ENABLE_DMA_MASK                 0x2000u
#define RXCSRH_HOST_EPN_ENABLE_AUTOREQ_MASK             0x4000u
#define RXCSRH_HOST_EPN_ENABLE_AUTOCLR_MASK             0x8000u


/******************************************************************************
* TXType register bit masks
*/

#define TXTYPE_HOST_TARGET_EP_NUM_MASK                  0x0Fu
#define TXTYPE_HOST_TARGET_EP_PROTOCOL_MASK             0x30u
#define TXTYPE_HOST_TARGET_EP_SPEED_MASK                0xC0u

#define TXTYPE_HOST_TARGET_EP_NUM_SHIFT                 0u
#define TXTYPE_HOST_TARGET_EP_PROTOCOL_SHIFT            4u
#define TXTYPE_HOST_TARGET_EP_SPEED_SHIFT               6u


/******************************************************************************
*  TXINTERVAL register bit masks
*/
#define TXINTERVAL_HOST_REG_MASK                        0x00

/******************************************************************************
* TXType register bit masks
*/

#define RXTYPE_HOST_TARGET_EP_NUM_MASK                  0x0Fu
#define RXTYPE_HOST_TARGET_EP_PROTOCOL_MASK             0x30u
#define RXTYPE_HOST_TARGET_EP_SPEED_MASK                0xC0u

#define RXTYPE_HOST_TARGET_EP_NUM_SHIFT                 0u
#define RXTYPE_HOST_TARGET_EP_PROTOCOL_SHIFT            4u
#define RXTYPE_HOST_TARGET_EP_SPEED_SHIFT               6u

/******************************************************************************
*  RXINTERVAL register bit masks
*/
#define RXINTERVAL_HOST_REG_MASK                        0x00u

/******************************************************************************
*  TX/RXFUNCTIONADDR register bit masks
*/
#define TARGET_DEVICE_ADDR_MASK                         0x7Fu

/******************************************************************************
*  TX/RXHUBADDR register bit masks
*/
#define TARGET_DEVICE_HUB_ADDR_MASK                     0x7Fu
#define TARGET_DEVICE_HUB_MT_MASK                       0x10u

#define TARGET_DEVICE_HUB_MT_SHIFT                      7u

/******************************************************************************
*  TX/RXHUBPORT register bit masks
*/
#define TARGET_DEVICE_HUB_PORT_MASK                     0x7Fu

#endif /*__MSS_USB_CORE_REGS_H_*/
