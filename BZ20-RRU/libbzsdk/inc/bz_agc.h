/**
* \file bz_agc.h
* \brief Contains Bz AGC function prototypes for bz_agc.c
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef _BZ_AGC_H_
#define _BZ_AGC_H_

#include "bz_agc_types.h"
#include "bz_types.h"


/**
 * \param device Pointer to the B20 data structure containing settings
 * \param rxAgcCtrl Pointer to AGC data structure containing settings
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_setupRxAgc(BzDevice_t *hw_priv, BzAgcCfg_t *rxAgcCtrl);
/***
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
#endif

