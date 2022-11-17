#ifndef BZ20_H
#define BZ20_H

typedef struct bz20_command
{
    unsigned short low_addr;
    unsigned short high_addr;
    unsigned int   data;
}bz20_command;

#define BZ20_IOC_MAGIC 'b'

#define BZ20_PIN_RESET    _IO(BZ20_IOC_MAGIC, 1)
#define BZ20_WRITE_REG    _IOW(BZ20_IOC_MAGIC, 2, long)
#define BZ20_READ_REG     _IOR(BZ20_IOC_MAGIC, 3, long)
#define BZ20_TX1_ENABLE   _IO(BZ20_IOC_MAGIC, 4)
#define BZ20_TX1_DISABLE  _IO(BZ20_IOC_MAGIC, 5)
#define BZ20_TX2_ENABLE   _IO(BZ20_IOC_MAGIC, 6)
#define BZ20_TX2_DISABLE  _IO(BZ20_IOC_MAGIC, 7)
#define BZ20_RX1_ENABLE   _IO(BZ20_IOC_MAGIC, 8)
#define BZ20_RX1_DISABLE  _IO(BZ20_IOC_MAGIC, 9)
#define BZ20_RX2_ENABLE   _IO(BZ20_IOC_MAGIC, 10)
#define BZ20_RX2_DISABLE  _IO(BZ20_IOC_MAGIC, 11)
#define BZ20_INTERRUPT_ON _IO(BZ20_IOC_MAGIC, 12)
#define BZ20_TX1_CALI_ON  _IO(BZ20_IOC_MAGIC, 13)
#define BZ20_TX1_CALI_OFF _IO(BZ20_IOC_MAGIC, 14)
#define BZ20_TX2_CALI_ON  _IO(BZ20_IOC_MAGIC, 15)
#define BZ20_TX2_CALI_OFF _IO(BZ20_IOC_MAGIC, 16)

#endif