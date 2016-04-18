#include "relay.h"

uint8_t isRelayOn[4][3] = {0};
Settings * pwmSetting = NULL;
int8_t pwmParts = 0;
uint8_t pwmPartsD = 0;

void Relay_Init()
{
	On(RELAY_DDR, RELAY1);
	On(RELAY_DDR, RELAY2);
	On(RELAY_DDR, RELAY3);
	On(RELAY_DDR, RELAY4);

	Off(RELAY_PORT, RELAY1);
	Off(RELAY_PORT, RELAY2);
	Off(RELAY_PORT, RELAY3);
	Off(RELAY_PORT, RELAY4);
	
	//TIMSK1 = _BV(OCIE1A); //Enable Interrupt Timer/Counter 1,Output Compare A
	//TCCR1B = _BV(CS11) | _BV(WGM12); //Clock/8, 0.000008 secs/tick, Mode=CTC
	//OCR1A = 16; // SIG_COMPARE1A triggered every 0.000008*16 Seconds
	cli();             // disable global interrupts
    TCCR1A = 0;        // set entire TCCR1A register to 0
    TCCR1B = 0;
 
    // enable Timer1 overflow interrupt:
    TIMSK1 = (1 << TOIE1);
    // Set CS10 bit so timer runs at clock speed:
    TCCR1B |= (1 << CS10)|(1 << CS11);
	//TCCR1B |= (1 << CS10);
    // enable global interrupts:
    sei();
}

void resetPWMParts(Settings * setting)
{
		pwmParts = setting->part;
		pwmPartsD = setting->partD;
}

int counter = 0;

ISR(TIMER1_OVF_vect)
{
	if (pwmSetting==NULL)
	{
		Off(RELAY_PORT, PWN_RELAY);
		return;
	}		
	
	counter++;
	if (counter>(N_PARTS*pwmSetting->period))
		counter = 1;
	if (counter<=pwmSetting->period*pwmParts/*pwmSetting->part*/)
		On(RELAY_PORT, PWN_RELAY);
	else
		Off(RELAY_PORT, PWN_RELAY);
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
			if (temp[i]<(setting->temp-setting->d))
			{
				//Off(RELAY_PORT, relay);
				isRelayOn[relay][i] = 0;
			}
			if (temp[i]>(setting->temp))
			{
				isRelayOn[relay][i] = 1;
			}
		}else
		{
			if (temp[i]<(setting->temp))
			{
				isRelayOn[relay][i] = 1;
			}
			if (temp[i]>(setting->temp+setting->d))
			{
				isRelayOn[relay][i] = 0;
			}
		}
	}
	if (relay!=PWN_RELAY)
	{
		uint8_t s = isRelayOn[relay][0]|isRelayOn[relay][1]|isRelayOn[relay][2];
		if (s)
			On(RELAY_PORT, relay);
		else
			Off(RELAY_PORT, relay);	
	}else
	{
		uint8_t isOn = pwmSetting!=NULL;
		if ((!all_settings[relay][0].on)&&(!all_settings[relay][1].on)&&(!all_settings[relay][2].on))
		{
			pwmSetting = NULL;	
			pwmParts = 0;		
		}else
		{

			setting = &all_settings[relay][0];
			if ((setting->on)&&(isRelayOn[relay][0]))
			{

				pwmSetting = setting;
				if (!isOn)
				{
					pwmParts = pwmParts-pwmPartsD;					
				}							
			}else
			{
				pwmSetting = NULL;				
			}	
			
		}				
	}			
}

/*void HandleRelay(Settings * setting, float temp, uint8_t relay)
{
	if (setting->direction)//UP
	{
		if (temp<(setting->temp-setting->d))
		{
			Off(RELAY_PORT, relay);
		}
		if (temp>(setting->temp))
		{
			On(RELAY_PORT, relay);
		}
	}else
	{
		if (temp<(setting->temp))
		{
			On(RELAY_PORT, relay);
		}
		if (temp>(setting->temp+setting->d))
		{
			Off(RELAY_PORT, relay);
		}
	}
}*/
