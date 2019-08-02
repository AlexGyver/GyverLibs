#ifndef directTimers_h
#define directTimers_h
#include <Arduino.h>

/* 	directTimers lib , created for 'Alex Gyver' 23/07/2019 , by Egor 'Nich1con' Zaharov. 
	Библиотека для расширенного (ручного) управления таймерами ATmega328
	Функции библиотеки позволяют получить доступ ко всем возможностям и режимам работы с таймерами/счётчиками + прерывания watchdog
	Ничего не урезано и не упрощено, доступен весь описанный в даташите функционал
*/

// данная библиотека даст тебе возможность полного контроля над всеми таймерами МК ATMega328
// прерывания wdt полностью совместимы со всеми AVR МК (ATMega2560,ATMega328,ATMega32u4)
// для ATMega2560  остается совместимость прерываний по таймеру и ШИМ, однако не будут работать каналы "C"
// у ATMega32u4 нет таймера 2

/* константы для TIMERn_clock() */
#define STOPPED 0  //- тактирование будет остановлено , таймер "заморожен"
#define EXTERNAL_FALLING 1 //-тактирование внешним клоком до 8 мгц по спаду
#define EXTERNAL_RISING 2 //-тактирование внешним клоком до 8 мгц по фронту
#define PRESCALER_1 3 // делитель 1
#define PRESCALER_8 4 // делитель 8
#define PRESCALER_32 5 // и тд
#define PRESCALER_64 6
#define PRESCALER_128 7
#define PRESCALER_256 8
#define PRESCALER_1024 9
/* константы для настройки режима работы таймеров */
#define STANDARD_MODE 10 // таймер считает до 255 и сбрасывается в 0, (может до 511/1023 для таймера 1)
#define CTC_MODE 11 // таймер считает от 0 до числа заданного функцией TIMERn_COMPA_setValue(), после чего сбрасывается (в этот момент может вызват прерывание)
#define FAST_PWM_8BIT 12 // аппаратный шим 8 бит
#define FAST_PWM_9BIT 13 // для таймера 1
#define FAST_PWM_10BIT 14 
#define PHASECORRECT_PWM_8BIT 15 // шим с коррекцией
#define PHASECORRECT_PWM_9BIT 16
#define PHASECORRECT_PWM_10BIT 17
#define FAST_PWM_CUSTOM 18 // шим с кастомной глубиной, глубина задается TIMER0/2_COMPA_setValue() и TIMER1_setTop();
#define PHASECORRECT_PWM_CUSTOM 19 // то же самое но с коррекцией
/* константы для управления аппаратными выходами с таймеров */
#define DISABLE_COMP 20 // выход отключен от ноги, ногой можно пользоваться
#define NORM_PWM 21 // выход генерирует шим >>> +5В при сбросе таймера, 0В при совпадении
#define INVERT_PWM 22 // выход генерирует инвертированный шим >>> 0В при сбросе таймера, +5В при совпадении
#define TOGGLE_PIN 23 // выход генерирует меандр >>> инверсия состояния пина при совпадении

#define ISR_T0_OVF		ISR(TIMER0_OVF_vect)
#define ISR_T0_COMPA	ISR(TIMER0_COMPA_vect)
#define ISR_T0_COMPB	ISR(TIMER0_COMPB_vect)
#define ISR_T1_OVF		ISR(TIMER1_OVF_vect)
#define ISR_T1_COMPA	ISR(TIMER1_COMPA_vect)
#define ISR_T1_COMPB	ISR(TIMER1_COMPB_vect)
#define ISR_T2_OVF		ISR(TIMER2_OVF_vect)
#define ISR_T2_COMPA	ISR(TIMER2_COMPA_vect)
#define ISR_T2_COMPB	ISR(TIMER2_COMPB_vect)
#define ISR_WDT			ISR(WDT_vect)

void TIMER2_attach_OVF(void);
void TIMER2_attach_COMPA(void);
void TIMER2_attach_COMPB(void);
void TIMER2_detach_COMPA(void);
void TIMER2_detach_COMPB(void);
void TIMER2_detach_OVF(void);
void TIMER2_setClock(byte clk);
void TIMER2_setMode(byte mode);
void TIMER2_COMPA_mode(byte mode);
void TIMER2_COMPB_mode(byte mode);
byte TIMER2_getCounter(void);
void TIMER2_setCounter(byte value);
void TIMER2_COMPA_setValue(byte value);
void TIMER2_COMPB_setValue(byte value);

void TIMER1_attach_OVF(void);
void TIMER1_attach_COMPA(void);
void TIMER1_attach_COMPB(void);
void TIMER1_detach_COMPA(void);
void TIMER1_detach_COMPB(void);
void TIMER1_detach_OVF(void);
void TIMER1_setClock(byte clk);
void TIMER1_setMode(byte mode);
void TIMER1_COMPA_mode(byte mode);
void TIMER1_COMPB_mode(byte mode);
void TIMER1_setTop(unsigned int top);
unsigned int TIMER1_getCounter(void);
void TIMER1_setCounter(unsigned int value);
void TIMER1_COMPA_setValue(unsigned int value);
void TIMER1_COMPB_setValue(unsigned int value);

void TIMER0_attach_COMPA(void);
void TIMER0_attach_COMPB(void);
void TIMER0_detach_COMPA(void);
void TIMER0_detach_COMPB(void);
void TIMER0_setClock(byte clk);
void TIMER0_setMode(byte mode);
void TIMER0_COMPA_mode(byte mode);
void TIMER0_COMPB_mode(byte mode);
byte TIMER0_getCounter(void);
void TIMER0_setCounter(byte value);
void TIMER0_COMPA_setValue(byte value);
void TIMER0_COMPB_setValue(byte value);

void WDT_attachInterrupt(int prescaler);
void WDT_detachInterrupt(void);

#endif