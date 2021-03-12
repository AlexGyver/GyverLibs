#ifndef microUART_print_h
#define microUART_print_h
class microUART_print : public Print  {
public:
	void begin(uint32_t baudrate) {
		UBRR0 = (F_CPU / (8L * baudrate)) - 1;;		
		UCSR0A = (1 << U2X0);
		UCSR0B = (1 << TXEN0) | (1 << RXEN0);
		UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	}
	virtual size_t write(byte data) {
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = data;
	}
	bool available() {
		return (UCSR0A & (1 << RXC0));
	}
	byte read() {
		byte data = UDR0;
		return data;
	}
	void end() {
		UCSR0B = 0;
	}
private:
};
#endif
