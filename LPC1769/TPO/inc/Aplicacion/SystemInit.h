/*
 * SystemInit.h
 *
 *  Created on: Dec 13, 2018
 *      Author: matias
 */

#ifndef APLICACION_SYSTEMINIT_H_
#define APLICACION_SYSTEMINIT_H_

#include "DR_PLL.h"
#include "DR_SysTick.h"
#include "DR_LCD_Display.h"
#include "DR_RTC.h"
#include "DR_Teclado.h"
#include "DR_UART.h"
#include "DR_SD.h"
#include "DR_Relay.h"

#include "DR_MAX31865.h"
#include "PR_MAX31865.h"

#include "PR_LCD_Display.h"

#include "API_SystemTime.h"

void Inicializacion(void);

#endif /* APLICACION_SYSTEMINIT_H_ */
