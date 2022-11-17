#include "bz_talise.h"
#include "command.h"
#include "auxADCDAC.h"
#include "bz_types.h"
#include "bz_gpio_types.h"
#include "bz_gpio.h"
#include <stdio.h>
#include <unistd.h>

static void init_device(uint32_t device_id)
{
    open_device(device_id);
    rest_device();
    wait_boot_status();
    load_firmware();
    wait_firmware_ready();
    load_config();
    write_profile();
    wait_profile_done();
}

static void set_freq(uint64_t rf_freq_hz, uint64_t aux_freq_hz)
{
    set_rf_pll_freq(rf_freq_hz);
    wait_pll_lock_done();
    set_aux_pll_freq(aux_freq_hz);
    wait_pll_lock_done();
}

static BzDevice_t g_device = { 0 };

static void test_auxadc()
{	
    BzAuxAdcConfig_t stAuxAdcCfg = {
        .auxAdcChannelSel = BZ_AUXADC_CH3, /*!< Selects the channel which is supposed to sample AuxADC input for A/D conversion */
        .auxAdcMode = BZ_AUXADC_PIN_MODE,  /*!< Selects mode to latch and store conversion results */
        .numSamples = 1000,                /*!< No. of A/D conversions to be performed in range 1 - 1000 */
        .samplingPeriod_us = 8,            /*!< Sampling interval time in microseconds (Mininum 1us,Maxnum 8us) NOTE: Valid only for non pin mode. Ignored for pin mode. */
        .auxadcGpioSel = 3                 /*!< auxadc gpiosel can use gpio2-9 & gpio10-13*/
    };

    BzAuxAdcResult_t stAdcResult = {
        .auxAdcCodeAvg = 0, /*!< 12-bit Average of AuxADC A/D conversion samples */
        .completeIndicator = 0 /*!< Flag to indicate if a scheduled AuxADC conversion completed. 1 - AuxADC Conversion Complete, 0 - AuxADC Conversion Incomplete */
    };
    start_auxadc(&stAuxAdcCfg);
    for(int i=0; i<5; i++)
    {
        read_auxadc(&stAdcResult);
        printf("aux ADC read result = %d\n" , stAdcResult.auxAdcCodeAvg);
        usleep(1000000);
    }
    printf("aux ADC test finish!\n");
}

static void test_auxdac()
{
    int siPin = 6;
    setup_auxdac(siPin);
    printf("aux DAC test Pin = %d\n",siPin);
    printf("testing ...\n");
    for(int i = 0; i < 1023; i++)
    {
        write_auxdac(siPin, i); // Pin = 6 , code = 0 ~ 1023
        usleep(10000); 
    }
    printf("aux DAC test finish!\n");
}

void aux_adcdac_test()
{
    RegisterCommand("init", "init bz20 device", (void *)init_device);
    RegisterCommand("term", "term bz20 device", (void *)close_device);
    RegisterCommand("setfreq", "setfreq rf_freq_hz  rf_freq_hz", (void *)set_freq);
    RegisterCommand("auxADCtest", "test aux ADC function",(void *)test_auxadc);
    RegisterCommand("auxDACtest", "test aux DAC function",(void *)test_auxdac);
}
