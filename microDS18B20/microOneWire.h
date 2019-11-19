#ifndef microOneWire_h
#define microOneWire_h

boolean oneWire_reset(byte pin);
void oneWire_write(uint8_t data, byte pin);
uint8_t oneWire_read(byte pin);

boolean oneWire_reset(byte pin) {
	pinMode(pin, 1);
	delayMicroseconds(640);
	pinMode(pin, 0);
	delayMicroseconds(2);
	for (int8_t c = 80; c; c--) {
		if (!digitalRead(pin)) {
			uint32_t tmr = micros();
			while (!digitalRead(pin)) { if (micros() - tmr > 200) return false;}
			return false;
		}
		delayMicroseconds(1);
	}
	return true;
}

void oneWire_write(uint8_t data, byte pin) {
	for (uint8_t p = 8; p; p--) {
		pinMode(pin, 1);
		if (data & 1) {
			delayMicroseconds(5);
			pinMode(pin, 0);
			delayMicroseconds(90);
		} else {
			delayMicroseconds(90);
			pinMode(pin, 0);
			delayMicroseconds(5);
		}
		data >>= 1;
	}
}

uint8_t oneWire_read(byte pin) {
	uint8_t data = 0;
	for (uint8_t p = 8; p; p--) {
		data >>= 1;
		pinMode(pin, 1);
		delayMicroseconds(2);
		pinMode(pin, 0);
		delayMicroseconds(8);
		bool dataBit = digitalRead(pin);
		delayMicroseconds(80);
		if (dataBit) data |= 0x80;
	}
	return data;
}

#endif