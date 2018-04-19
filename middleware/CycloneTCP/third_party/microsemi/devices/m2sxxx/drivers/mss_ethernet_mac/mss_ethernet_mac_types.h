/*******************************************************************************
 * (c) Copyright 2011-2013 Microsemi SoC Products Group.  All rights reserved.
 * 
 * This file contains type definitions used throughout SmartFusion2 Ethernet MAC
 * and PHY device drivers. User need not include this file in application source
 * code.
 * Inclusion of mss_ethernet_mac.h inherits these types.
 * 
 * SVN $Revision: 5942 $
 * SVN $Date: 2013-11-06 12:53:12 +0000 (Wed, 06 Nov 2013) $
 */
#ifndef MSS_ETHERNET_MAC_TYPES_H_
#define MSS_ETHERNET_MAC_TYPES_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************/
/* Public type definitions                                                     */
/*******************************************************************************/

/*******************************************************************************
 * MAC interface speed
 * This type definition provides various interface speeds supported by MAC hardware.
 */
typedef enum
{
    MAC10MBPS     = 0x00,
    MAC100MBPS    = 0x01,
    MAC1000MBPS   = 0x02,
    INVALID_SPEED = 0x03
} MAC_speed_t;

/*******************************************************************************
 * MAC interface type.
 * This type definition provides various interface types for PHY connection.
 */
typedef enum
{
    RMII            = 0x00, 
    RGMII           = 0x01,
    TBI             = 0x02,
    MII             = 0x03,
    GMII            = 0x04,
    INVALID_INF
} MAC_interface_t;

/*******************************************************************************
 * MAC Configuration Structure.
 * You need to create a record of this type to hold the configuration of the MAC.
 * The MSS_MAC_cfg_struct_def_init() API should be used to initialize the
 * configuration record to default values. Later, the configuration elements
 * in the record can be changed to desired values.
 */
typedef struct
{
    uint32_t speed_duplex_select;       /* Link speed and duplex mode allowed to setup a link. */
    uint8_t mac_addr[6];                /* Station's MAC address */
    uint8_t phy_addr;                   /* Address of Ethernet PHY on MII management interface. */
    MAC_interface_t interface;          /* PHY interface type: RMII/ RGMII/ MII /GMII / TBI for SGMII */
    uint8_t tx_edc_enable;              /* Enable / disable error detection and correction for tx FIFOs */
    uint8_t rx_edc_enable;              /* Enable / disable error detection and correction for rx FIFOs */
    uint8_t preamble_length;            /* 4-bit Length of preamble field: default value is 0x7 */
    uint8_t hugeframe_enable;           /* Enable / disable huge frame support: default is disable 0 */
    uint8_t length_field_check;         /* Enable / disable length field checking */
    uint8_t pad_n_CRC;                  /* Enable / disable padding and appending CRC */
    uint8_t append_CRC;                 /* Enable / disable appending CRC */
    uint8_t fullduplex;                 /* Enable / disable full duplex: default is disable 0 */
    uint8_t loopback;                   /* Enable / disable loopback mode: default is disable 0 */
    uint8_t rx_flow_ctrl;               /* Enable / disable receiver flow control: default is disable 0 */
    uint8_t tx_flow_ctrl;               /* Enable / disable transmitter flow control: default is disable 0 */
    uint8_t min_IFG;                    /* 8-bit minimum inter-frame gap value */
    uint8_t btb_IFG;                    /* 7-bit back to back inter-frame gap value */
    uint8_t max_retx_tries;             /* 5-bit maximum retransmission tries value: default is 0xF */
    uint8_t excessive_defer;            /* Enable / disable transmission of packet that exceeded max collisions: default is disable 0 */
    uint8_t nobackoff;                  /* Enable / disable back off. default is disable 0 */
    uint8_t backpres_nobackoff;         /* Enable / disable back off in back pressure mode: default is disable 0 */
    uint8_t ABEB_enable;                /* Enable / disable arbitrary binary exponential back-off: default is disable 0 */
    uint8_t ABEB_truncvalue;            /* 4-bit alternative binary exponential back-off value: default is 0xA */
    uint8_t phyclk;                     /* 3-bit MGMT clock divider value */
    uint8_t supress_preamble;           /* Enable / disable preamble suppression at PHY: default is disable 0 */
    uint8_t autoscan_phys;              /* Enable / disable auto scanning of PHYs with programmed addresses: default is disable 0 */
    uint16_t max_frame_length;          /* Maximum frame length: default value is 0x0600(1536d) */
    uint16_t non_btb_IFG;               /* 14-bit non back to back inter-frame gap value */
    uint16_t slottime;                  /* 10-bit collision window value : default is 0x37 */
    uint32_t framedrop_mask;            /* 18-bit mask to drop frames based on receive statistics */
} MAC_cfg_t;

/*******************************************************************************
 * Transmit and Receive statistics.
 * Statistic of below type, which is desired to be read is to be passed to the
 * function MSS_MAC_read_stat(). The width of returned statistic value is 
 * mentioned in the comment against the statistic.
 */
typedef enum
{
    FRAME_CNT_64,                 /* 18-bit */
    FRAME_CNT_127,                /* 18-bit */
    FRAME_CNT_255,                /* 18-bit */
    FRAME_CNT_511,                /* 18-bit */
    FRAME_CNT_1K,                 /* 18-bit */
    FRAME_CNT_MAX,                /* 18-bit */
    FRAME_CNT_VLAN,               /* 18-bit */
    RX_BYTE_CNT,                  /* 24-bit */
    RX_PKT_CNT,                   /* 18-bit */
    RX_FCS_ERR_CNT,               /* 12-bit */
    RX_MULTICAST_PKT_CNT,         /* 18-bit */
    RX_BROADCAST_PKT_CNT,         /* 22-bit */
    RX_CTRL_PKT_CNT,              /* 18-bit */
    RX_PAUSE_PKT_CNT,             /* 12-bit */
    RX_UNKNOWN_OPCODE_CNT,        /* 12-bit */
    RX_ALIGN_ERR_CNT,             /* 12-bit */
    RX_FRAMELENGTH_ERR_CNT,       /* 16-bit */
    RX_CODE_ERR_CNT,              /* 12-bit */
    RX_CS_ERR_CNT,                /* 12-bit */
    RX_UNDERSIZE_PKT_CNT,         /* 12-bit */
    RX_OVERSIZE_PKT_CNT,          /* 12-bit */
    RX_FRAGMENT_CNT,              /* 12-bit */
    RX_JABBER_CNT,                /* 12-bit */
    RX_DROP_CNT,                  /* 12-bit */
    TX_BYTE_CNT,                  /* 24-bit */
    TX_PKT_CNT,                   /* 18-bit */
    TX_MULTICAST_PKT_CNT,         /* 18-bit */
    TX_BROADCAST_PKT_CNT,         /* 18-bit */
    TX_PAUSE_PKT_CNT,             /* 12-bit */
    TX_DEFFERAL_PKT_CNT,          /* 12-bit */
    TX_EXCS_DEFFERAL_PKT_CNT,     /* 12-bit */
    TX_SINGLE_COLL_PKT_CNT,       /* 12-bit */
    TX_MULTI_COLL_PKT_CNT,        /* 12-bit */
    TX_LATE_COLL_PKT_CNT,         /* 12-bit */
    TX_EXCSS_COLL_PKT_CNT,        /* 12-bit */
    TX_TOTAL_COLL_PKT_CNT,        /* 13-bit */
    TX_PAUSE_HONORED_CNT,         /* 12-bit */
    TX_DROP_CNT,                  /* 12-bit */
    TX_JABBER_CNT,                /* 12-bit */
    TX_FCS_ERR_CNT,               /* 12-bit */
    TX_CNTRL_PKT_CNT,             /* 12-bit */
    TX_OVERSIZE_PKT_CNT,          /* 12-bit */
    TX_UNDERSIZE_PKT_CNT,         /* 12-bit */
    TX_FRAGMENT_CNT,              /* 12-bit */
    MAC_LAST_STAT
} MAC_stat_t;

/*******************************************************************************
 * 
 */
typedef void (*MAC_transmit_callback_t)(void * p_user_data);

/*******************************************************************************
 * 
 */
typedef void (*MAC_receive_callback_t)(uint8_t * p_rx_packet,
                                       uint32_t pckt_length,
                                       void * p_user_data);


#ifdef __cplusplus
}
#endif

#endif /* MSS_ETHERNET_MAC_TYPES_H_ */


