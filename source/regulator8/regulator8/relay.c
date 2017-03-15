#include "relay.h"


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
	
	Relay_PWM_Set_Duty_Cicle(relay, 0);
	relay->state = STATE0;
	
	relay->Tstart = 0;
	relay->Tstop = 0;
	
	Relay_PWM_Reset_Counter_Cycle(relay);
	Relay_PWM_Reset_Counter_M(relay);
	
	On(RELAY_DDR, relay->N);
	RELAY_OFF(relay->N);
}
void Relay_PWM_Process(RelayPWM * relay, float * temp, uint8_t overflowSignal)
{
	/*Relay_PWM_Set_Duty_Cicle(relay, 25);
	return;*/
		
	PWMSettings * pwmSettings = Relay_PWM_GetCurrentPWMSettings(relay);
	if (pwmSettings==NULL)
		return;
	
	float td = temp[relay->activeSettings];
	
	switch (relay->state)
	{
		case STATE0:
			Relay_PWM_Set_Duty_Cicle(relay, 0);
			relay->state = STATE1;
		break;	
		
		case STATE1:
			if (td >= pwmSettings->Tzap)
			{
				Relay_PWM_Reset_Counter_M(relay);
				relay->state = STATE2;				
			}
		break;	
		
		case STATE2:
			if (relay->counter_minute>=pwmSettings->t3)
			{
				relay->state = STATE3;				
			}
		break;
		
		case STATE3:
			if (overflowSignal)
			{
				relay->state = STATE5;
				relay->Tstop = td+pwmSettings->Tstop;
				relay->Tstart = td+pwmSettings->Tstop-pwmSettings->Tstart;
			}else
			{
				Relay_PWM_Set_Duty_Cicle(relay, pwmSettings->Sgshim);
			}
		break;
		
		case STATE5:
			Relay_PWM_Set_Duty_Cicle(relay, pwmSettings->Scshim);
			if (overflowSignal)
			{
				if (td>=pwmSettings->Tsliv)
				{
					relay->state = STATE6;
				}
			}else
			{
				relay->state = STATE0;
			}
		break;
		
		case STATE6:
			Relay_PWM_Set_Duty_Cicle(relay, 0);
			if (overflowSignal)
			{
				if (!(td > relay->Tstart))
				{
					relay->state = STATE7;
				}
			}else
			{
				relay->state = STATE0;
			}
		break;
		
		case STATE7:
			Relay_PWM_Set_Duty_Cicle(relay, relay->Sshim);
			if (overflowSignal)
			{
				if (td >= relay->Tstop)
				{
					relay->state = STATE8;
				}
			}else
			{
				relay->state = STATE0;
			}
		break;
		
		case STATE8:
			relay->Sshim = relay->Sshim - pwmSettings->SshimD;
			if (relay->Sshim < 1)
			{
				relay->Sshim = 1;				
			}
			Relay_PWM_Set_Duty_Cicle(relay, 0);
			relay->state = STATE6;
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
			relay->Sshim = relay->allSettings->PWMSettings[i].Sshim;
			return;		
		}		
	}
}

void Relay_PWM_Set_Duty_Cicle(RelayPWM * relay, uint8_t duty)
{
	relay->duty = duty;	
}



int counter = 0;



uint8_t readOverflowSignal()
{
	Off(RELAY_DDR, OVERFLOW_SIGNAL);
	if (RELAY_PIN & (1<<OVERFLOW_SIGNAL))
	{
		return 1;		
	}
	return 0;
}
