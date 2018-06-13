/*
  TM74HC595Display.h - Library for use 4LED display with 2 TM74HC595.
  Created by Kostarev V., February 23, 2016.
  Updated by AlexGyver 06.02.2017 (added float_dot and int_dot functions)
  
  Подробное описание здесь http://alexgyver.ru/tm74hc595_display/
  Текущая версия: 1.1 от 29.03.2018
*/
#ifndef TM74HC595Display_h
#define TM74HC595Display_h
  
class TM74HC595Display
{
  public:
    TM74HC595Display(int SCLK, int RCLK, int DIO);
	void timerIsr();
    void send(unsigned char X);
	void send(unsigned char X, unsigned char port);
	void digit4(int n, bool showZero);
	void digit4(int n);
	void digit4showZero(int n);
	void digit2(int n, int port);
	void set(unsigned char X, int port );
	void clear();
	
	void float_dot(float value, int pos); // вывод числа value с точкой СПРАВА по счёту pos
	void int_dot(int n, int pos); // вывод числа int с точкой СПРАВА по счёту pos

  private:
    int _SCLK;
	int _RCLK;
	int _DIO;
	int _DATA[4];
};

/************** БУКВЫ И СИМВОЛЫ *****************/
#define _A 0b10001000
#define _B 0b10000000
#define _C 0b11000110
#define _D 0b11000000
#define _E 0b10000110
#define _F 0b10001110
#define _G 0b11000010
#define _H 0b10001001
#define _J 0b11100001
#define _L 0b11000111
#define _N 0b11001000
#define _O 0b11000000
#define _P 0b10001100
#define _S 0b10010010
#define _U 0b11000001
#define _Y 0b10010001
#define _a 0b10100000
#define _b 0b10000011
#define _c 0b10100111
#define _d 0b10100001
#define _e 0b10000100
#define _f 0b10001110
#define _h 0b10001011
#define _i 0b11101111
#define _j 0b11110001
#define _l 0b11111001
#define _n 0b10101011
#define _o 0b10100011
#define _q 0b10011000
#define _r 0b10101111
#define _t 0b10000111
#define _u 0b11100011
#define _y 0b11100011
#define _dash 0b10111111
#define _under 0b11110111
#define _equal 0b10110111
#define _empty 0b1111111

#define _0 0b11000000
#define _1 0b11001111
#define _2 0b10100100
#define _3 0b10110000
#define _4 0b10011001
#define _5 0b10010010
#define _6 0b10000010
#define _7 0b11111000
#define _8 0b10000000
#define _9 0b10010000
/************** БУКВЫ И СИМВОЛЫ *****************/
 
#endif
