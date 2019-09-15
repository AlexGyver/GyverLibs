#ifndef GyverWDT_h
#define GyverWDT_h
#include <Arduino.h>

/* библиотека для расширенной и удобной работы с watchdog.
* created for AlexGyver by Egor 'Nich1con' Zaharov 10.08.2019
* MCU supported : ATmega328/168 , ATmega32u4/16u4 , ATmega2560/1280 , ATtiny85/45 , ATtiny88/48 ...
*/

/* О прескалерах и режимах работы watchdog таймера:
* 1) Осцилятор сторжевого таймера не предполагает высокой точности и имеет отклонение при производстве около 10%
* 2) Приблизительное соотношение делителей и таймаутов:
* [/2 ~ 16ms   /4 ~ 32ms  /8 ~ 64ms  /16 ~ 0.125s  /32 ~ 0.25s  /64 ~ 0.5s  /128 ~ 1s  /256 ~ 2s  /512 ~ 4s  /1024 ~ 8s ]
* 3) Описание режимов работы сторжевого таймера:
* - INTERRUPT_MODE  => Таймаут инициирует вызов прерывания , работает на любом bootloader'e 
* - RESET_MODE  => Таймаут инициирует сброс микроконтроллера , работает НЕ со всеми загрузчиками , для проверки воспользуйтесь примером "watchdog_support_test"
* - INTERRUPT_RESET_MODE  => Первый таймаут инициирует прерывание,и автоматически переводит watchdog в режим "RESET_MODE",
*  соотв. последующий таймаут иницирует сброс. Если в вызванном прерывании удалось побороть причину зависания,
*  вы можете перенастроить сторжевой таймер конструкцией [watchdog_disable() + watchdog_enable()],чтобы следующий таймаут не инициировал сброс.
*/
// версия 1.1 от 11.09.2019 - чуть улучшена стабильность
// версия 1.2 от 15.09.2019 - почищен код

/* варианты предделителей для watchdog'a */
#define WDT_PRESCALER_2		0
#define WDT_PRESCALER_4		1
#define WDT_PRESCALER_8		2
#define WDT_PRESCALER_16	3
#define WDT_PRESCALER_32	4
#define WDT_PRESCALER_64	5
#define WDT_PRESCALER_128	6
#define WDT_PRESCALER_256	7
#define WDT_PRESCALER_512	8
#define WDT_PRESCALER_1024	9

/* Режимы работы watchdog'a */
#define INTERRUPT_MODE			0 // вызывает прерывание при таймауте
#define INTERRUPT_RESET_MODE	1  // вызывает прерывание при таймауте, следующий таймаут инициирует сброс

/* указатель на функцию прерывания */
extern void (*isr_wdt)();

/* Все функции библиотеки */
void watchdog_reset(void); // сбросить watchdog 
void watchdog_disable(void); // полностью выключить watchdog 
void watchdog_enable(uint8_t mode , uint8_t  prescaler, void (*isr)()); // Включить и настроить ватчдог (с прерыванием)
void watchdog_enable(uint8_t prescaler); // Включить и настроить ватчдог (только reset)

#endif