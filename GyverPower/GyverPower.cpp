#include <GyverPower.h>
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
extern volatile unsigned long timer0_millis;
#endif
static volatile bool _wdtFlag = false;

void GyverPower::hardwareEnable(uint16_t data) {
#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega32U4__)
	PRR1 &= ~ highByte(data); 		// загрузили данные в регистр
	PRR0 &= ~ lowByte(data);
#else
	PRR &= ~ lowByte(data);
#endif
	if (data & PWR_ADC) { 			// если что то сказали про ацп
		ADCSRA |= (1 << ADEN); 		// вкл ацп
		ACSR &= ~ (1 << ACD); 		// вкл компаратор
	}
}

void GyverPower::hardwareDisable(uint16_t data) {
	if (data & PWR_ADC) { 				// если что то сказали про ацп
		ADCSRA &= ~ (1 << ADEN); 		// выкл ацп
		ACSR |= (1 << ACD); 			// выкл компаратор
	}
#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega32U4__)
	PRR1 |= highByte(data); 			// загрузили данные в регистр
	PRR0 |= lowByte(data);
#else
	PRR |= lowByte(data);
#endif
}

void GyverPower::setSystemPrescaler(prescalers_t prescaler) {
	CLKPR = (1 << CLKPCE); 			// разрешили менять делитель
	CLKPR = prescaler; 				// загрузили новый
}

void GyverPower::bodInSleep(bool state) {
	bodEnable = state;
}

void GyverPower::setSleepMode(sleepmodes_t mode) {
	sleepMode = mode;
}

uint16_t GyverPower::getMaxTimeout(void){
	WDTCSR |= (1 << WDCE) | (1 << WDE); 	// разрешаем вмешательство
	WDTCSR = 0x61; 							// таймаут ~ 8 c
	asm ("wdr"); 							// сбросили пса
	_wdtFlag = false;
	uint16_t startTime = millis(); 			// засекли время	
	while (!_wdtFlag); 						// ждем таймаута	
	uint16_t ms = millis() - startTime;
	WDTCSR |= (1 << WDCE) | (1 << WDE); 	// разрешаем вмешательство
	WDTCSR = 0; 							// выкл wdt 
	return  ms;
}

void GyverPower::autoCalibrate(void) {
	WDTCSR |= (1 << WDCE) | (1 << WDE); 	// разрешаем вмешательство
	WDTCSR = 0x47; 							// таймаут ~ 2 c
	asm ("wdr"); 							// сбросили пса
	_wdtFlag = false;
	uint16_t startTime = millis(); 			// засекли время
	while (!_wdtFlag); 						// ждем таймаута
	uint16_t ms = millis() - startTime;
	WDTCSR |= (1 << WDCE) | (1 << WDE); 	// разрешаем вмешательство
	WDTCSR = 0; 							// выкл wdt 
	for (uint8_t i = 0; i < 9 ; i++) { 		// пересчитываем массив
		timeOuts[9 - i] = ((ms * 4) >> i);
	}
}

void GyverPower::calibrate(uint16_t ms) { 	// пересчет массива таймаутов
	for (uint8_t i = 0; i < 9 ; i++) { 		// пересчитываем массив
		timeOuts[9 - i] = (ms >> i);
	}
}

void GyverPower::sleep(sleepprds_t period) {

	/* принудительно выкл АЦП и компаратор */
	if (sleepMode != IDLE_SLEEP && sleepMode != ADC_SLEEP) {
		ADCSRA &= ~ (1 << ADEN); 			// выкл ацп
		ACSR |= (1 << ACD); 				// выкл аналог компаратор
	}

	/* принудительное отключение PLL */
#if defined(__AVR_ATtiny85__)
	uint8_t pllCopy = PLLCSR; 			// запомнили настройки
	PLLCSR &= ~ (1 << PLLE); 			// выключили
#endif

	/* если спим с WDT - настраиваем его */
	if (period < 10) {
		uint8_t wdtReg = (1 << WDIE); 				// режим прерываний
		if (period > 7) { 							// если больше 7
			wdtReg |= (period - 8) | (1 << WDP3); 	// задвигаем делитель
		}
		else wdtReg |= period; 						// задвигаем делитель
		cli();
		WDTCSR |= (1 << WDCE) | (1 << WDE); 		// разрешаем вмешательство
		WDTCSR = wdtReg; 							// задвигаем весь регистр
		sei();
		asm ("wdr"); 								// сброс пса
	}

	/* настраиваем нужный режим сна */
#if defined(__AVR_ATtiny85__) ||  defined(__AVR_ATtiny84__)
	MCUCR |= (sleepMode << 3) | (1 << SE); 	// sleep mode + sleep enable
#else
	SMCR = (sleepMode << 1) | (1 << SE); 	// sleep mode + sleep enable
#endif

	/* процедура ухода в сон */
	if (bodEnable) asm ("sleep"); 	//  <<< точка ухода в сон (с БОД)
	else {
#if defined(__AVR_ATtiny85__) ||  defined(__AVR_ATtiny84__)
		MCUCR |= (1 << BODS) | (1 << BODSE); // выкл bod
		MCUCR = (MCUCR & 0x7B) | (1 << BODS);
#else
		MCUCR = (0x03 << 5); // выкл bod
		MCUCR = (0x02 << 5);
#endif
		asm ("sleep"); 				//  <<< точка ухода в сон (без БОД)
	}

	/* процедура выхода из сна*/
#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny84__)
	MCUCR = MCUCR & 0xC7; 	// откл сон
#else
	SMCR = 0; 				// откл сон
#endif

	/* восстановление настроек АЦП */
#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega32U4__)
	if (!(PRR0 & (1 << PRADC))) { 		// если ацп не выключен принудительно
		ADCSRA |= (1 << ADEN); 			// вкл после сна
		ACSR &= ~ (1 << ACD);
	}
#else
	if (!(PRR & (1 << PRADC))) {		// если ацп не выключен принудительно
		ADCSRA |= (1 << ADEN);			// вкл после сна
		ACSR &= ~ (1 << ACD);
	}
#endif
	/* восстановление настроек PLL (для тини85) */
#if defined(__AVR_ATtiny85__)
	PLLCSR = pllCopy;
#endif

}

uint8_t GyverPower::sleepDelay(uint32_t ms) {
	uint32_t saveMs = ms;
	wakeFlag = false;
	for (uint8_t i = 9; ms > timeOuts[0]; i--) { 	// начиная с длинного периода , пока мс > ~16
		while (ms > timeOuts[i]) {					// пока мс > текущего выбранного периода
			sleep(i);								// уйти в сон
			ms -= timeOuts[i];						// отнять время сна
			if (wakeFlag) {
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
				if (correct) timer0_millis += saveMs - ms;
#endif	
				return ms;
			}
		}
	}
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	if (correct) timer0_millis += saveMs - ms;
#endif	
	return ms; 										// вернуть остаток времени
}

void GyverPower::correctMillis(bool state) {
	correct = state;
}

void GyverPower::wakeUp(void) {
	wakeFlag = true;
}

void adjustInternalClock(int8_t adj) {
	static uint8_t currentCal;
	static bool startup = false;
	if (!startup) {
		startup = true;
		currentCal = OSCCAL;
	}
	uint8_t oldSreg = SREG;
	currentCal = constrain(currentCal + adj, 0, 255);
	cli();
	OSCCAL = currentCal;
	SREG = oldSreg;
}

ISR(WDT_vect) {							// просыпаемся тут
	_wdtFlag = true;					// для калибровки
	WDTCSR |= (1 << WDCE) | (1 << WDE); // разрешаем вмешательство
	WDTCSR = 0;							// выключаем пса
	asm ("wdr");						// обнуляем пса
}

GyverPower power = GyverPower();