#ifndef USER_HANDLER_H
#define USER_HANDLER_H
#include "handler.h"

struct UserHandler : public Handler
{
    void info(const http::Request &req, http::Response &res);
    void nav(const http::Request &req, http::Response &res);
};

#endif