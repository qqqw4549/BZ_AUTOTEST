/**
* \file bz_tx.h
* \brief Contains B20 transmit related function prototypes for
 *        bz_tx.c
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef __BZ_TX_H__
#define __BZ_TX_H__

#include "bz_tx_types.h"
#include "bz_types.h"

/**
 * \brief Sets the Tx1/Tx2 Manual Gain Index
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param txChannel select the channel to update the gainindex
 * \param gainindex Desired Rx1/Rx2 gain index depending on rxchannle parameter
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_setTxManualGain(BzDevice_t *hw_priv,BzTxChannels_t txChannel, uint8_t gainIndex);
/**
 * \brief configures the rx gain control mode
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param mode Desired gain mode to be configured MGC or AGC
 * \param ctrlway apimode or gpio mode
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_setTxGainControlMode(BzDevice_t *hw_priv, BzGainMode_t mode);

/**
 * \brief get Tx gain index
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param TxChannel
 * \param TxGainIndex
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_getTxGain(BzDevice_t *hw_priv,BzTxChannels_t TxChannel, uint32_t *TxGainIndex);

/**
 * \brief set PA protect config
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param txPaProtectCfg
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_setPaProtectionCfg(BzDevice_t *hw_priv,BzTxPaProtectCfg_t *txPaProtectCfg);
/**
 * \brief  this function for clear paprotectflag
 *
 * \param hw_priv Pointer to the B20 data structure containing settings
 * \param errorFlags getPaProtectErrorFlags
 *
 * \retval 
 */
uint32_t BZ_getPaProtectErrorFlags(BzDevice_t *hw_priv, uint32_t *errorFlags);

/**
 * \brief  this function for clear paprotectflag
 *
 * \param hw_priv Pointer to the B20 data structure containing settings
 * \param txChannel clear the channle flag
 *
 * \retval 
 */
uint32_t BZ_clearPaProtectErrorFlags(BzDevice_t *hw_priv,BzTxChannels_t txChannel);
/**
 * \brief  this function for set DDS config
 *
 * \param hw_priv Pointer to the B20 data structure containing settings
 * \param txChannel TX channel select of DDS tone
 * \param txDDSTone DDS tone freq
 * \param txDDSAtten DDS tone attenuation
 * 
 * \retval 
 */
uint32_t BZ_setDDsFreqConfig(BzDevice_t *hw_priv, BzTxChannels_t txChannel, BzTxDdsTestToneCfg_t txDDSTone, BzTxDdsAttenCfg_t txDDSAtten);
/**
 * \brief  this function for set DDS disable
 *
 * \param hw_priv Pointer to the B20 data structure containing settings
 * 
 * \retval 
 */
uint32_t BZ_setDdsOff(BzDevice_t *hw_priv);

#endif


