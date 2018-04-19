/**
 * @file bcm4xxxx_driver.c
 * @brief BCM4XXXX Wi-Fi controller
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
#include "core/net.h"
#include "drivers/wifi/bcm4xxxx_driver.h"
#include "debug.h"

//WICED dependencies
#include "imx6ul_bcm4xxxx_init.h"
#include "wwd_constants.h"
#include "wwd_structures.h"
#include "wwd_buffer.h"
#include "wwd_events.h"
#include "wwd_management.h"
#include "wwd_poll.h"
#include "wwd_wifi.h"
#include "wwd_buffer_interface.h"
#include "wwd_bus_protocol_interface.h"
#include "wwd_network_constants.h"
#include "wwd_network_interface.h"

//Underlying network interface
static NetInterface *bcm4xxxxStaInterface = NULL;
static NetInterface *bcm4xxxxApInterface = NULL;

//RX queue
QueueHandle_t wwdRxQueue;

//Regitered Wi-Fi events
static const wwd_event_num_t app_wifi_events[] =
{
	WLC_E_IF,
	WLC_E_LINK,
	WLC_E_ASSOC_IND,
	WLC_E_DISASSOC_IND,
	WLC_E_NONE
};

//Forward declaration of functions
void *app_wifi_event_handler(const wwd_event_header_t *event_header,const uint8_t *event_data, void *handler_user_data);


/**
 * @brief BCM4xxxx driver (STA mode)
 **/

const NicDriver bcm4xxxxStaDriver =
{
	NIC_TYPE_ETHERNET,
	ETH_MTU,
	bcm4xxxxInit,
	bcm4xxxxTick,
	bcm4xxxxEnableIrq,
	bcm4xxxxDisableIrq,
	bcm4xxxxEventHandler,
	bcm4xxxxSendPacket,
	bcm4xxxxSetMulticastFilter,
	NULL,
	NULL,
	NULL,
	TRUE,
	TRUE,
	TRUE,
	TRUE
};


/**
 * @brief BCM4xxxx driver (AP mode)
 **/

const NicDriver bcm4xxxxApDriver =
{
	NIC_TYPE_ETHERNET,
	ETH_MTU,
	bcm4xxxxInit,
	bcm4xxxxTick,
	bcm4xxxxEnableIrq,
	bcm4xxxxDisableIrq,
	bcm4xxxxEventHandler,
	bcm4xxxxSendPacket,
	bcm4xxxxSetMulticastFilter,
	NULL,
	NULL,
	NULL,
	TRUE,
	TRUE,
	TRUE,
	TRUE
};


/**
 * @brief BCM4xxxx initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t bcm4xxxxInit(NetInterface *interface)
{
	wwd_result_t ret;
	//MacAddr macAddr;

	//STA or AP mode?
	if(interface->nicDriver == &bcm4xxxxStaDriver)
	{
		//Debug message
		TRACE_INFO("Initializing BCM4xxxx (STA mode)...\r\n");
	}
	else
	{
		//Debug message
		TRACE_INFO("Initializing BCM4xxxx (AP mode)...\r\n");
	}

	//Start of exception handling block
	do
	{
		//Initialization sequence is performed once at startup
		if(bcm4xxxxStaInterface == NULL && bcm4xxxxApInterface == NULL)
		{
			platform_init_mcu_infrastructure();
			wwd_buffer_init(NULL);

			//Create TX queue
			wwdRxQueue = xQueueCreate(16, sizeof(wiced_buffer_t));

			//Initialize Wi-Fi controller
			ret = wwd_management_wifi_on(WICED_COUNTRY_FRANCE);
			if(ret!=WWD_SUCCESS){
				TRACE_INFO("wwd_management_wifi_on=%d (0x%04X) failed!\r\n", ret, ret);
				return ERROR_FAILURE;
			}else{
				TRACE_INFO("wwd_management_wifi_on=%d (0x%04X) sucessed!\r\n", ret, ret);
			}
			ret = wwd_management_set_event_handler(app_wifi_events, app_wifi_event_handler, NULL, WWD_AP_INTERFACE);
			TRACE_INFO("wwd_management_set_event_handler=%d (0x%04X)\r\n", ret, ret);
		}
		else
		{
			//Initialization was already done
			ret = WWD_SUCCESS;
		}

		//STA or AP mode?
		if(interface->nicDriver == &bcm4xxxxStaDriver)
		{
			//Save underlying network interface (STA mode)
			bcm4xxxxStaInterface = interface;

			//Optionally set the station MAC address
			//if(macCompAddr(&interface->macAddr, &MAC_UNSPECIFIED_ADDR))
			{
				//Use the factory preprogrammed station address
				ret = wwd_wifi_get_mac_address((wiced_mac_t *) &interface->macAddr, WWD_STA_INTERFACE);
				TRACE_INFO("wwd_wifi_get_mac_address=%d (0x%04X)\r\n", ret, ret);
				TRACE_INFO("MAC=%s\r\n", macAddrToString(&interface->macAddr, NULL));

				//Generate the 64-bit interface identifier
				macAddrToEui64(&interface->macAddr, &interface->eui64);
			}
		}
		else
		{
			//Save underlying network interface (AP mode)
			bcm4xxxxApInterface = interface;

			//Optionally set the station MAC address
			//if(macCompAddr(&interface->macAddr, &MAC_UNSPECIFIED_ADDR))
			{
				//Use the factory preprogrammed station address
				ret = wwd_wifi_get_mac_address((wiced_mac_t *) &interface->macAddr, WWD_STA_INTERFACE);
				TRACE_INFO("wwd_wifi_get_mac_address=%d (0x%04X)\r\n", ret, ret);
				TRACE_INFO("MAC=%s\r\n", macAddrToString(&interface->macAddr, NULL));

				//Generate the 64-bit interface identifier
				macAddrToEui64(&interface->macAddr, &interface->eui64);
			}
		}

		//End of exception handling block
	} while(0);

	//BCM4xxxx is now ready to send
	osSetEvent(&interface->nicTxEvent);

	//Successful initialization
	return NO_ERROR;
}


/**
 * @brief BCM4xxxx timer handler
 *
 * This routine is periodically called by the TCP/IP stack to
 * handle periodic operations such as polling the link state
 *
 * @param[in] interface Underlying network interface
 **/

void bcm4xxxxTick(NetInterface *interface)
{
}


/**
 * @brief Enable interrupts
 * @param[in] interface Underlying network interface
 **/

void bcm4xxxxEnableIrq(NetInterface *interface)
{
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void bcm4xxxxDisableIrq(NetInterface *interface)
{
}


/**
 * @brief BCM4xxxx interrupt service routine
 * @return TRUE if a higher priority task must be woken. Else FALSE is returned
 **/

bool_t bcm4xxxxIrqHandler(void)
{
	bool_t flag;

	//This flag will be set if a higher priority task must be woken
	flag = FALSE;

	//STA and/or AP mode?
	if(bcm4xxxxStaInterface != NULL)
		bcm4xxxxStaInterface->nicEvent = TRUE;
	else if(bcm4xxxxApInterface != NULL)
		bcm4xxxxApInterface->nicEvent = TRUE;

	//Notify the TCP/IP stack of the event
	flag = osSetEventFromIsr(&netEvent);

	//A higher priority task must be woken?
	return flag;
}


/**
 * @brief BCM4xxxx event handler
 * @param[in] interface Underlying network interface
 **/

void bcm4xxxxEventHandler(NetInterface *interface)
{
}


/**
 * @brief Send a packet
 * @param[in] interface Underlying network interface
 * @param[in] buffer Multi-part buffer containing the data to send
 * @param[in] offset Offset to the first data byte
 * @return Error code
 **/

error_t bcm4xxxxSendPacket(NetInterface *interface,
   const NetBuffer *buffer, size_t offset)
{
	wwd_result_t ret;
	wiced_buffer_t packet;
	size_t length;
	uint8_t *p;

	//Retrieve the length of the packet
	length = netBufferGetLength(buffer) - offset;

	//Allocate a network buffer
	ret = host_buffer_get(&packet, WWD_NETWORK_TX, length + WICED_LINK_OVERHEAD_BELOW_ETHERNET_FRAME_MAX, FALSE);

	//Check status code
	if(ret == WWD_SUCCESS)
	{
		//Make room for additional headers
		host_buffer_add_remove_at_front(&packet, WICED_LINK_OVERHEAD_BELOW_ETHERNET_FRAME_MAX);

		//Point to the data payload
		p = host_buffer_get_current_piece_data_pointer(packet);

		//Copy user data
		netBufferRead(p, buffer, offset, length);

		//Adjust the length of the buffer
		host_buffer_set_size(packet, length);

		//STA or AP mode?
		if(interface == bcm4xxxxStaInterface)
		{
			//Send packet
			wwd_network_send_ethernet_data(packet, WWD_STA_INTERFACE);
		}
		else
		{
			//Send packet
			wwd_network_send_ethernet_data(packet, WWD_AP_INTERFACE);
		}
	}
	else
	{
		TRACE_ERROR("##### bcm4xxxxSendPacket ALLOC FAILED ####\r\n");
	}

	//The transmitter can accept another packet
	osSetEvent(&interface->nicTxEvent);

	//Return status code
	if(ret == WWD_SUCCESS)
		return NO_ERROR;
	else
		return ERROR_FAILURE;
}


/**
 * @brief Configure multicast MAC address filtering
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t bcm4xxxxSetMulticastFilter(NetInterface *interface)
{
	uint_t i;
	wwd_result_t ret;
	MacFilterEntry *entry;

	//Debug message
	TRACE_INFO("Updating BCM4xxxx multicast filter...\r\n");

	//STA interface?
	if(interface == bcm4xxxxStaInterface)
	{
		//The MAC filter table contains the multicast MAC addresses
		//to accept when receiving an Ethernet frame
		for(i = 0; i < MAC_MULTICAST_FILTER_SIZE; i++)
		{
			//Point to the current entry
			entry = &interface->macMulticastFilter[i];

			//Check whether the MAC filter table should be updated for the
			//current multicast address
			if(!macCompAddr(&entry->addr, &MAC_UNSPECIFIED_ADDR))
			{
				if(entry->addFlag)
				{
					//Add a new entry to the MAC filter table
					//ret = wwd_wifi_register_multicast_address((wiced_mac_t *) entry->addr.b);
					//TRACE_ERROR("wwd_wifi_register_multicast_address=%d (0x%04X)\r\n", ret, ret);
				}
				else if(entry->deleteFlag)
				{
					//Remove the current entry from the MAC filter table
					//ret = wwd_wifi_unregister_multicast_address((wiced_mac_t *) entry->addr.b);
					//TRACE_ERROR("wwd_wifi_unregister_multicast_address=%d (0x%04X)\r\n", ret, ret);
				}
			}
		}
	}

	//Successful processing
	return NO_ERROR;
}


/**
 * @brief Callback function that handles Wi-Fi events
 **/

void *app_wifi_event_handler(const wwd_event_header_t *event_header, const uint8_t *event_data, void *handler_user_data)
{
	//Check event type
	switch(event_header->event_type)
	{
		//I/F change?
		case WLC_E_IF:
			TRACE_INFO("### app_wifi_event_handler: WLC_E_IF\r\n");
			break;

		//802.11 ASSOC indication?
		case WLC_E_ASSOC_IND:
			TRACE_INFO("### app_wifi_event_handler: WLC_E_ASSOC_IND\r\n");
			break;

		//802.11 DISASSOC indication?
		case WLC_E_DISASSOC_IND:
			TRACE_INFO("### app_wifi_event_handler: WLC_E_DISASSOC_IND\r\n");
			break;

		//Generic link indication?
		case WLC_E_LINK:
			//Debug message
			TRACE_INFO("### app_wifi_event_handler: WLC_E_LINK\r\n");

			//STA interface?
			if(event_header->interface == WWD_STA_INTERFACE)
			{
				if(bcm4xxxxStaInterface != NULL)
				{
					//Check link state
					if(event_header->flags & 0x01)
						bcm4xxxxStaInterface->linkState = TRUE;
					else
						bcm4xxxxStaInterface->linkState = FALSE;

					//Get exclusive access
					osAcquireMutex(&netMutex);
					//Process link state change event
					nicNotifyLinkChange(bcm4xxxxStaInterface);
					//Release exclusive access
					osReleaseMutex(&netMutex);
				}
			}
			//AP interface?
			else if(event_header->interface == WWD_AP_INTERFACE)
			{
				if(bcm4xxxxApInterface != NULL)
				{
					//Check link state
					if(event_header->flags & 0x01)
						bcm4xxxxApInterface->linkState = TRUE;
					else
						bcm4xxxxApInterface->linkState = FALSE;

					//Get exclusive access
					osAcquireMutex(&netMutex);
					//Process link state change event
					nicNotifyLinkChange(bcm4xxxxApInterface);
					//Release exclusive access
					osReleaseMutex(&netMutex);
				}
			}

			break;

		//Unknown event?
		default:
			TRACE_INFO("### app_wifi_event_handler: Unknown event\r\n");
			break;
	}

	return handler_user_data;
}


/**
 * @brief Callback function that handles incoming packets
 **/

void host_network_process_ethernet_data(wiced_buffer_t buffer, wwd_interface_t interface)
{
	size_t n;
	uint8_t *p;

	//Point to the incoming packet
	p = host_buffer_get_current_piece_data_pointer(buffer);
	//Retrieve the length of the packet
	n = host_buffer_get_current_piece_size(buffer);

	//Valid packet received?
	if(p != NULL && n > 0)
	{
		if(interface == WWD_STA_INTERFACE)
		{
			if(bcm4xxxxStaInterface != NULL)
			{
				//Get exclusive access
				osAcquireMutex(&netMutex);
				//Process link state change event
				nicProcessPacket(bcm4xxxxStaInterface, p, n);
				//Release exclusive access
				osReleaseMutex(&netMutex);
			}
		}
		else if(interface == WWD_AP_INTERFACE)
		{
			if(bcm4xxxxApInterface != NULL)
			{
				//Get exclusive access
				osAcquireMutex(&netMutex);
				//Process link state change event
				nicProcessPacket(bcm4xxxxApInterface, p, n);
				//Release exclusive access
				osReleaseMutex(&netMutex);
			}
		}
	}

	//Release network buffer
	host_buffer_release(buffer, WWD_NETWORK_RX);
}


//Miscellaneous WICED dependencies
signed int xTaskIsTaskFinished(void *xTask)
{
	TRACE_INFO("### xTaskIsTaskFinished\r\n");
	return pdTRUE;
}

portBASE_TYPE vTaskFreeTerminated(TaskHandle_t xTask)
{
	TRACE_INFO("### vTaskFreeTerminated\r\n");
	return pdTRUE;
}
