/**
* \file bz_types.h
* \brief Contains B20 API configuration and run-time type definitions
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef __BZ_TYPES_H__
#define __BZ_TYPES_H__
#include "bz_data_types.h"
#include "bz_config_types.h"

/**
 * \brief Data structure to hold a bz device instance status information
 */
typedef struct
{
    BzStates_t devState;          
    BzDigClocks_t clocks;            			
    BzGainIndex_t gainIndexes;    			
    uint8_t supportChannels;     
    uint8_t profilesValid;        
    uint32_t errSource;           
    uint32_t errCode;             
    uint32_t usedGpiopins;                     
    uint16_t usedGpio3p3pins;                  
    uint8_t rxFramerNp;                         
    uint8_t orxFramerNp;                       
    uint32_t rxOutputRate_kHz;                 
    uint32_t txInputRate_kHz;                  
    uint8_t rxDualBandEnabled;                 
    uint32_t rxBandwidth_Hz;                 
    uint32_t txBandwidth_Hz;                    
    uint32_t orxBandwidth_Hz;                   
}bzInfo_t;
typedef struct bz_caps_s
{
	char firmwareCompiler[32];
	uint32_t firmwareCurrentSvnversion;
	char firmwareBuildTime[32];
	uint32_t firmwareReady;
}bz_caps;

/**
 * \brief Data structure to hold bz device instance settings
 */
typedef struct
{
    void *devHalInfo;      
	bzInfo_t  BzState;
	bz_caps	 caps;
	uint32_t currentCalMask;
	uint32_t rfpll_done;
	uint32_t initcal_done;
	uint32_t profile_done;
	uint32_t j204b_done;
	uint32_t gpio_get_value_done;
	uint32_t waitTimeout;
	uint32_t dbg_done;
	uint32_t logLevel;
}BzDevice_t;

#endif
