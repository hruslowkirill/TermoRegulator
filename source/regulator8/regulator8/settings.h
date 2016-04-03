#ifndef SETTINGS_H
#define SETTINGS_H

#include "defines.h"
#include <avr/eeprom.h>
#define DEFAULT_TEMP 20.5
#define DEFAULT_D 1.0
#define DEFAULT_DIRECTION 0
#define DEFAULT_ON 0
#define DEFAULT_PERIOD 1
#define DEFAULT_PART 30

typedef struct
{
	float temp;
	float d;
	uint8_t direction;
	uint8_t on;	
	uint8_t period;
	uint8_t part;
}Settings;

typedef Settings SettingsType[4][3];


void Settings_Init(SettingsType all_settings);
void Settings_Read(SettingsType all_settings, uint8_t relay, uint8_t term);
void Settings_Write(SettingsType all_settings, uint8_t relay, uint8_t term);
void Settings_Reset(SettingsType all_settings);

#endif
