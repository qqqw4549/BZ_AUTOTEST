#ifndef __BZ_HAL_H__
#define __BZ_HAL_H__

#include "bz_plat_hal_types.h"

//#ifndef BZHAL_Printf
//#define BZHAL_Printf printf
//#endif

typedef struct BzHalHandle* bzHalHandle;

bzHalErr BZHAL_SpiOpen(bzHalDevId devid, bzHalHandle *handle);
bzHalErr BZHAL_SpiClose(bzHalHandle handle);
bzHalErr BZHAL_SpiWriteReg(bzHalHandle handle, uint16_t addr, uint32_t data);
bzHalErr BZHAL_SpiReadReg(bzHalHandle handle, uint16_t addr, uint32_t* data);

bzHalErr BZHAL_InterruptOn(bzHalHandle handle);

bzHalErr BZHAL_TX1CaliOn(bzHalHandle handle);
bzHalErr BZHAL_TX1CaliOff(bzHalHandle handle);

bzHalErr BZHAL_TX2CaliOn(bzHalHandle handle);
bzHalErr BZHAL_TX2CaliOff(bzHalHandle handle);

bzHalErr BZHAL_SetLogLevel(bzHalHandle handle, uint16_t halLogLevel);
bzHalErr BZHAL_WriteToLog(bzHalHandle handle, bzLogLevel_t logLevel, uint32_t errorCode, const char *comment);

void BZHAL_MDelay(uint32_t msecs);
void BZHAL_UDelay(uint32_t usecs);

bzHalErr BZHAL_Memcpy(void* dst,void *src, uint32_t len);

bzHalErr BZHAL_Tx1EnableCtrl(bzHalHandle handle, BzHalChannelEnableMode mode);
bzHalErr BZHAL_Tx2EnableCtrl(bzHalHandle handle, BzHalChannelEnableMode mode);
bzHalErr BZHAL_Rx1EnableCtrl(bzHalHandle handle, BzHalChannelEnableMode mode);
bzHalErr BZHAL_Rx2EnableCtrl(bzHalHandle handle, BzHalChannelEnableMode mode);
bzHalErr BZHAL_ORx1EnableCtrl(bzHalHandle handle, BzHalChannelEnableMode mode);
bzHalErr BZHAL_ORx2EnableCtrl(bzHalHandle handle, BzHalChannelEnableMode mode);

#endif
