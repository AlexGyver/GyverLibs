#include "GyverTimers.h"


// ========================== OUTPUT STATE ==========================
void Timer_0::outputState(uint8_t channel, bool state) {
	switch (channel) {
	case CHANNEL_A: TCCR0B = (TCCR0B & 0x7F) | (state << FOC0A); break;
	case CHANNEL_B: TCCR0B = (TCCR0B & 0xBF) | (state << FOC0B); break;
	}
}	

void Timer_1::outputState(uint8_t channel, bool state) {
	switch (channel) {
	case CHANNEL_A: TCCR1C = (TCCR1C & 0x7F) | (state << FOC1A); break;
	case CHANNEL_B: TCCR1C = (TCCR1C & 0xBF) | (state << FOC1B); break;
#if defined(__AVR_ATmega2560__)
	case CHANNEL_C: TCCR1C = (TCCR1C & 0xDF) | (state << FOC1C); break;
#endif
	}
}

void Timer_2::outputState(uint8_t channel, bool state) {
	switch (channel) {
	case CHANNEL_A: TCCR2B = (TCCR2B & 0x7F) | (state << FOC2A); break;
	case CHANNEL_B: TCCR2B = (TCCR2B & 0xBF) | (state << FOC2B); break;
	}
}

#if defined(__AVR_ATmega2560__)
void Timer_3::outputState(uint8_t channel, bool state) {
	switch (channel) {
	case CHANNEL_A: TCCR3C = (TCCR3C & 0x7F) | (state << FOC3A); break;
	case CHANNEL_B: TCCR3C = (TCCR3C & 0xBF) | (state << FOC3B); break;
	case CHANNEL_C: TCCR3C = (TCCR3C & 0xDF) | (state << FOC3C); break;
	}
}

void Timer_4::outputState(uint8_t channel,bool state){
	switch (channel) {
	case CHANNEL_A: TCCR4C = (TCCR4C & 0x7F) | (state << FOC4A); break;
	case CHANNEL_B: TCCR4C = (TCCR4C & 0xBF) | (state << FOC4B); break;
	case CHANNEL_C: TCCR4C = (TCCR4C & 0xDF) | (state << FOC4C); break;
	}	
}

void Timer_5::outputState(uint8_t channel,bool state){
	switch (channel) {
	case CHANNEL_A: TCCR5C = (TCCR5C & 0x7F) | (state << FOC5A); break;
	case CHANNEL_B: TCCR5C = (TCCR5C & 0xBF) | (state << FOC5B); break;
	case CHANNEL_C: TCCR5C = (TCCR5C & 0xDF) | (state << FOC5C); break;
	}		
}
#endif


// ========================== PAUSE ==========================
void Timer_0::pause(void) {
	TCCR0B = (TCCR0B & 0xF8);	// Clear timer clock bits
}

void Timer_1::pause(void) {
	TCCR1B = (TCCR1B & 0xF8);
}

void Timer_2::pause(void) {
	TCCR2B = (TCCR2B & 0xF8);
}

#if defined(__AVR_ATmega2560__)

void Timer_3::pause(void) {
	TCCR3B = (TCCR3B & 0xF8);
}

void Timer_4::pause(void) {
	TCCR4B = (TCCR4B & 0xF8);
}

void Timer_5::pause(void) {
	TCCR5B = (TCCR5B & 0xF8);
}

#endif

// ========================== RESUME ==========================
void Timer_0::resume(void) {
	TCCR0B = ((TCCR0B & 0xF8) |  _timer0_clock);  // Return clock timer settings
}

void Timer_1::resume(void) {
	TCCR1B = ((TCCR1B & 0xF8) |  _timer1_clock);
}

void Timer_2::resume(void) {
	TCCR2B = ((TCCR2B & 0xF8) |  _timer2_clock);
}

#if defined(__AVR_ATmega2560__)
void Timer_3::resume(void) {
	TCCR3B = ((TCCR3B & 0xF8) |  _timer3_clock);
}

void Timer_4::resume(void) {
	TCCR4B = ((TCCR4B & 0xF8) |  _timer4_clock);
}

void Timer_5::resume(void) {
	TCCR5B = ((TCCR5B & 0xF8) |  _timer5_clock);
}
#endif


// ========================== STOP ==========================
void Timer_0::stop(void) {
	Timer_0::pause();
	TCNT0 = 0x00;             // Clear timer counter
}

void Timer_1::stop(void) {
	Timer_1::pause();
	TCNT1 = 0x00;
}

void Timer_2::stop(void) {
	Timer_2::pause();
	TCNT2 = 0x00;
}

#if defined(__AVR_ATmega2560__)
void Timer_3::stop(void) {
	Timer_3::pause();
	TCNT3 = 0x00;
}

void Timer_4::stop(void) {
	Timer_4::pause();
	TCNT4 = 0x00;
}

void Timer_5::stop(void) {
	Timer_5::pause();
	TCNT5 = 0x00;
}
#endif


// ========================== RESTART ==========================
void Timer_0::restart(void) {
	Timer_0::resume();
	TCNT0 = 0x00;
}

void Timer_1::restart(void) {
	Timer_1::resume();
	TCNT1 = 0x00;
}

void Timer_2::restart(void) {
	Timer_2::resume();
	TCNT2 = 0x00;
}

#if defined(__AVR_ATmega2560__)
void Timer_3::restart(void) {
	Timer_3::resume();
	TCNT3 = 0x00;
}

void Timer_4::restart(void) {
	Timer_4::resume();
	TCNT4 = 0x00;
}

void Timer_5::restart(void) {
	Timer_5::resume();
	TCNT5 = 0x00;
}
#endif


// ========================== DISABLE ISR ==========================
void Timer_0::disableISR(uint8_t source) {
	TIMSK0 &= ~ (source ? (1 << OCIE0B) : (1 << OCIE0A));
}

void Timer_1::disableISR(uint8_t source) {
	switch (source) {
	case CHANNEL_A: TIMSK1 &= ~ (1 << OCIE1A); break;
	case CHANNEL_B: TIMSK1 &= ~ (1 << OCIE1B); break;
#if defined(__AVR_ATmega2560__)
	case CHANNEL_C: TIMSK1 &= ~ (1 << OCIE1C); break;
#endif
	}
}

void Timer_2::disableISR(uint8_t source) {
	TIMSK2 &= ~ (source ? (1 << OCIE2B) : (1 << OCIE2A));
}

#if defined(__AVR_ATmega2560__)
void Timer_3::disableISR(uint8_t source) {
	switch (source) {
	case CHANNEL_A: TIMSK3 &= ~ (1 << OCIE3A); break;
	case CHANNEL_B: TIMSK3 &= ~ (1 << OCIE3B); break;
	case CHANNEL_C: TIMSK3 &= ~ (1 << OCIE3C); break;
	}
}

void Timer_4::disableISR(uint8_t source) {
	switch (source) {
	case CHANNEL_A: TIMSK4 &= ~ (1 << OCIE4A); break;
	case CHANNEL_B: TIMSK4 &= ~ (1 << OCIE4B); break;
	case CHANNEL_C: TIMSK4 &= ~ (1 << OCIE4C); break;
	}
}

void Timer_5::disableISR(uint8_t source) { 
	switch (source) {
	case CHANNEL_A: TIMSK5 &= ~ (1 << OCIE5A); break;
	case CHANNEL_B: TIMSK5 &= ~ (1 << OCIE5B); break;
	case CHANNEL_C: TIMSK5 &= ~ (1 << OCIE5C); break;
	}
}
#endif

// ========================== DEFAULT ==========================
void Timer_0::setDefault(void) {
	TCCR0A = 0x03;  // Fast PWM , 8 bit
	TCCR0B = 0x03;  // Prescaler /64
	OCR0B = 0x00;   // Clear COMPA
	OCR0A = 0x00;   // Clear COMPB
	TCNT0 = 0x00;   // Clear counter
}

void Timer_1::setDefault(void) {
	TCCR1A = 0x01;  // Phasecorrect PWM , 8 bit
	TCCR1B = 0x0B;  // Prescaler /64
	OCR1B = 0x00;   // Clear COMPA
	OCR1A = 0x00;   // Clear COMPB
	TCNT1 = 0x00;   // Clear counter
}

void Timer_2::setDefault(void) {
	TCCR2A = 0x01;  // Phasecorrect PWM , 8 bit
	TCCR2B = 0x04;  // Prescaler /64
	OCR2B = 0x00;   // Clear COMPA
	OCR2A = 0x00;   // Clear COMPB
	TCNT2 = 0x00;   // Clear counter
}


#if defined(__AVR_ATmega2560__)
void Timer_3::setDefault(void) {
	TCCR3A = 0x01;  // Phasecorrect PWM , 8 bit
	TCCR3B = 0x0B;  // Prescaler /64
	OCR3B = 0x00;   // Clear COMPA
	OCR3A = 0x00;   // Clear COMPB
	TCNT3 = 0x00;   // Clear counter
}

void Timer_4::setDefault(void) {
	TCCR4A = 0x01;  // Phasecorrect PWM , 8 bit
	TCCR4B = 0x0B;  // Prescaler /64
	OCR4B = 0x00;   // Clear COMPA
	OCR4A = 0x00;   // Clear COMPB
	TCNT4 = 0x00;   // Clear counter
}

void Timer_5::setDefault(void) {
	TCCR5A = 0x01;  // Phasecorrect PWM , 8 bit
	TCCR5B = 0x0B;  // Prescaler /64
	OCR5B = 0x00;   // Clear COMPA
	OCR5A = 0x00;   // Clear COMPB
	TCNT5 = 0x00;   // Clear counter
}
#endif

// ========================== PHASE SHIFT ==========================
void Timer_0::phaseShift(uint8_t source, uint16_t phase) {
	if (source)	OCR0B = map(phase, 0, 360,  0, OCR0A);
}

void Timer_1::phaseShift(uint8_t source, uint16_t phase) {
	switch (source) {
	case CHANNEL_A: OCR1A = map(phase, 0, 360, 0, ICR1); break;
	case CHANNEL_B: OCR1B = map(phase, 0, 360, 0, ICR1); break;
#if defined(__AVR_ATmega2560__)
	case CHANNEL_C: OCR1C = map(phase, 0, 360, 0, ICR1); break;
#endif
	}
}

void Timer_2::phaseShift(uint8_t source, uint16_t phase) {
	if (source)	OCR2B = map(phase, 0, 360, 0, OCR2A);
}

#if defined(__AVR_ATmega2560__)

void Timer_3::phaseShift(uint8_t source, uint16_t phase) {
	switch (source) {
	case CHANNEL_A: OCR3A = map(phase, 0, 360, 0, ICR3); break;
	case CHANNEL_B: OCR3B = map(phase, 0, 360, 0, ICR3); break;
	case CHANNEL_C: OCR3C = map(phase, 0, 360, 0, ICR3); break;
	}
}

void Timer_4::phaseShift(uint8_t source, uint16_t phase) {
	switch (source) {
	case CHANNEL_A: OCR4A = map(phase, 0, 360, 0, ICR4); break;
	case CHANNEL_B: OCR4B = map(phase, 0, 360, 0, ICR4); break;
	case CHANNEL_C: OCR4C = map(phase, 0, 360, 0, ICR4); break;
	}
}

void Timer_5::phaseShift(uint8_t source, uint16_t phase) {
	switch (source) {
	case CHANNEL_A: OCR5A = map(phase, 0, 360, 0, ICR5); break;
	case CHANNEL_B: OCR5B = map(phase, 0, 360, 0, ICR5); break;
	case CHANNEL_C: OCR5C = map(phase, 0, 360, 0, ICR5); break;
	}
}
#endif


// ========================== ENABLE ISR ==========================
void Timer_0::enableISR(uint8_t source) {
	if (!source) TIMSK0 |= (1 << OCIE0A);
	else TIMSK0 |= (1 << OCIE0B);
}

void Timer_1::enableISR(uint8_t source) {
	switch (source) {
	case CHANNEL_A: TIMSK1 |= (1 << OCIE1A); break;
	case CHANNEL_B: TIMSK1 |= (1 << OCIE1B); break;
#if defined(__AVR_ATmega2560__)
	case CHANNEL_C: TIMSK1 |= (1 << OCIE1C); break;
#endif
	}
}

void Timer_2::enableISR(uint8_t source) {
	if (!source) TIMSK2 |= (1 << OCIE2A);
	else TIMSK2 |= (1 << OCIE2B);
}

#if defined(__AVR_ATmega2560__)

void Timer_3::enableISR(uint8_t source) {
	switch (source) {
	case CHANNEL_A: TIMSK3 |= (1 << OCIE3A); break;
	case CHANNEL_B: TIMSK3 |= (1 << OCIE3B); break;
	case CHANNEL_C: TIMSK3 |= (1 << OCIE3C); break;
	}
}

void Timer_4::enableISR(uint8_t source) {
	switch (source) {
	case CHANNEL_A: TIMSK4 |= (1 << OCIE4A); break;
	case CHANNEL_B: TIMSK4 |= (1 << OCIE4B); break;
	case CHANNEL_C: TIMSK4 |= (1 << OCIE4C); break;
	}
}

void Timer_5::enableISR(uint8_t source) {
	switch (source) {
	case CHANNEL_A: TIMSK5 |= (1 << OCIE5A); break;
	case CHANNEL_B: TIMSK5 |= (1 << OCIE5B); break;
	case CHANNEL_C: TIMSK5 |= (1 << OCIE5C); break;
	}
}
#endif

// ========================== SET FREQUENCY ==========================
uint32_t Timer_0::setFrequency(uint32_t _timer0_frequency) {
	return 1000000UL / (Timer_0::setPeriod(1000000UL / _timer0_frequency));
}

uint32_t Timer_1::setFrequency(uint32_t _timer1_frequency) {
	return 1000000UL / (Timer_1::setPeriod(1000000UL / _timer1_frequency));
}

uint32_t Timer_2::setFrequency(uint32_t _timer2_frequency) {
	return 1000000UL / (Timer_2::setPeriod(1000000UL / _timer2_frequency));
}

#if defined(__AVR_ATmega2560__)

uint32_t Timer_3::setFrequency(uint32_t _timer3_frequency) {
	return 1000000UL / (Timer_3::setPeriod(1000000UL / _timer3_frequency));
}

uint32_t Timer_4::setFrequency(uint32_t _timer4_frequency) {
	return 1000000UL / (Timer_4::setPeriod(1000000UL / _timer4_frequency));
}

uint32_t Timer_5::setFrequency(uint32_t _timer5_frequency) {
	return 1000000UL / (Timer_5::setPeriod(1000000UL / _timer5_frequency));
}

#endif


// ========================== SET FREQUENCY FLOAT ==========================
float Timer_0::setFrequencyFloat(float _timer0_frequency) {
	return 1000000.0F / (Timer_0::setPeriod(1000000.0F / _timer0_frequency));
}

float Timer_1::setFrequencyFloat(float _timer1_frequency) {
	return 1000000.0F / (Timer_1::setPeriod(1000000.0F / _timer1_frequency));
}

float Timer_2::setFrequencyFloat(float _timer2_frequency) {
	return 1000000.0F / (Timer_2::setPeriod(1000000.0F / _timer2_frequency));
}

#if defined(__AVR_ATmega2560__)

float Timer_3::setFrequencyFloat(float _timer3_frequency) {
	return 1000000.0F / (Timer_3::setPeriod(1000000.0F / _timer3_frequency));
}

float Timer_4::setFrequencyFloat(float _timer4_frequency) {
	return 1000000.0F / (Timer_4::setPeriod(1000000.0F / _timer4_frequency));
}

float Timer_5::setFrequencyFloat(float _timer5_frequency) {
	return 1000000.0F / (Timer_5::setPeriod(1000000.0F / _timer5_frequency));
}

#endif

// ========================== OUTPUT ENABLE ==========================
void Timer_0::outputEnable(uint8_t channel, uint8_t mode) {
	switch (channel) {
	case CHANNEL_A: TCCR0A = (TCCR0A & 0x3F) | (mode << 6); break;
	case CHANNEL_B: TCCR0A = (TCCR0A & 0xCF) | (mode << 4); break;
	}
}

void Timer_1::outputEnable(uint8_t channel, uint8_t mode) {
	switch (channel) {
	case CHANNEL_A: TCCR1A = (TCCR1A & 0x3F) | (mode << 6); break;
	case CHANNEL_B: TCCR1A = (TCCR1A & 0xCF) | (mode << 4); break;
#if defined(__AVR_ATmega2560__)
	case CHANNEL_C: TCCR1A = (TCCR1A & 0xF3) | (mode << 2); break;
#endif
	}
}

void Timer_2::outputEnable(uint8_t channel, uint8_t mode) {
	switch (channel) {
	case CHANNEL_A: TCCR2A = (TCCR2A & 0x3F) | (mode << 6); break;
	case CHANNEL_B: TCCR2A = (TCCR2A & 0xCF) | (mode << 4); break;
	}
}

#if defined(__AVR_ATmega2560__)
void Timer_3::outputEnable(uint8_t channel, uint8_t mode) {
	switch (channel) {
	case CHANNEL_A: TCCR3A = (TCCR3A & 0x3F) | (mode << 6); break;
	case CHANNEL_B: TCCR3A = (TCCR3A & 0xCF) | (mode << 4); break;
	case CHANNEL_C: TCCR3A = (TCCR3A & 0xF3) | (mode << 2); break;
	}
}

void Timer_4::outputEnable(uint8_t channel, uint8_t mode) {
	switch (channel) {
	case CHANNEL_A: TCCR4A = (TCCR4A & 0x3F) | (mode << 6); break;
	case CHANNEL_B: TCCR4A = (TCCR4A & 0xCF) | (mode << 4); break;
	case CHANNEL_C: TCCR4A = (TCCR4A & 0xF3) | (mode << 2); break;
	}
}

void Timer_5::outputEnable(uint8_t channel, uint8_t mode) {
	switch (channel) {
	case CHANNEL_A: TCCR5A = (TCCR5A & 0x3F) | (mode << 6); break;
	case CHANNEL_B: TCCR5A = (TCCR5A & 0xCF) | (mode << 4); break;
	case CHANNEL_C: TCCR5A = (TCCR5A & 0xF3) | (mode << 2); break;
	}
}
#endif


// ========================== OUTPUT DISABLE ==========================
void Timer_0::outputDisable(uint8_t channel) {
	switch (channel) {
	case CHANNEL_A: TCCR0A = (TCCR0A & 0x3F); break;
	case CHANNEL_B: TCCR0A = (TCCR0A & 0xCF); break;
	}
}

void Timer_1::outputDisable(uint8_t channel) {
	switch (channel) {
	case CHANNEL_A: TCCR1A = (TCCR1A & 0x3F); break;
	case CHANNEL_B: TCCR1A = (TCCR1A & 0xCF); break;
#if defined(__AVR_ATmega2560__)
	case CHANNEL_C: TCCR1A = (TCCR1A & 0xF3); break;
#endif
	}
}

void Timer_2::outputDisable(uint8_t channel) {
	switch (channel) {
	case CHANNEL_A: TCCR2A = (TCCR2A & 0x3F); break;
	case CHANNEL_B: TCCR2A = (TCCR2A & 0xCF); break;
	}
}

#if defined(__AVR_ATmega2560__)
void Timer_3::outputDisable(uint8_t channel) {
	switch (channel) {
	case CHANNEL_A: TCCR3A = (TCCR3A & 0x3F); break;
	case CHANNEL_B: TCCR3A = (TCCR3A & 0xCF); break;
	case CHANNEL_C: TCCR3A = (TCCR3A & 0xF3); break;
	}
}

void Timer_4::outputDisable(uint8_t channel) {
	switch (channel) {
	case CHANNEL_A: TCCR4A = (TCCR4A & 0x3F); break;
	case CHANNEL_B: TCCR4A = (TCCR4A & 0xCF); break;
	case CHANNEL_C: TCCR4A = (TCCR4A & 0xF3); break;
	}
}

void Timer_5::outputDisable(uint8_t channel) {
	switch (channel) {
	case CHANNEL_A: TCCR5A = (TCCR5A & 0x3F); break;
	case CHANNEL_B: TCCR5A = (TCCR5A & 0xCF); break;
	case CHANNEL_C: TCCR5A = (TCCR5A & 0xF3); break;
	}
}
#endif



// ========================== SET PERIOD ==========================
uint32_t Timer_0::setPeriod(uint32_t _timer0_period)  {	
	_timer0_period = constrain(_timer0_period, 1, 32258);

	uint32_t _timer0_cycles = F_CPU / 1000000 * _timer0_period;  // Calculation of the number of timer cycles per period
	uint8_t _timer0_prescaler = 0x00;
	uint16_t _timer0_divider = 0x00;

	if (_timer0_cycles < 256UL) {   // Сhoose optimal divider for the timer
		_timer0_prescaler = 0x01;
		_timer0_divider = 1UL;
	} else if (_timer0_cycles < 256UL * 8) {
		_timer0_prescaler = 0x02;
		_timer0_divider = 8UL;
	} else if (_timer0_cycles < 256UL * 64) {
		_timer0_prescaler = 0x03;
		_timer0_divider = 64UL;
	} else if (_timer0_cycles < 256UL * 256) {
		_timer0_prescaler = 0x04;
		_timer0_divider = 256UL;
	} else {
		_timer0_prescaler = 0x05;
		_timer0_divider = 1024UL;
	}

	uint8_t _timer0_top = (_timer0_cycles < 256UL * 1024 ? (_timer0_cycles / _timer0_divider) : 256UL) ;

	TCCR0A = (TCCR0A & 0xF0)|(1 << WGM21);	// CTC - mode
	TCCR0B = _timer0_prescaler;   			// Set timer prescaler
	OCR0A = _timer0_top - 1;      			// Set timer top
	_timer0_clock = (TCCR0B & 0x07);    	// Save timer clock settings
	
	return (2000000UL / ((F_CPU / _timer0_divider) / _timer0_top));   // Return real timer period
}

uint32_t Timer_1::setPeriod(uint32_t _timer1_period)  {	
	_timer1_period = constrain(_timer1_period, 1, 4000000);
	
	uint32_t _timer1_cycles = F_CPU / 1000000 * _timer1_period;  // Calculation of the number of timer cycles per period
	uint8_t _timer1_prescaler = 0x00;
	uint16_t _timer1_divider = 0x00;

	if (_timer1_cycles < 65536UL) {   // Сhoose optimal divider for the timer
		_timer1_prescaler = 0x01;
		_timer1_divider = 1UL;
	} else if (_timer1_cycles < 65536UL * 8) {
		_timer1_prescaler = 0x02;
		_timer1_divider = 8UL;
	} else if (_timer1_cycles < 65536UL * 64) {
		_timer1_prescaler = 0x03;
		_timer1_divider = 64UL;
	} else if (_timer1_cycles < 65536UL * 256) {
		_timer1_prescaler = 0x04;
		_timer1_divider = 256UL;
	} else {
		_timer1_prescaler = 0x05;
		_timer1_divider = 1024UL;
	}

	uint16_t _timer1_top = (_timer1_cycles < 65536UL * 1024 ? (_timer1_cycles / _timer1_divider) : 65536UL) ;
#if defined(__AVR_ATmega2560__)
	TCCR1A = (TCCR1A & 0xFC);
#else
	TCCR1A = (TCCR1A & 0xF0);
#endif
	TCCR1B = ((1 << WGM13) | (1 << WGM12) | _timer1_prescaler);   // CTC mode + set prescaler
	ICR1 = _timer1_top - 1;             // Set timer top
	_timer1_clock = (TCCR1B & 0x07);    // Save timer clock settings
	return (2000000UL / ((F_CPU / _timer1_divider) / _timer1_top));   // Return real timer period
}

uint32_t Timer_2::setPeriod(uint32_t _timer2_period)  {	
	_timer2_period = constrain(_timer2_period, 1, 32258);
	
	uint32_t _timer2_cycles = F_CPU / 1000000 * _timer2_period;  // Calculation of the number of timer cycles per period
	uint8_t _timer2_prescaler = 0x00;
	uint16_t _timer2_divider = 0x00;

	if (_timer2_cycles < 256UL) {   // Сhoose optimal divider for the timer
		_timer2_prescaler = 0x01;
		_timer2_divider = 1UL;
	} else if (_timer2_cycles < 256UL * 8) {
		_timer2_prescaler = 0x02;
		_timer2_divider = 8UL;
	} else if (_timer2_cycles < 256UL * 32) {
		_timer2_prescaler = 0x03;
		_timer2_divider = 32UL;
	} else if (_timer2_cycles < 256UL * 64) {
		_timer2_prescaler = 0x04;
		_timer2_divider = 64UL;
	} else if (_timer2_cycles < 256UL * 128) {
		_timer2_prescaler = 0x05;
		_timer2_divider = 128UL;
	} else if (_timer2_cycles < 256UL * 256) {
		_timer2_prescaler = 0x06;
		_timer2_divider = 256UL;
	} else {
		_timer2_prescaler = 0x07;
		_timer2_divider = 1024UL;
	}

	uint8_t _timer2_top = (_timer2_cycles < 256UL * 1024 ? (_timer2_cycles / _timer2_divider) : 256UL);

	TCCR2A = (TCCR2A & 0xF0)|(1 << WGM21);      // CTC - mode
	TCCR2B = _timer2_prescaler;   		// Set timer prescaler
	OCR2A = _timer2_top - 1;      		// Set timer top
	_timer2_clock = (TCCR2B & 0x07);    // Save timer clock settings
	
	return (2000000UL / ((F_CPU / _timer2_divider) / _timer2_top));   // Return real timer period
}

#if defined(__AVR_ATmega2560__)

uint32_t Timer_3::setPeriod(uint32_t _timer3_period)  {	
	_timer3_period = constrain(_timer3_period, 1, 4000000);
	
	uint32_t _timer3_cycles = F_CPU / 1000000 * _timer3_period;  // Calculation of the number of timer cycles per period
	uint8_t _timer3_prescaler = 0x00;
	uint16_t _timer3_divider = 0x00;

	if (_timer3_cycles < 65536UL) {   // Сhoose optimal divider for the timer
		_timer3_prescaler = 0x01;
		_timer3_divider = 1UL;
	} else if (_timer3_cycles < 65536UL * 8) {
		_timer3_prescaler = 0x02;
		_timer3_divider = 8UL;
	} else if (_timer3_cycles < 65536UL * 64) {
		_timer3_prescaler = 0x03;
		_timer3_divider = 64UL;
	} else if (_timer3_cycles < 65536UL * 256) {
		_timer3_prescaler = 0x04;
		_timer3_divider = 256UL;
	} else {
		_timer3_prescaler = 0x05;
		_timer3_divider = 1024UL;
	}

	uint16_t _timer3_top = (_timer3_cycles < 65536UL * 1024 ? (_timer3_cycles / _timer3_divider) : 65536UL) ;

	TCCR3A = (TCCR3A & 0xFC);
	TCCR3B = ((1 << WGM33) | (1 << WGM32) | _timer3_prescaler);   // CTC mode + set prescaler
	ICR3 = _timer3_top - 1;             // Set timer top
	_timer3_clock = (TCCR3B & 0x07);    // Save timer clock settings
	
	return (2000000UL / ((F_CPU / _timer3_divider) / _timer3_top));   // Return real timer period
}

uint32_t Timer_4::setPeriod(uint32_t _timer4_period)  {	
	_timer4_period = constrain(_timer4_period, 1, 4000000);
	
	uint32_t _timer4_cycles = F_CPU / 1000000 * _timer4_period;  // Calculation of the number of timer cycles per period
	uint8_t _timer4_prescaler = 0x00;
	uint16_t _timer4_divider = 0x00;

	if (_timer4_cycles < 65536UL) {   // Сhoose optimal divider for the timer
		_timer4_prescaler = 0x01;
		_timer4_divider = 1UL;
	} else if (_timer4_cycles < 65536UL * 8) {
		_timer4_prescaler = 0x02;
		_timer4_divider = 8UL;
	} else if (_timer4_cycles < 65536UL * 64) {
		_timer4_prescaler = 0x03;
		_timer4_divider = 64UL;
	} else if (_timer4_cycles < 65536UL * 256) {
		_timer4_prescaler = 0x04;
		_timer4_divider = 256UL;
	} else {
		_timer4_prescaler = 0x05;
		_timer4_divider = 1024UL;
	}

	uint16_t _timer4_top = (_timer4_cycles < 65536UL * 1024 ? (_timer4_cycles / _timer4_divider) : 65536UL) ;

	TCCR4A = (TCCR4A & 0xFC);
	TCCR4B = ((1 << WGM43) | (1 << WGM42) | _timer4_prescaler);   // CTC mode + set prescaler
	ICR4 = _timer4_top - 1;             // Set timer top
	_timer4_clock = (TCCR4B & 0x07);    // Save timer clock settings
	
	return (2000000UL / ((F_CPU / _timer4_divider) / _timer4_top));   // Return real timer period
}

uint32_t Timer_5::setPeriod(uint32_t _timer5_period)  {	
	_timer5_period = constrain(_timer5_period, 1, 4000000);
	
	uint32_t _timer5_cycles = F_CPU / 1000000 * _timer5_period;  // Calculation of the number of timer cycles per period
	uint8_t _timer5_prescaler = 0x00;
	uint16_t _timer5_divider = 0x00;

	if (_timer5_cycles < 65536UL) {   // Сhoose optimal divider for the timer
		_timer5_prescaler = 0x01;
		_timer5_divider = 1UL;
	} else if (_timer5_cycles < 65536UL * 8) {
		_timer5_prescaler = 0x02;
		_timer5_divider = 8UL;
	} else if (_timer5_cycles < 65536UL * 64) {
		_timer5_prescaler = 0x03;
		_timer5_divider = 64UL;
	} else if (_timer5_cycles < 65536UL * 256) {
		_timer5_prescaler = 0x04;
		_timer5_divider = 256UL;
	} else {
		_timer5_prescaler = 0x05;
		_timer5_divider = 1024UL;
	}

	uint16_t _timer5_top = (_timer5_cycles < 65536UL * 1024 ? (_timer5_cycles / _timer5_divider) : 65536UL) ;

	TCCR5A = (TCCR5A & 0xFC);
	TCCR5B = ((1 << WGM53) | (1 << WGM52) | _timer5_prescaler);   // CTC mode + set prescaler
	ICR5 = _timer5_top - 1;             // Set timer top
	_timer5_clock = (TCCR5B & 0x07);    // Save timer clock settings
	
	return (2000000UL / ((F_CPU / _timer5_divider) / _timer5_top));   // Return real timer period
}

#endif


Timer_0 Timer0 = Timer_0();
Timer_1 Timer1 = Timer_1();
Timer_2 Timer2 = Timer_2();

#if defined(__AVR_ATmega2560__)
Timer_3 Timer3 = Timer_3();
Timer_4 Timer4 = Timer_4();
Timer_5 Timer5 = Timer_5();
#endif