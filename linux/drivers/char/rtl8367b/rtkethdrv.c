#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/ioport.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/fs.h> //Ren
#include <linux/proc_fs.h>
#include <linux/seq_file.h> //Ren: for seq_file
#include <asm/uaccess.h>
#include <asm/io.h>

#include <rtk_api.h>
#include <rtk_api_ext.h>
#include <rtk_types.h>
#include <rtl8367b_asicdrv_port.h>

#define RTL8367RB_DEVNAME			"rtl8367rb"

MODULE_AUTHOR("ASUS");
MODULE_DESCRIPTION("Realtek Switch IC driver");
MODULE_LICENSE("GPL");

#define CHK_BUF() pos = begin + index; if (pos < off) { index = 0; begin = pos; }; if (pos > off + count) goto done;

extern unsigned long ReadRtk(unsigned long phyReg);
int rtk_switch_status = 0;

char* APIMIBString[61];

void set_mib_str_table()
{
	APIMIBString[0] = "ifInOctets";
	APIMIBString[1] = "dot3StatsFCSErrors";
	APIMIBString[2] = "dot3StatsSymbolErrors";
	APIMIBString[3] = "dot3InPauseFrames";
	APIMIBString[4] = "dot3ControlInUnknownOpcodes";
	APIMIBString[5] = "etherStatsFragments";
	APIMIBString[6] = "etherStatsJabbers";
	APIMIBString[7] = "ifInUcastPkts";
	APIMIBString[8] = "etherStatsDropEvents";
	APIMIBString[9] = "etherStatsOctets";
	APIMIBString[10] = "etherStatsUndersizePkts";
	APIMIBString[11] = "etherStatsOversizePkts";
	APIMIBString[12] = "etherStatsPkts64Octets";
	APIMIBString[13] = "etherStatsPkts65to127Octets";
	APIMIBString[14] = "etherStatsPkts128to255Octets";
	APIMIBString[15] = "etherStatsPkts256to511Octets";
	APIMIBString[16] = "etherStatsPkts512to1023Octets";
	APIMIBString[17] = "etherStatsPkts1024toMaxOctets";
	APIMIBString[18] = "etherStatsMcastPkts";
	APIMIBString[19] = "etherStatsBcastPkts";
	APIMIBString[20] = "ifOutOctets";
	APIMIBString[21] = "dot3StatsSingleCollisionFrames";
	APIMIBString[22] = "dot3StatsMultipleCollisionFrames";
	APIMIBString[23] = "dot3StatsDeferredTransmissions";
	APIMIBString[24] = "dot3StatsLateCollisions";
	APIMIBString[25] = "etherStatsCollisions";
	APIMIBString[26] = "dot3StatsExcessiveCollisions";
	APIMIBString[27] = "dot3OutPauseFrames";
	APIMIBString[28] = "dot1dBasePortDelayExceededDiscards";
	APIMIBString[29] = "dot1dTpPortInDiscards";
	APIMIBString[30] = "ifOutUcastPkts";
	APIMIBString[31] = "ifOutMulticastPkts";
	APIMIBString[32] = "ifOutBrocastPkts";
	APIMIBString[33] = "outOampduPkts";
	APIMIBString[34] = "inOampduPkts";
	APIMIBString[35] = "pktgenPkts";
	APIMIBString[36] = "inMldChecksumError";
	APIMIBString[37] = "inIgmpChecksumError";
	APIMIBString[38] = "inMldSpecificQuery";
	APIMIBString[39] = "inMldGeneralQuery";
	APIMIBString[40] = "inIgmpSpecificQuery";
	APIMIBString[41] = "inIgmpGeneralQuery";
	APIMIBString[42] = "inMldLeaves";
	APIMIBString[43] = "inIgmpLeaves";
	APIMIBString[44] = "inIgmpJoinsSuccess";
	APIMIBString[45] = "inIgmpJoinsFail";
	APIMIBString[46] = "inMldJoinsSuccess";
	APIMIBString[47] = "inMldJoinsFail";
	APIMIBString[48] = "inReportSuppressionDrop";
	APIMIBString[49] = "inLeaveSuppressionDrop";
	APIMIBString[50] = "outIgmpReports";
	APIMIBString[51] = "outIgmpLeaves";
	APIMIBString[52] = "outIgmpGeneralQuery";
	APIMIBString[53] = "outIgmpSpecificQuery";
	APIMIBString[54] = "outMldReports";
	APIMIBString[55] = "outMldLeaves";
	APIMIBString[56] = "outMldGeneralQuery";
	APIMIBString[57] = "outMldSpecificQuery";
	APIMIBString[58] = "inKnownMulticastPkts";
	APIMIBString[59] = "ifInMulticastPkts";
	APIMIBString[60] = "ifInBroadcastPkts";
}

void show_mib_counter(rtk_port_t port, rtk_stat_port_cntr_t* portCntrsPtr)
{	
	prom_printf("\r\nport %d: %-35s[%02d]:  %llu",port,APIMIBString[0],0,portCntrsPtr->ifInOctets);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[1],1,portCntrsPtr->dot3StatsFCSErrors);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[2],2,portCntrsPtr->dot3StatsSymbolErrors);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[3],3,portCntrsPtr->dot3InPauseFrames);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[4],4,portCntrsPtr->dot3ControlInUnknownOpcodes);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[5],5,portCntrsPtr->etherStatsFragments);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[6],6,portCntrsPtr->etherStatsJabbers);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[7],7,portCntrsPtr->ifInUcastPkts);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[8],8,portCntrsPtr->etherStatsDropEvents);
	prom_printf("\r\nport %d: %-35s[%02d]:  %llu",port,APIMIBString[9],9,portCntrsPtr->etherStatsOctets);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[10],10,portCntrsPtr->etherStatsUndersizePkts);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[11],11,portCntrsPtr->etherStatsOversizePkts);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[12],12,portCntrsPtr->etherStatsPkts64Octets);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[13],13,portCntrsPtr->etherStatsPkts65to127Octets);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[14],14,portCntrsPtr->etherStatsPkts128to255Octets);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[15],15,portCntrsPtr->etherStatsPkts256to511Octets);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[16],16,portCntrsPtr->etherStatsPkts512to1023Octets);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[17],17,portCntrsPtr->etherStatsPkts1024toMaxOctets);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[18],18,portCntrsPtr->etherStatsMcastPkts);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[19],19,portCntrsPtr->etherStatsBcastPkts);
	prom_printf("\r\nport %d: %-35s[%02d]:  %llu",port,APIMIBString[20],20,portCntrsPtr->ifOutOctets);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[21],21,portCntrsPtr->dot3StatsSingleCollisionFrames);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[22],22,portCntrsPtr->dot3StatsMultipleCollisionFrames);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[23],23,portCntrsPtr->dot3StatsDeferredTransmissions);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[24],24,portCntrsPtr->dot3StatsLateCollisions);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[25],25,portCntrsPtr->etherStatsCollisions);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[26],26,portCntrsPtr->dot3StatsExcessiveCollisions);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[27],27,portCntrsPtr->dot3OutPauseFrames);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[28],28,portCntrsPtr->dot1dBasePortDelayExceededDiscards);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[29],29,portCntrsPtr->dot1dTpPortInDiscards);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[30],30,portCntrsPtr->ifOutUcastPkts);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[31],31,portCntrsPtr->ifOutMulticastPkts);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[32],32,portCntrsPtr->ifOutBrocastPkts);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[33],33,portCntrsPtr->outOampduPkts);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[34],34,portCntrsPtr->inOampduPkts);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[35],35,portCntrsPtr->pktgenPkts);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[36],36,portCntrsPtr->inMldChecksumError);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[37],37,portCntrsPtr->inIgmpChecksumError);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[38],38,portCntrsPtr->inMldSpecificQuery);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[39],39,portCntrsPtr->inMldGeneralQuery);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[40],40,portCntrsPtr->inIgmpSpecificQuery);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[41],41,portCntrsPtr->inIgmpGeneralQuery);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[42],42,portCntrsPtr->inMldLeaves);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[43],43,portCntrsPtr->inIgmpLeaves);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[44],44,portCntrsPtr->inIgmpJoinsSuccess);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[45],45,portCntrsPtr->inIgmpJoinsFail);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[46],46,portCntrsPtr->inMldJoinsSuccess);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[47],47,portCntrsPtr->inMldJoinsFail);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[48],48,portCntrsPtr->inReportSuppressionDrop);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[49],49,portCntrsPtr->inLeaveSuppressionDrop);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[50],50,portCntrsPtr->outIgmpReports);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[51],51,portCntrsPtr->outIgmpLeaves);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[52],52,portCntrsPtr->outIgmpGeneralQuery);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[53],53,portCntrsPtr->outIgmpSpecificQuery);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[54],54,portCntrsPtr->outMldReports);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[55],55,portCntrsPtr->outMldLeaves);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[56],56,portCntrsPtr->outMldGeneralQuery);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[57],57,portCntrsPtr->outMldSpecificQuery);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[58],58,portCntrsPtr->inKnownMulticastPkts);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[59],59,portCntrsPtr->ifInMulticastPkts);
	prom_printf("\r\nport %d: %-35s[%02d]:  %u",port,APIMIBString[60],60,portCntrsPtr->ifInBroadcastPkts);
	prom_printf("\n");
}



//
// please refer RT-N65U source code
//
#define	CONTROL_REG_PORT_POWER_BIT	0x800
#define LAN_MAC_PORT			RTK_EXT_1_MAC
#define WAN_MAC_PORT			RTK_EXT_1_MAC
#define WAN_MAC_PORT_MASK		(1U << WAN_MAC_PORT)
#define INIC_MAC_PORT			RTK_EXT_1_MAC
#define INIC_MAC_PORT_MASK		(1U << INIC_MAC_PORT)
#define STB_MAC_PORT			RTK_EXT_1_MAC
#define STB_MAC_PORT_MASK		(1U << STB_MAC_PORT)
#define WAN_PORT			0	/* P0 */
#ifdef CONFIG_MIPS_RT63365	//for RT63368, DSL-N55U-C1, DSL-N16U
#define LAN_PORT_1			4	/* P4 */
#define LAN_PORT_2			3	/* P3 */
#define LAN_PORT_3			2	/* P2 */
#define LAN_PORT_4			1	/* P1 */
#else
#define LAN_PORT_1			1	/* P1 */
#define LAN_PORT_2			2	/* P2 */
#define LAN_PORT_3			3	/* P3 */
#define LAN_PORT_4			4	/* P4 */
#endif
#define INIC_PORT			7
#define INIC_PORT_MASK			(1U << INIC_PORT)
#define INIC_ALL_PORTS_MASK		(INIC_PORT_MASK | INIC_MAC_PORT_MASK)
#define WAN_PORT_MASK			(1U << WAN_PORT)
#define WAN_ALL_PORTS_MASK		(WAN_MAC_PORT_MASK | WAN_PORT_MASK)
#define LAN_PORT_1_MASK			(1U << LAN_PORT_1)
#define LAN_PORT_2_MASK			(1U << LAN_PORT_2)
#define LAN_PORT_3_MASK			(1U << LAN_PORT_3)
#define LAN_PORT_4_MASK			(1U << LAN_PORT_4)
#define LAN_MAC_PORT_MASK		(1U << LAN_MAC_PORT)
#define LAN_ALL_PORTS_MASK		(LAN_MAC_PORT_MASK | LAN_PORT_1_MASK | LAN_PORT_2_MASK | LAN_PORT_3_MASK | LAN_PORT_4_MASK)
#define LAN_PORTS_MASK			(LAN_PORT_1_MASK | LAN_PORT_2_MASK | LAN_PORT_3_MASK | LAN_PORT_4_MASK)
#define ALL_PORTS_MASK			(WAN_ALL_PORTS_MASK | LAN_ALL_PORTS_MASK)

static const unsigned int s_wan_stb_array[8] = {
	/* 0:WLLLL	LAN: P1,P2,P3,P4	WAN: P0, STB: N/A (default mode), but we will power down WAN port in ATM/PTM mode*/
	0,
	/* 1:WSLLL	LAN: P2,P3,P4		WAN: P0, STB: P1 */
	LAN_PORT_1_MASK,
	/* 2:WLSLL	LAN: P1,P3,P4		WAN: P0, STB: P2 */
	LAN_PORT_2_MASK,
	/* 3:WLLSL	LAN: P1,P2,P4		WAN: P0, STB: P3 */
	LAN_PORT_3_MASK,
	/* 4:WLLLS	LAN: P1,P2,P3		WAN: P0, STB: P4 */
	LAN_PORT_4_MASK,
	/* 5:WSSLL	LAN: P3,P4		WAN: P4, STB: P1,P2 */
	LAN_PORT_1_MASK | LAN_PORT_2_MASK,
	/* 6:WLLSS	LAN: P1,P2		WAN: P4, STB: P3,P4 */
	LAN_PORT_3_MASK | LAN_PORT_4_MASK,
	/* 7:SLLLL	LAN: P1,P2,P3,P4	WAN: N/A,STB: P0 */
	WAN_PORT_MASK,
};

#define VLAN_ID_LAN	1
#define VLAN_ID_WAN	2
#define VLAN_ID_INIC	3
#define VLAN_ID_STB	12

enum { //acl id from 0 ~ 63
	ACLID_REDIRECT = 0,
	ACLID_INIC_CTRL_PKT,
	ACLID_WAN_TR_SOC,
	ACLID_SOC_TR_WAN,
};

#define ENUM_PORT_BEGIN(p, m, port_mask, cond)	\
	for (p = 0, m = (port_mask); 		\
		cond && m > 0; 			\
		m >>= 1, p++) {			\
		if (!(m & 1U))			\
			continue;

#define ENUM_PORT_END	}

/* Power ON/OFF port
 * @port:	port id
 * @onoff:
 *  0:		means off
 *  otherwise:	means on
 * @return:
 *  0:		success
 * -1:		invalid parameter
 */
static int __ctrl_port_power(rtk_port_t port, int onoff)
{
	rtk_port_phy_data_t pData;

	if (port > 4) {
		printk("%s(): Invalid port id %d\n", __func__, port);
		return -1;
	}

	rtk_port_phyReg_get(port, PHY_CONTROL_REG, &pData);
	if (onoff)
		pData &= ~CONTROL_REG_PORT_POWER_BIT;
	else
		pData |= CONTROL_REG_PORT_POWER_BIT;
	rtk_port_phyReg_set(port, PHY_CONTROL_REG, pData);

	return 0;
}

static inline int power_up_port(rtk_port_t port)
{
	return __ctrl_port_power(port, 1);
}

static inline int power_down_port(rtk_port_t port)
{
	return __ctrl_port_power(port, 0);
}

static void setRedirect(int inputPortMask, int outputPortMask)
{ // forward frames received from inputPorts and forward to outputPorts
	int retVal, ruleNum;
	rtk_filter_field_t field;
	rtk_filter_cfg_t cfg;
	rtk_filter_action_t act;
	int rule_id = ACLID_REDIRECT;

	rtk_filter_igrAcl_cfg_del(rule_id);

	if (inputPortMask == 0)
	{
		printk("### remove redirect ACL ###\n");
		return;
	}

	memset(&field, 0, sizeof(field));
	memset(&cfg, 0, sizeof(cfg));
	memset(&act, 0, sizeof(act));

	field.fieldType = FILTER_FIELD_DMAC;
	field.filter_pattern_union.dmac.dataType = FILTER_FIELD_DATA_MASK;
	field.filter_pattern_union.dmac.value.octet[0] = 0;
	field.filter_pattern_union.dmac.value.octet[1] = 0;
	field.filter_pattern_union.dmac.value.octet[2] = 0;
	field.filter_pattern_union.dmac.value.octet[3] = 0;
	field.filter_pattern_union.dmac.value.octet[4] = 0;
	field.filter_pattern_union.dmac.value.octet[5] = 0;
	field.filter_pattern_union.dmac.mask.octet[0]  = 0;
	field.filter_pattern_union.dmac.mask.octet[1]  = 0;
	field.filter_pattern_union.dmac.mask.octet[2]  = 0;
	field.filter_pattern_union.dmac.mask.octet[3]  = 0;
	field.filter_pattern_union.dmac.mask.octet[4]  = 0;
	field.filter_pattern_union.dmac.mask.octet[5]  = 0;
	field.next = NULL;
	if ((retVal = rtk_filter_igrAcl_field_add(&cfg, &field)) != RT_ERR_OK)
	{
		printk("### set redirect ACL field fail(%d) ###\n", retVal);
		return;
	}

	act.actEnable[FILTER_ENACT_REDIRECT] = TRUE;
	act.filterRedirectPortmask = outputPortMask; //0xFF;  //Forward to all ports
	cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
	cfg.activeport.value = inputPortMask; //0x1; //By your own decision
	cfg.activeport.mask = 0xFF;
	if ((retVal = rtk_filter_igrAcl_cfg_add(rule_id, &cfg, &act, &ruleNum)) != RT_ERR_OK)
	{
		printk("### set redirect ACL cfg fail(%d) ###\n", retVal);
		return;
	}
	prom_printf("### set redirect rule success ruleNum(%d) ###\n", ruleNum);
}

#ifdef TCSUPPORT_CPU_RT65168	//DSL-N66U
static int iNICVLanSetting(void)
{
	rtk_api_ret_t retVal;
	rtk_portmask_t mbrmsk, untagmsk;
	rtk_filter_number_t ruleNum;
	rtk_filter_field_t field;
	rtk_filter_cfg_t cfg;
	rtk_filter_action_t act;

	mbrmsk.bits[0] = INIC_ALL_PORTS_MASK;
	untagmsk.bits[0] = 0x00FF;
	if((retVal = rtk_vlan_set(VLAN_ID_INIC, mbrmsk, untagmsk, 3))) {
		printk("\r\nVLAN 3 Setup Failed!!!");
		return retVal;
	}

	//for mssid support
	untagmsk.bits[0] = 0x0000;
	if((retVal = rtk_vlan_set(4, mbrmsk, untagmsk, 4))) {
		printk("\r\nVLAN 4 Setup Failed!!!");
		return retVal;
	}
	if((retVal = rtk_vlan_set(5, mbrmsk, untagmsk, 5))) {
		printk("\r\nVLAN 5 Setup Failed!!!");
		return retVal;
	}
	if((retVal = rtk_vlan_set(6, mbrmsk, untagmsk, 6))) {
		printk("\r\nVLAN 6 Setup Failed!!!");
		return retVal;
	}
	if((retVal = rtk_vlan_set(7, mbrmsk, untagmsk, 7))) {
		printk("\r\nVLAN 7 Setup Failed!!!");
		return retVal;
	}
	if((retVal = rtk_vlan_set(8, mbrmsk, untagmsk, 8))) {
		printk("\r\nVLAN 8 Setup Failed!!!");
		return retVal;
	}
	if((retVal = rtk_vlan_set(9, mbrmsk, untagmsk, 9))) {
		printk("\r\nVLAN 9 Setup Failed!!!");
		return retVal;
	}

	//avoid broadcasting iNIC control frame to lan ports
	rtk_filter_igrAcl_cfg_del(ACLID_INIC_CTRL_PKT);

	memset(&cfg, 0, sizeof(rtk_filter_cfg_t));
	memset(&act, 0, sizeof(rtk_filter_action_t));
	memset(&field, 0, sizeof(rtk_filter_field_t));
	field.fieldType = FILTER_FIELD_ETHERTYPE;
	field.filter_pattern_union.etherType.dataType = FILTER_FIELD_DATA_MASK;
	field.filter_pattern_union.etherType.value = 0xFFFF;
	field.filter_pattern_union.etherType.mask = 0xFFFF;
	if ((retVal = rtk_filter_igrAcl_field_add(&cfg, &field)) != RT_ERR_OK)
	{
		printk("### set iNIC ACL field fail(%d) ###\n", retVal);
		return RT_ERR_FAILED;
	}

	cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
	cfg.activeport.value = 0xC0; //Port 6&7
	cfg.activeport.mask = 0xFF;
	cfg.invert = FALSE;
	act.actEnable[FILTER_ENACT_INGRESS_CVLAN_VID] = TRUE;
	act.filterIngressCvlanVid= 3;
	if ((retVal = rtk_filter_igrAcl_cfg_add(ACLID_INIC_CTRL_PKT, &cfg, &act, &ruleNum)) != RT_ERR_OK)
	{
		printk("### set iNIC ACL cfg fail(%d) ###\n", retVal);
		return RT_ERR_FAILED;
	}
	prom_printf("### set iNIC rule success ruleNum(%d) ###\n", ruleNum);

	return 0;
}
#endif

static int __LANWANPartition(int wan_stb_x)
{
	rtk_api_ret_t retVal;
	rtk_portmask_t fwd_mask;
	unsigned int port, mask, lan_port_mask, wan_port_mask, stb_port_mask;

	if (wan_stb_x < 0 || wan_stb_x >= ARRAY_SIZE(s_wan_stb_array)) {
		printk("%s() Invalid wan_stb_x %d\n", __func__, wan_stb_x);
		return -EINVAL;
	}

	if( (retVal = rtk_vlan_init()) ) {
		printk("\r\nVLAN Initial Failed!!!");
		return retVal;
	}

#ifdef TCSUPPORT_MULTISERVICE_ON_WAN
	lan_port_mask = (INIC_PORT_MASK | LAN_ALL_PORTS_MASK) & ~s_wan_stb_array[wan_stb_x];
	wan_port_mask = WAN_ALL_PORTS_MASK;
	stb_port_mask = s_wan_stb_array[wan_stb_x] | STB_MAC_PORT_MASK;
#else
	stb_port_mask = s_wan_stb_array[wan_stb_x];
	lan_port_mask = INIC_PORT_MASK | LAN_ALL_PORTS_MASK & ~stb_port_mask;
	wan_port_mask = WAN_ALL_PORTS_MASK | stb_port_mask;
#endif
	prom_printf("wan_stb_x %d LAN/WAN/STB ports mask 0x%03x/%03x/%03x\n",
		wan_stb_x, lan_port_mask, wan_port_mask, stb_port_mask);

	/* LAN */
	ENUM_PORT_BEGIN(port, mask, lan_port_mask, 1)
		fwd_mask.bits[0] = lan_port_mask;
		if((retVal = rtk_port_isolation_set(port, fwd_mask)))	return retVal;
		if((retVal = rtk_port_efid_set(port, 0)))	return retVal;
	ENUM_PORT_END

	/* WAN */
	ENUM_PORT_BEGIN(port, mask, wan_port_mask, 1)
		fwd_mask.bits[0] = wan_port_mask;
		if((retVal = rtk_port_isolation_set(port, fwd_mask)))	return retVal;
		if((retVal = rtk_port_efid_set(port, 0)))	return retVal;
	ENUM_PORT_END

#ifdef TCSUPPORT_MULTISERVICE_ON_WAN
	/* STB */
	ENUM_PORT_BEGIN(port, mask, stb_port_mask, 1)
		fwd_mask.bits[0] = stb_port_mask;
		if((retVal = rtk_port_isolation_set(port, fwd_mask)))	return retVal;
		if((retVal = rtk_port_efid_set(port, 0)))	return retVal;
	ENUM_PORT_END
#endif

	port = LAN_MAC_PORT;
	fwd_mask.bits[0] = lan_port_mask | wan_port_mask | stb_port_mask;
	if((retVal = rtk_port_isolation_set(port, fwd_mask)))	return retVal;
	if((retVal = rtk_port_efid_set(port, 0)))	return retVal;

	{
		rtk_portmask_t mbrmsk, untagmsk;

		mbrmsk.bits[0] = lan_port_mask;
		untagmsk.bits[0] = 0x00FF & ~LAN_MAC_PORT_MASK; //SoC should get tag packet
		if((retVal = rtk_vlan_set(VLAN_ID_LAN, mbrmsk, untagmsk, 1))) {
			printk("\r\nVLAN 1 Setup Failed!!!");
			return retVal;
		}

		mbrmsk.bits[0] = wan_port_mask;
		untagmsk.bits[0] = 0x00FF & ~WAN_MAC_PORT_MASK; //SoC should get tag packet
		if((retVal = rtk_vlan_set(VLAN_ID_WAN, mbrmsk, untagmsk, 2))) {
			printk("\r\nVLAN 2 Setup Failed!!!");
			return retVal;
		}

#ifdef TCSUPPORT_MULTISERVICE_ON_WAN
		mbrmsk.bits[0] = stb_port_mask;
		untagmsk.bits[0] = 0x00FF & ~STB_MAC_PORT_MASK; //SoC should get tag packet
		if((retVal = rtk_vlan_set(VLAN_ID_STB, mbrmsk, untagmsk, RTK_IVL_MODE_FID))) {
			printk("\r\nVLAN %d Setup Failed!!!", VLAN_ID_STB);
			return retVal;
		}
#endif
	}

	/* LAN */
	ENUM_PORT_BEGIN(port, mask, lan_port_mask, 1)
		if((retVal = rtk_vlan_portPvid_set(port, VLAN_ID_LAN, 0))) {
			printk("\r\nPort %d Setup Failed!!!", port);
			return retVal;
		}
	ENUM_PORT_END

	/* WAN */
	ENUM_PORT_BEGIN(port, mask, wan_port_mask, 1)
		if((retVal = rtk_vlan_portPvid_set(port, VLAN_ID_WAN, 0))) {
			printk("\r\nPort %d Setup Failed!!!", port);
			return retVal;
		}
	ENUM_PORT_END

#ifdef TCSUPPORT_MULTISERVICE_ON_WAN
	/* STB */
	ENUM_PORT_BEGIN(port, mask, stb_port_mask, 1)
		if((retVal = rtk_vlan_portPvid_set(port, VLAN_ID_STB, 0))) {
			printk("\r\nPort %d Setup Failed!!!", port);
			return retVal;
		}
	ENUM_PORT_END
#endif

#ifdef TCSUPPORT_CPU_RT65168	//DSL-N66U
	iNICVLanSetting();
#endif

	return 0;
}

static int __LANWANPartition_forATE(int wan_stb_x)
{
	rtk_portmask_t fwd_mask;
	unsigned int port, mask, lan_port_mask, wan_port_mask, stb_port_mask;

	if (wan_stb_x < 0 || wan_stb_x >= ARRAY_SIZE(s_wan_stb_array)) {
		printk("%s() Invalid wan_stb_x %d\n", __func__, wan_stb_x);
		return -EINVAL;
	}

	if( RT_ERR_OK != rtk_vlan_init())
		printk("\r\nVLAN Initial Failed!!!");

	stb_port_mask = s_wan_stb_array[wan_stb_x];
	lan_port_mask = INIC_PORT_MASK | LAN_ALL_PORTS_MASK & ~stb_port_mask;	//Sam, 2012/06/26
	wan_port_mask = WAN_ALL_PORTS_MASK | stb_port_mask;
/*
	printk("wan_stb_x %d STB,LAN/WAN ports mask 0x%03x,%03x/%03x\n",
		wan_stb_x, stb_port_mask, lan_port_mask, wan_port_mask);
*/

	/* LAN */
	ENUM_PORT_BEGIN(port, mask, lan_port_mask, 1)
		fwd_mask.bits[0] = lan_port_mask;
		rtk_port_isolation_set(port, fwd_mask);
		rtk_port_efid_set(port, 0);
	ENUM_PORT_END

	/* WAN */
	ENUM_PORT_BEGIN(port, mask, wan_port_mask, 1)
		fwd_mask.bits[0] = wan_port_mask;
		rtk_port_isolation_set(port, fwd_mask);
		rtk_port_efid_set(port, 0);
	ENUM_PORT_END

	port = LAN_MAC_PORT;
	fwd_mask.bits[0] = lan_port_mask | wan_port_mask;
	rtk_port_isolation_set(port, fwd_mask);
	rtk_port_efid_set(port, 0);

	{
		rtk_portmask_t mbrmsk, untagmsk;

		mbrmsk.bits[0] = lan_port_mask;
		untagmsk.bits[0] = 0x00FF & ~LAN_MAC_PORT_MASK; //SoC should get tag packet
		if( RT_ERR_OK != rtk_vlan_set(1, mbrmsk, untagmsk, 1))
			printk("\r\nVLAN 1 Setup Failed!!!");

		mbrmsk.bits[0] = wan_port_mask;
		untagmsk.bits[0] = 0x00FF & ~LAN_MAC_PORT_MASK; //SoC should get tag packet
		if( RT_ERR_OK != rtk_vlan_set(VLAN_ID_WAN, mbrmsk, untagmsk, 2))
			printk("\r\nVLAN 2 Setup Failed!!!");
//#ifdef CONFIG_WLAN2_ON_SWITCH_GMAC2
//		mbrmsk.bits[0] = LAN_MAC_PORT_MASK | WLAN2_MAC_PORT_MASK;
//		untagmsk.bits[0] = 0x00FF;
//		if( RT_ERR_OK != rtk_vlan_set(3, mbrmsk, untagmsk, 3))
//			printk("\r\nVLAN 3 Setup Failed!!!");
//#if defined(CONFIG_RT3352_INIC_MII) || defined(CONFIG_RT3352_INIC_MII_MODULE) // for lanaccess control
//		mbrmsk.bits[0] = LAN_MAC_PORT_MASK | WLAN2_MAC_PORT_MASK; //wireless pkt through iNIC that NOT allow to access LAN devices
//		untagmsk.bits[0] = 0x0000;
//		if( RT_ERR_OK != rtk_vlan_set(4, mbrmsk, untagmsk, 4))
//			printk("\r\nVLAN 4 Setup Failed!!!");
//		if( RT_ERR_OK != rtk_vlan_set(5, mbrmsk, untagmsk, 5))
//			printk("\r\nVLAN 5 Setup Failed!!!");
//		if( RT_ERR_OK != rtk_vlan_set(6, mbrmsk, untagmsk, 6))
//			printk("\r\nVLAN 6 Setup Failed!!!");
//#endif
//#endif
	}

	/* LAN */
	ENUM_PORT_BEGIN(port, mask, lan_port_mask, 1)
		if(RT_ERR_OK != rtk_vlan_portPvid_set(port, 1, 0))
			printk("\r\nPort %d Setup Failed!!!", port);
	ENUM_PORT_END

	/* WAN */
	ENUM_PORT_BEGIN(port, mask, wan_port_mask, 1)
		if(RT_ERR_OK != rtk_vlan_portPvid_set(port, VLAN_ID_WAN, 0))
			printk("\r\nPort %d Setup Failed!!!", port);
	ENUM_PORT_END

#if 0
#ifdef CONFIG_WLAN2_ON_SWITCH_GMAC2
	{
		int retVal, ruleNum;
		rtk_filter_field_t field;
		rtk_filter_cfg_t cfg;
		rtk_filter_action_t act;

		memset(&cfg, 0, sizeof(rtk_filter_cfg_t));
		memset(&act, 0, sizeof(rtk_filter_action_t));
		memset(&field, 0, sizeof(rtk_filter_field_t));
		field.fieldType = FILTER_FIELD_ETHERTYPE;
		field.filter_pattern_union.etherType.dataType = FILTER_FIELD_DATA_MASK;
		field.filter_pattern_union.etherType.value = 0xFFFF;
		field.filter_pattern_union.etherType.mask = 0xFFFF;
		if ((retVal = rtk_filter_igrAcl_field_add(&cfg, &field)) != RT_ERR_OK)
		{
			printk("### set iNIC ACL field fail(%d) ###\n", retVal);
			return RT_ERR_FAILED;
		}

		cfg.activeport.dataType = FILTER_FIELD_DATA_MASK;
		cfg.activeport.value = 0xC0; //Port 6&7
		cfg.activeport.mask = 0xFF;
		cfg.invert = FALSE;
		act.actEnable[FILTER_ENACT_INGRESS_CVLAN_VID] = TRUE;
		act.filterIngressCvlanVid= 3;
		if ((retVal = rtk_filter_igrAcl_cfg_add(ACLID_INIC_CTRL_PKT, &cfg, &act, &ruleNum)) != RT_ERR_OK)
		{
			printk("### set iNIC ACL cfg fail(%d) ###\n", retVal);
			return RT_ERR_FAILED;
		}
		printk("### set iNIC rule success ruleNum(%d) ###\n", ruleNum);
	}
#endif //CONFIG_WLAN2_ON_SWITCH_GMAC2
#endif

	return 0;
}

int AllLanPartition(void)
{
	rtk_api_ret_t retVal;
	rtk_portmask_t fwd_mask, mbrmsk, untagmsk;
	unsigned int port, mask, lan_port_mask;
	
	if( (retVal = rtk_vlan_init())) {
		printk("rtk_vlan_init(): return %d\n", retVal);
		return retVal;
	}
		
	lan_port_mask = INIC_PORT_MASK | LAN_ALL_PORTS_MASK | WAN_ALL_PORTS_MASK;
	
	ENUM_PORT_BEGIN(port, mask, lan_port_mask, 1)
		fwd_mask.bits[0] = lan_port_mask;
		rtk_port_isolation_set(port, fwd_mask);
		rtk_port_efid_set(port, 0);
		rtk_vlan_portPvid_set(port, 1, 0);
	ENUM_PORT_END
	
	mbrmsk.bits[0] = lan_port_mask;
	untagmsk.bits[0] = 0x00FF & ~LAN_MAC_PORT_MASK; //SoC should get tag packet
	rtk_vlan_set(1, mbrmsk, untagmsk, 1);

#ifdef TCSUPPORT_CPU_RT65168	//DSL-N66U
	iNICVLanSetting();
#endif //TCSUPPORT_CPU_RT65168

	return 0;
}

static int initialize_switch(void)
{
	rtk_api_ret_t retVal;
	rtk_port_mac_ability_t mac_cfg;
	rtk_portmask_t portmask;
	int input_txDelay;
	int input_rxDelay;

	retVal = rtk_switch_init();
	prom_printf("rtk_switch_init(): return %d\n", retVal);

	// configure GMAC ports
	mac_cfg.forcemode = MAC_FORCE;
	mac_cfg.speed = SPD_1000M;
	mac_cfg.duplex = FULL_DUPLEX;
	mac_cfg.link = PORT_LINKUP;
	mac_cfg.nway = DISABLED;
	mac_cfg.txpause = ENABLED;
	mac_cfg.rxpause = ENABLED;
	retVal = rtk_port_macForceLinkExt_set (RTK_EXT_1, MODE_EXT_RGMII,&mac_cfg);
	prom_printf("rtk_port_macForceLinkExt_set(): return %d\n", retVal);

	input_txDelay = 1;
	input_rxDelay = 4;
	prom_printf("input_txDelay:%d, input_rxDelay:%d\n", input_txDelay, input_rxDelay);
	retVal = rtk_port_rgmiiDelayExt_set(RTK_EXT_1, input_txDelay, input_rxDelay);
	prom_printf("rtk_port_rgmiiDelayExt_set(): return %d\n", retVal);

#ifdef TCSUPPORT_CPU_RT65168	//DSL-N66U
	retVal = rtk_port_macForceLinkExt_set (RTK_EXT_2, MODE_EXT_RGMII,&mac_cfg);
	prom_printf("rtk_port_macForceLinkExt_set(): return %d\n", retVal);

	input_txDelay = 1;
	input_rxDelay = 0;
	prom_printf("input_txDelay:%d, input_rxDelay:%d\n", input_txDelay, input_rxDelay);
	retVal = rtk_port_rgmiiDelayExt_set(RTK_EXT_2, input_txDelay, input_rxDelay);
	prom_printf("rtk_port_rgmiiDelayExt_set(): return %d\n", retVal);
#endif

	portmask.bits[0]=0x1f;
	rtk_led_enable_set(LED_GROUP_0, portmask);
#ifdef TCSUPPORT_CPU_RT63365
	rtk_led_groupConfig_set(LED_GROUP_0, LED_CONFIG_LINK_ACT);
#else	//vsl-n66u
	rtk_led_groupConfig_set(LED_GROUP_0, LED_CONFIG_SPD10010ACT);

	rtk_led_enable_set(LED_GROUP_1, portmask);
	rtk_led_groupConfig_set(LED_GROUP_1, LED_CONFIG_SPD1000ACT);
#endif
	rtk_led_operation_set(LED_OP_PARALLEL);

	// power down all ports
	prom_printf("power down all ports\n");
	power_down_port(LAN_PORT_4);
	power_down_port(LAN_PORT_3);
	power_down_port(LAN_PORT_2);
	power_down_port(LAN_PORT_1);
	power_down_port(WAN_PORT);

	rtk_switch_portMaxPktLen_set(LAN_PORT_1, 16000);
	rtk_switch_portMaxPktLen_set(LAN_PORT_2, 16000);
	rtk_switch_portMaxPktLen_set(LAN_PORT_3, 16000);
	rtk_switch_portMaxPktLen_set(LAN_PORT_4, 16000);

//tmp, port 1 to 1000M
//	rtk_port_phy_ability_t	ability;
//	ability.Full_1000 = 1;
//	rtk_port_phyForceModeAbility_set(LAN_PORT_1, &ability);

//tmp, Let priority of iNIC control frame be highest.
#ifdef TCSUPPORT_CPU_RT65168	//DSL-N66U
{
	//priority 7, 6 int queue 7
	//priority 5, 4 int queue 1
	//priority 3 ~0 int queue 0
	rtk_qos_init(3);

	rtk_priority_select_t priDec;
	priDec.port_pri  = 6;
	priDec.dot1q_pri = 5;
	priDec.acl_pri   = 7;
	priDec.dscp_pri  = 4;
	priDec.cvlan_pri = 3;
	priDec.svlan_pri = 2;
	priDec.dmac_pri  = 1;
	priDec.smac_pri  = 0;
	rtk_qos_priSel_set(&priDec);

	rtk_qos_queue_weights_t qweights;
	qweights.weights[7] = 0;	//strict priority
	qweights.weights[6] = 0;	//strict priority
	qweights.weights[5] = 2;
	qweights.weights[4] = 2;
	qweights.weights[3] = 1;
	qweights.weights[2] = 1;
	qweights.weights[1] = 1;
	qweights.weights[0] = 1;
	rtk_qos_schedulingQueue_set(6, &qweights);
	rtk_qos_schedulingQueue_set(7, &qweights);
}
#endif

	retVal = rtk_vlan_init();
	prom_printf("rtk_vlan_init(): return %d\n", retVal);

	retVal = rtk_filter_igrAcl_init();
	prom_printf("rtk_filter_igrAcl_init(): return %d\n", retVal);

	//rtk_cpu_enable_set(ENABLE);
	//rtk_cpu_tagPort_set(RTK_EXT_1_MAC,CPU_INSERT_TO_NONE);
	//printk("all port together");

	//__LANWANPartition(0);

	return 0;
}

int rtl8367rb_ioctl(int req, int req_val)
{
	rtk_api_ret_t retVal;
	rtk_port_phy_data_t pData;
	rtk_port_t port;
	rtk_stat_port_cntr_t portCntrs;
	rtk_uint32  address = 0;
	rtk_l2_ucastAddr_t l2_data;
	rtk_port_mac_ability_t mac_cfg;
	int i;
	int input_txDelay = 0;
	int input_rxDelay = 4;
	unsigned long regData;
	rtk_portmask_t mbrmsk, untagmsk;
	rtk_fid_t fid;

	switch(req) {
	case 0:
	// all port together
		rtk_cpu_enable_set(ENABLE);
		rtk_cpu_tagPort_set(RTK_EXT_1_MAC,CPU_INSERT_TO_NONE);
		prom_printf("all port together");
		break;	
	case 1:	// vlan partition, set wan and stb
		if((retVal = __LANWANPartition(req_val)))	//req_val: 0 default, no stb port
			prom_printf("Partition VLAN failed: return %d\n", retVal);
		else
			prom_printf("VLAN partitioned\n");
		break;			
	case 2:			// show state of RTL8367RB
		set_mib_str_table();
		for (port = 0; port <5; port++)
		{

			if(RT_ERR_OK != rtk_stat_port_getAll(port, &portCntrs))
				return RT_ERR_FAILED;

			show_mib_counter(port, &portCntrs);
		}
		
		if(RT_ERR_OK != rtk_stat_port_getAll(RTK_EXT_1_MAC, &portCntrs))
			return RT_ERR_FAILED;
		show_mib_counter(RTK_EXT_1_MAC, &portCntrs);

		if(RT_ERR_OK != rtk_stat_port_getAll(RTK_EXT_2_MAC, &portCntrs))
			return RT_ERR_FAILED;
		show_mib_counter(RTK_EXT_2_MAC, &portCntrs);
		break;
	case 3:		//get l2 address
		while (1) {
			if((retVal=rtk_l2_addr_next_get(3, 0, &address, &l2_data))!=RT_ERR_OK)
				break;
			prom_printf("mac addr: %02x:%02x:%02x:%02x:%02x:%02x\n",
				l2_data.mac.octet[0], l2_data.mac.octet[1], l2_data.mac.octet[2],
				l2_data.mac.octet[3], l2_data.mac.octet[4], l2_data.mac.octet[5]);
			address++;
		}
		break;
	case 4:		//initial switch
		initialize_switch();
		break;
	case 5:	//change vlan partition, set all port as lan
		AllLanPartition();
		prom_printf("Set all as lan.\n");
		break;
	case 6:	//show mib counter
		set_mib_str_table();
		if((retVal = rtk_stat_port_getAll(req_val, &portCntrs))) {
			prom_printf("rtk_stat_port_getAll(): return %d\n", retVal);
			break;
		}
		show_mib_counter(req_val, &portCntrs);
		break;
	case 7:	//read reg
		regData = ReadRtk(req_val);
		prom_printf("Read rtk phy reg %x=%04x\n", req_val, ReadRtk(req_val));
		return (int)regData;
	case 8:	//modify rtk_ext_1 input txDelay
		rtk_port_rgmiiDelayExt0_get(&input_txDelay, &input_rxDelay);
		retVal = rtk_port_rgmiiDelayExt0_set(req_val, input_rxDelay);
		prom_printf("rtk_port_rgmiiDelayExt0_set(): return %d\n", retVal);
		break;
	case 9:	//modify rtk_ext_1 input rxDelay
		rtk_port_rgmiiDelayExt0_get(&input_txDelay, &input_rxDelay);
		retVal = rtk_port_rgmiiDelayExt0_set(input_txDelay, req_val);
		prom_printf("rtk_port_rgmiiDelayExt0_set(): return %d\n", retVal);
		break;
	case 12:		// power up specified port
		prom_printf("power up port %d\n", req_val);
		
		retVal = rtk_port_phyReg_get(req_val, PHY_CONTROL_REG, &pData);
		pData &= ~CONTROL_REG_PORT_POWER_BIT;
		retVal = rtk_port_phyReg_set(req_val, PHY_CONTROL_REG, pData);
		prom_printf("rtk_port_phyReg_set(): return %d\n", retVal);
		break;

	case 13:		// power down specified port
		prom_printf("power down port %d\n", req_val);
		
		retVal = rtk_port_phyReg_get(req_val, PHY_CONTROL_REG, &pData);
		pData |= CONTROL_REG_PORT_POWER_BIT;
		retVal = rtk_port_phyReg_set(req_val, PHY_CONTROL_REG, pData);
		prom_printf("rtk_port_phyReg_set(): return %d\n", retVal);
		break;

	case 14:		// power up LAN port(s)
		prom_printf("power up LAN ports\n");

		rtk_port_phyReg_get(LAN_PORT_4, PHY_CONTROL_REG, &pData);
		pData &= ~CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_4, PHY_CONTROL_REG, pData);

		rtk_port_phyReg_get(LAN_PORT_3, PHY_CONTROL_REG, &pData);
		pData &= ~CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_3, PHY_CONTROL_REG, pData);

		rtk_port_phyReg_get(LAN_PORT_2, PHY_CONTROL_REG, &pData);
		pData &= ~CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_2, PHY_CONTROL_REG, pData);

		rtk_port_phyReg_get(LAN_PORT_1, PHY_CONTROL_REG, &pData);
		pData &= ~CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_1, PHY_CONTROL_REG, pData);

		break;

	case 15:		// power down LAN port(s)
		prom_printf("power down LAN ports\n");

		rtk_port_phyReg_get(LAN_PORT_4, PHY_CONTROL_REG, &pData);
		pData |= CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_4, PHY_CONTROL_REG, pData);
                        
		rtk_port_phyReg_get(LAN_PORT_3, PHY_CONTROL_REG, &pData);
		pData |= CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_3, PHY_CONTROL_REG, pData);
                        
		rtk_port_phyReg_get(LAN_PORT_2, PHY_CONTROL_REG, &pData);
		pData |= CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_2, PHY_CONTROL_REG, pData);
                        
		rtk_port_phyReg_get(LAN_PORT_1, PHY_CONTROL_REG, &pData);
		pData |= CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_1, PHY_CONTROL_REG, pData);

		break;

	case 16:	// enable iNIC port
		mac_cfg.forcemode = MAC_FORCE;
		mac_cfg.speed = SPD_1000M;
		mac_cfg.duplex = FULL_DUPLEX;
		mac_cfg.link = PORT_LINKUP;
		mac_cfg.nway = DISABLED;
		mac_cfg.txpause = ENABLED;
		mac_cfg.rxpause = ENABLED;
		retVal = rtk_port_macForceLinkExt_set (RTK_EXT_2, MODE_EXT_RGMII, &mac_cfg);
		prom_printf("rtk_port_macForceLinkExt_set(): return %d\n", retVal);
		break;

	case 17:	// disable iNIC port
		mac_cfg.forcemode = MAC_FORCE;
		mac_cfg.speed = SPD_1000M;
		mac_cfg.duplex = FULL_DUPLEX;
		mac_cfg.link = PORT_LINKUP;
		mac_cfg.nway = DISABLED;
		mac_cfg.txpause = ENABLED;
		mac_cfg.rxpause = ENABLED;
		retVal = rtk_port_macForceLinkExt_set (RTK_EXT_2, MODE_EXT_DISABLE, &mac_cfg);
		prom_printf("rtk_port_macForceLinkExt_set(): return %d\n", retVal);
		break;

	case 18:
		setRedirect(req_val, 0xff);
		break;
	case 19:	//remove redirect rule
		rtk_filter_igrAcl_cfg_del(ACLID_REDIRECT);
		break;
	case 20:		//check Ethernet Port status
		mac_cfg.link = PORT_LINKDOWN;
		for(i=1 ; i<=4 ; i++){	//0 means WAN port,skip it
			rtk_port_macStatus_get(i, &mac_cfg);
			if(mac_cfg.link == PORT_LINKUP)
				break;
		}
		return mac_cfg.link;
	case 21:		//check the Ethernet WanLanStatus for ATE.
		rtk_port_macStatus_get(req_val, &mac_cfg);
		if(mac_cfg.link == PORT_LINKUP){
			if(mac_cfg.speed == PORT_SPEED_1000M)
				return 2;
			else
				return 1;
		}
		else{
			return 0;
		}
		break;
	//Ren.B
	case 22:
		{
			struct file *file;
			loff_t pos = 0;
			char data[600] = {0};
			mm_segment_t old_fs;
			rtk_uint64 inTotal = 0;
			rtk_uint64 outTotal = 0;

			old_fs = get_fs();
			set_fs(get_ds());
			file = filp_open("/tmp/ether_switch_traffic", O_WRONLY|O_CREAT, 0644);
			if(file)
			{
				for (port = 0; port <5; port++)
				{
					memset(&portCntrs, 0, sizeof(rtk_stat_port_cntr_t));
					if(RT_ERR_OK != rtk_stat_port_getAll(port, &portCntrs))
					{
						//do not show the unnecessary message
						//printk("[rtkethdrv.c][%d]rtk_stat_port_getAll() fail on port %d\n", __LINE__, port);
					}

					sprintf(data+strlen(data), "ifInOctets port:%d %llu\n", port, portCntrs.ifInOctets );
					sprintf(data+strlen(data), "ifOutOctets port:%d %llu\n", port, portCntrs.ifOutOctets );
					if(port==0)
					{
						sprintf(data+strlen(data), "-----------------------\n" );
					}
					if(port!=0)
					{
						inTotal += portCntrs.ifInOctets;
						outTotal += portCntrs.ifOutOctets;
					}
				}
				sprintf(data+strlen(data), "-----------------------\n" );
				/* the total wired data = port1+port2+port3+port4, port0 is Ethernet Wan which is not included. */
				sprintf(data+strlen(data), "TotalInOctets:%llu\n", inTotal );
				sprintf(data+strlen(data), "TotalOutOctets:%llu\n", outTotal );
				vfs_write(file, data, strlen(data), &pos);
				set_fs(old_fs);
				filp_close(file, NULL);
			}
			
		}
		break;
	//Ren.E
	case 23:	//read vlan member
		if((retVal = rtk_vlan_get(req_val, &mbrmsk, &untagmsk, &fid)))
			prom_printf("rtk_vlan_get return %d\n", retVal);
		else {
			prom_printf("mbrmsk.bits[0]: %x\n", mbrmsk.bits[0]);
			prom_printf("untagmsk.bits[0]: %x\n", untagmsk.bits[0]);
		}
		break;
	case 24:
			//force LED 'on' mode
			#ifdef TCSUPPORT_CPU_RT63365 //DSL-N55U-C1 for example
			rtk_led_modeForce_set(LED_GROUP_0, LED_FORCE_ON);
			#else //DSL-N66U
			rtk_led_modeForce_set(LED_GROUP_0, LED_FORCE_ON);
			rtk_led_modeForce_set(LED_GROUP_1, LED_FORCE_ON);
			#endif
		break;
	case 25:
			//force LED 'off' mode
			#ifdef TCSUPPORT_CPU_RT63365 //DSL-N55U-C1 for example
			rtk_led_modeForce_set(LED_GROUP_0, LED_FORCE_OFF);
			#else //DSL-N66U
			rtk_led_modeForce_set(LED_GROUP_0, LED_FORCE_OFF);
			rtk_led_modeForce_set(LED_GROUP_1, LED_FORCE_OFF);
			#endif
		break;
	case 26:
			//force LED 'normal' mode
			#ifdef TCSUPPORT_CPU_RT63365 //DSL-N55U-C1 for example
			rtk_led_modeForce_set(LED_GROUP_0, LED_FORCE_NORMAL);
			#else //DSL-N66U
			rtk_led_modeForce_set(LED_GROUP_0, LED_FORCE_NORMAL);
			rtk_led_modeForce_set(LED_GROUP_1, LED_FORCE_NORMAL);
			#endif
		break;

	/* For ATE command.B */
	//copy from case 1
	case 50:	// vlan partition, set wan and stb
		__LANWANPartition_forATE(req_val);	//req_val: 0 default, no stb port
		//printk("vlan partition");
		break;		

	//copy from case 5
	case 51:	//change vlan partition, set all port as lan
		AllLanPartition();
		//printk("Set all as lan.\n");
		break;
	/* For ATE command.E */
		
	}
	return 0;
}
EXPORT_SYMBOL(rtl8367rb_ioctl);

static int eth_ATE_link_st_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{

	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	int ret;

	/*
	ret = 0: X : Not Active
	ret = 1: M : 10/100 Mbps
	ret = 2: G : 1000 Mbps
	*/
	ret = rtl8367rb_ioctl(21, 0);
	index += sprintf(buf+index, "W0=%c;", (ret == 1) ? 'M': (ret == 2) ? 'G': 'X');
	CHK_BUF();

	ret = rtl8367rb_ioctl(21, 1);
	index += sprintf(buf+index, "L1=%c;", (ret == 1) ? 'M': (ret == 2) ? 'G': 'X');
	CHK_BUF();

	ret = rtl8367rb_ioctl(21, 2);
	index += sprintf(buf+index, "L2=%c;", (ret == 1) ? 'M': (ret == 2) ? 'G': 'X');
	CHK_BUF();

	ret = rtl8367rb_ioctl(21, 3);
	index += sprintf(buf+index, "L3=%c;", (ret == 1) ? 'M': (ret == 2) ? 'G': 'X');
	CHK_BUF();

	ret = rtl8367rb_ioctl(21, 4);
	index += sprintf(buf+index, "L4=%c;\n", (ret == 1) ? 'M': (ret == 2) ? 'G': 'X');
	CHK_BUF();

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0)
		index = 0;
	if (index>count)
		index = count;
	return index;
}

static int eth_ATE_link_st_2_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{

	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	int ret;

	/*
	ret = 0: X : Not Active
	ret = 1: M : 10/100 Mbps
	ret = 2: G : 1000 Mbps
	*/
	ret = rtl8367rb_ioctl(21, 0);
	index += sprintf(buf+index, "W0=%c;", (ret == 1) ? 'M': (ret == 2) ? 'G': 'X');
	CHK_BUF();

	ret = rtl8367rb_ioctl(21, 4);
	index += sprintf(buf+index, "L1=%c;", (ret == 1) ? 'M': (ret == 2) ? 'G': 'X');
	CHK_BUF();

	ret = rtl8367rb_ioctl(21, 3);
	index += sprintf(buf+index, "L2=%c;", (ret == 1) ? 'M': (ret == 2) ? 'G': 'X');
	CHK_BUF();

	ret = rtl8367rb_ioctl(21, 2);
	index += sprintf(buf+index, "L3=%c;", (ret == 1) ? 'M': (ret == 2) ? 'G': 'X');
	CHK_BUF();

	ret = rtl8367rb_ioctl(21, 1);
	index += sprintf(buf+index, "L4=%c;\n", (ret == 1) ? 'M': (ret == 2) ? 'G': 'X');
	CHK_BUF();

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0)
		index = 0;
	if (index>count)
		index = count;
	return index;
}
static int eth_wan_link_st_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{

	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	int ret;

	/*
	ret = 0: X : Not Active
	ret = 1: M : 10/100 Mbps
	ret = 2: G : 1000 Mbps
	*/
	ret = rtl8367rb_ioctl(21, 0);
	index += sprintf(buf, "%d", ret);
	CHK_BUF();

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0)
		index = 0;
	if (index>count)
		index = count;
	return index;
}

int eth_lan_link_st_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{

	int index = 0;
	off_t pos = 0;
	off_t begin = 0;
	int ret;

	ret = rtl8367rb_ioctl(20, 0);
	index += sprintf(buf, "%d", ret);
	CHK_BUF();

done:
	*start = buf + (off - begin);
	index -= (off - begin);
	if (index<0)
		index = 0;
	if (index>count)
		index = count;
	return index;
}

int rtk_switch_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", rtk_switch_status);
	len -= off;
	*start = page + off;

	if (len > count)
		len = count;
	else
		*eof = 1;

	if (len < 0)
		len = 0;

	return len;
}

int rtk_switch_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[32];
	int val1, val2;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	if (sscanf(val_string, "%d %d", &val1, &val2) != 2) {
		printk("error , <p1> <p2>\n");
		return count;
	}

	rtk_switch_status = rtl8367rb_ioctl(val1, val2);

	return count;
}

int getAll_rtk_port_stat(struct seq_file *seq)
{
	int port;
	rtk_uint64 inTotal = 0;
	rtk_uint64 outTotal = 0;
	rtk_stat_port_cntr_t portCntrs;
	rtk_stat_counter_t inCntrs;
	rtk_stat_counter_t outCntrs;

	inTotal = 0;
	outTotal = 0;
#if 1
	inCntrs = 0;
	outCntrs = 0;
	for( port = 0; port < 5; port++ )
	{
		if(RT_ERR_OK != rtk_stat_port_get(port, STAT_IfInOctets, &inCntrs))
		{
			seq_printf( seq, "[rtkethdrv.c][%d]rtk_stat_port_get() RX fail on port %d", __LINE__, port );
			return -1;
		}
		msleep(100);
		if(RT_ERR_OK != rtk_stat_port_get(port, STAT_IfOutOctets, &outCntrs))
		{
			seq_printf( seq, "[rtkethdrv.c][%d]rtk_stat_port_get() TX fail on port %d", __LINE__, port );
			return -1;
		}
		
		if( port == 0 )
		{
			seq_printf( seq, "------Ethernet WAN port------\n");
		}
		else
		{
			//accumulate LAN ports only.
			inTotal += inCntrs;
			outTotal += outCntrs;
		}
//printk("[port %d]in=%llu, ou=%llu\n", port, inCntrs, outCntrs);//Ren.test
		seq_printf( seq, "ifInOctets port:%d %llu\n", port, inCntrs );
		seq_printf( seq, "ifOutOctets port:%d %llu\n", port, outCntrs );

		if( port == 0 )
		{
			seq_printf( seq, "------LAN ports------\n");
		}
	}
#else
	for( port = 0; port < 5; port++ )
	{
		memset(&portCntrs, 0, sizeof(rtk_stat_port_cntr_t));
		if(RT_ERR_OK != rtk_stat_port_getAll(port, &portCntrs))
		{
			seq_printf( seq, "[rtkethdrv.c][%d]rtk_stat_port_getAll() fail on port %d", __LINE__, port );
			return -1;
		}
		msleep(100);
		
		if( port == 0 )
		{
			seq_printf( seq, "------Ethernet WAN port------\n");
		}
		else
		{
			//accumulate LAN ports only.
			inTotal += portCntrs.ifInOctets;
			outTotal += portCntrs.ifOutOctets;
			if(port==2){
				printk("inS=%llu, outS=%llu\n", portCntrs.ifInOctets, portCntrs.ifOutOctets);//Ren.test
				printk("inA=%llu, outA=%llu\n", inTotal, outTotal);//Ren.test
			}
		}

		seq_printf( seq, "ifInOctets port:%d %llu\n", port, portCntrs.ifInOctets );
		seq_printf( seq, "ifOutOctets port:%d %llu\n", port, portCntrs.ifOutOctets );

		if( port == 0 )
		{
			seq_printf( seq, "------LAN ports------\n");
		}
	}
#endif
	seq_printf( seq, "------accumulative LAN ports------\n");
	seq_printf( seq, "TotalInOctets:%llu\n", inTotal );
	seq_printf( seq, "TotalOutOctets:%llu\n", outTotal );
//printk("\n");//Ren.test

	return 0;
}

#define MAX_LINE 1
static void* eth_seq_start(struct seq_file *s, loff_t *pos)
{
	uint32_t *lines;

	if (*pos >= MAX_LINE) {
		return NULL; // no more data to read
	}

	lines = kzalloc(sizeof(uint32_t), GFP_KERNEL);
	if (!lines) {
		return NULL;
	}

	*lines = *pos + 1;

	return lines;
}

static void* eth_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	uint32_t *lines = v;

	*pos = ++(*lines);

	if (*pos >= MAX_LINE) {
		return NULL; // no more data to read
	}

	return lines;
}

static void eth_seq_stop(struct seq_file *s, void *v)
{
	kfree(v);
}

static int eth_seq_show(struct seq_file *s, void *v)
{
	getAll_rtk_port_stat(s);

	return 0;
}

static struct seq_operations eth_seq_ops = {
	.start = eth_seq_start,
	.next  = eth_seq_next,
	.stop  = eth_seq_stop,
	.show  = eth_seq_show
};

static int eth_proc_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &eth_seq_ops);
}

static struct file_operations eth_proc_ops = {
	.owner   = THIS_MODULE, // system
	.open    = eth_proc_open,
	.read    = seq_read,    // system
	.llseek  = seq_lseek,   // system
	.release = seq_release  // system
};

static int rtl8367rb_open(struct inode *inode, struct file *file)
{
	return 0;
}

static const struct file_operations rtl8367rb_fops = {
	.owner	= "rtl8367rb",
	.write = NULL,
	.read = NULL,
	.ioctl	= NULL,
	.open	= NULL,
	.release = NULL
};

static int __init rtl8367rb_init(void)
{
	struct proc_dir_entry *cmd_proc;
	struct proc_dir_entry *eth_proc;

	printk("rtl8367rb_init...\n");
	printk("switch IC = CHIP_RTL8367RB, SDK = V1.2.6\n");

	initialize_switch();

	eth_proc = create_proc_entry("tc3162/rtk_switch", 0, NULL);
	eth_proc->read_proc = rtk_switch_read_proc;
	eth_proc->write_proc = rtk_switch_write_proc;

	create_proc_read_entry("tc3162/eth_ATE_link_st", 0, NULL, eth_ATE_link_st_proc, NULL);	
	create_proc_read_entry("tc3162/eth_ATE_link_st_2", 0, NULL, eth_ATE_link_st_2_proc, NULL);	
	create_proc_read_entry("tc3162/eth_wan_link_st", 0, NULL, eth_wan_link_st_proc, NULL);	//Sam, 2012/11/1
	create_proc_read_entry("tc3162/eth_lan_link_st", 0, NULL, eth_lan_link_st_proc, NULL);

	cmd_proc = create_proc_entry("tc3162/ether_switch_traffic", 0, NULL);
	if(cmd_proc)
	{
		//create seq_file node
		cmd_proc->proc_fops =&eth_proc_ops;
	}

	return 0;
}

static void __exit rtl8367rb_cleanup(void)
{
	remove_proc_entry("tc3162/rtk_switch", 0);
	remove_proc_entry("tc3162/eth_ATE_link_st", 0);
	remove_proc_entry("tc3162/eth_wan_link_st", 0);
	remove_proc_entry("tc3162/eth_lan_link_st", 0);

	printk("RTL8367RB driver exited\n");
}

module_init(rtl8367rb_init);
module_exit(rtl8367rb_cleanup);
