#ifndef PLAXI_H
#define PLAXI_H

#define PLAXI_IOC_MAGIC 'b'

#define PLAXI_CHIP_SEL            _IOW(PLAXI_IOC_MAGIC, 15, int) //chip select 0-bz20_1,1- bz20_2
#define PLAXI_SET_LANPA_PWR       _IOW(PLAXI_IOC_MAGIC, 14, int) //set lan pa power on/off
#define PLAXI_SET_TRIGGER_OUT     _IOW(PLAXI_IOC_MAGIC, 13, int) //set trigger out  on/off
#define PLAXI_FPGA_VERSION        _IOR(PLAXI_IOC_MAGIC, 12, int) //io_0ffset 6
#define PLAXI_READ_204B_STATE     _IOR(PLAXI_IOC_MAGIC, 11, int) //io_0ffset 14
#define PLAXI_READ_IOPAD_PARAM    _IOR(PLAXI_IOC_MAGIC, 10, int) //io_0ffset 5/8
#define PLAXI_WRITE_IOPAD_PARAM   _IOW(PLAXI_IOC_MAGIC, 9, int)  //io_0ffset 5/8
#define PLAXI_READ_IO_DIR_PARAM   _IOR(PLAXI_IOC_MAGIC, 8, int)  //io_0ffset 4
#define PLAXI_WRITE_IO_DIR_PARAM  _IOW(PLAXI_IOC_MAGIC, 7, int)  //io_0ffset 4
#define PLAXI_READ_204B_PARAM     _IOR(PLAXI_IOC_MAGIC, 6, int)  //io_0ffset 3/7
#define PLAXI_WRITE_204B_PARAM    _IOW(PLAXI_IOC_MAGIC, 5, int)  //io_0ffset 3/7
#define PLAXI_READ_TX_DATA_PARAM  _IOR(PLAXI_IOC_MAGIC, 4, int)  //io_0ffset 2
#define PLAXI_WRITE_TX_DATA_PARAM _IOW(PLAXI_IOC_MAGIC, 3, int)  //io_0ffset 2
#define PLAXI_READ_RX_DATA_PARAM  _IOR(PLAXI_IOC_MAGIC, 2, int)  //io_0ffset 1
#define PLAXI_WRITE_RX_DATA_PARAM _IOR(PLAXI_IOC_MAGIC, 1, int)  //io_0ffset 1

#endif