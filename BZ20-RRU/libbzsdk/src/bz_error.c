/**
* \file bz_error.h
* \brief Contains B20 API error information functions
 *        These functions are public to the customer for getting more details on
 *        errors and debugging.
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/
#include "bz_error.h"
#include "bz_plat_hal.h"


static const char* bz_getErrorMessage(uint32_t errSrc, uint32_t errCode)
{
	switch(errSrc){
		case BZ_ERRHDL_HAL_INIT:
			if(errCode==BZHAL_OK){
				return "INIT->SPI_OK";
			}else{
				return "INIT->SPI_FAIL";
			}
			break;
		case BZ_ERRHDL_HAL_AGC:
			if(errCode==BZHAL_OK){
				return "AGC->SPI_OK";
			}else{
				return "AGC->SPI_FAIL";
			}
			break;
		case BZ_ERRHDL_HAL_JES204B:
			if(errCode==BZHAL_OK){
				return "JESD->SPI_OK";
			}else{
				return "JESD->SPI_FAIL";
			}
			break;
		case BZ_ERRHDL_HAL_OTP:
			if(errCode==BZHAL_OK){
				return "OTP->SPI_OK";
			}else{
				return "OTP->SPI_FAIL";
			}
			break;	
		case BZ_ERRHDL_HAL_RADIO:
			if(errCode==BZHAL_OK){
				return "RADIO->SPI_OK";
			}else{
				return "RADIO->SPI_FAIL";
			}
			break;
		case BZ_ERRHDL_HAL_RISCV:
			if(errCode==BZHAL_OK){
				return "RISCV->SPI_OK";
			}else{
				return "RISCV->SPI_FAIL";
			}
			break;
		case BZ_ERRHDL_HAL_RX:
			if(errCode==BZHAL_OK){
				return "RX->SPI_OK";
			}else{
				return "RX->SPI_FAIL";
			}
			break;
		case BZ_ERRHDL_HAL_TX:
			if(errCode==BZHAL_OK){
				return "TX->SPI_OK";
			}else{
				return "TX->SPI_FAIL";
			}
			break;
		case BZ_ERRHDL_HAL_GPIO:
			if(errCode==BZHAL_OK){
				return "GPIO->SPI_OK";
			}else{
				return "GPIO->SPI_FAIL";
			}
			break;
			default:
				break;
	}
	return NULL;
}

BzRecoveryActions_t BZ_ApiErrHandler(BzDevice_t *hw_priv, BzErrHdls_t errHdl,uint32_t detErr, BzRecoveryActions_t retVal,BzRecoveryActions_t recAction)
{
	if(detErr!=BZ_ERR_OK){
	    switch (errHdl)
	    {
	        case BZ_ERRHDL_HAL_INIT:
				/*Ignore Error from log as lower priority than error being handled*/
				BZ_WriteToLog(hw_priv, BZHAL_LOG_ERR, detErr,bz_getErrorMessage(BZ_ERRHDL_HAL_INIT, detErr));
				hw_priv->BzState.errSource = BZ_ERRHDL_HAL_INIT;
				return recAction;
				break;
			case BZ_ERRHDL_HAL_AGC:
				BZ_WriteToLog(hw_priv,BZHAL_LOG_ERR,detErr,bz_getErrorMessage(BZ_ERRHDL_HAL_AGC,detErr));
				hw_priv->BzState.errSource = BZ_ERRHDL_HAL_AGC;
				break;
			case BZ_ERRHDL_HAL_JES204B: 
				 BZ_WriteToLog(hw_priv, BZHAL_LOG_ERR, detErr,bz_getErrorMessage(BZ_ERRHDL_HAL_JES204B, detErr));
				 hw_priv->BzState.errSource = BZ_ERRHDL_HAL_JES204B;
				 break;
			case BZ_ERRHDL_HAL_OTP:			
				BZ_WriteToLog(hw_priv, BZHAL_LOG_ERR, detErr,bz_getErrorMessage(BZ_ERRHDL_HAL_OTP, detErr));
				hw_priv->BzState.errSource = BZ_ERRHDL_HAL_OTP;
				break;
			case BZ_ERRHDL_HAL_RADIO:			
				BZ_WriteToLog(hw_priv, BZHAL_LOG_ERR, detErr,bz_getErrorMessage(BZ_ERRHDL_HAL_RADIO, detErr));
				hw_priv->BzState.errSource = BZ_ERRHDL_HAL_OTP;
				break;
			case BZ_ERRHDL_HAL_RISCV:			
				BZ_WriteToLog(hw_priv, BZHAL_LOG_ERR, detErr,bz_getErrorMessage(BZ_ERRHDL_HAL_RISCV, detErr));
				hw_priv->BzState.errSource = BZ_ERRHDL_HAL_OTP;
				break;
			case BZ_ERRHDL_HAL_RX:			
				BZ_WriteToLog(hw_priv, BZHAL_LOG_ERR, detErr,bz_getErrorMessage(BZ_ERRHDL_HAL_RX, detErr));
				hw_priv->BzState.errSource = BZ_ERRHDL_HAL_OTP;
				break;
			case BZ_ERRHDL_HAL_TX:			
				BZ_WriteToLog(hw_priv, BZHAL_LOG_ERR, detErr,bz_getErrorMessage(BZ_ERRHDL_HAL_TX, detErr));
				hw_priv->BzState.errSource = BZ_ERRHDL_HAL_OTP;
				break;
			case BZ_ERRHDL_HAL_GPIO:			
				BZ_WriteToLog(hw_priv, BZHAL_LOG_ERR, detErr,bz_getErrorMessage(BZ_ERRHDL_HAL_GPIO, detErr));
				hw_priv->BzState.errSource = BZ_ERRHDL_HAL_OTP;
				break;
		     default:
	           break;
	    }
    }
	hw_priv->BzState.errCode = detErr;
	return retVal;
}
