#include "bz_commands.h"

#include <talise.h>
#include <command.h>

#include <iostream>

int main(int argc, char* argv[]) 
{
    int device_id = 0;
    Talise::ShardPtr talise(new Talise());
    if(argc > 1)
        device_id = std::stol(argv[1]);

    talise->set_device_id(device_id);
    talise->chip_select(device_id);
    talise->init_clock();
    talise->start_204b_sysc();
    talise->init_device();

    std::cout << "Wait 204B sync ..." << std::endl;

    bool is_sync = false;
    
    for(int i = 0; i < 10; i++)
    {
        talise->delay(1000);
        is_sync = talise->jesd204b_state();
        if(is_sync)
        {
            std::cout<<"jesd204b ok!"<<std::endl;
            break;
        }
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
    talise->set_gio_dir();

    SetTakise(talise);
    RegisterBzCommands();
    WaitForCommand();
    return 0;
}
