#include "microWire.h"

void I2C::begin(int baudrate) {		// частота шины в кгц (31-900кгц)	
	pinMode(A4, INPUT_PULLUP);		// подправить для других мк ~ input_pullup
	pinMode(A5, INPUT_PULLUP);		// подправить для других мк ~ input_pullup

	TWBR = ((F_CPU / ( baudrate * 1000L )) - 16) / 2; // расчет значения baudrate регистра
	TWSR = 0;
	TWCR = _BV(TWEN) | _BV(TWIE);
}

void I2C::setClock(uint32_t clock) {		// частота шины в кгц (31-900кгц)	
	TWBR = (((long)F_CPU / clock) - 16) / 2; // расчет значения baudrate регистра
}

void I2C::beginTransmission() {								// Начало передачи
	TWCR = _BV(TWSTA) | _BV(TWEN) | _BV(TWINT); // start + i2c enable + установка флага TWINT "Выполнить задание"
	while (!(TWCR & _BV(TWINT)));				// ожидания завершения
}

void I2C::beginTransmission(uint8_t addr) {		// начало передачи по адресу
	I2C::beginTransmission();
	I2C::write(addr << 0x1);
}

void I2C::endTransmission() {								// окончание передачи
	TWCR = _BV(TWSTO) | _BV(TWEN) | _BV(TWINT); // stop + i2c enable + установка флага TWINT "Выполнить задание"
}

void I2C::write(uint8_t data) {		// функция для отправки данных на шину
	TWDR = data;						// закинуть данные в data - регистр
	TWCR = _BV(TWEN) | _BV(TWINT);		// i2c enable + установка флага TWINT "Выполнить задание"
	while (!(TWCR & _BV(TWINT)));		// ожидания завершения
}

void I2C::requestFrom(uint8_t addr) {
	I2C::beginTransmission();
	I2C::write((addr << 1) | 0x1);
}

uint8_t I2C::receive_ack() {						// функция для чтения данных с шины с ожиданием подтверждения
	TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);	// подтвердение + i2c enable + установка флага TWINT "Выполнить задание"
	while (!(TWCR & _BV(TWINT)));				// ожидания завершения
	return TWDR;								// вернуть содержимое data - регистра 
}

uint8_t I2C::receive_nack() {			// функция для чтения данных с шины без ожидания подтверждения
	TWCR = _BV(TWEN) | _BV(TWINT);		// i2c enable + установка флага TWINT "Выполнить задание"
	while (!(TWCR & _BV(TWINT)));		// ожидания завершения
	return TWDR;						// вернуть содержимое data - регистра 
}