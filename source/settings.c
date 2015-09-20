#include "settings.h"

Settings EEMEM EEMEMsetting1 = {20.5, 5, 0};
Settings EEMEM EEMEMsetting2 = {20.5, 5, 0};
Settings EEMEM EEMEMsetting3 = {20.5, 5, 0};

extern Settings setting1;
extern Settings setting2;
extern Settings setting3;

void Settings_Init()
{
	Settings_Read(1);
	Settings_Read(2);
	Settings_Read(3);
}

void Settings_Read(uint8_t n)
{
	//setting1.d = 77;
	//return;
	switch(n)
	{
		case 1:
			eeprom_read_block (( void *) &setting1 , ( const void *) &EEMEMsetting1 , sizeof(setting1)) ;
		break;

		case 2:
			eeprom_read_block (( void *) &setting2 , ( const void *) &EEMEMsetting2 , sizeof(setting2)) ;
		break;

		case 3:
			eeprom_read_block (( void *) &setting3 , ( const void *) &EEMEMsetting3 , sizeof(setting3)) ;
		break;
	}
	
}


void Settings_Write(uint8_t n)
{
	switch(n)
	{
		case 1:
			eeprom_update_block (( void *) &setting1 , ( const void *) &EEMEMsetting1 , sizeof(setting1)) ;
		break;

		case 2:
			eeprom_update_block (( void *) &setting2 , ( const void *) &EEMEMsetting2 , sizeof(setting2)) ;
		break;

		case 3:
			eeprom_update_block (( void *) &setting3 , ( const void *) &EEMEMsetting3 , sizeof(setting3)) ;
		break;
	}
}

 
