//Dependencies
#include "core/net.h"
#include "os/include/m2m_wifi_ex.h"


sint8 os_m2m_wifi_download_mode(void)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_download_mode();

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_init(tstrWifiInitParam *pWifiInitParam)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_init(pWifiInitParam);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_deinit(void *arg)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_deinit(arg);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_handle_events(void *arg)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_handle_events(arg);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_default_connect(void)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_default_connect();

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_connect(char *pcSsid, uint8 u8SsidLen, uint8 u8SecType, void *pvAuthInfo, uint16 u16Ch)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Select the relevant interface
   status = m2m_wifi_set_control_ifc(INTERFACE_1);

   //Check status code
   if(status == M2M_SUCCESS)
   {
      //Invoke underlying function
      status = m2m_wifi_connect(pcSsid, u8SsidLen, u8SecType, pvAuthInfo, u16Ch);
   }

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_disconnect(void)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Select the relevant interface
   status = m2m_wifi_set_control_ifc(INTERFACE_1);

   //Check status code
   if(status == M2M_SUCCESS)
   {
      //Invoke underlying function
      status = m2m_wifi_disconnect();
   }

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}

#ifndef M2M_WILC1000
sint8 os_m2m_wifi_start_provision_mode(tstrM2MAPConfig *pstrAPConfig, char *pcHttpServerDomainName, uint8 bEnableHttpRedirect)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_start_provision_mode(pstrAPConfig, pcHttpServerDomainName, bEnableHttpRedirect);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}
#endif

#ifndef M2M_WILC1000
sint8 os_m2m_wifi_stop_provision_mode(void)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_stop_provision_mode();

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}
#endif


sint8 os_m2m_wifi_get_connection_info(void)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_get_connection_info();

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_set_mac_address(uint8 *au8MacAddress)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_set_mac_address(au8MacAddress);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_wps(uint8 u8TriggerType, const char *pcPinNumber)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_wps(u8TriggerType, pcPinNumber);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_wps_disable(void)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_wps_disable();

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_p2p(uint8 u8Channel)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_p2p(u8Channel);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_p2p_disconnect(void)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_p2p_disconnect();

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_enable_ap(const tstrM2MAPConfig *pstrM2MAPConfig)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Select the relevant interface
   status = m2m_wifi_set_control_ifc(INTERFACE_2);

   //Check status code
   if(status == M2M_SUCCESS)
   {
      //Invoke underlying function
      status = m2m_wifi_enable_ap(pstrM2MAPConfig);
   }

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_disable_ap(void)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Select the relevant interface
   status = m2m_wifi_set_control_ifc(INTERFACE_2);

   //Check status code
   if(status == M2M_SUCCESS)
   {
      //Invoke underlying function
      status = m2m_wifi_disable_ap();
   }

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_ap_get_assoc_info(void)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_ap_get_assoc_info();

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_set_static_ip(tstrM2MIPConfig *pstrStaticIPConf)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_set_static_ip(pstrStaticIPConf);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_request_dhcp_client(void)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_request_dhcp_client();

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_request_dhcp_server(uint8 *addr)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_request_dhcp_server(addr);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_set_scan_options(uint8 u8NumOfSlot, uint8 u8SlotTime)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_set_scan_options(u8NumOfSlot, u8SlotTime);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_set_scan_region(uint8 ScanRegion)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_set_scan_region(ScanRegion);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_set_scan_list(tstrM2MScanList *pstrScanList)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_set_scan_list(pstrScanList);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_request_scan(uint8 ch)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_request_scan(ch);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_request_scan_ssid(uint8 ch, char *pcssid)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_request_scan_ssid(ch, pcssid);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


uint8 os_m2m_wifi_get_num_ap_found(void)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_get_num_ap_found();

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_req_scan_result(uint8 index)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_req_scan_result(index);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_req_curr_rssi(void)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_req_curr_rssi();

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_get_otp_mac_address(uint8 *pu8MacAddr, uint8 *pu8IsValid)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_get_otp_mac_address(pu8MacAddr, pu8IsValid);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_get_mac_address(uint8 *pu8MacAddr)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_get_mac_address(pu8MacAddr);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_set_sleep_mode(uint8 PsTyp, uint8 BcastEn)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_set_sleep_mode(PsTyp, BcastEn);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_request_sleep(uint32 u32SlpReqTime)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_request_sleep(u32SlpReqTime);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


uint8 os_m2m_wifi_get_sleep_mode(void)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_get_sleep_mode();

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}

#ifndef M2M_WILC1000
sint8 os_m2m_wifi_req_client_ctrl(uint8 cmd)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_req_client_ctrl(cmd);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}
#endif

#ifndef M2M_WILC1000
sint8 os_m2m_wifi_req_server_init(uint8 ch)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_req_server_init(ch);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}
#endif


sint8 os_m2m_wifi_set_device_name(uint8 *pu8DeviceName, uint8 u8DeviceNameLength)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_set_device_name(pu8DeviceName, u8DeviceNameLength);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_set_lsn_int(tstrM2mLsnInt *pstrM2mLsnInt)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_set_lsn_int(pstrM2mLsnInt);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}



sint8 os_m2m_wifi_enable_monitoring_mode(tstrM2MWifiMonitorModeCtrl *pstrMtrCtrl,
   uint8 *pu8PayloadBuffer, uint16 u16BufferSize, uint16 u16DataOffset)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_enable_monitoring_mode(pstrMtrCtrl,
      pu8PayloadBuffer, u16BufferSize, u16DataOffset);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}



sint8 os_m2m_wifi_disable_monitoring_mode(void)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_disable_monitoring_mode();

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_change_monitoring_channel(uint8 u8ChannelID)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_change_monitoring_channel(u8ChannelID);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_send_wlan_pkt(uint8 *pu8WlanPacket, uint16 u16WlanHeaderLength, uint16 u16WlanPktSize)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_send_wlan_pkt(pu8WlanPacket, u16WlanHeaderLength, u16WlanPktSize);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_send_ethernet_pkt(uint8 *pu8Packet, uint16 u16PacketSize)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_send_ethernet_pkt(pu8Packet, u16PacketSize);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


#ifndef M2M_WILC1000
sint8 os_m2m_wifi_enable_sntp(uint8 bEnable)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_enable_sntp(bEnable);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}
#endif

sint8 os_m2m_wifi_set_sytem_time(uint32 u32UTCSeconds)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_set_sytem_time(u32UTCSeconds);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_set_cust_InfoElement(uint8 *pau8M2mCustInfoElement)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_set_cust_InfoElement(pau8M2mCustInfoElement);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_enable_mac_mcast(uint8 *pu8MulticastMacAddress, uint8 u8AddRemove)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_enable_mac_mcast(pu8MulticastMacAddress, u8AddRemove);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_set_receive_buffer(void *pvBuffer, uint16 u16BufferLen)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_set_receive_buffer(pvBuffer, u16BufferLen);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_download_cert(uint8 *pCertData, uint32 u32CertSize)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_download_cert(pCertData, u32CertSize);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_set_control_ifc(uint8 u8IfcId)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_set_control_ifc(u8IfcId);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}


sint8 os_m2m_wifi_send_ethernet_pkt_ifc1(uint8 *pu8Packet, uint16 u16PacketSize)
{
   sint8 status;

   //Get exclusive access
   osAcquireMutex(&netMutex);

   //Invoke underlying function
   status = m2m_wifi_send_ethernet_pkt_ifc1(pu8Packet, u16PacketSize);

   //Release exclusive access
   osReleaseMutex(&netMutex);
   
   //Return status code
   return status;
}
