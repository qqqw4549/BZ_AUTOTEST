/**
* \file bz_cals.h
* \brief Contains B20 calibration related function prototypes for
 *        bz_cals.c
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/
#ifndef _BZ_CALS_H_
#define _BZ_CALS_H_

#include "bz_cals_types.h"
#include "bz_types.h"


/**
 * \brief Run Init Cals
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param initCalMask Calibrated Mask.
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_runInitCals(BzDevice_t *hw_priv,uint32_t initCalMask);

/**
 * \brief Wait Init Cals Done
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param timeout wait time
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_waitInitCalsDone(BzDevice_t *hw_priv,uint32_t timeout);


/**
 * \brief Get Init Cals Status
 *
 * \param device Pointer to the B20 data structure containing settings
 *
 * \retval The first address of the string is returned correctly, and null is returned in case of error.
 */
uint32_t BZ_getInitCalsStatus(BzDevice_t *hw_priv);

#endif

