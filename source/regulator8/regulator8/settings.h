#ifndef SETTINGS_H
#define SETTINGS_H

#include "defines.h"
#include <avr/eeprom.h>
#define DEFAULT_TEMP 20.5
#define DEFAULT_D 1.0
#define DEFAULT_DIRECTION 1
#define DEFAULT_ON 0

#define DEFAULT_TZAP 70
#define DEFAULT_T3 1
#define DEFAULT_TSTOP 0
#define DEFAULT_TSTART 0
#define DEFAULT_TSLIV 70
#define DEFAULT_TPWM 10
#define DEFAULT_SGSHIM 1
#define DEFAULT_SCSHIM 1 
#define DEFAULT_SSHIM 1
#define DEFAULT_SSDIMD 1

#define INIT_PWNSET {DEFAULT_ON, DEFAULT_TZAP, DEFAULT_T3, DEFAULT_TSTOP, DEFAULT_TSTART, DEFAULT_TSLIV, DEFAULT_TPWM, DEFAULT_SGSHIM, DEFAULT_SCSHIM, DEFAULT_SSHIM, DEFAULT_SSDIMD}
#define INIT_NRLSET {DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON}

typedef struct 
{
	uint8_t on;
	float Tzap;
	uint8_t t3;
	float Tstop;
	float Tstart;
	float Tsliv;
	uint8_t tpwm;
	uint8_t Sgshim;
	uint8_t Scshim;
	uint8_t Sshim;
	uint8_t SshimD;
	
} PWMSettings; //22

typedef struct
{
	float temp;
	float d;
	uint8_t direction;
	uint8_t on;	
}Settings; //10

//typedef Settings SettingsType[4][3];

typedef struct  
{
	Settings settings[3][3]; //90
	PWMSettings PWMSettings[3]; // 66
}AllSettings;
 
void Settings_Init(AllSettings * allSettings);
void Settings_Read_Normal(AllSettings * allSettings, uint8_t relay, uint8_t term);
void Settings_Read_PWM(AllSettings * allSettings, uint8_t relay, uint8_t term);
void Settings_Write_Normal(AllSettings * allSettings, uint8_t relay, uint8_t term);
void Settings_Write_PWM(AllSettings * allSettings, uint8_t relay, uint8_t term);
void Settings_Reset(AllSettings * allSettings);


/*void Settings_Init(SettingsType all_settings);
void Settings_Read(SettingsType all_settings, uint8_t relay, uint8_t term);
void Settings_Write(SettingsType all_settings, uint8_t relay, uint8_t term);
void Settings_Reset(SettingsType all_settings);*/




#endif
