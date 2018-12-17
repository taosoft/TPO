/*
 * DR_SysTick.c
 *
 *  Created on: Sep 13, 2018
 *      Author: matias
 */
#include "DR_SysTick.h"

int16_t volatile flagSendTempReady = 0;
int16_t volatile flagSendEstadoCalentador = 0;
int16_t volatile flagUpdateLCD = 0;
int16_t volatile flagUpdateLCDCalentando = 1;

void InitSysTick(void) //1 mS
{
	STRELOAD = 0x1869F;
	STCURR = 0;
	STCTRL = 0x07;
}

void SysTick_Handler(void)
{
	static int16_t tempDelay = DELAY1S;
	static int16_t statusCalentadorDelay = DELAY500MS;
	static int16_t LCD_Delay = DELAY500MS;
	static int16_t LCD_Calentando_Delay = 0;

	//LCD
	if(lcdDelay > 0)
		lcdDelay--;
	LCD_Barrido();

	//Teclado
	Antirrebote();

	//Flag Mde Temperatura
	if(tempDelay > 0 && !flagSendTempReady)
	{
		tempDelay--;
		if(tempDelay == 0)
		{
			flagSendTempReady = 1;
			tempDelay = DELAY1S;
		}
	}

	//Flag Mde Estado Calentador
	if(statusCalentadorDelay > 0 && !flagSendEstadoCalentador)
	{
		statusCalentadorDelay--;
		if(statusCalentadorDelay == 0)
		{
			flagSendEstadoCalentador = 1;
			statusCalentadorDelay = DELAY1S;
		}
	}

	//Flag Mde Update LCD
	if(LCD_Delay > 0 && !flagUpdateLCD)
	{
		LCD_Delay--;
		if(LCD_Delay == 0)
		{
			flagUpdateLCD = 1;
			LCD_Delay = DELAY500MS;
		}
	}

	//Flag Mde Update LCD Estado Calentando
	if(flagUpdateLCDCalentando)
		LCD_Calentando_Delay = DELAY1S*2;
	if(LCD_Calentando_Delay > 0 && !flagUpdateLCDCalentando)
	{
		LCD_Calentando_Delay--;
		if(LCD_Calentando_Delay == 0)
		{
			flagUpdateLCDCalentando = 1;
			LCD_Calentando_Delay = DELAY1S*2;
		}
	}

	//STICK LED
	static uint8_t stickLedStatus = 0;
	static int32_t stickLedCounter = 500;
	if(!stickLedCounter)
	{
		SetPIN(PORT0, 22, stickLedStatus);
		stickLedStatus = !stickLedStatus;
		stickLedCounter = 500;
	}
	else
		stickLedCounter--;
}
