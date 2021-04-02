#ifndef TimerMillis_h
#define TimerMillis_h
#include <Arduino.h>
// Удобный класс таймера на millis()

class TimerMillis {
public:
	// период, 0 не запущен / 1 запущен, режим 0 период / 1 таймер
	TimerMillis(uint32_t prd = 1000, bool state = 0, bool mode = 0) {
		_prd = prd;
		_state = state;
		_mode = mode;
	}
	void setTimerMode() {   // установить в режим таймера
		_mode = 1;
	}
	void setPeriodMode() {  // установить в режим периода
		_mode = 0;
	}
	void setTime(uint32_t prd) {  // установить время работы таймера
		_prd = prd;
	}
	void attach(void (*handler)()) {
		_handler = *handler;
	}

	void start() {          // запустить/перезапустить таймер
		_state = true;  
		_tmr = millis();
	}
	void resume() {         // продолжить после остановки
		_state = true;
		_tmr = millis() - _buf;
	}
	void stop() {           // остановить таймер      
		_state = false;
		_buf = millis() - _tmr;
	}
	void force() {          // принудительно переполнить таймер
		_tmr = millis() - _prd;
	}

	// в режиме периода однократно вернёт true при каждом периоде
	// в режиме таймера будет возвращать true при переполнении
	bool tick() {
		if (_state) _buf = millis() - _tmr;
		if (_state && elapsed()) {
			if (!_mode) _tmr += _prd;    
			if (*_handler) _handler();
			_ready = 1;
			return true;
		}
		return false;
	}
	bool ready() {
		if (_state && _ready) {
			_ready = 0;
			return true;
		}
		return false;
	}
	bool elapsed() {        // вернёт true при переполнении
		return (millis() - _tmr >= _prd);
	}
	bool active() {         // работает ли таймер (вкл выкл)
		return _state;
	}
	bool status() {         // работает ли таймер + не переполнился ли он      
		return _state && !elapsed();
	}

	uint32_t timeLeft() {    // остаток времени в мс
		return tick() ? 0 : max(long(_prd - _buf), 0L);
	}
	uint8_t timeLeft8() {   // остаток времени в 0-255
		return tick() ? 0 : max(255 - _buf * 255l / _prd, 0ul);
	}
	uint16_t timeLeft16() { // остаток времени в 0-65535
		return tick() ? 0 : max(65535 - _buf * 65535l / _prd, 0ul);
	}

private:
	uint32_t _tmr = 0, _prd = 1000, _buf = 0;
	bool _state = 0, _mode = 0, _ready = 0;
	void (*_handler)() = NULL;
};
#endif
