#ifndef GyverMAX7219_h
#define GyverMAX7219_h
// самая резкая библиотека для матриц MAX7219 на диком западе
// v1.1 - оптимизирован SPI

#include <Arduino.h>
#include <SPI.h>
#include <GyverGFX.h>
#include "FastIO.h"

#ifndef MAX_SPI_SPEED
#define MAX_SPI_SPEED 1000000
#endif

static SPISettings MAX_SPI_SETT(MAX_SPI_SPEED, MSBFIRST, SPI_MODE0);

template < byte width, byte height, byte CSpin, byte DATpin = 0, byte CLKpin = 0 >
class MAX7219 : public GyverGFX {
public:
	MAX7219() : GyverGFX(width * 8, height * 8) {
		begin();
	}
	void begin() {
		pinMode(CSpin, OUTPUT);
		if (DATpin == CLKpin) {
			SPI.begin();			
		} else {			
			pinMode(DATpin, OUTPUT);
			pinMode(CLKpin, OUTPUT);
		}		
		sendCMD(0x0f, 0x00);  // отключить режим теста
		sendCMD(0x09, 0x00);  // выключить декодирование
		sendCMD(0x0a, 0x00);  // яркость
		sendCMD(0x0b, 0x0f);  // отображаем всё
		sendCMD(0x0C, 0x01);  // включить
	}
	void setBright(byte value) {  // 8x8: 0/8/15 - 30/310/540 ma
		sendCMD(0x0a, value);  // яркость 0-15
	}
	void setPower(bool value) {
		sendCMD(0x0c, value);
	}
	void clear() {
		fillByte(0);
	}
	void fill() {
		fillByte(255);
	}
	void fillByte(byte data) {
		for (int i = 0; i < width * height * 8; i++) buffer[i] = data;      
	}
	void dot(int x, int y, byte fill = 1) {
		if (x >= 0 && x < width * 8 && y >= 0 && y < height * 8) {
			if ((y >> 3) & 1) {               	// если это нечётная матрица: (y / 8) % 2
				x = width * 8 - 1 - x;          // отзеркалить x
				y = (y & 0xF8) + (7 - (y & 7)); // отзеркалить y: (y / 8) * 8 + (7 - (y % 8));
			}
			// позиция в буфере
			int curByte = width * (height - 1 - (y >> 3)) + (width - 1 - (x >> 3)) + (y & 7) * width * height;
			bitWrite(buffer[curByte], x & 7, fill);
		}
	}

	bool get(int x, int y) {
		if (x >= 0 && x < width * 8 && y >= 0 && y < height * 8) {
			if ((y >> 3) & 1) {               	// если это нечётная матрица: (y / 8) % 2
				x = width * 8 - 1 - x;          // отзеркалить x
				y = (y & 0xF8) + (7 - (y & 7)); // отзеркалить y: (y / 8) * 8 + (7 - (y % 8));
			}
			// позиция в буфере
			int curByte = width * (height - 1 - (y >> 3)) + (width - 1 - (x >> 3)) + (y & 7) * width * height;
			return bitRead(buffer[curByte], x & 7);
		}
		return 0;
	}

	void update() {
		int count = 0;
		for (int k = 0; k < 8; k++) {
			beginData();
			for (int i = 0; i < _amount; i++) sendData(8 - k, buffer[count++]);
			endData();
		}      
	}

	byte buffer[width * height * 8];

private:
	void beginData() {
		SPI.beginTransaction(MAX_SPI_SETT);
		fastWrite(CSpin, 0);		
	}
	void endData() {		
		fastWrite(CSpin, 1);
		SPI.endTransaction();
	}
	void sendCMD(uint8_t address, uint8_t value) {
		beginData();
		for (int i = 0; i < _amount; i++) sendData(address, value);
		endData();
	}
	void sendData(uint8_t address, uint8_t value) {
		if (DATpin == CLKpin) {
			SPI.transfer(address);
			SPI.transfer(value);
		} else {
			fastShiftOut(DATpin, CLKpin, MSBFIRST, address);
			fastShiftOut(DATpin, CLKpin, MSBFIRST, value);
		}		
	}

	const int _amount = width * height;
	int _row = 0, _count = 0;
};
#endif
