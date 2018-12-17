/*
 * PR_Teclado.c
 *
 *  Created on: Nov 24, 2018
 *      Author: matias
 */

#include "DR_Teclado.h"
#include "DR_GPIO.h"

uint8_t Tecla = NO_KEY;

void Antirrebote(void)
{
	uint8_t key = NO_KEY;
	static uint8_t lastKey = NO_KEY;
	static uint8_t contador = 0;
	key = BarridoTeclado();

	if(key == lastKey && contador < DEBOUNCE_COUNT)
	{
		contador++;
		if(contador == DEBOUNCE_COUNT)
		{
			Tecla = key;
		}
	}
	else if(key != lastKey)
	{
		contador = 0;
	}

	lastKey = key;
}

uint8_t BarridoTeclado(void)
{
	uint8_t result = NO_KEY;

	if(!GetPIN(SW2PIN))
		return SW2;
	if(!GetPIN(SW3PIN))
		return SW3;
	if(!GetPIN(SW4PIN))
		return SW4;

	return result;
}

void InitTeclado(void)
{
	SetPINSEL(SW2PIN, PINSEL_GPIO);
	SetPINMODE(SW2PIN, PINMODE_PULLUP);
	SetDIR(SW2PIN, GPIO_INPUT);

	SetPINSEL(SW3PIN, PINSEL_GPIO);
	SetPINMODE(SW3PIN, PINMODE_PULLUP);
	SetDIR(SW3PIN, GPIO_INPUT);

	SetPINSEL(SW4PIN, PINSEL_GPIO);
	SetPINMODE(SW4PIN, PINMODE_PULLUP);
	SetDIR(SW4PIN, GPIO_INPUT);
}

