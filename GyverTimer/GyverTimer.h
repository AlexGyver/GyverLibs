#pragma once
#include <Arduino.h>

/*
	GTimer - компактная альтернатива конструкции таймера с millis() / micros()
	- Вся работа с таймером заменяется одной функцией
	- Миллисекундный и микросекундный таймер
	- Автоматический и ручной режим работы
	- Версия 2.0 - улучшенный алгоритм работы таймера
		- Кратные интервалы
		- Защита от пропусков
		- Защита от переполнения millis()
		- Убраны дефайны
	- Версия 2.1 - возвращены дефайны
	- Версия 2.2 - улучшена стабильность
*/

#define MANUAL 0
#define AUTO 1

class GTimer_ms
{
  public:
	GTimer_ms();								// объявление таймера (МИЛЛИСЕКУНДНЫЙ)
	GTimer_ms(uint32_t interval);				// объявление таймера с указанием интервала
	void setInterval(uint32_t interval);	// установка интервала работы таймера
	void setMode(uint8_t mode);				// установка типа работы: AUTO или MANUAL (MANUAL нужно вручную сбрасывать reset)
	boolean isReady();						// возвращает true, когда пришло время. Сбрасывается в false сам (AUTO) или вручную (MANUAL)
	void reset();							// ручной сброс таймера на установленный интервал
	void stop();							// остановить таймер (без сброса счёта)
	void start();							// продолжить (без сброса счёта)
	
  private:
	uint32_t _timer = 0;
	uint32_t _interval = 0;
	boolean _mode = true;
	boolean _state = true;
};

class GTimer_us
{
  public:
	GTimer_us();							// объявление таймера (МИКРОСЕКУНДНЫЙ)
	GTimer_us(uint32_t interval);			// объявление таймера с указанием интервала
	void setInterval(uint32_t interval);	// установка интервала работы таймера
	void setMode(uint8_t mode);				// установка типа работы: AUTO или MANUAL (MANUAL нужно вручную сбрасывать reset)
	boolean isReady();						// возвращает true, когда пришло время. Сбрасывается в false сам (AUTO) или вручную (MANUAL)
	void reset();							// ручной сброс таймера на установленный интервал
	void stop();							// остановить таймер
	void start();							// продолжить
	
  private:
	uint32_t _timer = 0;
	uint32_t _interval = 0;
	boolean _mode = true;
	boolean _state = true;
};