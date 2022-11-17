#include "tonetest.h"
#include "bz_talise.h"
#include "command.h"

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

void tone_test(void)
{
    RegisterCommand("init", "init bz20 device", (void *)init_device);
    RegisterCommand("term", "term bz20 device", (void *)close_device);
    RegisterCommand("setfreq", "setfreq rf_freq_hz  aux_freq_hz", (void *)set_freq);
    RegisterCommand("radioopen", "open radio", (void *)radio_on);
    RegisterCommand("tx1tone", "tx1tone freq1_khz  freq2_khz", (void *)tx1_tone);
    RegisterCommand("tx2tone", "tx2tone freq1_khz  freq2_khz", (void *)tx2_tone);
}