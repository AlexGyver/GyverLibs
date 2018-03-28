#include "GyverHacks.h"
#include <Arduino.h>

GTimer::GTimer() {}

GTimer::GTimer(uint16_t interval) {
	_interval = interval;
}

void GTimer::setInterval(uint16_t interval) {
	_interval = interval;
}

boolean GTimer::isReady() {
	if (millis() - _timer >= _interval) {
		_timer = millis();
		return true;
	} else {
		return false;
	}
}

void GTimer::reset() {
	_timer = millis();
}

GFilterRA::GFilterRA() {}

void GFilterRA::setCoef(float coef) {
	_coef = coef;
}
void GFilterRA::setStep(uint16_t interval) {
	_filterInterval = interval;
}

float GFilterRA::filteredTime(int16_t value) {
	if (millis() - _filterTimer >= _filterInterval) {
		_filterTimer = millis();
		_lastValue = _lastValue * (1 - _coef) + value * _coef;
	}
	return _lastValue;
}

float GFilterRA::filtered(int16_t value) {
	_lastValue = _lastValue * (1 - _coef) + value * _coef;
	return _lastValue;
}