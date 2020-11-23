#include "NecDecoder.h"
#define _inRange(a,from,to) (a >= from and a <= to ? true : false) // Возвращает true если 'a' лежит в пределах от from до to

void NecDecoder::tick(void) {
	uint32_t period = micros() - _last_edge;                                      // Получаем время последнего импульса
	_last_edge = micros();                                                        // Обновляем таймер

	if (_inRange(period, 1125 - _NEC_TOLERANCE, 1125 + _NEC_TOLERANCE)) {          // (1125 +- _NEC_TOLERANCE) - '0' Бит
		_remain_counter--;                                                          // Декремент счетчика
		_temp_buffer = _temp_buffer << 1;                                           // Сдвиг на 1 влево (ничего не прибавляем)
	} else if (_inRange(period, 2250 - _NEC_TOLERANCE, 2250 + _NEC_TOLERANCE)) {   // (2250 +- _NEC_TOLERANCE) - '1' Бит
		_remain_counter--;                                                          // Декремент счетчика
		_temp_buffer = _temp_buffer << 1 | 1;                                       // Сдвиг на 1 влево (прибавляем 1)
	} else if (_inRange(period, 11250 - _NEC_TOLERANCE, 11250 + _NEC_TOLERANCE)) { // (11250 +- _NEC_TOLERANCE) - Повтор
		_repeat_flag = true;                                                        // Устанавливаем флаг повтора
	} else if (_inRange(period, 13500 - _NEC_TOLERANCE, 13500 + _NEC_TOLERANCE)) { // (13500 +- _NEC_TOLERANCE) - Начало пакета
		if (!_remain_counter) {                                                     // Если все 32 бита из предыдущего пакета приняли
			_packet_buffer = _temp_buffer;                                            // Перемещаем пакет из составного буфера в основной
			_decoded_flag  = true;                                                    // Устанавливаем флаг успешного приема пакета
		} _temp_buffer =  0;                                                        // Очищаем составной буфер
		_remain_counter = 32;                                                       // Сбрасываем счетчик
	}
}

bool NecDecoder::isDecoded(void) {                                              // Возвращает true если очередной пакет декодирован
	bool temp = _decoded_flag;                                                    // Сохраняем состояние флага
	_decoded_flag = false;                                                        // Принудительно очищаем флаг
	return temp;                                                                  // Возвращаем предыдущее состояннеи флага
}

bool NecDecoder::isRepeat(void) {                                               // Возвращает true если принята команда повтора
	bool temp = _repeat_flag;                                                     // Сохраняем состояние флага
	_repeat_flag = false;                                                         // Принудительно очищаем флаг
	return temp;                                                                  // Возвращаем предыдущее состояннеи флага
}

uint32_t NecDecoder::readPacket(void) {                                         // Возвращает принятый пакет целиком
	return _packet_buffer;
}

uint8_t NecDecoder::readAddress(void) {                                         // Прочитать только байт с адресом
	return (uint32_t)(_packet_buffer >> 24);                                      // Вернуть адрес [Биты 31:24]
}

uint8_t NecDecoder::readInvAddress(void) {                                      // Прочитать только байт с инвертированным адресом
	return (uint32_t)(_packet_buffer >> 16 & 0xFF);                               // Вернуть ~адрес [Биты 23:16]
}

uint8_t NecDecoder::readCommand(void) {                                         // Прочитать только байт с командой
	return (uint32_t)(_packet_buffer >> 8 & 0xFF);                                // Вернуть команду [Биты 15:8]
}

uint8_t NecDecoder::readInvCommand(void) {                                      // Прочитать только байт с инвертированной командой
	return (uint32_t)(_packet_buffer & 0xFF);                                     // Вернуть ~команду [Биты 7:0]
}

bool NecDecoder::addressIsValid(void) {                                        // Вернет true если адрес прошел проверку
	return !(readAddress() & readInvAddress());                                  // Наложение инвертированной битовой маски вернет 0 при совпадении (инвертируем результат)
}

bool NecDecoder::commandIsValid(void) {                                        // Вернет true если коанда прошла проверку
	return !(readCommand() & readInvCommand());                                  // Наложение инвертированной битовой маски вернет 0 при совпадении (инвертируем результат)
}

bool NecDecoder::packetIsValid(void) {                                         // Вернет true если весь пакет прошел проверку
	return addressIsValid() & commandIsValid();                                  // Вернет true только если обе функции вернут true
}