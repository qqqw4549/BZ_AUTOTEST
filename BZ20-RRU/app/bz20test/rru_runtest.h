#ifndef RRU_RUN_TEST_H
#define RRU_RUN_TEST_H
#include "bz_data_types.h"
static void set_jest204b();
static void cals(uint32_t mask);
void getcals();
void run_rru(int siMode);
void set_txgain(uint32_t channels ,uint8_t indexTx );
void set_rxgain(uint32_t channels,uint8_t indexRx );
void get_gain(uint32_t channels);
void get_radio();
void setpath(int siRX,int siORX,int siTX);
void readreg(uint16_t addr);
void readregbit(uint16_t addr,uint32_t endbit,uint32_t startbit);
void writereg(uint16_t addr,uint32_t data);
void writeregbit(uint16_t addr,uint32_t endbit,uint32_t startbit,uint32_t data);
void path();
void get204();
void orxdata(uint32_t points, uint32_t channels);
void rxdata(uint32_t points, uint32_t channels);
static void set_freq(uint64_t rf_freq_hz, uint64_t aux_freq_hz);
static void init_device(uint32_t device_id);
void fpgasend();
void getrxpower(uint32_t channels);
void log_riscv();
void boardInfoShow();
void rru_run_test(void);
#endif
