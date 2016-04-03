#ifndef DEFINES_H
#define DEFINES_H

#define F_CPU 16000000


#include <avr/io.h>
#include <util/delay.h>

#define byte unsigned char


#define Off(PORT, N) PORT &= ~(1<<N)
#define On(PORT, N) PORT |= 1<<N

#endif
