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
	void reverse(boolean rev);
	void setColor(uint32_t color);
	void setRGB(uint8_t R, uint8_t G, uint8_t B);
	void setHSV(uint8_t h, uint8_t s, uint8_t v);
	void setHSV_fast(uint8_t h, uint8_t s, uint8_t v);
  private:
	uint8_t _r, _g, _b;
	boolean _reverse_flag;
};

#define G_WHITE		0xFFFFFF
#define G_SILVER	0xC0C0C0
#define G_GRAY		0x808080
#define G_BLACK		0x000000
#define G_RED		0xFF0000
#define G_MAROON	0x800000
#define G_YELLOW	0xFFFF00
#define G_OLIVE		0x808000
#define G_LIME		0x00FF00
#define G_GREEN		0x008000
#define G_AQUA		0x00FFFF
#define G_TEAL		0x008080
#define G_BLUE		0x0000FF
#define G_NAVY		0x000080
#define G_PINK		0xFF00FF
#define G_PURPLE	0x800080

#endif