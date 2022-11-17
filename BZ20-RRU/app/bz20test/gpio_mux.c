#include "bz_talise.h"
#include "command.h"
#include "auxADCDAC.h"
#include "bz_types.h"
#include "bz_gpio_types.h"
#include "bz_gpio.h"

enum{
    MODE1 = 1,
    MODE2,
    MODE3,
    MODE4,
    MODE5,
    MODE6,
    MODE7,
}GPIO_MODE;

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

void gpiomux_test(int8_t enMode)
{
    //printf("test gpio mux mode = %d\n",enMode);
}

void gpio_mux_test()
{
    RegisterCommand("init", "init bz20 device", (void *)init_device);
    RegisterCommand("term", "term bz20 device", (void *)close_device);
    RegisterCommand("setfreq", "setfreq rf_freq_hz  aux_freq_hz", (void *)set_freq);
    RegisterCommand("gpiomux", "e.g. gpiomux 1 (test GPIO mode 1)", (void *)gpiomux_test);
}

