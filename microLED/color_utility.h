#pragma once
#define MICROLED_INLINE __attribute__((always_inline))

// ============================================== ДАТА ==============================================
#ifndef COLOR_DEBTH
#define COLOR_DEBTH 3
#endif

#if (COLOR_DEBTH == 1)
//#pragma message "Color debth 1 bit"
typedef uint8_t mData;
#elif (COLOR_DEBTH == 2)
//#pragma message "Color debth 2 bit"
typedef uint16_t mData;
#elif (COLOR_DEBTH == 3)
//#pragma message "Color debth 3 bit"
struct mData {
	uint8_t r, g, b;
	inline mData() MICROLED_INLINE {}
	inline mData(uint8_t _r, uint8_t _g, uint8_t _b) MICROLED_INLINE :r(_r), g(_g), b(_b) {}
	inline mData(uint32_t colorcode)  MICROLED_INLINE
	: r(((uint32_t)colorcode >> 16) & 0xFF), g(((uint32_t)colorcode >> 8) & 0xFF), b(colorcode & 0xFF){}	
	inline mData& operator= (const uint32_t colorcode) MICROLED_INLINE {
		r = ((uint32_t)colorcode >> 16) & 0xFF;
		g = ((uint32_t)colorcode >>  8) & 0xFF;
		b = colorcode & 0xFF;
		return *this;
	}		
};
inline MICROLED_INLINE bool operator== (const mData& lhs, const mData& rhs) {
	return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b);
}
inline MICROLED_INLINE bool operator!= (const mData& lhs, const mData& rhs) {
	return (lhs.r != rhs.r) || (lhs.g != rhs.g) || (lhs.b != rhs.b);
}
#endif

// ============================================ СТАНДАРТНЫЕ ЦВЕТА =============================================
enum COLORS {
	#if (COLOR_DEBTH == 3)
	mWhite =	0xFFFFFF,	// белый
	mSilver =	0xC0C0C0,	// серебро
	mGray =		0x808080,	// серый
	mBlack =	0x000000,	// чёрный	
	mRed =		0xFF0000,	// красный
	mMaroon =	0x800000,	// бордовый
	mOrange =	0xFF3000,	// оранжевый
	mYellow =	0xFF8000,	// жёлтый
	mOlive =	0x808000,	// олива
	mLime =		0x00FF00,	// лайм
	mGreen =	0x008000,	// зелёный
	mAqua =		0x00FFFF,	// аква
	mTeal =		0x008080,	// цвет головы утки чирка
	mBlue =		0x0000FF,	// голубой
	mNavy =		0x000080,	// тёмно-синий
	mMagenta =	0xFF00FF,	// розовый
	mPurple =	0x800080,	// пурпурный
	#elif (COLOR_DEBTH == 2)
	mWhite =	0xFFFF,		// белый
	mSilver =	0xC618,		// серебро
	mGray =		0x8410,		// серый
	mBlack =	0x0,		// чёрный	
	mRed =		0xF800,		// красный
	mMaroon =	0x8000,		// бордовый
	mOrange =	0xF980,		// оранжевый
	mYellow =	0xFC00,		// жёлтый
	mOlive =	0x8400,		// олива
	mLime =		0x7E0,		// лайм
	mGreen =	0x400,		// зелёный
	mAqua =		0x7FF,		// аква
	mTeal =		0x410,		// цвет головы утки чирка
	mBlue =		0x1F,		// голубой
	mNavy =		0x10,		// тёмно-синий
	mMagenta =	0xF81F,		// розовый
	mPurple =	0x8010,		// пурпурный
	#elif (COLOR_DEBTH == 1)
	mWhite =	0xFF,		// белый
	mSilver =	0xF6,		// серебро
	mGray =		0xA4,		// серый
	mBlack =	0x0,		// чёрный	
	mRed =		0xC0,		// красный
	mMaroon =	0x80,		// бордовый
	mOrange =	0xC8,		// оранжевый
	mYellow =	0xE0,		// жёлтый
	mOlive =	0xA0,		// олива
	mLime =		0x38,		// лайм
	mGreen =	0x20,		// зелёный
	mAqua =		0x3F,		// аква
	mTeal =		0x24,		// цвет головы утки чирка
	mBlue =		0x7,		// голубой
	mNavy =		0x4,		// тёмно-синий
	mMagenta =	0xC7,		// розовый
	mPurple =	0x84,		// пурпурный
	#endif
};

uint32_t getHEX(mData data);						// перепаковать в 24 бит HEX
mData getFade(mData data, uint8_t val);				// уменьшить яркость на val
mData getBlend(int x, int amount, mData c0, mData c1);	// получить промежуточный цвет

mData mRGB(uint8_t r, uint8_t g, uint8_t b);		// RGB 255, 255, 255
mData mWheel(int color, uint8_t bright=255);		// цвета 0-1530 + яркость 
mData mWheel8(uint8_t color, uint8_t bright=255);	// цвета 0-255 + яркость
mData mHEX(uint32_t color);							// mHEX цвет
mData mHSV(uint8_t h, uint8_t s, uint8_t v);		// HSV 255, 255, 255
mData mHSVfast(uint8_t h, uint8_t s, uint8_t v);	// HSV 255, 255, 255
mData mKelvin(int kelvin);							// температура

// ============================================ CRT GAMMA =============================================
#define getCRT_PGM(x) (pgm_read_byte(&_CRTgammaPGM[x]))
#define getCRT_SQUARE(x) (((long)(x) * (x) + 255) >> 8)
#define getCRT_CUBIC(x) (((long)(x) * (x) * (x) + 65535) >> 16)

// по умолчанию CRT_PGM
#if !defined(CRT_PGM) && !defined(CRT_SQUARE) && !defined(CRT_CUBIC) && !defined(CRT_OFF)
#define CRT_PGM
#endif

#if defined(CRT_PGM)
//#pragma message "CRT PGM"
#define getCRT(x) getCRT_PGM(x)
static const uint8_t _CRTgammaPGM[256] PROGMEM = {
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5,
	5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8,
	9, 10, 10, 10, 10, 11, 11, 12, 12, 12, 12, 13, 13, 13, 14, 14,
	15, 15, 15, 16, 17, 17, 17, 17, 18, 18, 19, 20, 20, 20, 20, 21,
	22, 22, 23, 23, 23, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 30,
	30, 31, 31, 32, 33, 33, 34, 35, 35, 36, 37, 37, 38, 38, 39, 40,
	41, 41, 42, 43, 44, 45, 45, 46, 47, 47, 48, 49, 50, 51, 52, 53,
	54, 54, 55, 56, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
	68, 69, 69, 70, 71, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 84,
	85, 86, 87, 89, 90, 91, 93, 94, 95, 96, 97, 98, 100, 101, 102, 103,
	105, 106, 108, 109, 110, 111, 113, 115, 117, 118, 119, 121, 122, 123, 124, 127,
	128, 130, 131, 133, 134, 136, 137, 139, 140, 143, 145, 146, 147, 148, 151, 153,
	154, 156, 158, 159, 162, 163, 165, 167, 169, 171, 173, 174, 176, 179, 180, 182,
	185, 186, 188, 190, 192, 194, 196, 199, 201, 202, 205, 207, 209, 211, 214, 216,
	218, 220, 223, 225, 226, 230, 231, 235, 236, 240, 241, 245, 246, 250, 251, 255,
};

#elif defined(CRT_SQUARE)
//#pragma message "CRT SQUARE"
#define getCRT(x) getCRT_SQUARE(x)

#elif defined(CRT_CUBIC)
//#pragma message "CRT CUBIC"
#define getCRT(x) getCRT_CUBIC(x)

#elif defined(CRT_OFF)
//#pragma message "CRT OFF"
#define getCRT(x) (x)

#endif


// ============================================== COLOR MACRO ===============================================
// склейка/расклейка ргб-инт
#define RGBto24(r,g,b) ( ((uint32_t)(r) << 16) | ((g) << 8 ) | (b) )
#define RGBto16(r,g,b) ( (((r) & 0b11111000) << 8) | (((g) & 0b11111100) << 3) | (((b) & 0b11111000) >> 3) )
#define RGBto8(r,g,b) ( ((r) & 0b11100000) | (((g) & 0b11000000) >> 3) | (((b) & 0b11100000) >> 5) )

#define RGB24toR(x) (((uint32_t)(x) >> 16) & 0xFF)
#define RGB24toG(x) (((uint32_t)(x) >> 8) & 0xFF)
#define RGB24toB(x) ((uint32_t)(x) & 0xFF)

// rrrrrrrr gggggggg bbbbbbbb
// rrrrrggg gggbbbbb
// rrrggbbb
#define RGB888to565(x) ( (((x) & 0xF80000) >> 8) | (((x) & 0xFC00) >> 5) | (((x) & 0xF8) >> 3) )
#define RGB888to323(x) ( (((x) & 0xE00000) >> 16) | (((x) & 0xC000) >> 11) | (((x) & 0xE0) >> 5) )
#define RGB565to888(x) ( (((x) & 0xF800) << 8) | (((x) & 0x7E0) << 5) | (((x) & 0x1F) << 3) )
#define RGB323to888(x) ( (((x) & 0xE0) << 16) | (((x) & 0x18) << 11) | (((x) & 0x7) << 5) )

// дубли
#define RGB24to16(x) RGB888to565(x)
#define RGB24to8(x) RGB888to323(x)
#define RGB16to24(x) RGB565to888(x)
#define RGB8to24(x) RGB323to888(x)

// макросы распаковки и упаковки цветов
#if (COLOR_DEBTH == 1)
#define getR(x)			((x) & 0b11000000)
#define getG(x)			(((x) & 0b00111000) << 2)
#define getB(x)			(((x) & 0b00000111) << 5)
#define mergeRGB(r,g,b)	(((getCRT(r) & 0b11000000) | ((getCRT(g) & 0b11100000) >> 2) | (getCRT(b) & 0b11100000) >> 5))
#define mergeRGBraw(r,g,b)	((((r) & 0b11000000) | (((g) & 0b11100000) >> 2) | ((b) & 0b11100000) >> 5))
#elif (COLOR_DEBTH == 2)
#define getR(x)			(((x) & 0b1111100000000000) >> 8)
#define getG(x)			(((x) & 0b0000011111100000) >> 3)
#define getB(x)			(((x) & 0b0000000000011111) << 3)
#define mergeRGB(r,g,b)	(((getCRT(r) & 0b11111000) << 8) | ((getCRT(g) & 0b11111100) << 3) | ((getCRT(b) & 0b11111000) >> 3))
#define mergeRGBraw(r,g,b)	((((r) & 0b11111000) << 8) | (((g) & 0b11111100) << 3) | (((b) & 0b11111000) >> 3))
#elif (COLOR_DEBTH == 3)	
#define getR(x)			(x.r)
#define getG(x)			(x.g)
#define getB(x)			(x.b)
#define mergeRGB(r,g,b)	mData(getCRT(r), getCRT(g), getCRT(b))
#define mergeRGBraw(r,g,b)	mData((r), (g), (b))
#endif

// макросы уменьшения яркости
#define fade8(x, b) 	(((uint16_t)(x) * ((b) + 1)) >> 8)
#define fade8R(x, b) 	fade8(getR(x), (b))
#define fade8G(x, b) 	fade8(getG(x), (b))
#define fade8B(x, b) 	fade8(getB(x), (b))

// ============================================== COLOR FUNC ===============================================
mData getFade(mData data, uint8_t val) {
	if (data == 0) return 0;
	val = 255 - val;
	return mergeRGBraw(fade8R(data, val), fade8G(data, val), fade8B(data, val));
}

uint32_t getHEX(mData data) {
	return RGBto24(getR(data), getG(data), getB(data));
}

mData getBlend(int x, int amount, mData c0, mData c1) {
	while (x >= amount) x -= amount;
	amount -= 1;
	return mergeRGBraw(
	getR(c0) + (getR(c1) - getR(c0)) * x / amount,
	getG(c0) + (getG(c1) - getG(c0)) * x / amount,
	getB(c0) + (getB(c1) - getB(c0)) * x / amount
	);
}

mData mRGB(uint8_t r, uint8_t g, uint8_t b) {
	return mergeRGB(r, g, b);
}

mData mHSVfast(uint8_t h, uint8_t s, uint8_t v) {	
	// быстрый HSV
	uint8_t r, g, b;
	uint8_t value = ((24 * h / 17) / 60) % 6;
	uint8_t vmin = (long)v - v * s / 255;
	uint8_t a = (long)v * s / 255 * (h * 24 / 17 % 60) / 60;
	uint8_t vinc = vmin + a;
	uint8_t vdec = v - a;
	switch (value) {
	case 0: r = v; g = vinc; b = vmin; break;
	case 1: r = vdec; g = v; b = vmin; break;
	case 2: r = vmin; g = v; b = vinc; break;
	case 3: r = vmin; g = vdec; b = v; break;
	case 4: r = vinc; g = vmin; b = v; break;
	case 5: r = v; g = vmin; b = vdec; break;
	}
	return mRGB(r, g, b);
}

mData mHSV(uint8_t h, uint8_t s, uint8_t v) {	
	// обычный HSV
	float r, g, b;
	
	float H = h / 255.0;
	float S = s / 255.0;
	float V = v / 255.0;
	
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
	r *= 255.0;
	g *= 255.0;
	b *= 255.0;
	return mRGB((byte)r, (byte)g, (byte)b);
}

mData mHEX(uint32_t color) {
	return mRGB(RGB24toR(color), RGB24toG(color), RGB24toB(color));
}

mData mWheel(int color, uint8_t bright) {
	uint8_t r, g, b;
	if (color <= 255) {           			  // красный макс, зелёный растёт
		r = 255;
		g = color;
		b = 0;
	}
	else if (color > 255 && color <= 510) {   // зелёный макс, падает красный
		r = 510 - color;
		g = 255;
		b = 0;
	}
	else if (color > 510 && color <= 765) {   // зелёный макс, растёт синий
		r = 0;
		g = 255;
		b = color - 510;
	}
	else if (color > 765 && color <= 1020) {  // синий макс, падает зелёный
		r = 0;
		g = 1020 - color;
		b = 255;
	}
	else if (color > 1020 && color <= 1275) { // синий макс, растёт красный
		r = color - 1020;
		g = 0;
		b = 255;
	}
	else if (color > 1275 && color <= 1530) { // красный макс, падает синий
		r = 255;
		g = 0;
		b = 1530 - color;
	}
	if (bright != 255) {
		r = fade8(r, bright);
		g = fade8(g, bright);
		b = fade8(b, bright);
	}
	return mRGB(r, g, b);
}

mData mWheel8(uint8_t color, uint8_t bright) {
	uint8_t r, g, b;
	byte shift;
	if (color > 170) {
		shift = (color - 170) * 3;
		r = shift;
		g = 0;
		b = 255 - shift;
	} else if (color > 85) {
		shift = (color - 85) * 3;
		r = 0;
		g = 255 - shift;
		b = shift;
	} else {
		shift = color * 3;
		r = 255 - shift;
		g = shift;
		b = 0;
	}
	if (bright != 255) {
		r = fade8(r, bright);
		g = fade8(g, bright);
		b = fade8(b, bright);
	}
	return mRGB(r, g, b);
}

mData mKelvin(int kelvin) {
	float tmpKelvin, tmpCalc;
	uint8_t _r, _g, _b;
	
	kelvin = constrain(kelvin, 1000, 40000);
	tmpKelvin = kelvin / 100;
	
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
	return mRGB(_r, _g, _b);
}

// ============================================ GRADIENT =============================================
template <int size>
struct mGradient {
	mData colors[size];
	mData get(int x, int amount) {
		int sectorSize = (amount + size - 2) / (size - 1); // (x+y-1)/y
		int sector = x / sectorSize;
		return getBlend(x-sector*sectorSize, sectorSize, colors[sector], colors[sector+1]);
	}
};