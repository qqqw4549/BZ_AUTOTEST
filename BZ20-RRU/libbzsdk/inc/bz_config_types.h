/**
* \file Bz_config_types.h
* \brief Contains B20 API configuration settings data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef _BZ_CONFIG_TYPES_H_
#define _BZ_CONFIG_TYPES_H_
#include "bz_data_types.h"
#include "bz_rx_types.h"
#include "bz_tx_types.h"
#include "bz_jesd204b_types.h"

typedef enum
{
    BZ_STATE_POWERONRESET 	= 0x00,
    BZ_STATE_INITIALIZED  	= 0x01,
    BZ_STATE_RISCVLOADED  	= 0x02,
    BZ_STATE_CALS_RUN     	= 0x04,
    BZ_STATE_RADIOON      	= 0x08
} BzStates_t;

/* interfaces for the drivers */
typedef enum
{
    BZ_RFPLLMCS_NOSYNC 				= 0x0,                
    BZ_RFPLLMCS_INIT_AND_SYNC 		= 0x1,        
    BZ_RFPLLMCS_INIT_AND_1TRACK 	= 0x2,       
    BZ_RFPLLMCS_INIT_AND_CONTTRACK  = 0x3 
} BzRfPllMcs_t;
/**
 *  \brief Data structure to hold B20 FIR filter settings
 */
typedef struct
{
    sint16_t gain_dB;         
    uint32_t numFirCoefs;    
    sint16_t *coefs;
} BzFir_t;

/**
 *  \brief Data structure to hold settings 
 * \for the current tx specific use case profile
 */
typedef struct
{
    BzFir_t txFir;                 		
	uint8_t txFirBypass;
    uint8_t txFirInterpolation;        
    uint8_t thb1Interpolation;          
    uint8_t thb2Interpolation;          
    uint8_t thb3Interpolation;          
    uint32_t txInputRate_kHz;           
    uint32_t primarySigBandwidth_Hz;    
    uint32_t rfBandwidth_Hz;            
	uint32_t txBbf3dBCorner_kHz; 	 
} BzTxProfile_t;
/**
 *  \brief Data structure to hold settings 
 * \for the current Rx specific use case profile
 */
typedef struct
{
    BzFir_t rxFir;			       
	uint8_t rxFirBypass;
    uint8_t rxFirDecimation;           
    uint8_t rhb1Decimation;            
    uint32_t rxOutputRate_kHz;         
    uint32_t rfBandwidth_Hz;           
    uint32_t rxBbf3dBCorner_kHz;     
} BzRxProfile_t;
/**
 *  \brief Data structure to hold settings 
 * \for the current oRx specific use case profile
 */
typedef struct
{
    BzFir_t rxFir;                  
	uint8_t orxFirBypass;
    uint8_t orxFirDecimation;          
    uint32_t orxOutputRate_kHz;        
    uint32_t rfBandwidth_Hz;           
    uint32_t rxBbf3dBCorner_kHz;   
} BzORxProfile_t;
/**
 *  \brief Data structure to hold digital clock settings
 */
typedef struct
{
    uint32_t deviceClock_kHz;       
    uint32_t rfpll_frequency_Hz;    
    uint8_t rfPllUseExternalLo;    
    BzRfPllMcs_t rfPllPhaseSyncMode;
} BzDigClocks_t;
/**
 * \brief Data structure to hold ORx gain control 
 * \settings for initialization and during use
*/
typedef struct
{
    BzGainMode_t orxgainMode; 			
	BzCtrlWay   orxmgcMode;
    uint8_t orx1GainIndex;     
    uint8_t orx2GainIndex;      
    uint8_t orx1MaxGainIndex;      
    uint8_t orx1MinGainIndex;       
    uint8_t orx2MaxGainIndex;       
    uint8_t orx2MinGainIndex;   
} BzORxGainControl_t;

/**
 * \brief Data structure to hold Tx gain control 
 * \settings for initialization and during use
*/
typedef struct
{
    BzGainMode_t txgainMode;  
	BzCtrlWay   txmgcMode;
    uint8_t tx1GainIndex;      
    uint8_t tx2GainIndex;       
    uint8_t tx1MaxGainIndex;    
    uint8_t tx1MinGainIndex;    
    uint8_t tx2MaxGainIndex;    
    uint8_t tx2MinGainIndex; 
} BzTxGainControl_t;

/**
 *  \brief Data structure to hold ORx data path settings
 */
typedef struct
{
    BzORxProfile_t orxProfile;              
    BzORxGainControl_t orxGainCtrl;        
    BzObsRxChannels_t obsRxChannelsEnable;
} BzObsRxSettings_t;
/**
 *  \brief Data structure to hold Tx data path settings
 */
typedef struct
{
    BzTxProfile_t txProfile;              
    BzTxChannels_t txChannels;            
    BzTxGainControl_t txGainCtrl;         
} BzTxSettings_t;
/**
 *  \brief Data structure to hold Rx data path settings
 */
typedef struct 
{
    BzGainMode_t rxgainMode; 
	BzCtrlWay   rxmgcMode;
    uint8_t rx1GainIndex;      
    uint8_t rx2GainIndex;     
    uint8_t rx1MaxGainIndex;    
    uint8_t rx1MinGainIndex;   
    uint8_t rx2MaxGainIndex;   
    uint8_t rx2MinGainIndex;
} BzRxGainControl_t;
/**
 *  \brief Data structure to hold Rx data path settings
 */
typedef struct 
{
    BzRxProfile_t rxProfile;       
    BzRxGainControl_t rxGainCtrl;  
	BzRxGainCtrlPin_t rxGainCtrlPinInfo;
	BzRxSlicerPin_t	rxSlicerPinInfo;
	BzRxChannels_t rxChannels;  
} BzRxSettings_t;

typedef struct
{
	uint32_t tddMode;
	uint32_t dpdMode;
	
}BzModeSetting_t;

typedef struct 
{
	BzModeSetting_t 	mode;
    BzRxSettings_t    rx;             
    BzTxSettings_t    tx;             
    BzObsRxSettings_t obsRx;          
    BzDigClocks_t     clocks;        
    BzJesdSettings_t  jesd204Settings;
} BzInit_t;

extern BzInit_t BzInit;

#endif

