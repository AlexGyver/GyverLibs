#include <filters/runningAverage.h>

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
		filtered(value);
	}
	return _lastValue;
}

float GFilterRA::filteredTime(float value) {
	if (millis() - _filterTimer >= _filterInterval) {
		_filterTimer = millis();
		filtered(value);
	}
	return _lastValue;
}

float GFilterRA::filtered(int16_t value) {
	_lastValue += (float)(value - _lastValue) * _coef;
	return _lastValue;
}

float GFilterRA::filtered(float value) {
	_lastValue += (float)(value - _lastValue) * _coef;
	return _lastValue;
}