#pragma once
// поддержка типа данных FastLED для microLED

#include <FastLED.h>
#include "microLED.h"

mData CRGBtoData(CRGB col) {
#if (COLOR_DEBTH == 1)
	return RGB24to8(((uint32_t)col.r << 16) | (col.g << 8) | col.b);
#elif (COLOR_DEBTH == 2)
	return RGB24to16(((uint32_t)col.r << 16) | (col.g << 8) | col.b);
#elif (COLOR_DEBTH == 3)
	mData buf;
	buf.r = col.r;
	buf.g = col.g;
	buf.b = col.b;
	return buf;
#endif
}