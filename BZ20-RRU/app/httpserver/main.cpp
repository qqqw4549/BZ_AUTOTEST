
#include "httpserver.h"
#include "authhandler.h"
#include "userhandler.h"
#include "confighandler.h"
#include "datahandler.h"
#include "functionhandler.h"
#include "debughandler.h"
#include "talise.h"

#include <iostream>

int main(int argc, char* argv[]) 
{
    HttpServer server(false);
    Talise::ShardPtr talise(new Talise());
    AuthHandler auth;
    UserHandler  user;
    ConfigHandler config(talise);
    DataHandler data(talise);
    FunctionHandler function(talise);
    DebugHandler debug(talise);
    
    server.post("/api/auth/login", HANDLER(AuthHandler, login, auth))
          .post("/api/auth/logout", HANDLER(AuthHandler, logout, auth))
          .post("/api/auth/2step-code", HANDLER(AuthHandler, get2step, auth))
          .get("/api/user/info", HANDLER(UserHandler, info, user))
          .get("/api/user/nav", HANDLER(UserHandler, nav, user))
          .put("/api/config/info", HANDLER(ConfigHandler, info, config))
          .put("/api/config/freq", HANDLER(ConfigHandler, freq, config))
          .put("/api/config/calibration", HANDLER(ConfigHandler, calibration, config))
          .put("/api/config/jesd204b", HANDLER(ConfigHandler, jesd204b, config))
          .put("/api/config/gpio", HANDLER(ConfigHandler, gpio, config))
          .put("/api/config/agc", HANDLER(ConfigHandler, agc, config))
          .put("/api/config/pa", HANDLER(ConfigHandler, pa, config))
          .put("/api/config/gainmode", HANDLER(ConfigHandler, gainMode, config))
          .put("/api/config/gpiogain", HANDLER(ConfigHandler, gpioGain, config))
          .get("/api/config/info", HANDLER(ConfigHandler, info, config))
          .get("/api/config/freq", HANDLER(ConfigHandler, freq, config))
          .get("/api/config/state", HANDLER(ConfigHandler, state, config))
          .get("/api/config/calibration", HANDLER(ConfigHandler, calibration, config))
          .get("/api/config/jesd204b", HANDLER(ConfigHandler, jesd204b, config))
          .get("/api/config/gpio", HANDLER(ConfigHandler, gpio, config))
          .get("/api/config/agc", HANDLER(ConfigHandler, agc, config))
          .get("/api/config/pa", HANDLER(ConfigHandler, pa, config))
          .get("/api/config/gainmode", HANDLER(ConfigHandler, gainMode, config))
          .post("/api/data/tx", HANDLER(DataHandler, tx, data))
          .get("/api/data/rx", HANDLER(DataHandler, rx, data))
          .get("/api/data/orx", HANDLER(DataHandler, orx, data))
          .get("/api/function/state", HANDLER(FunctionHandler, state, function))
          .post("/api/function/tx", HANDLER(FunctionHandler, tx, function))
          .post("/api/function/rx", HANDLER(FunctionHandler, rx, function))
          .post("/api/function/orx", HANDLER(FunctionHandler, orx, function))
          .post("/api/function/calibration", HANDLER(FunctionHandler, calibration, function))
          .post("/api/debug/reg", HANDLER(DebugHandler, reg, debug))
          .get("/api/debug/reg", HANDLER(DebugHandler, reg, debug))
          .post("/api/debug/fpga", HANDLER(DebugHandler, fpga, debug))
          .get("/api/debug/fpga", HANDLER(DebugHandler, fpga, debug))
          .get("/api/debug/pc", HANDLER(DebugHandler, pc, debug))
          .get("/api/debug/mem", HANDLER(DebugHandler, mem, debug))
          ;

    int device_id = 0;
    if(argc > 1)
        device_id = std::stol(argv[1]);

    talise->set_device_id(device_id);
    talise->chip_select(device_id);
    talise->init_clock();
    talise->start_204b_sysc();
    talise->init_device();
    talise->rx_slicer_enable();

    std::cout << "Wait 204B sync ..." << std::endl;

    bool is_sync = false;
    
    for(int i = 0; i < 10; i++)
    {
        talise->delay(1000);
        is_sync = talise->jesd204b_state();
        if(is_sync)
            break;
    }
    talise->end_204b_sysc();

    if(is_sync)
    {
        std::cout << "204B sync is OK" << std::endl;
        talise->set_204b_sysref(false);
    }

    talise->interruput_on();
    talise->lanpa_power(true);
    talise->radio_on();
    //talise->set_tx_pa_protection(1, 10000);

    talise->set_gio_dir();


    if(talise->gain_mode() == "api")
        talise->set_rx_gain_control_mode(true);
    else
    {
        talise->set_rx_gain_control_mode(false);
        talise->set_rx_gain_ctrl_pin();
    }

    if(argc > 2)
        server.set_root_path("/",argv[2]);
    else
        server.set_root_path("/", "/media/sd-mmcblk0p1/www");
    server.run("0.0.0.0", 80);

    return 0;
}
