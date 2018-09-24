#include "GyverRGB.h"
#include <Arduino.h>

volatile uint8_t pwm;
volatile uint8_t pwms[20];
volatile boolean anyPWMpins[20];

GRGB::GRGB(uint8_t rpin, uint8_t gpin, uint8_t bpin) {
	GRGB::GRGB(rpin, gpin, bpin, false);	// по умолчанию pwmmode NORM_PWM
}

GRGB::GRGB(uint8_t rpin, uint8_t gpin, uint8_t bpin, boolean pwmmode) {
	_PWMmode = pwmmode;
	
	_rpin = rpin;
	_gpin = gpin;
	_bpin = bpin;
	
	if (!_PWMmode) {	
		pinMode(_rpin, OUTPUT);
		pinMode(_gpin, OUTPUT);
		pinMode(_bpin, OUTPUT);
	} else {
		anyPWMinit(6);	// частота ~150 Гц
			
		anyPWMpin(_rpin);
		anyPWMpin(_gpin);
		anyPWMpin(_bpin);
	}
}

void GRGB::setDirection(boolean direction) {
	_reverse_flag = direction;
}

void GRGB::setRGB(uint8_t new_r, uint8_t new_g, uint8_t new_b) {
	_r = new_r;
	_g = new_g;
	_b = new_b;
	GRGB::setRGB();
}

void GRGB::setHEX(uint32_t color) {
	_r = (color >> 16) & 0xff;
	_g = (color >> 8) & 0xff;
	_b = color & 0xff;
	GRGB::setRGB();
}

void GRGB::setHSV_fast(uint8_t hue, uint8_t sat, uint8_t val) {
	byte h = ((24 * hue / 17) / 60) % 6;
	byte vmin = (long)val - val * sat / 255;
	byte a = (long)val * sat / 255 * (hue * 24 / 17 % 60) / 60;
	byte vinc = vmin + a;
	byte vdec = val - a;
	switch (h) {
		case 0: _r = val; _g = vinc; _b = vmin; break;
		case 1: _r = vdec; _g = val; _b = vmin; break;
		case 2: _r = vmin; _g = val; _b = vinc; break;
		case 3: _r = vmin; _g = vdec; _b = val; break;
		case 4: _r = vinc; _g = vmin; _b = val; break;
		case 5: _r = val; _g = vmin; _b = vdec; break;
	}	
	GRGB::setRGB();
}

void GRGB::setHSV(uint8_t h, uint8_t s, uint8_t v) {
	float r, g, b;
	
	float H = (float)h / 255;
	float S = (float)s / 255;
	float V = (float)v / 255;
	
    int i = int(H * 6);
    float f = H * 6 - i;
    float p = V * (1 - S);
    float q = V * (1 - f * S);
    float t = V * (1 - (1 - f) * S);

    switch(i % 6){
        case 0: r = V, g = t, b = p; break;
        case 1: r = q, g = V, b = p; break;
        case 2: r = p, g = V, b = t; break;
        case 3: r = p, g = q, b = V; break;
        case 4: r = t, g = p, b = V; break;
        case 5: r = V, g = p, b = q; break;
    }
	_r = r * 255;
	_g = g * 255;
	_b = b * 255;
	GRGB::setRGB();
}

// Для hex цветов
void GRGB::fadeTo(uint32_t newColor, uint16_t fadeTime) {
	// находим новые r g b
	byte new_r = (newColor >> 16) & 0xff;
	byte new_g = (newColor >> 8) & 0xff;
	byte new_b = newColor & 0xff;
	GRGB::fadeTo(new_r, new_g, new_b, fadeTime);
}

// Для r, g, b
void GRGB::fadeTo(byte new_r, byte new_g, byte new_b, uint16_t fadeTime) {
	// ищем изменение: новый цвет - текущий (м.б. отрицательным!)
	int deltaR = new_r - _r;
	int deltaG = new_g - _g;
	int deltaB = new_b - _b;

	// ищем наибольшее изменение по модулю
	byte deltaMax = 0;
	if (abs(deltaR) > deltaMax) deltaMax = abs(deltaR);
	if (abs(deltaG) > deltaMax) deltaMax = abs(deltaG);
	if (abs(deltaB) > deltaMax) deltaMax = abs(deltaB);

	// Шаг изменения цвета
	float stepR = (float)deltaR / deltaMax;
	float stepG = (float)deltaG / deltaMax;
	float stepB = (float)deltaB / deltaMax;

	// Защита от деления на 0. Завершаем работу
	if (deltaMax == 0) return 0;

	// Расчет задержки в мкс
	uint16_t stepDelay = (long) 1000 * fadeTime / deltaMax;

	// Дробные величины для плавности, начальное значение = текущему у светодиода
	float thisR = _r, thisG = _g, thisB = _b;

	// Основной цикл изменения яркости
	for (int i = 0; i < deltaMax; i++) {
		thisR += stepR;
		thisG += stepG;
		thisB += stepB;
		
		_r = thisR;
		_g = thisG;
		_b = thisB;
		GRGB::setRGB();
		delayMicroseconds(stepDelay);
	}
}

// служебные функции
void GRGB::setRGB() {
	if (!_PWMmode) {						// режим NORM_PWM
		if (_reverse_flag) {				// обратная полярность ШИМ
			analogWrite(_rpin, 255 - _r);
			analogWrite(_gpin, 255 - _g);
			analogWrite(_bpin, 255 - _b);
		} else {							// прямая полярность ШИМ
			analogWrite(_rpin, _r);
			analogWrite(_gpin, _g);
			analogWrite(_bpin, _b);
		}
	} else {								// режим ANY_PWM
		if (_reverse_flag) {				// обратная полярность ШИМ
			anyPWM(_rpin, 255 - _r);
			anyPWM(_gpin, 255 - _g);
			anyPWM(_bpin, 255 - _b);
		} else {							// прямая полярность ШИМ
			anyPWM(_rpin, _r);
			anyPWM(_gpin, _g);
			anyPWM(_bpin, _b);
		}
	}
}

// ***************************** anyPWM *****************************

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