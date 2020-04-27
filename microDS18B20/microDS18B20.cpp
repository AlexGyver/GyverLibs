#include "microDS18B20.h"
#include "microOneWire.h"
/************************************** Таблица CRC *******************************************/
#if (DS_CRC_USE_TABLE == true)
static const uint8_t PROGMEM _crc_table[] = {
	0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41, 
	0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc, 
	0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62, 
	0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff, 
	0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07, 
	0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a, 
	0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24, 
	0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b, 0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9, 
	0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd, 
	0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50, 
	0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee, 
	0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73, 
	0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b, 
	0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16, 
	0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8, 
	0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35  
};
#endif

MicroDS18B20::MicroDS18B20() {}

MicroDS18B20::MicroDS18B20(uint8_t pin) {							// Создать обьект без адреса	
	MicroDS18B20::setPin(pin);
}

MicroDS18B20::MicroDS18B20(uint8_t pin, uint8_t *address) {			// Создать обьект с адресом
	MicroDS18B20::setPin(pin);
	MicroDS18B20::setAddress(address);
}

void MicroDS18B20::setAddress(uint8_t *address) {
	_ds_address = address;
	_ds_address_defined = true;
}

void MicroDS18B20::setPin(uint8_t pin) {
	_ds_pin = pin;													// Присвоить пин 	
	pinMode(_ds_pin, INPUT);										// "Отпустить" линию
	digitalWrite(_ds_pin, LOW);										// Рабочее состояние линии	
}

void MicroDS18B20::setResolution(uint8_t resolution) {				// Установка рабочего разрешения	
	if (oneWire_reset(_ds_pin)) return;								// Сброс и проверка присутствия
	MicroDS18B20::addressRoutine();									// Процедура адресации
	oneWire_write(0x4E, _ds_pin);									// Запись RAM
	oneWire_write(0xFF, _ds_pin);									// Максимум в верхний регистр тревоги
	oneWire_write(0x00, _ds_pin);									// Минимум в верхний регистр тревоги
	oneWire_write(((constrain(resolution, 9, 12) - 9) << 5) | 0x1F, _ds_pin); // Запись конфигурации разрешения
}


void MicroDS18B20::readAddress(uint8_t *addressArray) {				// Чтение адреса датчика в массив
	if (_ds_address_defined or oneWire_reset(_ds_pin)) return;		// Проверка присутствия
	oneWire_write(0x33, _ds_pin);										// Запрос адреса
#if (DS_CHECK_CRC == true)												// Если требуется проверка подлинности
	uint8_t _calculated_crc = 0;										// Переменная для CRC8
	uint8_t _temp_address[8];											// Временный массив для адреса
	for (uint8_t i = 0; i < 8; i++) {									// Прочитать 8 байт адреса
		_temp_address[i] = oneWire_read(_ds_pin);						// Записать байты во временный массив
		_calculated_crc = MicroDS18B20::crc_update(_calculated_crc, _temp_address[i]);	// Обновить значение CRC8
	}	
	if (_calculated_crc) return;										// Если CRC не сошелся - данные в помойку
	memcpy(addressArray, _temp_address, 8);							// Если сошелся - переписать массив в основной
#else																// Если пропуск проверки CRC
	for (uint8_t i = 0; i < 8; i++) {									// Прочитать 8 байт
		addressArray[i] = oneWire_read(_ds_pin);						// Поместить в пользовательский массив
	}
#endif
}


void MicroDS18B20::requestTemp(void) {						// Запрос температуры
	if (oneWire_reset(_ds_pin)) return;								// Проверка присутствия
	MicroDS18B20::addressRoutine();									// Процедура адресации
	oneWire_write(0x44, _ds_pin);									// Запросить преобразование
}


DS_TEMP_TYPE MicroDS18B20::getTemp(void) {						// Чтение температуры
	uint8_t data[9];													// Временный массив для данных
	uint8_t _calculated_crc = 0;										// Переменная для хранения CRC
	if (oneWire_reset(_ds_pin)) return 0;								// Проверка присутствия
	MicroDS18B20::addressRoutine();									// Процедура адресации
	oneWire_write(0xBE, _ds_pin);										// Запросить температуру
#if (DS_CHECK_CRC == true)												// Если требуется проверка подлинности
	for (uint8_t i = 0; i < 9; i++) {									// Считать RAM 
		data[i] = oneWire_read(_ds_pin);								// Прочитать данные
		_calculated_crc = MicroDS18B20::crc_update(_calculated_crc, data[i]); // Обновить значение CRC8
	}	
	if (_calculated_crc) return 0;									// Если CRC не сошелся - данные в помойку
#else																// Если пропуск проверки CRC
	data[0] = oneWire_read(_ds_pin);									// Прочитать младший байт температуры
	data[1] = oneWire_read(_ds_pin);									// Прочитать старший байт температуры
#endif			
	return (DS_TEMP_TYPE)((data[1] << 8) | data[0]) / 16;				// Рассчитать значение температуры
}


uint8_t MicroDS18B20::crc_update(uint8_t crc, uint8_t data) {		// Процедура обновления CRC
#if (DS_CRC_USE_TABLE == true)											// Используем таблицу?
	return pgm_read_byte(&_crc_table[crc ^ data]);					// Тогда берем готовое значение
#else																// По - дедовски?
	uint8_t i = 8;													// Используем полином CRC8 1-Wire
	while (i--) {														 
		crc = ((crc ^ data) & 1) ? (crc >> 1) ^ 0b10001100 : (crc >> 1); 
		data >>= 1;
	}
	return crc;
#endif
}


void MicroDS18B20::addressRoutine(void) {							// Процедура адресации
	if (_ds_address_defined) {										// Адрес определен?
		oneWire_write(0x55, _ds_pin);									// Говорим термометрам слушать адрес
		for (uint8_t i = 0; i < 8; i++) {								// Отправляем 8 байт уникального адреса
			oneWire_write(_ds_address[i], _ds_pin);						// Из массива который нам указал пользователь
		}
	} else oneWire_write(0xCC, _ds_pin);								// Адреса нет - пропускаем адресацию на линии
}
