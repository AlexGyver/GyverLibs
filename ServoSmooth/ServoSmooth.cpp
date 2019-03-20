#include <Servo.h>
#include "ServoSmooth.h"

void ServoSmooth::write(uint16_t angle) {
	_servo.write(angle);
}

void ServoSmooth::writeMicroseconds(uint16_t angle) {
	_servo.writeMicroseconds(angle);
}

void ServoSmooth::attach(uint8_t pin) {
	_servo.attach(pin);
	_pin = pin;
}

void ServoSmooth::attach(uint8_t pin, int min, int max) {
	_servo.attach(pin);
	_pin = pin;
	_min = min;
	_max = max;
}

void ServoSmooth::start() {
	_servo.attach(_pin);
	_tickFlag = true;
}

void ServoSmooth::stop() {
	_servo.detach();
	_tickFlag = false;
}

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

boolean ServoSmooth::tickManual() {
	if (_tickFlag) {
		_newSpeed = _servoTargetPos - _servoCurrentPos;						// расчёт скорости
		if (_servoState) {
			_newSpeed = constrain(_newSpeed, -_servoMaxSpeed, _servoMaxSpeed);	// ограничиваем по макс.
			_servoCurrentPos += _newSpeed;										// получаем новую позицию
			_newPos += (float)(_servoCurrentPos - _newPos) * _k;				// и фильтруем её
			_newPos = constrain(_newPos, _min, _max);							// ограничиваем
			_servo.writeMicroseconds(_newPos);									// отправляем на серво	
		}			
	}
	if (abs(_newSpeed) < DEADZONE) {
		if (_servoState) {
			_timeoutCounter++;
			if (_timeoutCounter > TIMEOUT) {
				_servoState = false;
				_servo.detach();
			}
		}		
		return true;
	} else {
		if (!_servoState) {
			_servoState = true;
			_servo.attach(_pin);
		}
		_timeoutCounter = 0;
		return false;
	}
		
}

boolean ServoSmooth::tick() {
	if (millis() - _prevServoTime >= SERVO_PERIOD) {
		_prevServoTime = millis();
		if (ServoSmooth::tickManual()) return true;
		else return false;
	}
}