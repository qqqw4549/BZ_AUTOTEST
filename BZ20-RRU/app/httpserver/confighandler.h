#ifndef CONGIG_HANDLER_H
#define CONGIG_HANDLER_H
#include "handler.h"
#include "talise.h"

typedef uint32_t SettingState;
SettingState const SettingStateIdle    = 0;
SettingState const SettingStateRunning = 1;
struct ConfigHandler : public Handler
{
    ConfigHandler(Talise::ShardPtr const& talise)
    : talise_(talise)
    {}
    
    void info(const http::Request &req, http::Response &res);
    void freq(const http::Request &req, http::Response &res);
    void calibration(const http::Request &req, http::Response &res);
    void jesd204b(const http::Request &req, http::Response &res);
    void gpio(const http::Request &req, http::Response &res);
    void agc(const http::Request &req, http::Response &res);
    void pa(const http::Request &req, http::Response &res);
    void gainMode(const http::Request &req, http::Response &res);
    void gpioGain(const http::Request &req, http::Response &res);
    void state(const http::Request &req, http::Response &res);
private:
    void device_settings(Talise::ShardPtr & talise);
    std::string state_text();
private:
    Talise::WeakPtr talise_;
    SettingState state_ = SettingStateIdle;
    std::string text_;
};

#endif