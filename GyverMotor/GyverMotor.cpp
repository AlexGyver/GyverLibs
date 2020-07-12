#include "GyverMotor.h"
#include <Arduino.h>

GMotor::GMotor(driverType type, int8_t param1 = NC, int8_t param2 = NC, int8_t param3 = NC, int8_t param4 = NC) {
	_type = type;
	switch (_type) {
	case DRIVER2WIRE:
		_digA = param1;
		_pwmC = param2;
		if (param3 != NC) _level = !param3;
		break;
	case DRIVER3WIRE:
		_digA = param1;
		_digB = param2;
		_pwmC = param3;		
		if (param4 != NC) _level = !param4;
		break;	
	case RELAY2WIRE:
		_digA = param1;
		_digB = param2;
		if (param3 != NC) _level = !param3;
		break;		
	}

	if (_digA != NC) pinMode(_digA, OUTPUT);
	if (_digB != NC) pinMode(_digB, OUTPUT);
	if (_pwmC != NC) pinMode(_pwmC, OUTPUT);
	
	setMode(STOP);
}

void GMotor::setSpeed(int16_t duty) {
	if (_mode < 2) {
		// -254..254 8 бит, -1023..1023 16 бит
		// фикс для типичных драйверов на N ключах,
		// которым нельзя давать "100% ШИМ"
		_duty = constrain(duty, -_maxDuty, _maxDuty);
		
		// фикс зашкала analogWrite(пин, 255) для 10 бит
		if (_resolution && abs(_duty) == 255) _duty++;

		if (_duty > _minDuty) {
			run(_mode, _duty);
		} else if (_duty < -_minDuty) {
			run(BACKWARD, -_duty);
		} else {  // == 0
			run(STOP, 0);
		}
	}
}

void GMotor::run(workMode mode, int16_t duty = 0) {
	if (_deadtime > 0 && _lastMode != mode) {
		_lastMode = mode;
		setPins(_level, _level, 0);
		delayMicroseconds(_deadtime);
	}
	if (_direction && mode < 2) mode = 1 - mode;
	switch (mode) {
	case FORWARD:	setPins(_level, !_level, duty); _state = 1; break;		
	case BACKWARD:	setPins(!_level, _level, (_type == DRIVER2WIRE) ? (_maxDuty - duty) : (duty)); _state = -1; break;		
	case STOP:		setPins(_level, _level, _level * 255); _state = 0; break;		
	case BRAKE:		setPins(!_level, !_level, !_level * 255); _state = 0; break;		
	}
}

void GMotor::setPins(bool a, bool b, int c) {
	if (_digA != NC) digitalWrite(_digA, a);
	if (_digB != NC) digitalWrite(_digB, b);
	if (_pwmC != NC) analogWrite(_pwmC, c);
}

void GMotor::smoothTick(int16_t duty) {
	if (millis() - _tmr >= _SMOOTH_PRD) {
		_tmr += _SMOOTH_PRD;
		if (abs(_duty - duty) > _speed) _duty += (_duty < duty) ? _speed : -_speed;
		else _duty = duty;
		setSpeed(_duty);
	}
}

int GMotor::getState() {
	return _state;
}

void GMotor::setMinDuty(int duty) {
	_minDuty = duty;
}

void GMotor::setMode(workMode mode) {	
	_mode = mode;
	run(mode, _duty);
}

void GMotor::setSmoothSpeed(uint8_t speed) {
	_speed = speed;
}

void GMotor::setDirection(dir direction) {
	_direction = direction;
}

void GMotor::set8bitMode() {
	_resolution = 0;
	_maxDuty = 254;
}

void GMotor::set10bitMode() {
	_resolution = 1;
	_maxDuty = 1022;
}

void GMotor::setDeadtime(uint16_t deadtime) {
	_deadtime = deadtime;
}

void GMotor::setLevel(int8_t level) {
	_level = !level;
}