#include "GyverHacks.h"
#include <Arduino.h> 
#include <EEPROM.h>

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) 
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))


// ***************************** VCC *****************************
int vcc_const = 1100;

int getVCC() {
  //reads internal 1V1 reference against VCC
  #if defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny44__)
    ADMUX = _BV(MUX5) | _BV(MUX0); // For ATtiny84
  #elif defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__)
    ADMUX = _BV(MUX3) | _BV(MUX2); // For ATtiny85/45
  #elif defined(__AVR_ATmega1284P__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);  // For ATmega1284
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);  // For ATmega328
  #endif
	delay(2); // Wait for Vref to settle
	ADCSRA |= _BV(ADSC); // Start conversion
	while (bit_is_set(ADCSRA, ADSC)); // measuring
	uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
	uint8_t high = ADCH; // unlocks both
	long result = (high << 8) | low;
	result = (float)vcc_const * 1023 / result; // расчёт реального VCC
	return result; // возвращает VCC в милливольтах
}

int getVoltage(uint8_t pin) {
	long vcc_aver = 0;
	for (byte i = 0; i < 20; i++) {		
		vcc_aver += getVCC();
	}
	vcc_aver /= 20;
	return ((float)vcc_aver * analogRead(pin) / 1023);
}

void constantWizard() {
	vcc_const = 1100;     // начальаня константа калибровки
	
	// ищем среднее за 100 измерений
	long vcc_aver = 0;
	for (byte i = 0; i < 100; i++) {		
		vcc_aver += getVCC();
		delay(5);
	}
	vcc_aver /= 100;
	
	Serial.print(F("Actual VCC: "));
	Serial.print(vcc_aver);
	Serial.println(F(" mV"));
	Serial.println(F("Send your VCC in millivolts"));
	
	while (!Serial.available());	// ждём ответа
	int Vcc = Serial.parseInt(); 	// напряжение от пользователя
	delay(50);
	vcc_const = (float)vcc_const * Vcc / vcc_aver;              	// расчёт константы
	Serial.print(F("New voltage constant: "));
	Serial.println(vcc_const);
	Serial.println(F("Save in EEPROM? (Y / N)"));
	while(1) {
		while (!Serial.available());	// ждём ответа
		char answer = Serial.read();
		if (answer == 'N') {
			Serial.println(F("Bye"));
			return;
		}
		if (answer == 'Y') {
			break;
		}
	}
	Serial.println(F("Send address"));	
	unsigned long now = millis ();
	while (millis () - now < 500) Serial.read ();  // read and discard any input
	while (!Serial.available());	// ждём ответа
	int adr = Serial.parseInt(); 	// адрес ячейки
	EEPROM.put(adr, vcc_const);
	Serial.print(F("Write in "));
	Serial.print(adr);
	Serial.println(F(" OK"));
}

void restoreConstant(int adr) {
	EEPROM.get(adr, vcc_const);
}

void setConst(int new_const) {
	vcc_const = new_const;
}

int getConst() {
	return vcc_const;
}

// 5 шагов по 20%
// Литий 1 шт 4200, 3950, 3850, 3750, 3700, 2800
// Алкалин 3шт 4650, 4050, 3870, 3690, 3570, 3300
// Никель 3шт 4200, 3810, 3750, 3690, 3600, 3000
// Никель 4шт 5600, 5080, 5000, 4920, 4800, 4000

byte lithiumPercent(int volts) {
	return mVtoPercent(volts, 4200, 3950, 3850, 3750, 3700, 2800);
}

byte alkaline3Percent(int volts) {
	return mVtoPercent(volts, 4650, 4050, 3870, 3690, 3570, 3300);
}

byte nickel3Percent(int volts) {
	return mVtoPercent(volts, 4200, 3810, 3750, 3690, 3600, 3000);
}

byte nickel4Percent(int volts) {
	return mVtoPercent(volts, 5600, 5080, 5000, 4920, 4800, 4000);
}

byte mVtoPercent(int volts, int volt100, int volt80, int volt60, int volt40, int volt20, int volt0) {
	int capacity;
	if (volts > volt80) capacity = map(volts, volt100, volt80, 100, 80);
	else if ((volts <= volt80) && (volts > volt60) ) capacity = map(volts, volt80, volt60, 80, 60);
	else if ((volts <= volt60) && (volts > volt40) ) capacity = map(volts, volt60, volt40, 60, 40);
	else if ((volts <= volt40) && (volts > volt20) ) capacity = map(volts, volt40, volt20, 40, 20);
	else if (volts <= volt20) capacity = map(volts, volt20, volt0, 20, 0);
	capacity = constrain(capacity, 0, 100);
	return capacity;
}

// ***************************** TEMP *****************************

float getTemp() {
  /* from the data sheet
    Temperature / °C -45°C +25°C +85°C
    Voltage     / mV 242 mV 314 mV 380 mV
  */
  unsigned int wADC;
  double t;

  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC
  delay(10);            // wait for voltages to become stable.
  ADCSRA |= _BV(ADSC);  // Start the ADC
  // Detect end-of-conversion
  while (bit_is_set(ADCSRA, ADSC));
  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;
  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;
  // The returned temperature is in degrees Celsius.
  return (t);
}

// ***************************** PWM mode *****************************
void setPWMmode(byte pin, byte mode) {		// 0 - FastPWM, 1 - Phase-correct PWM
	if (pin == 5 || pin == 6) {
		if (mode) TCCR0A |= _BV(WGM00);
		else TCCR0A |= _BV(WGM00) | _BV(WGM01);
	} else
	if (pin == 9 || pin == 10) {
		if (mode) TCCR1B &= ~_BV(WGM12);
		else TCCR1B |= _BV(WGM12);
	} else
	if (pin == 3 || pin == 11) {
		if (mode) TCCR2A |= _BV(WGM20);
		else TCCR2A |= _BV(WGM20) | _BV(WGM21);
	} else {
		return;
	}
}
// ***************************** PWM resolution *****************************

void set8bitPWM() {
	TCCR1A = TCCR1A & 0xe0 | 1;
}
void set10bitPWM() {
	TCCR1A = TCCR1A & 0xe0 | 3;
}

// ***************************** PWM freq *****************************
float _fixMultiplier = 1.0;		// множитель для функций delayFix и millisFix, по умолч. 1

void setPWMprescaler(uint8_t pin, uint16_t mode) {
  byte prescale;
  if (pin == 5 || pin == 6) {
    switch (mode) {
      case 1: prescale = 0x01; _fixMultiplier = 64; break;
      case 2: prescale = 0x02; _fixMultiplier = 8; break;
      case 3: prescale = 0x03; _fixMultiplier = 1; break;
      case 4: prescale = 0x04; _fixMultiplier = 0.25; break;
      case 5: prescale = 0x05; _fixMultiplier = 0.0625; break;
      default: return;
    }
  } else if (pin == 9 || pin == 10) {
	  switch (mode) {
      case 1: prescale = 0x01; break;
      case 2: prescale = 0x02; break;
      case 3: prescale = 0x03; break;
      case 4: prescale = 0x04; break;
      case 5: prescale = 0x05; break;
      default: return;
    }
  } else if (pin == 3 || pin == 11) {
    switch (mode) {
      case 1: prescale = 0x01; break;
      case 2: prescale = 0x02; break;
      case 3: prescale = 0x03; break;
      case 4: prescale = 0x04; break;
      case 5: prescale = 0x05; break;
      case 6: prescale = 0x06; break;
      case 7: prescale = 0x07; break;
      default: return;
    }
  }
  if (pin == 5 || pin == 6) {
	TCCR0B = 0;
    TCCR0B = TCCR0B & 0b11111000 | prescale;
  } else if (pin == 9 || pin == 10) {
	TCCR1B = 0;
    TCCR1B = TCCR1B & 0b11111000 | prescale;
  } else if (pin == 3 || pin == 11) {
	TCCR2B = 0;
    TCCR2B = TCCR2B & 0b11111000 | prescale;
  }
}

void delayFix(uint32_t delayTime) {
	delay((float) delayTime * _fixMultiplier);
}

void delayMicrosecondsFix(uint32_t delayTime) {
	delayMicroseconds((float) delayTime * _fixMultiplier);
}

uint32_t millisFix() {
	return (float) millis() / _fixMultiplier;
}
uint32_t microsFix() {
	return (float) micros() / _fixMultiplier;
}


/*
Default: delay(1000) or 1000 millis() ~ 1 second

0x01: delay(64000) or 64000 millis() ~ 1 second x64
0x02: delay(8000) or 8000 millis() ~ 1 second x8
0x03: is the default
0x04: delay(250) or 250 millis() ~ 1 second x0.25
0x05: delay(62) or 62 millis() ~ 1 second x0.0625

(Or 63 if you need to round up.  The number is actually 62.5)
*/

// ***************************** fast digitalwrite *****************************
void setPin(uint8_t pin, uint8_t x) { 	
	if (pin < 8) bitWrite(PORTD, pin, x);
	else if (pin < 14) bitWrite(PORTB, (pin - 8), x); 
	else if (pin < 20) bitWrite(PORTC, (pin - 14), x);
	else return;
}

// ***************************** fast digitalread *****************************
boolean readPin(uint8_t pin) { 	
	if (pin < 8) return bitRead(PIND, pin);
	else if (pin < 14) return bitRead(PINB, pin - 8);
	else if (pin < 20) return bitRead(PINC, pin - 14);	
	else return false;
}

// ***************************** fast analogwrite *****************************

void setPWM(uint8_t pin, uint16_t duty) {
	switch (pin) {
		case 5:
		sbi(TCCR0A, COM0B1);
		OCR0B = duty;
		break;
		case 6:
		sbi(TCCR0A, COM0A1);
		OCR0A = duty;
		break;
		case 10:
		sbi(TCCR1A, COM1B1);
		OCR1B = duty;
		break;
		case 9:
		sbi(TCCR1A, COM1A1);
		OCR1A = duty;
		break;
		case 3:
		sbi(TCCR2A, COM2B1);
		OCR2B = duty;
		break;
		case 11:
		sbi(TCCR2A, COM2A1);
		OCR2A = duty;
		break;
		default:
		break;
	}	
}

// ***************************** anyPWM *****************************
volatile uint8_t pwm;
volatile uint8_t pwms[20];
volatile boolean anyPWMpins[20];

void anyPWMinit(byte prescaler) // 1 - 7
{
  cli();
  TCCR2A = 0;   //при совпадении уровень OC1A меняется на противоположный
  TCCR2B = 5;   //CLK
  OCR2A = 1;
  TIMSK2 = 2;   //разрешаем прерывание по совпадению
  sei();
  TCCR2B = prescaler;   // prescaler
}
 
void anyPWMpin(uint8_t pin) {	
	anyPWMpins[pin] = 1;
	pinMode(pin, OUTPUT);
}

void anyPWM(byte pin, byte duty)
{
	pwms[pin] = duty;
}

#if (defined(__AVR_ATmega328P__) && (ALLOW_ANYPWM == 1))
ISR(TIMER2_COMPA_vect)
{
  TCNT2 = 0;
  anyPWMpins[0] && pwms[0] > pwm ? PORTD |= B00000001 : PORTD &= B11111110;
  anyPWMpins[1] && pwms[1] > pwm ? PORTD |= B00000010 : PORTD &= B11111101;
  anyPWMpins[2] && pwms[2] > pwm ? PORTD |= B00000100 : PORTD &= B11111011;
  anyPWMpins[3] && pwms[3] > pwm ? PORTD |= B00001000 : PORTD &= B11110111;
  anyPWMpins[4] && pwms[4] > pwm ? PORTD |= B00010000 : PORTD &= B11101111;
  anyPWMpins[5] && pwms[5] > pwm ? PORTD |= B00100000 : PORTD &= B11011111;
  anyPWMpins[6] && pwms[6] > pwm ? PORTD |= B01000000 : PORTD &= B10111111;
  anyPWMpins[7] && pwms[7] > pwm ? PORTD |= B10000000 : PORTD &= B01111111;
  anyPWMpins[8] && pwms[8] > pwm ? PORTB |= B00000001 : PORTB &= B11111110;
  anyPWMpins[9] && pwms[9] > pwm ? PORTB |= B00000010 : PORTB &= B11111101;
  anyPWMpins[10] && pwms[10] > pwm ? PORTB |= B00000100 : PORTB &= B11111011;
  anyPWMpins[11] && pwms[11] > pwm ? PORTB |= B00001000 : PORTB &= B11110111;
  anyPWMpins[12] && pwms[12] > pwm ? PORTB |= B00010000 : PORTB &= B11101111;
  anyPWMpins[13] && pwms[13] > pwm ? PORTB |= B00100000 : PORTB &= B11011111;
  anyPWMpins[14] && pwms[14] > pwm ? PORTC |= B00000001 : PORTC &= B11111110;
  anyPWMpins[15] && pwms[15] > pwm ? PORTC |= B00000010 : PORTC &= B11111101;
  anyPWMpins[16] && pwms[16] > pwm ? PORTC |= B00000100 : PORTC &= B11111011;
  anyPWMpins[17] && pwms[17] > pwm ? PORTC |= B00001000 : PORTC &= B11110111;
  anyPWMpins[18] && pwms[18] > pwm ? PORTC |= B00010000 : PORTC &= B11101111;
  anyPWMpins[19] && pwms[19] > pwm ? PORTC |= B00100000 : PORTC &= B11011111;

  pwm++;
}
#endif

// ***************************** ADC *****************************

void setADCrate(byte mode) {
	if ((mode & (1 << 2))) sbi(ADCSRA, ADPS2);
    else cbi(ADCSRA, ADPS2);
    if ((mode & (1 << 1))) sbi(ADCSRA, ADPS1);
    else cbi(ADCSRA, ADPS1);
    if ((mode & (1 << 0))) sbi(ADCSRA, ADPS0);
    else cbi(ADCSRA, ADPS0);
}

// ***************************** GTimer *****************************
GTimer::GTimer() {}

GTimer::GTimer(uint32_t interval) {
	_interval = interval;
	_timer = (long)millis() + _interval;
}

void GTimer::setInterval(uint32_t interval) {
	_interval = interval;
	GTimer::reset();
}
void GTimer::setMode(boolean mode) {
	_mode = mode;
}

boolean GTimer::isReady() {
	if ((long)millis() > _timer) {
		if (_mode) _timer = millis() + _interval;
		return true;
	} else {
		return false;
	}
}

void GTimer::reset() {
	_timer = millis() + _interval;
}