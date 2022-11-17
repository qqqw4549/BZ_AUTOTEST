#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include "http.h"

#include <string>

struct HttpServer : private http::Server
{
    HttpServer(bool is_log = false);

    bool set_root_path(std::string const& url, std::string const& dir);

    HttpServer &get(const std::string &pattern, http::Server::Handler handler);
    HttpServer &post(const std::string &pattern, http::Server::Handler handler);
    HttpServer &put(const std::string &pattern, http::Server::Handler handler);


    bool run(const char *host, int port);
private:
    std::string log(const http::Request &req, const http::Response &res);
    std::string dump_headers(const http::Headers &headers);
private:
    bool is_log_;
};

#endif