#include "relay.h"

uint8_t isRelayOn[4][3] = {0};
PWMSettings * pwmSetting = NULL;
int8_t pwmParts = 0;
uint8_t pwmPartsD = 0;
uint8_t firstTime = 1;

void Relay_Init()
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
 
    /*// enable Timer1 overflow interrupt:
    TIMSK1 = (1 << TOIE1);
    // Set CS10 bit so timer runs at clock speed:
    TCCR1B |= (1 << CS10)|(1 << CS11);
	//TCCR1B |= (1 << CS12);
    // enable global interrupts:*/
	OCR1A = 0x0876;

    TCCR1B |= (1 << WGM12);
    // Mode 4, CTC on OCR1A

    TIMSK1 |= (1 << OCIE1A);
    //Set interrupt on compare match

    TCCR1B |= (1 << CS12) | (1 << CS10);
	
	
	/*TCCR0A=0b00000000;
	TIMSK0 |= (1<<TOIE0);
	TCNT0=0;
	//TCCR0B |= (1 << CS00)|(1 << CS01);
	TCCR0B |= (1 << CS00)|(1 << CS02);*/
    sei();
	
}

void resetPWMParts(SettingsType all_settings)
{
		//pwmParts = setting->part;
		//pwmPartsD = setting->partD;
		uint8_t q = readPWMSignal();
		pwmParts = all_settings[0][q].pwmSettings.part;
		pwmPartsD = all_settings[0][q].pwmSettings.partD;
		//pwmParts = all_pwm_setting[q].part;
		//pwmPartsD = all_pwm_setting[q].partD;
		firstTime = 1;
}

int counter = 0;

int qq = 0;

ISR(TIMER1_COMPA_vect)
{
	On(DDRB, 7);
	if (qq)
	{
		On(PORTB, 7);
	}else
	{
		Off(PORTB, 7);	
	}		
	qq = !qq;
	
	if (pwmSetting==NULL)
	{
		RELAY_OFF( PWN_RELAY);
		return;
	}		
	
	counter++;
	if (counter>(N_PARTS*pwmSetting->period))
		counter = 1;
	if (counter<=pwmSetting->period*pwmParts/*pwmSetting->part*/)
		RELAY_ON( PWN_RELAY);
	else
		RELAY_OFF( PWN_RELAY);
}

void HandleRelay(SettingsType all_settings, float * temp, uint8_t relay)
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
	if (relay!=PWN_RELAY)
	{
		uint8_t s = isRelayOn[relay][0]|isRelayOn[relay][1]|isRelayOn[relay][2];
		if (s)
			RELAY_ON( relay);
		else
			RELAY_OFF( relay);	
	}else
	{
		uint8_t isOn = pwmSetting!=NULL;
		if ((!all_settings[relay][0].on)&&(!all_settings[relay][1].on)&&(!all_settings[relay][2].on))
		{
			pwmSetting = NULL;	
			pwmParts = 0;		
		}else
		{
			uint8_t q = readPWMSignal();
			setting = &all_settings[relay][q];
			if ((setting->on)&&(isRelayOn[relay][q]))
			{
				
				pwmSetting = &all_settings[relay][q].pwmSettings;
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
	}			
}


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
