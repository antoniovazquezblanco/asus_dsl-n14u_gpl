TRUNK_DIR=$(shell pwd)
PROJECT_DIR=$(TRUNK_DIR)/Project
export PROJECT_DIR
PROJECT_MENUCONFIG_DIR=$(PROJECT_DIR)/config/menuconfig
PROJECT_AUTOCONFIG_DIR=$(PROJECT_DIR)/config/autoconfig
PROJECT_LIB_DIR=$(PROJECT_DIR)/lib
ifneq ($(strip $(CUSTOM)),)
PROFILE_DIR=$(PROJECT_DIR)/profile/$(CUSTOM)/$(PROFILE)
PROFILE_PATH=$(PROJECT_DIR)/profile/$(CUSTOM)
else
PROFILE_DIR=$(PROJECT_DIR)/profile/$(PROFILE)
PROFILE_PATH=$(PROJECT_DIR)/profile
endif
export PROFILE_DIR
SRCFS_DIR=$(PROFILE_DIR)/fs.src
APP_DIR=$(TRUNK_DIR)/apps
APP_BINARY_DIR=$(APP_DIR)/binary
APP_PUBLIC_DIR=$(APP_DIR)/public
APP_PRIVATE_DIR=$(APP_DIR)/private
export APP_PRIVATE_DIR

ifneq ($(strip $(TCSUPPORT_2_6_36_KERNEL)),)
ifeq ($(strip $(US_PROFILE)),DSL_N55U_D1)
BOOTROM_DIR=$(TRUNK_DIR)/bootrom
else
BOOTROM_DIR=$(TRUNK_DIR)/bootrom-2.6.36
endif
KERNEL_DIR=$(TRUNK_DIR)/linux-2.6.36
MODULE_DIR=$(TRUNK_DIR)/modules-2.6.36
else
BOOTROM_DIR=$(TRUNK_DIR)/bootrom
KERNEL_DIR=$(TRUNK_DIR)/linux
MODULE_DIR=$(TRUNK_DIR)/modules
endif

export BOOTROM_DIR
export KERNEL_DIR
TOOLS_DIR=$(TRUNK_DIR)/tools
MODULES_PUBLIC_SRC_DIR=$(MODULE_DIR)/public
MODULES_PRIV_SRC_DIR=$(MODULE_DIR)/private
export MODULES_PRIV_SRC_DIR
#ifneq ($(strip $(CUSTOMER_BUILD_RELEASECODE)),)
#DMT_DIR=$(MODULE_DIR)/dmt
#else
DMT_DIR=$(MODULES_PRIV_SRC_DIR)/dmt
#endif
#ifneq ($(strip $(CUSTOMER_BUILD_RELEASECODE)),)
#MODULES_BACKUP=$(MODULES_PRIV_SRC_DIR)/ko/modules
#else
MODULES_BACKUP=$(MODULES_PRIV_SRC_DIR)/ko/modules
#endif
MODULES_UFSD_DIR=$(MODULES_PRIV_SRC_DIR)/ufsd
MODULES_IGMPSNOOP_DIR=$(MODULES_PRIV_SRC_DIR)/net/igmpsnooping
MODULES_ACCESSLIMIT_DIR=$(MODULES_PRIV_SRC_DIR)/net/access_limit
MODULES_VOIP_DIR=$(MODULES_PRIV_SRC_DIR)/voip
MODULES_PCMSLIC_DIR=$(MODULES_PRIV_SRC_DIR)/voip/pcm_slic
MODULES_PCMTEST_DIR=$(MODULES_PRIV_SRC_DIR)/voip/module_test/PCM_test
MODULES_SLICTEST_DIR=$(MODULES_PRIV_SRC_DIR)/voip/module_test/SLIC_test/slic
MODULES_FLASHTEST_DIR=$(MODULES_PRIV_SRC_DIR)/voip/module_test/SLIC_test/flash
MODULES_SLICDRIVER_DIR=$(MODULES_PRIV_SRC_DIR)/voip/slic_driver
MODULES_VOICEDSP_AC_DRIVER_DIR=$(MODULES_PRIV_SRC_DIR)/voip/voice_DSP/AC/kernel_soft_dsp
MODULES_VOICEDSP_D2_DRIVER_DIR=$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/
MODULES_VOICEDSP_EVA_DRIVER_DIR=$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/EVA
MODULES_TDI_DIR=$(MODULES_PRIV_SRC_DIR)/voip/telephony_drv
export MODULES_TDI_DIR
export MODULES_VOICEDSP_D2_DRIVER_DIR
MODULES_RTP_DIR=$(MODULES_PRIV_SRC_DIR)/voip/tcRtp
MODULES_TCCI_DIR=$(MODULES_PRIV_SRC_DIR)/tcci
MODULES_TCPHY_DIR=$(MODULES_PRIV_SRC_DIR)/tcphy
MODULES_TC3162L2HP2H_DIR=$(MODULES_PRIV_SRC_DIR)/tc3162l2hp2h
MODULES_TC3262_DIR=$(MODULES_PRIV_SRC_DIR)/tc3262
MODULES_MT7510_PTM_DIR=$(MODULES_PRIV_SRC_DIR)/mt7510_ptm
MODULES_BONDING_PCIE_DIR=$(MODULES_PRIV_SRC_DIR)/bonding_pcie
MODULES_RAETH_DIR=$(MODULES_PRIV_SRC_DIR)/raeth
MODULES_HWNAT_DIR=$(MODULES_PRIV_SRC_DIR)/hwnat
MODULES_RA_HWNAT_DIR=$(MODULES_PRIV_SRC_DIR)/ra_hwnat
MODULES_RA_HWNAT_7510_DIR=$(MODULES_PRIV_SRC_DIR)/ra_hwnat_7510
RT2561AP_DIR=$(MODULES_PRIV_SRC_DIR)/wifi/rt61ap/Module
RT3390AP_DIR=$(MODULES_PRIV_SRC_DIR)/wifi/2009_1106_RT3390_LinuxAP_V2.3.2.0_DPA
#RT5392AP_DIR=$(MODULES_PRIV_SRC_DIR)/wifi/RT5392_Linux_AP_V2.5.0.0_DPA
#RT5392AP_DIR=$(MODULES_PRIV_SRC_DIR)/wifi/ASUS_RT5392_Linux_AP_V2.7.1.3_DPA
ifeq ($(strip $(PROFILE)),dsl-n10-c1)
	RT5392AP_DIR=$(MODULES_PRIV_SRC_DIR)/wifi/ASUS_RT5392_Linux_AP_V2.7.1.3_V2.7.1.5_DPA
else
ifeq ($(strip $(PROFILE)),dsl-n10-c1_dodo)
	RT5392AP_DIR=$(MODULES_PRIV_SRC_DIR)/wifi/ASUS_RT5392_Linux_AP_V2.7.1.3_V2.7.1.5_DPA
else
ifeq ($(strip $(PROFILE)),dsl-n10-c1_pol)
	RT5392AP_DIR=$(MODULES_PRIV_SRC_DIR)/wifi/ASUS_RT5392_Linux_AP_V2.7.1.3_V2.7.1.5_DPA
else
	RT5392AP_DIR=$(MODULES_PRIV_SRC_DIR)/wifi/ASUS_RT5392_Linux_AP_V2.7.1.3_DPA
endif
endif
endif
RT5592AP_DIR=$(MODULES_PRIV_SRC_DIR)/wifi/ASUS_RT5592_Linux_AP_V2.7.1.3_DPA
MT7592AP_DIR=$(MODULES_PRIV_SRC_DIR)/wifi/MT7603E_DPA_LinuxAP_4.0.0.8_rev1_20150325
MT7612EAP_DIR=$(MODULES_PRIV_SRC_DIR)/wifi/MT7612E_LinuxAP_3.0.4.0.P1_DPA_20151203
MT7610EAP_DIR=$(MODULES_PRIV_SRC_DIR)/wifi/mt7610e_wifi_v3009_dpa_20150430
RT3090AP_DIR=$(MODULES_PRIV_SRC_DIR)/wifi/2009_0904_RT3090_LinuxAP_v2.3.0.0_TC_for_3092
RT3092AP_DIR=$(MODULES_PRIV_SRC_DIR)/wifi/ASUS_RT5392_Linux_AP_V2.7.1.3_DPA
RT6856_DIR=$(MODULES_PRIV_SRC_DIR)/wifi/module-v2.7.0.5
export RT6856_DIR
WSCD_DIR=$(MODULES_PRIV_SRC_DIR)/wifi/WSC_UPNP-0.2.2
WIFI_MULTI_DRIVER_DIR=$(MODULES_PRIV_SRC_DIR)/wifi/DPA_RT5592_RT5392_LinuxAP_V2.7.x.x
MODULES_FULLCONE_DIR=$(MODULES_PRIV_SRC_DIR)/net/full_cone
MODULES_SMUX_DIR=$(MODULES_PRIV_SRC_DIR)/net/smux
MODULES_PORTBIND_DIR=$(MODULES_PRIV_SRC_DIR)/net/portbind
MODULES_VLAN_TAG_DIR=$(MODULES_PRIV_SRC_DIR)/net/vlan_tag
MODULES_AUTOBENCH_DIR=$(MODULES_PRIV_SRC_DIR)/auto_bench
MODULES_QDMA_DIR=$(MODULES_PRIV_SRC_DIR)/bufmgr
export MODULES_QDMA_DIR
FILESYSTEM_DIR=$(TRUNK_DIR)/filesystem
export FILESYSTEM_DIR
APP_CWMP_DIR=$(APP_PRIVATE_DIR)/TR69_64
APP_UPNP_DIR=$(APP_PUBLIC_DIR)/igd
ifneq ($(strip $(TCSUPPORT_CT)),)
APP_CFG_MANAGER_DIR=$(APP_PRIVATE_DIR)/cfg_manager_ct
else
APP_CFG_MANAGER_DIR=$(APP_PRIVATE_DIR)/cfg_manager
endif
export APP_CFG_MANAGER_DIR
APP_SKB_MANAGER_DIR=$(APP_PRIVATE_DIR)/skbmgr
APP_RESTORE_LINOS_INFO_DIR=$(APP_PRIVATE_DIR)/restore_linos_info
APP_CFG_PARSER_DIR=$(APP_PRIVATE_DIR)/cfg_parser
APP_CENTRAL_COORDINATOR_DIR=$(APP_PRIVATE_DIR)/Central_Coordinator
APP_CODE_REDUCE_PATCH_DIR=$(APP_PRIVATE_DIR)/code_reduce_patch
APP_ETC_SCRIPT_DIR=$(APP_PRIVATE_DIR)/etc_script
export APP_ETC_SCRIPT_DIR
APP_8021X_PARA_DIR=$(APP_PUBLIC_DIR)/wpa_supplicant-0.7.3/8021x_para
APP_FWUPGRADE_DIR=$(APP_PRIVATE_DIR)/FWUpgrade
APP_HUAWEI_CI_DIR=$(APP_PRIVATE_DIR)/huawei_ci
APP_LED_CONF_DIR=$(APP_PRIVATE_DIR)/led_conf
APP_BUSYBOX_DIR=$(APP_PUBLIC_DIR)/busybox-1.00
#APP_BUSYBOX_NEW_DIR=$(APP_PUBLIC_DIR)/busybox-1.22.1
APP_BUSYBOX_NEW_DIR=$(APP_PUBLIC_DIR)/busybox-1.23.1
APP_BRCTL_DIR=$(APP_PUBLIC_DIR)/bridge-utils-1.0.6
APP_BR2684CTL_DIR=$(APP_PUBLIC_DIR)/br2684ctl
APP_LIBATM_DIR=$(APP_PUBLIC_DIR)/linux-atm
export APP_LIBATM_DIR
APP_TCAPILIB_DIR=$(APP_PRIVATE_DIR)/tcapi_lib
APP_TCAPI_CWMP_LIB_DIR=$(APP_PRIVATE_DIR)/tcapi_lib_cwmp
export APP_TCAPILIB_DIR
LIB_DIR=$(APP_PRIVATE_DIR)/lib
export LIB_DIR
LIB_DIR_CWMP=$(APP_PRIVATE_DIR)/lib_cwmp
export LIB_DIR_CWMP
PUBLIC_LIB_DIR=$(APP_PUBLIC_DIR)/lib
export PUBLIC_LIB_DIR
PUBLIC_LIB_DIR_CWMP=$(APP_PUBLIC_DIR)/lib_cwmp
export PUBLIC_LIB_DIR_CWMP
APP_CPU_DIR=$(APP_PUBLIC_DIR)/cpu
#ifneq ($(strip $(TCSUPPORT_2_6_36_KERNEL)),)
APP_IPTABLES_DIR=$(APP_PUBLIC_DIR)/iptables-1.4.10
APP_IPTABLES_INSTALL_DIR=$(APP_IPTABLES_DIR)/install
#else
#APP_IPTABLES_DIR=$(APP_PUBLIC_DIR)/iptables-1.3.8
#endif
APP_IPROUTE_TC_DIR=$(APP_PUBLIC_DIR)/iproute2-2.6.22-070710
APP_IPROUTE_IP_DIR=$(APP_PUBLIC_DIR)/iproute
APP_EBTABLES_DIR=$(APP_PUBLIC_DIR)/ebtables-v2.0.8-2
APP_MTD_DIR=$(APP_PUBLIC_DIR)/mtd
ifneq ($(strip $(TCSUPPORT_CT)),)
APP_PPPD_DIR=$(APP_PUBLIC_DIR)/ppp-2.4.1.pppoe4.orig
else
APP_PPPD_DIR=$(APP_PUBLIC_DIR)/ppp-2.4.5
endif
APP_TCCI_DIR=$(APP_PRIVATE_DIR)/tcci
APP_SENDICMP_DIR=$(APP_PRIVATE_DIR)/sendicmp
APP_TCWDOG_DIR=$(APP_PRIVATE_DIR)/tcwdog
APP_UTELNETD_DIR=$(APP_PUBLIC_DIR)/utelnetd-0.1.2
APP_MXML_DIR=$(APP_PUBLIC_DIR)/mxml-2.4
export APP_MXML_DIR
APP_MSSL_DIR=$(APP_PUBLIC_DIR)/mssl
export APP_MSSL_DIR
APP_BOA_DIR=$(APP_PUBLIC_DIR)/boa-asp/src
APP_WEBPAGE_DIR=$(APP_PRIVATE_DIR)/webPage
APP_ROUTE_WEBPAGE_DIR=$(APP_PRIVATE_DIR)/webPage/Router/tc/boaroot
APP_ASUS_WEBPAGE_DIR=$(APP_PRIVATE_DIR)/webPage/Router/asus/boaroot
APP_BRIDGE_WEBPAGE_DIR=$(APP_PRIVATE_DIR)/webPage/Pure_Bridge/Huawei/MT881s-T/boaroot
APP_TCAPI_DIR=$(APP_PRIVATE_DIR)/tcapi
APP_RSTATS_DIR=$(APP_PRIVATE_DIR)/rstats
APP_ACCESSLOG_DIR=$(APP_PRIVATE_DIR)/access_log
export APP_ACCESSLOG_DIR
APP_AUTO_DET_DIR=$(APP_PRIVATE_DIR)/auto_det
APP_INFOSVR_DIR=$(APP_PRIVATE_DIR)/infosvr
APP_MONITORCFGMGR_DIR=$(APP_PRIVATE_DIR)/monitorcfgmgr
ifneq ($(strip $(ASUS_SPECTRUM)),)
APP_SPECTRUM_DIR=$(APP_PRIVATE_DIR)/spectrum
endif
APP_HWINFO_DIR=$(APP_PRIVATE_DIR)/hwinfo
ifneq ($(strip $(ASUS_SYSSTATE)),)
APP_SYSSTATE_DIR=$(APP_PRIVATE_DIR)/sysstate
endif
APP_DHCPRELAY_DIR=$(APP_PUBLIC_DIR)/dhcp-isc
APP_DPROXY_DIR=$(APP_PUBLIC_DIR)/dproxy-nexgen
#APP_EZ-IPUPDATE_DIR=$(APP_PUBLIC_DIR)/ez-ipupdate-3.0.10
APP_EZ-IPUPDATE_DIR=$(APP_PUBLIC_DIR)/ez-ipupdate-3.0.11
APP_EMAIL_DIR=$(APP_PUBLIC_DIR)/email-3.1.3
APP_BFTPD_DIR=$(APP_PUBLIC_DIR)/bftpd
APP_INETD_DIR=$(APP_PUBLIC_DIR)/inetd
APP_SNMPD_DIR=$(APP_PRIVATE_DIR)/net-snmp-5.3.1_reduced
APP_NTPCLIENT_DIR=$(APP_PUBLIC_DIR)/ntpclient
APP_TFTPD_DIR=$(APP_PUBLIC_DIR)/tftp-1.0
APP_WRIELESSTOOL_DIR=$(APP_PUBLIC_DIR)/wireless_tools.28
ifneq ($(strip $(TCSUPPORT_IGMP_PROXY_V3)),)
APP_IGMPPROXY_DIR=$(APP_PUBLIC_DIR)/igmpv3proxy
else
APP_IGMPPROXY_DIR=$(APP_PUBLIC_DIR)/igmpproxy
endif
APP_MTF_DIR=$(APP_PRIVATE_DIR)/RV_MTF_4
APP_TC_VOIP_API_DIR=$(APP_PRIVATE_DIR)/TC_VOIP_API
APP_SSL_DIR=$(APP_PUBLIC_DIR)/matrixssl-1-8/src
#APP_SSL_DIR=$(APP_PUBLIC_DIR)/matrixssl-3-7-1-open/
APP_PPPOERELAY_DIR=$(APP_PUBLIC_DIR)/rp-pppoe-3.10/src
APP_ZEBRA_DIR=$(APP_PRIVATE_DIR)/zebra-0.93a_reduced
APP_FLEX_DIR=$(APP_PUBLIC_DIR)/flex-2.5.35
APP_RADVD_DIR=$(APP_PUBLIC_DIR)/radvd-1.5
APP_DHCPV6_DIR=$(APP_PUBLIC_DIR)/wide-dhcpv6-20080615
APP_MLDPROXY_DIR=$(APP_PUBLIC_DIR)/ecmh-2005.02.09
APP_IPROUTE_DIR=$(APP_PUBLIC_DIR)/iproute
export APP_ETHCMD_DIR
ifneq ($(strip $(TCSUPPORT_CT)),)
ifneq ($(strip $(TCSUPPORT_CT_WAN_PTM)),)
APP_ETHCMD_DIR=$(APP_PRIVATE_DIR)/ethcmd
else
APP_ETHCMD_DIR=$(APP_PRIVATE_DIR)/ethcmd_ct
endif
else
APP_ETHCMD_DIR=$(APP_PRIVATE_DIR)/ethcmd
endif
APP_RTKETHCMD_DIR=$(APP_PRIVATE_DIR)/rtkethcmd
#APP_8021X_DIR=$(APP_PUBLIC_DIR)/8021X
#APP_8021X_DIR=$(APP_PUBLIC_DIR)/RT_DOT1X_v2.5.0.0
APP_8021X_DIR=$(APP_PUBLIC_DIR)/RT_DOT1X_v2.7.0.0
APP_VCONFIG_DIR=$(APP_PUBLIC_DIR)/vlan
APP_CMDCI_DIR=$(APP_PRIVATE_DIR)/cmd_ci
APP_ASUSTEST_DIR=$(APP_PRIVATE_DIR)/asustest
APP_TCLINUXBUILDER_DIR=$(APP_PRIVATE_DIR)/tclinux_builder
APP_RCS_DIR=$(APP_PRIVATE_DIR)/rcS
#APP_DNSMASQ_DIR=$(APP_PUBLIC_DIR)/dnsmasq-2.52
#dnsmasq - version 2.75 support IPv6
APP_DNSMASQ_DIR=$(APP_PUBLIC_DIR)/dnsmasq
APP_ODHCP6C_DIR=$(APP_PUBLIC_DIR)/odhcp6c
APP_SMUXCTL_DIR=$(APP_PRIVATE_DIR)/smuxctl
APP_USB_PRINTER_DIR=$(APP_PUBLIC_DIR)/p910nd-0.93
MODULES_USB_CAMERA_DRIVER=$(MODULES_PUBLIC_SRC_DIR)/usb_camera
APP_MJPEG_LIB_DIR=$(APP_PUBLIC_DIR)/jpeg-6b
APP_USB_MJPEG_STREAM_SERVER_DIR=$(APP_PUBLIC_DIR)/mjpg-streamer-r63
APP_USB_3G_PPP_DIR=$(APP_PUBLIC_DIR)/ppp-2.4.5
APP_NTFS_DIR=$(APP_PUBLIC_DIR)/ntfs-3g-2010.5.22
APP_AUTOMOUNT_DIR=$(APP_PRIVATE_DIR)/auto_mount
APP_PPPOE_SVR_DIR=$(APP_PUBLIC_DIR)/rp-pppoe-3.10
APP_QUAGGA_DIR=$(APP_PUBLIC_DIR)/quagga-0.98.6
APP_PCM_TEST_DIR=$(APP_PRIVATE_DIR)/PCM_test
APP_SLIC_TEST_DIR=$(APP_PRIVATE_DIR)/SLIC_test
APP_LIBDAEMON_DIR=$(APP_PUBLIC_DIR)/libdaemon-0.14
APP_AUTOIP_DIR=$(APP_PUBLIC_DIR)/avahi-0.6.28
APP_DMS_DIR=$(APP_PRIVATE_DIR)/dlna_src
BACKUP_DIR=$(TRUNK_DIR)/backup
APP_SSH_DIR=$(APP_PUBLIC_DIR)/dropbear-2016.74
APP_RA_MENU_DIR=$(APP_PRIVATE_DIR)/ra_menu
APP_LIBOSIP_DIR=$(APP_PUBLIC_DIR)/libosip2-2.0.4
APP_SIPROXD_DIR=$(APP_PUBLIC_DIR)/siproxd-0.5.12
APP_SAMBA_DIR=$(APP_PUBLIC_DIR)/samba-3.0.33
LZMA_LIB=$(TOOLS_DIR)/squashfs-tools/lzma/C/7zip/Compress/LZMA_Lib
LZMA_ALONE=$(TOOLS_DIR)/squashfs-tools/lzma/C/7zip/Compress/LZMA_Alone
TOOLS_MLCHECKHASH_DIR=$(TOOLS_DIR)/mlCheckHash
TOOLS_TRX_DIR=$(TOOLS_DIR)/trx
TOOLS_REDUCER_DIR=$(TOOLS_DIR)/reducer
TOOLS_ROMFILE_MERGE_DIR=$(TOOLS_DIR)/romfile_merge
TOOLS_RESTORE_ROM_DIR=$(TOOLS_DIR)/restore_rom_t_info
TOOLS_CPLOPTS_DIR=$(TOOLS_DIR)/cplopts_checktool
MODULES_FASTBRIDGE_DIR=$(MODULES_PRIV_SRC_DIR)/net/fastbridge
APP_PWCTLCMD_DIR=$(APP_PRIVATE_DIR)/pwctlcmd
#APP_WPA_SUPP_DIR=$(APP_PUBLIC_DIR)/wpa_supplicant-0.7.3/wpa_supplicant
#APP_WPA_SUPP_DIR=$(APP_PUBLIC_DIR)/wpa_supplicant_ra_v2.5.0.0/wpa_supplicant
APP_WPA_SUPP_DIR=$(APP_PUBLIC_DIR)/wpa_supplicant_ra_v2.7.0.0/wpa_supplicant
#APP_OPENSSL_DIR=$(APP_PUBLIC_DIR)/openssl-1.0.0q
APP_OPENSSL_DIR=$(APP_PUBLIC_DIR)/openssl-1.0.2d
export APP_OPENSSL_DIR
#ifneq ($(strip $(TCSUPPORT_CT_USB_BACKUPRESTORE)),)
APP_BACKUPRESTORE_DIR=$(APP_PRIVATE_DIR)/backuprestore
#endif
APP_TRACEROUTE_DIR=$(APP_PUBLIC_DIR)/traceroute-2.0.17
APP_RA_HWNAT_DIR=$(APP_PRIVATE_DIR)/hw_nat
APP_PROTECTION_SERVER_DIR=$(APP_PRIVATE_DIR)/protect_srv
export APP_PROTECTION_SERVER_DIR
APP_NOTIFICATION_CENTER_DIR=$(APP_PRIVATE_DIR)/nt_center
export APP_NOTIFICATION_CENTER_DIR
APP_RA_HWNAT_7510_DIR=$(APP_PRIVATE_DIR)/hw_nat_7510
APP_SYSSTAT_DIR=$(APP_PUBLIC_DIR)/tools/sysstat-9.0.4
APP_STRACE_DIR=$(APP_PUBLIC_DIR)/tools/strace-4.5.18
APP_ETHTOOL_DIR=$(APP_PUBLIC_DIR)/tools/ethtool-3.18
APP_STRESS_DIR=$(APP_PUBLIC_DIR)/tools/stress-1.0.4
MODULES_CRYPTO_DRIVER=$(MODULES_PRIV_SRC_DIR)/cryptoDriver
APP_IPSEC_TOOLS=$(APP_PUBLIC_DIR)/ipsec-tools-0.8.0
APP_OPENSSL_VPN_DIR=$(APP_PUBLIC_DIR)/openssl-1.0.0e
APP_SHARED_DIR=$(APP_PUBLIC_DIR)/shared
export APP_SHARED_DIR
LIB_UTILS_DIR=$(APP_PRIVATE_DIR)/utils
export LIB_UTILS_DIR
APP_LIBDISK_DIR=$(APP_PUBLIC_DIR)/libdisk
APP_SDPARM_DIR=$(APP_PUBLIC_DIR)/sdparm-1.02
ifneq ($(strip $(TCSUPPORT_2_6_36_KERNEL)),)
APP_MODESWITCH_DIR=$(APP_PUBLIC_DIR)/usb-modeswitch-2.2.0
else
APP_MODESWITCH_DIR=$(APP_PUBLIC_DIR)/usbmodeswitch
endif
APP_COMGT_DIR=$(APP_PUBLIC_DIR)/comgt-0.32
APP_UQMI_DIR=$(APP_PUBLIC_DIR)/uqmi
APP_VSFTPD_DIR=$(APP_PUBLIC_DIR)/vsftpd
APP_HOTPLUG2_DIR=$(APP_PUBLIC_DIR)/hotplug2
APP_UDEV_DIR=$(APP_PUBLIC_DIR)/udev
APP_RC_DIR=$(APP_PUBLIC_DIR)/rc
APP_LIBUSB10_DIR=$(APP_PUBLIC_DIR)/libusb10
APP_LIBUSB_DIR=$(APP_PUBLIC_DIR)/libusb
APP_LIBUSB0112_DIR=$(APP_PUBLIC_DIR)/libusb-0.1.12
APP_U2EC_DIR=$(APP_PUBLIC_DIR)/u2ec
APP_LPRNG_DIR=$(APP_PUBLIC_DIR)/LPRng
APP_SQLITE_DIR=$(APP_PUBLIC_DIR)/sqlite
export APP_SQLITE_DIR
APP_LIBXML2_DIR=$(APP_PUBLIC_DIR)/libxml2
export APP_LIBXML2_DIR
APP_PCRE_DIR=$(APP_PUBLIC_DIR)/pcre-8.12
APP_SAMBA_FOR_WEBDAV_DIR=$(APP_PUBLIC_DIR)/samba-3.5.8
export APP_SAMBA_FOR_WEBDAV_DIR
APP_LIGHTTPD_DIR=$(APP_PUBLIC_DIR)/lighttpd-1.4.39
APP_JSONC_DIR=$(APP_PUBLIC_DIR)/json-c
export APP_JSONC_DIR
APP_UFSD_DIR=$(APP_PRIVATE_DIR)/ufsd
APP_CURL_DIR=$(APP_PUBLIC_DIR)/curl-7.21.7
export APP_CURL_DIR
APP_ASUSWEBSTORAGE_DIR=$(APP_PUBLIC_DIR)/asuswebstorage
APP_INOTIFY_DIR=$(APP_PUBLIC_DIR)/inotify
APP_NEON_DIR=$(APP_PUBLIC_DIR)/neon
APP_USBCLIENT_DIR=$(APP_PUBLIC_DIR)/usbclient
export APP_NEON_DIR
APP_ZLIB_DIR=$(APP_PUBLIC_DIR)/zlib
export APP_ZLIB_DIR
APP_WEBDAVCLIENT_DIR=$(APP_PUBLIC_DIR)/webdav_client
APP_DROPBOXCLIENT_DIR=$(APP_PUBLIC_DIR)/dropbox_client
APP_SAMBACLIENT_DIR=$(APP_PUBLIC_DIR)/sambaclient
APP_FTPCLIENT_DIR=$(APP_PUBLIC_DIR)/ftpclient
APP_LIBICONV_DIR=$(APP_PUBLIC_DIR)/libiconv-1.14
export APP_LIBICONV_DIR
APP_ACCEL_PPTPD=$(APP_PUBLIC_DIR)/accel-pptpd
APP_UDPXY=$(APP_PUBLIC_DIR)/udpxy
APP_ACCEL_PPTP=$(APP_PUBLIC_DIR)/accel-pptp
APP_RP_L2TP=$(APP_PUBLIC_DIR)/rp-l2tp
APP_ETHER_WAKE_DIR=$(APP_PUBLIC_DIR)/ether-wake
APP_NETWORKMAP_DIR=$(APP_PUBLIC_DIR)/networkmap
export APP_NETWORKMAP_DIR
APP_LIBNFNETLINK_DIR=$(APP_PUBLIC_DIR)/libnfnetlink-1.0.1
export APP_LIBNFNETLINK_DIR
APP_MINIUPNPD_DIR=$(APP_PUBLIC_DIR)/miniupnpd-1.9
APP_TR069_DIR=$(APP_PRIVATE_DIR)/tr069
APP_NVRAM_DIR=$(APP_PUBLIC_DIR)/nvram
APP_LZO_DIR=$(APP_PUBLIC_DIR)/lzo-2.09
APP_OPENPAM_DIR=$(APP_PUBLIC_DIR)/openpam-20140912
APP_OPENVPN_DIR=$(APP_PUBLIC_DIR)/openvpn-2.3.2
APP_IPSEC_DIR=$(APP_PUBLIC_DIR)/strongswan-5.2.1
APP_NETSTAT_NAT_DIR=$(APP_PUBLIC_DIR)/netstat-nat
export APP_NETSTAT_NAT_DIR
APP_CHKWAN_DIR=$(APP_PRIVATE_DIR)/chkwan
APP_ACME_DIR=$(APP_PUBLIC_DIR)/acme
APP_GINPTRY_DIR=$(APP_PRIVATE_DIR)/ginptry
APP_PIMD_DIR=$(APP_PUBLIC_DIR)/pimd-2.3.2
APP_BWDPI_DIR=$(APP_PRIVATE_DIR)/bwdpi
export APP_BWDPI_DIR
