#ifndef HX711_h
#define HX711_h
#include <Arduino.h>

// режимы:
#define HX_GAIN128_A 0
#define HX_GAIN32_B 1
#define HX_GAIN64_A 2

class HX711 {
public:
	// пин дата, пин клок, режим HX_A_GAIN128/HX_B_GAIN32/HX_A_GAIN64
	HX711 (uint8_t data, uint8_t clock, uint8_t mode = 2) :
	_data(data), _clock(clock), _mode(mode) {
		pinMode(data, INPUT);
		pinMode(clock, OUTPUT);
	}
	
	// true - доступен для чтения
	boolean available() {
		return (!digitalRead(_data));
	}
	
	// получить данные
	long read() {
		if (available()) {
			_weight = 0;
			for (uint8_t i = 0; i < 24; i++) {
				digitalWrite(_clock, HIGH);
				delayMicroseconds(1);
				_weight <<= 1;
				if (digitalRead(_data)) _weight |= 1;
				digitalWrite(_clock, LOW);
				delayMicroseconds(1);
			}
			for (uint8_t i = 0; i < _mode + 1; i++) {
				digitalWrite(_clock, 1);
				delayMicroseconds(1);
				digitalWrite(_clock, 0);
				delayMicroseconds(1);
			}
			_weight = median3(_weight);
		}
		return _weight + _cal;
	}
	
	// тарировать (автоматическая калибровка)
	void tare() {
		_cal = -_weight;
	}
	
	// установить оффсет вручную
	void setOffset(long cal) {
		_cal = cal;
	}
	
	// получить оффсет
	long getOffset() {
		return _cal;
	}
	
	// true - режим сна, false - проснуться
	void sleepMode(bool mode) {
		digitalWrite(_clock, 0);
		if (mode) digitalWrite(_clock, 1);
	}

private:
	long buf[3];
	uint8_t counter = 0;
	// быстрая медиана
	long median3(long value) {
		buf[counter] = value;
		if (++counter > 2) counter = 0;
		if ((buf[0] <= buf[1]) && (buf[0] <= buf[2])) return (buf[1] <= buf[2]) ? buf[1] : buf[2];
		else {
			if ((buf[1] <= buf[0]) && (buf[1] <= buf[2])) return (buf[0] <= buf[2]) ? buf[0] : buf[2];
			else return (buf[0] <= buf[1]) ? buf[0] : buf[1];
		}
	}

	long _weight = 0, _cal = 0;
	const uint8_t _data, _clock, _mode;
};
#endif