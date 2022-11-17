/**
* \file bz_rx_types.h
* \brief Contains B20 API Rx datapath data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef _BZ_RX_TYPES_H_
#define _BZ_RX_TYPES_H_

#include "bz_data_types.h"
#include "bz_gpio_types.h"


#define ANA_RXGAIN_TABLE_LEN 128
#define REG_ERR_VALUE 0xFFFFFFFF

#define MAX_RXORXGAIN_TABLE_INDEX            127
#define MIN_RXORXGAIN_TABLE_INDEX            0


typedef enum
{
    BZ_RXOFF 	= 0x00,     		/*!< No Rx channels are enabled */
    BZ_RX1	= 0x01,          /*!< Rx1 channel enabled */
    BZ_RX2	= 0x02,          /*!< Rx2 channel enabled */
    BZ_RX1RX2	= 0x03         	/*!< Rx1 + Rx2 channels enabled */
} BzRxChannels_t;
typedef enum
{
    BZ_ORXOFF      = 0x00,		/*!< No ORx channels are enabled */	
    BZ_ORX1        = 0x01,		/*!< ORx1 channel enabled */
    BZ_ORX2        = 0x02,		/*!< ORx2 channel enabled */
    BZ_ORX1ORX2    = 0x03		/*!< ORx1 + Rx2 channels enabled */
} BzObsRxChannels_t;
/**
 *  \brief Enum to set the Rx Gain control mode
 */
typedef enum
{
    BZ_MGC = 0x00,          /*Manual Gain Control */
    BZ_AGC = 0x01,   	   /*Auto Gain Control */
} BzGainMode_t;


typedef enum
{
	 BZ_APIMODE = 0x00,		/*!<Gaincontrol ways spi*/
	 BZ_GPIOMODE = 0x01,    /*!<Gaincontrol ways gpio*/
}BzCtrlWay;


typedef enum
{
	BZ_RXSLICER_DIS = 0x00,	/*!<Rxslicer enable*/
	BZ_RXSLICER_EN  = 0x01,	/*!<Rxslicer disable*/
}BzSlicerEn;
/**
 *  \brief Enum to set the Rx Gain ctrl
 */
typedef struct
{      
    float Atten;
    uint32_t regVelue;
} BzRXPPAGainAttSel_t;

/**
 * \brief Data Structure to hold B20 device gain settings
 */
typedef struct
{
    uint8_t rx1MinGainIndex;         /*!<minimum Rx1 gain index */
    uint8_t rx1MaxGainIndex;         /*!<maximum Rx1 gain index */
    uint8_t rx2MinGainIndex;         /*!<minimum Rx2 gain index */
    uint8_t rx2MaxGainIndex;         /*!<maximum Rx2 gain index */
    uint8_t orx1MinGainIndex;        /*!<minimum Orx1 gain index */
    uint8_t orx1MaxGainIndex;        /*!<maximum Orx1 gain index */
    uint8_t orx2MinGainIndex;        /*!<minimum Orx2 gain index */
    uint8_t orx2MaxGainIndex;        /*!<maximum Orx2 gain index */
    uint8_t tx1MinGainIndex;         /*!<minimum Tx1 gain index */
    uint8_t tx1MaxGainIndex;         /*!<maximum Tx1 gain index */
    uint8_t tx2MinGainIndex;         /*!<minimum Tx2 gain index */
    uint8_t tx2MaxGainIndex;         /*!<maximum Tx2 gain index */
} BzGainIndex_t;

/**
 *  \brief Data structure to hold B20 Rx Gain Control Pin Configuration Info
 */
typedef struct
{
    BzGpioPinSel_t  rxGainIncPin;		/*!< GPIO used for the Increment  gain input:the gpioNum user can select via gpio function*/
    BzGpioPinSel_t  rxGainDecPin;		/*!< GPIO used for the Decrement  gain input:the gpioNum user can select via gpio function*/
	BzGpioPinFunc_t rxGainIncPinFunc;   /*!< GPIO function select 0-7: make GPIO function  gain Increment function */
	BzGpioPinFunc_t rxGainDecPinFunc;   /*!< GPIO function select 0-7:*make GPIO function  gain Decrement function */
} BzRxGainCtrlPinInfo_t;


/**
 *  \brief Data structure to hold B20 Rx Gain Control Pin Configuration
 */
typedef struct
{
    uint8_t incStep;      				/*!< Increment in gain index applied when the increment gain pin is pulsed(1-16) */          
    uint8_t decStep;               		/*!< Decrement in gain index applied when the increment gain pin is pulsed(1-16) */
    BzRxGainCtrlPinInfo_t  rx1GainCtrlPinInfo;		/*!< rx1 gain control pin configuration info*/
    BzRxGainCtrlPinInfo_t  rx2GainCtrlPinInfo;		/*!< rx2 gain control pin configuration info*/
} BzRxGainCtrlPin_t;

/**
 *  \brief Data structure to hold B20 Rx slicer Pin Configuration
 */

typedef struct
{
    BzGpioPinSel_t  rx1SlicerPin_0;		/*!< GPIO used for the slicer output:the gpioNum user can select via gpio function*/
    BzGpioPinSel_t  rx1SlicerPin_1;		/*!< GPIO used for the slicer output:the gpioNum user can select via gpio function*/
	BzGpioPinSel_t	rx1SlicerPin_2;     /*!< GPIO used for the slicer output:the gpioNum user can select via gpio function*/
} BzRx1SlicerPin_t;
/**
 *  \brief Data structure to hold B20 Rx slicer Pin Configuration
 */

typedef struct
{
    BzGpioPinSel_t  rx2SlicerPin_0;		/*!< GPIO used for the slicer output:the gpioNum user can select via gpio function*/
    BzGpioPinSel_t  rx2SlicerPin_1;		/*!< GPIO used for the slicer output:the gpioNum user can select via gpio function*/
	BzGpioPinSel_t	rx2SlicerPin_2;     /*!< GPIO used for the slicer output:the gpioNum user can select via gpio function*/
} BzRx2SlicerPin_t;

/**
 *  \brief Data structure to hold B20 Rx Slicer out Pin Configuration
 */
typedef struct
{
	BzSlicerEn		  EnableSlicer;			/*!<Digital Gain compensation with slicer gpio output*/
    BzRx1SlicerPin_t  rx1SlicerPinInfo;		/*!< rx1 slicer control pin configuration info*/
    BzRx2SlicerPin_t  rx2SlicerPinInfo;		/*!< rx2 slicer control pin configuration info*/
	BzGpioPinFunc_t   rxSlicerPinFunc;		/*!<select gpio fun for slicer*/
} BzRxSlicerPin_t;

#endif

