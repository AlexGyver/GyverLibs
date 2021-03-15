#ifndef microOneWire_h
#define microOneWire_h
// v1.2 от 26.04.2020. Повышена стабильность

/* ~10722CK ~670.1250 us (16MHZ) */
/* max time without interrupts ~90us*/
bool oneWire_reset(byte pin) { 
	pinMode(pin, 1);
	delayMicroseconds(520);
	uint8_t oldSreg = SREG;
	cli();
	pinMode(pin, 0);
	delayMicroseconds(2);
	for (uint8_t c = 80; c; c--) {
		if (!digitalRead(pin)) {
			SREG = oldSreg;
			for (uint8_t i = 200;  !digitalRead(pin) and i; i--) {
				delayMicroseconds(1);
			}
			return false;
		}
		delayMicroseconds(1);
	}
	SREG = oldSreg;
	return true;
}


/* ~9150CK ~571.8750 us (16MHZ) */
/* max time without interrupts ~70us*/
void oneWire_write(uint8_t data, byte pin) {
	for (uint8_t p = 8; p; p--) {
		uint8_t oldSreg = SREG;
		cli();
		pinMode(pin, 1);
		if (data & 1) {
			delayMicroseconds(5);
			pinMode(pin, 0);
			delayMicroseconds(60);
		} else {
			delayMicroseconds(60);
			pinMode(pin, 0);
			delayMicroseconds(5);
		}
		SREG = oldSreg;
		data >>= 1;
	}
}

/* ~9500CK ~593.75 us (16MHZ) */
/* max time without interrupts ~70us*/
uint8_t oneWire_read(byte pin) {
	uint8_t data = 0;
	for (uint8_t p = 8; p; p--) {
		data >>= 1;
		uint8_t oldSreg = SREG;
		cli();
		pinMode(pin, 1);
		delayMicroseconds(2);
		pinMode(pin, 0);
		delayMicroseconds(8);
		if (digitalRead(pin)) data |= 1 << 7;
		delayMicroseconds(60);
		SREG = oldSreg;
	}
	return data;
}
#endif