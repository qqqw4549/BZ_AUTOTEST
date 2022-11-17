/**
* \file bz_radio_types.h
* \brief Contains B20 API Radio Control data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef __BZ_RADIO_TYPES_H__
#define __BZ_RADIO_TYPES_H__


typedef enum
{
    BZ_PIN_MODE = 0x1,   /*!<radio ctrl by pin mode*/
	BZ_SPI_MODE = 0x2	 /*!<radio ctrl by spi mode*/
} BzRadioCtlMode;
	

typedef enum
{
       BZ_OFF_Mask 			= 0x0,  /*!<Get radio Off state mask*/
       BZ_BIT1_Mask 		= 0x1,	/*!<Get radio On state mask*/
       BZ_BIT0_Mask 		= 0x2,	/*!<Get radio On state mask*/
       BZ_BIT0BIT1_Mask 	= 0x3	/*!<Get radio On state mask*/
}BzBIT0BIT1Mask_t;
/**
 *  \brief Enum of PLL selections
 */
typedef enum
{
    BZ_CLK_PLL = 0x0,                /*!< Selects CLK PLL for Rx and Tx */
    BZ_RF_PLL  = 0x1,                /*!< Selects RF PLL for Rx and Tx */
    BZ_AUX_PLL = 0x2                 /*!< Selects AUX PLL for Rx and tx*/
} BzRfPllName_t;

/**
 *  \brief Enum of RADIO STATUS selections
 */
typedef enum
{
    RFPLLMASK =  0x80,              /*!< Get RF pll Status MASK */
    BBPLLMASK  = 0x100,             /*!< Get BB pll Status MASK */
    AUXPLLMASK = 0x200,             /*!< Get AUX pll Status MASK */
	TX0ENMASK =  0x400,				/*!< Get TX0 EN  Status MASK */
	TX1ENMASK =  0x800,				/*!< Get TX1 EN  Status MASK */
	RX0ENMASK =  0x1000,			/*!< Get RX0 EN  Status MASK */
	RX1ENMASK =  0x2000,			/*!< Get TX1 EN  Status MASK */
	ORX0ENMASK = 0x4000,			/*!< Get ORX0 EN  Status MASK */
	ORX1ENMASK = 0x8000				/*!< Get ORX1 EN Status MASK */
} BzChnStatus_t;
#endif //__BZ_RADIO_TYPES_H__

