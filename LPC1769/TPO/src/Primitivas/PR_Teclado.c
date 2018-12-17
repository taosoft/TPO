/*
 * PR_Teclado.c
 *
 *  Created on: Nov 24, 2018
 *      Author: matias
 */

#include "RegsLPC17xx.h"
#include "DR_Teclado.h"
#include "PR_Teclado.h"

uint8_t GetKey(void)
{
	uint8_t aux;
	aux = Tecla;
	Tecla = NO_KEY;
	return aux;
}
