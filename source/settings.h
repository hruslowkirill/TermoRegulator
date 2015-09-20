#ifndef SETTINGS_H
#define SETTINGS_H

#include "defines.h"
#include <avr/eeprom.h>

typedef struct
{
	float temp;
	uint8_t d;
	uint8_t direction;	
}Settings;


void Settings_Init();
void Settings_Read(uint8_t n);
void Settings_Write(uint8_t n);

#endif
