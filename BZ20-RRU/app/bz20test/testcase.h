#ifndef TESTCASE_H
#define TESTCASE_H
#include <stdio.h>
#include <stdlib.h>
#include "bz_data_types.h"
extern uint32_t gulLogLevel;

typedef enum
{
    BZ_LOG_LEVEL_LOG 		= 1, 
    BZ_LOG_LEVEL_ERR 		= 2, 
    BZ_LOG_LEVEL_DBG 		= 3, 
}BzLoglevel_t;

#define BZ_LOGPRINT(str, ...) \
    if(gulLogLevel >= BZ_LOG_LEVEL_LOG){ \
        printf("[%s]:"str"\n",__FUNCTION__,##__VA_ARGS__); \
    }
#define BZ_ERRPRINT(str, ...) \
    if(gulLogLevel >= BZ_LOG_LEVEL_ERR){ \
        printf("[ERR]:"str"(%s:%d)%s\n",##__VA_ARGS__,__FILE__,__LINE__,__FUNCTION__); \
    }
#define BZ_DBGPRINT(str, ...) \
    if(gulLogLevel >= BZ_LOG_LEVEL_DBG){ \
        printf("[DBG]:"str"\n",##__VA_ARGS__); \
    }
#define BZ_PRINT(str, ...) \
    if(gulLogLevel >= BZ_LOG_LEVEL_DBG){ \
        printf(str,##__VA_ARGS__); \
    }


uint32_t FT_Rand(uint32_t start, uint32_t end);
void FT_SpiTest(uint32_t Addr);
void FT_mainTest(uint32_t ulMode,uint32_t ulLogLevel);

#endif
