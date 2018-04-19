#ifndef _M2M_WIFI_EX_H
#define _M2M_WIFI_EX_H

//Dependencies
#include "driver/include/m2m_wifi.h"

//Thread-safe functions
sint8 os_m2m_wifi_init(tstrWifiInitParam *pWifiInitParam);
sint8 os_m2m_wifi_deinit(void *arg);
sint8 os_m2m_wifi_handle_events(void *arg);
sint8 os_m2m_wifi_default_connect(void);
sint8 os_m2m_wifi_connect(char *pcSsid, uint8 u8SsidLen, uint8 u8SecType, void *pvAuthInfo, uint16 u16Ch);
sint8 os_m2m_wifi_connect_sc(char *pcSsid, uint8 u8SsidLen, uint8 u8SecType, void *pvAuthInfo, uint16 u16Ch, uint8 u8SaveCred);
sint8 os_m2m_wifi_disconnect(void);
sint8 os_m2m_wifi_start_provision_mode(tstrM2MAPConfig *pstrAPConfig, char *pcHttpServerDomainName, uint8 bEnableHttpRedirect);
sint8 os_m2m_wifi_stop_provision_mode(void);
sint8 os_m2m_wifi_get_connection_info(void);
sint8 os_m2m_wifi_set_mac_address(uint8 au8MacAddress[6]);
sint8 os_m2m_wifi_wps(uint8 u8TriggerType, const char *pcPinNumber);
sint8 os_m2m_wifi_wps_disable(void);
sint8 os_m2m_wifi_p2p(uint8 u8Channel);
sint8 os_m2m_wifi_p2p_disconnect(void);
sint8 os_m2m_wifi_enable_ap(const tstrM2MAPConfig *pstrM2MAPConfig);
sint8 os_m2m_wifi_disable_ap(void);
sint8 os_m2m_wifi_set_static_ip(tstrM2MIPConfig *pstrStaticIPConf);
sint8 os_m2m_wifi_request_dhcp_client(void);
sint8 os_m2m_wifi_request_dhcp_server(uint8 *addr);
sint8 os_m2m_wifi_enable_dhcp(uint8 u8DhcpEn);
sint8 os_m2m_wifi_set_scan_options(tstrM2MScanOption *ptstrM2MScanOption);
sint8 os_m2m_wifi_set_scan_region(uint16 ScanRegion);
sint8 os_m2m_wifi_request_scan(uint8 ch);
uint8 os_m2m_wifi_get_num_ap_found(void);
sint8 os_m2m_wifi_req_scan_result(uint8 index);
sint8 os_m2m_wifi_req_curr_rssi(void);
sint8 os_m2m_wifi_get_otp_mac_address(uint8 *pu8MacAddr, uint8 *pu8IsValid);
sint8 os_m2m_wifi_get_mac_address(uint8 *pu8MacAddr);
sint8 os_m2m_wifi_set_sleep_mode(uint8 PsTyp, uint8 BcastEn);
sint8 os_m2m_wifi_request_sleep(uint32 u32SlpReqTime);
uint8 os_m2m_wifi_get_sleep_mode(void);
sint8 os_m2m_wifi_req_client_ctrl(uint8 cmd);
sint8 os_m2m_wifi_req_server_init(uint8 ch);
sint8 os_m2m_wifi_set_device_name(uint8 *pu8DeviceName, uint8 u8DeviceNameLength);
sint8 os_m2m_wifi_set_lsn_int(tstrM2mLsnInt *pstrM2mLsnInt);

sint8 os_m2m_wifi_enable_monitoring_mode(tstrM2MWifiMonitorModeCtrl *pstrMtrCtrl,
   uint8 *pu8PayloadBuffer, uint16 u16BufferSize, uint16 u16DataOffset);

sint8 os_m2m_wifi_disable_monitoring_mode(void);
sint8 os_m2m_wifi_send_wlan_pkt(uint8 *pu8WlanPacket, uint16 u16WlanHeaderLength, uint16 u16WlanPktSize);
sint8 os_m2m_wifi_send_ethernet_pkt(uint8 *pu8Packet, uint16 u16PacketSize);
sint8 os_m2m_wifi_enable_sntp(uint8 bEnable);
sint8 os_m2m_wifi_set_sytem_time(uint32 u32UTCSeconds);
sint8 os_m2m_wifi_get_sytem_time(void);
sint8 os_m2m_wifi_set_cust_InfoElement(uint8 *pau8M2mCustInfoElement);
sint8 os_m2m_wifi_set_power_profile(uint8 u8PwrMode);
sint8 os_m2m_wifi_set_tx_power(uint8 u8TxPwrLevel);
sint8 os_m2m_wifi_enable_firmware_logs(uint8 u8Enable);
sint8 os_m2m_wifi_set_battery_voltage(uint16 u16BattVoltx100);
sint8 os_m2m_wifi_get_firmware_version(tstrM2mRev *pstrRev);

#ifdef ETH_MODE
sint8 os_m2m_wifi_enable_mac_mcast(uint8 *pu8MulticastMacAddress, uint8 u8AddRemove);
sint8 os_m2m_wifi_set_receive_buffer(void *pvBuffer, uint16 u16BufferLen);
#endif

sint8 os_m2m_wifi_prng_get_random_bytes(uint8 *pu8PrngBuff,uint16 u16PrngSize);

#endif
