#ifndef HANDLER_H
#define HANDLER_H
#include "json.hpp"
#include "http.h"

#define HANDLER(Type, Fun, Obj) \
    std::bind(&Type::Fun, &Obj, std::placeholders::_1,std::placeholders::_2)

using json = nlohmann::json;
struct Handler
{
protected:
    std::string body(json const& data, int code = 200, std::string const& msg = "")
    {
        json response = {
            { "message", msg },
            { "timestamp", 0 },
            { "result", data },
            { "code", code }
        };
        return response.dump();
    }
    json response_body()
    {
        json response = {
            { "message", "" },
            { "timestamp", 0 },
            { "result", {} },
            { "code", 0 }
        };
        return response;
    }
};
#endif