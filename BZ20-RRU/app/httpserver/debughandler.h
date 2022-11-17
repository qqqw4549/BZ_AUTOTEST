#ifndef DEBUG_HANDLER_H
#define DEBUG_HANDLER_H
#include "handler.h"
#include "talise.h"

struct DebugHandler : public Handler
{
    DebugHandler(Talise::ShardPtr const& talise)
    : talise_(talise)
    {}

    void reg(const http::Request &req, http::Response &res);
    void mem(const http::Request &req, http::Response &res);
    void pc(const http::Request &req, http::Response &res);
    void fpga(const http::Request &req, http::Response &res);
private:
    Talise::WeakPtr talise_;
};

#endif