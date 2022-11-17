/**
* \file bz_plat_hal_types.h
* \brief Contains B20 API Hardware Abstraction Layer (HAL) data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef __BZ_PLAT_HAL_TYPES_H__
#define __BZ_PLAT_HAL_TYPES_H__
#include "bz_types.h"
#include "bz_error_types.h"
typedef enum
{
    BZHAL_OK 		= 0x0,       
    BZHAL_SPI_FAIL	= 0x1,    
    BZHAL_GPIO_FAIL	= 0x2,
	BZHAL_INV_AGC_CLK_PARM_FAIL = 0x3,
	BZHAL_INV_AGC_RX_GAIN_UPDATE_TIME_PARM_FAIL,

} bzHalErr;


typedef enum
{
    BZHAL_LOG_NONE = 0x0,  
    BZHAL_LOG_MSG  = 0x1,  
    BZHAL_LOG_WARN = 0x2,  
    BZHAL_LOG_ERR  = 0x4,  
    BZHAL_LOG_SPI  = 0x8,  
    BZHAL_LOG_ALL  = 0xF  
} bzLogLevel_t;
	
typedef enum {
	BZHAL_DEVID0	= 0x0,
	BZHAL_DEVID1	= 0x1,
	BZHAL_DEVID2	= 0x2,
	BZHAL_DEVID3	= 0x3,
	BZHAL_DEVID4	= 0x4,
	BZHAL_DEVID5	= 0x5,
}bzHalDevId;

typedef enum {
	BZHAL_GPIORXEN0 = 0x0,
	BZHAL_GPIORXEN1 = 0x1,
	BZHAL_GPIOTXEN0 = 0x2,
	BZHAL_GPIOTXEN1 = 0x3,
	BZHAL_GPIOINT	= 0x4,
} bzHalGpio;

typedef enum
{
	BZHAL_CHANNEL_DISABLE = 0x0,  /*!<Tx/RX channel disable*/
	BZHAL_CHANNEL_ENABLE  = 0x01  /*!<Tx/RX channel enable*/
} BzHalChannelEnableMode;
#endif
