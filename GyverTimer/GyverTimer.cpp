#include <Arduino.h>
#include "GyverTimer.h"

// конструктор
GTimer::GTimer(const timerType &type, const uint32_t &interval, const boolean &readyOnStart) {
	flagWrite(GTBIT_TYPE, type); 					// установка типа
	flagWrite(GTBIT_READYONSTART, readyOnStart); 	// установка флага на нулевой запуск
	setInterval(interval);							// запуск в режиме ИНТЕРВАЛА
}

// запуск в режиме интервала
void GTimer::setInterval(const uint32_t &interval) {
	setTimeout(interval);							// задать интервал и запустить
	flagWrite(GTBIT_MODE, TIMER_INTERVAL);			// режим "интервал"
}
// запуск в режиме таймаута
void GTimer::setTimeout(const uint32_t &timeout) {
	if (timeout != 0) {								// защита от ввода 0
		_interval = timeout;						// установка
		flagWrite(GTBIT_MODE, TIMER_TIMEOUT); 		// режим "таймаут"
		start();									// сброс и запуск	
	} else stop();									// остановка, если время == 0
}
// изменить режим на интервал, установить значение флага нулевой готовности и запустить
void GTimer::setReadyOnStart(const bool &readyOnStart) {
	flagWrite(GTBIT_READYONSTART, readyOnStart);	// выставляем флаг нулевой готовности
	flagWrite(GTBIT_MODE, TIMER_INTERVAL); 			// режим "интервал"
	start();										// сброс и запуск
}

// возвращает значение интервала/таймаута
uint32_t GTimer::getInterval() {
	return _interval;
}

// остановить счёт
void GTimer::stop() {
	// TODO: здесь должна быть защита от остановки остановленного таймера
	//       иначе в буфере будет только время последнего вызова stop()
	flagClear(GTBIT_STATE);
	_resumeBuffer = ( (flagRead(GTBIT_TYPE)) ? millis() : micros() ) - _timer; 	// запомнили "время остановки"
}

// продолжить счёт
void GTimer::resume() {
	if (flagRead(GTBIT_STATE)) return;				// защита от вызова, если таймер не остановлен
	start();
	_timer -= _resumeBuffer;						// восстановили время остановки
}

// перезапустить счёт
void GTimer::start() {
	flagSet(GTBIT_STATE);
	flagWrite(GTBIT_JUSTSTARTED, flagRead(GTBIT_READYONSTART)); 	// поднимаем флаг нулевой готовности
	reset();
}

// сброс периода
void GTimer::reset() {
	_timer = flagRead(GTBIT_TYPE) ? millis() : micros();
}

// состояние
boolean GTimer::isEnabled() {
	return flagRead(GTBIT_STATE);
}

// проверка таймера v2.0 (соблюдение интервалов, защита от пропуска и переполнения)
boolean GTimer::isReady() {	
	if (!flagRead(GTBIT_STATE)) return false;						// если таймер остановлен
	if (flagRead(GTBIT_JUSTSTARTED) && flagRead(GTBIT_MODE)) { 		// если режим интервалов и включен нулевой запуск
		flagWrite(GTBIT_JUSTSTARTED, 0);
		return true;
	}
	uint32_t thisTime = (flagRead(GTBIT_TYPE)) ? millis() : micros(); // текущее время
	if (thisTime - _timer >= _interval) { 							// проверка времени
		if (flagRead(GTBIT_MODE)) {									// если режим интервалов
			do {
				_timer += _interval;
				if (_timer < _interval) break;						// защита от переполнения uint32_t
			} while (_timer < thisTime - _interval);				// защита от пропуска шага			
		} else {													// если режим таймаута
			flagClear(GTBIT_STATE);									// остановить таймер
		}
		return true;
	} else {
		return false;
	}
}

// сменить режим вручную
void GTimer::setMode(const boolean &mode) {
	flagWrite(GTBIT_MODE, mode);
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