/**
 * @file mimx6ul_eth_driver.c
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

//Switch to the appropriate trace level
#define TRACE_LEVEL NIC_TRACE_LEVEL

//Dependencies
#include "fsl_device_registers.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "core/net.h"
#include "drivers/mac/mimx6ul_eth_driver.h"
#include "debug.h"
#include "fsl_enet.h"
#include "drivers/phy/fsl_phy.h"
#include "FreeRTOS.h"
#include "event_groups.h"

/* System clock name. */
#define ENET_CLOCK_NAME kCLOCK_AhbClk

//#define DRIVER_ENET_BUFF_ALIGNMENT MAX(ENET_BUFF_ALIGNMENT, FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)

#if defined(FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL) && FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL
    #if defined(FSL_FEATURE_L2CACHE_LINESIZE_BYTE) \
        && ((!defined(FSL_SDK_DISBLE_L2CACHE_PRESENT)) || (FSL_SDK_DISBLE_L2CACHE_PRESENT == 0))
        #if defined(FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
            #define FSL_CACHE_LINESIZE_MAX MAX(FSL_FEATURE_L1DCACHE_LINESIZE_BYTE, FSL_FEATURE_L2CACHE_LINESIZE_BYTE)
            #define DRIVER_ENET_BUFF_ALIGNMENT MAX(ENET_BUFF_ALIGNMENT, FSL_CACHE_LINESIZE_MAX)
        #else
            #define DRIVER_ENET_BUFF_ALIGNMENT MAX(ENET_BUFF_ALIGNMENT, FSL_FEATURE_L2CACHE_LINESIZE_BYTE)
        #endif
    #elif defined(FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
        #define DRIVER_ENET_BUFF_ALIGNMENT MAX(ENET_BUFF_ALIGNMENT, FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
    #else
        #define DRIVER_ENET_BUFF_ALIGNMENT ENET_BUFF_ALIGNMENT
    #endif
#else
    #define DRIVER_ENET_BUFF_ALIGNMENT ENET_BUFF_ALIGNMENT
#endif

void mimx6ulEthClockInit(NetInterface *interface,unsigned index)
{
	uint32_t sysClock;  
	const clock_enet_pll_config_t config =
	{
		true,
		true,
		false,
		1,
		1
	};
	CLOCK_InitEnetPll(&config);
	sysClock = CLOCK_GetFreq(ENET_CLOCK_NAME);
	if(index==0){
		PHY_Init(ENET1, interface->phyAddr, sysClock);
	}else{
		PHY_Init(ENET2, interface->phyAddr, sysClock);
	}
	CLOCK_EnableClock(kCLOCK_Enet);
}

void mimx6ulEthSetPhy(unsigned index)
{
	if(index==0){
		IOMUXC_SetPinMux(IOMUXC_GPIO1_IO06_ENET1_MDIO, 0U);
		IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO06_ENET1_MDIO, 
			IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
			IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
			IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
			IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
		IOMUXC_SetPinMux(IOMUXC_GPIO1_IO07_ENET1_MDC, 0U);
		IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO07_ENET1_MDC, 
			IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
			IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
			IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
			IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
			IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	}else{
		IOMUXC_SetPinMux(IOMUXC_GPIO1_IO06_ENET2_MDIO, 0U);
		IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO06_ENET2_MDIO, 
			IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
			IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
			IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
			IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
		IOMUXC_SetPinMux(IOMUXC_GPIO1_IO07_ENET2_MDC, 0U);
		IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO07_ENET2_MDC, 
			IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
			IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
			IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
			IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
			IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	}
}

#if defined(FSL_USE_ENET1)
#define ENET1_PHY_RESET_GPIO GPIO5
#define ENET1_PHY_RESET_GPIO_PIN 7

/* Address of PHY interface. */
#define ENET1_PHY_ADDRESS 0
	  
AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t rxBufferEth1Desc[MIMX6UL_ETH_RX_BUFFER_COUNT], DRIVER_ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t txBufferEth1Desc[MIMX6UL_ETH_RX_BUFFER_COUNT], DRIVER_ENET_BUFF_ALIGNMENT);
/*! @brief The data buffers can be in cacheable region or in non-cacheable region.
 * If use cacheable region, the alignment size should be the maximum size of "CACHE LINE SIZE" and "ENET_BUFF_ALIGNMENT"
 * If use non-cache region, the alignment size is the "ENET_BUFF_ALIGNMENT".
 */
SDK_ALIGN(uint8_t rxBufferEth1[MIMX6UL_ETH_RX_BUFFER_COUNT][SDK_SIZEALIGN(MIMX6UL_ETH_RX_BUFFER_SIZE, DRIVER_ENET_BUFF_ALIGNMENT)],
          DRIVER_ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t txBufferEth1[MIMX6UL_ETH_TX_BUFFER_COUNT][SDK_SIZEALIGN(MIMX6UL_ETH_TX_BUFFER_SIZE, DRIVER_ENET_BUFF_ALIGNMENT)],
          DRIVER_ENET_BUFF_ALIGNMENT);

enet_handle_t ethernetifEth1Handle;
EventGroupHandle_t ethernetifEth1TransmitAccessEvent;
EventBits_t ethernetifEth1TxFlag;

const NicDriver mimx6ulEthPort1Driver =
{
   NIC_TYPE_ETHERNET,
   ETH_MTU,
   mimx6ulEthInitPort1,
   mimx6ulEthTickPort1,
   mimx6ulEthEnableIrqPort1,
   mimx6ulEthDisableIrqPort1,
   mimx6ulEthEventHandlerPort1,
   mimx6ulEthSendPacketPort1,
   mimx6ulEthSetMulticastFilterPort1,
   mimx6ulEthUpdateMacConfigPort1,
   mimx6ulEthWritePhyRegPort1,
   mimx6ulEthReadPhyRegPort1,
   TRUE,
   TRUE,
   TRUE,
   TRUE
};

static void mimx6ulEthernetCallbackPort1(ENET_Type *base, enet_handle_t *handle, enet_event_t event, void *param){
  	
	uint32_t flag=0;
	NetInterface *interface =(NetInterface *)param;
	//Enter interrupt service routine
	osEnterIsr();
	switch (event)
	{
		case kENET_RxEvent:
			//Set event flag
			interface->nicEvent = TRUE;
			//Notify the TCP/IP stack of the event
			flag = osSetEventFromIsr(&netEvent);
			break;
		case kENET_TxEvent:
		{
			portBASE_TYPE taskToWake = pdFALSE;

			if (SystemGetIRQNestingLevel()){
				xEventGroupSetBitsFromISR(ethernetifEth1TransmitAccessEvent, ethernetifEth1TxFlag, &taskToWake);
				if (pdTRUE == taskToWake)
				{
					portYIELD_FROM_ISR(taskToWake);
				}
			}else{
                		xEventGroupSetBits(ethernetifEth1TransmitAccessEvent, ethernetifEth1TxFlag);
            		}
			flag = osSetEventFromIsr(&interface->nicTxEvent);
			break;
		}
		default:
			break;
	}
	//Leave interrupt service routine
	osExitIsr(flag);
}

void mimx6ulEthernetifInitPort1(NetInterface *interface){
	enet_config_t config;
	uint32_t sysClock;
	bool link = false;
	phy_speed_t speed;
	phy_duplex_t duplex;
	/*uint32_t txnumber = 0;
	status_t status;
	uint32_t length = 0;
	enet_data_error_stats_t eErrStatic;*/
    	
	enet_buffer_config_t buffConfig = {
		MIMX6UL_ETH_RX_BUFFER_COUNT,
		MIMX6UL_ETH_TX_BUFFER_COUNT,
		SDK_SIZEALIGN(MIMX6UL_ETH_RX_BUFFER_SIZE, DRIVER_ENET_BUFF_ALIGNMENT),
		SDK_SIZEALIGN(MIMX6UL_ETH_TX_BUFFER_SIZE, DRIVER_ENET_BUFF_ALIGNMENT),
		&rxBufferEth1Desc[0],
		&txBufferEth1Desc[0],
		&rxBufferEth1[0][0],
		&txBufferEth1[0][0],
	};
    
	ENET_GetDefaultConfig(&config);

	/* Set SMI to get PHY link status. */
	sysClock = CLOCK_GetFreq(ENET_CLOCK_NAME);
	mimx6ulEthSetPhy(0);
	PHY_Init(ENET1, interface->phyAddr, sysClock);
	PHY_GetLinkStatus(ENET1, interface->phyAddr, &link);
	if (link)
	{
		/* Get the actual PHY link speed. */
		PHY_GetLinkSpeedDuplex(ENET1, interface->phyAddr, &speed, &duplex);
		/* Change the MII speed and duplex for actual link status. */
		config.miiSpeed = (enet_mii_speed_t)speed;
		config.miiDuplex = (enet_mii_duplex_t)duplex;
	}
	else
	{
		TRACE_INFO("\r\nPHY Link down, please check the cable connection and link partner setting.\r\n");
	}
	config.interrupt |= kENET_RxFrameInterrupt | kENET_TxFrameInterrupt | kENET_TxBufferInterrupt;
	ethernetifEth1TransmitAccessEvent = xEventGroupCreate();
	ethernetifEth1TxFlag = 0x1;
	static const IRQn_Type enetTxIrqId[] = ENET_Transmit_IRQS;
	/*! @brief Pointers to enet receive IRQ number for each instance. */
	static const IRQn_Type enetRxIrqId[] = ENET_Receive_IRQS;
#if defined(ENET_ENHANCEDBUFFERDESCRIPTOR_MODE) && ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
	/*! @brief Pointers to enet timestamp IRQ number for each instance. */
	static const IRQn_Type enetTsIrqId[] = ENET_1588_Timer_IRQS;
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
	GIC_SetPriority(enetRxIrqId[1], MIMX6UL_ENET_PRIORITY);
	GIC_SetPriority(enetTxIrqId[1], MIMX6UL_ENET_PRIORITY);
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
	GIC_SetPriority(enetTsIrqId[1], MIMX6UL_ENET_1588_PRIORITY);
#endif
	ethernetifEth1Handle.callback=mimx6ulEthernetCallbackPort1;
	ENET_Init(ENET1, &ethernetifEth1Handle, &config, &buffConfig, (uint8_t *)(&interface->macAddr), sysClock);
	ENET_SetCallback(&ethernetifEth1Handle, mimx6ulEthernetCallbackPort1, interface);
	ENET_ActiveRead(ENET1);
}

/**
 * @brief i.MX6UL Ethernet MAC initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t mimx6ulEthInitPort1(NetInterface *interface)
{
	error_t error;

	//Debug message
	TRACE_INFO("Initializing i.MX6UL Ethernet ENET1 MAC...\r\n");
	//GPIO configuration
	mimx6ulEthInitGpioPort1(interface);
	interface->phyAddr=ENET1_PHY_ADDRESS;
	//Enable ENET peripheral clock
	mimx6ulEthClockInit(interface,0);

	error = interface->phyDriver->init(interface);
	//Failed to initialize PHY transceiver?
	if(error)
		return ERROR_FAILURE;
	mimx6ulEthernetifInitPort1(interface);
	//Accept any packets from the upper layer
	osSetEvent(&interface->nicTxEvent);
	//Successful initialization
	return NO_ERROR;
}

/**
 * @brief GPIO configuration
 * @param[in] interface Underlying network interface
 **/

void mimx6ulEthInitGpioPort1(NetInterface *interface)
{
   	gpio_pin_config_t config = {
		kGPIO_DigitalOutput, 0,
	};
	IOMUXC_SetPinMux(IOMUXC_SNVS_TAMPER7_GPIO5_IO07, 0U);
	IOMUXC_SetPinMux(IOMUXC_ENET1_RX_DATA0_ENET1_RDATA00, 0U);
	IOMUXC_SetPinConfig(IOMUXC_ENET1_RX_DATA0_ENET1_RDATA00, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	IOMUXC_SetPinMux(IOMUXC_ENET1_RX_DATA1_ENET1_RDATA01, 0U);
	IOMUXC_SetPinConfig(IOMUXC_ENET1_RX_DATA1_ENET1_RDATA01, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	IOMUXC_SetPinMux(IOMUXC_ENET1_RX_EN_ENET1_RX_EN, 0U);
	IOMUXC_SetPinConfig(IOMUXC_ENET1_RX_EN_ENET1_RX_EN, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	IOMUXC_SetPinMux(IOMUXC_ENET1_RX_ER_ENET1_RX_ER, 0U);
	IOMUXC_SetPinConfig(IOMUXC_ENET1_RX_ER_ENET1_RX_ER, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	IOMUXC_SetPinMux(IOMUXC_ENET1_TX_CLK_ENET1_REF_CLK1, 1U);
	IOMUXC_SetPinConfig(IOMUXC_ENET1_TX_CLK_ENET1_REF_CLK1, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
		IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
	IOMUXC_SetPinMux(IOMUXC_ENET1_TX_DATA0_ENET1_TDATA00, 0U);
	IOMUXC_SetPinConfig(IOMUXC_ENET1_TX_DATA0_ENET1_TDATA00, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	IOMUXC_SetPinMux(IOMUXC_ENET1_TX_DATA1_ENET1_TDATA01, 0U);
	IOMUXC_SetPinConfig(IOMUXC_ENET1_TX_DATA1_ENET1_TDATA01, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	IOMUXC_SetPinMux(IOMUXC_ENET1_TX_EN_ENET1_TX_EN, 0U);
	IOMUXC_SetPinConfig(IOMUXC_ENET1_TX_EN_ENET1_TX_EN, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
		IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO06_ENET1_MDIO, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO07_ENET1_MDC, 0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO07_ENET1_MDC, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));

	IOMUXC_GPR->GPR1 |= 0x20000;
	GPIO_PinInit(ENET1_PHY_RESET_GPIO, ENET1_PHY_RESET_GPIO_PIN, &config);
	//Reset PHY transceiver (hard reset)
	GPIO_WritePinOutput(ENET1_PHY_RESET_GPIO, ENET1_PHY_RESET_GPIO_PIN, 0);
	sleep(1);
	GPIO_WritePinOutput(ENET1_PHY_RESET_GPIO, ENET1_PHY_RESET_GPIO_PIN, 1);
	sleep(1);
}


/**
 * @brief i.MX6UL Ethernet MAC timer handler
 *
 * This routine is periodically called by the TCP/IP stack to
 * handle periodic operations such as polling the link state
 *
 * @param[in] interface Underlying network interface
 **/

void mimx6ulEthTickPort1(NetInterface *interface)
{
	//Handle periodic operations
	interface->phyDriver->tick(interface);
}


/**
 * @brief Enable interrupts
 * @param[in] interface Underlying network interface
 **/

void mimx6ulEthEnableIrqPort1(NetInterface *interface)
{
	//Enable Ethernet MAC interrupts
	EnableIRQ(ENET1_IRQn);
	//Enable Ethernet PHY interrupts
	interface->phyDriver->enableIrq(interface);
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void mimx6ulEthDisableIrqPort1(NetInterface *interface)
{
	//Disable Ethernet MAC interrupts
	DisableIRQ(ENET1_IRQn);
	//Disable Ethernet PHY interrupts
	interface->phyDriver->disableIrq(interface);
}


/**
 * @brief i.MX6UL Ethernet MAC event handler
 * @param[in] interface Underlying network interface
 **/

void mimx6ulEthEventHandlerPort1(NetInterface *interface)
{
	error_t error;
	
	do
	{
		//Read incoming packet
		error = mimx6ulEthReceivePacketPort1(interface);
		//No more data in the receive buffer?
	} while(error != ERROR_BUFFER_EMPTY);
}

/**
 * @brief Send a packet
 * @param[in] interface Underlying network interface
 * @param[in] buffer Multi-part buffer containing the data to send
 * @param[in] offset Offset to the first data byte
 * @return Error code
 **/

error_t mimx6ulEthSendPacketPort1(NetInterface *interface,
   const NetBuffer *buffer, size_t offset)
{
	static uint8_t temp[SDK_SIZEALIGN(MIMX6UL_ETH_TX_BUFFER_SIZE, DRIVER_ENET_BUFF_ALIGNMENT)];
	size_t length;
        status_t result;

	//Retrieve the length of the packet
	length = netBufferGetLength(buffer) - offset;
	//Check the frame length
	if(length > SDK_SIZEALIGN(MIMX6UL_ETH_TX_BUFFER_SIZE, DRIVER_ENET_BUFF_ALIGNMENT))
	{
		//The transmitter can accept another packet
		osSetEvent(&interface->nicTxEvent);
		//Report an error
		return ERROR_INVALID_LENGTH;
	}
	//Copy user data to the transmit buffer
	memset(temp,0,sizeof(temp));
	netBufferRead(temp, buffer, offset, length);
	do
        {
            result = ENET_SendFrame(ENET1, &ethernetifEth1Handle, temp, length);

            if (result == kStatus_ENET_TxFrameBusy)
            {
                xEventGroupWaitBits(ethernetifEth1TransmitAccessEvent, ethernetifEth1TxFlag, pdTRUE, (BaseType_t) false,
                                    portMAX_DELAY);
            }
        }while (result == kStatus_ENET_TxFrameBusy);
	//Successful processing
	return NO_ERROR;
}


/**
 * @brief Receive a packet
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t mimx6ulEthReceivePacketPort1(NetInterface *interface)
{
	error_t error= ERROR_INVALID_PACKET ;
	static uint8_t temp[SDK_SIZEALIGN(MIMX6UL_ETH_RX_BUFFER_SIZE, DRIVER_ENET_BUFF_ALIGNMENT)];
	status_t status;
	uint32_t length = 0;
	enet_data_error_stats_t eErrStatic;

	status = ENET_GetRxFrameSize(&ethernetifEth1Handle, &length);
        /* Call ENET_ReadFrame when there is a received frame. */
        if (length != 0)
        {
		memset(temp,0,sizeof(temp));
		status = ENET_ReadFrame(ENET1, &ethernetifEth1Handle, temp, length);
		if (status == kStatus_Success)
		{
			nicProcessPacket(interface, temp, length);
		}
		error = NO_ERROR;
	}else if (status == kStatus_ENET_RxFrameError)
        {
		/* Update the received buffer when error happened. */
		/* Get the error information of the received g_frame. */
		ENET_GetRxErrBeforeReadFrame(&ethernetifEth1Handle, &eErrStatic);
		/* update the receive buffer. */
		ENET_ReadFrame(ENET1, &ethernetifEth1Handle, NULL, 0);
		error = ERROR_INVALID_PACKET;
	}else{
		error = ERROR_BUFFER_EMPTY;
	}
	//Return status code
	return error;
}

/**
 * @brief Configure multicast MAC address filtering
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t mimx6ulEthSetMulticastFilterPort1(NetInterface *interface)
{
	uint_t i;
	MacFilterEntry *entry;
	
	//The MAC filter table contains the multicast MAC addresses
	//to accept when receiving an Ethernet frame
	for(i = 0; i < MAC_MULTICAST_FILTER_SIZE; i++)
	{
		//Point to the current entry
		entry = &interface->macMulticastFilter[i];

		//Valid entry?
		if(entry->refCount > 0)
		{
			ENET_AddMulticastGroup(ENET1, &entry->addr);
		}
	}

	//Successful processing
	return NO_ERROR;
}


/**
 * @brief Adjust MAC configuration parameters for proper operation
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t mimx6ulEthUpdateMacConfigPort1(NetInterface *interface)
{
	mimx6ulEthernetifInitPort1(interface);
	//Successful processing
	return NO_ERROR;
}


/**
 * @brief Write PHY register
 * @param[in] phyAddr PHY address
 * @param[in] regAddr Register address
 * @param[in] data Register value
 **/

void mimx6ulEthWritePhyRegPort1(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
	mimx6ulEthSetPhy(0);
	PHY_Write(ENET1, phyAddr, regAddr, data);
}


/**
 * @brief Read PHY register
 * @param[in] phyAddr PHY address
 * @param[in] regAddr Register address
 * @return Register value
 **/

uint16_t mimx6ulEthReadPhyRegPort1(uint8_t phyAddr, uint8_t regAddr)
{
	uint32_t data=0;
	mimx6ulEthSetPhy(0);
	PHY_Read(ENET1, phyAddr, regAddr, &data);
	return data;
}
#endif

#if defined(FSL_USE_ENET2)
#define ENET2_PHY_RESET_GPIO GPIO5
#define ENET2_PHY_RESET_GPIO_PIN 6

/* Address of PHY interface. */
#define ENET2_PHY_ADDRESS 1
	  
AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t rxBufferEth2Desc[MIMX6UL_ETH_RX_BUFFER_COUNT], DRIVER_ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t txBufferEth2Desc[MIMX6UL_ETH_RX_BUFFER_COUNT], DRIVER_ENET_BUFF_ALIGNMENT);
/*! @brief The data buffers can be in cacheable region or in non-cacheable region.
 * If use cacheable region, the alignment size should be the maximum size of "CACHE LINE SIZE" and "ENET_BUFF_ALIGNMENT"
 * If use non-cache region, the alignment size is the "ENET_BUFF_ALIGNMENT".
 */
SDK_ALIGN(uint8_t rxBufferEth2[MIMX6UL_ETH_RX_BUFFER_COUNT][SDK_SIZEALIGN(MIMX6UL_ETH_RX_BUFFER_SIZE, DRIVER_ENET_BUFF_ALIGNMENT)],
          DRIVER_ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t txBufferEth2[MIMX6UL_ETH_TX_BUFFER_COUNT][SDK_SIZEALIGN(MIMX6UL_ETH_TX_BUFFER_SIZE, DRIVER_ENET_BUFF_ALIGNMENT)],
          DRIVER_ENET_BUFF_ALIGNMENT);

enet_handle_t ethernetifEth2Handle;
EventGroupHandle_t ethernetifEth2TransmitAccessEvent;
EventBits_t ethernetifEth2TxFlag;

const NicDriver mimx6ulEthPort2Driver =
{
   NIC_TYPE_ETHERNET,
   ETH_MTU,
   mimx6ulEthInitPort2,
   mimx6ulEthTickPort2,
   mimx6ulEthEnableIrqPort2,
   mimx6ulEthDisableIrqPort2,
   mimx6ulEthEventHandlerPort2,
   mimx6ulEthSendPacketPort2,
   mimx6ulEthSetMulticastFilterPort2,
   mimx6ulEthUpdateMacConfigPort2,
   mimx6ulEthWritePhyRegPort2,
   mimx6ulEthReadPhyRegPort2,
   TRUE,
   TRUE,
   TRUE,
   TRUE
};

static void mimx6ulEthernetCallbackPort2(ENET_Type *base, enet_handle_t *handle, enet_event_t event, void *param){
  	
	uint32_t flag=0;
	NetInterface *interface =(NetInterface *)param;
	//Enter interrupt service routine
	osEnterIsr();
	switch (event)
	{
		case kENET_RxEvent:
			//Set event flag
			interface->nicEvent = TRUE;
			//Notify the TCP/IP stack of the event
			flag = osSetEventFromIsr(&netEvent);
			break;
		case kENET_TxEvent:
		{
			portBASE_TYPE taskToWake = pdFALSE;

			if (SystemGetIRQNestingLevel()){
				xEventGroupSetBitsFromISR(ethernetifEth2TransmitAccessEvent, ethernetifEth2TxFlag, &taskToWake);
				if (pdTRUE == taskToWake)
				{
					portYIELD_FROM_ISR(taskToWake);
				}
			}else{
                		xEventGroupSetBits(ethernetifEth2TransmitAccessEvent, ethernetifEth2TxFlag);
            		}
			flag = osSetEventFromIsr(&interface->nicTxEvent);
			break;
		}
		default:
			break;
	}
	//Leave interrupt service routine
	osExitIsr(flag);
}

void mimx6ulEthernetifInitPort2(NetInterface *interface){
	enet_config_t config;
	uint32_t sysClock;
	bool link = false;
	phy_speed_t speed;
	phy_duplex_t duplex;
	/*uint32_t txnumber = 0;
	status_t status;
	uint32_t length = 0;
	enet_data_error_stats_t eErrStatic;*/
    	
	enet_buffer_config_t buffConfig = {
		MIMX6UL_ETH_RX_BUFFER_COUNT,
		MIMX6UL_ETH_TX_BUFFER_COUNT,
		SDK_SIZEALIGN(MIMX6UL_ETH_RX_BUFFER_SIZE, DRIVER_ENET_BUFF_ALIGNMENT),
		SDK_SIZEALIGN(MIMX6UL_ETH_TX_BUFFER_SIZE, DRIVER_ENET_BUFF_ALIGNMENT),
		&rxBufferEth2Desc[0],
		&txBufferEth2Desc[0],
		&rxBufferEth2[0][0],
		&txBufferEth2[0][0],
	};
    
	ENET_GetDefaultConfig(&config);

	/* Set SMI to get PHY link status. */
	sysClock = CLOCK_GetFreq(ENET_CLOCK_NAME);
	mimx6ulEthSetPhy(1);
	PHY_Init(ENET2, interface->phyAddr, sysClock);
	PHY_GetLinkStatus(ENET2, interface->phyAddr, &link);
	if (link)
	{
		/* Get the actual PHY link speed. */
		PHY_GetLinkSpeedDuplex(ENET2, interface->phyAddr, &speed, &duplex);
		/* Change the MII speed and duplex for actual link status. */
		config.miiSpeed = (enet_mii_speed_t)speed;
		config.miiDuplex = (enet_mii_duplex_t)duplex;
	}
	else
	{
		TRACE_INFO("\r\nPHY Link down, please check the cable connection and link partner setting.\r\n");
	}
	config.interrupt |= kENET_RxFrameInterrupt | kENET_TxFrameInterrupt | kENET_TxBufferInterrupt;
	ethernetifEth2TransmitAccessEvent = xEventGroupCreate();
	ethernetifEth2TxFlag = 0x1;
	static const IRQn_Type enetTxIrqId[] = ENET_Transmit_IRQS;
	/*! @brief Pointers to enet receive IRQ number for each instance. */
	static const IRQn_Type enetRxIrqId[] = ENET_Receive_IRQS;
#if defined(ENET_ENHANCEDBUFFERDESCRIPTOR_MODE) && ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
	/*! @brief Pointers to enet timestamp IRQ number for each instance. */
	static const IRQn_Type enetTsIrqId[] = ENET_1588_Timer_IRQS;
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
	GIC_SetPriority(enetRxIrqId[2], MIMX6UL_ENET_PRIORITY);
	GIC_SetPriority(enetTxIrqId[2], MIMX6UL_ENET_PRIORITY);
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
	GIC_SetPriority(enetTsIrqId[2], MIMX6UL_ENET_1588_PRIORITY);
#endif
	ethernetifEth2Handle.callback=mimx6ulEthernetCallbackPort2;
	ENET_Init(ENET2, &ethernetifEth2Handle, &config, &buffConfig, (uint8_t *)(&interface->macAddr), sysClock);
	ENET_SetCallback(&ethernetifEth2Handle, mimx6ulEthernetCallbackPort2, interface);
	ENET_ActiveRead(ENET2);
}

/**
 * @brief i.MX6UL Ethernet MAC initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t mimx6ulEthInitPort2(NetInterface *interface)
{
	error_t error;

	//Debug message
	TRACE_INFO("Initializing i.MX6UL Ethernet ENET2 MAC...\r\n");
	interface->phyAddr=ENET2_PHY_ADDRESS;
	//GPIO configuration
	mimx6ulEthInitGpioPort2(interface);
	//Enable ENET peripheral clock
	mimx6ulEthClockInit(interface,1);

	error = interface->phyDriver->init(interface);
	//Failed to initialize PHY transceiver?
	if(error)
		return ERROR_FAILURE;
	//Accept any packets from the upper layer
	osSetEvent(&interface->nicTxEvent);
	//Successful initialization
	return NO_ERROR;
}

/**
 * @brief GPIO configuration
 * @param[in] interface Underlying network interface
 **/

void mimx6ulEthInitGpioPort2(NetInterface *interface)
{
   	gpio_pin_config_t config = {
		kGPIO_DigitalOutput, 0,
	};
	IOMUXC_SetPinMux(IOMUXC_SNVS_TAMPER6_GPIO5_IO06, 0U);
	IOMUXC_SetPinMux(IOMUXC_ENET2_RX_DATA0_ENET2_RDATA00, 0U);
	IOMUXC_SetPinConfig(IOMUXC_ENET2_RX_DATA0_ENET2_RDATA00, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	IOMUXC_SetPinMux(IOMUXC_ENET2_RX_DATA1_ENET2_RDATA01, 0U);
	IOMUXC_SetPinConfig(IOMUXC_ENET2_RX_DATA1_ENET2_RDATA01, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	IOMUXC_SetPinMux(IOMUXC_ENET2_RX_EN_ENET2_RX_EN, 0U);
	IOMUXC_SetPinConfig(IOMUXC_ENET2_RX_EN_ENET2_RX_EN, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	IOMUXC_SetPinMux(IOMUXC_ENET2_RX_ER_ENET2_RX_ER, 0U);
	IOMUXC_SetPinConfig(IOMUXC_ENET2_RX_ER_ENET2_RX_ER, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	IOMUXC_SetPinMux(IOMUXC_ENET2_TX_CLK_ENET2_REF_CLK2, 1U);
	IOMUXC_SetPinConfig(IOMUXC_ENET2_TX_CLK_ENET2_REF_CLK2, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(6U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
		IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
	IOMUXC_SetPinMux(IOMUXC_ENET2_TX_DATA0_ENET2_TDATA00, 0U);
	IOMUXC_SetPinConfig(IOMUXC_ENET2_TX_DATA0_ENET2_TDATA00, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	IOMUXC_SetPinMux(IOMUXC_ENET2_TX_DATA1_ENET2_TDATA01, 0U);
	IOMUXC_SetPinConfig(IOMUXC_ENET2_TX_DATA1_ENET2_TDATA01, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	IOMUXC_SetPinMux(IOMUXC_ENET2_TX_EN_ENET2_TX_EN, 0U);
	IOMUXC_SetPinConfig(IOMUXC_ENET2_TX_EN_ENET2_TX_EN, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
		IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO06_ENET2_MDIO, 0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO06_ENET2_MDIO, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO07_ENET2_MDC, 0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO07_ENET2_MDC, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(5U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(3U) |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U));

	IOMUXC_GPR->GPR1 |= 0x40000;
	GPIO_PinInit(ENET2_PHY_RESET_GPIO, ENET2_PHY_RESET_GPIO_PIN, &config);
	//Reset PHY transceiver (hard reset)
	GPIO_WritePinOutput(ENET2_PHY_RESET_GPIO, ENET2_PHY_RESET_GPIO_PIN, 0);
	sleep(1);
	GPIO_WritePinOutput(ENET2_PHY_RESET_GPIO, ENET2_PHY_RESET_GPIO_PIN, 1);
	sleep(1);
}


/**
 * @brief i.MX6UL Ethernet MAC timer handler
 *
 * This routine is periodically called by the TCP/IP stack to
 * handle periodic operations such as polling the link state
 *
 * @param[in] interface Underlying network interface
 **/

void mimx6ulEthTickPort2(NetInterface *interface)
{
	//Handle periodic operations
	interface->phyDriver->tick(interface);
}


/**
 * @brief Enable interrupts
 * @param[in] interface Underlying network interface
 **/

void mimx6ulEthEnableIrqPort2(NetInterface *interface)
{
	//Enable Ethernet MAC interrupts
	EnableIRQ(ENET2_IRQn);
	//Enable Ethernet PHY interrupts
	interface->phyDriver->enableIrq(interface);
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void mimx6ulEthDisableIrqPort2(NetInterface *interface)
{
	//Disable Ethernet MAC interrupts
	DisableIRQ(ENET2_IRQn);
	//Disable Ethernet PHY interrupts
	interface->phyDriver->disableIrq(interface);
}


/**
 * @brief i.MX6UL Ethernet MAC event handler
 * @param[in] interface Underlying network interface
 **/

void mimx6ulEthEventHandlerPort2(NetInterface *interface)
{
	error_t error;
	
	do
	{
		//Read incoming packet
		error = mimx6ulEthReceivePacketPort2(interface);
		//No more data in the receive buffer?
	} while(error != ERROR_BUFFER_EMPTY);
}

/**
 * @brief Send a packet
 * @param[in] interface Underlying network interface
 * @param[in] buffer Multi-part buffer containing the data to send
 * @param[in] offset Offset to the first data byte
 * @return Error code
 **/

error_t mimx6ulEthSendPacketPort2(NetInterface *interface,
   const NetBuffer *buffer, size_t offset)
{
	static uint8_t temp[SDK_SIZEALIGN(MIMX6UL_ETH_TX_BUFFER_SIZE, DRIVER_ENET_BUFF_ALIGNMENT)];
	size_t length;
        status_t result;

	//Retrieve the length of the packet
	length = netBufferGetLength(buffer) - offset;
	//Check the frame length
	if(length > SDK_SIZEALIGN(MIMX6UL_ETH_TX_BUFFER_SIZE, DRIVER_ENET_BUFF_ALIGNMENT))
	{
		//The transmitter can accept another packet
		osSetEvent(&interface->nicTxEvent);
		//Report an error
		return ERROR_INVALID_LENGTH;
	}
	//Copy user data to the transmit buffer
	memset(temp,0,sizeof(temp));
	netBufferRead(temp, buffer, offset, length);
	do
        {
            result = ENET_SendFrame(ENET2, &ethernetifEth2Handle, temp, length);

            if (result == kStatus_ENET_TxFrameBusy)
            {
                xEventGroupWaitBits(ethernetifEth2TransmitAccessEvent, ethernetifEth2TxFlag, pdTRUE, (BaseType_t) false,
                                    portMAX_DELAY);
            }
        }while (result == kStatus_ENET_TxFrameBusy);
	//Successful processing
	return NO_ERROR;
}


/**
 * @brief Receive a packet
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t mimx6ulEthReceivePacketPort2(NetInterface *interface)
{
	error_t error= ERROR_INVALID_PACKET ;
	static uint8_t temp[SDK_SIZEALIGN(MIMX6UL_ETH_RX_BUFFER_SIZE, DRIVER_ENET_BUFF_ALIGNMENT)];
	status_t status;
	uint32_t length = 0;
	enet_data_error_stats_t eErrStatic;

	status = ENET_GetRxFrameSize(&ethernetifEth2Handle, &length);
        /* Call ENET_ReadFrame when there is a received frame. */
        if (length != 0)
        {
		memset(temp,0,sizeof(temp));
		status = ENET_ReadFrame(ENET2, &ethernetifEth2Handle, temp, length);
		if (status == kStatus_Success)
		{
			nicProcessPacket(interface, temp, length);
		}
		error = NO_ERROR;
	}else if (status == kStatus_ENET_RxFrameError)
        {
		/* Update the received buffer when error happened. */
		/* Get the error information of the received g_frame. */
		ENET_GetRxErrBeforeReadFrame(&ethernetifEth2Handle, &eErrStatic);
		/* update the receive buffer. */
		ENET_ReadFrame(ENET2, &ethernetifEth2Handle, NULL, 0);
		error = ERROR_INVALID_PACKET;
	}else{
		error = ERROR_BUFFER_EMPTY;
	}
	//Return status code
	return error;
}

/**
 * @brief Configure multicast MAC address filtering
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t mimx6ulEthSetMulticastFilterPort2(NetInterface *interface)
{
	uint_t i;
	MacFilterEntry *entry;
	
	//The MAC filter table contains the multicast MAC addresses
	//to accept when receiving an Ethernet frame
	for(i = 0; i < MAC_MULTICAST_FILTER_SIZE; i++)
	{
		//Point to the current entry
		entry = &interface->macMulticastFilter[i];

		//Valid entry?
		if(entry->refCount > 0)
		{
			ENET_AddMulticastGroup(ENET2, &entry->addr);
		}
	}

	//Successful processing
	return NO_ERROR;
}


/**
 * @brief Adjust MAC configuration parameters for proper operation
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t mimx6ulEthUpdateMacConfigPort2(NetInterface *interface)
{
	mimx6ulEthernetifInitPort2(interface);
	//Successful processing
	return NO_ERROR;
}


/**
 * @brief Write PHY register
 * @param[in] phyAddr PHY address
 * @param[in] regAddr Register address
 * @param[in] data Register value
 **/

void mimx6ulEthWritePhyRegPort2(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
	mimx6ulEthSetPhy(1);
	PHY_Write(ENET2, phyAddr, regAddr, data);
}


/**
 * @brief Read PHY register
 * @param[in] phyAddr PHY address
 * @param[in] regAddr Register address
 * @return Register value
 **/

uint16_t mimx6ulEthReadPhyRegPort2(uint8_t phyAddr, uint8_t regAddr)
{
	uint32_t data=0;
  	mimx6ulEthSetPhy(1);
	PHY_Read(ENET2, phyAddr, regAddr, &data);
	return data;
}
#endif
