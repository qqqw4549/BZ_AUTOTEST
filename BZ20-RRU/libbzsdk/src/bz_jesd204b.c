/**
* \file bz_jesd204.c
* \brief Contains functions to support Bz JESD204b data interface
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/
#include "bz_jesd204b.h"
#include "bz_plat_hal.h"
#include "bz_riscv.h"
#include "bz_reg_dig.h"
#include "bz_reg_ana.h"
#include "bz_error.h"

#include <string.h>


uint32_t BZ_writeJes204bConfig(BzDevice_t *priv,BzInit_t *Init)
{
    BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    //u32 i = 0;
	BzData DataInfo;
	bzHalErr halError=BZHAL_OK;
	BzMem *pMsg;
	BzJesdSettings_t jesd204Config;
	BzJesdSettings_t *pjesd204Config = &jesd204Config;

	pMsg=&DataInfo.u.Mmsg;
	memset(&DataInfo,0,sizeof(DataInfo));

	DataInfo.CmdType=0xff;
	pMsg->Cmd=BZ_WRITE_MEM_OPCODE;
	pMsg->Msg.Id=HI_MSG_TO_J204B_MSGID;
	pMsg->Msg.Lenth=sizeof(BzJesdSettings_t);
	pMsg->Msg.addr=J204B_MSG_ADDR;

	if(CLK_TX_RX_500M == Init->tx.txProfile.txInputRate_kHz)
	{
		halError=BZ_spiWriteField(priv,REG_J_SEL_4,0x02,3,2);
		bzRetAct=BZ_ApiErrHandler(priv,BZ_ERRHDL_HAL_JES204B,halError,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);		
	}
	else
	{
		halError=BZ_spiWriteField(priv,REG_J_SEL_4,0x00,3,2);
		bzRetAct=BZ_ApiErrHandler(priv,BZ_ERRHDL_HAL_JES204B,halError,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
	}


	if(CLK_TX_RX_500M == Init->obsRx.orxProfile.orxOutputRate_kHz)
	{
		halError=BZ_spiWriteField(priv,REG_J_SEL_4,0x02,11,10);
		bzRetAct=BZ_ApiErrHandler(priv,BZ_ERRHDL_HAL_JES204B,halError,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
	}
	else
	{
		halError=BZ_spiWriteField(priv,REG_J_SEL_4,0x00,11,10);
		bzRetAct=BZ_ApiErrHandler(priv,BZ_ERRHDL_HAL_JES204B,halError,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
	}	

	pjesd204Config = (BzJesdSettings_t *)pMsg->Msg.data;
	memcpy(pjesd204Config, &Init->jesd204Settings, sizeof(BzJesdSettings_t));

	bzRetAct=BZ_sendRiscvCmd(priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	return (uint32_t)bzRetAct;
}

uint32_t BZ_waitJes204bConfigDone(BzDevice_t *hw_priv, uint32_t timeout)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    uint32_t uiTry = WIAT_TIMEOUT;
	
	/*Wait for JESD204B interface synchronization*/
    while(uiTry > 0){
		BZ_mdelay(timeout);
		bzRetAct=BZ_spiGetCmdStatus(hw_priv);
		IF_ERR_RETURN(bzRetAct);
		if(hw_priv->j204b_done==1){
			BZ_clearStatus(hw_priv);
			hw_priv->j204b_done=0;
			break;
		}
        uiTry--;
	}
    if(uiTry <= 0){
		BZHAL_Printf("ERROR:%s Trytimeout!\n", __func__);
    }

	return (uint32_t)bzRetAct;
}

uint32_t BZ_Jes204bSysrefControl(BzDevice_t *priv, BzJes204bSysrefControlMode Mode)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halError=BZHAL_OK;
	
	switch (Mode)
	{
		case BZ_JES204B_SYSREF_ON: 
			halError=BZ_spiWriteField(priv, REG_SYSREF0,0x00,15,15);
			bzRetAct=BZ_ApiErrHandler(priv,BZ_ERRHDL_HAL_JES204B,halError,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;
		case BZ_JES204B_SYSREF_OFF: 
			halError=BZ_spiWriteField(priv, REG_SYSREF0,0x01,15,15);
			bzRetAct=BZ_ApiErrHandler(priv,BZ_ERRHDL_HAL_JES204B,halError,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;
		default:
			break;
	} 
	
	return (uint32_t)bzRetAct;	
}

/*Prbs checker 7*/
uint32_t BZ_Prbs7Check(BzDevice_t *hw_priv,uint32_t LaneId)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halStatus=BZHAL_OK;
	uint32_t RxLaneRdy=0;
	uint32_t CheckResult=0;

	switch(LaneId)
	{
		case 0:
			halStatus=BZ_spiReadField(hw_priv,REGOUT_J204BRX00,&RxLaneRdy,12,12);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);

			halStatus=BZ_spiWriteField(hw_priv,REG_J204BRX03,1,8,8);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halStatus=BZ_spiReadField(hw_priv,REGOUT_J204BRX00,&CheckResult,14,14);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;
		case 1:
			halStatus=BZ_spiReadField(hw_priv,REGOUT_J204BRX00,&RxLaneRdy,28,28);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halStatus=BZ_spiWriteField(hw_priv,REG_J204BRX10,1,8,8);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halStatus=BZ_spiReadField(hw_priv,REGOUT_J204BRX00,&CheckResult,30,30);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;
		case 2:
			halStatus=BZ_spiReadField(hw_priv,REGOUT_J204BRX01,&RxLaneRdy,12,12);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halStatus=BZ_spiWriteField(hw_priv,REG_J204BRX17,1,8,8);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
		
			halStatus=BZ_spiReadField(hw_priv,REGOUT_J204BRX01,&CheckResult,14,14);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;	
		case 3:
			halStatus=BZ_spiReadField(hw_priv,REGOUT_J204BRX01,&RxLaneRdy,28,28);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halStatus=BZ_spiWriteField(hw_priv,REG_J204BRX24,1,8,8);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);

			halStatus=BZ_spiReadField(hw_priv,REGOUT_J204BRX01,&CheckResult,30,30);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;	
		default:
				break;

	}			
	if(RxLaneRdy!=1){
		BZHAL_Printf("LanId %d is not Rdy\n",LaneId);
	}			
	if(CheckResult==1){
		BZHAL_Printf("LanId %d prbs7 check ok\n",LaneId);
	}else{
		BZHAL_Printf("LanId %d prbs7 check Fail\n",LaneId);
	}
	return (uint32_t)bzRetAct;	
}

/*Get debug data */
uint32_t BZ_spiGetJesdDebugData(BzDevice_t *hw_priv, uint32_t addr, uint32_t sample_point, uint32_t count,uint32_t sample_rate)
{
	BzRecoveryActions_t bzRetAct = BZ_NO_ACTION;
    bzHalErr halStatus = BZHAL_OK;  
	uint32_t debug_data = 0;
	
	/*Disable jesdb Err intr*/
	halStatus=BZ_spiWriteField(hw_priv,REG_EXT_INTR_ENA,0,18,16);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	//halStatus=BZ_spiWriteMem(hw_priv,0x25006050,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halStatus=BZ_spiWriteField(hw_priv,REG_CLK_SWAP_0,1,17,17);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	if(sample_rate==1){ //245Mhz
		halStatus=BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,1,18,17);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
	}
	if(sample_rate==2){ //491Mhz
		halStatus=BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,2,18,17);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
	}
	
	/*2.disable store sram addr*/
	halStatus=BZ_spiWriteField(hw_priv,REG_PC_BASE_ADDR,0,31,31);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	if(sample_point != 32)
		halStatus=BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,sample_point,31,27);
	else
	{
		halStatus=BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,0,31,27);
		halStatus=BZ_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,1,23,23);
	}
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*1,set debug_cntcnt*/
	halStatus=BZ_spiWriteReg(hw_priv,REG_PC_COUNT,count);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*2.set debug data store sram addr*/
	halStatus=BZ_spiWriteField(hw_priv,REG_PC_BASE_ADDR,addr,28,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	//halStatus=BZ_spiWriteMem(hw_priv,0x25006050,1);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*2.Enable store sram addr*/
	halStatus=BZ_spiWriteField(hw_priv,REG_PC_BASE_ADDR,1,31,31);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
		
	{
		uint32_t i;
		for(i = 0; i < count; i += 4)
		{
			bzRetAct=BZ_spiReadMem(hw_priv, addr, &debug_data);
			BZHAL_Printf("0x%x=0x%08xx\n", addr, debug_data);
			addr += 4;
		}
	}
	
	halStatus=BZ_spiWriteField(hw_priv,REG_CLK_SWAP_0,0,17,17);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halStatus=BZ_spiWriteField(hw_priv,REG_EXT_INTR_ENA,0x7,18,16);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	return (uint32_t)bzRetAct;	
}
uint32_t BZ_enableFramerLink(BzDevice_t *hw_priv,uint32_t framerSel,uint32_t enable)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	BzData DataInfo;
	BzCmdTx *pMsg=NULL;
	
	BZ_Memset(&DataInfo,0,sizeof(BzData));
	pMsg=&DataInfo.u.Tx;
	DataInfo.CmdType=0xff;
	pMsg->Cmd=BZ_SET_OPCODE;
	

    if ((framerSel != BZ_FRAMER_A) && (framerSel != BZ_FRAMER_B) && (framerSel != BZ_FRAMER_A_AND_B))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_JES204B,
				BZ_ERR_VERTORXPFILE_RXINPUTRATE,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    if ((framerSel == BZ_FRAMER_A) ||
        (framerSel == BZ_FRAMER_A_AND_B))
    {
		pMsg->TxData.value=(HI_ENABLE_FRAMER_A|(enable<<16));
    }

    if ((framerSel == BZ_FRAMER_B) ||
        (framerSel == BZ_FRAMER_A_AND_B))
    {
		pMsg->TxData.value=(HI_ENABLE_FRAMER_B|(enable<<16));
    }
	
	bzRetAct=BZ_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;
}
uint32_t BZ_enableDeframerLink(BzDevice_t *hw_priv,uint32_t deframerSel,uint32_t enable)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	BzData DataInfo;
	BzCmdTx *pMsg=NULL;
	
	BZ_Memset(&DataInfo,0,sizeof(BzData));
	pMsg=&DataInfo.u.Tx;
	DataInfo.CmdType=0xff;
	pMsg->Cmd=BZ_SET_OPCODE;
	pMsg->TxData.value=(HI_ENABLE_DFRAMER|(enable<<16));
		
	bzRetAct=BZ_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(bzRetAct);
    
	return (uint32_t)bzRetAct;
}

uint32_t BZ_enableSysRefToFramer(BzDevice_t *hw_priv,BzInit_t *Init)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	BzData DataInfo;
	BzCmdTx *pMsg=NULL;
	
	BZ_Memset(&DataInfo,0,sizeof(BzData));
	pMsg=&DataInfo.u.Tx;
	DataInfo.CmdType=0xff;
	pMsg->Cmd=BZ_SET_OPCODE;
	pMsg->TxData.value=ENABLE_SYSREF_INIT;
	if(Init->clocks.rfPllPhaseSyncMode){
		bzRetAct=BZ_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
		IF_ERR_RETURN(bzRetAct);
	}
	return (uint32_t)bzRetAct;
}
uint32_t  BZ_getSyncStatus(BzDevice_t * hw_priv,uint32_t *TypeStatus)
{

	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	BzData DataInfo;
	BzMem *pMsg = NULL;
	struct elog_s *elog = NULL;
	//unsigned short newCaLiEId;
	uint32_t elogCnt=0;
	uint32_t addr=0;
    bzHalErr halStatus = BZHAL_OK;  
	uint32_t DeFramerStatus=0;
	uint32_t FramerAStatus=0;
	uint32_t FramerBStatus=0;
	uint32_t RegData=0;
	
	BZ_Memset(&DataInfo,0,sizeof(BzData));
	pMsg=&DataInfo.u.Mmsg;
	DataInfo.CmdType=0xff;
	
	pMsg->Cmd=BZ_READ_MEM_OPCODE;
	pMsg->Msg.Id=HI_MSG_TO_CALSTATUS_MSGID;
	pMsg->Msg.Lenth=(CALIBRITION_SRAM_LEN*4);
	pMsg->Msg.addr=CALIBRITION_SRAM_ADDR;
	*TypeStatus = 0;
	BZ_spiReadField(hw_priv,REGOUT_J204BRX00,&RegData,31,0);
	
	/*Disable unlock Err intr*/
	halStatus=BZ_spiWriteField(hw_priv,REG_EXT_INTR_ENA,0,19,19);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	addr=(CALIBRITION_SRAM_ADDR+12);
	bzRetAct=BZ_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_READ_CMD);
	
	if(bzRetAct!=BZ_NO_ACTION){
		return BZ_ERR_CHECK_PARAM;
	}
	
	elog = (struct elog_s *)(pMsg->Msg.data);
	for (elogCnt=0;elogCnt<1024;elogCnt++)
	{
		
		{
			if (elog->e_id[elogCnt].id == 0x82){		
				if((elog->e_id[elogCnt].data)==0xA){
					DeFramerStatus++;
				}
			}
			if (elog->e_id[elogCnt].id == 0x83){
				if((elog->e_id[elogCnt].data)==0xA){
					FramerAStatus++;
				}
			}			
			if (elog->e_id[elogCnt].id == 0x84){
				if((elog->e_id[elogCnt].data)==0xA){
					FramerBStatus++;
				}
			}
		}
		
		bzRetAct=BZ_spiWriteMem(hw_priv,(addr+elogCnt*8),0x0); 
		IF_ERR_RETURN(bzRetAct);
	}	
	if((DeFramerStatus>3)||(0x03 != (RegData&0x03))){
		*TypeStatus=0x1;  //TX DeFramer
	}
	if((FramerAStatus>3)||(0x04 != (RegData&0x04))){
		*TypeStatus|=0x10;  //RX Framer
	}
	if((FramerBStatus>3)||(0x08 != (RegData&0x08))){
		*TypeStatus|=0x100;  //ORX Framer
	}
	/*Eanble unlock Err intr*/
	halStatus=BZ_spiWriteField(hw_priv,REG_EXT_INTR_ENA,1,19,19);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	return bzRetAct;
}

