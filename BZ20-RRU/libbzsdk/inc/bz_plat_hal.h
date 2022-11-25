/**
* \file bz_plat_hal.h
* \brief Contains prototypes and macro definitions for Private B20 HAL wrapper
 *        functions implemented in bz_plat_hal.c
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef __BZ_PLAT_HAL_H__
#define __BZ_PLAT_HAL_H__

#include"bz_plat_hal_types.h"
#include "bz_types.h"

/** lower two bits indicate debug level
 * - 0 all
 * - 1 warning
 * - 2 serious
 * - 3 severe
 */
#define DBG_LEVEL_ALL     0x00
#define DBG_LEVEL_OFF     DBG_LEVEL_ALL 
#define DBG_LEVEL_WARNING 0x01 
#define DBG_LEVEL_SERIOUS 0x02 
#define DBG_LEVEL_SEVERE  0x03
#define DBG_MASK_LEVEL    0x03

/*Enable Debug Msg ,Else Disable debug Msg*/
#define DEBUG_VERSION  1

#ifdef DEBUG_VERSION
#define B20_DBG_TYPES_ON  B20_DBG_ON
#define B20_DBG_MIN_LEVEL DBG_LEVEL_OFF

#define B20_DEBUGF(debug, message) do { \
									if ( \
									   (((debug) & DBG_MASK_LEVEL) >= B20_DBG_MIN_LEVEL)) { \
									 	BZHAL_Printf(message); \
										} \
									} while(0)

#else  /* DEBUG_VERSION */
#define B20_DEBUGF(debug, message)
#endif /* DEBUG_VERSION */

#define IF_ERR_RETURN(a) if(a != BZ_NO_ACTION) { BZHAL_Printf("SPI CONF ERROR\n"); return a; }
#define POINTER_CHECK(p) if(p == NULL){ BZHAL_Printf("[ERROR]:POINTER_NULL(function:%s) %s(Line:%d)" \
                                        ,__FUNCTION__,__FILE__,__LINE__);                            \
                                        return BZ_ERR_NULL_POINTER;}


/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
bzHalErr BZ_spiOpen(BzDevice_t *hw_priv,uint32_t DeviceId);
/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
bzHalErr BZ_spiClose(BzDevice_t *hw_priv);

/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
bzHalErr BZ_spiReadReg(BzDevice_t *hw_priv, uint16_t addr, uint32_t *data);

/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
bzHalErr BZ_spiWriteReg(BzDevice_t *hw_priv, uint16_t addr,uint32_t data);

/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
bzHalErr BZ_spiReadField(BzDevice_t *hw_priv,uint16_t addr,uint32_t *fieldVal,uint8_t endBit,uint8_t statBit);

/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
bzHalErr BZ_spiWriteField(BzDevice_t *hw_priv,uint16_t addr,uint32_t fieldVal,uint8_t endBit,uint8_t statBit);

/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
bzHalErr BZ_mdelay(uint32_t mdelay_time);

/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
bzHalErr BZ_WriteToLog(BzDevice_t *hw_priv,bzLogLevel_t loglevel,uint32_t errorCode,const char *comment);

bzHalErr BZ_Memset(void* Addr,uint32_t Data,uint32_t len);

bzHalErr BZ_Memcpy(void* src,void *dst,uint32_t len);
/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
bzHalErr BZ_Tx1EnableCtrl(BzDevice_t *hw_priv, BzHalChannelEnableMode mode);
/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */

bzHalErr BZ_Tx2EnableCtrl(BzDevice_t *hw_priv, BzHalChannelEnableMode mode);
/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
bzHalErr BZ_Rx1EnableCtrl(BzDevice_t *hw_priv, BzHalChannelEnableMode mode);
/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
bzHalErr BZ_Rx2EnableCtrl(BzDevice_t *hw_priv, BzHalChannelEnableMode mode);
/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
bzHalErr BZ_ORx1EnableCtrl(BzDevice_t *hw_priv, BzHalChannelEnableMode mode);
/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */

bzHalErr BZ_ORx2EnableCtrl(BzDevice_t *hw_priv, BzHalChannelEnableMode mode);

#endif

