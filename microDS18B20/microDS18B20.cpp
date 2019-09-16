#include "microDS18B20.h"
#include <Arduino.h>
#include "microOneWire.h"

MicroDS18B20::MicroDS18B20(uint8_t pin) {
	_pin = pin;
	pinMode(_pin, INPUT);
	digitalWrite(_pin, LOW);
}

void MicroDS18B20::requestTemp() {
	if (oneWire_reset(_pin)) return;
	oneWire_write(0xCC, _pin);
	oneWire_write(0x44, _pin);
}

float MicroDS18B20::getTemp() {
	uint8_t data[2];
	if (oneWire_reset(_pin)) return;
	oneWire_write(0xCC, _pin);
	oneWire_write(0xBE, _pin);
	data[0] = oneWire_read(_pin);
	data[1] = oneWire_read(_pin);
	return (float)((data[1] << 8) | data[0]) * 0.0625;
}