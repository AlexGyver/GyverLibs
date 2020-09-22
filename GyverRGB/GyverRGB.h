#ifndef GyverRGB_h
#define GyverRGB_h
#include <Arduino.h>
#include "PWM.h"

#define ALLOW_ANYPWM 0		// (0 / 1) - включить или отключить ANYPWM
// необходимо отключить, если этот режим не нужен и вы сами используете прерывания timer2 (COMPA_vect)

/*	
	Библиотека для удобного управления RGB светодиодами и лентами
	Документация: https://alexgyver.ru/gyverrgb-lib/
	- 1530 значений для colorWheel
	- Работа в пространстве RGB
	- Работа в пространстве HSV
	- Установка цвета в формате HEX
	- Установка цветовой температуры
	- 16 предустановленных цветов 
	- Настройка полярности ШИМ	
	- Функция плавной смены цвета
	- Ограничение тока (по расчёту)
	- Регулировка общей яркости
	- Поддержание яркости LED ленты по мере разряда АКБ
	- Возможность управления 6-ю RGB диодами/лентами с одной Arduino (только для ATmega328)
		(встроенный генератор ШИМ на ВСЕХ 20 пинах atmega328)
		
	Версия 1.13 от 08.06.2019
	- Добавлен режим с настройкой частоты ШИМ
	- Добавлена матрица коррекции LUT
	- Добавлено коррекция по минимальному сигналу ШИМ
	- Добавлена гамма-коррекция яркости
	
	Версия 1.14 от 09.09.2019
	- Убраны дефайны
	
	Версия 1.15 от 16.09.2019
	- Возвращены дефайны
	
	Версия 1.16 от 15.12.2019
	- Исправлен баг в fadeTo
	
	Версия 1.17 от 11.03.2020
	- Добавлена перегруженная setHEX
*/


#define NORMAL 0
#define REVERSE 1
#define NORM_PWM 0
#define ANY_PWM 1

enum colors {
	WHITE =		0xFFFFFF,	// белый
	SILVER =	0xC0C0C0,	// серебро
	GRAY =		0x808080,	// серый
	BLACK =		0x000000,	// чёрный
	RED =		0xFF0000,	// красный
	MAROON =	0x800000,	// бордовый
	YELLOW =	0xFFFF00,	// жёлтый
	OLIVE =		0x808000,	// олива
	LIME =		0x00FF00,	// лайм
	GREEN =		0x008000,	// зелёный
	AQUA =		0x00FFFF,	// аква
	TEAL =		0x008080,	// цвет головы утки чирка
	BLUE =		0x0000FF,	// голубой
	NAVY =		0x000080,	// тёмно-синий
	PINK =		0xFF00FF,	// розовый
	PURPLE =	0x800080,	// пурпурный
};

class GRGB
{
  public:  
	GRGB(uint8_t rpin, uint8_t gpin, uint8_t bpin);						// объявление
	
	GRGB(uint8_t rpin, uint8_t gpin, uint8_t bpin, uint8_t pwmmode);	// объявление с выбором режима генерации ШИМ (NORM_PWM / ANY_PWM)
																		// NORM_PWM - дефолтные ШИМ пины (3, 5, 6, 9, 10, 11 для UNO/NANO/MINI)
																		// ANY_PWM - любой пин делается ШИМ пином (частота ~150 Гц). Подробности в библиотеке GyverHacks
																		
	void highFrequency(long frequency);					// режим работы на высокой частоте ШИМ (указать в Герцах). Работает с библиотекой PWM.h
	
	void setDirection(uint8_t direction);				// NORMAL / REVERSE - направление ШИМ
														// общий катод - NORMAL
														// общий анод - REVERSE
														
	void setMaxCurrent(uint16_t numLeds, float vcc, int maxCur);	// установка ограничения по току: 
																	// количество светодиодов
																	// напряжение питания в милливольтах
																	// максимальный ток
																	
	void setBrightness(byte bright);					// установка яркости (0-255)
	void setGammaBright(boolean val);					// вкл/выкл коррекции яркости
	void setMinPWM(byte val);							// минимальный сигнал PWM
	void setLUT(float rc, float gc, float bc);			// установка коррекции цвета (матрица LUT)
	void constantBrightTick(int minVolts, int vcc);		// корректировка под напряжение питания
	void gammaTick(int vcc);							// корректировка красного цвета при падении напряжения питания
													
	void setHEX(colors color);							// установка цвета из предустановленных
	void setHEX(uint32_t color);						// установка цвета в формате HEX (вида 0x808080 )
	void setRGB(uint8_t r, uint8_t g, uint8_t b);		// установка цвета в пространстве RGB (каждый цвет 0-255)
	void setHSV(uint8_t h, uint8_t s, uint8_t v);		// установка цвета в пространстве HSV (каждая велиична 0-255)
	void setHSV_fast(uint8_t h, uint8_t s, uint8_t v);	// более быстрый, но менее красивый вариант предыдущей функции
	void setKelvin(int16_t temperature);				// установить цвет как температуру в Кельвинах (от 1000 до 10'000 - от красного к синему)
	void colorWheel(int color);							// установить цвет (0 - 1530). Максимально широкая палитра ярких цветов (смеси RGB)
	
	// плавно изменить текущий цвет к новому за вермя fadeTime в миллисекундах
	// для HEX цвета
	void fadeTo(colors newColor, uint16_t fadeTime);
	
	// для R G B
	void fadeTo(uint8_t new_r, uint8_t new_g, uint8_t new_b, uint16_t fadeTime);
	
	byte showR, showG, showB;		// сигнал для отладки
	
  private:
	void setRGB();
	uint8_t _rpin = 0, _gpin = 0, _bpin = 0;	// пины
	uint8_t _r = 0, _g = 0, _b = 0;				// цвета	
	boolean _reverse_flag = false;
	boolean _PWMmode = false;
	boolean _brightFlag = false;
	float _brightC = 1.0;
	float _constCoef = 0.0;
	boolean _maxCurFlag = false;
	boolean _constBrFlag = false;
	boolean _gammaFlag;
	boolean _highFreqFlag = false;
	boolean _LUTflag = false;
	boolean _minPWMflag = false;
	boolean _gammaBright = false;
	byte _minPWMval;
	float _rc = 1.0, _gc = 1.0, _bc = 1.0;
	float _gammaR = 1.0;
	float _gammaG = 1.0;
	int _vcc = 0;
	int _maxCurrent = 0;
	int _numLeds = 0;
};

void anyPWMinitRGB(byte prescaler);									
void anyPWMpinRGB(uint8_t pin);
void anyPWMRGB(byte pin, byte duty);

#endif