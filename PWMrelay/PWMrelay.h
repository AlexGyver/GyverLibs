#pragma once
#include <Arduino.h>
/*
	PWMrelay - библиотека для генерации низкочастотного ШИМ сигнала для реле (для ПИД регуляторов и проч.)
	- Настройка периода ШИМ
	- Настройка сигнала 0-255 (8 бит)
	- Поддержка реле низкого и высокого уровня
	- Неблокирующий вызов, не использует таймеры (кроме системного), работает на millis()
*/

class PWMrelay {
public:
	PWMrelay(byte pin, bool dir, int period);	// пин, уровень реле HIGH/LOW, период
	void tick();					// тик, вызывать как можно чаще, сам управляет реле
	void setPWM(byte duty);			// установить величину ШИМ, 0-255. При значении 0 и 255 тик неактивен!
	byte getPWM();					// возвращает величину ШИМ
	void setPeriod(int period);		// установить период ШИМ в миллисек. (по умолч. 1000мс == 1с)
	int getPeriod();				// получить период
	void setLevel(bool level);		// установить установить уровень реле (HIGH/LOW)
	
private:
	bool _flag = false;
	bool _dir = false;
	byte _pin = 0;
	byte _duty = 0;
	int _period = 1000;
	int _activePeriod = 0;
	uint32_t _tmr = 0;
};

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