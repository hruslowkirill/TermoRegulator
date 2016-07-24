#ifndef SETTINGS_H
#define SETTINGS_H

#include "defines.h"
#include <avr/eeprom.h>
#define DEFAULT_TEMP 20.5
#define DEFAULT_D 1.0
#define DEFAULT_DIRECTION 1
#define DEFAULT_ON 0
#define DEFAULT_PERIOD 1
#define DEFAULT_PART 30
#define DEFAULT_PART_D 0

typedef struct 
{
	uint8_t period;
	uint8_t part;
	uint8_t partD;
} PWMSettings;

typedef struct
{
	float temp;
	float d;
	uint8_t direction;
	uint8_t on;	
	PWMSettings pwmSettings;
	//uint8_t period;
	//uint8_t part;
	//uint8_t partD;
}Settings;

typedef Settings SettingsType[4][3];
//typedef PWMSettings PWMSettingsType[2];


void Settings_Init(SettingsType all_settings);
void Settings_Read(SettingsType all_settings, uint8_t relay, uint8_t term);
void Settings_Write(SettingsType all_settings, uint8_t relay, uint8_t term);
void Settings_Reset(SettingsType all_settings);

/*void PWMSettingsRead(PWMSettingsType all_pwm_settings);
void PWMSettingsWrite(PWMSettingsType all_pwm_settings);*/

#endif
