#ifndef FUNCTION_HANDLER_H
#define FUNCTION_HANDLER_H
#include "handler.h"
#include "talise.h"

typedef uint32_t CaliState;
CaliState const CaliStateIdle    = 0;
CaliState const CaliStateRunning = 1;

struct FunctionHandler : public Handler
{
    FunctionHandler(Talise::ShardPtr const& talise)
    : talise_(talise)
    {}

    void tx(const http::Request &req, http::Response &res);
    void rx(const http::Request &req, http::Response &res);
    void orx(const http::Request &req, http::Response &res);
    void calibration(const http::Request &req, http::Response &res);
    void state(const http::Request &req, http::Response &res);
private:
    std::string state_text();
private:
    Talise::WeakPtr talise_;
    CaliState state_ = CaliStateIdle;
    std::string text_;
};

#endif