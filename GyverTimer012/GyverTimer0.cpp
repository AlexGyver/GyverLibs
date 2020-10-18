#include "GyverTimer0.h"

// ************************** TIMER 0 ***************************
void (*isr0)();
volatile uint8_t isr0_counter;

void timer0_ISR(void (*isr)()) {
	isr0 = *isr;
}

void timer0_setPeriod(uint32_t time) {
	int divisor;
	long period;
	byte divValue;    

	TCCR0A = 0;
	TCCR0B = 0;
	//TCNT0  = 0;     // сбросит миллис

	if (time > 1 && time <= 16) {
		divisor = 1;
		divValue = 1;
	} else if (time > 16 && time <= 128) {
		divisor = 8;
		divValue = 2;
	} else if (time > 128 && time <= 1024) {
		divisor = 32;
		divValue = 3;
	} else if (time > 1024 && time <= 4096) {
		divisor = 64;
		divValue = 4;
	} else if (time > 4096 && time <= 16384) {
		divisor = 128;
		divValue = 5;
	} else {
		TCCR0B = 1;
		return;
	}

	TCCR0B = (TCCR0B & B11111000) | divValue;	// применяем делитель	
	period = (long)16 * time / divisor - 1;		// COMPA

	// для режима COMPARE
	OCR0A = period;				// set compare match register COMPA 
	TCCR0A |= (1 << WGM21);		// turn on CTC mode COMPA  
}

void timer0_setFrequency(uint32_t hz) {
	timer0_setPeriod((long)1000000 / hz);
}

void timer0_start(void) {
	//TCNT0 = 0;					// выставляем начальное значение TCNT0 (сбросит миллис)
	TIMSK0 |= (1 << OCIE0A);		// enable timer compare interrupt COMPA
}

void timer0_stop(void) {
	TIMSK0 &= ~(1 << OCIE0A);		// disable timer compare interrupt COMPA
}

void timer0_resume(void) {
	TIMSK0 |= (1 << OCIE0A);		// enable timer compare interrupt COMPA
}

void timer0_restart(void) {
	TIMSK0 |= (1 << OCIE0A);		// enable timer compare interrupt COMPA
}

ISR(TIMER0_COMPA_vect) {
	(*isr0)();
}
// 16000000 / freq / divisor - 1 < 2^bit
// 16000000 * period / divisor - 1 < 2^bit