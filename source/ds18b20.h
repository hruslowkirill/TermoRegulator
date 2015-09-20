#ifndef DS18B20_H
#define DS18B20_H

#include "defines.h"

#define DSDDR DDRC
#define DSPORT PORTC
#define DSPIN PINC
#define DSBIT1 5
#define DSBIT2 4
#define DSBIT3 3

int DS_isPresent(byte DSBIT);
void DS_Init(byte DSBIT);
uint16_t DS_getTemperature(byte DSBIT);
float DS_getFloatTemperature(byte DSBIT);
void DS_Send_Byte(byte DSBIT, byte b);
byte DS_Read_Bit(byte DSBIT );
byte DS_Read_Byte(byte DSBIT);	
	
#endif
