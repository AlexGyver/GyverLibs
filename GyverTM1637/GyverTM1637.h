#ifndef GyverTM1637_h
#define GyverTM1637_h
#include <inttypes.h>
#include <Arduino.h>

/*	
	GyverTM1637 - бибилотека для 7 сегментного дисплея на чипе TM1637 с кучей приколюх
	- Вывод цифр массивом или прицельно
	- Вывод букв из списка доступных (листай ниже) массивом или прицельно
	- Отдельная функция вывода часов и минут (часы без нуля слева, минуты с нулём)
	- Вывод числа от -999 до 9999 с учётом знака
	- Готовая функция бегущей строки
	- Функции смены яркости и состояния двоеточия автоматически обновляют дисплей
	- Функция обновления значения с эффектом вертикальной прокрутки
	- Функция обновления значения с эффектом скручивания (лучше один раз увидеть)
	
	Основано на оригинальной библиотеке "Grove_4Digital_Display" от Frankie.Chu
	https://github.com/Seeed-Studio/Grove_4Digital_Display
	
	Текущая версия: 1.4 от 02.11.2019
	- Поправлены типы данных и ошибки
	- Добавлена совместимость с ESP
*/

class GyverTM1637
{
public:
	
	GyverTM1637(uint8_t clk, uint8_t dio);									// объявление и инициализация
	
	void display(uint8_t DispData[]);										// выводит цифры массивом по ячейкам. От 0 до 9 (byte values[] = {3, 5, 9, 0}; )
	void display(uint8_t BitAddr, uint8_t DispData);							// выводит цифру DispData в указанную ячейку дисплея BitAddr
	void display(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3);	// если лень создавать массив, выводит цифры в ячейки
	
	void displayByte(uint8_t DispData[]);									// выводит байт вида 0xe6 и буквы-константы вида _a , _b .... массивом
	void displayByte(uint8_t BitAddr, uint8_t DispData);						// выводит байт вида 0xe6 и буквы-константы вида _a , _b .... в ячейку
	void displayByte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3);	// если лень создавать массив, выводит байты в ячейки
	
	void displayClock(uint8_t hrs, uint8_t mins);							// выводит часы и минуты
	void displayClockScroll(uint8_t hrs, uint8_t mins, int delayms);		// выводит часы и минуты с эффектом прокрутки
	void displayClockTwist(uint8_t hrs, uint8_t mins, int delayms);			// выводит часы и минуты с эффектом скрутки
	
	void displayInt(int value);												// выводит число от -999 до 9999 (да, со знаком минус)
	
	void runningString(uint8_t DispData[], byte amount, int delayMs);  		// бегущая строка (array, sizeof(array), задержка в мс)
	
	void clear(void);														// очистить дисплей
	
	void point(boolean PointFlag);											// вкл / выкл точку (POINT_ON / POINT_OFF)

	void brightness(uint8_t bright, uint8_t = 0x40, uint8_t = 0xc0);		// яркость 0 - 7	
	
	void scroll(uint8_t BitAddr, uint8_t DispData, int delayms);				// обновить значение прокруткой (адрес, ЦИФРА, задержка в мс)
	void scroll(uint8_t DispData[], int delayms);							// обновить значение прокруткой (массив ЦИФР, задержка в мс)
	void scroll(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, int delayms);	// прокрутка посимвольно
	void scrollByte(uint8_t BitAddr, uint8_t DispData, int delayms);			// обновить значение прокруткой (адрес, БАЙТ, задержка в мс)
	void scrollByte(uint8_t DispData[], int delayms);						// обновить значение прокруткой (массив БАЙТ, задержка в мс)
	void scrollByte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, int delayms);	// прокрутка посимвольно
	
	void twist(uint8_t BitAddr, uint8_t DispData, int delayms);				// обновить значение скручиванием (адрес, ЦИФРА, задержка в мс)
	void twist(uint8_t DispData[], int delayms);								// обновить значение скручиванием (массив ЦИФР, задержка в мс)
	void twist(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, int delayms);	// скрутка посимвольно
	void twistByte(uint8_t BitAddr, uint8_t DispData, int delayms);			// обновить значение скручиванием (адрес, БАЙТ, задержка в мс)
	void twistByte(uint8_t DispData[], int delayms);							// обновить значение скручиванием (массив БАЙТ, задержка в мс)
	void twistByte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, int delayms);	// скрутка посимвольно
	
private:
	uint8_t lastData[4];
	void update();
	int  writeByte(int8_t wr_data);
	void start(void);
	void stop(void);
	
	void sendByte(uint8_t BitAddr, int8_t sendData);
	void sendArray(uint8_t sendData[]);

	uint8_t Cmd_SetData;
	uint8_t Cmd_SetAddr;
	uint8_t Cmd_DispCtrl;
	uint8_t PointData;

	uint8_t Clkpin;
	uint8_t Datapin;	
};

void swapBytes(byte* newByte, byte oldByte, byte oldP, byte newP);
uint8_t digToHEX(uint8_t digit);		// вернёт код цифры для displayByte

//************definitions for TM1637*********************
#define ADDR_AUTO  0x40
#define ADDR_FIXED 0x44
#define STARTADDR  0xc0

/**** definitions for the clock point of the digit tube *******/
#define POINT_ON   1
#define POINT_OFF  0

/**************definitions for brightness***********************/
#define  BRIGHT_DARKEST 0
#define  BRIGHT_TYPICAL 2
#define  BRIGHTEST      7

/************** БУКВЫ И СИМВОЛЫ *****************/

#if defined(ESP8266)
#define _A 0x77
#define _B 0x7f
#define _C 0x39
#define _D 0x3f
#define _E 0x79
#define _F 0x71
#define _G 0x3d
#define _H 0x76
#define _J 0x1e
#define _L 0x38
#define _N 0x37
#define _O 0x3f
#define _P 0x73
#define _S 0x6d
#define _U 0x3e
#define _Y 0x6e
#define _a 0x5f
#define _b 0x7c
#define _c 0x58
#define _d 0x5e
#define _e 0x7b
#define _f 0x71
#define _h 0x74
#define _i 0x10
#define _j 0x0e
#define _l 0x06
#define _n 0x54
#define _o 0x5c
#define _q 0x67
#define _r 0x50
#define _t 0x78
#define _u 0x1c
#define _y 0x6e
#define _dash 0x40
#define _under 0x08
#define _equal 0x48
#define _empty 0x00
#define _degree 0x63

#define _0 0x3f
#define _1 0x06
#define _2 0x5b
#define _3 0x4f
#define _4 0x66
#define _5 0x6d
#define _6 0x7d
#define _7 0x07
#define _8 0x7f
#define _9 0x6f

#else
	
enum TM1637_letters {
	_A = 0x77,
	_B = 0x7f,
	_C = 0x39,
	_D = 0x3f,
	_E = 0x79,
	_F = 0x71,
	_G = 0x3d,
	_H = 0x76,
	_J = 0x1e,
	_L = 0x38,
	_N = 0x37,
	_O = 0x3f,
	_P = 0x73,
	_S = 0x6d,
	_U = 0x3e,
	_Y = 0x6e,
	_a = 0x5f,
	_b = 0x7c,
	_c = 0x58,
	_d = 0x5e,
	_e = 0x7b,
	_f = 0x71,
	_h = 0x74,
	_i = 0x10,
	_j = 0x0e,
	_l = 0x06,
	_n = 0x54,
	_o = 0x5c,
	_q = 0x67,
	_r = 0x50,
	_t = 0x78,
	_u = 0x1c,
	_y = 0x6e,
	_dash = 0x40,
	_under = 0x08,
	_equal = 0x48,
	_empty = 0x00,
	_degree = 0x63,

	_0 = 0x3f,
	_1 = 0x06,
	_2 = 0x5b,
	_3 = 0x4f,
	_4 = 0x66,
	_5 = 0x6d,
	_6 = 0x7d,
	_7 = 0x07,
	_8 = 0x7f,
	_9 = 0x6f,
};
#endif
#endif