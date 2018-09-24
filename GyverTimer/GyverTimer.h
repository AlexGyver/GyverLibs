#ifndef GyverTimer_h
#define GyverTimer_h
#include <Arduino.h>
#define LIBRARY_VERSION	1.1

/*
	Текущая версия: 1.1 от 23.09.2018
	GTimer - компактная альтернатива конструкции таймера с millis() / micros()
	- Вся работа с таймером заменяется одной функцией
	- Миллисекундный и микросекундный таймер
	- Автоматический и ручной режим работы
*/

#define AUTO 0
#define MANUAL 1

class GTimer
{
  public:
	GTimer();								// объявление таймера (МИЛЛИСЕКУНДНЫЙ)
	GTimer(uint16_t interval);				// объявление таймера с указанием интервала
	void setInterval(uint16_t interval);	// установка интервала работы таймера
	void setMode(boolean mode);				// установка типа работы: AUTO или MANUAL (MANUAL нужно вручную сбрасывать reset)
	boolean isReady();						// возвращает true, когда пришло время. Сбрасывается в false сам (AUTO) или вручную (MANUAL)
	void reset();							// ручной сброс таймера на установленный интервал
	
  private:
	uint32_t _timer;
	uint16_t _interval;
	boolean _mode;
};

class GTimer_us
{
  public:
	GTimer_us();							// объявление таймера (МИКРОСЕКУНДНЫЙ)
	GTimer_us(uint16_t interval);			// объявление таймера с указанием интервала
	void setInterval(uint16_t interval);	// установка интервала работы таймера
	void setMode(boolean mode);				// установка типа работы: AUTO или MANUAL (MANUAL нужно вручную сбрасывать reset)
	boolean isReady();						// возвращает true, когда пришло время. Сбрасывается в false сам (AUTO) или вручную (MANUAL)
	void reset();							// ручной сброс таймера на установленный интервал
	
  private:
	uint32_t _timer;
	uint32_t _interval;
	boolean _mode;
};

#endif