#pragma once
// Это оболочка протокола GBUS на любой интерфейс связи Stream (Serial, программный Serial)
// Обеспечивает асинхронный приём данных по Serial (в отличие от readBytes())
// Полностью совместима на приём-передачу с остальными способами общения по GBUS
// Можно использовать родной аппаратный UART с библиотеками Serial (+Serial1 Serial2...) или любой другой встроенной в ядро
// Можно работать с аппаратным Serial и SoftwareSerial на любой платформе в пределах "экосистемы" Ардуино
// В отличие от основной GBUS тут более высокая возможная скорость и надёжная передача, но для двухсторонней связи нужны два провода
// (только отправлять или только принимать можно и по одному)

/*
	Коды статусов и ошибок (tick() и getStatus()):
	0 	[GBUS_IDLE] 		- ожидание (холостой режим)
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
	11 	[RX_DATA] 			- успешный приём данных
	12 	[RX_ACK]			- успешное получение подтверждения
*/

/*	
	Инициализация:
	GBUS объект(обработчик, адрес, размер буфера)
	- обработчик: адрес объекта-обработчика интерфейса (например &Serial, &mySerial)
	- адрес: адрес этого устрйоства в сети
	- размер буфера: размер буфера
	
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
	
	Протокол: 
	Дата [суммарное количество байт, адрес получателя, адрес отправителя, ...байты даты..., CRC]
	Запрос (request) [0, адрес получателя, адрес отправителя, CRC]
	Подтверждение (ack) [1, адрес получателя, адрес отправителя, CRC]
	CRC опционален
*/

// ============================ НАСТРОЙКИ ================================
// таймаут отправки после предыдущей активности на линии, мкс
// он же МАКСИМАЛЬНЫЙ таймаут приёма (в процессе работы пересчитывается)
#define GBUS_BUSY_TIMEOUT 50000	

// =======================================================================
#include "GyverBus.h"

class GBUS {
public:
	GBUS(Stream* serial, byte addr, byte bufSize) : _bufSize(bufSize + GBUS_OFFSET), _addr(addr) {
		buffer = (byte *)malloc(_bufSize);
		memset(buffer, 0, _bufSize);
		port = serial;
	}

	~GBUS() {
		free(buffer);
	}

	// ==== ШИНА ====
	bool isBusy() {
		return !(micros() - _tmr > GBUS_BUSY_TIMEOUT);
	}

	// в данной реализации это только функция ПАРСИНГА данных
	GBUSstatus tick() {
		int state = readBytesAsync(buffer, _bufSize);
		if (state <= 0) {
			switch (state) {
			case -2: _status = GBUS_IDLE; break;
			case -1: _status = RX_OVERFLOW; break;
			case 0: _status = RECEIVING; break;
			}
		} else {
			// если принят пакет
			_status = checkGBUS(buffer, _bufSize, state, _addr);
			switch (_status) {
			case RX_REQUEST: _requestFlag = true; break;
			case RX_ACK: _ackFlag = true; break;
			case RX_DATA: _dataFlag = true; break;
			}
			_rawFlag = true;
			_rawSize = state;
			_txAddress = buffer[2];
		}
		return _status;
	}
	
	GBUSstatus getStatus() {return _status;}
	byte getTXaddress() {return _txAddress;}

	// асинхронное чтение потока байтов. Возвращает:
	// -2: приём не идёт
	// -1: буфер переполнен
	// 0: приём идёт
	// кол-во байт: передача завершена
	int readBytesAsync(byte* buffer, byte size) {
		while (port->available()) {				// пока есть данные в буфере сериал
			if (!_parseFlag) {					// начало приёма			
				_parseFlag = true;				// ключ на старт
				_byteCount = 0;					// сбросили счетчик
				_timeout = GBUS_BUSY_TIMEOUT;	// сбросили таймаут
			}
			buffer[_byteCount++] = port->read();// читаем
			if (_byteCount == 2) _timeout = (micros() - _tmr) * 10;	// таймаут как время передачи 10 бит
			if (_byteCount > size) {			// буфер переполнен			
				_parseFlag = false;
				return -1;
			}
			_tmr = micros();			// сброс таймера (идёт приём)			
			return 0;
		}

		// таймаут ожидания приёма
		if (_parseFlag && micros() - _tmr >= _timeout) {
			_parseFlag = false;			// приём окончен
			return _byteCount;
		}
		return -2;
	}

	// ===== REQUEST =====
	// отправить
	void sendRequest(byte to) {
		_txSize = packGBUScmd(buffer, 0, to, _addr);
		sendRaw(buffer, _txSize);
		if (_ackStage == ACK_IDLE) {
			_ackTimer = millis();
			_ackTries = 0;
			_ackStage = ACK_WAIT;
		}
	}

	// принять
	bool gotRequest() {
		bool temp = _requestFlag;
		_requestFlag = false;
		return temp;
	}

	// ждать ответа асинхронно
	byte waitAck(byte to, byte tries, int timeout) {
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
				sendRequest(to);
			}
			break;
		}
		return _ackStage;
	}

	// ждать ответа (блокирующая функция)
	byte sendRequestAck(byte to, byte tries, int timeout) {
		sendRequest(to);
		while (tick() == GBUS_IDLE);			// ждём таймаут
		while (tick() != TX_COMPLETE);			// ждём отправку
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
				sendRequest(to);				// снова шлём запрос
			}
		}
		return ACK_ERROR;						// не дождались
	}


	// ===== ACK =====
	// отправить
	void sendAck(byte to) {
		_txSize = packGBUScmd(buffer, 1, to, _addr);
		sendRaw(buffer, _txSize);
	}

	// принять
	bool gotAck() {
		bool temp = _ackFlag;
		_ackFlag = false;
		return temp;
	}

	// ===== RAW =====
	void sendRaw(byte* data, byte size) {
		for (byte i = 0; i < size; i++) port->write(data[i]);
		_status = TX_COMPLETE;
	}

	bool gotRaw() {
		bool temp = _rawFlag;
		_rawFlag = false;
		return temp;
	}

	byte rawSize() {return _rawSize;}

	// ===== DATA =====
	// принять
	bool gotData() {
		bool temp = _dataFlag;
		_dataFlag = false;
		return temp;
	}

	// отправить
	template <typename T>
	bool sendData(byte to, T &data) {
		_txSize = packGBUSdata(buffer, _bufSize, data, to, _addr);
		if (_txSize == 0) {
			_status = TX_OVERFLOW;
			return false;
		}
		sendRaw(buffer, _txSize);
		return true;
	}

	// прочитать
	template <typename T>
	bool readData(T &data) {
		return unpackGBUSdata(buffer, _bufSize, data);
	}

	// ===== БУФЕР =====
	byte *buffer;

private:
	const int _bufSize;
	uint32_t _tmr;
	uint32_t _ackTimer;
	long _timeout = GBUS_BUSY_TIMEOUT;
	byte _txAddress;
	byte _txSize = 0;
	byte _byteCount = 0;
	byte _addr;
	byte _rawSize = 0;
	bool _parseFlag = false;
	bool _dataFlag = false;
	bool _requestFlag = false;
	bool _ackFlag = false;
	bool _rawFlag = false;
	byte _ackTries = 0;
	byte _ackStage = 0;
	GBUSstatus _status = GBUS_IDLE;
	Stream* port;
};