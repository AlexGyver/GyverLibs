#include <GyverWDT.h>

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny167__)
#define WDTCSR WDTCR
#endif

/* указатель на функцию прерывания */
void (*isr_wdt)();

/* непосредственно прерывание ватчдога */
ISR(WDT_vect) {
	(*isr_wdt)();
}

/* сброс счетчика watchdog в 0 */
void watchdog_reset(void) {
	asm volatile("wdr"); // ассемблерная вставка "watchdog reset"
}

/* Полное отключение watchdog'a */
void watchdog_disable(void) {
	WDTCSR |= (1 << WDCE) | (1 << WDE); // разрешение на вмешательство
	WDTCSR = 0; // обнуление регистра wdt
}

/* Вариант функции для работы ватчдога с прерываниями при таймауте */
void watchdog_enable(uint8_t  mode , uint8_t prescaler , void (*isr)()) {
	isr_wdt = *isr;  //указатель на функцию
	uint8_t wdtReg;
	if (mode) wdtReg = (1 << WDIE) | (1 << WDE);
	else wdtReg = (1 << WDIE);
	if (prescaler > 7) wdtReg |= (1 << WDP3)|(prescaler - 8);
	else wdtReg |= prescaler;
	cli();
	WDTCSR |= (1 << WDCE) | (1 << WDE);
	WDTCSR = wdtReg;
	sei();
}

/* вариант для работы без прерываний (только сброс) */
void watchdog_enable(uint8_t prescaler) {
	uint8_t wdtReg = (1 << WDE);
	if (prescaler > 7) wdtReg |= (1 << WDP3)|(prescaler - 8);
	else wdtReg |= prescaler;
	cli();
	WDTCSR |= (1 << WDCE) | (1 << WDE);
	WDTCSR = wdtReg;
	sei();
}