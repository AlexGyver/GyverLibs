#include <Arduino.h>
#include "GyverTimer.h"

// конструктор
GTimer::GTimer(timerType type, uint32_t interval) {
	setInterval(interval);		// запуск в режиме ИНТЕРВАЛА
	_type = type;				// установка типа
}

// запуск в режиме интервала
void GTimer::setInterval(uint32_t interval) {
	if (interval != 0) {				// защита от ввода 0
		_interval = interval;			// установка
		_mode = TIMER_INTERVAL;			// режим "интервал"
		start();						// сброс и запуск	
	} else stop();						// остановка, если время == 0
}

// запуск в режиме таймаута
void GTimer::setTimeout(uint32_t timeout) {
	setInterval(timeout);		// задать интервал и запустить
	_mode = TIMER_TIMEOUT;		// режим "таймаут"
}

// остановить счёт
void GTimer::stop() {
	_state = false;
	_resumeBuffer = ( (_type) ? millis() : micros() ) - _timer;		// запомнили "время остановки"
}

// продолжить счёт
void GTimer::resume() {
	start();
	_timer -= _resumeBuffer;	// восстановили время остановки
}

// перезапустить счёт
void GTimer::start() {
	_state = true;
	reset();
}

// сброс периода
void GTimer::reset() {
	_timer = (_type) ? millis() : micros();
}

// состояние
boolean GTimer::isEnabled() {
	return _state;
}

// проверка таймера v2.0 (соблюдение интервалов, защита от пропуска и переполнения)
boolean GTimer::isReady() {	
	if (!_state) return false;							// если таймер остановлен
	uint32_t thisTime = (_type) ? millis() : micros();	// текущее время
	if (thisTime - _timer >= _interval) {				// проверка времени
		if (_mode) {						// если режим интервалов
			do {
				_timer += _interval;
				if (_timer < _interval) break;			// защита от переполнения uint32_t
			} while (_timer < thisTime - _interval);	// защита от пропуска шага			
		} else {							// если режим таймаута
			_state = false;					// остановить таймер
		}
		return true;
	} else {
		return false;
	}
}

// сменить режим вручную
void GTimer::setMode(boolean mode) {
	_mode = mode;
}

// ================== УСТАРЕЛО (но работает, для совместимости) ===================

// ====== millis ======
GTimer_ms::GTimer_ms() {}

GTimer_ms::GTimer_ms(uint32_t interval) {
	_interval = (interval == 0) ? 1 : interval;		// защита от ввода 0
	reset();
}

void GTimer_ms::setInterval(uint32_t interval) {
	_interval = (interval == 0) ? 1 : interval;		// защита от ввода 0
	GTimer_ms::reset();
	_state = true;
	_mode = true;
}
void GTimer_ms::setTimeout(uint32_t interval) {
	setInterval(interval);
	_mode = false;
}
void GTimer_ms::setMode(uint8_t mode) {
	_mode = mode;
}
void GTimer_ms::start() {
	_state = true;
}
void GTimer_ms::stop() {
	_state = false;
}
boolean GTimer_ms::isReady() {	
	if (!_state) return false;
	uint32_t thisMls = millis();
	if (thisMls - _timer >= _interval) {
		if (_mode) {
			do {
				_timer += _interval;
				if (_timer < _interval) break;          // переполнение uint32_t
			} while (_timer < thisMls - _interval);  // защита от пропуска шага			
		} else {
			_state = false;
		}
		return true;
	} else {
		return false;
	}
}

void GTimer_ms::reset() {
	_timer = millis();
}

// ====== micros ======
GTimer_us::GTimer_us() {}

GTimer_us::GTimer_us(uint32_t interval) {
	_interval = (interval == 0) ? 1 : interval;		// защита от ввода 0
	reset();
}

void GTimer_us::setInterval(uint32_t interval) {
	_interval = (interval == 0) ? 1 : interval;		// защита от ввода 0
	GTimer_us::reset();
	_state = true;
	_mode = true;
}
void GTimer_us::setTimeout(uint32_t interval) {
	setInterval(interval);
	_mode = false;
}
void GTimer_us::setMode(uint8_t mode) {
	_mode = mode;
}
void GTimer_us::start() {
	_state = true;
}
void GTimer_us::stop() {
	_state = false;
}
boolean GTimer_us::isReady() {
	if (!_state) return false;
	uint32_t thisUs = micros();
	if (thisUs - _timer >= _interval) {
		if (_mode) {
			do {
				_timer += _interval;
				if (_timer < _interval) break;          // переполнение uint32_t
			} while (_timer < thisUs - _interval);  // защита от пропуска шага			
		} else {
			_state = false;
		}
		return true;
	} else {
		return false;
	}
}

void GTimer_us::reset() {
	_timer = micros();
}