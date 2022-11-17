/**
* \file bz_tx_types.h
* \brief Contains B20 API Tx datapath data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef _BZ_TX_TYPES_H_
#define _BZ_TX_TYPES_H_
#include "bz_data_types.h"


#define ANA_TXGAIN_TABLE_LEN 33
#define REG_ERR_VALUE 0xFFFFFFFF

#define MAX_TXGAIN_TABLE_INDEX            63
#define MIN_TXGAIN_TABLE_INDEX            0

/**
 *  \brief Enum of possible Tx channel enables
 */
typedef enum
{
    BZ_TXOFF 	= 0x0,      /*!< Tx channels are disable */
    BZ_TX1	 	= 0x1,      /*!< Tx1 channel enabled */
    BZ_TX2	 	= 0x2,      /*!< Tx2 channel enabled */
    BZ_TX1TX2 	= 0x3     	/*!< Tx1 + Tx2 channels enabled */
} BzTxChannels_t;

/**
*  \brief Enum of Tx DC options
*/
typedef enum 
{
	LO_LOP 		= 0x0,
	TX_OUTP   	= 0x1
}BzTxDcOut;
/**
*  \brief Enum of Tx DDS options
*/
 typedef enum
 {
     BZ_DDS_SINGLE_TONE 	= 0x0,	  /*!< Dds transmit a single tone*/
     BZ_DDS_DOUBLE_TONE 	= 0x1     /*!< Dds transmit a double tone */
 } BzDdsTone_t;
/**
  *  \brief Enum to set the Tx Gain mode
*/
 typedef enum
 {      
     BZ_LOW_GAIN_MODE 	= 0x0, 
     BZ_HIGH_GAIN_MODE 	= 0x1
} BzTXPPAGainMode_t;

/**
 *  \brief Enum to set the Tx Gain ctrl
 */
typedef struct
{      
    sint32_t Atten;
    uint32_t regValue;
} BzTXPPAGainAttSel_t;
/**
 *  \brief Data structure to hold B20 Tx DDS test tone Configuration
 */
typedef struct
{
    int txTone1Freq_kHz;        /*!< Signed frequency in kHz of the desired tone1 */
    int txTone2Freq_kHz;        /*!< Signed frequency in kHz of the desired tone2 */
} BzTxDdsTestToneCfg_t;
/**
 *  \brief Data structure to hold B20 Tx DDS test tone Configuration
 */
typedef enum
{
	TX_THB1_BYPASS	   = 0x4,	 /*!< txFirter Bypass*/
	TX_FIRANDTHB3_BYPASS = 0x9,	 /*!< txFirter Bypass & THB3 By pass*/
	TX_FIR_BYPASS = 0x8         /*!< THB1 By pass*/
}BzTxFirCfg_t;

/**
 *  \brief Data structure to hold B20 Tx DDS test tone attenuation Configuration
 */
typedef struct
{
    uint32_t txTone0Atten;        /*!< Set attenuation of tone1 with I data,Value:0-7,0:0db,1:3db,2:6db etc, 7: close tone*/
    uint32_t txTone1Atten;        /*!< Set attenuation of tone1 with Q data,Value:0-7,0:0db,1:3db,2:6db etc, 7: close tone */
    uint32_t txTone2Atten;        /*!< Set attenuation of tone2 with I data,Value:0-7,0:0db,1:3db,2:6db etc, 7: close tone */
	uint32_t txTone3Atten;        /*!< Set attenuation of tone2 with Q data,Value:0-7,0:0db,1:3db,2:6db etc, 7: close tone */
} BzTxDdsAttenCfg_t;

/**
 *  \brief Data structure to hold B20 Tx DDS test tone Configuration
 */
typedef struct 
{
	uint32_t TxCaliCnt;			 /*!< Set TxCaliCnt for PAProtect */
	uint32_t TxPowerThreshold;	 /*!< Set TxPowerThreshold for PAProtect */
	uint32_t Enable;   			 /*!< Enable Tx1/Tx2 PAProtect */
}BzTxPaProtectCfg_t;


#endif

