#include "confighandler.h"
#include "bz_config.h"

#include <iostream>

int const KHZ_UNIT = 1000;
int const MHZ_UNIT = 1000 * KHZ_UNIT;
int const GHZ_UNIT = 1000 * MHZ_UNIT;

void ConfigHandler::info(const http::Request &req, http::Response &res)
{
    json data = {};
    Talise::ShardPtr talise = talise_.lock();
    if(req.method == "GET")
    {
        if(talise)
        { 
            data["devName"] = talise->dev_name();
            data["rxChannel"] = talise->rx_channel();
            data["txChannel"] = talise->tx_channel();
            data["oxChannel"] = talise->ox_channel();
            data["rxtxChannelMode"] = talise->channel_ctrl_mode();
            data["rxBandWidth"] = talise->rx_band_width_hz() / MHZ_UNIT;
            data["txBandWidth"] = talise->tx_band_width_hz() / MHZ_UNIT;
            data["oxBandWidth"] = talise->ox_band_width_hz() / MHZ_UNIT;
            data["rxIQRate"] = talise->rx_iq_rate_khz();
            data["txIQRate"] = talise->tx_iq_rate_khz();
            data["oxIQRate"] = talise->ox_iq_rate_khz();
        }
    }
    else if(req.method == "PUT")
    {
        json param = json::parse(req.body);
        if(talise)
        {
            talise->set_dev_name(param["devName"].get<std::string>());
            talise->set_rx_channel(param["rxChannel"].get<std::string>());
            talise->set_tx_channel(param["txChannel"].get<std::string>());
            talise->set_ox_channel(param["oxChannel"].get<std::string>());
            talise->set_channel_ctr_mode(param["rxtxChannelMode"].get<std::string>());
            talise->set_rx_band_width_hz(param["rxBandWidth"].get<uint32_t>() * MHZ_UNIT);
            talise->set_tx_band_width_hz(param["txBandWidth"].get<uint32_t>() * MHZ_UNIT);
            talise->set_ox_band_width_hz(param["oxBandWidth"].get<uint32_t>() * MHZ_UNIT);
            talise->set_rx_iq_rate_khz(param["rxIQRate"].get<uint32_t>());
            talise->set_tx_iq_rate_khz(param["txIQRate"].get<uint32_t>());
            talise->set_ox_iq_rate_khz(param["oxIQRate"].get<uint32_t>());
            state_ = SettingStateRunning;
            text_  = "";
            device_settings(talise);
            state_ = SettingStateIdle;
            text_  = "配置完毕！";
        }
    }

    std::cout  << "ConfigHandler: " << __func__ << std::endl;
    res.set_content(body(data), "application/json");
}

void ConfigHandler::freq(const http::Request &req, http::Response &res)
{
    json data = {};
    Talise::ShardPtr talise = talise_.lock();

    if(req.method == "PUT")
    {
        json param = json::parse(req.body);
        std::string mode = param["mode"].get<std::string>();
        std::string unit = param["unit"].get<std::string>();
        uint64_t freq = param["freq"].get<uint64_t>();
        if(unit == "khz")
            freq *= KHZ_UNIT;
        else if(unit == "mhz")
            freq *= MHZ_UNIT;
        else if(unit == "ghz")
            freq *= GHZ_UNIT;
        if(talise)
        {
            if(mode == "rf")
                talise->set_pll_frequency(BZ_RF_PLL, freq);
            else if(mode == "aux")
                talise->set_pll_frequency(BZ_AUX_PLL, freq);
        }               
        std::cout << req.body << std::endl;
    }
    else if(req.method == "GET")
    {
        if(talise && req.has_param("mode"))
        {
            std::string mode = req.get_param_value("mode");
            uint64_t freq = 0;
            if(mode == "rf")
                talise->pll_frequency(BZ_RF_PLL, freq);
            else if(mode == "aux")
                talise->pll_frequency(BZ_AUX_PLL, freq);

            data["mode"] = mode;
            if(freq % GHZ_UNIT == 0)
            {
                data["freq"] = freq / GHZ_UNIT;
                data["unit"] = "ghz";
            }
            else if(freq % MHZ_UNIT == 0)
            {
                data["freq"] = freq / MHZ_UNIT;
                data["unit"] = "mhz";
            }
            else if(freq % KHZ_UNIT == 0)
            {
                data["freq"] = freq / KHZ_UNIT;
                data["unit"] = "khz";
            }
            else
            {
                data["freq"] = freq;
                data["unit"] = "hz";
            }
        }
    }
    res.set_content(body(data), "application/json");
}

void ConfigHandler::calibration(const http::Request &req, http::Response &res)
{
    json data = {};
    std::cout  << "ConfigHandler: " << __func__ << std::endl;
    std::cout << req.body << std::endl;
    res.set_content(body(data), "application/json");
}

void ConfigHandler::jesd204b(const http::Request &req, http::Response &res)
{
    json data = {};
    std::cout  << "ConfigHandler: " << __func__ << std::endl;
    std::cout << req.body << std::endl;
    res.set_content(body(data), "application/json");
}

void ConfigHandler::gpio(const http::Request &req, http::Response &res)
{
    json data = {};
    std::cout  << "ConfigHandler: " << __func__ << std::endl;
    std::cout << req.body << std::endl;
    res.set_content(body(data), "application/json");
}

void ConfigHandler::agc(const http::Request &req, http::Response &res)
{
    json data = {};
    Talise::ShardPtr talise = talise_.lock();

    std::cout  << "ConfigHandler: " << __func__ << std::endl;
    //std::cout << req.body << std::endl;
    if(talise)
    {
        BzAgcCfg_t cfg;
        json param = json::parse(req.body);

        cfg.agcClkDiv = std::strtol(
            param["clockDiv"].get<std::string>().data(), nullptr, 10);
        cfg.agcClkDiv = 3;
        cfg.agcPeakWaitCounter = param["peakWaitCounter"].get<uint8_t>();
        cfg.agcGainUpdateCounter = param["gainUpdateCounter"].get<uint32_t>();
        cfg.agcThreshPreventGain = std::strtol(
            param["threshPreventGain"].get<std::string>().data(), nullptr, 10);
        cfg.agcPeakThreshGainControlMode = std::strtol(
            param["gainCtlMode"].get<std::string>().data(), nullptr, 10);
        

        if(param["apdEnableMeasurement"].get<bool>())
            cfg.agcPeak.apdEnableMeasurement = 1;
        else
            cfg.agcPeak.apdEnableMeasurement = 0;
        
        cfg.agcPeak.apdAttackMode = std::strtol(
            param["apdAttackMode"].get<std::string>().data(), nullptr, 10);
        cfg.agcPeak.apdMeasurementDuration = param["apdMeasurementDuration"].get<uint8_t>();
        cfg.agcPeak.apdImmdOverNum = param["apdImmdOverNum"].get<uint16_t>();
        cfg.agcPeak.apdPeriOverNum = param["apdPeriOverNum"].get<uint16_t>();
        cfg.agcPeak.apdUnderNum = param["apdUnderNum"].get<uint16_t>();
        cfg.agcPeak.apdGainStepAttack = param["apdGainStepAttack"].get<uint8_t>();
        cfg.agcPeak.apdGainStepRecovery = param["apdGainStepRecovery"].get<uint8_t>();

        if(param["hb2EnableMeasurement"].get<bool>())
            cfg.agcPeak.hb2EnableMeasurement = 1;
        else
            cfg.agcPeak.hb2EnableMeasurement = 0;

        cfg.agcPeak.hb2MeasurementDuration = param["hb2MeasurementDuration"].get<uint8_t>();
        cfg.agcPeak.hb2RecoveryMode = param["hb2RecoveryMode"].get<uint8_t>();
        cfg.agcPeak.hb2AttackMode = param["hb2AttackMode"].get<uint8_t>();
        cfg.agcPeak.hb2GainStepHighRecovery = param["hb2GainStepHighRecovery"].get<uint8_t>();
        cfg.agcPeak.hb2GainStepLowRecovery = param["hb2GainStepLowRecovery"].get<uint8_t>();
        cfg.agcPeak.hb2GainStepMidRecovery = param["hb2GainStepMidRecovery"].get<uint8_t>();
        cfg.agcPeak.hb2GainStepAttack = param["hb2GainStepAttack"].get<uint8_t>();

         if(param["powerEnableMeasurement"].get<bool>())
            cfg.agcPower.powerEnableMeasurement = 1;
        else
            cfg.agcPower.powerEnableMeasurement = 0;

        cfg.agcPower.powerInMux = std::strtol(
            param["powerInMux"].get<std::string>().data(), nullptr, 10);

        cfg.agcPower.powerMeasurementDuration = param["powerMeasurementDuration"].get<uint8_t>();
        cfg.agcPower.powerAttackHigStep = param["powerAttackHigStep"].get<uint16_t>();
        cfg.agcPower.powerAttackLowStep = param["powerAttackLowStep"].get<uint16_t>();
        cfg.agcPower.powerRecoveryHigStep = param["powerRecoveryHigStep"].get<uint16_t>();
        cfg.agcPower.powerRecoveryLowStep = param["powerRecoveryLowStep"].get<uint16_t>();
        cfg.agcPower.powerOverHigThresh = param["powerOverHigThresh"].get<uint16_t>();
        cfg.agcPower.powerOverLowThresh = param["powerOverLowThresh"].get<uint16_t>();
        cfg.agcPower.powerUnderHigThresh = param["powerUnderHigThresh"].get<uint16_t>();
        cfg.agcPower.powerUnderLowThresh = param["powerUnderLowThresh"].get<uint16_t>();

        std::cerr << "agcClkDiv: " << (int)cfg.agcClkDiv << std::endl
                  << "agcPeakWaitCounter: " << (int)cfg.agcPeakWaitCounter << std::endl
                  << "agcGainUpdateCounter: " << (int)cfg.agcGainUpdateCounter << std::endl
                  << "agcThreshPreventGain: " << (int)cfg.agcThreshPreventGain << std::endl
                  << "agcPeakThreshGainControlMode: " << (int)cfg.agcPeakThreshGainControlMode << std::endl
                  << "apdEnableMeasurement: " << (int)cfg.agcPeak.apdEnableMeasurement << std::endl
                  << "apdAttackMode: " << (int)cfg.agcPeak.apdAttackMode << std::endl
                  << "apdMeasurementDuration: " << (int)cfg.agcPeak.apdMeasurementDuration << std::endl
                  << "apdImmdOverNum: " << cfg.agcPeak.apdImmdOverNum << std::endl
                  << "apdPeriOverNum: " << cfg.agcPeak.apdPeriOverNum << std::endl
                  << "apdUnderNum: " << cfg.agcPeak.apdUnderNum << std::endl
                  << "apdGainStepAttack: " << (int)cfg.agcPeak.apdGainStepAttack << std::endl
                  << "apdGainStepRecovery: " << (int)cfg.agcPeak.apdGainStepRecovery << std::endl
                  << "hb2EnableMeasurement: " << (int)cfg.agcPeak.hb2EnableMeasurement << std::endl
                  << "hb2MeasurementDuration: " << (int)cfg.agcPeak.hb2MeasurementDuration << std::endl
                  << "hb2RecoveryMode: " << (int)cfg.agcPeak.hb2RecoveryMode << std::endl
                  << "hb2AttackMode: " << (int)cfg.agcPeak.hb2AttackMode << std::endl
                  << "hb2GainStepHighRecovery: " << (int)cfg.agcPeak.hb2GainStepHighRecovery << std::endl
                  << "hb2GainStepLowRecovery: " << (int)cfg.agcPeak.hb2GainStepLowRecovery << std::endl
                  << "hb2GainStepMidRecovery: " << (int)cfg.agcPeak.hb2GainStepMidRecovery << std::endl
                  << "hb2GainStepAttack: " << (int)cfg.agcPeak.hb2GainStepAttack << std::endl
                  << "powerEnableMeasurement: " << (int)cfg.agcPower.powerEnableMeasurement << std::endl
                  << "powerInMux: " << (int)cfg.agcPower.powerInMux << std::endl
                  << "powerMeasurementDuration: " << (int)cfg.agcPower.powerMeasurementDuration << std::endl
                  << "powerAttackHigStep: " << (int)cfg.agcPower.powerAttackHigStep << std::endl
                  << "powerAttackLowStep: " << cfg.agcPower.powerAttackLowStep << std::endl
                  << "powerRecoveryHigStep: " << cfg.agcPower.powerRecoveryHigStep << std::endl
                  << "powerRecoveryLowStep: " << cfg.agcPower.powerRecoveryLowStep << std::endl
                  << "powerOverHigThresh: " << cfg.agcPower.powerOverHigThresh << std::endl
                  << "powerOverLowThresh: " << cfg.agcPower.powerOverLowThresh << std::endl
                  << "powerUnderHigThresh: " << cfg.agcPower.powerUnderHigThresh << std::endl
                  << "powerUnderLowThresh: " << cfg.agcPower.powerUnderLowThresh << std::endl;

        talise->setup_rx_agc(&cfg);
    }
    res.set_content(body(data), "application/json");
}

void ConfigHandler::state(const http::Request &req, http::Response &res)
{
    json data = {};
    data["state"] = state_text();
    data["text"] = text_;

    res.set_content(body(data), "application/json");
}

void ConfigHandler::pa(const http::Request &req, http::Response &res)
{
    json data = {};
    Talise::ShardPtr talise = talise_.lock();

    if(req.method == "PUT")
    {
        json param = json::parse(req.body);
        bool enable = param["enable"].get<bool>();
        uint32_t count = param["count"].get<uint32_t>();
        uint32_t threshold = param["threshold"].get<uint32_t>();
        if(talise)
            talise->set_tx_pa_protection(count, threshold, enable);

        std::cout << req.body << std::endl;
    }
    else if(req.method == "GET")
    {
        if(talise)
        {
            uint32_t count = 0;
            uint32_t threshold = 0;
            bool enable = false;
            
            talise->get_tx_pa_protection(count, threshold, enable);

            data["count"] = count;
            data["threshold"] = threshold;
            data["enable"] = enable;
        }
    }
    res.set_content(body(data), "application/json");
}

void ConfigHandler::gainMode(const http::Request &req, http::Response &res)
{
    json data = {};
    Talise::ShardPtr talise = talise_.lock();

    if(req.method == "PUT")
    {
        json param = json::parse(req.body);
        std::string channel = param["channel"].get<std::string>();
        std::string mode = param["mode"].get<std::string>();
        if(talise)
        {
            if(channel == "rx1on")
            {
                talise->set_rx_channel(BZ_RX1);
            }  
            else if(channel == "rx2on")
            {
                talise->set_rx_channel(BZ_RX2);
            }
            else if(channel == "rx1rx2on")
            {
                talise->set_rx_channel(BZ_RX1RX2);
            }

            if(mode == "api")
                talise->set_rx_gain_control_mode(true);
            else if(mode == "gpio")
            {
                talise->set_rx_gain_control_mode(false);
                talise->set_rx_gain_ctrl_pin();
            }       

        }
        std::cout << req.body << std::endl;
    }
    else if(req.method == "GET")
    {
        if(talise)
        {
            data["channel"] = talise->get_rx_channel();
            data["mode"] = talise->gain_mode();
            data["gainIndex"] = talise->rx_gain_index();
        }
    }
    res.set_content(body(data), "application/json");
}

void ConfigHandler::gpioGain(const http::Request &req, http::Response &res)
{
    json data = {};
    Talise::ShardPtr talise = talise_.lock();
    if(talise)
    {
        json param = json::parse(req.body);
        std::string method = param["method"].get<std::string>();
        std::string channel = param["channel"].get<std::string>();
        if(method == "add")
        {
            if(channel == "rx1on")
                talise->rx1_gain(true);
            else if(channel == "rx2on")
                talise->rx2_gain(true);
            else if(channel == "rx1rx2on")
            {
                talise->rx1_gain(true);
                talise->rx2_gain(true);
            }
        }
        else
        {
            if(channel == "rx1on")
                talise->rx1_gain(false);
            else if(channel == "rx2on")
                talise->rx2_gain(false);
            else if(channel == "rx1rx2on")
            {
                talise->rx1_gain(false);
                talise->rx2_gain(false);
            }
        }
        data["gainIndex"] = talise->rx_gain_index();
    }
    res.set_content(body(data), "application/json");
}

std::string ConfigHandler::state_text()
{
    if(state_ == SettingStateIdle)
        return std::string("idle");
    else if(state_ == SettingStateRunning)
        return std::string("running");
    return std::string();
}

void ConfigHandler::device_settings(Talise::ShardPtr & talise)
{
    talise->radio_off();
    talise->set_tx_pa_protection(1, 10000, false);
    talise->term_device();
    talise->chip_select();
    talise->start_204b_sysc();
    talise->init_device();

    std::cout << "Wait 204B sync ..." << std::endl;

    bool is_sync = false;
    
    for(int i = 0; i < 10; i++)
    {
        talise->delay(1000);
        is_sync = talise->jesd204b_state();
        if(is_sync)
            break;
    }
    talise->end_204b_sysc();

    if(is_sync)
    {
        std::cout << "204B sync is OK" << std::endl;
        talise->set_204b_sysref(false);
    }
        

    talise->interruput_on();
    talise->lanpa_power(true);
    talise->radio_on();

    talise->set_tx_pa_protection(1, 10000);
}