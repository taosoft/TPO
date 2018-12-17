/*
 * PR_Relay.c
 *
 *  Created on: Dec 13, 2018
 *      Author: matias
 */

#include "PR_Relay.h"

void SetRelay(uint8_t estado)
{
	SetPIN(RELE_PIN, estado);
}

uint8_t GetRelay(void)
{
	return GetPIN(RELE_PIN);
}
