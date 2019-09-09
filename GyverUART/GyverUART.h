/*
	Лёгкая библиотека для работы с последовательным портом
	Практически полный аналог Serial, но гораздо легче
	Данная версия встроена в ядро GyverCore!!!
	Отдельное спасибо Egor 'Nich1con' Zaharov за помощь с регистрами
	Версия 1.2 - добавлен циклический буфер
	Версия 1.3 - поправлен вывод float и добавлен вывод с базисом
*/

#ifndef GyverUART_h
#define GyverUART_h

#include "Arduino.h"
#include <avr/io.h>

#define DEC 10
#define HEX 16
#define OCT 8

void uartBegin(uint32_t baudrate = 9600);
void uartEnd();

uint8_t uartAvailable();
boolean uartAvailableForWrite();
char uartRead();
char uartPeek();
void uartClear();

void uartSetTimeout(int timeout);
int32_t uartParseInt();
float uartParseFloat();
String uartReadString();
boolean uartParsePacket(int *intArray);

void uartWrite(byte data);
void uartPrintln(void);

void uartPrint(int8_t data, byte base = DEC);
void uartPrint(uint8_t data, byte base = DEC);
void uartPrint(int16_t data, byte base = DEC);
void uartPrint(uint16_t data, byte base = DEC);
void uartPrint(int32_t data, byte base = DEC);
void uartPrint(uint32_t data, byte base = DEC);
void uartPrint(double data, byte decimals = 2);
void uartPrint(String data);
void uartPrint(char data[]);

void uartPrintln(int8_t data, byte base = DEC);
void uartPrintln(uint8_t data, byte base = DEC);
void uartPrintln(int16_t data, byte base = DEC);
void uartPrintln(uint16_t data, byte base = DEC);
void uartPrintln(int32_t data, byte base = DEC);
void uartPrintln(uint32_t data, byte base = DEC);
void uartPrintln(double data, byte decimals = 2);
void uartPrintln(String data);
void uartPrintln(char data[]);

void printHelper(int32_t data, byte base);
void printHelper(uint32_t data, byte base);
void printBytes(uint32_t data);

#endif