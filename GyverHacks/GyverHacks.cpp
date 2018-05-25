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
	if ((long)millis() - _timer >= _interval) {
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

void setPWMPrescaler(uint8_t pin, uint16_t prescale) {
  byte mode;
  if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch (prescale) {
      case    1: mode = 0b001; break;
      case    8: mode = 0b010; break;
      case   64: mode = 0b011; break;
      case  256: mode = 0b100; break;
      case 1024: mode = 0b101; break;
      default: return;
    }
  } else if (pin == 3 || pin == 11) {
    switch (prescale) {
      case    1: mode = 0b001; break;
      case    8: mode = 0b010; break;
      case   32: mode = 0b011; break;
      case   64: mode = 0b100; break;
      case  128: mode = 0b101; break;
      case  256: mode = 0b110; break;
      case 1024: mode = 0b111; break;
      default: return;
    }
  }
  if (pin == 5 || pin == 6) {
    TCCR0B = TCCR0B & 0b11111000 | mode;
  } else if (pin == 9 || pin == 10) {
    TCCR1B = TCCR1B & 0b11111000 | mode;
  } else if (pin == 3 || pin == 11) {
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}