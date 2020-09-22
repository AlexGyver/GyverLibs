#pragma once
#include <Arduino.h>
/*
	NecDecoder.h  - легкий класс для декодирования ИК протокола NEC (Японский вариант) + отработка команды повтора
	Требует вызова специального метода при отрицательном (FALLING) фронте на пине ИК приемника
	Отсчет времени базируется на micros
	Хранение ОДНОГО последнего декодированного пакета данных в буфере

	NEC пакет: (32 бита)
	| START | LSB - 8 бит адрес - MSB | LSB - 8 бит !адрес - MSB | LSB - 8 бит команда - MSB | LSB - 8 бит !команда - MSB |
	1 байт адрес + 1 байт команда (и их инвертированные копии)
*/

#define _NEC_TOLERANCE 150                                        // Допуск таймингов протокола в микросекундах (10-250)

class NecDecoder {
public:
	NecDecoder;
	void tick(void);                                              // Вызывается при ОТРИЦАТЕЛЬНОМ (FALLING) фронте на пине ИК приемника, например в прерывании
	bool isDecoded(void);                                         // Возвращает true когда очередной пакет декодирован
	bool isRepeat(void);                                          // Возвращает true если принят флаг повтора команды
	uint32_t readPacket(void);                                    // Прочитать весь пакет целиком (адрес + ~адрес + команда + ~команда)
	uint8_t readAddress(void);                                    // Прочитать только байт с адресом
	uint8_t readInvAddress(void);                                 // Прочитать только байт с инвертированным адресом
	uint8_t readCommand(void);                                    // Прочитать только байт с командой
	uint8_t readInvCommand(void);                                 // Прочитать только байт с инвертированной командой
	bool addressIsValid(void);                                    // Вернет true если адрес прошел проверку
	bool commandIsValid(void);                                    // Вернет true если команда прошла проверку
	bool packetIsValid(void);                                     // Вернет true если весь пакет прошел проверку
	
private:
	volatile uint32_t _packet_buffer = 0;
	volatile uint32_t _temp_buffer = 0;
	volatile uint32_t _last_edge = 0;
	volatile uint8_t _remain_counter = 32;
	volatile bool _decoded_flag = false;
	volatile bool _repeat_flag = false;
};