#include "settings.h"

/*Settings EEMEM EEMEMsetting1 = {20.5, 1.0, 0, 1};
Settings EEMEM EEMEMsetting2 = {20.5, 1.0, 0, 1};
Settings EEMEM EEMEMsetting3 = {20.5, 1.0, 0, 1};

extern Settings setting1;
extern Settings setting2;
extern Settings setting3;*/

/*PWMSettings EEMEM EEMPWMSettings[2] = {
	{DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}, {DEFAULT_PERIOD, DEFAULT_PART, DEFAULT_PART_D}
};*/

AllSettings EEMEM EEMEMsettings = {
	
	{
		
			{INIT_NRLSET, INIT_NRLSET, INIT_NRLSET},	
			{INIT_NRLSET, INIT_NRLSET, INIT_NRLSET},	
			{INIT_NRLSET, INIT_NRLSET, INIT_NRLSET}				
			
	},
	{
		
			INIT_PWNSET, INIT_PWNSET, INIT_PWNSET
	}
};

/*Settings EEMEM EEMEMsettings[4][3] = {
	{
			{DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON}, {DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON}, {DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON}	
	},
	{
			{DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON}, {DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON}, {DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON}
	},		
	{
			{DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON}, {DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON}, {DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON}
	},
	{
			{DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON}, {DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON}, {DEFAULT_TEMP, DEFAULT_D, DEFAULT_DIRECTION, DEFAULT_ON}
	}
};*/

void Settings_Init(AllSettings * allSettings)
{
	uint8_t i, j;
	for (i=0; i<4; i++)
		for (j=0; j<3; j++)
			Settings_Read_Normal(allSettings, i, j);
			
	for (i=0; i<3; i++)
		Settings_Read_PWM(allSettings, PWM_RELAY, i);
		
	/*TEMP!*/
	/*allSettings->PWMSettings[0].on = 1;
	allSettings->PWMSettings[0].tpwm = 10;
	allSettings->PWMSettings[0].t3 = 1;*/
}
void Settings_Read_Normal(AllSettings * allSettings, uint8_t relay, uint8_t term)
{
	eeprom_read_block (( void *) &allSettings->settings[relay][term] , ( const void *) &EEMEMsettings.settings[relay][term] , sizeof(Settings)) ;
}
void Settings_Read_PWM(AllSettings * allSettings, uint8_t relay, uint8_t term)
{
	eeprom_read_block (( void *) &allSettings->PWMSettings[term] , ( const void *) &EEMEMsettings.PWMSettings[term] , sizeof(PWMSettings)) ;
}
void Settings_Write_Normal(AllSettings * allSettings, uint8_t relay, uint8_t term)
{
	eeprom_update_block (( void *) &allSettings->settings[relay][term] , ( const void *) &EEMEMsettings.settings[relay][term] , sizeof(Settings)) ;
}
void Settings_Write_PWM(AllSettings * allSettings, uint8_t relay, uint8_t term)
{
	eeprom_update_block (( void *) &allSettings->PWMSettings[term] , ( const void *) &EEMEMsettings.PWMSettings[term] , sizeof(PWMSettings)) ;
}
void Settings_Reset(AllSettings * allSettings)
{
	uint8_t i, j;
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
		{
			allSettings->settings[i][j].temp = DEFAULT_TEMP;
			allSettings->settings[i][j].d = DEFAULT_D;
			allSettings->settings[i][j].direction = DEFAULT_DIRECTION;
			allSettings->settings[i][j].on = DEFAULT_ON;
			Settings_Write_Normal(allSettings, i, j);
		}
	
	for (i=0; i<3; i++)
	{
		allSettings->PWMSettings[i].on = DEFAULT_ON;		
		allSettings->PWMSettings[i].Tzap = DEFAULT_TZAP;
		allSettings->PWMSettings[i].t3 = DEFAULT_T3;
		allSettings->PWMSettings[i].Tstart = DEFAULT_TSTART;
		allSettings->PWMSettings[i].Tstop = DEFAULT_TSTOP;
		allSettings->PWMSettings[i].Tsliv = DEFAULT_TSLIV;
		allSettings->PWMSettings[i].tpwm = DEFAULT_TPWM;
		allSettings->PWMSettings[i].Sgshim = DEFAULT_SGSHIM;
		allSettings->PWMSettings[i].Scshim = DEFAULT_SCSHIM;
		allSettings->PWMSettings[i].Sshim = DEFAULT_SSHIM;
		allSettings->PWMSettings[i].SshimD = DEFAULT_SSDIMD;
		Settings_Write_PWM(allSettings, PWM_RELAY, i);
	}			
}


/*

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
			Settings_Write(all_settings, i, j);
		}				
}
*/

 
