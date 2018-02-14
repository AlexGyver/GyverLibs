/*
  TM74HC595Display.h - Library for use 4LED display with 2 TM74HC595.
  Created by Kostarev V., February 23, 2016.
  Updated by AlexGyver 06.02.2017 (added float_dot and int_dot functions)
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
 
#endif
