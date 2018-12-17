/*
 * DR_MAX31865.h
 *
 *  Created on: Dec 13, 2018
 *      Author: matias
 */

#ifndef DRIVERS_DR_MAX31865_H_
#define DRIVERS_DR_MAX31865_H_

#include "DR_GPIO.h"
#include "pt100.h"

#define CS1_PIN 	1,27 //Expansion 12
#define SCK1_PIN	1,24 //Expansion 13
#define MISO1_PIN	1,21 //Expansion 14
#define MOSI1_PIN	1,18 //Expansion 15

#define ALTO	1
#define BAJO 	0

#define MAX31856_CONFIG_REG            	0x00
#define MAX31856_CONFIG_BIAS           	0x80
#define MAX31856_CONFIG_MODEAUTO       	0x40
#define MAX31856_CONFIG_MODEOFF        	0x00
#define MAX31856_CONFIG_1SHOT          	0x20
#define MAX31856_CONFIG_3WIRE          	0x10
#define MAX31856_CONFIG_24WIRE         	0x00
#define MAX31856_CONFIG_FAULTSTAT      	0x02
#define MAX31856_CONFIG_FILT50HZ       	0x01
#define MAX31856_CONFIG_FILT60HZ       	0x00

#define MAX31856_RTDMSB_REG				0x01
#define MAX31856_RTDLSB_REG           	0x02
#define MAX31856_HFAULTMSB_REG        	0x03
#define MAX31856_HFAULTLSB_REG        	0x04
#define MAX31856_LFAULTMSB_REG        	0x05
#define MAX31856_LFAULTLSB_REG        	0x06
#define MAX31856_FAULTSTAT_REG        	0x07

#define MAX31865_FAULT_HIGHTHRESH     	0x80
#define MAX31865_FAULT_LOWTHRESH      	0x40
#define MAX31865_FAULT_REFINLOW       	0x20
#define MAX31865_FAULT_REFINHIGH      	0x10
#define MAX31865_FAULT_RTDINLOW       	0x08
#define MAX31865_FAULT_OVUV           	0x04

void Init_MAX31685(void);
void ConfigMAX31865(void);

uint16_t readRTD(void);
void checkFault(void);

uint8_t readFault(void);
void clearFault(void);

void readRegisterN(uint8_t, uint8_t *, uint8_t);
uint8_t readRegister8(uint8_t);
uint16_t readRegister16(uint8_t);
void writeRegister8(uint8_t, uint8_t);
uint8_t spixfer(uint8_t);

#endif /* DRIVERS_DR_MAX31865_H_ */
