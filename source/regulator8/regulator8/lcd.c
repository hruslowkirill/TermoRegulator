#include "lcd.h"

char LCD_buffer[32];
uint8_t LCD_pointer = 0;

/*void LCD_Show_FloatTemperature1(float f)
{
	if (f<0)
	{
		LCD_Send_Data('-');
		f = -f;
	}else
	{
		LCD_Send_Data('+');
	}
	uint16_t first = (uint16_t)f;
	uint16_t second = ((uint16_t)(f*10))%10;
	LCD_Print_Number(first);
	LCD_Send_Data('.');
	LCD_Print_Number(second);
}*/


/*void LCD_Show_FloatTemperature(uint16_t f)
{
	if ((f&(1<<15))!=0)
	{
		LCD_Send_Data('-');
		f = 0xFFFF-(unsigned int)f;
		f++;
	}else
	{
		LCD_Send_Data('+');
	}
	int drob = f&0x0F;
	drob = drob * 625;
	int ss = drob % 1000;
	drob = drob / 1000;
	if (ss>=500)
		drob++;
	f = (f>>4)&0x7F;
	LCD_Print_Number(f);
	LCD_Send_Data('.');
	LCD_Send_Data(drob+'0');
}*/

/*void LCD_Print_Number(int f)
{
	byte buffer[4];
	int n = 0;
	if (f==0)
	{
		LCD_Send_Data('0');
	}
	while (f>0)
	{
		buffer[n++] = f%10;
		f = f/10;
	}
	for (int i = n-1; i>=0; i--)
	{
		LCD_Send_Data(buffer[i]+'0');
	}
}*/

/*void LCD_Show_Byte(byte b)
{
	char data[3];
	data[2] = 0x00;
	for (int i=1; i>=0; i--)
	{
		data[i] = LCD_getChar(b&0x0F);
		b = b>>4;
	}
	LCD_printStr(data);
}
*/
/*void LCD_Show_Bytes(int d)
{
	//LCD_Clear_Display();
	LCD_Show_Byte((d>>8)&0xFF);
	LCD_Show_Byte(d&0xFF);
}*/

/*char LCD_getChar(byte d)
{
	if (d<10)
	{
		return d+'0';
	}
	return d+'A'-10;
}*/

void LCD_Init(void)
{
	CONTROL_DDR = 0xFF;
	CONTROL_PORT = 0x00;
	_delay_ms(30);
	Select_InstructionRegister;
	Write_Lcd;	
	CONTROL_PORT&= 0xF0;
	CONTROL_PORT|= 0x03;
	_delay_ms(5);
	CONTROL_PORT&= 0xF0;
	CONTROL_PORT|= 0x03;
	_delay_us(250);

	CONTROL_PORT&= 0xF0;
	CONTROL_PORT|= 0x03;
	_delay_ms(5);

	CONTROL_PORT&= 0xF0;
	CONTROL_PORT|= 0x02;
	//_delay_ms(6);
	Set_Enable;
	_delay_ms(6);
	Clear_Enable;
	LCD_Send_Instruction(0x28);
	LCD_Send_Instruction(0x08);
	LCD_Send_Instruction(0x01);
	LCD_Send_Instruction(0x06);
	LCD_Send_Instruction(0x0C);
}

void LCD_Clear_Display()
{
	LCD_Send_Instruction(0x01);
}

void LCD_Move_Cursor(byte position)
{
	byte p = 128;
	if (position>15)
	{
		p = p+LCD_2ROW_OFFSET;//40
		position = position-16;
	}
	p = p+position;
	LCD_Send_Instruction(p);
}

void LCD_Send_Data_Instruction(byte q)
{
	Write_Lcd;	
	CONTROL_PORT&= 0xF0;
	CONTROL_PORT|= (q>>4)&0x0F;
	Set_Enable;
	_delay_ms(1);
	Clear_Enable;
	_delay_ms(6);
	CONTROL_PORT&= 0xF0;
	CONTROL_PORT|= q&0x0F;
	Set_Enable;
	_delay_ms(1);
	Clear_Enable;
}


void LCD_Send_Data(byte q)
{
	Select_DataRegister;
	/*Write_Lcd;	
	CONTROL_PORT&= 0xF0;
	CONTROL_PORT|= (q>>4)&0x0F;
	Set_Enable;
	_delay_ms(1);
	Clear_Enable;
	_delay_us(500);
	CONTROL_PORT&= 0xF0;
	CONTROL_PORT|= q&0x0F;
	Set_Enable;
	_delay_ms(1);
	Clear_Enable;*/
	LCD_Send_Data_Instruction(q);
}

void LCD_Send_Instruction(byte q)
{
	Select_InstructionRegister;
	/*Write_Lcd;
	CONTROL_PORT&= 0xF0;
	CONTROL_PORT|= (q>>4)&0x0F;
	Set_Enable;
	_delay_ms(1);
	Clear_Enable;
	_delay_ms(6);
	CONTROL_PORT&= 0xF0;
	CONTROL_PORT|= q&0x0F;
	Set_Enable;
	_delay_ms(1);
	Clear_Enable;*/
	LCD_Send_Data_Instruction(q);
	_delay_ms(6);	
}

void LCD_printStr(char * s)
{
	while (*s)
	{
		LCD_Send_Data(*s);
		s++;
	}
}

void LCD_2buffer_begin()
{
	memset(LCD_buffer, ' ', 32);
	LCD_pointer = 0;
}
void LCD_2buffer_end()
{	
	int i;
	LCD_Move_Cursor(0);
	for (i=0; i<16; i++)
	{	
		LCD_Send_Data(LCD_buffer[i]);
	}

	LCD_Move_Cursor(16);
	for (i=0; i<16; i++)
	{	
		LCD_Send_Data(LCD_buffer[16+i]);
	}
}

void LCD_2buffer_printStr(char * s)
{
	while (*s)
	{
		LCD_buffer[LCD_pointer++] = *s;
		s++;
	}
}

void LCD_2buffer_Move_Cursor(byte position)
{
	LCD_pointer = position;
}

void LCD_2buffer_Print_Number(int16_t f)
{
	byte buffer[6];
	int n = 0;
	if (f==0)
	{
		LCD_buffer[LCD_pointer++] = '0';
		return;
	}
	if (f<0)
	{
		LCD_buffer[LCD_pointer++] = '-';
		f = -f;
	}
	while (f>0)
	{
		buffer[n++] = f%10;
		f = f/10;
	}
	for (int i = n-1; i>=0; i--)
	{
		LCD_buffer[LCD_pointer++] = buffer[i]+'0';
		//LCD_Send_Data(buffer[i]+'0');
	}
}

void LCD_2buffer_Show_FloatTemperature1(float f)
{
	if (f<0)
	{
		LCD_buffer[LCD_pointer++]='-';
		f = -f;
	}else
	{
		LCD_buffer[LCD_pointer++]='+';
	}
	uint16_t first = (uint16_t)f;
	uint16_t second = ((uint16_t)(f*10))%10;
	LCD_2buffer_Print_Number(first);
	LCD_buffer[LCD_pointer++]='.';
	LCD_2buffer_Print_Number(second);
}
