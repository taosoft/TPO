/*
 * FW_RTC.h
 *
 *  Created on: 14/03/2012
 *      Author: Pablo
 */

#ifndef DR_RTC_H_
#define DR_RTC_H_

#include "RegsLPC17xx.h"

//!< ///////////////////   RTC   /////////////////////////////
//!< 0x40034000UL: Registro de configuracion y estado del RTC y registros de backup:
#define		RTC 			( (__RW uint32_t *) 0x40024000UL )

//!< Miscellaneous registers
#define		ILR			RTC[0]
#define		CCR			RTC[2]
#define		CIIR		RTC[3]
#define		AMR			RTC[4]
#define		RTC_AUX		RTC[23]
#define		RTC_AUXEN	RTC[22]
#define		CTIME0		RTC[5]
#define		CTIME1		RTC[6]
#define		CTIME2		RTC[7]

//!< Time counter registers
#define		SEC			RTC[8]
#define		MIN			RTC[9]
#define		HOUR		RTC[10]
#define		DOM			RTC[11]
#define		DOW			RTC[12]
#define		DOY			RTC[13]
#define		MONTH		RTC[14]
#define		YEAR		RTC[15]
#define		CALIBRATION	RTC[16]

//!< General purpose registers
#define		GPREG0		RTC[17]
#define		GPREG1		RTC[18]
#define		GPREG2		RTC[19]
#define		GPREG3		RTC[20]
#define		GPREG4		RTC[21]

//!< Alarm register group
#define		ALSEC		RTC[24]
#define		ALMIN		RTC[25]
#define		ALHOUR		RTC[26]
#define		ALDOM		RTC[27]
#define		ALDOW		RTC[28]
#define		ALDOY		RTC[29]
#define		ALMON		RTC[30]
#define		ALYEAR		RTC[31]


// Definiciones RTC
#define RTC_INT_SEG			0x01
#define RTC_INT_MIN			0x02
#define RTC_INT_HOUR		0x04
#define RTC_INT_DAYMONTH	0x08
#define RTC_INT_DAYWEEK		0x10
#define RTC_INT_DAYYEAR		0x20
#define RTC_INT_MONTH		0x40
#define RTC_INT_YEAR		0x80

#define RTC_AL_SEG			0x01
#define RTC_AL_MIN			0x02
#define RTC_AL_HOUR			0x04
#define RTC_AL_DAYMONTH		0x08
#define RTC_AL_DAYWEEK		0x10
#define RTC_AL_DAYYEAR		0x20
#define RTC_AL_MONTH		0x40
#define RTC_AL_YEAR			0x80

//Macros
#define RTC_SetHour(hour)		(HOUR = hour)
#define RTC_SetMin(min)			(MIN = min)
#define RTC_SetSec(sec)			(SEC = sec)
#define RTC_SetYear(year)		(YEAR = year)
#define RTC_SetMonth(month)		(MONTH = month)
#define RTC_SetDayOfMonth(day)	(DOM = day)
#define RTC_SetDayOfWeek(day)	(DOW = day)
#define RTC_SetDayOfYear(day)	(DOY = day)

#define RTC_GetHour(hour)		(HOUR)
#define RTC_GetMin(min)			(MIN)
#define RTC_GetSec(sec)			(SEC)
#define RTC_GetYear(year)		(YEAR)
#define RTC_GetMonth(month)		(MONTH)
#define RTC_GetDayOfMonth(day)	(DOM)
#define RTC_GetDayOfWeek(day)	(DOW)
#define RTC_GetDayOfYear(day)	(DOY)

typedef void (*RTC_InterruptCallback)(void);

// Prototipos
void RTC_Init(void);
void RTC_EnableIncrementInterrupt(uint32_t);
void RTC_EnableAlarmInterrupt(uint32_t);
void RTC_SetIncrementCallback(RTC_InterruptCallback callback);
void RTC_SetAlarmCallback(RTC_InterruptCallback callback);

#endif /* DR_RTC_H_ */
