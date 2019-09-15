/*
	Лёгкая библиотека для работы с последовательным портом
	Практически полный аналог Serial, но гораздо легче
	Данная версия встроена в ядро GyverCore!!!
	Отдельное спасибо Egor 'Nich1con' Zaharov за помощь с регистрами
	Версия 1.2 - добавлен циклический буфер
	Версия 1.3 - поправлен вывод float и добавлен вывод с базисом
	Версия 1.4 - либа собрана в класс, добавлена readStringUntil
	Версия 1.5 - добавлен буфер на отправку и flush
	Версия 1.6 - ускорена запись и чтение
*/

#ifndef GyverUART_h
#define GyverUART_h

#include "Arduino.h"
#include <avr/io.h>

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

class GyverUart {
public:
	void begin(uint32_t baudrate = 9600);
	void end();

	uint8_t available();
	boolean availableForWrite();
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

	void write(byte data);
	void println(void);

	void print(char data);
	void print(int8_t data, byte base = DEC);
	void print(uint8_t data, byte base = DEC);
	void print(int16_t data, byte base = DEC);
	void print(uint16_t data, byte base = DEC);
	void print(int32_t data, byte base = DEC);
	void print(uint32_t data, byte base = DEC);
	void print(double data, byte decimals = 2);
	void print(String data);
	void print(char data[]);

	void println(char data);
	void println(int8_t data, byte base = DEC);
	void println(uint8_t data, byte base = DEC);
	void println(int16_t data, byte base = DEC);
	void println(uint16_t data, byte base = DEC);
	void println(int32_t data, byte base = DEC);
	void println(uint32_t data, byte base = DEC);
	void println(double data, byte decimals = 2);
	void println(String data);
	void println(char data[]);	
	
private:
	void printHelper(int32_t data, byte base);
	void printHelper(uint32_t data, byte base);
	void printBytes(uint32_t data);
	void startTransmission();
};

extern GyverUart uart;

#endif