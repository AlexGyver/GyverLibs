#ifndef GyverTimer2_h
#define GyverTimer2_h
#include <Arduino.h>

/*
	GyverTimer012 версия 1.0 от 03.03.2019
	Лёгкая библиотека для управления всеми тремя таймерами ATmega328
	Позволяет настраивать время прерывания по таймеру, запускать и останавлвать таймер
	
	=== Timer2 ===
	- При использовании не работает ШИМ на выводах 3 и 11
	- Влияет на tone() и pulse-функции, соответственно на ВСЕ библиотеки, где используются эти команды	
	- Работает в режиме OVERFLOW и позволяет продолжить (resume) с момента остановки
	- Максимальный период - 16384 мкс (соотв. частота 61 Гц)
*/

// макрос для перевода Герц в микросекунды
#define hz_to_us(hz) ((long)1000000 / (hz))

extern void (*isr2)();

void timer2_ISR(void (*isr)());			// подключить прерывание
void timer2_setPeriod(uint32_t time);	// установить период (мкс)
void timer2_setFrequency(uint32_t hz);	// установить частоту (Гц)
void timer2_start(void);				// запустить
void timer2_stop(void);					// остановить
void timer2_resume(void);				// продолжить
void timer2_restart(void);				// перезапустить

#endif