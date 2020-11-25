#include "smoothUtil.h"

// знак числа
static int _sign(int x) {return ((x) > 0 ? 1 : -1);}

// ====== WRITE ======
// отправить на драйвер с учётом направления
void Smooth::writeUs(int val) {
	sendToDriver(_dir ? (_max + _min - val) : val);
}

// жёстко повернуть на угол (опираясь на min и max)
void Smooth::write(uint16_t angle) {
	angle = map(angle, 0, _maxAngle, _min, _max);
	writeMicroseconds(angle);
}

// жёстко повернуть на импульс (без ограничений)
void Smooth::writeMicroseconds(uint16_t val) {
	writeUs(val);
	_servoCurrentPos = val;
	_servoTargetPos = val;
}

// отправить значение на драйвер
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
	if (target <= _maxAngle) write(target);		// если в градусах
	else writeMicroseconds(target);
}

void Smooth::attach(int pin, int min, int max, int target) {
	_min = min;
	_max = max;
	attach(pin, target);
}

void Smooth::start() {
	attach(_pin);	
	_tickFlag = true;
}

void Smooth::stop() {
	detach();
	_tickFlag = false;
	_speed = 0;
	_lastSpeed = 0;
}

void Smooth::smoothStart() {
	for (byte i = 0; i < 10; i++) {
		detach();
		delay(75);
		attach();
		writeUs(_servoCurrentPos);
		delay(25);
	}	
}

// ====== SET ======
void Smooth::setSpeed(int speed) {
	_servoMaxSpeed = (long)speed * _max / _maxAngle;	// ~ перевод из градусов в секунду в тики
}

void Smooth::setAccel(double accel) {
	_acceleration = (float)accel * _max * 3;			// для совместимости со старыми скетчами (уск. 0.1-1)
}

void Smooth::setAccel(int accel) {
	_acceleration = (long)accel * (_max - _min) / _maxAngle;	// напрямую в градусах/сек/сек (перевод в тики)
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
	return round(_servoCurrentPos);
}
int Smooth::getCurrentDeg() {
	return (map(round(_servoCurrentPos), _min, _max, 0, _maxAngle));
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
boolean Smooth::tick() {
	if (millis() - _prevServoTime >= SS_SERVO_PERIOD) {
		_prevServoTime = millis();
		Smooth::tickManual();
	}
	return !_servoState;
}

boolean Smooth::tickManual() {	
	if (_tickFlag) {
		int err = _servoTargetPos - _servoCurrentPos;
		if (abs(err) > SS_DEADZONE && abs(_lastSpeed - _speed) < SS_DEADZONE_SP) {			// условие остановки
			if (_acceleration != 0) {
				bool thisDir = ((float)_speed * _speed / _acceleration / 2.0 >= abs(err));  	// пора тормозить
				_speed += (float)_acceleration * _delta * (thisDir ? -_sign(_speed) : _sign(err));
			} else {
				_speed = err/_delta;
			}
			_speed = constrain(_speed, -_servoMaxSpeed, _servoMaxSpeed);
			_servoCurrentPos += _speed * _delta;
			if (!_servoState) {
				_servoState = true;
				timeoutCounter = 0;
				if (_autoDetach) attach(_pin);
			}
			writeUs(_servoCurrentPos);
		} else {
			//_servoCurrentPos = _servoTargetPos;
			_speed = 0;			
			if (_servoState) {
				writeUs(_servoCurrentPos);
				timeoutCounter++;
			}
			if (timeoutCounter > SS_DEADTIME && _servoState) {			
				_servoState = false;
				if (_autoDetach) detach();
			}
		}
		_lastSpeed = _speed;
	}
	return !_servoState;
}

/*
// алгоритм версии 3.0
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
*/
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