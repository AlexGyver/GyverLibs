#pragma once
#include <Arduino.h>
/*
	GyverPower - библиотека для управления энергопотреблением AVR
	- Управление системным клоком
		- Функции времени (millis/delay) корректируются под выбранный клок
	- Включение/выключение периферии:
		- BOD
		- Таймеры
		- I2C/UART/SPI
		- USB
		- ADC
	- Сон в разных режимах:
		- IDLE_SLEEP		- отключается только CPU и Flash
		- ADC_SLEEP			- автоматически начинает преобразование при уходе в сон, отключается CPU и system clock
		- POWERDOWN_SLEEP	- отключается всё, кроме WDT и внешних прерываний, относительно долгий выход из сна
		- POWERSAVE_SLEEP	- аналог power down, но timer 2 остается активным, можно использовать для счета времени
		- STANDBY_SLEEP		- аналог power down, но system clock остается активен, быстрый выход из сна, большее потребление		
		- EXTSTANDBY_SLEEP	- аналог STANDBY, но timer 2 остается активным, можно использовать для счета времени
	- Сон на любой период
		- Калибровка таймера для точных периодов сна
	- Поддерживаемые МК
		- Atmega2560/32u4/328
		- Attiny85/84/167
		
	by Egor 'Nich1con' Zaharov 07.10.2019
	Версия 1.0
*/

// ================= ФУНКЦИИ ==================
void hardwareEnable(uint16_t data);			// Включение указанной периферии (см. ниже "Константы периферии")
void hardwareDisable(uint16_t data);		// Выключение указанной периферии (см. ниже "Константы периферии")
void setSystemPrescaler(uint8_t prescaler); // Установка делителя системной частоты (см. ниже "Константы делителя")
void bodInSleep(bool en);					// Brown-out detector в режиме сна [вкл-выкл] [true - false]
void setSleepMode(uint8_t mode);			// Установка текущего режима сна (см. ниже "Режимы сна")
void autoCalibrate(void);					// автоматическая калибровка таймера сна, выполняется 2 секунды
uint16_t getMaxTimeout(void);				// возвращает реальный период 8с, выполняется 8 секунд
void calibrate(uint16_t ms);				// калибровка тайм-аутов watchdog для sleepDelay [ввести макс период из getMaxTimeout]
void sleep(uint8_t period);					// Переход мк в режим сна (см. ниже "Периоды сна")
uint8_t sleepDelay(uint32_t ms);			// сон на произвольный период (до 52 суток)

/* ======== РЕЖИМЫ СНА ========
IDLE_SLEEP			- Легкий сон , отключается только CPU и Flash
ADC_SLEEP			- Легкий сон , автоматически начинает преобразование при уходе в сон, отключается CPU и system clock
POWERDOWN_SLEEP		- Наиболее глубокий сон, отключается все, кроме WDT и внешних прерываний,относительно долгий выход из сна
STANDBY_SLEEP		- Глубокий сон, аналог power down, но system clock остается активен, быстрый выход из сна , большее потребление
POWERSAVE_SLEEP		- Глубокий сон, аналог power down, но timer 2 остается активным, можно использовать для счета времени
EXTSTANDBY_SLEEP	- Глубокий сон, аналог STANDBY, но timer 2 остается активным, можно использовать для счета времени
*/

/* ======= ПЕРИОДЫ СНА =======
SLEEP_16MS
SLEEP_32MS
SLEEP_64MS
SLEEP_128MS
SLEEP_256MS
SLEEP_512MS
SLEEP_1024MS
SLEEP_2048MS
SLEEP_4096MS
SLEEP_8192MS

SLEEP_FOREVER
*/

/* ==== КОНСТАНТЫ ДЕЛИТЕЛЯ ====
PRESCALER_1
PRESCALER_2
PRESCALER_4
PRESCALER_8
PRESCALER_16
PRESCALER_32
PRESCALER_64
PRESCALER_128
PRESCALER_256
*/

/* ==== КОНСТАНТЫ ПЕРИФЕРИИ ====
PWR_ALL		- всё
PWR_USB		- usb
PWR_TIMER5	- таймер 5
PWR_TIMER4	- таймер 4
PWR_TIMER3	- таймер 3
PWR_UART3	- Serial3
PWR_UART2	- Serial2
PWR_UART1	- Serial1
PWR_I2C		- Wire
PWR_TIMER2	- таймер 2
PWR_TIMER0	- таймер 1
PWR_TIMER1	- таймер 0
PWR_SPI		- Spi
PWR_UART0	- Serial
PWR_ADC		- АЦП
PWR_USI		- Wire + Spi (ATtinyXX)
PWR_LIN		- USART LIN (ATtinyXX)
*/

// ================= КОНСТАНТЫ ==================
/* --- time fix --- */
#ifdef FIX_TIME
#define millis() (millis() << (CLKPR & 0xF))
#define micros() (micros() << (CLKPR & 0xF))
#define delay(x) delay((x) >> (CLKPR & 0xf))
#define delayMicroseconds(x) delayMicroseconds((x) >> (CLKPR & 0xf))
#endif

/* --- system prescaler --- */
#define PRESCALER_1 0x0
#define PRESCALER_2 0x1
#define PRESCALER_4 0x2
#define PRESCALER_8 0x3
#define PRESCALER_16 0x4
#define PRESCALER_32 0x5
#define PRESCALER_64 0x6
#define PRESCALER_128 0x7
#define PRESCALER_256 0x8

/* --- sleep periods --- */
#define SLEEP_16MS 0
#define SLEEP_32MS 1
#define SLEEP_64MS 2
#define SLEEP_128MS 3
#define SLEEP_256MS 4
#define SLEEP_512MS 5
#define SLEEP_1024MS 6
#define SLEEP_2048MS 7
#define SLEEP_4096MS 8
#define SLEEP_8192MS 9
#define SLEEP_FOREVER 10

/* --- sleep modes --- */
#define IDLE_SLEEP 0x0
#define ADC_SLEEP 0x1
#define POWERDOWN_SLEEP 0x2
#define EXTSTANDBY_SLEEP 0x7
#if defined(__AVR_ATtiny84__)
#define STANDBY_SLEEP 0x3
#else
#define POWERSAVE_SLEEP 0x3
#define STANDBY_SLEEP 0x6
#endif

/* --- periphery disable / enable --- */
#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny167__) || defined(__AVR_ATtiny84__)
#define PWR_ALL 0xFFFF
#define PWR_LIN _BV(5)
#define PWR_SPI _BV(4)
#define PWR_TIMER1 _BV(3)
#define PWR_TIMER0 _BV(2)
#define PWR_USI _BV(1)
#define PWR_ADC _BV(0)
#else
#define PWR_ALL 0xFFFF
#define PWR_USB _BV(15)
#define PWR_TIMER5 _BV(13)
#define PWR_TIMER4 _BV(12)
#define PWR_TIMER3 _BV(11)
#define PWR_UART3 _BV(10)
#define PWR_UART2 _BV(9)
#define PWR_UART1 _BV(8)
#define PWR_I2C _BV(7)
#define PWR_TIMER2 _BV(6)
#define PWR_TIMER0 _BV(5)
#define PWR_TIMER1 _BV(3)
#define PWR_SPI _BV(2)
#define PWR_UART0 _BV(1)
#define PWR_ADC _BV(0)
#endif

/* --->>> directives <<<--- */
#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny167__)
#define WDTCSR WDTCR
#endif

/* --->>> default sleep parameters <<<--- */
bool bodEnable = false;
uint8_t sleepMode = POWERDOWN_SLEEP;
uint16_t timeOuts[10] = {16 , 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192};


/* --->>> the implementation of the functions <<<--- */

void hardwareEnable(uint16_t data) {
#if defined(__AVR_ATmega2560__) || (__AVR_ATmega32U4__)
	PRR1 &= ~ highByte(data); // загрузили данные в регистр
	PRR0 &= ~ lowByte(data);
#else
	PRR &= ~ lowByte(data);
#endif
	if (data & PWR_ADC) { // если что то сказали про ацп
		ADCSRA |= (1 << ADEN); // вкл ацп
		ACSR &= ~ (1 << ACD); // вкл компаратор
	}
}

void hardwareDisable(uint16_t data) {
	if (data & PWR_ADC) { // если что то сказали про ацп
		ADCSRA &= ~ (1 << ADEN); // выкл ацп
		ACSR |= (1 << ACD); // выкл компаратор
	}
#if defined(__AVR_ATmega2560__) || (__AVR_ATmega32U4__)
	PRR1 |= highByte(data); // загрузили данные в регистр
	PRR0 |= lowByte(data);
#else
	PRR |= lowByte(data);
#endif
}

void setSystemPrescaler(uint8_t prescaler) {
	CLKPR = (1 << CLKPCE); // разрешили менять делитель
	CLKPR = prescaler; // загрузили новый
}

void bodInSleep(bool state) {
	bodEnable = state;
}

void setSleepMode(uint8_t mode) {
	sleepMode = mode;
}

uint16_t getMaxTimeout(void){
	WDTCSR |= (1 << WDCE) | (1 << WDE); // разрешаем вмешательство
	WDTCSR = 0x61; // таймаут ~ 8 c
	asm ("wdr"); // сбросили пса
	uint16_t startTime = millis(); // засекли время
	while (!(WDTCSR & (1 << WDIF))); // ждем таймаута
	uint16_t ms = millis() - startTime;
	WDTCSR |= (1 << WDCE) | (1 << WDE); // разрешаем вмешательство
	WDTCSR = 0; // выкл wdt 
	return  ms;
}

void autoCalibrate(void) {
	WDTCSR |= (1 << WDCE) | (1 << WDE); // разрешаем вмешательство
	WDTCSR = 0x47; // таймаут ~ 2 c
	asm ("wdr"); // сбросили пса
	uint16_t startTime = millis(); // засекли время
	while (!(WDTCSR & (1 << WDIF))); // ждем таймаута
	uint16_t ms = millis() - startTime;
	WDTCSR |= (1 << WDCE) | (1 << WDE); // разрешаем вмешательство
	WDTCSR = 0; // выкл wdt 
	for (uint8_t i = 0; i < 9 ; i++) { // пересчитываем массив
		timeOuts[9 - i] = ((ms * 4) >> i);
	}
}

void calibrate(uint16_t ms) { // пересчет массива таймаутов
	for (uint8_t i = 0; i < 9 ; i++) { // пересчитываем массив
		timeOuts[9 - i] = (ms >> i);
	}
}

void sleep(uint8_t period) {

	/* принудительно выкл АЦП и компаратор */
	if (sleepMode != IDLE_SLEEP && sleepMode != ADC_SLEEP) {
		ADCSRA &= ~ (1 << ADEN); // выкл ацп
		ACSR |= (1 << ACD); // выкл аналог компаратор
	}

	/* принудительное отключение PLL */
#if defined(__AVR_ATtiny85__)
	uint8_t pllCopy = PLLCSR; // запомнили настройки
	PLLCSR &= ~ (1 << PLLE); // выключили
#endif

	/* если спим с WDT - настраиваем его */
	if (period < 10) {
		uint8_t wdtReg = (1 << WDIE); // режим прерываний
		if (period > 7) { // если больше 7
			wdtReg |= (period - 8) | (1 << WDP3); // задвигаем делитель
		}
		else wdtReg |= period; // задвигаем делитель
		cli();
		WDTCSR |= (1 << WDCE) | (1 << WDE); // разрешаем вмешательство
		WDTCSR = wdtReg; // задвигаем весь регистр
		sei();
		asm ("wdr"); // сброс пса
	}

	/* настраиваем нужный режим сна */
#if defined(__AVR_ATtiny85__) ||  defined(__AVR_ATtiny84__)
	MCUCR |= (sleepMode << 3) | (1 << SE); // sleep mode + sleep enable
#else
	SMCR = (sleepMode << 1) | (1 << SE); // sleep mode + sleep enable
#endif

	/* процедура ухода в сон */
	if (bodEnable) asm ("sleep"); //  <<< точка ухода в сон
	else {
#if defined(__AVR_ATtiny85__) ||  defined(__AVR_ATtiny84__)
		MCUCR |= (1 << BODS) | (1 << BODSE); // выкл bod
		MCUCR = (MCUCR & 0x7B) | (1 << BODS);
#else
		MCUCR = (0x03 << 5); // выкл bod
		MCUCR = (0x02 << 5);
#endif
		asm ("sleep"); //  <<< точка ухода в сон
	}

	/* процедура выхода из сна*/
#if defined(__AVR_ATtiny85__) ||  defined(__AVR_ATtiny84__)
	MCUCR = MCUCR & 0xC7; // откл сон
#else
	SMCR = 0; // откл сон
#endif

	/* восстановление настроек АЦП */
#if defined(__AVR_ATmega2560__) || (__AVR_ATmega32U4__)
	if (!(PRR0 & (1 << PRADC))) { // если ацп не выключен принудительно
		ADCSRA |= (1 << ADEN); // вкл после сна
		ACSR &= ~ (1 << ACD);
	}
#else
	if (!(PRR & (1 << PRADC))) {	// если ацп не выключен принудительно
		ADCSRA |= (1 << ADEN);		// вкл после сна
		ACSR &= ~ (1 << ACD);
	}
#endif
	/* восстановление настроек PLL */
#if defined(__AVR_ATtiny85__)
	PLLCSR = pllCopy;
#endif

}

uint8_t sleepDelay(uint32_t ms) {
	for (uint8_t i = 9; ms > timeOuts[0]; i--) { // начиная с длинного периода , пока мс > ~16
		while (ms > timeOuts[i]) {	// пока мс > текущего выбранного периода
			sleep(i);				// уйти в сон
			ms -= timeOuts[i];		// отнять время сна
		}
	}
	return ms; // вернуть остаток времени
}

ISR(WDT_vect) {		// просыпаемся тут
	WDTCSR |= (1 << WDCE) | (1 << WDE); // разрешаем вмешательство
	WDTCSR = 0;		// выключаем пса
	asm ("wdr");		// обнуляем пса
}