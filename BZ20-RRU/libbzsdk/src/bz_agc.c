/**
* \file bz_agc.c
* \brief Contains B20 API AGC function calls
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/
#include "bz_agc.h"
#include "bz_error.h"
#include "bz_reg_dig.h"
#include "bz_reg_ana.h"
#include "bz_plat_hal.h"

uint32_t BZ_setupRxAgc(BzDevice_t *hw_priv, BzAgcCfg_t *rxAgcCtrl)
{
    BzRecoveryActions_t bzRetAct = BZ_NO_ACTION;	
   	bzHalErr halError = BZHAL_OK;
    uint32_t agcRegister = 0;
	
	/*Disable rx0/rx1 analog gain ctrl external mode*/
	/*And use AGC module*/
	BZ_spiWriteField(hw_priv,REG_GC_50,0x0,3,2);
    bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
    IF_ERR_RETURN(bzRetAct);
	
    /* checking for valid BzAgcCfg_t hw_priv->rx->rxAgcCtrl pointer to determine if it has been initialized */
    if (rxAgcCtrl == NULL)
    {
		B20_DEBUGF(DBG_LEVEL_SERIOUS,"Error:rxAgcCtrl parameter is NULL\n");
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_AGC,halError,bzRetAct,BZ_ERR_CHECK_PARAM);

    }
    /*set RX AGC Clock Division Ratio*/
    halError = BZ_spiWriteField(hw_priv, REG_GC_00, rxAgcCtrl->agcClkDiv, 1, 0);
    bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
    IF_ERR_RETURN(bzRetAct);
	
	/* config agc register */
	agcRegister = (rxAgcCtrl->agcThreshPreventGain<<0)|(rxAgcCtrl->agcPeakThreshGainControlMode<<1);
	halError = BZ_spiWriteField(hw_priv, REG_GC_01, agcRegister,2,1);
	bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
    /* performing AGC peak wait time value check */
    if (rxAgcCtrl->agcPeakWaitCounter > 0x0F)
    {  
		B20_DEBUGF(DBG_LEVEL_SERIOUS,"Error:rxAgcCtrl parameter agcPeakWaitCounter exceeded\n");
        return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_AGC,halError,bzRetAct,BZ_ERR_CHECK_PARAM);
    }
    else
    {
        /* write AGC peak wait time */
       	halError = BZ_spiWriteField(hw_priv,REG_GC_01,rxAgcCtrl->agcPeakWaitCounter,7,4);	
	  	bzRetAct = BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_AGC,halError,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
    }
	if(rxAgcCtrl->agcGainUpdateCounter>0x7FFF)
	{
		B20_DEBUGF(DBG_LEVEL_SERIOUS,"Error:rxAgcCtrl parameter agcGainUpdateCounter exceeded\n");
        return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_AGC,halError,bzRetAct,BZ_ERR_CHECK_PARAM);
		
	}else{
	    /* performing range check for gain update time */
		halError = BZ_spiWriteField(hw_priv, REG_GC_01, rxAgcCtrl->agcGainUpdateCounter,22,8);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
	}
	
    /* If peak detect data structure is not included into project */
	if(rxAgcCtrl->agcPeak.apdEnableMeasurement){
		
		/* APD Configuration */
		halError = BZ_spiWriteField(hw_priv, REG_RXFE3, rxAgcCtrl->agcPeak.apdHighTresh,9,4);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_RXFE3, rxAgcCtrl->agcPeak.apdLowTresh,15,10);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_RXFE11, rxAgcCtrl->agcPeak.apdHighTresh,9,4);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_RXFE11, rxAgcCtrl->agcPeak.apdLowTresh,15,10);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_07, 0x0,8,8);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_07, rxAgcCtrl->agcPeak.apdAttackMode,0,0);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_07, rxAgcCtrl->agcPeak.apdMeasurementDuration,7,4);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_07, rxAgcCtrl->agcPeak.apdGainStepAttack,15,12);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_07, rxAgcCtrl->agcPeak.apdGainStepRecovery,19,16);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);		

		halError = BZ_spiWriteField(hw_priv, REG_GC_08, rxAgcCtrl->agcPeak.apdImmdOverNum,9,0);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_08, rxAgcCtrl->agcPeak.apdPeriOverNum,19,10);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_08, rxAgcCtrl->agcPeak.apdUnderNum,29,20);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);	
	}
	if(rxAgcCtrl->agcPeak.hb2EnableMeasurement){
		/* HB2 Configuration */
		halError = BZ_spiWriteField(hw_priv, REG_GC_09, 0x0,4,4);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_09, rxAgcCtrl->agcPeak.hb2MeasurementDuration,3,0);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_09, rxAgcCtrl->agcPeak.hb2RecoveryMode,5,5);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_09, rxAgcCtrl->agcPeak.hb2AttackMode,6,6);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);		

		halError = BZ_spiWriteField(hw_priv, REG_GC_10, rxAgcCtrl->agcPeak.hb2GainStepHighRecovery,7,4);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_10, rxAgcCtrl->agcPeak.hb2GainStepLowRecovery,11,8);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_10, rxAgcCtrl->agcPeak.hb2GainStepMidRecovery,15,12);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_10, rxAgcCtrl->agcPeak.hb2GainStepAttack,3,0);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_11, rxAgcCtrl->agcPeak.hb2OverThresh,13,0);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_11, rxAgcCtrl->agcPeak.hb2UnderRangeHighThresh,29,16);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);	

		halError = BZ_spiWriteField(hw_priv, REG_GC_12, rxAgcCtrl->agcPeak.hb2UnderRangeLowThresh,13,0);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_12, rxAgcCtrl->agcPeak.hb2UnderRangeMidThresh,29,16);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_13, rxAgcCtrl->agcPeak.hb2OverloadThreshCnt,9,0);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);		

		halError = BZ_spiWriteField(hw_priv, REG_GC_13, rxAgcCtrl->agcPeak.hb2UnderRangeHighThreshCnt,25,16);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_14, rxAgcCtrl->agcPeak.hb2UnderRangeLowThreshCnt,9,0);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_14, rxAgcCtrl->agcPeak.hb2UnderRangeMidThreshCnt,25,16);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);		

		halError = BZ_spiWriteField(hw_priv, REG_GC_15, rxAgcCtrl->agcPeak.hb2PeriExceededCnt,9,0);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_15, rxAgcCtrl->agcPeak.hb2PeriUnderExceededCnt,25,16);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);	

		halError = BZ_spiWriteField(hw_priv, REG_GC_16, rxAgcCtrl->agcPeak.hb2ImmdExceededCnt,9,0);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_16, rxAgcCtrl->agcPeak.hb2ImmdUnderHigCnt,25,16);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_17, rxAgcCtrl->agcPeak.hb2ImmdUnderLowCnt,9,0);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_17, rxAgcCtrl->agcPeak.hb2ImmdUnderMidCnt,25,16);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
	}

	if(rxAgcCtrl->agcPower.powerEnableMeasurement){
		/* Power Configuration register */
		halError = BZ_spiWriteField(hw_priv, REG_GC_18, 0,4,4);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_18, rxAgcCtrl->agcPower.powerInMux,9,8);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_18, rxAgcCtrl->agcPower.powerMeasurementDuration,3,0);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_19, rxAgcCtrl->agcPower.powerAttackHigStep,3,0);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_19, rxAgcCtrl->agcPower.powerAttackLowStep,7,4);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_19, rxAgcCtrl->agcPower.powerRecoveryHigStep,11,8);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_19, rxAgcCtrl->agcPower.powerRecoveryLowStep,15,12);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);	

		halError = BZ_spiWriteField(hw_priv, REG_GC_20, rxAgcCtrl->agcPower.powerOverHigThresh,13,0);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_20, rxAgcCtrl->agcPower.powerOverLowThresh,29,16);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
		
		halError = BZ_spiWriteField(hw_priv, REG_GC_21, rxAgcCtrl->agcPower.powerUnderHigThresh,13,0);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);

		halError = BZ_spiWriteField(hw_priv, REG_GC_21, rxAgcCtrl->agcPower.powerUnderLowThresh,29,16);
		bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
	}
    
	/*Enable AGC Mode*/
	halError = BZ_spiWriteField(hw_priv, REG_GC_01,0x1,0,0);
	bzRetAct = BZ_ApiErrHandler(hw_priv, BZ_ERRHDL_HAL_AGC, halError, bzRetAct, BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
    return (uint32_t)bzRetAct;
}

