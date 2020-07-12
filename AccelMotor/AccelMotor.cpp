#include "AccelMotor.h"
#define _sign(x) ((x) > 0 ? 1 : -1)

bool AccelMotor::tick(long pos) {
	_currentPos = pos;
	if (millis() - _tmr2 > _dt) {
		_tmr2 += _dt;
		_curSpeed = (long)(_currentPos - _lastPos) / _dts;	// ищем скорость в тиках/секунду
		_curSpeed = filter(_curSpeed);  // медиана + RA
		_lastPos = _currentPos;
		
		switch (_runMode) {		
		case ACCEL_POS:
			{
				long err = _targetPos - controlPos;												// "ошибка" позиции
				if (err != 0) {
					float accelDt = _accel * _dts;
					float accel = accelDt;
					if (abs(err) < _stopzone && abs(_lastSpeed - controlSpeed) < 2) {			// условие завершения движения
						controlPos = _targetPos;
						controlSpeed = 0;
						accel = 0;
					}
					if (abs(err) < (float)(controlSpeed * controlSpeed) / (2.0 * accelDt)) {	// если пора тормозить
						err = -err;																// разворачиваем ускорение
						accel = (float)(controlSpeed * controlSpeed) / (2.0 * abs(err));		// пересчёт ускорения для плавности
						if (_sign(controlSpeed) == _sign(err)) err = -err;						// поворачивай обратно
					}
					controlSpeed += accel * _sign(err);											// применяем ускорение
					controlSpeed = constrain(controlSpeed, -_maxSpeed*_dts, _maxSpeed*_dts);	// ограничим
					controlPos += controlSpeed;													// двигаем позицию
					_lastSpeed = controlSpeed;	
					
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
				float reducer = min(abs(err) / _accel, 1.0);			// уменьшает ускорение, если шаг будет дальше чем установка
				_dutyF += (float)_sign(err) * _accel * _dts * reducer;	// ускоряем/замедляем
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
	long err = target - current;
	_dutyF = err * kp;										// P составляющая	
	_dutyF += (float)(_prevInput - current) * kd / _dts;	// D составляющая
	_prevInput = current;
	integral += (float)err * ki * _dts;
	_dutyF += integral;										// I составляющая	
	if (cutoff) {		// отсечка
		if (abs(err) > _stopzone) _dutyF += _sign(err)*_minDuty;	// учитываем _minDuty - мёртвую зону мотора (метод setMinDuty)
		else integral = 0;		
	} else {
		if (err == 0 && target == 0) integral = 0;
		else _dutyF += _sign(err)*_minDuty;
	}
	_dutyF = constrain(_dutyF, -_maxDuty, _maxDuty);	// ограничиваем по 8 или 10 бит
	setSpeed(_dutyF);									// и поехали
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
void AccelMotor::setRunMode(runMode mode) {
	_runMode = mode;
	if (mode == ACCEL_POS) controlPos = _currentPos;		// костыль!
}

// ===== current position =====
long AccelMotor::getCurrent() {
	return _currentPos;
}
long AccelMotor::getCurrentDeg() {
	return (long)_currentPos * 360 / _ratio;
}

// ===== current speed =====
int AccelMotor::getSpeed() {
	return _curSpeed;
}
int AccelMotor::getSpeedDeg() {
	return (long)_curSpeed * 360 / _ratio;
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
	_targetPos = (long)pos * _ratio / 360;
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
void AccelMotor::setAcceleration(float accel) {
	_accel = accel;
}
void AccelMotor::setAccelerationDeg(float accel) {
	_accel = (float)accel * _ratio / 360.0;
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
