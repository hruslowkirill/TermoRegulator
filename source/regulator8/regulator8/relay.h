#ifndef RELAY_H
#define RELAY_H

#include "defines.h"
#include <avr/interrupt.h>
#include "settings.h"

#define N_PARTS 30

#define RELAY_ON(N) Off(RELAY_PORT, N) 
#define RELAY_OFF(N) On(RELAY_PORT, N)


void Relay_Init();
void HandleRelay(SettingsType all_settings, float * temp, uint8_t relay);
void resetPWMParts(SettingsType all_settings);

uint8_t readPWMSignal();


//UP - heat
//Down - cold

#endif

