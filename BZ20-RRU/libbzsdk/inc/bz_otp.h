/**
* \file bz_otp.h
* \brief B20 Otp header file
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef BZ_OTP_H__
#define BZ_OTP_H__

#include "bz_otp_types.h"
#include "bz_types.h"


/*operation otp function*/

/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param OtpAddr optddr that read from otp 
 * \param Data Read Otp data
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_OtpRead(BzDevice_t *hw_priv, uint32_t OtpAddr, uint32_t *Data);

/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param OtpAddr optddr that write to otp 
 * \param Data write Otp data
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_OtpWrite(BzDevice_t *hw_priv, uint32_t OtpAddr, uint32_t Data);

#endif
