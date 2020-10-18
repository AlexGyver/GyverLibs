#include "GyverTimer2.h"

// ************************** TIMER 2 ***************************
void (*isr2)();
volatile uint8_t isr2_counter;

void timer2_ISR(void (*isr)()) {
	isr2 = *isr;
}

void timer2_setPeriod(uint32_t time) {
	int divisor;
	byte divValue;

	TCCR2A = 0;
	TCCR2B = 0;
	TCNT2 = 0;

	if (time > 1 && time <= 16) {
		divisor = 1;
		divValue = 1;
	} else if (time > 16 && time <= 128) {
		divisor = 8;
		divValue = 2;
	} else if (time > 128 && time <= 512) {
		divisor = 32;
		divValue = 3;
	} else if (time > 512 && time <= 1024) {
		divisor = 64;
		divValue = 4;
	} else if (time > 1024 && time <= 2048) {
		divisor = 128;
		divValue = 5;
	} else if (time > 2048 && time <= 4096) {
		divisor = 256;
		divValue = 6;
	} else if (time > 4096 && time <= 16384) {
		divisor = 1024;
		divValue = 7;
	} else {
		TCCR2B = 1;
		return;
	}

	TCCR2B = (TCCR2B & B11111000) | divValue;			// применяем делитель
	isr2_counter = 256 - (long)16 * time / divisor;		// OVF 
}

void timer2_setFrequency(uint32_t hz) {
	timer2_setPeriod((long)1000000 / hz);
}

void timer2_start(void) {
	TCNT2 = 0;					// выставляем начальное значение TCNT2
	TIMSK2 |= (1 << TOIE2);		// разрешаем прерывание по переполнению таймера
}

void timer2_stop(void) {
	TIMSK2 &= ~(1 << TOIE2);  	// запретить прерывания по таймеру
}

void timer2_resume(void) {
	TIMSK2 |= (1 << TOIE2); 	// Продолжить отсчет
}

void timer2_restart(void) {
	TCNT2 = isr2_counter;		// сброс
	TIMSK2 |= (1 << TOIE2); 	// Продолжить отсчет
}

ISR(TIMER2_OVF_vect) {
	TCNT2 = isr2_counter;
	(*isr2)();
}
// 16000000 / freq / divisor - 1 < 2^bit
// 16000000 * period / divisor - 1 < 2^bit