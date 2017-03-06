#include "relay.h"

/*uint8_t isRelayOn[4][3] = {1};
PWMSettings * pwmSetting = NULL;
int8_t pwmParts = 0;
uint8_t pwmPartsD = 0;
uint8_t firstTime = 1;*/

/*void Relay_Init()
{
	Off(RELAY_DDR, PWN_SIGNAL);
	On(RELAY_DDR, RELAY1);
	On(RELAY_DDR, RELAY2);
	On(RELAY_DDR, RELAY3);
	On(RELAY_DDR, RELAY4);

	RELAY_OFF( RELAY1);
	RELAY_OFF( RELAY2);
	RELAY_OFF( RELAY3);
	RELAY_OFF( RELAY4);
	
	//TIMSK1 = _BV(OCIE1A); //Enable Interrupt Timer/Counter 1,Output Compare A
	//TCCR1B = _BV(CS11) | _BV(WGM12); //Clock/8, 0.000008 secs/tick, Mode=CTC
	//OCR1A = 16; // SIG_COMPARE1A triggered every 0.000008*16 Seconds
	cli();             // disable global interrupts
    TCCR1A = 0;        // set entire TCCR1A register to 0
    TCCR1B = 0;
 
    
	OCR1A = 0x021D;

    TCCR1B |= (1 << WGM12);
    // Mode 4, CTC on OCR1A

    TIMSK1 |= (1 << OCIE1A);
    //Set interrupt on compare match

    TCCR1B |= (1 << CS12) | (1 << CS10);
	
	
	//TCCR0A=0b00000000;
	//TIMSK0 |= (1<<TOIE0);
	//TCNT0=0;
	//TCCR0B |= (1 << CS00)|(1 << CS01);
	//TCCR0B |= (1 << CS00)|(1 << CS02);
    sei();
	
}*/

void Relay_Normal_Init(RelayNormal * relay, uint8_t N)
{
	relay->N = N;
	relay->isRelayOn[0] = 0;
	relay->isRelayOn[1] = 0;
	relay->isRelayOn[2] = 0;
	
	On(RELAY_DDR, relay->N);
	RELAY_OFF(relay->N);
}
void Relay_Normal_Process(RelayNormal * relay, AllSettings * allSettings, float * temp)
{
	Settings * setting;
	uint8_t i;
	for (i=0; i<3; i++)
	{
		setting = &allSettings->settings[relay->N][i];
		if (!setting->on)
		{
			relay->isRelayOn[i] = 0;
			continue;				
		}
		if (setting->direction)//UP
		{
			if (temp[i]<(setting->temp))
			{
				//RELAY_OFF( relay);
				relay->isRelayOn[i] = 0;
			}
			if (temp[i]>(setting->temp+setting->d))
			{
				relay->isRelayOn[i] = 1;
			}
		}else
		{
			if (temp[i]<(setting->temp-setting->d))
			{
				relay->isRelayOn[i] = 1;
			}
			if (temp[i]>(setting->temp))
			{
				relay->isRelayOn[i] = 0;
			}
		}
	}
	uint8_t s = allSettings->settings[relay->N][0].on|allSettings->settings[relay->N][1].on|allSettings->settings[relay->N][2].on;//0 if all are turned off
	for (int i=0; i<3; i++)
	{
		if (!allSettings->settings[relay->N][i].on)
		{
			continue;
		}		
		if (!relay->isRelayOn[i])
			s = 0;
	}
	if (s)
		RELAY_ON( relay->N );
	else
		RELAY_OFF( relay->N );	
}


void Relay_PWM_Init(RelayPWM * relay, AllSettings * allSettings, uint8_t N)
{
	relay->N = N;
	relay->allSettings = allSettings;
	Relay_PWM_Find_Active_Settings(relay);
	
	relay->isRelayOn[0] = 0;
	relay->isRelayOn[1] = 0;
	relay->isRelayOn[2] = 0;
	
	relay->duty = 0;
	relay->state = STATE0;
	
	Relay_PWM_Reset_Counter_Cycle(relay);
	Relay_PWM_Reset_Counter_M(relay);
	
	On(RELAY_DDR, relay->N);
	RELAY_OFF(relay->N);
}
void Relay_PWM_Process(RelayPWM * relay, float * temp, uint8_t PWMSignal)
{
	/*Relay_PWM_Set_Duty_Cicle(relay, 25);
	return;*/
	switch (relay->state)
	{
		case STATE0:
			
		break;		
	}
}

void Relay_PWM_Interrupt(RelayPWM * relay)
{
	PWMSettings * pwmSetting = Relay_PWM_GetCurrentPWMSettings(relay);
	if (pwmSetting==NULL)
		return;
		
		
	relay->counter_cycle2++;
	if (relay->counter_cycle2==N_PARTS*60)
	{
		relay->counter_minute++;
		relay->counter_cycle2 = 0;
	}
		
	relay->counter_cycle++;
	if (relay->counter_cycle>(N_PARTS*pwmSetting->tpwm))
		relay->counter_cycle = 1;
	if (relay->counter_cycle<=relay->duty*pwmSetting->tpwm)
		RELAY_ON( relay->N);
	else
		RELAY_OFF( relay->N );
	
}

void Relay_PWM_Reset_Counter_Cycle(RelayPWM * relay)
{
	relay->counter_cycle = 0;
}
void Relay_PWM_Reset_Counter_M(RelayPWM * relay)
{
	relay->counter_cycle2 = 0;
	relay->counter_minute = 0;
}

PWMSettings * Relay_PWM_GetCurrentPWMSettings(RelayPWM * relay)
{
	if (relay->activeSettings==0xFF)
		return NULL;
	return &relay->allSettings->PWMSettings[relay->activeSettings];	
}

void Relay_PWM_Find_Active_Settings(RelayPWM * relay)
{
	relay->activeSettings = 0xFF;
	uint8_t i;
	for (i=0; i<3; i++)
	{
		if (relay->allSettings->PWMSettings[i].on)
		{
			relay->activeSettings = i;
			return;		
		}		
	}
}

void Relay_PWM_Set_Duty_Cicle(RelayPWM * relay, uint8_t duty)
{
	relay->duty = duty;	
}

/*void resetPWMParts(SettingsType all_settings)
{
		//pwmParts = setting->part;
		//pwmPartsD = setting->partD;
		uint8_t q = readPWMSignal();
		pwmParts = all_settings[0][q].pwmSettings.part;
		pwmPartsD = all_settings[0][q].pwmSettings.partD;
		//pwmParts = all_pwm_setting[q].part;
		//pwmPartsD = all_pwm_setting[q].partD;
		firstTime = 1;
}*/

int counter = 0;


/*ISR(TIMER1_COMPA_vect)
{
	
	if (pwmSetting==NULL)
	{
		RELAY_OFF( PWN_RELAY);
		return;
	}		
	
	counter++;
	if (counter>(N_PARTS*pwmSetting->period))
		counter = 1;
	if (counter<=pwmSetting->period*pwmParts)
		RELAY_ON( PWN_RELAY);
	else
		RELAY_OFF( PWN_RELAY);
}*/

/*void HandleRelay(SettingsType all_settings, float * temp, uint8_t relay)
{
	if (relay!=PWM_RELAY)
	{
		HandleRelayNormal(all_settings, temp, relay);
	}else
	{
		//HandleRelayPWM(all_settings, temp, relay);
	}			
}

void HandleRelayNormal(SettingsType all_settings, float * temp, uint8_t relay)
{
	Settings * setting;
	uint8_t i;
	for (i=0; i<3; i++)
	{
		setting = &all_settings[relay][i];
		if (!setting->on)
		{
			isRelayOn[relay][i] = 0;
			continue;				
		}
		if (setting->direction)//UP
		{
			if (temp[i]<(setting->temp))
			{
				//RELAY_OFF( relay);
				isRelayOn[relay][i] = 0;
			}
			if (temp[i]>(setting->temp+setting->d))
			{
				isRelayOn[relay][i] = 1;
			}
		}else
		{
			if (temp[i]<(setting->temp-setting->d))
			{
				isRelayOn[relay][i] = 1;
			}
			if (temp[i]>(setting->temp))
			{
				isRelayOn[relay][i] = 0;
			}
		}
	}
	//uint8_t s = isRelayOn[relay][0]|isRelayOn[relay][1]|isRelayOn[relay][2];
	uint8_t s = all_settings[relay][0].on|all_settings[relay][1].on|all_settings[relay][2].on;//0 if all are turned off
	for (int i=0; i<3; i++)
	{
		if (!all_settings[relay][i].on)
		{
			continue;
		}		
		if (!isRelayOn[relay][i])
			s = 0;
	}
	if (s)
		RELAY_ON( relay);
	else
		RELAY_OFF( relay);
}*/

/*void HandleRelayPWM(SettingsType all_settings, float * temp, uint8_t relay)
{
	Settings * setting;
	uint8_t q = readPWMSignal();
	setting = &all_settings[relay][q];
	uint8_t isOn = pwmSetting!=NULL;
		if (!setting->on)
		{
			pwmSetting = NULL;	
			pwmParts = 0;		
		}else
		{
			if (setting->direction)//UP
			{
				if (temp[0]<(setting->temp))
				{
					isRelayOn[relay][q] = 0;
				}
				if (temp[0]>(setting->temp+setting->d))
				{
					isRelayOn[relay][q] = 1;
				}
			}else
			{
				if (temp[0]<(setting->temp-setting->d))
				{
					isRelayOn[relay][q] = 1;
				}
				if (temp[0]>(setting->temp))
				{
					isRelayOn[relay][q] = 0;
				}
			}
			if (isRelayOn[relay][q])
			{
				
				pwmSetting = &setting->pwmSettings;
				if (!isOn)
				{
					if (!firstTime)
					{
						pwmParts = pwmParts-pwmPartsD;					
					}
					firstTime = 0;					
				}							
			}else
			{
				pwmSetting = NULL;				
			}	
			
		}		
}*/


uint8_t readPWMSignal()
{
	if (RELAY_PIN & (1<<PWN_SIGNAL))
	{
		return 1;		
	}
	return 0;
}

/*void HandleRelay(Settings * setting, float temp, uint8_t relay)
{
	if (setting->direction)//UP
	{
		if (temp<(setting->temp-setting->d))
		{
			RELAY_OFF( relay);
		}
		if (temp>(setting->temp))
		{
			RELAY_ON( relay);
		}
	}else
	{
		if (temp<(setting->temp))
		{
			RELAY_ON( relay);
		}
		if (temp>(setting->temp+setting->d))
		{
			RELAY_OFF( relay);
		}
	}
}*/
