/**
* \file bz_plat_hal.c
* \brief Contains B20 API Hardware Abstraction Layer (HAL) functions
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/
#include "bz_plat_hal.h"
#include "bz_hal.h"
#include <string.h>

bzHalErr BZ_spiOpen(BzDevice_t *hw_priv,uint32_t DeviceId)
{
    bzHalErr halStatus = BZHAL_OK;
	bzHalHandle SpiHandle=NULL;
	
	BZHAL_SpiOpen(DeviceId,&SpiHandle);
	hw_priv->devHalInfo=(void*)SpiHandle;
	if(hw_priv->devHalInfo==NULL)
	{
		B20_DEBUGF(DBG_LEVEL_SERIOUS,"hw_priv->devHalInfo is NULL\n");
	}
	return halStatus;
}
bzHalErr BZ_spiClose(BzDevice_t *hw_priv)
{
    bzHalErr halStatus = BZHAL_OK;
	
	halStatus =BZHAL_SpiClose(hw_priv->devHalInfo);
	return halStatus;
}
bzHalErr BZ_spiReadReg(BzDevice_t *hw_priv, uint16_t addr, uint32_t *data)
{
    bzHalErr halStatus = BZHAL_OK;
	
	/*customer read interface*/
    halStatus=BZHAL_SpiReadReg(hw_priv->devHalInfo,addr, data);
    BZHAL_UDelay(20);
	return halStatus;
}
bzHalErr BZ_spiWriteReg(BzDevice_t *hw_priv, uint16_t addr,uint32_t data)
{
    bzHalErr halStatus = BZHAL_OK;
	
	/*customer write interface*/
    halStatus=BZHAL_SpiWriteReg(hw_priv->devHalInfo,addr, data);
	return halStatus;
}
bzHalErr BZ_spiReadField(BzDevice_t *hw_priv,uint16_t addr,uint32_t *fieldVal,uint8_t endBit,uint8_t startBit)
{
    bzHalErr halStatus = BZHAL_OK;
	uint32_t regdata=0;
	uint32_t regmask=0;
	
	regmask = ~((1<<startBit) -1);
	regmask &= ((1<<endBit) -1)|(1<<endBit);
	halStatus=BZ_spiReadReg(hw_priv,addr,&regdata);	
	regdata &= regmask;
	regdata >>=  startBit;
	*fieldVal = regdata;
	return halStatus;
}
bzHalErr BZ_spiWriteField(BzDevice_t *hw_priv,uint16_t addr,uint32_t fieldVal,uint8_t endBit,uint8_t startBit)
{
    bzHalErr halStatus = BZHAL_OK;
	uint32_t regdata=0;
	uint32_t regmask=0;
	
	halStatus=BZ_spiReadReg(hw_priv,addr,&regdata);
	
	regmask = ~((1<<startBit) -1);
	regmask &= ((1<<endBit) -1)|(1<<endBit);
	regdata &= ~regmask;
	regdata |= (fieldVal <<startBit)&regmask;
	halStatus=BZ_spiWriteReg(hw_priv,addr,regdata);
	return halStatus;
}

bzHalErr BZ_mdelay(uint32_t mdelay_time)
{
    bzHalErr halStatus = BZHAL_OK;
	
    BZHAL_MDelay(mdelay_time);
	return halStatus;	
}

bzHalErr BZ_WriteToLog(BzDevice_t *hw_priv,bzLogLevel_t loglevel,uint32_t errorCode,const char *comment)
{
    bzHalErr halStatus = BZHAL_OK;
    char scStr[100];
    sprintf(scStr,"OPERATION SPILOG:%d:%s\n",errorCode,comment);
	B20_DEBUGF(DBG_LEVEL_SEVERE,scStr);
	halStatus=BZHAL_WriteToLog(hw_priv->devHalInfo,loglevel,errorCode,comment);
	return halStatus;	

}
bzHalErr BZ_Memset(void* Addr,uint32_t Data,uint32_t len)
{
    bzHalErr halStatus = BZHAL_OK;
	
	memset(Addr,Data,len);
	return halStatus;	
}
bzHalErr BZ_Memcpy(void* dst,void *src,uint32_t len)
{
    bzHalErr halStatus = BZHAL_OK;
	
	memcpy(dst,src,len);
	return halStatus;	

}

/*Api to control pin enable, When the Tx/RX channel enable mode is GPIO mode*/
bzHalErr BZ_Tx1EnableCtrl(BzDevice_t *hw_priv, BzHalChannelEnableMode mode)
{
    bzHalErr halStatus = BZHAL_OK;
	halStatus = BZHAL_Tx1EnableCtrl(hw_priv->devHalInfo, mode);
    return halStatus;
}

bzHalErr BZ_Tx2EnableCtrl(BzDevice_t *hw_priv, BzHalChannelEnableMode mode)
{
    bzHalErr halStatus = BZHAL_OK;
	halStatus = BZHAL_Tx2EnableCtrl(hw_priv->devHalInfo, mode);
    return halStatus;
}

bzHalErr BZ_Rx1EnableCtrl(BzDevice_t *hw_priv, BzHalChannelEnableMode mode)
{
    bzHalErr halStatus = BZHAL_OK;
	halStatus = BZHAL_Rx1EnableCtrl(hw_priv->devHalInfo, mode);
    return halStatus;
}

bzHalErr BZ_Rx2EnableCtrl(BzDevice_t *hw_priv, BzHalChannelEnableMode mode)
{
    bzHalErr halStatus = BZHAL_OK;
	halStatus = BZHAL_Rx2EnableCtrl(hw_priv->devHalInfo, mode);
    return halStatus;
}

bzHalErr BZ_ORx1EnableCtrl(BzDevice_t *hw_priv, BzHalChannelEnableMode mode)
{
    bzHalErr halStatus = BZHAL_OK;
	halStatus = BZHAL_ORx1EnableCtrl(hw_priv->devHalInfo, mode);
    return halStatus;
}

bzHalErr BZ_ORx2EnableCtrl(BzDevice_t *hw_priv, BzHalChannelEnableMode mode)
{
    bzHalErr halStatus = BZHAL_OK;
	halStatus = BZHAL_ORx2EnableCtrl(hw_priv->devHalInfo, mode);
    return halStatus;
}


