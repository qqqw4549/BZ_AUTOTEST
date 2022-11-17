#ifndef TALISE_H
#define TALISE_H

#include <bz_types.h>
#include <bz_cals_types.h>
#include <bz_agc_types.h>
#include <bz_radio_types.h>

#include <string>
#include <cstdint>
#include <memory>
#include <valarray>

typedef std::valarray<double>  DataArray;
struct Talise
{
    Talise();
    ~Talise();
    
    Talise(Talise const&) = delete;
    Talise& operator = (Talise const&) = delete;

     using ShardPtr = std::shared_ptr<Talise>;
     using WeakPtr = std::weak_ptr<Talise>;
    
    std::string dev_name();
    std::string rx_channel();
    std::string tx_channel();
    std::string ox_channel();
    std::string channel_ctrl_mode();
    uint32_t rx_band_width_hz();
    uint32_t tx_band_width_hz();
    uint32_t ox_band_width_hz();
    uint32_t rx_iq_rate_khz();
    uint32_t tx_iq_rate_khz();
    uint32_t ox_iq_rate_khz();
    
    void set_dev_name(std::string const& name);
    void set_rx_channel(std::string const& channel);
    void set_tx_channel(std::string const& channel);
    void set_ox_channel(std::string const& channel);
    void set_channel_ctr_mode(std::string const& mode);
    void set_rx_band_width_hz(uint32_t band_width);
    void set_tx_band_width_hz(uint32_t band_width);
    void set_ox_band_width_hz(uint32_t band_width);
    void set_rx_iq_rate_khz(uint32_t iq_rate);
    void set_tx_iq_rate_khz(uint32_t iq_rate);
    void set_ox_iq_rate_khz(uint32_t iq_rate);
    
    void delay(uint32_t ms);
    
    bool set_device_id(int device_id);
    void chip_select(int device_id);
    void chip_select() { chip_select(dev_id_); }
    bool init_clock();
    bool init_device();
    bool cals(uint32_t bitmap);
    bool term_device();

    void start_204b_sysc();
    void end_204b_sysc();
    bool jesd204b_state();

    void read_all();
    
    bool write_reg(uint16_t addr, uint32_t data);
    bool write_reg(uint16_t addr, uint32_t data,
        uint8_t end_bit, uint8_t start_bit);
    bool read_reg(uint16_t addr, uint32_t &data);
    bool read_mem(uint32_t addr, uint32_t &data);

    bool set_pll_frequency(BzRfPllName_t name, uint64_t freq);
    bool pll_frequency(BzRfPllName_t name, uint64_t &freq);

    void set_tx_channel(BzTxChannels_t channel) { tx_channel_ = channel; }
    bool set_tx_power(int32_t freq1_khz, int32_t freq2_khz,
        uint32_t gain1, uint32_t gain2);
    bool set_tx_gain(uint32_t gain_index);
    bool tx_gain(uint32_t &gain_index);
    bool set_tx_pa_protection(uint32_t tx_cali_cnt,
        uint32_t tx_power_threshold, bool enable =  true);
    void get_tx_pa_protection(uint32_t &tx_cali_cnt,
        uint32_t &tx_power_threshold, bool &enable);
    bool send_tx_data(std::string const& filename);
    void stop_tx_data();
    
    bool check_tx_data(uint32_t value);

    void set_rx_channel(BzRxChannels_t channel) { rx_channel_ = channel; }
    bool set_rx_gain(uint32_t gain_index);
    std::string recv_rx_data(uint32_t points, uint32_t channels);
    std::string get_rx_channel();

    void recv_rx_data(uint32_t points, uint32_t channels, DataArray & x,
        DataArray & rx1_i, DataArray & rx1_q, DataArray & rx1_f,
        DataArray & rx2_i, DataArray & rx2_q, DataArray & rx2_f);

    bool set_rx_gain_control_mode(bool is_apimode = false);
    bool set_rx_gain_ctrl_pin(int decStep = 1, int incStep = 1);
    bool set_gio_dir(bool is_out = true);
    bool rx1_gain(bool add = true);
    bool rx2_gain(bool add = true);

    bool set_orx_gain(BzObsRxChannels_t channel, uint32_t gain_index);
    std::string recv_orx_data(uint32_t points);
    char* get_rx_data() { return rx_data; }

    void set_cals_channels(std::string const& channels);
    bool run_cals(uint32_t masks);
    uint32_t cals_error_code();
    std::string cals_error_text(uint32_t error_code);

    bool setup_rx_agc(BzAgcCfg_t *cfg);

    bool radio_on();
    bool radio_off();

    void interruput_on();
    void tx1_cali_enable(bool on);
    void tx2_cali_enable(bool on);
    void trigger_out(bool on);
    void lanpa_power(bool on);
    void set_debug() { is_debug_ = true; }
    int32_t rx_gain_index();
    std::string gain_mode() { return gain_mode_; }

    bool set_204b_sysref(bool on);
    bool set_dds_off();

    bool write_fpga_reg(int offset, uint32_t value);
    bool read_fpga_reg(int offset, uint32_t& value);

    bool rx_slicer_enable();

    bool tx1_enable(bool on);
    bool tx2_enable(bool on);
    bool rx1_enable(bool on);
    bool rx2_enable(bool on);
    bool orx1_enable(bool on);
    bool orx2_enable(bool on);
private:
    std::string get_filepath(std::string const& filename);
    std::string get_rxfilename(uint32_t points, bool is_full = true);
    std::string get_orxfilename(uint32_t points, bool is_full = true);
    bool save_data_file(std::string const& filename, uint32_t points);
    std::string save_hex_data(uint32_t points);
    std::string save_dec_data(uint32_t points);
    BzInit_t* dev_config();
    void start_recv_rx_data(uint32_t points, uint32_t channels);
    void end_recv_rx_data();

    bool open_hardware();
    bool reset_device();
    bool get_boot_status();
    bool load_firmware();
    bool wait_firmware_ready();
    bool load_config();
    bool write_profile();
    bool wait_profile_done();
    bool set_rf_freq();
    bool set_aux_freq();
    bool wait_plls_lock_done();
    bool run_cals();
    bool wait_cals_done();
    bool jesd204b_config();
    bool wait_jes204b_config_done();
    bool enable_tracking_cals();
    bool enable_sysref_toframe();
private:
    BzDevice_t device_{};
    BzTxPaProtectCfg_t cfg{};
    BzTxChannels_t tx_channel_;
    BzRxChannels_t rx_channel_;
    BzRadioCtlMode mode_ = BZ_PIN_MODE;
    uint32_t dev_id_;
    BzInit_t dev1_config_;
    BzInit_t dev2_config_;
    uint64_t rf_freq;
    uint64_t aux_freq;
    std::string gain_mode_;
    int32_t rx1_gain_index_;
    int32_t rx2_gain_index_;
    int plaxi_file_;
    int mem_file_;
    char *tx_data;
    char *rx_data;
    bool is_debug_ = true;
};

#endif
