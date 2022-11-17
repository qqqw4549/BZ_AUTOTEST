#include "bz_hal.h"
#include "bz20.h"
#include "plaxi.h"

#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

typedef struct BzHalHandle{
	bzHalDevId devid;
	int spi_file;
	int plaxi_file;
}BzHalHandle;

#define BZ20_NAME0 "/dev/bz20spi0"
#define BZ20_NAME1 "/dev/bz20spi1"
#define PLAXI_NAME "/dev/plaxi"

bzHalErr BZHAL_SpiOpen(bzHalDevId devid, bzHalHandle *handle)
{
	BzHalHandle *bzHalSpi = (BzHalHandle *)malloc(sizeof(BzHalHandle));

	if(!bzHalSpi)
		return BZHAL_SPI_FAIL;

	bzHalSpi->devid = devid;
	if(bzHalSpi->devid == BZHAL_DEVID0)
		bzHalSpi->spi_file = open(BZ20_NAME0, O_RDWR);
	else if(bzHalSpi->devid == BZHAL_DEVID1)
		bzHalSpi->spi_file = open(BZ20_NAME1, O_RDWR);
	else {
		free(bzHalSpi);
		return BZHAL_SPI_FAIL;
	}
	ioctl(bzHalSpi->spi_file, BZ20_PIN_RESET);
	bzHalSpi->plaxi_file = open(PLAXI_NAME, O_RDWR);

	if(bzHalSpi->plaxi_file < 0)
		printf("%s(%d),open %s fail\n",  __FUNCTION__, __LINE__, PLAXI_NAME);
	else
	{
		//set gpio 5,6 output direct
		uint32_t value;
		ioctl(bzHalSpi->plaxi_file, PLAXI_READ_IO_DIR_PARAM, &value);
		value |= 0x60; // 5bit = 1, 6bit = 1
		ioctl(bzHalSpi->plaxi_file, PLAXI_WRITE_IO_DIR_PARAM, &value);
	}

	*handle = bzHalSpi;
	return BZHAL_OK;
}

bzHalErr BZHAL_SpiClose(bzHalHandle handle)
{
	if(!handle)
        return BZHAL_SPI_FAIL;

    close(handle->spi_file);
	close(handle->plaxi_file);
    free(handle);

    return BZHAL_OK;
}

bzHalErr BZHAL_SpiWriteReg(bzHalHandle handle, uint16_t addr, uint32_t data)
{
	bz20_command command;
	command.low_addr = addr;
	command.data = data;
	ioctl(handle->spi_file, BZ20_WRITE_REG, &command);
	
	return BZHAL_OK;
}

bzHalErr BZHAL_SpiReadReg(bzHalHandle handle, uint16_t addr, uint32_t* data)
{
	bz20_command command;
	command.low_addr = addr;
	command.data = 0;
	ioctl(handle->spi_file, BZ20_READ_REG, &command);
	*data = command.data;

	return BZHAL_OK;
}

bzHalErr BZHAL_InterruptOn(bzHalHandle handle)
{
	ioctl(handle->spi_file, BZ20_INTERRUPT_ON);
	return BZHAL_OK;
}

bzHalErr BZHAL_TX1CaliOn(bzHalHandle handle)
{
	ioctl(handle->spi_file, BZ20_TX1_CALI_ON);
	return BZHAL_OK;
}
bzHalErr BZHAL_TX1CaliOff(bzHalHandle handle)
{
	ioctl(handle->spi_file, BZ20_TX1_CALI_OFF);
	return BZHAL_OK;
}

bzHalErr BZHAL_TX2CaliOn(bzHalHandle handle)
{
	ioctl(handle->spi_file, BZ20_TX2_CALI_ON);
	return BZHAL_OK;
}

bzHalErr BZHAL_TX2CaliOff(bzHalHandle handle)
{
	ioctl(handle->spi_file, BZ20_TX2_CALI_OFF);
	return BZHAL_OK;
}

void BZHAL_MDelay(uint32_t msecs)
{
	usleep(msecs * 1000);
}

void BZHAL_UDelay(uint32_t usecs)
{
	usleep(1000);
}

bzHalErr BZHAL_SetLogLevel(bzHalHandle handle, uint16_t halLogLevel)
{
	return BZHAL_OK;
}

bzHalErr BZHAL_WriteToLog(bzHalHandle handle, bzLogLevel_t  logLevel,
	uint32_t errorCode, const char *comment)
{
	return BZHAL_OK;
}

bzHalErr BZHAL_Memcpy(void* dst,void *src, uint32_t len)
{
	memcpy(dst, src, len);
	return BZHAL_OK;
}

bzHalErr BZHAL_Tx1EnableCtrl(bzHalHandle handle, BzHalChannelEnableMode mode)
{
	printf("%s(%d),value = %d\n",  __FUNCTION__, __LINE__, mode);	
	if(mode == BZHAL_CHANNEL_ENABLE)
	{
		ioctl(handle->spi_file, BZ20_TX1_ENABLE);
		return BZHAL_OK;
	}
    else if(mode == BZHAL_CHANNEL_DISABLE)
	{
		ioctl(handle->spi_file, BZ20_TX1_DISABLE);
		return BZHAL_OK;
	}
	return BZHAL_GPIO_FAIL;
}

bzHalErr BZHAL_Tx2EnableCtrl(bzHalHandle handle, BzHalChannelEnableMode mode)
{
	printf("%s(%d),value = %d\n",  __FUNCTION__, __LINE__, mode);
	if(mode == BZHAL_CHANNEL_ENABLE)
	{
		ioctl(handle->spi_file, BZ20_TX2_ENABLE);
		return BZHAL_OK;
	}
    else if(mode == BZHAL_CHANNEL_DISABLE)
	{
		ioctl(handle->spi_file, BZ20_TX2_DISABLE);
		return BZHAL_OK;
	}
	return BZHAL_GPIO_FAIL;
}

bzHalErr BZHAL_Rx1EnableCtrl(bzHalHandle handle, BzHalChannelEnableMode mode)
{
	printf("%s(%d),value = %d\n",  __FUNCTION__, __LINE__, mode);
	if(mode == BZHAL_CHANNEL_ENABLE)
	{
		ioctl(handle->spi_file, BZ20_RX1_ENABLE);
		return BZHAL_OK;
	}
    else if(mode == BZHAL_CHANNEL_DISABLE)
	{
		ioctl(handle->spi_file, BZ20_RX1_DISABLE);
		return BZHAL_OK;
	}
	return BZHAL_GPIO_FAIL;
}

bzHalErr BZHAL_Rx2EnableCtrl(bzHalHandle handle, BzHalChannelEnableMode mode)
{
	printf("%s(%d),value = %d\n",  __FUNCTION__, __LINE__, mode);
	if(mode == BZHAL_CHANNEL_ENABLE)
	{
		ioctl(handle->spi_file, BZ20_RX2_ENABLE);
		return BZHAL_OK;
	}
    else if(mode == BZHAL_CHANNEL_DISABLE)
	{
		ioctl(handle->spi_file, BZ20_RX2_DISABLE);
		return BZHAL_OK;
	}
	return BZHAL_GPIO_FAIL;
}

bzHalErr BZHAL_ORx1EnableCtrl(bzHalHandle handle, BzHalChannelEnableMode mode)
{
	uint32_t value;

	printf("%s(%d),value = %d\n",  __FUNCTION__, __LINE__, mode);

    ioctl(handle->plaxi_file, PLAXI_READ_IOPAD_PARAM, &value);
	if(mode == BZHAL_CHANNEL_ENABLE)
	{
		value |= (1 << 21);//21bit
		ioctl(handle->plaxi_file, PLAXI_WRITE_IOPAD_PARAM, &value);
		return BZHAL_OK;
	}
    else if(mode == BZHAL_CHANNEL_DISABLE)
	{
		value &= (~(1 << 21));//21bit
		ioctl(handle->plaxi_file, PLAXI_WRITE_IOPAD_PARAM, &value);
		return BZHAL_OK;
	}
	return BZHAL_GPIO_FAIL;
}

bzHalErr BZHAL_ORx2EnableCtrl(bzHalHandle handle, BzHalChannelEnableMode mode)
{
	uint32_t value;

	printf("%s(%d),value = %d\n",  __FUNCTION__, __LINE__, mode);

	ioctl(handle->plaxi_file, PLAXI_READ_IOPAD_PARAM, &value);
	if(mode == BZHAL_CHANNEL_ENABLE)
	{
		value |= (1 << 22);//22bit
		ioctl(handle->plaxi_file, PLAXI_WRITE_IOPAD_PARAM, &value);
		return BZHAL_OK;
	}
    else if(mode == BZHAL_CHANNEL_DISABLE)
	{
		value &= (~(1 << 22));//22bit
		ioctl(handle->plaxi_file, PLAXI_WRITE_IOPAD_PARAM, &value);
		return BZHAL_OK;
	}
	return BZHAL_GPIO_FAIL;
}