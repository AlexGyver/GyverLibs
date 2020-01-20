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

void MicroDS18B20::setResolution(uint8_t res) {
	uint8_t reg = (res - 9 << 5) | 0x1F; // собрали байт
	if (oneWire_reset(_pin)) return; // если устройство не ответило - вышли
	oneWire_write(0x4E, _pin); // команда на запись 
	oneWire_write(0x00, _pin); // мусор
	oneWire_write(0x00, _pin); // мусор
	oneWire_write(reg, _pin); // наш байт
}

float MicroDS18B20::getTemp() {
	uint8_t data[2];
	if (oneWire_reset(_pin)) return 0;
	oneWire_write(0xCC, _pin);
	oneWire_write(0xBE, _pin);
	data[0] = oneWire_read(_pin);
	data[1] = oneWire_read(_pin);
	switch (_resolution) {
	case 9:
		return (float)((data[1] << 8) | data[0]) * 0.5;
		break;
	case 10:
		return (float)((data[1] << 8) | data[0]) * 0.25;
		break;
	case 11:
		return (float)((data[1] << 8) | data[0]) * 0.125;
		break;
	case 12:
		return (float)((data[1] << 8) | data[0]) * 0.0625;
		break;
	}
}