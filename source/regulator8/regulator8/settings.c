#include "settings.h"

/*Settings EEMEM EEMEMsetting1 = {20.5, 1.0, 0, 1};
Settings EEMEM EEMEMsetting2 = {20.5, 1.0, 0, 1};
Settings EEMEM EEMEMsetting3 = {20.5, 1.0, 0, 1};

extern Settings setting1;
extern Settings setting2;
extern Settings setting3;*/


Settings EEMEM EEMEMsettings[4][3] = {
	{
			{DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}, {DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}, {DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}	
	},
	{
			{DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}, {DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}, {DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}
	},		
	{
			{DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}, {DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}, {DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}
	},
	{
			{DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}, {DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}, {DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}
	}
};

/*Settings EEMEM EEMEMsettings[4][3] = {
	{
			{DEFAULT_TEMP, 1, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}, {DEFAULT_TEMP, 2, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}, {DEFAULT_TEMP, 3, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}	
	},
	{
			{DEFAULT_TEMP, 4, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}, {DEFAULT_TEMP, 5, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}, {DEFAULT_TEMP, 6, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}
	},		
	{
			{DEFAULT_TEMP, 7, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}, {DEFAULT_TEMP, 8, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}, {DEFAULT_TEMP, 9, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}
	},
	{
			{DEFAULT_TEMP, 10, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}, {DEFAULT_TEMP, 11, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}, {DEFAULT_TEMP, 12, DEFAULT_DIRECTION, DEFAULT_ON, DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}
	}
};*/


void Settings_Init(SettingsType all_settings)
{
	uint8_t i, j;
	for (i=0; i<4; i++)
		for (j=0; j<3; j++)
			Settings_Read(all_settings, i, j);
}

void Settings_Read(SettingsType all_settings, uint8_t relay, uint8_t term)
{
		eeprom_read_block (( void *) &all_settings[relay][term] , ( const void *) &EEMEMsettings[relay][term] , sizeof(Settings)) ;
		if (relay==PWN_RELAY)
		{
			resetPWMParts(&all_settings[relay][0]);			
		}
}

void Settings_Write(SettingsType all_settings, uint8_t relay, uint8_t term)
{
	eeprom_update_block (( void *) &all_settings[relay][term] , ( const void *) &EEMEMsettings[relay][term] , sizeof(Settings)) ;
	if (relay==PWN_RELAY)
	{
		resetPWMParts(&all_settings[relay][0]);			
	}
}

void Settings_Reset(SettingsType all_settings)
{
	uint8_t i, j;
	for (i=0; i<4; i++)
		for (j=0; j<3; j++)
		{
			all_settings[i][j].temp = DEFAULT_TEMP;
			all_settings[i][j].d = DEFAULT_D;
			all_settings[i][j].direction = DEFAULT_DIRECTION;
			all_settings[i][j].on = DEFAULT_ON;
			all_settings[i][j].period = DEFAULT_PERIOD;
			all_settings[i][j].part = DEFAULT_PART;
			Settings_Write(all_settings, i, j);
		}				
}

/*void Settings_Read(uint8_t relay, uint8_t term)
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
}*/

 
