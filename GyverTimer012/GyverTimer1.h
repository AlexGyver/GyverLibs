#ifndef GyverTimer1_h
#define GyverTimer1_h
#include <Arduino.h>

/*
	GyverTimer012 версия 1.0 от 03.03.2019
	Лёгкая библиотека для управления всеми тремя таймерами ATmega328
	Позволяет настраивать время прерывания по таймеру, запускать и останавлвать таймер
	
	=== Timer1 ===
	- При использовании не работает ШИМ на выводах 5 и 6
	- При использовании не работает библиотека Servo
	- Работает в режиме OVERFLOW и позволяет продолжить (resume) с момента остановки
	- Максимальный период 4194304 мкс (соотв. частота 0,24 Гц)
*/

// макрос для перевода Герц в микросекунды
#define hz_to_us(hz) ((long)1000000 / (hz))

extern void (*isr1)();

void timer1_ISR(void (*isr)());			// подключить прерывание
void timer1_setPeriod(uint32_t time);	// установить период (мкс)
void timer1_setFrequency(uint32_t hz);	// установить частоту (Гц)
void timer1_start(void);				// запустить
void timer1_stop(void);					// остановить
void timer1_resume(void);				// продолжить
void timer1_restart(void);				// перезапустить

#endif