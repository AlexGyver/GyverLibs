#pragma once

// класс тахометра
// встроенный медианный фильтр
// вызывай tick в прерывании по фронту

#define _TACHO_TICKS_AMOUNT 5	// количество тиков для усреднения
#define _TACHO_TIMEOUT 1000000	// таймаут прерываний (мкс), после которого считаем что вращение прекратилось

class Tacho {
public:
	void tick() {
		ticks++;
		if (ticks == _TACHO_TICKS_AMOUNT) {
			ticks = 0;
			tachoTime = micros() - tachoTimer;
			tachoTimer += tachoTime;	//tachoTimer = micros();
			ready = true;
		}
	}
	
	int getRPM() {
		if (ready) {	// если готовы новые данные
			ready = false;
			rpm = median3(6ul * _TACHO_TICKS_AMOUNT * 1000000 / tachoTime);
		}
		if (micros() - tachoTimer > _TACHO_TIMEOUT) rpm = 0;
		return rpm;
	}
	
private:
	// быстрая медиана
	int median3(int value) {
		buf[counter] = value;
		if (++counter > 2) counter = 0;
		if ((buf[0] <= buf[1]) && (buf[0] <= buf[2])) return (buf[1] <= buf[2]) ? buf[1] : buf[2];
		else {
			if ((buf[1] <= buf[0]) && (buf[1] <= buf[2])) return (buf[0] <= buf[2]) ? buf[0] : buf[2];
			else return (buf[0] <= buf[1]) ? buf[0] : buf[1];
		}
	}

	volatile long tachoTime = 0;
	volatile uint32_t tachoTimer = 0;
	volatile byte ticks = 0;
	volatile bool ready;
	int buf[3];
	byte counter = 0;
	int rpm = 0;
};