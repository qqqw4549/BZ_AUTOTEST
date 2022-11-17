#include "spitest.h"
#include "bz_hal.h"
#include "command.h"

#define IP_SRAM_CTL_REG  0x97
#define REG_DEFAULT_VAL  0x42
#define REG_NEW_VAL      0x02

#define CHECK_HANDLE(handle)              \
    if(handle)                            \
    {                                     \
        BZHAL_Printf("spi had opened!\n");\
        return;                           \
    }

#define CHECK_HANDLE_IS_NULL(handle)      \
    if(!handle)                           \
    {                                     \
        BZHAL_Printf("spi had opened!\n");\
        return;                           \
    }

static bzHalHandle g_handle = NULL;

static void spi_open(void)
{
    CHECK_HANDLE(g_handle)

    if(BZHAL_SpiOpen(0, &g_handle) == BZHAL_OK)
        BZHAL_Printf("spi open ok!\n");
    else
        BZHAL_Printf("spi open fail!\n");
}

static void spi_close(void)
{
    CHECK_HANDLE_IS_NULL(g_handle)

    if(BZHAL_SpiClose(g_handle) != BZHAL_OK)
        BZHAL_Printf("spi close fail!\n");
    else
    {
        g_handle = NULL;
        BZHAL_Printf("spi close ok!\n");
    }
}

void spi_read(void)
{
    CHECK_HANDLE_IS_NULL(g_handle)

    uint32_t data;

    if(BZHAL_SpiReadReg(g_handle, IP_SRAM_CTL_REG, &data) != BZHAL_OK)
        BZHAL_Printf("spi read fail!\n");
    else
    {
        BZHAL_Printf("spi read val: 0x%08x\n", data);
        if(data == REG_DEFAULT_VAL)
            BZHAL_Printf("spi read is ok\n");
    }   
}

void spi_write(void)
{
    CHECK_HANDLE_IS_NULL(g_handle)

    uint32_t data = REG_NEW_VAL;

    if(BZHAL_SpiWriteReg(g_handle, IP_SRAM_CTL_REG, data) != BZHAL_OK)
        BZHAL_Printf("spi write fail!\n");
    else
    {
        data = 0;
        BZHAL_SpiReadReg(g_handle, IP_SRAM_CTL_REG, &data);
        BZHAL_Printf("spi write val: 0x%08x\n", data);
        if(data == REG_NEW_VAL)
            BZHAL_Printf("spi write is ok\n");
    }   
}

void spi_test(void)
{
    RegisterCommand("spiopen", "open spi device", (void *)spi_open);
    RegisterCommand("spiclose", "close spi device", (void *)spi_close);
    RegisterCommand("spiread", "read by spi from device", (void *)spi_read);\
    RegisterCommand("spiwrite", "write by spi from device", (void *)spi_write);
}