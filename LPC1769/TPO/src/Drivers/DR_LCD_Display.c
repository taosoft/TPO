/*
 * DR_LCD_Display.h
 *
 *  Created on: Dec 7, 2018
 *      Author: matias
 */

#include "DR_LCD_Display.h"

uint32_t volatile lcdDelay = 0;
uint8_t lcdBuffer[LCD_BUFFER_SIZE] = {0};
int32_t lcdDataCount = 0;
int16_t lcdInxIn = 0;
int16_t lcdOutxOut = 0;

void LCD_Push(uint8_t data, uint8_t control)
{
	if(lcdDataCount >= LCD_BUFFER_SIZE)
		return;

	lcdBuffer[lcdInxIn] = (data >> 4) & 0x0F;

	if(control == LCD_CONTROL)
		lcdBuffer[lcdInxIn] |= 0x80;

	lcdInxIn++;

	lcdBuffer[lcdInxIn] = data & 0x0F;

	if(control == LCD_CONTROL)
		lcdBuffer[lcdInxIn] |= 0x80;

	lcdDataCount += 2;

	lcdInxIn++;
	lcdInxIn %= LCD_BUFFER_SIZE;

}

int16_t LCD_Pop(void)
{
	if(!lcdDataCount)
		return -1;

	int16_t data = lcdBuffer[lcdOutxOut];

	lcdDataCount--;

	lcdOutxOut++;
	lcdOutxOut %= LCD_BUFFER_SIZE;

	return data;
}

void LCD_Barrido(void)
{
	int16_t data = 0;
	if((data = LCD_Pop()) == -1)
		return;
	SetPIN(LCD_D4, (((uint8_t) data) >> 0) & 0x01);
	SetPIN(LCD_D5, (((uint8_t) data) >> 1) & 0x01);
	SetPIN(LCD_D6, (((uint8_t) data) >> 2) & 0x01);
	SetPIN(LCD_D7, (((uint8_t) data) >> 3) & 0x01);

	if(((uint8_t) data) & 0x80)
		SetPIN(LCD_RS, OFF);
	else
		SetPIN(LCD_RS, ON);

	SetPIN(LCD_EN, ON);
	SetPIN(LCD_EN, OFF);
}

void Init_LCD_Display(void)
{
	//Inicialización pines LCD
	SetPINSEL(LCD_D4, PINSEL_GPIO);
	SetDIR(LCD_D4, GPIO_OUTPUT);
	SetPIN(LCD_D4, OFF);

	SetPINSEL(LCD_D5, PINSEL_GPIO);
	SetDIR(LCD_D5, GPIO_OUTPUT);
	SetPIN(LCD_D5, OFF);

	SetPINSEL(LCD_D6, PINSEL_GPIO);
	SetDIR(LCD_D6, GPIO_OUTPUT);
	SetPIN(LCD_D6, OFF);

	SetPINSEL(LCD_D7, PINSEL_GPIO);
	SetDIR(LCD_D7, GPIO_OUTPUT);
	SetPIN(LCD_D7, OFF);

	SetPINSEL(LCD_RS, PINSEL_GPIO);
	SetDIR(LCD_RS, GPIO_OUTPUT);
	SetPIN(LCD_RS, OFF);

	SetPINSEL(LCD_EN, PINSEL_GPIO);
	SetDIR(LCD_EN, GPIO_OUTPUT);
	SetPIN(LCD_EN, OFF);

	//Pin LED Stick
	#define LED_STICK PORT0, 22

	SetPINSEL(LED_STICK, PINSEL_GPIO);
	SetDIR(LED_STICK, GPIO_OUTPUT);
	SetPIN(LED_STICK, OFF);

	lcdDelay = 50;
	while(lcdDelay);

	// Configuración en 8 bits
	for(uint8_t i = 0; i < 3; i++)
	{
		SetPIN(LCD_D4, ON);
		SetPIN(LCD_D5, ON);
		SetPIN(LCD_D6, OFF);
		SetPIN(LCD_D7, OFF);

		SetPIN(LCD_RS, OFF);
		SetPIN(LCD_EN, ON);
		SetPIN(LCD_EN, OFF);
		lcdDelay = 10;
		while(lcdDelay);
	}

	// Configuración en 4 bits
	SetPIN(LCD_D4, OFF);
	SetPIN(LCD_D5, ON);
	SetPIN(LCD_D6, OFF);
	SetPIN(LCD_D7, OFF);

	SetPIN(LCD_RS, OFF);
	SetPIN(LCD_EN, ON);
	SetPIN(LCD_EN, OFF);

	lcdDelay = 5;
	while(lcdDelay);

	// Toda configuración será de 4 bits
	LCD_Push(0x28, LCD_CONTROL);
	lcdDelay = 4;
	while(lcdDelay);
	LCD_Push(0x08, LCD_CONTROL);
	lcdDelay = 4;
	while(lcdDelay);
	LCD_Push(0x01, LCD_CONTROL);
	lcdDelay = 4;
	while(lcdDelay);
	LCD_Push(0x06, LCD_CONTROL);
	lcdDelay = 4;
	while(lcdDelay);
	LCD_Push(0x0C, LCD_CONTROL);
	lcdDelay = 4;
	while(lcdDelay);
}
