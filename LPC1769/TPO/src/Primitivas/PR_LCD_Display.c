/*
 * PR_LCD_Display.c
 *
 *  Created on: Dec 7, 2018
 *      Author: matias
 */

#include "PR_LCD_Display.h"

void LCD_Display(const char *string, uint8_t line, uint8_t pos)
{
	uint8_t i;
	switch(line)
	{
		case 0:
			LCD_Push(pos + (0x80 | LINE_1_ADDRESS), LCD_CONTROL);
			break;
		case 1:
			LCD_Push(pos + (0x80 | LINE_2_ADDRESS), LCD_CONTROL);
			break;
	}
	for(i = 0; string[i] != '\0'; i++)
		LCD_Push(string[i], LCD_DATA);
}

void GuardarMensajeLCD(char *mensaje, char *lugar)
{
	uint8_t i;

	for( i = 0; mensaje[i] != '\0'; i++ )
		lugar[i] = mensaje[i];

	for( ; i < 16; i++ )
		lugar[i] = ' ';

	lugar[17] = '\0';
}
