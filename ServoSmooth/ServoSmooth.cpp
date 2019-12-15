#include <Servo.h>
#include "ServoSmooth.h"

// ====== WRITE ======
void ServoSmooth::write(uint16_t angle) {
	_servo.write(angle);
}

void ServoSmooth::writeMicroseconds(uint16_t angle) {
	_servo.writeMicroseconds(angle);
}

// ====== ATTACH ======
void ServoSmooth::attach(uint8_t pin, int target) {
	_pin = pin;
	_servo.attach(_pin);
	if (target <= 180) target = map(target, 0, 180, _min, _max);	
	_servo.writeMicroseconds(target);
	_servoTargetPos = target;
	_servoCurrentPos = target;
	_newPos = target;
}

void ServoSmooth::attach(uint8_t pin, int min, int max, int target) {
	attach(pin, target);
	_min = min;
	_max = max;
}

void ServoSmooth::detach() {
	_servo.detach();
}

void ServoSmooth::start() {
	_servo.attach(_pin);	
	_tickFlag = true;
}

void ServoSmooth::stop() {
	_servo.detach();
	_tickFlag = false;
}

// ====== SET ======
void ServoSmooth::setSpeed(int speed) {
	_servoMaxSpeed = speed;
}

void ServoSmooth::setAccel(float accel) {
	_k = accel;
}

void ServoSmooth::setTarget(int target) {
	_servoTargetPos = target;
}

void ServoSmooth::setTargetDeg(int target) {
	target = constrain(target, 0, 180);
	_servoTargetPos = map(target, 0, 180, _min, _max);
}

void ServoSmooth::setCurrent(int target) {
	_servoCurrentPos = target;
	_newPos = _servoCurrentPos;
}

void ServoSmooth::setCurrentDeg(int target) {
	target = constrain(target, 0, 180);
	_servoCurrentPos = map(target, 0, 180, _min, _max);
	_newPos = _servoCurrentPos;
}

int ServoSmooth::getCurrent() {
	return (int)_newPos;
}
int ServoSmooth::getCurrentDeg() {
	return (map((int)_newPos, _min, _max, 0, 180));
}

int ServoSmooth::getTarget() {
	return _servoTargetPos;
}
int ServoSmooth::getTargetDeg() {
	return (map(_servoTargetPos, _min, _max, 0, 180));
}

void ServoSmooth::setAutoDetach(boolean set) {
	_autoDetach = set;
}

// ====== TICK ======
boolean ServoSmooth::tickManual() {
	if (_tickFlag) {
		_newSpeed = _servoTargetPos - _servoCurrentPos;							// расчёт скорости
		if (_servoState) {
			_newSpeed = constrain(_newSpeed, -_servoMaxSpeed, _servoMaxSpeed);	// ограничиваем по макс.
			_servoCurrentPos += _newSpeed;										// получаем новую позицию			
			_newPos += (float)(_servoCurrentPos - _newPos) * _k;				// и фильтруем её
			_newPos = constrain(_newPos, _min, _max);							// ограничиваем
			_servo.writeMicroseconds((int)_newPos);									// отправляем на серво
		}			
	}
	if (abs(_servoTargetPos - (int)_newPos) < SS_DEADZONE) {		
		if (_autoDetach && _servoState) {
			_timeoutCounter++;
			if (_timeoutCounter > SS_TIMEOUT) {
				_servoState = false;
				_servo.detach();
			}
		}
		return true;
	} else {
		if (_autoDetach && !_servoState) {
			_servoState = true;
			_servo.attach(_pin);
		}
		_timeoutCounter = 0;
		return false;
	}
	
}

boolean ServoSmooth::tick() {
	if (millis() - _prevServoTime >= SS_SERVO_PERIOD) {
		_prevServoTime = millis();
		if (ServoSmooth::tickManual()) return true;
		else return false;
	}
}