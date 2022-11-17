#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "si5344.h"
#include "Si5344-RevD-Registers.h"

#define SI5344_NAME "/dev/si5344spi0"

static int spi_file;
int32_t reg_write_spi(uint16_t regAddr, uint8_t value)
{
	si5344_command command;
	command.addr = regAddr;
	command.data = value;
	
	ioctl(spi_file, SI5344_WRITE_REG, &command);
	return 0;
}

uint8_t reg_read_spi(uint16_t regAddr)
{
	si5344_command command;
	command.addr = regAddr;
	command.data = 0;
	ioctl(spi_file, SI5344_READ_REG, &command);

	return command.data;
}

int32_t si5344_init(void)
{
	spi_file = open(SI5344_NAME, O_RDWR);
	if(!spi_file)
	{
		printf("cannot open file: %s\n", SI5344_NAME);
        return -1;
	}
	ioctl(spi_file, SI5344_PIN_RESET);
	int32_t ret =  reg_read_spi(0x03);
	if(ret != 0x53)
	{
		printf("%s spi read error,addr=[%02X] value[0x53]=[%02x]\n",__func__,0x03,ret);
	}
	ret =  reg_read_spi(0x02);
	if(ret != 0x44)
	{
		printf("%s spi read error,addr=[%02X] value[0x53]=[%02x]\n",__func__,0x03,ret);
	}
	{
		uint16_t m;
		uint8_t n = 3;
		if(n > 0)      //repeat 3 times
		{
			ret=reg_write_spi(si5344_revd_registers[0].address,si5344_revd_registers[0].value);
			ret=reg_write_spi(si5344_revd_registers[1].address,si5344_revd_registers[1].value);
			ret=reg_write_spi(si5344_revd_registers[2].address,si5344_revd_registers[2].value);
			usleep(2000 * 1000); //wait 800ms

			for(m = 3; m < SI5344_REVD_REG_CONFIG_NUM_REGS; m++)
			{
				ret=reg_write_spi(si5344_revd_registers[m].address, si5344_revd_registers[m].value);
			}
			n--;
		}
	}
	return 0;
}

void si5344_term(void)
{
	if(spi_file > 0)
		close(spi_file);
}

