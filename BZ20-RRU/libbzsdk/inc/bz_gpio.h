/**
* \file bz_gpio.h
* \brief B20 GPIO header file
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zealync Inc.
*/

#ifndef BZ_GPIO_H_
#define BZ_GPIO_H_

#include "bz_gpio_types.h"
#include "bz_types.h"

/**
 * \brief Sets the bz20 GPIO output value
 * This function will set the gpio value given by the passed parameter,
 * The gpio output can be set to high or low
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param gpioNum (valid range 0-9),This is the will use gpionum
 * \param Value (valid range 0-1),1 means high,ohterwise 0 means low.
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t  BZ_setGpioValue(BzDevice_t *hw_priv, BzGpioPinSel_t gpioNum, uint32_t Value);
/**
 * \brief Sets the bz20 GPIO output value
 * This function will get the gpio value given by the passed parameter,
 * The gpio output can be get to high or low
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param gpioNum (valid range 0-9),This is the will use gpionum
 * \param Value (valid range 0-1),1 means high,ohterwise 0 means low.
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t  BZ_getGpioValue(BzDevice_t *hw_priv, BzGpioPinSel_t gpioNum, uint32_t *value);
/**
 * \brief Sets the bz20 low voltage GPIO pin directions
 * This function will set the gpio direction given by the passed parameter,
 * The direction can be set output or input per pin
 *
 * \param device Pointer to the B20 data structure containing settings
 * \param gpioOutEn (valid range 0-1),1 means out,ohterwise 0 means input.
 * \param gpioNum (valid range 2-19),This is the will use gpionum
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
 uint32_t  BZ_setGpioDirection(BzDevice_t *hw_priv, BzGpioDir_t gpioOutEn, BzGpioPinSel_t gpioNum);
 /**
  * \brief Gets the bz20 low voltage GPIO pin directions
  * This function will set the gpio direction given by the passed parameter,
  * The direction can be get output or input per pin num
  *
  * \param device Pointer to the B20 data structure containing settings
  * \param gpioOutEn (valid range 0-1),1 means out,ohterwise 0 means input.
  * \param gpioNum (valid range 2-19),This is the will use gpionum
  * \retval BZHAL_OK means function completed successfully, otherwise it will fail
  */
uint32_t  BZ_getGpioDirection(BzDevice_t *hw_priv, BzGpioDir_t *gpioOutEn, BzGpioPinSel_t gpioNum);
 /**
  * \brief Sets the bz20 low voltage GPIO pin control by spi or riscv
  * This function will set the gpio source given by the passed parameter,
  * The direction can be set spi or riscv per pin
  *
  * \param device Pointer to the B20 data structure containing settings
  * \param gpioCtrl (valid range 0-1),1 means the spi is controled by spi,ohterwise 0 means controled by riscv.
  * \param gpioNum (valid range 2-19),This is the will use gpionum
  * \retval BZHAL_OK means function completed successfully, otherwise it will fail
  */
uint32_t  BZ_setGpioSourceCtrl(BzDevice_t *hw_priv, BzGpioCtrlMode_t gpioCtrl, BzGpioPinSel_t gpioNum);
 /**
  * \brief Gets the bz20 low voltage GPIO pin control by spi or riscv
  * This function will set the gpio source given by the passed parameter,
  * The ctrl ways can be get by parmameter gpionum
  *
  * \param device Pointer to the B20 data structure containing settings
  * \param gpioCtrl (valid range 0-1),1 means the spi is controled by spi,ohterwise 0 means controled by riscv.
  * \param gpioNum (valid range 2-19),This is the will use gpionum
  * \retval BZHAL_OK means function completed successfully, otherwise it will fail
  */
uint32_t  BZ_getGpioSourceCtrl(BzDevice_t *hw_priv, BzGpioCtrlMode_t *gpioCtrl, BzGpioPinSel_t gpioNum);
 /**
  * \brief Sets the bz20 low voltage GPIO pin functionality
  * This function will set the gpio source given by the passed parameter,
  * The function can be set to gpionum
  *
  * \param device Pointer to the B20 data structure containing settings
  * \param gpioFunc (valid range 0-7).the function can be know from gpio userguaid
  * \param gpioNum (valid range 2-19),This is the will use gpionum
  	
  * \retval BZHAL_OK means function completed successfully, otherwise it will fail
  */
uint32_t  BZ_setGpioFuncSel(BzDevice_t *hw_priv, BzGpioPinFunc_t gpioFunc, BzGpioPinSel_t gpioNum);
 /**
  * \brief Gets the bz20 low voltage GPIO pin functionality
  * This function will set the gpio source given by the passed parameter,
  * The function can be set to gpionum
  *
  * \param device Pointer to the B20 data structure containing settings
  * \param gpioFunc (valid range 0-7).the function can be know from gpio userguaid
  * \param gpioNum (valid range 2-19),This is the will use gpionum
  * \retval BZHAL_OK means function completed successfully, otherwise it will fail
  */
uint32_t  BZ_getGpioFuncSel(BzDevice_t *hw_priv, BzGpioPinFunc_t *gpioFunc, BzGpioPinSel_t gpioNum);
 /**
  * \brief Sets the bz20 3v3 GPIO pin directions to output
  * This function will set the 3v3 gpio direction given by the passed parameter,
  * The direction can be set output or input per pin
  *
  * \param device Pointer to the B20 data structure containing settings
  * \param gpio3v3OutEn (valid range 0-1),1 means out,ohterwise 0 means input.
  * \param gpioNum (valid range 0-11),This is the will use gpionum
  * \retval BZHAL_OK means function completed successfully, otherwise it will fail
  */
uint32_t  BZ_setGpio3v3Oe(BzDevice_t *hw_priv, uint32_t gpio3v3OutEn, BzGpioPinSel_t gpioNum);
 /**
  * \brief Gets the bz20 outpur 3v3 GPIO pins
  * This function will get the output 3v3 gpio pins given by the passed parameter,
  *
  * \param device Pointer to the B20 data structure containing settings
  * \param gpio3v3OutEn (valid range 0-1),1 means out,ohterwise 0 means input.
  * \retval BZHAL_OK means function completed successfully, otherwise it will fail
  */
uint32_t  BZ_getGpio3v3Oe(BzDevice_t *hw_priv, uint32_t *gpio3v3OutEn);
 /**
  * \brief Sets the bz20 3v3 GPIO pin directions
  * \brief Sets the bz20 3v3 GPIO pin directions to input
  * The direction can be set output or input per pin
  *
  * \param device Pointer to the B20 data structure containing settings
  * \param gpio3v3InEn (valid range 0-1),1 means out,ohterwise 0 means input.
  * \param gpioNum (valid range 0-11),This is the will use gpionum
  * \retval BZHAL_OK means function completed successfully, otherwise it will fail
  */
uint32_t  BZ_setGpio3v3Ie(BzDevice_t *hw_priv, uint32_t gpio3v3InEn, BzGpioPinSel_t gpioNum);
 /**
  * \brief Gets the bz20 outpur 3v3 GPIO pins
  * This function will get the input 3v3 gpio pins given by the passed parameter,
  *
  * \param device Pointer to the B20 data structure containing settings
  * \param gpio3v3OutEn (valid range 0-1),1 means out,ohterwise 0 means input.
  * \retval BZHAL_OK means function completed successfully, otherwise it will fail
  */
uint32_t  BZ_getGpio3v3Ie(BzDevice_t *hw_priv, uint32_t *gpio3v3InEn);
 /**
  * \brief Sets up the 12 AuxDACs on the bz20 device.
  * function parameter to setup each of the (0-11) DACs
  * After calling this setup function, it is possible to 
  * change a specific DAC code by calling the BZ_setupAuxDacs() function.
  *
  * The AuxDAC outputs share the GPIO 3.3v pins.  When using an AuxDAC on a particular GPIO pin,
  * ensure that GPIO pin is set to be an input to tri-state the GPIO pad driver.
  *
  * AuxDac[n]	|  GPIO3.3v Pin[m]
  * ------------|-------------------
  *  AuxDac[0]	| GPIO3p3v[6]
  *  AuxDac[1]	| GPIO3p3v[7]
  *  AuxDac[2]	| GPIO3p3v[8]
  *  AuxDac[3]	| GPIO3p3v[9]
  *  AuxDac[4]	| GPIO3p3v[10]
  *  AuxDac[5]	| GPIO3p3v[11]
  *  AuxDac[6]	| GPIO3p3v[0]
  *  AuxDac[7]	| GPIO3p3v[1]
  *  AuxDac[8]	| GPIO3p3v[2]
  *  AuxDac[9]	| GPIO3p3v[3]
  *  AuxDac[10] | GPIO3p3v[4]
  *  AuxDac[11] | GPIO3p3v[5]
  *
  * \param device Pointer to the B20 data structure containing settings
  * \param Gpio3V3Num (valid range 0-11)
  *
  * \retval BZHAL_OK means function completed successfully, otherwise it will fail
  */
uint32_t BZ_setupAuxDacs(BzDevice_t *hw_priv,BzGpioPinSel_t Gpio3V3Num);
 /**
  * \brief Writes the current auxiliary DAC code for a specific auxiliary DAC
  *
  * \param device Pointer to the B20 data structure containing settings
  * \param auxDacIndex Select the desired DAC to load the auxDacCode for DACs 0-11. Where, values 0-9 correspond to the
  * the ten 10-bit DACs, and 10-11 corresponds to the two 12-bit DACs
  * \param auxDacCode DAC code to write to the selected DAC. Sets the output voltage of the DAC (valid 0-1023 for auxDacIndex 0-9)
  *
  * \retval BZHAL_OK means function completed successfully, otherwise it will fail
  */
 uint32_t BZ_writeAuxDac(BzDevice_t *hw_priv,BzGpioPinSel_t Gpio3V3Num,uint16_t auxDacCode);
 /**
  * \brief Configures one of the four 12-bit AuxADC on the bz20 device for sampling and A/D conversion.
  * AuxADC on bz20 is also used by the temparature sensor. This API configures bz20 to allow external
  * use of Aux ADC.
  *
  * This function uses the configuration in the BzAuxAdcConfig_t data structure
  * function parameter to setup the AuxADC channel to be used for external A/D conversion use.
  *
  * The Pin-mode AuxADC conversion expects a low(0) to high(1) pulse on the GPIO pin assigned to Aux ADC
  * start signal via auxAdcConfig->auxadcGpioSel parameter to schedule A/D Conversion.A rising edge on this pin is needed for
  * each AuxADC measurement up to the number of AuxADC samples requested. The rising edges will be ignored during
  * a temperature sensor measurement.
  *
  * When using a particular 1.8v GPIO pin for AuxADC Pin-mode, ensure that the GPIO pin selected is not in
  * use by another feature.
  *
  * NOTE: Temparature sensor measurements are prioritized over external AuxADC use.
  *
  * NOTE: The pulse on the GPIO pin is expected to be held high for at least 1 clock cycle duration.
  *
  * \param device Pointer to the B20 data structure containing settings
  * \param auxdac structure pointer 
  * \retval BZHAL_OK means function completed successfully, otherwise it will fail
  */
uint32_t BZ_startAuxAdc(BzDevice_t *hw_priv,BzAuxAdcConfig_t *auxAdcConfig);
 /**
  * \brief Reads the AuxADC conversion result that was scheduled prior to calling this API.
  *
  * This function reads the A/D conversion result And updates the auxAdcResult data structure.
  *
  * The auxAdcResult is valid depending on the complete indicate field.
  *
  * 	auxAdcResult->	   |
  * 	completeIndicator  | Field Description
  * 	-------------------|-------------------------------
  * 					0  | AuxADC Conversion not complete
  * 					1  | AuxADC Conversion complete
  *
  * \param device Pointer to the B20 data structure containing settings
  * \param auxAdcResult Pointer to the BzAuxAdcConfig_t data structure where the result
  * 	   of aux ADC A/D conversion is written to.
  *
  * \retval BZHAL_OK means function completed successfully, otherwise it will fail
  */
uint32_t BZ_readAuxAdc(BzDevice_t *hw_priv,BzAuxAdcResult_t *auxAdcConfig);
 /**
  * \brief This API function configures the monitor output function for the GPIOs
  *
  * The monitor outputs allow visibility to some internal  signals.  Each
  * monitor index outputs a set of eight signals.  To output these signals on
  * the low voltage GPIO[18:0] pins, first set the desired GPIO[18:0] pin
  * direction, then set the GPIO nibble source control to allow the monitor
  * signals to route to a set of 4 GPIO pins.  If GPIO[18:16] nibble source is
  * set to monitor outs, monitor out[6:4] signals are routed to GPIO[18:16].
  * When the nibble source is set to monitor out for GPIO[15:0], monitorOut[7:0]
  * is routed to GPIO[7:0] and monitorOut[7:0] is also routed to GPIO[15:8].
  
  * \param device Pointer to the B20 data structure containing settings
  * \param monitorIndex Used to select what signal is used as the monitor signal
  * \param monitor_ena The index that outputs a set of 8 monitor outputs. See
  * 		 table in documentation for which signals output on each GPIO pins
  * \param monitor_pos_sel_type Used to select the position of 8-bit monitor signal on GPIO. 

  * \retval BZHAL_OK means function completed successfully, otherwise it will fail
  */
uint32_t BZ_setGpioMonitorOut(BzDevice_t *hw_priv,uint32_t monitorIndex,uint32_t monitor_ena,BzGpioMonitorPosSel_t monitor_pos_sel_type);

 /**
  * \brief This API function reads the GPIO monitor index and mask from bz20
  *
  * \param device Pointer to the B20 data structure containing settings
  * \param monitorIndex Pointer to single uint8_t variable which returns the
  * 					current monitor signal selection index
  *
  * \param monitorEna Pointer to single uint8_t variable which returns the
  * 				   monitor out signal masking.	Bit per monitor out [7:0]
  * 				   signal.
  *
  * \retval BZHAL_OK means function completed successfully, otherwise it will fail
  */
uint32_t BZ_getGpioMonitorOut(BzDevice_t *hw_priv,uint32_t *monitorIndex,uint32_t* monitorEna);
/**
 * \brief 
 *
 * \param device Pointer to the B20 data structure containing settings
 *
 * \retval BZHAL_OK means function completed successfully, otherwise it will fail
 */
	uint32_t BZ_getGpIntStatus(BzDevice_t *hw_priv,BzGpIntMask_t *gpIntStatus);
#endif

