#include "datahandler.h"
#include <iostream>
#include <stdexcept>

#define DATA_DEBUG
#ifdef DATA_DEBUG
#define DATA_DEBUG_INFO \
    std::cout  << "FunctionHandler: " << __func__ << std::endl; \
    std::cout << req.body << std::endl;
#else
#define DATA_DEBUG_INFO 
#endif

void DataHandler::orx(const http::Request &req, http::Response &res)
{
    json data = {};
    Talise::ShardPtr talise = talise_.lock();

    if(talise && req.has_param("points"))
    {
        uint32_t points = std::stoul(req.get_param_value("points"));
        uint32_t channels = std::stoul(req.get_param_value("channels"));
        //talise->write_reg(0x1C7, 0xFF3F3FF7);
        talise->rx1_enable(false);
        talise->rx2_enable(false);
        if(channels == 1)
            talise->orx1_enable(true);
        else
            talise->orx2_enable(true);
        std::string orxData = talise->recv_orx_data(points);
        data["orxData"] = orxData;
        talise->orx1_enable(false);
        talise->orx2_enable(false);
        //talise->write_reg(0x1C7, 0xFFFF3FFF);
        talise->rx1_enable(true);
        talise->rx2_enable(true);
    }

    DATA_DEBUG_INFO
    res.set_content(body(data), "application/json");
}

void DataHandler::rx(const http::Request &req, http::Response &res)
{
    json data = {};
    Talise::ShardPtr talise = talise_.lock();

    if(talise 
        && req.has_param("points") 
        && req.has_param("channels") 
        && req.has_param("type"))
    {
        uint32_t points = std::stoul(req.get_param_value("points"));
        uint32_t channels = std::stoul(req.get_param_value("channels"));
        std::string type = req.get_param_value("type");
        if(type == "text")
        {
            std::string rxData = talise->recv_rx_data(points, channels);
            data["rxData"] = rxData;
        }
        else if(type == "data")
        {
            DataArray x;
            DataArray rx1_i;
            DataArray rx1_q;
            DataArray rx1_f;
            DataArray rx2_i;
            DataArray rx2_q;
            DataArray rx2_f;
            talise->recv_rx_data(points, channels, x, 
                rx1_i, rx1_q, rx1_f,
                rx2_i, rx2_q, rx2_f);
            data["x"] = x;
            data["rx1_i"] = rx1_i;
            data["rx1_q"] = rx1_q;
            data["rx1_f"] = rx1_f;
            data["rx1_f_max"] = rx1_f.max();
            data["rx2_i"] = rx2_i;
            data["rx2_q"] = rx2_q;
            data["rx2_f"] = rx2_f;
            data["rx2_f_max"] = rx2_f.max();
        }
    }

    DATA_DEBUG_INFO
    res.set_content(body(data), "application/json");
}

void DataHandler::tx(const http::Request &req, http::Response &res)
{
    json data = {};
    Talise::ShardPtr talise = talise_.lock();

    if(talise)
    {
        json param = json::parse(req.body);
        std::string dataSrc = param["dataSrc"].get<std::string>();
        if(dataSrc == "stop")
        {
            talise->stop_tx_data();
        }
        else if(dataSrc == "file")
        {
            std::string dataFile = param["dataFile"].get<std::string>();
            uint32_t channels = std::stoul(param["channels"].get<std::string>());
            talise->set_dds_off();
            talise->send_tx_data(dataFile);
        }
    }

    DATA_DEBUG_INFO
    res.set_content(body(data), "application/json");
}
