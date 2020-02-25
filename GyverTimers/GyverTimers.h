/*************************************************************************************
* Developed for AlexGyver https://github.com/AlexGyver/  by Egor 'Nich1con' Zaharov  *
* Library for generating interrupts on hardware timers ATmega328p					 *
* Distributed under a free license indicating the source							 *
* Supported MCU's : ATmega328p, ATmega2560											 *
* -> v1.0 from 18.02.2020 (Release)													 *
*************************************************************************************/

/*
	Настройка и контроль прерываний по аппаратным таймерам:
		- Поддерживаются все три таймера на ATmega328 и шесть таймеров на ATmega2560;		
		- Настройка периода (мкс) и частоты (Гц) прерываний:
			- 8 бит таймеры: 31 Гц - 1 МГц (32 258 мкс - 1 мкс);
			- 16 бит таймеры: 0.11 Гц - 1 МГц (9 000 000 мкс - 1 мкс);
		- Автоматическая корректировка настройки периода от частоты тактирования (F_CPU);
		- Функция возвращает точный установившийся период/частоту для отладки (частота ограничена разрешением таймера);
		- Поддержка многоканального режима работы: один таймер вызывает 2 (ATmega328) или
		3 (ATmega2560, таймеры 1, 3, 4, 5) прерывания с настраиваемым сдвигом по фазе 0-360 градусов;
		- Настраиваемое действие аппаратного вывода таймера по прерыванию: высокий сигнал, низкий сигнал, переключение. 
		Позволяет генерировать меандр (одно- и двухтактный);
		- Контроль работы таймера: старт/стоп/пауза/продолжить/инициализация;
*/

/*
----------------------------------- Arduino NANO (ATmega328) ----------------------------------------
Таймер	| Разрядность	| Частоты			| Периоды			| Выходы	| Пин Arduino	| Пин МК|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer0	| 8 бит			| 31 Гц - 1 МГц		| 32 258 - 1 мкс	| CHANNEL_A	| D6			| PD6	|
		| 				| 					| 					| CHANNEL_B	| D5			| PD5	|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer1	| 16 бит		| 0.11 Гц - 1 МГц	| 9 000 000 - 1 мкс	| CHANNEL_A	| D9			| PB1	|
		| 				| 					| 					| CHANNEL_B	| D10			| PB2	|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer2	| 8 бит			| 31 Гц - 1 МГц		| 32 258 - 1 мкс	| CHANNEL_A	| D11			| PB3	|
		| 				| 					| 					| CHANNEL_B	| D3			| PD3	|
-----------------------------------------------------------------------------------------------------
						
--------------------------------- Arduino MEGA (ATmega2560) -----------------------------------------
Таймер	| Разрядность	| Частоты			| Периоды			| Выходы	| Пин Arduino	| Пин МК|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer0	| 8 бит			| 31 Гц - 1 МГц		| 32 258 - 1 мкс	| CHANNEL_A	| 13			| PB7	|
		| 				| 					| 					| CHANNEL_B	| 4				| PG5	|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer1	| 16 бит		| 0.11 Гц - 1 МГц	| 9 000 000 - 1 мкс	| CHANNEL_A	| 11			| PB5	|
		| 				| 					| 					| CHANNEL_B	| 12			| PB6	|
		| 				| 					| 					| CHANNEL_C	| 13			| PB7	|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer2	| 8 бит			| 31 Гц - 1 МГц		| 32 258 - 1 мкс	| CHANNEL_A	| 10			| PB4	|
		| 				| 					| 					| CHANNEL_B	| 9				| PH6	|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer3	| 16 бит		| 0.11 Гц - 1 МГц	| 9 000 000 - 1 мкс	| CHANNEL_A	| 5				| PE3	|
		| 				| 					| 					| CHANNEL_B	| 2				| PE4	|
		| 				| 					| 					| CHANNEL_C	| 3				| PE5	|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer4	| 16 бит		| 0.11 Гц - 1 МГц	| 9 000 000 - 1 мкс	| CHANNEL_A	| 6				| PH3	|
		| 				| 					| 					| CHANNEL_B	| 7				| PH4	|
		| 				| 					| 					| CHANNEL_C	| 8				| PH5	|
--------|---------------|-------------------|-------------------|-----------|---------------|-------|
Timer5	| 16 бит		| 0.11 Гц - 1 МГц	| 9 000 000 - 1 мкс	| CHANNEL_A	| 46			| PL3	|
		| 				| 					| 					| CHANNEL_B	| 45			| PL4	|
		| 				| 					| 					| CHANNEL_C	| 44			| PL5	|
-----------------------------------------------------------------------------------------------------
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

// This code was slightly improved by Aleev Aleksey
// Свернул простынку ;-)
// Исправил расчёт времени, теперь как в мануале (CTC mode),   500 500 500 500   
// на один период приходиться два прерывания. 500 мкс = 1 Кгц  ¯¯¯|___|¯¯¯|___
// Выкинул не нужные inline и не только :-)
// Дописал конструкор
// setFrequency() теперь понимает int и float 
// Табличка вверху немного не точная! MAX для 8bit = 16384 мкс, для 16bit = 4194304 мкс
// поленился добавить проверку на создание второго объекта для одного таймера :-))

#pragma once

#include <Arduino.h>

#define WGM1  1
#define WGM2  3
#define WGM3  4
#define FOCC  5 
#define FOCB  6
#define FOCA  7 
#define OCIEA  1
#define OCIEB  2
#define OCIEC  3

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

class Timer {   
           uint8_t  TIMER;                                                                          // Current used Timer
           uint8_t  timer_clock = 0x00;                                                             // Variable to store timer clock settings
  volatile uint8_t  *TCCRA, *TCCRB, *TCCRC, *TIMSK,                                                 // REGISTER POINTERS
                    *TCNT,  *OCRA,  *OCRB;                                                          // <-- Timer0 and Timer2 = 8 bit others 16 bit 
  volatile uint16_t *ICR;      
public:
            Timer(uint8_t timer);                                                                   // Constructor [Timers Number]
  uint32_t  setPeriod(uint32_t timer_period);                                                       // Set timer period [us]
  uint32_t  setFrequency(uint32_t frequency){return 5e5 / (setPeriod(1e6 / (frequency << 1)));}     // Set PWM frequency [Hz]
  float     setFrequency(float frequency){return 5.0e5 / (setPeriod(1.0e6 / (frequency * 2.0)));}
  float     setFrequencyFloat(float frequency){setFrequency(frequency);}                            // Old version 
  void      enableISR(uint8_t source, uint16_t phase);                                              // Enable timer interrupt , channel A or B , set phase shift between interrupts
  void      disableISR(uint8_t source);                                                             // Disable timer interrupt , channel A or B
  void      pause() {timer_clock = (*TCCRB & 0x07); *TCCRB = (*TCCRB & 0xF8);}                      // Disable timer clock , not cleaning the counter
  void      resume(){*TCCRB = ((*TCCRB & 0xF8) |  timer_clock);}                                    // Return clock timer settings
  void      stop()  {pause(); *TCNT = 0x00; if(TIMER == 1 || TIMER >= 3) *(TCNT+1) = 0x00;}         // Disable timer clock , and cleaning the counter
  void      restart(){resume(); *TCNT = 0x00; if(TIMER == 1 || TIMER >= 3) *(TCNT+1) = 0x00;}       // Return clock timer settings & reset counter
  void      setDefault();                                                                           // Set default timer settings
  void      outputEnable(uint8_t channel, uint8_t mode);                                            // Enable and configurate timer hardware output
  void      outputDisable(uint8_t channel);                                                         // Disable timer hardware outbreak
  void      outputState(uint8_t channel,bool state);                                                // Set High / Low on the timer output 
};

Timer::Timer(uint8_t timer){                          // Конструктор
  TIMER = timer;                                      // Номер используемого таймера
  switch (TIMER){                                     // Инициализация указателей для соответствующего таймера
    case 0: TCCRA = &TCCR0A;
            TCCRB = &TCCR0B;
            TCNT  = &TCNT0;
            TIMSK = &TIMSK0;
            OCRA  = &OCR0A;
            OCRB  = &OCR0B;
            break;
    case 1: TCCRA = &TCCR1A;
            TCCRB = &TCCR1B;
            TCCRC = &TCCR1C;
            TIMSK = &TIMSK1;
            TCNT  = &TCNT1L;
            OCRA  = &OCR1AL;
            OCRB  = &OCR1BL;
            ICR   = &ICR1;
            break;
    case 2: TCCRA = &TCCR2A;
            TCCRB = &TCCR2B;
            TIMSK = &TIMSK2;
            TCNT  = &TCNT2;
            OCRA  = &OCR2A;
            OCRB  = &OCR2B;
            break;
#if defined(__AVR_ATmega2560__)
    case 3: TCCRA = &TCCR3A;
            TCCRB = &TCCR3B;
            TCCRC = &TCCR3C;
            TIMSK = &TIMSK3;
            TCNT  = &TCNT3L;
            OCRA  = &OCR3AL;
            OCRB  = &OCR3BL;
            ICR   = &ICR3;
            break;
    case 4: TCCRA = &TCCR4A;
            TCCRB = &TCCR4B;
            TCCRC = &TCCR4C;
            TIMSK = &TIMSK4;
            TCNT  = &TCNT4L;
            OCRA  = &OCR4AL;
            OCRB  = &OCR4BL;
            ICR   = &ICR4;
            break;
    case 5: TCCRA = &TCCR5A;
            TCCRB = &TCCR5B;
            TCCRC = &TCCR5C;
            TIMSK = &TIMSK5;
            TCNT  = &TCNT5L;
            OCRA  = &OCR5AL;
            OCRB  = &OCR5BL;
            ICR   = &ICR5;
            break;
#endif
    default:TIMER = 1;                                // Если не верный номер таймера то Timer1                         
            TCCRA = &TCCR1A;
            TCCRB = &TCCR1B;
            TCCRC = &TCCR1C;
            TIMSK = &TIMSK1;
            TCNT  = &TCNT1L;
            OCRA  = &OCR1AL;
            OCRB  = &OCR1BL;
            ICR   = &ICR1;
  }
}

void Timer::outputState(uint8_t channel, bool state) {
  switch (channel) {
  case CHANNEL_A:
    *TCCRB = (*TCCRB & 0x7F) | (state << FOCA);
    return;
  case CHANNEL_B:
    *TCCRB = (*TCCRB & 0xBF) | (state << FOCB);
    return;
  case CHANNEL_C:
    if(TIMER == 1 || TIMER >= 3)
      *TCCRC = (*TCCRC & 0xDF) | (state << FOCC);
    return;
  }
}

void Timer::disableISR(uint8_t source = CHANNEL_A) {  // Disable timer interrupt , channel A , B or C
  switch (source) {
  case CHANNEL_A:
    *TIMSK &= ~ (1 << OCIEA);
    return;
  case CHANNEL_B:
    *TIMSK &= ~ (1 << OCIEB);
    return;
#if defined(__AVR_ATmega2560__)
  case CHANNEL_C:
    *TIMSK &= ~ (1 << OCIEC);
    return;
#endif
  }
}

void Timer::setDefault(void) { 
  switch (TIMER){
    case 0  : *TCCRA = 0x03;                        // Fast PWM , 8 bit
              *TCCRB = 0x03;                        // Prescaler /64
              break;
    case 2  : *TCCRA = 0x01;                        // Phasecorrect PWM , 8 bit
              *TCCRB = 0x04;                        // Prescaler /64
              break;
    default : *TCCRA = 0x01;                        // Phasecorrect PWM , 8 bit
              *TCCRB = 0x0B;                        // Prescaler /64
              *(OCRB+1) = 0x00;                     // Clear COMPA
              *(OCRA+1) = 0x00;                     // Clear COMPB
              *(TCNT+1) = 0x00;                     // Clear counter
  }
  *OCRB = 0x00;                                     // Clear COMPA
  *OCRA = 0x00;                                     // Clear COMPB
  *TCNT = 0x00;                                     // Clear counter
}

void Timer::enableISR(uint8_t source = CHANNEL_A , uint16_t phase = 0x00) {   //.enableISR() = CHANNEL_A , phase = 0;
  union{uint16_t w; uint8_t b[2];} ph;
  if (TIMER == 0 || TIMER == 2){
    if (!source){ 
      *TIMSK |= (1 << OCIEA);                       // Channel A , interrupt enable
    }else {                                         // Channel B
      *TIMSK |= (1 << OCIEB);                       // Interrupt enable
      *OCRB = map(phase, 0, 360, 0, *OCRA);         // Convert 0...360 degrees to 0...timer top (channel B only)
    }
  }
  else {
    ph.w = map(phase, 0, 360, 0, *ICR);             // Convert 0...360 degrees to 0...timer top
    switch (source) {
    case CHANNEL_A:
      *TIMSK |= (1 << OCIEA);                       // Interrupt enable
      *OCRA = ph.b[0];
      *(OCRA+1) = ph.b[1];
      return;
    case CHANNEL_B:
      *TIMSK |= (1 << OCIEB);                       // Interrupt enable
      *OCRB = ph.b[0];
      *(OCRB+1) = ph.b[1];
      return;
#if defined(__AVR_ATmega2560__)
    case CHANNEL_C:
      *TIMSK |= (1 << OCIEC);                       // Interrupt enable
      *OCRC = ph.b[0];
      *(OCRC+1) = ph.b[1];
      return;
#endif
    }
  }
}

void Timer::outputEnable(uint8_t channel, uint8_t mode) {
  switch (channel) {
  case CHANNEL_A:
    *TCCRA = (*TCCRA & 0x3F) | (mode << 6);         // set mode bits 
    return;
  case CHANNEL_B:
    *TCCRA = (*TCCRA & 0xCF) | (mode << 4);
    return;
#if defined(__AVR_ATmega2560__)
  case CHANNEL_C:
    *TCCRA = (*TCCRA & 0xF3) | (mode << 2);
    return;
#endif
  }
}

void Timer::outputDisable(uint8_t channel) {
  switch (channel) {
  case CHANNEL_A:
    *TCCRA = (*TCCRA & 0x3F);                       // disable output from timer
    return;
  case CHANNEL_B:
    *TCCRA = (*TCCRA & 0xCF);
    return;
#if defined(__AVR_ATmega2560__)
  case CHANNEL_C:
    *TCCRA = (*TCCRA & 0xF3);
    return;
#endif
  }
}

uint32_t Timer::setPeriod(uint32_t timer_period)  {
  uint32_t timer_cycles;
  uint16_t timer_divider = 1;
  uint16_t timer_top;
  uint8_t  timer_prescaler = 1;
  uint32_t x;
  switch (TIMER){
    case 0  :   timer_period = constrain(timer_period, 1, 256*1024/(F_CPU / 1e6)); // for 16Mhz max = 16384
                timer_cycles = F_CPU / 1e6 * timer_period;
                x = timer_cycles / 256;
                while (x){  // Сhoose optimal divider for the Timer0
                  timer_divider <<= 3 - (++timer_prescaler >> 2);
                  x >>= 3 - (timer_prescaler >> 2);
                }
                break;
                
    case 2  :   timer_period = constrain(timer_period, 1, 256*1024/(F_CPU / 1e6) ); // for 16Mhz max = 16384
                timer_cycles = F_CPU / 1e6 * timer_period;
                x = timer_cycles / 256;
                while (x){  // Сhoose optimal divider for the Timer2
                  timer_divider <<= (timer_prescaler * timer_prescaler >> 2) - (timer_prescaler << 1) +17;
                  x >>= (timer_prescaler * timer_prescaler >> 2) - (timer_prescaler << 1) +17;                 // x = x >> x^2/4-2x+17
                  ++timer_prescaler;
                }
                break;
#if defined(__AVR_ATmega2560__)
    case 3  :   
    case 4  :   
    case 5  :
#endif                
    case 1  :   timer_period = constrain(timer_period, 1, 65536*1024/(F_CPU / 1e6));  // for 16Mhz max = 4194304
                timer_cycles = F_CPU / 1e6 * timer_period;
                x = timer_cycles / 65536;
                while (x){  // Сhoose optimal divider for the Timer1
                  timer_divider <<= 3 - (++timer_prescaler >> 2);
                  x >>= 3 - (timer_prescaler >> 2);
                }
  }
  timer_top = timer_cycles / timer_divider;
  if (TIMER == 0 || TIMER == 2){
    *TCCRA = (*TCCRA & 0xF0)|(1 << WGM1);                         // CTC - mode
    *TCCRB = timer_prescaler;                                     // Set timer prescaler
    *OCRA  = timer_top - 1;                                       // Set timer top
  }
  else{
    #if defined(__AVR_ATmega2560__)
      *TCCRA = (*TCCRA & 0xFC);
    #else
      *TCCRA = (*TCCRA & 0xF0);
    #endif
    *TCCRB = ((1 << WGM3) | (1 << WGM2) | timer_prescaler);      // CTC mode + set prescaler
    *ICR = timer_top - 1;                                        // Set timer top
  }
  return (1e6 / ((F_CPU / timer_divider) / timer_top));          // Return real timer period
}

////////////////////////////// objects //////////////////////////////

// Это здесь для полной совместимости с предыдущей версией,
// а по уму, объекты надо в программе по мере надобности создавать, 
// только в классе нужно проверку устроить, чтобы по одному объекту на таймер, и не больше :-)

Timer Timer0 = Timer(0);
Timer Timer1 = Timer(1);
Timer Timer2 = Timer(2);

#if defined(__AVR_ATmega2560__)
Timer Timer3 = Timer(3);
Timer Timer4 = Timer(4);
Timer Timer5 = Timer(5);
#endif
