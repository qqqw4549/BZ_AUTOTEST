#include "functionhandler.h"
#include <iostream>
#include <stdexcept>

#define FUNCTION_DEBUG
#ifdef FUNCTION_DEBUG
#define FUNCTION_DEBUG_INFO \
    std::cout  << "FunctionHandler: " << __func__ << std::endl; \
    std::cout << req.body << std::endl;
#else
#define FUNCTION_DEBUG_INFO 
#endif

void FunctionHandler::tx(const http::Request &req, http::Response &res)
{
    json data = {};
    Talise::ShardPtr talise = talise_.lock();

    if(talise)
    {
        json param = json::parse(req.body);
        std::string method = param["method"].get<std::string>();
        std::string channel = param["channel"].get<std::string>();
        if(channel == "1")
            talise->set_tx_channel(BZ_TX1);
        else if(channel == "2")
            talise->set_tx_channel(BZ_TX2);
        if(method == "cwpower")
        {
            int32_t freq1 = param["freq1"].get<int32_t>() / 1000;
            int32_t freq2 = param["freq2"].get<int32_t>() / 1000;
            uint32_t gain1 = param["gain1"].get<uint32_t>();
            uint32_t gain2 = param["gain2"].get<uint32_t>();

            talise->set_tx_power(freq1, freq2, gain1, gain2);
        }
        else if(method == "gainrange")
        {
            uint32_t gainIndex = param["gainIndex"].get<uint32_t>();
            talise->set_tx_gain(gainIndex);
        }
        else if(method == "evm")
        {
            std::string dataSrc = param["dataSrc"].get<std::string>();
            if (dataSrc == "stop")
                talise->stop_tx_data();
            else if(dataSrc == "file")
            {
                std::string dataFile = param["dataFile"].get<std::string>();
                talise->set_dds_off();
                talise->send_tx_data(dataFile);
            }
        }
    }
    FUNCTION_DEBUG_INFO
    res.set_content(body(data), "application/json");
}

void FunctionHandler::rx(const http::Request &req, http::Response &res)
{
    json data = {};
    Talise::ShardPtr talise = talise_.lock();

    if(talise)
    {
        json param = json::parse(req.body);
        std::string method = param["method"].get<std::string>();
        std::string channel = param["channel"].get<std::string>();
        if(channel == "1")
            talise->set_rx_channel(BZ_RX1);
        else if(channel == "2")
            talise->set_rx_channel(BZ_RX2);
        if(method == "gainrange")
        {
            uint32_t gainIndex = param["gainIndex"].get<uint32_t>();
            talise->set_rx_gain(gainIndex);
        }
    }

    FUNCTION_DEBUG_INFO
    res.set_content(body(data), "application/json");
}

void FunctionHandler::orx(const http::Request &req, http::Response &res)
{
    json data = {};
    Talise::ShardPtr talise = talise_.lock();
    if(talise)
    {
        json param = json::parse(req.body);
        std::string method = param["method"].get<std::string>();
        std::string channel = param["channel"].get<std::string>();
        
        if(method == "gainrange")
        {
            uint32_t gainIndex = param["gainIndex"].get<uint32_t>();
            if(channel == "1")
                talise->set_orx_gain(BZ_ORX1, gainIndex);
            else
                talise->set_orx_gain(BZ_ORX2, gainIndex);
        }
    }
    FUNCTION_DEBUG_INFO
    res.set_content(body(data), "application/json");
}

void FunctionHandler::calibration(const http::Request &req, http::Response &res)
{
    json data = {};
    Talise::ShardPtr talise = talise_.lock();

    if(talise)
    {
        json param = json::parse(req.body);
        std::string method = param["method"].get<std::string>();
        std::string channels = param["channels"].get<std::string>();
        uint32_t calsMask = 0;
        if(method == "rxdcoc")
            calsMask |= DC_OFFSET;
        else if(method == "rxqec")
            calsMask |= RX_QEC_INIT;
        else if(method == "txqec")
            calsMask |= TX_QEC_INIT;
        else if(method == "txlol")
            calsMask |= TX_LO_LEAKAGE_INTERNAL;
        else if(method == "all")
            calsMask = DC_OFFSET | RX_QEC_INIT | TX_QEC_INIT | TX_LO_LEAKAGE_INTERNAL;
        if(method == "txqec" || method == "txlol" || method == "all")
        {
            talise->tx1_cali_enable(true);
            talise->tx2_cali_enable(true);
        }
        state_ = CaliStateRunning;
        text_  = "";
        talise->set_cals_channels(channels);
        talise->run_cals(calsMask);

        uint32_t code = talise->cals_error_code();
        if(!code)
            text_  = "校准成功！";
        else
            text_  = talise->cals_error_text(code);
        state_ = CaliStateIdle;
        talise->tx1_cali_enable(false);
        talise->tx2_cali_enable(false);
    }

    FUNCTION_DEBUG_INFO
    res.set_content(body(data), "application/json");
}

void FunctionHandler::state(const http::Request &req, http::Response &res)
{
    json data = {};
    data["state"] = state_text();
    data["text"] = text_;

    FUNCTION_DEBUG_INFO
    res.set_content(body(data), "application/json");
}

std::string FunctionHandler::state_text()
{
    if(state_ == CaliStateIdle)
        return std::string("idle");
    else if(state_ == CaliStateRunning)
        return std::string("running");
    return std::string();
}
