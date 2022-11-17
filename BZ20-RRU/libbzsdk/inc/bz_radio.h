/**
* \file bz_radio.h
* \brief Contains B20 related function prototypes for bz_radio.c
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef __BZ_RADIOCTRL_H__
#define __BZ_RADIOCTRL_H__

#include "bz_radio_types.h"
#include "bz_types.h"
#include "bz_rx_types.h"
#include "bz_tx_types.h"

/**
 * \brief Sets the Rx、Tx And Orx radio on by spi control mode
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param rxChannel Enable Rx channle num
 * \param orxChannel Enable ORx channle num
 * \param txChannel Enable Tx channle num
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_setRadioCtrlSpiMode(BzDevice_t *hw_priv, BzRxChannels_t rxChannel,BzObsRxChannels_t OrxChannel,BzTxChannels_t txChannel);

/**
 * \brief Gets the Rx、Tx And Orx radio on by spi control mode
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param rxChannel Enable Rx channle num
 * \param orxChannel Enable ORx channle num
 * \param txChannel Enable Tx channle num
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_getRadioCtrlSpiMode(BzDevice_t *hw_priv, BzRxChannels_t *rxChannel,BzObsRxChannels_t *OrxChannel,BzTxChannels_t *txChannel);
/**
 * \brief Sets the Rx、Tx And Orx radio on by pin control mode
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param rxChannel Enable Rx channle num
 * \param orxChannel Enable ORx channle num
 * \param txChannel Enable Tx channle num
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_setRadioCtrlPinMode(BzDevice_t *hw_priv, BzRxChannels_t rxChannel,BzObsRxChannels_t OrxChannel,BzTxChannels_t txChannel);

/**
 * \brief Gets the Rx、Tx And Orx radio on by pin control mode
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param rxChannel Enable Rx channle num
 * \param orxChannel Enable ORx channle num
 * \param txChannel Enable Tx channle num
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_getRadioCtrlPinMode(BzDevice_t *hw_priv,uint32_t* ChanEnable);

/**
 * \brief Sets the RF PLL local oscillator frequency 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param name of the pll to configure baseed on the enumerated types in bzRfPllName_t
 * \param pllLoFreq_Hz Desired RF LO frequenct in Hz
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_setPllFrequency(BzDevice_t *hw_priv,uint32_t name,uint64_t pllLoFreq_Hz);

/**
 * \brief Gets the RF PLL local oscillator frequency 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param name of the pll to configure baseed on the enumerated types in bzRfPllName_t
 * \param pllLoFreq_Hz Desired RF LO frequenct in Hz
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_getPllFrequency(BzDevice_t *hw_priv,uint32_t name,uint64_t *pllLoFreq_Hz);
/**
 * \brief check if the plls are locked
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param timeout wait for locked time
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_waitPllsLockDone(BzDevice_t *hw_priv, uint32_t timeout);
/**
 * \brief Set the bz20 radio state to the radio on state
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param Mode spi mode or pin mode
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_radioOn(BzDevice_t * hw_priv, BzRadioCtlMode Mode);

/**
 * \brief Set the bz20 radio state to the radio off state
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param Mode spi mode or pin mode
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_radioOff(BzDevice_t *hw_priv,BzRadioCtlMode Mode);
/**
 * \brief Get the bz20 radio to the radio  state
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param Mode spi mode or pin mode
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_getRadioState(BzDevice_t *device);
/**
 * \brief this is the Rx test function
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param txChannel 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_setDataGenDcOff(BzDevice_t *hw_priv, BzTxChannels_t txChannel);
/**
 * \brief this is the Rx test function
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param enable 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */

uint32_t BZ_setPllTemperatureTrackEnable(BzDevice_t *hw_priv,uint32_t enable);

#endif

