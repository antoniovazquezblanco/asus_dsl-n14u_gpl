/************************************************************************
 *
 *	Copyright (C) 2009 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/
#ifndef _ETHERCMD_H
#define _ETHERCMD_H

#include "libmii.h"
#include "../../../version/tcversion.h"


//#define ETHCMD_VERSION          "1.0.0"


#define VPORT_FILE     "/var/vport_result" 

#define ETHCMD_ENABLE	"enable"
#define ETHCMD_DISABLE	"disable"
#define ETHCMD_PORT	"port"
#define ETHCMD_QUERY	"query"

#define ETHCMD_MEDIA_TYPE	"media_type"
#define ETHCMD_RESET		"reset"
#define ETHCMD_VLANPT		"vlanpt"
#define ETHCMD_LANCHIP		"lanchip"
#define ETHCMD_STATUS		"status"

#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
#define HELP_MEDIA_TYPE	"%s [option] [port 0-%d][swap]\tget/set media type\n" \
							"  [option]: auto - auto select\n" \
							"            100FD - 100Mb, Full Duplex\n" \
							"            100HD - 100Mb, Half Duplex\n" \
							"            10FD  - 10Mb,  Full Duplex\n" \
							"            10HD  - 10Mb,  Half Duplex\n" \
							" {port 0-%d}: required if <interface> is Ethernet Switch"
#define HELP_MEDIA_TYPE_INFO	"[option] [port num] [swap]\tget/set media type\n" \
							"  [option]: auto - auto select\n" \
							"            100FD - 100Mb, Full Duplex\n" \
							"            100HD - 100Mb, Half Duplex\n" \
							"            10FD  - 10Mb,  Full Duplex\n" \
							"            10HD  - 10Mb,  Half Duplex\n" \
							" {port num}: required if <interface> is Ethernet Switch"							
#else
#define HELP_MEDIA_TYPE	"[option] [port 0-3]\tget/set media type\n" \
							"  [option]: auto - auto select\n" \
							"            100FD - 100Mb, Full Duplex\n" \
							"            100HD - 100Mb, Half Duplex\n" \
							"            10FD  - 10Mb,  Full Duplex\n" \
							"            10HD  - 10Mb,  Half Duplex\n" \
							" {port 0-3}: required if <interface> is Ethernet Switch"
#endif
#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
#define HELP_RESET	"%s {port 0-%d} [swap]\tsoft reset transceiver\n" \
					"  {port 0-%d}: required if <interface> is Ethernet Switch"
#define HELP_RESET_INFO	"{port num} [swap]\tsoft reset transceiver\n" \
					"  {port num} : required if <interface> is Ethernet Switch"
#else					
#define HELP_RESET	"%s {port 0-%d}\tsoft reset transceiver\n" \
					"  {port num}: required if <interface> is Ethernet Switch\n"
#endif					
#define HELP_VLANPT	"enable|disable|query VLAN port of LanSwitch "

#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
#define HELP_LANCHIP 	"%s {enable|disable}\tenable/disable all port of lan chip\n" \
						"  [port 0-%d] [swap]\t\t\tenable/disable a port of lan chip\n"
#define HELP_LANCHIP_INFO 	"{enable|disable}\tenable/disable all port of lan chip\n" \
						"  [port num] [swap]\t\t\tenable/disable a port of lan chip\n"	
#else
#define HELP_LANCHIP 	"{enable|disable}\tenable/disable all port of lan chip\n" \
						"  [port 0-3]\t\t\tenable/disable a port of lan chip\n"					
#endif						
#define HELP_STATUS 	"get the Link status of ports\n"
#define HELP_IGMPSNOOP 	"enable|disable|query igmpsnoop of LanSwitch "

#define HELP_VLAN_ACTIVE	"active <0|1>"
#define HELP_VLAN_PVID		"pvid <port> <pvid>"
#define HELP_VLAN_VID		"vid <index> <active:0|1> <vid> <portMap> <tagPortMap>"

#ifdef ETHCMD_EXTENSION
#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
#define HELP_PORTBINDING	"portbinding <index> <add:1|delete:0> <interface:e0~e%d, p0~p7,w0~w3, u0>"
#define HELP_PORTBINDING_INFO	"portbinding <index> <add:1|delete:0> <interface:ex, px,wx, ux>"
#else
#define HELP_PORTBINDING	"portbinding <index> <add:1|delete:0> <interface:e0~e3, p0~p7,w0~w3, u0>"
#endif
#endif

#define ERR_MEDIAY_TYPE 0xFFFF
#define ENABLE_LAN_MASK 0xF7FF

#define MAX_VID_VALUE	4095
#define MAX_VLAN_RULE	16
#define NONE_IF	-1
#define ETHCMD_ERR	-1
#define MAX_PVCS	8

#define LAN_IF	"eth0"
#define ETHUP_NAMESIZE 16
#define ETHUP_WAN "nas0"
#define ETHUP_MAXPRORITY 7
#define ETHUP_MINPRORITY 0
#define ETHUP_CMDSIZE 128
#define ETHUP_ADDWANCMDNUM 5
#ifdef TCSUPPORT_TC2031
#define ETHUP_WAIT_PHY_READY	5
#else
#define ETHUP_WAIT_PHY_READY	3
#endif
#define PHY_COMPANYID 0x03a2
#define MAX_PHY_NUM 32

#if 0
#define EBT_FWD_FLUSH 		"ebtables -F FORWARD"
#define EBT_FWD_ACCEPT		"ebtables -P FORWARD ACCEPT"
#define EBT_MODULES_INSTALL	"insmod /lib/modules/%s/kernel/net/bridge/netfilter/%s"
#define EBT_MODULES_REMOVE	"rmmod ebtable_filter.ko;rmmod ebtables.ko"
#define EBT_FWD_DROP_RULE 	"ebtables -A FORWARD -i eth0.%d -o eth0.%d -j DROP"
#endif

#define VLAN_GROUP_ARRAY_LEN 4096
#define START_VLAN_VID	0x1

struct linkType{
	char* name;
	u16	val; 
};

struct portMap{
	char* ifname;
	char* real_name;
};

enum vport_t{
	enable=0,
	disable,
	query
};

#endif
