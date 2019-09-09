#include "GyverUART.h"
#define UART_RX_BUFFER_SIZE 64
static volatile char _UART_RX_BUFFER[UART_RX_BUFFER_SIZE];
static volatile uint8_t _UART_RX_BUFFER_HEAD;
static volatile uint8_t _UART_RX_BUFFER_TAIL;

// ===== INIT =====
void uartBegin(uint32_t baudrate){
	uint16_t speed = (F_CPU / (8L * baudrate)) - 1;
	UBRR0H = highByte(speed);
	UBRR0L = lowByte(speed);
	UCSR0A = (1 << U2X0);
	UCSR0B = ((1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0));
	UCSR0C = ((1<<UCSZ01) | (1<<UCSZ00));
	_UART_RX_BUFFER_HEAD = _UART_RX_BUFFER_TAIL = 0;
}

void uartEnd(){
	UCSR0B = 0;
}

// ===== READ =====
ISR(USART_RX_vect) {
	if (bit_is_set(UCSR0A, UPE0)) UDR0; // Не сохранять новые данные если parity error
	else {
		unsigned char c = UDR0;
		uint8_t i = (unsigned int)(_UART_RX_BUFFER_HEAD + 1) % UART_RX_BUFFER_SIZE;
		// Не сохранять новые данные если нет места
		if (i != _UART_RX_BUFFER_TAIL) {
			_UART_RX_BUFFER[_UART_RX_BUFFER_HEAD] = c;
			_UART_RX_BUFFER_HEAD = i;
		}
	}
}

char uartRead() {
	if (_UART_RX_BUFFER_HEAD == _UART_RX_BUFFER_TAIL) return -1;
	unsigned char c = _UART_RX_BUFFER[_UART_RX_BUFFER_TAIL];
	_UART_RX_BUFFER_TAIL = (_UART_RX_BUFFER_TAIL + 1) % UART_RX_BUFFER_SIZE;
	return c;
}

char uartPeek() {
	//return _UART_RX_BUFFER[0];
	return _UART_RX_BUFFER_HEAD != _UART_RX_BUFFER_TAIL? _UART_RX_BUFFER[_UART_RX_BUFFER_TAIL]: -1;
}

uint8_t uartAvailable() {
	return ((unsigned int)(UART_RX_BUFFER_SIZE + _UART_RX_BUFFER_HEAD - _UART_RX_BUFFER_TAIL)) % UART_RX_BUFFER_SIZE;
}

boolean uartAvailableForWrite() {return 1;}

void uartClear() {
	_UART_RX_BUFFER_HEAD = _UART_RX_BUFFER_TAIL = 0;
}

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

void uartPrint(int8_t data, byte base)		{printHelper( (long) data, base);}
void uartPrint(uint8_t data, byte base)		{printHelper( (uint32_t) data, base);}
void uartPrint(int16_t data, byte base)		{printHelper( (long) data, base);}
void uartPrint(uint16_t data, byte base)	{printHelper( (uint32_t) data, base);}
void uartPrint(int32_t data, byte base)		{printHelper( (long) data, base);}
void uartPrint(uint32_t data, byte base)	{printHelper( (uint32_t) data, base);}

void uartPrintln(int8_t data, byte base)	{printHelper( (long) data, base); uartPrintln();}
void uartPrintln(uint8_t data, byte base)	{printHelper( (uint32_t) data, base); uartPrintln();}
void uartPrintln(int16_t data, byte base)	{printHelper( (long) data, base); uartPrintln();}
void uartPrintln(uint16_t data, byte base)	{printHelper( (uint32_t) data, base); uartPrintln();}
void uartPrintln(int32_t data, byte base)	{printHelper( (long) data, base); uartPrintln();}
void uartPrintln(uint32_t data, byte base)	{printHelper( (uint32_t) data, base); uartPrintln();}


void printHelper(int32_t data, byte base) {
	if (data < 0) {
		uartWrite(45);
		data = -data;
	}
	printHelper((uint32_t) data, base);
}
void printHelper(uint32_t data, byte base) {
	if (base == 10) {
		printBytes(data);
	} else {
		char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
		char *str = &buf[sizeof(buf) - 1];
		*str = '\0';
		if (base < 2) base = 10;
		do {
			char c = data % base;
			data /= base;
			*--str = c < 10 ? c + '0' : c + 'A' - 10;
		} while (data);
		uartPrint(str);
	}
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

void uartPrint(double data, byte decimals) {
	if (data < 0) {
		uartWrite(45);
		data = -data;
	}
	uint32_t integer = data;
	printBytes(integer);
	uartWrite(46);	// точка
	data -= integer;
	for (byte i = 0; i < decimals; i++) {	
		data *= 10.0;
		uartPrint((byte)data);
		data -= (byte)data;
	}
}

void uartPrint(double data) {
	uartPrint(data, 2);
}

void uartPrintln(double data, byte decimals) {
	uartPrint(data, decimals);
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