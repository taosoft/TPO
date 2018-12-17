/*
 * SD.c
 *
 *  Created on: Dec 13, 2018
 *      Author: matias
 */

#include "SD.h"

uint8_t saveTemperatureInSdCard(void)
{
	/*FATFS fat;
	FIL *file;
	if(!f_mount(&fat,"",0))
	{
		if(!f_open(file,"data.txt", FA_WRITE | FA_OPEN_APPEND))
		{
			uint32_t data = 0;

			char buffer2[20] = {'\0'};

			uint8_t cantidad = prepareStringToSdCard(buffer2);

			f_write(file,buffer2, cantidad, &data);

			f_close(file);

			return 1;
		}
	}
	return 0;*/

	/*unsigned const char buffer[]="Hola que tal PERROOO";

	//unsigned char buffer2[20] = {'\0'};

	//uint8_t cantidad = prepareStringToSdCard(buffer2);

	//FATFS fat;
	FIL *file;
	//FRESULT res;
	uint32_t data = 0;

	res = f_mount(&fat,"",0);
	res = f_open(file,"prueba.txt", FA_WRITE | FA_OPEN_APPEND);

	//if(!res)
		//SetPIN(0,22,0);



	//f_write(file,buffer2,cantidad-1,&data);
	f_write(file,buffer,sizeof(buffer)-1,&data);

	if(data > 0)
		data = data + 1;
		//LedsRGB(ROJO,ON);

	f_close(file);*/

	return 0;
}
