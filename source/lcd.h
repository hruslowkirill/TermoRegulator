#ifndef LCD_H
#define LCD_H

#include "defines.h"

//#define LCD_2ROW_OFFSET 40 //real device

#define LCD_2ROW_OFFSET 64 //simulation


#define CONTROL_DDR	 	 DDRD
#define CONTROL_PORT 	 PORTD
#define Enable_Pin		  5
#define RegSelect_Pin     7
#define ReadWrite_Pin     6
#define Led_Pin           4

#define Set_Enable  				CONTROL_PORT|=_BV(Enable_Pin)
#define Clear_Enable 				CONTROL_PORT&=~_BV(Enable_Pin)
#define Write_Lcd			    	CONTROL_PORT&=~_BV(ReadWrite_Pin)
#define Read_Lcd			   		CONTROL_PORT|=_BV(ReadWrite_Pin)
#define Select_InstructionRegister 	CONTROL_PORT&=~_BV(RegSelect_Pin)
#define Select_DataRegister	    	CONTROL_PORT|=_BV(RegSelect_Pin)
#define ledON                       CONTROL_PORT|=_BV(Led_Pin)
#define ledOFF                      CONTROL_PORT&=~_BV(Led_Pin)



void LCD_Show_FloatTemperature1(float f);
void LCD_Show_FloatTemperature(uint16_t f);
void LCD_Print_Number(int f);
void LCD_Show_Byte(byte b);
void LCD_Show_Bytes(int d);
char LCD_getChar(byte d);
void LCD_Init(void);
void LCD_Move_Cursor(byte position);
void LCD_Clear_Display();
void LCD_Send_Data(byte q);
void LCD_Send_Instruction(byte q);
void LCD_printStr(char * s);

void LCD_2buffer_begin();
void LCD_2buffer_end();
void LCD_2buffer_Print_Number(int16_t f);
void LCD_2buffer_Show_FloatTemperature(uint16_t f);
void LCD_2buffer_printStr(char * s);
void LCD_2buffer_Move_Cursor(byte position);
void LCD_2buffer_Show_FloatTemperature1(float f);

#endif
