# TermoRegulator
The project contains the source code and Proteus simulation for a device that turns on/off heaters accroding to the temperature.

#To run the simulation:
1. Open project in proteus.

2. Right click on AtMega8 -> Edit Properties.

3. Set Pogram File to "\source\default\regulator8.hex"

4. Set Initial Content of Data EEPROM to "\source\default\regulator8.bin" (Note: bin file was made by HEX2BIN.exe from regulator8.eep file)

5. Run Simulation.

6. To enter menu press OK button and wait for 10 seconds.



#Video with working device is available here:
https://www.youtube.com/watch?v=qEKEROn3Aic

Инструкция по эксплуатации:

1. В рабочем режиме:
	Длинное нажатие кнопки "ОК" - вход в меню выбора реле.
	Длинное нажание кнопки "+" - вход в меню сброса настроек.
2. В режиме настроек:
	Длинное нажатие кнопки "ОК" - выход в рабочий режим.
	Длинное нажание кнопки "-" - выход на предыдущий уровень меню.
	
Список настроек (слева направо сверху вниз):
	Температура
	Гестерезис
	Направление (Up Down)
	Вкл/Выкл
	Период
	Количество частей периода, при которых реле включен
	Шаг