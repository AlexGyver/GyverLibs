#include <PWMrelay.h>

PWMrelay::PWMrelay(byte pin, bool dir = false, int period = 1000) {
	_pin = pin;
	_dir = !dir;
	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, _dir);  // сразу выкл
	PWMrelay::setPeriod(period);
}

void PWMrelay::tick() {
	if (_duty != 0 && _duty != 255) {
		if (millis() - _tmr >= (_flag ? _activePeriod : (_period - _activePeriod))) {
			_tmr = millis();
			_flag = !_flag;
			digitalWrite(_pin, _flag ^ _dir);
		}
	}
}

void PWMrelay::setPWM(byte duty) {
	_duty = duty;
	_activePeriod = (long)_duty * _period / 255;
	if (_duty == 0) digitalWrite(_pin, _dir);		// выкл
	if (_duty == 255) digitalWrite(_pin, !_dir);	// вкл
}

byte PWMrelay::getPWM() {
	return _duty;
}

void PWMrelay::setPeriod(int period) {
	_period = period;
	PWMrelay::setPWM(_duty);	// на случай "горячей" смены периода
}

int PWMrelay::getPeriod() {
	return _period;
}

void PWMrelay::setLevel(bool level) {
	_dir = level;
}