
#include "defines.h"
#include "lcd.h"
#include "ds18b20.h"
#include "settings.h"
#include "relay.h"

#define BTNDDR DDRC
#define BTNPORT PORTC
#define BTNPIN PINC

#define BTN1 0
#define BTN2 2
#define BTN3 1

#define BTN_long_time 1000

#define LED_OFF_TIME 10000



#define STATUS_DEFAULT 0
#define STATUS_MAIN_MAENU 10
#define STATUS_SETTINGS 20

Settings setting1;
Settings setting2;
Settings setting3;

int32_t BTN1_status;
int32_t BTN2_status;
int32_t BTN3_status;
uint32_t Led_On_time;

void BTN_Init();
void BTN_Process();
void BTN1_Pressed();
void BTN2_Pressed();
void BTN3_Pressed();
void BTN3_Long_Pressed();


void DoWork();
void ShowMainMenu();
void ShowSettings();

uint8_t current_status = 0;
uint8_t sub_current_status = 0;

uint8_t settings_item1;
uint8_t settings_item2;

uint32_t iterations = 0;

uint8_t blinker = 0;

Settings * settings;

int main()
{
	
	int turn = 0;
	LCD_Init();
	BTN_Init();
	Settings_Init();
	Relay_Init();

	ledON;
	LCD_Clear_Display();
	

	
	while (1)
	{
		BTN_Process();
		_delay_ms(1);
		//LCD_Clear_Display();
		switch(current_status)
		{
			case STATUS_DEFAULT:
				DoWork();
			break;
			case STATUS_MAIN_MAENU:
				ShowMainMenu();
			break; 
			case STATUS_SETTINGS:
				ShowSettings();
			break; 
		}
		

		


	}
	return 0;
}

void DoWork()
{

	//LCD_Clear_Display();
	if (iterations%500!=0)
		{
			iterations++;
			//_delay_ms(1);
			return;
		}
	//LCD_Clear_Display();

	
	//LCD_printStr("Waiting...");
	//return ;
	//DS_Init(DSBIT1);
	DS_Init(DSBIT1);
	DS_Init(DSBIT2);
	DS_Init(DSBIT3);

		float t1 = DS_getFloatTemperature(DSBIT1);


		//DS_Init(DSBIT2);
		float t2 = DS_getFloatTemperature(DSBIT2);


		//DS_Init(DSBIT3);
		float t3 = DS_getFloatTemperature(DSBIT3);

		HandleRelay(&setting1, t1, RELAY1);
		HandleRelay(&setting2, t2, RELAY2);
		HandleRelay(&setting3, t3, RELAY3);

		LCD_2buffer_begin();
		//LCD_Clear_Display();
		LCD_2buffer_Move_Cursor(0);
		LCD_2buffer_Show_FloatTemperature1(t1);

		LCD_2buffer_Move_Cursor(8);
		LCD_2buffer_Show_FloatTemperature1(t2);

		LCD_2buffer_Move_Cursor(16);
		LCD_2buffer_Show_FloatTemperature1(t3);
		LCD_2buffer_end();
		iterations = 1;
		_delay_us(100);
}

void ShowMainMenu()
{
	if (iterations%12000!=0)
		{
			iterations++;
			//_delay_ms(1);
			return;
		}
	LCD_2buffer_begin();
	LCD_2buffer_Move_Cursor(1);
	LCD_2buffer_printStr("Temp1");

	LCD_2buffer_Move_Cursor(9);
	LCD_2buffer_printStr("Temp2");

	LCD_2buffer_Move_Cursor(17);
	LCD_2buffer_printStr("Temp3");

	switch(	sub_current_status)
	{
		case 0:
			LCD_2buffer_Move_Cursor(0);
		break;

		case 1:
			LCD_2buffer_Move_Cursor(8);
		break;

		case 2:
			LCD_2buffer_Move_Cursor(16);
		break;
	}
	
	LCD_2buffer_printStr(">");

	LCD_2buffer_end();
	//LCD_Clear_Display();
	//LCD_printStr("HELLO");
	//_delay_ms(1);
	iterations = 1;
}

void ShowSettings()
{
	if (iterations%500!=0)
		{
			iterations++;
			//_delay_ms(1);
			return;
		}

	
	switch(sub_current_status)
	{
		case 0: settings = &setting1; break;	
		case 1: settings = &setting2; break;
		case 2: settings = &setting3; break;	
	}

	blinker = !blinker;

	LCD_2buffer_begin();
	if ((blinker)||(settings_item2!=1))
	{
		LCD_2buffer_Move_Cursor(1);
		LCD_2buffer_Show_FloatTemperature1(settings->temp);
	}
	
	if ((blinker)||(settings_item2!=2))
	{
		LCD_2buffer_Move_Cursor(9);
		LCD_2buffer_Show_FloatTemperature1(settings->d);
	}
	if ((blinker)||(settings_item2!=3))
	{
		LCD_2buffer_Move_Cursor(17);
		if(settings->direction)
		{
			LCD_2buffer_printStr("UP");
		}else
		{
			LCD_2buffer_printStr("DOWN");
		}
	}

	switch(	settings_item1)
	{
		case 0:
			LCD_2buffer_Move_Cursor(0);
		break;

		case 1:
			LCD_2buffer_Move_Cursor(8);
		break;

		case 2:
			LCD_2buffer_Move_Cursor(16);
		break;
	}
	LCD_2buffer_printStr(">");
	LCD_2buffer_end();
	iterations = 1;
}

void BTN1_Pressed()
{
	switch(current_status)
	{
		case STATUS_MAIN_MAENU:
			if(sub_current_status==0)
				sub_current_status = 3;
			sub_current_status--;
			
		break;
		case STATUS_SETTINGS:
			if (settings_item2==0)
			{
				if(settings_item1==0)
					settings_item1 = 3;
				settings_item1--;
			}
			if (settings_item2==1)
			{
				settings->temp-=0.1;
			}
			if (settings_item2==2)
			{
				settings->d-=0.1;
			}
			if (settings_item2==3)
			{
				settings->direction = !settings->direction;
			}
		break;
	}
	iterations = 0;
}
void BTN2_Pressed()
{
	switch(current_status)
	{
		case STATUS_MAIN_MAENU:
			sub_current_status++;
			if(sub_current_status==3)
				sub_current_status = 0;
			
		break;
		case STATUS_SETTINGS:
			if (settings_item2==0)
			{	
				settings_item1++;
				if(settings_item1==3)
					settings_item1 = 0;
			}
			if (settings_item2==1)
			{
				settings->temp += 0.1;
			}
			if (settings_item2==2)
			{
				settings->d+= 0.1;
			}
			if (settings_item2==3)
			{
				settings->direction = !settings->direction;
			}
		break;
	}
	iterations = 0;
}
void BTN3_Pressed()
{
	switch(current_status)
	{
		case STATUS_MAIN_MAENU:
			current_status = STATUS_SETTINGS;
			iterations = 0;
			settings_item1 = 0;
			settings_item2 = 0;
		break;

		case STATUS_SETTINGS:
			if (settings_item2==0)
			{
				settings_item2 = settings_item1+1;
			}else
			{	
				settings_item2 = 0;
				Settings_Write(sub_current_status+1);
			}
		break;
	}
}

void BTN1_Long_Pressed()
{
	switch(current_status)
	{
		case STATUS_SETTINGS:
			if (settings_item2==1)
			{
				settings->temp -= 5;
			}
			if (settings_item2==2)
			{
				settings->d -= 5;
			}
		break;
	}
}

void BTN2_Long_Pressed()
{
	switch(current_status)
	{
		case STATUS_SETTINGS:
			if (settings_item2==1)
			{
				settings->temp += 5;
			}
			if (settings_item2==2)
			{
				settings->d += 5;
			}

		break;
	}
}

void BTN3_Long_Pressed()
{
	//LCD_printStr("HELLO");
	if (current_status==STATUS_DEFAULT)
	{
		current_status = STATUS_MAIN_MAENU;
		sub_current_status = 0;
		
	}else
	{
		current_status = STATUS_DEFAULT;
		
	}
	iterations=0;
}

void BTN_Init()
{
	BTN1_status = 0;
	BTN2_status = 0;
	BTN3_status = 0;

	Off(BTNDDR, BTN1); 
	Off(BTNDDR, BTN2); 
	Off(BTNDDR, BTN3); 

	On(BTNPORT, BTN1); 
	On(BTNPORT, BTN2); 
	On(BTNPORT, BTN3); 

	//Led_On_time = 0;
}

void BTN_Process()
{
	if ((BTNPIN & (1<<BTN1))==0)
	{
		if (BTN1_status>=0)
			BTN1_status ++;
		//Led_On_time = 0;
	}

	if ((BTNPIN & (1<<BTN2))==0)
	{
		if (BTN2_status>=0)
			BTN2_status ++;
		//Led_On_time = 0;
	}

	if ((BTNPIN & (1<<BTN3))==0)
	{
		if (BTN3_status>=0)
			BTN3_status ++;
		//Led_On_time = 0;
	}

	if (BTN1_status>BTN_long_time)
	{
		if (BTN1_status>=0)
			BTN1_Long_Pressed();
		BTN1_status = -1;
	}
	
	if (BTN2_status>BTN_long_time)
	{
		if (BTN2_status>=0)
			BTN2_Long_Pressed();
		BTN2_status = -1;
	}

	if (BTN3_status>BTN_long_time)
	{
		if (BTN3_status>=0)
			BTN3_Long_Pressed();
		BTN3_status = -1;
	}

	if ((BTNPIN & (1<<BTN1))!=0)
	{
		if ((BTN1_status>0)&&(BTN1_status<BTN_long_time))
		{
			BTN1_Pressed();
		}
		BTN1_status = 0;
	}

	if ((BTNPIN & (1<<BTN2))!=0)
	{
		if ((BTN2_status>0)&&(BTN2_status<BTN_long_time))
		{
			BTN2_Pressed();
		}
		BTN2_status = 0;
	}

	if ((BTNPIN & (1<<BTN3))!=0)
	{
		if ((BTN3_status>0)&&(BTN3_status<BTN_long_time))
		{
			BTN3_Pressed();
		}
		BTN3_status = 0;
	}
	/*Led_On_time++;
	if (Led_On_time>LED_OFF_TIME)
	{
		ledOFF;
	}else
	{
		ledON;
	}*/
}
