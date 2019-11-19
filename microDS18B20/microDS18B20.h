/*
	Микро версия библиотеки для работы с ds18b20
	Разработано by Egor, допилено AlexGyver
	Версия 1.0
	Версия 1.1 - добавлена установка разрешения
*/

#ifndef microDS18B20_h
#define microDS18B20_h
#include <Arduino.h>

class MicroDS18B20 {
public:
	MicroDS18B20(uint8_t pin);	// создать объект
	void requestTemp();			// запросить температуру
	float getTemp(void);		// получить температуру
	void setResolution(uint8_t res);  // установить разрешение
	
	
private:    
	uint8_t _pin;
	uint8_t _resolution = 12;
};

#endif