#include "jesd204btest.h"
#include "bz_talise.h"
#include "command.h"
#include "bz_jesd204b_types.h"

#include <unistd.h>
#include <stdio.h>

#define JESD204B_WAIT_MAX_COUNT 10
#define JESD204B_WAIT_TIME 1000000 //1s
#define TESTCASE 3 

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



static void set_jest204b()
{
    int index = 0;
    /*typedef struct stJESD204B_Cfg{
        rfBandwidth_Hz
        TX DAC RATE(MHz) ?
        txInputRate_kHz
        serdesClk
        rep rate ?
        f 
        l 
        m 
        n
        s 
    }BzJesd204bCfg_t;*/
    BzTxConfig_t stArrayTXCfg[TESTCASE] = {
        { .f = 4, .k = 32, .l = 2, .n = 16, .m = 1, .ntotal = 16, .s = 1, .hd = 0x1},
        { .f = 4, .k = 32, .l = 2, .n = 16, .m = 1, .ntotal = 16, .s = 1, .hd = 0x1},
        { .f = 4, .k = 32, .l = 2, .n = 16, .m = 1, .ntotal = 16, .s = 1, .hd = 0x1}
    }; 
    BzRxOrXConfig_t stArrayRXCfg[TESTCASE] = {
        { .f = 4, .k = 32, .l = 2, .n = 16, .m = 1, .ntotal = 16, .s = 1, .hd = 0x1, .txIlasDelay = 4},
        { .f = 4, .k = 32, .l = 2, .n = 16, .m = 1, .ntotal = 16, .s = 1, .hd = 0x1, .txIlasDelay = 6},
        { .f = 4, .k = 32, .l = 2, .n = 16, .m = 1, .ntotal = 16, .s = 1, .hd = 0x1, .txIlasDelay = 8}
    }; 
    
    for(index = 0; index < TESTCASE; index++) // batch test for JESD204B sync stability
    {
        start_204b_sysc(); 
        jesd204b_cfgchange(stArrayTXCfg[index],stArrayRXCfg[index]); // change 204b parameters
        jesd204b_config(); // configuration
        
        wait_jesd204b_config();
        {
            int i;
            for(i = 0; i < JESD204B_WAIT_MAX_COUNT; i++)
            {
                if(jesd204b_sysc_is_ok())
                {
                    printf("204B sync is OK\n\n");
                    break;
                }
                usleep(JESD204B_WAIT_TIME);
            }
            if(i==JESD204B_WAIT_MAX_COUNT)printf("204B sync fail\n");
        }
        end_204b_sysc();
    }
}

void jesd204b_test(void)
{
    RegisterCommand("init", "init bz20 device", (void *)init_device);
    RegisterCommand("term", "term bz20 device", (void *)close_device);
    RegisterCommand("setfreq", "setfreq rf_freq_hz  aux_freq_hz", (void *)set_freq);
    RegisterCommand("set204b", "config 204b", (void *)set_jest204b);
}
