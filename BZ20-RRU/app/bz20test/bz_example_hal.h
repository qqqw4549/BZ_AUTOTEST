#ifndef __BZ_HAL_H__
#define __BZ_HAL_H__
typedef void* bzHalHandle;
#include "bz_plat_hal_types.h"
//#ifndef BZHAL_Printf
//#define     BZHAL_Printf printf 
//#endif
/*delay interface provided by the platform */
bzHalErr BZHAL_UDelay(uint32_t usecs);
bzHalErr BZHAL_MDelay(uint32_t msecs);
bzHalErr BZHAL_setTimeout(void *devHalInfo, uint32_t halTimeout_ms);


/*Gpio interface provided by the platform */
bzHalErr BZHAL_GpioOpen(bzHalDevId devid, bzHalGpio gpio);
bzHalErr BZHAL_GpioOpenExt(bzHalDevId devid, int gpioid);
bzHalErr BZHAL_GpioClose(bzHalHandle handle);
bzHalErr BZHAL_GpioSetDirection(bzHalHandle handle, uint8_t isOutput);
bzHalErr BZHAL_GpioGetDirection(bzHalHandle handle, uint8_t* isOutput);
bzHalErr BZHAL_GpioSet(bzHalHandle handle, uint8_t value);
bzHalErr BZHAL_GpioGet(bzHalHandle handle, uint8_t* value);

/*Spi interface provided by the platform */
bzHalErr BZHAL_SpiOpen(bzHalDevId devid);
bzHalErr BZHAL_SpiClose(bzHalHandle handle);
bzHalErr BZHAL_SpiWriteReg(bzHalHandle handle, uint16_t addr, uint32_t data);
bzHalErr BZHAL_SpiReadReg(bzHalHandle handle, uint16_t addr, uint32_t* data);
bzHalErr BZHAL_spiWriteField(bzHalHandle devHalInfo, uint16_t addr, uint8_t fieldVal, uint8_t end, uint8_t startBit);
bzHalErr BZHAL_spiReadField(bzHalHandle devHalInfo, uint16_t addr, uint8_t *fieldVal, uint8_t end, uint8_t startBit);


/*Log interface provided by the platform */
bzHalErr BZHAL_setLogLevel(bzHalHandle devHalInfo, uint16_t halLogLevel);
bzHalErr BZHAL_writeToLog(bzHalHandle devHalInfo, bzLogLevel_t logLevel, uint32_t errorCode, const char *comment);

#endif //__BZ_HAL_H__
