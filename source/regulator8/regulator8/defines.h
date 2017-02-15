#ifndef DEFINES_H
#define DEFINES_H

#define F_CPU 16000000
//#define F_CPU 10000000


#include <avr/io.h>
#include <util/delay.h>
//#include "relay.h"
//#include "settings.h"

#define byte unsigned char

#define RELAY_DDR DDRB
#define RELAY_PORT PORTB
#define RELAY_PIN PINB
#define RELAY1 0
#define RELAY2 1
#define RELAY3 2
#define RELAY4 3

#define PWM_RELAY RELAY4

#define PWN_SIGNAL 5


#define Off(PORT, N) PORT &= ~(1<<N)
#define On(PORT, N) PORT |= 1<<N

#endif
