#include "smoothUtil.h"

// ====== WRITE ======
void Smooth::writeUs(int val) {
	sendToDriver(_dir ? (_max + _min - val) : val);
}
void Smooth::write(uint16_t angle) {
	writeUs(map(angle, 0, _maxAngle, _min, _max));
	_servoCurrentPos = (map(angle, 0, _maxAngle, _min, _max));
}

void Smooth::writeMicroseconds(uint16_t val) {
	sendToDriver(val);
}

void Smooth::sendToDriver(uint16_t val) {
	// пустышка, заменить производным
}

// ====== ATTACH ======
void Smooth::attach(int pin) {
	// пустышка, заменить производным
}

void Smooth::detach() {
	// пустышка, заменить производным
}

void Smooth::attach() {
	attach(_pin);
}

void Smooth::attach(int pin, int target) {
	_pin = pin;
	attach(_pin);
	if (target <= _maxAngle) target = map(target, 0, _maxAngle, _min, _max);	
	writeUs(target);
	_servoTargetPos = target;
	_servoCurrentPos = target;
}

void Smooth::attach(int pin, int min, int max, int target) {
	attach(pin, target);
	_min = min;
	_max = max;
}

void Smooth::start() {
	attach(_pin);	
	_tickFlag = true;
}

void Smooth::stop() {
	detach();
	_tickFlag = false;
}

// ====== SET ======
void Smooth::setSpeed(int speed) {
	_servoMaxSpeed = speed;
}

void Smooth::setAccel(float accel) {
	_k = accel;
}

void Smooth::setTarget(int target) {
	_servoTargetPos = target;
}

void Smooth::setTargetDeg(int target) {
	target = constrain(target, 0, _maxAngle);
	_servoTargetPos = map(target, 0, _maxAngle, _min, _max);
}

void Smooth::setCurrent(int target) {
	_servoCurrentPos = target;
}

void Smooth::setCurrentDeg(int target) {
	target = constrain(target, 0, _maxAngle);
	_servoCurrentPos = map(target, 0, _maxAngle, _min, _max);
}

int Smooth::getCurrent() {
	return (int)_servoCurrentPos;
}
int Smooth::getCurrentDeg() {
	return (map((int)_servoCurrentPos, _min, _max, 0, _maxAngle));
}

int Smooth::getTarget() {
	return _servoTargetPos;
}
int Smooth::getTargetDeg() {
	return (map(_servoTargetPos, _min, _max, 0, _maxAngle));
}

void Smooth::setAutoDetach(boolean set) {
	_autoDetach = set;
}

void Smooth::setMaxAngle(int maxAngle) {
	_maxAngle = maxAngle;
}

void Smooth::setDirection(bool dir) {
	_dir = dir;
}


// ====== TICK ======
// знак числа
#define _sign(x) ((x) > 0 ? 1 : -1)

boolean Smooth::tickManual() {	
	if (_tickFlag) {
		int err = _servoTargetPos - _servoCurrentPos;
		if (err != 0) {
			float accel = _k;
			static float lastSpeed;
			if (abs(err) < SS_DEADZONE && abs(lastSpeed - _speed) < 1) {
				writeUs(_servoTargetPos);
				_servoCurrentPos = _servoTargetPos;
				_speed = 0;
				lastSpeed = 0;
				if (_autoDetach && _servoState) {			
					_servoState = false;
					detach();
				}
				return true;
			} else {
				if (_autoDetach && !_servoState) {
					_servoState = true;
					attach(_pin);
				}
			}
			if (_servoState) {
				if (abs(err) < (float)(_speed * _speed) / (2.0 * _k)) {
					err = -err;
					accel = (float)(_speed * _speed) / (2.0 * abs(err));
					if (_sign(_speed) == _sign(err)) err = -err;
				}
				_speed += (float)accel * _sign(err);
				_speed = constrain(_speed, -_servoMaxSpeed, _servoMaxSpeed);
				_servoCurrentPos += _speed;
				lastSpeed = _speed;
				writeUs(_servoCurrentPos);
			}
			return false;
		}
	}
	return true;
	/*
	// алгоритм до версии 3.0
	if (_tickFlag) {
		int _newSpeed = _servoTargetPos - _servoCurrentPos;						// расчёт скорости
		if (_servoState) {
			_newSpeed = constrain(_newSpeed, -_servoMaxSpeed, _servoMaxSpeed);	// ограничиваем по макс.
			_servoCurrentPos += _newSpeed;										// получаем новую позицию			
			_newPos += (float)(_servoCurrentPos - _newPos) * _k;				// и фильтруем её
			_newPos = constrain(_newPos, _min, _max);							// ограничиваем
			writeUs((int)_newPos);												// отправляем на серво
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
	}
	return false;
	*/
}

boolean Smooth::tick() {
	if (millis() - _prevServoTime >= SS_SERVO_PERIOD) {
		_prevServoTime += SS_SERVO_PERIOD;
		Smooth::tickManual();
	}
	return !_servoState;
}