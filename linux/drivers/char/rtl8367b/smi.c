#include <rtk_types.h>
#include "rtk_error.h"

#define MDC_MDIO_DUMMY_ID           0
#define MDC_MDIO_CTRL0_REG          31
#define MDC_MDIO_START_REG          29
#define MDC_MDIO_CTRL1_REG          21
#define MDC_MDIO_ADDRESS_REG        23
#define MDC_MDIO_DATA_WRITE_REG     24
#define MDC_MDIO_DATA_READ_REG      25
#define MDC_MDIO_PREAMBLE_LEN       32

#define MDC_MDIO_START_OP          0xFFFF
#define MDC_MDIO_ADDR_OP           0x000E
#define MDC_MDIO_READ_OP           0x0001
#define MDC_MDIO_WRITE_OP          0x0003

#define VPint			*(volatile unsigned int *)
#define read_reg_word(reg) 			VPint(reg)
#define write_reg_word(reg, wdata) 	VPint(reg)=wdata

#ifdef TCSUPPORT_CPU_RT63365

#define GSW_BASE     		0xBFB58000
#define GSW_CFG_BASE     	(GSW_BASE + 0x7000)
#define GSW_CFG_PIAC     	(GSW_CFG_BASE + 0x4)
#define PHY_ACS_ST		(1<<31)
#define MDIO_ST_START		(1)
#define MDIO_ST_SHIFT		(16)
#define MDIO_CMD_WRITE		(1)
#define MDIO_CMD_SHIFT		(18)
#define MDIO_PHY_ADDR_SHIFT	(20)
#define MDIO_REG_ADDR_SHIFT	(25)
#define MDIO_RW_DATA_SHIFT	(0)
#define MDIO_RW_DATA		(0xffff<<MDIO_RW_DATA_SHIFT)
#define MDIO_CMD_READ		(2)

#else

#define NEW_PHYOR_DATA		(0xffff<<0)	
#define NEW_PHYOR_REG_SHIFT	16
#define NEW_PHYOR_ADDR_SHIFT	21
#define NEW_PHYOR_OP_SHIFT	26
#define NEW_PHYOR_OP_WR		(1)
#define NEW_PHYOR_OP_RD		(2)
#define NEW_PHYOR_ST_SHIFT	28
#define NEW_PHYOR_ST_IEEE_NEW	(0)
#define NEW_PHYOR_ST_IEEE_OLD	(1)
#define NEW_PHYOR_BUSY		(1<<31)

#define CR_GMAC_BASE     	0xBFB50000
#define CR_GMAC_PHYOR    	(0x108 | CR_GMAC_BASE)

#endif

void sendMdioRtk(unsigned long enetPhyAddr, unsigned long phyReg, unsigned long miiData)
{
	unsigned long reg;
	unsigned long cnt=1000;	
#ifdef TCSUPPORT_CPU_RT63365
	do {
		reg=read_reg_word (GSW_CFG_PIAC);
		cnt--;
	} while (reg & PHY_ACS_ST);

	reg = PHY_ACS_ST | (MDIO_ST_START << MDIO_ST_SHIFT) | (MDIO_CMD_WRITE<<MDIO_CMD_SHIFT) | 
		(enetPhyAddr << MDIO_PHY_ADDR_SHIFT) | (phyReg << MDIO_REG_ADDR_SHIFT) | 
		(miiData & MDIO_RW_DATA);
	write_reg_word (GSW_CFG_PIAC, reg);

	cnt = 1000;
	do {
		//pause(1);
		reg=read_reg_word (GSW_CFG_PIAC);
		cnt--;
	} while (reg & PHY_ACS_ST);
#else
	reg = (miiData & NEW_PHYOR_DATA) | 
			(phyReg << NEW_PHYOR_REG_SHIFT) | (enetPhyAddr << NEW_PHYOR_ADDR_SHIFT) | 
			(NEW_PHYOR_OP_WR << NEW_PHYOR_OP_SHIFT) | (NEW_PHYOR_ST_IEEE_OLD << NEW_PHYOR_ST_SHIFT) |
			NEW_PHYOR_BUSY;
	write_reg_word (CR_GMAC_PHYOR, reg);
	
	do {
		//pause(1);
		reg=read_reg_word (CR_GMAC_PHYOR);
		cnt--;
	} while (reg & NEW_PHYOR_BUSY);
#endif
}

unsigned long getMdioRtk(unsigned long enetPhyAddr, unsigned long phyReg)
{
	unsigned long reg;
	unsigned long cnt=1000;
#ifdef TCSUPPORT_CPU_RT63365
	do {
		reg=read_reg_word (GSW_CFG_PIAC);
		cnt--;
	} while (reg & PHY_ACS_ST);

	reg = PHY_ACS_ST | (MDIO_ST_START << MDIO_ST_SHIFT) | (MDIO_CMD_READ<<MDIO_CMD_SHIFT) | 
		(enetPhyAddr << MDIO_PHY_ADDR_SHIFT) | (phyReg << MDIO_REG_ADDR_SHIFT);
	write_reg_word (GSW_CFG_PIAC, reg);

	cnt = 1000;
	do {
		//pause(1);
		reg=read_reg_word (GSW_CFG_PIAC);
		cnt--;
	} while (reg & PHY_ACS_ST);
	reg = reg & MDIO_RW_DATA;

	return reg;
#else
	reg = (phyReg << NEW_PHYOR_REG_SHIFT) | (enetPhyAddr << NEW_PHYOR_ADDR_SHIFT) | 
			(NEW_PHYOR_OP_RD << NEW_PHYOR_OP_SHIFT) | (NEW_PHYOR_ST_IEEE_OLD << NEW_PHYOR_ST_SHIFT) |
			NEW_PHYOR_BUSY;
	write_reg_word (CR_GMAC_PHYOR, reg);

	do {
		//pause(1);
		reg=read_reg_word (CR_GMAC_PHYOR);
		cnt--;
	} while (reg & NEW_PHYOR_BUSY);

	return (reg & NEW_PHYOR_DATA);
#endif
}

void WriteRtk(unsigned long phyReg, unsigned long miiData)
{
	unsigned long enetPhyAddr = 0;
	sendMdioRtk(enetPhyAddr,MDC_MDIO_START_REG,MDC_MDIO_START_OP);
	sendMdioRtk(enetPhyAddr,MDC_MDIO_CTRL0_REG,MDC_MDIO_ADDR_OP);
	sendMdioRtk(enetPhyAddr,MDC_MDIO_START_REG,MDC_MDIO_START_OP);
	sendMdioRtk(enetPhyAddr,MDC_MDIO_ADDRESS_REG,phyReg);
	sendMdioRtk(enetPhyAddr,MDC_MDIO_START_REG,MDC_MDIO_START_OP);
	sendMdioRtk(enetPhyAddr,MDC_MDIO_DATA_WRITE_REG,miiData);
	sendMdioRtk(enetPhyAddr,MDC_MDIO_START_REG,MDC_MDIO_START_OP);
	sendMdioRtk(enetPhyAddr,MDC_MDIO_CTRL1_REG,MDC_MDIO_WRITE_OP);
}

unsigned long ReadRtk(unsigned long phyReg)
{
	unsigned long enetPhyAddr = 0;
	sendMdioRtk(enetPhyAddr,MDC_MDIO_START_REG,MDC_MDIO_START_OP);
	sendMdioRtk(enetPhyAddr,MDC_MDIO_CTRL0_REG,MDC_MDIO_ADDR_OP);
	sendMdioRtk(enetPhyAddr,MDC_MDIO_START_REG,MDC_MDIO_START_OP);
	sendMdioRtk(enetPhyAddr,MDC_MDIO_ADDRESS_REG,phyReg);
	sendMdioRtk(enetPhyAddr,MDC_MDIO_START_REG,MDC_MDIO_START_OP);
	sendMdioRtk(enetPhyAddr,MDC_MDIO_CTRL1_REG,MDC_MDIO_READ_OP);
	sendMdioRtk(enetPhyAddr,MDC_MDIO_START_REG,MDC_MDIO_START_OP);
	return getMdioRtk(enetPhyAddr,MDC_MDIO_DATA_READ_REG);
}


rtk_int32 smi_read(rtk_uint32 mAddrs, rtk_uint32 *rData)
{
	*rData = ReadRtk(mAddrs);
	return RT_ERR_OK;
}

rtk_int32 smi_write(rtk_uint32 mAddrs, rtk_uint32 rData)
{
	WriteRtk(mAddrs,rData);
	return RT_ERR_OK;
}



