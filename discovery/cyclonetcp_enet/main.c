/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_uart_freertos.h"
#include "fsl_uart.h"
#include "fsl_epit.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
#include "os_port.h"
#include "core/net.h"
#include "drivers/mac/mimx6ul_eth_driver.h"
#include "drivers/phy/lan8720_driver.h"
#include "dhcp/dhcp_client.h"
#include "ipv6/slaac.h"
#include "http/http_server.h"
#include "http/mime.h"
#include "path.h"
#include "date_time.h"
#include "resource_manager.h"
#include "debug.h"

#include "fsl_enet.h"

#include "pin_mux.h"
#include "fsl_cache.h"
#include "sd_init.h"

#define CONFIG_USE_PORT2 0
//Application configuration
#define ENET1_MAC_ADDR "02-E7-F5-EF-10-52"

#define ENET1_USE_DHCP ENABLED
//#define ENET1_USE_DHCP DISABLED
#define ENET1_IPV4_HOST_ADDR "192.168.3.220"
#define ENET1_IPV4_SUBNET_MASK "255.255.255.0"
#define ENET1_IPV4_DEFAULT_GATEWAY "192.168.3.1"
#define ENET1_IPV4_PRIMARY_DNS "192.168.3.1"
#define ENET1_IPV4_SECONDARY_DNS "192.168.3.1"

//#define ENET1_USE_SLAAC DISABLED
#define ENET1_USE_SLAAC ENABLED
#define ENET1_IPV6_LINK_LOCAL_ADDR "fe80::1052"
#define ENET1_IPV6_PREFIX "2001:db8::"
#define ENET1_IPV6_PREFIX_LENGTH 64
#define ENET1_IPV6_GLOBAL_ADDR "2001:db8::1052"
#define ENET1_IPV6_ROUTER "fe80::1"
#define ENET1_IPV6_PRIMARY_DNS "2001:4860:4860::8888"
#define ENET1_IPV6_SECONDARY_DNS "2001:4860:4860::8844"

#define ENET1_HTTP_MAX_CONNECTIONS 4
//Global variables
DhcpClientSettings dhcpClientSettingsENET1;
DhcpClientContext dhcpClientContextENET1;
SlaacSettings slaacSettingsENET1;
SlaacContext slaacContextENET1;
HttpServerSettings httpServerSettingsENET1;
HttpServerContext httpServerContextENET1;
HttpConnection httpConnectionsENET1[ENET1_HTTP_MAX_CONNECTIONS];

#define ENET2_HTTP_MAX_CONNECTIONS 4
HttpServerSettings httpServerSettingsENET2;
HttpServerContext httpServerContextENET2;
HttpConnection httpConnectionsENET2[ENET2_HTTP_MAX_CONNECTIONS];

#if CONFIG_USE_PORT2
//Application configuration
#define ENET2_MAC_ADDR "02-E7-F5-EF-10-53"

#define ENET2_USE_DHCP ENABLED
//#define ENET2_USE_DHCP DISABLED
#define ENET2_IPV4_HOST_ADDR "192.168.3.221"
#define ENET2_IPV4_SUBNET_MASK "255.255.255.0"
#define ENET2_IPV4_DEFAULT_GATEWAY "192.168.3.1"
#define ENET2_IPV4_PRIMARY_DNS "192.168.3.1"
#define ENET2_IPV4_SECONDARY_DNS "192.168.3.1"

//#define ENET2_USE_SLAAC DISABLED
#define ENET2_USE_SLAAC ENABLED
#define ENET2_IPV6_LINK_LOCAL_ADDR "fe80::1053"
#define ENET2_IPV6_PREFIX "2001:db8::"
#define ENET2_IPV6_PREFIX_LENGTH 64
#define ENET2_IPV6_GLOBAL_ADDR "2001:db8::1053"
#define ENET2_IPV6_ROUTER "fe80::1"
#define ENET2_IPV6_PRIMARY_DNS "2001:4860:4860::8888"
#define ENET2_IPV6_SECONDARY_DNS "2001:4860:4860::8844"
//Global variables
DhcpClientSettings dhcpClientSettingsENET2;
DhcpClientContext dhcpClientContextENET2;
SlaacSettings slaacSettingsENET2;
SlaacContext slaacContextENET2;
#endif

//Forward declaration of functions
error_t httpServerCgiCallback(HttpConnection *connection,
   const char_t *param);

error_t httpServerUriNotFoundCallback(HttpConnection *connection,
   const char_t *uri);


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define LV_TICK_TIMER_EPIT_IRQn	EPIT2_IRQn
#define LV_TICK_TIMER_EPIT	EPIT2
/* Get source clock for EPIT driver (EPIT prescaler = 0) */
#define LV_TICK_TIMER_EPIT_CLK_FREQ (CLOCK_GetFreq(kCLOCK_IpgClk) / \
                              (CLOCK_GetDiv(kCLOCK_PerclkDiv) + 1U))


/*******************************************************************************
 * Code
 ******************************************************************************/

/*Provides own implementation of vApplicationIRQHandler() to call SystemIrqHandler()
 *directly without saving the FPU registers on interrupt entry.
 */
void vApplicationIRQHandler(uint32_t ulICCIAR)
{
	SystemIrqHandler(ulICCIAR);
}

DhcpState dhcpPrevState_ENET1 = DHCP_STATE_INIT;
DhcpState dhcpPrevState_ENET2 = DHCP_STATE_INIT;

void dhcpClientStateChangeCallback(DhcpClientCtx *context,
   NetInterface *interface, DhcpState state)
{
	char ipBuffer[32];
	Ipv4Addr ipv4Addr;
	
	//DHCP process complete?
	if(interface->index==0){
		if(state == DHCP_STATE_BOUND && dhcpPrevState_ENET1 == DHCP_STATE_PROBING)
		{
			ipv4Addr=interface->ipv4Context.addr;
			TRACE_INFO("ENET1 Set IP:%s\r\n",ipv4AddrToString(ipv4Addr,ipBuffer));
			ipv4Addr=interface->ipv4Context.subnetMask;
			TRACE_INFO("ENET1 Set Sub Net:%s\r\n",ipv4AddrToString(ipv4Addr,ipBuffer));
			ipv4Addr=interface->ipv4Context.defaultGateway;
			TRACE_INFO("ENET1 Set Gate:%s\r\n",ipv4AddrToString(ipv4Addr,ipBuffer));
			ipv4Addr=interface->ipv4Context.dnsServerList[0];
			TRACE_INFO("ENET1 Set DNS1:%s\r\n",ipv4AddrToString(ipv4Addr,ipBuffer));
			ipv4Addr=interface->ipv4Context.dnsServerList[1];
			TRACE_INFO("ENET1 Set DNS2:%s\r\n",ipv4AddrToString(ipv4Addr,ipBuffer));
		}

		//Save current state
		dhcpPrevState_ENET1 = state;
	}else{
		if(state == DHCP_STATE_BOUND && dhcpPrevState_ENET2 == DHCP_STATE_PROBING)
		{
			ipv4Addr=interface->ipv4Context.addr;
			TRACE_INFO("ENET2 Set IP:%s\r\n",ipv4AddrToString(ipv4Addr,ipBuffer));
			ipv4Addr=interface->ipv4Context.subnetMask;
			TRACE_INFO("ENET2 ENET2 Set Sub Net:%s\r\n",ipv4AddrToString(ipv4Addr,ipBuffer));
			ipv4Addr=interface->ipv4Context.defaultGateway;
			TRACE_INFO("ENET2 Set Gate:%s\r\n",ipv4AddrToString(ipv4Addr,ipBuffer));
			ipv4Addr=interface->ipv4Context.dnsServerList[0];
			TRACE_INFO("ENET2 Set DNS1:%s\r\n",ipv4AddrToString(ipv4Addr,ipBuffer));
			ipv4Addr=interface->ipv4Context.dnsServerList[1];
			TRACE_INFO("ENET2 Set DNS2:%s\r\n",ipv4AddrToString(ipv4Addr,ipBuffer));
		}

		//Save current state
		dhcpPrevState_ENET2 = state;
	}
}

void networkTask(void *param)
{
	error_t error;
   	NetInterface *interfacePort1;
#if CONFIG_USE_PORT2
   	NetInterface *interfacePort2;
#endif
	MacAddr macAddr;
#if ((ENET1_USE_DHCP == DISABLED)||(ENET2_USE_DHCP == DISABLED))
   	Ipv4Addr ipv4Addr;
#endif
	
#if (ENET1_USE_SLAAC == DISABLED)
   	Ipv6Addr ipv6Addr;
#endif
	InitSD();
	error = netInit();
	//Any error to report?
	if(error)
	{
		//Debug message
		TRACE_ERROR("Failed to initialize TCP/IP stack!\r\n");
	}
	//Configure the first Ethernet interface
	interfacePort1 = &netInterface[0];
	//Set interface name
	interfacePort1->index=0;
	netSetInterfaceName(interfacePort1, "eth0");
	//Set host name
	netSetHostname(interfacePort1, "ENET1");
	//Select the relevant network adapter
	//netSetDriver(interfacePort1, &bcm4xxxxStaDriver);
	netSetDriver(interfacePort1, &mimx6ulEthPort1Driver);
	netSetPhyDriver(interfacePort1, &lan8720PhyDriver);
	//Set host MAC address
	macStringToAddr(ENET1_MAC_ADDR, &macAddr);
	netSetMacAddr(interfacePort1, &macAddr);
	//Initialize network interfacePort1
	error = netConfigInterface(interfacePort1);
	//Any error to report?
	if(error)
	{
      		//Debug message
		TRACE_ERROR("ENET1 Failed to configure interface %s!\r\n", interfacePort1->name);
	}

#if (IPV4_SUPPORT == ENABLED)
#if (ENET1_USE_DHCP == ENABLED)
   	//Get default settings
   	dhcpClientGetDefaultSettings(&dhcpClientSettingsENET1);
   	//Set the network interface to be configured by DHCP
   	dhcpClientSettingsENET1.interface = interfacePort1;
   	//Disable rapid commit option
   	dhcpClientSettingsENET1.rapidCommit = FALSE;
	//FSM state change event
	dhcpClientSettingsENET1.stateChangeEvent = dhcpClientStateChangeCallback;

   	//DHCP client initialization
   	error = dhcpClientInit(&dhcpClientContextENET1, &dhcpClientSettingsENET1);
   	//Failed to initialize DHCP client?
   	if(error)
   	{
		//Debug message
      		TRACE_ERROR("ENET1 Failed to initialize DHCP client!\r\n");
   	}

   	//Start DHCP client
   	error = dhcpClientStart(&dhcpClientContextENET1);
   	//Failed to start DHCP client?
   	if(error)
   	{
      		//Debug message
      		TRACE_ERROR("ENET1 Failed to start DHCP client!\r\n");
   	}
#else
   	//Set IPv4 host address
   	ipv4StringToAddr(ENET1_IPV4_HOST_ADDR, &ipv4Addr);
   	ipv4SetHostAddr(interfacePort1, ipv4Addr);

   	//Set subnet mask
   	ipv4StringToAddr(ENET1_IPV4_SUBNET_MASK, &ipv4Addr);
   	ipv4SetSubnetMask(interfacePort1, ipv4Addr);

   	//Set default gateway
   	ipv4StringToAddr(ENET1_IPV4_DEFAULT_GATEWAY, &ipv4Addr);
   	ipv4SetDefaultGateway(interfacePort1, ipv4Addr);

   	//Set primary and secondary DNS servers
   	ipv4StringToAddr(ENET1_IPV4_PRIMARY_DNS, &ipv4Addr);
   	ipv4SetDnsServer(interfacePort1, 0, ipv4Addr);
   	ipv4StringToAddr(ENET1_IPV4_SECONDARY_DNS, &ipv4Addr);
   	ipv4SetDnsServer(interfacePort1, 1, ipv4Addr);
#endif
#endif

#if (IPV6_SUPPORT == ENABLED)
#if (ENET1_USE_SLAAC == ENABLED)
   	//Get default settings
   	slaacGetDefaultSettings(&slaacSettingsENET1);
   	//Set the network interfacePort1 to be configured
   	slaacSettingsENET1.interface = interfacePort1;

   	//SLAAC initialization
   	error = slaacInit(&slaacContextENET1, &slaacSettingsENET1);
   	//Failed to initialize SLAAC?
   	if(error)
   	{
      		//Debug message
      		TRACE_ERROR("ENET1 Failed to initialize SLAAC!\r\n");
   	}

   	//Start IPv6 address autoconfiguration process
   	error = slaacStart(&slaacContextENET1);
   	//Failed to start SLAAC process?
   	if(error)
   	{
      		//Debug message
      		TRACE_ERROR("ENET1 Failed to start SLAAC!\r\n");
   	}
#else
   	//Set link-local address
   	ipv6StringToAddr(ENET1_IPV6_LINK_LOCAL_ADDR, &ipv6Addr);
   	ipv6SetLinkLocalAddr(interfacePort1, &ipv6Addr);

   	//Set IPv6 prefix
   	ipv6StringToAddr(ENET1_IPV6_PREFIX, &ipv6Addr);
   	ipv6SetPrefix(interfacePort1, 0, &ipv6Addr, ENET1_IPV6_PREFIX_LENGTH);

   	//Set global address
   	ipv6StringToAddr(ENET1_IPV6_GLOBAL_ADDR, &ipv6Addr);
   	ipv6SetGlobalAddr(interfacePort1, 0, &ipv6Addr);

   	//Set default router
   	ipv6StringToAddr(ENET1_IPV6_ROUTER, &ipv6Addr);
   	ipv6SetDefaultRouter(interfacePort1, 0, &ipv6Addr);

   	//Set primary and secondary DNS servers
   	ipv6StringToAddr(ENET1_IPV6_PRIMARY_DNS, &ipv6Addr);
   	ipv6SetDnsServer(interfacePort1, 0, &ipv6Addr);
   	ipv6StringToAddr(ENET1_IPV6_SECONDARY_DNS, &ipv6Addr);
   	ipv6SetDnsServer(interfacePort1, 1, &ipv6Addr);
#endif
#endif
#if CONFIG_USE_PORT2
	interfacePort2 = &netInterface[1];
	interfacePort2->index=1;
	netSetInterfaceName(interfacePort2, "eth1");
	//Set host name
	netSetHostname(interfacePort2, "ENET2");
	//Select the relevant network adapter
	netSetDriver(interfacePort2, &mimx6ulEthPort2Driver);
	netSetPhyDriver(interfacePort2, &lan8720PhyDriver);
	//Set host MAC address
	macStringToAddr(ENET2_MAC_ADDR, &macAddr);
	netSetMacAddr(interfacePort2, &macAddr);
	//Initialize network interfacePort2
	error = netConfigInterface(interfacePort2);
	//Any error to report?
	if(error)
	{
      		//Debug message
		TRACE_ERROR("ENET2 Failed to configure interface %s!\r\n", interfacePort2->name);
	}

#if (IPV4_SUPPORT == ENABLED)
#if (ENET2_USE_DHCP == ENABLED)
   	//Get default settings
   	dhcpClientGetDefaultSettings(&dhcpClientSettingsENET2);
   	//Set the network interface to be configured by DHCP
   	dhcpClientSettingsENET2.interface = interfacePort2;
   	//Disable rapid commit option
   	dhcpClientSettingsENET2.rapidCommit = FALSE;
	//FSM state change event
	dhcpClientSettingsENET2.stateChangeEvent = dhcpClientStateChangeCallback;

   	//DHCP client initialization
   	error = dhcpClientInit(&dhcpClientContextENET2, &dhcpClientSettingsENET2);
   	//Failed to initialize DHCP client?
   	if(error)
   	{
		//Debug message
      		TRACE_ERROR("ENET2 Failed to initialize DHCP client!\r\n");
   	}

   	//Start DHCP client
   	error = dhcpClientStart(&dhcpClientContextENET2);
   	//Failed to start DHCP client?
   	if(error)
   	{
      		//Debug message
      		TRACE_ERROR("ENET2 Failed to start DHCP client!\r\n");
   	}
#else
   	//Set IPv4 host address
   	ipv4StringToAddr(ENET2_IPV4_HOST_ADDR, &ipv4Addr);
   	ipv4SetHostAddr(interfacePort2, ipv4Addr);

   	//Set subnet mask
   	ipv4StringToAddr(ENET2_IPV4_SUBNET_MASK, &ipv4Addr);
   	ipv4SetSubnetMask(interfacePort2, ipv4Addr);

   	//Set default gateway
   	ipv4StringToAddr(ENET2_IPV4_DEFAULT_GATEWAY, &ipv4Addr);
   	ipv4SetDefaultGateway(interfacePort2, ipv4Addr);

   	//Set primary and secondary DNS servers
   	ipv4StringToAddr(ENET2_IPV4_PRIMARY_DNS, &ipv4Addr);
   	ipv4SetDnsServer(interfacePort2, 0, ipv4Addr);
   	ipv4StringToAddr(ENET2_IPV4_SECONDARY_DNS, &ipv4Addr);
   	ipv4SetDnsServer(interfacePort2, 1, ipv4Addr);
#endif
#endif

#if (IPV6_SUPPORT == ENABLED)
#if (ENET2_USE_SLAAC == ENABLED)
   	//Get default settings
   	slaacGetDefaultSettings(&slaacSettingsENET2);
   	//Set the network interfacePort1 to be configured
   	slaacSettingsENET2.interface = interfacePort2;

   	//SLAAC initialization
   	error = slaacInit(&slaacContextENET2, &slaacSettingsENET2);
   	//Failed to initialize SLAAC?
   	if(error)
   	{
      		//Debug message
      		TRACE_ERROR("ENET2 Failed to initialize SLAAC!\r\n");
   	}

   	//Start IPv6 address autoconfiguration process
   	error = slaacStart(&slaacContextENET2);
   	//Failed to start SLAAC process?
   	if(error)
   	{
      		//Debug message
      		TRACE_ERROR("ENET2 Failed to start SLAAC!\r\n");
   	}
#else
   	//Set link-local address
   	ipv6StringToAddr(ENET2_IPV6_LINK_LOCAL_ADDR, &ipv6Addr);
   	ipv6SetLinkLocalAddr(interfacePort2, &ipv6Addr);

   	//Set IPv6 prefix
   	ipv6StringToAddr(ENET2_IPV6_PREFIX, &ipv6Addr);
   	ipv6SetPrefix(interfacePort2, 0, &ipv6Addr, ENET2_IPV6_PREFIX_LENGTH);

   	//Set global address
   	ipv6StringToAddr(ENET2_IPV6_GLOBAL_ADDR, &ipv6Addr);
   	ipv6SetGlobalAddr(interfacePort2, 0, &ipv6Addr);

   	//Set default router
   	ipv6StringToAddr(ENET2_IPV6_ROUTER, &ipv6Addr);
   	ipv6SetDefaultRouter(interfacePort2, 0, &ipv6Addr);

   	//Set primary and secondary DNS servers
   	ipv6StringToAddr(ENET2_IPV6_PRIMARY_DNS, &ipv6Addr);
   	ipv6SetDnsServer(interfacePort2, 0, &ipv6Addr);
   	ipv6StringToAddr(ENET2_IPV6_SECONDARY_DNS, &ipv6Addr);
   	ipv6SetDnsServer(interfacePort2, 1, &ipv6Addr);
#endif
#endif
#endif	
   	//Get default settings
   	httpServerGetDefaultSettings(&httpServerSettingsENET1);
   	//Bind HTTP server to the desired interface
   	httpServerSettingsENET1.interface = &netInterface[0];
   	//Listen to port 80
   	httpServerSettingsENET1.port = HTTP_PORT;
   	//Client connections
   	httpServerSettingsENET1.maxConnections = ENET1_HTTP_MAX_CONNECTIONS;
   	httpServerSettingsENET1.connections = httpConnectionsENET1;
   	//Specify the server's root directory
   	strcpy(httpServerSettingsENET1.rootDirectory, "/www/");
   	//Set default home page
   	strcpy(httpServerSettingsENET1.defaultDocument, "index.shtm");
   	//Callback functions
   	httpServerSettingsENET1.cgiCallback = httpServerCgiCallback;
   	httpServerSettingsENET1.uriNotFoundCallback = httpServerUriNotFoundCallback;

   	//HTTP server initialization
   	error = httpServerInit(&httpServerContextENET1, &httpServerSettingsENET1);
   	//Failed to initialize HTTP server?
   	if(error)
   	{
      		//Debug message
      		TRACE_ERROR("Failed to initialize ENET1 HTTP server!\r\n");
   	}

   	//Start HTTP server
   	error = httpServerStart(&httpServerContextENET1);
   	//Failed to start HTTP server?
   	if(error)
   	{
      		//Debug message
      		TRACE_ERROR("Failed to start ENET1 HTTP server!\r\n");
   	}
#if CONFIG_USE_PORT2
	//Get default settings
   	httpServerGetDefaultSettings(&httpServerSettingsENET2);
   	//Bind HTTP server to the desired interface
   	httpServerSettingsENET2.interface = &netInterface[1];
   	//Listen to port 80
   	httpServerSettingsENET2.port = HTTP_PORT;
   	//Client connections
   	httpServerSettingsENET2.maxConnections = ENET2_HTTP_MAX_CONNECTIONS;
   	httpServerSettingsENET2.connections = httpConnectionsENET2;
   	//Specify the server's root directory
   	strcpy(httpServerSettingsENET2.rootDirectory, "/www/");
   	//Set default home page
   	strcpy(httpServerSettingsENET2.defaultDocument, "index.shtm");
   	//Callback functions
   	httpServerSettingsENET2.cgiCallback = httpServerCgiCallback;
   	httpServerSettingsENET2.uriNotFoundCallback = httpServerUriNotFoundCallback;

   	//HTTP server initialization
   	error = httpServerInit(&httpServerContextENET2, &httpServerSettingsENET2);
   	//Failed to initialize HTTP server?
   	if(error)
   	{
      		//Debug message
      		TRACE_ERROR("Failed to initialize ENET2 HTTP server!\r\n");
   	}

   	//Start HTTP server
   	error = httpServerStart(&httpServerContextENET2);
   	//Failed to start HTTP server?
   	if(error)
   	{
      		//Debug message
      		TRACE_ERROR("Failed to start ENET2 HTTP server!\r\n");
   	}
#endif
	while(1)
	{
		vTaskDelay(10);
	}
}
/*!
 * @brief Application entry point.
 */

int main(void)
{
   	OsTask *task;
   	
	/* Init board hardware. */
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitMemory();
	/* Install IRQ Handler */
	SystemInitIrqTable();
	osInitKernel();
	//Configure debug UART
	debugInit(115200);
	//Start-up message
	TRACE_INFO("\r\n");
	TRACE_INFO("**********************************\r\n");
	TRACE_INFO("*** CycloneTCP Web Server Demo ***\r\n");
	TRACE_INFO("**********************************\r\n");
	TRACE_INFO("Copyright: 2010-2017 Oryx Embedded SARL\r\n");
	TRACE_INFO("Compiled: %s %s\r\n", __DATE__, __TIME__);
	TRACE_INFO("Target: i.MX6UL\r\n");
	TRACE_INFO("\r\n");
   	//Create a task to blink the LED
   	task = osCreateTask("Network", networkTask, NULL, 1024, OS_TASK_PRIORITY_NORMAL);
   	//Failed to create the task?
   	if(task == OS_INVALID_HANDLE)
   	{
      		//Debug message
      		TRACE_ERROR("Failed to create task!\r\n");
   	}
   	//Start the execution of tasks
   	osStartKernel();
   	//This function should never return
   	return 0;
}


/**
 * @brief CGI callback function
 * @param[in] connection Handle referencing a client connection
 * @param[in] param NULL-terminated string that contains the CGI parameter
 * @return Error code
 **/

error_t httpServerCgiCallback(HttpConnection *connection,
   const char_t *param)
{
   static uint_t pageCounter = 0;
   uint_t length;
   MacAddr macAddr;
#if (IPV4_SUPPORT == ENABLED)
   Ipv4Addr ipv4Addr;
#endif
#if (IPV6_SUPPORT == ENABLED)
   uint_t n;
   Ipv6Addr ipv6Addr;
#endif

   //Underlying network interface
   NetInterface *interface = connection->socket->interface;

   //Check parameter name
   if(!strcasecmp(param, "PAGE_COUNTER"))
   {
      pageCounter++;
      sprintf(connection->buffer, "%u time%s", pageCounter, (pageCounter >= 2) ? "s" : "");
   }
   else if(!strcasecmp(param, "BOARD_NAME"))
   {
      strcpy(connection->buffer, "MIMX6UL-EVK");
   }
   else if(!strcasecmp(param, "SYSTEM_TIME"))
   {
      systime_t time = osGetSystemTime();
      formatSystemTime(time, connection->buffer);
   }
   else if(!strcasecmp(param, "MAC_ADDR"))
   {
      netGetMacAddr(interface, &macAddr);
      macAddrToString(&macAddr, connection->buffer);
   }
   else if(!strcasecmp(param, "IPV4_ADDR"))
   {
      ipv4GetHostAddr(interface, &ipv4Addr);
      ipv4AddrToString(ipv4Addr, connection->buffer);
   }
   else if(!strcasecmp(param, "SUBNET_MASK"))
   {
      ipv4GetSubnetMask(interface, &ipv4Addr);
      ipv4AddrToString(ipv4Addr, connection->buffer);
   }
   else if(!strcasecmp(param, "DEFAULT_GATEWAY"))
   {
      ipv4GetDefaultGateway(interface, &ipv4Addr);
      ipv4AddrToString(ipv4Addr, connection->buffer);
   }
   else if(!strcasecmp(param, "IPV4_PRIMARY_DNS"))
   {
      ipv4GetDnsServer(interface, 0, &ipv4Addr);
      ipv4AddrToString(ipv4Addr, connection->buffer);
   }
   else if(!strcasecmp(param, "IPV4_SECONDARY_DNS"))
   {
      ipv4GetDnsServer(interface, 1, &ipv4Addr);
      ipv4AddrToString(ipv4Addr, connection->buffer);
   }
#if (IPV6_SUPPORT == ENABLED)
   else if(!strcasecmp(param, "LINK_LOCAL_ADDR"))
   {
      ipv6GetLinkLocalAddr(interface, &ipv6Addr);
      ipv6AddrToString(&ipv6Addr, connection->buffer);
   }
   else if(!strcasecmp(param, "GLOBAL_ADDR"))
   {
      ipv6GetGlobalAddr(interface, 0, &ipv6Addr);
      ipv6AddrToString(&ipv6Addr, connection->buffer);
   }
   else if(!strcasecmp(param, "IPV6_PREFIX"))
   {
      ipv6GetPrefix(interface, 0, &ipv6Addr, &n);
      ipv6AddrToString(&ipv6Addr, connection->buffer);
      length = strlen(connection->buffer);
      sprintf(connection->buffer + length, "/%u", n);
   }
   else if(!strcasecmp(param, "ROUTER"))
   {
      ipv6GetDefaultRouter(interface, 0, &ipv6Addr);
      ipv6AddrToString(&ipv6Addr, connection->buffer);
   }
   else if(!strcasecmp(param, "IPV6_PRIMARY_DNS"))
   {
      ipv6GetDnsServer(interface, 0, &ipv6Addr);
      ipv6AddrToString(&ipv6Addr, connection->buffer);
   }
   else if(!strcasecmp(param, "IPV6_SECONDARY_DNS"))
   {
      ipv6GetDnsServer(interface, 1, &ipv6Addr);
      ipv6AddrToString(&ipv6Addr, connection->buffer);
   }
#endif
   else
   {
      return ERROR_INVALID_TAG;
   }

   //Get the length of the resulting string
   length = strlen(connection->buffer);

   //Send the contents of the specified environment variable
   return httpWriteStream(connection, connection->buffer, length);
}


/**
 * @brief URI not found callback
 * @param[in] connection Handle referencing a client connection
 * @param[in] uri NULL-terminated string containing the path to the requested resource
 * @return Error code
 **/

error_t httpServerUriNotFoundCallback(HttpConnection *connection,
   const char_t *uri)
{
   //Not implemented
   return ERROR_NOT_FOUND;
}
