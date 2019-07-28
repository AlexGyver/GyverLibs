#include "GyverTimer1.h"

// ************************** TIMER 1 ***************************
void (*isr1)();
volatile uint16_t isr1_counter;

void timer1_ISR(void (*isr)()) {
	isr1 = *isr;
}

void timer1_setPeriod(uint32_t time) {
	TCCR1A = 0;
	TCCR1B = 0;

	if (time > 5 && time < 4096) {
		time = 65584 - (time << 4);
		TCCR1B = (1 << CS10);
	} else if (time > 4095 && time < 32768) {
		time = 65542 - (time << 1);
		TCCR1B = (1 << CS11);
	} else if (time > 32767 && time < 262144) {
		time = 65536 - (time >> 2);
		TCCR1B = ((1 << CS11) | (1 << CS10));
	} else if (time > 262143 && time < 1048576) {
		time = 65536 - (time >> 4);
		TCCR1B = (1 << CS12);
	} else if (time > 1048575 && time < 4194304) {
		time = 65536 - (time >> 6);
		TCCR1B = ((1 << CS12) | (1 << CS10));
	} else TCCR1B = 1;

	isr1_counter = time;
	TCNT1 = 0;  				// выставляем начальное значение TCNT1
}

void timer1_setFrequency(uint32_t hz) {
	timer1_setPeriod((long)1000000 / hz);
}

void timer1_start(void) {
	TCNT1 = 0;					// выставляем начальное значение TCNT1
	TIMSK1 |= (1 << TOIE1);		// разрешаем прерывание по переполнению таймера
}

void timer1_stop(void) {
	TIMSK1 &= ~(1 << TOIE1);	// запретить прерывания по таймеру1
}

void timer1_resume(void) {
	TIMSK1 |= (1 << TOIE1); 	// Продолжить отсчет, разрешить прерывания по таймеру1
}

void timer1_restart(void) {
	TCNT1 = isr1_counter;		// сброс
	TIMSK1 |= (1 << TOIE1); 	// разрешить прерывания по таймеру1
}

ISR(TIMER1_OVF_vect) {
	TCNT1 = isr1_counter;
	(*isr1)();
}
// 16000000 / freq / divisor - 1 < 2^bit
// 16000000 * period / divisor - 1 < 2^bit