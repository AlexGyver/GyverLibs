//  Author:Fred.Chu
//  Date:9 April,2013
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
//  Modified record:
//  07.01.2018 AlexGyver
//  Added a lot of functions for display custom symbols and int values

//  Подробное описание здесь http://alexgyver.ru/tm1637_display/
//  Текущая версия: 1.1 от 29.03.2018
/*******************************************************************************/

#ifndef TM1637_h
#define TM1637_h
#include <inttypes.h>
#include <Arduino.h>
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
/************** БУКВЫ И СИМВОЛЫ *****************/


class TM1637
{
  public:
    uint8_t Cmd_SetData;
    uint8_t Cmd_SetAddr;
    uint8_t Cmd_DispCtrl;
    boolean _PointFlag;     //_PointFlag=1:the clock point on
    TM1637(uint8_t, uint8_t);
    void init(void);        //To clear the display
    void writeByte(int8_t wr_data);//write 8bit data to tm1637
    void start(void);//send start bits
    void stop(void); //send stop bits
	
    void display(int8_t DispData[]);                    // вывести массив ЦИФР
	void displayByte(int8_t DispData[]);                // вывести массив БАЙТОВ
    void display(uint8_t BitAddr,int8_t DispData);      // вывести цифру в указанный порт
	void displayByte(uint8_t BitAddr,int8_t DispData);                    // вывести байт в указанный порт
	void displayByte(uint8_t bit1,int8_t bit2,int8_t bit3,int8_t bit4);  // вывести побайтово символы
	void displayInt(int n);       // вывод числа (0-9999) без нулей слева
	void displayIntZero(int n);  // вывод числа (0-9999) с нулями слева
	void runningString(int8_t DispData[], byte amount, int delayMs);
	
    void clearDisplay(void);
    void set(uint8_t = BRIGHT_TYPICAL,uint8_t = 0x40,uint8_t = 0xc0);//To take effect the next time it displays.
    void point(boolean PointFlag);//whether to light the clock point ":".To take effect the next time it displays.
	
    void coding(int8_t DispData[]); 
	void codingByte(int8_t DispData[]); 
    int8_t coding(int8_t DispData); 
	int8_t codingByte(int8_t DispData); 
  private:
    uint8_t Clkpin;
    uint8_t Datapin;
};
#endif
