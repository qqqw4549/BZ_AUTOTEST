#include "rru_runtest.h"
#include "appclock.h"
#include "bz_talise.h"
#include "command.h"
#include <unistd.h>

#define JESD204B_WAIT_MAX_COUNT 10
#define JESD204B_WAIT_TIME 1000000 //1s
#define TESTCASE 3 

static void set_jest204b()
{
    int index = 0;
    BzTxConfig_t stArrayTXCfg[TESTCASE] = {
        { .f = 4, .k = 32, .l = 2, .n = 16, .m = 1, .ntotal = 16, .s = 1, .hd = 0x1},
        { .f = 4, .k = 32, .l = 2, .n = 16, .m = 1, .ntotal = 16, .s = 1, .hd = 0x1},
        { .f = 4, .k = 32, .l = 2, .n = 16, .m = 1, .ntotal = 16, .s = 1, .hd = 0x1}
    }; 
    BzRxOrXConfig_t stArrayRXCfg[TESTCASE] = {
        { .f = 4, .k = 32, .l = 2, .n = 16, .m = 1, .ntotal = 16, .s = 1, .hd = 0x1, .txIlasDelay = 4},
        { .f = 4, .k = 32, .l = 2, .n = 16, .m = 1, .ntotal = 16, .s = 1, .hd = 0x1, .txIlasDelay = 6},
        { .f = 4, .k = 32, .l = 2, .n = 16, .m = 1, .ntotal = 16, .s = 1, .hd = 0x1, .txIlasDelay = 8}
    }; 
    
    for(index = 0; index < TESTCASE; index++) // batch test for JESD204B sync stability
    {
        start_204b_sysc(); 
        jesd204b_cfgchange(stArrayTXCfg[index],stArrayRXCfg[index]); // change 204b parameters
        jesd204b_config(); // configuration
        
        wait_jesd204b_config();
        {
            int i;
            for(i = 0; i < JESD204B_WAIT_MAX_COUNT; i++)
            {
                if(jesd204b_sysc_is_ok())
                {
                    printf("204B sync is OK\n\n");
                    break;
                }
                usleep(JESD204B_WAIT_TIME);
            }
            if(i==JESD204B_WAIT_MAX_COUNT)printf("204B sync fail\n");
        }
        end_204b_sysc();
    }
}


static void cals(uint32_t mask)
{
    run_cals(mask);
    wait_cals_done();
}

static void set_freq(uint64_t rf_freq_hz, uint64_t aux_freq_hz)
{
    set_rf_pll_freq(rf_freq_hz);
    wait_pll_lock_done();
    set_aux_pll_freq(aux_freq_hz);
    wait_pll_lock_done();
}

static void init_device(uint32_t device_id)
{
    open_device(device_id);
    rest_device();
    wait_boot_status();
    load_firmware();
    wait_firmware_ready();
    load_config();
    write_profile();
    wait_profile_done();
}

void debugdata(uint32_t position,uint32_t count,uint32_t rate)
{
    debug_data(0x1008000, position, count, rate);
}

void rru_run_test(void)
{
    RegisterCommand("init", "初始化b20芯片：init 0", (void *)init_device);
    RegisterCommand("autotest", "自动化测试", (void *)auto_test);
    RegisterCommand("term", "关闭所有设备文件，做退出准备。", (void *)close_device);
    RegisterCommand("run","启动bz20初始化流程 run 1:pin模式启动，run 2:spi模式启动",(void *)RRUrun);
    RegisterCommand("info","查看RRU当前工作状态",(void *)InfoShow);

    RegisterCommand("cals", "cals mask eg. cals 0x40\n"
                    "             0x40   - DC offset Calibration\n"
                    "             0x80   - Rx Quadrature error correction Calibration\n"
                    "             0x2000 - Exterbal Tx LO leakage Calibration\n"
                    "             0x4000 - Tx Quadrature error correction Calibration", 
        (void *)cals);
    //RegisterCommand("set204b", "config 204b", (void *)set_jest204b);
    RegisterCommand("radiostate","获取PLL状态，以及pin模式下通道状态",(void *)get_radio_state);
    RegisterCommand("pllfreq","获取PLL频率",(void *)get_pllfreq);
    RegisterCommand("path","获取通道状态",(void *)get_path);
    RegisterCommand("setpath" ,"设置通道开关: setpath rx orx tx",(void *)set_path);
    RegisterCommand("getcals" ,"获取校准状态",(void *)getcals_status);
    RegisterCommand("get204" ,"获取204b同步状态",(void *)get_204);
    RegisterCommand("set204tx" ,"设置204btx通道开关:set204tx 1:on 0:off",(void *)set_204b_tx);
    RegisterCommand("set204rx" ,"设置204brx通道开关:set204rx 1:on 0:off",(void *)set_204b_rx);
    RegisterCommand("set204orx" ,"设置204borx通道开关:set204orx 1:on 0:off",(void *)set_204b_orx);
    RegisterCommand("sysref" ,"设置sysref使能:sysref 1:on 0:off",(void *)sysref_control);
    RegisterCommand("slicer" ,"设置slicer使能:slicer 1:on 0:off",(void *)en_slicer);
    RegisterCommand("read","读寄存器值：read addr",(void *)read_reg);
    RegisterCommand("readbit","读寄存器特定位宽内的值：read addr endbit startbit",(void *)read_regbit);
    RegisterCommand("write","写寄存器值：write addr value",(void *)write_reg);
    RegisterCommand("writebit","写寄存器特定位宽内的值：write addr endbit startbit value",(void *)write_regbit);
    RegisterCommand("rxdata","rx采数 点数 通道：rxdata points channels",(void *)recv_rx_data);
    RegisterCommand("orxdata","orx采数 点数 通道：orxdata points channels",(void *)recv_orx_data);
    RegisterCommand("setrxgainctrl","设置rx gain的模式：setrxgainctrl mode(0:MGC 1:AGC) ctrlway(0:spi 1:pin)",(void *)setrxgainCtrl);
    RegisterCommand("getrxgainctrl","获取rx gain的模式：getrxgainctrl",(void *)getrxgainCtrl);
    RegisterCommand("setorxgainctrl","设置orx gain的模式：setorxgainctrl mode(0:MGC 1:AGC) ctrlway(0:spi 1:pin)",(void *)setorxgainCtrl);
    RegisterCommand("getorxgainctrl","获取orx gain的模式：getorxgainctrl",(void *)getorxgainCtrl);
    RegisterCommand("txgain","设置txgain 通道 index值：txgain channels index",(void *)settxgain);
    RegisterCommand("rxgain","设置rxgain 通道 index值：rxgain channels index",(void *)setrxgain);
    RegisterCommand("orxgain","设置orxgain 通道 index值：orxgain channels index",(void *)setorxgain);
    RegisterCommand("getgain","获取rx/tx对应通道的gainindex：getgain channels",(void *)getgain);
    RegisterCommand("dds1", "发送tx1DDS：dds1 frqkhz atten", (void *)tx1_tone);
    RegisterCommand("dds2", "发送tx2DDS：dds2 frqkhz atten", (void *)tx2_tone);
    RegisterCommand("ddsoff", "关闭DDS", (void *)toneoff);
    RegisterCommand("setfreq", "设置PLL频率：setfreq rf_freq_hz  aux_freq_hz", (void *)set_freq);
    RegisterCommand("radioopen", "打开通道：1.pin 2.spi", (void *)radio_on);
    RegisterCommand("fpgasend","从fpga中发DDS:(同目录需要有.txt文件)",(void *)fpga_send);
    RegisterCommand("debugdata","在BZ20内部采数 位置 采数大小 速率(245/491)：debugdata position bytes 1:245Mhz 2:491Mhz",(void *)debugdata);
    RegisterCommand("rxpower","rx功率统计：rxpower adcchannels",(void *)getrxdecpower);
    RegisterCommand("orxpower","orx功率统计：orxpower adcchannels",(void *)getorxdecpower);
    RegisterCommand("setagc","设置agc：rxpower adcchannels",(void *)set_agc);
    RegisterCommand("log","打印riscvlog",(void *)getlog);
    RegisterCommand("bt","打印riscvtrace",(void *)riscv_trace);
    RegisterCommand("paprotect","测试PA保护",(void *)paprotection);
}


