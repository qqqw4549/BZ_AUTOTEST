#include "httpserver.h"
#include "json.hpp"

#include <cstdio>

using json = nlohmann::json;

HttpServer::HttpServer(bool is_log)
: is_log_(is_log)
{
    if(is_log_)
      set_logger([this](const http::Request &req, const http::Response &res) 
      {
        printf("%s", this->log(req, res).c_str());
      });
}

bool HttpServer::set_root_path(std::string const& url, std::string const& dir)
{
    return set_mount_point(url, dir);
}

HttpServer &HttpServer::get(const std::string &pattern, http::Server::Handler handler)
{
    Get(pattern, handler);
    return *this;
}

HttpServer &HttpServer::post(const std::string &pattern, http::Server::Handler handler)
{
    Post(pattern, handler);
    return *this;
}

HttpServer &HttpServer::put(const std::string &pattern, http::Server::Handler handler)
{
    Put(pattern, handler);
    return *this;
}

bool HttpServer::run(const char *host, int port)
{
    return listen(host, port);
}

std::string HttpServer::log(const http::Request &req, const http::Response &res)
{
    std::string s;
    char buf[BUFSIZ];

    s += "================================\n";

    snprintf(buf, sizeof(buf), "%s %s %s", req.method.c_str(),
            req.version.c_str(), req.path.c_str());
    s += buf;

    std::string query;
    for (auto it = req.params.begin(); it != req.params.end(); ++it) {
        const auto &x = *it;
        snprintf(buf, sizeof(buf), "%c%s=%s",
                (it == req.params.begin()) ? '?' : '&', x.first.c_str(),
                x.second.c_str());
        query += buf;
    }
    snprintf(buf, sizeof(buf), "%s\n", query.c_str());
    s += buf;

    s += dump_headers(req.headers);

    s += "--------------------------------\n";

    snprintf(buf, sizeof(buf), "%d %s\n", res.status, res.version.c_str());
    s += buf;
    s += dump_headers(res.headers);
    s += "\n";

    if (!req.body.empty()) { s += req.body; }

    s += "\n";

  return s;
}

std::string HttpServer::dump_headers(const http::Headers &headers) {
  std::string s;
  char buf[BUFSIZ];

  for (auto it = headers.begin(); it != headers.end(); ++it) {
    const auto &x = *it;
    snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(), x.second.c_str());
    s += buf;
  }

  return s;
}