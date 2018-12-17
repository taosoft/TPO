/*
===============================================================================
 Name        : TPO.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/
#include <cr_section_macros.h>

#include "SystemInit.h"
#include "MdE.h"

#include "ff.h"

FATFS fat;
FRESULT res;

int main(void) {

	Inicializacion();

    while(1) {
    	MdE_Temp();
    	MdE_LCD();
    	MdE_ESP();
    	if(flagSendTempReady)
    	{

    		//Escritura en SD Card
    		FIL *file;
    		uint32_t data = 0;
    		if(res = f_mount(&fat,"",0) == 0)
    		{
    			if(res = f_open(file,"data.txt", FA_WRITE | FA_OPEN_APPEND) == 0)
    			{
					char buffer2[20] = {'\0'};
					uint8_t cantidad = prepareStringToSdCard(buffer2);
					f_write(file,buffer2,cantidad-1,&data);
					f_close(file);
    			}
    		}
    		f_mount(0, "", 0);

    		flagSendTempReady = 0;
    	}
    }
    return 0 ;
}
