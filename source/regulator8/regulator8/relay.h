#ifndef RELAY_H
#define RELAY_H

#include "defines.h"
#include <avr/interrupt.h>
#include "settings.h"

#define N_PARTS 50

#define RELAY_ON(N) Off(RELAY_PORT, N) 
#define RELAY_OFF(N) On(RELAY_PORT, N)

typedef struct
{
	uint8_t N;
	uint8_t isRelayOn[3];
}RelayNormal; //10

void Relay_Normal_Init(RelayNormal * relay, uint8_t N);
void Relay_Normal_Process(RelayNormal * relay, AllSettings * allSettings, float * temp);



void Relay_Init();
/*void HandleRelay(SettingsType all_settings, float * temp, uint8_t relay);
void HandleRelayNormal(SettingsType all_settings, float * temp, uint8_t relay);

void HandleRelayPWM(SettingsType all_settings, float * temp, uint8_t relay);
void resetPWMParts(SettingsType all_settings);*/

uint8_t readPWMSignal();


//UP - heat
//Down - cold

#endif

