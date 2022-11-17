/**
* \file bz_cals_types.h
* \brief Contains B20 API Calibration data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/
#ifndef _BZ_CALS_TYPES_H_
#define _BZ_CALS_TYPES_H_
typedef enum
{
	ADC_TUNER 				= 0x00000001,   /*!<ADC tunner Calibration>*/
	SAR_CAL 				= 0x00000002,   /*!<SAR Calibratuib>*/
	TX_DAC					= 0x00000004,	/*!<Tx Dac passband Calibration>*/
	TX_BB_FILTER 			= 0x00000008,	/*!<Tx BB filter Calibration>*/
	TIA_3DB_CORNER  		= 0x00000010,	/*!<TIA 3db corner Calibration>*/
	TIA_LPF_3DB_CORNER 		= 0x00000020,	/*!<LPF 3db corner calibration>*/
	DC_OFFSET 				= 0x00000040,	/*!<DC offset Calibration*/
	RX_QEC_INIT 			= 0x00000080,	/*!<Rx Quadrature error correction Calibration*/
	ORX_QEC_INIT			= 0x00000100,	/*!<ORx Quadrature error correction Calibration*/
	LOOPBACK_RX_LO_DELAY 	= 0x00000200, 	/*!<Rx Lo Delay path Calibration*/
	LOOPBACK_RX_QEC_INIT 	= 0x00000400,	/*!<Loopback Rx Quadrature error correction Calibration*/
	PATH_DELAY 				= 0x00000800, 	/*!<Path  delay equalization Calibration*/
	TX_LO_LEAKAGE_INTERNAL	= 0x00001000, 	/*!<Internal Tx LO leakage Calibration*/
	TX_LO_LEAKAGE_EXTERNAL 	= 0x00002000, 	/*!<Exterbal Tx LO leakage Calibration*/	
	TX_QEC_INIT				= 0x00004000  	/*!<Tx Quadrature error correction Calibration*/
} BzInitCalibrations_t;

typedef enum
{
    TRACK_NONE              = 0x00000000,   /*!< Disable all tracking calibrations */
    TRACK_RX1_QEC           = 0x00000001,   /*!< Rx1 quadrature error correction tracking calibration */
    TRACK_RX2_QEC           = 0x00000002,   /*!< Rx2 quadrature error correction tracking calibration */
    TRACK_ORX1_QEC          = 0x00000004,   /*!< ORx1 quadrature error correction tracking calibration */
    TRACK_ORX2_QEC          = 0x00000008,   /*!< ORx2 quadrature error correction tracking calibration */
    TRACK_TX1_LOL           = 0x00000010,   /*!< Tx1 LO leakage tracking calibration */
    TRACK_TX2_LOL           = 0x00000020,   /*!< Tx2 LO leakage tracking calibration */
    TRACK_TX1_QEC           = 0x00000040,   /*!< Tx1 quadrature error correction tracking calibration */
    TRACK_TX2_QEC           = 0x00000080,   /*!< Tx2 quadrature error correction tracking calibration */
    TRACK_RX1_HD2           = 0x00000100,   /*!< Rx1 HD2 error correction tracking calibration */
    TRACK_RX2_HD2           = 0x00000200,   /*!< Rx2 HD2 error correction tracking calibration */
    TRACK_ALL               = 0x000003FF    /*!< ENUM specifies all tracking cals */
} BzTrackingCalibrations_t;

/**
 * \brief Enum of unique error codes from the B20 API functions.
 *        Each error condition in the library shall have its own enum value
 *        to ease debug of errors.
 */
typedef enum
{
	RX1_DC_OFFSET_ERROR				= 0x10000040,	/*!<RX1_DC_OFFSET Error Status*/
	RX2_DC_OFFSET_ERROR 			= 0x11000040,	/*!<RX2_DC_OFFSET Error Status*/
	RX1_QEC_INIT_ERROR 				= 0x12000080,	/*!<RX1_QEC_INIT Error Status*/
	RX2_QEC_INIT_ERROR				= 0x13000080,	/*!<RX2_QEC_INIT Error Status*/
	TX1_LO_LEAKAGE_INTERNAL_ERROR	= 0x20001000,	/*!<TX1_LO_LEAKAGE_INTERNA Error Status*/
	TX2_LO_LEAKAGE_INTERNAL_ERROR	= 0x21001000,	/*!<TX2_LO_LEAKAGE_INTERNA Error Status*/
	TX1_LO_LEAKAGE_EXTERNAL_ERROR	= 0x22002000,	/*!<TX1_LO_LEAKAGE_EXTERNA Error Status*/
	TX2_LO_LEAKAGE_EXTERNAL_ERROR	= 0x23002000,	/*!<TX2_LO_LEAKAGE_EXTERNA Error Status*/
	TX1_QEC_INIT_ERROR	 			= 0x24004000,	/*!<TX1_QEC_INIT Error Status*/
	TX2_QEC_INIT_ERROR				= 0x25004000,	/*!<TX2_QEC_INIT Error Status*/
	BZ_BBPLL_LOCK_ERROR			    = 0x33000000,   /*!<BB PLL Calibrate Error Status*/
	BZ_RFPLL_LOCK_ERROR 			= 0x31000000,	/*!<RF PLL Calibrate Error Status*/
	BZ_AUXPLL_LOCK_ERROR			= 0x32800000,	/*!<AUX PLL Calibrate Error Status*/
    BZ_CALI_STATS_OK                = 0x50000000    /*!<BZ CALIBRATION STATUS OK*/
 } BzCalErr_t;

#endif
