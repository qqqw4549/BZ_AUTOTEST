/**
* \file bz_date_types.h
* \brief Contains B20 API Data Type Definition
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef __B20_DATA_TYPES_H__
#define __B20_DATA_TYPES_H__
#include <string.h>

#ifndef BZHAL_Printf
#include <stdio.h>
#define BZHAL_Printf printf
#endif


/**
* \file bz_date_types.h
* \brief It is the typedef data for bz device transfer

* Copyright 2021- Zealync Inc.
*/
typedef  unsigned char 			uint8_t;
typedef	 signed char 			sint8_t;
typedef	 signed char 			int8_t;
typedef  unsigned short			uint16_t;
typedef  short					sint16_t;
typedef  short					int16_t;
typedef  unsigned int 			uint32_t;
typedef  int 					sint32_t;
typedef  int 					int32_t;
typedef  unsigned long int  	uint64_t;

#define  BIT(n) (1<<(n))

#endif
