#ifndef microUART_ISR_h
#define microUART_ISR_h

// должно быть кратно степени двойки для быстродействия
#define UART_RX_BUFFER_SIZE 16
#define UART_TX_BUFFER_SIZE 16

static volatile char _UART_RX_BUFFER[UART_RX_BUFFER_SIZE];
static volatile uint8_t _UART_RX_BUFFER_HEAD;
static volatile uint8_t _UART_RX_BUFFER_TAIL;
static volatile char _UART_TX_BUFFER[UART_TX_BUFFER_SIZE];
static volatile uint8_t _UART_TX_BUFFER_HEAD;
static volatile uint8_t _UART_TX_BUFFER_TAIL;

class microUART_ISR {
public:
	void begin(uint32_t baudrate) {
		uint16_t speed = (F_CPU / (8L * baudrate)) - 1;
		UBRR0 = speed;
		UCSR0A = (1 << U2X0);
		UCSR0A = (1 << U2X0);
		UCSR0B = ((1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0));
		UCSR0C = ((1 << UCSZ01) | (1 << UCSZ00));
		_UART_RX_BUFFER_HEAD = _UART_RX_BUFFER_TAIL = 0;
		_UART_TX_BUFFER_HEAD = _UART_TX_BUFFER_TAIL = 0;
	}

	void write(byte data) {
		uint8_t i = (_UART_TX_BUFFER_HEAD + 1) % UART_TX_BUFFER_SIZE;
		// ждать освобождения места в буфере
		while (i == _UART_TX_BUFFER_TAIL);
		// Не сохранять новые данные если нет места
		if (i != _UART_TX_BUFFER_TAIL) {
			_UART_TX_BUFFER[_UART_TX_BUFFER_HEAD] = data;
			_UART_TX_BUFFER_HEAD = i;
			UCSR0B |= (1 << UDRIE0);
		}
	}

	bool available() {
		return ((uint16_t)(UART_RX_BUFFER_SIZE + _UART_RX_BUFFER_HEAD - _UART_RX_BUFFER_TAIL) % UART_RX_BUFFER_SIZE);
	}

	byte read() {
		if (_UART_RX_BUFFER_HEAD == _UART_RX_BUFFER_TAIL) return -1;
		
		uint8_t c = _UART_RX_BUFFER[_UART_RX_BUFFER_TAIL];
		_UART_RX_BUFFER_TAIL = (_UART_RX_BUFFER_TAIL + 1) % UART_RX_BUFFER_SIZE;  // хвост двигаем
		return c;
	}

	void end() {
		UCSR0B = 0;
	}
private:
};

ISR(USART_RX_vect) {
	uint8_t c, i;
	if (UCSR0A & (1 << UPE0)) {
		c = UDR0; // Не сохранять новые данные если parity error
	} else {
		c = UDR0;
		i = (_UART_RX_BUFFER_HEAD + 1) % UART_RX_BUFFER_SIZE;
		// Не сохранять новые данные если нет места
		if (i != _UART_RX_BUFFER_TAIL) {
			_UART_RX_BUFFER[_UART_RX_BUFFER_HEAD] = c;
			_UART_RX_BUFFER_HEAD = i;
		}
	}
}

ISR(USART_UDRE_vect) {
	UDR0 = _UART_TX_BUFFER[_UART_TX_BUFFER_TAIL];
	_UART_TX_BUFFER_TAIL = (_UART_TX_BUFFER_TAIL + 1) % UART_TX_BUFFER_SIZE;
	if (_UART_TX_BUFFER_HEAD == _UART_TX_BUFFER_TAIL) {
		UCSR0B &= ~ (1 << UDRIE0);
	}
}
#endif