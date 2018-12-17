/*
 * DR_LCD_Display.h
 *
 *  Created on: Dec 7, 2018
 *      Author: matias
 */

#ifndef INC_DRIVERS_DR_LCD_DISPLAY_H_
#define INC_DRIVERS_DR_LCD_DISPLAY_H_

#include "DR_GPIO.h"

#define		LCD_CONTROL		1
#define		LCD_DATA		0

#define 	LCD_BUFFER_SIZE	512

#define	 	LCD_LINE_SIZE	20
#define 	LCD_LINES		2

#define 	LINE_1_ADDRESS	0x00
#define 	LINE_2_ADDRESS	0x40

#define		LCD_Clear()		LCD_Push(0x01, LCD_CONTROL)

#define		LCD_D4			PORT0,5
#define		LCD_D5			PORT0,10
#define		LCD_D6			PORT2,4
#define		LCD_D7			PORT2,5
#define		LCD_RS			PORT2,6
#define		LCD_EN			PORT0,4

extern uint32_t volatile lcdDelay;

void Init_LCD_Display(void);
void LCD_Push(uint8_t, uint8_t);
int16_t LCD_Pop(void);
void LCD_Barrido(void);

#endif /* INC_DRIVERS_DR_LCD_DISPLAY_H_ */
