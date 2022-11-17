#include "testcase.h"
#include "bz_talise.h"

uint32_t gulLogLevel = 3;
void FT_mainTest(uint32_t ulMode,uint32_t ulLogLevel)
{
    char *str[] = {"auto_test_spi_mode","auto_test_pin_mode"};
    uint32_t gulLogLevel_bak = gulLogLevel; 
    gulLogLevel = ulLogLevel;
    //1.测试bz20 pinmode
    printf("\n|***************************************************\n");
    printf("----------------%s------------------\n",str[ulMode-1]);
    printf("|***************************************************\n");
    //RRUrun(mode);
    
    //1.测试SPI读写 
    FT_SpiTest(0x1CE);
    
    //close_device();
    gulLogLevel = gulLogLevel_bak;
}

uint32_t FT_Rand(uint32_t start, uint32_t end)
{
    return (rand() % start + end);
}

void FT_SpiTest(uint32_t Addr)
{
    uint32_t i=0, ulVal=0, ulRET=0,ulTestCase = 100;

    for(i = 0; i < ulTestCase; i++)
    {
        ulVal = FT_Rand(0xFFFF,0);
        uint32_t ulTmp = 0;
        BZ_DBGPRINT("write_reg:%d",ulVal);
        write_reg(Addr, ulVal);
        ulTmp = read_reg(Addr);
        BZ_DBGPRINT("read_reg:%d",ulTmp);
        if(ulVal != ulTmp)
        {
            BZ_ERRPRINT("write_reg:%d,read_reg:%d",ulVal,ulTmp);
            ulRET++;
        }
        usleep(1000);
    }
    if(ulRET>0)
    {
        BZ_LOGPRINT("TestCase:%d ErrCNT:%d Error",ulTestCase,ulRET);
    }
    else {
        BZ_LOGPRINT("TestCase:%d ErrCNT:%d Ok",ulTestCase,ulRET);
    }
}
