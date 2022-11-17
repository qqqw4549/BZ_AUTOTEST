#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H
#include "handler.h"
#include "talise.h"

struct DataHandler : public Handler
{
    DataHandler(Talise::ShardPtr const& talise)
    : talise_(talise)
    {}

    void orx(const http::Request &req, http::Response &res);
    void rx(const http::Request &req, http::Response &res);
    void tx(const http::Request &req, http::Response &res);
private:
    Talise::WeakPtr talise_;
};

#endif