/**
* \file bz_radio.c
* \brief Contains functions to support Bz radio control and pin control
 *        functions
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/
#include "bz_radio.h"
#include "bz_reg_ana.h"
#include "bz_reg_dig.h"
#include "bz_plat_hal.h"
#include "bz_riscv.h"
#include "bz_error.h"



uint32_t BZ_setPllFrequency(BzDevice_t *hw_priv,uint32_t name,uint64_t pllLoFreq_Hz)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	BzData DataInfo;
	BzMem *pMsg;
	
	BZ_Memset(&DataInfo,0,sizeof(BzData));
	pMsg=&DataInfo.u.Mmsg;
	DataInfo.CmdType=0xff;
	pMsg->Cmd=BZ_WRITE_MEM_OPCODE;

	if(name == BZ_RF_PLL)
	{
		pMsg->Msg.Id=HI_MSG_TO_RFFRQ_MSGID;
	
		pMsg->Msg.Lenth=HI_MSG_TO_RFFRQ_MSGLEN;
		/*Write RfPllFreq to SRAM*/
		/*long byte array with parsed bytes from rfPllLoFrequency_Hz word */
		pMsg->Msg.data[0] = (uint8_t)(pllLoFreq_Hz & 0xFF);
		pMsg->Msg.data[1] = (uint8_t)((pllLoFreq_Hz >> 8) & 0xFF);
		pMsg->Msg.data[2] = (uint8_t)((pllLoFreq_Hz >> 16) & 0xFF);
		pMsg->Msg.data[3] = (uint8_t)((pllLoFreq_Hz >> 24) & 0xFF);
		pMsg->Msg.data[4] = (uint8_t)((pllLoFreq_Hz >> 32) & 0xFF);
		pMsg->Msg.data[5] = (uint8_t)((pllLoFreq_Hz >> 40) & 0xFF);
		pMsg->Msg.data[6] = (uint8_t)((pllLoFreq_Hz >> 48) & 0xFF);
		pMsg->Msg.data[7] = (uint8_t)((pllLoFreq_Hz >> 56) & 0xFF);
	}
	else if(name == BZ_AUX_PLL)
	{
		pMsg->Msg.Id=HI_MSG_TO_AUXFRQ_MSGID;
		pMsg->Msg.Lenth=HI_MSG_AUXFRQ_MSGLEN;
		/*Write AUXPllFreq to SRAM*/
		/*long byte array with parsed bytes from rfPllLoFrequency_Hz word */
		pMsg->Msg.data[0] = (uint8_t)(pllLoFreq_Hz & 0xFF);
		pMsg->Msg.data[1] = (uint8_t)((pllLoFreq_Hz >> 8) & 0xFF);
		pMsg->Msg.data[2] = (uint8_t)((pllLoFreq_Hz >> 16) & 0xFF);
		pMsg->Msg.data[3] = (uint8_t)((pllLoFreq_Hz >> 24) & 0xFF);
		pMsg->Msg.data[4] = (uint8_t)((pllLoFreq_Hz >> 32) & 0xFF);
		pMsg->Msg.data[5] = (uint8_t)((pllLoFreq_Hz >> 40) & 0xFF);
		pMsg->Msg.data[6] = (uint8_t)((pllLoFreq_Hz >> 48) & 0xFF);
		pMsg->Msg.data[7] = (uint8_t)((pllLoFreq_Hz >> 56) & 0xFF);
	}
	/*Construct the cmdto fw*/
	bzRetAct=BZ_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;	
}

uint32_t BZ_getPllFrequency(BzDevice_t *hw_priv,uint32_t name,uint64_t *pllLoFreq_Hz)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	BzData DataInfo;
	BzMem *pMsg;
	
	BZ_Memset(&DataInfo,0,sizeof(BzData));
	pMsg=&DataInfo.u.Mmsg;
	DataInfo.CmdType=0xff;
	pMsg->Cmd=BZ_READ_MEM_OPCODE;
	pMsg->Msg.Lenth=HI_MSG_TO_RFFRQ_MSGLEN;

	if(name == BZ_RF_PLL)
	{
		pMsg->Msg.Id=HI_MSG_TO_RFFRQ_MSGID;
		pMsg->Msg.addr=RFFRQ_MSG_ADDR;
	}
	else if(name == BZ_AUX_PLL)
	{
		pMsg->Msg.Id=HI_MSG_TO_AUXFRQ_MSGID;
		pMsg->Msg.addr=AUXFRQ_MSG_ADDR;
	}
	bzRetAct=BZ_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_READ_CMD);
	IF_ERR_RETURN(bzRetAct);
	
	/*Write RfPllFreq to SRAM*/
    /*long byte array with parsed bytes from rfPllLoFrequency_Hz word */

	*pllLoFreq_Hz = pMsg->Msg.data[0]  & 0xFF;
	*pllLoFreq_Hz |= ((uint64_t)pMsg->Msg.data[1])<<8;
	*pllLoFreq_Hz |= ((uint64_t)pMsg->Msg.data[2])<<16;
	*pllLoFreq_Hz |= ((uint64_t)pMsg->Msg.data[3])<<24;
	*pllLoFreq_Hz |= ((uint64_t)pMsg->Msg.data[4])<<32;
	*pllLoFreq_Hz |= ((uint64_t)pMsg->Msg.data[5])<<40;
	*pllLoFreq_Hz |= ((uint64_t)pMsg->Msg.data[6])<<48;
	*pllLoFreq_Hz |= ((uint64_t)pMsg->Msg.data[7])<<56;
	
	return (uint32_t)bzRetAct;	
}

uint32_t BZ_setRadioCtrlSpiMode(BzDevice_t *hw_priv, BzRxChannels_t rxChannel,BzObsRxChannels_t OrxChannel,BzTxChannels_t txChannel)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;
	
	halstatus=BZ_spiWriteField(hw_priv,REG_CH_ENA,0x0,31,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	
	
	halstatus=BZ_spiWriteField(hw_priv,REG_CLK_SWAP_1,0x1f,31,24);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	halstatus=BZ_spiWriteField(hw_priv,REG_ENA_EXT_0,0x3f,5,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	halstatus=BZ_spiWriteField(hw_priv, REG_PD0, 0x0, 31, 0);	
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halstatus=BZ_spiWriteField(hw_priv, REG_RSTN0, 0, 1, 0);	
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	halstatus=BZ_spiWriteField(hw_priv, REG_RSTN0, 0, 7, 4);	
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	switch (txChannel)
	{
	    case BZ_TXOFF: 
			/*close TX1/TX2: TXFE2_LOBUF_PD=1,TXFE1_LOBUF_PD=1*/
			halstatus=BZ_spiWriteField(hw_priv, REG_PD0, 0xFFF, 11, 0);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);	
		
			halstatus=BZ_spiWriteField(hw_priv, REG_PD0, 0xF, 19, 16);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);			
			break;
	    case BZ_TX1: 
            // Enable TX1
			halstatus=BZ_spiWriteField(hw_priv, REG_RSTN0, 0x1, 1, 0);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halstatus=BZ_spiWriteField(hw_priv, REG_SYNC_1, 0x2, 1, 0);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halstatus=BZ_spiWriteField(hw_priv, REG_TXLPF0, 0x0, 8, 8);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);				

            halstatus=BZ_spiWriteField(hw_priv, REG_TXLPF3, 0x1, 8, 8);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);

            halstatus=BZ_spiWriteField(hw_priv, REG_PD0, 0x1, 9, 8);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);

			break;
	    case BZ_TX2: 
            // Enable TX2
		    halstatus=BZ_spiWriteField(hw_priv, REG_RSTN0, 0x2, 1, 0);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halstatus=BZ_spiWriteField(hw_priv, REG_SYNC_1, 0x1, 1, 0);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halstatus=BZ_spiWriteField(hw_priv, REG_TXLPF0, 0x1, 8, 8);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);				

            halstatus=BZ_spiWriteField(hw_priv, REG_TXLPF3, 0x0, 8, 8);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);	

            halstatus=BZ_spiWriteField(hw_priv, REG_PD0, 0x2, 9, 8);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			 
			break;
	    case BZ_TX1TX2:
            halstatus=BZ_spiWriteField(hw_priv, REG_PD0, 0x0, 9, 8);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			halstatus=BZ_spiWriteField(hw_priv, REG_RSTN0, 0x3, 1, 0);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halstatus=BZ_spiWriteField(hw_priv, REG_SYNC_1, 0x0, 1, 0);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halstatus=BZ_spiWriteField(hw_priv, REG_TXLPF0, 0x1, 8, 8);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			halstatus=BZ_spiWriteField(hw_priv, REG_TXLPF3, 0x1, 8, 8);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);		
			break;
	    default:
			break;
	}	

	switch(rxChannel)
	{
	    case BZ_RXOFF: 			
			/*close RX1/RX2: RXFE2_LOBUF_PD=1,RXFE1_LOBUF_PD=1*/
			halstatus=BZ_spiWriteField(hw_priv, REG_PD0, 0x3, 13, 12);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);	
			/*close RX1: RX1_RX_ENN2=1*/
			halstatus=BZ_spiWriteField(hw_priv, REG_RXFE6, 0x7, 21, 19);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			/*close RX2: RX2_RX_ENN2=1*/
			halstatus=BZ_spiWriteField(hw_priv, REG_RXFE14, 0x7, 21, 19);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;
	    case BZ_RX1:  
			/*Enable RX1 RXFE1_LOBUF_PD=1*/
			halstatus=BZ_spiWriteField(hw_priv, REG_PD0, 0x1, 13, 12);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);	
			
			halstatus=BZ_spiWriteField(hw_priv, REG_RSTN0, 0x2, 5, 4);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halstatus=BZ_spiWriteField(hw_priv, REG_SYNC_1, 0x2, 5, 4);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			halstatus=BZ_spiWriteField(hw_priv, REG_RXFE6, 0x0, 21, 19);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
            halstatus=BZ_spiWriteField(hw_priv, REG_RXFE14, 0x7, 21, 19);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;
	    case BZ_RX2:			
			/*Enable RX2 RXFE2_LOBUF_PD=1*/
			halstatus=BZ_spiWriteField(hw_priv, REG_PD0, 0x2, 13, 12);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);	
			
			halstatus=BZ_spiWriteField(hw_priv, REG_RSTN0, 0x1, 5, 4);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halstatus=BZ_spiWriteField(hw_priv, REG_SYNC_1, 0x1, 5, 4);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			halstatus=BZ_spiWriteField(hw_priv, REG_RXFE14, 0x0, 21, 19);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
            halstatus=BZ_spiWriteField(hw_priv, REG_RXFE6, 0x7, 21, 19);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;
	    case BZ_RX1RX2:
			/*Enable RX1/RX2 RXFE2_LOBUF_PD=1*/
			halstatus=BZ_spiWriteField(hw_priv, REG_PD0, 0x0, 13, 12);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);	
			
			halstatus=BZ_spiWriteField(hw_priv, REG_RSTN0, 0x3, 5, 4);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halstatus=BZ_spiWriteField(hw_priv, REG_SYNC_1, 0x0, 5,4);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);

			halstatus=BZ_spiWriteField(hw_priv, REG_RXFE6, 0x0, 21, 19);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);

			halstatus=BZ_spiWriteField(hw_priv, REG_RXFE14, 0x0, 21, 19);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;		
	    default:
			break;
	}		
	
	switch(OrxChannel){
        case BZ_ORXOFF: 
			/*PD SAR1 & SAR2*/
			halstatus=BZ_spiWriteField(hw_priv, REG_PD0, 0x3, 15, 14);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);	
			
			/*close ORX1/ORX2*/
			halstatus=BZ_spiWriteField(hw_priv, REG_RXFE5, 0x7, 21, 19);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
		
			halstatus=BZ_spiWriteField(hw_priv, REG_RXFE13, 0x7, 21, 19);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;
        case BZ_ORX1:			
			/*PD  SAR2*/
			halstatus=BZ_spiWriteField(hw_priv, REG_PD0, 0x1, 15, 14);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			/*RX1_ORX_ENN2=0*/
			halstatus=BZ_spiWriteField(hw_priv, REG_RXFE5, 0, 21, 19);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
            /*RX2_ORX_ENN2=1*/
			halstatus=BZ_spiWriteField(hw_priv, REG_RXFE13, 0x7, 21, 19);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halstatus=BZ_spiWriteField(hw_priv, REG_RSTN0, 0x2, 7, 6);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halstatus=BZ_spiWriteField(hw_priv,REG_CLK_SWAP_1,0x0,15,15);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;
        case BZ_ORX2:					
			/*PD SAR1*/
			halstatus=BZ_spiWriteField(hw_priv, REG_PD0, 0x2, 15, 14);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);	

            /*RX1_ORX_ENN2=1*/
			halstatus=BZ_spiWriteField(hw_priv, REG_RXFE5, 0x7, 21, 19);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			/*RX2_ORX_ENN2=0*/
			halstatus=BZ_spiWriteField(hw_priv, REG_RXFE13, 0, 21, 19);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halstatus=BZ_spiWriteField(hw_priv, REG_RSTN0, 0x1, 7, 6);	
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			halstatus=BZ_spiWriteField(hw_priv,REG_CLK_SWAP_1,0x1,15,15);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;
        case BZ_ORX1ORX2:
			/*PD SAR1*/
			halstatus=BZ_spiWriteField(hw_priv, REG_PD0, 0x3, 15, 14);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);

            /*RX1_ORX_ENN2=1*/
			halstatus=BZ_spiWriteField(hw_priv, REG_RXFE5, 0x0, 21, 19);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			/*RX2_ORX_ENN2=0*/
			halstatus=BZ_spiWriteField(hw_priv, REG_RXFE13, 0x0, 21, 19);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);

			halstatus=BZ_spiWriteField(hw_priv, REG_RSTN0, 0x3, 7, 6);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);

			break;
        default:
			break;
	}
	
	return (uint32_t)bzRetAct;	
}


uint32_t BZ_getRadioCtrlSpiMode(BzDevice_t *hw_priv, BzRxChannels_t *rxChannel,BzObsRxChannels_t *OrxChannel,BzTxChannels_t *txChannel)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halStatus=BZHAL_OK;

	uint32_t fieldVal1 = 0;
	uint32_t fieldVal2 = 0;
	halStatus=BZ_spiReadField(hw_priv,REG_PD0,&fieldVal1,9,8);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
    switch (fieldVal1)
    {
        case BZ_BIT0BIT1_Mask: 
			/*TXFE1_LOBUF_PD=1*/			
			/*TXFE2_LOBUF_PD=1*/
			*txChannel= BZ_TXOFF;			
			break;
        case BZ_BIT0_Mask: 
			/*TXFE1_LOBUF_PD=0*/
			/*TXFE2_LOBUF_PD=1*/
			*txChannel= BZ_TX1;
            break;
        case BZ_BIT1_Mask: 
			/*TXFE1_LOBUF_PD=1*/
			/*TXFE2_LOBUF_PD=0*/
			*txChannel= BZ_TX2;
            break;
        case BZ_OFF_Mask:
			/*TXFE1_LOBUF_PD=0*/
			/*TXFE2_LOBUF_PD=0*/
			*txChannel= BZ_TX1TX2;
			break;
        default:
			break;
    } 
	fieldVal1 = 0;
	fieldVal2 = 0;
	halStatus=BZ_spiReadField(hw_priv,REG_RXFE6,&fieldVal1,19,19);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halStatus=BZ_spiReadField(hw_priv,REG_RXFE14,&fieldVal2,19,19);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	int fieldVal_mask = ((fieldVal1<<1)|fieldVal2);
	switch(fieldVal_mask)
	{
        case BZ_BIT0BIT1_Mask: 
			/*RX1_RX_ENN2=1*/
			/*RX2_RX_ENN2=1*/
			*rxChannel = BZ_RXOFF;
			break;
        case BZ_BIT0_Mask:  
			/*RX1_RX_ENN2=0*/
			/*RX2_RX_ENN2=1*/
			*rxChannel = BZ_RX1;
			break;
        case BZ_BIT1_Mask:			
			/*RX1_RX_ENN2=1*/
			/*RX2_RX_ENN2=0*/
			*rxChannel = BZ_RX2;
			break;
        case BZ_OFF_Mask:
			/*RX1_RX_ENN2=0*/			
			/*RX2_RX_ENN2=0*/
			*rxChannel = BZ_RX1RX2;
			break;		
        default:
			break;
	}

	fieldVal1 = 0;
	fieldVal2 = 0;
	halStatus=BZ_spiReadField(hw_priv,REG_RXFE5,&fieldVal1,19,19);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halStatus=BZ_spiReadField(hw_priv,REG_RXFE13,&fieldVal2,19,19);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	fieldVal_mask = ((fieldVal1<<1)|fieldVal2);	
	switch(fieldVal_mask){
        case BZ_BIT0BIT1_Mask: 
			/*RX1_RX_ENN2=1*/
			/*RX2_RX_ENN2=1*/
			*OrxChannel = BZ_ORXOFF;
			break;
        case BZ_BIT0_Mask:			
			/*RX1_ORX_ENN2=0*/			
			/*RX2_ORX_ENN2=1*/
			*OrxChannel = BZ_ORX1;		
			break;
        case BZ_BIT1_Mask:				
			/*RX1_ORX_ENN2=1*/			
			/*RX2_ORX_ENN2=0*/
			*OrxChannel = BZ_ORX2;		
			break;
        case BZ_OFF_Mask: 
			/*RX1_ORX_ENN2=0*/	
			/*RX2_ORX_ENN2=0*/
			*OrxChannel = BZ_ORX1ORX2;				
			break;
        default:
			break;
	}
	return (uint32_t)bzRetAct;	
}
uint32_t BZ_setRadioCtrlPinMode(BzDevice_t *hw_priv, BzRxChannels_t rxChannel,BzObsRxChannels_t OrxChannel,BzTxChannels_t txChannel)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halStatus=BZHAL_OK;
	
    if((OrxChannel != BZ_RXOFF) && (rxChannel != BZ_ORXOFF))
        IF_ERR_RETURN(BZ_ERR_CHECK_PARAM);

	halStatus=BZ_spiWriteField(hw_priv,REG_CH_ENA,0x21802,31,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	
	
	halStatus=BZ_spiWriteField(hw_priv,REG_CLK_SWAP_1,0xE0,31,24);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*
	 *RX/TX control mode: pin mode
	 *ext_tx1_ena = 0	
	 *ext_tx2_ena = 0	
	 *ext_rx1_ena = 0	
	 *ext_rx2_ena = 0	
	 *ext_orx1_ena = 0	
	 *ext_orx2_ena = 0
	 */	
	halStatus=BZ_spiWriteField(hw_priv, REG_ENA_EXT_0,0x0,5,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	/*disable TX/RX channel, by gpio*/
	halStatus = BZ_Tx1EnableCtrl(hw_priv, BZHAL_CHANNEL_DISABLE);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
	IF_ERR_RETURN(bzRetAct);			
	halStatus = BZ_Tx2EnableCtrl(hw_priv, BZHAL_CHANNEL_DISABLE);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
	IF_ERR_RETURN(bzRetAct);
	halStatus = BZ_Rx1EnableCtrl(hw_priv, BZHAL_CHANNEL_DISABLE);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
	IF_ERR_RETURN(bzRetAct);	
	halStatus = BZ_Rx2EnableCtrl(hw_priv, BZHAL_CHANNEL_DISABLE);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
	IF_ERR_RETURN(bzRetAct);
	halStatus = BZ_ORx1EnableCtrl(hw_priv, BZHAL_CHANNEL_DISABLE);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
	IF_ERR_RETURN(bzRetAct);	
	halStatus = BZ_ORx2EnableCtrl(hw_priv, BZHAL_CHANNEL_DISABLE);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
	IF_ERR_RETURN(bzRetAct);

	switch(txChannel) {
		case BZ_TXOFF: 
			/*disable TX1/TX2 channel*/
			halStatus = BZ_Tx1EnableCtrl(hw_priv, BZHAL_CHANNEL_DISABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);			
			halStatus = BZ_Tx2EnableCtrl(hw_priv, BZHAL_CHANNEL_DISABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);			
			break;
		case BZ_TX1:
			/*enable TX1 channel*/
			halStatus = BZ_Tx1EnableCtrl(hw_priv, BZHAL_CHANNEL_ENABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);
			break;
		case BZ_TX2:
			/*enable TX2 channel*/
			halStatus = BZ_Tx2EnableCtrl(hw_priv, BZHAL_CHANNEL_ENABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);			
			break;
		case BZ_TX1TX2:
			/*enable TX1/TX2 channel*/
			halStatus = BZ_Tx1EnableCtrl(hw_priv, BZHAL_CHANNEL_ENABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);
			halStatus = BZ_Tx2EnableCtrl(hw_priv, BZHAL_CHANNEL_ENABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);			
			break;
		default:
			break;
	}
	switch(rxChannel) {
		case BZ_RXOFF:
			/*disable RX1/RX2 channel*/
			halStatus = BZ_Rx1EnableCtrl(hw_priv, BZHAL_CHANNEL_DISABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);			
			halStatus = BZ_Rx2EnableCtrl(hw_priv, BZHAL_CHANNEL_DISABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);			
			break;
		case BZ_RX1:
			/*enable RX1 channel*/
			halStatus = BZ_Rx1EnableCtrl(hw_priv, BZHAL_CHANNEL_ENABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);			
			break;
		case BZ_RX2:
			/*enable RX2 channel*/
			halStatus = BZ_Rx2EnableCtrl(hw_priv, BZHAL_CHANNEL_ENABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);			
			break;
		case BZ_RX1RX2:			
			/*enable RX1/RX2 channel*/
			halStatus = BZ_Rx1EnableCtrl(hw_priv, BZHAL_CHANNEL_ENABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);	
			halStatus = BZ_Rx2EnableCtrl(hw_priv, BZHAL_CHANNEL_ENABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);			
			break;
		default:
			break;
	}

	switch(OrxChannel) {
		case BZ_ORXOFF:
			/*disable ORX1/ORX2 channel*/
			halStatus = BZ_ORx1EnableCtrl(hw_priv, BZHAL_CHANNEL_DISABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);			
			halStatus = BZ_ORx2EnableCtrl(hw_priv, BZHAL_CHANNEL_DISABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);
			break;
		case BZ_ORX1:
			/*disable ORX1 channel*/
			halStatus = BZ_ORx1EnableCtrl(hw_priv, BZHAL_CHANNEL_ENABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);
			break;
		case BZ_ORX2:
			/*disable ORX2 channel*/
			halStatus = BZ_ORx2EnableCtrl(hw_priv, BZHAL_CHANNEL_ENABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);	
			break;
		case BZ_ORX1ORX2:
			/*enable ORX1/ORX2 channel*/
			halStatus = BZ_ORx1EnableCtrl(hw_priv, BZHAL_CHANNEL_ENABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);	
			halStatus = BZ_ORx2EnableCtrl(hw_priv, BZHAL_CHANNEL_ENABLE);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_GPIO);
			IF_ERR_RETURN(bzRetAct);	
			break;
		default:
			break;
	}

	return (uint32_t)bzRetAct;	
}
uint32_t BZ_getRadioCtrlPinMode(BzDevice_t *hw_priv,uint32_t* ChanEnable)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halStatus=BZHAL_OK;
	
	/*ext_rx0_ena =1,use register enable rx0_path*/
	halStatus=BZ_spiReadField(hw_priv,REG_ENA_EXT_0,ChanEnable,5,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RADIO,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;	
}
uint32_t BZ_radioOn(BzDevice_t * hw_priv, BzRadioCtlMode Mode)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    uint8_t txChannel = 0;
    uint8_t rxOrxChannel = 0;
	uint8_t rxChannel=0;

	txChannel= hw_priv->BzState.supportChannels &3;
	rxChannel= (hw_priv->BzState.supportChannels >>2)&3;
	rxOrxChannel= (hw_priv->BzState.supportChannels >>4)&3;
		
	/*Enable channel*/
	if(Mode==BZ_SPI_MODE){
		bzRetAct=BZ_setRadioCtrlSpiMode(hw_priv,rxChannel,rxOrxChannel,txChannel);
		IF_ERR_RETURN(bzRetAct);
	}else{
		bzRetAct=BZ_setRadioCtrlPinMode(hw_priv,rxChannel,rxOrxChannel,txChannel);
		IF_ERR_RETURN(bzRetAct);

	}
	
	hw_priv->BzState.devState=BZ_STATE_RADIOON;
	
	return (uint32_t)bzRetAct;	
}

uint32_t BZ_radioOff(BzDevice_t * hw_priv,BzRadioCtlMode Mode)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;

	/*Disable channel*/
	if(Mode==BZ_SPI_MODE){
		bzRetAct=BZ_setRadioCtrlSpiMode(hw_priv,BZ_RXOFF,BZ_ORXOFF,BZ_TXOFF);
		IF_ERR_RETURN(bzRetAct);
	}else{
		bzRetAct=BZ_setRadioCtrlPinMode(hw_priv,BZ_RXOFF,BZ_ORXOFF,BZ_TXOFF);
		IF_ERR_RETURN(bzRetAct);
	}
	return (uint32_t)bzRetAct;	
}

uint32_t BZ_waitPllsLockDone(BzDevice_t *hw_priv, uint32_t timeout)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    uint32_t uiTry = WIAT_TIMEOUT;
	
	/*Wait for pll is already done*/
    while(uiTry > 0){
		BZ_mdelay(timeout);
		bzRetAct=BZ_spiGetCmdStatus(hw_priv);
		IF_ERR_RETURN(bzRetAct);
		
		if(hw_priv->rfpll_done==1){
			BZ_clearStatus(hw_priv);
			hw_priv->rfpll_done=0;
			break;
		}
        uiTry--;
	}
    if(uiTry <= 0){
		BZHAL_Printf("ERROR:%s Trytimeout!\n", __func__);
    }
	return (uint32_t)bzRetAct;	
}
uint32_t BZ_getRadioState(BzDevice_t *hw_priv)
{
    BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halStatus=BZHAL_OK;
	uint32_t radioStatus = 0;
	
	halStatus=BZ_spiReadReg(hw_priv, REGOUT_CALI_DONE_0,&radioStatus);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
    BZHAL_Printf("|****** radioStatus *******|\n");	
	if((radioStatus&RFPLLMASK) == 0x80)BZHAL_Printf("RFPLL LOCKED\n");
                                 else BZHAL_Printf("RFPLL UNLOCKED\n");
    if((radioStatus&BBPLLMASK) == 0x100)BZHAL_Printf("BBPLL LOCKED\n");
                                 else BZHAL_Printf("BBPLL UNLOCKED\n");
	if((radioStatus&AUXPLLMASK) == 0x200)BZHAL_Printf("AUXPLL LOCKED\n");
                                 else BZHAL_Printf("AUXPLL UNLOCKED\n");
	if((radioStatus&TX0ENMASK) == 0x400)BZHAL_Printf("TX1 ENABLE\n");
                                 else BZHAL_Printf("TX1 DISABLE\n");
	if((radioStatus&TX1ENMASK) == 0x800)BZHAL_Printf("TX2 ENABLE\n");
                                 else BZHAL_Printf("TX2 DISABLE\n");
	if((radioStatus&RX0ENMASK) == 0x1000)BZHAL_Printf("RX1 ENABLE\n");
                                 else BZHAL_Printf("RX1 DISABLE\n");
	if((radioStatus&RX1ENMASK) == 0x2000)BZHAL_Printf("RX2 ENABLE\n");
                                 else BZHAL_Printf("RX2 DISABLE\n");
	if((radioStatus&ORX0ENMASK) == 0x4000)BZHAL_Printf("ORX1 ENABLE\n");
                                 else BZHAL_Printf("ORX1 DISABLE\n");
	if((radioStatus&ORX1ENMASK) == 0x8000)BZHAL_Printf("ORX2 ENABLE\n");
                                 else BZHAL_Printf("ORX2 DISABLE\n");
	return (uint32_t)bzRetAct;		
}

uint32_t BZ_setPllTemperatureTrackEnable(BzDevice_t *hw_priv,uint32_t enable){
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION; 
	BzData DataInfo; BzCmdTx *pMsg;  
	BZ_Memset(&DataInfo,0,sizeof(BzData));
	pMsg=&DataInfo.u.Tx;  
	DataInfo.CmdType=0xff; 
	pMsg->Cmd=BZ_SET_OPCODE; 
	
	/*Send cmd to riscv change gaintable to normal digital gain mode*/ 
	DataInfo.u.Tx.TxData.value=(HI_ENABLE_PLLTC_TRACK|(enable<<16)); 
	bzRetAct=BZ_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD); 
	IF_ERR_RETURN(bzRetAct);   
	return (uint32_t)bzRetAct; 

}

