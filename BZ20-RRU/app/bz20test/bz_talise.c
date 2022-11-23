#include "bz_config.h"
#include "bz_hal.h"
#include "bz_example_hal.h"
#include "plaxi.h"

#include <bz_agc.h>
#include <bz_cals.h>
#include <bz_error.h>
#include <bz_gpio.h>
#include <bz_init.h>
#include <bz_jesd204b.h>
#include <bz_otp.h>
#include <bz_plat_hal.h>
#include <bz_radio.h>
#include <bz_riscv.h>
#include <bz_rx.h>
#include <bz_tx.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <malloc.h>
#include "appclock.h"
#include "testcase.h"
extern uint32_t gulLogLevel;
static BzDevice_t g_device = { 0 };
static int g_plaxi_file = -1;
static int g_mem_file = -1;
char * pRX_Map;
char * pTX_Map;
#define DEVICE_HANDLE &g_device
#define PLAXI_NAME "/dev/plaxi"
#define MEM_NAME   "/dev/mem"
#define RX_204B_OFFSET  0x38000000
#define RX_204B_SIZE    0x8000000
#define TX_204B_OFFSET  0x30000000
#define TX_204B_SIZE    0x8000000
#define DATA_SIZE 19660800

int gMode = 0x2;


#define FUNCTION_CHECK(action)                   \
    if(action == BZHAL_OK) {                     \
        BZHAL_Printf("%s is OK!\n", __func__);   \
        return 0;                                \
    }                                            \
    else {                                       \
        BZHAL_Printf("%s is fail!\n", __func__); \
        return -1;                               \
    }

#define RETURN_CHECK(retval)\
    if(retval == BZHAL_OK){ \
        BZHAL_Printf("%s is OK!\n", __func__);   \
    }\
    else{\
        BZHAL_Printf("%s is fail!\n", __func__); \
    }

#define FUNCTION_START \
    BZHAL_Printf("%s ...\n", __func__);

int32_t open_device(uint32_t device_id)
{
    uint32_t bzAction = BZ_openHw(DEVICE_HANDLE, device_id);
    g_plaxi_file = open(PLAXI_NAME, O_RDWR);

    FUNCTION_CHECK(bzAction)
}

int32_t close_device()
{
    munmap(pTX_Map, TX_204B_SIZE);
    munmap(pRX_Map, RX_204B_SIZE);
    close(g_plaxi_file);
    close(g_mem_file);
    BZ_closeHw(DEVICE_HANDLE);
}

int32_t rest_device(void)
{
    uint32_t bzAction = BZ_resetDevice(DEVICE_HANDLE);
    FUNCTION_CHECK(bzAction)
}

int32_t wait_boot_status(void)
{
    FUNCTION_START
    uint32_t bzAction = BZ_getBootStatus(DEVICE_HANDLE);
    FUNCTION_CHECK(bzAction)
}

int32_t load_firmware(void)
{
    uint32_t bzAction = BZ_loadFirmware(DEVICE_HANDLE);
    FUNCTION_CHECK(bzAction)
}

int32_t wait_firmware_ready(void)
{
    FUNCTION_START
    uint32_t bzAction = BZ_waitFirmwareReady(DEVICE_HANDLE, 100);
    bzAction = BZ_getSdkVersion(DEVICE_HANDLE);
    FUNCTION_CHECK(bzAction)
}

int32_t load_config(void)
{
    uint32_t bzAction = BZ_initialize(DEVICE_HANDLE, &B20Init);
    FUNCTION_CHECK(bzAction)
}

int32_t write_profile(void)
{
    uint32_t bzAction = BZ_writeRiscvProfile(DEVICE_HANDLE, &B20Init);
    FUNCTION_CHECK(bzAction)
}

int32_t wait_profile_done(void)
{
    FUNCTION_START
    uint32_t bzAction = BZ_waitProfileDone(DEVICE_HANDLE, 100);
    FUNCTION_CHECK(bzAction)
}

int32_t set_rf_pll_freq(uint64_t freq_hz)
{
    uint32_t bzAction = BZ_setPllFrequency(DEVICE_HANDLE, BZ_RF_PLL, freq_hz);
    FUNCTION_CHECK(bzAction)
}

int32_t set_aux_pll_freq(uint64_t freq_hz)
{
    uint32_t bzAction = BZ_setPllFrequency(DEVICE_HANDLE, BZ_AUX_PLL, freq_hz);
    FUNCTION_CHECK(bzAction)
}

int32_t wait_pll_lock_done(void)
{
    FUNCTION_START
    uint32_t bzAction = BZ_waitPllsLockDone(DEVICE_HANDLE, 5000);
    FUNCTION_CHECK(bzAction)
}

int32_t run_cals(uint32_t mask)
{
    uint32_t bzAction = BZ_runInitCals(DEVICE_HANDLE, mask);
    FUNCTION_CHECK(bzAction)
}

int32_t wait_cals_done(void)
{
    FUNCTION_START
    uint32_t bzAction = BZ_waitInitCalsDone(DEVICE_HANDLE, 5000);
    BZ_getInitCalsStatus(DEVICE_HANDLE);
    FUNCTION_CHECK(bzAction)
}

int32_t getcals_status(){
    FUNCTION_START
    uint32_t bzAction = BZ_getInitCalsStatus(DEVICE_HANDLE);
    FUNCTION_CHECK(bzAction)
}

void start_204b_sysc(void)
{
    uint32_t value;
    ioctl(g_plaxi_file, PLAXI_READ_204B_PARAM, &value);
    value |= (1 << 16);
    ioctl(g_plaxi_file, PLAXI_WRITE_204B_PARAM, &value);
}

void end_204b_sysc(void)
{
    uint32_t value;
    ioctl(g_plaxi_file, PLAXI_READ_204B_PARAM, &value);
    value &= (~(1 << 16));
    ioctl(g_plaxi_file, PLAXI_WRITE_204B_PARAM, &value);
}

int32_t jesd204b_sysc_is_ok()
{
    uint32_t value;
    ioctl(g_plaxi_file, PLAXI_READ_204B_STATE, &value);
    BZ_PRINT("state = 0x%x",value);
    return (value & 0xF) == 0xF;
}

int32_t jesd204b_config(void)
{
    uint32_t bzAction = BZ_writeJes204bConfig(DEVICE_HANDLE, &B20Init);
    FUNCTION_CHECK(bzAction)
}
 
uint32_t start_auxadc(BzAuxAdcConfig_t *auxAdcConfig)
{
    uint32_t bzAction = BZ_startAuxAdc(&g_device, auxAdcConfig);
    FUNCTION_CHECK(bzAction)
}

uint32_t read_auxadc(BzAuxAdcResult_t *auxAdcConfig)
{
    uint32_t bzAction = BZ_readAuxAdc(&g_device, auxAdcConfig);
    FUNCTION_CHECK(bzAction)
}

uint32_t setup_auxdac(uint16_t Gpio3V3Num)
{
    uint32_t bzAction = BZ_setupAuxDacs(&g_device, Gpio3V3Num); 
    FUNCTION_CHECK(bzAction)
}

uint32_t write_auxdac(uint16_t Gpio3V3Num,uint16_t auxDacCode)
{
    uint32_t bzAction = BZ_writeAuxDac(&g_device, Gpio3V3Num, auxDacCode); 
    FUNCTION_CHECK(bzAction)
}

int32_t jesd204b_cfgchange( BzTxConfig_t stTx204BParam, BzRxOrXConfig_t stRx204BParam )
{
    // Tx Cfg
    BZ_PRINT("JESD 204B Tx Cfg:\n");
    BZ_PRINT("\t cf = %d", B20Init.jesd204Settings.txConfig.cf = stTx204BParam.cf);
    BZ_PRINT("\t cs = %d", B20Init.jesd204Settings.txConfig.cs = stTx204BParam.cs);
    BZ_PRINT("\t f = %d", B20Init.jesd204Settings.txConfig.f = stTx204BParam.f);
    BZ_PRINT("\t k = %d", B20Init.jesd204Settings.txConfig.k = stTx204BParam.k); 
    BZ_PRINT("\t l = %d", B20Init.jesd204Settings.txConfig.l = stTx204BParam.l); 
    BZ_PRINT("\t n = %d", B20Init.jesd204Settings.txConfig.n = stTx204BParam.n);
    BZ_PRINT("\t m = %d", B20Init.jesd204Settings.txConfig.m = stTx204BParam.m);
    BZ_PRINT("\t ntotal = %d", B20Init.jesd204Settings.txConfig.ntotal = stTx204BParam.ntotal); 
    BZ_PRINT("\t s = %d", B20Init.jesd204Settings.txConfig.s = stTx204BParam.s); 
    BZ_PRINT("\t hd = %d\n", B20Init.jesd204Settings.txConfig.hd = stTx204BParam.hd); 
    // Rx Cfg
    BZ_PRINT("JESD 204B Rx Cfg:\n");
    BZ_PRINT("\t cf = %d",B20Init.jesd204Settings.rxConfig.cf = stRx204BParam.cf);
    BZ_PRINT("\t cs = %d",B20Init.jesd204Settings.rxConfig.cs = stRx204BParam.cs);
    BZ_PRINT("\t f = %d",B20Init.jesd204Settings.rxConfig.f = stRx204BParam.f);
    BZ_PRINT("\t k = %d",B20Init.jesd204Settings.rxConfig.k = stRx204BParam.k);
    BZ_PRINT("\t l = %d",B20Init.jesd204Settings.rxConfig.l = stRx204BParam.l);
    BZ_PRINT("\t n = %d",B20Init.jesd204Settings.rxConfig.n = stRx204BParam.n);
    BZ_PRINT("\t m = %d",B20Init.jesd204Settings.rxConfig.m = stRx204BParam.m);
    BZ_PRINT("\t ntotal = %d",B20Init.jesd204Settings.rxConfig.ntotal = stRx204BParam.ntotal);
    BZ_PRINT("\t s = %d",B20Init.jesd204Settings.rxConfig.s = stRx204BParam.s);
    BZ_PRINT("\t hd = %d",B20Init.jesd204Settings.rxConfig.hd = stRx204BParam.hd);
    BZ_PRINT("\t txIlasDelay = %d\n",B20Init.jesd204Settings.rxConfig.txIlasDelay = stRx204BParam.txIlasDelay);
}

int32_t wait_jesd204b_config(void)
{
    FUNCTION_START
    uint32_t bzAction = BZ_waitJes204bConfigDone(DEVICE_HANDLE, 100);
    bzAction = BZ_enableFramerLink(DEVICE_HANDLE, BZ_FRAMER_A, 0);
    bzAction = BZ_enableFramerLink(DEVICE_HANDLE, BZ_FRAMER_A, 1);
    bzAction = BZ_enableFramerLink(DEVICE_HANDLE, BZ_FRAMER_B, 0);
    bzAction = BZ_enableFramerLink(DEVICE_HANDLE, BZ_FRAMER_B, 1);
    bzAction = BZ_enableDeframerLink(DEVICE_HANDLE, BZ_DEFRAMER, 0);
    bzAction = BZ_enableDeframerLink(DEVICE_HANDLE, BZ_DEFRAMER, 1);
    FUNCTION_CHECK(bzAction)
}


int32_t radio_on(int32_t siMode)
{
    uint32_t bzAction = BZ_radioOn(DEVICE_HANDLE,siMode);
    gMode = siMode;
    FUNCTION_CHECK(bzAction)
}

int32_t tx1_tone(uint32_t freq1_khz,int32_t gain1)
{
    BzTxDdsTestToneCfg_t config;
    config.txTone1Freq_kHz = freq1_khz;
    config.txTone2Freq_kHz = freq1_khz;

    BzTxDdsAttenCfg_t ddsAtten;
    ddsAtten.txTone0Atten = gain1;
    ddsAtten.txTone1Atten = gain1;
    ddsAtten.txTone2Atten = gain1;
    ddsAtten.txTone3Atten = gain1;

    uint32_t bzAction = BZ_setDDsFreqConfig(DEVICE_HANDLE,
        BZ_TX1, config, ddsAtten);
    FUNCTION_CHECK(bzAction)
}

int32_t tx2_tone(uint32_t freq2_khz,int32_t gain2)
{
    BzTxDdsTestToneCfg_t config;
    config.txTone1Freq_kHz = freq2_khz;
    config.txTone2Freq_kHz = freq2_khz;

    BzTxDdsAttenCfg_t ddsAtten;
    ddsAtten.txTone0Atten = gain2;
    ddsAtten.txTone1Atten = gain2;
    ddsAtten.txTone2Atten = gain2;
    ddsAtten.txTone3Atten = gain2;

    uint32_t bzAction = BZ_setDDsFreqConfig(DEVICE_HANDLE,
        BZ_TX2, config, ddsAtten);
    FUNCTION_CHECK(bzAction)
    return bzAction == BZHAL_OK ? 0: -1;
}
uint32_t get_204()
{
    uint32_t uiRet=0;
    BZ_getSyncStatus(DEVICE_HANDLE,&uiRet);
    if(uiRet==0)
    {
        BZ_PRINT("204 sync OK!\n");
    }
    else BZ_PRINT("204 sync fail! ERRCODE = %d \n",uiRet);
}

uint32_t recv_orx_data(uint32_t points, uint32_t channels)
{
    /****************** start recv ***********************/
    uint32_t value = 0;
    int16_t* data = NULL;
    int i=0,count = points*2;
    int16_t siTmpData = 0;
    FILE *fp;

    ioctl(g_plaxi_file, PLAXI_READ_IOPAD_PARAM, &value);
    BZ_PRINT("Reg.RegAddr = 0x5,Reg.RegValue = 0x%x\n", value);

    value &= 0xFFFFFF8F;//0xFFFFFFCF
    ioctl(g_plaxi_file, PLAXI_WRITE_IOPAD_PARAM, &value);
    ioctl(g_plaxi_file, PLAXI_READ_IOPAD_PARAM, &value);
    BZ_PRINT("Reg.RegAddr = 0x5,Reg.RegValue = 0x%x\n", value);
    usleep(10 * 1000);

    ioctl(g_plaxi_file, PLAXI_READ_RX_DATA_PARAM, &value);
    BZ_PRINT("Reg.RegAddr = 0x1,Reg.RegValue = 0x%x\n", value);
    value = 0;
    ioctl(g_plaxi_file, PLAXI_WRITE_RX_DATA_PARAM, &value);
    ioctl(g_plaxi_file, PLAXI_READ_RX_DATA_PARAM, &value);
    BZ_PRINT("Reg.RegAddr = 0x1,Reg.RegValue = 0x%x\n", value);
    usleep(10 * 1000);

    value = ((points / 1024) << 16) & 0xFFFF0000;
    ioctl(g_plaxi_file, PLAXI_WRITE_RX_DATA_PARAM, &value);
    ioctl(g_plaxi_file, PLAXI_READ_RX_DATA_PARAM, &value);
    BZ_PRINT("Reg.RegAddr = 0x1,Reg.RegValue = 0x%x\n", value);
    usleep(10 * 1000);

    value = ((points / 1024) << 16) & 0xFFFF0000;
    value |= (0x01 << 3);
    ioctl(g_plaxi_file, PLAXI_WRITE_RX_DATA_PARAM, &value);
    ioctl(g_plaxi_file, PLAXI_READ_RX_DATA_PARAM, &value);
    BZ_PRINT("Reg.RegAddr = 0x1,Reg.RegValue = 0x%x\n", value);
    sleep(2);

    value = ((points / 1024) << 16) & 0xFFFF0000;
    ioctl(g_plaxi_file, PLAXI_WRITE_RX_DATA_PARAM, &value);
    ioctl(g_plaxi_file, PLAXI_READ_RX_DATA_PARAM, &value);
    BZ_PRINT("Reg.RegAddr = 0x1,Reg.RegValue = 0x%x\n", value);
    usleep(10 * 1000);

    //recv data;
    //memcpy

    /*********************** BZ_PRINT   ************************/
    data = (int16_t*)pRX_Map;
    BZ_PRINT("|********  rx data **********|\n");
    fp = fopen("data.txt","w+");
    for(i = 0; i < count ; i++)
    {
        siTmpData = *data;
        fprintf(fp,"%d\n",siTmpData/4);
        BZ_PRINT("%d\n",siTmpData/4);
        data++;
    }
    BZ_PRINT("|********  finish  **********|\n");
    fclose(fp);
    usleep(30 * 1000);
    value = 0;
    /*********************** end recv ************************/

    usleep(30 * 1000);

    value = 0;
    ioctl(g_plaxi_file, PLAXI_WRITE_RX_DATA_PARAM, &value);
    usleep(10 * 1000);
}

uint32_t test_tx_data()
{
    uint32_t* data = (uint32_t*)pTX_Map;
    BZ_PRINT("print top 10 send TX data.\n");
    for(uint32_t i = 0; i < 10; i++)
    {
        BZ_PRINT("%d \n",*data);
        data++;
    }
    BZ_PRINT("finish\n");
}

uint32_t debug_data(uint32_t addr,uint32_t position,uint32_t count,uint32_t rate)
{
    BZ_spiGetJesdDebugData(DEVICE_HANDLE,addr,position,count,rate);
}

uint32_t getrxdecpower(BzAuxAdcChannels_t auxadcChannel)
{
    sint16_t siPower=0;
    BZ_getRxDecPower(DEVICE_HANDLE,auxadcChannel,&siPower);
    BZ_PRINT("getrxDecPower = %d dBFS\n",siPower);
}
uint32_t getorxdecpower(BzAuxAdcChannels_t auxadcChannel)
{
    sint16_t siPower=0;
    BZ_getORxDecPower(DEVICE_HANDLE,auxadcChannel,&siPower);
    BZ_PRINT("getorxDecPower = %d dBFS\n",siPower);
}

uint32_t fpga_send()
{
    BZ_setDdsOff(DEVICE_HANDLE);
    int file = open("./sin10M.dat", O_RDONLY);
    uint32_t value = 0;
    char * buf = NULL;
    ssize_t size = 0;
    if(file<0)
    {
        BZ_PRINT("file read error!\n");
        return 0;
    }
    ioctl(g_plaxi_file, PLAXI_WRITE_TX_DATA_PARAM, &value);
    buf = (char *)malloc(sizeof(char)*DATA_SIZE);
    size = read(file,buf,DATA_SIZE);
    if(size<0)
    {
        BZ_PRINT("file copy error!\n");
        close(file);
        return 0;
    }
    memcpy(pTX_Map,buf,size);
    test_tx_data();
    value = ((size / 4 / 1024) << 16);
    value |= (0x02 << 1);
    ioctl(g_plaxi_file, PLAXI_WRITE_TX_DATA_PARAM, &value);
    usleep(50 * 1000);

    value = ((size / 4 / 1024) << 16);
    value |= (0x02 << 1);
    value |= (0x01 << 4);
    ioctl(g_plaxi_file, PLAXI_WRITE_TX_DATA_PARAM, &value);
    close(file);
    free(buf);
}
uint32_t recv_rx_data(uint32_t points, uint32_t channels)
{
    /****************** start recv ***********************/
    uint32_t value = 0;
    int16_t* data = NULL;
    int i=0,count = points*2;
    int16_t siTmpData = 0;
    FILE *fp;

    ioctl(g_plaxi_file, PLAXI_READ_IOPAD_PARAM, &value);
    BZ_PRINT("Reg.RegAddr = 0x5,Reg.RegValue = 0x%x\n", value);

    value &= 0xFFFFFF8F;//0xFFFFFFCF
    ioctl(g_plaxi_file, PLAXI_WRITE_IOPAD_PARAM, &value);
    ioctl(g_plaxi_file, PLAXI_READ_IOPAD_PARAM, &value);
    BZ_PRINT("Reg.RegAddr = 0x5,Reg.RegValue = 0x%x\n", value);
    usleep(10 * 1000);

    value |= (channels << 4); // 1 - rx1 2 - rx2 3 -rx1rx2
    ioctl(g_plaxi_file, PLAXI_WRITE_IOPAD_PARAM, &value);
    ioctl(g_plaxi_file, PLAXI_READ_IOPAD_PARAM, &value);
    BZ_PRINT("Reg.RegAddr = 0x5,Reg.RegValue = 0x%x\n", value);
    usleep(10 * 1000);

    ioctl(g_plaxi_file, PLAXI_READ_RX_DATA_PARAM, &value);
    BZ_PRINT("Reg.RegAddr = 0x1,Reg.RegValue = 0x%x\n", value);
    value = 0;
    ioctl(g_plaxi_file, PLAXI_WRITE_RX_DATA_PARAM, &value);
    ioctl(g_plaxi_file, PLAXI_READ_RX_DATA_PARAM, &value);
    BZ_PRINT("Reg.RegAddr = 0x1,Reg.RegValue = 0x%x\n", value);
    usleep(10 * 1000);

    value = ((points / 1024) << 16) & 0xFFFF0000;
    ioctl(g_plaxi_file, PLAXI_WRITE_RX_DATA_PARAM, &value);
    ioctl(g_plaxi_file, PLAXI_READ_RX_DATA_PARAM, &value);
    BZ_PRINT("Reg.RegAddr = 0x1,Reg.RegValue = 0x%x\n", value);
    usleep(10 * 1000);

    value = ((points / 1024) << 16) & 0xFFFF0000;
    value |= (0x01 << 3);
    ioctl(g_plaxi_file, PLAXI_WRITE_RX_DATA_PARAM, &value);
    ioctl(g_plaxi_file, PLAXI_READ_RX_DATA_PARAM, &value);
    BZ_PRINT("Reg.RegAddr = 0x1,Reg.RegValue = 0x%x\n", value);
    sleep(2);

    value = ((points / 1024) << 16) & 0xFFFF0000;
    ioctl(g_plaxi_file, PLAXI_WRITE_RX_DATA_PARAM, &value);
    ioctl(g_plaxi_file, PLAXI_READ_RX_DATA_PARAM, &value);
    BZ_PRINT("Reg.RegAddr = 0x1,Reg.RegValue = 0x%x\n", value);
    usleep(10 * 1000); 
    /*********************** BZ_PRINT   ************************/
    data = (int16_t*)pRX_Map;
    BZ_PRINT("|********  rx data **********|\n");
    fp = fopen("data.txt","w+");
    for(i = 0; i < count ; i++)
    {
        siTmpData = *data;
        fprintf(fp,"%d\n",siTmpData/4);
        BZ_PRINT("%d\n",siTmpData/4);
        data++;
    }
    BZ_PRINT("|********  finish  **********|\n");
    fclose(fp);
    usleep(30 * 1000);
    value = 0;
    /*********************** end recv ************************/
    ioctl(g_plaxi_file, PLAXI_WRITE_RX_DATA_PARAM, &value);
    usleep(10 * 1000);
}

uint32_t get_radio_state()
{
    BZ_getRadioState(DEVICE_HANDLE);
}
uint32_t read_reg(uint16_t addr)
{
    uint32_t data;
   BZ_spiReadReg(DEVICE_HANDLE,addr,&data);
   BZ_PRINT("readreg 0x%x value = 0x%x\n",addr,data);
   return data;
}
uint32_t read_regbit(uint16_t addr,uint32_t endbit,uint32_t startbit)
{
    uint32_t regdata=0;

    BZ_spiReadField(DEVICE_HANDLE,addr,&regdata,endbit,startbit);
    BZ_PRINT("readregbit <%d:%d> = 0x%x\n",endbit,startbit,regdata);
    return regdata;
}
uint32_t write_reg(uint16_t addr ,uint32_t data)
{
    BZ_spiWriteReg(DEVICE_HANDLE,addr,data);
}
uint32_t write_regbit(uint16_t addr ,uint32_t endbit,uint32_t startbit,uint32_t data)
{
    uint32_t regdata=0,regmask=0;
    BZ_spiReadReg(DEVICE_HANDLE,addr,&regdata);
    regmask = ~((1<<startbit)-1);
    regmask &= ((1<<endbit)-1)|(1<<endbit);
    regdata &= ~regmask;
    regdata |= (data << startbit) & regmask;
    BZ_spiWriteReg(DEVICE_HANDLE,addr,data);
}
uint32_t get_path()
{
    uint32_t siRX, siORX,siTX;
   uint32_t siPathBit=0;
   if(gMode==0x2)
   {
        BZ_getRadioCtrlSpiMode(DEVICE_HANDLE,&siRX,&siORX,&siTX);
        BZ_PRINT("spimode RX = %d ,ORX = %d ,TX = %d \n",siRX,siORX,siTX);   
   }
   else if(gMode == 0x1)
   {
        BzRecoveryActions_t bzRetAct=BZ_NO_ACTION;
	    bzHalErr halStatus=BZHAL_OK;
	    uint32_t radioStatus = 0i,ulRX=0,ulTX=0,ulORX=0;

	    halStatus=BZ_spiReadReg(DEVICE_HANDLE,0x411,&radioStatus);
	    bzRetAct=BZ_ApiErrHandler(DEVICE_HANDLE,BZ_ERRHDL_HAL_TX,halStatus,bzRetAct,BZ_ERR_RESET_SPI);
	    IF_ERR_RETURN(bzRetAct);
	    if((radioStatus&TX0ENMASK) == 0x400)ulTX+=1;
	    if((radioStatus&TX1ENMASK) == 0x800)ulTX+=2;
	    if((radioStatus&RX0ENMASK) == 0x1000)ulRX+=1;
	    if((radioStatus&RX1ENMASK) == 0x2000)ulRX+=2;
	    if((radioStatus&ORX0ENMASK) == 0x4000)ulORX+=1;
	    if((radioStatus&ORX1ENMASK) == 0x8000)ulORX+=2;
        BZ_PRINT("pinmode , RX = %d ,ORX = %d ,TX = %d \n",ulRX,ulORX,ulTX);
   }
}
uint32_t set_path(int siRX,int siORX,int siTX)
{
   uint32_t siPathBit=0;
   if(gMode==0x2)
   {
        BZ_PRINT("spimode\n");
        BZ_setRadioCtrlSpiMode(DEVICE_HANDLE,siRX,siORX,siTX); 
   }
   else if(gMode == 0x1)
   {
        BZ_PRINT("pinmode\n");
        BZ_setRadioCtrlPinMode(DEVICE_HANDLE,siRX,siORX,siTX);
   }
}

uint32_t settxgain(uint32_t channels, uint8_t gainIndexTx)
{
    BZ_setTxManualGain(DEVICE_HANDLE,channels,gainIndexTx);
}

uint32_t setrxgain(uint32_t channels, uint8_t gainIndexRx)
{
    BZ_setRxManualGain(DEVICE_HANDLE,channels,gainIndexRx);
}
uint32_t setorxgain(uint32_t channels ,uint8_t gainIndexOrx)
{
    BZ_setObsRxManualGain(DEVICE_HANDLE,channels,gainIndexOrx);
}


uint32_t getgain(uint32_t channels)
{
    uint32_t indexrx=0,indextx=0,indexorx=0;
    BZ_getTxGain(DEVICE_HANDLE,channels,&indexrx);
    BZ_PRINT("Tx = %d ,gain = %d\n",channels,indexrx);
    BZ_getRxGain(DEVICE_HANDLE,channels,&indextx);
    BZ_PRINT("Rx = %d ,gain = %d\n",channels,indextx);
    BZ_getObsRxGain(DEVICE_HANDLE,channels,&indexorx);
    BZ_PRINT("Orx = %d ,gain = %d\n",channels,indexorx);
}

void init_process()
{
    uint32_t value = 0;
// open memfile
    g_mem_file = open(MEM_NAME, O_RDWR | O_SYNC);
    g_plaxi_file = open(PLAXI_NAME, O_RDWR);
    pRX_Map = (char *)mmap(0, RX_204B_SIZE,PROT_READ | PROT_WRITE, MAP_SHARED, g_mem_file, RX_204B_OFFSET);
    pTX_Map = (char *)mmap(0, TX_204B_SIZE,PROT_READ | PROT_WRITE, MAP_SHARED, g_mem_file, TX_204B_OFFSET);
    // chip select
    ioctl(g_plaxi_file, PLAXI_CHIP_SEL, &value);
}

void end_process()
{
    int i=0;
    uint32_t value =0;
    // check state
    for(i = 0 ;i < 10 ;i++)
    {
        usleep(1000*100);
        if(jesd204b_sysc_is_ok())
        {
            BZ_PRINT("204sync ok!\n");
            break;
        }
    }
    // end 204B
    value = 0x4f4f;
    ioctl(g_plaxi_file, PLAXI_WRITE_204B_PARAM, &value);

    //204b SYSREF
    BZ_Jes204bSysrefControl(&g_device, BZ_JES204B_SYSREF_OFF);
    // interruput_on
    BZHAL_InterruptOn(g_device.devHalInfo);
    // lanpa on
    value = 1;
    ioctl(g_plaxi_file, PLAXI_SET_LANPA_PWR, &value);
    // gio
    value = 0;
    ioctl(g_plaxi_file, PLAXI_READ_IO_DIR_PARAM, &value);
    value |= 0xF00;
    ioctl(g_plaxi_file, PLAXI_WRITE_IO_DIR_PARAM, &value);
}

uint32_t sdk_example(int siMode)
{   
   BzDevice_t *hw_priv=NULL; 

    uint32_t initCalType = TX_BB_FILTER|ADC_TUNER|TIA_3DB_CORNER
            |DC_OFFSET|PATH_DELAY|TX_LO_LEAKAGE_INTERNAL
            |TX_QEC_INIT|LOOPBACK_RX_LO_DELAY|TX_DAC| 
            LOOPBACK_RX_QEC_INIT|RX_QEC_INIT|TIA_LPF_3DB_CORNER|
            ORX_QEC_INIT;

    BZ_PRINT("all cals = 0x%x\n",initCalType);
	/*Disable B20 calibration*/
	//initCalType = 0;
	//initCalType = 0;
	//1initCalType = 0;
	//initCalType = 0x4000 | 0x1000;


    uint8_t errorFlag = 0;
    uint16_t deframerStatus = 0;

    uint8_t framerStatus = 0;
    uint32_t bzAction = BZHAL_OK;
    uint32_t value=0,i=0;

    gMode = siMode;

    // start 204B
    value = 0x14f4f;
    ioctl(g_plaxi_file, PLAXI_WRITE_204B_PARAM, &value);
    
    /**********************************************************/
    /**********************************************************/
    /************ 1.B20 Initialization Sequence *************/
    /**********************************************************/
    /**********************************************************/

    /*******************************/
    /**** 2.B20 Initialization ***/
    /*******************************/
    hw_priv = &g_device;

	/*Get B20Sdk Version*/
	bzAction=BZ_getSdkVersion(hw_priv);
    //RETURN_CHECK(bzAction)
    /*Open B20 Hw Device*/
    bzAction = BZ_openHw(hw_priv,BZHAL_DEVID0);
    if(bzAction==0){BZ_PRINT("BZ_openHw OK\n");}
    else {BZ_PRINT("BZ_openHw Error\n");}
    
    /* RESET B20 device */
    bzAction = BZ_resetDevice(hw_priv);
        if(bzAction==0){BZ_PRINT("BZ_resetDevice OK\n");}
    else {BZ_PRINT("BZ_resetDevice Error\n");}
	
	/*Get the state at startup on B20 device */
	bzAction = BZ_getBootStatus(hw_priv);
    //RETURN_CHECK(bzAction)
	
	/*DownLoad B20 Firmware*/
	bzAction = BZ_loadFirmware(hw_priv);
    //RETURN_CHECK(bzAction)
	/*Wait Write Riscv Profile To B20 device Done */
	bzAction = BZ_waitFirmwareReady(hw_priv,100);
    //RETURN_CHECK(bzAction)
	
	/*Write Riscv initial configure To b20 device */
    bzAction = BZ_initialize(hw_priv, &B20Init);
    //RETURN_CHECK(bzAction)

	/*Write Riscv Profile To B20 device*/
	bzAction = BZ_writeRiscvProfile(hw_priv,&B20Init);
    //RETURN_CHECK(bzAction)

	/*Wait Riscv Profile To B20 device Done */
	bzAction = BZ_waitProfileDone(hw_priv,100);
    //RETURN_CHECK(bzAction)

	
    /***********************************************/
    /**3.Set RF AND AUX PLL Frequencies, Default 5.5GHz ****/
    /***********************************************/
	bzAction = BZ_setPllFrequency(hw_priv,BZ_RF_PLL,1950000000);
    //RETURN_CHECK(bzAction)
    
	
	/*Wait RF PLL Frequencies Set Done */
	bzAction = BZ_waitPllsLockDone(hw_priv,100);
    //RETURN_CHECK(bzAction)
   

    bzAction = BZ_setPllFrequency(hw_priv,BZ_AUX_PLL,2140000000);
    //RETURN_CHECK(bzAction)
  	
	/*Wait RF PLL Frequencies Set Done */
	bzAction = BZ_waitPllsLockDone(hw_priv,100);
    //RETURN_CHECK(bzAction)
   	
    /****************************************************/
    /**** 4.Run B20 ARM Initialization Calibrations ***/
    /****************************************************/
    /*bzAction = BZ_runInitCals(hw_priv, initCalType);
    RETURN_CHECK(bzAction)
   
    bzAction = BZ_waitInitCalsDone(hw_priv,100);
    RETURN_CHECK(bzAction)
  */
    /***************************************************/
    /**** 5.Enable  B20 JESD204B  ***/
    /***************************************************/

	/*Write JESD204B Config To B20 device*/
	bzAction = BZ_writeJes204bConfig(hw_priv,&B20Init);
    //RETURN_CHECK(bzAction)
 	
	/*Wait Write JESD204B Config To B20 device Done */
	bzAction = BZ_waitJes204bConfigDone(hw_priv,100);
    //RETURN_CHECK(bzAction)
   

    /***************************************************/
    /**** Enable  B20 JESD204B Framer ***/
    /***************************************************/

    bzAction = BZ_enableFramerLink(hw_priv, BZ_FRAMER_A, 0);
    //RETURN_CHECK(bzAction)
  
    bzAction = BZ_enableFramerLink(hw_priv, BZ_FRAMER_A, 1);
    //RETURN_CHECK(bzAction)
 	
    bzAction = BZ_enableFramerLink(hw_priv, BZ_FRAMER_B, 0);
    //RETURN_CHECK(bzAction)
   
    bzAction = BZ_enableFramerLink(hw_priv, BZ_FRAMER_B, 1);
    //RETURN_CHECK(bzAction)
  
    /***************************************************/
    /**** Enable  B20 JESD204B DeFramer ***/
    /***************************************************/
	bzAction = BZ_enableDeframerLink(hw_priv, BZ_DEFRAMER, 0);
    //RETURN_CHECK(bzAction)
 
	bzAction = BZ_enableDeframerLink(hw_priv, BZ_DEFRAMER, 1);
    //RETURN_CHECK(bzAction)
    /*************************************************/
    /**** 6.Enable SYSREF to B20 JESD204B Framer ***/
    /*************************************************/

    bzAction = BZ_enableSysRefToFramer(hw_priv, &B20Init);
    //RETURN_CHECK(bzAction)
	
    /* Function to turn radio on, Enables transmitters and receivers */
    bzAction = BZ_radioOn(hw_priv,siMode);
    //RETURN_CHECK(bzAction)
   	
    //BZ_PRINT("------------riscv log-------------------\n");
    //BZ_getRiscvLog(hw_priv);
    
    return 0; 
}

void toneoff()
{
    BZ_setDdsOff(DEVICE_HANDLE);  
    BZ_PRINT("dds off\n");
}
void getlog()
{
   BZ_getRiscvLog(&g_device); 
}
void get_pllfreq()
{
    uint64_t ulRFPLL=0,ulAUXPLL=0;
    BZ_getPllFrequency(DEVICE_HANDLE,BZ_RF_PLL,&ulRFPLL);
    BZ_getPllFrequency(DEVICE_HANDLE,BZ_AUX_PLL,&ulAUXPLL);
    BZ_PRINT("RFPLL = %lldhz,AUXPLL = %lldhz\n",ulRFPLL,ulAUXPLL);
}
void set_204b_tx(uint32_t enable)
{
    BZ_enableDeframerLink(DEVICE_HANDLE,BZ_DEFRAMER,enable);
    BZ_PRINT("set_204b_tx = %d\n",enable);
}
void set_204b_rx(uint32_t enable)
{
    BZ_enableFramerLink(DEVICE_HANDLE,BZ_FRAMER_A,enable);
    BZ_PRINT("set_204b_rx = %d\n",enable);
}
void set_204b_orx(uint32_t enable)
{
    BZ_enableFramerLink(DEVICE_HANDLE,BZ_FRAMER_B,enable);
    BZ_PRINT("set_204b_orx = %d\n",enable);
}
void sysref_control(uint32_t ONOFF)
{
    if(ONOFF == 1)
    {
        BZ_Jes204bSysrefControl(DEVICE_HANDLE,BZ_JES204B_SYSREF_ON);   
        BZ_PRINT("sysref on\n");
    }
    else if(ONOFF == 0) 
    {
        BZ_Jes204bSysrefControl(DEVICE_HANDLE,BZ_JES204B_SYSREF_OFF);   
        BZ_PRINT("sysref off\n");
    }else {
        BZ_PRINT("Undefined VALLUE \n 1. BZ_JES204B_SYSREF_ON = 1 \n 2.BZ_JES204B_SYSREF_OFF = 0 \n");
    }
}

void paprotection()
{
    BzTxPaProtectCfg_t stPaProCfg = {
        .TxCaliCnt = 1,
        .TxPowerThreshold = 100 ,
        .Enable = 1 
    };
    BZ_setPaProtectionCfg(DEVICE_HANDLE,&stPaProCfg);
}

void en_slicer(uint32_t channels,uint32_t enable)
{
    BzRxSlicerPin_t * pBzRxSlicerPin = &B20Init.rx.rxSlicerPinInfo;
    pBzRxSlicerPin->EnableSlicer = enable; 
    BZ_setRxSlicerEnable(DEVICE_HANDLE,channels,pBzRxSlicerPin);
}
void setrxgainCtrl(uint32_t ulMode,uint32_t ulCtrlWay)
{
    BZ_setRxGainControlMode(DEVICE_HANDLE,ulMode,ulCtrlWay);
}

void getrxgainCtrl()
{
    uint32_t ulMode,ulCtrlWay;
    //BZ_getRxGainControlMode(DEVICE_HANDLE,&ulMode,&ulCtrlWay);
    BZ_PRINT("还不支持! RxGain Mode = %d , CtrlWay = %d\n",ulMode,ulCtrlWay);
}

void setorxgainCtrl(uint32_t ulMode,uint32_t ulCtrlWay)
{
    BZ_setObsRxGainControlMode(DEVICE_HANDLE,ulMode,ulCtrlWay);  
}

void getorxgainCtrl()
{
    uint32_t ulMode,ulCtrlWay;
    BZ_getObsRxGainControlMode(DEVICE_HANDLE,&ulMode,&ulCtrlWay);
    BZ_PRINT("OrxGain Mode = %d , CtrlWay = %d\n",ulMode,ulCtrlWay);
}

void riscv_trace()
{
    BZ_GetRiscvTraceEvent(DEVICE_HANDLE);
}

void set_agc()
{
	BzAgcCfg_t bzAgcCfg_t;
	memset(&bzAgcCfg_t, 0, sizeof(BzAgcCfg_t));	
	bzAgcCfg_t.agcClkDiv = 2;
	bzAgcCfg_t.agcPeakWaitCounter = 0xa;
	bzAgcCfg_t.agcGainUpdateCounter = 0x3ff;
	bzAgcCfg_t.agcThreshPreventGain = 0;
	bzAgcCfg_t.agcPeakThreshGainControlMode = 0;
	bzAgcCfg_t.agcPower.powerEnableMeasurement = 1;
	bzAgcCfg_t.agcPower.powerInMux = 1;
	bzAgcCfg_t.agcPower.powerMeasurementDuration = 7;
	bzAgcCfg_t.agcPower.powerAttackHigStep = 2;
	bzAgcCfg_t.agcPower.powerAttackLowStep = 2;
	bzAgcCfg_t.agcPower.powerRecoveryHigStep = 2;
	bzAgcCfg_t.agcPower.powerRecoveryLowStep = 2;
    bzAgcCfg_t.agcPower.powerOverHigThresh = 2048; 
	bzAgcCfg_t.agcPower.powerOverLowThresh = 2048;	
	bzAgcCfg_t.agcPower.powerUnderHigThresh = 1024; 
	bzAgcCfg_t.agcPower.powerUnderLowThresh = 1024; 
	bzAgcCfg_t.agcPeak.apdEnableMeasurement = 1;               
	bzAgcCfg_t.agcPeak.apdAttackMode = 0;                		
    bzAgcCfg_t.agcPeak.apdMeasurementDuration = 0;             
    //bzAgcCfg_t.agcPeak.apdImmdOverNum;                     
    bzAgcCfg_t.agcPeak.apdPeriOverNum = 7;                     
    bzAgcCfg_t.agcPeak.apdUnderNum  =383;                       
    bzAgcCfg_t.agcPeak.apdLowTresh = 3;
    bzAgcCfg_t.agcPeak.apdHighTresh = 8;
    bzAgcCfg_t.agcPeak.apdGainStepAttack = 2;                 
    bzAgcCfg_t.agcPeak.apdGainStepRecovery = 2; 
	bzAgcCfg_t.agcPeak.hb2EnableMeasurement = 1;     
	bzAgcCfg_t.agcPeak.hb2MeasurementDuration = 0;   
	bzAgcCfg_t.agcPeak.hb2RecoveryMode = 0;         
	bzAgcCfg_t.agcPeak.hb2AttackMode = 0;            
	bzAgcCfg_t.agcPeak.hb2OverThresh = 5972;          
	//bzAgcCfg_t.agcPeak.hb2UnderRangeLowThresh = ;    
	//bzAgcCfg_t.agcPeak.hb2UnderRangeMidThresh = ;    
	bzAgcCfg_t.agcPeak.hb2UnderRangeHighThresh = 4096;    
	bzAgcCfg_t.agcPeak.hb2OverloadThreshCnt = 1;       
	bzAgcCfg_t.agcPeak.hb2UnderRangeHighThreshCnt = 255;  
	//bzAgcCfg_t.agcPeak.hb2UnderRangeMidThreshCnt = ;
	//bzAgcCfg_t.agcPeak.hb2UnderRangeLowThreshCnt = ;
	bzAgcCfg_t.agcPeak.hb2PeriExceededCnt = 7;		
	bzAgcCfg_t.agcPeak.hb2PeriUnderExceededCnt = 255;
	//bzAgcCfg_t.agcPeak.hb2ImmdExceededCnt = ;		
	//bzAgcCfg_t.agcPeak.hb2ImmdUnderHigCnt = ;		
	//bzAgcCfg_t.agcPeak.hb2ImmdUnderLowCnt = ;		
	//bzAgcCfg_t.agcPeak.hb2ImmdUnderMidCnt = ;		
	bzAgcCfg_t.agcPeak.hb2GainStepHighRecovery = 2;  
	bzAgcCfg_t.agcPeak.hb2GainStepLowRecovery = 2;   
	bzAgcCfg_t.agcPeak.hb2GainStepMidRecovery = 2;   
	bzAgcCfg_t.agcPeak.hb2GainStepAttack = 2;        
	BZ_setupRxAgc(DEVICE_HANDLE, &bzAgcCfg_t);
}
void RRUrun(uint32_t siMode)
{
    init_process();
    appclock_init();
    appclock_sync();
    sdk_example(siMode);
    end_process();
}
void InfoShow()
{
   if(gMode==2)BZ_PRINT("BZ20 run in SPI Mode.");
   if(gMode==1)BZ_PRINT("BZ20 run in PIN Mode.");
   bz_getFirmwareVersion(DEVICE_HANDLE);//固件版本 
   BZ_PRINT("|---------------PathInfoShow-------------------|\n");
   get_path();//获取通道
   BZ_PRINT("|----------------PLLInfoShow-------------------|\n");
   get_radio_state();//获取PLL状态
   get_pllfreq();
   BZ_PRINT("|----------------204InfoShow-------------------|\n");
   get_204();//204状态
   BZ_PRINT("|---------------GainInfoShow-------------------|\n");
   getgain(1);//增益
   getgain(2);
   //1.获取RX 模式AGC
   if(B20Init.rx.rxGainCtrl.rxgainMode == BZ_AGC)
    {
        BZ_PRINT("RX AGC Mode.\n");
       getrxdecpower(0x3);
    }
   else BZ_PRINT("RX MGC Mode.\n");
}
void auto_test(uint32_t ulLoglevel)
{
    FT_mainTest(1,ulLoglevel);//测试SPI模式
    FT_mainTest(2,ulLoglevel);//测试PIN模式
}

