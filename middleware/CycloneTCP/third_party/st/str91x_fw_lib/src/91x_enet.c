/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : 91x_enet.c
* Author             : MCD Application Team
* Version            : V2.0
* Date               : 12/07/2007
* Description        : This file provides all the ENET software functions.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS
* A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "91x_lib.h"
#include "string.h"  //include when using memcpy function

/* Include of other module interface headers ---------------------------------*/
/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Rx & Tx DMA Descriptors */
ENET_DMADSCRBase  dmaRxDscrBase[ENET_RXBUFNB], dmaTxDscrBase[ENET_TXBUFNB];

/* ENET buffers */
u8 RxBuff[ENET_RXBUFNB][ENET_MAX_PACKET_SIZE], TxBuff[ENET_TXBUFNB][ENET_MAX_PACKET_SIZE];

/* Rx & Tx Buffers Counters */
vu8 RxBC = 0, TxBC = 0;

/* Rx Frame status */
ENET_RxStatus RxStatus;

/* Private function prototypes -----------------------------------------------*/
extern MEMCOPY_L2S_BY4();
static void ENET_MACControlConfig(ENET_MACConfig *MAC_Config);
static void ENET_RxDscrInit(void);
static void ENET_TxDscrInit(void);

/* Interface functions -------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : ENET_MACControlConfig(ENET_MACConfig * MAC_Config)
* Description    : MAC Control Register Configuration.
* Input          : MAC_Config structure.
* Output         : None
* Return         : None
*******************************************************************************/
void ENET_MACControlConfig(ENET_MACConfig *MAC_Config)
{
  
  ENET_MAC->MCR = 0;
  
  /* ReceiveALL bit */
  if (MAC_Config->ReceiveALL == ENABLE)
  {
    ENET_MAC->MCR |= ENET_MCR_RA;
  }
  else
  {
    ENET_MAC->MCR &= ~ENET_MCR_RA;
  }

  /* MIIPrescaler */
  ENET_MAC->MCR &= ~ENET_MCR_PS;
  if ((MAC_Config->MIIPrescaler) == MIIPrescaler_2)
  {
    ENET_MAC->MCR |=0x1<<24;
  }
  
  /* Loopback mode: enable frame reception during transmission*/
  if (MAC_Config->LoopbackMode == ENABLE)
  {
    ENET_MAC->MCR |= ENET_MCR_LM;
    ENET_MAC->MCR &= ~ENET_MCR_DRO; 
  }
  else
  {
    ENET_MAC->MCR |= ENET_MCR_DRO;
  }

  /* Address filtering mode */
  ENET_MAC->MCR |= MAC_Config->AddressFilteringMode;

  /* VLAN Filtering Mode */
  ENET_MAC->MCR |= (MAC_Config->VLANFilteringMode << 15);

  /*Wrong Frame Pass */
  if (MAC_Config->PassWrongFrame == ENABLE) 
  {
    ENET_MAC->MCR |= ENET_MCR_PWF;
  }
  else 
  {
    ENET_MAC->MCR &= ~ENET_MCR_PWF;
  }

  /* Late Collision Retransmission*/
  if (MAC_Config->LateCollision == ENABLE)
  {
    ENET_MAC->MCR |= ENET_MCR_ELC;
  }
  else 
  {
    ENET_MAC->MCR &= ~ENET_MCR_ELC;
  }

  /* Broadcast Frame Reception */
  if (MAC_Config->BroadcastFrameReception == ENABLE) 
  {
    ENET_MAC->MCR &= ~ENET_MCR_DBF;
  }
  else 
  {
    ENET_MAC->MCR |=ENET_MCR_DBF;
  }

  /* PacketRetry */
  if (MAC_Config->PacketRetry == ENABLE) 
  {
    ENET_MAC->MCR &= ~ENET_MCR_DPR;
  }
  else 
  {
    ENET_MAC->MCR |= ENET_MCR_DPR;
  }

  /* RxFrameFiltering */
  if (MAC_Config->RxFrameFiltering == ENABLE) 
  {
    ENET_MAC->MCR |= ENET_MCR_RVFF;
  }
  else 
  {
    ENET_MAC->MCR &= ~ENET_MCR_RVFF;
  }

  /* AutomaticPadRemoval */
  if (MAC_Config->AutomaticPadRemoval == ENABLE) 
  {
    ENET_MAC->MCR |= ENET_MCR_APR;
  }
  else 
  {
    ENET_MAC->MCR &= ~ENET_MCR_APR;
  }

  /* DefferalCheck */
  if (MAC_Config->DeferralCheck == ENABLE) 
  {
    ENET_MAC->MCR |= ENET_MCR_DCE;
  }
  else 
  {
    ENET_MAC->MCR &= ~ENET_MCR_DCE;
  }
}

/*******************************************************************************
* Function Name  : ENET_GetRxStatus
* Description    : Get Rx status flags.
* Input          : RxStatus : pointer to structure of type ENET_RxStatus.
* Output         : None
* Return         : None
*******************************************************************************/
void ENET_GetRxStatus(ENET_RxStatus * RxStatus)
{
  u32 regValue;

  regValue = ENET_MAC->MRS;

  RxStatus->FrameAborted      = (FlagStatus)((ENET_MRS_FA & regValue) >> 31);
  RxStatus->PacketFilter      = (FlagStatus)((ENET_MRS_PF & regValue) >> 30);
  RxStatus->FilteringFail     = (FlagStatus)((ENET_MRS_FF & regValue) >> 29);
  RxStatus->BroadCastFrame    = (FlagStatus)((ENET_MRS_BF & regValue) >> 28);
  RxStatus->MulticastFrame    = (FlagStatus)((ENET_MRS_MCF & regValue) >> 27);
  RxStatus->UnsupportedControFrame = (FlagStatus)((ENET_MRS_UCF & regValue) >> 26);
  RxStatus->ControlFrame      = (FlagStatus)((ENET_MRS_CF & regValue) >> 25);
  RxStatus->LengthError       = (FlagStatus)((ENET_MRS_LE & regValue) >> 24);
  RxStatus->Vlan2Tag          = (FlagStatus)((ENET_MRS_VL2 & regValue) >> 23);
  RxStatus->Vlan1Tag          = (FlagStatus)((ENET_MRS_VL1 & regValue) >> 22);
  RxStatus->CRCError          = (FlagStatus)((ENET_MRS_CE & regValue) >> 21);
  RxStatus->ExtraBit          = (FlagStatus)((ENET_MRS_EB & regValue) >> 20);
  RxStatus->MIIError          = (FlagStatus)((ENET_MRS_ME & regValue) >> 19);
  RxStatus->FrameType         = (FlagStatus)((ENET_MRS_FT & regValue) >> 18);
  RxStatus->LateCollision     = (FlagStatus)((ENET_MRS_LC & regValue) >> 17);
  RxStatus->OverLength        = (FlagStatus)((ENET_MRS_OL & regValue) >> 16);
  RxStatus->RuntFrame         = (FlagStatus)((ENET_MRS_RF & regValue) >> 15);
  RxStatus->WatchDogTimout    = (FlagStatus)((ENET_MRS_WT & regValue) >> 14);
  RxStatus->FalseCarrierIndication = (FlagStatus)((ENET_MRS_FCI & regValue) >> 13);
  RxStatus->FrameLength       = (u16)(ENET_MRS_FL & regValue);

}

/*******************************************************************************
* Function Name  : ENET_GetTxStatus
* Description    : Get Tx status flags.
* Input          : TxStatus: pointer on structure of type ENET_TxStatus.
* Output         : None
* Return         : None
*******************************************************************************/
void ENET_GetTxStatus(ENET_TxStatus * TxStatus)
{
  u32 regValue;

  regValue = ENET_MAC->MTS;

  TxStatus->PacketRetry        = (FlagStatus)((ENET_MTS_PR&regValue) >> 31);
  TxStatus->ByteCount          = (u16)((ENET_MTS_BC&regValue) >> 18);
  TxStatus->CollisionCount     = (u8)((ENET_MTS_CC&regValue) >> 10);
  TxStatus->LateCollisionObserved = (FlagStatus)((ENET_MTS_LCO&regValue) >> 9);
  TxStatus->Deffered           = (FlagStatus)((ENET_MTS_DEF&regValue) >> 8);
  TxStatus->UnderRun           = (FlagStatus)((ENET_MTS_UR&regValue) >> 7);
  TxStatus->ExcessiveCollision = (FlagStatus)((ENET_MTS_EC&regValue) >> 6);
  TxStatus->LateCollision      = (FlagStatus)((ENET_MTS_LC&regValue) >> 5);
  TxStatus->ExcessiveDefferal  = (FlagStatus)((ENET_MTS_ED&regValue) >> 4);
  TxStatus->LossOfCarrier      = (FlagStatus)((ENET_MTS_LOC&regValue) >> 3);
  TxStatus->NoCarrier          = (FlagStatus)((ENET_MTS_NC&regValue) >> 2);
  TxStatus->FrameAborted       = (FlagStatus)(ENET_MTS_FA&regValue);
}

/*******************************************************************************
* Function Name  : ENET_SetOperatingMode
* Description    : Sets the Operating mode.
* Input          : ENET_OperatingMode: the operating mode of MAC. This parameter
*                  can be any the following values:
*                       - AUTO_NEGOTIATION
*                       - FULLDUPLEX_100M
*                       - HALFDUPLEX_100M
*                       - FULLDUPLEX_10M
*                       - HALFDUPLEX_10M
* Output         : None
* Return         : None
*******************************************************************************/
void ENET_SetOperatingMode(u32 ENET_OperatingMode)
{
  u32 regValue;
  
  if(ENET_OperatingMode == AUTO_NEGOTIATION)
  {
    /* The Link Status bit is a Latching High bit, so we need to read twice to get the real value */
    ENET_MIIReadReg (PHY_ADDRESS, PHY_XSR);
  
    /* We wait for linked satus... */
    while( !(ENET_MIIReadReg (PHY_ADDRESS, PHY_XSR) & PHY_Linked_Status) );
  
    /* Enable Auto-Negotiation */
    ENET_MIIWriteReg (PHY_ADDRESS, PHY_XCR, PHY_AutoNegotiation);
  
    /* Wait until the autonegotiation will be completed */
    while( !(ENET_MIIReadReg (PHY_ADDRESS, PHY_XSR) & PHY_AutoNego_Complete) );
  
    /* Read the result of the autonegotiation */
    regValue = ENET_MIIReadReg (PHY_ADDRESS, PHY_XCIIS);
  
    /* Configure the MAC with the Duplex Mode fixed by the autonegotiation process */
    if( regValue & PHY_Configured_Speed)
    {
      ENET_MAC->MCR |=ENET_MCR_FDM;   /* Full Duplex Mode */
      ENET_MAC->MCR &=~ENET_MCR_DRO;  /* Enable frame reception during transmission */
    }
    else
    {
      ENET_MAC->MCR &=~ENET_MCR_FDM; /* Half Duplex Mode */
      ENET_MAC->MCR |=ENET_MCR_DRO;  /* Disable frame reception during transmission */
    }
  }
  else
  {
    if (ENET_OperatingMode==PHY_FULLDUPLEX_100M ||ENET_OperatingMode==PHY_FULLDUPLEX_10M )
    {
      ENET_MAC->MCR |=ENET_MCR_FDM;   /* Full Duplex Mode */
      ENET_MAC->MCR &=~ENET_MCR_DRO;  /* Enable Frame Reception During Transmission */
    }
    else
    {
      ENET_MAC->MCR &=~ENET_MCR_FDM; /* Half Duplex Mode */
      ENET_MAC->MCR |=ENET_MCR_DRO;  /* Disable Frame Reception During Transmission */
    }
  
    /* Set the operating mode in the PHY device */
    ENET_MIIWriteReg(PHY_ADDRESS,PHY_XCR, ENET_OperatingMode);

  }  
}

/*******************************************************************************
* Function Name  : ENET_MIIWriteReg
* Description    : Writes a value on the PHY registers.
* Input          : - phyDev PHY device address.
*                  - phyReg PHY register to be written.
*                  - phyVal PHY register value.
* Output         : None
* Return         : None
*******************************************************************************/
void ENET_MIIWriteReg (u8 phyDev, u8 phyReg, u32  phyVal)
{
  u32 address;
  u32 result;     /* temporary result for address register status */
  u32 timeout;

  /* Prepare the MII register address */
  address = 0;
  address |= ((phyDev<<11) & ENET_MIIA_PADDR); /* set the PHY address */
  address |= ((phyReg<<6) & ENET_MIIA_RADDR); /* select the corresponding register */
  address |= ENET_MIIA_WR;  /* in write mode */
  address |= ENET_MIIA_BUSY;

  /* Check for the Busy flag */
  timeout=0;
  do
  {
    timeout++;
    result = ENET_MAC->MIIA;
  } while ((result & ENET_MIIA_BUSY) && (timeout < (u32 )MII_WRITE_TO));

  /* Give the value to the MII data register */
  ENET_MAC->MIID = (phyVal & ENET_MIID_RDATA);

  /* Write the result value into the MII Address register */
  ENET_MAC->MIIA = address;

  /* Check for the Busy flag */
  timeout=0;
  do
  {
    timeout++;
    result = ENET_MAC->MIIA;
  } while ((result & ENET_MIIA_BUSY) && (timeout < (u32 )MII_WRITE_TO));

}

/*******************************************************************************
* Function Name  : ENET_MIIReadReg
* Description    : Writes a value on the PHY
* Input          : - phyDev PHY device address
*                  - phyReg PHY register to be read
* Output         : None
* Return         : The read value (16 bits)
*******************************************************************************/
u32 ENET_MIIReadReg (u8 phyDev, u8 phyReg )
{
  u32 regValue;
  u32 address;
  u32 result;     /* temporary result for address register status */
  u32 timeout;

  /* Prepare the MII register address */
  address = 0;
  address |= ((phyDev<<11) & ENET_MIIA_PADDR); /* set the PHY address */
  address |= ((phyReg<<6) & ENET_MIIA_RADDR); /* select the corresponding register */
  address &= ~ENET_MIIA_WR;                   /* ... in read mode */
  address |= ENET_MIIA_BUSY;

  /* Check for the Busy flag */
  timeout = 0;

  do
  {
    timeout++;
    result = ENET_MAC->MIIA;
  } while ((result & ENET_MIIA_BUSY) && (timeout < (u32 )MII_READ_TO));

  /* write the result value into the MII Address register */
  ENET_MAC->MIIA = address;

  /* Check for the Busy flag */
  timeout = 0;

  do
  {
    timeout++;
    result = ENET_MAC->MIIA;
  } while ((result & ENET_MIIA_BUSY) && (timeout < (u32 )MII_READ_TO));

  /* Read the result value from data register*/
  regValue = ENET_MAC->MIID;

  return (regValue & ENET_MIID_RDATA);
}

/*******************************************************************************
* Function Name  : ENET_RxDscrInit
* Description    : Initializes the Rx ENET descriptor chain.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ENET_RxDscrInit(void)
{
  int i;
  
  for(i=0; i < ENET_RXBUFNB; )
  {
    /* Assign temp Rx array to the ENET buffer */
    dmaRxDscrBase[i].dmaAddr = (u32)&RxBuff[i][0];
    
    /* Initialize ENET RX Control */
    dmaRxDscrBase[i].dmaCntl = ENET_MAX_PACKET_SIZE | ENET_RXCR_NXTEN;
    
    /* Setting the VALID bit */
    dmaRxDscrBase[i].dmaPackStatus = ENET_DSCR_RX_STATUS_VALID_MSK;

    /* Initialize the next descriptor with the Next Descriptor Polling Enable */
    if(i < ENET_RXBUFNB-1)
    {
      dmaRxDscrBase[i].dmaNext = (u32) &dmaRxDscrBase[i+1] | ENET_RXNDAR_NPOL_EN;
    }
    else
    {
      dmaRxDscrBase[i].dmaNext = (u32) &dmaRxDscrBase[0] | ENET_RXNDAR_NPOL_EN;
    }
    
    i++;
  }
  
  /* Setting the RX NEXT Descriptor Register inside the ENET with the Next Descriptor Polling Enable*/
  ENET_DMA->RXNDAR = ((u32)&dmaRxDscrBase[0]) | ENET_RXNDAR_NPOL_EN;
  
}

/*******************************************************************************
* Function Name  : ENET_TxDscrInit
* Description    : Initializes the Tx ENET descriptor chain
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ENET_TxDscrInit(void)
{
  int i;
  
  for(i=0; i < ENET_TXBUFNB; )
  {
    /* ENET DMA Start Address */
    dmaTxDscrBase[i].dmaAddr = (u32)TxBuff[i];

    /* Initialize ENET TX Control */
    dmaTxDscrBase[i].dmaCntl = ENET_TXCR_NXTEN;
  
    /* Initialize the Packet Status */
    dmaTxDscrBase[i].dmaPackStatus = 0;
  
    /* Initialize the next descriptor with the Next Descriptor Polling Enable */
    if(i < ENET_TXBUFNB-1)
    {
      dmaTxDscrBase[i].dmaNext = (u32) &dmaTxDscrBase[i+1] | ENET_TXNDAR_NPOL_EN;
    }
    else
    {
      dmaTxDscrBase[i].dmaNext = (u32) &dmaTxDscrBase[0] | ENET_TXNDAR_NPOL_EN;
    }
    
    i++;
  }

  /* Setting the TX NEXT Descriptor Register inside the ENET with the Next Descriptor Polling Enable*/
  ENET_DMA->TXNDAR = ((u32)dmaTxDscrBase) | ENET_TXNDAR_NPOL_EN;
  
}

/*******************************************************************************
* Function Name  : ENET_Init
* Description    : ENET MAC, DMA and PHY device initializations
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ENET_Init (ENET_MACConfig *MAC_Config)
{

  u32 i;
  u32 regValue;
  u32 macAddrLow, macAddrHigh;
  
  /* MAC initialization ------------------------------------------------------*/
  
  /* De-assert the SRESET bit of ENET + MAC devices */
  ENET_DMA->SCR &= ~ENET_SCR_SRESET;
 
  /* Configure MAC control register */
  ENET_MACControlConfig(MAC_Config);
  
  /* MAC address low setting */
  macAddrLow  = (MAC_ADDR3 << 24) + (MAC_ADDR2 << 16) + \
                (MAC_ADDR1 << 8) + MAC_ADDR0;
                                        
  /* MAC address high setting */                    
  macAddrHigh = (MAC_ADDR5 << 8) + MAC_ADDR4;
                                          
  /* Write the MAC address Low Register */
  ENET_MAC->MAH = macAddrHigh;                  
                                            
  /* Write the MAC address High Register */ 
  ENET_MAC->MAL = macAddrLow;
  
  /* Multicast address low setting */
  macAddrLow  = (MCAST_ADDR3 << 24) + (MCAST_ADDR2 << 16) + \
                (MCAST_ADDR1 << 8) + MCAST_ADDR0;
                                        
  /* Multicast address high setting */                    
  macAddrHigh = (MCAST_ADDR5 << 8) + MCAST_ADDR4;
                                          
  /* Write the Multicast address Low Register */
  ENET_MAC->MCHA = macAddrHigh;                  
                                            
  /* Write the Multicast address High Register */ 
  ENET_MAC->MCLA = macAddrLow;
  
  /* VLAN initialization */
  ENET_MAC->VL1 = (VLANID1 << 16) | VLANTAG1;
  ENET_MAC->VL2 = (VLANID2 << 16) | VLANTAG2;
    
  /* ENET DMA initialisation -------------------------------------------------*/ 
  
  /* Read the ENET DMA Status and Control Register */
  regValue = ENET_DMA->SCR;

  /* Setup Tx Max burst size */
  regValue &= ~(u32)ENET_SCR_TX_MAX_BURST_SZ;
  regValue |= (u32)ENET_SCR_TX_MAX_BURST_SZ_VAL;

  /* Setup Rx Max Burst size */
  regValue &= ~(u32)ENET_SCR_RX_MAX_BURST_SZ;
  regValue |= (u32)ENET_SCR_RX_MAX_BURST_SZ_VAL;

  /* Write Tx & Rx burst size to the ENET status and control register */
  ENET_DMA->SCR = regValue;

  /* Initialize Rx and Tx descriptors in memory */
  ENET_TxDscrInit();
  ENET_RxDscrInit();                                          
 
  /* PHY DEVICE initialization and OPERATING MODE setting --------------------*/
  
  /* Put the PHY in reset mode */
  ENET_MIIWriteReg(PHY_ADDRESS,PHY_XCR, PHY_Reset_Control);

  /* Delay to assure PHY reset */
  for(i=PHY_ResetDelay; i!=0; i--)
  {
    regValue = (u32) i;
  }

}

/*******************************************************************************
* Function Name  : ENET_Start
* Description    : Enables ENET MAC reception / transmission & starts DMA fetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  ENET_Start ( void)
{
  u32 regValue;
  
  /* Force a ENET abort by software for the receive block */
   ENET_DMA->RXSTR &= ~ENET_RXSTR_DMA_EN;

  /* Force a ENET abort by software for the transmit block */
   ENET_DMA->TXSTR &= ~ENET_TXSTR_DMA_EN;

  /* Reset all interrupts */
  ENET_DMA->ISR = ENET_IT_All;

  /* Setup Descriptor Fetch ENET_PhyDelay for Receive Block */
  regValue = ENET_DMA->RXSTR;
  regValue &= ~ENET_RXSTR_DFETCH_DLY;
  regValue |= ENET_RXSTR_DFETCH_DEFAULT;
  ENET_DMA->RXSTR=  regValue;

  /* Setup Descriptor Fetch ENET_PhyDelay for Transmit Block */
  regValue = ENET_DMA->TXSTR;
  regValue &= ~ENET_TXSTR_DFETCH_DLY;
  regValue |= ENET_TXSTR_DFETCH_DEFAULT;
  ENET_DMA->TXSTR = regValue;

  /* Set Tx underrun bit */
  regValue &= ~ENET_TXSTR_URUN;
  regValue |= ENET_TXSTR_URUN;
  ENET_DMA->TXSTR = regValue;

  /* Clear the interrupts */
  ENET_DMA->IER = ~ENET_IT_All;

  /* MAC TX enable */
  ENET_MAC->MCR|= ENET_MCR_TE;

  /* MAC RX enable */
  ENET_MAC->MCR|= ENET_MCR_RE;

  /* Start the RX DMA Fetch */
  ENET_DMA->RXSTR|= ENET_RXSTR_FETCH;
  
}

/*******************************************************************************
* Function Name  : ENET_RxBuff_Free
* Description    : Gives the current buffer back to ENET DMA
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ENET_UpdateRxDscr(void)
{
  /* Give the buffer back to ENET */
  dmaRxDscrBase[RxBC].dmaPackStatus = ENET_DSCR_RX_STATUS_VALID_MSK;
  
  /* Update the Rx Buffers Counter */
  RxBC++;
  if( RxBC >=ENET_RXBUFNB ) 
  {
    RxBC=0;
  }
}

/********************************************************************************
* Function Name  : ENET_HandleRxPkt
* Description    : Receives a packet and copies it to memory pointed by ppkt.
* Input          : ppkt: pointer on application receive buffer.
* Output         : None
* Return         : ERROR - If there is no packet
*                : The size of the received packet  - If there is a packet
*******************************************************************************/
u32 ENET_HandleRxPkt ( void *ppkt)
{ 
  u16 size;

  /* Check if the descriptor is owned by the ENET or CPU */
  if(dmaRxDscrBase[RxBC].dmaPackStatus & ENET_DSCR_RX_STATUS_VALID_MSK)
  {
    /* Return error */
    return ERROR;
  }
  
  /* Get the size of the packet */
  size = ((dmaRxDscrBase[RxBC].dmaPackStatus & 0x7ff) - 4);
  
  /* Get the status of the packet */
  ENET_GetRxStatus(&RxStatus);
    
  if((size!=0) && (RxStatus.FrameAborted==RESET))
  {
    /* Copy the received frame into the ENET DMA current Rx buffer */
      MEMCOPY_L2S_BY4((u8*)ppkt, RxBuff[RxBC], size); /* optimized memcopy function */ 
    //memcpy(ppkt, RxBuff[RxBC], size); /* string.h library */
  }
  else
  {
    size = ERROR;
  }
  
  /* Give the buffer back to the ENET DMA */
  ENET_UpdateRxDscr();

  /* Return no error */
  return (size);
}

/*******************************************************************************
* Function Name  : ENET_UpdateTxDscr
* Description    : Update the current ENET DMA descriptor to start sending frame
* Input          : FrameSize: the size of the frame to be sent 
* Output         : None
* Return         : None
* Precondition   : Frame to be sent, must has been copied in the ENET DMA buffer
*******************************************************************************/
void ENET_UpdateTxDscr(u16 FrameSize)
{    
  static u32 i=0;
  
  /* Setting the Frame Length*/
  dmaTxDscrBase[TxBC].dmaCntl = (FrameSize&0xFFF) | ENET_TXCR_NXTEN;
    
  /* Start the transmission by setting the VALID bit in the dmaPackStatus word*/
  /* of the current descriptor */
  dmaTxDscrBase[TxBC].dmaPackStatus = ENET_DSCR_TX_STATUS_VALID_MSK;
  
  /* Update the Tx Buffers Counter */
  TxBC++;
  if(TxBC >= ENET_TXBUFNB)
  {
    TxBC=0;
  }
  
  /* If it's the first frame to be sent, we start the ENET_DMA descriptor */
  /* fetch. After that, as soon as a DMA transfer is completed, a new ENET */
  /* DMA descriptor is fetched */
  if(i==0)
  {
    ENET_DMA->TXSTR|= ENET_TXSTR_FETCH;
    i++;
  }
}

/*******************************************************************************
* Function Name  : ENET_HandleTxPkt
* Description    : Transmits a packet, from application buffer, pointed by ppkt.
* Input          : - ppkt: pointer to application packet Buffer.
*                  - size: Tx Packet size.
* Output         : None
* Return         : None
*******************************************************************************/
void ENET_HandleTxPkt(void *ppkt, u16 size)
{ 
  /* If the CPU still work on the buffer, we wait...*/   
  while((dmaTxDscrBase[TxBC].dmaPackStatus & ENET_DSCR_TX_STATUS_VALID_MSK));
  
  /* Copy the frame to be sent into the ENET DMA current Tx buffer */  
  MEMCOPY_L2S_BY4( TxBuff[TxBC], ppkt, size); /* optimized memcopy function */
  //memcpy( TxBuff[TxBC], ppkt, size);  /* string.h library */
  
  /* Update the ENET DMA current descriptor */
  ENET_UpdateTxDscr(size);
   
}

/*******************************************************************************
* Function Name  : ENET_ITConfig
* Description    : Enables or disables the specified ENET DMA interrupt.
* Input          : - ENET_IT: specifies the ENET DMA interrupts sources to be
*                    enabled or disabled. This parameter can be any combination
*                    of the following values:
*                         + ENET_IT_TX_CURR_DONE
*                         + ENET_IT_MAC_INT                  
*                         + ENET_IT_TX_MERR_INT              
*                         + ENET_IT_TX_DONE                  
*                         + ENET_IT_TX_NEXT                  
*                         + ENET_IT_TX_TO                    
*                         + ENET_IT_TX_ENTRY                 
*                         + ENET_IT_TX_FULL                  
*                         + ENET_IT_TX_EMPTY                 
*                         + ENET_IT_RX_CURR_DONE             
*                         + ENET_IT_RX_MERR_INT              
*                         + ENET_IT_RX_DONE                  
*                         + ENET_IT_RX_NEXT                  
*                         + ENET_IT_PACKET_LOST              
*                         + ENET_IT_RX_TO                    
*                         + ENET_IT_RX_ENTRY                 
*                         + ENET_IT_RX_FULL                  
*                         + ENET_IT_RX_EMPTY                 
*
*                  - NewState: new state of the specified ENET DMA interrupt.
* Output         : None
* Return         : None
*******************************************************************************/
void ENET_ITConfig(u32 ENET_IT, FunctionalState NewState)
{
  u32 regValue;
  
  /* Get the ENET_IER register value */
  regValue = ENET_DMA->IER; 
  
  if (NewState == ENABLE) 
  {
    /* Enable the specified interrupt */
    regValue |= ENET_IT;
  }
  else
  {
    /* Disable the specified interrupt */
    regValue &= ~ENET_IT; 
  }
  
  /* Update the ENET_IER register with the new state */
  ENET_DMA->IER = regValue; 
}

/*******************************************************************************
* Function Name  : ENET_GetITSrc
* Description    : Specifies the source of the ENET DMA interrupt.
* Input          : None.
* Output         : None.
* Return         : ENET DMA interrupt source.
*******************************************************************************/
u32 ENET_GetITSrc(void)
{
  u32 regValue;
  
  /* Read the ENET_ISR register to get the source of the interrupt */
  regValue = ENET_DMA->ISR; 
  
  /* Return the source of the interrupt */
  return(regValue);  
}

/*******************************************************************************
* Function Name  : ENET_ClearIT
* Description    : Clears the ENET DMA's interrupt pending bit.
* Input          : ENET_IT: specifies the interrupt pending bit to clear.
*                  This parameter can be any combination of the following values:
*                         + ENET_IT_TX_CURR_DONE
*                         + ENET_IT_MAC_INT                  
*                         + ENET_IT_TX_MERR_INT              
*                         + ENET_IT_TX_DONE                  
*                         + ENET_IT_TX_NEXT                  
*                         + ENET_IT_TX_TO                    
*                         + ENET_IT_TX_ENTRY                 
*                         + ENET_IT_TX_FULL                  
*                         + ENET_IT_TX_EMPTY                 
*                         + ENET_IT_RX_CURR_DONE             
*                         + ENET_IT_RX_MERR_INT              
*                         + ENET_IT_RX_DONE                  
*                         + ENET_IT_RX_NEXT                  
*                         + ENET_IT_PACKET_LOST              
*                         + ENET_IT_RX_TO                    
*                         + ENET_IT_RX_ENTRY                 
*                         + ENET_IT_RX_FULL                  
*                         + ENET_IT_RX_EMPTY              
* Output         : None
* Return         : None
*******************************************************************************/
void ENET_ClearIT(u32 ENET_IT)
{
  /* Clear the specified interrupt by writing 1 in the corresponding pending */
  /* bit */
  ENET_DMA->ISR |= ENET_IT; 
}

/*******************************************************************************
* Function Name  : ENET_PHYITConfig
* Description    : Enables or disables the specified PHY interrupt.
* Input          : - phyDev: specifies the PHY device address.
*                  - PHY_IT: specifies the PHY interrupt source to be configured.
*                  - NewState: new state of the specified PHY interrupt.
* Output         : None
* Return         : None
*******************************************************************************/
void ENET_PHYITConfig(u8 phyDev, u32 PHY_IT, FunctionalState NewState)
{
  u32 regValue;
  
  if( NewState == ENABLE )                             
  {
    /* Enable the specified PHY_IT */
    ENET_MIIWriteReg(phyDev,PHY_XIE, PHY_IT);  
  }  
  else                               
  {
    /* Get the XIE register valur */
    regValue = ENET_MIIReadReg(phyDev,PHY_XIE);
    
    /* Disable, only, the specified PHY_IT */
    regValue &= ~PHY_IT;
    ENET_MIIWriteReg(phyDev,PHY_XIE, regValue); 
  }
}

/*******************************************************************************
* Function Name  : ENET_PHYGetITSrc
* Description    : Specifies the source of the PHY interrupt.
* Input          : phyDev: specifies the PHY device address.
* Output         : None
* Return         : PHY source interrupt.
*******************************************************************************/
u32 ENET_PHYGetITSrc(u8 phyDev)
{
  u32 regValue;
  
  /* Read the XCIIS register */
  regValue = ENET_MIIReadReg(phyDev,PHY_XCIIS); 
  
  /* Subtract the configuration part */
  regValue &= ~PHY_ConfigurationMask; 
  
  /* Return the interrupt status */
  return (regValue);
}

/*******************************************************************************
* Function Name  : ENET_PHYIsolate
* Description    : Isolates PHY from MII.
* Input          : - phyDev: specifies the PHY device address.
*                  - NewState: new state of the link PHY/MII.
* Output         : None
* Return         : None
*******************************************************************************/
void ENET_PHYIsolate(u8 phyDev, FunctionalState NewState)
{
  u32 regValue;
  
  /* Get the PHY configuration to update it */
  regValue = ENET_MIIReadReg(phyDev, PHY_XCR); 
  
  if (NewState == ENABLE)
  {
    /* Isolate the PHY from the MII */
    regValue |= PHY_Isolate;  
  }
  else
  {
    /* Restablish the link MII/PHY */
    regValue &= ~PHY_Isolate;  
  }
  
  /* Update the PHY control register with the new configuration */
  ENET_MIIWriteReg(phyDev, PHY_XCR, regValue); 
}

/*******************************************************************************
* Function Name  : ENET_PHYPowerdown
* Description    : Puts the PHY device in the power-down mode.
* Input          : - phyDev: specifies the PHY device address.
*                  - NewState: new state of the PHY power mode.
* Output         : None
* Return         : None
*******************************************************************************/
void ENET_PHYPowerdown(u8 phyDev, FunctionalState NewState)
{
  u32 regValue;
  
  /* Get the PHY configuration to update it */
  regValue = ENET_MIIReadReg(phyDev, PHY_XCR); 
  
  if (NewState == ENABLE)       
  {
    /* Put the PHY in the power-down mode */
    regValue |= PHY_Powerdown;  
  }
  else
  {
    /* Put the PHY in the normal power mode */
    regValue &= ~PHY_Powerdown; 
  }
  
  /* Update the PHY control register with the new configuration */
  ENET_MIIWriteReg(phyDev, PHY_XCR, regValue);                                               
}

/*******************************************************************************
* Function Name  : ENET_PHYLoopBack
* Description    : Enables or disables the PHY loopBack mode.
* Input          : - phyDev: specifies the PHY device address.
*                  - NewState: new state of the PHY peripheral.
* Output         : Non
* Return         : None
*******************************************************************************/
void ENET_PHYLoopBack(u8 phyDev, FunctionalState NewState)
{
  u32 regValue;
  
  /* Get the PHY configuration to update it */
  regValue = ENET_MIIReadReg(phyDev, PHY_XCR); 
  
  if (NewState == ENABLE)
  {
    /* Enable the loopback mode */
    regValue |= PHY_Loopback;  
  }
  else
  {
    /* Put the PHY in the normal mode */
    regValue &= ~PHY_Loopback;
  }
  
  /* Update the PHY control register with the new configuration */
  ENET_MIIWriteReg(phyDev, PHY_XCR, regValue);
  ENET_MIIReadReg(phyDev, PHY_XCR);
}

/*******************************************************************************
* Function Name  : ENET_PHYReset
* Description    : PHY software reset.
* Input          : phyDev: specifies the PHY device address.
* Output         : None.
* Return         : None.
*******************************************************************************/
void ENET_PHYReset(u8 phyDev)
{
  /* Reset the PHY device */
  ENET_MIIWriteReg(phyDev, PHY_XCR, PHY_Reset_Control);
  
  /* Wait until the reset will be completed */
  while( ENET_MIIReadReg(phyDev, PHY_XCR) & PHY_Reset_Control);
}

/******************** (C) COPYRIGHT 2007 STMicroelectronics *******************/


