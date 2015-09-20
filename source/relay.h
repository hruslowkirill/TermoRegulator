#ifndef RELAY_H
#define RELAY_H

#include "defines.h"
#include "settings.h"

#define RELAY_DDR DDRB
#define RELAY_PORT PORTB
#define RELAY1 2
#define RELAY2 1
#define RELAY3 0

void Relay_Init();
void HandleRelay(Settings * setting, float temp, uint8_t relay);

#endif

