#ifndef QuickIO_h
#define QuickIO_h
class QuickIO {
public:
	QuickIO (uint8_t pin);
	void write(uint8_t state);
	void mode(uint8_t mode);
	uint8_t read(void);
	void toggle(void);
	void clear(void);
	void set(void);

private:
	volatile uint8_t *_port_reg;
	volatile uint8_t *_pin_reg;
	volatile uint8_t *_ddr_reg;
	volatile uint8_t _bit_mask;
};

QuickIO::QuickIO(uint8_t pin) {
	_port_reg = portOutputRegister(digitalPinToPort(pin));
	_pin_reg = portInputRegister(digitalPinToPort(pin));
	_ddr_reg = portModeRegister(digitalPinToPort(pin));
	_bit_mask = digitalPinToBitMask(pin);
}

void QuickIO::write(uint8_t state) {
	if (state) *_port_reg |= _bit_mask;
	else *_port_reg &= ~ _bit_mask;  
}

void QuickIO::toggle(void) {
	*_port_reg ^= _bit_mask;
}

void QuickIO::set(void) {
	*_port_reg |= _bit_mask;
}

void QuickIO::clear(void) {
	*_port_reg &= ~ _bit_mask;
}

uint8_t QuickIO::read(void) {
	return !!(*_pin_reg & _bit_mask);
}

void QuickIO::mode(uint8_t mode) {
	switch (mode) {
	case INPUT:
		*_ddr_reg &= ~ _bit_mask;
		*_port_reg &= ~ _bit_mask;
		return;
	case OUTPUT:
		*_ddr_reg |= _bit_mask;
		return;
	case INPUT_PULLUP:
		*_ddr_reg &= ~ _bit_mask;
		*_port_reg |= _bit_mask;
		return;
	}
}
#endif