#ifndef RELAY_H
#define RELAY_H

#include "defines.h"
#include <avr/interrupt.h>
#include "settings.h"

#define N_PARTS 30


void Relay_Init();
void HandleRelay(SettingsType all_settings, float * temp, uint8_t relay);
void resetPWMParts(Settings * setting);

#endif

