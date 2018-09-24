#include "GyverTimer.h"
#include <Arduino.h> 

GTimer::GTimer() {}

GTimer::GTimer(uint16_t interval) {
	_interval = interval;
	_timer = (long)millis() + _interval;
}

void GTimer::setInterval(uint16_t interval) {
	_interval = interval;
	GTimer::reset();
}
void GTimer::setMode(boolean mode) {
	_mode = mode;
}
boolean GTimer::isReady() {
	if ((long)millis() > _timer) {
		if (!_mode) _timer = millis() + _interval;
		return true;
	} else {
		return false;
	}
}

void GTimer::reset() {
	_timer = millis() + _interval;
}

GTimer_us::GTimer_us() {}

GTimer_us::GTimer_us(uint16_t interval) {
	_interval = interval;
	_timer = micros();
}

void GTimer_us::setInterval(uint16_t interval) {
	_interval = interval;
	_timer = micros();
}
void GTimer_us::setMode(boolean mode) {
	_mode = mode;
}
boolean GTimer_us::isReady() {
	if ((long)micros() - _timer > _interval) {
		if (!_mode) _timer = micros();
		return true;
	} else {
		return false;
	}
}

void GTimer_us::reset() {
	_timer = micros();
}