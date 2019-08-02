#include "GyverUART.h"

// ===== INIT =====
void uartBegin(uint32_t baudrate){
	uint16_t speed = (2000000/baudrate)-1;
	UBRR0H = highByte(speed);
	UBRR0L = lowByte(speed);
	UCSR0A = (1 << U2X0);
	UCSR0B = ((1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0));
	UCSR0C = ((1<<UCSZ01) | (1<<UCSZ00));
}
void uartBegin(void) {
	uartBegin(9600);
}

void uartEnd(){
	UCSR0B = 0;
}

// ===== READ =====
volatile char _UART_RX_BUFFER[64];
volatile int8_t _UART_RX_COUNTER;
ISR(USART_RX_vect) {
	_UART_RX_BUFFER[_UART_RX_COUNTER] = UDR0;
	_UART_RX_COUNTER++;
}

char uartRead() {
	char thisChar = _UART_RX_BUFFER[0];
	for (byte i = 0; i < _UART_RX_COUNTER; i++) _UART_RX_BUFFER[i] = _UART_RX_BUFFER[i + 1];
	if (--_UART_RX_COUNTER < 0) _UART_RX_COUNTER = 0;
	return thisChar;
}

char uartPeek() {
	return _UART_RX_BUFFER[0];
}

boolean uartAvailable() {
	return _UART_RX_COUNTER;
}

void uartClear() {
	_UART_RX_COUNTER = 0;
}

/*
byte uartRead(){
	if (UCSR0A & (1<<RXC0))
	{
		return UDR0;
	} else return false; 
}*/
uint32_t _UART_TIMEOUT = 100;
void uartSetTimeout(int timeout) {
	_UART_TIMEOUT = timeout;
}

int32_t uartParseInt() {
	uint32_t timeoutTime = millis();
	uint32_t value = 0;
	boolean negative = false;

	while (millis() - timeoutTime < _UART_TIMEOUT) {
		if (uartAvailable()) {
			timeoutTime = millis();
			char newByte = uartRead();
			if (newByte == '-') negative = true;
			else {
				value += (newByte - '0');
				value *= 10L;
			}
		}
	}
	value /= 10L;
	return (!negative) ? value : -value;
}

boolean uartParsePacket(int *intArray) {
	if (uartAvailable()) {
		uint32_t timeoutTime = millis();
		int value = 0;
		byte index = 0;
		boolean parseStart = 0;

		while (millis() - timeoutTime < 100) {
			if (uartAvailable()) {
				timeoutTime = millis();
				if (uartPeek() == '$') {
					parseStart = true;
					uartRead();
					uartPrintln("start");
					continue;
				}
				if (parseStart) {
					if (uartPeek() == ' ') {
						intArray[index] = value / 10;
						value = 0;
						index++;
						uartRead();
						continue;
					}
					if (uartPeek() == ';') {
						intArray[index] = value / 10;
						uartRead();
						return true;
					}
					value += uartRead() - '0';
					value *= 10;
				}
			}
		}
	}
	return false;
}

float uartParseFloat() {
	uint32_t timeoutTime = millis();
	float whole = 0.0;
	float fract = 0.0;
	boolean negative = false;
	boolean decimal = false;
	byte fractSize = 0;

	while (millis() - timeoutTime < 100) {
		if (uartAvailable()) {
			timeoutTime = millis();
			char newByte = uartRead();
			if (newByte == '-') {
				negative = true;
				continue;
			}
			if (newByte == '.') {
				decimal = true;
				continue;
			}
			if (!decimal) {
				whole += (newByte - '0');
				whole *= 10L;
			} else {
				fract += (newByte - '0');
				fract *= 10L;
				fractSize++;
			}
		}
	}
	whole /= 10L;
	for (byte i = 0; i <= fractSize; i++) fract /= 10;
	whole += fract;
	return (!negative) ? whole : -whole;
}

String uartReadString() {
	uint32_t timeoutTime = millis();
	String buf = "";
	while (millis() - timeoutTime < _UART_TIMEOUT) {
		if (uartAvailable()) {
			timeoutTime = millis();
			buf += uartRead();			
		}
	}
	return buf;
}

// ===== WRITE =====

void uartWrite(byte data){	
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void uartPrintln(void) {
	uartWrite('\r');
	uartWrite('\n');
}

void uartPrint(int8_t data)		{printHelper( (long) data);}
void uartPrint(uint8_t data)	{printHelper( (long) data);}
void uartPrint(int16_t data)	{printHelper( (long) data);}
void uartPrint(uint16_t data)	{printHelper( (long) data);}
void uartPrint(int32_t data)	{printHelper( (long) data);}
void uartPrint(uint32_t data)	{printHelper( (uint32_t) data);}

void uartPrintln(int8_t data)	{printHelper( (long) data); uartPrintln();}
void uartPrintln(uint8_t data)	{printHelper( (long) data); uartPrintln();}
void uartPrintln(int16_t data)	{printHelper( (long) data); uartPrintln();}
void uartPrintln(uint16_t data)	{printHelper( (long) data); uartPrintln();}
void uartPrintln(int32_t data)	{printHelper( (long) data); uartPrintln();}
void uartPrintln(uint32_t data)	{printHelper( (uint32_t) data); uartPrintln();}


void printHelper(int32_t data) {
	if (data < 0) {
		uartWrite(45);
		data = -data;
	}
	printBytes(data);
}
void printHelper(uint32_t data) {
	if (data < 0) {
		uartWrite(45);
		data = -data;
	}
	printBytes(data);
}

void printBytes(uint32_t data) {
	int8_t bytes[10];
	byte amount;
	for (byte i = 0; i < 10; i++) {
		bytes[i] = data % 10;
		data /= 10;
		if (data == 0) {
			amount = i;
			break;
		}
	}	
	for (int8_t i = amount; i >= 0; i--) {
		uartWrite(bytes[i] + '0');
	}
}

void printBytes(uint32_t data, byte decimals) {
	int8_t bytes[10];
	byte amount;
	for (byte i = 0; i < 10; i++) {
		bytes[i] = data % 10;
		data /= 10;
		if (data == 0) {
			amount = i;
			break;
		}
	}	
	for (int8_t i = amount; i >= amount - decimals + 1; i--) {
		uartWrite(bytes[i] + '0');
	}
}

void uartPrint(double data, byte decimals) {
	if (data < 0) {
		uartWrite(45);
		data = -data;
	}
	uint32_t integer = data;
	uint32_t fract = ((float)data - integer) * 1000000000;
	printBytes(integer);
	uartWrite(46);
	printBytes(fract, decimals);
}

void uartPrint(double data) {
	uartPrint(data, 2);
}

void uartPrintln(double data, byte decimals) {
	uartPrint(data, decimals);
	uartPrintln();
}

void uartPrintln(double data) {
	uartPrint(data, 2);
	uartPrintln();
}

void uartPrint(String data) {
	byte stringSize = data.length();
	for (byte i = 0; i < stringSize; i++) {
		uartWrite(data[i]);
	}
}
void uartPrintln(String data) {
	uartPrint(data);
	uartPrintln();
}

void uartPrint(char data[]) {
	byte i = 0;
	while (data[i] != '\0') {
		uartWrite(data[i]);
		i++;
	}
}
void uartPrintln(char data[]) {
	uartPrint(data);
	uartPrintln();
}