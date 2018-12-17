/*
 * DR_Relay.c
 *
 *  Created on: Dec 13, 2018
 *      Author: matias
 */

#include "DR_Relay.h"

void Init_Rele(void)
{
	SetPINSEL(RELE_PIN, PINSEL_GPIO);
	SetDIR(RELE_PIN, GPIO_OUTPUT);

	SetPIN(RELE_PIN, APAGADO);
}
