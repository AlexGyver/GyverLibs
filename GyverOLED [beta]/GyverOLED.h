/*
	БЕТА ВЕРСИЯ от 25.02.2020
	
	TODO:
	- Перевести на горизонтальную адресацию
	- Сделать возможность выводить текст в любую точку
	
	GyverOLED - лёгкая и быстрая библиотека для олед дисплея
	- Поддержка OLED дисплеев на SSD1306 с разрешением 128х64 и 128х32
	- Выбор буфера
		- Без буфера
		- Буфер на стороне МК
		- (В РАЗРАБОТКЕ) Буфер на стороне дисплея (только для SSH1106)		
	- Вывод текста
		- Поддержка русского языка
		- Два размера букв
		- Возможность писать чёрным-по-белому и белым-по-чёрному
		- Несколько красивых сторонних шрифтов
	- Использование библиотеки microWire позволяет максимально облегчить и ускорить код
	- Установка яркости
	- Включение/выключение дисплея из скетча
	- Изменение ориентации дисплея (зеркально по вертикали и горизонтали)
	- Графика
		- Точки (рисовать, стирать)
		- Линии (рисовать, стирать)
		- Прямоугольники (контур, заливка, очистка)
		- Прямоугольники со скруглёнными углами (контур, заливка, очистка)
		- Окружности (контур, заливка, очистка)
		- Вывод битмапа + программа для конвертирования изображений
	
	===== ДЕФАЙНЫ (применять до подключения библиотеки) =====
	OLED_NO_BUFFER			- не использовать буфер вообще (активно по умолчанию)
	[в разработке] OLED_HARD_BUFFER_64 	- буфер на стороне дисплея (только для SSH1106)
	OLED_SOFT_BUFFER_32		- буфер на стороне МК (для 128x32)
	OLED_SOFT_BUFFER_64		- буфер на стороне МК (для 128x64)
	USE_MICRO_WIRE			- использовать библиотеку microWire.h (сильно облегчает код) --> ПО УМОЛЧАНИЮ
	USE_DEFAULT_WIRE		- использовать библиотеку Wire.h (работа с дисплеем станет доступна на других платформах)
	OLED_EXTFONTS			- использовать дополнительные шрифты (см. пример customFont)
	
	Разработано by AlexGyver, https://alexgyver.ru/
	Отдельное спасибо Петру Соболеву за программу BITmaper и Хакеру Шило за дополнительные шрифты
	"Ядро" библиотеки сделано по мотивам статьи с сайта http://www.technoblogy.com/
*/

#pragma once

#if defined(USE_MICRO_WIRE)
#include <microWire.h>	// лёгкая библиотека Wire (для atmega328)
#elif defined(USE_DEFAULT_WIRE)
#include <Wire.h>		// обычная Wire
#else
#include <microWire.h>	// лёгкая библиотека Wire (для atmega328)
#endif

#include <Arduino.h>
#include "charMap.h"

#define OLED128x32 0
#define OLED128x64 1

#define OLED_HEIGHT_32          0x02  // oled 128x32
#define OLED_HEIGHT_64          0x12  // oled 128x64
#define OLED_64          		0x3F
#define OLED_32          		0x1F

#define OLED_COMMAND_MODE       0x00
#define OLED_ONE_COMMAND_MODE   0x80
#define OLED_DATA_MODE          0x40
#define OLED_ONE_DATA_MODE      0xC0

#define OLED_DISPLAY_OFF		0xAE
#define OLED_DISPLAY_ON			0xAF

static const uint8_t _oled_init[] PROGMEM = {
	0xAE,	// oled off
	0xD5,	// CLOCK_DIV_RATIO
	0x80,
	0x8D,	// Charge pump
	0x14,
	0x20,	// Memory mode
	0x01,	// Vertical _addressing
	0xA1,	// Flip horizontally
	0xC8,	// Flip vertically
	0x81,	// Set contrast
	0xCF,	// brighter
	0xDB,	// Set vcom detect
	0x40,	// brighter
	0xAF,	// oled on
};

#ifdef OLED_HARD_BUFFER_64
#define HARD_BUFFER
#endif

#ifdef OLED_SOFT_BUFFER_32
#define SOFT_BUFFER
#define OLED_BUF_SIZE 128*32/8
byte _oled_buffer[OLED_BUF_SIZE];
#endif

#ifdef OLED_SOFT_BUFFER_64
#define SOFT_BUFFER
#define OLED_BUF_SIZE 128*64/8
byte _oled_buffer[OLED_BUF_SIZE];
#endif

#ifndef SOFT_BUFFER
#define OLED_NO_BUFFER
#endif

#ifdef OLED_NO_BUFFER
#pragma message "Using OLED without buffer!"
#endif

#ifdef OLED_EXTFONTS
#define fontbyte(x) pgm_read_byte(&cfont.font[x])

// структура шрифта для OLED_EXTFONTS
struct _current_font
{
	uint8_t* font;
	uint8_t x_size;
	uint8_t y_size;
	uint8_t offset;
	uint8_t numchars;
	//	uint8_t inverted;
	uint8_t	startSpace[3];	// начальная позиция пустых интервалов в массиве символов
	uint8_t	sumSpace[3];	// количество отсутсвующих символов (ширина пустых интервалов)
};
#endif

enum _fillMode {
	NO_FILL,
	FILL,
	CLEAR,
};
static const byte INVERT = 0xFF;

class GyverOLED {
public:
	GyverOLED(byte address = 0x3C);			// создаём с указанием адреса
	
	// СЕРВИС
	void init(uint8_t oledType, int freq);	// инициализация. Можно передать частоту шины (до 800 кГц). Дефолт 400
	void clear();							// очистить
	void setContrast(byte value);			// яркость
	void setPower(boolean mode);			// вкл/выкл (энергосбережение)
	void flipH();							// отразить по горизонтали
	void flipV();							// отразить по вертикали
	void update();							// обновить дисплей из буфера (если буфер включен)
	
	// ПЕЧАТЬ
	void home();							// отправить курсор в 0,0
	void setCursor(int column, int line);	// поставить курсор для символа (20 по горизонтали, 4 или 8 по вертикали)
	void setCursorXY(int x, int y);			// поставить курсор для символа (0-127 по горизонтали, 0-32 или 64 по вертикали)
	void scale1X();							// масштаб шрифта х1
	void scale2X();							// масштаб шрифта х2
	void inverse(boolean inv);				// инвертировать текст (чёрным по белому)
	
	void printChar(unsigned char c);		// вывести букву	
	void println();							// перевод строки
	
	// методы для печати
	void print(char c);	
	void print(char data[]);
	void print(String data);
	void print(uint32_t data);
	void print(int32_t data);
	void print(uint16_t data);
	void print(int16_t data);
	void print(uint8_t data);
	void print(int8_t data);
	void print(double data);
	void print(double data, byte decimals);
	
	void println(char c);
	void println(char data[]);
	void println(String data);
	void println(uint32_t data);
	void println(int32_t data);
	void println(uint16_t data);
	void println(int16_t data);
	void println(uint8_t data);
	void println(int8_t data);	
	void println(double data);
	void println(double data, byte decimals);
	
	boolean isEnd();							// возвращает true, если дисплей "кончился" - при побуквенном выводе
	
	// ГРАФИКА
	void setPosition(int x, int y);				// установка курсора на "столбик" x: 0-128, y: 0-4 (128*32) / 0-8 (128*64)
	void sendData(byte data);					// шлёт байт в указанный в setPosition "столбик"
	void dot(int x, int y, bool fill = true);						// точка (заливка 1/0)
	void line(int x0, int y0, int x1, int y1, bool fill);			// линия
	void rect(int x0, int y0, int x1, int y1, _fillMode fill);		// прямоугольник (лев. верхн, прав. нижн)
	void roundRect(int x0, int y0, int x1, int y1, _fillMode fill);	// прямоугольник скруглённый (лев. верхн, прав. нижн)
	void circle(int x, int y, int radius, _fillMode fill);			// окружность
	void drawBitmap(int x, int y, const uint8_t *frame, int sizeX, int sizeY, byte invert);	// битмап
	
	// ДРУГОЕ
	void sendCommand(byte command);				// отправить команду по I2C
	
	// динамический буфер
	void createBuf(int x, int sizeX, int y, int sizeY);
	void sendBuf();
	
	// Расширение шрифтов
#ifdef OLED_EXTFONTS
	void setFont(uint8_t* font);				// установка шрифта OLED_EXTFONTS
	uint8_t cFontWidth();						// ширина текущего шрифта
	uint8_t cFontHeight();						// высота текущего шрифта
#endif

private:
	void setWindow(byte x0, byte y0, byte x1, byte y1);	
	void _printBytes(uint32_t data);
	void _printBytes(uint32_t data, byte decimals);
	byte _address = 0x3C;
	uint8_t getFont(uint8_t font, uint8_t row);
	int _stretch(int x);
	boolean _type, _invState = 0;
	byte _scale = 1;
	int _x = 0, _y = 0;
	byte _lastChar;
	byte _numRows = 8;
	
	byte _bufsizeX, _bufsizeY;
	byte _bufX, _bufY;
	byte *_buf_ptr;
	bool _buf_flag = false;
	bool _printType = false;

#ifdef OLED_EXTFONTS
protected:
	char* _codingCP866(char* StrIn);
	_current_font cfont;				// текущий шрифт OLED_EXTFONTS
#endif
};

// ====================== РЕАЛИЗАЦИЯ =======================
GyverOLED::GyverOLED(byte address){
	_address = address;
}

// ======================= INIT =======================
void GyverOLED::init(uint8_t oledType, int freq = 400) {
	Wire.begin();
	Wire.setClock(freq * 1000ul);
	Wire.beginTransmission(_address);
	Wire.write(OLED_COMMAND_MODE);
	for (uint8_t i = 0; i < 14; i++) Wire.write(pgm_read_byte(&_oled_init[i]));
	
	Wire.write(0xDA);
	if (oledType) Wire.write(OLED_HEIGHT_64);
	else Wire.write(OLED_HEIGHT_32);

	Wire.write(0xA8);
	if (oledType) Wire.write(OLED_64);
	else Wire.write(OLED_32);
	
	Wire.endTransmission();
	
	_type = oledType;
	_numRows = (_type) ? 8 : 4;
	GyverOLED::setCursor(0, 0);

#ifdef OLED_EXTFONTS
	cfont.font = 0;		// при инициализации шрифт OLED_EXTFONTS не установлен
#endif
}

// ======================= CLEAR =======================
void GyverOLED::clear() {
#ifdef SOFT_BUFFER
	memset(_oled_buffer, 0, OLED_BUF_SIZE);		
#else
	setWindow(0, 127, 0, _numRows-1);
	
#if defined(USE_MICRO_WIRE)
	Wire.beginTransmission(_address);
	Wire.write(OLED_DATA_MODE);
	int maxPacks = (_type) ? 1024 : 512;
	for (int i = 0; i < maxPacks; i++) {
		Wire.write(0);
	}
	Wire.endTransmission();
#else
	int maxPacks = (_type) ? 32 : 16;
	for (int i = 0; i < maxPacks; i++) {
		Wire.beginTransmission(_address);
		Wire.write(OLED_DATA_MODE);
		for (int j = 0; j < 32; j++) {
			Wire.write(0);
		}
		Wire.endTransmission();
	}
#endif
	
#endif
}

// ======================= ЯРКОСТЬ =======================
void GyverOLED::setContrast(byte value) {
	sendCommand(0x81);
	sendCommand(value);
}

void GyverOLED::setPower(boolean mode) {
	sendCommand(mode ? OLED_DISPLAY_ON : OLED_DISPLAY_OFF);
}

// ======================= КУРСОР =======================
void GyverOLED::setCursor(int column, int line) {
	setWindow(column * 6, column * 6 + _scale * 6 - 1, line, line + _scale - 1);
	_x = column;
	_y = line;
	_printType = false;
}

void GyverOLED::setCursorXY(int x, int y) {
	setWindow(x, 127, y, _numRows-1);
	_x = x;
	_y = y;
	_printType = true;
}

void GyverOLED::home() {
	if (_printType) setCursorXY(0, 0);
	else setCursor(0, 0);
}

// ======================= ПЕЧАТЬ =======================
uint8_t GyverOLED::getFont(uint8_t font, uint8_t row) {
	font = font - '0' + 16;   // перевод код символа из таблицы ASCII в номер согласно нумерации массива
	if (font <= 90) return pgm_read_byte(&(charMap[font][row]));	// для английских букв и символов
	else if (font >= 112 && font <= 159) {							// и пизд*ц для русских
		return pgm_read_byte(&(charMap[font - 17][row]));
	} else if (font >= 96 && font <= 111) {
		return pgm_read_byte(&(charMap[font + 47][row]));
	}
}

#ifdef OLED_EXTFONTS
// высота шрифта OLED_EXTFONTS
uint8_t GyverOLED::cFontHeight() {
	return cfont.y_size;
}

// ширина шрифта OLED_EXTFONTS
uint8_t GyverOLED::cFontWidth() {
	return cfont.x_size;
}

// печать одного символа OLED_EXTFONTS
void GyverOLED::printChar(unsigned char c) {
#if !defined(SOFT_BUFFER) || defined(HARD_BUFFER)
	Wire.beginTransmission(_address);
	Wire.write(OLED_DATA_MODE);
#endif
	if(cfont.startSpace[0]<c){c-=cfont.sumSpace[0];}										//	Если в массиве символов, до кода текущего символа, имеется пустой интервал, то уменьшаем код текущего символа на количество символов в пустом интервале.
	if(cfont.startSpace[1]<c){c-=cfont.sumSpace[1];}										//	Если в массиве символов, до кода текущего символа, имеется пустой интервал, то уменьшаем код текущего символа на количество символов в пустом интервале.
	if(cfont.startSpace[2]<c){c-=cfont.sumSpace[2];}										//	Если в массиве символов, до кода текущего символа, имеется пустой интервал, то уменьшаем код текущего символа на количество символов в пустом интервале.

	int font_idx = ((c - cfont.offset)*((cfont.x_size*cfont.y_size/8)))+4;
	if ((cfont.x_size*cfont.y_size % 8) != 0 ) font_idx++;
	int cbyte=fontbyte(font_idx);
	int cbit=0;

	for (int cx=cfont.x_size; cx>0; cx--) {
		for (int cy=0; cy<cfont.y_size; cy++) {
			if ((cbyte & (1 << cbit)) != 0) {
				GyverOLED::dot(_x+cx, _y+cy, true);
			} else {
				GyverOLED::dot(_x+cx, _y+cy, false);
			}
			cbit++;
			if (cbit>7) {
				cbit=0;
				font_idx++;
				cbyte=fontbyte(font_idx);
			}
		}
	}
}

#else	// #ifdef OLED_EXTFONTS

void GyverOLED::printChar(unsigned char c) {	
#if !defined(SOFT_BUFFER) || defined(HARD_BUFFER)
	Wire.beginTransmission(_address);
	Wire.write(OLED_DATA_MODE);
#endif
	for (uint8_t col = 0; col < 6; col++) {
		int bits;
		bits = (col != 5) ? getFont((char)c, col) : 0;
		if (_invState) bits = ~bits;
		
#ifdef SOFT_BUFFER		
		/*int index;
		if (_printType) index = _y + (_x*6 + col * _scale) * _numRows;
		else index = _y + (_x*6 + col * _scale) * _numRows;*/
		int index = _y + (_x*6 + col * _scale) * _numRows;
		
		if (_scale == 1) {		
			_oled_buffer[index] |= bits;
			bits = _oled_buffer[index];
		} else {
			int newBits = _stretch(bits);
			_oled_buffer[index] |= newBits;
			_oled_buffer[index+1] |= newBits >> 8;
			_oled_buffer[index+_numRows] |= newBits;
			_oled_buffer[index+_numRows+1] |= newBits >> 8;
		}
#endif

#if !defined(SOFT_BUFFER) || defined(HARD_BUFFER)
		// В РАЗРАБОТКЕ
		// тут нужно для ХАРД буфера прочитать байт и дописать к нему bits
		
		if (_scale == 1) Wire.write(bits);
		else {
			bits = _stretch(bits);
			Wire.write(bits);
			Wire.write(bits >> 8);
			Wire.write(bits);
			Wire.write(bits >> 8);
		}
#endif
	}
#if !defined(SOFT_BUFFER) || defined(HARD_BUFFER)
	Wire.endTransmission();
#endif
}
#endif

void GyverOLED::_printBytes(uint32_t data) {
	int8_t bytes[10];
	byte amount;
	for (byte i = 0; i < 10; i++) {
		bytes[i] = data % 10;
		data /= 10;
		if (data == 0) {
			amount = i;
			break;
		}
	}	
	for (int8_t i = amount; i >= 0; i--) {
		GyverOLED::print((char)(bytes[i] + '0'));
	}
}

void GyverOLED::print(uint32_t data) {GyverOLED::_printBytes((uint32_t) data);}
void GyverOLED::println(uint32_t data) {GyverOLED::_printBytes((uint32_t) data); GyverOLED::println();}
void GyverOLED::print(uint16_t data) {GyverOLED::_printBytes((uint32_t) data);}
void GyverOLED::println(uint16_t data) {GyverOLED::_printBytes((uint32_t) data); GyverOLED::println();}
void GyverOLED::print(uint8_t data) {GyverOLED::_printBytes((uint32_t) data);}
void GyverOLED::println(uint8_t data) {GyverOLED::_printBytes((uint32_t) data); GyverOLED::println();}

void GyverOLED::print(int32_t data) {GyverOLED::_printBytes((uint32_t) data);}
void GyverOLED::println(int32_t data) {GyverOLED::_printBytes((uint32_t) data); GyverOLED::println();}
void GyverOLED::print(int16_t data) {GyverOLED::_printBytes((uint32_t) data);}
void GyverOLED::println(int16_t data) {GyverOLED::_printBytes((uint32_t) data); GyverOLED::println();}
void GyverOLED::print(int8_t data) {GyverOLED::_printBytes((uint32_t) data);}
void GyverOLED::println(int8_t data) {GyverOLED::_printBytes((uint32_t) data); GyverOLED::println();}

void GyverOLED::print(double data, byte decimals) {
	if (data < 0) {
		print('-');
		data = -data;
	}
	uint32_t integer = data;
	_printBytes((uint32_t) integer);
	print('.');
	data -= integer;
	for (byte i = 0; i < decimals; i++) {	
		data *= 10.0;
		_printBytes((uint32_t) data);
		data -= (byte)data;
	}
}

void GyverOLED::print(double data) {
	GyverOLED::print(data, 2);
}

void GyverOLED::println(double data, byte decimals) {
	print(data, decimals);
	println();
}

void GyverOLED::println(double data) {
	print(data, 2);
	println();
}

void GyverOLED::println() {
#ifdef OLED_EXTFONTS
	_y += cfont.y_size;
#else
	_y += _scale;
#endif
	_x = 0;
	if (_y > _numRows-1) _y = _numRows-1;
	setCursor(_x, _y);
}

#ifdef OLED_EXTFONTS
// установка шрифта OLED_EXTFONTS
void GyverOLED::setFont(uint8_t* font) {
	cfont.font=font;
	cfont.x_size=fontbyte(0);
	cfont.y_size=fontbyte(1);
	cfont.offset=fontbyte(2);
	cfont.numchars=fontbyte(3);

	uint16_t i			=	(uint16_t) cfont.numchars * cfont.x_size * cfont.y_size / 8 + 0x04;			//	Определяем позицию бита указывающего количество пустых интервалов в массиве шрифта.
	uint16_t j			=	pgm_read_byte(&font[i]);													//	Определяем количество пустых интервалов в массиве шрифта.
	cfont.startSpace[0]=0xFF;	cfont.sumSpace[0]=0;							//	Указываем что первый пустой интервал в массиве шрифта находится после символа с кодом (0xFF) и состоит из 0 символов
	cfont.startSpace[1]=0xFF;	cfont.sumSpace[1]=0;							//	Указываем что второй пустой интервал в массиве шрифта находится после символа с кодом (0xFF) и состоит из 0 символов
	cfont.startSpace[2]=0xFF;	cfont.sumSpace[2]=0;							//	Указываем что третий пустой интервал в массиве шрифта находится после символа с кодом (0xFF) и состоит из 0 символов
	if(j>0)				{	cfont.startSpace[0]=pgm_read_byte(&font[i+1]);								//	Если количество пустых интервалов больше 0, то	сохраняем начало первого пустого интервала символов
		cfont.sumSpace  [0]=pgm_read_byte(&font[i+2]);}								//															и размер первого пустого интервала символов
	if(j>1)				{	cfont.startSpace[1]=pgm_read_byte(&font[i+3]);								//	Если количество пустых интервалов больше 1, то	сохраняем начало второго пустого интервала символов
		cfont.sumSpace  [1]=pgm_read_byte(&font[i+4]);}								//															и размер второго пустого интервала символов
	if(j>2)				{	cfont.startSpace[2]=pgm_read_byte(&font[i+5]);								//	Если количество пустых интервалов больше 2, то	сохраняем начало третьего пустого интервала символов
		cfont.sumSpace  [2]=pgm_read_byte(&font[i+6]);}								//															и размер третьего пустого интервала символов
}
// печать символа OLED_EXTFONTS
void GyverOLED::print(char c) {	
	if ((byte)c == 10) GyverOLED::println();
	else {
		if ((byte)c <= 127) {printChar(c); _x += cfont.x_size;}				// если лат. буквы и символы
		else {
			if(_lastChar==0xD0 && byte(c)>=0x90 && byte(c)<=0xBF) {			//	Если код текущего символа равен 208, а за ним следует символ с кодом 144...191, значит это буква «А»...«п» требующая преобразования к коду 128...175
				printChar(c-0x10); _x += cfont.x_size;
			} else if(_lastChar==0xD0 && byte(c)==0x81) {					//	Если код текущего символа равен 208, а за ним следует символ с кодом 129      , значит это буква «Ё»       требующая преобразования к коду 240
				printChar(0xF0); _x += cfont.x_size;
			} else if(_lastChar==0xD1 && byte(c)>=0x80 && byte(c)<=0x8F) {	//	Если код текущего символа равен 209, а за ним следует символ с кодом 128...143, значит это буква «р»...«я» требующая преобразования к коду 224...239
				printChar(c+0x60); _x += cfont.x_size;
			} else
			if(_lastChar==0xD1 && byte(c)==0x91) {							//	Если код текущего символа равен 209, а за ним следует символ с кодом 145      , значит это буква «ё»       требующая преобразования к коду 241
				printChar(0xF1); _x += cfont.x_size;
			}
		}
		_lastChar = c;
		
		if (_x >= 128 ) {
			_x = 0;
			_y += cfont.y_size;			
		}
		setCursor(_x, _y);
	}	
}
#else
void GyverOLED::print(char c) {	
	if ((byte)c == 10) GyverOLED::println();
	else {
		if ((byte)c <= 127) {printChar(c); _x += _scale;}		// если лат. буквы и символы
		else {
			if (_lastChar == 209 && (byte)c == 145) printChar((char)181);			// е вместо ё
			else if (_lastChar == 208 && (byte)c == 129) printChar((char)149);		// Е вместо Ё
			else if (_lastChar == 226 && (byte)c == 128) ;							// тире вместо длинного тире
			else if (_lastChar == 128 && (byte)c == 147) {printChar((char)45);_x += _scale;}		// тире вместо длинного тире			
			else if ((byte)c <= 191) {printChar(c); _x += _scale;}
		}
		_lastChar = c;		
		
		if (_x >= 20) {
			_x = 0;
			_y += _scale;			
		}
		setCursor(_x, _y);
	}	
}
#endif

void GyverOLED::println(char c) {
	printChar(c);
#ifdef OLED_EXTFONTS
	_y += cfont.y_size;
#else
	_y += _scale;
#endif
	if (_y > _numRows-1) _y = _numRows-1;
	setCursor(_x, _y);
}

void GyverOLED::print(char data[]) {
	byte i = 0;
	byte lastPref;
	while (data[i] != '\0') {
		print(data[i]);
		i++;
	}
}

void GyverOLED::println(char data[]) {
	GyverOLED::print(data);
	GyverOLED::println();
}

void GyverOLED::print(String data) {
	byte stringSize = data.length();
	for (byte i = 0; i < stringSize; i++) {
		print((char)data[i]);
	}
}

void GyverOLED::println(String data) {
	GyverOLED::print(data);
	GyverOLED::println();
}

// ======================= СЕРВИС =======================
void GyverOLED::scale1X() {
	_scale = 1;
	setCursor(_x, _y);
}
void GyverOLED::scale2X() {
	_scale = 2;
	setCursor(_x, _y);
}

void GyverOLED::inverse(boolean inv) {
	_invState = inv;
}

void GyverOLED::flipH() {
	GyverOLED::sendCommand(0xA0);
}

void GyverOLED::flipV() {
	GyverOLED::sendCommand(0xC0);
}

boolean GyverOLED::isEnd() {
	if (_y > _numRows-1) return true;
	else return false;
}
// ======================= ГРАФИКА =======================

// динамический буфер
void GyverOLED::createBuf(int x, int sizeX, int y, int sizeY) {
#ifdef OLED_NO_BUFFER
	_bufX = x;
	_bufsizeX = sizeX;
	_bufY = y;
	_bufsizeY = sizeY;
	
	_buf_flag = true;
	int bufSize = _bufsizeX * _bufsizeY;
	_buf_ptr = malloc(bufSize);
	memset(_buf_ptr, 0, bufSize);
#endif
}

void GyverOLED::sendBuf() {
#ifdef OLED_NO_BUFFER
	setWindow(_bufX, _bufX + _bufsizeX, _bufY/8, _bufY/8 + _bufsizeY - 1);
	Wire.beginTransmission(_address);
	Wire.write(OLED_DATA_MODE);
	for (int i = 0; i < _bufsizeX * _bufsizeY; i++) {
		Wire.write(_buf_ptr[i]);
	}
	Wire.endTransmission();
	_buf_flag = false;
	free(_buf_ptr);
#endif
}


// точка
void GyverOLED::dot(int x, int y, bool fill) {
	if (x > 127 || y > ((_type) ? 63 : 31) ||
			x < 0 || y < 0) return;
#if defined(SOFT_BUFFER)
	// записали в софт буфер
	int index = y / 8 + x * _numRows;
	bitWrite(_oled_buffer[index], y % 8, fill);

	/*
	// отправить на дисплей сразу
	Wire.beginTransmission(_address);
	Wire.write(OLED_COMMAND_MODE);
	Wire.write(0x21); Wire.write(x); Wire.write(127);
	Wire.write(0x22); Wire.write(y / _numRows); Wire.write(_numRows-1);
	Wire.endTransmission();

	Wire.beginTransmission(_address);
	Wire.write(OLED_DATA_MODE);
	Wire.write(_oled_buffer[index]);
	Wire.endTransmission();
*/
	
#elif defined(HARD_BUFFER)
	// хард буфер - прочитали, дописали, отправили
	

#else	
	// без буфера - отправили и всё
	if (!_buf_flag) {
		setWindow(x, 127, y / 8, _numRows-1);

		Wire.beginTransmission(_address);
		Wire.write(OLED_DATA_MODE);
		Wire.write(1 << (y % 8));	// задвигаем 1 на высоту y (для этого столбец-байта)
		Wire.endTransmission();
	} else {
		// локальный мини буфер
		x -= _bufX;
		y -= _bufY;
		if (x < 0 || x > _bufsizeX || y < 0 || y > _bufsizeY*8) return;
		int index = y / 8 + x * _bufsizeY;
		bitWrite(_buf_ptr[index], y % 8, fill);
	}
	
#endif
}

// линия
void GyverOLED::line(int x0, int y0, int x1, int y1, bool fill = 1) {
	if (x0 == x1) {
		//#if defined(SOFT_BUFFER)
		for (int i = min(y0, y1); i < max(y0, y1); i++) dot(x0, i, fill);
		/*#else
	setPosition(x0, y0 / 8 - 1);
	byte thisByte = 0;
	if (y0 / 8 != y1 / 8) {		// в разных блоках
		for (byte i = 0; i < y0 % 8; i++) bitSet(thisByte, i);
		sendData(255);
		thisByte = 0;
		for (byte i = 0; i < y0 % 8; i++) bitSet(thisByte, i);
		//sendData(~thisByte);
	}
	
#endif*/
	} else if (y0 == y1) {
		for (int i = min(x0, x1); i < max(x0, x1); i++) dot(i, y0, fill);
	} else {
		int sx, sy, e2, err;
		int dx = abs(x1 - x0);
		int dy = abs(y1 - y0);
		if (x0 < x1) sx = 1; else sx = -1;
		if (y0 < y1) sy = 1; else sy = -1;
		err = dx - dy;
		for (;;) {
			dot(x0, y0, fill);
			if (x0==x1 && y0==y1) return;
			e2 = err<<1;
			if (e2 > -dy) { err = err - dy; x0 = x0 + sx; }
			if (e2 < dx) { err = err + dx; y0 = y0 + sy; }
		}
	}	
}


// прямоугольник
void GyverOLED::rect(int x0, int y0, int x1, int y1, _fillMode fill = 0) {
	int tmp;
	if (x0>x1) {
		tmp=x1;
		x0=x1;
		x1=tmp;
	}
	if (y0>y1) {
		tmp=y0;
		y0=y1;
		y1=tmp;
	}
#ifdef OLED_NO_BUFFER
	createBuf(x0, x1 - x0, y0, y1 / 8 - y0 / 8 + 1);
#endif
	
	switch (fill) {
	case NO_FILL:
		line(x0, y0, x1, y0);
		line(x1, y0, x1, y1);
		line(x1, y1, x0, y1);
		line(x0, y1, x0, y0);
		break;
	case FILL: for (byte i = y0; i < y1; i++) line(x0, i, x1, i);
		break;
	case CLEAR: for (byte i = y0; i < y1; i++) line(x0, i, x1, i, 0);
		break;
	}

#ifdef OLED_NO_BUFFER
	sendBuf();
#endif	
}

// скруглённый прямоугольник
void GyverOLED::roundRect(int x0, int y0, int x1, int y1, _fillMode fill = 0) {
	int tmp;
	if (x0>x1) {
		tmp=x1;
		x0=x1;
		x1=tmp;
	}
	if (y0>y1) {
		tmp=y0;
		y0=y1;
		y1=tmp;
	}

#ifdef OLED_NO_BUFFER
	createBuf(x0, x1 - x0, y0, y1 / 8 - y0 / 8 + 1);
#endif

	bool fillLine = (fill == CLEAR) ? false : true;
	line(x0+2, y0, x1-1, y0, fillLine);
	line(x1, y0+2, x1, y1-1, fillLine);
	line(x1-1, y1, x0+2, y1, fillLine);
	line(x0, y1-1, x0, y0+2, fillLine);
	
	switch (fill) {
	case NO_FILL:
		dot(x0+1, y0+1);
		dot(x1-1, y0+1);
		dot(x1-1, y1-1);
		dot(x0+1, y1-1);
		break;
	case FILL: for (byte i = y0+1; i < y1; i++) line(x0+1, i, x1, i);
		break;
	case CLEAR: for (byte i = y0+1; i < y1; i++) line(x0+1, i, x1, i, 0);
		break;
	}
#ifdef OLED_NO_BUFFER
	sendBuf();
#endif	
}

// круг
void GyverOLED::circle(int x, int y, int radius, _fillMode fill = 0) {
#ifdef OLED_NO_BUFFER
	createBuf(x - radius, radius + radius + 1, y - radius, _bufsizeX / 8 + 1);
#endif
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;
	
	bool fillLine = (fill == CLEAR) ? false : true;
	dot(x, y + radius, fillLine);
	dot(x, y - radius, fillLine);
	dot(x + radius, y, fillLine);
	dot(x - radius, y, fillLine);
	
	if (fill == FILL) line(x - radius, y, x + radius, y);
	else if (fill == CLEAR) line(x - radius, y, x + radius, y, 0);

	while(x1 < y1) {
		if(f >= 0) {
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;
		if (fill == NO_FILL) {			
			dot(x + x1, y + y1);
			dot(x - x1, y + y1);
			dot(x + x1, y - y1);
			dot(x - x1, y - y1);			
			dot(x + y1, y + x1);
			dot(x - y1, y + x1);
			dot(x + y1, y - x1);
			dot(x - y1, y - x1);
		} else {			
			line(x + x1 + 1, y + y1, x - x1, y + y1, fillLine);
			line(x + x1 + 1, y - y1, x - x1, y - y1, fillLine);
			line(x + y1 + 1, y + x1, x - y1, y + x1, fillLine);
			line(x + y1 + 1, y - x1, x - y1, y - x1, fillLine);
		}	
	}
#ifdef OLED_NO_BUFFER
	sendBuf();
#endif
}

// битмап
void GyverOLED::drawBitmap(int x, int y, const uint8_t *frame, int sizeX, int sizeY, byte invert = 0) {
#ifdef OLED_NO_BUFFER
	sendCommand(0x20);
	sendCommand(0x00);
	setWindow(x, x+sizeX-1, y/8, y/8+sizeY/8-1);
	Wire.beginTransmission(_address);
	Wire.write(OLED_DATA_MODE);
	for (int i = 0; i < (sizeX * sizeY/8); i++) Wire.write(pgm_read_word(&(frame[i])) ^ invert);
	Wire.endTransmission();
	sendCommand(0x20);
	sendCommand(0x01);
#else
	int index = y / 8 + x * _numRows;
	byte col = 0;
	byte row = 0;
	for (int i = 0; i < (sizeX * sizeY/8); i++) {
		_oled_buffer[index + _numRows * col + row] = pgm_read_word(&(frame[i])) ^ invert;
		if (++col >= sizeX) {
			col = 0;
			row++;
		}
	}
#endif
}

// обновить (для буфера на стороне МК)
void GyverOLED::update() {
#if defined(SOFT_BUFFER)
	setWindow(0, 127, 0, (_type) ? 7 : 3);
	
#if defined(USE_MICRO_WIRE)
	Wire.beginTransmission(_address);
	Wire.write(OLED_DATA_MODE);
	int maxPacks = (_type) ? 1024 : 512;	
	for (int i = 0; i < maxPacks; i++)
	Wire.write(_oled_buffer[i]);
	Wire.endTransmission();
#else
	int maxPacks = (_type) ? 32 : 16;
	int counter = 0;
	for (int i = 0; i < maxPacks; i++) {
		Wire.beginTransmission(_address);
		Wire.write(OLED_DATA_MODE);
		for (int j = 0; j < 32; j++) {
			Wire.write(_oled_buffer[counter]);
			counter++;
		}
		Wire.endTransmission();
	}
#endif
	
#endif
}

// ======================= ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ =======================
// выбрать "окно"
void GyverOLED::setWindow(byte x0, byte x1, byte y0, byte y1) {
	Wire.beginTransmission(_address);
	Wire.write(OLED_COMMAND_MODE);
	Wire.write(0x21);
	Wire.write(x0);
	Wire.write(x1);	
	Wire.write(0x22);
	Wire.write(y0);
	Wire.write(y1);	
	Wire.endTransmission();
}

// курсор
void GyverOLED::setPosition(int x, int y) {
	setWindow(x, 127, y, _numRows-1);
}

// отправить команду
void GyverOLED::sendCommand(byte command) {
	Wire.beginTransmission(_address);
	Wire.write(OLED_ONE_COMMAND_MODE);
	Wire.write(command);
	Wire.endTransmission();
}

// отправить байт данных
void GyverOLED::sendData(byte data) {
	Wire.beginTransmission(_address);
	Wire.write(OLED_ONE_DATA_MODE);
	Wire.write(data);
	Wire.endTransmission();
}

// растягиватель шрифта х2
int GyverOLED::_stretch (int x) {
	x = (x & 0xF0) << 4 | (x & 0x0F);
	x = (x << 2 | x) & 0x3333;
	x = (x << 1 | x) & 0x5555;
	return x | x << 1;
}