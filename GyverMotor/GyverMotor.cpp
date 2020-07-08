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
		_duty = duty;
		// фикс зашкала analogWrite(пин, 255) для 10 бит
		// также фикс для типичных драйверов на N ключах,
		// которым нельзя давать "100% ШИМ"
		if (abs(_duty) == 255) _duty = (_duty > 0) ? 254 : -254;
		if (_resolution && abs(_duty) == 1023) _duty = (_duty > 0) ? 1022 : -1022;

		if (duty > _minDuty) {
			run(_mode ^ _direction, _duty);
		} else if (duty < -_minDuty) {
			run(BACKWARD ^ _direction, -_duty);
		} else {  // == 0
			run(STOP, 0);
		}
	}
}

void GMotor::run(workMode mode, int16_t duty) {
	if (_deadtime > 0 && _lastMode != mode) {
		_lastMode = mode;
		setPins(_level, _level, 0);
		delayMicroseconds(_deadtime);
	}
	switch (mode) {
	case FORWARD:	setPins(_level, !_level, duty); break;		
	case BACKWARD:	setPins(!_level, _level, (_type == DRIVER2WIRE) ? (_resolution ? (1022 - duty) : (254 - duty)) : (duty)); break;		
	case STOP:		setPins(_level, _level, _level * (_resolution ? 1023 : 255)); break;		
	case BRAKE:		setPins(!_level, !_level, !_level * (_resolution ? 1023 : 255)); break;		
	}
}

void GMotor::setPins(bool a, bool b, int c) {
	if (_digA != NC) digitalWrite(_digA, a);
	if (_digB != NC) digitalWrite(_digB, b);
	if (_pwmC != NC) analogWrite(_pwmC, c);
}

void GMotor::smoothTick(int16_t duty) {
	if (millis() - _tmr >= 50) {
		_tmr += 50;
		if (abs(_duty - duty) > _speed)
			_duty += (_duty < duty) ? _speed : -_speed;
		else _duty = duty;
		setSpeed(_duty);
	}
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
}

void GMotor::set10bitMode() {
	_resolution = 1;
}

void GMotor::setDeadtime(uint16_t deadtime) {
	_deadtime = deadtime;
}

void GMotor::setLevel(int8_t level) {
	_level = !level;
}