/**
 * @file mimxrt1052_eth_driver.h
 * @brief i.MX RT1050 Ethernet MAC controller
 *
 * @section License
 *
 * Copyright (C) 2010-2017 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Open.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.0
 **/

#ifndef _MIMX6UL_ETH_DRIVER_H
#define _MIMX6UL_ETH_DRIVER_H

//Config Use ENET1
#define FSL_USE_ENET1 1
//Config Use ENET2
#define FSL_USE_ENET2 1

//Number of TX buffers
#ifndef MIMX6UL_ETH_TX_BUFFER_COUNT
   #define MIMX6UL_ETH_TX_BUFFER_COUNT 8
#elif (MIMX6UL_ETH_TX_BUFFER_COUNT < 1)
   #error MIMX6UL_ETH_TX_BUFFER_COUNT parameter is not valid
#endif

//TX buffer size
#define MIMX6UL_ETH_TX_BUFFER_SIZE ENET_FRAME_MAX_FRAMELEN 
/*#ifndef MIMX6UL_ETH_TX_BUFFER_SIZE
   #define MIMX6UL_ETH_TX_BUFFER_SIZE 1536
#elif (MIMX6UL_ETH_TX_BUFFER_SIZE != 1536)
   #error MIMX6UL_ETH_TX_BUFFER_SIZE parameter is not valid
#endif*/

//Number of RX buffers
#ifndef MIMX6UL_ETH_RX_BUFFER_COUNT
   #define MIMX6UL_ETH_RX_BUFFER_COUNT 8
#elif (MIMX6UL_ETH_RX_BUFFER_COUNT < 1)
   #error MIMX6UL_ETH_RX_BUFFER_COUNT parameter is not valid
#endif

//RX buffer size
#define MIMX6UL_ETH_RX_BUFFER_SIZE ENET_FRAME_MAX_FRAMELEN 
/*#ifndef MIMX6UL_ETH_RX_BUFFER_SIZE
   #define MIMX6UL_ETH_RX_BUFFER_SIZE 1536
#elif (MIMX6UL_ETH_RX_BUFFER_SIZE != 1536)
   #error MIMX6UL_ETH_RX_BUFFER_SIZE parameter is not valid
#endif*/

//Ethernet interrupt group priority
#ifndef MIMX6UL_ETH_IRQ_PRIORITY
   #define MIMX6UL_ETH_IRQ_PRIORITY 21
#elif (MIMX6UL_ETH_IRQ_PRIORITY < 0)
   #error MIMX6UL_ETH_IRQ_GROUP_PRIORITY parameter is not valid
#endif

#ifndef MIMX6UL_ENET_PRIORITY
    #define MIMX6UL_ENET_PRIORITY       (21U)
#endif
#ifndef MIMX6UL_ENET_1588_PRIORITY
    #define MIMX6UL_ENET_1588_PRIORITY  (20U)
#endif


typedef struct ethernetif_config
{
    uint32_t phyAddress;
    clock_name_t clockName;
} ethernetif_config_t;

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif

#if defined(FSL_USE_ENET1)
//i.MX6UL ENET1 Ethernet MAC driver
extern const NicDriver mimx6ulEthPort1Driver;
//i.MX6UL Ethernet MAC related functions
error_t mimx6ulEthInitPort1(NetInterface *interface);
void mimx6ulEthInitGpioPort1(NetInterface *interface);

void mimx6ulEthTickPort1(NetInterface *interface);

void mimx6ulEthEnableIrqPort1(NetInterface *interface);
void mimx6ulEthDisableIrqPort1(NetInterface *interface);
void mimx6ulEthEventHandlerPort1(NetInterface *interface);

error_t mimx6ulEthSendPacketPort1(NetInterface *interface,
   const NetBuffer *buffer, size_t offset);

error_t mimx6ulEthReceivePacketPort1(NetInterface *interface);

error_t mimx6ulEthSetMulticastFilterPort1(NetInterface *interface);
error_t mimx6ulEthUpdateMacConfigPort1(NetInterface *interface);

void mimx6ulEthWritePhyRegPort1(uint8_t phyAddr, uint8_t regAddr, uint16_t data);
uint16_t mimx6ulEthReadPhyRegPort1(uint8_t phyAddr, uint8_t regAddr);
#endif
#if defined(FSL_USE_ENET2)
//i.MX6UL ENET2 Ethernet MAC driver
extern const NicDriver mimx6ulEthPort2Driver;
//i.MX6UL Ethernet MAC related functions
error_t mimx6ulEthInitPort2(NetInterface *interface);
void mimx6ulEthInitGpioPort2(NetInterface *interface);

void mimx6ulEthTickPort2(NetInterface *interface);

void mimx6ulEthEnableIrqPort2(NetInterface *interface);
void mimx6ulEthDisableIrqPort2(NetInterface *interface);
void mimx6ulEthEventHandlerPort2(NetInterface *interface);

error_t mimx6ulEthSendPacketPort2(NetInterface *interface,
   const NetBuffer *buffer, size_t offset);

error_t mimx6ulEthReceivePacketPort2(NetInterface *interface);

error_t mimx6ulEthSetMulticastFilterPort2(NetInterface *interface);
error_t mimx6ulEthUpdateMacConfigPort2(NetInterface *interface);

void mimx6ulEthWritePhyRegPort2(uint8_t phyAddr, uint8_t regAddr, uint16_t data);
uint16_t mimx6ulEthReadPhyRegPort2(uint8_t phyAddr, uint8_t regAddr);
#endif
//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
