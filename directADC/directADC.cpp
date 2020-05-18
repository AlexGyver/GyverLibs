#include "directADC.h"

/* directTimers lib , created for 'Alex Gyver' 23/07/2019 , by Egor 'Nich1con' Zaharov. */

/* данная библиотека даст тебе возможность полного контроля над АЦП и аналоговым компаратором МК ATMegADC_A328 */
/* параметры помеченные знаком "default" вступают в силу после сброса МК до вызова соответствующих функций */

/* сервисные указатели на функции прерываний */
void (*adc_isr)();
void (*acomp_isr)();

/* выбор аналог входа */
void setAnalogMux(ADC_modes mux) {
	cli();
	switch (mux) {
	case ADC_A0: // ADC_A0 (default)
		ADMUX &= ~ ((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
		break;
	case ADC_A1: //ADC_A1
		ADMUX &= ~ ((1 << MUX3) | (1 << MUX2) | (1 << MUX1));
		ADMUX |= (1 << MUX0);
		break;
	case ADC_A2: //ADC_A2
		ADMUX &= ~ ((1 << MUX3) | (1 << MUX2) | (1 << MUX0));
		ADMUX |= (1 << MUX1);
		break;
	case ADC_A3: //ADC_A3
		ADMUX &= ~ ((1 << MUX3) | (1 << MUX2));
		ADMUX |= ((1 << MUX1) | (1 << MUX0));
		break;
	case ADC_A4: //ADC_A4
		ADMUX &= ~ ((1 << MUX3) | (1 << MUX1) | (1 << MUX0));
		ADMUX |= (1 << MUX2);
		break;
	case ADC_A5: //ADC_A5
		ADMUX &= ~ ((1 << MUX3) | (1 << MUX1));
		ADMUX |= ((1 << MUX2) | (1 << MUX0));
		break;
	case ADC_A6: //ADC_A6
		ADMUX &= ~ ((1 << MUX3) | (1 << MUX0));
		ADMUX |= ((1 << MUX2) | (1 << MUX1));
		break;
	case ADC_A7: //ADC_A7
		ADMUX &= ~ (1 << MUX3);
		ADMUX |= ((1 << MUX2) | (1 << MUX1) | (1 << MUX0));
		break;
	case ADC_SENSOR: //ADC_SENSOR - термодатчик
		ADMUX &= ~ ((1 << MUX2) | (1 << MUX1) | (1 << MUX0));
		ADMUX |= (1 << MUX3);
		break;
	case ADC_1V1: //ADC_1V1 - внутреннее опорное 1.1В 
		ADMUX &= ~ (1 << MUX0);
		ADMUX |= ((1 << MUX3) | (1 << MUX2) | (1 << MUX1));
		break;
	case ADC_GND: //ADC_GND - подключить выход ацп к земле
		ADMUX |= ((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
		break;
	}
	sei();
}


void ADC_enable() { // включить ацп
	cli();
	ADCSRA |= (1 << ADEN); // set adc enable
	sei();
}

void ADC_disable() { // выключить ацп (default)
	cli();
	ADCSRA &= ~ (1 << ADEN); // clear adc enable 
	sei();
}

/* выбор делителя частоты тактирования АЦП */
void ADC_setPrescaler(byte prescl) { 
	cli();
	switch (prescl) {
	case 2: // (defalut)
		ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
		break;
	case 4:
		ADCSRA &= ~((1 << ADPS2) | (1 << ADPS0));
		ADCSRA |= (1 << ADPS1);
		break;
	case 8:
		ADCSRA &= ~ (1 << ADPS2);
		ADCSRA |= ((1 << ADPS1) | (1 << ADPS0));
		break;
	case 16:
		ADCSRA &= ~((1 << ADPS1) | (1 << ADPS0));
		ADCSRA |= (1 << ADPS2);
		break;
	case 32:
		ADCSRA &= ~ (1 << ADPS1);
		ADCSRA |= ((1 << ADPS2) | (1 << ADPS0));
		break;
	case 64:
		ADCSRA &= ~ (1 << ADPS0);
		ADCSRA |= ((1 << ADPS2) | (1 << ADPS1));
		break;
	case 128:
		ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
		break;
	}
	sei();
}

/* Выбор опорного напряжения АЦП */
void ADC_setReference(ADC_modes ref) {
	cli();
	switch (ref) {
	case ADC_1V1: // 1v1
		ADMUX |= ((1 << REFS1) | (1 << REFS0));
		break;
	case ADC_AREF: // ADC_AREF (default)
		ADMUX &= ~((1 << REFS1) | (1 << REFS0));
		break;
	case ADC_VCC: // ADC_VCC
		ADMUX &= ~(1 << REFS1);
		ADMUX |= (1 << REFS0);
		break;
	}
	sei();
}

void ADC_setResolution(uint8_t res)
{
	cli();
	switch(res)
	{
		case 10:
			ADMUX &= ~(1<<ADLAR); // right adjustment
			break;
		case 8:
			ADMUX |= (1<<ADLAR); //left adjustment
			break;
	}
	sei();
}

/* Включить режим автозапуска АЦП и выбрать его источник. Внимание! Запуск преобразования начинается по ФРОНТУ (RISING) события */
void ADC_autoTriggerEnable(ADC_modes trig) {
	cli();
	ADCSRA |= (1 << ADATE); // autotrigger en
	switch (trig) {
	case FREE_RUN: // free run (default)
		ADCSRB &= ~ ((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));
		break;
	case ANALOG_COMP: // analog_comp
		ADCSRB &= ~ ((1 << ADTS2) | (1 << ADTS1));
		ADCSRB |= (1 << ADTS0);
		break;
	case ADC_INT0: // ADC_INT0
		ADCSRB &= ~ ((1 << ADTS2) | (1 << ADTS0));
		ADCSRB |= (1 << ADTS1);
		break;
	case TIMER0_COMPA: // t0_compa
		ADCSRB &= ~ (1 << ADTS2);
		ADCSRB |= ((1 << ADTS1) | (1 << ADTS0));
		break;
	case TIMER0_OVF: // t0_ovf
		ADCSRB &= ~ ((1 << ADTS1) | (1 << ADTS0));
		ADCSRB |= (1 << ADTS2);
		break;
	case TIMER1_COMPB: // t1_compb
		ADCSRB &= ~ (1 << ADTS1);
		ADCSRB |= ((1 << ADTS2) | (1 << ADTS0));
		break;
	case TIMER1_OVF: // t1_ovf
		ADCSRB &= ~ (1 << ADTS0);
		ADCSRB |= ((1 << ADTS2) | (1 << ADTS1));
		break;
	}
	sei();
}

void ADC_autoTriggerDisable() { // выключить автозапуск АЦП (defalut);
	cli();
	ADCSRA &= ~ (1 << ADATE); 
	sei();
}

void ADC_attachInterrupt(void (*isr)()) { // подключить прерывание готовности АЦП
	cli();
	adc_isr = *isr;
	ADCSRA |= (1 << ADIE); 
	sei();
}

void ADC_detachInterrupt() { // выключить прерывание готовности АЦП (default)
	cli();
	ADCSRA &= ~ (1 << ADIE); 
	sei();
}

void ADC_startConvert() { // ручной запуск преобразования
	cli();
	ADCSRA |= (1 << ADSC);
	sei();
}

unsigned int ADC_read() { // вернуть значение АЦП
	return ADC;
}

uint8_t ADC_read8(void) { // вернуть 8-битное значение АЦП
	return ADCH;
}

boolean ADC_available() { // проверить готовность АЦП
	if (ADCSRA & (1 << ADSC)) { // if flag set - convert in process
		return false;
	}
	else { // convert complete
		return true;
	}
}

unsigned int ADC_readWhenAvailable() { // дождаться окончания текущего преобразования,склеить и вернуть результат
	while (ADCSRA & (1 << ADSC));
	return ADC; 
}

uint8_t ADC_read8WhenAvailable(void) {
	while (ADCSRA & (1 << ADSC));
	return ADCH;
}

ISR(ADC_vect){ // прерывание ацп
	(*adc_isr)();
}

void ACOMP_attachInterrupt(void (*isr)(), ADC_modes source) { // подключить прерывание ацп и выбрать условие вызова
	cli();
	acomp_isr = *isr;
	ACSR |= (1 << ACIE); // set int bit
	switch (source) {
	case FALLING_TRIGGER: // falling
		ACSR &= ~ (1 << ACIS0);
		ACSR |= (1 << ACIS1);
		break;
	case RISING_TRIGGER: // rising
		ACSR |=  ((1 << ACIS1) | (1 << ACIS0));
		break;
	case CHANGE_TRIGGER: // change (default)
		ACSR &= ~ ((1 << ACIS1) | (1 << ACIS0));
		break;
	}
	sei();
}

void ACOMP_detachInterrupt() { // отключить прерывания компаратора (default)
	cli();
	ACSR &= ~ (1 << ACIE); 
	sei();
}

void ACOMP_enable() { // включить компаратор после его отключения (default)
	cli();
	ACSR &= ~ (1 << ACD); 
	sei();
}

void ACOMP_disable() { // выключить компаратор
	cli();
	ACSR |= (1 << ACD); // set bit "analog comp disable"
	sei();
}

boolean ACOMP_read() { // прочитать значение на выходе компаратора
	return ACSR & (1 << ACO); 
}

void ACOMP_setPositiveInput(ADC_modes in) { // настроить прямой вход компаратора
	cli();
	switch (in) {
	case ADC_1V1: // 1v1 - подключить его к источнику 1.1В
		ACSR |= (1 << ACBG); // connect 1v1 to +in
		break;
	case ADC_AIN0: // ADC_AIN0 (default) - отключить от источника 1.1В
		ACSR &= ~ (1 << ACBG); // disconnect 1v1
		break;
	}
	sei();
}

void ACOMP_setNegativeInput(ADC_modes in) { // настроить инверсный вход компаратора
	cli();
	switch (in) {
	case ADC_AIN1: // ADC_AIN1 (default) - отключить выход от мультиплексора АЦП
		ADCSRB &= ~ (1 << ACME); // disable -in from mux
		break;
	case ANALOG_MUX: // analog MUX - подключить выход к мультиплексору АЦП
		ADCSRB |= (1 << ACME); // connetct -in to analog mux
		break;
	}
	sei();
}


ISR(ANALOG_COMP_vect){ // прерывание компаратора
	(*acomp_isr)();
}