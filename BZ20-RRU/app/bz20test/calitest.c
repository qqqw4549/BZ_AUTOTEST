#include "calitest.h"

#include "bz_talise.h"
#include "command.h"

#define JESD204B_WAIT_MAX_COUNT 10
#define JESD204B_WAIT_TIME 1000000 //1s

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

static void cals(uint32_t mask)
{
    run_cals(mask);
    wait_cals_done();
}

void cali_test(void)
{
    RegisterCommand("init", "init bz20 device", (void *)init_device);
    RegisterCommand("term", "term bz20 device", (void *)close_device);
    RegisterCommand("setfreq", "setfreq rf_freq_hz  rf_freq_hz", (void *)set_freq);
    RegisterCommand("cals", "cals mask eg. cals 0x40\n"
                    "             0x40   - DC offset Calibration\n"
                    "             0x80   - Rx Quadrature error correction Calibration\n"
                    "             0x2000 - Exterbal Tx LO leakage Calibration\n"
                    "             0x4000 - Tx Quadrature error correction Calibration", 
        (void *)cals);
    RegisterCommand("radioopen", "open radio", (void *)radio_on);
    RegisterCommand("tx1tone", "tx1tone freq1_khz  freq2_khz", (void *)tx1_tone);
    RegisterCommand("tx2tone", "tx2tone freq1_khz  freq2_khz", (void *)tx2_tone);
}
