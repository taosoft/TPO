/* Copyright 2017, Pablo Irrera Condines

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "API_SystemTime.h"

static DateTime_t g_dateTime = {
		.sec = 0,
		.min = 0,
		.hour = 0,
		.day = 1,
		.month = 1,
		.year = 1970
};
static int8_t g_timezone = 0;

static const uint8_t g_daysOfMonths[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
static const uint16_t g_daysPerMonth[4][12] =
{
	{   0,  31,  59,  90, 120, 151, 181, 212, 243, 273, 304, 334},
    { 365, 396, 424, 455, 485, 516, 546, 577, 608, 638, 669, 699},
    { 730, 761, 790, 821, 851, 882, 912, 943, 974,1004,1035,1065},
    {1096,1127,1155,1186,1216,1247,1277,1308,1339,1369,1400,1430},
};

static uint32_t DateTimeToUnixTime(DateTime_t* dateTime);
static void UnixTimeToDateTime(DateTime_t* dateTime, uint32_t unixTime);
static uint8_t IsLeapYear(uint16_t year);

static unsigned int DateTimeToUnixTime(DateTime_t* dateTime)
{
    unsigned int sec = dateTime->sec;  // 0-59
    unsigned int min = dateTime->min;  // 0-59
    unsigned int hour = dateTime->hour;    // 0-23
    unsigned int day = dateTime->day - 1;   // 0-30
    unsigned int month = dateTime->month - 1; // 0-11
    unsigned int year = dateTime->year-1970;    // 0-...
    return (((year/4*(365*4+1)+g_daysPerMonth[year%4][month]+day)*24+hour)*60+min)*60+sec;
}

static void UnixTimeToDateTime(DateTime_t* dateTime, unsigned int unixTime)
{
    dateTime->sec = unixTime%60;
    unixTime /= 60;
    dateTime->min = unixTime%60;
    unixTime /= 60;
    dateTime->hour   = unixTime%24;
    unixTime /= 24;

    unsigned int years = (unixTime/(365*4+1)*4)+1970;
    unixTime %= 365*4+1;

    unsigned int year;
    for (year=3; year>0; year--)
    {
        if (unixTime >= g_daysPerMonth[year][0])
            break;
    }

    unsigned int month;
    for (month=11; month>0; month--)
    {
        if (unixTime >= g_daysPerMonth[year][month])
            break;
    }

    dateTime->year  = years+year;
    dateTime->month = month + 1;
    dateTime->day   = unixTime-g_daysPerMonth[year][month] + 1;
}

static uint8_t IsLeapYear(uint16_t year)
{
	if(((year % 4 == 0) && (year % 100 != 0)) || year % 400 == 0)
		return 1;
	else
		return 0;
}

void Time_SetTimezone(int8_t timezone)
{
	g_timezone = timezone;
}

int8_t Time_GetTimezone(void)
{
	return g_timezone;
}

void Time_GetDateTime(DateTime_t *dateTime)
{
	dateTime->sec = RTC_GetSec();
	dateTime->min = RTC_GetMin();
	dateTime->hour = RTC_GetHour();
	dateTime->day = RTC_GetDayOfMonth();
	dateTime->month = RTC_GetMonth();
	dateTime->year = RTC_GetYear();
}

uint32_t Time_GetUnixTime(void)
{
	DateTime_t dateTime;
	Time_GetDateTime(&dateTime);
	return DateTimeToUnixTime(&dateTime) - g_timezone * 3600;
}

void Time_SetDateTime(const DateTime_t *dateTime)
{
	RTC_SetSec(dateTime->sec);
	RTC_SetMin(dateTime->min);
	RTC_SetHour(dateTime->hour);
	RTC_SetDayOfMonth(dateTime->day);
	RTC_SetMonth(dateTime->month);
	RTC_SetYear(dateTime->year);
}

void Time_SetUnixTime(uint32_t unixTime)
{
	DateTime_t dateTime;
	UnixTimeToDateTime(&dateTime, unixTime + g_timezone * 3600);
	Time_SetDateTime(&dateTime);
}
