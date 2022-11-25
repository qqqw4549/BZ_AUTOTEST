/**
* \file bz_cals.c
* \brief Contains functions to support Bz init and tracking calibrations
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/
#include "bz_cals.h"
#include "bz_error.h"
#include "bz_riscv.h"
#include "bz_plat_hal.h"
#include "bz_riscv_types.h"

uint32_t BZ_runInitCals(BzDevice_t *hw_priv,uint32_t initCalMask)
{	
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	BzData DataInfo;
	BzCmdTx *pMsg;
	
	BZ_Memset(&DataInfo,0,sizeof(BzData));
	pMsg=&DataInfo.u.Tx;
	DataInfo.CmdType=0xff;
	pMsg->Cmd=BZ_RUNN_INIT_OPCODE;
	pMsg->TxData.value=initCalMask;
	bzRetAct=BZ_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(bzRetAct);
	
	hw_priv->currentCalMask = initCalMask;
	return (uint32_t)bzRetAct;
}
uint32_t BZ_waitInitCalsDone(BzDevice_t *hw_priv,uint32_t timeout)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    uint32_t uiTry = WIAT_TIMEOUT;
	
	/*Wait for initial calibration is already donw*/
    while(uiTry > 0){
		BZ_mdelay(timeout);
		bzRetAct=BZ_spiGetCmdStatus(hw_priv);
		IF_ERR_RETURN(bzRetAct);
		if(hw_priv->initcal_done==1){
			BZ_clearStatus(hw_priv);
			hw_priv->initcal_done=0;
			break;
		}
        uiTry--;
	}
    if(uiTry <= 0){ 
		BZHAL_Printf("ERROR:%s Trytimeout!\n", __func__);
    }

	return (uint32_t)bzRetAct;
}

const char* bz_getInitCalsErrMsg(uint32_t errSrc, uint32_t errCode)
{
    if (errSrc == BZ_ERRHDL_INITCALS)
    {
	    switch (errCode)
        {
			case RX1_DC_OFFSET_ERROR: 			return  "RX1_DC_OFFSET Error Status ";
			case RX2_DC_OFFSET_ERROR: 			return  "RX2 DC OFFSET Error Status";
			case RX1_QEC_INIT_ERROR:			return  "RX1 QEC INIT Error Status";
			case RX2_QEC_INIT_ERROR:			return  "RX2 QEC INIT Error Status";
			case TX1_LO_LEAKAGE_INTERNAL_ERROR:	return  "TX1 LO LEAKAGE INTERNAL Error Status";
			case TX2_LO_LEAKAGE_INTERNAL_ERROR:	return  "TX2 LO LEAKAGE INTERNAL Error Status";
			case TX1_LO_LEAKAGE_EXTERNAL_ERROR:	return  "TX1 LO LEAKAGE EXTERNAL Error Status";
			case TX2_LO_LEAKAGE_EXTERNAL_ERROR:	return  "TX2 LO LEAKAGE EXTERNAL Error Status";
			case TX1_QEC_INIT_ERROR:			return  "TX1 QEC INIT Error Status";
			case TX2_QEC_INIT_ERROR:			return  "TX2 QEC INIT Error Status";
			case BZ_BBPLL_LOCK_ERROR: 			return  "BB PLL Calibration Error Status";
			case BZ_RFPLL_LOCK_ERROR: 			return  "RF PLL Calibration Error Status";
			case BZ_AUXPLL_LOCK_ERROR:			return  "AUX PLL Calibration Error Status";
            case BZ_CALI_STATS_OK:              return  "BZ Calibration Status Ok";
			default :				
												return "Unknown Init Calibration Error\n";
        }
    }
    return "Unknown Init Calibration Error\n";
}

uint32_t  BZ_getInitCalsStatus(BzDevice_t *hw_priv)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	BzData DataInfo;
	BzMem *pMsg = NULL;
	struct elog_s *elog = NULL;
	unsigned short newCaLiEId;
	uint32_t i,elogCnt, isSendCaliId;
	uint32_t szCaLiEventId[CALI_EVENTID_CNT]={0x50, 0x10, 0x11, 0x12, 0x13, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x30, 0x31, 0x32};
	
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
	isSendCaliId = 0;
	for (newCaLiEId = elog->index - 1, elogCnt = 0; !isSendCaliId; newCaLiEId--, elogCnt++)
	{
		if (newCaLiEId >= elog->len)
		{
			newCaLiEId = elog->len - 1;
		}
		
		for (i = 0; i < CALI_EVENTID_CNT; i++)
		{
			if (elog->e_id[newCaLiEId].id == szCaLiEventId[i])
			{		
				BZHAL_Printf("Calibration Results:%s\n",bz_getInitCalsErrMsg(BZ_ERRHDL_INITCALS, ((elog->e_id[newCaLiEId].id<<24)|(elog->e_id[newCaLiEId].data & 0xffff))));
				isSendCaliId = 1;
				break;
			}
		}
		
		if (elogCnt > 1024)
		{
			break;
		}
	}	

	return bzRetAct;
}


