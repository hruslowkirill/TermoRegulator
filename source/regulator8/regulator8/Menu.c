/*
 * Menu.c
 *
 * Created: 06.02.2017 13:44:25
 *  Author: kirill
 */ 

#include "menu.h"

uint8_t NUMBER_OF_SETTINGS(uint8_t RELAY)
{
	if (RELAY==PWM_RELAY)
		return 11;
	return 4;		
}

uint8_t NUMBER_OF_TERMS(uint8_t RELAY)
{
	if (RELAY==PWM_RELAY)
		return 2;
	return 3;		
}

void Menu_Init(Menu * menu, AllSettings * allSettings)
{
	menu->allSettings = allSettings;
}

void Menu_Reset(Menu * menu, MenuStatus ms)
{
	menu->ms = ms;
	menu->sub_current_status = 0;
	menu->current_relay = 0;
	menu->current_term = 0;
	menu->blinker = 0;
	
}

void Menu_Loop(Menu * menu)
{
	switch (menu->ms)
	{
		case MS_DEFAULT:
		break;
		case MS_TEMP:
			Menu_ShowTerm(menu);
		break; 
		case MS_SETTINGS:
			if (menu->current_relay==PWM_RELAY)
			{
				Menu_ShowSettingsPWM(menu);
			}else
			{
				Menu_ShowSettings(menu);
			}			
		break; 
		case MS_RELAY:
			Menu_ShowRelayMenu(menu);
		break; 
		case MS_RESET:
			Menu_ShowReset(menu);
		break;
	}			
}

void Menu_ShowReset(Menu * menu)
{
	LCD_2buffer_begin();
	LCD_2buffer_Move_Cursor(1);
	LCD_2buffer_printStr("RESET?????");

	LCD_2buffer_Move_Cursor(17);
	LCD_2buffer_printStr("No");
	
	LCD_2buffer_Move_Cursor(25);
	LCD_2buffer_printStr("Yes");

	switch(	menu->sub_current_status)
	{
		case 0:
			LCD_2buffer_Move_Cursor(16);
		break;

		case 1:
			LCD_2buffer_Move_Cursor(24);
		break;

	}
	
	LCD_2buffer_printStr(">");

	LCD_2buffer_end();

}

void Menu_ShowTerm(Menu * menu)
{
	LCD_2buffer_begin();
	LCD_2buffer_Move_Cursor(1);
	LCD_2buffer_printStr("Temp1");

	
	
	if (menu->current_relay!=PWM_RELAY)
	{
		LCD_2buffer_Move_Cursor(9);
		LCD_2buffer_printStr("Temp2");
		
		LCD_2buffer_Move_Cursor(17);
		LCD_2buffer_printStr("Temp3");
	}
	switch(	menu->sub_current_status)
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
	
	if (menu->current_relay==PWM_RELAY)
	{
		if (menu->allSettings->PWMSettings[0].on)
		{
			LCD_2buffer_Move_Cursor(6);		
			LCD_2buffer_printStr("*");
		}		
	}else
	{
		if (menu->allSettings->settings[menu->current_relay][0].on)
		{
			LCD_2buffer_Move_Cursor(6);		
			LCD_2buffer_printStr("*");
		}
		if (menu->allSettings->settings[menu->current_relay][1].on)
		{
			LCD_2buffer_Move_Cursor(14);		
			LCD_2buffer_printStr("*");
		}
		if (menu->allSettings->settings[menu->current_relay][2].on)
		{
			LCD_2buffer_Move_Cursor(22);		
			LCD_2buffer_printStr("*");
		}
	}	

	LCD_2buffer_end();
	//LCD_Clear_Display();
	//LCD_printStr("HELLO");
	//_delay_ms(1);
}

void Menu_ShowSettings(Menu * menu)
{
	
	menu->settings = &menu->allSettings->settings[menu->current_relay][menu->current_term];	

	menu->blinker = !menu->blinker;


	LCD_2buffer_begin();
	if (menu->settings_item1<=3)
	{
		
		if ((menu->blinker)||(menu->settings_item2!=1))
		{
			LCD_2buffer_Move_Cursor(1);
			LCD_2buffer_Show_FloatTemperature1(menu->settings->temp);
		}
	
		if ((menu->blinker)||(menu->settings_item2!=2))
		{
			LCD_2buffer_Move_Cursor(9);
			LCD_2buffer_Show_FloatTemperature1(menu->settings->d);
		}
		if ((menu->blinker)||(menu->settings_item2!=3))
		{
			LCD_2buffer_Move_Cursor(17);
			if(menu->settings->direction)
			{
				LCD_2buffer_printStr("COLD");
			}else
			{
				LCD_2buffer_printStr("HEAT");
			}
		}
		if ((menu->blinker)||(menu->settings_item2!=4))
		{
			LCD_2buffer_Move_Cursor(25);
			if(menu->settings->on)
			{
				LCD_2buffer_printStr("On");
			}else
			{
				LCD_2buffer_printStr("Off");
			}
		}
	}
	switch(	menu->settings_item1)
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
		
		case 3:
			LCD_2buffer_Move_Cursor(24);
		break;
		
		case 4:
			LCD_2buffer_Move_Cursor(0);
		break;
		
		case 5:
			LCD_2buffer_Move_Cursor(8);
		break;
		case 6:
			LCD_2buffer_Move_Cursor(16);
		break;
		case 7:
			LCD_2buffer_Move_Cursor(0);
		break;
		case 8:
			LCD_2buffer_Move_Cursor(8);
		break;
		case 9:
			LCD_2buffer_Move_Cursor(16);
		break;
		}
		LCD_2buffer_printStr(">");	
	LCD_2buffer_end();
}

void Menu_ShowSettingsPWM(Menu * menu)
{
	
	PWMSettings * pwmSettings = &menu->allSettings->PWMSettings[menu->current_term];	

	menu->blinker = !menu->blinker;


	LCD_2buffer_begin();
	if (menu->settings_item1<=3)
	{
		MENU_SHOW_ONOFF(1, 1, pwmSettings->on)
		MENU_SHOW_TEMP(2, 9, "Tz", pwmSettings->Tzap)
		MENU_SHOW_NUMBER(3, 17, "t3 ", pwmSettings->t3)
		MENU_SHOW_TEMP(4, 25, "T1", pwmSettings->Tstart)
		
	}
	else if (menu->settings_item1<=7)
	{
		MENU_SHOW_TEMP(5, 1, "T2", pwmSettings->Tstop)
		MENU_SHOW_TEMP(6, 9, "T3", pwmSettings->Tsliv)  
		MENU_SHOW_NUMBER(7, 17, "tp", pwmSettings->tpwm) 	
		MENU_SHOW_PARTS(8, 25, "Sg", pwmSettings->Sgshim) 			
	}else if (menu->settings_item1<=11)
	{	
		MENU_SHOW_PARTS(9, 1, "Sc", pwmSettings->Scshim) 	
		MENU_SHOW_PARTS(10, 9, "Ss", pwmSettings->Sshim) 
		MENU_SHOW_NUMBER(11, 17, "Sd", pwmSettings->SshimD) 	
	}
	
	switch(	menu->settings_item1)
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
		
		case 3:
			LCD_2buffer_Move_Cursor(24);
		break;
		
		case 4:
			LCD_2buffer_Move_Cursor(0);
		break;
		
		case 5:
			LCD_2buffer_Move_Cursor(8);
		break;
		case 6:
			LCD_2buffer_Move_Cursor(16);
		break;
		case 7:
			LCD_2buffer_Move_Cursor(24);
		break;
		case 8:
			LCD_2buffer_Move_Cursor(0);
		break;
		case 9:
			LCD_2buffer_Move_Cursor(8);
		break;
		case 10:
			LCD_2buffer_Move_Cursor(16);
		break;
		}
		LCD_2buffer_printStr(">");	
	LCD_2buffer_end();
}

void Menu_ShowRelayMenu(Menu * menu)
{

	LCD_2buffer_begin();
	LCD_2buffer_Move_Cursor(1);
	LCD_2buffer_printStr("Relay1");

	LCD_2buffer_Move_Cursor(9);
	LCD_2buffer_printStr("Relay2");

	LCD_2buffer_Move_Cursor(17);
	LCD_2buffer_printStr("Relay3");
	
	LCD_2buffer_Move_Cursor(25);
	LCD_2buffer_printStr("Relay4");

	switch(	menu->sub_current_status)
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
		
		case 3:
			LCD_2buffer_Move_Cursor(24);
		break;
	}
	
	LCD_2buffer_printStr(">");

	LCD_2buffer_end();
	//LCD_Clear_Display();
	//LCD_printStr("HELLO");
	//_delay_ms(1);
}

void Menu_BtnMinusPressed(Menu * menu)
{
	switch(menu->ms)
	{
		case MS_DEFAULT:
		break;
		case MS_RELAY:
			if(menu->sub_current_status==0)
				menu->sub_current_status = 4;
			menu->sub_current_status--;
		break;
		case MS_RESET:
			if(menu->sub_current_status==0)
				menu->sub_current_status = 2;
			menu->sub_current_status--;
		break;
		case MS_TEMP:
			if(menu->sub_current_status==0)
				menu->sub_current_status = NUMBER_OF_TERMS(menu->current_relay);
			menu->sub_current_status--;
		break;
		case MS_SETTINGS:
			if (menu->settings_item2==0)
			{
				if(menu->settings_item1==0)
					menu->settings_item1 = NUMBER_OF_SETTINGS(menu->current_relay);
				menu->settings_item1--;
			}
			if (menu->current_relay!=PWM_RELAY)
			{
				Menu_Update_Settings_Normal(menu, -1, 0);
			}else
			{
				Menu_Update_Settings_PWM(menu, -1, 0);
			}			
		break;
	}
}
void Menu_BtnPlusPressed(Menu * menu)
{

	switch(menu->ms)
	{
		case MS_DEFAULT:
		break;
		case MS_RESET:
		break;
		case MS_RELAY:
			menu->sub_current_status++;
			if(menu->sub_current_status==4)
				menu->sub_current_status = 0;
		break;
		case MS_TEMP:
			menu->sub_current_status++;
			if(menu->sub_current_status==NUMBER_OF_TERMS(menu->current_relay))
				menu->sub_current_status = 0;
		break;
		case MS_SETTINGS:
			if (menu->settings_item2==0)
			{	
				menu->settings_item1++;
				if(menu->settings_item1==NUMBER_OF_SETTINGS(menu->current_relay))
					menu->settings_item1 = 0;
			}
			if (menu->current_relay!=PWM_RELAY)
			{
				Menu_Update_Settings_Normal(menu, 1, 0);
			}else
			{
				Menu_Update_Settings_PWM(menu, 1, 0);
			}	
			
		break;
	}
}
void Menu_BtnOKPressed(Menu * menu)
{
	switch(menu->ms)
	{
		case MS_DEFAULT:
		break;
		case MS_TEMP:
			Menu_switchToSettings(menu, menu->current_relay, menu->sub_current_status);
		break;
		
		case MS_RESET:
			Settings_Reset(menu->allSettings);
			Menu_Reset(menu, MS_RELAY);
		break;
		
		case MS_RELAY:
			Menu_switchToTerm(menu, menu->sub_current_status, 0);
		break;

		case MS_SETTINGS:
			if (menu->settings_item2==0)
			{
				menu->settings_item2 = menu->settings_item1+1;
			}else
			{	
				menu->settings_item2 = 0;
				if (menu->current_relay!=PWM_RELAY)
				{
					Settings_Write_Normal(menu->allSettings, menu->current_relay, menu->current_term);
				}else
				{
					Settings_Write_PWM(menu->allSettings, menu->current_relay, menu->current_term);
				}							
			}
		break;
	}
}

void Menu_BtnMinusLongPressed(Menu * menu)
{
	switch(menu->ms)
	{
		case MS_TEMP:
			Menu_switchToRelay(menu, menu->current_relay);
		break;
		case MS_SETTINGS:
			if (menu->settings_item2==0)
			{
				Menu_switchToTerm(menu, menu->current_relay, menu->current_term);
			}else
			{
				if (menu->current_relay!=PWM_RELAY)
				{
					Menu_Update_Settings_Normal(menu, -1, 1);
				}else
				{
					Menu_Update_Settings_PWM(menu, -1, 1);
				}	
			}			
		break;
		case MS_DEFAULT:
		break;
		case MS_RELAY:
		break;
		case MS_RESET:
		break;
	}
}

void Menu_BtnPlusLongPressed(Menu * menu)
{
	switch(menu->ms)
	{
		case MS_DEFAULT:
			Menu_switchToReset(menu);
		break;
		case MS_SETTINGS:
			if (menu->current_relay!=PWM_RELAY)
			{
				Menu_Update_Settings_Normal(menu, 1, 1);
			}else
			{
				Menu_Update_Settings_PWM(menu, 1, 1);
			}	

		break;
		case MS_RELAY:
		break;
		case MS_TEMP:
		break;
		case MS_RESET:
		break;
	}
}

void Menu_Update_Settings_Normal(Menu * menu, int8_t sign, uint8_t isLong)
{
	uint8_t mult = 1;
	
	if (isLong)
		mult = 50;
	
	if (menu->settings_item2==1)
	{
		menu->settings->temp+=sign*0.1*mult;
	}
	if (menu->settings_item2==2)
	{
		menu->settings->d+=sign*0.1*mult;
	}
	if (menu->settings_item2==3)
	{
		menu->settings->direction = !menu->settings->direction;
	}
	if (menu->settings_item2==4)
	{
		menu->settings->on = !menu->settings->on;
	}
}

void Menu_Update_Settings_PWM(Menu * menu, int8_t sign, uint8_t isLong)
{
	uint8_t multB = 1;
	uint8_t multC = 1;
	if (isLong)
	{
		multB = 50;
		multC = 10;
	}		
	PWMSettings * pwmSettings = &menu->allSettings->PWMSettings[menu->current_term];
	switch (menu->settings_item2)	
	{
		case 1: 
			pwmSettings->on = !pwmSettings->on;
		break;
		case 2:
			pwmSettings->Tzap+=sign*0.1*multB;
		break;
		case 3:
			pwmSettings->t3+=sign*1*multC;
			CONTROL_LIMITS(pwmSettings->t3, 0, 60);
		break;
		case 4:
			pwmSettings->Tstart+=sign*0.1*multB;
		break;
		case 5:
			pwmSettings->Tstop+=sign*0.1*multB;
		break;
		case 6:
			pwmSettings->Tsliv+=sign*0.1*multB;
		break;
		case 7:
			pwmSettings->tpwm+=sign*1*multC;
			CONTROL_LIMITS(pwmSettings->tpwm, 0, 30);
		break;
		case 8:
			pwmSettings->Sgshim+=sign*1*multC;
			CONTROL_LIMITS(pwmSettings->Sgshim, 0, N_PARTS);
		break;
		case 9:
			pwmSettings->Scshim+=sign*1*multC;
			CONTROL_LIMITS(pwmSettings->Scshim, 0, N_PARTS);
		break;
		case 10:
			pwmSettings->Sshim+=sign*1*multC;
			CONTROL_LIMITS(pwmSettings->Sshim, 0, N_PARTS);
		break;
		case 11:
			pwmSettings->SshimD+=sign*1*multC;
			CONTROL_LIMITS(pwmSettings->SshimD, 0, N_PARTS);
		break;
	}
}

void Menu_switchToDefault(Menu * menu)
{
	LCD_Clear_Display();
	menu->ms=MS_DEFAULT;
	menu->sub_current_status = 0;
}	

void Menu_switchToRelay(Menu * menu, uint8_t r)
{
	menu->ms=MS_RELAY;
	menu->sub_current_status = r;
}	

void Menu_switchToTerm(Menu * menu, uint8_t r, uint8_t t)
{
	menu->ms = MS_TEMP;
	menu->current_relay = r;
	menu->sub_current_status = t;
}	

void Menu_switchToSettings(Menu * menu, uint8_t r, uint8_t t)
{
	menu->ms = MS_SETTINGS;
	menu->sub_current_status = 0;
	menu->current_relay = r;
	menu->current_term = t;
	menu->settings_item1 = 0;
	menu->settings_item2 = 0;
}

void Menu_switchToReset(Menu * menu)
{
	menu->ms = MS_RESET;
	menu->sub_current_status = 0;
	menu->settings_item1 = 0;
	menu->settings_item2 = 0;
}