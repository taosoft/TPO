/*
 * MdE.h
 *
 *  Created on: Dec 2, 2018
 *      Author: matias
 */

#ifndef MDE_H_
#define MDE_H_

#include "RegsLPC17xx.h"

#include "string.h"

#include "API_SystemTime.h"

#include "stdlib.h"
#include "PR_LCD_Display.h"

#include "PR_Serial.h"

#include "PR_Teclado.h"

#include "PR_MAX31865.h"

#include "DR_SysTick.h"

#include "PR_Relay.h"

uint8_t chargeInt32TemperatureToCharBuffer(char *, int32_t);
uint8_t charge_unixTime_ToCharBuffer(char *, uint32_t);
char * itoaCustom(int32_t, char *);
uint8_t prepareStringToSdCard(char *);
uint8_t saveTemperatureInSdCard(void);

/***************************************************************************/

void MdE_LCD(void);

uint8_t getCantDigits(int16_t);
void printMenuConfigOK(void);
void printTempChange(uint8_t, int32_t);
void printMenuConfig(void);
void printCalentando(void);

#define MENU_CONF 1
#define SET_TEMP 2
#define CALENTANDO 3
#define ERROR 4

/***************************************************************************/

void MdE_ESP(void);

uint8_t isNumber(uint8_t *);
void saveUnixTimeRTC(uint8_t *);
void saveNewConfigTemp(uint8_t *, uint8_t);

#define ESPERANDO 1
#define CLASIFICACION 2
#define UNIXTIME 3
#define CONFIG_TEMP_STICK 4

/***************************************************************************/

void MdE_Temp(void);

uint8_t prepareStringToESP(char *);

#endif /* MDE_H_ */
