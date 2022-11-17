#include "authhandler.h"
#include <iostream>

void AuthHandler::login(const http::Request &req, http::Response &res)
{
    json data =  {
            { "id", "4291d7da9005377ec9aec4a71ea837f" },
            { "name", "管理员" },
            { "username", "admin" },
            { "password", "" },
            { "avatar", "https://gw.alipayobjects.com/zos/rmsportal/jZUIxmJycoymBprLOUbT.png" },
            { "status", 1 },
            { "telephone", "" },
            { "lastLoginIp", 1497160610259},
            { "lastLoginTime", "1534837621348" },
            { "creatorId", "admin" },
            { "createTime", 1497160610259 },
            { "deleted", 0 },
            { "roleId", "admin" },
            { "lang", "zh-CN" },
            { "token", "4291d7da9005377ec9aec4a71ea837f" }
    };
    res.set_content(body(data), "application/json");
}

void AuthHandler::logout(const http::Request &req, http::Response &res)
{
    json data = {};
    res.set_content(body(data), "application/json");
}

void AuthHandler::get2step(const http::Request &req, http::Response &res)
{
    json data =  { { "stepCode", 1 } };
    res.set_content(body(data), "application/json");
}