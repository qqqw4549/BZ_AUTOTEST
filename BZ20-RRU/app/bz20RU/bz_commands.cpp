#include "bz_commands.h"
#include "command.h"

#include <cstdint>
#include <cstdio>
#include <iostream>
namespace 
{
    Talise::WeakPtr gTalise;
}

void SetTakise(Talise::WeakPtr const& talise)
{
    gTalise = talise;
}
extern "C"
{
void WriteReg(uint16_t addr, uint32_t data)
{
    Talise::ShardPtr talise = gTalise.lock();
    if(talise)
    {
        uint32_t data1;
        talise->write_reg(addr, data);
        talise->read_reg(addr, data1);
        printf("WriteReg,readback reg: 0x%04x=0x%08x\n", addr, data1);
    }
}

void WriteRegFiled(uint16_t addr, uint32_t data, uint8_t end, uint8_t start)
{
    Talise::ShardPtr talise = gTalise.lock();
    if(talise)
    {
        uint32_t data1;
        talise->write_reg(addr, data, end, start);
        talise->read_reg(addr, data1);
        printf("WriteReg,readback reg: 0x%04x=0x%08x\n", addr, data1);
    }
}


void ReadReg(uint16_t addr)
{
    Talise::ShardPtr talise = gTalise.lock();
    if(talise)
    {
        uint32_t data;
        talise->read_reg(addr, data);
        printf("ReadReg,readback reg: 0x%04x=0x%08x\n", addr, data);
    }
}

void SetFreq(uint32_t freq)
{
    Talise::ShardPtr talise = gTalise.lock();
    if(talise)
    {
        talise->set_pll_frequency( BZ_RF_PLL, freq * 1000);
        talise->set_pll_frequency( BZ_AUX_PLL, freq * 1000);
    }
}

void Tx1Tone(int32_t toneFreq1, int32_t toneFreq2,
    uint32_t gain1, uint32_t gain2)
{
    Talise::ShardPtr talise = gTalise.lock();
    if(talise)
    {
        BzTxDdsTestToneCfg_t tongCfg;
        talise->set_tx_channel(BZ_TX1);
        talise->set_tx_power(toneFreq1, toneFreq2, gain1, gain2);
    }
}

void Tx2Tone(int32_t toneFreq1, int32_t toneFreq2,
    uint32_t gain1, uint32_t gain2)
{
    Talise::ShardPtr talise = gTalise.lock();
    if(talise)
    {
        BzTxDdsTestToneCfg_t tongCfg;
        talise->set_tx_channel(BZ_TX2);
        talise->set_tx_power(toneFreq1, toneFreq2, gain1, gain2);
    }
}

void EVM(uint32_t index)
{
    const char* filenames[] = {
        "20M_TM3p1_eco_6baize.dat",
        "20M_TM3p1a_eco_6baize.dat",
        "5G_DL_TIME_phase.dat",
        "5G_100M_TM31a_30_eco_6baize_e.dat",
        "5G_100M_TM31a_30_FDD_eco_6baize_b.dat",
        "5G_100M_TM31a_FDD_nofif_eco_6baize.dat",
        "Sine_10M_10ms_0p7_eco_6baize.dat",
        "Sine_3p84_10ms_0p7_eco_6baize.dat",
        "Test_0x55555555.dat",
        "Test_0x12345678.dat"  
    };

    Talise::ShardPtr talise = gTalise.lock();
    if(talise)
    {
        if(index >=0 && index <=  9)
        {
            talise->set_dds_off();
            talise->send_tx_data(filenames[index]);
        }
        else
            talise->stop_tx_data();
    }
}

void CheckTxData(uint32_t value)
{
    Talise::ShardPtr talise = gTalise.lock();
    if(talise)
        talise->check_tx_data(value);
}

void SetTxChannel(uint32_t channel)
{
    Talise::ShardPtr talise = gTalise.lock();
    if(talise)
    {
        if(channel == 0)
            talise->set_tx_channel(BZ_TXOFF);
        else if(channel == 1)
            talise->set_tx_channel(BZ_TX1);
        else if(channel == 2)
            talise->set_tx_channel(BZ_TX2);
        else if(channel == 3)
            talise->set_tx_channel(BZ_TX1TX2);
    }
}

void SetRxChannel(uint32_t channel)
{
    Talise::ShardPtr talise = gTalise.lock();
    if(talise)
    {
        if(channel == 0)
            talise->set_rx_channel(BZ_RXOFF);
        else if(channel == 1)
            talise->set_rx_channel(BZ_RX1);
        else if(channel == 2)
            talise->set_rx_channel(BZ_RX2);
        else if(channel == 3)
            talise->set_rx_channel(BZ_RX1RX2);
    }
}

void SetRxGainControlMode(uint32_t index)
{
    Talise::ShardPtr talise = gTalise.lock();
    if(talise)
    {
        if(index == 0) //api
            talise->set_rx_gain_control_mode(true);
        else
        {
            talise->set_rx_gain_control_mode(false);
            talise->set_rx_gain_ctrl_pin();
        }
    }
}

void GpioRxGainAdd(uint32_t channel)
{
    Talise::ShardPtr talise = gTalise.lock();
    if(talise)
    {
        if(channel == 0)
            talise->rx1_gain(true);
        else
            talise->rx2_gain(true);
    }
}

void GpioRxGainSub(uint32_t channel)
{
    Talise::ShardPtr talise = gTalise.lock();
    if(talise)
    {
        if(channel == 0)
            talise->rx1_gain(false);
        else
            talise->rx2_gain(false);
    }
}

void SetTxGain(uint32_t index)
{
    Talise::ShardPtr talise = gTalise.lock();
    if(talise)
        talise->set_tx_gain(index);
}

void GetTxGain(void)
{
    Talise::ShardPtr talise = gTalise.lock();
    if(talise)
    {
        uint32_t index = 0;
        talise->tx_gain(index);
        printf("Tx gain index: %d\n", index);
    }
}

void SetTxPaProtect(uint32_t count, uint32_t tx_power_threshold)
{
    Talise::ShardPtr talise = gTalise.lock();
    if(talise)
        talise->set_tx_pa_protection(count, tx_power_threshold);
}

void SwitchChip(uint32_t device_id)
{
    Talise::ShardPtr talise = gTalise.lock();
    if(talise && (device_id >=0 && device_id < 2))
    {
        talise->set_tx_pa_protection(1, 10000, false);
        talise->term_device();
        talise->set_device_id(device_id);
        talise->chip_select(device_id);
        talise->start_204b_sysc();
        talise->init_device();

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

        if(is_sync){
            std::cout << "204B sync is OK" << std::endl;
            talise->set_204b_sysref(false);
        }

        talise->interruput_on();
        talise->lanpa_power(true);
        talise->radio_on();
        talise->set_tx_pa_protection(1, 10000);
    }
}

void RxData(uint32_t points, uint32_t channels)
{
    Talise::ShardPtr talise = gTalise.lock();
    if(talise)
        talise->recv_rx_data(points, channels);
}

}
void cals(uint32_t bitmap)
{
    Talise::ShardPtr talise = gTalise.lock();
    talise->cals(bitmap);
}

void RegisterBzCommands()
{
    RegisterCommand("bzr", "16 bit regitser read  eg. bzr 0x37", (void *)ReadReg);
    RegisterCommand("bzw", "16 bit register write eg. bzw 0x37 0x8033", (void *)WriteReg);
    RegisterCommand("bzwf", "16 bit register write eg. bzw 0x37 0x8033 5 0", (void *)WriteRegFiled);
    RegisterCommand("bzf", "set freq(kHz). eg. bzf 2600000", (void *)SetFreq);
    RegisterCommand("bztone1", "tx1 tone eg. bztone1  10000 10000 0~7 0~7", (void *)Tx1Tone);
    RegisterCommand("bztone2", "tx2 tone eg. bztone1  10000 10000 0~7 0~7", (void *)Tx2Tone);
    RegisterCommand("txchannel", "txchannel index eg. txchannel  1\n"
                    "             0 - TXOFF\n"
                    "             1 - TX1\n"
                    "             2 - TX2\n"
                    "             3 - TX1TX2", (void *)SetTxChannel);
    RegisterCommand("rxchannel", "rxchannel index eg. rxchannel  1\n"
                    "             0 - RXOFF\n"
                    "             1 - RX1\n"
                    "             2 - RX2\n"
                    "             3 - RX1RX2", (void *)SetRxChannel);  
    RegisterCommand("rxgainmode", "rxgainmode index eg. rxgainmode  1\n"
                    "             0 - API\n"
                    "             1 - GPIO", (void *)SetRxGainControlMode);
    RegisterCommand("rxgaini", "rxgaininc channel eg. rxgaini  0/1", (void *)GpioRxGainAdd);
    RegisterCommand("rxgaind", "rxgaindec channel eg. rxgaind  0/1", (void *)GpioRxGainSub);              
    RegisterCommand("txgain", "txgain index(0-63) eg. txgain  1", (void *)SetTxGain);
    RegisterCommand("txgaing", "get tx gain index(0-63) eg. txgaing", (void *)GetTxGain);
    RegisterCommand("evm", "evm index(0-9) eg. evm  3 index\n"
                    "             0 - 20M_TM3p1_eco_6baize.dat\n"
                    "             1 - 20M_TM3p1a_eco_6baize.dat\n"
                    "             2 - 5G_DL_TIME_phase.dat\n"
                    "             3 - 5G_100M_TM31a_30_eco_6baize_e.dat\n"
                    "             4 - 5G_100M_TM31a_30_FDD_eco_6baize_b.dat\n"
                    "             5 - 5G_100M_TM31a_FDD_nofif_eco_6baize.dat\n"
                    "             6 - Sine_10M_10ms_0p7_eco_6baize.dat\n"
                    "             7 - Sine_3p84_10ms_0p7_eco_6baize.dat\n"
                    "             8 - Test_0x55555555.dat\n"
                    "             9 - Test_0x12345678.dat\n"
                    "            10 - Stop send evm", (void *)EVM);
    RegisterCommand("checktxd", "checktxd value eg. checktxd  0x12345678", (void *)CheckTxData);
    RegisterCommand("paprotect", "paprotect count threshold eg. paprotect  1 10000", (void *)SetTxPaProtect);
    RegisterCommand("switchchip", "switchchip device_id  eg. switchchip  0/1", (void *)SwitchChip);
    RegisterCommand("rxdata", "rxdata points channels  eg. rxdata  1024 1/2/3", (void *)RxData);
    RegisterCommand("cals", "cals 0x400", (void *)cals);
}
