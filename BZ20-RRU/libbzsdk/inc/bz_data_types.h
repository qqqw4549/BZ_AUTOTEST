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
//#include <stdio.h>
#include <string.h>
#include <stdint.h>
/**
* \file bz_date_types.h
* \brief It is the typedef data for bz device transfer

* Copyright 2021- Zealync Inc.
*/
//#define NULL					(void*)0
typedef  unsigned char 			uint8_t;
typedef	 char 					sint8_t;
typedef  unsigned short			uint16_t;
typedef  short					sint16_t;
typedef  unsigned int 			uint32_t;
typedef  int 					sint32_t;

#define  BIT(n) (1<<(n))

#endif
