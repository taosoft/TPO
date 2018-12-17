/**
 	\file DR_RTC.c
 	\brief Drivers del RTC
 	\details Manejo de interrupciones por incremento y alarma
 	\author Pablo Irrera Condines
 	\date 2018.09.27
*/

#include <stdio.h>
#include "DR_RTC.h"

static RTC_InterruptCallback g_incrementCallback = NULL;
static RTC_InterruptCallback g_alarmCallback = NULL;

void RTC_Init(void)
{
	ILR = 0x00;
	CIIR = 0x00;
	AMR = 0xFF;
	CALIBRATION = 0x00;
	CCR = 0x01;
}

void RTC_EnableIncrementInterrupt(uint32_t intType)
{
	CIIR |= intType;	//se habilita el tipo de interrupcion pedido
	ILR |= 0x01;			//limpio flag de interrupcion
	ISER0 |= 1<<17;		//habilito interrupcion desde el NVIC
}

void RTC_EnableAlarmInterrupt(uint32_t intType)
{
	intType = ~intType;
	AMR &= intType;		//se desenmascara el bit de alarma pedido
	ILR |= 0x02;			//limpio flag de interrupcion
	ISER0 |= 1<<17;		//habilito interrupcion desde el NVIC
}
void RTC_SetIncrementCallback(RTC_InterruptCallback callback)
{
	g_incrementCallback = callback;
}

void RTC_SetAlarmCallback(RTC_InterruptCallback callback)
{
	g_alarmCallback = callback;
}

void RTC_IRQHandler(void)
{
	if(ILR & (1<<0) && g_incrementCallback)		//Interrupcion por counter increment
		g_incrementCallback();
	if(ILR & (1<<1) && g_alarmCallback)			//Interrupcion por alarma
		g_alarmCallback();
	ILR = ILR;				//limpieza flags de interrupcion
}
