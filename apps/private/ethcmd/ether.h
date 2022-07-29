/************************************************************************
 *
 *	Copyright (C) 2006 Trendchip Technologies, Corp.
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
#ifndef _ETHER_H
#define _ETHER_H
#include "libmii.h"

#define QUERY	(1<<1)
#define ACTIVED	(1<<0)
#define DEACTIVED	(0)
#define TC2206_IGMPSNOOP		(1<<0)
#define TC2206_IGMPFASTLEAVE	(1<<8)	

etherChannVlanCap_t* getEtherVlanCap(void);

int macTC2206VlanActive(u8 type);
int macTC2206VlanSetPvid(u8 port, u16 pvid);
int macTC2206VlanSetVid(u8 index, u8 active, u16 vid, u8 portMap, u8 tagPortMap);
int macTC2206VlanCap(void);
int macTC2206VlanGetPvid(u8 port);
int macTC2206VlanGetVtbl(u8 index);
int macTC2206VlanDisp(void);
int macTC2206Igmpsnoop(u8 type);
int macTC2206IgmpFastLeave(u8 type);

int macRT63365VlanActive(u8 type);
int macRT63365VlanSetPvid(u8 port, u16 pvid);
int macRT63365VlanSetVid(u8 index, u8 active, u16 vid, u8 portMap, u8 tagPortMap);
int macRT63365VlanCap(void);
int macRT63365VlanGetPvid(u8 port);
int macRT63365VlanGetVtbl(u8 index);
int macRT63365VlanDisp(void);
int macRT63365Igmpsnoop(u8 type);
int macRT63365IgmpFastLeave(u8 type);

int macIP175CVlanActive(u8 type);
int macIP175CVlanSetPvid(u8 port, u16 pvid);
int macIP175CVlanSetVid(u8 index, u8 active, u16 vid, u8 portMap, u8 tagPortMap);
int macIP175CVlanCap(void);
#endif
