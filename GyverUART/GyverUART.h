/*
	Лёгкая библиотека для работы с последовательным портом
	Практически полный аналог Serial, но гораздо легче
	Данная версия встроена в ядро GyverCore!!!
	Отдельное спасибо Egor 'Nich1con' Zaharov за помощь с регистрами
	Версия 1.0
*/

#ifndef GyverUART_h
#define GyverUART_h

#include "Arduino.h"
#include <avr/io.h>

void uartBegin(void);
void uartBegin(uint32_t baudrate);
void uartEnd();

boolean uartAvailable();
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

void uartPrint(int8_t data);
void uartPrint(uint8_t data);
void uartPrint(int16_t data);
void uartPrint(uint16_t data);
void uartPrint(int32_t data);
void uartPrint(uint32_t data);
void uartPrint(double data);
void uartPrint(double data, byte decimals);
void uartPrint(String data);
void uartPrint(char data[]);

void uartPrintln(int8_t data);
void uartPrintln(uint8_t data);
void uartPrintln(int16_t data);
void uartPrintln(uint16_t data);
void uartPrintln(int32_t data);
void uartPrintln(uint32_t data);
void uartPrintln(double data);
void uartPrintln(double data, byte decimals);
void uartPrintln(String data);
void uartPrintln(char data[]);

void printHelper(int32_t data);
void printHelper(uint32_t data);
void printBytes(uint32_t data);
void printBytes(uint32_t data, byte amount);

#endif