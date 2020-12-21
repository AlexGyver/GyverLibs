#ifndef GyverPower_h
#define GyverPower_h
#include <Arduino.h>
#include "powerConstants.h"
/*
	GyverPower - библиотека для управления энергопотреблением AVR
	Документация: https://alexgyver.ru/gyverpower/
	- Управление системным клоком		
	- Включение/выключение периферии:
		- BOD
		- Таймеры
		- I2C/UART/SPI
		- USB
		- ADC		
	- Сон в разных режимах (список ниже)		
	- Сон на любой период
		- Калибровка таймера для точных периодов сна
		- Корректировка millis()		
	- Поддерживаемые МК
		- Atmega2560/32u4/328
		- Attiny85/84/167
		
	by Egor 'Nich1con' Zaharov и AlexGyver 07.10.2019
	
	v1.1 от 24.05.2020
	- Добавлена функция wakeUp для просыпания по прерыванию из sleepDelay (см. пример interruptWakesSleepDelay)
	- Библиотека обёрнута в класс (несовместимо с версией 1.0!!! Нужно добавить везде power. )
	- Убраны дефайны (для безопасности)
	- Добавлена коррекция миллис на время сна
	- Добавлены примеры и расширенное описание
	- Улучшена стабильность калибровки (не зависает)
	
	v1.2 - фикс калибровки
	v1.3 - фикс для 32U4
	v1.4 - добавлен adjustInternalClock
	v1.5 - совместимость с аттини
*/

// =============== ФУНКЦИИ ===============
class GyverPower {
public:
	void hardwareEnable(uint16_t data);				// включение указанной периферии (см. ниже "Константы периферии")
	void hardwareDisable(uint16_t data);			// выключение указанной периферии (см. ниже "Константы периферии")
	void setSystemPrescaler(prescalers_t prescaler);// установка делителя системной частоты (см. ниже "Константы делителя")
	void bodInSleep(bool en);						// Brown-out detector в режиме сна (true вкл - false выкл) по умолч. отключен!
	void setSleepMode(sleepmodes_t mode);			// установка текущего режима сна (см. ниже "Режимы сна")
	void autoCalibrate(void);						// автоматическая калибровка таймера сна, выполняется 2 секунды
	uint16_t getMaxTimeout(void);					// возвращает реальный период "8 секунд", выполняется ~8 секунд
	void calibrate(uint16_t ms);					// ручная калибровка тайм-аутов watchdog для sleepDelay (ввести макс период из getMaxTimeout)
	void sleep(sleepprds_t period);					// сон на фиксированный период (см. ниже "Периоды сна")
	uint8_t sleepDelay(uint32_t ms);				// сон на произвольный период в миллисекундах (до 52 суток), возвращает остаток времени для коррекции таймеров
	void correctMillis(bool state);					// корректировать миллис на время сна sleepDelay() (по умолчанию включено)
	void wakeUp(void);								// помогает выйти из sleepDelay прерыванием (вызывать в будящем прерывании)	
	void adjustInternalClock(int8_t adj);     		// подстройка частоты внутреннего генератора (число -120...+120)

private:
	volatile bool wakeFlag = false;
	bool correct = true;
	bool bodEnable = false;
	uint8_t sleepMode = 0x2;	// (POWERDOWN_SLEEP по умолчанию)
	uint16_t timeOuts[10] = {16 , 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192};
};
extern GyverPower power;

// =============== КОНСТАНТЫ ===============

/* 
	РЕЖИМЫ СНА для setSleepMode()
	
	IDLE_SLEEP			- Легкий сон, отключается только клок CPU и Flash, просыпается мгновенно от любых прерываний
	ADC_SLEEP			- Легкий сон, отключается CPU и system clock, АЦП начинает преобразование при уходе в сон (см. пример ADCinSleep)	
	POWERDOWN_SLEEP		- Наиболее глубокий сон, отключается всё кроме WDT и внешних прерываний, просыпается от аппаратных (обычных + PCINT) или WDT, пробуждение за 1000 тактов (62 мкс)
	STANDBY_SLEEP		- Глубокий сон, идентичен POWERDOWN_SLEEP + system clock активен, пробуждение за 6 тактов (0.4 мкс)
	POWERSAVE_SLEEP		- Глубокий сон, идентичен POWERDOWN_SLEEP + timer 2 активен (+ можно проснуться от его прерываний), можно использовать для счета времени (см. пример powersaveMillis)
	EXTSTANDBY_SLEEP	- Глубокий сон, идентичен POWERSAVE_SLEEP + system clock активен, пробуждение за 6 тактов (0.4 мкс)
*/

/* 
	ПЕРИОДЫ СНА для sleep()
	
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
	SLEEP_FOREVER	- вечный сон без таймера
*/

/* 
	КОНСТАНТЫ ДЕЛИТЕЛЯ для setSystemPrescaler()
	
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

/* 
	КОНСТАНТЫ ПЕРИФЕРИИ для hardwareDisable() и hardwareEnable()
	
	PWR_ALL		- всё железо
	PWR_ADC		- АЦП и компаратор
	PWR_TIMER1	- Таймер 0
	PWR_TIMER0	- Таймер 1
	PWR_TIMER2	- Таймер 2
	PWR_TIMER3	- Таймер 3
	PWR_TIMER4	- Таймер 4
	PWR_TIMER5	- Таймер 5	
	PWR_UART0	- Serial 0
	PWR_UART1	- Serial 1
	PWR_UART2	- Serial 2
	PWR_UART3	- Serial 3
	PWR_I2C		- Wire
	PWR_SPI		- SPI
	PWR_USB		- USB	
	PWR_USI		- Wire + Spi (ATtinyXX)
	PWR_LIN		- USART LIN (ATtinyXX)
*/
#endif