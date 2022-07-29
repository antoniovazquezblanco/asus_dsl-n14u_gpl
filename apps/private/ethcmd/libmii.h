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
#ifndef _LIBMII_H
#define _LIBMII_H

#include <net/if.h>
#include <linux/types.h>

#define IP175C		0x1
#define ADM6996M	0x2
#define TC2206		0x3
#define SWIC_RT63365	0x4
#define UNKNOW_SWIC	0xff
#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
#define SWIC_PORTS_NUM	2
#else
#if defined(TCSUPPORT_CPU_RT63365)
#define SWIC_PORTS_NUM	6
#else
#define SWIC_PORTS_NUM	4
#endif
#endif
#define SINGLE_PORT		1

typedef u_int32_t u32;
typedef u_int16_t u16;
typedef u_int8_t u8;

#define ETHER_VLAN_SAME_VID_IDX	(1<<0)
#define ETHER_8021Q_VLAN	(1<<0)

/* This data structure is used for all the MII ioctl's */
struct mii_data {
      u16   phy_id;
      u16   reg_num;
      u16   val_in;
      u16   val_out;
};

/* ioctl commands */
#define RAETH_GSW_REG_READ		0x89F1
#define RAETH_GSW_REG_WRITE		0x89F2

typedef struct _gsw_reg {
	u32 off;
	u32 val;
} gsw_reg;

typedef struct swic_api{
	int (*hw_igmpsnoop)(u8 type);
	int (*hw_fastleave)(u8 type);
	int (*spTag)(u8 type);
	int (*vlan_active)(u8 type);
	int (*vlan_pvid)(u8 port_id, u16 pvid);
	int (*vlan_get_pvid)(u8 port_id);
	int (*vlan_pvid_disp)(u8 port_id);
	int (*vlan_vid)(u8 index, u8 type, u16 vid, u8 portMap, u8 tagPortMap);
	int (*vlan_get_vtbl)(u8 index);
	int (*vlan_disp)(void);
	int (*vlan_getCap)(void);
}swic_api_t;

struct vender{
	u8 phy_id1;
	u8 reg1;
	u16 id1;
	u8 phy_id2;
	u8 reg2;
	u16 id2;
	char* name;
	u16 swic_id;
	void (*api_init)(swic_api_t* a);
};

#define 	PORT_REVERSE    (1<<0) /* port reverse */
#define		STAG_TO_VTAG	(1<<1) /* special tag to vlan tag */

struct swconfig{
	int skfd;
	struct ifreq ifr;
	u8 swic_id;
	//u8 reverse_flag;
	u8 flag;
};

typedef struct{
	u16 max_port_no;
	u16 max_vid;
	u32 flags;
}etherChannVlanCap_t;

/*Define Function Prototype*/
int mdio_read(int skfd, struct ifreq ifr,int phy_id, int reg_num);
void mdio_write(int skfd, struct ifreq ifr,int phy_id, int reg_num, int value);
int gsw_reg_read(int skfd, struct ifreq ifr,int offset);
void gsw_reg_write(int skfd, struct ifreq ifr,int offset, int value);

void searchSwic(int skfd, struct ifreq ifr); 
int init_swconfig(void);
struct swconfig* getSwcfg(void);

int get_wan_port(void);
#endif
