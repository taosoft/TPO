/*
 * MdE.c
 *
 *  Created on: Dec 2, 2018
 *      Author: matias
 */

#include "MdE.h"

int32_t temperaturaActual = 0;
int32_t temperaturaConfigurada = 350;
uint8_t flagTemperaturaSeteada = 0;

uint8_t chargeInt32TemperatureToCharBuffer(char * buffer, int32_t number)
{
	uint8_t contador = 0;

	buffer = itoaCustom(number, buffer);

	if(number < 0)
		number *= -1;
	do
	{
		number /= 10;
		contador++;
	}while(number > 0);

	return contador;
}

uint8_t charge_unixTime_ToCharBuffer(char * buffer, uint32_t number)
{
	int16_t digito = 0;
	uint8_t contador = 0;
	char buff[15] = {""};

	do
	{
		digito = number % 10;
		number /= 10;
		buff[contador] = digito + 48;
		contador++;
	}while(number > 0);

	for(uint8_t i = 0; i < contador; i++)
	{
		buffer[i] = buff[contador - 1 - i];
	}

	return contador;
}

char * itoaCustom(int32_t i, char b[])
{
    char const digit[] = "0123456789";
    char* p = b;

    if(i<0)
    {
        *p++ = '-';
        i *= -1;
    }

    int32_t shifter = i;

    do
    {
    	++p;
        shifter = shifter/10;
    }while(shifter);

    *p = '\0';

    do
    {
    	*--p = digit[i%10];
        i = i/10;
    }while(i);

    return b;
}

uint8_t prepareStringToSdCard(char buffer2[])
{
	char buffer3[10] = {'\0'};

	uint8_t largoTime = charge_unixTime_ToCharBuffer(buffer2, Time_GetUnixTime());

	const char separador[] = ":";
	const char terminador[] = "\n";

	uint8_t largoTemperatura = chargeInt32TemperatureToCharBuffer(buffer3, temperaturaActual);

	strcat(buffer2, separador);
	strcat(buffer2, buffer3);
	strcat(buffer2, terminador);

	return (largoTime + sizeof(separador) + largoTemperatura + sizeof(terminador) - 1);
}

uint8_t getCantDigits(int16_t number)
{
	uint8_t contadorDigitos = 0;
	while(number > 0)
	{
		number /= 10;
		contadorDigitos++;
	}
	return contadorDigitos;
}

/***************************************************************************/

void printMenuConfigOK(void)
{
	char string1[18] = {0};
	GuardarMensajeLCD("Actual: ", string1);
	LCD_Display(string1, 1 , 0);
	GuardarMensajeLCD("Config: ", string1);
	LCD_Display(string1, 0 , 0);
	printTempChange(0, temperaturaConfigurada);
	printTempChange(1, temperaturaActual);
}

void printTempChange(uint8_t line, int32_t temperatura)
{
	char buffer[8] = {0};
	int8_t largoTemp = chargeInt32TemperatureToCharBuffer(buffer, temperatura/10);

	if(largoTemp < 7)
	{
		buffer[(uint8_t)largoTemp] = '.';
		buffer[largoTemp + 1] = (temperatura % 10) + 48;
	}
	LCD_Display(buffer, line, 8);
}

void printMenuConfig(void)
{
	char buffer1[18] = {0};
	GuardarMensajeLCD("Menu", buffer1);
	LCD_Display(buffer1, 0 , 0);
	GuardarMensajeLCD("Configuracion", buffer1);
	LCD_Display(buffer1, 1 , 0);
}

void printCalentando(void)
{
	char buffer1[18] = {0};
	GuardarMensajeLCD("Iniciando", buffer1);
	LCD_Display(buffer1, 0 , 0);
	GuardarMensajeLCD("Termotanque", buffer1);
	LCD_Display(buffer1, 1 , 0);
}

void MdE_LCD(void)
{
	static uint8_t estado = MENU_CONF;

	static int32_t tempActualCopiaAnterior = 0;

	static uint8_t flagTempCalentando = 0;

	uint8_t key = GetKey();

	switch(estado)
	{
		case MENU_CONF:

			if(key == TECLA_OK)
			{
				printMenuConfigOK();
				estado = SET_TEMP;
			}
		break;

		case SET_TEMP:

			if(key == TECLA_SUMA)
			{
				if(temperaturaConfigurada < 990)
				{
					temperaturaConfigurada += 5;
					printTempChange(0, temperaturaConfigurada);
				}
			}
			else if(key == TECLA_RESTA)
			{
				if(temperaturaConfigurada > 0)
				{
					temperaturaConfigurada -= 5;
					printTempChange(0, temperaturaConfigurada);
				}
			}
			else if(key == TECLA_OK)
			{
				printCalentando();
				flagUpdateLCDCalentando = 0;
				flagTemperaturaSeteada = 1;
				estado = CALENTANDO;
				lcdDelay = 1000;
			}
			else if(tempActualCopiaAnterior != temperaturaActual && flagUpdateLCD)
			{
				tempActualCopiaAnterior = temperaturaActual;
				printTempChange(1, temperaturaActual);
				flagUpdateLCD = 0;
			}
		break;

		case CALENTANDO:

			if(flagUpdateLCDCalentando)
			{
				if(!flagTempCalentando)
				{
					printMenuConfigOK();
					flagTempCalentando = 1;
				}
				else if(key == TECLA_OK)
				{
					flagTemperaturaSeteada = 0;
					printMenuConfig();
					estado = MENU_CONF;
				}
				else if(tempActualCopiaAnterior != temperaturaActual && flagUpdateLCD)
				{
					tempActualCopiaAnterior = temperaturaActual;
					printTempChange(1, temperaturaActual);
					flagUpdateLCD = 0;
				}
			}
			else
				flagTempCalentando = 0;
		break;
	}
}

/***************************************************************************/

uint8_t isNumber(uint8_t *dato)
{
	*dato -= 48;
	return (*dato >= 0 && *dato <= 9);
}

void saveUnixTimeRTC(uint8_t *espUnixTime)
{
	static uint32_t unixTimeRunning = 0;
	uint32_t unixTime = 0;
	uint32_t multiplicador = 1000000000;
	for(uint8_t i = 0; i < 10; i++)
	{
		unixTime += (espUnixTime[i] * multiplicador);
		multiplicador /= 10;
	}
	if(unixTime > unixTimeRunning)
	{
		Time_SetUnixTime(unixTime);
		unixTimeRunning = unixTime;
	}
}

void saveNewConfigTemp(uint8_t *configTempEsp, uint8_t largoTemp)
{
	int16_t newTemp = 0;
	int16_t multiplicador = 1;

	for(uint8_t i = 1; i < largoTemp; i++)
	{
		multiplicador *= 10;
	}

	for(uint8_t i = 0; i < largoTemp; i++)
	{
		newTemp += (configTempEsp[i] * multiplicador);
		multiplicador /= 10;
	}

	if(newTemp >= 0 && newTemp <= 1000)
	{
		temperaturaConfigurada = newTemp;
	}
}

void MdE_ESP(void)
{
	//Inicio de trama
	static uint8_t estado = ESPERANDO;
	static uint8_t contadorPesos = 0;

	//Dato por puerto serie recibido
	uint8_t dato;

	//UnixTime
	static uint8_t espUnixTime[10] = {0};
	static uint8_t contadorNumerosUnixTime = 0;
	static uint8_t flagUnixTimeReady = 0;

	//Configuracion Temperatura desde GUI
	static uint8_t contadorDigitosTemp = 0;
	static uint8_t flagCantidadDigitos = 0;
	static uint8_t cantidadDigitos = 0;
	static uint8_t nuevaTempConfigEsp[3] = {0};
	static uint8_t flagConfigTempReady = 0;

	if(!Serial_PopRx(&dato))
	{
		switch(estado)
		{
			case ESPERANDO:
				if(dato == '$')
				{
					contadorPesos++;
					if(contadorPesos == 3)
						estado = CLASIFICACION;
				}
				else
					contadorPesos = 0;
			break;

			case CLASIFICACION:
				if(dato == 'u')
				{
					contadorNumerosUnixTime = 0;
					flagUnixTimeReady = 0;
					estado = UNIXTIME;
				}
				else if(dato == 't')
				{
					flagCantidadDigitos = 0;
					cantidadDigitos = 0;
					contadorDigitosTemp = 0;
					flagConfigTempReady = 0;
					estado = CONFIG_TEMP_STICK;
				}
				else
					estado = ESPERANDO;
			break;

			case UNIXTIME:
				if(dato != '\n')
				{
					if(isNumber(&dato) && !flagUnixTimeReady)
					{
						espUnixTime[contadorNumerosUnixTime] = dato;
						contadorNumerosUnixTime++;
						if(contadorNumerosUnixTime == 10)
							flagUnixTimeReady = 1;
					}
					else
						estado = ESPERANDO;
				}
				else if(flagUnixTimeReady)
				{
					saveUnixTimeRTC(espUnixTime);
					estado = ESPERANDO;
				}
				else
					estado = ESPERANDO;
			break;

			case CONFIG_TEMP_STICK:
				if(dato != '\n')
				{
					if(isNumber(&dato) && !flagConfigTempReady)
					{
						if(!flagCantidadDigitos)
						{
							flagCantidadDigitos = 1;
							if(dato > 0 && dato < 4)
								cantidadDigitos = dato;
							else
								estado = ESPERANDO;
						}
						else
						{
							nuevaTempConfigEsp[contadorDigitosTemp] = dato;
							contadorDigitosTemp++;
							if(contadorDigitosTemp == cantidadDigitos)
								flagConfigTempReady = 1;
						}
					}
					else
						estado = ESPERANDO;
				}
				else if(flagConfigTempReady)
				{
					saveNewConfigTemp(nuevaTempConfigEsp, cantidadDigitos);
					estado = ESPERANDO;
				}
				else
					estado = ESPERANDO;
			break;
		}
	}
}

/***************************************************************************/

uint8_t prepareStringToESP(char bufferPrincipal[])
{
	char bufferUnixTime[11] = {0};
	char bufferTemp[10] = {0};
	char inicioCadena[6] = "$$$log";

	strcat(bufferPrincipal, inicioCadena);

	uint8_t largoTime = charge_unixTime_ToCharBuffer(bufferUnixTime, Time_GetUnixTime());
	uint8_t largoTemperatura = chargeInt32TemperatureToCharBuffer(bufferTemp, temperaturaActual);

	char buffer[2] = {largoTemperatura + 48, 0};

	char terminador[] = "\0";

	strcat(bufferPrincipal, bufferUnixTime);
	strcat(bufferPrincipal, buffer);
	strcat(bufferPrincipal, bufferTemp);
	strcat(bufferPrincipal, terminador);

	return (sizeof(inicioCadena) + largoTime + sizeof(largoTemperatura) + largoTemperatura + sizeof(terminador) - 1);
}

void MdE_Temp(void)
{
	int16_t temp_entero = 0;
	uint8_t temp_decimal = 0;

	temperaturaActual = getPT100Temp(&temp_entero, &temp_decimal);

	if(temperaturaActual < temperaturaConfigurada && flagTemperaturaSeteada)
		SetRelay(ENCENDIDO);
	else
		SetRelay(APAGADO);

	if(flagSendTempReady)
	{
		//saveTemperatureInSdCard();

		char bufferToSendUART[24] = {0};
		uint8_t largoBufferToSendUART = prepareStringToESP(bufferToSendUART);
		Serial_Send((uint8_t *)bufferToSendUART, largoBufferToSendUART);

		//flagSendTempReady = 0;
	}

	if(flagSendEstadoCalentador)
	{
		char bufferToSendUART[12] = "$$$status";
		bufferToSendUART[9] = GetRelay() + 48;
		bufferToSendUART[10] = '\0';
		Serial_Send((uint8_t *)bufferToSendUART, 10);

		flagSendEstadoCalentador = 0;
	}
}
