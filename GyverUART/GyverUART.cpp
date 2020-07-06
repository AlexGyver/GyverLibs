#include "GyverUART.h"

#define UART_RX_BUFFER_SIZE 64
static volatile char _UART_RX_BUFFER[UART_RX_BUFFER_SIZE];
static volatile uint8_t _UART_RX_BUFFER_HEAD;
static volatile uint8_t _UART_RX_BUFFER_TAIL;
uint32_t _UART_TIMEOUT = 100;

#define UART_TX_BUFFER_SIZE 32
static volatile char _UART_TX_BUFFER[UART_TX_BUFFER_SIZE];
static volatile uint8_t _UART_TX_BUFFER_HEAD;
static volatile uint8_t _UART_TX_BUFFER_TAIL;

#if defined(__AVR_ATmega2560__)
#define	USARTx_RX_vect		USART0_RX_vect
#define	USARTx_UDRE_vect	USART0_UDRE_vect
#else
#define	USARTx_RX_vect		USART_RX_vect
#define	USARTx_UDRE_vect	USART_UDRE_vect
#endif

// =========================== INIT ========================
void GyverUart::begin(uint32_t baudrate){
	uint16_t speed = (F_CPU / (8L * baudrate)) - 1;
	UBRR0H = highByte(speed);
	UBRR0L = lowByte(speed);
	UCSR0A = (1 << U2X0);
	UCSR0B = ((1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0));
	UCSR0C = ((1<<UCSZ01) | (1<<UCSZ00));
	_UART_RX_BUFFER_HEAD = _UART_RX_BUFFER_TAIL = 0;
	_UART_TX_BUFFER_HEAD = _UART_TX_BUFFER_TAIL = 0;
}

void GyverUart::end(){
	UCSR0B = 0;
}

// =========================== READ ============================
ISR(USARTx_RX_vect) {
	uint8_t c,i;
	if (UCSR0A & (1<<UPE0)){c = UDR0;} // Не сохранять новые данные если parity error
	else{
		c = UDR0;
		if (_UART_RX_BUFFER_HEAD + 1 == UART_RX_BUFFER_SIZE){i=0;}
		else {i = _UART_RX_BUFFER_HEAD + 1;}
		// Не сохранять новые данные если нет места
		if (i != _UART_RX_BUFFER_TAIL) {
			_UART_RX_BUFFER[_UART_RX_BUFFER_HEAD] = c;
			_UART_RX_BUFFER_HEAD = i;
		}
	}
}

char GyverUart::read() {
	if (_UART_RX_BUFFER_HEAD == _UART_RX_BUFFER_TAIL) {return -1;}
	uint8_t c = _UART_RX_BUFFER[_UART_RX_BUFFER_TAIL];
	if (++_UART_RX_BUFFER_TAIL >= UART_RX_BUFFER_SIZE) {_UART_RX_BUFFER_TAIL = 0;}  // хвост двигаем
	return c;
}

char GyverUart::peek() {
	return _UART_RX_BUFFER_HEAD != _UART_RX_BUFFER_TAIL ? _UART_RX_BUFFER[_UART_RX_BUFFER_TAIL]: -1;
}

void GyverUart::flush() {
	while (_UART_RX_BUFFER_HEAD != _UART_RX_BUFFER_TAIL);
}

uint8_t GyverUart::available() {
	return ((uint16_t)(UART_RX_BUFFER_SIZE + _UART_RX_BUFFER_HEAD - _UART_RX_BUFFER_TAIL)% UART_RX_BUFFER_SIZE);
}

void GyverUart::clear() {
	_UART_RX_BUFFER_HEAD = _UART_RX_BUFFER_TAIL = 0;
}


void GyverUart::setTimeout(int timeout) {
	_UART_TIMEOUT = timeout;
}

int32_t GyverUart::parseInt() {
	uint32_t timeoutTime = millis();
	uint32_t value = 0;
	boolean negative = false;

	while (millis() - timeoutTime < _UART_TIMEOUT) {
		if (available()) {
			timeoutTime = millis();
			char newByte = read();
			if (newByte == '-') {negative = true;}
			else {
				value += (newByte - '0');
				value *= 10L;
			}
		}
	}
	value /= 10L;
	return (!negative) ? value : -value;
}

boolean GyverUart::parsePacket(int *intArray) {
	if (available()) {
		uint32_t timeoutTime = millis();
		int value = 0;
		byte index = 0;
		boolean parseStart = 0;

		while (millis() - timeoutTime < 100) {
			if (available()) {
				timeoutTime = millis();
				if (peek() == '$') {
					parseStart = true;
					read();
					continue;
				}
				if (parseStart) {
					if (peek() == ' ') {
						intArray[index] = value / 10;
						value = 0;
						index++;
						read();
						continue;
					}
					if (peek() == ';') {
						intArray[index] = value / 10;
						read();
						return true;
					}
					value += read() - '0';
					value *= 10;
				}
			}
		}
	}
	return false;
}

float GyverUart::parseFloat() {
	uint32_t timeoutTime = millis();
	float whole = 0.0;
	float fract = 0.0;
	boolean negative = false;
	boolean decimal = false;
	byte fractSize = 0;

	while (millis() - timeoutTime < 100) {
		if (available()) {
			timeoutTime = millis();
			char newByte = read();
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
	for (byte i = 0; i <= fractSize; i++) {fract /= 10;}
	whole += fract;
	return (!negative) ? whole : -whole;
}

String GyverUart::readString() {
	uint32_t timeoutTime = millis();
	String buf = "";
	while (millis() - timeoutTime < _UART_TIMEOUT) {
		if (available()) {
			timeoutTime = millis();
			buf += read();			
		}
	}
	return buf;
}

String GyverUart::readStringUntil(char terminator) {
	uint32_t timeoutTime = millis();
	String buf = "";
	while (millis() - timeoutTime < _UART_TIMEOUT) {
		if (available()) {
			if (peek() == terminator) {
				clear();
				return buf;
			}
			timeoutTime = millis();
			buf += read();
		}
	}
	return buf;
}

// ====================== WRITE ===========================

/*
// прямая запись без буфера
void GyverUart::writeBuffer(byte data){	
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}
*/

void GyverUart::writeBuffer(byte data) {
	uint8_t i;
	if (_UART_TX_BUFFER_HEAD + 1 == UART_TX_BUFFER_SIZE){i=0;}
	else {i = _UART_TX_BUFFER_HEAD + 1;}
	// ждать освобождения места в буфере
	while (i == _UART_TX_BUFFER_TAIL);
	// Не сохранять новые данные если нет места
	if (i != _UART_TX_BUFFER_TAIL) 
	{
		_UART_TX_BUFFER[_UART_TX_BUFFER_HEAD] = data;
		_UART_TX_BUFFER_HEAD = i;
		UCSR0B |= (1<<UDRIE0);
	}
}

ISR(USARTx_UDRE_vect) {
	UDR0 = _UART_TX_BUFFER[_UART_TX_BUFFER_TAIL];
	if (++_UART_TX_BUFFER_TAIL >= UART_TX_BUFFER_SIZE) {_UART_TX_BUFFER_TAIL = 0;}  // хвост двигаем
	if (_UART_TX_BUFFER_HEAD == _UART_TX_BUFFER_TAIL) {UCSR0B &=~ (1<<UDRIE0);}
}

#ifdef USE_PRINT_H
size_t GyverUart::write(uint8_t data) {
	writeBuffer(data);
	return 1;
}

#else
	
boolean GyverUart::availableForWrite() {return 1;}

void GyverUart::println(void) {
	writeBuffer('\r');
	writeBuffer('\n');
}

void GyverUart::print(char data) {
	writeBuffer(data);
}
void GyverUart::println(char data) {
	print(data);
	println();
}

void GyverUart::print(int8_t data, byte base)		{printHelper( (int32_t) data, base);}
void GyverUart::print(uint8_t data, byte base)		{printHelper( (uint32_t) data, base);}
void GyverUart::print(int16_t data, byte base)		{printHelper( (int32_t) data, base);}
void GyverUart::print(uint16_t data, byte base)		{printHelper( (uint32_t) data, base);}
void GyverUart::print(int32_t data, byte base)		{printHelper( (int32_t) data, base);}
void GyverUart::print(uint32_t data, byte base)		{printHelper( (uint32_t) data, base);}

void GyverUart::println(int8_t data, byte base)		{printHelper( (int32_t) data, base); println();}
void GyverUart::println(uint8_t data, byte base)	{printHelper( (uint32_t) data, base); println();}
void GyverUart::println(int16_t data, byte base)	{printHelper( (int32_t) data, base); println();}
void GyverUart::println(uint16_t data, byte base)	{printHelper( (uint32_t) data, base); println();}
void GyverUart::println(int32_t data, byte base)	{printHelper( (int32_t) data, base); println();}
void GyverUart::println(uint32_t data, byte base)	{printHelper( (uint32_t) data, base); println();}

void GyverUart::printHelper(int32_t data, byte base) {
	if (data < 0) {
		writeBuffer(45);
		data = -data;
	}
	printHelper((uint32_t) data, base);
}
void GyverUart::printHelper(uint32_t data, byte base) {
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
		print(str);
	}
}


void GyverUart::printBytes(uint32_t data) {
	byte bytes[10];
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
		writeBuffer(bytes[i] + '0');
	}
}

void GyverUart::print(double data, byte decimals) {
	if (data < 0) {
		writeBuffer(45);
		data = -data;
	}
	uint32_t integer = data;
	printBytes(integer);
	writeBuffer(46);	// точка
	data -= integer;
	for (byte i = 0; i < decimals; i++) {	
		data *= 10.0;
		print((byte)data);
		data -= (byte)data;
	}
}

void GyverUart::println(double data, byte decimals) {
	print(data, decimals);
	println();
}


void GyverUart::print(String data) {
	byte stringSize = data.length();
	for (byte i = 0; i < stringSize; i++) {
		print(data[i]);
	}
}
void GyverUart::println(String data) {
	print(data);
	println();
}

void GyverUart::print(const char data[]) {
	byte i = 0;
	while (data[i] != '\0') {
		print(data[i]);
		i++;
	}
}
void GyverUart::println(const char data[]) {
	print(data);
	println();
}
#endif