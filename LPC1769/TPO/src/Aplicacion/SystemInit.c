/*
 * SystemInit.c
 *
 *  Created on: Dec 13, 2018
 *      Author: matias
 */

#include "SystemInit.h"

void Inicializacion(void)
{
	InicializarPLL();
	InitSysTick();
	Init_LCD_Display();
	Init_Rele();
	RTC_Init();
	InitTeclado();
	UART_Init(115200);
	Init_SD();

	Init_MAX31685();
	ConfigMAX31865();

	//Init Menu Configuracion
	char buffer1[18] = {0};
	GuardarMensajeLCD("Menu", buffer1);
	LCD_Display(buffer1, 0 , 0);
	GuardarMensajeLCD("Configuracion", buffer1);
	LCD_Display(buffer1, 1 , 0);
	lcdDelay = 2000;
	while(lcdDelay);

	//RTC
	Time_SetTimezone(-3);
}
