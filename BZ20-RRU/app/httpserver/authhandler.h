#ifndef AUTH_HANDLER_H
#define AUTH_HANDLER_H
#include "handler.h"

struct AuthHandler : public Handler
{
    void login(const http::Request &req, http::Response &res);
    void logout(const http::Request &req, http::Response &res);
    void get2step(const http::Request &req, http::Response &res);
};

#endif