/*
 * DR_Teclado.h
 *
 *  Created on: Nov 24, 2018
 *      Author: matias
 */

#ifndef DR_TECLADO_H_
#define DR_TECLADO_H_

#include "RegsLPC17xx.h"

#define NO_KEY 0

#define SW2 2
#define SW3 3
#define SW4 4

#define TECLA_RESTA SW2
#define TECLA_OK 	SW3
#define TECLA_SUMA	SW4

#define SW2PIN	0,18
#define SW3PIN	0,11
#define SW4PIN 	2,13

#define DEBOUNCE_COUNT 30

extern uint8_t Tecla;

void Antirrebote(void);
uint8_t BarridoTeclado(void);
void InitTeclado(void);

#endif /* DR_TECLADO_H_ */
