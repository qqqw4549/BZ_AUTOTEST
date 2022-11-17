/**
* \file bz_gpio.c
* \brief B20 GPIO functions
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/
#include "bz_gpio.h"
#include "bz_error.h"
#include "bz_plat_hal.h"
#include "bz_reg_dig.h"
#include "bz_reg_ana.h"
#include "bz_riscv_types.h"
#include "bz_riscv.h"
#include "bz_tx.h"


/** The paramter Value-->1-->high**/
/** otherwise 	 Value-->0-->low **/
/** gpioNum from 0->9 **/
uint32_t  BZ_setGpioValue(BzDevice_t *hw_priv, BzGpioPinSel_t gpioNum, uint32_t value)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;	
	BzData DataInfo;

	if((gpioNum < BZ_GPIO_00)||(gpioNum > BZ_GPIO_09))
	{		
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
			BZ_ERR_GPIO_NUM_VALID,bzRetAct,BZ_ERR_CHECK_PARAM);
	}

	/*gpioCtrl -->0 Ctrl by riscv, 1 Ctrl by spi*/
	BZ_setGpioSourceCtrl(hw_priv, 0, gpioNum);
	IF_ERR_RETURN(bzRetAct);
	
	/*BZ_SEL_5 -->set gpio functiong to gpio*/
	BZ_setGpioFuncSel(hw_priv, BZ_SEL_5, gpioNum);
	IF_ERR_RETURN(bzRetAct);

	BZ_Memset(&DataInfo,0,sizeof(BzData));
	DataInfo.CmdType=0xff;
	DataInfo.u.Tx.Cmd=BZ_SET_OPCODE;
	DataInfo.u.Tx.TxData.value=HI_SET_GPIO_VALUE|(value<<31|gpioNum<<16);
	bzRetAct=BZ_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;
}

uint32_t BZ_waitGetGpioValueDone(BzDevice_t *hw_priv, uint32_t timeout)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
    uint32_t uiTry = WIAT_TIMEOUT;
	
	/*Wait Get Gpio Value Done*/
    while(uiTry > 0){
		BZ_mdelay(timeout);
		bzRetAct=BZ_spiGetCmdStatus(hw_priv);
		IF_ERR_RETURN(bzRetAct);
		if(hw_priv->gpio_get_value_done==1){
			BZ_clearStatus(hw_priv);
			hw_priv->gpio_get_value_done=0;
			break;
		}
        uiTry--;
	}
    if(uiTry <= 0){
		BZHAL_Printf("ERROR:%s Trytimeout!\n", __func__);
    }

	return (uint32_t)bzRetAct;
}


/** The paramter  gpioNum from 0->9 **/
/** value output value **/
uint32_t  BZ_getGpioValue(BzDevice_t *hw_priv, BzGpioPinSel_t gpioNum, uint32_t *value)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;	
	BzData DataInfo;
	
	if((gpioNum < BZ_GPIO_00)||(gpioNum > BZ_GPIO_09))
	{		
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
			BZ_ERR_GPIO_NUM_VALID,bzRetAct,BZ_ERR_CHECK_PARAM);
	}

	/*gpioCtrl -->0 Ctrl by riscv, 1 Ctrl by spi*/
	BZ_setGpioSourceCtrl(hw_priv, 0, gpioNum);
	IF_ERR_RETURN(bzRetAct);
	
	/*BZ_SEL_5 -->set gpio functiong to gpio*/
	BZ_setGpioFuncSel(hw_priv, BZ_SEL_5, gpioNum);
	IF_ERR_RETURN(bzRetAct);

	BZ_Memset(&DataInfo,0,sizeof(BzData));
	DataInfo.CmdType=0xff;
	DataInfo.u.Tx.Cmd=BZ_SET_OPCODE;
	DataInfo.u.Tx.TxData.value=HI_GET_GPIO_VALUE|gpioNum<<16;
	bzRetAct=BZ_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(bzRetAct);

	bzRetAct=BZ_waitGetGpioValueDone(hw_priv, 100);
	IF_ERR_RETURN(bzRetAct);
	
	bzRetAct=BZ_spiReadField(hw_priv,REG_GPIO_CTRL_0,value,31,31);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;
}

/** The paramter gpioOutEn-->1-->output**/
/** otherwise gpioOutEn-->0-->input **/
/** gpioNum from 0->18 **/
uint32_t  BZ_setGpioDirection(BzDevice_t *hw_priv, BzGpioDir_t gpioOutEn, BzGpioPinSel_t gpioNum)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;
	uint32_t GpioMap_RegBit;

	if((gpioNum >= BZ_GPIO_00)&&(gpioNum <= BZ_GPIO_09))
	{
		GpioMap_RegBit=gpioNum;
	}
	else if((gpioNum >= BZ_GPIO_10)&&(gpioNum <= BZ_GPIO_16))
	{
		GpioMap_RegBit=gpioNum + BZ_GPIO_10;
	}
	else
	{
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
			BZ_ERR_GPIO_NUM_VALID,bzRetAct,BZ_ERR_CHECK_PARAM);
	}

    /* B20 SPI regs to set GPIO OE direction */
    halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_0,gpioOutEn,GpioMap_RegBit,GpioMap_RegBit);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;
}

uint32_t  BZ_getGpioDirection(BzDevice_t *hw_priv, BzGpioDir_t *gpioOutEn, BzGpioPinSel_t gpioNum)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;
	uint32_t GpioMap_RegBit;
	
	if((gpioNum >= BZ_GPIO_00)&&(gpioNum <= BZ_GPIO_09))
	{
		GpioMap_RegBit=gpioNum;
	}
	else if((gpioNum >= BZ_GPIO_10)&&(gpioNum <= BZ_GPIO_16))
	{
		GpioMap_RegBit=gpioNum + BZ_GPIO_10;
	}
	else
	{
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
				BZ_ERR_GPIO_NUM_VALID,bzRetAct,BZ_ERR_CHECK_PARAM);
	}

    /* B20 SPI regs to set GPIO OE direction */
    halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_0,(uint32_t*)gpioOutEn,GpioMap_RegBit,GpioMap_RegBit);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;
}
/** The paramter gpioCtrlMode-->1--> spi**/
/** otherwise gpioNum-->0-->riscv **/
/** gpioNum from 0->18 **/

uint32_t  BZ_setGpioSourceCtrl(BzDevice_t *hw_priv, BzGpioCtrlMode_t gpioCtrlMode, BzGpioPinSel_t gpioNum)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;
	uint32_t GpioMap_RegBit;
	
	if((gpioNum >= BZ_GPIO_00)&&(gpioNum <= BZ_GPIO_09))
	{
		GpioMap_RegBit=(gpioNum+10);

		/* B20 SPI regs to set GPIO Ctrl */
    	halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_0,(uint32_t)gpioCtrlMode,GpioMap_RegBit,GpioMap_RegBit);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
	}else if((gpioNum >= BZ_GPIO_10)&&(gpioNum <= BZ_GPIO_16)){
		//Gpio10-Gpio16 cant be controled by riscv,it is controled by spi default
	}else{
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
			BZ_ERR_GPIO_NUM_VALID,bzRetAct,BZ_ERR_CHECK_PARAM);
	}

	return (uint32_t)bzRetAct;
}

uint32_t  BZ_getGpioSourceCtrl(BzDevice_t *hw_priv, BzGpioCtrlMode_t *gpioCtrlMode, BzGpioPinSel_t gpioNum)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;
	uint32_t GpioMap_RegBit;
	GpioMap_RegBit=(gpioNum+10);
    /* B20 SPI regs to get GPIO Ctrl */
    halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_0,(uint32_t*)gpioCtrlMode,GpioMap_RegBit,GpioMap_RegBit);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;
}

/** The paramter gpioFuncSel-->1--> spi**/
/** otherwise gpioNum-->0-->riscv **/
/** gpioNum from 0->18 **/
uint32_t  BZ_setGpioFuncSel(BzDevice_t *hw_priv, BzGpioPinFunc_t gpioFuncSel, BzGpioPinSel_t gpioNum)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;
	
	switch(gpioNum){
		case BZ_GPIO_00: 
			halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,2,0);
			break;
		case BZ_GPIO_01: 
			halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,5,3);
			break;
		case BZ_GPIO_02: 
			halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,8,6);
			break;
		case BZ_GPIO_03: 
			halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,11,9);
			break;
		case BZ_GPIO_04: 
			halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,14,12);
			break;
		case BZ_GPIO_05: 
			halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,17,15);
			break;
		case BZ_GPIO_06: 
			halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,20,18);
			break;
		case BZ_GPIO_07: 
			halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,23,21);
			break;
		case BZ_GPIO_08: 
			halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,26,24);
			break;
		case BZ_GPIO_09: 
			halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,29,27);
			break;
		case BZ_GPIO_10: 
			halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_2,(uint32_t)gpioFuncSel,2,0);
			break;
		case BZ_GPIO_11: 
			halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_2,(uint32_t)gpioFuncSel,5,3);
			break;
		case BZ_GPIO_12: 
			halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_2,(uint32_t)gpioFuncSel,8,6);
			break;
		case BZ_GPIO_13: 
			halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_2,(uint32_t)gpioFuncSel,11,9);
			break;
		case BZ_GPIO_14: 
			halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_2,(uint32_t)gpioFuncSel,14,12);
			break;
		case BZ_GPIO_15: 
			halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_2,(uint32_t)gpioFuncSel,17,15);
			break;
		case BZ_GPIO_16: 
			halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_CTRL_2,(uint32_t)gpioFuncSel,20,18);
			break;		
		default:
			break;
	}
	
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;
}

uint32_t  BZ_getGpioFuncSel(BzDevice_t *hw_priv, BzGpioPinFunc_t *gpioFuncSel, BzGpioPinSel_t gpioNum)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;
	
	switch(gpioNum){
		case BZ_GPIO_00: 
			halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,2,0);
			break;
		case BZ_GPIO_01: 
			halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,5,3);
			break;
		case BZ_GPIO_02: 
			halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,8,6);
			break;
		case BZ_GPIO_03: 
			halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,11,9);
			break;
		case BZ_GPIO_04: 
			halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,14,12);
			break;
		case BZ_GPIO_05: 
			halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,17,15);
			break;
		case BZ_GPIO_06: 
			halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,20,18);
			break;
		case BZ_GPIO_07: 
			halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,23,21);
			break;
		case BZ_GPIO_08: 
			halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,26,24);
			break;
		case BZ_GPIO_09: 
			halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,29,27);
			break;		
		case BZ_GPIO_10: 
			halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_2,(uint32_t*)gpioFuncSel,2,0);
			break;
		case BZ_GPIO_11: 
			halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_2,(uint32_t*)gpioFuncSel,5,3);
			break;
		case BZ_GPIO_12: 
			halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_2,(uint32_t*)gpioFuncSel,8,6);
			break;
		case BZ_GPIO_13: 
			halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_2,(uint32_t*)gpioFuncSel,11,9);
			break;
		case BZ_GPIO_14: 
			halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_2,(uint32_t*)gpioFuncSel,14,12);
			break;
		case BZ_GPIO_15: 
			halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_2,(uint32_t*)gpioFuncSel,17,15);
			break;
		case BZ_GPIO_16: 
			halstatus = BZ_spiReadField(hw_priv,REG_GPIO_CTRL_2,(uint32_t*)gpioFuncSel,20,18);
			break;		
		default:
			break;
	}
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;
}

uint32_t  BZ_setGpio3v3Oe(BzDevice_t *hw_priv, uint32_t gpio3v3OutEn, BzGpioPinSel_t gpioNum)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;

	uint32_t GpioMap5_0=0;
	uint32_t GpioMap11_6=16;
    /* B20 SPI regs to set GPIO 3V3 OE direction */
	/*Gpio11-6 output enable*/
	if(gpioNum>=6){
	    halstatus = BZ_spiWriteField(hw_priv,REG_MISC_TOP2,gpio3v3OutEn,GpioMap5_0+gpioNum,GpioMap5_0+gpioNum);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
	}
	/*Gpio5-0 output enable*/
	if(gpioNum<6){
	    halstatus = BZ_spiWriteField(hw_priv,REG_MISC_TOP4,gpio3v3OutEn,GpioMap11_6+gpioNum,GpioMap11_6+gpioNum);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
	}
	
	return (uint32_t)bzRetAct;
}

uint32_t  BZ_getGpio3v3Oe(BzDevice_t *hw_priv, uint32_t *gpio3v3OutEn)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;

	uint32_t Gpio5_0Mask=0;
	uint32_t Gpio11_6Mask=0;
    /* B20 SPI regs to get GPIO 3V3 OE direction */
	
	/*Gpio11-6 output enable*/
    halstatus = BZ_spiReadField(hw_priv,REG_MISC_TOP2,&Gpio5_0Mask,5,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*Gpio5-0 output enable*/
    halstatus = BZ_spiReadField(hw_priv,REG_MISC_TOP4,&Gpio11_6Mask,21,16);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	*gpio3v3OutEn=(Gpio5_0Mask|Gpio11_6Mask<<6);
	return (uint32_t)bzRetAct;
}

uint32_t  BZ_setGpio3v3Ie(BzDevice_t *hw_priv, uint32_t gpio3v3InEn, BzGpioPinSel_t gpioNum)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;
	uint32_t GpioMap5_0=0;
	uint32_t GpioMap11_6=24;
	
	/*Gpio11-6 input enable*/
	if(gpioNum>=6){
		halstatus = BZ_spiWriteField(hw_priv,REG_MISC_TOP2,gpio3v3InEn,GpioMap5_0+gpioNum,GpioMap5_0+gpioNum);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
	}
	/*Gpio5-0 input enable*/
	if(gpioNum<6){
		halstatus = BZ_spiWriteField(hw_priv,REG_MISC_TOP4,gpio3v3InEn,GpioMap11_6+gpioNum,GpioMap11_6+gpioNum);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
	}
	return (uint32_t)bzRetAct;
}

uint32_t BZ_getGpIntStatus(BzDevice_t *hw_priv,BzGpIntMask_t *gpIntStatus)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;
	
	halstatus = BZ_spiReadReg(hw_priv,DOCMD_STATUS,(uint32_t *)gpIntStatus);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	return (uint32_t)bzRetAct;
}
uint32_t BZ_getTemperature(BzDevice_t *hw_priv,uint32_t* temperatureDegc)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;

	return (uint32_t)bzRetAct;
}

uint32_t  BZ_getGpio3v3Ie(BzDevice_t *hw_priv, uint32_t *gpio3v3InEn)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;
	
	uint32_t Gpio5_0Mask=0;
	uint32_t Gpio11_6Mask=0;
    /* B20 SPI regs to get GPIO 3V3 IE direction */	
	
	/*Gpio11-6 input enable*/
	halstatus = BZ_spiReadField(hw_priv,REG_MISC_TOP2,&Gpio5_0Mask,13,8);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	/*Gpio5-0 input enable*/
	halstatus = BZ_spiReadField(hw_priv,REG_MISC_TOP4,&Gpio11_6Mask,29,24);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	*gpio3v3InEn=(Gpio5_0Mask|Gpio11_6Mask<<6);
	return (uint32_t)bzRetAct;
	
}

uint32_t BZ_setupAuxDacs(BzDevice_t *hw_priv,BzGpioPinSel_t Gpio3V3Num)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;
	
    uint32_t auxDacCnf = 0x00;
    uint32_t auxDacGPIOCnf = 0x00;
    uint32_t auxDacPowerCnf = 0x00;
    uint32_t auxDacModeCnf = 0x00;

	if(Gpio3V3Num<=5)
	{
		//for(i=0;i<5;i++)
		{	
			halstatus = BZ_spiReadReg(hw_priv,REG_MISC_TOP4,&auxDacGPIOCnf);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			/*gpio input mode disable*/
			auxDacGPIOCnf&=~BIT(Gpio3V3Num+24);
			/*gpio output mode disable*/
			auxDacGPIOCnf&=~BIT(Gpio3V3Num+16);
			
			halstatus = BZ_spiReadReg(hw_priv,REG_MISC_TOP2,&auxDacPowerCnf);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			
			/*auxdac top bias powerup*/
			auxDacPowerCnf&=~BIT(25);
			/*auxdac powerup*/
			auxDacPowerCnf&=~BIT(Gpio3V3Num+16);
			
			halstatus = BZ_spiReadReg(hw_priv,REG_MISC_TOP3,&auxDacModeCnf);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);

			/*vref set to 1.5v seting*/
			auxDacModeCnf|=(BZ_AUXDACVREF_1P5V<<(Gpio3V3Num*2));
			/*auxdac rgain/10bit mode*/
			auxDacModeCnf &=~BIT(Gpio3V3Num+16);
			/*auxdac ictl/10bit mode*/
			auxDacModeCnf |=BIT(Gpio3V3Num+22);
		
			if(Gpio3V3Num<5)
			{
				halstatus = BZ_spiWriteReg(hw_priv,REG_MISC_TOP4,auxDacGPIOCnf);
				bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);
			
				halstatus = BZ_spiWriteReg(hw_priv,REG_MISC_TOP2,auxDacPowerCnf);
				bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);

				
				halstatus = BZ_spiWriteReg(hw_priv,REG_MISC_TOP3,auxDacModeCnf);
				bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);
				
			}else{
				halstatus = BZ_spiReadReg(hw_priv,REG_MISC_TOP4,&auxDacCnf);
				bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);
				
				auxDacCnf|=(BZ_AUXDACVREF_1P5V);
				auxDacCnf&=~BIT(12);
				auxDacCnf|=BIT(5);
				halstatus = BZ_spiWriteReg(hw_priv,REG_MISC_TOP4,auxDacCnf);
				bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);
			}
		}
	}else{
		//for(i=0;i<5;i++)
		{	
			halstatus = BZ_spiReadReg(hw_priv,REG_MISC_TOP2,&auxDacGPIOCnf);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			/*gpio input mode disable*/
			auxDacGPIOCnf&=~BIT(Gpio3V3Num+2);
			/*gpio output mode disable*/
			auxDacGPIOCnf&=~BIT(Gpio3V3Num-6);
			
			halstatus = BZ_spiReadReg(hw_priv,REG_MISC_TOP0,&auxDacPowerCnf);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			/*auxdac top bias powerup*/
			auxDacPowerCnf&=~BIT(9);
			/*auxdac powerup*/
			auxDacPowerCnf&=~BIT(Gpio3V3Num-6);
		
			halstatus = BZ_spiReadReg(hw_priv,REG_MISC_TOP1,&auxDacModeCnf);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			/*vref set to 1.5v seting*/
			auxDacModeCnf|=(BZ_AUXDACVREF_1P5V<<((Gpio3V3Num*2)+4));
			/*auxdac rgain/10bit mode*/
			auxDacModeCnf &=~BIT(Gpio3V3Num-6);
			/*auxdac ictl/10bit mode*/
			auxDacModeCnf |=BIT(Gpio3V3Num);
		
			if(Gpio3V3Num<11)
			{
				halstatus = BZ_spiWriteReg(hw_priv,REG_MISC_TOP2,auxDacGPIOCnf);
				bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);
			
				halstatus = BZ_spiWriteReg(hw_priv,REG_MISC_TOP0,auxDacPowerCnf);
				bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);
			
				
				halstatus = BZ_spiWriteReg(hw_priv,REG_MISC_TOP1,auxDacModeCnf);
				bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);
				
			}else{		
				
				halstatus = BZ_spiReadReg(hw_priv,REG_MISC_TOP2,&auxDacGPIOCnf);
				bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);
				
				/*gpio input mode disable*/
				auxDacGPIOCnf&=~BIT(13);
				/*gpio output mode disable*/
				auxDacGPIOCnf&=~BIT(5);
				halstatus = BZ_spiWriteReg(hw_priv,REG_MISC_TOP2,auxDacGPIOCnf);
				bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);
				
				halstatus = BZ_spiReadReg(hw_priv,REG_MISC_TOP1,&auxDacModeCnf);
				bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);
								
				/*vref set to 1.5v seting*/
				auxDacModeCnf|=(BZ_AUXDACVREF_1P5V<<16);
				/*auxdac rgain/10bit mode*/
				auxDacModeCnf &=~BIT(20);
				/*auxdac ictl/10bit mode*/
				auxDacModeCnf |=BIT(21);
				/*auxdac powerup*/
				auxDacModeCnf &=~BIT(28);
				halstatus = BZ_spiWriteReg(hw_priv,REG_MISC_TOP1,auxDacModeCnf);
				bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
				IF_ERR_RETURN(bzRetAct);
			}
		}

	}
    return (uint32_t)bzRetAct;
}
uint32_t BZ_writeAuxDac(BzDevice_t *hw_priv,BzGpioPinSel_t Gpio3V3Num,uint16_t auxDacCode)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;
	
	switch(Gpio3V3Num){
		case BZ_GPIO_00:
			halstatus = BZ_spiWriteField(hw_priv,REG_AUXDA_2,auxDacCode,9,0);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;
		case BZ_GPIO_01:
			halstatus = BZ_spiWriteField(hw_priv,REG_AUXDA_2,auxDacCode,19,10);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;
		case BZ_GPIO_02:
			halstatus = BZ_spiWriteField(hw_priv,REG_AUXDA_2,auxDacCode,29,20);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;
		case BZ_GPIO_03:
			halstatus = BZ_spiWriteField(hw_priv,REG_AUXDA_3,auxDacCode,9,0);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
		case BZ_GPIO_04:
			halstatus = BZ_spiWriteField(hw_priv,REG_AUXDA_3,auxDacCode,19,10);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
		case BZ_GPIO_05:
			halstatus = BZ_spiWriteField(hw_priv,REG_AUXDA_3,auxDacCode,29,20);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;	
		case BZ_GPIO_06:
			halstatus = BZ_spiWriteField(hw_priv,REG_AUXDA_0,auxDacCode,9,0);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
		case BZ_GPIO_07:
			halstatus = BZ_spiWriteField(hw_priv,REG_AUXDA_0,auxDacCode,19,10);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
		case BZ_GPIO_08:
			halstatus = BZ_spiWriteField(hw_priv,REG_AUXDA_0,auxDacCode,29,20);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;			
		case BZ_GPIO_09:
			halstatus = BZ_spiWriteField(hw_priv,REG_AUXDA_1,auxDacCode,9,0);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
		case BZ_GPIO_10:
			halstatus = BZ_spiWriteField(hw_priv,REG_AUXDA_1,auxDacCode,19,10);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
		case BZ_GPIO_11:
			halstatus = BZ_spiWriteField(hw_priv,REG_AUXDA_1,auxDacCode,29,20);
			bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
			IF_ERR_RETURN(bzRetAct);
			break;	
			default:
				break;
		}

    return (uint32_t)bzRetAct;
}


/** gpio monitor pos sel types(Used to select the position of 8-bit monitor signal on GPIO)**/
/** GPIO Select :{ [15:12],[11:8],[7:4],[3:0] } **/
/** 8d0 and 4d0 means invalid bit data **/

/*!< GPIO Select :{8d0, monitor_valid_data[7:0]}*/
static uint32_t BZ_setGpioFunForMonitorPosSelD0(BzDevice_t *hw_priv, uint32_t monitor_ena)
{	
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	int i = 0;

	for(i = 0; i < BZ_GPIO_08; i++)
	{
		if(monitor_ena&(1<<i))
		{
			bzRetAct=BZ_setGpioSourceCtrl(hw_priv, 1, i);
			IF_ERR_RETURN(bzRetAct);
			bzRetAct=BZ_setGpioDirection(hw_priv, 1, i);
			IF_ERR_RETURN(bzRetAct);
			bzRetAct=BZ_setGpioFuncSel(hw_priv, BZ_SEL_7, i);
			IF_ERR_RETURN(bzRetAct);
		}
	}
	
    return (uint32_t)bzRetAct;	
}

/*!< GPIO Select :{monitor_valid_data[7:4], 4d0, 4d0, monitor_valid_data[3:0]}*/ 
static uint32_t BZ_setGpioFunForMonitorPosSelD1(BzDevice_t *hw_priv, uint32_t monitor_ena)
{	
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	int i = 0;

	for(i = 0; i < BZ_GPIO_08; i++)
	{
		if(monitor_ena&(1<<i))
		{
			if(i <= BZ_GPIO_03)
			{
				bzRetAct=BZ_setGpioSourceCtrl(hw_priv, 1, i);
				IF_ERR_RETURN(bzRetAct);
				bzRetAct=BZ_setGpioDirection(hw_priv, 1, i);
				IF_ERR_RETURN(bzRetAct);
				bzRetAct=BZ_setGpioFuncSel(hw_priv, BZ_SEL_7, i);
				IF_ERR_RETURN(bzRetAct);
			}
			else
			{
				bzRetAct=BZ_setGpioSourceCtrl(hw_priv, 1, (i+BZ_GPIO_08));
				IF_ERR_RETURN(bzRetAct);
				bzRetAct=BZ_setGpioDirection(hw_priv, 1, (i+BZ_GPIO_08));
				IF_ERR_RETURN(bzRetAct);
				bzRetAct=BZ_setGpioFuncSel(hw_priv, BZ_SEL_7, (i+BZ_GPIO_08));
				IF_ERR_RETURN(bzRetAct);
			}
		}
	}
	
    return (uint32_t)bzRetAct;	
}

/*!< GPIO Select :{4d0, monitor_valid_data[3:0], monitor_valid_data[7:4], 4d0}*/ 
static uint32_t BZ_setGpioFunForMonitorPosSelD2(BzDevice_t *hw_priv, uint32_t monitor_ena)
{	
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	int i = 0;

	for(i = 0; i < BZ_GPIO_08; i++)
	{
		if(monitor_ena&(1<<i))
		{
			if(i <= BZ_GPIO_03)
			{
				bzRetAct=BZ_setGpioSourceCtrl(hw_priv, 1, (i+BZ_GPIO_08));
				IF_ERR_RETURN(bzRetAct);
				bzRetAct=BZ_setGpioDirection(hw_priv, 1, (i+BZ_GPIO_08));
				IF_ERR_RETURN(bzRetAct);
				bzRetAct=BZ_setGpioFuncSel(hw_priv, BZ_SEL_7, (i+BZ_GPIO_08));
				IF_ERR_RETURN(bzRetAct);
			}
			else
			{
				bzRetAct=BZ_setGpioSourceCtrl(hw_priv, 1, i);
				IF_ERR_RETURN(bzRetAct);
				bzRetAct=BZ_setGpioDirection(hw_priv, 1, i);
				IF_ERR_RETURN(bzRetAct);
				bzRetAct=BZ_setGpioFuncSel(hw_priv, BZ_SEL_7, i);
				IF_ERR_RETURN(bzRetAct);
			}
		}
	}
	
    return (uint32_t)bzRetAct;	
}

/*!< GPIO Select :{monitor_valid_data[7:0], 8d0}*/ 
static uint32_t BZ_setGpioFunForMonitorPosSelD3(BzDevice_t *hw_priv, uint32_t monitor_ena)
{	
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	int i = 0;

	for(i = 0; i < BZ_GPIO_08; i++)
	{
		if(monitor_ena&(1<<i))
		{
			bzRetAct=BZ_setGpioSourceCtrl(hw_priv, BZ_SPI_CTRL, (i+BZ_GPIO_08));
			IF_ERR_RETURN(bzRetAct);
			bzRetAct=BZ_setGpioDirection(hw_priv, BZ_SPI_CTRL, (i+BZ_GPIO_08));
			IF_ERR_RETURN(bzRetAct);
			bzRetAct=BZ_setGpioFuncSel(hw_priv, BZ_SEL_7, (i+BZ_GPIO_08));
			IF_ERR_RETURN(bzRetAct);
		}
	}
	
    return (uint32_t)bzRetAct;	
}

uint32_t BZ_setGpioMonitorOut(BzDevice_t *hw_priv,uint32_t monitorIndex,uint32_t monitor_ena,BzGpioMonitorPosSel_t monitor_pos_sel_type)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;

	switch (monitor_pos_sel_type)
	{
		case BZ_GPIO_MONITOR_POS_SEL_D0: 
			/*!< GPIO Select :{8d0, monitor_valid_data[7:0]}*/
			bzRetAct=BZ_setGpioFunForMonitorPosSelD0(hw_priv, monitor_ena);
			IF_ERR_RETURN(bzRetAct);
			break;
		case BZ_GPIO_MONITOR_POS_SEL_D1:
			/*!< GPIO Select :{monitor_valid_data[7:4], 4d0, 4d0, monitor_valid_data[3:0]}*/
			bzRetAct=BZ_setGpioFunForMonitorPosSelD1(hw_priv, monitor_ena);
			IF_ERR_RETURN(bzRetAct);
			break;
		case BZ_GPIO_MONITOR_POS_SEL_D2: 
			/*!< GPIO Select :{4d0, monitor_valid_data[3:0], monitor_valid_data[7:4], 4d0}*/ 
			bzRetAct=BZ_setGpioFunForMonitorPosSelD2(hw_priv, monitor_ena);
			IF_ERR_RETURN(bzRetAct);
			break;
		case BZ_GPIO_MONITOR_POS_SEL_D3:
			/*!< GPIO Select :{monitor_valid_data[7:0], 8d0}*/ 
			bzRetAct=BZ_setGpioFunForMonitorPosSelD3(hw_priv, monitor_ena);
			IF_ERR_RETURN(bzRetAct);
			break;
		default:
			return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,
				BZ_ERR_GPIO_MONITOR_PARAM_VALID,bzRetAct,BZ_ERR_CHECK_PARAM);
			break;
	}	

	/*  set gpio_monitor_pos_sel,value =0,1,2,3 */
	halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_MODE_2,monitor_pos_sel_type,9,8); 
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	/*Enable monitor*/
	halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_MODE_2,monitor_ena,7,0); 
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	/*Setup monitorIndex*/
	halstatus = BZ_spiWriteField(hw_priv,REG_GPIO_MODE_2,monitorIndex,15,12); 
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
    return (uint32_t)bzRetAct;
}
uint32_t BZ_getGpioMonitorOut(BzDevice_t *hw_priv,uint32_t *monitorIndex,uint32_t* monitorEna)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;

	/*Get monitor out enable*/
	halstatus = BZ_spiReadField(hw_priv,REG_GPIO_MODE_2,monitorEna,7,0); 
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	/*Get monitor Index*/
	halstatus = BZ_spiReadField(hw_priv,REG_GPIO_MODE_2,monitorIndex,15,12); 
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

    return (uint32_t)bzRetAct;
}

uint32_t BZ_startAuxAdc(BzDevice_t *hw_priv,BzAuxAdcConfig_t *auxAdcConfig)
{
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;
	
    static const uint16_t MIN_AUXADC_SAMPLES = 1;
    static const uint16_t MAX_AUXADC_SAMPLES = 1000;
    static const uint16_t MIN_AUXADC_SAMPLING_PERIOD_US = 1;
    static const uint16_t MAX_AUXADC_SAMPLING_PERIOD_US = 10;
	
    /*Null Check auxAdcConfig structure*/
    if (auxAdcConfig == NULL)
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
					BZ_ERR_DEVICE_INIT_NULL_PARAM,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    /*Range check that AuxADC Channel selected is valid*/
    if((auxAdcConfig->auxAdcChannelSel != BZ_AUXADC_CH0) &&
       (auxAdcConfig->auxAdcChannelSel != BZ_AUXADC_CH1) &&
       (auxAdcConfig->auxAdcChannelSel != BZ_AUXADC_CH2) &&
       (auxAdcConfig->auxAdcChannelSel != BZ_AUXADC_CH3))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
					BZ_ERR_DEVICE_INIT_NULL_PARAM,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    /*Range check that AuxADC mode is valid*/
    if((auxAdcConfig->auxAdcMode != BZ_AUXADC_NONPIN_MODE) &&
       (auxAdcConfig->auxAdcMode != BZ_AUXADC_PIN_MODE))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
					BZ_ERR_DEVICE_INIT_NULL_PARAM,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    /*Range check that number of samples for conversion is within range*/
    if((auxAdcConfig->numSamples < MIN_AUXADC_SAMPLES) ||
       (auxAdcConfig->numSamples > MAX_AUXADC_SAMPLES))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
					BZ_ERR_DEVICE_INIT_NULL_PARAM,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

    /*Range check that sampling period is at least minimum sampling period for a non-pin mode*/
    if((auxAdcConfig->auxAdcMode == BZ_AUXADC_NONPIN_MODE) &&
       (auxAdcConfig->samplingPeriod_us < MIN_AUXADC_SAMPLING_PERIOD_US) &&
       (auxAdcConfig->samplingPeriod_us > MAX_AUXADC_SAMPLING_PERIOD_US))
    {
		return (uint32_t)BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_INIT,
					BZ_ERR_DEVICE_INIT_NULL_PARAM,bzRetAct,BZ_ERR_CHECK_PARAM);
    }

	/*write auxadcMode*/
	halstatus = BZ_spiWriteField(hw_priv,REG_AUXADC_0,auxAdcConfig->auxAdcMode,0,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	/*auxadc intnumber,This value use default*/
	halstatus = BZ_spiWriteField(hw_priv,REG_AUXADC_1,0x03,18,16); 
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*write auxadc_numbersamples */
	halstatus = BZ_spiWriteField(hw_priv,REG_AUXADC_1,auxAdcConfig->numSamples,9,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*Setup auxadc_clkdiv */
	halstatus = BZ_spiWriteField(hw_priv,REG_AUXADC_1,0x03,26,24);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);

	/*Setup auxadc  powerup*/
	halstatus = BZ_spiWriteField(hw_priv,REG_AUXADC_1,0x11003000,26,24);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*Setup auxadc channelsel*/
	halstatus = BZ_spiWriteField(hw_priv,REG_AUXADC_1,auxAdcConfig->auxAdcChannelSel,3,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
	/*only use nopin mode*/
	if(auxAdcConfig->auxAdcMode == BZ_AUXADC_NONPIN_MODE)
	{
		/*write auxadc samplingPeriod_us*/
		halstatus = BZ_spiWriteField(hw_priv,REG_AUXADC_1,auxAdcConfig->samplingPeriod_us,13,12);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
		
		/*Enable auxad _start*/
		halstatus = BZ_spiWriteField(hw_priv,REG_AUXADC_0,1,4,4);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
		halstatus = BZ_spiWriteField(hw_priv,REG_AUXADC_0,0,4,4);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
		
	}else{ //pinmode 
		/*Setup gpio function to auxadc mode*/
		BZ_setGpioSourceCtrl(hw_priv,1,auxAdcConfig->auxadcGpioSel);
		BZ_setGpioDirection(hw_priv,0,auxAdcConfig->auxadcGpioSel);
		BZ_setGpioFuncSel(hw_priv,BZ_SEL_6,auxAdcConfig->auxadcGpioSel);
		/*setup auxadc gpiosel */
		halstatus = BZ_spiWriteField(hw_priv,REG_AUXADC_1,auxAdcConfig->auxadcGpioSel,23,20);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);		
	}
    return (uint32_t)bzRetAct;
}

uint32_t BZ_readAuxAdc(BzDevice_t *hw_priv,BzAuxAdcResult_t *auxAdcConfig)
{	
	BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	bzHalErr halstatus=BZHAL_OK;
    uint32_t uiTry = WIAT_TIMEOUT;
	
	do{
		halstatus = BZ_spiReadField(hw_priv,REGOUT_AUXADC_0,&auxAdcConfig->completeIndicator,16,16);
		bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
		IF_ERR_RETURN(bzRetAct);
		if(auxAdcConfig->completeIndicator){
			break;
		}
        uiTry--;
	}while(uiTry > 0);

    if(uiTry<=0)
    {
		BZHAL_Printf("ERROR:%s Trytimeout!\n", __func__);
        return (uint32_t)bzRetAct;
    }

	halstatus = BZ_spiReadField(hw_priv,REGOUT_AUXADC_0,&auxAdcConfig->auxAdcCodeAvg,15,0);
	bzRetAct=BZ_ApiErrHandler(hw_priv,BZ_ERRHDL_HAL_GPIO,halstatus,bzRetAct,BZ_ERR_RESET_SPI);
	IF_ERR_RETURN(bzRetAct);
	
    return (uint32_t)bzRetAct;	
}

