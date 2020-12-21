/*
	Лёгкая библиотека для работы с последовательным портом
	Документация: https://alexgyver.ru/gyveruart/
	- Практически полный аналог Serial, но гораздо легче
	- Данная vвстроена в ядро GyverCore!!!
	
	v1.2 - добавлен циклический буфер
	v1.3 - поправлен вывод float и добавлен вывод с базисом
	v1.4 - либа собрана в класс, добавлена readStringUntil
	v1.5 - добавлен буфер на отправку и flush
	v1.6 - ускорена запись и чтение
	v1.7 - чуть оптимизирован код
	v1.8 - пофикшен write (спасибо eugenebartosh)
	v1.9 - пофикшен write + оптимизация + поддержка USART0 atmega2560 (by Siliverst)
	v1.10 - вывод переведён на Print.h. Размер стал чуть больше, но будет меньше при работе с другими либами на Print.h
	
	Отдельное спасибо Egor 'Nich1con' Zaharov за помощь с регистрами
*/

#define USE_PRINT_H		// закомментируй, чтобы использовать gyver-вывод

#ifndef GyverUART_h
#define GyverUART_h
#include "Arduino.h"
#include <avr/io.h>

#ifdef USE_PRINT_H
#include "Print.h"
class GyverUart : public Print {
#else
	
#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

class GyverUart {
#endif
public:
	void begin(uint32_t baudrate = 9600);
	void end();

	uint8_t available();	
	char read();
	char peek();
	void clear();
	void flush();

	void setTimeout(int timeout);
	int32_t parseInt();
	float parseFloat();
	String readString();
	String readStringUntil(char terminator);
	boolean parsePacket(int *intArray);
	
#ifdef USE_PRINT_H
	virtual size_t write(uint8_t);
#else
	boolean availableForWrite();
	void write(byte data);
	void println();

	void print(char data);
	void print(int8_t data, byte base = DEC);
	void print(uint8_t data, byte base = DEC);
	void print(int16_t data, byte base = DEC);
	void print(uint16_t data, byte base = DEC);
	void print(int32_t data, byte base = DEC);
	void print(uint32_t data, byte base = DEC);
	void print(double data, byte decimals = 2);
	void print(String data);
	void print(const char data[]);

	void println(char data);
	void println(int8_t data, byte base = DEC);
	void println(uint8_t data, byte base = DEC);
	void println(int16_t data, byte base = DEC);
	void println(uint16_t data, byte base = DEC);
	void println(int32_t data, byte base = DEC);
	void println(uint32_t data, byte base = DEC);
	void println(double data, byte decimals = 2);
	void println(String data);
	void println(const char data[]);
#endif
private:
	void writeBuffer(byte data);
#ifndef USE_PRINT_H
	void printHelper(int32_t data, byte base);
	void printHelper(uint32_t data, byte base);
	void printBytes(uint32_t data);
#endif
};

extern GyverUart uart;
#endif