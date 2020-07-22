#include "ServoDriverSmooth.h"

bool ServoDriverSmooth::_startFlag = false;

ServoDriverSmooth::ServoDriverSmooth(uint8_t addr, int maxAngle) {	
	_maxAngle = maxAngle;
	_servo._i2caddr = addr;	
	_min = 150;
	_max = 600;
	SS_DEADZONE = _DRIVER_DEADZONE;
	SS_DEADZONE_SP = _DRIVER_DEADZONE_SP;
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