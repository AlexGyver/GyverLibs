/*
	Ультра лёгкая библиотека для работы с RTC DS3231
	Для работы нужна библиотека microWire.h
	Версия 1.3 by Egor
	- Совместимость с microWire 2.0
	v1.2 - добавлены ограничения на вводимые в setTime числа. Также нельзя ввести 29 февраля увы =)
	v1.3 - пофикшено зависание, когда модуль отключен но опрашивается
	v1.4 - незначительный фикс
*/

#ifndef _microDS3231_lib_H
#define _microDS3231_lib_H

#pragma once
#include <Arduino.h>
const bool COMPILE_TIME = true;

class MicroDS3231 {
public:
	MicroDS3231();	// конструктор
	void setTime(int8_t seconds , int8_t minutes , int8_t hours , int8_t date, int8_t month, int16_t year);	// установка времени
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

#endif // _microDS3231_lib_H