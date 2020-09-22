#include "GyverRGB.h"
#include <Arduino.h>

volatile uint8_t pwmRGB;
volatile uint8_t pwmsRGB[20];
volatile boolean anyPWMpinsRGB[20];
/*
const uint8_t CRTgamma[256] PROGMEM = {
	0,		0,		0,		0,		0,		0,		0,		0,
	0,		0,		0,		0,		0,		0,		0,		0,		
	0,		0,		0,		0,		0,		0,		0,		0,		
	0,		0,		0,		1,		1,		1,		1,		1,		
	1,		1,		1,		1,		2,		2,		2,		2,		
	2,		2,		2,		3,		3,		3,		3,		3,		
	4,		4,		4,		4,		4,		5,		5,		5,		
	5,		6,		6,		6,		7,		7,		7,		8,		
	8,		8,		8,		9,		9,		10,		10,		10,		
	11,		11,		11,		12,		12,		13,		13,		13,		
	14,		14,		15,		15,		16,		16,		17,		17,		
	18,		18,		19,		19,		20,		20,		21,		22,		
	22,		23,		23,		24,		25,		25,		26,		26,		
	27,		28,		28,		29,		30,		30,		31,		32,		
	33,		33,		34,		35,		36,		36,		37,		38,		
	39,		40,		40,		41,		42,		43,		44,		45,		
	46,		47,		47,		48,		49,		50,		51,		52,		
	53,		54,		55,		56,		57,		58,		59,		60,		
	61,		62,		63,		64,		66,		67,		68,		69,		
	70,		71,		73,		74,		75,		76,		77,		79,		
	80,		81,		82,		84,		85,		86,		87,		89,		
	90,		91,		93,		94,		96,		97,		98,		100,		
	101,	103,	104,	106,	107,	109,	110,	112,	
	113,	115,	116,	118,	119,	121,	123,	124,	
	126,	127,	129,	131,	132,	134,	136,	138,	
	139,	141,	143,	145,	146,	148,	150,	152,	
	154,	156,	157,	159,	161,	163,	165,	167,	
	169,	171,	173,	175,	177,	179,	181,	183,	
	185,	187,	189,	191,	193,	195,	198,	200,	
	202,	204,	206,	208,	211,	213,	215,	217,	
	220,	222,	224,	227,	229,	231,	234,	236,	
	238,	241,	243,	246,	248,	251,	253,	255,		
};
*/
const uint8_t CRTgamma[256] PROGMEM =
{
	0,		0,		1,		1,		1,		1,		1,		1,
	1,		1,		1,		1,		1,		1,		1,		1,
	2,		2,		2,		2,		2,		2,		2,		2,
	3,		3,		3,		3,		3,		3,		4,		4,
	4,		4,		4,		5,		5,		5,		5,		6,
	6,		6,		7,		7,		7,		8,		8,		8,
	9,		9,		9,		10,		10,		10,		11,		11,
	12,		12,		12,		13,		13,		14,		14,		15,
	15,		16,		16,		17,		17,		18,		18,		19,
	19,		20,		20,		21,		22,		22,		23,		23,
	24,		25,		25,		26,		26,		27,		28,		28,
	29,		30,		30,		31,		32,		33,		33,		34,
	35,		35,		36,		37,		38,		39,		39,		40,
	41,		42,		43,		43,		44,		45,		46,		47,
	48,		49,		49,		50,		51,		52,		53,		54,
	55,		56,		57,		58,		59,		60,		61,		62,
	63,		64,		65,		66,		67,		68,		69,		70,
	71,		72,		73,		74,		75,		76,		77,		79,
	80,		81,		82,		83,		84,		85,		87,		88,
	89,		90,		91,		93,		94,		95,		96,		98,
	99,		100,	101,	103,	104,	105,	107,	108,
	109,	110,	112,	113,	115,	116,	117,	119,
	120,	121,	123,	124,	126,	127,	129,	130,
	131,	133,	134,	136,	137,	139,	140,	142,
	143,	145,	146,	148,	149,	151,	153,	154,
	156,	157,	159,	161,	162,	164,	165,	167,
	169,	170,	172,	174,	175,	177,	179,	180,
	182,	184,	186,	187,	189,	191,	193,	194,
	196,	198,	200,	202,	203,	205,	207,	209,
	211,	213,	214,	216,	218,	220,	222,	224,
	226,	228,	230,	232,	233,	235,	237,	239,
	241,	243,	245,	247,	249,	251,	253,	255,
};

GRGB::GRGB(uint8_t rpin, uint8_t gpin, uint8_t bpin) {
	// по умолчанию pwmmode NORM_PWM
	_rpin = rpin;
	_gpin = gpin;
	_bpin = bpin;
	pinMode(_rpin, OUTPUT);
	pinMode(_gpin, OUTPUT);
	pinMode(_bpin, OUTPUT);
}

GRGB::GRGB(uint8_t rpin, uint8_t gpin, uint8_t bpin, uint8_t pwmmode) {
	_PWMmode = pwmmode;
	
	_rpin = rpin;
	_gpin = gpin;
	_bpin = bpin;
	
	if (!_PWMmode) {	
		pinMode(_rpin, OUTPUT);
		pinMode(_gpin, OUTPUT);
		pinMode(_bpin, OUTPUT);
	} else {
		anyPWMinitRGB(6);	// частота ~150 Гц
		
		anyPWMpinRGB(_rpin);
		anyPWMpinRGB(_gpin);
		anyPWMpinRGB(_bpin);
	}
}
// для пинов 3, 9, 10. Для 5 и 6 делать вручную, т.к. влияет на millis() и прочие
void GRGB::highFrequency(long frequency) {
	InitTimersSafe();
	// таймер 2
	SetPinFrequencySafe(3, frequency);
	// на пине 11 не работает
	
	// таймер 1
	SetPinFrequencySafe(9, frequency);
	SetPinFrequencySafe(10, frequency);
	
	_highFreqFlag = true;
}

void GRGB::setDirection(uint8_t direction) {
	_reverse_flag = direction;
}

void GRGB::setBrightness(byte bright) {
	_brightFlag = true;
	_brightC = (float)bright / 255;
	GRGB::setRGB();
}

void GRGB::setGammaBright(boolean val) {
	_gammaBright = val;
}

void GRGB::setMinPWM(byte val) {
	_minPWMflag = true;
	_minPWMval = val;	
}

void GRGB::setMaxCurrent(uint16_t numLeds, float vcc, int maxCur) {
	_maxCurFlag = true;
	_vcc = vcc;
	_maxCurrent = maxCur;
	_numLeds = numLeds;	
}

void GRGB::setLUT(float rc, float gc, float bc) {
	_LUTflag = true;
	_rc = rc;
	_gc = gc;
	_bc = bc;
}

void GRGB::constantBrightTick(int minVolts, int vcc) {
	_constBrFlag = true;
	if (vcc > minVolts) {
		int minCur = minVolts * 3.8 - 28700;
		int nowCur = vcc * 3.8 - 28700;
		_constCoef = (float)minCur / nowCur;
	} else _constCoef = 1;
	GRGB::setRGB();
}

void GRGB::gammaTick(int vcc) {
	_gammaFlag = true;
	if (vcc < 12000) {
		_gammaR = (float)vcc / 10000 - 0.5;
		_gammaG = (float)vcc / 10000 - 0.3;	
	} else {
		_gammaR = 1;
		_gammaG = 1;
	}
	GRGB::setRGB();
}

void GRGB::setRGB(uint8_t new_r, uint8_t new_g, uint8_t new_b) {
	_r = new_r;
	_g = new_g;
	_b = new_b;
	GRGB::setRGB();
}

void GRGB::setHEX(colors color) {
	setHEX((uint32_t)color);
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

// источник: http://www.tannerhelland.com/4435/convert-temperature-rgb-algorithm-code/
void GRGB::setKelvin(int16_t temperature) {
	float tmpKelvin, tmpCalc;

	temperature = constrain(temperature, 1000, 40000);
	tmpKelvin = temperature / 100;

	// red
	if (tmpKelvin <= 66) _r = 255;
	else {
		tmpCalc = tmpKelvin - 60;
		tmpCalc = (float)pow(tmpCalc, -0.1332047592);
		tmpCalc *= (float)329.698727446;
		tmpCalc = constrain(tmpCalc, 0, 255);
		_r = tmpCalc;
	}

	// green
	if (tmpKelvin <= 66) {
		tmpCalc = tmpKelvin;
		tmpCalc = (float)99.4708025861 * log(tmpCalc) - 161.1195681661;
		tmpCalc = constrain(tmpCalc, 0, 255);
		_g = tmpCalc;
	} else {
		tmpCalc = tmpKelvin - 60;
		tmpCalc = (float)pow(tmpCalc, -0.0755148492);
		tmpCalc *= (float)288.1221695283;
		tmpCalc = constrain(tmpCalc, 0, 255);
		_g = tmpCalc;
	}

	// blue
	if (tmpKelvin >= 66) _b = 255;
	else if (tmpKelvin <= 19) _b = 0;
	else {
		tmpCalc = tmpKelvin - 10;
		tmpCalc = (float)138.5177312231 * log(tmpCalc) - 305.0447927307;
		tmpCalc = constrain(tmpCalc, 0, 255);
		_b = tmpCalc;
	}
	GRGB::setRGB();
}

// Для hex цветов
void GRGB::fadeTo(colors newColor, uint16_t fadeTime) {
	uint32_t newColor32 = newColor;
	// находим новые r g b
	byte new_r = (newColor32 >> 16) & 0xff;
	byte new_g = (newColor32 >> 8) & 0xff;
	byte new_b = newColor32 & 0xff;
	GRGB::fadeTo(new_r, new_g, new_b, fadeTime);
}

// Для r, g, b
void GRGB::fadeTo(byte new_r, byte new_g, byte new_b, uint16_t fadeTime) {
	// ищем изменение: новый цвет - текущий (м.б. отрицательным!)
	int deltaR = new_r - _r;
	int deltaG = new_g - _g;
	int deltaB = new_b - _b;

	// ищем наибольшее изменение по модулю
	int deltaMax = 0;
	if (abs(deltaR) > deltaMax) deltaMax = abs(deltaR);
	if (abs(deltaG) > deltaMax) deltaMax = abs(deltaG);
	if (abs(deltaB) > deltaMax) deltaMax = abs(deltaB);
	
	// Защита от деления на 0. Завершаем работу
	if (deltaMax == 0) return;

	// Шаг изменения цвета
	float stepR = (float)deltaR / deltaMax;
	float stepG = (float)deltaG / deltaMax;
	float stepB = (float)deltaB / deltaMax;

	// Расчет задержки в мкс
	uint32_t stepDelay = (float) 1000 * fadeTime / deltaMax;

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
		
		uint32_t us_timer = micros();
		while (micros() - us_timer <= stepDelay);
	}
}

void GRGB::colorWheel(int color) {
	if (color <= 255) {						// красный макс, зелёный растёт
		_r = 255;
		_g = color;
		_b = 0;
	}
	else if (color > 255 && color <= 510) {		// зелёный макс, падает красный 
		_r = 510 - color;
		_g = 255;
		_b = 0;
	}
	else if (color > 510 && color <= 765) {		// зелёный макс, растёт синий
		_r = 0;
		_g = 255;
		_b = color - 510;
	}
	else if (color > 765 && color <= 1020) {	// синий макс, падает зелёный
		_r = 0;
		_g = 1020 - color;
		_b = 255;
	}
	else if (color > 1020 && color <= 1275) { 	// синий макс, растёт красный
		_r = color - 1020;
		_g = 0;
		_b = 255;
	}
	else if (color > 1275 && color <= 1530) {	// красный макс, падает синий
		_r = 255;
		_g = 0;
		_b = 1530 - color;
	}
	GRGB::setRGB();
}

// служебные функции
void GRGB::setRGB() {
	showR = _r;
	showG = _g;
	showB = _b;
	
	if (_brightFlag) {
		showR *= (float)_brightC;
		showG *= (float)_brightC;
		showB *= (float)_brightC;
	}
	
	if (_constBrFlag) {
		showR *= (float)_constCoef;
		showG *= (float)_constCoef;
		showB *= (float)_constCoef;
	}
	
	if (_maxCurFlag) {
		float maxColorCur = ((float)_vcc * 3.8 - 28700) / 3000;	// макс ток (ма) на один цвет при напряжении vcc
		float ledCur = (float)(showR + showG + showB) * maxColorCur / 255;	// реальный ток на один LED
		int comLedCur = ledCur * _numLeds;							// ток всей ленты
		if (comLedCur > _maxCurrent) {
			float coef = (float)_maxCurrent / comLedCur;
			showR *= (float)coef;
			showG *= (float)coef;
			showB *= (float)coef;
		}
	}
	
	if (_gammaFlag) {
		showR *= (float)_gammaR;
		showG *= (float)_gammaG;
	}
	
	if (_LUTflag) {
		showR *= (float)_rc;
		showG *= (float)_gc;
		showB *= (float)_bc;
	}
	
	if (_gammaBright) {
		showR = pgm_read_byte(&(CRTgamma[showR]));
		showG = pgm_read_byte(&(CRTgamma[showG]));
		showB = pgm_read_byte(&(CRTgamma[showB]));
	}
	
	if (_minPWMflag) {
		showR = map(showR, 0, 255, _minPWMval, 255);
		showG = map(showG, 0, 255, _minPWMval, 255);
		showB = map(showB, 0, 255, _minPWMval, 255);
	}
	
	if (_reverse_flag) {
		showR = 255 - showR;
		showG = 255 - showG;
		showB = 255 - showB;
	}
	
	if (!_PWMmode) {						// режим NORM_PWM
		if (_highFreqFlag) {
			pwmWrite(_rpin, showR);
			pwmWrite(_gpin, showG);
			pwmWrite(_bpin, showB);
		} else {
			analogWrite(_rpin, showR);
			analogWrite(_gpin, showG);
			analogWrite(_bpin, showB);
		}
		
	} else {								// режим ANY_PWM
		anyPWMRGB(_rpin, showR);
		anyPWMRGB(_gpin, showG);
		anyPWMRGB(_bpin, showB);
	}
}

// ***************************** anyPWM *****************************

void anyPWMinitRGB(byte prescaler) // 1 - 7
{
	#if defined(__AVR_ATmega328P__)
	cli();
	TCCR2A = 0;   //при совпадении уровень OC1A меняется на противоположный
	TCCR2B = 5;   //CLK
	OCR2A = 1;
	TIMSK2 = 2;   //разрешаем прерывание по совпадению
	sei();
	TCCR2B = prescaler;   // prescaler
	#endif
}

void anyPWMpinRGB(uint8_t pin) {
	#if defined(__AVR_ATmega328P__)
	anyPWMpinsRGB[pin] = 1;
	pinMode(pin, OUTPUT);
	#endif
}

void anyPWMRGB(byte pin, byte duty)
{
	pwmsRGB[pin] = duty;
}

#if (defined(__AVR_ATmega328P__) && ALLOW_ANYPWM)
ISR(TIMER2_COMPA_vect)
{
	TCNT2 = 0;
	anyPWMpinsRGB[0] && pwmsRGB[0] > pwmRGB ? PORTD |= B00000001 : PORTD &= B11111110;
	anyPWMpinsRGB[1] && pwmsRGB[1] > pwmRGB ? PORTD |= B00000010 : PORTD &= B11111101;
	anyPWMpinsRGB[2] && pwmsRGB[2] > pwmRGB ? PORTD |= B00000100 : PORTD &= B11111011;
	anyPWMpinsRGB[3] && pwmsRGB[3] > pwmRGB ? PORTD |= B00001000 : PORTD &= B11110111;
	anyPWMpinsRGB[4] && pwmsRGB[4] > pwmRGB ? PORTD |= B00010000 : PORTD &= B11101111;
	anyPWMpinsRGB[5] && pwmsRGB[5] > pwmRGB ? PORTD |= B00100000 : PORTD &= B11011111;
	anyPWMpinsRGB[6] && pwmsRGB[6] > pwmRGB ? PORTD |= B01000000 : PORTD &= B10111111;
	anyPWMpinsRGB[7] && pwmsRGB[7] > pwmRGB ? PORTD |= B10000000 : PORTD &= B01111111;
	anyPWMpinsRGB[8] && pwmsRGB[8] > pwmRGB ? PORTB |= B00000001 : PORTB &= B11111110;
	anyPWMpinsRGB[9] && pwmsRGB[9] > pwmRGB ? PORTB |= B00000010 : PORTB &= B11111101;
	anyPWMpinsRGB[10] && pwmsRGB[10] > pwmRGB ? PORTB |= B00000100 : PORTB &= B11111011;
	anyPWMpinsRGB[11] && pwmsRGB[11] > pwmRGB ? PORTB |= B00001000 : PORTB &= B11110111;
	anyPWMpinsRGB[12] && pwmsRGB[12] > pwmRGB ? PORTB |= B00010000 : PORTB &= B11101111;
	anyPWMpinsRGB[13] && pwmsRGB[13] > pwmRGB ? PORTB |= B00100000 : PORTB &= B11011111;
	anyPWMpinsRGB[14] && pwmsRGB[14] > pwmRGB ? PORTC |= B00000001 : PORTC &= B11111110;
	anyPWMpinsRGB[15] && pwmsRGB[15] > pwmRGB ? PORTC |= B00000010 : PORTC &= B11111101;
	anyPWMpinsRGB[16] && pwmsRGB[16] > pwmRGB ? PORTC |= B00000100 : PORTC &= B11111011;
	anyPWMpinsRGB[17] && pwmsRGB[17] > pwmRGB ? PORTC |= B00001000 : PORTC &= B11110111;
	anyPWMpinsRGB[18] && pwmsRGB[18] > pwmRGB ? PORTC |= B00010000 : PORTC &= B11101111;
	anyPWMpinsRGB[19] && pwmsRGB[19] > pwmRGB ? PORTC |= B00100000 : PORTC &= B11011111;

	pwmRGB++;
}
#endif