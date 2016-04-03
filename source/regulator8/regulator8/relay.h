#ifndef RELAY_H
#define RELAY_H

#include "defines.h"
#include "settings.h"
#include <avr/interrupt.h>

#define N_PARTS 30

#define RELAY_DDR DDRB
#define RELAY_PORT PORTB
#define RELAY_PIN PINB
#define RELAY1 0
#define RELAY2 1
#define RELAY3 2
#define RELAY4 3

#define PWN_RELAY RELAY4

void Relay_Init();
void HandleRelay(SettingsType all_settings, float * temp, uint8_t relay);

#endif

