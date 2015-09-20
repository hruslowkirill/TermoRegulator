#include "relay.h"

void Relay_Init()
{
	On(RELAY_DDR, RELAY1);
	On(RELAY_DDR, RELAY2);
	On(RELAY_DDR, RELAY3);

	Off(RELAY_PORT, RELAY1);
	Off(RELAY_PORT, RELAY2);
	Off(RELAY_PORT, RELAY3);
}

void HandleRelay(Settings * setting, float temp, uint8_t relay)
{
	if (setting->direction)
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
		if (temp<(setting->temp-setting->d))
		{
			On(RELAY_PORT, relay);
		}
		if (temp>(setting->temp))
		{
			Off(RELAY_PORT, relay);
		}
	}
}
