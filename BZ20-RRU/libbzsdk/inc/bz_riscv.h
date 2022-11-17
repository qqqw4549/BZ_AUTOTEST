/**
* \file bz_riscv.h
* \brief Contains B20 related function prototypes for bz_riscv.c
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef __BZ_RISCV_H__
#define __BZ_RISCV_H__

#include "bz_riscv_types.h"
#include "bz_types.h"

/**
 * \brief reset b20 by spi
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param RestType can be interrupt or spi register
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_spiResetChip(BzDevice_t *hw_priv,uint8_t RestType);
/**
 * \brief download firmware
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_loadFirmware(BzDevice_t *hw_priv);

/**
 * \brief download firmware function
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param addr firmware itcm/dtcm memory
 * \param data firmware itcm/dtcm data
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_writeFirmware(BzDevice_t *hw_priv,uint16_t addr, uint32_t data);
/**
 * \brief write riscv memory data
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param regdata wirte memory data
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_spiWriteMem(BzDevice_t *hw_priv,uint32_t addr,uint32_t data);
/**
 * \brief read riscv memory data
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param regdata store memory data
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_spiReadMem(BzDevice_t *hw_priv,uint32_t addr,uint32_t *regdata);
/**
 * \brief write the initail profile
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param Init pointer the initial profile 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_writeRiscvProfile(BzDevice_t *priv,BzInit_t *Init);

/**
 * \brief read the initail profile
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param Init pointer the initial profile 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_readRiscvProfile(BzDevice_t *priv,BzInit_t *Init);
/**
 * \brief get the riscvtraceEvent
 *
 * \param device Pointer to the B20 data structure containing settings
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_GetRiscvTraceEvent(BzDevice_t *hw_priv);

/**
 * \brief get the riscvlog
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param addr the riscvlog store
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint8_t * BZ_spiGetRiscvLog(BzDevice_t *hw_priv,uint32_t addr);
/**
 * \brief enable riscv log
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param debug 1 enable.othewise 0 is disable
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_enableRiscvLog(BzDevice_t *hw_priv,uint32_t debug);
/**
 * \brief get riscv pc
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param addr pc store memory
 * \param addr pc store cnt
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_spiGetRiscvPc(BzDevice_t *hw_priv,uint32_t addr,uint32_t pc_cnt);

/**
 * \brief Assert spi interrupt to riscv
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param writeData 1, 1 means assert spi interrupt otherwise 0 cannot assert spi interrupt to riscv
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t bz_spiInterrupt(BzDevice_t *hw_priv,uint32_t writeData);

/**
 * \brief wait for command confirm status
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_spiWaitCmdConfirm(BzDevice_t *hw_priv);/**
 * \brief get riscvcmd  statuc
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_spiGetCmdStatus(BzDevice_t *hw_priv);
/** 
\brief setcommad to risv
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param Command set command to riscv
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_sendRiscvCmd(BzDevice_t *priv,void *UserData, unsigned int Command);
/** 
\brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_getBootStatus(BzDevice_t *hw_priv);
/** 
\brief wait fot boot already bring up
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param 
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t  BZ_waitFirmwareReady(BzDevice_t *hw_priv, uint32_t timeout);
/** 
 \brief wait for profile write done
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param timeout wait time
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t  BZ_waitProfileDone(BzDevice_t *hw_priv, uint32_t timeout);
/** 
\brief clear the status 
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param timeout wait time
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */

uint32_t BZ_clearStatus(BzDevice_t *hw_priv);
/**
 * \brief get the sdkversion message
 *
 * \param device Pointer to the B20 data structure containing settings
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_getSdkVersion(BzDevice_t *hw_priv);

/**
 * \brief get the firmware version message
 *
 * \param device Pointer to the B20 data structure containing settings
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t bz_getFirmwareVersion(BzDevice_t *hw_priv);


/**
 * \brief get riscv log
 *
 * \param device Pointer to the B20 data structure containing settings
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t BZ_getRiscvLog(BzDevice_t *hw_priv);
#endif
