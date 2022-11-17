/**
* \file bz_otp_types.h
* \brief Contains B20 API Otp data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef __BZ_OTP_TYPES_H__
#define __BZ_OTP_TYPES_H__

#define BZ_REG_OTPIN_CMD_ADDR    	0x208
#define BZ_REG_OTPIN_DATA_ADDR		0x209

#define BZ_REG_OTPOUT_DATA_ADDR		0x44c
#define BZ_REG_OTPOUT_STA_ADDR		0x44d			

/*REG_OTPIN_CMD_ADDR ->bit[6]---otp_pas_en */
typedef enum {
	BZ_PAS_DIS  = 0x0,
	BZ_PAS_EN   = 0x1
}BZ_OTP_PAS_EN;

/*REG_OTPIN_CMD_ADDR ->bit[5]---otp_ptr_vld */
typedef enum{
	BZ_TEST_ROW_INVALID	= 0x0,
	BZ_TEST_ROW_VALID	= 0x1
}BZ_OTP_PTR_VLD;

/*REG_OTPIN_CMD_ADDR ->bit[4]---otp_pas_vld */
typedef enum{
	BZ_CELL_INVALID		= 0x0,
	BZ_CELL_VALID		= 0x1	
}BZ_OTP_PAS_VLD;

/*REG_OTPIN_CMD_ADDR ->bit[3]---otp_cmd_vld */
typedef enum{
	CMD_INVALID	 = 0x0,
	CMD_VALID	 = 0x1
}BZ_OTP_CMD_VLD;

/*REG_OTPIN_CMD_ADDR ->bit[2:0]---otp_cmd */
typedef enum{
	BZ_DEEP_STANDBY_MODE	= 0x0,
	BZ_STANDBY_MODE			= 0x1,
	BZ_READ_MODE			= 0x2,
	BZ_PROG_MODE			= 0x3,
	BZ_INIT_MARGIN_READ_MODE = 0x4,
	BZ_PGM_MARGIN_READ_MODE	 = 0x5,
}BZ_OTP_CMD;



#endif

