#include "directTimers.h"

/* directTimers lib , created for 'Alex Gyver' 23/07/2019 , by Egor 'Nich1con' Zaharov. */

/* данная библиотека даст тебе возможность полного контроля над всеми таймерами МК ATMega328 */
/* прерывания wdt полностью совместимы со всеми AVR МК (ATMega2560,ATMega328,ATMega32u4)*/
/* для ATMega2560  остается совместимость прерываний по таймеру и ШИМ, однако не будут работать каналы "C" */
/* у ATMega32u4 нет таймера 2 */ 

// ============== Timer 1 ==============

/* функция установки тактирования таймера */
void TIMER0_setClock(byte clk) { 
	switch (clk) {
	case 0: // stop - тактирование откл
		TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
		break;
	case 1: // ext_fall - внешний источник, подкл к pin 4 (до 1/2 частоты МК), тактирование по спаду
		TCCR0B &= ~ (1 << CS00);
		TCCR0B |= ((1 << CS02) | (1 << CS01));
		break;
	case 2: // ext_rise - внешний источник, подкл к pin 4 (до 1/2 частоты МК), тактирование по фронту
		TCCR0B |= ((1 << CS02) | (1 << CS01) | (1 << CS00));
		break;
	case 3: // 1 - далее тактирование от системной частоты (8/16 мгц) с делителем
		TCCR0B &= ~((1 << CS02) | (1 << CS01));
		TCCR0B |= (1 << CS00);
		break;
	case 4: // 8 - делитель 8
		TCCR0B &= ~((1 << CS02) |  (1 << CS00));
		TCCR0B |= (1 << CS01);
		break;
	case 6: //64 - делитель 64 и тд.
		TCCR0B &= ~ (1 << CS02);
		TCCR0B |= ((1 << CS01) | (1 << CS00));
		break;
	case 8: //256
		TCCR0B &= ~((1 << CS01) |  (1 << CS00)); // сброс бит делителя 
		TCCR0B |= (1 << CS02); // установка бита
		break;
	case 9: // 1024
		TCCR0B &= ~ (1 << CS01);
		TCCR0B |= ((1 << CS02) | (1 << CS00));
		break;
	}
}


/* функция установки режима работы таймера */
void TIMER0_setMode(byte mode) { 
	switch (mode) {
	case 10: // normal - обычный счет от 0 до 255
		TCCR0B &= ~ (1 << WGM02); // top = 255
		TCCR0A &= ~((1 << WGM01) | (1 << WGM00)); // pwm & correct disable
		break;
	case 11: // ctc до OCR0A - счет до выбранного числа , функцией  TIMER0_COMPA_setValue()
		TCCR0B &= ~ (1 << WGM02);
		TCCR0A &= ~ (1 << WGM00); // pwm disable
		TCCR0A |= (1 << WGM01); // top = OCR0A
		break;
	case 12: // fast_pwm_8bit - аппаратный шим 8 бит
		TCCR0B &= ~ (1 << WGM02); // top = 255
		TCCR0A |= ((1 << WGM01) | (1 << WGM00)); // pwm en , correct disable
		break;
	case 15: // phasecorrect_pwm_8bit - аппаратный шим с коррекцией 8 бит
		TCCR0B &= ~ (1 << WGM02); // top = 255
		TCCR0A &= ~ (1 << WGM01); // correct en
		TCCR0A |= (1 << WGM00); // pwm en
		break;
	case 18: // fast_pwm_custom - аппаратный шим с глубиной, заданой функцией TIMER0_COMPA_setValue() , меняет разрядность и соотв частоту
		TCCR0B |= (1 << WGM02); // top = OCR0A
		TCCR0A |= ((1 << WGM01) | (1 << WGM00)); // pwm en , correct disable
		break;
	case 19: // phasecorrect_pwm_custom - то же что и выше, но с коррекцией.
		TCCR0B |= (1 << WGM02); // top = OCR0A 
		TCCR0A &= ~ (1 << WGM01); // correct en
		TCCR0A |= (1 << WGM00); // pwm en
		break;
	}
}


/* функция установки режима работы аппаратного выхода с таймера А */
void TIMER0_COMPA_mode(byte mode) { 
	switch (mode) {
	case 20: // disable - нога откл от аппаратного выхода,может делать что угодно,принимает значение последнего digitalWrite 
		TCCR0A &= ~ ((1 << COM0A1) | (1 << COM0A0));
		break;
	case 21: // pwm - прямой шим => на выходе: +5В при обнулении таймера, 0В как только таймер досчитает до числа TIMER0_COMPA_setValue()
		TCCR0A &= ~ (1 << COM0A0);
		TCCR0A |= (1 << COM0A1);
		TCCR0B &=~(1<<FOC0A);
		break;
	case 22: // invert_pwm - обратный шим => на выходе: 0В при обнулении таймера, +5В как только таймер досчитает до числа TIMER0_COMPA_setValue()
		TCCR0A |= ((1 << COM0A1) | (1 << COM0A0));
		TCCR0B &=~(1<<FOC0A);
		break;
	case 23: // toggle - используется для генерации меандра, меняет состояние пина на противоположное, как только досчитает до TIMER0_COMPA_setValue()
		TCCR0A &= ~ (1 << COM0A1);
		TCCR0A |= (1 << COM0A0);
		TCCR0B |=(1<<FOC0A); // прямой вывод с таймера 
		break;
	}
}

/* функция установки режима работы аппаратного выхода с таймера Б */
void TIMER0_COMPB_mode(byte mode) { 
	switch (mode) {
	case 20: // disable - нога откл от аппаратного выхода,может делать что угодно,принимает значение последнего digitalWrite 
		TCCR0A &= ~ ((1 << COM0B1) | (1 << COM0B0));
		break;
	case 21: // pwm - прямой шим => на выходе: +5В при обнулении таймера, 0В как только таймер досчитает до числа TIMER0_COMPB_setValue()
		TCCR0A &= ~ (1 << COM0B0);
		TCCR0A |= (1 << COM0B1);
		TCCR0B &=~(1<<FOC0B);
		break;
	case 22: // invert_pwm - обратный шим => на выходе: 0В при обнулении таймера, +5В как только таймер досчитает до числа TIMER0_COMPB_setValue()
		TCCR0A |= ((1 << COM0B1) | (1 << COM0B0));
		TCCR0B &=~(1<<FOC0B);
		break;
	case 23: // toggle - используется для генерации меандра, меняет состояние пина на противоположное, как только досчитает до TIMER0_COMPB_setValue()
		TCCR0A &= ~ (1 << COM0B1);
		TCCR0A |= (1 << COM0B0);
		TCCR0B |=(1<<FOC0B);
		break;
	}
}


/*функции прерываний по таймеру*/
void TIMER0_attach_COMPA(void) { 
	TIMSK0 |= (1 << OCIE0A); // подкл прерывание
}

void TIMER0_attach_COMPB(void) { 
	TIMSK0 |= (1 << OCIE0B);
}

void TIMER0_detach_COMPA() { 
	TIMSK0 &=~ (1 << OCIE0A); // выкл прерывание
}

void TIMER0_detach_COMPB() { 
	TIMSK0 &=~  (1 << OCIE0B); // выкл прерывание
}



/* функции чтения/записи счетного регистра таймера */
byte TIMER0_getCounter() { // прочитать и вернуть значение счетного регистра
	return TCNT0;
}

void TIMER0_setCounter(byte value) { // записать новое значение счетного регистра
	cli(); // запретить прерывания
	TCNT0 = value;
	sei(); // разрешить прерывания
}



/*функции записи числа в регистры сравнения таймера */
void TIMER0_COMPA_setValue(byte value) { // записать значение в регистр сравнения А
	cli();
	OCR0A = value;
	sei();
}
void TIMER0_COMPB_setValue(byte value) { // записать значение в регистр сравнения Б
	cli();
	OCR0B = value;
	sei();
}

// ============== Timer 1 ==============

void TIMER1_setClock(byte clk) {
	switch (clk) {
	case 0: // stop
		TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
		break;
	case 1: // ext_fall
		TCCR1B &= ~ (1 << CS10);
		TCCR1B |= ((1 << CS12) | (1 << CS11));
		break;
	case 2: // ext_rise
		TCCR1B |= ((1 << CS12) | (1 << CS11) | (1 << CS10));
		break;
	case 3: // 1
		TCCR1B &= ~((1 << CS12) | (1 << CS11));
		TCCR1B |= (1 << CS10);
		break;
	case 4: // 8
		TCCR1B &= ~((1 << CS12) |  (1 << CS10));
		TCCR1B |= (1 << CS11);
		break;
	case 6: //64
		TCCR1B &= ~ (1 << CS12);
		TCCR1B |= ((1 << CS11) | (1 << CS10));
		break;
	case 8: //256
		TCCR1B &= ~((1 << CS11) |  (1 << CS10));
		TCCR1B |= (1 << CS12);
		break;
	case 9: // 1024
		TCCR1B &= ~ (1 << CS11);
		TCCR1B |= ((1 << CS12) | (1 << CS10));
		break;
	}
}


void TIMER1_setMode(byte mode) {
	switch (mode) {
	case 10: // normal
		TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
		TCCR1B &= ~((1 << WGM13) | (1 << WGM12));
		break;
	case 11: // ctc по OCR1A!!
		TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
		TCCR1B &= ~(1 << WGM13);
		TCCR1B |= (1 << WGM12);
		break;
	case 12: // fast_pwm_8bit
		TCCR1A &= ~(1 << WGM11);
		TCCR1B &= ~(1 << WGM13);
		TCCR1A |= (1 << WGM10);
		TCCR1B |= (1 << WGM12);
		break;
	case 13: // fast_pwm_9bit
		TCCR1A &= ~(1 << WGM10);
		TCCR1B &= ~(1 << WGM13);
		TCCR1A |= (1 << WGM11);
		TCCR1B |= (1 << WGM12);
		break;
	case 14: // fast_pwm_10bit
		TCCR1A |= (1 << WGM10);
		TCCR1B &= ~(1 << WGM13);
		TCCR1A |= (1 << WGM11);
		TCCR1B |= (1 << WGM12);
		break;
	case 15: // phasecorrect_pwm_8bit
		TCCR1B &= ~((1 << WGM13) | (1 << WGM12));
		TCCR1A &= ~ (1 << WGM11);
		TCCR1A |= (1 << WGM10);
		break;
	case 16: // phasecorrect_pwm_9bit
		TCCR1B &= ~((1 << WGM13) | (1 << WGM12));
		TCCR1A &= ~ (1 << WGM10);
		TCCR1A |= (1 << WGM11);
		break;
	case 17: // phasecorrect_pwm_10bit
		TCCR1A |= ((1 << WGM11) | (1 << WGM10));
		TCCR1B &= ~((1 << WGM13) | (1 << WGM12));
		break;
	case 18: // fast_pwm_custom   top = ICR1
		TCCR1B |= ((1 << WGM13) | (1 << WGM12));
		TCCR1A &= ~ (1 << WGM10);
		TCCR1A |= (1 << WGM11);
		break;
	case 19: // phasecorrect_pwm_custom   top = ICR1
		TCCR1B &= ~ (1 << WGM12);
		TCCR1A &= ~ (1 << WGM10);
		TCCR1B |= (1 << WGM13);
		TCCR1A |= (1 << WGM11);
		break;
	}
}


void TIMER1_COMPA_mode(byte mode) { // установить режим работы компаратора А
	switch (mode) {
	case 20: // disable
		TCCR1A &= ~ ((1 << COM1A1) | (1 << COM1A0));
		break;
	case 21: // pwm
		TCCR1A &= ~ (1 << COM1A0);
		TCCR1A |= (1 << COM1A1);
		TCCR1C &=~(1<<FOC1A);
		break;
	case 22: // invert_pwm
		TCCR1A |= ((1 << COM1A1) | (1 << COM1A0));
		TCCR1C &=~(1<<FOC1A);
		break;
	case 23: // toggle
		TCCR1A &= ~ (1 << COM1A1);
		TCCR1A |= (1 << COM1A0);
		TCCR1C |= (1<<FOC1A);
		break;
	}
}


void TIMER1_COMPB_mode(byte mode) { // установить режим работы компаратора Б
	switch (mode) {
	case 20: // disable
		TCCR1A &= ~ ((1 << COM1B1) | (1 << COM1B0));
		break;
	case 21: // pwm
		TCCR1A &= ~ (1 << COM1B0);
		TCCR1A |= (1 << COM1B1);
		TCCR1C &=~(1<<FOC1B);
		break;
	case 22: // invert_pwm
		TCCR1A |= ((1 << COM1B1) | (1 << COM1B0));
		TCCR1C &=~(1<<FOC1B);
		break;
	case 23: // toggle
		TCCR1A &= ~ (1 << COM1B1);
		TCCR1A |= (1 << COM1B0);
		TCCR1C |= (1<<FOC1B);
		break;
	}
}


void TIMER1_setTop(unsigned int top) { // установка предела счета для таймера 1, используется для управления глубиной шим
	cli();
	ICR1H = highByte(top);
	ICR1L = lowByte(top);
	sei();
}


void TIMER1_attach_OVF(void) { // подкл прерывание переполнения
	TIMSK1 |= (1 << TOIE1);
}

void TIMER1_attach_COMPA(void) { // подкл прерывание А
	TIMSK1 |= (1 << OCIE1A);
}

void TIMER1_attach_COMPB(void) { // подкл прерывание Б
	TIMSK1 |= (1 << OCIE1B);
}

void TIMER1_detach_OVF() { 
	TIMSK1 &=~ (1 << TOIE1);
}

void TIMER1_detach_COMPA() { 
	TIMSK1 &=~ (1 << OCIE1A);
}

void TIMER1_detach_COMPB() { 
	TIMSK1 &=~ (1 << OCIE1B);
}



unsigned int TIMER1_getCounter() { // прочитать счетный регистр
	return TCNT1L | (TCNT1H << 8); // склеиваем 16 бит число
}

void TIMER1_setCounter(unsigned int value) { // записать счетный  регистр
	cli();
	TCNT1H = highByte(value);
	TCNT1L = lowByte(value);
	sei();
}


void TIMER1_COMPA_setValue(unsigned int value) { // записать значение в регистр сравнения А
	cli();
	OCR1AH = highByte(value);
	OCR1AL = lowByte(value);
	sei();
}
void TIMER1_COMPB_setValue(unsigned int value) { // записать значение в регистр сравнения Б
	cli();
	OCR1BH = highByte(value);
	OCR1BL = lowByte(value);
	sei();
}

// ============== Timer 2 ==============

void TIMER2_setClock(byte clk) {
	switch (clk) {
	case 0: // stop
		TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
		break;
	case 3: // 1
		TCCR2B &= ~((1 << CS22) | (1 << CS21));
		TCCR2B |= (1 << CS20);
		break;
	case 4: // 8
		TCCR2B &= ~((1 << CS22) | (1 << CS20));
		TCCR2B |= (1 << CS21);
		break;
	case 5: //32
		TCCR2B &= ~(1 << CS22);
		TCCR2B |= ((1 << CS21) | (1 << CS20));
		break;
	case 6: //64
		TCCR2B &= ~((1 << CS21) | (1 << CS20));
		TCCR2B |= (1 << CS22);
		break;
	case 7: //128
		TCCR2B &= ~(1 << CS21);
		TCCR2B |= ((1 << CS22) | (1 << CS20));
		break;
	case 8: //256
		TCCR2B &= ~(1 << CS20);
		TCCR2B |= ((1 << CS22) | (1 << CS21));
		break;
	case 9: // 1024
		TCCR2B |= ((1 << CS22) | (1 << CS21) | (1 << CS20));
		break;
	}
}


void TIMER2_setMode(byte mode) {
	switch (mode) {
	case 10: // normal
		TCCR2B &= ~ (1 << WGM22); // top = 255
		TCCR2A &= ~((1 << WGM21) | (1 << WGM20)); // pwm & correct disable
		break;
	case 11: // ctc до OCR0A
		TCCR2B &= ~ (1 << WGM22);
		TCCR2A &= ~ (1 << WGM20); // pwm disable
		TCCR2A |= (1 << WGM21); // top = OCR0A
		break;
	case 12: // fast_pwm_8bit
		TCCR2B &= ~ (1 << WGM22); // top = 255
		TCCR2A |= ((1 << WGM21) | (1 << WGM20)); // pwm en , correct disable
		break;
	case 15: // phasecorrect_pwm_8bit
		TCCR2B &= ~ (1 << WGM22); // top = 255
		TCCR2A &= ~ (1 << WGM21); // correct en
		TCCR2A |= (1 << WGM20); // pwm en
		break;
	case 18: // fast_pwm_custom
		TCCR2B |= (1 << WGM22); // top = OCR0A
		TCCR2A |= ((1 << WGM21) | (1 << WGM20)); // pwm en , correct disable
		break;
	case 19: // phasecorrect_pwm_custom
		TCCR2B |= (1 << WGM22); // top = OCR0A
		TCCR2A &= ~ (1 << WGM21); // correct en
		TCCR2A |= (1 << WGM20); // pwm en
		break;
	}
}


void TIMER2_COMPA_mode(byte mode) { // установить режим работы компаратора А
	switch (mode) {
	case 20: // disable
		TCCR2A &= ~ ((1 << COM2A1) | (1 << COM2A0));
		break;
	case 21: // pwm
		TCCR2A &= ~ (1 << COM2A0);
		TCCR2A |= (1 << COM2A1);
		TCCR2B &=~(1<<FOC2A);
		break;
	case 22: // invert_pwm
		TCCR2A |= ((1 << COM2A1) | (1 << COM2A0));
		TCCR2B &=~(1<<FOC2A);
		break;
	case 23: // toggle
		TCCR2A &= ~ (1 << COM2A1);
		TCCR2A |= (1 << COM2A0);
		TCCR2B |=(1<<FOC2A);
		break;
	}
}


void TIMER2_COMPB_mode(byte mode) { // установить режим работы компаратора Б
	switch (mode) {
	case 20: // disable
		TCCR2A &= ~ ((1 << COM2B1) | (1 << COM2B0));
		break;
	case 21: // pwm
		TCCR2A &= ~ (1 << COM2B0);
		TCCR2A |= (1 << COM2B1);
		TCCR2B &=~(1<<FOC2B);
		break;
	case 22: // invert_pwm
		TCCR2A |= ((1 << COM2B1) | (1 << COM2B0));
		TCCR2B &=~(1<<FOC2B);
		break;
	case 23: // toggle
		TCCR2A &= ~ (1 << COM2B1);
		TCCR2A |= (1 << COM2B0);
		TCCR2B |=(1<<FOC2B);
		break;
	}
}


void TIMER2_attach_OVF(void) { // подкл прерывание переполнения
	TIMSK2 |= (1 << TOIE2);
}

void TIMER2_attach_COMPA(void) { // подкл прерывание А
	TIMSK2 |= (1 << OCIE2A);
}

void TIMER2_attach_COMPB(void) { // подкл прерывание Б
	TIMSK2 |= (1 << OCIE2B);
}
void TIMER2_detach_OVF() { 
	TIMSK2 &=~ (1 << TOIE2);
}

void TIMER2_detach_COMPA() { 
	TIMSK2 &=~ (1 << OCIE2A);
}

void TIMER2_detach_COMPB() { 
	TIMSK2 &=~ (1 << OCIE2B);
}

byte TIMER2_getCounter() { // прочитать и вернуть значение счетного регистра
	return TCNT2;
}

void TIMER2_setCounter(byte value) { // записать новое значение счетного регистра
	cli();
	TCNT2 = value;
	sei();
}

void TIMER2_COMPA_setValue(byte value) { // записать значение в регистр сравнения А
	cli();
	OCR2A = value;
	sei();
}
void TIMER2_COMPB_setValue(byte value) { // записать значение в регистр сравнения Б
	cli();
	OCR2B = value;
	sei();
}

// ============== Watchdog ==============

void WDT_attachInterrupt(int prescaler) {
	cli();
	WDTCSR |= 0b00011000; // тут мы включаем прерывания ватдога
	switch (prescaler) {
	case 2:		// предделитель 2, период 16мс
		WDTCSR = 0b01000000; // тут настраиваем делитель		
		break;
	case 4:		// предделитель 4, период 32мс
		WDTCSR = 0b01000001;
		break;
	case 8:		// предделитель 8, период 64мс
		WDTCSR = 0b01000010;
		break;
	case 16:	// предделитель 16, период 125мс
		WDTCSR = 0b01000011;
		break;
	case 32:	// предделитель 32, период 250мс
		WDTCSR = 0b01000100;
		break;
	case 64:	// предделитель 64, период 500мс
		WDTCSR = 0b01000101;
		break;
	case 128:	// предделитель 128, период 1с
		WDTCSR = 0b01000110;
		break;
	case 256:	// предделитель 256, период 2с
		WDTCSR = 0b01000111;
		break;
	case 512:	// предделитель 512, период 4с
		WDTCSR = 0b01100100;
		break;
	case 1024:	// предделитель 1024, период 8с
		WDTCSR = 0b01100001;
		break;
	}
	sei();
}

void WDT_detachInterrupt() {
	cli();
	WDTCSR = 0; // полностью душим ватчдог
	sei();
}