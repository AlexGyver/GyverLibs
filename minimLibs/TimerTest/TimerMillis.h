#ifndef TimerMillis_h
#define TimerMillis_h
#include <Arduino.h>
// Удобный класс таймера на millis()

class TimerMillis {
public:
	// (период, мс), (0 не запущен / 1 запущен), (режим: 0 период / 1 таймер)
	TimerMillis(uint32_t prd = 1000, bool state = 0, bool mode = 0) {
		_prd = prd;
		_state = state;
		_mode = mode;
	}
	uint32_t uptime() {					// на случай использования в других фреймворках
		return millis();
	}
	void setTimerMode() {   			// установить в режим таймера
		_mode = 1;
	}
	void setPeriodMode() {  			// установить в режим периода
		_mode = 0;
	}
	void setTime(uint32_t prd) {  		// установить время работы
		_prd = prd;
	}
	void attach(void (*handler)()) {	// подключить коллбэк
		_handler = *handler;
	}

	void start() {          	// запустить/перезапустить таймер
		_state = true;  
		_tmr = uptime();
	}
	void restart() {
		start();
	}
	void resume() {         	// продолжить после остановки
		_state = true;
		_tmr = uptime() - _buf;
	}
	void stop() {           	// остановить/приостановить таймер
		_state = false;
		_buf = uptime() - _tmr;
	}
	void force() {          	// принудительно переполнить таймер
		_tmr = uptime() - _prd;
	}

	// в режиме периода однократно вернёт true при каждом периоде
	// в режиме таймера будет возвращать true при срабатывании
	bool tick() {
		if (_state) _buf = uptime() - _tmr;
		if (_state && elapsed()) {
			if (!_mode) _tmr += _prd;
			else stop();
			if (*_handler) _handler();
			_ready = 1;
			return true;
		}
		return false;
	}
	
	bool ready() {			// однократно вернёт true при срабатывании (флаг)
		if (_ready) {
			_ready = 0;
			return true;
		}
		return false;
	}
	bool elapsed() {        // всегда возвращает true при срабатывании
		return (uptime() - _tmr >= _prd);
	}
	bool active() {         // работает ли таймер (start/resume)
		return _state;
	}
	bool status() {         // elapsed+active: работает ли таймер + не сработал ли он      
		return _state && !elapsed();
	}
	
	// остаток времени
	uint32_t timeLeft() {   	// остаток времени в мс
		return max(long(_prd - _buf), 0L);
	}
	uint8_t timeLeft8() {   	// остаток времени в 0-255
		return max(255 - _buf * 255l / _prd, 0ul);
	}
	uint16_t timeLeft16() { 	// остаток времени в 0-65535
		return max(65535 - _buf * 65535l / _prd, 0ul);
	}

private:
	uint32_t _tmr = 0, _prd = 1000, _buf = 0;
	bool _state = 0, _mode = 0, _ready = 0;
	void (*_handler)() = NULL;
};
#endif
