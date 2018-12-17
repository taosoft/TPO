/*
 * DR_SysTick.h
 *
 *  Created on: Sep 13, 2018
 *      Author: matias
 */

#ifndef DR_SYSTICK_H_
#define DR_SYSTICK_H_

#include "RegsLPC17xx.h"

#include "DR_LCD_Display.h"
#include "DR_Teclado.h"

void InitSysTick(void);
void SysTick_Handler(void);

#define DELAY1S 1000
#define DELAY500MS 500

extern int16_t volatile flagSendTempReady;
extern int16_t volatile flagSendEstadoCalentador;
extern int16_t volatile flagUpdateLCD;
extern int16_t volatile flagUpdateLCDCalentando;

#endif /* DR_SYSTICK_H_ */
