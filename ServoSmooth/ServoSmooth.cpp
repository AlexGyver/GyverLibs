#include "ServoSmooth.h"

ServoSmooth::ServoSmooth(int maxAngle) {
	_maxAngle = maxAngle;
}

void ServoSmooth::sendToDriver(uint16_t val) {
	_servo.writeMicroseconds(val);
}

void ServoSmooth::attach(int pin) {
	_pin = pin;
	_servo.attach(_pin);
}

void ServoSmooth::detach() {
	_servo.detach();
}