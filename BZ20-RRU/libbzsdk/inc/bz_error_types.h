/**
* \file Bz_error_types.h
* \brief Contains B20 data types for API Error messaging
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/
/* Copyright 2020 Zealy Copyright.*/


#ifndef BZ_ERROR_TYPES_H_
#define BZ_ERROR_TYPES_H_
/**
 *  \brief Enum of possible sources of error codes.
 */
typedef enum
{
	BZ_ERR_OK						= 0x0,
	BZ_ERR_INV_NULL_INIT_PARAM		= 0x1,
	BZ_ERR_DEVICE_INIT_NULL_PARAM	= 0x2,
	BZ_ERR_VERTXPFILE_TXINPUTRATE	= 0x3,
	BZ_ERR_VERTXPFILE_RFBAND		= 0x4,
	BZ_ERR_VERTXPFILE_THB1			= 0x5,
	BZ_ERR_VERTXPFILE_THB2			= 0x6,
	BZ_ERR_VERTXPFILE_THB3			= 0x7,
	BZ_ERR_VERTXPFILE_TXFIR			= 0x8,
	BZ_ERR_VERTXPFILE_TXFIR1		= 0x9,
	BZ_ERR_VERTRXPFILE_RXINPUTRATE  = 0xA,
	BZ_ERR_VERXPFILE_RFBAND			= 0xB,
	BZ_ERR_VERRXPFILE_THB1			= 0xC,
	BZ_ERR_VERRXPFILE_RXFIR1		= 0xD,
	BZ_ERR_VERTORXPFILE_RXINPUTRATE	= 0xE,
	BZ_ERR_VEORXPFILE_RFBAND		= 0xF,
	BZ_ERR_VERORXPFILE_FIR			= 0x10,
	BZ_ERR_VERORXPFILE_FIR1			= 0x11,
	BZ_ERR_CLOCK_RFPLL_PHASE		= 0x12,
	BZ_ERR_TXRXORX_CHANNLE			= 0x13,
	BZ_ERR_TXRXORX_PRFILE_VALID		= 0x14,
	BZ_ERR_TX1_MAX_GAIN_VALID		= 0x15,
	BZ_ERR_TX2_MAX_GAIN_VALID		= 0x16,
	BZ_ERR_RX_GAINPIN_CTRL_VALID	= 0x17,
	BZ_ERR_RX1_MAX_GAIN_VALID		= 0x18,
	BZ_ERR_RX2_MAX_GAIN_VALID		= 0x19,
	BZ_ERR_ORX1_MAX_GAIN_VALID		= 0x1A,
	BZ_ERR_ORX2_MAX_GAIN_VALID		= 0x1B,	
	BZ_ERR_GPIO_NUM_VALID		    = 0x1C,	
	BZ_ERR_GPIO_MONITOR_PARAM_VALID = 0x1D
}BzErr_t;
/**
 *  \brief Private Enum to list of available error handlers
 */
typedef enum
{
    BZ_ERRHDL_HAL_INIT	  = 0x0,  
    BZ_ERRHDL_HAL_AGC	  = 0x1,
    BZ_ERRHDL_HAL_JES204B = 0x2,  
    BZ_ERRHDL_HAL_OTP	  = 0x3,
	BZ_ERRHDL_HAL_RADIO   = 0x4,
	BZ_ERRHDL_HAL_RISCV   = 0x5,
	BZ_ERRHDL_HAL_RX 	  = 0x6,
	BZ_ERRHDL_HAL_TX	  = 0x7,	
    BZ_ERRHDL_HAL_GPIO    = 0x8,
    BZ_ERRHDL_INITCALS    = 0x9,
} BzErrHdls_t;

/**
 *  \brief Enumerated list of B20 Recovery Actions used as return values
 *         APIs.
 */
typedef enum
{
    BZ_NO_ACTION 		= 0x0, 
	BZ_ERR_INIT_PLAT	= 0x1,
    BZ_ERR_RESET_RISCV	= 0x2,        
    BZ_ERR_INIT_CALS	= 0x3,  
    BZ_ERR_RESET_SPI	= 0x4,         
    BZ_ERR_RESET_GPIO	= 0x5,         
    BZ_ERR_CHECK_PARAM	= 0x6,       
}BzRecoveryActions_t;

#endif /* B20_ERROR_TYPES_H_ */
