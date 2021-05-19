#ifndef GyverDimmer_h
#define GyverDimmer_h
#include <Arduino.h>
#include "FastIO.h"

// брезенхем одноканальный
template < uint8_t _D_PIN >
class DimmerBres {
public:
	DimmerBres() {
		pinMode(_D_PIN, OUTPUT);
		fastWrite(_D_PIN, LOW);
	}
	void write(uint8_t dim) {
		dimmer = dim;
	}
	void tick() {		
		int val = ((uint16_t)++count * dimmer) >> 9;		
		if (lastVal != (val != last)) fastWrite(_D_PIN, val != last);
		lastVal = (val != last);
		last = val;
	}
private:
	uint8_t count = 0, last = 0, lastVal = 0, dimmer = 0;
};

// брезенхем многоканальный
template < uint8_t _D_AMOUNT >
class DimmerBresMulti {
public:
	void attach(uint8_t num, uint8_t pin) {
		dimPins[num] = pin;
		pinMode(pin, OUTPUT);
	}
	void write(uint8_t pin, uint8_t dim) {
		dimmer[pin] = dim;
	}
	void tick() {		
		count++;
		for (byte i = 0; i < _D_AMOUNT; i++) {
			int val = ((uint16_t)count * dimmer[i]) >> 9;
			if (lastState[i] != (val != last[i])) fastWrite(dimPins[i], val != last[i]);
			lastState[i] = (val != last[i]);
			last[i] = val;
		}
	}
private:
	uint8_t count, last[_D_AMOUNT], lastState[_D_AMOUNT], dimmer[_D_AMOUNT], dimPins[_D_AMOUNT];
};

// плавный диммер одноканальный
template < uint8_t _D_PIN >
class Dimmer {
public:
	Dimmer(uint8_t freq = 50) {
		pinMode(_D_PIN, OUTPUT);
		fastWrite(_D_PIN, LOW);
		if (freq == 50) maxVal = 9300;
		else maxVal = 7600;
	}
	void write(uint8_t dim) {
		dimmer = map(dim, 0, 255, maxVal, 500);
	}
	bool tickZero() {
		fastWrite(_D_PIN, LOW);
		if (lastDim != dimmer) {
			lastDim = dimmer;
			return true;
		}
		return false;
	}
	void tickTimer() {
		fastWrite(_D_PIN, HIGH);
	}
	int getPeriod() {
		return dimmer;
	}
	
private:
	int dimmer = 0, lastDim = 0;
	int maxVal = 0;
};

// плавный диммер многоканальный
template < uint8_t _D_AMOUNT >
class DimmerMulti {
public:
	DimmerMulti(uint8_t freq = 50) {
		if (freq == 50) maxVal = 9300;
		else maxVal = 7600;
	}
	void attach(uint8_t num, uint8_t pin) {
		dimPins[num] = pin;
		pinMode(pin, OUTPUT);
	}
	void write(uint8_t pin, uint8_t dim) {
		dimmer[pin] = dim;
	}
	bool tickZero() {
		counter = 255;
	}
	void tickTimer() {
		for (byte i = 0; i < _D_AMOUNT; i++) {
			if (counter == dimmer[i]) fastWrite(dimPins[i], 1);  			// на текущем тике включаем
			else if (counter == dimmer[i] - 1) fastWrite(dimPins[i], 0);  	// на следующем выключаем
		}
		counter--;
	}
	int getPeriod() {
		return (maxVal == 9300) ? 37 : 31;	// us
	}
	
private:
	uint8_t dimmer[_D_AMOUNT], dimPins[_D_AMOUNT];
	int maxVal = 0;
	volatile int counter = 0;
};

#endif