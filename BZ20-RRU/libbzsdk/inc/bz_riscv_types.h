/**
* \file bz_riscv_types.h
* \brief Contains Bz API Riscv data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/
#ifndef __BZ_RISCV_TYPES_H__
#define __BZ_RISCV_TYPES_H__

#include "bz_data_types.h"

#define TX_PROFILE_VALID    			0x01
#define RX_PROFILE_VALID    			0x02
#define ORX_PROFILE_VALID   			0x04
/*Drvier commandType*/
#define BZ_ABORT_OPCODE					0x0
#define BZ_RUNN_INIT_OPCODE				0x1
#define BZ_RADIO_ON_OPCODE				0x2
#define BZ_WRITE_MEM_OPCODE				0x3
#define BZ_READ_MEM_OPCODE				0x4
#define	BZ_SET_OPCODE					0x5
#define BZ_GET_OPCODE					0x6
#define BZ_GET_MEMORY_DATA 		        0x7



#define PHY_INIT_CALI_INIT            	0x11
#define RFPLL_FREQ_CALI_INIT	        0x22
#define ENABLE_SYSREF_INIT  	        0x33
#define ENABLE_DBG_PRINT2SPI  	      	0x34
#define HI_MSG_TO_JESD204B_MSGID  		0x36
#define RESET_DEVICE_MSGID  		   	0x37
#define HI_MSG_ENABLE_LOG_MSGID  		0x38
#define HI_SET_GPIO_VALUE  		        0x3A
#define HI_GET_GPIO_VALUE  		        0x3B
#define HI_ENABLE_SLICER  		        0x3C
#define HI_ENABLE_PLLTC_TRACK			0x3D
#define HI_ENABLE_FRAMER_A  		    0x3E
#define HI_ENABLE_FRAMER_B  		    0x3F
#define HI_ENABLE_DFRAMER  		        0x40
#define HI_GET_DACPOWER 		        0x41
#define HI_GET_ORXDACPOWER 		        0x42

#define RECV_CMD_CONFIRM  				REG_RSV
#define SEND_CMD_TYPE  					REG_RSV_1
#define SEND_CMD_DATA					REG_RSV_2
#define DOCMD_STATUS					REG_RSV_3

#define DBG_PRINT_BUF_SIZE_MAX 			 256

#define DOWNLOAD_ITCM_ADDR				 (0x1000)
#define DOWNLOAD_ITCM_SIZE				 (64*1024)
#define DOWNLOAD_DTCM_ADDR				 (0x1001)
#define DOWNLOAD_DTCM_SIZE				 (64*1024)
#define DOWNLOAD_MEM_ADDR				 (0x1003)
#define DOWNLOAD_MEM_SIZE				 (64*1024)
	
#define HI_STATUS_SUCCESS	   			0x88

#define HI_STATUS_SUCCESS_BIT	   		0
#define HI_STATUS_INVALID_DATA_BIT      1
#define HI_STATUS_FIRMWARE_READY_BIT	2
#define RFPLL_CALI_DONE_BIT				3
#define PHY_INIT_CALI_DONE_BIT			4
#define PHY_INIT_PROFILE_DONE_BIT		5
#define DBG_2SPI_DONE_BIT				6
#define JESD204B_DONE_BIT				7
#define GPIO_GET_VALUE_DONE_BIT         8

#define WDT_ERROR_BIT					31
#define PAPROTECT_ERROR_BIT				30
#define PAPROTECT_ERROR_CH_BIT			29

#define MAGIC							0xa5

#define RESET_BY_ISR  	   				0x0
#define RESET_BY_RESET     				0x1

#define IOCTL_WRITE_CMD 				(0x1)
#define IOCTL_READ_CMD 					(0x2)
#define BZ_ADDR_DATA_REGION                      0x10030000
#define BZ_ADDR_FW_VERSION                       0x10032000

#define BZ_ADDR_CALISTA_ADDR                     0x1003f010

#define HI_MSG_TO_PROFILE_MSGID  		 		0x1
#define HI_MSG_TO_PROFILE_MSGLEN		 		68
#define PROFILE_MSG_ADDR				 		BZ_ADDR_DATA_REGION

#define HI_MSG_TO_RFFRQ_MSGID  			 		0x2
#define HI_MSG_TO_RFFRQ_MSGLEN		 	 		0x8
#define RFFRQ_MSG_ADDR				 			(PROFILE_MSG_ADDR+HI_MSG_TO_PROFILE_MSGLEN) //last addr offset

#define HI_MSG_TO_AUXFRQ_MSGID  				0x6
#define HI_MSG_AUXFRQ_MSGLEN         	 		0x8
#define AUXFRQ_MSG_ADDR				 			(RFFRQ_MSG_ADDR+HI_MSG_TO_RFFRQ_MSGLEN) //last addr offset

#define HI_MSG_TO_READFIRDY_MSGID  		 		0x3
#define HI_MSG_TO_READFIRDY_MSGLEN		 		20

#define HI_MSG_TO_FILTER_MSGID  		        0x4
#define HI_MSG_TO_FILTER_GAINDB_NUMFIR_LEN		8
#define HI_MSG_TO_TXFILTER_COEFS_LEN	      	80
#define HI_MSG_TO_RX_ORX_FILTER_COEFS_LEN	  	72
#define HI_MSG_TO_TXFILTER_MSGLEN		        (HI_MSG_TO_FILTER_GAINDB_NUMFIR_LEN+HI_MSG_TO_TXFILTER_COEFS_LEN*2)
#define HI_MSG_TO_RX_ORXFILTER_MSGLEN	     	(HI_MSG_TO_FILTER_GAINDB_NUMFIR_LEN+HI_MSG_TO_RX_ORX_FILTER_COEFS_LEN*2)
#define FILTER_MSG_ADDR				 			(AUXFRQ_MSG_ADDR+HI_MSG_AUXFRQ_MSGLEN)//last addr offset
#define HI_MSG_TO_TXFILTER_TOATL_MSGLEN	      (HI_MSG_TO_TXFILTER_MSGLEN+(HI_MSG_TO_RX_ORXFILTER_MSGLEN*2))


#define HI_MSG_TO_CALSTATUS_MSGID  		 		0x5
#define HI_MSG_TO_CALSTATUS_MSGLEN       		72


#define HI_MSG_TO_J204B_MSGID        0x7
#define HI_MSG_TO_J204B_MSGLEN       256
#define J204B_MSG_ADDR				(FILTER_MSG_ADDR+HI_MSG_TO_TXFILTER_TOATL_MSGLEN) //last addr offset


#define CALIBRITION_SRAM_ADDR  (0x10039788)
#define CALIBRITION_SRAM_LEN   (2050)
#define CALI_EVENTID_CNT	   (14)       //14 calibration events

#define WIAT_TIMEOUT           0xFFFFFFFF

typedef enum
{
	BZ_TX_FILTER 	= 0x0,	  /*!< Tx Filter*/
	BZ_RX_FILTER 	= 0x1,	  /*!< Rx Filter*/
	BZ_ORX_FILTER	= 0x2	  /*!< ORx Filter*/
}BzFilterType;

#define GPIO_NUM 						19

#define REG_BIT(NUM)					NUM
#define SOFT_RESETB						REG_BIT(0)
#define BOOT_RDY						REG_BIT(6)

typedef struct __TxData{
	uint32_t value;
	uint32_t addr;
	uint32_t lo;
}BzTxData;

typedef struct CmdTxType{
	uint32_t Cmd;
	BzTxData TxData;
}BzCmdTx;

typedef struct __RxData{
	uint32_t value;
	uint32_t addr;
	uint32_t lo;
}BzRxData;

typedef struct CmdRxType{
	uint32_t Cmd;
	BzRxData RxData;
}BzCmdRx;
typedef struct _BzMsg{
	uint32_t addr;
	uint16_t Lenth;
	uint16_t Id;
	uint32_t offset;
	uint8_t data[(CALIBRITION_SRAM_LEN*4)];  
}BzMsg;

typedef struct BzMemMsg{
	uint32_t Cmd;
	BzMsg Msg;
}BzMem;

typedef struct ConfigAgcInfo{
   int regrx_clk_div;
   int regtx_clk_div;
   int regrx_gain_ctrl_mode;
   int regrx_gain_prevent;
   int regrx_agc_mode;
   int regrx_gain_wait_cnt;
   int regrx_gain_update_cnt;
   int regrx_mgc_api_index;
   int regrx_mgc_gpiodec_step;
   int regrx_mgc_gpioinc_step;
   int regrx_mgc_mode;
   int regorx_gain_ctrl_mode;
   int regorx_gain_prevent;
   int regorx_agc_mode;
   int regorx_gain_wait_cnt;
   int regorx_gain_update_cnt;
   int regorx_mgc_api_index;
   int regorx_mgc_gpiodec_step;
   int regorx_mgc_gpioinc_step;
   int regorx_mgc_mode;
   int regtx_gain_ctrl_mode;
   int regtx_gain_prevent;
   int regtx_agc_mode;
   int regtx_gain_wait_cnt;
   int regtx_gain_update_cnt;
   int regtx_mgc_api_index;
   int regrx_apd_attack_mode;
   int regrx_apd_cnt;
   int regrx_apd_disabled;
   int regrx_apd_attack_step;
   int regrx_apd_recovery_step;
   int regrx_apd_immd_over_num;
   int regrx_apd_peri_over_num;
   int regrx_apd_under_num;
   int regrx_hpd_cnt;
   int regrx_hpd_disabled;
   int regrx_hpd_recovery_mode;
   int regrx_hpd_attack_mode;
   int regrx_hpd_attack_step;
   int regrx_hpd_recovery_hig_step;
   int regrx_hpd_recovery_low_step;
   int regrx_hpd_recovery_mid_step;
   int regrx_hpd_over_th;
   int regrx_hpd_under_hig_th;
   int regrx_hpd_under_low_th;
   int regrx_hpd_under_mid_th;
   int regrx_hpd_over_num;
   int regrx_hpd_under_hig_num;
   int regrx_hpd_under_low_num;
   int regrx_hpd_under_mid_num;
   int regrx_hpd_peri_over_num;
   int regrx_hpd_peri_under_num;
   int regrx_hpd_immd_over_num;
   int regrx_hpd_immd_under_hig_num;
   int regrx_hpd_immd_under_low_num;
   int regrx_hpd_immd_under_mid_num;
   int regrx_pmd_cnt;
   int regrx_pmd_disabled;
   int regrx_in_mux;
   int regrx_pmd_attack_hig_step;
   int regrx_pmd_attack_low_step;
   int regrx_pmd_recovery_hig_step;
   int regrx_pmd_recovery_low_step;
   int regrx_pmd_over_hig_th;
   int regrx_pmd_over_low_th;
   int regrx_pmd_under_hig_th;
   int regrx_pmd_under_low_th;
   int regorx_apd_attack_mode;
   int regorx_apd_cnt;
   int regorx_apd_disabled;
   int regorx_apd_attack_step;
   int regorx_apd_recovery_step;
   int regorx_apd_immd_over_num;
   int regorx_apd_peri_over_num;
   int regorx_apd_under_num;
   int regorx_hpd_cnt;
   int regorx_hpd_disabled;
   int regorx_hpd_recovery_mode;
   int regorx_hpd_attack_mode;
   int regorx_hpd_attack_step;
   int regorx_hpd_recovery_hig_step;
   int regorx_hpd_recovery_low_step;
   int regorx_hpd_recovery_mid_step;
   int regorx_hpd_over_th;
   int regorx_hpd_under_hig_th;
   int regorx_hpd_under_low_th;
   int regorx_hpd_under_mid_th;
   int regorx_hpd_over_num;
   int regorx_hpd_under_hig_num;
   int regorx_hpd_under_low_num;
   int regorx_hpd_under_mid_num;
   int regorx_hpd_peri_over_num;
   int regorx_hpd_peri_under_num;
   int regorx_hpd_immd_over_num;
   int regorx_hpd_immd_under_hig_num;
   int regorx_hpd_immd_under_low_num;
   int regorx_hpd_immd_under_mid_num;
   int regorx_pmd_cnt;
   int regorx_pmd_disabled;
   int regorx_in_mux;
   int regorx_pmd_attack_hig_step;
   int regorx_pmd_attack_low_step;
   int regorx_pmd_recovery_hig_step;
   int regorx_pmd_recovery_low_step;
   int regorx_pmd_over_hig_th;
   int regorx_pmd_over_low_th;
   int regorx_pmd_under_hig_th;
   int regorx_pmd_under_low_th;
   int regtx_hpd_cnt;
   int regtx_hpd_disabled;
   int regtx_hpd_recovery_mode;
   int regtx_hpd_attack_mode;
   int regtx_hpd_attack_step;
   int regtx_hpd_recovery_hig_step;
   int regtx_hpd_recovery_low_step;
   int regtx_hpd_recovery_mid_step;
   int regtx_hpd_over_th;
   int regtx_hpd_under_hig_th;
   int regtx_hpd_under_low_th;
   int regtx_hpd_under_mid_th;
   int regtx_hpd_over_num;
   int regtx_hpd_under_hig_num;
   int regtx_hpd_under_low_num;
   int regtx_hpd_under_mid_num;
   int regtx_hpd_peri_over_num;
   int regtx_hpd_peri_under_num;
   int regtx_hpd_immd_over_num;
   int regtx_hpd_immd_under_hig_num;
   int regtx_hpd_immd_under_low_num;
   int regtx_hpd_immd_under_mid_num;
   int regtx_pmd_cnt;
   int regtx_pmd_disabled;
   int regtx_in_mux;
   int regtx_pmd_attack_hig_step;
   int regtx_pmd_attack_low_step;
   int regtx_pmd_recovery_hig_step;
   int regtx_pmd_recovery_low_step;
   int regtx_pmd_over_hig_th;
   int regtx_pmd_over_low_th;
   int regtx_pmd_under_hig_th;
   int regtx_pmd_under_low_th;
   int rx0_orx0_dc_ext;
   int rx1_orx1_dc_ext;
   int rx0_gain_ext;
   int rx1_gain_ext;
   int orx0_gain_ext;
   int orx1_gain_ext;
   int tx0_gain_ext;
   int tx1_gain_ext;
   int apd_reset_ext;
   int apd_rx_latch_ext;
   int apd_tx_latch_ext;
   int tx0_ext_index;
   int tx1_ext_index;
 }BzConfigAgcInfo;

typedef struct ConfigJesd204b{
	unsigned int tx_jrxlink_mode; 
	unsigned int tx_clk_input_tx;
	unsigned int tx_real_data_rate; 
	unsigned int tx_clk_character_rx; 
	unsigned int tx_serdes_clk;
	unsigned int tx_rep_rate; 
	unsigned int tx_lane;    
	unsigned int rx_jtx0link_mode; 
	unsigned int rx_clk_output_rx;
	unsigned int rx_real_data_rate;
	unsigned int rx_clk_character_tx;
	unsigned int rx_serdes_clk;
	unsigned int rx_rep_rate; 
	unsigned int rx_lane;  
	unsigned int orx_jrxlink_mode;
	unsigned int orx_clk_output_orx;
	unsigned int orx_real_data_rate;
	unsigned int orx_clk_character_tx;
	unsigned int orx_serdes_clk;
	unsigned int orx_rep_rate; 
	unsigned int orx_lane;  
}BzConfigJesd204b;

typedef struct DataType{
	uint32_t CmdType;
	union{
		BzCmdTx Tx;
		BzCmdRx Rx;
		BzMem Mmsg;
		BzConfigAgcInfo ConfigAgcInfo;
		BzConfigJesd204b ConfigJ204bInfo;
	}u;
}BzData;


struct firmware_headr {
	uint32_t itcmAddrOffset;
	uint32_t itcmAddr;
	uint32_t itcmLen;
	uint32_t itcmBlockSize;
	uint32_t dtcmAddrOffset;
	uint32_t dtcmAddr;
	uint32_t dtcmLen;
	uint32_t dtcmBlockSize;
};

struct firmware_B20 {
	struct firmware_headr hdr;
	uint8_t *fw_dccm;
	uint8_t *fw_iccm;
};

#define ELOG_LEN  1024 

struct elog_id{
	unsigned short 	id;
	unsigned short	valid;
	unsigned int  data;
};

struct elog_s {
	unsigned short start;
	unsigned short index;
	unsigned int len;
	struct elog_id e_id[ELOG_LEN];
};

struct build_info{
	int total_svn_version;
	char driver_info[64];
	char current_svn_version[64];
};


#endif
