/*
	microLED - ультра-лёгкая библиотека для работы с адресной лентой/матрицей
	Основная фишка: сжатие цвета, код занимает в разы меньше места в SRAM по сравнению с аналогами (FastLED, NeoPixel и др.)
	- Поддержка сжатия цвета: 8, 16 и 24 бита
	- Поддержка порядка цветов: RGB, GRB, BRG
	- Работа с цветом:
		- RGB
		- HSV
		- HEX цвета
		- "Цветовое колесо" (1500 самых ярких оттенков)
		- 16 встроенных цветов
	- Возможность чтения сжатого цвета в HEX 0xRRGGBB
	- Поддержка работы с адресными матрицами (см. пример)	
	- Поддержка чипов: 2811/2812/2813/2815
	- Частичная совместимость со скетчами для FastLED (смотри пример fastToMicro)	
	- Режим MAX_DATA_SPEED - скорость вывода увеличивается на 40%
	
	by AlexGyver & Egor 'Nich1con' Zaharov 07.10.2019
	
	Версия 1.1
	- Поправлена инициализация
	- Добавлен оранжевый цвет
	
	Версия 2.0
	- Переписан и сильно ускорен алгоритм вывода
	- Добавлено ограничение тока
	
	Версия 2.1
	- Поправлена ошибка с матрицей
	
	Версия 2.2
	- Цвет PINK заменён на MAGENTA
	
	Версия 2.3
	- Добавлена дефайн настройка MICROLED_ALLOW_INTERRUPTS
	- Исправлены мелкие ошибки, улучшена стабильность
*/

#pragma once
#include "ws2812_send.h"


#ifndef COLOR_DEBTH
#define COLOR_DEBTH 3	// по умолчанию 24 бита
#endif

#ifdef REPLACE_FASTLED
#define CRGB mRGB
#define CHSV mHSV
#endif

// ================ ДАТА =================
#if (COLOR_DEBTH == 1)
typedef uint8_t LEDdata;
#elif (COLOR_DEBTH == 2)
typedef uint16_t LEDdata;
#elif (COLOR_DEBTH == 3)
typedef struct LEDdata {
	byte r, g, b;
	// default values are UNINITIALIZED
	inline LEDdata() __attribute__((always_inline))
	{
	}
	
	// allow construction from 32-bit (really 24-bit) bit 0xRRGGBB color code
	inline LEDdata(uint32_t colorcode)  __attribute__((always_inline))
	: r((colorcode >> 16) & 0xFF), g((colorcode >> 8) & 0xFF), b((colorcode >> 0) & 0xFF)
	{
	}
	
	inline LEDdata& operator= (const uint32_t colorcode) __attribute__((always_inline))
	{
		r = (colorcode >> 16) & 0xFF;
		g = (colorcode >>  8) & 0xFF;
		b = (colorcode >>  0) & 0xFF;
		return *this;
	}
};
inline __attribute__((always_inline)) bool operator== (const LEDdata& lhs, const LEDdata& rhs) {
	return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b);
}
#endif

// ========== ПОДКЛЮЧЕНИЕ МАТРИЦЫ ==========
enum M_type {
	ZIGZAG,
	PARALLEL,
};
enum M_connection {
	LEFT_BOTTOM,
	LEFT_TOP,
	RIGHT_TOP,
	RIGHT_BOTTOM,
};
enum M_dir {
	DIR_RIGHT,
	DIR_UP,
	DIR_LEFT,
	DIR_DOWN,
};

// ========== ИНСТРУМЕНТЫ ЦВЕТА ==========
// Стандартные цвета
enum COLORS {
	WHITE =		0xFFFFFF,	// белый
	SILVER =	0xC0C0C0,	// серебро
	GRAY =		0x808080,	// серый
	BLACK =		0x000000,	// чёрный	
	RED =		0xFF0000,	// красный
	MAROON =	0x800000,	// бордовый
	ORANGE =	0xFF3000,	// оранжевый
	YELLOW =	0xFF8000,	// жёлтый
	OLIVE =		0x808000,	// олива
	LIME =		0x00FF00,	// лайм
	GREEN =		0x008000,	// зелёный
	AQUA =		0x00FFFF,	// аква
	TEAL =		0x008080,	// цвет головы утки чирка
	BLUE =		0x0000FF,	// голубой
	NAVY =		0x000080,	// тёмно-синий
	MAGENTA =	0xFF00FF,	// розовый
	PURPLE =	0x800080,	// пурпурный
};

// Функции
LEDdata mRGB(byte r, byte g, byte b);	// RGB 255, 255, 255
LEDdata mWHEEL(int color);				// цвета 0-1530
LEDdata mHEX(uint32_t color);			// HEX цвет
LEDdata mHSV(byte h, byte s, byte v);	// HSV 255, 255, 255
LEDdata mCOLOR(COLORS color);			// цвет

// =================== КЛАСС ===================
class microLED {
public:
	// ЛЕНТА: имя буфера, количество ледов, пин
	microLED(LEDdata *LEDarray, int LEDamount, byte pin);
	
	// МАТРИЦА: имя буфера, пин, ширина матрицы, высота матрицы, тип матрицы, угол подключения, направление	(см. ПОДКЛЮЧЕНИЕ МАТРИЦЫ)
	microLED(LEDdata *LEDarray, byte pin, byte width, byte height, M_type type, M_connection conn, M_dir dir);
	
	// лента и матрица
	void setRGB(int num, byte r, byte g, byte b);	// RGB
	void setHSV(int num, byte h, byte s, byte v); 	// HSV
	void setHEX(int num, uint32_t color);			// HEX
	void setColor(int num, COLORS color);			// стандартный цвет (см. "Стандартные цвета")
	void colorWheel(int num, int color);			// цвет 0-1530	
	void fill(LEDdata color);						// заливка цветом (mRGB, mWHEEL, mHEX, mHSV)
	void setLED(int n, LEDdata color);				// ставим цвет светодиода (mRGB, mWHEEL, mHEX, mHSV)	
	uint32_t getColorHEX(int num);					// получить HEX цвет диода (для сравнения и т.п.)
	LEDdata getColor(int num);						// получить цвет диода в LEDdata
	void fade(int num, byte val);					// уменьшить яркость на val
	
	void setBrightness(byte newBright);				// яркость 0-255
	void clear();									// очистка
	void show();									// отправка
	
	// матрица
	void setPix(int x, int y, LEDdata color);		// ставим цвет пикселя x y в LEDdata (mRGB, mWHEEL, mHEX, mHSV)
	uint32_t getColorHEX(int x, int y);				// получить цвет пикселя в HEX
	LEDdata getColor(int x, int y);					// получить цвет пикселя в LEDdata
	void fadePix(int x, int y, byte val);			// уменьшить яркость пикселя на val
	uint16_t getPixNumber(int x, int y);			// получить номер пикселя в ленте по координатам
	
	void setVoltage(int mv);						// установить напряжение питания в мв, по умолч. 5000 (для расчёта тока)
	void setMaxCurrent(int ma);						// установить максимальный ток (автокоррекция яркости). 0 - выключено
	
private:
	void setCurrentSettings();
	byte correctBright();
	void getColorPtr(int num, byte *ptr);
	int _numLEDs;
	LEDdata *LEDbuffer;
	byte _bright = 0;
	byte _matrixConfig = 0;
	byte _matrixType = 0;
	byte _matrixW;
	byte _width, _height;
	int _maxCurrent = 0;
	int _voltage = 5000;
	int _idleCurrent = 0;
	byte _oneLedCurrent = 0;
	
	const volatile uint8_t *ws2812_port;
	volatile uint8_t *ws2812_port_reg;
	uint8_t pinMask;
};

// ================== МЕТОДЫ ==================
microLED::microLED(LEDdata *LEDarray, int LEDamount, byte pin) {
	_numLEDs = LEDamount;
	LEDbuffer = LEDarray;
	
	pinMask = digitalPinToBitMask(pin);
	ws2812_port = portOutputRegister(digitalPinToPort(pin));
	ws2812_port_reg = portModeRegister(digitalPinToPort(pin));
}

microLED::microLED(LEDdata *LEDarray, byte pin, byte width, byte height, M_type type, M_connection conn, M_dir dir) {
	_numLEDs = width * height;
	LEDbuffer = LEDarray;
	
	pinMask = digitalPinToBitMask(pin);
	ws2812_port = portOutputRegister(digitalPinToPort(pin));
	ws2812_port_reg = portModeRegister(digitalPinToPort(pin));
	
	_width = width;
	_height = height;
	
	_matrixConfig = (byte)conn | ((byte)dir << 2);
	_matrixType = (byte)type;
	
	if (_matrixConfig == 4 || _matrixConfig == 13 || _matrixConfig == 14 || _matrixConfig == 7)
	_matrixW = height;
	else
	_matrixW = width;	
}

void microLED::setRGB(int num, byte r, byte g, byte b) {
	LEDbuffer[num] = mRGB(r, g, b);
}

void microLED::setHSV(int num, byte h, byte s, byte v) {
	LEDbuffer[num] = mHSV(h, s, v);
}

void microLED::setHEX(int num, uint32_t color) {
	LEDbuffer[num] = mHEX(color);
}

void microLED::colorWheel(int num, int color) {
	LEDbuffer[num] = mWHEEL(color);
}

void microLED::setColor(int num, COLORS color) {
	LEDbuffer[num] = mCOLOR(color);
}

void microLED::setBrightness(byte newBright) {
#if (COLOR_DEBTH == 1)
	_bright = (255 - newBright) >> 5;
#else
	_bright = newBright;
#endif
}

void microLED::clear() {
	for (int i = 0; i < _numLEDs; i++) {
#if (COLOR_DEBTH == 1)
		LEDbuffer[i] = 0;
#elif (COLOR_DEBTH == 2)
		LEDbuffer[i] = 0;
#elif (COLOR_DEBTH == 3)
		LEDbuffer[i].r = 0;
		LEDbuffer[i].g = 0;
		LEDbuffer[i].b = 0;
#endif
	}
}

void microLED::fill(LEDdata color) {
	for (int i = 0; i < _numLEDs; i++) {
		LEDbuffer[i] = color;
	}
}

void microLED::setLED(int n, LEDdata color) {
	LEDbuffer[n] = color;
}

uint32_t microLED::getColorHEX(int num) {
	byte thisData[3];
	getColorPtr(num, thisData);
	return (((uint32_t)thisData[0] << 16) | ((uint32_t)thisData[1] << 8 ) | (uint32_t)thisData[2]);
}

LEDdata microLED::getColor(int num) {
	return LEDbuffer[num];
}

void microLED::getColorPtr(int num, byte *ptr) {
#if (COLOR_DEBTH == 1)
	ptr[0] = (LEDbuffer[num] & 0b11000000);
	ptr[1] = ((LEDbuffer[num] & 0b00111000) << 2);
	ptr[2] = ((LEDbuffer[num] & 0b00000111) << 5);
#elif (COLOR_DEBTH == 2)
	ptr[0] = (((LEDbuffer[num] & 0b1111100000000000) >> 8));
	ptr[1] = (((LEDbuffer[num] & 0b0000011111100000) >> 3));
	ptr[2] = (((LEDbuffer[num] & 0b0000000000011111) << 3));
#elif (COLOR_DEBTH == 3)	
	ptr[0] = LEDbuffer[num].r;
	ptr[1] = LEDbuffer[num].g;
	ptr[2] = LEDbuffer[num].b;
#endif
}

void microLED::fade(int num, byte val) {
	if (LEDbuffer[num] == LEDdata(0)) return;
	byte rgb[3];
	getColorPtr(num, rgb);

	byte maximum = max(max(rgb[0], rgb[1]), rgb[2]);
	float coef = 0;

	if (maximum >= val) {
		coef = (float)(maximum - val) / maximum;
	} else {
		LEDbuffer[num] = 0;
		return;
	}

	for (byte i = 0; i < 3; i++) {
		rgb[i] *= coef;
	}
	LEDbuffer[num] = mRGB(rgb[0], rgb[1], rgb[2]);
}

void microLED::fadePix(int x, int y, byte val) {
	fade(getPixNumber(x, y), val);
}

// ====================== МАТРИЦА ======================
uint16_t microLED::getPixNumber(int x, int y) {
	int thisX, thisY;
	switch(_matrixConfig) {
	case 0:
		thisX = x;
		thisY = y;
		break;
	case 4:
		thisX = y;
		thisY = x;
		break;
	case 1:
		thisX = x;
		thisY = (_height - y - 1);
		break;
	case 13:
		thisX = (_height - y - 1);
		thisY = x;
		break;
	case 10:
		thisX = (_width - x - 1);
		thisY = (_height - y - 1);
		break;
	case 14:
		thisX = (_height - y - 1);
		thisY = (_width - x - 1);
		break;
	case 11:
		thisX = (_width - x - 1);
		thisY = y;
		break;
	case 7:
		thisX = y;
		thisY = (_width - x - 1);
		break;
	}
	
	if (_matrixType || !(thisY % 2)) {               // если чётная строка
		return (thisY * _matrixW + thisX);
	} else {                                              // если нечётная строка
		return (thisY * _matrixW + _matrixW - thisX - 1);
	}
}

void microLED::setPix(int x, int y, LEDdata color) {
	LEDbuffer[getPixNumber(x, y)] = color;
}

uint32_t microLED::getColorHEX(int x, int y) {
	return getColorHEX(getPixNumber(x, y));
}

LEDdata microLED::getColor(int x, int y) {
	return LEDbuffer[getPixNumber(x, y)];
}

// ======================УТИЛИТЫ =====================
#if (COLOR_DEBTH == 2)
#define PTR_TYPE uint16_t*
#else
#define PTR_TYPE uint8_t*
#endif

void microLED::setVoltage(int mv) {
	_voltage = mv;
	setCurrentSettings();
}
void microLED::setMaxCurrent(int ma) {
	_maxCurrent = ma;
	setCurrentSettings();
}

void microLED::setCurrentSettings() {
	_idleCurrent = (float)_numLEDs * map(_voltage, 3000, 6000, 184, 934) / 1000L;
	_oneLedCurrent = map(_voltage, 3000, 6000, 9, 12);
}

byte microLED::correctBright() {
	if (_maxCurrent == 0) return _bright;
	else {
		long sum = 0;
		for (int i = 0; i < _numLEDs; i++) {
#if (COLOR_DEBTH == 1)
			// 8 бит
			sum += (LEDbuffer[i] & 0b11000000) >> _bright;
			sum += ((LEDbuffer[i] & 0b00111000) << 2) >> _bright;
			sum += ((LEDbuffer[i] & 0b00000111) << 5) >> _bright;
#elif (COLOR_DEBTH == 2)
			// 16 бит
			sum += (((LEDbuffer[i] & 0b1111100000000000) >> 8) * _bright) >> 8;
			sum += (((LEDbuffer[i] & 0b0000011111100000) >> 3) * _bright) >> 8;
			sum += (((LEDbuffer[i] & 0b0000000000011111) << 3) * _bright) >> 8;
#elif (COLOR_DEBTH == 3)
			// 32 бит		
			sum += ((long)LEDbuffer[i].r * _bright) >> 8;
			sum += ((long)LEDbuffer[i].g * _bright) >> 8;
			sum += ((long)LEDbuffer[i].b * _bright) >> 8;
#endif
		}		
		sum = (float)sum / 255 * _oneLedCurrent;		// текущий ток (* макс ток одного канала от напряжения)
		int realMax = _maxCurrent - _idleCurrent;		// макс. ток - минус холостой ток (от напряжения)
		if (sum < realMax) return _bright;
		else return ((float)realMax / sum * _bright);		
	}
}

// ====================== ВЫВОД ======================
void microLED::show() {
	*ws2812_port_reg |= pinMask; // Enable DDR
	WS2812B_sendData((PTR_TYPE)LEDbuffer, (int16_t)COLOR_DEBTH * _numLEDs, pinMask, (uint8_t*) ws2812_port, (uint8_t*) ws2812_port_reg, correctBright());
}

// ================== COLOR UTILITY ===================
LEDdata mRGB(byte r, byte g, byte b) {
#if (COLOR_DEBTH == 1)
	return ( (r & 0b11000000) | ((g & 0b11100000) >> 2) | (b & 0b11100000) >> 5);
#elif (COLOR_DEBTH == 2)
	return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | ((b & 0b11111000) >> 3);
#elif (COLOR_DEBTH == 3)
	LEDdata bufferData;
	bufferData.r = r;
	bufferData.g = g;
	bufferData.b = b;
	return bufferData;
#endif
}

LEDdata mHSV(byte h, byte s, byte v) {	
#if (COLOR_DEBTH == 1)
	// быстрый HSV
	byte r, g, b;
	byte value = ((24 * h / 17) / 60) % 6;
	byte vmin = (long)v - v * s / 255;
	byte a = (long)v * s / 255 * (h * 24 / 17 % 60) / 60;
	byte vinc = vmin + a;
	byte vdec = v - a;
	switch (value) {
	case 0: r = v; g = vinc; b = vmin; break;
	case 1: r = vdec; g = v; b = vmin; break;
	case 2: r = vmin; g = v; b = vinc; break;
	case 3: r = vmin; g = vdec; b = v; break;
	case 4: r = vinc; g = vmin; b = v; break;
	case 5: r = v; g = vmin; b = vdec; break;
	}
	return mRGB(r, g, b);
#else
	// обычный HSV
	float r, g, b;
	
	float H = (float)h / 255;
	float S = (float)s / 255;
	float V = (float)v / 255;

	int i = int(H * 6);
	float f = H * 6 - i;
	float p = V * (1 - S);
	float q = V * (1 - f * S);
	float t = V * (1 - (1 - f) * S);

	switch (i % 6) {
	case 0: r = V, g = t, b = p; break;
	case 1: r = q, g = V, b = p; break;
	case 2: r = p, g = V, b = t; break;
	case 3: r = p, g = q, b = V; break;
	case 4: r = t, g = p, b = V; break;
	case 5: r = V, g = p, b = q; break;	
	}
	return mRGB(r * 255, g * 255, b * 255);
#endif
}

LEDdata mHEX(uint32_t color) {
	return mRGB( (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF );
}

LEDdata mWHEEL(int color) {
	byte _r, _g, _b;
	if (color <= 255) {           // красный макс, зелёный растёт
		_r = 255;
		_g = color;
		_b = 0;
	}
	else if (color > 255 && color <= 510) {   // зелёный макс, падает красный
		_r = 510 - color;
		_g = 255;
		_b = 0;
	}
	else if (color > 510 && color <= 765) {   // зелёный макс, растёт синий
		_r = 0;
		_g = 255;
		_b = color - 510;
	}
	else if (color > 765 && color <= 1020) {  // синий макс, падает зелёный
		_r = 0;
		_g = 1020 - color;
		_b = 255;
	}
	else if (color > 1020 && color <= 1275) {   // синий макс, растёт красный
		_r = color - 1020;
		_g = 0;
		_b = 255;
	}
	else if (color > 1275 && color <= 1530) { // красный макс, падает синий
		_r = 255;
		_g = 0;
		_b = 1530 - color;
	}
	return mRGB(_r, _g, _b);
}

LEDdata mCOLOR(COLORS color) {
	return mHEX(color);
}