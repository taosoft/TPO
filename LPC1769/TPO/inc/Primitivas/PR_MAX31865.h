/*
 * PR_MAX31865.h
 *
 *  Created on: Dec 13, 2018
 *      Author: matias
 */

#ifndef PRIMITIVAS_PR_MAX31865_H_
#define PRIMITIVAS_PR_MAX31865_H_

#include "RegsLPC17xx.h"
#include "DR_MAX31865.h"

#define RREF 430

int16_t getPT100Temp(int16_t *, uint8_t *);

#endif /* PRIMITIVAS_PR_MAX31865_H_ */
