#ifndef GyverRGB_h
#define GyverRGB_h
#include <Arduino.h>

/*
	Библиотека для удобного управления RGB светодиодами и лентами
	setRGB принимает значения 0-255 для каждого цвета (красный, зелёный, синий)
	setHSV принимает 0-255 для модели HSV (цвет, насыщенность, яркость)
	setHSV_fast аналогично, но работает с целыми числами. Работает быстрее, но не так красиво как setHSV
*/

class GRGB
{
  public:
	GRGB(uint8_t r, uint8_t g, uint8_t b);
	void init();
	void setRGB(uint8_t R, uint8_t G, uint8_t B);
	void setHSV(uint8_t h, uint8_t s, uint8_t v);
	void setHSV_fast(uint8_t h, uint8_t s, uint8_t v);
  private:
	uint8_t _r, _g, _b;
};
 
#endif