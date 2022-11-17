#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "si5344.h"
#include "ad9528.h"

#include <error.h>
#include "util.h"

#include <appclock.h>

static struct ad9528_dev* clkchip_device;

static void init_ad9528(void)
{
	int32_t  status;

	struct ad9528_channel_spec ad9528_channels[14];
	struct ad9528_init_param ad9528_param;
	struct ad9528_platform_data ad9528_pdata;

	// ad9528 defaults
	ad9528_param.pdata = &ad9528_pdata;
	ad9528_param.pdata->num_channels = 14;
	ad9528_param.pdata->channels = &ad9528_channels[0];

	status = ad9528_init(&ad9528_param);
	if(status) {
		printf("error: ad9528_init() failed with %d\n", status);
		goto error_0;
	}

	// ad9528 channel defaults
	for(unsigned int ch = 0; ch < ad9528_param.pdata->num_channels; ch++) {
		ad9528_channels[ch].channel_num = ch;
		ad9528_channels[ch].output_dis = 1;
	}

	// ad9528 channel specifics

	//ADRV9009_A JESD204B Refclk = 122.88
	ad9528_channels[0].output_dis = 0;
	ad9528_channels[0].driver_mode = DRIVER_MODE_LVDS;
	ad9528_channels[0].divider_phase = 0;
	ad9528_channels[0].channel_divider = 10;
	ad9528_channels[0].signal_source = SOURCE_VCO;

	//ADRV9009_A JESD204B SYSRef = 960k
	ad9528_channels[1].output_dis = 0;
	ad9528_channels[1].driver_mode = DRIVER_MODE_LVDS;
	ad9528_channels[1].divider_phase = 0;
	//ad9528_channels[1].channel_divider = 2;
	ad9528_channels[1].signal_source = SOURCE_SYSREF_VCO;

    //FPGA JESD204B Refclk0 = 122.88
	ad9528_channels[2].output_dis = 0;
	ad9528_channels[2].driver_mode = DRIVER_MODE_LVDS_BOOST;
	ad9528_channels[2].divider_phase = 0;
	ad9528_channels[2].channel_divider = 10;
	ad9528_channels[2].signal_source = SOURCE_VCO;

    //ADRV9009_B JESD204B Refclk = 122.88
	ad9528_channels[3].output_dis = 0;
	ad9528_channels[3].driver_mode = DRIVER_MODE_LVDS;
	ad9528_channels[3].divider_phase = 0;
	ad9528_channels[3].channel_divider = 10;
	ad9528_channels[3].signal_source = SOURCE_VCO;

	//ADRV9009_B JESD204B SYSRef = 960k
	ad9528_channels[4].output_dis = 0;
	ad9528_channels[4].driver_mode = DRIVER_MODE_LVDS;
	ad9528_channels[4].divider_phase = 0;
	//ad9528_channels[4].channel_divider = 2;
	ad9528_channels[4].signal_source = SOURCE_SYSREF_VCO;

    //FPGA JESD204B Refclk1 = 122.88
	ad9528_channels[5].output_dis = 0;
	ad9528_channels[5].driver_mode = DRIVER_MODE_LVDS_BOOST;
	ad9528_channels[5].divider_phase = 0;
	ad9528_channels[5].channel_divider = 10;
	ad9528_channels[5].signal_source = SOURCE_VCO;

	//FPGA JESD204B SYSRef = 960k
	ad9528_channels[12].output_dis = 0;
	ad9528_channels[12].driver_mode = DRIVER_MODE_LVDS;
	ad9528_channels[12].divider_phase = 0;
	ad9528_channels[12].signal_source = SOURCE_SYSREF_VCO;


	// ad9528 settings
	ad9528_param.pdata->spi3wire = 0;
	ad9528_param.pdata->vcxo_freq = 122880000;
	ad9528_param.pdata->refb_en = 1;
	ad9528_param.pdata->refa_en = 0;
	ad9528_param.pdata->refb_diff_rcv_en = 1;
	ad9528_param.pdata->refa_diff_rcv_en = 1;
	//ad9528_param.pdata->refb_cmos_neg_inp_en = 0;
	ad9528_param.pdata->refb_r_div = 1;
	ad9528_param.pdata->refa_r_div = 1;
	ad9528_param.pdata->osc_in_diff_en = 0;
	ad9528_param.pdata->osc_in_cmos_neg_inp_en = 0;
	ad9528_param.pdata->pll1_feedback_div = 1;
	ad9528_param.pdata->pll1_feedback_src_vcxo = 0; /* VCO */
	ad9528_param.pdata->pll1_charge_pump_current_nA = 500; //R ext_zero	 293k   LF1 ext_cap	 176nF
	ad9528_param.pdata->pll1_bypass_en = 0;
	ad9528_param.pdata->pll2_vco_div_m1 = 3;
	ad9528_param.pdata->pll2_n2_div = 10;
	ad9528_param.pdata->pll2_r1_div = 1;
	ad9528_param.pdata->pll2_charge_pump_current_nA = 805000;
	ad9528_param.pdata->pll2_bypass_en = false;
	ad9528_param.pdata->sysref_src = SYSREF_SRC_INTERNAL;
	ad9528_param.pdata->sysref_k_div = 80;    //80->768K,64->960K
	ad9528_param.pdata->sysref_pattern_mode = SYSREF_PATTERN_CONTINUOUS;
	ad9528_param.pdata->sysref_req_en = true;
	ad9528_param.pdata->sysref_nshot_mode = SYSREF_NSHOT_4_PULSES;
	ad9528_param.pdata->sysref_req_trigger_mode = SYSREF_LEVEL_HIGH;
	ad9528_param.pdata->rpole2 = RPOLE2_900_OHM;
	ad9528_param.pdata->rzero  = RZERO_2750_OHM;  //RZERO_2750_OHM
	ad9528_param.pdata->cpole1 = CPOLE1_32_PF;    //CPOLE1_32_PF;
	ad9528_param.pdata->stat0_pin_func_sel = 0x1; /* PLL1 & PLL2 Locked */
	ad9528_param.pdata->stat1_pin_func_sel = 0x8; /* REFA Correct */

	/** < Insert User System Clock(s) Initialization Code Here >
	* System Clock should provide a device clock and SYSREF signal
	* to the Talise device.
	**/
	status = ad9528_setup(&clkchip_device, ad9528_param);
	if(status < 0) {
		printf("error: ad9528_setup() failed with %d\n", status);
		goto error_1;
	}
	return;
error_1:
	printf("test_2\n");

error_0:
	return;
}

static void term_ad9528(void)
{
	ad9528_remove(clkchip_device);
}

void appclock_init(void)
{
	si5344_init();
	init_ad9528();
}

void appclock_sync(void)
{
	ad9528_204b_sync_clock();
}

void appclock_term(void)
{
	term_ad9528();
	si5344_term();
}
