/**
* \file bz_riscv.c
* \brief Contains functions to support B20 Riscv Write and Read functions
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/
#include "bz_riscv.h"
#include "firmware.h"
#include "bz_plat_hal.h"
#include "bz_reg_dig.h"
#include "bz_error.h"
#include "svn_version.h"

#include <string.h>

uint32_t BZ_clearStatus(BzDevice_t *hw_priv)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halStatus = BZHAL_OK;
	
	halStatus=BZ_spiWriteReg(hw_priv,DOCMD_STATUS,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	
	return bzRetAct;
}
uint32_t BZ_writeFirmware(BzDevice_t *hw_priv,uint16_t addr, uint32_t data)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    bzHalErr halStatus = BZHAL_OK;
	
	halStatus=BZ_spiWriteReg(hw_priv,addr,data);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;	
}


/*Read sram process*/
uint32_t BZ_spiReadMem(BzDevice_t *hw_priv,uint32_t addr,uint32_t *regdata)
{		
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    bzHalErr halStatus = BZHAL_OK;

	if(addr & 0x20000000){
		BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,0,3,1);
	}else{
		BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,1,3,1);
	}
	
	 /*read sram status*/
	halStatus=BZ_spiWriteReg(hw_priv,REG_IP_SRAM_ACC_ADDR,addr);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	 
	/*2.read sram data*/
	halStatus=BZ_spiReadReg(hw_priv,SRAM_READ_DATA,regdata);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	return (uint32_t)bzRetAct;	
}
/*Write sram process*/
uint32_t BZ_spiWriteMem(BzDevice_t *hw_priv,uint32_t addr,uint32_t data)
{		
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    bzHalErr halStatus = BZHAL_OK;
	
	halStatus=BZ_spiWriteReg(hw_priv,REG_IP_SRAM_ACC_WDATA,data);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*Enable write func*/
	addr|=0x80000000;
	//printk("%x = %x\n",addr,data);
	halStatus=BZ_spiWriteReg(hw_priv,REG_IP_SRAM_ACC_ADDR,addr);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;	
}
static uint32_t bz_spiGetRiscvLog(BzDevice_t *hw_priv,uint32_t addr,uint32_t *data)
{		
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    bzHalErr halStatus = BZHAL_OK;  
	
	 /*read sram status*/
	halStatus=BZ_spiWriteReg(hw_priv,REG_IP_SRAM_ACC_ADDR,addr);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*2.read sram data*/
	halStatus=BZ_spiReadReg(hw_priv,SRAM_READ_DATA,data);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	return (uint32_t)bzRetAct;	
}
/*Get PC */
uint32_t BZ_spiGetRiscvPc(BzDevice_t *hw_priv,uint32_t addr,uint32_t pc_cnt)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    bzHalErr halStatus = BZHAL_OK;  
	uint32_t pc;
	uint32_t debug_ram;
	
	halStatus=BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,0xf,31,28);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*1,set pc cnt*/
	halStatus=BZ_spiWriteReg(hw_priv,REG_PC_COUNT,pc_cnt);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*2.set pc store sram addr*/
	debug_ram=(addr|0x80000000);
	halStatus=BZ_spiWriteReg(hw_priv,REG_PC_BASE_ADDR,debug_ram);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	bzRetAct=BZ_spiReadMem(hw_priv,addr,&pc);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;	
}
uint32_t bz_spiInterrupt(BzDevice_t *hw_priv,uint32_t writeData)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    bzHalErr halStatus = BZHAL_OK;  
	
	/*request isr*/  
	halStatus=BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,writeData,20,20);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	BZ_mdelay(100);
	halStatus=BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,!writeData,20,20);			
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*cancel isr*/ 
	halStatus=BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,writeData,24,24);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halStatus=BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,!writeData,24,24);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;	
}
uint32_t BZ_spiWaitCmdConfirm(BzDevice_t *hw_priv)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    bzHalErr halStatus = BZHAL_OK;
    uint32_t readata;
    uint32_t uiTry = WIAT_TIMEOUT; 	
	while(uiTry > 0){ 
		halStatus=BZ_spiReadReg(hw_priv,RECV_CMD_CONFIRM,&readata);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);	
		if(readata==HI_STATUS_SUCCESS){ 
			break; 
		}
        uiTry--;
	} 
    if(uiTry <= 0){
		BZHAL_Printf("ERROR:%s Trytimeout!\n", __func__);
    }

	return (uint32_t)bzRetAct;	
}
uint32_t bz_getFirmwareVersion(BzDevice_t *hw_priv)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	BzData DataInfo;
	BzMem *pMsg;
	
	pMsg=&DataInfo.u.Mmsg;
	DataInfo.CmdType=0xff;
	
	pMsg->Cmd=BZ_READ_MEM_OPCODE;
	pMsg->Msg.Id=HI_MSG_TO_READFIRDY_MSGID;
	pMsg->Msg.Lenth=sizeof(bz_caps);
	pMsg->Msg.addr=BZ_ADDR_FW_VERSION;
	bzRetAct=BZ_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_READ_CMD);
	IF_ERR_RETURN(bzRetAct);

	BZ_Memset(&hw_priv->caps,0x0,sizeof(bz_caps));
	BZ_Memcpy(&hw_priv->caps,(bz_caps*)&pMsg->Msg.data[0],sizeof(bz_caps));

	BZHAL_Printf("\n*************Firmware BUILD INFO*************\n"
		   "Compiler is %s\n"
		   "Current Svn version is %u\n"
		   "Build Time is %s\n"
		   "****************************************\n\n", 
		   hw_priv->caps.firmwareCompiler, (int)hw_priv->caps.firmwareCurrentSvnversion, hw_priv->caps.firmwareBuildTime);
	return (uint32_t)bzRetAct;
}
static short bz_calCheckSum(void *data,int len)
{
	unsigned int acc;
	unsigned char src;
	char *octetptr;
	unsigned char ucXor=MAGIC;
	acc=0;
	
	octetptr=(char*)data;
	while(len>0){
		src=(*octetptr^ucXor);
		octetptr++;
		acc+=src;
		len--;
	}
	return acc;
}

static uint32_t bz_downLoadFirmware(BzDevice_t *hw_priv, uint8_t *data,uint32_t size,uint16_t addr)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halStatus=BZHAL_OK;
	uint32_t DatLen = 0;
	uint32_t Data;
	
	for (DatLen = 0; DatLen < size ;DatLen += 4) {
		/*Little endian */
		Data=((data[DatLen]<<24)|data[DatLen+1]<<16|data[DatLen+2]<<8|data[DatLen+3]);
		bzRetAct = BZ_writeFirmware(hw_priv,addr,Data);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);	
	} 
	return (uint32_t)bzRetAct;	
}
uint32_t bz_downLoadItcmBefore(BzDevice_t *hw_priv,int writeData,int dtcmOffset)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halStatus=BZHAL_OK;
	
	halStatus=BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,writeData,0,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	

	halStatus=BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,writeData,4,4);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	
	
	return (uint32_t)bzRetAct;	
}
uint32_t bz_downLoadDtcmBefore(BzDevice_t *hw_priv,int writeData,int dtcmOffset)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halStatus=BZHAL_OK;
	
	halStatus=BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,writeData,0,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	
	
	halStatus=BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,writeData,4,4);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	
	
	halStatus=BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,dtcmOffset,3,1);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	

	halStatus = BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,!writeData,8,8);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	

	halStatus = BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,!writeData,4,4);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	

	return (uint32_t)bzRetAct;	
}
static uint32_t bz_getFirmwareInfo(BzDevice_t *hw_priv,struct firmware_B20 *fw_B20)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halStatus=BZHAL_OK;
	uint16_t chcksum=0;
	uint32_t value=0;
	
	fw_B20->hdr.itcmLen=(fw_code[8]<<24|fw_code[9]<<16|fw_code[10]<<8|fw_code[11]);
	fw_B20->hdr.itcmAddr=(fw_code[0]<<24|fw_code[1]<<16|fw_code[2]<<8|fw_code[3]);
	fw_B20->fw_iccm=&fw_code[fw_B20->hdr.itcmAddr];
	
	chcksum+=bz_calCheckSum(fw_B20->fw_iccm,fw_B20->hdr.itcmLen);
	fw_B20->hdr.dtcmLen=(fw_code[24]<<24|fw_code[25]<<16|fw_code[26]<<8|fw_code[27]);
	fw_B20->hdr.dtcmAddr=(fw_code[16]<<24|fw_code[17]<<16|fw_code[18]<<8|fw_code[19]);
	fw_B20->fw_dccm=&fw_code[fw_B20->hdr.dtcmAddr];
	chcksum+=bz_calCheckSum(fw_B20->fw_dccm,fw_B20->hdr.dtcmLen);
	
	value=(MAGIC<<16|chcksum);
	halStatus=BZ_spiWriteReg(hw_priv,REG_IP_CHKSUM,value);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	

	/*Enalbe Checksum */
	halStatus=BZ_spiWriteField(hw_priv, REG_IP_SRAM_CTRL, 1, 16, 16);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	
	
	BZHAL_Printf("ITCM & DTCM len  %d  %d \r\n",(int)fw_B20->hdr.itcmLen,(int)fw_B20->hdr.dtcmLen);
	BZHAL_Printf("ITCM & DTCM Addr  %d  %d \r\n",(int)fw_B20->hdr.itcmAddr,(int)fw_B20->hdr.dtcmAddr);
	
	return (uint32_t)bzRetAct;		
}
static uint32_t bz_downLoadItcm(BzDevice_t *hw_priv,struct firmware_B20 *fw_B20)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halStatus=BZHAL_OK;
	
	halStatus = bz_downLoadFirmware(hw_priv,fw_B20->fw_iccm,fw_B20->hdr.itcmLen,DOWNLOAD_ITCM_ADDR);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	

	return (uint32_t)bzRetAct;		
}

static uint32_t bz_downLoadDtcm(BzDevice_t *hw_priv,struct firmware_B20 *fw_B20)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	
	bzRetAct = bz_downLoadFirmware(hw_priv,fw_B20->fw_dccm,fw_B20->hdr.dtcmLen,DOWNLOAD_DTCM_ADDR);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;		
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static uint32_t bz_downLoadAfter(BzDevice_t *hw_priv,int writeData)
{ 
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halStatus=BZHAL_OK;
	
	halStatus = BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,writeData,8,8);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	

	halStatus = BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,writeData,12,12);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	

	halStatus=BZ_spiResetChip(hw_priv,RESET_BY_ISR);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	
	
	return (uint32_t)bzRetAct;		
}

uint32_t BZ_loadFirmware(BzDevice_t *hw_priv)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	struct firmware_B20 fw_B20;
	
	BZ_Memset(&fw_B20,0,sizeof(struct firmware_B20));
	bzRetAct=bz_getFirmwareInfo(hw_priv,&fw_B20);
	IF_ERR_RETURN(bzRetAct);
	
	bzRetAct = bz_downLoadDtcmBefore(hw_priv,1,0);
	IF_ERR_RETURN(bzRetAct);
	
	bzRetAct = bz_downLoadItcm(hw_priv,&fw_B20);
	IF_ERR_RETURN(bzRetAct);
	
	bzRetAct = bz_downLoadDtcmBefore(hw_priv,1,2);
	IF_ERR_RETURN(bzRetAct);
	
	bzRetAct = bz_downLoadDtcm(hw_priv,&fw_B20);
	IF_ERR_RETURN(bzRetAct);
	
	bzRetAct = bz_downLoadAfter(hw_priv,1);
	IF_ERR_RETURN(bzRetAct);
	
	BZHAL_Printf("FIRMWARE DOWNLOAD SUCCESS\n");
	return (uint32_t)bzRetAct;		
}

uint32_t BZ_writeRiscvMem(BzDevice_t *hw_priv,uint8_t *data,uint32_t addr,uint32_t size,uint32_t msgId,uint32_t offset)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	uint32_t DatLen = 0;
	uint32_t Data;
	
	switch (msgId){
		case HI_MSG_TO_FILTER_MSGID:
			addr = FILTER_MSG_ADDR+offset;
			break;
		case HI_MSG_TO_PROFILE_MSGID:
			addr=PROFILE_MSG_ADDR;
			break;
		case HI_MSG_TO_RFFRQ_MSGID:
			addr=RFFRQ_MSG_ADDR;
			break;		
		case HI_MSG_TO_AUXFRQ_MSGID:
			addr=AUXFRQ_MSG_ADDR;
			break;
		case HI_MSG_TO_J204B_MSGID:
			addr=J204B_MSG_ADDR;
			break;			
		default:
			break;
	}
	for(DatLen=0;DatLen<size;DatLen+=4){
		Data=((data[DatLen])|data[DatLen+1]<<8|data[DatLen+2]<<16|data[DatLen+3]<<24);
		bzRetAct=BZ_spiWriteMem(hw_priv,addr,Data);	
		IF_ERR_RETURN(bzRetAct);
		addr+=4;
	}
	return (uint32_t)bzRetAct;		
}
uint32_t BZ_readRiscvMem(BzDevice_t *hw_priv,uint32_t *data,uint32_t addr,uint32_t size)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	int index;
	
	for(index=0;index<size;index+=4){
		bzRetAct=BZ_spiReadMem(hw_priv,addr,data);
		IF_ERR_RETURN(bzRetAct);
		addr+=4;
		data++;
	}
	return (uint32_t)bzRetAct;		
}

uint32_t BZ_sendRiscvCmd(BzDevice_t *priv,void *UserData, unsigned int Command)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    bzHalErr halStatus = BZHAL_OK;
	int MemData=0;
	BzMsg *pMmsg=NULL;
	
	BzData *DataInfo = (BzData*)UserData;	
	if(IOCTL_WRITE_CMD==Command)
	{
		switch(DataInfo->u.Tx.Cmd){
			case BZ_ABORT_OPCODE:				
			case BZ_RUNN_INIT_OPCODE:
			case BZ_RADIO_ON_OPCODE:
				{
					halStatus=BZ_spiWriteReg(priv,SEND_CMD_TYPE,DataInfo->u.Tx.Cmd);
					bzRetAct=BZ_ApiErrHandler(priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
					IF_ERR_RETURN(bzRetAct);	
					
					halStatus=BZ_spiWriteReg(priv,SEND_CMD_DATA,DataInfo->u.Tx.TxData.value);
					bzRetAct=BZ_ApiErrHandler(priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
					IF_ERR_RETURN(bzRetAct);	
					if(BZ_RUNN_INIT_OPCODE == DataInfo->u.Tx.Cmd)
					{
						priv->currentCalMask = DataInfo->u.Tx.TxData.value;
					}
				}
				break;
			case BZ_WRITE_MEM_OPCODE:
				pMmsg=&DataInfo->u.Mmsg.Msg;
				MemData=(pMmsg->Id<<16|pMmsg->Lenth);
				halStatus=BZ_spiWriteReg(priv,SEND_CMD_TYPE,DataInfo->u.Mmsg.Cmd);
				bzRetAct=BZ_ApiErrHandler(priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);	
				
				halStatus=BZ_spiWriteReg(priv,SEND_CMD_DATA,MemData);
				bzRetAct=BZ_ApiErrHandler(priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);	
				
				halStatus=BZ_writeRiscvMem(priv,&pMmsg->data[0],DOWNLOAD_DTCM_ADDR,pMmsg->Lenth,pMmsg->Id,pMmsg->offset);
				bzRetAct=BZ_ApiErrHandler(priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);	
				break;
			case BZ_SET_OPCODE:
				halStatus=BZ_spiWriteReg(priv,SEND_CMD_TYPE,DataInfo->u.Tx.Cmd);
				bzRetAct=BZ_ApiErrHandler(priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);	
			
				halStatus=BZ_spiWriteReg(priv,SEND_CMD_DATA,DataInfo->u.Tx.TxData.value);
				bzRetAct=BZ_ApiErrHandler(priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);	
				break;
		}
		bzRetAct=bz_spiInterrupt(priv,1);
		IF_ERR_RETURN(bzRetAct);	
		/*Read Confirm From Chip*/
		bzRetAct=BZ_spiWaitCmdConfirm(priv);
		IF_ERR_RETURN(bzRetAct);
	}
	else if(IOCTL_READ_CMD == Command)
	{
		switch(DataInfo->u.Tx.Cmd){
			case BZ_READ_MEM_OPCODE:
				pMmsg=&DataInfo->u.Mmsg.Msg;
				MemData=(pMmsg->Id<<16|pMmsg->Lenth);
				halStatus=BZ_spiWriteReg(priv,SEND_CMD_TYPE,DataInfo->u.Mmsg.Cmd);
				bzRetAct=BZ_ApiErrHandler(priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);	
				
				halStatus=BZ_spiWriteReg(priv,SEND_CMD_DATA,MemData);
				bzRetAct=BZ_ApiErrHandler(priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);	
				
				bzRetAct=BZ_readRiscvMem(priv,(uint32_t *)(&pMmsg->data[0]),pMmsg->addr,pMmsg->Lenth);
				IF_ERR_RETURN(bzRetAct);
				break;			
			case BZ_GET_OPCODE:
				halStatus=BZ_spiWriteReg(priv,SEND_CMD_TYPE,DataInfo->u.Tx.Cmd);
				bzRetAct=BZ_ApiErrHandler(priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);	
			
				halStatus=BZ_spiWriteReg(priv,SEND_CMD_DATA,DataInfo->u.Rx.RxData.value);
				bzRetAct=BZ_ApiErrHandler(priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);	
				break;
			case BZ_GET_MEMORY_DATA:
				bzRetAct=BZ_readRiscvMem(priv,(uint32_t *)((uint64_t)DataInfo->u.Rx.RxData.lo),DataInfo->u.Rx.RxData.addr,DataInfo->u.Rx.RxData.value);
				IF_ERR_RETURN(bzRetAct);
				break;
			default:
				break;
		}
		bzRetAct=bz_spiInterrupt(priv,1);
		IF_ERR_RETURN(bzRetAct);
		/*Read Confirm From Chip*/
		bzRetAct=BZ_spiWaitCmdConfirm(priv);
		IF_ERR_RETURN(bzRetAct);
	}
	return (uint32_t)bzRetAct;		
}


uint32_t BZ_writeRiscvProfile(BzDevice_t *hw_priv,BzInit_t *Init)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    uint32_t i = 0,j=0;
	bzInfo_t *pStateInfo;
	BzData DataInfo;
	BzMem *pMsg;
	
	pStateInfo= &hw_priv->BzState;
	BZ_Memset(&DataInfo,0,sizeof(BzData));

	pMsg=&DataInfo.u.Mmsg;
	DataInfo.CmdType=0xff;
	pMsg->Cmd=BZ_WRITE_MEM_OPCODE;
	pMsg->Msg.Id=HI_MSG_TO_PROFILE_MSGID;
	pMsg->Msg.Lenth=HI_MSG_TO_PROFILE_MSGLEN;
    for (i = 0; i < 4; i++)
    {
        pMsg->Msg.data[i] = (uint8_t)(((Init->clocks.deviceClock_kHz) >> (i * 8)) & 0x000000FF);
    }
    for (i = 4; i < 8; i++)
    {
        pMsg->Msg.data[i] = (uint8_t)(((Init->clocks.rfpll_frequency_Hz) >> ((i - 4) * 8)) & 0x000000FF);
    }	
    pMsg->Msg.data[8] = ((Init->clocks.rfPllUseExternalLo > 0) ? 1 : 0); 
	
    pMsg->Msg.data[9] = (Init->clocks.rfPllPhaseSyncMode & 0x03); 
	pMsg->Msg.data[10]=0;//reserved
	pMsg->Msg.data[11]=0;//reserved
    if (pStateInfo->profilesValid & TX_PROFILE_VALID)
    {
		/* start of Tx profile data */
	    pMsg->Msg.data[12] = Init->tx.txProfile.txFirBypass;
	    pMsg->Msg.data[13] = Init->tx.txProfile.txFirInterpolation;
	    pMsg->Msg.data[14] = Init->tx.txProfile.thb1Interpolation;
	    pMsg->Msg.data[15] = Init->tx.txProfile.thb2Interpolation;
	    pMsg->Msg.data[16] = Init->tx.txProfile.thb3Interpolation;
		pMsg->Msg.data[17]=0;//reserved
		pMsg->Msg.data[18]=0;//reserved
		pMsg->Msg.data[19]=0;//reserved
	    for (i = 20; i < 24; i++)
	    {
	        pMsg->Msg.data[i] = (uint8_t)(((Init->tx.txProfile.txInputRate_kHz) >> ((i - 20) * 8)) & 0x000000FF);
	    }

	    for (i = 24; i < 28; i++)
	    {
	        pMsg->Msg.data[i] = (uint8_t)(((Init->tx.txProfile.primarySigBandwidth_Hz) >> ((i - 24) * 8)) & 0x000000FF);
	    }

	    for (i = 28; i < 32; i++)
	    {
	        pMsg->Msg.data[i] = (uint8_t)(((Init->tx.txProfile.rfBandwidth_Hz) >> ((i - 28) * 8)) & 0x000000FF);
	    }
	    for (i = 32; i < 36; i++)
	    {
	        pMsg->Msg.data[i] = (uint8_t)(((Init->tx.txProfile.txBbf3dBCorner_kHz) >> ((i - 32) * 8)) & 0x000000FF);
	    }
	}else{
		/* clear pMsg->Msg.data[12] to pMsg->Msg.data[32] */
		for (j = 12; j < 36; j++)
		{
			pMsg->Msg.data[j] = 0;
		}
	}	
    if (pStateInfo->profilesValid & RX_PROFILE_VALID)
    {
        /* start of Rx profile data */
        pMsg->Msg.data[36] = Init->rx.rxProfile.rxFirBypass;
        pMsg->Msg.data[37] = Init->rx.rxProfile.rxFirDecimation;
        pMsg->Msg.data[38] = Init->rx.rxProfile.rhb1Decimation;
		pMsg->Msg.data[39]=0;//pad

        for (i = 40; i < 44; i++)
        {
            pMsg->Msg.data[i] = (uint8_t)(((Init->rx.rxProfile.rxOutputRate_kHz) >> ((i - 40) * 8)) & 0x000000FF);
        }

        for (i = 44; i < 48; i++)
        {
            /* sig bw placeholder */
            pMsg->Msg.data[i] = (uint8_t)(((Init->rx.rxProfile.rfBandwidth_Hz) >> ((i - 44) * 8)) & 0x000000FF);
        }
        for (i = 48; i < 52; i++)
        {
            pMsg->Msg.data[i] = (uint8_t)(((Init->rx.rxProfile.rxBbf3dBCorner_kHz) >> ((i - 48) * 8)) & 0x000000FF);
        }
    }
    else
    {
        /* clear pMsg->Msg.data[32] to pMsg->Msg.data[48] */
        for (j = 36; j < 52; j++)
        {
            pMsg->Msg.data[j] = 0;
        }
    }
    if (pStateInfo->profilesValid & ORX_PROFILE_VALID)
    {
        /* start of ObsRx profile data */
        pMsg->Msg.data[52] = Init->obsRx.orxProfile.orxFirBypass;
        pMsg->Msg.data[53] = Init->obsRx.orxProfile.orxFirDecimation;
        pMsg->Msg.data[54] = 0;//pad
		pMsg->Msg.data[55] = 0;//pad

        for (i = 56; i < 60; i++)
        {
            pMsg->Msg.data[i] = (uint8_t)(((Init->obsRx.orxProfile.orxOutputRate_kHz) >> ((i - 56) * 8)) & 0x000000FF);
        }

        for (i = 60; i < 64; i++)
        {
            /* sig bw placeholder */
            pMsg->Msg.data[i] = (uint8_t)(((Init->obsRx.orxProfile.rfBandwidth_Hz) >> ((i - 60) * 8)) & 0x000000FF);
        }
        for (i = 64; i < 68; i++)
        {
            pMsg->Msg.data[i] = (uint8_t)(((Init->obsRx.orxProfile.rxBbf3dBCorner_kHz) >> ((i - 64) * 8)) & 0x000000FF);
        }
    }
    else
    {
        /* clear pMsg->Msg.data[48] to pMsg->Msg.data[64] */
        for (j = 52; j < 68; j++)
        {
            pMsg->Msg.data[j] = 0;
        }
    }
	bzRetAct=BZ_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);	
	return (uint32_t)bzRetAct;		
}

uint32_t BZ_readRiscvProfile(BzDevice_t *hw_priv,BzInit_t *Init)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	
    uint32_t i = 0;
	bzInfo_t *pStateInfo;
	BzData DataInfo;
	BzMem *pMsg;
	pStateInfo=&hw_priv->BzState;

	pMsg=&DataInfo.u.Mmsg;

	DataInfo.CmdType=0xff;
	pMsg->Cmd=BZ_READ_MEM_OPCODE;
	pMsg->Msg.Id=HI_MSG_TO_PROFILE_MSGID;
	pMsg->Msg.Lenth=HI_MSG_TO_PROFILE_MSGLEN;
	pMsg->Msg.addr=PROFILE_MSG_ADDR;
	bzRetAct=BZ_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_READ_CMD);
	IF_ERR_RETURN(bzRetAct);

	BZ_Memset(Init, 0, sizeof(BzInit_t));
    for (i = 0; i < 4; i++)
    {
		Init->clocks.deviceClock_kHz |= (pMsg->Msg.data[i]<<(i * 8));
    }

	
    for (i = 4; i < 8; i++)
    {
		Init->clocks.rfpll_frequency_Hz |= (pMsg->Msg.data[i]) << ((i - 4) * 8);
    }

	Init->clocks.rfPllUseExternalLo = pMsg->Msg.data[8];
	Init->clocks.rfPllPhaseSyncMode = (pMsg->Msg.data[9] & 0x03);

	
	//pMsg->Msg.data[10]=0;//reserved
	//pMsg->Msg.data[11]=0;//reserved
    if (pStateInfo->profilesValid & TX_PROFILE_VALID)
    {
		// start of Tx profile data 
		Init->tx.txProfile.txFirBypass = pMsg->Msg.data[12];
		Init->tx.txProfile.txFirInterpolation = pMsg->Msg.data[13];
		Init->tx.txProfile.thb1Interpolation = pMsg->Msg.data[14];
	    Init->tx.txProfile.thb2Interpolation = pMsg->Msg.data[15];
	    Init->tx.txProfile.thb3Interpolation = pMsg->Msg.data[16];
		//pMsg->Msg.data[17]=0;//reserved
		//pMsg->Msg.data[18]=0;//reserved
		//pMsg->Msg.data[19]=0;//reserved
	    for (i = 20; i < 24; i++)
	    {
			Init->tx.txProfile.txInputRate_kHz |= (pMsg->Msg.data[i]) << ((i - 20) * 8);
	    }		

		for (i = 24; i < 28; i++)
	    {
			Init->tx.txProfile.primarySigBandwidth_Hz |= (pMsg->Msg.data[i]) << ((i - 24) * 8);
	    }

		for (i = 28; i < 32; i++)
		{
			Init->tx.txProfile.rfBandwidth_Hz |= (pMsg->Msg.data[i]) << ((i - 28) * 8);
		}
		
		for (i = 32; i < 36; i++)
	    {
			Init->tx.txProfile.txBbf3dBCorner_kHz |= (pMsg->Msg.data[i]) << ((i - 32) * 8);
	    }

		
    }	
    if (pStateInfo->profilesValid & RX_PROFILE_VALID)
    {
        // start of Rx profile data 
        Init->rx.rxProfile.rxFirBypass = pMsg->Msg.data[36];
        Init->rx.rxProfile.rxFirDecimation = pMsg->Msg.data[37];
        Init->rx.rxProfile.rhb1Decimation = pMsg->Msg.data[38];
		//pMsg->Msg.data[39]=0;//pad

        for (i = 40; i < 44; i++)
        {
			Init->rx.rxProfile.rxOutputRate_kHz |= (pMsg->Msg.data[i]) << ((i - 40) * 8);
        }

        for (i = 44; i < 48; i++)
        {
            // sig bw placeholder 
			Init->rx.rxProfile.rfBandwidth_Hz |= (pMsg->Msg.data[i]) << ((i - 44) * 8);
        }
		
        for (i = 48; i < 52; i++)
        {
			Init->rx.rxProfile.rxBbf3dBCorner_kHz |= (pMsg->Msg.data[i]) << ((i - 48) * 8);
        }
    }
   
    if (pStateInfo->profilesValid & ORX_PROFILE_VALID)
    {
        // start of ObsRx profile data 
        Init->obsRx.orxProfile.orxFirBypass = pMsg->Msg.data[52];
        Init->obsRx.orxProfile.orxFirDecimation = pMsg->Msg.data[53];
        //pMsg->Msg.data[50] = 0;//pad
		//pMsg->Msg.data[51] = 0;//pad

        for (i = 56; i < 60; i++)
        {
			Init->obsRx.orxProfile.orxOutputRate_kHz |= (pMsg->Msg.data[i]) << ((i - 56) * 8);
        }

        for (i = 60; i < 64; i++)
        {
            /// sig bw placeholder 
			Init->obsRx.orxProfile.rfBandwidth_Hz |= (pMsg->Msg.data[i]) << ((i - 60) * 8);
        }
        for (i = 64; i < 68; i++)
        {
			Init->obsRx.orxProfile.rxBbf3dBCorner_kHz |= (pMsg->Msg.data[i]) << ((i - 64) * 8);
        }
    }
	return (uint32_t)bzRetAct;		
}
uint32_t  BZ_waitFirmwareReady(BzDevice_t *hw_priv, uint32_t timeout)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    uint32_t uiTry = WIAT_TIMEOUT;	
	/*Wait for Riscv is already running*/
    while(uiTry > 0){
		BZ_mdelay(timeout);
		bzRetAct=BZ_spiGetCmdStatus(hw_priv); 
		IF_ERR_RETURN(bzRetAct);	
		if(hw_priv->caps.firmwareReady==1){
			BZ_clearStatus(hw_priv);
			hw_priv->caps.firmwareReady=0;	
			break;
		}
        uiTry--;
	}
    if(uiTry <= 0){
		BZHAL_Printf("ERROR:%s Trytimeout!\n", __func__);
    }
	return (uint32_t)bzRetAct;		
}
uint32_t  BZ_waitProfileDone(BzDevice_t *hw_priv, uint32_t timeout)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    uint32_t uiTry = WIAT_TIMEOUT;	
	/*Wait for profile is already done*/
    while(uiTry > 0){
		BZ_mdelay(timeout);
		bzRetAct=BZ_spiGetCmdStatus(hw_priv);
		IF_ERR_RETURN(bzRetAct);
		if(hw_priv->profile_done==1){
			BZ_clearStatus(hw_priv);
			hw_priv->profile_done=0;
			break;
		}
        uiTry--;
	}
    if(uiTry <= 0){
		BZHAL_Printf("ERROR:%s Trytimeout!\n", __func__);
    }
	return (uint32_t)bzRetAct;		
}

uint32_t BZ_GetRiscvTraceEvent(BzDevice_t *hw_priv)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	BzData DataInfo;
	BzMem *pMsg = NULL;
	struct elog_s *elog = NULL;
	unsigned short newTraceEventId;
	uint32_t elogCnt;
	pMsg=&DataInfo.u.Mmsg;
	DataInfo.CmdType=0xff;
	
	pMsg->Cmd=BZ_READ_MEM_OPCODE;
	pMsg->Msg.Id=HI_MSG_TO_CALSTATUS_MSGID;
	pMsg->Msg.Lenth=CALIBRITION_SRAM_LEN;/*CalibrationInfo Length*/
	pMsg->Msg.addr=CALIBRITION_SRAM_ADDR;/*CalibrationInfo ram*/
	
	bzRetAct=BZ_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_READ_CMD);
	if(bzRetAct!=BZ_NO_ACTION){
		return BZ_ERR_CHECK_PARAM;
	}
	
	elog = (struct elog_s *)(pMsg->Msg.data);
	for (newTraceEventId = elog->index - 1, elogCnt = 0;newTraceEventId--;elogCnt++)
	{
		if (newTraceEventId >= elog->len)
		{
			newTraceEventId = elog->len - 1;
		}		
		BZHAL_Printf("TraceEventId %d  = 0x%x\n",newTraceEventId,((elog->e_id[newTraceEventId].id<<24)|(elog->e_id[newTraceEventId].data & 0xffff)));
	}
	return bzRetAct;
}

uint32_t BZ_getRiscvLog(BzDevice_t *hw_priv)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	uint32_t DbgData=0;
	uint32_t DbgAddr,length,index=0;
	BzData DataInfo;
	uint8_t debug_print[DBG_PRINT_BUF_SIZE_MAX + 1];
	BzCmdTx *pMsg=NULL;
	
	pMsg=&DataInfo.u.Tx;
	DataInfo.CmdType=0xff;
	pMsg->Cmd=BZ_SET_OPCODE;
	pMsg->TxData.value=ENABLE_DBG_PRINT2SPI;
	
	bzRetAct=BZ_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(bzRetAct);
	
	bzRetAct=BZ_spiGetCmdStatus(hw_priv);
	IF_ERR_RETURN(bzRetAct);	

	if(hw_priv->dbg_done==1){
		
		bzRetAct=BZ_spiReadReg(hw_priv,SEND_CMD_DATA,&DbgAddr);
		IF_ERR_RETURN(bzRetAct);	
		
		BZ_Memset(debug_print,0,sizeof(debug_print));
		
		for(length=0;length<(DBG_PRINT_BUF_SIZE_MAX>>2);length++){
			bzRetAct=bz_spiGetRiscvLog(hw_priv,DbgAddr,&DbgData);
			IF_ERR_RETURN(bzRetAct);	
            memcpy(debug_print+index,&DbgData,4);
			DbgAddr+=4;index+=4;
		}
		
		debug_print[DBG_PRINT_BUF_SIZE_MAX] = '\0';
		
		BZHAL_Printf("\n[Riscv]:%s\n",debug_print);
		
		BZ_clearStatus(hw_priv);
		hw_priv->dbg_done=0;
	}
	return (uint32_t)bzRetAct;		
}

uint32_t BZ_enableRiscvLog(BzDevice_t *hw_priv,uint32_t debug)
{   
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
   	
	BzData DataInfo;
	BzCmdTx *pMsg=NULL;
	DataInfo.CmdType=0xff;
	pMsg=&DataInfo.u.Tx;
	pMsg->Cmd=BZ_SET_OPCODE;
	pMsg->TxData.value=(((debug &0xffff)<<16)|(HI_MSG_ENABLE_LOG_MSGID));	

	bzRetAct=BZ_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(bzRetAct);
	
	return bzRetAct;
}



uint32_t BZ_spiGetCmdStatus(BzDevice_t *hw_priv)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    bzHalErr halStatus = BZHAL_OK;
    uint32_t ReaData;
	
	halStatus=BZ_spiReadReg(hw_priv,DOCMD_STATUS,&ReaData);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	
	
	if(ReaData&BIT(HI_STATUS_FIRMWARE_READY_BIT))
	{
		bzRetAct=bz_getFirmwareVersion(hw_priv);
		hw_priv->caps.firmwareReady=1;	
	}
	if(ReaData&BIT(RFPLL_CALI_DONE_BIT))
	{
		hw_priv->rfpll_done=1;
	}
	if(ReaData&BIT(PHY_INIT_CALI_DONE_BIT))
	{
		hw_priv->initcal_done=1;
	}
	if(ReaData&BIT(PHY_INIT_PROFILE_DONE_BIT))
	{
		hw_priv->profile_done=1;
	}
	if(ReaData&BIT(JESD204B_DONE_BIT))
	{
		hw_priv->j204b_done=1;
	}
	if(ReaData&BIT(DBG_2SPI_DONE_BIT))
	{
		hw_priv->dbg_done=1;
	}
	if(ReaData&BIT(GPIO_GET_VALUE_DONE_BIT))
	{
		hw_priv->gpio_get_value_done=1;
	}
	
	return (uint32_t)bzRetAct;		
}

uint32_t BZ_spiResetChip(BzDevice_t *hw_priv,uint8_t RestType)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	
	bzRetAct=bz_spiInterrupt(hw_priv,1);
	IF_ERR_RETURN(bzRetAct); 
	return (uint32_t)bzRetAct;		
}
uint32_t BZ_getBootStatus(BzDevice_t *hw_priv)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    bzHalErr halstatus = BZHAL_OK;
	uint32_t regValue=0;
    uint32_t uiTry = WIAT_TIMEOUT;
	
	/*Wait for Boot Ready*/
    while(uiTry > 0){
		halstatus=BZ_spiReadField(hw_priv,REG_IP_SRAM_CTRL,&regValue,BOOT_RDY,BOOT_RDY);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);	
		if(regValue==1){
			break;
		}
        uiTry--;
	}
    if(uiTry <= 0){
		BZHAL_Printf("ERROR:%s Trytimeout!\n", __func__);
    }

	return (uint32_t)bzRetAct;		
}
uint32_t BZ_getSdkVersion(BzDevice_t *hw_priv)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	struct build_info build;
	memset(&build, 0, sizeof(struct build_info));

	build.total_svn_version=TOTAL_SVN_VERSION;
	strncpy(build.current_svn_version,CURRENT_SVN_VERSION,(sizeof(build.current_svn_version)-1));

	BZHAL_Printf("\n************SDK VERSION MSG************\n"
	   "total_svn_version: %d\n"
	   "compiler: %s\n"
	   "current_svn_version: %s\n"
	   "*************************************\n\n",
	   build.total_svn_version, COMPILER, build.current_svn_version);

	return bzRetAct;
}


