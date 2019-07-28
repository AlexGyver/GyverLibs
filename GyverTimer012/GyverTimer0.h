#ifndef GyverTimer0_h
#define GyverTimer0_h
#include <Arduino.h>

/*
	GyverTimer012 версия 1.0 от 03.03.2019
	Лёгкая библиотека для управления всеми тремя таймерами ATmega328
	Позволяет настраивать время прерывания по таймеру, запускать и останавлвать таймер
	
	=== Timer0 ===
	- При использовании не работает ШИМ на выводах 5 и 6
	- Влияет на millis() И delay(), соответственно на ВСЕ библиотеки, где используются эти команды	
	- Работает в режиме COMPARE (OVERFLOW ардуино забрала себе)	
	- Максимальный период - 16384 мкс (соотв. частота 61 Гц)
*/

extern void (*isr0)();

void timer0_ISR(void (*isr)());			// подключить прерывание
void timer0_setPeriod(uint32_t time);	// установить период (мкс)
void timer0_setFrequency(uint32_t hz);	// установить частоту (Гц)
void timer0_start(void);				// запустить
void timer0_stop(void);					// остановить
void timer0_resume(void);				// продолжить
void timer0_restart(void);				// перезапустить (в данном случае то же, что продолжить)

#endif