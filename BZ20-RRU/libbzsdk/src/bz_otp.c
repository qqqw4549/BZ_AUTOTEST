/**
* \file bz_otp.c
* \brief Knowledge of the otp on bz transfer device
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/
#include "bz_otp.h"
#include "bz_plat_hal.h"
#include "bz_error.h"
#include "bz_riscv_types.h"

/*Read OTP Mode.The Highest Clk is 20MHZ,High and Low level still 20ns at less */
uint32_t BZ_OtpRead(BzDevice_t *hw_priv,uint32_t OtpAddr,uint32_t *Data)
{
	uint32_t OptCmd=0;
	uint32_t status=0;
    uint32_t uiTry = WIAT_TIMEOUT;
	bzHalErr halStatus = BZHAL_OK;
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	
	/*construct otp_cmd   [17:8]->otpaddr */
	OptCmd|=((OtpAddr&0x3FF)<<8);
	/*construct otp_cmd   [6]->PAS_EN */
	//OptCmd|=(PAS_EN<<6);
	/*construct otp_cmd   [5]->TEST_ROW_VALID */
	//OptCmd|=(TEST_ROW_VALID<<5);
	/*construct otp_cmd   [4]->CELL_VALID */
	//OptCmd|=(CELL_VALID<<4);
	/*construct otp_cmd   [3]->CMD_VALID */
	OptCmd|=(CMD_VALID<<3);
	/*construct otp_cmd   [2:0]->OTP_CMD_VLD */
	OptCmd|=(BZ_READ_MODE<<0);
	
	/*First Write otp cmd*/
    while(uiTry > 0){
		halStatus = BZ_spiReadReg(hw_priv,BZ_REG_OTPOUT_STA_ADDR,&status);	
		bzRetAct = BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_OTP,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
		if((status&0x40)!=0x40){
			break;
		}
		BZHAL_Printf("status=%x\n",status);
        uiTry--;
	}
    if(uiTry <= 0){
		BZHAL_Printf("ERROR:%s Trytimeout!\n", __func__);
	}
	halStatus = BZ_spiWriteReg(hw_priv,BZ_REG_OTPIN_CMD_ADDR,OptCmd);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_OTP,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	/*second Read otp data*/
	halStatus = BZ_spiReadReg(hw_priv,BZ_REG_OTPOUT_DATA_ADDR,Data);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_OTP,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	return 0;
	
}
uint32_t BZ_OtpWrite(BzDevice_t *hw_priv,uint32_t OtpAddr,uint32_t Data)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halStatus = BZHAL_OK;
	uint32_t OptCmd=0;
	
	/*construct otp_cmd   [17:8]->otpaddr */
	OptCmd|=((OtpAddr&0x3FF)<<8);
	/*construct otp_cmd   [6]->PAS_EN */
	//OptCmd|=(PAS_EN<<6);
	/*construct otp_cmd   [5]->TEST_ROW_VALID */
	//OptCmd|=(TEST_ROW_VALID<<5);
	/*construct otp_cmd   [4]->CELL_VALID */
	//OptCmd|=(CELL_VALID<<4);
	/*construct otp_cmd   [3]->CMD_VALID */
	OptCmd|=(CMD_VALID<<3);
	/*construct otp_cmd   [2:0]->OTP_CMD_VLD */
	OptCmd|=(BZ_PROG_MODE<<0);
	
	/*second Write otp data*/
	halStatus = BZ_spiWriteReg(hw_priv,BZ_REG_OTPIN_DATA_ADDR,Data);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_OTP,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	/*First Write otp cmd*/
	halStatus = BZ_spiWriteReg(hw_priv,BZ_REG_OTPIN_CMD_ADDR,OptCmd);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_OTP,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	return 0;
}

