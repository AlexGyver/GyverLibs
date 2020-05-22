#pragma once
#include <Arduino.h>

// компактная альтернатива фильтра экспоненциальное бегущее среднее
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

// ***************************** GFilterRA *****************************
GFilterRA::GFilterRA() {}

GFilterRA::GFilterRA(float coef, uint16_t interval) {
	_coef = coef;
	_filterInterval = interval;
}

GFilterRA::GFilterRA(float coef) {
	_coef = coef;
}

void GFilterRA::setCoef(float coef) {
	_coef = coef;
}
void GFilterRA::setStep(uint16_t interval) {
	_filterInterval = interval;
}

float GFilterRA::filteredTime(int16_t value) {
	if (millis() - _filterTimer >= _filterInterval) {
		_filterTimer = millis();
		return GFilterRA::filtered(value);
	}
}

float GFilterRA::filteredTime(float value) {
	if (millis() - _filterTimer >= _filterInterval) {
		_filterTimer = millis();
		return GFilterRA::filtered(value);
	}
}

float GFilterRA::filtered(int16_t value) {
	_lastValue += (float)(value - _lastValue) * _coef;
	return _lastValue;
}

float GFilterRA::filtered(float value) {
	_lastValue += (float)(value - _lastValue) * _coef;
	return _lastValue;
}