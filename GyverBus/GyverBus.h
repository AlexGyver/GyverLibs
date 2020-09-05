#pragma once
#include <Arduino.h>

/*
	GyverBus - библиотека для общения по протоколу GBUS
	- Очень простой, надёжный, устойчивый к помехам и задержкам, но медленный интерфейс связи на базе UART
	- Двухсторонняя связь по одному проводу
	- Асинхронная отправка и чтение (на базе millis())
	- Двухсторонняя совместимость с аппаратным UART
	- Возможность принимать и отправлять данные внутри сети Ардуинок
	- Адресация до 254 устройств в сети (от 1 до 255)
	- Всеядная функция отправки и приёма (ест переменные, структуры, массивы)
	- Встроенная проверка CRC (контроль целостности) пакета данных
	- Возможность отправки и чтения короткого "запроса"
	- Поддерживаются все Arduino-совместимые платы
	- Сама библиотека предоставляет возможности по отладке (коды ошибок)
		- В примерах есть компактные варианты чтения и отправки данных, влезет даже в ATtiny
		
	- Версия 1.1: добавлена waitAck() и исправлена ошибочка
	- Версия 1.2: улучшена стабильность, функции оптимизированы, уменьшен вес
	- Версия 1.3: добавлен CRC в запрос и ответ
*/

/*
	Коды статусов и ошибок (tick() и getStatus()):
	0 	[GBUS_IDLE] 		- ожидание
	1 	[TRANSMITTING] 		- передача
	2	[TX_OVERFLOW]		- буфер переполнен
	3 	[TX_COMPLETE] 		- передача завершена
	4 	[RECEIVING] 		- приём
	5	[RX_ERROR] 			- ошибка приёма
	6	[RX_ABORT] 			- ошибка. Приём прерван
	7 	[RX_OVERFLOW] 		- ошибка. Буфер или пакет переполнен
	8 	[RX_ADDRESS_ERROR] 	- ошибка. Не наш адрес
	9 	[RX_CRC_ERROR] 		- ошибка. Не совпадает CRC
	10 	[RX_REQUEST] 		- успешное получение запроса
	11 	[RX_COMPLETE] 		- успешный приём данных
	12 	[RX_ACK]			- успешное получение подтверждения
*/

/*	
	Инициализация:
	GBus<пин, тип, crc> объект(адрес, размер буфера)
	- пин: номер пина
	- тип: GBUS_FULL - двухсторонняя связь (активен по умолчанию, можно не указывать)
	- тип: GBUS_TX - только отправка (экономит память)
	- тип: GBUS_RX - только приём (экономит память)
	- crc: GBUS_CRC - с контролем целостности данных CRC (активен по умолчанию, можно не указывать)
	- crc: GBUS_NO_CRC - отключить ожидание и проверку CRC
	
	Список методов:
	tick() - отправляет и принимает данные. Вызывать хотя бы каждую миллисекунду. Возвращает коды статусов, тип GBUSstatus (enum)
	getStatus() - возвращает коды статусов (такие же как tick()), тип GBUSstatus (enum)
	
	sendData(адрес, дата) - вводит данные на отправку по адресу получателя. Принимает любй тип данных (переменные, массивы, структуры). Отправка начнётся автоматически после "освобождения" линии
	gotData() - вернёт true, если новый пакет данных принят без ошибок. Сам сбрасывается в false
	readData(дата) - выводит данные из внутреннего буфера в указанную "переменную" (переменные, массивы, структуры)
	
	sendRequest(адрес) - отправить "запрос" по адресу получателя
	gotRequest() - вернёт true, если на наш адрес пришёл запрос. Сам сбрасывается в false. Адрес отправителя можно узнать в getTXaddress()	
	
	getTXaddress() - возвращает адрес отправителя с последнего успешного получения данных или запроса
	isBusy() - возвращает true, если линия занята. Возвращает false, если с момента последнего общения по линии прошло GBUS_BUSY_TIMEOUT миллисекунд
	
	sendAck(адрес) - отправляет ack (ответ на запрос) по адресу
	gotAck() - вернёт true, если на наш адрес пришёл ack (ответ на запрос). Сам сбрасывается в false. Адрес отправителя можно узнать в getTXaddress()	
	
	waitAck(адрес, кол-во попыток, таймаут) - ждёт ответа с указанного адреса. Если не дожидается за таймаут - отправляет ещё реквест
	И так далее пока не выйдет количество попыток.
	ФУНКЦИЯ НЕ БЛОКИРУЮЩАЯ
	Коды возврата:
	0 [ACK_IDLE] - функция работает вхолостую
	1 [ACK_WAIT] - ждём ответа
	2 [ACK_ERROR] - не дождались ответа
	3 [ACK_ONLY] - получен ack
	4 [ACK_DATA] - получена дата
	Смотри примеры wait_ack
	
	sendRequestAck(адрес, кол-во попыток, таймаут) - отправляет реквест на адрес. 
	Ждёт ответ (ack или data), если не дожидается в теч. таймаута - отправляет ещё раз.
	Коды возврата:
	2 [ACK_ERROR] - не дождались ответа
	3 [ACK_ONLY] - получен ack
	4 [ACK_DATA] - получена дата
	ФУНКЦИЯ БЛОКИРУЮЩАЯ
	Смотри примеры call_response_ack
	
	sendRaw(байтовый массив, размер) - отправит сырые байты (без протокола) по шине. Смотри примеры в папке raw	
	gotRaw() - вернёт true, были приняты какие-то данные (приём завершён успешно). Сам сбрасывается в false.
	rawSize() - вернёт количество принятых байт (последний успешный приём). Иначе вернёт 0
	
	buffer - можно обращаться как к массиву. Встроенный буфер библиотеки	
*/

/*
	Интерфейс: UART. start бит 0, stop бит 1. Кодирование даты: HIGH - 0x1, LOW - 0x0
	
	1  _______     ___        ___     ______     ___     .........._________
			  |___|   |______|   |___|      |___|   |___|                    конец передачи
	0         start 0   1   2  3   4   5  6   7 stop start         stop     
	
	Протокол: 
	Дата [суммарное количество байт, адрес получателя, адрес отправителя, ...байты даты..., CRC]
	Запрос (request) [0, адрес получателя, адрес отправителя, CRC]
	Подтверждение (ack) [1, адрес получателя, адрес отправителя, CRC]
	CRC может не передаваться 
*/

// НАСТРОЙКИ
#define GBUS_SPEED  300			// скорость (baud)
#define DEBUG_CODES				// закомментируй, чтобы сэкономить 160 байт =)
#define DELAY_OFFSET_WRITE 8	// коррекция задержки записи, мкс
#define DELAY_OFFSET_READ  5	// коррекция задержки чтения, мкс

// КОНСТАНТЫ
#define GBUS_RX 0
#define GBUS_TX 1
#define GBUS_FULL 2
#define GBUS_NO_CRC 0
#define GBUS_CRC 1

#define ACK_IDLE 0
#define ACK_WAIT 1
#define ACK_ERROR 2
#define ACK_ONLY 3
#define ACK_DATA 4

// ЗАМЕТКИ
// Отключение DEBUG_CODES экономит ~160 байт Flash
// Отключение CRC экономит 50-70 байт Flash
// GBUS_RX вместо GBUS_FULL экономит 432 байт Flash
// GBUS_TX вместо GBUS_FULL экономит 930 байт Flash
// Смотри GBUSmini.h, там куча простых лёгких функций отправки и чтения

#if (GBUS_SPEED > 200)
#define GBUS_DELAY(x) delayMicroseconds(x)
#define GBUS_BIT    (1000000UL / GBUS_SPEED)
#define GBUS_TIME micros
#else
#define GBUS_DELAY(x) delay(x)
#define GBUS_BIT    (1000UL / GBUS_SPEED)
#define GBUS_TIME millis
#define DELAY_OFFSET_WRITE 0
#define DELAY_OFFSET_READ  0
#endif

#define GBUS_BIT_2  (GBUS_BIT >> 1)
#define GBUS_BUSY_TIMEOUT 50

#ifdef DEBUG_CODES
#define _set_status_code(x) _status = (x)
#else
#define GBUSstatus int
#define _set_status_code(x)
#endif

#ifdef DEBUG_CODES
enum GBUSstatus {
	GBUS_IDLE,	
	TRANSMITTING,
	TX_OVERFLOW,	
	TX_COMPLETE,	
	RECEIVING,	
	RX_ERROR,
	RX_ABORT,
	RX_OVERFLOW,
	RX_ADDRESS_ERROR,	
	RX_CRC_ERROR,
	RX_REQUEST,	
	RX_COMPLETE,
	RX_ACK,
};
#endif

void GBUS_crc_update(uint8_t &crc, uint8_t data);
byte GBUS_crc_bytes(byte *data, byte size);
template <typename T> void packDataBytes(byte *buffer, T &data);
template <typename T> void unpackDataBytes(byte *buffer, T &data);

template <byte _PIN, byte _ROLE = GBUS_FULL, byte _CRC = GBUS_CRC>
class GBus {
public:
	enum TX_stage {
		TX_IDLE,
		TX_START,
		TX_SENDING,
		TX_FINISH,
	};
	enum RX_stage {
		RX_IDLE,
		RX_START,
		RX_READING,
		RX_FINISH,
	};	

	GBus(byte addr, byte bufSize)
	: _bufSize(bufSize + (_CRC ? 4 : 3)), _addr(addr) {
		buffer = malloc(_bufSize);
		memset(buffer, 0, _bufSize);		
		pinMode(_PIN, INPUT_PULLUP);
	}

	~GBus() {
		free(buffer);
	}

	GBUSstatus tick() {				
		if (_role == GBUS_RX) {								// приёмник
			if (_ROLE == GBUS_RX || _ROLE == GBUS_FULL) {	// компилятор вырежет при выбранной роли
				byte bit = digitalRead(_PIN);
				switch(_rxstage) {
				case RX_IDLE:
					if (!bit) {
						_tmr = GBUS_TIME();
						_rxstage = RX_START;
					}
					break;
					
				case RX_START:
					if (GBUS_TIME() - _tmr >= GBUS_BIT_2) {
						if (!bit) {
							_rxstage = RX_READING;
							_set_status_code(RECEIVING);
							_bitCount = 0;
							_byteCount = 0;
							_crc = 0;
							_readFlag = false;
							_ackFlag = false;
							_requestFlag = false;
							_rawFlag = false;
							_rawSize = 0;
							_tmr += GBUS_BIT_2;
						} else {
							_rxstage = RX_FINISH;
							_set_status_code(RX_ERROR);
						}				
					}
					break;
					
				case RX_READING:
					if (GBUS_TIME() - _tmr >= GBUS_BIT) {
						_tmr += GBUS_BIT;
						_lastPulse = millis();
						
						if (_bitCount < 8) {						// передача битов даты						
							bitWrite(buffer[_byteCount], _bitCount, bit);  // пишем в буфер
						} else if (_bitCount == 8) {				// стоп бит (high)
							if (!bit) {                   			// ошибка стоп бита. Завершаем
								_rxstage = RX_FINISH;
								_set_status_code(RX_ERROR);
								return _status;
							}
							if (_CRC) GBUS_crc_update(_crc, buffer[_byteCount]);
							_byteCount++;							// счётчик собранных байтов
						} else if (_bitCount == 9) {				// старт бит (ожидаем low)
							_bitCount = -1;          				// костыль
							if (bit) {             					// не дождались нового старт бита. Конец приёма?
								_rawFlag = true;
								_rawSize = _byteCount;
								_txAddress = buffer[2];
								if (_CRC) {
									if (_crc != 0) {						// не совпал CRC
										_rxstage = RX_FINISH;
										_set_status_code(RX_CRC_ERROR);
										return _status;
									}
								}
								if (buffer[1] != _addr) {					// адрес не совпал
									_rxstage = RX_FINISH;
									_set_status_code(RX_ADDRESS_ERROR);
									return _status;
								}
								if (_byteCount == _bufOffset && buffer[0] == 0) {	// реквест									
									_rxstage = RX_FINISH;
									_set_status_code(RX_REQUEST);
									_requestFlag = true;
									return _status;
								}
								if (_byteCount == _bufOffset && buffer[0] == 1) {	// подтверждение									
									_rxstage = RX_FINISH;
									_set_status_code(RX_ACK);
									_ackFlag = true;
									return _status;
								}
								if (_byteCount < buffer[0]) {				// передача прервана
									_rxstage = RX_FINISH;
									_set_status_code(RX_ABORT);
									return _status;
								}								
								
								// данные приняты
								_readFlag = true;
								_set_status_code(RX_COMPLETE);
								_rxstage = RX_FINISH;
								return _status;
							}
							if (_byteCount > 2) {					// мин. размер пакета
								if (_byteCount > _bufSize) {   		// буфер переполнен, завершаем									
									_rxstage = RX_FINISH;
									_set_status_code(RX_OVERFLOW);
								}
								if (_byteCount >= buffer[0]) {		// пакет переполнен (ну и *** с ним)									
									_set_status_code(RX_OVERFLOW);
								}
							}							
						}
						_bitCount++;								// следующий бит
					}
					break;
					
				case RX_FINISH:
					_rxstage = RX_IDLE;
					_set_status_code(GBUS_IDLE);					
					break;
				}
			}				
		} else {												// передатчик	
			if (_ROLE == GBUS_TX || _ROLE == GBUS_FULL) {		// компилятор вырежет при выбранной роли
				switch(_txstage) {
				case TX_IDLE:					
					break;
					
				case TX_START:
					if (!isBusy()) {
						_txstage = TX_SENDING;
						_bitCount = -1;
						_byteCount = 0;
						_set_status_code(TRANSMITTING);
						_tmr = GBUS_TIME()/* - GBUS_BIT*/;
					}
					break;
					
				case TX_SENDING:
					if (GBUS_TIME() - _tmr >= GBUS_BIT) {						
						bool bit;
						if (_bitCount < 0) bit = 0;						
						else if (_bitCount < 8) bit = (buffer[_byteCount] >> _bitCount) & 1;						
						else {
							bit = 1;							
							_byteCount++;
							if (_byteCount == _txSize) {
								_txstage = TX_FINISH;
								_set_status_code(TX_COMPLETE);
								_lastPulse = millis();
							}
							_bitCount = -2;
						}
						pinMode(_PIN, !bit);
						digitalWrite(_PIN, bit);						
						_bitCount++;
						_tmr += GBUS_BIT;
					}
					break;
					
				case TX_FINISH:					
					if (_ROLE == GBUS_FULL) _role = GBUS_RX;
					_set_status_code(GBUS_IDLE);
					_txstage = TX_IDLE;					
					break;
				}
			}
		}
		return _status;
	}
	
	GBUSstatus getStatus() {return _status;}
	
	bool gotData() {
		bool temp = _readFlag;
		_readFlag = false;
		return temp;
	}
	
	byte getTXaddress() {return _txAddress;}
	
	bool isBusy() {		
		return !(GBUS_TIME() - _lastPulse > GBUS_BUSY_TIMEOUT);
	}
	
	// REQUEST
	void sendRequest(byte addr) {
		if (_ROLE == GBUS_TX || _ROLE == GBUS_FULL) {
			buffer[0] = 0;  		// размер даты (0-запрос)
			buffer[1] = addr;       // адрес приёмника
			buffer[2] = _addr;		// адрес передатчика (наш)
			
			if (_CRC) buffer[3] = GBUS_crc_bytes(buffer, 3);
			
			_txstage = TX_START;  	// флаг на передачу
			_role = GBUS_TX;		// теперь ты отправитель
			_txSize = _bufOffset;	// отправляем _bufOffset байта
			if (_ackStage == ACK_IDLE) {
				_ackTimer = millis();
				_ackTries = 0;
				_ackStage = ACK_WAIT;
			}
		}
	}
	
	byte waitAck(byte addr, byte tries, int timeout) {
		switch (_ackStage) {
		case ACK_IDLE:
			break;
			
		case ACK_WAIT:
			if (gotData()) {
				_ackStage = ACK_IDLE;
				return ACK_DATA;
			}
			if (gotAck()) {
				_ackStage = ACK_IDLE;
				return ACK_ONLY;
			}
			if (millis() - _ackTimer >= timeout) {
				_ackTimer = millis();
				_ackTries++;
				if (_ackTries >= tries) {
					_ackStage = ACK_IDLE;
					return ACK_ERROR;
				}
				sendRequest(addr);	
			}
			break;
		}
		return _ackStage;
	}
	
	bool gotRequest() {
		bool temp = _requestFlag;
		_requestFlag = false;
		return temp;
	}
	
	// ACK
	void sendAck(byte addr) {
		if (_ROLE == GBUS_TX || _ROLE == GBUS_FULL) {
			buffer[0] = 1;  		// размер даты (1-ack)
			buffer[1] = addr;       // адрес приёмника
			buffer[2] = _addr;		// адрес передатчика (наш)
			if (_CRC) buffer[3] = GBUS_crc_bytes(buffer, 3);
			_txstage = TX_START;  	// флаг на передачу
			_role = GBUS_TX;		// теперь ты отправитель
			_txSize = _bufOffset;	// отправляем _bufOffset байт
		}
	}
	
	bool gotAck() {
		bool temp = _ackFlag;
		_ackFlag = false;
		return temp;
	}
	
	byte sendRequestAck(byte addr, byte tries, int timeout) {
		if (_ROLE == GBUS_TX || _ROLE == GBUS_FULL) {
			sendRequest(addr);
			while (tick() == GBUS_IDLE);	// ждём таймаут
			while (tick() != TX_COMPLETE);	// ждём отправку
			byte thisTry = 0;
			uint32_t tmr = millis();
			while (1) {
				tick();								// принимаем или отправляем
				if (gotData()) return ACK_DATA;		// приняли дату
				if (gotAck()) return ACK_ONLY;		// приняли ответ
				if (millis() - tmr >= timeout) {	// таймаут
					tmr = millis();
					thisTry++;
					if (thisTry >= tries) return ACK_ERROR;	// превышено количество попыток
					sendRequest(addr);				// снова шлём запрос
				}
			}
		}
		return ACK_ERROR;
	}
	
	// RAW
	void sendRaw(byte* data, byte size) {
		if (_ROLE == GBUS_TX || _ROLE == GBUS_FULL) {
			byte i = 0;
			for (; i < min(size, _bufSize); i++) buffer[i] = data[i];
			_txSize = i;
			_txstage = TX_START;  	// флаг на начало передачи
			_role = GBUS_TX;		// теперь мы отправитель
		}
	}
	
	bool gotRaw() {
		bool temp = _rawFlag;
		_rawFlag = false;
		return temp;
	}
	
	byte rawSize() {return _rawSize;}
	
	// send-read
	template <typename T>
	void sendData(byte addr, T &data) {
		if (_ROLE == GBUS_TX || _ROLE == GBUS_FULL) {
			buffer[0] = sizeof(T) + _bufOffset;  	// размер пакета
			if (buffer[0] > _bufSize) {				// переполним буфер
				_txstage = TX_FINISH;
				_set_status_code(TX_OVERFLOW);
				return;
			}
			buffer[1] = addr;       				// адрес приёмника
			buffer[2] = _addr;						// адрес передатчика (наш)			
			const uint8_t *ptr = (const uint8_t*) &data;
			for (uint16_t i = 0; i < sizeof(T); i++) {
				buffer[i + 3] = *ptr++;
			}		
			
			// crc
			if (_CRC) buffer[sizeof(T) + 3] = GBUS_crc_bytes(buffer, sizeof(T) + 3);
			
			_txSize = sizeof(T) + _bufOffset;
			_txstage = TX_START;  	// флаг на начало передачи
			_role = GBUS_TX;		// теперь мы отправитель
		}
	}

	template <typename T>
	void readData(T &data) {
		uint8_t *ptr = (uint8_t*) &data;
		for (uint16_t i = 0; i < sizeof(T); i++) {
			*ptr++ = buffer[i + 3];
		}
	}
	
	// buffer
	byte *buffer;
	
private:
	const int _bufSize;
	int8_t _bitCount = 0;
	byte _txSize = 0, _byteCount = 0;;
	uint32_t _tmr;
	uint32_t _lastPulse;
	byte _txAddress;
	byte _addr;
	byte _bufOffset = _CRC ? 4 : 3;
	byte _crc = 0;
	
	byte _role = GBUS_RX;
	TX_stage _txstage = TX_IDLE;
	RX_stage _rxstage = RX_IDLE;
	
#ifdef DEBUG_CODES
	GBUSstatus _status = GBUS_IDLE;
#else
	int _status = 0;
#endif	
	
	bool _readFlag = false;
	bool _requestFlag = false;
	bool _ackFlag = false;
	bool _rawFlag = false;
	byte _rawSize = 0;
	
	uint32_t _ackTimer;
	byte _ackTries = 0;
	byte _ackStage = 0;
};

// =============== УТИЛИТЫ ===============
// пакуем любой тип данных в байтовый буфер
template <typename T>
void packDataBytes(byte *buffer, T &data) {
	const uint8_t *ptr = (const uint8_t*) &data;
	for (uint16_t i = 0; i < sizeof(T); i++) {
		buffer[i] = *ptr++;
	}
}

// распаковываем из байтового буфера обратно
template <typename T>
void unpackDataBytes(byte *buffer, T &data) {
	uint8_t *ptr = (uint8_t*) &data;
	for (uint16_t i = 0; i < sizeof(T); i++) {
		*ptr++ = buffer[i];
	}
}

// расчёт CRC
void GBUS_crc_update(uint8_t &crc, uint8_t data) {
#if defined (__AVR__)
	// резкий алгоритм для AVR
	uint8_t counter;
	uint8_t buffer;
	asm volatile (
	"EOR %[crc_out], %[data_in] \n\t"
	"LDI %[counter], 8          \n\t"
	"LDI %[buffer], 0x8C        \n\t"
	"_loop_start_%=:            \n\t"
	"LSR %[crc_out]             \n\t"
	"BRCC _loop_end_%=          \n\t"
	"EOR %[crc_out], %[buffer]  \n\t"
	"_loop_end_%=:              \n\t"
	"DEC %[counter]             \n\t"
	"BRNE _loop_start_%="
	: [crc_out]"=r" (crc), [counter]"=d" (counter), [buffer]"=d" (buffer)
	: [crc_in]"0" (crc), [data_in]"r" (data)
	);
#else
	// обычный для всех
	uint8_t i = 8;
	while (i--) {
		crc = ((crc ^ data) & 1) ? (crc >> 1) ^ 0x8C : (crc >> 1);
		data >>= 1;
	}
#endif
}

byte GBUS_crc_bytes(byte *data, byte size) {
	byte crc = 0;
	for (byte i = 0; i < size; i++) GBUS_crc_update(crc, data[i]);
	return crc;
}