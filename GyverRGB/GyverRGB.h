#ifndef GyverRGB_h
#define GyverRGB_h
#include <Arduino.h>
#define LIBRARY_VERSION	1.6

/*
	Версия 1.6 от 24.09.2018
	Библиотека для удобного управления RGB светодиодами и лентами
	- Работа в пространстве RGB
	- Работа в пространстве HSV
	- Установка цвета в формате HEX
	- 16 предустановленных цветов
	- Настройка полярности ШИМ	
	- Функция плавной смены цвета
	- Возможность управления 6-ю RGB диодами/лентами с одной Arduino
		(встроенный генератор ШИМ на ВСЕХ 20 пинах atmega328)
*/

class GRGB
{
  public:
	GRGB(uint8_t rpin, uint8_t gpin, uint8_t bpin);						// объявление
	
	GRGB(uint8_t rpin, uint8_t gpin, uint8_t bpin, boolean pwmmode);	// объявление с выбором режима генерации ШИМ (NORM_PWM / ANY_PWM)
																		// NORM_PWM - дефолтные ШИМ пины (3, 5, 6, 9, 10, 11 для UNO/NANO/MINI)
																		// ANY_PWM - любой пин делается ШИМ пином (частота ~150 Гц). Подробности в библиотеке GyverHacks
	
	void setDirection(boolean direction);				// NORMAL / REVERSE - направление ШИМ
														// общий катод - NORMAL
														// общий анод - REVERSE
													
	void setHEX(uint32_t color);						// установка цвета в формате HEX (вида 0x808080 )
	void setRGB(uint8_t r, uint8_t g, uint8_t b);		// установка цвета в пространстве RGB (каждый цвет 0-255)
	void setHSV(uint8_t h, uint8_t s, uint8_t v);		// установка цвета в пространстве HSV (каждая велиична 0-255)
	void setHSV_fast(uint8_t h, uint8_t s, uint8_t v);	// более быстрый, но менее красивый вариант предыдущей функции
	
	// плавно изменить текущий цвет к новому за вермя fadeTime в миллисекундах
	// для HEX цвета
	void fadeTo(uint32_t newColor, uint16_t fadeTime);
	
	// для R G B
	void fadeTo(uint8_t new_r, uint8_t new_g, uint8_t new_b, uint16_t fadeTime);

	
  private:
	void setRGB();
	uint8_t _rpin, _gpin, _bpin;	// пины
	uint8_t _r, _g, _b;				// цвета
	boolean _reverse_flag;
	boolean _PWMmode;
};

void anyPWMinitRGB(byte prescaler);									
void anyPWMpinRGB(uint8_t pin);
void anyPWMRGB(byte pin, byte duty);

#define NORMAL 0
#define REVERSE 1
#define NORM_PWM 0
#define ANY_PWM 1

#define WHITE		0xFFFFFF	// белый
#define SILVER		0xC0C0C0	// серебро
#define GRAY		0x808080	// серый
#define BLACK		0x000000	// чёрный
#define RED			0xFF0000	// красный
#define MAROON		0x800000	// бордовый
#define YELLOW		0xFFFF00	// жёлтый
#define OLIVE		0x808000	// олива
#define LIME		0x00FF00	// лайм
#define GREEN		0x008000	// зелёный
#define AQUA		0x00FFFF	// аква
#define TEAL		0x008080	// цвет головы утки чирка https://odesign.ru/teal-color/
#define BLUE		0x0000FF	// голубой
#define NAVY		0x000080	// тёмно-синий
#define PINK		0xFF00FF	// розовый
#define PURPLE		0x800080	// пурпурный

#endif