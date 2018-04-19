/*******************************************************************************
 * (c) Copyright 2011-2013 Microsemi SoC Products Group.  All rights reserved.
 *
 * SmartFusion2 Microcontroller Subsystem 10/100/1000 Mbps Ethernet MAC bare
 * metal software driver implementation.
 *
 * SVN $Revision: 5942 $
 * SVN $Date: 2013-11-06 12:53:12 +0000 (Wed, 06 Nov 2013) $
 */
#include "../../CMSIS/m2sxxx.h"
#include "../../CMSIS/mss_assert.h"
#include "../../CMSIS/hw_reg_io.h"
#include "mss_ethernet_mac_regs.h"
#include "mss_ethernet_mac.h"
#include "phy.h"
#include "mss_ethernet_mac_user_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/
/* Preprocessor Macros                                                    */
/**************************************************************************/
#define NULL_POINTER    0

#if !defined(NDEBUG)
#define IS_STATE(x)             ( ((x) == MSS_MAC_ENABLE) || ((x) == MSS_MAC_DISABLE) )

#define IS_WORD_ALIGNED(x)      ((uint32_t)0 == ((uint32_t)(x) & (uint32_t)3))
#endif  /* NDEBUG */

#define SF2_MSGMII_PHY_ADDR     0x1Eu

#define INVALID_INDEX           (-1)

#define DMA_TX_DISABLED     0u
#define DMA_TX_ENABLED      1u

/*******************************************************************************
 * DMA Descriptor
 * Driver creates and manages two descriptor rings for transmission and reception.
 */
typedef struct MAC_tx_desc MAC_tx_desc_t;
typedef struct MAC_rx_desc MAC_rx_desc_t;

struct MAC_tx_desc
{
    uint8_t const * pkt_start_addr; /* Packet start address */
    volatile uint32_t pkt_size;     /* Packet size & Per packet override flags */
    MAC_tx_desc_t * next_desriptor; /* Link to next descriptor */
    int32_t index;                  /* Index: helps in handling interrupts */
    void * caller_info;             /* Pointer to user specific data */
};

struct MAC_rx_desc
{
    uint8_t * pkt_start_addr;       /* Packet start address */
    volatile uint32_t pkt_size;     /* Packet size & Per packet override flags */
    MAC_rx_desc_t * next_desriptor; /* Link to next descriptor */
    int32_t index;                  /* Index: helps in handling interrupts */
    void * caller_info;             /* Pointer to user specific data */
};

/*******************************************************************************
 * SmartFusion2 Ethernet MAC instance
 * A local record of this type "g_mac" will be created and maintained by the
 * driver.
 */
typedef struct
{
    MAC_tx_desc_t               tx_desc_tab[MSS_MAC_TX_RING_SIZE]; /* Transmit descriptor table */
    MAC_rx_desc_t               rx_desc_tab[MSS_MAC_RX_RING_SIZE]; /* Receive descriptor table */
    MAC_transmit_callback_t     tx_complete_handler;
    MAC_receive_callback_t      pckt_rx_callback;
    int16_t                     nb_available_tx_desc;
    int16_t                     first_tx_index;
    int16_t                     last_tx_index;
    int16_t                     next_tx_index;
    int16_t                     nb_available_rx_desc;
    int16_t                     next_free_rx_desc_index;
    int16_t                     first_rx_desc_index;
} MAC_instance_t;

/*******************************************************************************
 * MAC interrupt definitions
 */
#define MSS_MAC_TXPKTSENT_IRQ    0u
#define MSS_MAC_RXPKTRCVD_IRQ    4u

#define MSS_MAC_TXPKTSENT_IRQ_MASK      0x01u
#define MSS_MAC_RXPKTRCVD_IRQ_MASK      ((uint32_t)0x01u << MSS_MAC_RXPKTRCVD_IRQ)

/**************************************************************************/
/* Type defines                                                           */
/**************************************************************************/

/**************************************************************************/
/* Global variables                                                       */
/**************************************************************************/

static MAC_instance_t g_mac;

/**************************************************************************/
/* Private Functions                                                      */
/**************************************************************************/
static void mac_reset(void);
static void config_mac_hw(const MAC_cfg_t * cfg);
static void tx_desc_ring_init(void);
static void rx_desc_ring_init(void);
static void assign_station_addr(const uint8_t mac_addr[6]);
static void rxpkt_handler(void);
static void txpkt_handler(void);
static void update_mac_cfg(void);

/**************************************************************************/
/* Public Functions                                                       */
/**************************************************************************/
void EthernetMAC_IRQHandler(void);

/**************************************************************************//**
 * See mss_ethernet_mac.h for details of how to use this function.
 */
void 
MSS_MAC_init
( 
    MAC_cfg_t * cfg 
)
{
    ASSERT(cfg != NULL_POINTER);

    if(cfg != NULL_POINTER)
    {
        NVIC_DisableIRQ(EthernetMAC_IRQn);
        mac_reset();
        NVIC_ClearPendingIRQ(EthernetMAC_IRQn);
        
        config_mac_hw(cfg);
        
        /* Assign MAC station address */
        assign_station_addr(cfg->mac_addr);

        /* Intialize Tx & Rx descriptor rings */
        tx_desc_ring_init();
        rx_desc_ring_init();

        /* Initialize Tx descriptors related variables. */
        g_mac.first_tx_index = INVALID_INDEX;
        g_mac.last_tx_index = INVALID_INDEX;
        g_mac.next_tx_index = 0;
        g_mac.nb_available_tx_desc = MSS_MAC_TX_RING_SIZE;
        
        /* Initialize Rx descriptors related variables. */
        g_mac.nb_available_rx_desc = MSS_MAC_RX_RING_SIZE;
        g_mac.next_free_rx_desc_index = 0;
        g_mac.first_rx_desc_index = INVALID_INDEX;
        
        /* initialize default interrupt handlers */
        g_mac.tx_complete_handler = NULL_POINTER;
        g_mac.pckt_rx_callback = NULL_POINTER;
        
        /* Initialize PHY interface */
        phy_init(cfg);
        phy_set_link_speed(cfg->speed_duplex_select);
        phy_autonegotiate();
        update_mac_cfg();
        
        /* Enable transmission at MAC level. */
        set_bit_reg32( &MAC->CFG1, CFG1_TX_EN);
        /* Enable reception at MAC level.    */
        set_bit_reg32( &MAC->CFG1, CFG1_RX_EN);
        
        /* Enable Tx Packet interrupt */
        set_bit_reg32(&MAC->DMA_IRQ_MASK, MSS_MAC_TXPKTSENT_IRQ);
        /* Enable RX Packet interrupt */
        set_bit_reg32(&MAC->DMA_IRQ_MASK, MSS_MAC_RXPKTRCVD_IRQ);
    }
}

/**************************************************************************//**
 * 
 */
static void update_mac_cfg(void)
{
    MAC_speed_t speed;
    uint8_t fullduplex;
    uint8_t link_up;
    
    link_up = phy_get_link_status(&speed, &fullduplex);
    if(link_up != 0u)
    {
        /* Reconfigure MAC based on PHY configuration. */
        if(MAC100MBPS == speed)
        {
            MAC->INTERFACE_CTRL |= INTERFACE_SPEED_MASK;
        }
        else
        {
            MAC->INTERFACE_CTRL &= ~INTERFACE_SPEED_MASK;
        }
        
        /* Set byte/nibble mode based on interface type and link speed. */
        if(MAC1000MBPS == speed)
        {
            /* Set interface to byte mode. */
            MAC->CFG2 &= ~(CFG2_BYTE_MASK | CFG2_NIBBLE_MASK);
            MAC->CFG2 |= CFG2_BYTE_MASK;
        }
        else
        {
            /* Set interface to nibble mode. */
            MAC->CFG2 &= ~(CFG2_BYTE_MASK | CFG2_NIBBLE_MASK);
            MAC->CFG2 |= CFG2_NIBBLE_MASK;
        }
        
        if(MAC10MBPS == speed)
        {
            /* Avoid extra divide by 10 of clock for 10Mbps links. */
            speed = MAC100MBPS;
        }
        SYSREG->MAC_CR = (SYSREG->MAC_CR & ~MAC_CONFIG_SPEED_MASK) | (uint32_t)speed;
        
        /* Configure duplex mode */
        if(0u == fullduplex)
        {
            /* half duplex */
            MAC->CFG2 &= ~CFG2_FDX_MASK;
            MAC->FIFO_CFG5 |= FIFO_CFG5_CFGHDPLX_MASK;
        }
        else
        {
            /* full duplex */
            MAC->CFG2 |= CFG2_FDX_MASK;
            MAC->FIFO_CFG5 &= ~FIFO_CFG5_CFGHDPLX_MASK;
        }
    }
}

uint8_t MSS_MAC_get_link_status
(
    MAC_speed_t * speed,
    uint8_t *     fullduplex
)
{
    static MAC_speed_t previous_speed = INVALID_SPEED;
    static uint8_t previous_duplex = 0xAAu;
    MAC_speed_t link_speed;
    uint8_t link_fullduplex;
    uint8_t link_up;
    
    link_up = phy_get_link_status(&link_speed, &link_fullduplex);
    if(link_up != 0u)
    {
        MAC_interface_t interface;
        
        interface = (MAC_interface_t)((SYSREG->MAC_CR >> MAC_CONFIG_INTF) & 0x00000007u);
        
        /*----------------------------------------------------------------------
         * Update MAC configuration if link characteristics changed.
         */
        if(link_speed != previous_speed)
        {
            MAC_speed_t mac_link_speed;
            
            /* Reconfigure MAC based on PHY configuration. */
            if(MAC100MBPS == link_speed)
            {
                MAC->INTERFACE_CTRL |= INTERFACE_SPEED_MASK;
            }
            else
            {
                MAC->INTERFACE_CTRL &= ~INTERFACE_SPEED_MASK;
            }
            
            /* Set byte/nibble mode based on interface type and link speed. */
            if(MAC1000MBPS == link_speed)
            {
                /* Set interface to byte mode. */
                MAC->CFG2 &= ~(CFG2_BYTE_MASK | CFG2_NIBBLE_MASK);
                MAC->CFG2 |= CFG2_BYTE_MASK;
            }
            else
            {
                MAC->CFG2 &= ~(CFG2_BYTE_MASK | CFG2_NIBBLE_MASK);
                MAC->CFG2 |= CFG2_NIBBLE_MASK;
            }
            
            if(TBI == interface)
            {
                mac_link_speed = link_speed;
            }
            else
            {
                if(MAC10MBPS == link_speed)
                {
                    /* Avoid extra divide by 10 of clock for 10Mbps links. */
                    mac_link_speed = MAC100MBPS;
                }
                else
                {
                    mac_link_speed = link_speed;
                }
            }
            
            SYSREG->MAC_CR = (SYSREG->MAC_CR & ~MAC_CONFIG_SPEED_MASK) | (uint32_t)mac_link_speed;
        }
        previous_speed = link_speed;
        
        if(link_fullduplex != previous_duplex)
        {
            /* Configure duplex mode */
            if(0u == link_fullduplex)
            {
                /* half duplex */
                MAC->CFG2 &= ~CFG2_FDX_MASK;
                MAC->FIFO_CFG5 |= FIFO_CFG5_CFGHDPLX_MASK;
            }
            else
            {
                /* full duplex */
                MAC->CFG2 |= CFG2_FDX_MASK;
                MAC->FIFO_CFG5 &= ~FIFO_CFG5_CFGHDPLX_MASK;
            }
        }
        previous_duplex = link_fullduplex;
        
        /*----------------------------------------------------------------------
         * Return current link speed and duplex mode.
         */
        if(speed != NULL_POINTER)
        {
            *speed = link_speed;
        }
        
        if(fullduplex != NULL_POINTER)
        {
            *fullduplex = link_fullduplex;
        }
        
        /*----------------------------------------------------------------------
         * Make sure SGMII interface link is up.
         */
        if(TBI == interface)
        {
            uint16_t phy_reg;
            uint16_t sgmii_link_up;
            
            /* Find out if link is up on SGMII link between MAC and external PHY. */
            phy_reg = MSS_MAC_read_phy_reg(SF2_MSGMII_PHY_ADDR, MII_BMSR);
            sgmii_link_up = phy_reg & BMSR_LSTATUS;
            
            if(0u == sgmii_link_up)
            {
                /* Initiate auto-negotiation on the SGMII link. */
                phy_reg = MSS_MAC_read_phy_reg(SF2_MSGMII_PHY_ADDR, MII_BMCR);
                phy_reg |= BMCR_ANENABLE;
                MSS_MAC_write_phy_reg(SF2_MSGMII_PHY_ADDR, MII_BMCR, phy_reg);
                phy_reg |= BMCR_ANRESTART;
                MSS_MAC_write_phy_reg(SF2_MSGMII_PHY_ADDR, MII_BMCR, phy_reg);
            }
        }
    }
    
    return link_up;
}

/**************************************************************************//**
 * See mss_ethernet_mac.h for details of how to use this function.
 */
void 
MSS_MAC_cfg_struct_def_init
(
    MAC_cfg_t * cfg
)
{
    const uint8_t g_default_mac_address[6] =
    {
        0xC0u, 0xB1u, 0x3Cu, 0x88u, 0x88u, 0x88u
    };

    ASSERT( NULL_POINTER != cfg);
    if(NULL_POINTER != cfg)
    {
        cfg->speed_duplex_select = MSS_MAC_ANEG_ALL_SPEEDS;
        cfg->phy_addr           = 0u;
        cfg->interface          = MII;
        cfg->tx_edc_enable      = MSS_MAC_ERR_DET_CORR_ENABLE;
        cfg->rx_edc_enable      = MSS_MAC_ERR_DET_CORR_ENABLE;
        cfg->preamble_length    = MSS_MAC_PREAMLEN_DEFVAL;
        cfg->hugeframe_enable   = MSS_MAC_HUGE_FRAME_DISABLE;
        cfg->length_field_check = MSS_MAC_LENGTH_FILED_CHECK_ENABLE;
        cfg->pad_n_CRC          = MSS_MAC_PAD_N_CRC_ENABLE;
        cfg->append_CRC         = MSS_MAC_CRC_ENABLE;
        cfg->fullduplex         = MSS_MAC_FULLDUPLEX_ENABLE;
        cfg->loopback           = MSS_MAC_LOOPBACK_DISABLE;
        cfg->rx_flow_ctrl       = MSS_MAC_RX_FLOW_CTRL_ENABLE;
        cfg->tx_flow_ctrl       = MSS_MAC_TX_FLOW_CTRL_ENABLE;
        cfg->min_IFG            = MSS_MAC_MINIFG_DEFVAL;
        cfg->btb_IFG            = MSS_MAC_BTBIFG_DEFVAL;
        cfg->max_retx_tries     = MSS_MAC_MAXRETX_DEFVAL;
        cfg->excessive_defer    = MSS_MAC_EXSS_DEFER_DISABLE;
        cfg->nobackoff          = MSS_MAC_NO_BACKOFF_DISABLE;
        cfg->backpres_nobackoff = MSS_MAC_BACKPRESS_NO_BACKOFF_DISABLE;
        cfg->ABEB_enable        = MSS_MAC_ABEB_DISABLE;
        cfg->ABEB_truncvalue    = MSS_MAC_ABEBTRUNC_DEFVAL;
        cfg->phyclk             = MSS_MAC_DEF_PHY_CLK;
        cfg->supress_preamble   = MSS_MAC_SUPPRESS_PREAMBLE_DISABLE;
        cfg->autoscan_phys      = MSS_MAC_PHY_AUTOSCAN_DISABLE;
        cfg->max_frame_length   = MSS_MAC_MAXFRAMELEN_DEFVAL;
        cfg->non_btb_IFG        = MSS_MAC_NONBTBIFG_DEFVAL;
        cfg->slottime           = MSS_MAC_SLOTTIME_DEFVAL;
        cfg->framedrop_mask     = MSS_MAC_FRAME_DROP_MASK_DEFVAL;
        
        cfg->mac_addr[0] = g_default_mac_address[0];
        cfg->mac_addr[1] = g_default_mac_address[1];
        cfg->mac_addr[2] = g_default_mac_address[2];
        cfg->mac_addr[3] = g_default_mac_address[3];
        cfg->mac_addr[4] = g_default_mac_address[4];
        cfg->mac_addr[5] = g_default_mac_address[5];
    }
}

/**************************************************************************//**
 * 
 */
static void 
mac_reset
(
    void
)
{
    /* Reset MCXMAC TX functionality */
    set_bit_reg32( &MAC->CFG1, CFG1_TX_RST);
    /* Reset MCXMAC RX functionality */
    set_bit_reg32( &MAC->CFG1, CFG1_RX_RST);
    /* Reset MCXMAC TX control */
    set_bit_reg32( &MAC->CFG1, CFG1_TXCTL_RST);
    /* Reset MCXMAC RX control */
    set_bit_reg32( &MAC->CFG1, CFG1_RXCTL_RST);
    /* Reset MCXMAC interface control */
    set_bit_reg32( &MAC->INTERFACE_CTRL, MII_INTF_RESET);
    /* Reset FIFO watermark module */
    set_bit_reg32( &MAC->FIFO_CFG0, FIFO_CFG0_WMM_RST);
    /* Reset FIFO Rx system module */
    set_bit_reg32( &MAC->FIFO_CFG0, FIFO_CFG0_RSYS_RST);
    /* Reset FIFO Rx fab module */
    set_bit_reg32( &MAC->FIFO_CFG0, FIFO_CFG0_RFAB_RST);
    /* Reset FIFO Tx system module */
    set_bit_reg32( &MAC->FIFO_CFG0, FIFO_CFG0_TSYS_RST);
    /* Reset FIFO Tx system module */
    set_bit_reg32( &MAC->FIFO_CFG0, FIFO_CFG0_TFAB_RST);
}

/**************************************************************************//**
 * 
 */
static void config_mac_hw(const MAC_cfg_t * cfg)
{
    uint32_t tempreg;
    
    /* Check for validity of configuration parameters */
    ASSERT( INVALID_INF > cfg->interface );
    ASSERT( IS_STATE(cfg->tx_edc_enable) );
    ASSERT( IS_STATE(cfg->rx_edc_enable) );        
    ASSERT( MSS_MAC_PREAMLEN_MAXVAL >= cfg->preamble_length );
    ASSERT( IS_STATE(cfg->hugeframe_enable) );
    ASSERT( IS_STATE(cfg->length_field_check) );
    ASSERT( IS_STATE(cfg->pad_n_CRC) );
    ASSERT( IS_STATE(cfg->append_CRC) );
    ASSERT( IS_STATE(cfg->loopback) );
    ASSERT( IS_STATE(cfg->rx_flow_ctrl) );
    ASSERT( IS_STATE(cfg->tx_flow_ctrl) );
    ASSERT( MSS_MAC_MINIFG_MAXVAL >= cfg->min_IFG );            
    ASSERT( MSS_MAC_BTBIFG_MAXVAL >= cfg->btb_IFG );           
    ASSERT( MSS_MAC_MAXRETX_MAXVAL >= cfg->max_retx_tries );      
    ASSERT( IS_STATE(cfg->excessive_defer) );    
    ASSERT( IS_STATE(cfg->nobackoff) );         
    ASSERT( IS_STATE(cfg->backpres_nobackoff) );
    ASSERT( IS_STATE(cfg->ABEB_enable) );        
    ASSERT( MSS_MAC_ABEBTRUNC_MAXVAL >= cfg->ABEB_truncvalue );    
    ASSERT( MSS_MAC_BY28_PHY_CLK >= cfg->phyclk );            
    ASSERT( IS_STATE(cfg->supress_preamble) );   
    ASSERT( IS_STATE(cfg->autoscan_phys) );      
    ASSERT( MSS_MAC_MAXFRAMELEN_MAXVAL >= cfg->max_frame_length );   
    ASSERT( MSS_MAC_SLOTTIME_MAXVAL >= cfg->slottime );
    
    /*--------------------------------------------------------------------------
     * Configure MAC FIFOs error detection and correction (EDAC)
     */
    /* Tx FIFO EDAC configuration */
    if(MSS_MAC_ENABLE == cfg->tx_edc_enable)
    {
        set_bit_reg32(&SYSREG->EDAC_CR, MAC_EDAC_TX_EN);
    }
    else
    {
        clear_bit_reg32(&SYSREG->EDAC_CR, MAC_EDAC_TX_EN);
    }
    /* Rx FIFO EDAC configuration */
    if(MSS_MAC_ENABLE == cfg->rx_edc_enable)
    {
        set_bit_reg32(&SYSREG->EDAC_CR,MAC_EDAC_RX_EN);
    }
    else
    {
        clear_bit_reg32(&SYSREG->EDAC_CR,MAC_EDAC_RX_EN);
    }
    
    /*--------------------------------------------------------------------------
     * Configure PHY related MII MGMT registers
     */
    tempreg = (uint32_t)cfg->phyclk & MII_CLOCK_SELECT_MASK;
    if(MSS_MAC_ENABLE == cfg->supress_preamble)
    {
        tempreg |= MII_PREAM_SUPRESS_MASK;
    }
    
    if(MSS_MAC_ENABLE == cfg->autoscan_phys)
    {
        tempreg |= MII_SCAN_AUTO_INC_MASK;
    }
    MAC->MII_CONFIG = tempreg;

    /*--------------------------------------------------------------------------
     * Clear all reset bits
     */              
    /* Clear soft reset for MCXMAC, Tx function, Rx function, Tx MAC control and
     * Rx MAC control. */
    MAC->CFG1 &= ~(CFG1_SOFT_RST_MASK | CFG1_TXCTL_RST_MASK |
                   CFG1_RXCTL_RST_MASK | CFG1_CFG1_TX_RST_MASK |
                   CFG1_CFG1_RX_RST_MASK);
    
    /* Clear MCXMAC interface reset. */
    clear_bit_reg32( &MAC->INTERFACE_CTRL, MII_INTF_RESET);
    
    /* Clear FIFO resets. */
    MAC->FIFO_CFG0 &= ~(FIFO_CFG0_WMM_RST_MASK | FIFO_CFG0_RSYS_RST_MASK |
                        FIFO_CFG0_RFAB_RST_MASK | FIFO_CFG0_TSYS_RST_MASK |
                        FIFO_CFG0_TFAB_RST_MASK);

    /*--------------------------------------------------------------------------
     * configure loppback and flow control enables.
     */
    if(MSS_MAC_ENABLE == cfg->loopback)
    {
        tempreg = CFG1_LOOPBACK_MASK;
    }
    else
    {
        tempreg = 0u;
    }
    
    if(MSS_MAC_ENABLE == cfg->rx_flow_ctrl)
    {
        tempreg |= CFG1_RX_FCTL_MASK;
    }
    
    if(MSS_MAC_ENABLE == cfg->tx_flow_ctrl)
    {
        tempreg |= CFG1_TX_FCTL_MASK;
    }
    MAC->CFG1 = tempreg;

    /*--------------------------------------------------------------------------
     *
     */
    if(MSS_MAC_ENABLE == cfg->fullduplex)
    {
        tempreg = CFG2_FDX_MASK;
    }
    else
    {
        tempreg = 0u;
    }
    
    if(MSS_MAC_ENABLE == cfg->append_CRC)
    {
        tempreg |= CFG2_CRC_EN_MASK;
    }
                                   
    if(MSS_MAC_ENABLE == cfg->pad_n_CRC)
    {
        tempreg |= CFG2_PAD_CRC_EN_MASK;
    }
                                  
    if(MSS_MAC_ENABLE == cfg->length_field_check)
    {
        tempreg |= CFG2_LEN_CHECK_MASK;
    }
                                           
    if(MSS_MAC_ENABLE == cfg->hugeframe_enable)
    {
        tempreg |= CFG2_HUGE_FRAME_EN_MASK;
    }
                                         
    if((MII == cfg->interface) || (RMII == cfg->interface))
    {
        /* MII and RMII use nibble mode interface. */
        tempreg |= CFG2_NIBBLE_MASK;
    }
    else
    {
        /* TBI and GMII use byte interface. */
        tempreg |= CFG2_BYTE_MASK;
    }
    tempreg |= (((uint32_t)cfg->preamble_length ) << CFG2_PREAM_LEN);
    MAC->CFG2 = tempreg;
    
    /*--------------------------------------------------------------------------
     *
     */
    tempreg = cfg->btb_IFG;
    tempreg |= ( (uint32_t)cfg->min_IFG << IFG_MINIFGENF );
    tempreg |= ( (uint32_t)cfg->non_btb_IFG << IFG_NONBTBIPG );
    MAC->IFG = tempreg;
    
    /*--------------------------------------------------------------------------
     *
     */
    tempreg = (uint32_t)cfg->slottime & HALF_DUPLEX_SLOTTIME_MASK;
    
    tempreg |= (uint32_t)cfg->max_retx_tries << HALF_DUPLEX_RETX_MAX_OFFSET;
    
    if(MSS_MAC_ENABLE == cfg->excessive_defer)
    {
        tempreg |= HALF_DUPLEX_EXCS_DEFER_MASK;
    }
                                       
    if(MSS_MAC_ENABLE == cfg->nobackoff)
    {
        tempreg |= HALF_DUPLEX_NO_BACKOFF_MASK;
    }
                                       
    if(MSS_MAC_ENABLE == cfg->backpres_nobackoff)
    {
        tempreg |= HALF_DUPLEX_BACKPRES_NOBACKOFF_MASK;
    }
                                       
    if(MSS_MAC_ENABLE == cfg->ABEB_enable)
    {
        tempreg |= HALF_DUPLEX_ABEB_ENABLE_MASK;
    }
                                       
    tempreg |= (uint32_t)cfg->ABEB_truncvalue << HALF_DUPLEX_ABEB_TUNC_OFFSET;
    MAC->HALF_DUPLEX = tempreg;

    MAC->MAX_FRAME_LENGTH = (uint32_t)cfg->max_frame_length;

    /*--------------------------------------------------------------------------
     * Configure FIFOs
     */
    MAC->FIFO_CFG0 = ( FIFO_CFG0_WMM_ENABLE  | FIFO_CFG0_RSYS_ENABLE |
                       FIFO_CFG0_RFAB_ENABLE | FIFO_CFG0_TSYS_ENABLE |
                       FIFO_CFG0_TFAB_ENABLE );
    /* RX FIFO size : 8KB  */        
    MAC->FIFO_CFG1 = FIFO_CFG1_DEFVAL;
    /* Rx FIFO watermark */
    MAC->FIFO_CFG2 = FIFO_CFG2_DEFVAL;
    /* Tx FIFO watermark: 4KB Tx FIFO */
    MAC->FIFO_CFG3 = FIFO_CFG3_DEFVAL;
    /* Drop junk frames (Frames suffered CRC error or short collision) */
    MAC->FIFO_CFG4 = FIFO_CFG4_DEFVAL;
    /* Drop frames with CRC errors and PAUSE frames */
    MAC->FIFO_CFG5 = FIFO_CFG5_DEFVAL;
    
    /*--------------------------------------------------------------------------
     * Interface type: MII/GMII/RMII/TBI
     */
    SYSREG->MAC_CR = ((uint32_t)cfg->interface) << MAC_CONFIG_INTF;
}

/**************************************************************************//**
 * See mss_ethernet_mac.h for details of how to use this function.
 */
void
MSS_MAC_write_phy_reg
(
    uint8_t phyaddr,
    uint8_t regaddr,
    uint16_t regval
)
{
    ASSERT( MSS_MAC_PHYADDR_MAXVAL >= phyaddr); 
    ASSERT( MSS_MAC_PHYREGADDR_MAXVAL >= regaddr );
    /* 
     * Write PHY address in MII Mgmt address register.
     * Makes previous register address 0 & invalid.
     */ 
    if((MSS_MAC_PHYADDR_MAXVAL >= phyaddr) && 
       (MSS_MAC_PHYREGADDR_MAXVAL >= regaddr))
    {
        /* Wait for MII Mgmt interface to complete previous operation. */
        do {
            ;
        } while(MAC->MII_INDICATORS & MII_BUSY_INDICATOR_MASK);
    
        /* Load PHY address in MII Mgmt address register */
        MAC->MII_ADDRESS = (((uint32_t)phyaddr)<<MII_PHY_ADDR);
        /* Load register address in MII Mgmt address register */
        MAC->MII_ADDRESS |= (uint32_t)regaddr;
        /* Load value to be written in MII Mgmt control register */
        MAC->MII_CTRL = (uint32_t)regval ;
    }
}

/**************************************************************************//**
 * See mss_ethernet_mac.h for details of how to use this function.
 */
uint16_t
MSS_MAC_read_phy_reg
(
    uint8_t phyaddr,
    uint8_t regaddr
)
{
    uint32_t timeout = 100000u;
    uint16_t read_val = 0u;

    ASSERT(MSS_MAC_PHYADDR_MAXVAL >= phyaddr); 
    ASSERT(MSS_MAC_PHYREGADDR_MAXVAL >= regaddr );
    /* 
     * Write PHY address in MII Mgmt address register.
     * Makes previous register address 0 & invalid.
     */ 
    if((MSS_MAC_PHYADDR_MAXVAL >= phyaddr) && 
       (MSS_MAC_PHYREGADDR_MAXVAL >= regaddr))
    {
        uint32_t mii_not_valid;
        uint32_t mii_busy;
        
        /* Wait for MII Mgmt interface to complete previous operation. */
        do {
            ;
        } while((MAC->MII_INDICATORS & MII_BUSY_INDICATOR_MASK) != 0u);
        
        MAC->MII_ADDRESS = (((uint32_t)phyaddr) << MII_PHY_ADDR);
        /* Load PHY register address in MII Mgmt address register */
        MAC->MII_ADDRESS |= (uint32_t)regaddr;
        /* Issue read command to PHY */
        set_bit_reg32( &MAC->MII_COMMAND, MII_READ);
        /* Poll till PHY read cycle is completed or timeout expired */
        do {
            --timeout;
            mii_not_valid = MAC->MII_INDICATORS & MII_NV_INDICATOR_MASK;
            mii_busy = MAC->MII_INDICATORS & MII_BUSY_INDICATOR_MASK;
        } while((timeout > 0u) && ((mii_busy != 0u) || (mii_not_valid != 0u)));

        /* Read value from MII Mgmt status register only if timeout is not expired
           but the read the read cycle is completed */
        ASSERT(timeout > 0u);
        if(timeout != 0u)
        {
            read_val = (uint16_t)MAC->MII_STATUS;
        }
    }
    clear_bit_reg32(&MAC->MII_COMMAND, MII_READ);
    return read_val;
}

/**************************************************************************//**
 * See mss_ethernet_mac.h for details of how to use this function.
 */
uint32_t 
MSS_MAC_read_stat
(
    MAC_stat_t stat
)
{
    uint32_t stat_val = 0u;

    volatile uint32_t * const stat_regs_lut[] =
    {
        &MAC->TR64,     /* FRAME_CNT_64 */        
        &MAC->TR127,    /* FRAME_CNT_127 */
        &MAC->TR255,    /* FRAME_CNT_255 */
        &MAC->TR511,    /* FRAME_CNT_511 */
        &MAC->TR1K,     /* FRAME_CNT_1K */
        &MAC->TRMAX,    /* FRAME_CNT_MAX */
        &MAC->TRMGV,    /* FRAME_CNT_VLAN */
        &MAC->RBYT,     /* RX_BYTE_CNT */
        &MAC->PPKT,     /* RX_PKT_CNT */
        &MAC->RFCS,     /* RX_FCS_ERR_CNT */
        &MAC->RMCA,     /* RX_MULTICAST_PKT_CNT */
        &MAC->RBCA,     /* RX_BROADCAST_PKT_CNT */
        &MAC->RXCF,     /* RX_CTRL_PKT_CNT */
        &MAC->RXPF,     /* RX_PAUSE_PKT_CNT */
        &MAC->RXUO,     /* RX_UNKNOWN_OPCODE_CNT */
        &MAC->RALN,     /* RX_ALIGN_ERR_CNT */
        &MAC->RFLR,     /* RX_FRAMELENGTH_ERR_CNT */
        &MAC->RCDE,     /* RX_CODE_ERR_CNT */
        &MAC->RCSE,     /* RX_CS_ERR_CNT */
        &MAC->RUND,     /* RX_UNDERSIZE_PKT_CNT */
        &MAC->ROVR,     /* RX_OVERSIZE_PKT_CNT */
        &MAC->RFRG,     /* RX_FRAGMENT_CNT */
        &MAC->RJBR,     /* RX_JABBER_CNT */
        &MAC->RDRP,     /* RX_DROP_CNT */
        &MAC->TBYT,     /* TX_BYTE_CNT */
        &MAC->TPKT,     /* TX_PKT_CNT */
        &MAC->TMCA,     /* TX_MULTICAST_PKT_CNT */
        &MAC->TBCA,     /* TX_BROADCAST_PKT_CNT */
        &MAC->TXPF,     /* TX_PAUSE_PKT_CNT */
        &MAC->TDFR,     /* TX_DEFFERAL_PKT_CNT */
        &MAC->TEDF,     /* TX_EXCS_DEFFERAL_PKT_CNT */
        &MAC->TSCL,     /* TX_SINGLE_COLL_PKT_CNT */
        &MAC->TMCL,     /* TX_MULTI_COLL_PKT_CNT */
        &MAC->TLCL,     /* TX_LATE_COLL_PKT_CNT */
        &MAC->TXCL,     /* TX_EXCSS_COLL_PKT_CNT */
        &MAC->TNCL,     /* TX_TOTAL_COLL_PKT_CNT */
        &MAC->TPFH,     /* TX_PAUSE_HONORED_CNT */
        &MAC->TDRP,     /* TX_DROP_CNT */
        &MAC->TJBR,     /* TX_JABBER_CNT */
        &MAC->TFCS,     /* TX_FCS_ERR_CNT */
        &MAC->TXCF,     /* TX_CNTRL_PKT_CNT */
        &MAC->TOVR,     /* TX_OVERSIZE_PKT_CNT */
        &MAC->TUND,     /* TX_UNDERSIZE_PKT_CNT */
        &MAC->TFRG      /* TX_FRAGMENT_CNT */
    };
    
    ASSERT ( MAC_LAST_STAT > stat );
    
    if( MAC_LAST_STAT > stat )
    {
        stat_val = *stat_regs_lut[stat];
    }

    return stat_val;
}

/**************************************************************************//**
 * See mss_ethernet_mac.h for details of how to use this function.
 */
uint8_t
MSS_MAC_receive_pkt
(
    uint8_t * rx_pkt_buffer,
    void * p_user_data
)
{
    uint8_t status = MSS_MAC_FAILED;

    /* Make this function atomic w.r.to EMAC interrupt */
    NVIC_DisableIRQ(EthernetMAC_IRQn);
    
    ASSERT(NULL_POINTER != rx_pkt_buffer);
    ASSERT(IS_WORD_ALIGNED(rx_pkt_buffer));
    
    if(g_mac.nb_available_rx_desc > 0)
    {
        int16_t next_rx_desc_index;
        
        --g_mac.nb_available_rx_desc;
        next_rx_desc_index = g_mac.next_free_rx_desc_index;
        
        g_mac.rx_desc_tab[next_rx_desc_index].pkt_start_addr = rx_pkt_buffer;
        g_mac.rx_desc_tab[next_rx_desc_index].caller_info = p_user_data;

        /* Set own flag of this descriptor to indicate ready for RX */
        g_mac.rx_desc_tab[next_rx_desc_index].pkt_size |= DMA_DESC_EMPTY_FLAG_MASK;

        /* 
           If the RX is found disabled, then it might be because this is the
           first time a packet is scheduled for reception or the RX ENABLE is 
           made zero by RX overflow or RX bus error. In either case, this
           function tries to schedule the current packet for reception.
        */
        if(0u == read_bit_reg32(&MAC->DMA_RX_CTRL, DMA_RXENABLE))
        {
            /* Make current descriptor to point to the packet requested */
            g_mac.first_rx_desc_index = next_rx_desc_index;
            MAC->DMA_RX_DESC = (uint32_t)&g_mac.rx_desc_tab[next_rx_desc_index];
            set_bit_reg32(&MAC->DMA_RX_STATUS, DMA_RXOVRFLOW);
            set_bit_reg32(&MAC->DMA_RX_CTRL, DMA_RXENABLE);
        }

        /* Point the next_rx_desc to next free descriptor in the ring */
        /* Wrap around in case next descriptor is pointing to last in the ring */
        ++g_mac.next_free_rx_desc_index;
        if(g_mac.next_free_rx_desc_index >= MSS_MAC_RX_RING_SIZE)
        {
            g_mac.next_free_rx_desc_index = 0;
        }
        
        /* Packet scheduled for reception successfully only when there 
           is no new RX bus error */
        if(0u == read_bit_reg32(&MAC->DMA_RX_STATUS, DMA_RXBUSERR))
        {
            status = MSS_MAC_SUCCESS;
        }
    }
    /* Ethernet Interrupt Enable function. */
    NVIC_EnableIRQ(EthernetMAC_IRQn);
    return status;
}

/***************************************************************************//**
 * See mss_ethernet_mac.h for details of how to use this function.
 */
uint8_t 
MSS_MAC_send_pkt
(
    uint8_t const * tx_buffer,
    uint32_t tx_length,
    void * p_user_data
)
{
    uint8_t status = MSS_MAC_FAILED;

    /* Make this function atomic w.r.to EMAC interrupt */
    NVIC_DisableIRQ(EthernetMAC_IRQn);

    ASSERT(NULL_POINTER != tx_buffer);
    ASSERT(0 != tx_length);
    ASSERT(IS_WORD_ALIGNED(tx_buffer));
    
    if(g_mac.nb_available_tx_desc > 0)
    {
        MAC_tx_desc_t * p_next_tx_desc;
        
        --g_mac.nb_available_tx_desc;
        if(INVALID_INDEX == g_mac.first_tx_index)
        {
            g_mac.first_tx_index = g_mac.next_tx_index;
        }
        g_mac.last_tx_index = g_mac.next_tx_index;
        
        p_next_tx_desc = &g_mac.tx_desc_tab[g_mac.next_tx_index];
        p_next_tx_desc->pkt_start_addr = tx_buffer;
        p_next_tx_desc->caller_info = p_user_data;
        /* Set the packet length, packet overrides and clear descriptor empty: */
        p_next_tx_desc->pkt_size = tx_length;
        
        /*
            If TX is found disabled, this might be because this is the first
            time packet is being sent or the DMA completed previous transfer and
            stopped transmission by itself caused by TX underrun or bus error.
            This function neglects the errors and tries to send the current packet.
         */
        if(DMA_TX_DISABLED == MAC->DMA_TX_CTRL)
        {
            MAC->DMA_TX_DESC = (uint32_t)p_next_tx_desc;
        }
        /* Enable DMA transmit anyway to cover the case where Tx completed after
           the read of DMA_TX_CTRL. */
        MAC->DMA_TX_CTRL = DMA_TX_ENABLED;
        
        /* Point the next_tx_desc to next free descriptor in the ring */
        /* Wrap around in case next descriptor is pointing to last in the ring */
        if((MSS_MAC_TX_RING_SIZE - 1) == g_mac.next_tx_index)
        {
            g_mac.next_tx_index = 0;
        }
        else
        {
            ++g_mac.next_tx_index;
        }
        
        /* Packet scheduled for transmission successfully only when 
           there is no TX bus error */
        if(0u == read_bit_reg32( &MAC->DMA_TX_STATUS, DMA_TXBUSERR))
        {
            status = MSS_MAC_SUCCESS;
        }
    }
    
    /* Ethernet Interrupt Enable function. */
    NVIC_EnableIRQ(EthernetMAC_IRQn);
    return status;
}

/**************************************************************************//**
 * See mss_ethernet_mac.h for details of how to use this function.
 */
void
EthernetMAC_IRQHandler
(
    void
)
{
    uint32_t dma_irq;
    uint32_t packet_sent;
    uint32_t packet_received;
    
    dma_irq = MAC->DMA_IRQ;

    /* Transmit packet sent interrupt */
    packet_sent = dma_irq & MSS_MAC_TXPKTSENT_IRQ_MASK;
    if(packet_sent != 0u)
    {
        txpkt_handler();
    }
    /* Packet received interrupt */
    packet_received = dma_irq & MSS_MAC_RXPKTRCVD_IRQ_MASK;
    if(packet_received != 0u)
    {
        rxpkt_handler();
    }
}

/**************************************************************************//**
 * See mss_ethernet_mac.h for details of how to use this function.
 */
void MSS_MAC_set_tx_callback
(
    MAC_transmit_callback_t tx_complete_handler
)
{
    g_mac.tx_complete_handler = tx_complete_handler;
}

/**************************************************************************//**
 * See mss_ethernet_mac.h for details of how to use this function.
 */
void MSS_MAC_set_rx_callback
(
    MAC_receive_callback_t rx_callback
)
{
    g_mac.pckt_rx_callback = rx_callback;
}

/**************************************************************************/
/* Private Function definitions                                           */
/**************************************************************************/  

/**************************************************************************//**
 * This is default "Receive packet interrupt handler. This function finds the
 * descriptor that received the packet and caused the interrupt.
 * This informs the received packet size to the application and
 * relinquishes the packet buffer from the associated DMA descriptor.
 */
static void 
rxpkt_handler
(
    void
)
{
    uint32_t rxcnt = 0u;

    /* Execution comes here because at-least one packet is received.
       Hence rxcnt can be read after freeing one descriptor */
    do
    {
        MAC_rx_desc_t * cdesc = &g_mac.rx_desc_tab[g_mac.first_rx_desc_index];

        /* Point the curr_rx_desc to next descriptor in the ring */
        /* Wrap around in case next descriptor is pointing to last in the ring */
        ++g_mac.first_rx_desc_index;
        if(g_mac.first_rx_desc_index >= MSS_MAC_RX_RING_SIZE)
        {
            g_mac.first_rx_desc_index = 0;
        }
        
        /* Clear the rx packet received interrupt once. If this 
           bit still persists, then another rx packet received 
           interrupt will be generated. Rx count will be decremented.*/
        set_bit_reg32(&MAC->DMA_RX_STATUS, DMA_RXPKTRCVD);
        /* Get the latest current received count */
        rxcnt = (MAC->DMA_RX_STATUS & DMA_PKTCOUNT_MASK) >> DMA_PKTCOUNT;
        
        /* Pass received packet up to application layer. */
        if(NULL_POINTER != g_mac.pckt_rx_callback)
        {
            uint8_t * p_rx_packet;
            uint32_t pckt_length;
            
            p_rx_packet = cdesc->pkt_start_addr;
            pckt_length = (cdesc->pkt_size & DMA_DESC_PKT_SIZE_MASK) - 4u;
            if(NULL_POINTER != g_mac.pckt_rx_callback)
            {
                g_mac.pckt_rx_callback(p_rx_packet, pckt_length, cdesc->caller_info);
            }
        }
        
        ++g_mac.nb_available_rx_desc;
    } while(0u != rxcnt);
}

/**************************************************************************//**
 * This is default "Transmit packet interrupt handler. This function finds the
 * descriptor that transmitted the packet and caused the interrupt.
 * This relinquishes the packet buffer from the associated DMA descriptor.
 */
static void 
txpkt_handler
(
    void
)
{
    uint32_t empty_flag;
    int16_t index;
    uint32_t completed = 0u;
        
    ASSERT(g_mac.first_tx_index != INVALID_INDEX);
    ASSERT(g_mac.last_tx_index != INVALID_INDEX);
    
    index = g_mac.first_tx_index;
    do
    {
        /* Call packet Tx completion handler if it exists. */
        if(NULL_POINTER != g_mac.tx_complete_handler)
        {
            g_mac.tx_complete_handler(g_mac.tx_desc_tab[index].caller_info);
        }
        
        if(index == g_mac.last_tx_index)
        {
            /* all pending tx packets sent. */
            g_mac.first_tx_index = INVALID_INDEX;
            g_mac.last_tx_index = INVALID_INDEX;
            completed = 1u;
        }
        else
        {
            /* Move on to next transmit descriptor. */
            ++index;
            if(index >= MSS_MAC_TX_RING_SIZE)
            {
                index = 0;
            }
            g_mac.first_tx_index = index;
            
            /* Check if we reached a descriptor still pending tx. */
            empty_flag = g_mac.tx_desc_tab[index].pkt_size & DMA_DESC_EMPTY_FLAG_MASK;
            if(0u == empty_flag)
            {
                completed = 1u;
            }
        }
        
        ++g_mac.nb_available_tx_desc;
        
        /* Clear the tx packet sent interrupt. Please note that this must be
         * done for every packet sent as it decrements the TXPKTCOUNT. */
        set_bit_reg32(&MAC->DMA_TX_STATUS, DMA_TXPKTSENT);
    } while(0u == completed);
}

/**************************************************************************//**
 * 
 */
static void tx_desc_ring_init(void)
{
    int32_t inc;
    
    for(inc = 0; inc < MSS_MAC_TX_RING_SIZE; ++inc)
    {
        g_mac.tx_desc_tab[inc].pkt_start_addr = 0u;
        g_mac.tx_desc_tab[inc].pkt_size = DMA_DESC_EMPTY_FLAG_MASK;
        if((MSS_MAC_TX_RING_SIZE - 1) == inc)
        {
            g_mac.tx_desc_tab[inc].next_desriptor = &g_mac.tx_desc_tab[0];
        }
        else
        {
            g_mac.tx_desc_tab[inc].next_desriptor = &g_mac.tx_desc_tab[inc + 1];
        }
        g_mac.tx_desc_tab[inc].index = inc;
    }
}

static void rx_desc_ring_init(void)
{
    int32_t inc;
    
    for(inc = 0; inc < MSS_MAC_RX_RING_SIZE; ++inc)
    {
        g_mac.rx_desc_tab[inc].pkt_start_addr = 0u;
        g_mac.rx_desc_tab[inc].pkt_size = 0u;
        if((MSS_MAC_RX_RING_SIZE - 1) == inc)
        {
            g_mac.rx_desc_tab[inc].next_desriptor = &g_mac.rx_desc_tab[0];
        }
        else
        {
            g_mac.rx_desc_tab[inc].next_desriptor = &g_mac.rx_desc_tab[inc + 1];
        }
        g_mac.rx_desc_tab[inc].index = inc;
    }
}

/**************************************************************************//**
 * 
 */
static void assign_station_addr
(
    const uint8_t mac_addr[6]
)
{
    uint32_t address32;

    ASSERT(NULL_POINTER!= mac_addr);

    if(NULL_POINTER != mac_addr)
    {
        address32 = ((uint32_t)mac_addr[5]) << 24u;
        address32 |= ((uint32_t)mac_addr[4]) << 16u;
        address32 |= ((uint32_t)mac_addr[3]) << 8u;
        address32 |= ((uint32_t)mac_addr[2]);
        MAC->STATION_ADDRESS1 = address32;
        address32 = ((uint32_t)mac_addr[1]) << 24u;
        address32 |= ((uint32_t)mac_addr[0]) << 16u;
        MAC->STATION_ADDRESS2 = address32;
    }
}

#ifdef __cplusplus
}
#endif

/******************************** END OF FILE ******************************/
