
#include "defines.h"
#include "lcd.h"
#include "ds18b20.h"
#include "relay.h"
#include "settings.h"
#include "menu.h"

#define BTNDDR DDRC
#define BTNPORT PORTC
#define BTNPIN PINC

#define BTN1 0
#define BTN2 2
#define BTN3 1
#define IS_BTN_PRESSED(BTN)  ((BTNPIN & (1<<BTN))==0)

#define BTN_long_time 1000

#define LED_OFF_TIME 10000



#define STATUS_DEFAULT 0
#define STATUS_TERM 10
#define STATUS_SETTINGS 20
#define STATUS_RELAY 30
#define STATUS_RESET 40

//#define NUMBER_OF_SETTINGS(RELAY) (RELAY==PWN_RELAY) ? 6 : 4
//#define NUMBER_OF_SETTINGS(RELAY) 4

AllSettings allSettings;
//PWMSettingsType all_pwm_settings;

extern int8_t pwmParts;


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
void ShowTerm();
void ShowSettings();
void ShowRelayMenu();
void ShowReset();

typedef enum {PS_WORK, PS_MENU} ProgramState;

ProgramState ps;
Menu menu;

uint32_t iterations = 0;


float temp[3];


uint8_t previousPWMSignal;

void onPWMSignalChange()
{
	//resetPWMParts(all_settings);
}

int main()
{
	ps = PS_WORK;
	int turn = 0;
	previousPWMSignal = readPWMSignal();
	LCD_Init();
	BTN_Init();
	Settings_Init(&allSettings);
	Relay_Init();
	
	Menu_Init(&menu, &allSettings);
	
	//resetPWMParts(all_settings);


	/*LCD_Clear_Display();
	LCD_2buffer_begin();
	LCD_2buffer_printStr("HELLO");
	LCD_2buffer_end();
	while (1)
	{
		ledOFF;
		_delay_ms(1000);
		ledON;
		_delay_ms(1000);		
	}*/
	while (1)
	{
		uint8_t q = readPWMSignal();
		if (q!=previousPWMSignal)
		{
			onPWMSignalChange();			
		}
		previousPWMSignal = q;
		
		
		BTN_Process();
		_delay_ms(1);
		//LCD_Clear_Display();
		switch(ps)
		{
			case PS_WORK:
				
				DoWork();			
			break;
			case PS_MENU:
				//if (iterations%12000!=0)
				if (iterations%300!=0)
				{
					iterations++;
				}else
				{
					Menu_Loop(&menu);
					iterations = 1;
				}				
			break;
		}

	}
	return 0;
}

void ShowMenu(MenuStatus ms)
{
	cli();
	Menu_Reset(&menu, ms);
	ps = PS_MENU;
	RELAY_OFF(RELAY1);
	RELAY_OFF(RELAY2);
	RELAY_OFF(RELAY3);
	RELAY_OFF(RELAY4);
	iterations = 0;
}

void HideMenu()
{
	ps = PS_WORK;
	sei();
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

		temp[0] = DS_getFloatTemperature(DSBIT1);


		//DS_Init(DSBIT2);
		temp[1] = DS_getFloatTemperature(DSBIT2);


		//DS_Init(DSBIT3);
		temp[2] = DS_getFloatTemperature(DSBIT3);

		
		/*HandleRelay(all_settings, temp, RELAY1);
		HandleRelay(all_settings, temp, RELAY2);
		HandleRelay(all_settings, temp, RELAY3);
		HandleRelay(all_settings, temp, RELAY4);*/
		

		LCD_2buffer_begin();
		//LCD_Clear_Display();
		LCD_2buffer_Move_Cursor(0);
		LCD_2buffer_Show_FloatTemperature1(temp[0]);

		LCD_2buffer_Move_Cursor(8);
		LCD_2buffer_Show_FloatTemperature1(temp[1]);

		LCD_2buffer_Move_Cursor(16);
		LCD_2buffer_Show_FloatTemperature1(temp[2]);
		
		LCD_2buffer_Move_Cursor(24);
		LCD_2buffer_Print_Number(pwmParts);
		
		LCD_2buffer_Move_Cursor(31);
		char pwmSignalChar[2];
		pwmSignalChar[1] = '\0';
		if (readPWMSignal())
			pwmSignalChar[0] = '+';
		else
			pwmSignalChar[0] = '-';
		LCD_2buffer_printStr(pwmSignalChar);
		
		/*LCD_2buffer_Move_Cursor(24);
		uint8_t i;
		for (i=0; i<3; i++)
		{
			if (RELAY_PIN & (1<<i))
				LCD_2buffer_printStr("1");
			else
				LCD_2buffer_printStr("0");			
		}*/
		
		LCD_2buffer_end();
		iterations = 1;
		_delay_us(100);
}


void BTN1_Pressed()
{
	Menu_BtnMinusPressed(&menu);
	iterations = 0;
}
void BTN2_Pressed()
{
	Menu_BtnPlusPressed(&menu);
	iterations = 0;
}
void BTN3_Pressed()
{
	Menu_BtnOKPressed(&menu);
	iterations = 0;
}
void BTN1_Long_Pressed()
{
	Menu_BtnMinusLongPressed(&menu);
	iterations = 0;
}

void BTN2_Long_Pressed()
{
	if (ps==PS_WORK)
	{
		ShowMenu(MS_RESET);	
	}else
	{
		Menu_BtnPlusLongPressed(&menu);
	}	
	iterations = 0;
}

void BTN3_Long_Pressed()
{

	if (ps==PS_WORK)
	{
		ShowMenu(MS_RELAY);		
	}else
	{
		HideMenu();
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
	//if ((BTNPIN & (1<<BTN1))==0)
	if (IS_BTN_PRESSED(BTN1))
	{
		if (BTN1_status>=0)
			BTN1_status ++;
		//Led_On_time = 0;
	}

	if (IS_BTN_PRESSED(BTN2))
	{
		if (BTN2_status>=0)
			BTN2_status ++;
		//Led_On_time = 0;
	}

	if (IS_BTN_PRESSED(BTN3))
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

	if (!IS_BTN_PRESSED(BTN1))
	{
		if ((BTN1_status>0)&&(BTN1_status<BTN_long_time))
		{
			BTN1_Pressed();
		}
		BTN1_status = 0;
	}

	if (!IS_BTN_PRESSED(BTN2))
	{
		if ((BTN2_status>0)&&(BTN2_status<BTN_long_time))
		{
			BTN2_Pressed();
		}
		BTN2_status = 0;
	}

	if (!IS_BTN_PRESSED(BTN3))
	{
		if ((BTN3_status>0)&&(BTN3_status<BTN_long_time))
		{
			BTN3_Pressed();
		}
		BTN3_status = 0;
	}
	
}
