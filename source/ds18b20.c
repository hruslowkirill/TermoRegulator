#include "ds18b20.h"

int DS_isPresent(byte DSBIT)
{
	byte r = 0;
	DSDDR |= 1<<DSBIT;
	DSPORT &= ~(1<<DSBIT);
	_delay_us(500);
	DSDDR &= ~(1<<DSBIT);
	_delay_us(70);
	if (DSPIN&(1<<DSBIT))
	{
		r = 0;
	}
	r = 1;
	_delay_us(420);
	return r;
}

void DS_Init(byte DSBIT)
{
	if (DS_isPresent(DSBIT))
	{
		_delay_ms(1);
		//DS_Send_Byte(0x33);
		DS_Send_Byte(DSBIT, 0xCC);
		DS_Send_Byte(DSBIT, 0x44);
		_delay_ms(1000);
		//while (!DS_Read_Bit()){}
	}
}

uint16_t DS_getTemperature(byte DSBIT)
{
	if (DS_isPresent(DSBIT))
	{
		//LCD_Clear_Display();
		//_delay_ms(1);
		//DS_Send_Byte(0x33);
		DS_Send_Byte(DSBIT, 0xCC);
		DS_Send_Byte(DSBIT, 0xBE);
		/*for (int i=0; i<8; i++)
		{
			byte b = DS_Read_Byte();
			LCD_Show_Byte(b);
		}*/
		
		byte b0 = DS_Read_Byte(DSBIT);
		byte b1 = DS_Read_Byte(DSBIT);
		byte b2 = DS_Read_Byte(DSBIT);
		byte b3 = DS_Read_Byte(DSBIT);
		byte b4 = DS_Read_Byte(DSBIT);
		byte b5 = DS_Read_Byte(DSBIT);
		byte b6 = DS_Read_Byte(DSBIT);
		byte b7 = DS_Read_Byte(DSBIT);
		byte b8 = DS_Read_Byte(DSBIT);
		//float temp = 0;
		uint16_t tempInt = (b1<<8)+b0;
		//temp = (tempInt>>4)&0x7F;
		return (b1<<8)+b0;
	}
	return 0;
}

float DS_getFloatTemperature(byte DSBIT)
{
	float res;
	uint16_t temp = DS_getTemperature(DSBIT);
	float sign = 1;
	if ((temp&(1<<15))!=0)
	{
		sign = -1;
		temp = 0xFFFF-(unsigned int)temp;
		temp++;
	}
	
	int drob = temp&0x0F;
	drob = drob * 625;
	int ss = drob % 1000;
	drob = drob / 1000;
	if (ss>=500)
		drob++;
	temp = (temp>>4)&0x7F;
	res = temp+(drob/10.0);
	res = res*sign;
	return res;
}

void DS_Send_Byte(byte DSBIT, byte b)
{
	for (int i=0; i<8; i++)
	{
		if (b&0x01)
		{
			DSDDR |= 1<<DSBIT;
			DSPORT &= ~(1<<DSBIT);
			_delay_us(5);
			DSDDR &= ~(1<<DSBIT);
			_delay_us(55);
		}else
		{
			DSDDR |= 1<<DSBIT;
			DSPORT &= ~(1<<DSBIT);
			_delay_us(60);
			DSDDR &= ~(1<<DSBIT);
			//_delay_us(10);
		}
		b = b >> 1;
	}
}

byte DS_Read_Bit(byte DSBIT)
{
	byte r = 0;
		DSDDR |= 1<<DSBIT;
		DSPORT &= ~(1<<DSBIT);
		_delay_us(1);
		DSDDR &= ~(1<<DSBIT);
		_delay_us(10);
		if (DSPIN&(1<<DSBIT))
		{
			r = 1;
		}
		_delay_us(50);
		return r;
}

byte DS_Read_Byte(byte DSBIT)
{
	byte b = 0;
	for (int i=0; i<8; i++)
	{
		b = b >> 1;
		b = b | (DS_Read_Bit(DSBIT)<<7);
	}
	return b;
}
