#include "GyverMotor.h"
#include <Arduino.h>

GMotor::GMotor(GM_driverType type, int8_t param1, int8_t param2, int8_t param3, int8_t param4) {
	_type = type;
	switch (_type) {
	case DRIVER2WIRE_NO_INVERT:	
	case DRIVER2WIRE:
		_digA = param1;
		_pwmC = param2;
		if (param3 != _GM_NC) _level = !param3;
		break;
	case DRIVER3WIRE:
		_digA = param1;
		_digB = param2;
		_pwmC = param3;	
		if (param4 != _GM_NC) _level = !param4;
		break;	
	case RELAY2WIRE:
		_digA = param1;
		_digB = param2;
		if (param3 != _GM_NC) _level = !param3;
		break;		
	}

	if (_digA != _GM_NC) pinMode(_digA, OUTPUT);
	if (_digB != _GM_NC) pinMode(_digB, OUTPUT);
	if (_pwmC != _GM_NC) pinMode(_pwmC, OUTPUT);
	
	setMode(STOP);
}

void GMotor::setSpeed(int16_t duty) {
	if (_mode < 2) {
		_duty = constrain(duty, -_maxDuty, _maxDuty);
		
		// фикс стандартного analogWrite(пин, 255) для >8 бит
		if (_maxDuty > 255 && abs(_duty) == 255) _duty++;
		
		if (duty == 0) run(STOP, 0);
		else {
			if (duty > 0) {
				if (_minDuty != 0) _duty = _duty * _k + _minDuty;	// сжимаем диапазон
				run(_mode, _duty);
			} else {
				if (_minDuty != 0) _duty = _duty * _k - _minDuty;	// сжимаем диапазон
				run(BACKWARD, -_duty);
			}
		}
	}
}

void GMotor::run(GM_workMode mode, int16_t duty) {
	// дедтайм
	if (_deadtime > 0 && _lastMode != mode) {
		_lastMode = mode;
		setPins(_level, _level, 0);	// выключить всё
		delayMicroseconds(_deadtime);
	}

	// инверт (совместимость с есп)
	if (_direction) {
		if (mode == FORWARD) mode = BACKWARD;
		else if (mode == BACKWARD) mode = FORWARD;
	}

	switch (mode) {
	case FORWARD:	setPins(_level, !_level, duty); _state = 1; break;		
	case BACKWARD:	setPins(!_level, _level, (_type == DRIVER2WIRE) ? (_maxDuty - duty) : (duty)); _state = -1; break;
	case BRAKE:		setPins(!_level, !_level, !_level * 255); _state = 0; break;	// при 0/255 analogWrite сделает 0/1
	case STOP:		setPins(_level, _level, _level * 255); _state = 0; break;		
	}
}

void GMotor::setPins(bool a, bool b, int c) {
	if (_digA != _GM_NC) digitalWrite(_digA, a);
	if (_digB != _GM_NC) digitalWrite(_digB, b);
	if (_pwmC != _GM_NC) analogWrite(_pwmC, c);
}

void GMotor::smoothTick(int16_t duty) {
	if (millis() - _tmr >= _SMOOTH_PRD) {
		_tmr = millis();
		if (abs(_dutyS - duty) > _speed) _dutyS += (_dutyS < duty) ? _speed : -_speed;
		else _dutyS = duty;
		setSpeed(_dutyS);
	}
}

int GMotor::getState() {
	return _state;
}

void GMotor::setResolution(byte bit) {
	_maxDuty = (1 << bit) - 1;	// -1 для смещения
	setMinDuty(_minDuty);		// пересчитаем k
}

void GMotor::setMinDuty(int duty) {
	_minDuty = duty;
	_k = 1.0 - (float)_minDuty / _maxDuty;
}

void GMotor::setMode(GM_workMode mode) {	
	_mode = mode;
	run(mode, _duty);
}

void GMotor::setSmoothSpeed(uint8_t speed) {
	_speed = speed;
}

void GMotor::setDirection(bool direction) {
	_direction = direction;
}

void GMotor::setDeadtime(uint16_t deadtime) {
	_deadtime = deadtime;
}

void GMotor::setLevel(int8_t level) {
	_level = !level;
}

// совместимость
void GMotor::set8bitMode() {
	setResolution(8);
}

void GMotor::set10bitMode() {
	setResolution(10);
}