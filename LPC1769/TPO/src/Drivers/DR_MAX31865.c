/*
 * DR_MAX31865.c
 *
 *  Created on: Dec 13, 2018
 *      Author: matias
 */

#include "DR_MAX31865.h"

void Init_MAX31685(void)
{
	SetPINSEL(CS1_PIN, PINSEL_GPIO);
	SetDIR(CS1_PIN, GPIO_OUTPUT);

	SetPINSEL(SCK1_PIN, PINSEL_GPIO);
	SetDIR(SCK1_PIN, GPIO_OUTPUT);

	SetPINSEL(MISO1_PIN, PINSEL_GPIO);
	SetPINMODE(MISO1_PIN, PINMODE_PULLDOWN);
	SetDIR(MISO1_PIN, GPIO_INPUT);

	SetPINSEL(MOSI1_PIN, PINSEL_GPIO);
	SetDIR(MOSI1_PIN, GPIO_OUTPUT);

	SetPIN(CS1_PIN, ALTO);
	SetPIN(SCK1_PIN, BAJO);
}

void ConfigMAX31865(void)
{
	uint8_t t = readRegister8(MAX31856_CONFIG_REG);
	t &= ~MAX31856_CONFIG_3WIRE;
	writeRegister8(MAX31856_CONFIG_REG, t);

	uint8_t t1 = readRegister8(MAX31856_CONFIG_REG);
	t1 &= ~MAX31856_CONFIG_BIAS;
	writeRegister8(MAX31856_CONFIG_REG, t1);

	uint8_t t2 = readRegister8(MAX31856_CONFIG_REG);
	t2 &= ~MAX31856_CONFIG_MODEAUTO;
	writeRegister8(MAX31856_CONFIG_REG, t2);

	clearFault();
}

uint8_t readFault(void)
{
	return readRegister8(MAX31856_FAULTSTAT_REG);
}

void clearFault(void)
{
	uint8_t t = readRegister8(MAX31856_CONFIG_REG);
	t &= ~0x2C;
	t |= MAX31856_CONFIG_FAULTSTAT;
	writeRegister8(MAX31856_CONFIG_REG, t);
}

uint16_t readRTD (void)
{
	clearFault();

	uint8_t t = readRegister8(MAX31856_CONFIG_REG);
	t |= MAX31856_CONFIG_BIAS;
	writeRegister8(MAX31856_CONFIG_REG, t);

	uint8_t t1 = readRegister8(MAX31856_CONFIG_REG);
	t1 |= MAX31856_CONFIG_1SHOT;
	writeRegister8(MAX31856_CONFIG_REG, t1);

	uint16_t rtd = readRegister16(MAX31856_RTDMSB_REG);

	rtd >>= 1;

	return rtd;
}

uint8_t readRegister8(uint8_t addr)
{
	uint8_t ret = 0;
	readRegisterN(addr, &ret, 1);

	return ret;
}

uint16_t readRegister16(uint8_t addr)
{
	uint8_t buffer[2] = {0, 0};
	readRegisterN(addr, buffer, 2);

	uint16_t ret = buffer[0];
	ret <<= 8;
	ret |=  buffer[1];

	return ret;
}

void readRegisterN(uint8_t addr, uint8_t buffer[], uint8_t n)
{
	SetPIN(SCK1_PIN, BAJO);
	SetPIN(CS1_PIN, BAJO);

	addr &= 0x7F;
	spixfer(addr);

	while(n--)
	{
		buffer[0] = spixfer(0xFF);
		if(n != 0)
			buffer++;
	}
	SetPIN(CS1_PIN, ALTO);
}


void writeRegister8(uint8_t addr, uint8_t data)
{
	SetPIN(SCK1_PIN, BAJO);
	SetPIN(CS1_PIN, BAJO);
	spixfer(addr | 0x80);
	spixfer(data);
	SetPIN(CS1_PIN, ALTO);
}

uint8_t spixfer(uint8_t x) {
	uint8_t reply = 0;

	for (uint8_t i=7; i>=0; i--)
	{
		reply <<= 1;
		SetPIN(SCK1_PIN, ALTO);
		SetPIN(MOSI1_PIN, x & (1<<i));
		SetPIN(SCK1_PIN, BAJO);
		if(GetPIN(MISO1_PIN))
			reply |= 1;
		if(!i)
			break;
	}

	return reply;
}


void checkFault(void)
{
    uint8_t fault = readFault();
    if (fault)
    {
      if (fault & MAX31865_FAULT_HIGHTHRESH)
      {

      }
      if (fault & MAX31865_FAULT_LOWTHRESH)
      {

      }
      if (fault & MAX31865_FAULT_REFINLOW)
      {

      }
      if (fault & MAX31865_FAULT_REFINHIGH)
      {

      }
      if (fault & MAX31865_FAULT_RTDINLOW)
      {

      }
      if (fault & MAX31865_FAULT_OVUV)
      {

      }
      clearFault();
    }
}
