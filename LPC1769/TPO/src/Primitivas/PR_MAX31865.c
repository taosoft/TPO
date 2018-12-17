/*
 * PR_MAX31865.c
 *
 *  Created on: Dec 13, 2018
 *      Author: matias
 */

#include "PR_MAX31865.h"

int16_t getPT100Temp(int16_t *entero, uint8_t *decimal)
{
	uint16_t rtd = 0, ohmsx100 = 0;
	uint32_t dummy = 0;
	int16_t temperaturaTabla = 0;

	rtd = readRTD();

	dummy = ((uint32_t)(rtd << 1)) * 100 * ((uint32_t) RREF);
	dummy >>= 16;
	ohmsx100 = (uint16_t) (dummy & 0xFFFF);

	temperaturaTabla = celsius(ohmsx100);

	*decimal = temperaturaTabla % 10;
	*entero = temperaturaTabla / 10;

	checkFault();

	return temperaturaTabla;
}
