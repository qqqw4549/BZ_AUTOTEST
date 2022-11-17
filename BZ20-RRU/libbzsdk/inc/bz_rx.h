/**
* \file bz_rx.h
* \brief Contains B20 receive related function prototypes for
 *        bz_rx.c
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef __BZ_RX_H__
#define __BZ_RX_H__

#include "bz_rx_types.h"
#include "bz_types.h"

/**
 * \brief Sets the Rx1/Rx2 Manual Gain Index
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param rxChannel select the channel to update the gainindex
 * \param gainindex Desired Rx1/Rx2 gain index depending on rxchannle parameter
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_setRxManualGain(BzDevice_t *hw_priv, BzRxChannels_t rxChannel, uint8_t gainIndex);

/**
 * \brief get the Rx1/Rx2 Manual Gain Index
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param rxChannel select the channel to update the gainindex
 * \param gainindex Desired Rx1/Rx2 gain index depending on rxchannle parameter
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_getRxGain(BzDevice_t *hw_priv,BzRxChannels_t rxChannel, uint32_t *rxGainIndex);

/**
 * \brief configure gain steps and the gpio inputs for manual Rx gain control
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param rxChannel select the channel to update the gainindex
 * \param rxGainCtrlPin poPointer to B20RxGainCtrlPin_t structure that configures the mannual rx gain pin control
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t  BZ_setRxGainCtrlPin(BzDevice_t *hw_priv, BzRxChannels_t rxChannel, BzRxGainCtrlPin_t *rxGainCtrlPin);

/**
 * \brief configures the rx gain control mode
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param mode Desired gain mode to be configured MGC or AGC
 * \param ctrlway apimode or gpio mode
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_setRxGainControlMode(BzDevice_t *hw_priv, BzGainMode_t mode,BzCtrlWay ctrlWay);

/**
 * \brief get rx gain control mode
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param mode Desired gain mode to be configured MGC or AGC
 * \param ctrlway apimode or gpio mode
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_getRxGainControlMode(BzDevice_t *hw_priv, BzGainMode_t *mode,BzCtrlWay *ctrlWay);

/**
 * \brief programs the gain table settings for either Rx1\Rx2\
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param numGainIndexesInTable configure table
 * \param orxChannel  Desired orxchannle
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_programObsRxGainTable(BzDevice_t *hw_priv, uint8_t numGainIndexesInTable, BzObsRxChannels_t orxChannel);

/**
 * \brief Sets the ORx1/ORx2 Manual Gain Index
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param rxChannel select the channel to update the gainindex
 * \param gainindex Desired Rx1/Rx2 gain index depending on rxchannle parameter
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_setObsRxManualGain(BzDevice_t *hw_priv,BzObsRxChannels_t obsRxCh, uint8_t gainIndex);
/**
 * \brief configure gain steps and the gpio inputs for manual ORx gain control
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param orxChannel select the channel to update the gainindex
 * \param rxGainCtrlPin poPointer to B20RxGainCtrlPin_t structure that configures the mannual rx gain pin control
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t  BZ_setObsRxGainCtrlPin(BzDevice_t *hw_priv, BzObsRxChannels_t orxChannel, BzRxGainCtrlPin_t *rxGainCtrlPin);

/**
 * \brief configures the orx gain control mode
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param mode Desired gain mode to be configured MGC or AGC
 * \param ctrlway apimode or gpio mode
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_setObsRxGainControlMode(BzDevice_t *hw_priv, BzGainMode_t mode,BzCtrlWay ctrlWay);

/**
 * \brief get orx gain control mode
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param mode Desired gain mode to be configured MGC or AGC
 * \param ctrlway apimode or gpio mode
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_getObsRxGainControlMode(BzDevice_t *hw_priv, BzGainMode_t *mode,BzCtrlWay *ctrlWay);

/**
 * \brief get obsrx gain index
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param obsRxChannel
 * \param obsRxGainIndex
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */

uint32_t BZ_getObsRxGain(BzDevice_t *hw_priv,BzObsRxChannels_t obsRxChannel, uint32_t *obsRxGainIndex);
/**
 * \brief set rxslicer
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param rxChannel
 * \param rxSlierPin
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */

uint32_t BZ_setRxSlicerEnable(BzDevice_t *hw_priv,BzRxChannels_t rxChannel,BzRxSlicerPin_t *rxSlierPin);

/**
 * \brief set orx pin function
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param obsRxChannel

 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_setORxPinFunction(BzDevice_t *hw_priv,BzObsRxChannels_t obsRxChannel);
/**
 * \brief set rxslicer
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param rxChannel
 * \param rxDecPower_dBFS
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */

uint32_t BZ_getRxDecPower(BzDevice_t *hw_priv, BzRxChannels_t rxChannel, sint16_t *rxDecPower_dBFS);
/**
 * \brief set rxslicer
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param orxChannel
 * \param rxDecPower_dBFS
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_getORxDecPower(BzDevice_t *hw_priv, BzObsRxChannels_t orxChannel, sint16_t *rxDecPower_dBFS);

#endif

