#include <Servo.h>
#include "ServoSmooth.h"

// ====== CONSTRUCTOR ======
ServoSmooth::ServoSmooth(int maxAngle) {
	_maxAngle = maxAngle;
}

// ====== WRITE ======
void ServoSmooth::writeUs(int val) {
	_servo.writeMicroseconds(_dir ? (_max - val) : val);
}
void ServoSmooth::write(uint16_t angle) {
	writeUs(map(angle, 0, _maxAngle, _min, _max));
}

void ServoSmooth::writeMicroseconds(uint16_t angle) {
	writeUs(angle);
}

// ====== ATTACH ======
void ServoSmooth::attach() {
	_servo.attach(_pin);
}

void ServoSmooth::attach(uint8_t pin, int target) {
	_pin = pin;
	_servo.attach(_pin);
	if (target <= _maxAngle) target = map(target, 0, _maxAngle, _min, _max);	
	writeUs(target);
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
	target = constrain(target, 0, _maxAngle);
	_servoTargetPos = map(target, 0, _maxAngle, _min, _max);
}

void ServoSmooth::setCurrent(int target) {
	_servoCurrentPos = target;
	_newPos = _servoCurrentPos;
}

void ServoSmooth::setCurrentDeg(int target) {
	target = constrain(target, 0, _maxAngle);
	_servoCurrentPos = map(target, 0, _maxAngle, _min, _max);
	_newPos = _servoCurrentPos;
}

int ServoSmooth::getCurrent() {
	return (int)_newPos;
}
int ServoSmooth::getCurrentDeg() {
	return (map((int)_newPos, _min, _max, 0, _maxAngle));
}

int ServoSmooth::getTarget() {
	return _servoTargetPos;
}
int ServoSmooth::getTargetDeg() {
	return (map(_servoTargetPos, _min, _max, 0, _maxAngle));
}

void ServoSmooth::setAutoDetach(boolean set) {
	_autoDetach = set;
}

void ServoSmooth::setMaxAngle(int maxAngle) {
	_maxAngle = maxAngle;
}

void ServoSmooth::setDirection(bool dir) {
	_dir = dir;
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
			writeUs((int)_newPos);								// отправляем на серво
		}
	}
	if (abs(_servoTargetPos - (int)_newPos) < SS_DEADZONE) {
		if (_autoDetach && _servoState) {			
			_servoCurrentPos = _servoTargetPos;
			_servoState = false;
			_servo.detach();
		}			
		return !_servoState;	// приехали
	} else {
		if (_autoDetach && !_servoState) {
			_servoState = true;
			_servo.attach(_pin);
		}
		_timeoutCounter = 0;		
	}
	return false;
}

boolean ServoSmooth::tick() {
	if (millis() - _prevServoTime >= SS_SERVO_PERIOD) {
		_prevServoTime = millis();
		ServoSmooth::tickManual();
	}
	return !_servoState;
}