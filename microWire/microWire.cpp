#include "microWire.h"

void TwoWire::begin() {	  			// Инициализация шины в роли master
	pinMode(SDA, INPUT_PULLUP); 	// Подтяжка шины 
	pinMode(SCL, INPUT_PULLUP);	// Подтяжка шины 

	TWBR = 72;					// Стандартная скорость - 100kHz
	TWSR = 0;						// Делитель - /1 , статус - 0;
}

void TwoWire::setClock(uint32_t clock) { 		// Функция установки частоты шины 31-900 kHz (в герцах)
	TWBR = (((long)F_CPU / clock) - 16) / 2; 	// Расчет baudrate - регистра
}

void TwoWire::beginTransmission(uint8_t address) { 	// Начать передачу (для записи данных)
	TwoWire::start();                        			// Старт
	TwoWire::write(address << 1);            			// Отправка slave - устройству адреса с битом "write"
}

void TwoWire::endTransmission(bool stop = true) { 	// Завершить передачу (после записи данных)
	if (stop) TwoWire::stop();						// Если задано stop или аргумент пуст - отпустить шину 
	else TwoWire::start();							// Иначе - restart (другой master на шине не сможет влезть между сообщениями)
	return (TWSR & 0xF8);							// Вернуть содержимое статус - регистра (для отладки)
}

void TwoWire::write(uint8_t data) {					// Прямая отправка байта на шину
	TWDR = data;									// Записать данные в data - регистр
	TWCR = _BV(TWEN) | _BV(TWINT);				// Запустить передачу
	while (!(TWCR & _BV(TWINT)));					// Дождаться окончания
}

uint8_t TwoWire::available() {						// Вернуть оставшееся количество запрошенных для чтения байт
	return _requested_bytes;						// Это содержимое этой переменной
}

uint8_t TwoWire::read() {						  	// Прямое чтение байта из шины после запроса
	if (_requested_bytes > 1) {					// Если байт не последний 
		_requested_bytes--;							// Отнять от оставшееся количества еденицу
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);	// Запустить чтение шины (с подтверждением "ACK")
		while (!(TWCR & _BV(TWINT)));				// Дождаться окончания приема данных
		return TWDR;								// Вернуть принятые данные , это содержимое data - регистра 
	}
	_requested_bytes = 0; 						// Если читаем последний байт
	TWCR = _BV(TWEN) | _BV(TWINT);				// Запустить чтение шины (БЕЗ подтверждения "NACK")
	while (!(TWCR & _BV(TWINT)));					// Дождаться окончания приема данных
	if (_stop_after_request) TwoWire::stop();  		// Если в requestFrom не задан аргумент stop , или stop задан как true - отпустить шину
	else TwoWire::start();							// Иначе - restart (другой master на шине не сможет влезть между сообщениями)
	return TWDR;									// Вернуть принятый ранее байт из data - регистра
}

void TwoWire::requestFrom(uint8_t address , uint8_t length , bool stop = true) {  // Запрос n-го кол-ва байт от ведомого устройства (Читайте все байты сразу!!!)
	_stop_after_request = stop; 					// stop или restart после чтения последнего байта
	_requested_bytes = length;					// Записать в переменную количество запрошенных байт
	TwoWire::start();									// Начать работу на шине
	TwoWire::write((address << 1) | 0x1);				// Отправить устройству адрес + бит "read" 
}

void TwoWire::start() {								// сервисная функция с нее начинается любая работа с шиной
	TWCR = _BV(TWSTA) | _BV(TWEN) | _BV(TWINT); 	// start + TwoWire enable + установка флага "выполнить задачу"
	while (!(TWCR & _BV(TWINT)));					// Ожидание завершения 
}

void TwoWire::stop() {								// сервисная функция ей заканчивается работа с шиной
	TWCR = _BV(TWSTO) | _BV(TWEN) | _BV(TWINT);	// stop + TwoWire enable + установка флага "выполнить задачу"
}

TwoWire Wire = TwoWire();