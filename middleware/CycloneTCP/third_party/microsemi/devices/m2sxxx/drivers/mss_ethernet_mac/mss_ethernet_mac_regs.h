/*******************************************************************************
 * (c) Copyright 2011-2013 Microsemi SoC Products Group.  All rights reserved.
 * 
 * Register bit offsets and masks defintions for G4Main MAC.
 * 
 * SVN $Revision: 5883 $
 * SVN $Date: 2013-10-10 16:32:33 +0100 (Thu, 10 Oct 2013) $
 */
#ifndef MSS_ETHERNET_MAC_REGS_H_
#define MSS_ETHERNET_MAC_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
 * G4M MAC Configuration system register 
 * Speed and interface configuration fields.
 */
#define MAC_CONFIG_SPEED              0u
#define MAC_CONFIG_INTF               2u
#define MAC_CONFIG_SPEED_MASK         ((uint32_t)0x03)
#define MAC_CONFIG_INTF_MASK          (7u << 2) 

/*------------------------------------------------------------------------------
 * G4M MAC Error detection and correction enable system register 
 * Fields of EDAC_ENABLE system register.
 */
#define MAC_EDAC_TX_EN                3u
#define MAC_EDAC_RX_EN                4u

/*------------------------------------------------------------------------------
 * G4M MAC Error detection and correction interrupt enable system register 
 * Fields of EDAC_INT_ENABLE system register.
 */
#define MAC_EDAC_TX_1E_EN             6u
#define MAC_EDAC_TX_2E_EN             7u
#define MAC_EDAC_RX_1E_EN             8u
#define MAC_EDAC_RX_2E_EN             9u

/*------------------------------------------------------------------------------
 * G4M MAC Error detection and correction counter system registers
 * Contain 1bit EDAC count (15:0) and 2bit EDAC count (31:16).
 * Masks for 1bit and 2bit counts
 */
#define MAC_1B_EDAC_TX_CNT             0xFFFFu
#define MAC_2B_EDAC_TX_CNT             (0xFFFFu << 16)
#define MAC_1B_EDAC_RX_CNT             0xFFFFu
#define MAC_2B_EDAC_RX_CNT             (0xFFFFu << 16)

/*------------------------------------------------------------------------------
 * G4M MAC Error detection and correction address system registers
 * Contain 1bit EDAC address (12:0) and 2bit EDAC address (26:13).
 * Masks for 1bit and 2bit counts
 */
#define MAC_EDAC_RX_1E_AD              0x1FFFu
#define MAC_EDAC_RX_2E_AD              (0x1FFFu << 13)
#define MAC_EDAC_TX_1E_AD              0x1FFFu
#define MAC_EDAC_TX_2E_AD              (0x1FFFu << 13)

/*------------------------------------------------------------------------------
 * G4M MAC Error detection and correction status system registers
 * Fields of EDAC_STATUS system register.
  */
#define MAC_EDAC_TX_1E_STATUS          6u
#define MAC_EDAC_TX_2E_STATUS          7u
#define MAC_EDAC_RX_1E_STATUS          8u
#define MAC_EDAC_RX_2E_STATUS          9u

/*------------------------------------------------------------------------------
 * G4M MAC Error detection and correction counter clear system registers
 * Fields of CLR_EDAC_COUNTERS system register.
  */
#define MAC_EDAC_TX_CNTCLR_1E          6u
#define MAC_EDAC_TX_CNTCLR_2E          7u
#define MAC_EDAC_RX_CNTCLR_1E          8u
#define MAC_EDAC_RX_CNTCLR_2E          9u


/*------------------------------------------------------------------------------
 * MAC Configuration register 1
 */
#define CFG1_SOFT_RST               31u
#define CFG1_RXCTL_RST              19u
#define CFG1_TXCTL_RST              18u
#define CFG1_RX_RST                 17u
#define CFG1_TX_RST                 16u
#define CFG1_LOOPBACK               8u
#define CFG1_RX_FCTL                5u
#define CFG1_TX_FCTL                4u
#define CFG1_RX_EN                  2u
#define CFG1_TX_EN                  0u
#define CFG1_SOFT_RST_MASK          ((uint32_t)1u << CFG1_SOFT_RST)
#define CFG1_RXCTL_RST_MASK         ((uint32_t)1u << CFG1_RXCTL_RST)
#define CFG1_TXCTL_RST_MASK         ((uint32_t)1u << CFG1_TXCTL_RST)
#define CFG1_CFG1_RX_RST_MASK       ((uint32_t)1u << CFG1_RX_RST)
#define CFG1_CFG1_TX_RST_MASK       ((uint32_t)1u << CFG1_TX_RST)
#define CFG1_LOOPBACK_MASK          ((uint32_t)1u << CFG1_LOOPBACK)
#define CFG1_RX_FCTL_MASK           ((uint32_t)1u << CFG1_RX_FCTL)
#define CFG1_TX_FCTL_MASK           ((uint32_t)1u << CFG1_TX_FCTL)
#define CFG1_RX_EN_MASK             ((uint32_t)1u << CFG1_RX_EN)
#define CFG1_TX_EN_MASK             ((uint32_t)1u)

/*------------------------------------------------------------------------------
 * MAC Configuration register 2
 */
#define CFG2_FDX                    0u
#define CFG2_CRC_EN                 1u
#define CFG2_PAD_CRC_EN             2u
#define CFG2_LEN_CHECK              4u
#define CFG2_HUGE_FRAME_EN          5u
#define CFG2_IF_1000                9u
#define CFG2_IF_10_100              8u
#define CFG2_PREAM_LEN              12u
#define CFG2_FDX_MASK               ((uint32_t)1u)
#define CFG2_CRC_EN_MASK            ((uint32_t)1u << 1)
#define CFG2_PAD_CRC_EN_MASK        ((uint32_t)1u << 2)
#define CFG2_LEN_CHECK_MASK         ((uint32_t)1u << 4)
#define CFG2_HUGE_FRAME_EN_MASK     ((uint32_t)1u << 5)
#define CFG2_BYTE_MASK              ((uint32_t)1u << 9)
#define CFG2_NIBBLE_MASK            ((uint32_t)1u << 8)

/*------------------------------------------------------------------------------
 * MAC IPG/IFG register
 */
#define IFG_BTBIPG                  0u
#define IFG_MINIFGENF               8u
#define IFG_NONBTBIPG               16u

/*------------------------------------------------------------------------------
 * MAC half-duplex register
 */
#define HALF_DUPLEX_SLOTTIME                  0u
#define HALF_DUPLEX_RETX_MAX_OFFSET           12u
#define HALF_DUPLEX_EXCS_DEFER_MASK           ((uint32_t)1u<<16)
#define HALF_DUPLEX_NO_BACKOFF_MASK           ((uint32_t)1u<<17)
#define HALF_DUPLEX_BACKPRES_NOBACKOFF_MASK   ((uint32_t)1u<<18)
#define HALF_DUPLEX_ABEB_ENABLE_MASK          ((uint32_t)1u<<19)
#define HALF_DUPLEX_ABEB_TUNC_OFFSET          20u

#define HALF_DUPLEX_SLOTTIME_MASK             ((uint32_t)0x2FF)

/*------------------------------------------------------------------------------
 * MAC MII Management configuration register
 */
#define MII_CLOCK_SELECT_MASK                 ((uint32_t)7u)
#define MII_PREAM_SUPRESS_MASK                ((uint32_t)1u << 4)
#define MII_SCAN_AUTO_INC_MASK                ((uint32_t)1u << 5)
#define MII_RESET_MII_MGMT_MASK               ((uint32_t)1u << 31)

/*------------------------------------------------------------------------------
 * MAC MII Management address register
 */
#define MII_PHY_ADDR                    8u
#define MII_PHY_ADDR_MASK               (0x1Fu<<8)
#define MII_INTF_RESET                  31u

/*------------------------------------------------------------------------------
 * MAC MII Management command register
 */
#define MII_READ                        0u
#define MII_SCAN                        1u

/*------------------------------------------------------------------------------
 * MAC MII Management indicators register
 */
#define MII_BUSY_INDICATOR_MASK         ((uint32_t)1u)
#define MII_SCAN_INDICATOR_MASK         ((uint32_t)1u << 1)
#define MII_NV_INDICATOR_MASK           ((uint32_t)1u << 2)

/*------------------------------------------------------------------------------
 * DMA Tx control & Rx control registers
 */
#define DMA_TXENABLE                    0u
#define DMA_RXENABLE                    0u

/*------------------------------------------------------------------------------
 * DMA Tx status register
 */
#define DMA_TXPKTSENT                   0u
#define DMA_TXUNDRRUN                   1u
#define DMA_TXBUSERR                    3u

/*------------------------------------------------------------------------------
 * DMA Rx status register
 */
#define DMA_RXPKTRCVD                   0u
#define DMA_RXOVRFLOW                   2u
#define DMA_RXBUSERR                    3u

/*------------------------------------------------------------------------------
 * DMA descriptor definitions
 */
#define DMA_DESC_EMPTY_FLAG_MASK        ((uint32_t)1u << 31u)
#define DMA_DESC_PKT_SIZE_MASK          0xFFFu

/*------------------------------------------------------------------------------
 * DMA descriptor packet size 
 */
#define DMA_PKTCOUNT                    16u
#define DMA_PKTCOUNT_MASK               ((uint32_t)0xFFu << 16)

/*------------------------------------------------------------------------------
 * FIFO control registers definitions
 */
#define FIFO_CFG0_WMM_RST               0u
#define FIFO_CFG0_RSYS_RST              1u
#define FIFO_CFG0_RFAB_RST              2u
#define FIFO_CFG0_TSYS_RST              3u
#define FIFO_CFG0_TFAB_RST              4u

#define FIFO_CFG0_WMM_RST_MASK          ((uint32_t)1u)
#define FIFO_CFG0_RSYS_RST_MASK         ((uint32_t)1u << FIFO_CFG0_RSYS_RST)
#define FIFO_CFG0_RFAB_RST_MASK         ((uint32_t)1u << FIFO_CFG0_RFAB_RST)
#define FIFO_CFG0_TSYS_RST_MASK         ((uint32_t)1u << FIFO_CFG0_TSYS_RST)
#define FIFO_CFG0_TFAB_RST_MASK         ((uint32_t)1u << FIFO_CFG0_TFAB_RST)

#define FIFO_CFG0_WMM_ENABLE            ((uint32_t)1u << 8)
#define FIFO_CFG0_RSYS_ENABLE           ((uint32_t)1u << 9)
#define FIFO_CFG0_RFAB_ENABLE           ((uint32_t)1u << 10)
#define FIFO_CFG0_TSYS_ENABLE           ((uint32_t)1u << 11)
#define FIFO_CFG0_TFAB_ENABLE           ((uint32_t)1u << 12)

#define FIFO_CFG5_CFGHDPLX_MASK         ((uint32_t)1u << 22)

#define FIFO_CFG1_DEFVAL                0x0FFF0000u
#define FIFO_CFG2_DEFVAL                0x04000180u
#define FIFO_CFG3_DEFVAL                0x0258FFFFu
#define FIFO_CFG4_DEFVAL                0x0000FFFFu
#define FIFO_CFG5_DEFVAL                0x0007EFEFu

/*------------------------------------------------------------------------------
 * Interface control register definitions
 */
#define INTERFACE_SPEED_MASK            ((uint32_t)1 << 16)

/*------------------------------------------------------------------------------
 * Interface status register definitions
 */
#define INF_PE10T_JABBER                (1u<<0)
#define INF_PE10T_SQE                   ((uint32_t)1 << 1)
#define INF_LOC                         ((uint32_t)1 << 2)
#define INF_LINK_FAIL                   ((uint32_t)1 << 3)
#define INF_SPEED                       ((uint32_t)1 << 4)
#define INF_FULL_DUPLEX                 ((uint32_t)1 << 5)
#define INF_SGMII_LINK_OK               ((uint32_t)1 << 6)
#define INF_SGMII_JABBER                ((uint32_t)1 << 7)
#define INF_SGMII_CLASH                 ((uint32_t)1 << 8)
#define INF_EXCS_DEFER                  ((uint32_t)1 << 9)
#define INF_WOLDTCTD                    ((uint32_t)1 << 10)

/*------------------------------------------------------------------------------
 * Receive Statistics Vectors
 */
#define RSV_UCAD_MASK                   ((uint32_t)1 << 17)

#ifdef __cplusplus                       
}
#endif

#endif /* MSS_ETHERNET_MAC_REGS_H_ */
