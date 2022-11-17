/**
* \file bz_error.h
* \brief Contains B20 API error handling function prototypes and data types for B20_error.c
 *        These functions are public to the customer for getting more details on
 *        errors and debugging.
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/
#ifndef __BZ_ERROR_H__
#define __BZ_ERROR_H__

#include "bz_error_types.h"
#include "bz_types.h"
/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param errHdl Error Handler type
 * \param detErr Error detected to be processed by handler
 * \param retVal current Recovery Action
 * \param RecAction new Recovery action to be returned should err handle determin an error
 * \retval uint32 value representing the latest recovery action following processing of detected error
 */
BzRecoveryActions_t BZ_ApiErrHandler(BzDevice_t *hw_priv, BzErrHdls_t errHdl, uint32_t detErr, BzRecoveryActions_t retVal, BzRecoveryActions_t recAction);
#endif
