#pragma once
#include <GyverBus.h>
//#define GBUS_MINI_NO_CRC	// раскомментируй или добавь в скетч, чтобы отключить CRC. Уменьшает размер функций на 30 байт для тини13

// ******* ЛЁГКИЕ ФУНКЦИИ ОТПРАВКИ И ЧТЕНИЯ GBUS *******
// Все функции (кроме GBUS_is_busy) блокируют выполнение кода на время приёма/отправки

// Возвращает true, если линия занята. Таймаут GBUS_BUSY_TIMEOUT мс
bool GBUS_is_busy(byte pin);

// Отправить сырые данные (пин, дата, размер)
// Формат посылки GBUS [суммарное количество байт, адрес получателя, адрес отправителя, ...байты даты..., CRC]
// Функция блокирующая на всё время отправки
// 640 байт (Arduino Nano), 88 байт ATtiny13 (microCore)
void GBUS_send_raw(byte pin, byte* data, byte size);

// Отправить данные с CRC (пин, адрес получателя, адрес отправителя, дата, размер)
// Функция блокирующая на всё время отправки
// 740 байт (Arduino Nano), 160 байт ATtiny13 (microCore)
void GBUS_send(byte pin, byte to, byte from, byte* data, byte size);

// Прочитать сырые данные (пин, дата, размер)
// Вызывать как можно чаще, чтобы не пропустить. Функция блокирующая на всё время приёма.
// Возвращает количество принятых байт при завершении приёма
// 520 байт (Arduino Nano), 220 байт ATtiny13 (microCore)
byte GBUS_read_raw(byte pin, byte* buf, byte size);

// Прочитать данные с CRC (пин, наш адрес, дата, размер)
// Вызывать как можно чаще, чтобы не пропустить. Функция блокирующая на всё время приёма
// Возвращает адрес отправителя при успешном завершении приёма. При ошибке возвращает 0
// 650 байт (Arduino Nano), 330 байт ATtiny13 (microCore)
byte GBUS_read(byte pin, byte addr, byte* buf, byte size);

// Отправить запрос (пин, адрес получателя, адрес отправителя)
// Функция блокирующая на всё время отправки
// 640 байт (Arduino Nano), 150 байт ATtiny13 (microCore)
void GBUS_send_request(byte pin, byte to, byte from);

// Принять запрос (пин, наш адрес)
// Вызывать как можно чаще, чтобы не пропустить. Функция блокирующая на всё время приёма
// Возвращает адрес отправителя при успешном завершении приёма. При ошибке возвращает 0
// 540 байт (Arduino Nano), 220 байт ATtiny13 (microCore)
byte GBUS_read_request(byte pin, byte addr);

// Отправить подтверждение (пин, адрес получателя, адрес отправителя)
// Функция блокирующая на всё время отправки
void GBUS_send_ack(byte pin, byte to, byte from);

// Принять подтверждение (пин, наш адрес)
// Вызывать как можно чаще, чтобы не пропустить. Функция блокирующая на всё время приёма
// Возвращает адрес отправителя при успешном завершении приёма. При ошибке возвращает 0
byte GBUS_read_ack(byte pin, byte addr);

// Отправить запрос и ждать подтверждения приёма, т.е. пытаться достучаться
// (пин, адрес получателя, адрес отправителя, кол-во попыток, таймаут между попытками)
// возвращает 0 при таймауте, 1 при успехе (получили ack)
// см. примеры call response _ack
byte GBUS_send_request_ack(byte pin, byte to, byte from, byte tries, int timeout);


// *******************************************************
// ********************** СЕРВИС *************************
// *******************************************************
#ifndef GBUS_MINI_NO_CRC
#define GBUS_OFFSET 4
#else
#define GBUS_OFFSET 3
#endif

byte GBUS_send_request_ack(byte pin, byte to, byte from, byte tries, int timeout) {
	GBUS_send_request(pin, to, from);
	byte thisTry = 0;
	uint32_t tmr = millis();
	while (1) {
		byte addr = GBUS_read_ack(pin, from);
		if (addr == to) return ACK_ONLY;
		if (millis() - tmr >= timeout) {
			tmr = millis();
			thisTry++;
			if (thisTry >= tries) return ACK_ERROR;
			GBUS_send_request(pin, to, from);
		}
	}		
	return ACK_ERROR;
}

// *******************************************************
bool GBUS_is_busy(byte pin) {
	static uint32_t tmr;
	if (digitalRead(pin)) {
		if (millis() - tmr >= GBUS_BUSY_TIMEOUT) return false;
		else return true;
	} else {
		tmr = millis();
		return true;
	}
}

// *******************************************************
// ********************* ЧТЕНИЕ **************************
// *******************************************************
byte GBUS_read_raw(byte pin, byte* buf, byte size) {
	if (!digitalRead(pin)) {    	// проверяем старт бит (low)
		GBUS_DELAY(GBUS_BIT_2);		// ждём половину времени
		if (!digitalRead(pin)) { 	// если всё ещё старт бит (low)
			int8_t bitCount = 0;	// счётчик битов
			byte byteCount = 0;		// счётчик байтов
			while (1) {				
				GBUS_DELAY(GBUS_BIT-DELAY_OFFSET_READ);			// ждём бит
				byte bit = digitalRead(pin);					// читаем
				if (bitCount < 8) {								// передача битов даты
					bitWrite(buf[byteCount], bitCount, bit);  	// пишем в буфер
				} else if (bitCount == 8) {						// стоп бит (high)
					if (!bit) return 0;                   		// ошибка стоп бита. Завершаем
					byteCount++;								// счётчик собранных байтов					
				} else if (bitCount == 9) {						// старт бит (low)					
					if (bit) return byteCount;                	// не дождались старт бита. Конец приёма, возврат количества
					if (byteCount >= size) return 0;        	// буфер переполнен. Завершаем
					bitCount = -1;          					// костыль
				}
				bitCount++;										// следующий бит
			}
		}
	}
	return 0;
}


// *******************************************************
byte GBUS_read(byte pin, byte addr, byte* buf, byte size) {
	byte buf2[size + GBUS_OFFSET];									// буфер на приём
	byte bytes = GBUS_read_raw(pin, buf2, (size + GBUS_OFFSET));	// принимаем, получаем количество байт посылки
	if (buf2[0] == bytes && buf2[1] == addr) {						// если совпало количество байт и адрес
#ifndef GBUS_MINI_NO_CRC
		if (GBUS_crc_bytes(buf2, bytes) != 0) return 0;
#endif
		for (byte i = 0; i < bytes - GBUS_OFFSET; i++) buf[i] = buf2[i + 3];	// переписываем в буфер в скетче
		return buf2[2];												// возвращаем адрес
	}
	return 0;														// иначе возвращаем ошибку
}

// *******************************************************
// структура буфера: [0, адрес получателя, адрес отправителя, CRC]
byte GBUS_read_request(byte pin, byte addr) {
	byte buf[GBUS_OFFSET];	
	if (GBUS_read_raw(pin, buf, GBUS_OFFSET) == GBUS_OFFSET 
			&& buf[1] == addr
#ifndef GBUS_MINI_NO_CRC
			&& GBUS_crc_bytes(buf, GBUS_OFFSET) == 0
#endif
			&& buf[0] == 0) return buf[2];
	else return 0;
}

// *******************************************************
// структура буфера: [1, адрес получателя, адрес отправителя, CRC]
byte GBUS_read_ack(byte pin, byte addr) {
	byte buf[GBUS_OFFSET];	
	if (GBUS_read_raw(pin, buf, GBUS_OFFSET) == GBUS_OFFSET 
			&& buf[1] == addr 
#ifndef GBUS_MINI_NO_CRC
			&& GBUS_crc_bytes(buf, GBUS_OFFSET) == 0
#endif
			&& buf[0] == 1) return buf[2];
	else return 0;
}


// *******************************************************
// ******************** ОТПРАВКА *************************
// *******************************************************
void GBUS_send_raw(byte pin, byte* buf, byte size) {
	for (byte bytes = 0; bytes < size; bytes++) {
		for (int8_t bits = -1; bits < 9; bits++) {
			bool bit = 0;
			if (bits < 0) bit = 0;
			else if (bits < 8) bit = (buf[bytes] >> bits) & 1;
			else bit = 1;
			pinMode(pin, !bit);
			digitalWrite(pin, bit);
			GBUS_DELAY(GBUS_BIT-DELAY_OFFSET_WRITE);
		}
	}
}

// *******************************************************
void GBUS_send(byte pin, byte to, byte from, byte* data, byte size) {
	byte buf[size + GBUS_OFFSET];
	byte crc = 0;
	buf[0] = size + GBUS_OFFSET;
	buf[1] = to;
	buf[2] = from;
	for (byte i = 0; i < size; i++) buf[i + 3] = data[i];
#ifndef GBUS_MINI_NO_CRC
	buf[size + 3] = GBUS_crc_bytes(buf, size + 3);
#endif
	GBUS_send_raw(pin, buf, size+GBUS_OFFSET);
}

// *******************************************************
void GBUS_send_request(byte pin, byte to, byte from) {
	byte buf[GBUS_OFFSET] = {0, to, from};
#ifndef GBUS_MINI_NO_CRC
	buf[3] = GBUS_crc_bytes(buf, 3);
#endif
	GBUS_send_raw(pin, buf, GBUS_OFFSET);
}

// *******************************************************
void GBUS_send_ack(byte pin, byte to, byte from) {
	byte buf[GBUS_OFFSET] = {1, to, from};
#ifndef GBUS_MINI_NO_CRC
	buf[3] = GBUS_crc_bytes(buf, 3);
#endif
	GBUS_send_raw(pin, buf, GBUS_OFFSET);
}