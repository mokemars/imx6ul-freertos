/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : riic.h
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : RIIC driver header
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
*******************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef RIIC_H
#define RIIC_H

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define RIIC_CKS_DIVISION_1     (0)
#define RIIC_CKS_DIVISION_2     (1)
#define RIIC_CKS_DIVISION_4     (2)
#define RIIC_CKS_DIVISION_8     (3)
#define RIIC_CKS_DIVISION_16    (4)
#define RIIC_CKS_DIVISION_32    (5)
#define RIIC_CKS_DIVISION_64    (6)
#define RIIC_CKS_DIVISION_128   (7)

#define RIIC_TX_MODE_START      (0)
#define RIIC_TX_MODE_RESTART    (1)
#define RIIC_TX_MODE_STOP       (2)

#define RIIC_TEND_WAIT_TRANSMIT (0)
#define RIIC_TEND_WAIT_RECEIVE  (1)

#define RIIC_BUS_MASTERSHIP_WAIT_FREE   (0)
#define RIIC_BUS_MASTERSHIP_WAIT_BUSY   (1)

/******************************************************************************
Functions Prototypes
******************************************************************************/
int32_t R_RIIC_Init (uint32_t channel, uint32_t cks, uint32_t brl, uint32_t brh);
int32_t R_RIIC_SingleRead (uint32_t channel, uint8_t * pdata);
int32_t R_RIIC_SingleWrite (uint32_t channel, uint8_t data, uint32_t mode);
int32_t R_RIIC_WriteCond (uint32_t channel, uint32_t mode);
int32_t R_RIIC_Write (uint32_t channel, const uint8_t * pbuffer, uint32_t byte);
int32_t R_RIIC_Read (uint32_t channel, uint8_t * pbuffer, uint32_t byte);
int32_t R_RIIC_ReadDummy (uint32_t channel);
int32_t R_RIIC_DetectStop (uint32_t channel);
int32_t R_RIIC_ClearNack (uint32_t channel);
int32_t R_RIIC_RiInterrupt (uint32_t channel);
int32_t R_RIIC_TiInterrupt (uint32_t channel);
int32_t R_RIIC_TeiInterrupt (uint32_t channel);

void userdef_riic0_init (uint32_t cks, uint32_t brl, uint32_t brh);
void userdef_riic1_init (uint32_t cks, uint32_t brl, uint32_t brh);
void userdef_riic2_init (uint32_t cks, uint32_t brl, uint32_t brh);
void userdef_riic3_init (uint32_t cks, uint32_t brl, uint32_t brh);
void userdef_riic0_init_rx_full (void);
void userdef_riic1_init_rx_full (void);
void userdef_riic2_init_rx_full (void);
void userdef_riic3_init_rx_full (void);
void userdef_riic0_init_tx_empty (void);
void userdef_riic1_init_tx_empty (void);
void userdef_riic2_init_tx_empty (void);
void userdef_riic3_init_tx_empty (void);
void userdef_riic0_init_tx_end (void);
void userdef_riic1_init_tx_end (void);
void userdef_riic2_init_tx_end (void);
void userdef_riic3_init_tx_end (void);
void userdef_riic0_set_rx_full (void);
void userdef_riic1_set_rx_full (void);
void userdef_riic2_set_rx_full (void);
void userdef_riic3_set_rx_full (void);
void userdef_riic0_set_tx_empty (void);
void userdef_riic1_set_tx_empty (void);
void userdef_riic2_set_tx_empty (void);
void userdef_riic3_set_tx_empty (void);
void userdef_riic0_set_tx_end (void);
void userdef_riic1_set_tx_end (void);
void userdef_riic2_set_tx_end (void);
void userdef_riic3_set_tx_end (void);
void userdef_riic0_wait_rx_full (void);
void userdef_riic1_wait_rx_full (void);
void userdef_riic2_wait_rx_full (void);
void userdef_riic3_wait_rx_full (void);
void userdef_riic0_wait_tx_empty (void);
void userdef_riic1_wait_tx_empty (void);
void userdef_riic2_wait_tx_empty (void);
void userdef_riic3_wait_tx_empty (void);
void userdef_riic0_wait_tx_end (uint32_t mode);
void userdef_riic1_wait_tx_end (uint32_t mode);
void userdef_riic2_wait_tx_end (uint32_t mode);
void userdef_riic3_wait_tx_end (uint32_t mode);
void userdef_riic0_wait_bus_master (uint32_t mode);
void userdef_riic1_wait_bus_master (uint32_t mode);
void userdef_riic2_wait_bus_master (uint32_t mode);
void userdef_riic3_wait_bus_master (uint32_t mode);
void userdef_riic0_wait_stop (void);
void userdef_riic1_wait_stop (void);
void userdef_riic2_wait_stop (void);
void userdef_riic3_wait_stop (void);

/* RIIC_H */
#endif

/* End of File */
