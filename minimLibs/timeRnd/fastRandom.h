#ifndef FastRandom_h
#define FastRandom_h
#include <Arduino.h>

class FastRandom {
public:
	// установить сид
	void setSeed(uint16_t seed) {
		_seed = seed;
	}
	uint16_t get() {
		_seed = (_seed * 2053ul) + 13849;
		return _seed;
	}
	uint16_t get(uint16_t max) {
		return ((uint32_t)max * get()) >> 16;
	}
	uint16_t get(uint16_t min, uint16_t max) {
		return (get(max - min) + min);
	}
private:
	uint16_t _seed;
};

#endif
