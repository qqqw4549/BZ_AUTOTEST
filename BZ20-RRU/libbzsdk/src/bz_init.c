/**
* \file bz_init.c
* \brief B20 Init functions
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/
#include "bz_init.h"
#include "bz_plat_hal.h"
#include "bz_error.h"
#include "bz_riscv.h"
#include "bz_reg_dig.h"
#include "bz_reg_ana.h"
#include "bz_rx.h"
#include "bz_tx.h"


uint32_t BZ_openHw(BzDevice_t *hw_priv,uint32_t DeviceId)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    bzHalErr halStatus = BZHAL_OK;	
	
	/*inital Spi device*/
	halStatus=BZ_spiOpen(hw_priv,DeviceId);
	
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halStatus,bzRetAct,BZ_ERR_INIT_PLAT);
	IF_ERR_RETURN(bzRetAct);

	return (uint32_t)bzRetAct;
}
uint32_t BZ_closeHw(BzDevice_t *hw_priv)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    bzHalErr halStatus = BZHAL_OK;
	
	halStatus=BZ_spiClose(hw_priv);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halStatus,bzRetAct,BZ_ERR_INIT_PLAT);
	IF_ERR_RETURN(bzRetAct);
	/*Release hw_priv ressource*/
	
	return (uint32_t)bzRetAct;
}

uint32_t BZ_resetDevice(BzDevice_t *hw_priv)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    bzHalErr halstatus= BZHAL_OK;

	/*BBPLL_REG_RESETB_AAFC=1*/
	/*BBPLL_REG_RESETB_GEN=1*/
	/*BBPLL_REG_RESETB_BB=1*/
	/*BBPLL_REG_RESETB_RF=1*/
	/*BBPLL_REG_RESETB_1GHZ=1*/
	halstatus=BZ_spiWriteField(hw_priv,REG_BBPLL6,1,15,15);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halstatus=BZ_spiWriteField(hw_priv,REG_BBPLL6,0x0F,31,28);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*RFPLL_RESETB_SDM=1*/
	/*RFPLL_RESETB_AAC_AFC=1*/
	/*RFPLL_RESETB_SDMCTRL=1*/
	/*RFPLL_RESETB_RFSYNC=1*/
	halstatus=BZ_spiWriteField(hw_priv,REG_RFPLL10,0x0F,3,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*AUXPLL_RESETB_SDM=1*/
	/*AUXPLL_RESETB_AAC_AFC=1*/
	/*AUXPLL_RESETB_SDMCTRL=1*/
	/*AUXPLL_RESETB_RFSYNC=1*/
	halstatus=BZ_spiWriteField(hw_priv,REG_AUXPLL10,0x0F,3,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	//BBPLL_REF_DIV_RSTN = 0
	halstatus=BZ_spiWriteField(hw_priv,REG_BBPLL0,0,15,15);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	//BBPLL_REF_DIV_RSTN = 1
	halstatus=BZ_spiWriteField(hw_priv,REG_BBPLL0,1,15,15);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	//RFPLL_REF_DIV_RSTN = 0
	halstatus=BZ_spiWriteField(hw_priv,REG_RFPLL0,0,15,15);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	//RFPLL_REF_DIV_RSTN = 1
	halstatus=BZ_spiWriteField(hw_priv,REG_RFPLL0,1,15,15);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	//AUXPLL_REF_DIV_RSTN = 0
	halstatus=BZ_spiWriteField(hw_priv,REG_AUXPLL0,0,15,15);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	//AUXPLL_REF_DIV_RSTN = 1
	halstatus=BZ_spiWriteField(hw_priv,REG_AUXPLL0,1,15,15);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	//BBPLL_EN_CLK_500M = 0
	halstatus=BZ_spiWriteField(hw_priv,REG_BBPLL4,0,21,21);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	//BBPLL_EN_CLK_500M = 1
	halstatus=BZ_spiWriteField(hw_priv,REG_BBPLL4,1,21,21);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	//BBPLL_EN_CLK_SARADC = 0
	halstatus=BZ_spiWriteField(hw_priv,REG_BBPLL4,0,20,20);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	//BBPLL_EN_CLK_SARADC = 1
	halstatus=BZ_spiWriteField(hw_priv,REG_BBPLL4,1,20,20);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	halstatus=BZ_spiWriteField(hw_priv,REG_RFPLL11,1,10,10);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halstatus=BZ_spiWriteField(hw_priv,REG_AUXPLL11,1,10,10);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	halstatus=BZ_spiWriteField(hw_priv,REG_SYNC_0,1,29,29);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	/*apd_reset_ext=1*/
	halstatus=BZ_spiWriteField(hw_priv,REG_GC_50,1,8,8);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*RX1_PDET_PD_0=1*/
	/*RX1_PDET_PD_0=0*/
	halstatus=BZ_spiWriteField(hw_priv,REG_RXFE0,1,16,16);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halstatus=BZ_spiWriteField(hw_priv,REG_RXFE0,0,16,16);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*RX2_PDET_PD_0=1*/
	/*RX2_PDET_PD_0=0*/
	halstatus=BZ_spiWriteField(hw_priv,REG_RXFE8,1,16,16);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halstatus=BZ_spiWriteField(hw_priv,REG_RXFE8,0,16,16);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	/*apd_reset_ext=0*/
	halstatus=BZ_spiWriteField(hw_priv,REG_GC_50,0,8,8);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	halstatus=BZ_spiWriteField(hw_priv,REG_CLK_SWAP_0,0,2,2);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	halstatus=BZ_spiWriteField(hw_priv,REG_CLK_SWAP_0,1,2,2);	
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;
}

static uint32_t bz_verifyRxProfile(BzDevice_t *hw_priv, BzRxProfile_t *rxProfile, uint32_t *rxHsDigClk_kHz)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;

    *rxHsDigClk_kHz = 0;
	
    /********************************/
    /* Check for a valid Rx profile */
    /********************************/
    if ((rxProfile->rxOutputRate_kHz < 61440) ||
        (rxProfile->rxOutputRate_kHz > 245760))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
				BZ_ERR_VERTRXPFILE_RXINPUTRATE,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    if ((rxProfile->rfBandwidth_Hz < 50000000) ||
        (rxProfile->rfBandwidth_Hz > 200000000))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
				BZ_ERR_VERXPFILE_RFBAND,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    if ((rxProfile->rhb1Decimation != 1) &&
        (rxProfile->rhb1Decimation != 2))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
			BZ_ERR_VERRXPFILE_THB1,bzRetAct,BZ_ERR_CHECK_PARAM);
    }
    if ((rxProfile->rxFirDecimation != 1) &&
        (rxProfile->rxFirDecimation != 2) &&
        (rxProfile->rxFirDecimation != 4))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
			BZ_ERR_VERRXPFILE_RXFIR1,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    if ((rxProfile->rxFir.coefs == NULL) &&
        (rxProfile->rxFirDecimation != 1))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
			BZ_ERR_VERRXPFILE_RXFIR1,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    *rxHsDigClk_kHz = ( rxProfile->rxOutputRate_kHz * rxProfile->rxFirDecimation *
                        rxProfile->rhb1Decimation);

	return (uint32_t)bzRetAct;
}

static bzHalErr bz_verifyObsRxprofile(BzDevice_t *hw_priv,BzORxProfile_t *orxProfile, uint32_t *orxHsDigClk_kHz)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    *orxHsDigClk_kHz = 0;

    /********************************/
    /* Check for a valid ORx profile */
    /********************************/
    if ((orxProfile->orxOutputRate_kHz < 245760) ||
        (orxProfile->orxOutputRate_kHz > 491520))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
				BZ_ERR_VERTORXPFILE_RXINPUTRATE,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    if ((orxProfile->rfBandwidth_Hz < 50000000) ||
        (orxProfile->rfBandwidth_Hz > 450000000))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
				BZ_ERR_VEORXPFILE_RFBAND,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    if ((orxProfile->orxFirDecimation != 1) &&
        (orxProfile->orxFirDecimation != 2))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
			BZ_ERR_VERORXPFILE_FIR,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    if ((orxProfile->rxFir.coefs == NULL) &&
        (orxProfile->orxFirDecimation != 1))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
			BZ_ERR_VERORXPFILE_FIR1,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    *orxHsDigClk_kHz = (orxProfile->orxOutputRate_kHz * orxProfile->orxFirDecimation);
	
	return (uint32_t)bzRetAct;
}
/**
* Verify the TxProfile Valid,If the parameter is valid.
* Return BZ_NO_ACTION,otherwise return BZ_ERR_CHECK_PARAM
*/
static uint32_t bz_verifyTxProfile(BzDevice_t *hw_priv,BzTxProfile_t *txProfile, 
													uint32_t *txHsDigClk_kHz)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;

    *txHsDigClk_kHz = 0;
    /********************************/
    /* Check for a valid Tx profile */
    /********************************/

    if ((txProfile->txInputRate_kHz < 122880) ||
        (txProfile->txInputRate_kHz > 491520))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
				BZ_ERR_VERTXPFILE_TXINPUTRATE,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    if ((txProfile->rfBandwidth_Hz < 50000000) ||
        (txProfile->rfBandwidth_Hz > 450000000))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
			BZ_ERR_VERTXPFILE_RFBAND,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    if ((txProfile->thb1Interpolation != 1) &&
        (txProfile->thb1Interpolation != 2))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
			BZ_ERR_VERTXPFILE_THB1,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    if ((txProfile->thb2Interpolation != 1) &&
        (txProfile->thb2Interpolation != 2))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
			BZ_ERR_VERTXPFILE_THB2,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    if ((txProfile->thb3Interpolation != 1) &&
        (txProfile->thb3Interpolation != 2))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
			BZ_ERR_VERTXPFILE_THB3,bzRetAct,BZ_ERR_CHECK_PARAM);
    }
	
    if ((txProfile->txFirInterpolation != 1) &&
        (txProfile->txFirInterpolation != 2) &&
        (txProfile->txFirInterpolation != 4))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
			BZ_ERR_VERTXPFILE_TXFIR,bzRetAct,BZ_ERR_CHECK_PARAM);

    }

    if ((txProfile->txFir.coefs == NULL) &&
        (txProfile->txFirInterpolation != 1))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
			BZ_ERR_VERTXPFILE_TXFIR1,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    *txHsDigClk_kHz = (txProfile->txInputRate_kHz * txProfile->txFirInterpolation *
                       txProfile->thb1Interpolation * txProfile->thb2Interpolation *
                       txProfile->thb3Interpolation);
		
	return (uint32_t)bzRetAct;
}
uint32_t BZ_verifyProfiles(BzDevice_t *hw_priv, BzInit_t *init)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    uint32_t rxHsDigClk_kHz = 0;
    uint32_t orxHsDigClk_kHz = 0;
    uint32_t txHsDigClk_kHz = 0;
    BzRxProfile_t *rxProfile  = NULL;
    BzTxProfile_t *txProfile = NULL;
    BzORxProfile_t *orxProfile  = NULL;
	
	if(init==NULL){
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
					BZ_ERR_DEVICE_INIT_NULL_PARAM,bzRetAct,BZ_ERR_CHECK_PARAM);
	}

	hw_priv->BzState.gainIndexes.tx1MaxGainIndex = init->tx.txGainCtrl.tx1MaxGainIndex;
    hw_priv->BzState.gainIndexes.tx1MinGainIndex = init->tx.txGainCtrl.tx1MinGainIndex;
	hw_priv->BzState.gainIndexes.tx2MaxGainIndex = init->tx.txGainCtrl.tx2MaxGainIndex;
    hw_priv->BzState.gainIndexes.tx2MinGainIndex = init->tx.txGainCtrl.tx2MinGainIndex;

	hw_priv->BzState.gainIndexes.rx1MaxGainIndex = init->rx.rxGainCtrl.rx1MaxGainIndex;
    hw_priv->BzState.gainIndexes.rx1MinGainIndex = init->rx.rxGainCtrl.rx1MinGainIndex;
	hw_priv->BzState.gainIndexes.rx2MaxGainIndex = init->rx.rxGainCtrl.rx2MaxGainIndex;
    hw_priv->BzState.gainIndexes.rx2MinGainIndex = init->rx.rxGainCtrl.rx2MinGainIndex;

	hw_priv->BzState.gainIndexes.orx1MaxGainIndex = init->obsRx.orxGainCtrl.orx1MaxGainIndex;
    hw_priv->BzState.gainIndexes.orx1MinGainIndex = init->obsRx.orxGainCtrl.orx1MinGainIndex;
	hw_priv->BzState.gainIndexes.orx2MaxGainIndex = init->obsRx.orxGainCtrl.orx2MaxGainIndex;
    hw_priv->BzState.gainIndexes.orx2MinGainIndex = init->obsRx.orxGainCtrl.orx2MinGainIndex;
	
    hw_priv->BzState.profilesValid = 0;
	
    //if(init->tx.txChannels != BZ_TXOFF)
    {
        txProfile = &init->tx.txProfile;
        bzRetAct = bz_verifyTxProfile(hw_priv, txProfile, &txHsDigClk_kHz);
    }
    //if(init->rx.rxChannels != BZ_RXOFF)
    {
        rxProfile = &init->rx.rxProfile;
        bzRetAct = bz_verifyRxProfile(hw_priv, rxProfile, &rxHsDigClk_kHz);
    }
    //if((init->obsRx.obsRxChannelsEnable != BZ_ORXOFF) || (init->tx.txChannels != BZ_TXOFF))
    {
        orxProfile = &init->obsRx.orxProfile;
        bzRetAct = bz_verifyObsRxprofile(hw_priv, orxProfile, &orxHsDigClk_kHz);
        IF_ERR_RETURN(bzRetAct);
    }
    if ((init->clocks.rfPllPhaseSyncMode != BZ_RFPLLMCS_NOSYNC)
        && (init->clocks.rfPllPhaseSyncMode != BZ_RFPLLMCS_INIT_AND_SYNC)
        && (init->clocks.rfPllPhaseSyncMode != BZ_RFPLLMCS_INIT_AND_1TRACK)
        && (init->clocks.rfPllPhaseSyncMode != BZ_RFPLLMCS_INIT_AND_CONTTRACK))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
				BZ_ERR_CLOCK_RFPLL_PHASE,bzRetAct,BZ_ERR_RESET_SPI);
    }
	/* Verify Tx profile is valid */
    if (txHsDigClk_kHz > 0)
    {
        hw_priv->BzState.profilesValid|= TX_PROFILE_VALID;
    }
    /* Verify Rx profile is valid */
    if (rxHsDigClk_kHz > 0)
    {
        hw_priv->BzState.profilesValid |= RX_PROFILE_VALID;
    }
    /* Verify ORx profile is valid */
    if (orxHsDigClk_kHz > 0)
    {
         hw_priv->BzState.profilesValid |= ORX_PROFILE_VALID;
    }
	
	return (uint32_t)bzRetAct;
}


uint32_t BZ_programFir(BzDevice_t *hw_priv,BzFir_t *filter, int FirSel)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    uint32_t i = 0,j=0, curpos = 0;
    int len;
	BzData DataInfo ={0};
	BzMem *pMsg=NULL;

	pMsg=&DataInfo.u.Mmsg;

	DataInfo.CmdType=0xff;
	pMsg->Cmd=BZ_WRITE_MEM_OPCODE;
	pMsg->Msg.Id=HI_MSG_TO_FILTER_MSGID;
	
    for (i = 0; i < 4; i++)
    {
        pMsg->Msg.data[i] = (uint8_t)(((filter->gain_dB) >> (i * 8)) & 0x000000FF);
    }
    for (i = 4; i < 8; i++)
    {
        pMsg->Msg.data[i] = (uint8_t)(((filter->numFirCoefs) >> ((i - 4) * 8)) & 0x000000FF);
    }
    
    
    if(FirSel == BZ_TX_FILTER)
    {
    	pMsg->Msg.offset=0;             
        pMsg->Msg.Lenth=HI_MSG_TO_TXFILTER_MSGLEN;
        len = HI_MSG_TO_TXFILTER_COEFS_LEN; 
    }
    else if(FirSel == BZ_RX_FILTER)
    {
    	pMsg->Msg.offset=HI_MSG_TO_TXFILTER_MSGLEN;
        pMsg->Msg.Lenth=HI_MSG_TO_RX_ORXFILTER_MSGLEN;
        len = HI_MSG_TO_RX_ORX_FILTER_COEFS_LEN; 
    }else{
    	pMsg->Msg.offset=HI_MSG_TO_TXFILTER_MSGLEN+HI_MSG_TO_RX_ORXFILTER_MSGLEN;
        pMsg->Msg.Lenth=HI_MSG_TO_RX_ORXFILTER_MSGLEN;
        len = HI_MSG_TO_RX_ORX_FILTER_COEFS_LEN; 
	}
    curpos = i;
    for(i = 0; i < len; i++)
    {
        for(j = 0; j < 2; j++)
        {
            pMsg->Msg.data[curpos] = (uint8_t)(((filter->coefs[i]) >> (j * 8)) & 0x000000FF);
            curpos++;
        }
    }
	bzRetAct = BZ_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;
}

uint32_t BZ_initialize(BzDevice_t *hw_priv,BzInit_t *Init)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    bzHalErr halstatus = BZHAL_OK;
	
	bzInfo_t *pStateInfo = &hw_priv->BzState;
	pStateInfo->devState=BZ_STATE_INITIALIZED;
	
	bzRetAct=BZ_verifyProfiles(hw_priv,Init);
    IF_ERR_RETURN(bzRetAct);

    /* Program FIR filters */
    if (pStateInfo->profilesValid & TX_PROFILE_VALID){
        if (Init->tx.txProfile.txFir.numFirCoefs > 0)
        {
            bzRetAct =BZ_programFir(hw_priv,&Init->tx.txProfile.txFir,BZ_TX_FILTER);
            IF_ERR_RETURN(bzRetAct);
            
            bzRetAct = BZ_setTxManualGain(hw_priv, BZ_TX1, Init->tx.txGainCtrl.tx1GainIndex);
            IF_ERR_RETURN(bzRetAct);

            bzRetAct = BZ_setTxManualGain(hw_priv, BZ_TX2, Init->tx.txGainCtrl.tx2GainIndex);
            IF_ERR_RETURN(bzRetAct);
	
            bzRetAct=BZ_setTxGainControlMode(hw_priv,Init->tx.txGainCtrl.txgainMode);
            IF_ERR_RETURN(bzRetAct);
        }       
	}
    if (pStateInfo->profilesValid & RX_PROFILE_VALID)
    {
        if (Init->rx.rxProfile.rxFir.numFirCoefs > 0)
        {
            bzRetAct = BZ_programFir(hw_priv,&Init->rx.rxProfile.rxFir,BZ_RX_FILTER);
            IF_ERR_RETURN(bzRetAct);

            bzRetAct =BZ_setRxManualGain(hw_priv, BZ_RX1, Init->rx.rxGainCtrl.rx1GainIndex);
            IF_ERR_RETURN(bzRetAct);

            bzRetAct = BZ_setRxManualGain(hw_priv, BZ_RX2, Init->rx.rxGainCtrl.rx2GainIndex);
            IF_ERR_RETURN(bzRetAct);
			
			bzRetAct=BZ_setRxGainControlMode(hw_priv,Init->rx.rxGainCtrl.rxgainMode,Init->rx.rxGainCtrl.rxmgcMode);           
            IF_ERR_RETURN(bzRetAct);
        }
    }

    if (pStateInfo->profilesValid & ORX_PROFILE_VALID)
    {
        if (Init->obsRx.orxProfile.rxFir.numFirCoefs > 0)
        {
        	/* if pointer to orx rxFIR is valid */
            bzRetAct = BZ_programFir(hw_priv,&Init->obsRx.orxProfile.rxFir,BZ_ORX_FILTER);
            IF_ERR_RETURN(bzRetAct);

            bzRetAct = BZ_setObsRxManualGain(hw_priv, BZ_ORX1, Init->obsRx.orxGainCtrl.orx1GainIndex);
            IF_ERR_RETURN(bzRetAct);

            bzRetAct = BZ_setObsRxManualGain(hw_priv, BZ_ORX2, Init->obsRx.orxGainCtrl.orx2GainIndex);
            IF_ERR_RETURN(bzRetAct);
			
			bzRetAct=BZ_setObsRxGainControlMode(hw_priv,Init->obsRx.orxGainCtrl.orxgainMode,Init->obsRx.orxGainCtrl.orxmgcMode);
            IF_ERR_RETURN(bzRetAct);
        }
    }
	pStateInfo->clocks.deviceClock_kHz=Init->clocks.deviceClock_kHz;
    pStateInfo->supportChannels =  (Init->tx.txChannels & 3);
    pStateInfo->supportChannels |= ((Init->rx.rxChannels & 3) << 2);
    pStateInfo->supportChannels |= ((Init->obsRx.obsRxChannelsEnable & 3) << 4);
    pStateInfo->txInputRate_kHz = Init->tx.txProfile.txInputRate_kHz;

    pStateInfo->rxBandwidth_Hz = Init->rx.rxProfile.rfBandwidth_Hz;
    pStateInfo->txBandwidth_Hz = Init->tx.txProfile.rfBandwidth_Hz;
    pStateInfo->orxBandwidth_Hz = Init->obsRx.orxProfile.rfBandwidth_Hz;
	return (uint32_t)halstatus;
}

uint32_t BZ_enableTrackingCals(BzDevice_t *hw_priv,uint32_t enableMask)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	return (uint32_t)bzRetAct;
}

uint32_t BZ_getApiVersion(uint32_t ApiVer)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	return (uint32_t)bzRetAct;
}
uint32_t BZ_getDeviceRev(uint32_t DevVer)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	return (uint32_t)bzRetAct;
}
uint32_t BZ_getProductId(uint32_t ProId)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	return (uint32_t)bzRetAct;
}
