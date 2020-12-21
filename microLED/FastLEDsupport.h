#ifndef FastLEDsupport_h
#define FastLEDsupport_h
// поддержка типа данных FastLED для microLED

#include <FastLED.h>
#include "microLED.h"

mData CRGBtoData(CRGB col) {
#if (COLOR_DEBTH == 1)
	return RGBto8(col.r, col.g, col.b);
#elif (COLOR_DEBTH == 2)
	return RGBto16(col.r, col.g, col.b);
#elif (COLOR_DEBTH == 3)
	return mData(col.r, col.g, col.b);
#endif
}
#endif