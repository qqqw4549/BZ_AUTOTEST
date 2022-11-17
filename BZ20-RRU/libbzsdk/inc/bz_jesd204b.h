/**
* \file bz_jesd204.h
* \brief Contains B20 JESD204b data path related function prototypes for
 *        bz_jesd204.c
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef _BZ_JESD204B_H_
#define _BZ_JESD204B_H_

#include "bz_jesd204b_types.h"
#include "bz_types.h"

/**
 * \brief write Jes204b config
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param Init pointer the initial profile 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_writeJes204bConfig(BzDevice_t *priv,BzInit_t *Init);

/**
 * \brief wait for Jes204b config done
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param timeout wait time
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_waitJes204bConfigDone(BzDevice_t *hw_priv, uint32_t timeout);

/**
 * \brief configures the Jes204b sysref control mode
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param mode Desired Jes204b Sysref ON or OFF
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_Jes204bSysrefControl(BzDevice_t *priv, BzJes204bSysrefControlMode Mode);
/**
 * \brief configures the Jes204b prbs7 control mode
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param LaneId Desired Jes204b LaneId
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_Prbs7Check(BzDevice_t *hw_priv,uint32_t LaneId);
/**
 * \brief configures the Jes204b prbs7 control mode
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param addr setDebug addr of Data
 * \param sample_point Desired Jes204b sample point
 * \param count setDebug count of Data
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
	uint32_t BZ_spiGetJesdDebugData(BzDevice_t *hw_priv, uint32_t addr, uint32_t sample_point, uint32_t count,uint32_t sample_rate);
/**
 * \brief wait for Jes204b framerLink enable
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param timeout wait time
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */

uint32_t BZ_enableFramerLink(BzDevice_t *hw_priv,uint32_t framerSel,uint32_t enable);
/**
 * \brief wait for Jes204b deframerLink enable
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param timeout wait time
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */

uint32_t BZ_enableDeframerLink(BzDevice_t *hw_priv,uint32_t deframerSel,uint32_t enable);
/**
 * \brief wait for Jes204b status
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param timeout wait time
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */

uint32_t  BZ_getSyncStatus(BzDevice_t * hw_priv,uint32_t *TypeStatus);
/**
 * \brief wait for Jes204b status
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param BzInit_t
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */

uint32_t BZ_enableSysRefToFramer(BzDevice_t *hw_priv,BzInit_t *Init);

#endif //__B20_JESD204B_H__
