/*
	Ультра лёгкая библиотека для работы с RTC DS3231
	Для работы нужна библиотека microWire.h
	Версия 1.0 by Egor
*/

#ifndef microDS3231_h
#define microDS3231_h
#include <Arduino.h>
#include <microWire.h>

const boolean COMPILE_TIME = true;

class MicroDS3231 {
public:
	MicroDS3231();	// конструктор
	void setTime(uint8_t seconds , uint8_t minutes , uint8_t hours , uint8_t date, uint8_t month, uint16_t year);	// установка времени
	void setTime(uint8_t param);	// установка времени == времени компиляции
	bool lostPower(void);			// проверка на сброс питания
	uint8_t getSeconds(void);		// получить секунды
	uint8_t getMinutes(void);		// получить минуты
	uint8_t getHours(void);			// получить часы
	uint8_t getDay(void);			// получить день недели
	uint8_t getDate(void);			// получить число
	uint16_t getYear(void);			// получить год
	uint8_t getMonth(void);			// получить месяц
	uint8_t readRegister(uint8_t addr);
private:
};

#endif