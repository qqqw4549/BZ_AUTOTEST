#include "debughandler.h"
#include <iostream>
#include <sstream>
#include <iomanip>

void DebugHandler::reg(const http::Request &req, http::Response &res)
{
    json data = {};
    Talise::ShardPtr talise = talise_.lock();
    if(req.method == "POST")
    {
        json param = json::parse(req.body);
        if(talise)
        {
            uint16_t addr = std::stoul(param["addr"].get<std::string>(), nullptr, 16);
            uint32_t value = std::stoul(param["value"].get<std::string>(), nullptr, 16);
            talise->write_reg(addr, value);
        }
    }
    else if(req.method == "GET")
    {
        if(talise && req.has_param("addr"))
        {
            std::string addr_str = req.get_param_value("addr");
            uint16_t addr = std::stoul(addr_str, nullptr, 16);
            uint32_t value;
            std::ostringstream os;
            talise->read_reg(addr, value);
            os << std::hex << std::uppercase << value;
            data["addr"] = addr_str;
            data["value"] = os.str();
        }
    }
    res.set_content(body(data), "application/json");
}

void DebugHandler::mem(const http::Request &req, http::Response &res)
{
    json data = {};
    Talise::ShardPtr talise = talise_.lock();
    if(req.method == "GET" && talise)
    {
        if(req.has_param("addr") && req.has_param("count"))
        {
            std::string addr_str = req.get_param_value("addr");
            uint32_t addr = std::stoul(addr_str, nullptr, 16);
            uint32_t count = std::stoul(req.get_param_value("count"), nullptr, 16);
            std::ostringstream os;
            os << std::hex << std::uppercase;
            for(uint32_t i = 0; i < count; i++)
            {
                uint32_t value = 0;
                talise->read_mem(addr, value);
                addr += sizeof(uint32_t);
                os << std::setw(8) << std::setfill('0') << value
                   << std::setw(0) << std::setfill(' ');
                if((i + 1) % 8 == 0)
                    os << std::endl;
                else 
                {
                    if(i != count - 1)
                        os << " ";
                }
            }
            data["addr"] = addr_str;
            data["values"] = os.str();
        }
    }
    std::cout  << "DebugHandler: " << __func__ << std::endl;
    res.set_content(body(data), "application/json");
}

void DebugHandler::pc(const http::Request &req, http::Response &res)
{
    json data = {};
    std::cout  << "DebugHandler: " << __func__ << std::endl;
    res.set_content(body(data), "application/json");
}

void DebugHandler::fpga(const http::Request &req, http::Response &res)
{
    json data = {};
    Talise::ShardPtr talise = talise_.lock();
    if(req.method == "POST")
    {
        json param = json::parse(req.body);
        if(talise)
        {
            uint16_t offset = std::stoul(param["offset"].get<std::string>(), nullptr, 16);
            uint32_t value = std::stoul(param["value"].get<std::string>(), nullptr, 16);
            talise->write_fpga_reg(offset, value);
        }
    }
    else if(req.method == "GET")
    {
        if(talise && req.has_param("offset"))
        {
            std::string offset_str = req.get_param_value("offset");
            uint16_t offset = std::stoul(offset_str, nullptr, 10);
            uint32_t value;
            std::ostringstream os;
            talise->read_fpga_reg(offset, value);
            os << std::hex << std::uppercase << value;
            data["offset"] = offset_str;
            data["value"] = os.str();
        }
    }
    res.set_content(body(data), "application/json");
}