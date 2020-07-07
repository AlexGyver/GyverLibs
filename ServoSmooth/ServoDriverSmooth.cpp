#include "ServoDriverSmooth.h"

ServoDriverSmooth::ServoDriverSmooth(uint8_t addr, int maxAngle) {	
	_maxAngle = maxAngle;
	_servo._i2caddr = addr;	
	_min = 150;
	_max = 600;
}

void ServoDriverSmooth::sendToDriver(uint16_t val) {
	_servo.setPWM(_pin, 0, val);
}

void ServoDriverSmooth::attach(int pin) {
	_pin = pin;
	if (!_startFlag) {
		_startFlag = true;
		_servo.begin();
		_servo.setPWMFreq(60);
	}
}

void ServoDriverSmooth::detach() {
	_servo.setPWM(_pin, 0, 4096);
}