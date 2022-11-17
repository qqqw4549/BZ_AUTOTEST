/**
* \file bz_init.h
* \brief B20 Init header file
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef __BZ_INIT_H__
#define __BZ_INIT_H__ 

#include "bz_init_types.h"
#include "bz_types.h"


/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */

uint32_t BZ_openHw(BzDevice_t *hw_priv,uint32_t DeviceId);
/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */

uint32_t BZ_closeHw(BzDevice_t *hw_priv);

/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_resetDevice(BzDevice_t *hw_priv);

/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_programFir(BzDevice_t *hw_priv,BzFir_t *filter,int FirSel);

/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_initialize(BzDevice_t *hw_priv,BzInit_t *Init);
/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_enableTrackingCals(BzDevice_t *hw_priv,uint32_t enableMask);
/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_getApiVersion(uint32_t ApiVer);
/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */

uint32_t BZ_getDeviceRev(uint32_t DevVer);
/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */

uint32_t BZ_getProductId(uint32_t ProId);
#endif
