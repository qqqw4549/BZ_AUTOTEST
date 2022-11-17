/**
* \file Bz_jesd204_types.h
* \brief Contains B20 API JESD data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef _BZ_JESD204B_TYPE_H_
#define _BZ_JESD204B_TYPE_H_
#include "bz_data_types.h"


#define CLK_FOR_JESD204B_62500k    (62500) 
#define CLK_FOR_JESD204B_125000k   (125000) 
#define CLK_FOR_JESD204B_250000k   (250000)
#define CLK_FOR_JESD204B_500000k   (500000) 

#define JESD204B_REP_RATE_1   (1) 
#define JESD204B_REP_RATE_2   (2)
#define JESD204B_REP_RATE_4   (4)
#define JESD204B_REP_RATE_8   (8)

#define JESD204B_LANE_NUM_1   (1) 
#define JESD204B_LANE_NUM_2   (2)
#define JESD204B_LANE_NUM_4   (4)


typedef struct baizeTXConfig
{
	uint32_t lid;
	uint32_t subClass;
	uint32_t cf;
	uint32_t cs;
	uint32_t f;
	uint32_t k;
	uint32_t l;
	uint32_t n;
	uint32_t m;
	uint32_t ntotal;
	uint32_t s;
	uint32_t hd;
	uint32_t rxCoreInterruptMask;
	uint32_t rxIlasAdnMiscDetails12c;
	uint32_t scr;
	uint32_t enableLinkTest;
	uint32_t rxFeatureDisable;  
} BzTxConfig_t;

typedef struct 
{
	uint32_t lid;
	uint32_t subClass;
	uint32_t cf;
	uint32_t cs;
	uint32_t f; 
	uint32_t k;
	uint32_t l;
	uint32_t n;
	uint32_t m;
	uint32_t ntotal;
	uint32_t s;
	uint32_t hd;
	uint32_t txIlasDelay;
	uint32_t rxIlasAdnMiscDetails12c;
	uint32_t scr;
	uint32_t coreInterruptMask;
	uint32_t tailbits;
	uint32_t enableLinkTest;
	uint32_t rxFeatureDisable; 
} BzRxOrXConfig_t;


typedef struct 
{
	uint32_t		serdesClk;
	BzTxConfig_t	txConfig;
    BzRxOrXConfig_t	rxConfig;	
	BzRxOrXConfig_t	orxConfig;	
}BzJesdSettings_t;

typedef enum
{
    BZ_FRAMER_A = 0,       /*!< Framer A selection */
    BZ_FRAMER_B,           /*!< Framer B selection */
    BZ_FRAMER_A_AND_B      /*!< Used for cases where Orx uses one framer, Rx uses the second framer */
} BzFramerSel_t;

typedef enum
{
    BZ_DEFRAMER = 0,    /*!< Deframer selection */
} BzDeframerSel_t;


#define JES204B_LINK_MODE_0  (0) 
#define JES204B_LINK_MODE_1  (1) 
#define JES204B_LINK_MODE_2  (2)  
#define JES204B_LINK_MODE_4  (4) 


/* tx/rx/orx Config settings, all clk = (clk *1000)*/
#define CLK_TX_RX_62_5M  (61440)
#define CLK_TX_RX_125M   (122880)
#define CLK_TX_RX_250M   (245760)
#define CLK_TX_RX_500M   (491520)



#define CLK_CHARACTER_125M   (125) 
#define CLK_CHARACTER_250M   (250)



#define SERDES_CLK_10G   (10) 
#define SERDES_CLK_5G    (5)


#define JES204B_REP_RATE_1 (1)
#define JES204B_REP_RATE_2 (2)
#define JES204B_REP_RATE_4 (4)
#define JES204B_REP_RATE_8 (8)

#define JES204B_LANE_1 (1)
#define JES204B_LANE_2 (2)
#define JES204B_LANE_4 (4)
#define JES204B_LANE_8 (8)

#define JESD_DEFRAMER_ADDR 	0x25006050
#define JESD_FRAMER_A_ADDR 	0x25007050
#define JESD_FRAMER_B_ADDR 	0x25008050

typedef enum
{
	BZ_JES204B_SYSREF_ON = 0x1,   /*!<Jes204b sysref enable*/
	BZ_JES204B_SYSREF_OFF = 0x2	  /*!<Jes204b sysref disable*/
} BzJes204bSysrefControlMode;


#endif //__BZ_JESD204B_TYPE_H__ 

