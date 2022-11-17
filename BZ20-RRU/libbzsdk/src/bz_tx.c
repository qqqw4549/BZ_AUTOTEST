/**
* \file bz_tx.c
* \brief Contains functions to support B20 Tx data path control
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/
#include "bz_tx.h"
#include "bz_radio.h"
#include "bz_error.h"
#include "bz_plat_hal.h"
#include "bz_riscv_types.h"
#include "bz_reg_dig.h"
#include "bz_reg_ana.h"

uint32_t BZ_setTxManualGain(BzDevice_t *hw_priv,BzTxChannels_t TxCh, uint8_t gainIndex)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halError=BZHAL_OK;
	
    switch(TxCh)
    {
        case BZ_TX1:
            if ((gainIndex < hw_priv->BzState.gainIndexes.tx1MinGainIndex) ||
                (gainIndex > hw_priv->BzState.gainIndexes.tx1MaxGainIndex))
            {
				return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,
						BZ_ERR_ORX1_MAX_GAIN_VALID,bzRetAct,BZ_ERR_CHECK_PARAM);
            }
            break;
        case BZ_TX2:
            if ((gainIndex < hw_priv->BzState.gainIndexes.tx2MinGainIndex) ||
                (gainIndex > hw_priv->BzState.gainIndexes.tx2MaxGainIndex))
            {
				return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,
						BZ_ERR_ORX2_MAX_GAIN_VALID,bzRetAct,BZ_ERR_CHECK_PARAM);
            }
			break;
		default:
            break;
    }

    if(TxCh == BZ_TX1)
    {
        bzRetAct = BZ_spiWriteField(hw_priv, REG_GC_06,1,16,16);
        bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
		
		bzRetAct = BZ_spiWriteField(hw_priv, REG_GC_06,gainIndex,5,0);
        bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
    }

    if(TxCh == BZ_TX2)
    {
        bzRetAct = BZ_spiWriteField(hw_priv, REG_GC_06,1,17,17);
        bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
		
		bzRetAct = BZ_spiWriteField(hw_priv, REG_GC_06,gainIndex,13,8);
       	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
    }
	
	return (uint32_t)bzRetAct;	
}
uint32_t BZ_setTxGainControlMode(BzDevice_t *hw_priv, BzGainMode_t mode)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halError=BZHAL_OK;
	
	/*TxGain only control by api*/
	halError = BZ_spiWriteField(hw_priv,REG_GC_05,mode,0,0);	
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;	
}

uint32_t BZ_getTxGain(BzDevice_t *hw_priv,BzTxChannels_t TxChannel, uint32_t *TxGainIndex)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halError=BZHAL_OK;	

	switch(TxChannel){
		case BZ_TX1:
			halError = BZ_spiReadField(hw_priv, REGOUT_GC_22,TxGainIndex,6,0);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;
		case BZ_TX2:
			halError = BZ_spiReadField(hw_priv, REGOUT_GC_22,TxGainIndex,14,8);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;
		default:
			return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,
									BZ_ERR_TXRXORX_CHANNLE,bzRetAct,BZ_ERR_CHECK_PARAM);
	}
    return (uint32_t)bzRetAct;
}

uint32_t BZ_setPaProtectionCfg(BzDevice_t *hw_priv,BzTxPaProtectCfg_t *txPaProtectCfg)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halStatus=BZHAL_OK;	
	
	/*Cnf the cali data cnt*/
	halStatus = BZ_spiWriteField(hw_priv,REG_GC_46,txPaProtectCfg->TxCaliCnt,31,0);	
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RX,halStatus,bzRetAct,BZ_ERR_RESET_SPI);	
	IF_ERR_RETURN(bzRetAct);

	/*Cnf the HighThreshold*/
	halStatus = BZ_spiWriteField(hw_priv,REG_GC_48,txPaProtectCfg->TxPowerThreshold,31,0);	
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RX,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	if(txPaProtectCfg->Enable==0x0){
		/*Clear Paprotect status*/
		halStatus=BZ_spiWriteField(hw_priv,REG_GC_49,0x0,30,29);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
	}
	/*Enable PA Protect*/
	halStatus = BZ_spiWriteField(hw_priv,REG_GC_47,txPaProtectCfg->Enable,0,0);	
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RX,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
    return (uint32_t)bzRetAct;
}

uint32_t BZ_getPaProtectErrorFlags(BzDevice_t *hw_priv, uint32_t *errorFlags)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halStatus=BZHAL_OK;	
	
    /* Read back PA Protection Error flags */
	/*errorFlags==1,it indicate Tx1 generate Pa protect*/
	/*errorFlags==2,it indicate Tx2 generate Pa protect*/
	/*errorFlags==3,it indicate Tx1/Tx2 generate Pa protect*/
    halStatus = BZ_spiReadField(hw_priv, REG_GC_49, errorFlags,30,29);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	
	
    return (uint32_t)bzRetAct;
}

uint32_t BZ_RevertClearPaProtect(BzDevice_t *hw_priv)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halStatus = BZHAL_OK;
	uint32_t errorFlag,index;
	
	BZ_getPaProtectErrorFlags(hw_priv,&errorFlag);

	/*revert Paprotect gain*/
	if(errorFlag==0x1){
		
		halStatus = BZ_spiReadField(hw_priv, REG_GC_49,&index,6,0);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);	

		bzRetAct=BZ_setTxManualGain(hw_priv,BZ_TX1,index);
		IF_ERR_RETURN(bzRetAct);	
	}
	if(errorFlag==0x2){
		
		halStatus = BZ_spiReadField(hw_priv, REG_GC_49,&index,14,8);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);	
		
		bzRetAct=BZ_setTxManualGain(hw_priv,BZ_TX2,index);
		IF_ERR_RETURN(bzRetAct);	
	}
	
	/*Clear Paprotect status*/
	halStatus=BZ_spiWriteField(hw_priv,REG_GC_49,0x0,30,29);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_RISCV,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);	

    return (uint32_t)bzRetAct;
}

uint32_t BZ_getTxSamplePower(BzDevice_t *hw_priv,BzTxChannels_t txChannel, uint16_t *channelPower)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	
    return (uint32_t)bzRetAct;
}

/*This function enable a digital numerically controlled oscillator in the Bz20*/
/*Digital to create a teset Cw tone on Tx1 or Tx2 RF output*/
uint32_t BZ_setDDsFreqConfig(BzDevice_t *hw_priv, BzTxChannels_t txChannel, BzTxDdsTestToneCfg_t txDDSTone, BzTxDdsAttenCfg_t txDDSAtten)
{
   	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halError=BZHAL_OK;
	uint32_t txFltBypassMode;
	sint32_t Freq_word0,Freq_word1;
	
	/*Calculate the DDS Freq Deviation*/
	float para = 4294967296.0F/245760;
	
	halError=BZ_spiReadField(hw_priv, REG_CALI_TXFLT_CTRL_0, &txFltBypassMode,3,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halError=BZ_spiWriteField(hw_priv, REG_CLK_SWAP_0,1,11,11);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	if(txFltBypassMode == TX_FIR_BYPASS)
	{
		Freq_word0 = para*txDDSTone.txTone1Freq_kHz;
		Freq_word1 = para*txDDSTone.txTone2Freq_kHz;;
	}
	else if(txFltBypassMode == TX_FIRANDTHB3_BYPASS)
	{
		Freq_word0 = para/2*txDDSTone.txTone1Freq_kHz;
		Freq_word1 = para/2*txDDSTone.txTone2Freq_kHz;;
	}
	else if(txFltBypassMode == TX_THB1_BYPASS)
	{
		Freq_word0 = para/4*txDDSTone.txTone1Freq_kHz;
		Freq_word1 = para/4*txDDSTone.txTone2Freq_kHz;
	}
	else
	{
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,
								BZ_ERR_TXRXORX_CHANNLE,bzRetAct,BZ_ERR_CHECK_PARAM);
	}

 	halError=BZ_spiWriteField(hw_priv, REG_CALI_DG_0, txDDSAtten.txTone0Atten, 6, 4);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halError=BZ_spiWriteField(hw_priv, REG_CALI_DG_0, txDDSAtten.txTone1Atten, 9, 7);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halError=BZ_spiWriteField(hw_priv, REG_CALI_DG_0, txDDSAtten.txTone2Atten, 12, 10);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halError=BZ_spiWriteField(hw_priv, REG_CALI_DG_0, txDDSAtten.txTone3Atten, 15, 13);	
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halError=BZ_spiWriteField(hw_priv, REG_CALI_DG_2, Freq_word0, 31, 0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halError=BZ_spiWriteField(hw_priv, REG_CALI_DG_3, Freq_word1, 31, 0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	halError=BZ_spiWriteField(hw_priv, REG_CALI_CTRL_0, 1, 0, 0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	switch(txChannel){
		case BZ_TX1:			
			halError=BZ_spiWriteField(hw_priv, REG_CALI_CTRL_0, 0, 4, 4); 
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;
		case BZ_TX2:
			halError=BZ_spiWriteField(hw_priv, REG_CALI_CTRL_0, 1, 4, 4); 
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;
		default:
			break;
	}
	//Enable Nco 
	halError=BZ_spiWriteField(hw_priv, REG_CALI_OP_0, 1, 12, 12);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	halError=BZ_spiWriteField(hw_priv, REG_CALI_OP_0, 0 , 12, 12);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	halError=BZ_spiWriteField(hw_priv, REG_CALI_OP_0, 0, 1, 1);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halError=BZ_spiWriteField(hw_priv, REG_CALI_OP_0, 1, 1, 1);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	halError=BZ_spiWriteField(hw_priv, REG_CALI_DG_0, 1, 1, 0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	return bzRetAct;
}

uint32_t BZ_setDdsOff(BzDevice_t *hw_priv)
{	
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halError= BZHAL_OK;
	
	halError=BZ_spiWriteField(hw_priv, REG_CALI_CTRL_0, 0x00,0,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	halError=BZ_spiWriteField(hw_priv, REG_CALI_OP_0, 0x00,1,1);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	halError=BZ_spiWriteField(hw_priv, REG_CALI_DG_0, 0, 1, 0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	halError=BZ_spiWriteField(hw_priv, REG_CLK_SWAP_0,0,11,11);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_TX,halError,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	return bzRetAct;
}

