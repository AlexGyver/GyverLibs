#include "GyverTimer.h"
#include <Arduino.h> 

GTimer_ms::GTimer_ms() {}

GTimer_ms::GTimer_ms(uint32_t interval) {
	_interval = interval;
	_timer = millis();
}

void GTimer_ms::setInterval(uint32_t interval) {
	_interval = interval;
	GTimer_ms::reset();
}
void GTimer_ms::setMode(boolean mode) {
	_mode = mode;
}
boolean GTimer_ms::isReady() {
	if ((long)millis() - _timer >= _interval) {
		if (_mode) _timer = millis();
		return true;
	} else {
		return false;
	}
}

void GTimer_ms::reset() {
	_timer = millis();
}

GTimer_us::GTimer_us() {}

GTimer_us::GTimer_us(uint32_t interval) {
	_interval = interval;
	_timer = micros();
}

void GTimer_us::setInterval(uint32_t interval) {
	_interval = interval;
	_timer = micros();
}
void GTimer_us::setMode(boolean mode) {
	_mode = mode;
}
boolean GTimer_us::isReady() {
	if ((long)micros() - _timer >= _interval) {
		if (_mode) _timer = micros();
		return true;
	} else {
		return false;
	}
}

void GTimer_us::reset() {
	_timer = micros();
}