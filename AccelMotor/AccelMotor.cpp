#include "AccelMotor.h"
#define _sign(x) ((x) > 0 ? 1 : -1)

bool AccelMotor::tick(long pos) {
	_currentPos = pos;
	if (millis() - _tmr2 > _dt) {
		_dts = (millis() - _tmr2) / 1000.0;
		_tmr2 = millis();
		_curSpeed = (long)(_currentPos - _lastPos) / _dts;	// ищем скорость в тиках/секунду
		_curSpeed = filter(_curSpeed);  // медиана + RA
		_lastPos = _currentPos;
		switch (_runMode) {		
		case ACCEL_POS:
			{
				long err = _targetPos - controlPos;												// "ошибка" позиции
				if (err != 0) {
					if (_accel != 0) {
						bool thisDir = (controlSpeed * controlSpeed / _accel / 2.0 >= abs(err));  // пора тормозить
						controlSpeed += _accel * _dts * (thisDir ? -_sign(controlSpeed) : _sign(err));
					} else {
						controlSpeed = err / _dts;	// профиль постоянной скорости
					}
					controlSpeed = constrain(controlSpeed, -_maxSpeed, _maxSpeed);
					controlPos += controlSpeed * _dts;
				}
				PIDcontrol(controlPos, _currentPos, true);
			}
			break;
		case PID_POS:				
			PIDcontrol(_targetPos, _currentPos, true);
			break;
		case ACCEL_SPEED:
			{				
				int err = _targetSpeed - _curSpeed;						// ошибка скорости
				//float reducer = min(abs(err) / _accel*10.0, 1.0);		// уменьшает ускорение, если шаг будет дальше чем установка
				_dutyF += (float)_sign(err) * _accel/10 * _dts;			// ускоряем/замедляем
				_dutyF = constrain(_dutyF, -_maxDuty, _maxDuty);		// ограничитель 8/10 бит
				setSpeed(_dutyF);				
			}			
			break;
		case PID_SPEED:			
			PIDcontrol(_targetSpeed, _curSpeed, false);			
			break;
		}		
	}	
	if (_runMode > 1) return (getState() != 0);
	else return (getState() != 0 || abs(_targetPos - _currentPos) > _stopzone);
}

void AccelMotor::PIDcontrol(long target, long current, bool cutoff) {
	// cutoff нужен только для стабилизации позиции, обнуляет integral и учитывает мёртвую зону мотора
	long err = target - current;				// ошибка регулирования
	long deltaInput = _prevInput - current;		// изменение входного сигнала
	_dutyF = 0;
	if (!cutoff) _dutyF = err * kp;				// P составляющая для режимов скорости	
	_dutyF += (float)deltaInput * kd / _dts;	// D составляющая
	_prevInput = current;						// запомнили текущий
	integral += (float)err * ki * _dts;			// интегральная сумма
	if (cutoff) integral += deltaInput * kp;	// +P по скорости изменения для режимов позиции
	integral = constrain(integral, -_maxDuty, _maxDuty);	// ограничили
	_dutyF += integral;							// I составляющая	
	if (cutoff) {								// отсечка (для режимов позиции)
		if (abs(err) < _stopzone) {integral = 0; _dutyF = 0;}
	} else {									// для скорости
		if (err == 0 && target == 0) integral = 0;
	}
	_dutyF = constrain(_dutyF, -_maxDuty, _maxDuty);	// ограничиваем по разрешению
	if (cutoff && _min != 0 && _max != 0 && (current <= _min || current >= _max)) {
		setSpeed(0);	// вырубаем, если вышли за диапазон
	} else setSpeed(_dutyF);								// и поехали
}

void AccelMotor::setRange(long min, long max) {
	_min = min;
	_max = max;
}
void AccelMotor::setRangeDeg(long min, long max) {
	_min = min * _ratio / 360.0;
	_max = max * _ratio / 360.0;
}

bool AccelMotor::isBlocked() {
	return (abs(_dutyF) > _minDuty && _curSpeed == 0);
}

// ===== settings =====
void AccelMotor::setRatio(float ratio) {
	_ratio = ratio;
}
void AccelMotor::setDt(int dt) {
	_dt = dt;
	_dts = dt / 1000.0;
}
void AccelMotor::setCurrent(long pos) {	
	_currentPos = pos;
}
void AccelMotor::setRunMode(AM_runMode mode) {
	_runMode = mode;
	if (mode == ACCEL_POS) controlPos = _currentPos;		// костыль!
}

// ===== current position =====
long AccelMotor::getCurrent() {
	return _currentPos;
}
long AccelMotor::getCurrentDeg() {
	return (long)_currentPos * 360.0 / _ratio;
}

// ===== current speed =====
int AccelMotor::getSpeed() {
	return _curSpeed;
}
int AccelMotor::getSpeedDeg() {
	return (long)_curSpeed * 360.0 / _ratio;
}
float AccelMotor::getDuty() {
	return _dutyF;
}

// ===== target position mode =====
void AccelMotor::setTarget(long pos) {
	_targetPos = pos;
	_mode = AUTO;
}
void AccelMotor::setTargetDeg(long pos) {
	_targetPos = (long)pos * _ratio / 360.0;
	_mode = AUTO;
}
long AccelMotor::getTarget() {
	return _targetPos;
}
long AccelMotor::getTargetDeg() {
	return (long)_targetPos * 360 / _ratio;
}

void AccelMotor::setStopZone(int zone) {
	_stopzone = zone;
}

// ===== target speed mode =====
void AccelMotor::setTargetSpeed(int speed) {
	_targetSpeed = speed;
	_mode = AUTO;
}
void AccelMotor::setTargetSpeedDeg(int speed) {
	_targetSpeed = (long)speed * _ratio / 360;
	_mode = AUTO;
}
int AccelMotor::getTargetSpeed() {
	return _targetSpeed;
}
int AccelMotor::getTargetSpeedDeg() {
	return (long)_targetSpeed * 360 / _ratio;
}

// ===== max speed / acceleration =====
void AccelMotor::setMaxSpeed(int speed) {
	_maxSpeed = speed;
}
void AccelMotor::setMaxSpeedDeg(int speed) {
	_maxSpeed = (long)speed * _ratio / 360;
}
void AccelMotor::setAcceleration(int accel) {
	_accel = accel;
}
void AccelMotor::setAccelerationDeg(int accel) {
	_accel = accel * _ratio / 360.0;
}

// ==== filter ====
int AccelMotor::filter(int newVal) {
	_buf[_count] = newVal;
	if (++_count >= 3) _count = 0;
	int middle = 0;
	if ((_buf[0] <= _buf[1]) && (_buf[0] <= _buf[2])) {
		middle = (_buf[1] <= _buf[2]) ? _buf[1] : _buf[2];
	} else {
		if ((_buf[1] <= _buf[0]) && (_buf[1] <= _buf[2])) {
			middle = (_buf[0] <= _buf[2]) ? _buf[0] : _buf[2];
		}
		else {
			middle = (_buf[0] <= _buf[1]) ? _buf[0] : _buf[1];
		}
	}
	_middle_f += (middle-_middle_f) * 0.7;
	return _middle_f;
}
