/*
 * DR_SD.c
 *
 *  Created on: Dec 13, 2018
 *      Author: matias
 */

#include "DR_SD.h"

void Init_SD(void)
{
	PCONP |= (0x01 << 8); // Alimento SPI (Ya estaría hecho por reset value)
	PCLKSEL0 |= (0x03 << 16); // Configuro el clock a 12,5 MHz
	PINSEL0 &= ~(0x03 << 12); // Configuro los pines P0.6 a P0.9
	PINSEL0 |= (0x02 << 12);
	PINSEL0 &= ~(0x03 << 14);
	PINSEL0 |= (0x02 << 14);
	PINSEL0 &= ~(0x03 << 16);
	PINSEL0 |= (0x02 << 16);
	PINSEL0 &= ~(0x03 << 18);
	PINSEL0 |= (0x02 << 18);
	S0SPCR |= (0x01 << 2); // Selecciono para enviar una cantidad de bits mayor a 8
	S0SPCR |= (0x01 << 5); // Selecciono master
	S0SPCR |= (0x01 << 6); // Transmito primero los bits menos significativos
	S0SPCR |= (0x01 << 7); // Habilito interrupciones del SPI
	S0SPCR &= ~(0x0F << 8); // Selecciono 16 bits de transferencia
	S0SPCCR |= (0x08); // Master debe ser valor par mayor o igual a 8
	ISER0 |= (0x01 << 13); // Habilito NVIC
}
