/*************************************************************************************
* Developed for AlexGyver https://github.com/AlexGyver/  by Egor 'Nich1con' Zaharov  *
* Library for generating interrupts on hardware timers ATmega328p					 *
* Distributed under a free license indicating the source							 *
* Supported MCU's : ATmega328p, ATmega2560											 *
* v1.0 from 18.02.2020 (Release)													 *
* v1.1 - исправлена ошибка в расчёте периодов									 	 *
* v1.2 - код разбит на h и cpp								 						 *
* v1.3 - поправлен незначительный баг												 *
* v1.4 - исправлена таблица частот и периодов										 *
* v1.5 - исправлен restart и resume			 										 *
* v1.6 - phase shift вынесен отдельным методом 										 *
*************************************************************************************/

/*
	Настройка и контроль прерываний по аппаратным таймерам
	Документация: https://alexgyver.ru/gyvertimers/
	- Поддерживаются все три таймера на ATmega328 и шесть таймеров на ATmega2560;		
	- Настройка периода (мкс) и частоты (Гц) прерываний:
		- 8 бит таймеры: 31 Гц - 1 МГц (32 258 мкс - 1 мкс);
		- 16 бит таймеры: 0.25 Гц - 1 МГц (4 000 000 мкс - 1 мкс);
	- Автоматическая корректировка настройки периода от частоты тактирования (F_CPU);
	- Функция возвращает точный установившийся период/частоту для отладки (частота ограничена разрешением таймера);
	- Поддержка многоканального режима работы: один таймер вызывает 2 (ATmega328) или
	3 (ATmega2560, таймеры 1, 3, 4, 5) прерывания с настраиваемым сдвигом по фазе 0-360 градусов;
	- Настраиваемое действие аппаратного вывода таймера по прерыванию: высокий сигнал, низкий сигнал, переключение. 
	Позволяет генерировать меандр (одно- и двухтактный);
	- Контроль работы таймера: старт/стоп/пауза/продолжить/инициализация;
*/

/*
----------------------------------- Arduino NANO (ATmega328) ---------------------------------------
Таймер	| Разрядность	| Частоты			| Периоды			| Выходы	| Пин Arduino	| Пин МК|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer0	| 8 бит			| 31 Гц - 1 МГц		| 32 258.. 1 мкс	| CHANNEL_A	| D6			| PD6	|
		| 				| 					| 					| CHANNEL_B	| D5			| PD5	|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer1	| 16 бит		| 0.25 Гц - 1 МГц	| 4 000 000.. 1 мкс	| CHANNEL_A	| D9			| PB1	|
		| 				| 					| 					| CHANNEL_B	| D10			| PB2	|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer2	| 8 бит			| 31 Гц - 1 МГц		| 32 258.. 1 мкс	| CHANNEL_A	| D11			| PB3	|
		| 				| 					| 					| CHANNEL_B	| D3			| PD3	|
----------------------------------------------------------------------------------------------------
						
--------------------------------- Arduino MEGA (ATmega2560) ----------------------------------------
Таймер	| Разрядность	| Частоты			| Периоды			| Выходы	| Пин Arduino	| Пин МК|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer0	| 8 бит			| 31 Гц - 1 МГц		| 32 258.. 1 мкс	| CHANNEL_A	| 13			| PB7	|
		| 				| 					| 					| CHANNEL_B	| 4				| PG5	|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer1	| 16 бит		| 0.25 Гц - 1 МГц	| 4 000 000.. 1 мкс	| CHANNEL_A	| 11			| PB5	|
		| 				| 					| 					| CHANNEL_B	| 12			| PB6	|
		| 				| 					| 					| CHANNEL_C	| 13			| PB7	|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer2	| 8 бит			| 31 Гц - 1 МГц		| 32 258.. 1 мкс	| CHANNEL_A	| 10			| PB4	|
		| 				| 					| 					| CHANNEL_B	| 9				| PH6	|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer3	| 16 бит		| 0.25 Гц - 1 МГц	| 4 000 000.. 1 мкс	| CHANNEL_A	| 5				| PE3	|
		| 				| 					| 					| CHANNEL_B	| 2				| PE4	|
		| 				| 					| 					| CHANNEL_C	| 3				| PE5	|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer4	| 16 бит		| 0.25 Гц - 1 МГц	| 4 000 000.. 1 мкс	| CHANNEL_A	| 6				| PH3	|
		| 				| 					| 					| CHANNEL_B	| 7				| PH4	|
		| 				| 					| 					| CHANNEL_C	| 8				| PH5	|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer5	| 16 бит		| 0.25 Гц - 1 МГц	| 4 000 000.. 1 мкс	| CHANNEL_A	| 46			| PL3	|
		| 				| 					| 					| CHANNEL_B	| 45			| PL4	|
		| 				| 					| 					| CHANNEL_C	| 44			| PL5	|
----------------------------------------------------------------------------------------------------
*/

/*
	setPeriod(период) - установка периода в микросекундах и запуск таймера. Возвращает реальный период (точность ограничена разрешением таймера).
	setFrequency(частота) - установка частоты в Герцах и запуск таймера. Возвращает реальную частоту (точность ограничена разрешением таймера).
	setFrequencyFloat(частота float) - установка частоты в Герцах и запуск таймера, разрешены десятичные дроби. Возвращает реальную частоту (точность ограничена разрешением таймера).
	enableISR(источник, фаза) - включить прерывания, канал прерываний CHANNEL_A или CHANNEL_B (+ CHANNEL_C у Mega2560), сдвиг фазы 0-359 (без указания параметров будет включен канал А и сдвиг фазы 0).
	disableISR(источник) - выключить прерывания, канал CHANNEL_A или CHANNEL_B. Счёт таймера не останавливается (без указания параметров будет выключен канал А).
	pause() - приостановить счёт таймера, не сбрасывая счётчик
	resume() - продолжить счёт после паузы
	stop() - остановить счёт и сбросить счётчик
	restart() - перезапустить таймер (сбросить счётчик)
	setDefault() - установить параметры таймера по умолчанию ("Ардуино-умолчания")
	outputEnable(канал, режим) - канал: включить выход таймера CHANNEL_A или CHANNEL_B (+ CHANNEL_C у Mega2560). Режим: TOGGLE_PIN, CLEAR_PIN, SET_PIN (переключить/выключить/включить пин по прерыванию)
	outputDisable(канал) - отключить выход таймера CHANNEL_A или CHANNEL_B (+ CHANNEL_C у Mega2560, см. такблицу таймеров)
	outputState(канал, состояние) - сменить состояние канала: HIGH / LOW
*/

#pragma once
#include <Arduino.h>

/* ==========  Константы ========== */
#define CHANNEL_A 0x00
#define CHANNEL_B 0x01
#define CHANNEL_C 0x02

#define TOGGLE_PIN 0x01
#define CLEAR_PIN 0x02
#define SET_PIN 0x03

#define TIMER0_A  TIMER0_COMPA_vect
#define TIMER0_B  TIMER0_COMPB_vect
#define TIMER1_A  TIMER1_COMPA_vect
#define TIMER1_B  TIMER1_COMPB_vect
#define TIMER2_A  TIMER2_COMPA_vect
#define TIMER2_B  TIMER2_COMPB_vect

#if defined(__AVR_ATmega2560__)
#define TIMER1_C  TIMER1_COMPC_vect
#define TIMER3_A  TIMER3_COMPA_vect
#define TIMER3_B  TIMER3_COMPB_vect
#define TIMER3_C  TIMER3_COMPC_vect
#define TIMER4_A  TIMER4_COMPA_vect
#define TIMER4_B  TIMER4_COMPB_vect
#define TIMER4_C  TIMER4_COMPC_vect
#define TIMER5_A  TIMER5_COMPA_vect
#define TIMER5_B  TIMER5_COMPB_vect
#define TIMER5_C  TIMER5_COMPC_vect
#endif

#define GYVERTIMERS_INLINE
/*inline __attribute__((always_inline))*/

/* ================ Сlasses of timers ================ */
class Timer_0 {                       					  // Timer 0
public:
	uint32_t setPeriod(uint32_t _timer0_period);          // Set timer period [us]
	uint32_t setFrequency(uint32_t _timer0_frequency);    // Set timer frequency [Hz]
	float setFrequencyFloat(float _timer0_frequency);  	  // Set timer float frequency [Hz]
	
	GYVERTIMERS_INLINE
	void enableISR(uint8_t source = CHANNEL_A);           // Enable timer interrupt , channel A or B 
	
	GYVERTIMERS_INLINE
	void disableISR(uint8_t source = CHANNEL_A);          // Disable timer interrupt , channel A or B
	
	GYVERTIMERS_INLINE
	void pause(void);                   				  // Disable timer clock , not cleaning the counter
	
	GYVERTIMERS_INLINE
	void resume(void);                 				      // Return clock timer settings
	
	GYVERTIMERS_INLINE
	void stop(void);                   					  // Disable timer clock , and cleaning the counter
	
	GYVERTIMERS_INLINE
	void restart(void);                  				  // Return clock timer settings & reset counter
	
	GYVERTIMERS_INLINE
	void setDefault(void);               			      // Set default timer settings
	
	GYVERTIMERS_INLINE
	void outputEnable(uint8_t channel, uint8_t mode);	  // Enable and configurate timer hardware output
	
	GYVERTIMERS_INLINE
	void outputDisable(uint8_t channel);				  // Disable timer hardware output
	
	GYVERTIMERS_INLINE
	void outputState(uint8_t channel,bool state);		  // Set High / Low on the timer output 
	
	GYVERTIMERS_INLINE
	void phaseShift(uint8_t source, uint16_t phase);
	
private:
	uint8_t _timer0_clock = 0x00;           			  // Variable to store timer clock settings
};

class Timer_1 {                      					  // Timer 1
public:
	uint32_t setPeriod(uint32_t _timer1_period);      	  // Set timer period [Hz]
	uint32_t setFrequency(uint32_t _timer1_frequency);    // Set timer frequency [Hz]
	float setFrequencyFloat(float _timer1_frequency);     // Set timer float frequency [Hz]
	
	GYVERTIMERS_INLINE
	void enableISR(uint8_t source = CHANNEL_A);       	  // Enable timer interrupt , channel A or B 
	
	GYVERTIMERS_INLINE
	void disableISR(uint8_t source = CHANNEL_A);          // Disable timer interrupt , channel A or B
	
	GYVERTIMERS_INLINE
	void pause(void);                   				  // Disable timer clock , not cleaning the counter
	
	GYVERTIMERS_INLINE
	void resume(void);                    				  // Return clock timer settings
	
	GYVERTIMERS_INLINE
	void stop(void);                    				  // Disable timer clock , and cleaning the counter	
	
	GYVERTIMERS_INLINE
	void restart(void);                   				  // Return clock timer settings & reset counter
	
	GYVERTIMERS_INLINE
	void setDefault(void);                  			  // Set default timer settings
	
	GYVERTIMERS_INLINE
	void outputEnable(uint8_t channel, uint8_t mode);	  // Enable and configurate timer hardware output
	
	GYVERTIMERS_INLINE
	void outputDisable(uint8_t channel);				  // Disable timer hardware output
	
	GYVERTIMERS_INLINE
	void outputState(uint8_t channel,bool state);		  // Set High / Low on the timer output  
	
	GYVERTIMERS_INLINE
	void phaseShift(uint8_t source, uint16_t phase);
	
private:
	uint8_t _timer1_clock = 0x00;             			  // Variable to store timer clock settings
};

class Timer_2 {                       					  // Timer 2
public:
	uint32_t setPeriod(uint32_t _timer2_period);      	  // Set timer period [Hz]
	uint32_t setFrequency(uint32_t _timer2_frequency);    // Set timer frequency [Hz]
	float setFrequencyFloat(float _timer2_frequency);     // Set timer float frequency [Hz]
	
	GYVERTIMERS_INLINE
	void enableISR(uint8_t source = CHANNEL_A);      	  // Enable timer interrupt , channel A or B 
	
	GYVERTIMERS_INLINE
	void disableISR(uint8_t source = CHANNEL_A);          // Disable timer interrupt , channel A or B
	
	GYVERTIMERS_INLINE
	void pause(void);                   				  // Disable timer clock , not cleaning the counter
	
	GYVERTIMERS_INLINE
	void resume(void);                 				      // Return clock timer settings
	
	GYVERTIMERS_INLINE
	void stop(void);                    				  // Disable timer clock , and cleaning the counter
	
	GYVERTIMERS_INLINE
	void restart(void);                  				  // Return clock timer settings & reset counter
	
	GYVERTIMERS_INLINE
	void setDefault(void);                  			  // Set default timer settings
	
	GYVERTIMERS_INLINE
	void outputEnable(uint8_t channel, uint8_t mode);	  // Enable and configurate timer hardware output
	
	GYVERTIMERS_INLINE
	void outputDisable(uint8_t channel);				  // Disable timer hardware output
	
	GYVERTIMERS_INLINE
	void outputState(uint8_t channel,bool state);		  // Set High / Low on the timer output  
	
	GYVERTIMERS_INLINE
	void phaseShift(uint8_t source, uint16_t phase);
	
private:
	uint8_t _timer2_clock = 0x00;             			  // Variable to store timer clock settings	
};

#if defined(__AVR_ATmega2560__)
class Timer_3 {                       					  // Timer 3
public:
	uint32_t setPeriod(uint32_t _timer3_period);     	  // Set timer period [Hz]
	uint32_t setFrequency(uint32_t _timer3_frequency);    // Set timer frequency [Hz]
	float setFrequencyFloat(float _timer3_frequency);     // Set timer float frequency [Hz]
	
	GYVERTIMERS_INLINE
	void enableISR(uint8_t source = CHANNEL_A);       	  // Enable timer interrupt , channel A or B
	
	GYVERTIMERS_INLINE
	void disableISR(uint8_t source = CHANNEL_A);          // Disable timer interrupt , channel A or B
	
	GYVERTIMERS_INLINE
	void pause(void);                  					  // Disable timer clock , not cleaning the counter
	
	GYVERTIMERS_INLINE
	void resume(void);                   				  // Return clock timer settings
	
	GYVERTIMERS_INLINE
	void stop(void);                    				  // Disable timer clock , and cleaning the counter
	
	GYVERTIMERS_INLINE
	void restart(void);                   				  // Return clock timer settings & reset counter
	
	GYVERTIMERS_INLINE
	void setDefault(void);                				  // Set default timer settings
	
	GYVERTIMERS_INLINE
	void outputEnable(uint8_t channel, uint8_t mode);	  // Enable and configurate timer hardware output
	
	GYVERTIMERS_INLINE
	void outputDisable(uint8_t channel);				  // Disable timer hardware output
	
	GYVERTIMERS_INLINE
	void outputState(uint8_t channel,bool state);		  // Set High / Low on the timer output 
	
	GYVERTIMERS_INLINE
	void phaseShift(uint8_t source, uint16_t phase);
	
private:
	uint8_t _timer3_clock = 0x00;             			  // Variable to store timer clock settings
};

class Timer_4 {                      					  // Timer 4
public:
	uint32_t setPeriod(uint32_t _timer4_period);      	  // Set timer period [Hz]
	uint32_t setFrequency(uint32_t _timer4_frequency);    // Set timer frequency [Hz]
	float setFrequencyFloat(float _timer4_frequency);     // Set timer float frequency [Hz]
	
	GYVERTIMERS_INLINE
	void enableISR(uint8_t source = CHANNEL_A);       	  // Enable timer interrupt , channel A or B
	
	GYVERTIMERS_INLINE
	void disableISR(uint8_t source = CHANNEL_A);          // Disable timer interrupt , channel A or B
	
	GYVERTIMERS_INLINE
	void pause(void);                  					  // Disable timer clock , not cleaning the counter
	
	GYVERTIMERS_INLINE
	void resume(void);                    				  // Return clock timer settings
	
	GYVERTIMERS_INLINE
	void stop(void);                   				      // Disable timer clock , and cleaning the counter
	
	GYVERTIMERS_INLINE
	void restart(void);                        			  // Return clock timer settings & reset counter
	
	GYVERTIMERS_INLINE
	void setDefault(void);                  			  // Set default timer settings
	
	GYVERTIMERS_INLINE
	void outputEnable(uint8_t channel, uint8_t mode);	  // Enable and configurate timer hardware output
	
	GYVERTIMERS_INLINE
	void outputDisable(uint8_t channel);				  // Disable timer hardware output
	
	GYVERTIMERS_INLINE
	void outputState(uint8_t channel,bool state);		  // Set High / Low on the timer output 
	
	GYVERTIMERS_INLINE
	void phaseShift(uint8_t source, uint16_t phase);
	
private:
	uint8_t _timer4_clock = 0x00;            			  // Variable to store timer clock settings
};

class Timer_5 {                     					  // Timer 5
public:
	uint32_t setPeriod(uint32_t _timer5_period);          // Set timer period [Hz]
	uint32_t setFrequency(uint32_t _timer5_frequency);    // Set timer frequency [Hz]
	float setFrequencyFloat(float _timer5_frequency);     // Set timer float frequency [Hz]
	
	GYVERTIMERS_INLINE
	void enableISR(uint8_t source = CHANNEL_A);       	  // Enable timer interrupt , channel A or B
	
	GYVERTIMERS_INLINE
	void disableISR(uint8_t source = CHANNEL_A);          // Disable timer interrupt , channel A or B
	
	GYVERTIMERS_INLINE
	void pause(void);                  					  // Disable timer clock , not cleaning the counter
	
	GYVERTIMERS_INLINE
	void resume(void);                    				  // Return clock timer settings
	
	GYVERTIMERS_INLINE
	void stop(void);                    				  // Disable timer clock , and cleaning the counter
	
	GYVERTIMERS_INLINE
	void restart(void);                  				  // Return clock timer settings & reset counter
	
	GYVERTIMERS_INLINE
	void setDefault(void);                 				  // Set default timer settings
	
	GYVERTIMERS_INLINE
	void outputEnable(uint8_t channel, uint8_t mode);	  // Enable and configurate timer hardware output
	
	GYVERTIMERS_INLINE
	void outputDisable(uint8_t channel);				  // Disable timer hardware output
	
	GYVERTIMERS_INLINE
	void outputState(uint8_t channel,bool state);		  // Set High / Low on the timer output 
	
	GYVERTIMERS_INLINE
	void phaseShift(uint8_t source, uint16_t phase);
	
private:
	uint8_t _timer5_clock = 0x00;                		  // Variable to store timer clock settings
};

#endif

extern Timer_0 Timer0;
extern Timer_1 Timer1;
extern Timer_2 Timer2;

#if defined(__AVR_ATmega2560__)
extern Timer_3 Timer3;
extern Timer_4 Timer4;
extern Timer_5 Timer5;
#endif