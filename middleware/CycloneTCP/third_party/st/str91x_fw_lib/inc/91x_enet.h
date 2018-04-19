/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : 91x_enet.h
* Author             : MCD Application Team
* Version            : V2.0
* Date               : 12/07/2007
* Description        : ENET driver defines & functions prototypes.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS
* A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef _ENET_H_
#define _ENET_H_


/* Includes ------------------------------------------------------------------*/
#include "91x_lib.h"

/* Exported variables --------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                Constants                                   */
/*----------------------------------------------------------------------------*/

/* ENET Operating Mode */
/* Set the Operation Mode by Auto-Negotiation process */
#define AUTO_NEGOTIATION                 PHY_AutoNegotiation
/* Set the full/half-duplex mode at 100 Mb/s */
#define FULLDUPLEX_100M                  PHY_FULLDUPLEX_100M
#define HALFDUPLEX_100M                  PHY_HALFDUPLEX_100M
/* Set the full/half-duplex mode at 10 Mb/s */
#define FULLDUPLEX_10M                   PHY_FULLDUPLEX_10M
#define HALFDUPLEX_10M                   PHY_HALFDUPLEX_10M

/* AddressFilteringMode */
#define MAC_Perfect_Multicast_Perfect    0x0
#define MAC_Perfect_Muticast_Hash        (0x1<<17)
#define MAC_Hash_Multicast_Hash          (0x2<<17)
#define Inverse                          (0x3<<17)
#define Promiscuous                      (0x4<<17)
#define MAC_Perfect_Multicast_All        (0x5<<17)
#define MAC_Hash_Muticast_All            (0x6<<17)

/* VLANFilteringMode */
#define VLANFilter_VLTAG_VLID            0x1
#define VLANfilter_VLTAG                 0x0

/* VLAN 1 */
#define VLANID1                          0x0000
#define VLANTAG1                         0x8100

/* VLAN 2 */
#define VLANID2                          0x0000
#define VLANTAG2                         0x8100

/* MIIPrescaler */
#define MIIPrescaler_1                   0x0       /* Prescaler for MDC clock when HCLK < 50 MHz */
#define MIIPrescaler_2                   0x1       /* Precaler for MDC when HCLK > = 50 MHz */

/* MAC Address */  
#define MAC_ADDR0                        0x00
#define MAC_ADDR1                        0x02
#define MAC_ADDR2                        0x04
#define MAC_ADDR3                        0x08
#define MAC_ADDR4                        0x0A
#define MAC_ADDR5                        0x0D

/* Multicast Address */
#define MCAST_ADDR0                      0xFF
#define MCAST_ADDR1                      0x00
#define MCAST_ADDR2                      0xFF
#define MCAST_ADDR3                      0x00
#define MCAST_ADDR4                      0xFF
#define MCAST_ADDR5                      0x00

/* ENET Buffers setting */                                                    
#define ENET_MAX_PACKET_SIZE             1520
#define ENET_RXBUFNB                     6
#define ENET_TXBUFNB                     3

/* PHY Address */
#define PHY_ADDRESS                      0x15
                                                          
                                                                                                    
/*----------------------------------------------------------------------------*/
/*                        ENET registers description                          */
/*----------------------------------------------------------------------------*/
                                                                          
/* ENET_MCR */                                                       
#define ENET_MCR_RA                      0x80000000  /*  Received All */
#define ENET_MCR_EN                      0x40000000  /*  Endianity */
#define ENET_MCR_PS                      0x03000000  /*  Prescaler bits */
#define ENET_MCR_DRO                     0x00800000  /*  Disable Receive Own */
#define ENET_MCR_LM                      0x00200000  /*  Loopback Mode */
#define ENET_MCR_FDM                     0x00100000  /*  Full Duplex Mode */
#define ENET_MCR_AFM                     0x000E0000  /*  Address Filtering Mode */
#define ENET_MCR_PWF                     0x00010000  /*  Pass Wrong Frame */
#define ENET_MCR_VFM                     0x00008000  /*  VLAN Filtering Mode */
#define ENET_MCR_ELC                     0x00001000  /*  Enable Late Collision */
#define ENET_MCR_DBF                     0x00000800  /*  Disable Broadcast Frame */
#define ENET_MCR_DPR                     0x00000400  /*  Disable Packet Retry */
#define ENET_MCR_RVFF                    0x00000200  /*  VCI Rx Frame Filtering */
#define ENET_MCR_APR                     0x00000100  /*  Automatic Pad Removal */
#define ENET_MCR_BL                      0x000000C0  /*  Back-off Limit */
#define ENET_MCR_DCE                     0x00000020  /*  Deferral Check Enable */
#define ENET_MCR_RVBE                    0x00000010  /*  Reception VCI Burst Enable */
#define ENET_MCR_TE                      0x00000008  /*  Transmission Enable */
#define ENET_MCR_RE                      0x00000004  /*  Reception Enable */
#define ENET_MCR_RCFA                    0x00000001  /*  Reverse Control Frame Address */
                                        
/* ENET_MTS */                                                                                                
#define ENET_MTS_PR                      0x80000000  /*  Packet Retry */
#define ENET_MTS_BC                      0x7FFC0000  /*  Byte Count */
#define ENET_MTS_CC                      0x00003C00  /*  Collision Count */
#define ENET_MTS_LCO                     0x00000200  /*  Late Collision Observed */
#define ENET_MTS_DEF                     0x00000100  /*  Defferd */
#define ENET_MTS_UR                      0x00000080  /*  Under Run */
#define ENET_MTS_EC                      0x00000040  /*  Excessive Collision */
#define ENET_MTS_LC                      0x00000020  /*  Late Collision */
#define ENET_MTS_ED                      0x00000010  /*  Excessive Deferral */
#define ENET_MTS_LOC                     0x00000008  /*  Loss of Carrier */
#define ENET_MTS_NC                      0x00000004  /*  No Carrier */
#define ENET_MTS_FA                      0x00000001  /*  Frame Aborted */
                                        
/* ENET_MRS */                               
#define ENET_MRS_FA                      0x80000000  /*  Frame Aborted */
#define ENET_MRS_PF                      0x40000000  /*  Packet Filter */
#define ENET_MRS_FF                      0x20000000  /*  Filtering Fail */
#define ENET_MRS_BF                      0x10000000  /*  Broadcast Frame */
#define ENET_MRS_MCF                     0x08000000  /*  Multicast Frame */
#define ENET_MRS_UCF                     0x04000000  /*  Unsupported control Frame */
#define ENET_MRS_CF                      0x02000000  /*  Control Frame */
#define ENET_MRS_LE                      0x01000000  /*  Length Error */
#define ENET_MRS_VL2                     0x00800000  /*  Vlan2 Tag */
#define ENET_MRS_VL1                     0x00400000  /*  Vlan1 Tag */
#define ENET_MRS_CE                      0x00200000  /*  CRC Error */
#define ENET_MRS_EB                      0x00100000  /*  Extra Bit */
#define ENET_MRS_ME                      0x00080000  /*  MII Error */
#define ENET_MRS_FT                      0x00040000  /*  Frame Type */
#define ENET_MRS_LC                      0x00020000  /*  Late Collision */
#define ENET_MRS_OL                      0x00010000  /*  Over Length */
#define ENET_MRS_RF                      0x00008000  /*  Runt Frame */
#define ENET_MRS_WT                      0x00004000  /*  Watchdog Time-out */
#define ENET_MRS_FCI                     0x00002000  /*  False Carrier Indication */
#define ENET_MRS_FL                      0x000007FF  /*  Frame Length */
                                                                                                                                        
/* ENET_SCR */                               
#define ENET_SCR_SRESET                  0x00000001 /* Soft Reset (ENET DMA_SCR_RESET) */
#define ENET_SCR_LOOPB                   0x00000002 /* Loopback mode (ENET DMA_SCR_LOOPB) */
#define ENET_SCR_RX_MBSIZE               0x00000010 /* Max defined burst length in RX mode (ENET DMA_SCR_RX_MAX_BURST_...) */
#define ENET_SCR_TX_MBSIZE               0x000000C0 /* Max defined burst length in TX mode (ENET ENET DMA_SCR_TX_MAX_BURST_...) */
#define ENET_SCR_RX_MAX_BURST_SZ         ENET_SCR_RX_MBSIZE /* Maximum value of defined burst length in RX mode */
#define ENET_SCR_RX_MAX_BURST_SZ_VAL     0x00000000 /* Default value of burst length in RX mode */
#define ENET_SCR_TX_MAX_BURST_SZ         ENET_SCR_TX_MBSIZE /* Maximum value of defined burst length in TX mode */
#define ENET_SCR_TX_MAX_BURST_SZ_VAL     0x000000C0 /* Default value of burst length in TX mode */

/* ENET DMA_RX_START   */
#define ENET_RXSTR_DMA_EN                0x00000001 /* set = 0 by sw force a ENET DMA abort */
#define ENET_RXSTR_FETCH                 0x00000004 /* start fetching the 1st descriptor */
#define ENET_RXSTR_FILTER_FAIL           0x00000020 /* if = 1 the address filtering failed cond */
#define ENET_RXSTR_RUNT                  0x00000040 /* discard damaged RX frames from cpu charge */
#define ENET_RXSTR_COLLS_SEEN            0x00000080 /* Late Collision Seen Cond discard frame automat. */
#define ENET_RXSTR_DFETCH_DLY            0x00FFFF00 /* Descriptor Fetch Delay */
#define ENET_RXSTR_DFETCH_DEFAULT        0x00010000 /* Descriptor Fetch Delay default value */

/* ENET DMA_TX_START   */
#define ENET_TXSTR_DMA_EN                0x00000001 /* set = 0 by sw force a ENET DMA abort */
#define ENET_TXSTR_FETCH                 0x00000004 /* start fetching the 1st descriptor */
#define ENET_TXSTR_URUN                  0x00000020 /* Underrun Enable */
#define ENET_TXSTR_DFETCH_DLY            0x00FFFF00 /* Descriptor Fetch Delay */
#define ENET_TXSTR_DFETCH_DEFAULT        0x00010000 /* Descriptor Fetch Delay default value */
                                        
/* ENET DMA TX CTRL    */                  
#define ENET_TXCR_DLYEN                  0x00008000 /* ENET DMA trigger delay enable */
#define ENET_TXCR_NXTEN                  0x00004000 /* Next Descriptor Fetch Mode Enable */
#define ENET_TXCR_CONTEN                 0x00001000 /* Continuous Mode Enable */
#define ENET_TXCR_XFERCOUNT              0x00000FFF /* ENET DMA transfer count */

/* ENET DMA RX CTRL    */                  
#define ENET_RXCR_DLYEN                  0x00008000 /* ENET DMA trigger delay enable */
#define ENET_RXCR_NXTEN                  0x00004000 /* Next Descriptor Fetch Mode Enable */
#define ENET_RXCR_CONTEN                 0x00001000 /* Continuous Mode Enable */
#define ENET_RXCR_XFERCOUNT              0x00000FFF /* ENET DMA transfer count */

/* ENET DMA Tx Start Address */                  
#define ENET_TXSAR_TXADDR                0xFFFFFFFC /* for ENET DMA Start Address (32 bit Word Align) */
#define ENET_TXSAR_ADDR_FIX_ADDR         0x00000002 /* Disable incrementing of ENET DMA_ADDR */
#define ENET_TXSAR_WRAP_EN               0x00000001

/* ENET DMA Rx Start Address */                  
#define ENET_RXSAR_RXADDR                0xFFFFFFFC /* for ENET DMA Start Address (32 bit Word Align) */
#define ENET_RXSAR_ADDR_FIX_ADDR         0x00000002 /* Disable incrementing of ENET DMA_ADDR */
#define ENET_RXSAR_WRAP_EN               0x00000001


/* ENET DMA TX Next Descriptor Address */
#define ENET_TXNDAR_DSCRADDR             0xFFFFFFFC /* Points to Next descriptor starting address */
#define ENET_TXNDAR_NPOL_EN              0x00000001 /* Next Descriptor Polling Enable */

/* ENET DMA RX Next Descriptor Address */
#define ENET_RXNDAR_DSCRADDR             0xFFFFFFFC /* Points to Next descriptor starting address */
#define ENET_RXNDAR_NPOL_EN              0x00000001 /* Next Descriptor Polling Enable */
                                        
/* ENET DMA Descriptor Packet Status: TX */  
#define ENET_DSCR_TX_STATUS_FA_MSK       0x00000001 /* Frame Aborted */
#define ENET_DSCR_TX_STATUS_JTO_MSK      0x00000002 /* Jabber Timeout. */
#define ENET_DSCR_TX_STATUS_NOC_MSK      0x00000004 /* No Carrier */
#define ENET_DSCR_TX_STATUS_LOC_MSK      0x00000008 /* Loss of Carrier */
#define ENET_DSCR_TX_STATUS_EXCD_MSK     0x00000010 /* Excessive Deferral */
#define ENET_DSCR_TX_STATUS_LCOLL_MSK    0x00000020 /* Late Collision */
#define ENET_DSCR_TX_STATUS_ECOLL_MSK    0x00000040 /* Excessive Collisions */
#define ENET_DSCR_TX_STATUS_URUN_MSK     0x00000080 /* Under Run */
#define ENET_DSCR_TX_STATUS_DEFER_MSK    0x00000100 /* Deferred */
#define ENET_DSCR_TX_STATUS_LCOLLO_MSK   0x00000200 /* Late Collision Observed */
#define ENET_DSCR_TX_STATUS_CCNT_MSK     0x00003C00 /* Collision Count */
#define ENET_DSCR_TX_STATUS_HBFAIL_MSK   0x00004000 /* Heart Beat Fail */
#define ENET_DSCR_TX_STATUS_VALID_MSK    0x00010000 /* Valid bit indicator - This bit marks the dscriptors this word belong */
#define ENET_DSCR_TX_STATUS_PKT_RTRY_MSK 0x80000000 /* Packet Retry */
#define ENET_DSCR_TX_STATUS_ORED_ERR_MSK 0x000003D7 /* for total number of errors */
                                        
/* ENET DMA Descriptor Packet Status: RX */  
#define ENET_DSCR_RX_STATUS_FLEN_MSK     0x000007ff /* 0x00003FFF * Frame Length (max 2047) */
#define ENET_DSCR_RX_STATUS_FTLONG_MSK   0x00001000 /* Over Lenght */
#define ENET_DSCR_RX_STATUS_FCI_MSK      0x00002000 /* Frame too Long */
#define ENET_DSCR_RX_STATUS_WDTO_MSK     0x00004000 /* Watchdog Timeout */
#define ENET_DSCR_RX_STATUS_RUNTFR_MSK   0x00008000 /* Runt Frame */
#define ENET_DSCR_RX_STATUS_VALID_MSK    0x00010000 /* Valid bit indicator - This bit marks the dscriptors this word  */
#define ENET_DSCR_RX_STATUS_COLLSEEN_MSK 0x00020000 /* Collision Seen */
#define ENET_DSCR_RX_STATUS_FTYPE_MSK    0x00040000 /* Frame Type */
#define ENET_DSCR_RX_STATUS_MII_ERR_MSK  0x00080000 /* MII Error */
#define ENET_DSCR_RX_STATUS_DRBBIT_MSK   0x00100000 /* Dribbling Bit */
#define ENET_DSCR_RX_STATUS_CRC_ERR_MSK  0x00200000 /* CRC Error */
#define ENET_DSCR_RX_STATUS_VLAN1_FR_MSK 0x00400000 /* One-Level VLAN Frame */
#define ENET_DSCR_RX_STATUS_VLAN2_FR_MSK 0x00800000 /* Two-Level VLAN Frame */
#define ENET_DSCR_RX_STATUS_LEN_ERR_MSK  0x01000000 /* Length Error */
#define ENET_DSCR_RX_STATUS_CTL_FR_MSK   0x02000000 /* Control Frame */
#define ENET_DSCR_RX_STATUS_UCTRL_FR_MSK 0x04000000 /* Unsupported Control Frame */
#define ENET_DSCR_RX_STATUS_MCAST_FR_MSK 0x08000000 /* Multicast Frame */
#define ENET_DSCR_RX_STATUS_BCAST_FR_MSK 0x10000000 /* BroadCast Frame */
#define ENET_DSCR_RX_STATUS_FLT_FAIL_MSK 0x20000000 /* Filtering Fail */
#define ENET_DSCR_RX_STATUS_PKT_FILT_MSK 0x40000000 /* Packet Filter */
#define ENET_DSCR_RX_STATUS_MIS_FR_MSK   0x80000000 /* Missed Frame */
#define ENET_DSCR_RX_STATUS_ERROR_MSK   (ENET_DSCR_RX_STATUS_LEN_ERR | ENET_DSCR_RX_STATUS_CRC_ERR | \
                                        ENET_DSCR_RX_STATUS_MII_ERR | ENET_DSCR_RX_STATUS_RUNTFR |  \
                                        ENET_DSCR_RX_STATUS_FTLONG | ENET_DSCR_RX_STATUS_COLLSEEN)
#define ENET_DSCR_RX_STATUS_ORED_ERR_MSK 0x00000000 /*Mask for total number of errors */
                                                
/* ENET DMA Interrupt Definition */                                                                       
#define ENET_IT_All                     0xFFFFFFFF
#define ENET_IT_TX_CURR_DONE            0x80000000  /*  Tx Current Done interrupt */  
#define ENET_IT_TX_MERR_INT             0x02000000  /*  Tx Master Error interrupt */
#define ENET_IT_TX_DONE                 0x00800000  /*  Tx Done interrupt */
#define ENET_IT_TX_NEXT                 0x00400000  /*  Tx Invalid Next Descriptor interrupt */
#define ENET_IT_TX_TO                   0x00080000  /*  Tx timeout interrupt */
#define ENET_IT_TX_ENTRY                0x00040000  /*  Tx FIFO Entry Error interrupt */ 
#define ENET_IT_TX_FULL                 0x00020000  /*  Tx FIFO Full interrupt */
#define ENET_IT_TX_EMPTY                0x00010000  /*  Tx FIFO Empty interrupt */
#define ENET_IT_RX_CURR_DONE            0x00008000  /*  Rx Current Done interrupt */
#define ENET_IT_RX_MERR_INT             0x00000200  /*  Rx Master Error interrupt */ 
#define ENET_IT_RX_DONE                 0x00000080  /*  Rx Done interrupt */
#define ENET_IT_RX_NEXT                 0x00000040  /*  Rx Invalid Next Descriptor interrupt */ 
#define ENET_IT_PACKET_LOST             0x00000020  /*  Packet Lost interrupt */
#define ENET_IT_RX_TO                   0x00000008  /*  Rx Timeout */
#define ENET_IT_RX_ENTRY                0x00000004  /*  Rx FIFO Entry Error interrupt */
#define ENET_IT_RX_FULL                 0x00000002  /*  Rx FIFO Full interrupt */
#define ENET_IT_RX_EMPTY                0x00000001  /*  Rx FIFO Empty interrupt */

/* ENET_MIIA */
#define ENET_MIIA_PADDR                 0x0000F800  /* Physical address mask */
#define ENET_MIIA_RADDR                 0x000007C0  /* Register address mask */
#define ENET_MIIA_PR                    0x00000004  /* MII Preamble removal */
#define ENET_MIIA_WR                    0x00000002  /* MII Write/Read */
#define ENET_MIIA_BUSY                  0x00000001  /* MII Busy */

/* ENET_MIID */
#define ENET_MIID_RDATA                 0x0000FFFF  /* MII Data */
                                                            
/* MII Read/write Timeouts*/                            
#define MII_READ_TO                     0x0004FFFF      
#define MII_WRITE_TO                    0x0004FFFF    

/*----------------------------------------------------------------------------*/
/*                  Description of common PHY registers                       */
/*----------------------------------------------------------------------------*/
                                                                    
/* Register address */                                  
#define PHY_XCR                          0          /* Tranceiver Control Register */
#define PHY_XSR                          1          /* Tranceiver Status Register */
#define PHY_PID1                         2          /* Tranceiver PHY IDentifier 1 */
#define PHY_PID2                         3          /* Tranceiver PHY IDentifier 2 */
#define PHY_ANA                          4          /* Auto-Negotiation Advertissement register */
#define PHY_ANLPA                        5          /* Auto-Negotiation Link Partner Ability register */
#define PHY_ANE                          6          /* Auto-Negotiation Expansion register */
#define PHY_XCIIS                       17          /* Configuration Information and Interrupt Status */
#define PHY_XIE                         18          /* Interrupt Enable register*/
#define PHY_100CTR                      19          /* 100Base-TX control register */
#define PHY_XMC                         20          /* Mode control */
                                                      
/* XCR, control register */                             
#define PHY_Reset_Control               0x8000      /* PHY Reset */
#define PHY_Loopback                    0x4000      /* Select loop-back mode */
#define PHY_FULLDUPLEX_100M             0x2100      /* Set the full-duplex mode at 100 Mb/s */
#define PHY_HALFDUPLEX_100M             0x2000      /* Set the half-duplex mode at 100 Mb/s */
#define PHY_FULLDUPLEX_10M              0x0100      /* Set the full-duplex mode at 10 Mb/s */
#define PHY_HALFDUPLEX_10M              0x0000      /* Set the half-duplex mode at 10 Mb/s */
#define PHY_AutoNegotiation             0x1000      /* Enable auto-negotiation function */
#define PHY_Restart_AutoNegotiation     0x200       /* Restart auto-negotiation function */
#define PHY_Powerdown                   0x800       /* Select the power down mode */
#define PHY_Isolate                     0x400       /* Isolate PHY from MII */

#define PHY_ResetDelay                  0xFFFFF    

/* XSR, status register */
/* Read only register */                                 
#define PHY_AutoNego_Complete           0x20        /* Auto-Negotioation process completed */
#define PHY_Linked_Status               0x04        /* Valid link established */
#define PHY_Jabber_detection            0x02        /* Jabber condition detected */
                                                      
/* ANLP, Auto-Negotiation Link Partner Ability */
/* Read only register */      
#define PHY_LinkPartner_NextPage        0x8000      /* Link partner with Next page ability */
#define PHY_LinkPartner_ACK             0x4000      /* Link partner Acknowledge */
#define PHY_LinkPartner_Fault           0x2000      /* Link partner's remote fault status */ 
#define PHY_LinkPartner_FlowCtl         0x0400      /* Link partner with PAUSE function ability */
#define PHY_LinkPartner_100T4           0x0200      /* 100BASE-T4 ability */
#define PHY_LinkPartner_100F            0x0100      /* 100M Full duplex ability */
#define PHY_LinkPartner_100H            0x0080      /* 100M Half duplex ability */
#define PHY_LinkPartner_10F             0x0040      /* 10M Full duplex ability */
#define PHY_LinkPartner_10H             0x0020      /* 10M Half duplex ability */
                                                              
/* ANE, Auto-Negotiation expansion */
/* Read only register */                             
#define PHY_ParallelFault               0x10        /* Parallel detection fault */
#define PHY_NextPage                    0x04        /* In case of STE100P this bit is always 0 since it without next page ability */
#define PHY_NewPageRcv                  0x02        /* Page Received */
#define PHY_LinkPartner_AutoNego        0x01        /* Link Partner Auto-Negotiation ability */
                                                        
/* XCIIS, configuration information and interrupt status */
/* Read only register */ 
#define PHY_Configured_Speed            0x200       /* Configured information of Speed */
#define PHY_Configured_Duplex           0x100       /* Configured information of Duplex */
#define PHY_Configured_PAUSE            0x080       /* Configured information of PAUSE function for flow control */
#define PHY_ITSrc_AutoNego_Cmplt        0x040       /* Interrupt source of Auto-Negotiation Completed */
#define PHY_ITSrc_RemoteFault           0x020       /* Interrupt source of Remote Fault Detected */
#define PHY_ITSrc_LinkFail              0x010       /* Interrupt source of Link Fail */
#define PHY_ITSrc_AutoNeg_Ack           0x008       /* Interrupt source of Auto-Negotiation ACK received */
#define PHY_ITSrc_ParallelFault         0x004       /* Interrupt source of Parallel Detection Fault */
#define PHY_ITSrc_AutoNeg_PageRcv       0x002       /* Interrupt source of Auto-Negotiation Page Received */
#define PHY_ITSrc_ErrorFull             0x001       /* Interrupt source of receive Error Full */  
#define PHY_ConfigurationMask           0x380       /* XCIIS Configuration bits Mask */

/* XIE, interrupt enable register */    
#define PHY_IT_AutoNego_Cmplt           0x40        /* Auto-Negotiation Completed interrupt Enable */
#define PHY_IT_RemoteFault              0x20        /* Remote Fault detected interrupt Enable */
#define PHY_IT_LinkFail                 0x10        /* Link Down interrupt Enable */
#define PHY_IT_AutoNeg_Ack              0x08        /* Auto-Negotiation ACK interrupt Enable */
#define PHY_IT_ParallelFault            0x04        /* Parallel Fault Detection interrupt Enable */
#define PHY_IT_AutoNeg_PageRcv          0x02        /* Auto-Negotiation Page Received interrupt Enable */
#define PHY_IT_ErrorFull                0x01        /* Receive Error Full interrupt Enable */

/*----------------------------------------------------------------------------*/
/*                             Structures typedef                             */
/*----------------------------------------------------------------------------*/

/*Struct containing the DMA Descriptor data */
typedef struct  {
  vu32 dmaCntl;                /* DMA Control Register                     */
  vu32 dmaAddr;                /* DMA Start Address Register               */
  vu32 dmaNext;                /* DMA Next Descriptor Register             */
  vu32 dmaPackStatus;          /* DMA Packet Status and Control Register   */
} ENET_DMADSCRBase;

/* ENET_MACConfig Struct*/
typedef struct {
  FunctionalState ReceiveALL;                 /* Receive All frames: no address rule filtering */
  u32             MIIPrescaler;               /* MII Clock Prescaler value */
  FunctionalState LoopbackMode;               /* MAC Loopback mode */
  u32             AddressFilteringMode;       /* Address Filtering Mode */
  u32             VLANFilteringMode;          /* VLAN Filtering Mode */
  FunctionalState PassWrongFrame;             /* Pass wrong frame (CRC, overlength, runt..)*/
  FunctionalState LateCollision;              /* Retransmit frame when late collision*/
  FunctionalState BroadcastFrameReception;    /* Accept broardcast frame */
  FunctionalState PacketRetry;                /* Retransmit frame in case of collision */
  FunctionalState RxFrameFiltering;           /* Filter early runt frame and address filter fail frames*/
  FunctionalState AutomaticPadRemoval;        /* Automatic Padding removal */
  FunctionalState DeferralCheck;              /* Excessive Defferal check */
} ENET_MACConfig;

/* ENET_TxStatus Struct*/
typedef struct {
  FlagStatus PacketRetry;
  u16         ByteCount;
  u8         CollisionCount;
  FlagStatus LateCollisionObserved;
  FlagStatus Deffered;
  FlagStatus UnderRun;
  FlagStatus ExcessiveCollision;
  FlagStatus LateCollision;
  FlagStatus ExcessiveDefferal;
  FlagStatus LossOfCarrier;
  FlagStatus NoCarrier;
  FlagStatus FrameAborted;
} ENET_TxStatus;

/* ENET_RxStatus Struct*/
typedef struct {
  FlagStatus FrameAborted;
  FlagStatus PacketFilter;
  FlagStatus FilteringFail;
  FlagStatus BroadCastFrame;
  FlagStatus MulticastFrame;
  FlagStatus UnsupportedControFrame;
  FlagStatus ControlFrame;
  FlagStatus LengthError;
  FlagStatus Vlan2Tag;
  FlagStatus Vlan1Tag;
  FlagStatus CRCError;
  FlagStatus ExtraBit;
  FlagStatus MIIError;
  FlagStatus FrameType;
  FlagStatus LateCollision;
  FlagStatus OverLength;
  FlagStatus RuntFrame;
  FlagStatus WatchDogTimout;
  FlagStatus FalseCarrierIndication;
  u16        FrameLength;
} ENET_RxStatus;

/*----------------------------------------------------------------------------*/
/*                              Exported variables                            */
/*----------------------------------------------------------------------------*/
extern ENET_DMADSCRBase  dmaRxDscrBase[ENET_RXBUFNB],
                         dmaTxDscrBase[ENET_TXBUFNB];/* Rx & Tx DMA Descriptors */

extern u8 RxBuff[ENET_RXBUFNB][ENET_MAX_PACKET_SIZE], 
          TxBuff[ENET_TXBUFNB][ENET_MAX_PACKET_SIZE];/* ENET buffers */

extern vu8 RxBC, TxBC;/* Rx & Tx Buffers Counters */


/*----------------------------------------------------------------------------*/
/*                        Functions  Prototypes                               */
/*----------------------------------------------------------------------------*/

/** ENET Initializations ******************************************************/
void ENET_Init(ENET_MACConfig *MAC_Config);
void ENET_SetOperatingMode(u32 ENET_OperatingMode);
void ENET_Start(void);

/** ENET frames transfer ******************************************************/
u32 ENET_HandleRxPkt(void *ppkt);
void ENET_UpdateRxDscr(void);
void ENET_HandleTxPkt(void *ppkt, u16 size);
void ENET_UpdateTxDscr(u16 FrameSize);

/** ENET frames status ********************************************************/
void ENET_GetRxStatus(ENET_RxStatus * RxStatus);
void ENET_GetTxStatus(ENET_TxStatus * TxStatus);

/** ENET DMA Interrupt control functions **************************************/
void ENET_ITConfig(u32 ENET_IT, FunctionalState NewState);
u32 ENET_GetITSrc(void);
void ENET_ClearIT(u32 ENET_IT);

/** ENET PHY control **********************************************************/
void ENET_MIIWriteReg (u8 phyDev, u8 phyReg, u32  phyVal);
u32 ENET_MIIReadReg (u8 phyDev, u8 phyReg );
void ENET_PHYITConfig(u8 phyDev, u32 PHY_IT, FunctionalState NewState);
u32 ENET_PHYGetITSrc(u8 phyDev);
void ENET_PHYIsolate(u8 phyDev, FunctionalState NewState);
void ENET_PHYPowerdown(u8 phyDev, FunctionalState NewState);
void ENET_PHYLoopBack(u8 phyDev, FunctionalState NewState);
void ENET_PHYReset(u8 phyDev);


#endif  /* _ENET_H_ */

/******************** (C) COPYRIGHT 2007 STMicroelectronics *******************/
