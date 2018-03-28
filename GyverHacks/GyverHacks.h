#ifndef GyverHacks_h
#define GyverHacks_h
#include <Arduino.h>

/*
Текущая версия: 1.1 от 29.03.2018
GyverHacks - библиотека с некоторыми удобными хаками:
	GTimer - компактная альтернатива конструкции таймера с millis()
		Настройка периода вызова
		Сброс
	GFilterRA - компактная альтернатива фильтра бегущее среднее (Running Average)
		Настройка коэффициента фильтрации
		Настройка шага дискретизации (период фильтрафии) - встроенный таймер!
*/

class GTimer
{
  public:
	GTimer();
	GTimer(uint16_t interval);
	void setInterval(uint16_t interval);
	boolean isReady();
	void reset();
  private:
	uint32_t _timer;
	uint16_t _interval;
};

class GFilterRA
{
  public:
	GFilterRA();	
	void setCoef(float coef);	    // чем меньше тем плавнее
	void setStep(uint16_t interval);
	float filteredTime(int16_t value);
	float filtered(int16_t value);
  private:
	float _coef, _lastValue;
	uint32_t _filterTimer;
	uint16_t _filterInterval;
};
 
#endif