/**
* \file bz_config.c
* \brief Contains B20 configuration settings for the B20 API
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/
#include "bz_config.h"

sint16_t bzTxFirCoefs[40] = {185,307,-238,-43,163,87,-242,-50,293,13,-353,46,410,-124,-463,225,506,-350,-536,503,545,-685,-526,901,
					 471,-1155,-366,1459,193,-1831,81,2312,-521,-2995,1285,4148,-2887,-6906,8596,30631};
sint16_t bzRxFirCoefs[36] = {163,87,-242,-50,293,13,-353,46,410,-124,-463,225,506,-350,-536,503,545,-685,-526,901,471,-1155,-366,
					1459,193,-1831,81,2312,-521,-2995,1285,4148,-2887,-6906,8596,30631};
sint16_t  bzObsrxFirCoefs[36] = {163,87,-242,-50,293,13,-353,46,410,-124,-463,225,506,-350,-536,503,545,-685,-526,901,471,-1155,-366,
					1459,193,-1831,81,2312,-521,-2995,1285,4148,-2887,-6906,8596,30631};

BzInit_t B20Init =
{
	.mode =
	{
		.tddMode=1,
		.dpdMode=0,
	},
     /* Rx settings */
    .rx =
    {
        .rxProfile =
        {
            .rxFir =
            {
                .gain_dB = -6,      			/* The setting(in DB) for the gain block within the Rx FIR(6,0,-6,-12) */
                .numFirCoefs = 72,  		/* The numbers of taps to be used in the Rx Fir (24,48,72) */
                .coefs = &bzRxFirCoefs[0],
            },
            .rxFirBypass		= 1,	  	/* The RX FIR bypass enable (0,1)*/
            .rxFirDecimation    = 1,      	/* The Rx FIR decimation (1,2,4) */
            .rhb1Decimation     = 2,      	/* The Rx Halfband1 Decimation setting(1,2) */
            .rxOutputRate_kHz   = 245760, 	/* The I/Q data rate at input to the Rx output data rate in kHz */
            .rfBandwidth_Hz     = 200000000,/* The Rx RF passband bandwidth for the profile */
            .rxBbf3dBCorner_kHz = 220000,	/* The Rx BBF 3dB corner in kHz  <1.1 * rxrfBandwidth_Hz/1000>*/
        },
        .rxGainCtrl =
        {
            .rxgainMode      = BZ_MGC,  	/* The MGC mode setting(0,1)*/
			.rxmgcMode		 = BZ_MGC,		/* The Ways for mgcMode(0,1) */
            .rx1GainIndex    = 127,  		/* The rx1GainIndex setting(0,127) */
            .rx2GainIndex    = 127,  		/* The rx2GainIndex setting(0,127)*/
			.rx1MaxGainIndex = 127,  		/* The Rx1 GainMax index 127*/
			.rx1MinGainIndex = 0,  			/* The Rx1 GainMix index 0;*/
			.rx2MaxGainIndex = 127,  		/* The Rx2 GainMax index 127*/
			.rx2MinGainIndex = 0	 		/* The Rx2 GainMix index 0;*/
        },
        .rxGainCtrlPinInfo =
        {
			.incStep = 1,			        /* Increment in gain index applied when the increment gain pin is pulsed(1-16) */			
			.decStep = 1,			        /* Decrement in gain index applied when the increment gain pin is pulsed(1-16) */
			.rx1GainCtrlPinInfo = 
			{
				.rxGainIncPin   = BZ_GPIO_08,  /*GPIO used for the rx1 Increment gain input*/
				.rxGainDecPin   = BZ_GPIO_09,  /*GPIO used for the rx1 Decrement gain input*/
				.rxGainIncPinFunc = BZ_SEL_1,  /*GPIO function select 0-7: make GPIO function rx1 gain Increment function */
				.rxGainDecPinFunc = BZ_SEL_1,  /*GPIO function select 0-7:*make GPIO function rx1 gain Decrement function */
			},
			.rx2GainCtrlPinInfo = 
			{			
				.rxGainIncPin   = BZ_GPIO_10,  /*GPIO used for the rx2 Increment gain input*/
				.rxGainDecPin   = BZ_GPIO_11,  /*GPIO used for the rx2 Decrement gain input*/
				.rxGainIncPinFunc = BZ_SEL_1,  /*GPIO function select 0-7: make GPIO function rx2 gain Increment function */
				.rxGainDecPinFunc = BZ_SEL_1,  /*GPIO function select 0-7:*make GPIO function rx2 gain Decrement function */
			},
        },        

		.rxSlicerPinInfo = 
		{
			
			.EnableSlicer = BZ_RXSLICER_EN,	/*Default No Digital gain compensation slicer output*/ 
			.rx1SlicerPinInfo = 
			{
				.rx1SlicerPin_0   = BZ_GPIO_07,  /*GPIO used for the rx1 Slicer pin output*/
				.rx1SlicerPin_1   = BZ_GPIO_08,  /*GPIO used for the rx1 Slicer pin output*/
				.rx1SlicerPin_2   = BZ_GPIO_09,  /*GPIO used for the rx1 Slicer pin output*/
			},			
			.rx2SlicerPinInfo = 
			{
				.rx2SlicerPin_0   = BZ_GPIO_10,  /*GPIO used for the rx2 Slicer pin output*/
				.rx2SlicerPin_1   = BZ_GPIO_11,  /*GPIO used for the rx2 Slicer pin output*/
				.rx2SlicerPin_2   = BZ_GPIO_12,  /*GPIO used for the rx2 Slicer pin output*/
			},
			.rxSlicerPinFunc = BZ_SEL_3,  		 /*GPIO function select 0-7: make GPIO function slicer function */
		},
        .rxChannels = BZ_RXOFF      		/* Select The enable rx channel */
    },

    /* Tx settings */
    .tx =
    {
        .txProfile =
        {
            .txFir =
            {
                .gain_dB = 0,       		/* The setting(in DB) for the gain block within the Tx FIR(6,0,-6,-12) */
                .numFirCoefs = 80,  		/* The numbers of taps to be used in the Tx Fir (20,40,80) */
                .coefs = &bzTxFirCoefs[0],
            },
			 .txFirBypass			 = 1,		/* The TX FIR bypass enable (0,1)*/
            .txFirInterpolation      = 1,       /* The Tx FIR filter interpolation setting(1,2,4)     */
            .thb1Interpolation       = 2,     	/* The Tx Halfband1 filter interpolation setting(1,2) */
            .thb2Interpolation       = 2,       /* The Tx Halfband2 filter interpolation settingg(1,2)*/
            .thb3Interpolation       = 1,       /* The Tx Halfband3 filter interpolation setting(1,2) */
            .txInputRate_kHz         = 491520,  /* The I/Q data rate at input to the TxFir rate in kHz range 491.52/245.76/122.88Mhz*/
            .rfBandwidth_Hz          = 400000000,/* The Tx RF passband bandwidth for the profile */
            .txBbf3dBCorner_kHz		 = 220000,  /*  The Tx BBF 3dB corner in kHz  <1.1 * txrfBandwidth_Hz/1000>*/
        },        
        .txChannels = BZ_TX1TX2,         /* Select The enable tx channel */
        .txGainCtrl =
        {
            .txgainMode 	  = BZ_MGC, /* The MGC mode setting(0)*/
			.txmgcMode		  = BZ_APIMODE,/* The Ways for mgcMode(0) */
			.tx1GainIndex	  = 40,   	/* The tx1GainIndex setting(0,63) */
			.tx2GainIndex	  = 40,		/* The tx2GainIndex setting(0,63) */
			.tx1MaxGainIndex = 63,		/* The Tx1 GainMax index 63*/
			.tx1MinGainIndex = 0,		/* The Tx1 GainMin index 0*/
			.tx2MaxGainIndex = 63,		/* The Tx2 GainMax index 63*/
			.tx2MinGainIndex = 0		/* The Tx2 GainMin index 0*/
        },
    },

    /* ObsRx settings */
    .obsRx =
    {
        .orxProfile =
        {
            .rxFir =
            {
                .gain_dB = 6,               /* The setting(in DB) for the gain block within the Orx FIR(6,0,-6,-12) */
                .numFirCoefs = 72,          /* The numbers of taps to be used in the Orx Fir (24,48,72) */
                .coefs = &bzObsrxFirCoefs[0],
            },
            .orxFirBypass		 = 1,        /* The ORX FIR bypass enable (0,1)*/
            .orxFirDecimation    = 1,        /* The ORx FIR decimation (1,2) */
            .orxOutputRate_kHz  = 491520,    /* The I/Q data rate at input to the ORX data rate in kHz */
            .rfBandwidth_Hz     = 450000000, /* The ORx RF passband bandwidth for the profile */
            .rxBbf3dBCorner_kHz = 495000,    /* The ORx BBF 3dB corner in kHz  <1.1 * orxrfBandwidth_Hz/1000>*/
        },
        .orxGainCtrl =
        {
            .orxgainMode 	  = BZ_MGC,		/* The MGC mode setting(0,1)*/
			.orxmgcMode		  = BZ_APIMODE,	/* The Ways for mgcMode(0,1) */
			.orx1GainIndex	  = 127,			/* The orx1GainIndex setting(0,127) */
			.orx2GainIndex	  = 127,			/* The orx2GainIndex setting(0,127) */
			.orx1MaxGainIndex = 127,		/* The Orx1 GainMax index 127*/
			.orx1MinGainIndex = 0,			/* The Orx1 GainMax index 0*/
			.orx2MaxGainIndex = 127,		/* The Orx2 GainMax index 127*/
			.orx2MinGainIndex = 0			/* The Orx2 GainMax index 0*/
        },
        .obsRxChannelsEnable = BZ_ORX1ORX2,    /* The desired ObsRx Channels to enable during initialization */
    },

    /* Digital Clock settings */
    .clocks =
    {
        .deviceClock_kHz    = 122880,  				/* CLKPLL and device reference clock frequency in kHz */
        .rfpll_frequency_Hz  = 2400000,  			/* CLKPLL VCO frequency in kHz */
        .rfPllUseExternalLo = 0,         			/*  1= Use external LO for RF PLL, 0 = use internal LO generation for RF PLL */
        .rfPllPhaseSyncMode = BZ_RFPLLMCS_NOSYNC  	/* RFPLL MCS (Phase sync) mode */
    },
    
    /* JESD204B settings */
	.jesd204Settings =
	{
		.serdesClk = SERDES_CLK_10G, /* JESD204B  Transmission Speed (10G:real speed 9.8304G, 5G:real speed 4.9152G)*/
		
		/* tx Config settings */
		.txConfig =
		{
			.lid						 = 0x0a418820,/* Lane ID for Lane0 to 3 (LID0 [0:4], LID1 [5:9], LID2 [10:14], LID3 [15:19])*/
			.subClass					 = 1,         /* Subclass mode of operation (0, 1, or 2) */
			.cf 						 = 0,         /* number of control words per converter in 1 frame period */
			.cs 						 = 0,         /* number of control bits in each converter sample data */
			.f							 = 2,         /* F (number of bytes per JESD204 Frame) */
			.k							 = 32,        /* the number of frames in each multi frame. It is recommended to set it to 32 */
			.l							 = 4,         /* number of channels (l can be set to 1 ,2 or 4) */
			.n							 = 16,        /* converter resolution (n can be set to 16) */
			.m							 = 4,         /* Number of converters (ADC) (M can be set to 0, 2 or 4) */
			.ntotal 					 = 16,        /* bit width of each sample data (n' can be set to 16), N'= n + control and pseudo data bits */
			.s							 = 1,         /* number of samples sent by each converter per frame (s can be set to 1) */
			.hd 						 = 0x1,       /* High Density Format(0:High Density format not used,1:High Density format used)*/
			.rxCoreInterruptMask 	     = 0xFFFD,    /* Register to hold interrupt mask bits (0:indicates the interrupt source is unmasked.1: indicates the interrupt source is masked)*/
			.rxIlasAdnMiscDetails12c     = 0x0F000080,/* Register to hold details of ILAS length and RBD information */
			.scr						 = 0x0,       /* Scrambling enabled */
			.enableLinkTest 			 = 0x0,       /* Enable of link layer test(0:No link layer test performed.1:Transmit continuous sequence of /K28.5/characters (LINKTEST1).
																				  2:Transmit code group sequence, followed by repeated lane alignment sequence (LINKTEST2).3:No link layer test performed)*/
			.rxFeatureDisable 		     = 0x10,	  /*Disable features in the Rx module for compatibility with other devices.*/
		},
		/* rx Config settings */
		.rxConfig =
		{
			.lid						 = 0x0a418820,/* Lane ID for Lane0 to 3 (LID0 [0:4], LID1 [5:9], LID2 [10:14], LID3 [15:19])*/
   			.subClass					 = 1,         /* Subclass mode of operation (0, 1, or 2) */
   			.cf 						 = 0,         /* number of control words per converter in 1 frame period */
   			.cs 						 = 0,         /* number of control bits in each converter sample data */
   			.f							 = 4,         /* F (number of bytes per JESD204 Frame) */
   			.k							 = 32,        /* the number of frames in each multi frame. It is recommended to set it to 32 */
   			.l							 = 2,         /* number of channels (l can be set to 1 ,2 or 4) */
   			.n							 = 16,        /* converter resolution (n can be set to 16) */
   			.m							 = 4,         /* Number of converters (ADC) (M can be set to 0, 2 or 4) */
   			.ntotal 					 = 16,        /* bit width of each sample data (n' can be set to 16), N'= n + control and pseudo data bits */
   			.s							 = 1,         /* number of samples sent by each converter per frame (s can be set to 1) */
   			.hd 						 = 0x1,       /* High Density Format(0:High Density format not used,1:High Density format used) Currently not supported*/
			.txIlasDelay				 = 0xc,      /* Defines the delay from initial sample requests on the TX sample interface until the link layer requests the first frame from the transport layer. 
														The resolution is clk_character cycles. A rough formula is:F=1 - delay must be set to 4，F=2 - delay must be set to 6，F=3 - delay must be set to 8，
														F>3 - delay must be 8 + F，For F>3 the value may need fine tuning, especially if the ratio between clk_character and clk_sample is not 1:1. 
														It should work in most cases, though.The user is encouraged to verify the setting in simulations for specific cases */
			.rxIlasAdnMiscDetails12c     = 0x0F000080,/* Register to hold details of ILAS length and other link parameters */
			.scr						 = 0x0,       /* Scrambling enabled */
			.coreInterruptMask		     = 0xd,       /* Register to hold interrupt mask bits (0:indicates the interrupt source is unmasked.1: indicates the interrupt source is masked)*/
			.tailbits					 = 0x0,       /* Control insertion of tail bits (0:Insert ‘0’,1:Insert ‘1’,2:Insert tail bit generated from an LFSR (or other construct), 
			                                                                             that ensures low DC content,3:Invalid (Insert ‘0’)) */
			.enableLinkTest 			 = 0x1,       /*Enable of link layer test (0:No link layer test performed,1:Transmit continuous sequence of /K28.5/characters (LINKTEST1).
															                       2:Transmit code group sequence, followed by repeated lane alignment sequence (LINKTEST2),3:No link layer test performed)*/
			.rxFeatureDisable 		     = 0x10,      /* Disable features in the Rx module for compatibility with other devices */
		},
		/* orx Config settings */
		.orxConfig =
		{
			.lid						 = 0x0a418820,/* Lane ID for Lane0 to 3 (LID0 [0:4], LID1 [5:9], LID2 [10:14], LID3 [15:19])*/
			.subClass					 = 1,         /* Subclass mode of operation (0, 1, or 2) */
			.cf 						 = 0,         /* number of control words per converter in 1 frame period */
			.cs 						 = 0,         /* number of control bits in each converter sample data */
			.f							 = 2,         /* F (number of bytes per JESD204 Frame) */
			.k							 = 32,        /* the number of frames in each multi frame. It is recommended to set it to 32 */
			.l							 = 2,         /* number of channels (l can be set to 1 ,2 or 4) */
			.n							 = 16,        /* converter resolution (n can be set to 16) */
			.m							 = 4,         /* Number of converters (ADC) (M can be set to 0, 2 or 4) */
			.ntotal 					 = 16,        /* bit width of each sample data (n' can be set to 16), N'= n + control and pseudo data bits */
			.s							 = 1,         /* number of samples sent by each converter per frame (s can be set to 1) */
			.hd 						 = 0x1,       /* High Density Format(0:High Density format not used,1:High Density format used) Currently not supported*/
			.txIlasDelay				 = 0xc,      /* Defines the delay from initial sample requests on the TX sample interface until the link layer requests the first frame from the transport layer. 
														The resolution is clk_character cycles. A rough formula is:F=1 - delay must be set to 4，F=2 - delay must be set to 6，F=3 - delay must be set to 8，
														F>3 - delay must be 8 + F，For F>3 the value may need fine tuning, especially if the ratio between clk_character and clk_sample is not 1:1. 
														It should work in most cases, though.The user is encouraged to verify the setting in simulations for specific cases */
			.rxIlasAdnMiscDetails12c     = 0x0F000080,/* Register to hold details of ILAS length and other link parameters */
			.scr						 = 0x0,       /* Scrambling enabled */
			.coreInterruptMask		     = 0xd,       /* Register to hold interrupt mask bits (0:indicates the interrupt source is unmasked.1: indicates the interrupt source is masked)*/
			.tailbits					 = 0x0,       /* Control insertion of tail bits (0:Insert ‘0’,1:Insert ‘1’,2:Insert tail bit generated from an LFSR (or other construct), 
			                                                                             that ensures low DC content,3:Invalid (Insert ‘0’)) */
			.enableLinkTest 			 = 0x0,       /*Enable of link layer test (0:No link layer test performed,1:Transmit continuous sequence of /K28.5/characters (LINKTEST1).
															                       2:Transmit code group sequence, followed by repeated lane alignment sequence (LINKTEST2),3:No link layer test performed)*/
			.rxFeatureDisable 		     = 0x10,      /* Disable features in the Rx module for compatibility with other devices */
	   },
	}

};
