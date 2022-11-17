#ifndef SI5344_H
#define SI5344_H
#include <stdint.h>

typedef struct si5344_command
{
    unsigned short addr;
    unsigned char   data;
}si5344_command;

#define SI5344_IOC_MAGIC 's'

#define SI5344_PIN_RESET _IO(SI5344_IOC_MAGIC, 1)
#define SI5344_WRITE_REG _IOW(SI5344_IOC_MAGIC, 2, int)
#define SI5344_READ_REG  _IOR(SI5344_IOC_MAGIC, 3, int)

int32_t si5344_init(void);
void    si5344_term(void);
#endif