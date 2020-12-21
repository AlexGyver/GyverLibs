#ifndef directADC_h
#define directADC_h
#include <Arduino.h>

/* 	directADC, created for 'Alex Gyver' 23/07/2019 , by Egor 'Nich1con' Zaharov.
	Библиотека для расширенного (ручного) управления АЦП и компаратором ATmega328
	Функции библиотеки позволяют получить доступ ко всем возможностям и режимам работы с АЦП
	Ничего не урезано и не упрощено, доступен весь описанный в даташите функционал
	v1.1 - добавлены функции чтения 8 бит (спасибо Vitve4) https://github.com/AlexGyver/GyverLibs/pull/43
*/

enum ADC_modes {
	/* For setAnalogMux() */
	ADC_A0,
	ADC_A1,
	ADC_A2,
	ADC_A3,
	ADC_A4,
	ADC_A5,
	ADC_A6,
	ADC_A7,
	ADC_SENSOR,
	ADC_1V1,
	ADC_GND,
	/* For autoTriggerEnable() */
	FREE_RUN,
	ANALOG_COMP,
	ADC_INT0,
	TIMER0_COMPA,
	TIMER0_OVF,
	TIMER1_COMPB,
	TIMER1_OVF,
	/* For ACOMP_setPositiveInput()*/
	ADC_AIN0,
	// + ADC_1V1
	/* For ACOMP_setNegativeInput()*/
	ADC_AIN1,
	ANALOG_MUX,
	/*For ACOMP_attachInterrupt()*/
	FALLING_TRIGGER,
	RISING_TRIGGER,
	CHANGE_TRIGGER,
	/* For ADC_setReference()*/
	ADC_AREF,
	ADC_VCC,
	// + ADC_1V1	
};

extern void (*adc_isr)();
extern void (*acomp_isr)();

void setAnalogMux(ADC_modes mux);		// Аналоговый вход (ADC_A0-ADC_A7)/ термодатчик (ADC_SENSOR)/ 1.1V (ADC_1V1)/ ADC_GND (default: ADC_A0)
void ADC_enable(void);  				// Включить АЦП 
void ADC_disable(void);   				// Выключить АЦП (default)
void ADC_setPrescaler(byte prescl);   	// Выбрать делитель частоты АЦП (2, 4, 8, 16, 32, 64, 128) // (default: 2)
void ADC_setReference(ADC_modes ref);	// Выбрать источник опорного напряжения АЦП (ADC_1V1, ADC_AREF, ADC_VCC) // (default: ADC_AREF)
void ADC_setResolution(uint8_t res);  // Выбрать разрядность АЦП (8, 10) // (default: 10)
void ADC_autoTriggerEnable(ADC_modes trig); 	// Включить автозапуск АЦП и выбрать событие (FREE_RUN, ANALOG_COMP, ADC_INT0, TIMER0_COMPA, TIMER0_OVF, TIMER1_COMPB, TIMER1_OVF)
void ADC_autoTriggerDisable(void);   	// Выключить автозапуск АЦП // (default)
void ADC_attachInterrupt(void (*isr)());// Включить прерывание готовности АЦП и выбрать функцию, которая будет при этом выполняться
void ADC_detachInterrupt(void);			// Выключить прерывание готовности АЦП // (default) 
void ADC_startConvert(void);			// Ручной запуск преобразования
unsigned int ADC_read(void);			// Прочитать значение регистров АЦП (Вызов до окончания преобразования вернет неверный результат)
uint8_t ADC_read8(void);				// То же самое что ADC_read(), но возвращает 8-битный результат
boolean ADC_available(void);			// Проверить готовность преобразования АЦП
unsigned int ADC_readWhenAvailable(void);	// Дождаться окончания текущего преобразования и вернуть результат
uint8_t ADC_read8WhenAvailable(void);	// То же самое что ADC_readWhenAvailable(), но возвращает 8-битный результат
void ACOMP_attachInterrupt(void (*isr)(), ADC_modes source); // Включить прерывание компаратора и выбрать при каком событии оно будет вызвано (FALLING_TRIGGER, RISING_TRIGGER, CHANGE_TRIGGER)
void ACOMP_detachInterrupt(void); 		// Выключить прерывание компаратора // (default)
void ACOMP_enable(void); 				// Включить компаратор // (default: Включен)
void ACOMP_disable(void); 				// Принудительно выключить компаратор
boolean ACOMP_read(void); 				// Прочитать значение на выходе компаратора 
void ACOMP_setPositiveInput(ADC_modes in);	// Настроить куда подкл +Вход компаратора (ADC_1V1, ADC_AIN0) (default: ADC_AIN0 - pin 6) 
void ACOMP_setNegativeInput(ADC_modes in);	// Настроить куда подкл -Вход компаратора (ADC_AIN1, ANALOG_MUX) (default: ADC_AIN1 - pin 7)

// делитель 2: 3.04 мкс (частота оцифровки 329 000 Гц)
// делитель 4: 4.72 мкс (частота оцифровки 210 000 Гц)
// делитель 8: 8.04 мкс (частота оцифровки 125 000 Гц)
// делитель 16: 15.12 мкс (частота оцифровки 66 100 Гц)
// делитель 32: 28.04 мкс (частота оцифровки 35 600 Гц)
// делитель 64: 56.04 мкс (частота оцифровки 17 800 Гц)													
// делитель 128: 112 мкс (частота оцифровки 8 900 Гц)
#endif
