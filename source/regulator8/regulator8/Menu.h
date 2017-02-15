/*
 * Menu.h
 *
 * Created: 06.02.2017 13:44:13
 *  Author: kirill
 */ 


#ifndef MENU_H_
#define MENU_H_

#include "lcd.h"
#include "settings.h"
#include "relay.h"

#define CONTROL_LIMITS(VALUE, LOW, HIGH) if (VALUE<LOW) VALUE=HIGH; if (VALUE>HIGH) VALUE=LOW;


#define MENU_SHOW_TEMP(N, POS, STR, VAR) if ((menu->blinker)||(menu->settings_item2!=N)){LCD_2buffer_Move_Cursor(POS);LCD_2buffer_printStr(STR);LCD_2buffer_Show_FloatTemperature1(VAR);}
#define MENU_SHOW_NUMBER(N, POS, STR, VAR) if ((menu->blinker)||(menu->settings_item2!=N)){LCD_2buffer_Move_Cursor(POS);LCD_2buffer_printStr(STR);LCD_2buffer_Print_Number(VAR);}
#define MENU_SHOW_PARTS(N, POS, STR, VAR) if ((menu->blinker)||(menu->settings_item2!=N)){LCD_2buffer_Move_Cursor(POS);LCD_2buffer_printStr(STR);LCD_2buffer_Print_Number(VAR);LCD_2buffer_printStr("/");LCD_2buffer_Print_Number(N_PARTS);}


#define MENU_SHOW_ONOFF(N, POS, VAR) if ((menu->blinker)||(menu->settings_item2!=N)){LCD_2buffer_Move_Cursor(POS);if(VAR){LCD_2buffer_printStr("On");}else{LCD_2buffer_printStr("Off");}}

typedef enum {MS_RELAY, MS_TEMP, MS_SETTINGS, MS_RESET, MS_DEFAULT} MenuStatus;

typedef struct
{
	MenuStatus ms;
	uint8_t sub_current_status;
	uint8_t current_relay;
	uint8_t current_term;

	uint8_t settings_item1;
	uint8_t settings_item2;

	uint8_t blinker;

	Settings * settings;
	
	AllSettings * allSettings;
}Menu;

uint8_t NUMBER_OF_SETTINGS(uint8_t RELAY);
uint8_t NUMBER_OF_TERMS(uint8_t RELAY);

void Menu_Init(Menu * menu, AllSettings * allSettings);
void Menu_Reset(Menu * menu, MenuStatus ms);
void Menu_Loop(Menu * menu);

void Menu_ShowReset(Menu * menu);
void Menu_ShowTerm(Menu * menu);
void Menu_ShowSettings(Menu * menu);
void Menu_ShowSettingsPWM(Menu * menu);
void Menu_ShowRelayMenu(Menu * menu);

void Menu_BtnMinusPressed(Menu * menu);
void Menu_BtnPlusPressed(Menu * menu);
void Menu_BtnOKPressed(Menu * menu);

void Menu_BtnMinusLongPressed(Menu * menu);
void Menu_BtnPlusLongPressed(Menu * menu);

void Menu_Update_Settings_Normal(Menu * menu, int8_t sign, uint8_t isLong);
void Menu_Update_Settings_PWM(Menu * menu, int8_t sign, uint8_t isLong);

void Menu_switchToDefault(Menu * menu);

void Menu_switchToRelay(Menu * menu, uint8_t r);

void Menu_switchToTerm(Menu * menu, uint8_t r, uint8_t t);	

void Menu_switchToSettings(Menu * menu, uint8_t r, uint8_t t);

void Menu_switchToReset(Menu * menu);


#endif /* MENU_H_ */