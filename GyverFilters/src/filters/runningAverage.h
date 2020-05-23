#pragma once
#include <Arduino.h>

// экспоненциальное бегущее среднее
class GFilterRA
{
public:
	GFilterRA();								// инициализация фильтра
	GFilterRA(float coef);						// расширенная инициализация фильтра (коэффициент)
	GFilterRA(float coef, uint16_t interval);	// расширенная инициализация фильтра (коэффициент, шаг фильтрации)
	void setCoef(float coef);	    			// настройка коэффициента фильтрации (0.00 - 1.00). Чем меньше, тем плавнее
	void setStep(uint16_t interval);			// установка шага фильтрации (мс). Чем меньше, тем резче фильтр
	
	float filteredTime(int16_t value);			// возвращает фильтрованное значение с опорой на встроенный таймер	
	float filtered(int16_t value);				// возвращает фильтрованное значение
	
	float filteredTime(float value);			// возвращает фильтрованное значение с опорой на встроенный таймер	
	float filtered(float value);				// возвращает фильтрованное значение
	
private:
	float _coef = 0.0, _lastValue = 0.0;
	uint32_t _filterTimer = 0;
	uint16_t _filterInterval = 0;
};