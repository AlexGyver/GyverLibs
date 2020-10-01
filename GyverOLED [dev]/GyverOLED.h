#pragma once
/*
GyverOLED - лёгкая и быстрая библиотека для OLED дисплея
- Поддержка OLED дисплеев на SSD1306/SSH1106 с разрешением 128х64 и 128х32 с подключением по I2C
- Выбор буфера
	- Вообще без буфера (и без особой потери возможностей)
	- Буфер на стороне МК (тратит кучу оперативки, но удобнее в работе)
	- Обновление буфера в выбранном месте (для быстрой отрисовки)
	- Динамический буфер выбранного размера (вся геометрия, текст, байты)
	- Буфер на стороне дисплея (только для SSH1106!!!)
- Вывод текста
	- Самый быстрый вывод текста среди OLED библиотек
	- Поддержка русского языка и буквы ё (!)
	- Более приятный шрифт (по сравнению с beta)
	- Координаты вне дисплея для возможности прокрутки
	- Вывод текста в любую точку (попиксельная адресация)
	- Полноэкранный вывод с удалением лишних пробелов
	- 4 размера букв (на базе одного шрифта, экономит кучу памяти!)
	- Возможность писать чёрным-по-белому и белым-по-чёрному
- Управление дисплеем
	- Установка яркости
	- Быстрая инверсия всего дисплея
	- Включение/выключение дисплея из скетча
	- Изменение ориентации дисплея (зеркально по вертикали и горизонтали)
- Графика (контур, заливка, очистка)
	- Точки
	- Линии
	- Прямоугольники
	- Прямоугольники со скруглёнными углами
	- Окружности
- Изображения (битмап)
	- Вывод битмапа в любую точку дисплея
	- Вывод "за дисплей"
	- Программа для конвертации изображений есть в библиотеке
- Поддержка библиотеки microWire для ATmega328 (очень лёгкий и быстрый вывод)

Разработано by AlexGyver, https://alexgyver.ru/
Отдельное спасибо Петру Соболеву за программу BITmaper (не используется) и Хакеру Шило за дополнительные шрифты (не используется)
Зачем либа сделана в одном файле и расписана в классе? Потому что template и лень
"Ядро" библиотеки сделано по мотивам статьи с сайта http://www.technoblogy.com/
*/

/*
GyverOLED<type, buff> экземпляр
type:	SSD1306_128x32 / SSD1306_128x64 / SSH1106_128x64
buff:	OLED_BUFFER / OLED_NO_BUFFER
*/

// ===== константы =====
#define SSD1306_128x32	0
#define SSD1306_128x64 	1
#define SSH1106_128x64 	2

#define OLED_NO_BUFFER	0
#define OLED_BUFFER	1

#define OLED_CLEAR 0
#define OLED_FILL 1
#define OLED_STROKE 2

#define BUF_ADD 0
#define BUF_SUBTRACT 1
#define BUF_REPLACE 2

#define BITMAP_NORMAL 0
#define BITMAP_INVERT 1

// ===========================================================================

#if defined(USE_MICRO_WIRE)
#include <microWire.h>  // лёгкая библиотека Wire (для atmega328)
#else
#include <Wire.h>   	// обычная Wire
#endif

#include <Arduino.h>
#include <Print.h>
#include "charMap.h"


// ============================ БЭКЭНД КОНСТАНТЫ ==============================
// внутренние константы
#define OLED_HEIGHT_32      	0x02
#define OLED_HEIGHT_64      	0x12
#define OLED_64             	0x3F
#define OLED_32             	0x1F

#define OLED_DISPLAY_OFF    	0xAE
#define OLED_DISPLAY_ON     	0xAF

#define OLED_COMMAND_MODE       0x00
#define OLED_ONE_COMMAND_MODE   0x80
#define OLED_DATA_MODE          0x40
#define OLED_ONE_DATA_MODE      0xC0

#define OLED_ADDRESSING_MODE	0x20
#define OLED_HORIZONTAL			0x00
#define OLED_VERTICAL   		0x01

#define OLED_NORMAL_V			0xC8
#define OLED_FLIP_V				0xC0
#define OLED_NORMAL_H			0xA1
#define OLED_FLIP_H				0xA0

#define OLED_CONTRAST   		0x81

#define OLED_NORMALDISPLAY		0xA6
#define OLED_INVERTDISPLAY		0xA7

#define BUFSIZE_128x64 (128*64/8)
#define BUFSIZE_128x32 (128*32/8)

// индекс в буфере
#define _bufIndex(x, y) (((y) >> 3) + ((x) << (_TYPE ? 3 : 2)))		// _y / 8 + _x * (4 или 8)
#define _swap(x, y) x = x ^ y ^ (y = x)
#define _inRange(x, mi, ma) ((x) >= (mi) && (x) <= (ma))

// список инициализации
static const uint8_t _oled_init[] PROGMEM = {
	OLED_DISPLAY_OFF,
	0xD5, 	// CLOCK_DIV_RATIO
	0x80,	// value
	0x8D, 	// Charge pump
	0x14,	// value
	OLED_ADDRESSING_MODE,
	OLED_VERTICAL,
	OLED_NORMAL_H,
	OLED_NORMAL_V,
	OLED_CONTRAST,
	0x7F,	// value
	0xDB, 	// Set vcom detect
	0x40, 	// value
	OLED_NORMALDISPLAY,
	OLED_DISPLAY_ON,
};

// ========================== КЛАСС КЛАСС КЛАСС ============================= 
template <int _TYPE, int _BUFF = OLED_BUFFER>
class GyverOLED : public Print {
public:
	// ========================== КОНСТРУКТОР ============================= 
	GyverOLED(uint8_t address = 0x3C) : _address(address) {};

	// ============================= СЕРВИС =============================== 
	// инициализация
	void init() {  							
		Wire.begin();
		Wire.beginTransmission(_address);
		Wire.write(OLED_COMMAND_MODE);
		for (uint8_t i = 0; i < 15; i++) Wire.write(pgm_read_byte(&_oled_init[i]));
		Wire.write(0xDA);
		Wire.write(_TYPE ? OLED_HEIGHT_64 : OLED_HEIGHT_32);
		Wire.write(0xA8);
		Wire.write(_TYPE ? OLED_64 : OLED_32);
		Wire.endTransmission();
		setCursorXY(0, 0);
		if (_BUFF) setWindow(0, 0, _maxX, _maxRow);		// для буфера включаем всю область
	}
	
	// очистить дисплей
	void clear() { fill(0); }
	
	// очистить область
	void clear(int x0, int y0, int x1, int y1) {
		if (_TYPE < 2) {	// для SSD1306			
			x1++;
			y1++;
			y0 >>= 3;
			y1 = (y1 - 1) >> 3;
			y0 = constrain(y0, 0, _maxRow);
			y1 = constrain(y1, 0, _maxRow);
			x0 = constrain(x0, 0, _maxX);
			x1 = constrain(x1, 0, _maxX);				
			if (!_BUFF) setWindow(x0, y0, x1, y1);
			if (!_BUFF) beginData();
			for (int x = x0; x < x1; x++)
			for (int y = y0; y < y1+1; y++)
			writeData(0, y, x, 2);
			if (!_BUFF) endData();			
		} else {
			// для SSH1108
		}
	}
	
	// яркость
	void setContrast(uint8_t value) { sendCommand(OLED_CONTRAST, value); }
	
	// вкл/выкл
	void setPower(bool mode) { sendCommand(mode ? OLED_DISPLAY_ON : OLED_DISPLAY_OFF); }
	
	// отразить по горизонтали
	void flipH(bool mode) { sendCommand(mode ? OLED_FLIP_H : OLED_NORMAL_H); }
	
	// инвертировать дисплей
	void invertDisplay(bool mode) { sendCommand(mode ? OLED_INVERTDISPLAY : OLED_NORMALDISPLAY); }
	
	// отразить по вертикали
	void flipV(bool mode) { sendCommand(mode ? OLED_FLIP_V : OLED_NORMAL_V); }

	// ============================= ПЕЧАТЬ ================================== 
	virtual size_t write(uint8_t data) {
		// переносы и пределы
		bool newPos = false;		
		if (data == '\r') { _x = 0; newPos = true; data = 0; }					// получен возврат каретки
		if (data == '\n') { _y += _scaleY; newPos = true; data = 0; }			// получен перевод строки
		if (_println && (_x + 6*_scaleX) >= _maxX) { _x = 0; _y += _scaleY; newPos = true; }	// строка переполненена, перевод и возврат
		if (newPos) setCursorXY(_x, _y);										// переставляем курсор
		if (_y + _scaleY > _maxY) data = 0;										// дисплей переполнен
		if (_println && data == ' ' && _x == 0) { data = 0; }					// убираем первый пробел в строке
		
		// фикс русских букв и некоторых символов
		if (data > 127) {
			uint8_t thisData = data;
			// data = 0 - флаг на пропуск
			if (data > 191) data = 0;
			else if (_lastChar == 209 && data == 145) data = 192;   // ё кастомная
			else if (_lastChar == 208 && data == 129) data = 149;	// Е вместо Ё
			else if (_lastChar == 226 && data == 128) data = 0;		// тире вместо длинного тире (начало)
			else if (_lastChar == 128 && data == 148) data = 45;	// тире вместо длинного тире	
			_lastChar = thisData;
		}
		if (data == 0) return 1;	
		// если тут не вылетели - печатаем символ		
		
		if (_TYPE < 2) {					// для SSD1306
			int newX = _x + _scaleX * 6;
			if (newX < 0 || _x > _maxX) {	// пропускаем вывод "за экраном"
				_x = newX;
			} else {				
				if (!_BUFF) beginData();
				for (uint8_t col = 0; col < 6; col++) {					// 6 стобиков буквы
					uint8_t bits = getFont(data, col);					// получаем байт
					byte inv = 0;
					if (_invState) {
						bits = ~bits;									// инверсия
						inv = 2;
					}
					
					if (_scaleX == 1) {									// если масштаб 1						
						if (_x >= 0 && _x <= _maxX) {					// внутри дисплея
							if (_shift == 0) {							// если вывод без сдвига на строку
								writeData(bits, 0, 0, inv);				// выводим
							} else {									// со сдвигом
								writeData(bits << _shift, 0, 0, inv);	// верхняя часть
								writeData(bits >> (8 - _shift), 1, 0, inv);	// нижняя часть
							}	
						}									
					} else {											// масштаб 2, 3 или 4 - растягиваем шрифт						
						long newData = 0;								// буфер
						for (uint8_t i = 0, count = 0; i < 8; i++)
						for (uint8_t j = 0; j < _scaleX; j++, count++)							
						bitWrite(newData, count, bitRead(bits, i));		// пакуем растянутый шрифт
						
						for (uint8_t i = 0; i < _scaleX; i++) {			// выводим. По Х
							byte prevData = 0;
							if (_x + i >= 0 && _x + i <= _maxX) 		// внутри дисплея
							for (uint8_t j = 0; j < _scaleX; j++) {		// выводим. По Y
								byte data = newData >> (j * 8);			// получаем кусок буфера
								if (_shift == 0) {						// если вывод без сдвига на строку
									writeData(data, j, i, inv);	// выводим							
								} else {								// со сдвигом
									writeData((prevData >> (8 - _shift)) | (data << _shift), j, i, inv);	// склеиваем и выводим
									prevData = data;					// запоминаем предыдущий
								}
							}
							if (_shift != 0) writeData(prevData >> (8 - _shift), _scaleX, i, inv);			// выводим нижний кусочек, если со сдвигом
						}
					}
					_x += _scaleX;										// двигаемся на ширину пикселя (1-4)
				}
				if (!_BUFF) endData();
			}
		} else {						
			// для SSH1106
		}		
		return 1;
	}
	
	// автоматически переносить текст
	void autoPrintln(bool mode) { _println = mode; }
	
	// отправить курсор в 0,0
	void home() { setCursorXY(0, 0); }
	
	// поставить курсор для символа 0-127, 0-8(4)
	void setCursor(int x, int y) { setCursorXY(x, y << 3); }
	
	// поставить курсор для символа 0-127, 0-63(31)
	void setCursorXY(int x, int y) { 			
		_x = x;
		_y = y;	
		setWindowShift(x, y, _maxX, _scaleY);
	}
	
	// масштаб шрифта (1-4)
	void setScale(uint8_t scale) {
		scale = constrain(scale, 1, 4);	// защита от нечитающих доку
		_scaleX = scale;
		_scaleY = scale * 8;
		setCursorXY(_x, _y);
	}
	
	// инвертировать текст (0-1)
	void invertText(bool inv) { _invState = inv; }
	
	// возвращает true, если дисплей "кончился" - при побуквенном выводе
	bool isEnd() { return (_y > _maxRow); }

	// ================================== ГРАФИКА ================================== 
	// точка (заливка 1/0)
	void dot(int x, int y, byte fill = 1) {		
		if (x < 0 || x > _maxX || y < 0 || y > _maxY) return;
		_x = 0;
		_y = 0;
		if (_BUFF) {			
			bitWrite(_oled_buffer[_bufIndex(x, y)], y & 0b111, fill);			
		} else {
			if (!_buf_flag) {
				setWindow(x, y>>3, _maxX, _maxRow);
				beginData();			
				Wire.write(1 << (y & 0b111));	// задвигаем 1 на высоту y
				endData();
			} else {				
				x -= _bufX;
				y -= _bufY;
				if (x < 0 || x > _bufsizeX || y < 0 || y > (_bufsizeY << 3)) return;				
				bitWrite(_buf_ptr[ (y>>3) + x * _bufsizeY ], y & 0b111, fill);
			}
		}
	}
	
	// линия
	void line(int x0, int y0, int x1, int y1, byte fill = 1) {
		_x = 0;
		_y = 0;
		if (x0 == x1) fastLineV(x0, y0, y1, fill);
		else if (y0 == y1) fastLineH(y0, x0, x1, fill);
		else {
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
	
	// горизонтальная линия
	void fastLineH(int y, int x0, int x1, byte fill = 1) {
		_x = 0;
		_y = 0;
		if (y < 0 || y > _maxY) return;
		if (x0 == x1) {
			dot(x0, y, fill);
			return;		
		}
		x1++;
		x0 = constrain(x0, 0, _maxX);
		x1 = constrain(x1, 0, _maxX);
		if (_BUFF) {
			for (int x = x0; x < x1; x++) dot(x, y, fill);
		} else {
			if (_buf_flag) {
				for (int x = x0; x < x1; x++) dot(x, y, fill);
				return;
			}
			byte data = 0b1 << (y & 0b111);
			y >>= 3;			
			setWindow(x0, y, x1, y);
			beginData();
			for (int x = x0; x < x1; x++) writeData(data, y, x);
			endData();
		}			
	}
	
	// вертикальная линия
	void fastLineV(int x, int y0, int y1, byte fill = 1) {
		_x = 0;
		_y = 0;
		if (x < 0 || x > _maxX) return;
		if (y0 == y1) {
			dot(x, y0, fill);
			return;			
		}
		y1++;
		if (_BUFF) {
			for (int y = y0; y < y1; y++) dot(x, y, fill);
		} else {
			if (_buf_flag) {
				for (int y = y0; y < y1; y++) dot(x, y, fill);
				return;
			}
			if (fill) fill = 255;
			byte shift = y0 & 0b111;
			byte shift2 = 8 - (y1 & 0b111);
			if (shift2 == 8) shift2 = 0;
			int height = y1 - y0;
			y0 >>= 3;
			y1 = (y1 - 1) >> 3;
			byte numBytes = y1 - y0;
			setWindow(x, y0, x, y1);
			
			beginData();
			if (numBytes == 0) {			
				if (_inRange(y0, 0, _maxRow)) writeData( (fill >> (8-height)) << shift, y0, x );
			} else {
				if (_inRange(y0, 0, _maxRow)) writeData(fill << shift, y0, x);										// начальный кусок
				y0++;
				for (byte i = 0; i < numBytes - 1; i++, y0++) if (_inRange(y0, 0, _maxRow)) writeData(fill, y0, x);	// столбик
				if (_inRange(y0, 0, _maxRow)) writeData(fill >> shift2, y0, x);										// нижний кусок			
			}
			endData();

		}
	}
	
	// прямоугольник (лев. верхн, прав. нижн)	
	void rect(int x0, int y0, int x1, int y1, byte fill = 1) {
		_x = 0;
		_y = 0;
		if (fill == OLED_STROKE) {
			fastLineH(y0, x0+1, x1-1);
			fastLineH(y1, x0+1, x1-1);
			fastLineV(x0, y0, y1);
			fastLineV(x1, y0, y1);
		} else {		
			if (x0 == x1) {
				fastLineV(x0, y0, y1);
				return;
			}
			if (y0 == y1) {
				fastLineH(y0, x0, x1);
				return;
			}
			if (!_BUFF && fill == OLED_CLEAR) {
				clear(x0, y0, x1, y1);
				return;
			}			
			
			// если рисуем в мини-буфер
			if (_buf_flag) {
				x0 = constrain(x0, 0, _maxX);
				x1 = constrain(x1, 0, _maxX);
				for (byte x = x0; x <= x1; x++) fastLineV(x, y0, y1, fill == OLED_FILL ? 1 : 0);
				return;
			}
			byte thisFill = (fill == OLED_FILL ? 0 : 1);
			// рисуем в олед и в большой буфер
			x1++;
			y1++;
			byte shift = y0 & 0b111;
			byte shift2 = 8 - (y1 & 0b111);
			if (shift2 == 8) shift2 = 0;
			int height = y1 - y0;
			y0 >>= 3;
			y1 = (y1 - 1) >> 3;
			byte numBytes = y1 - y0;			
			x0 = constrain(x0, 0, _maxX);
			x1 = constrain(x1, 0, _maxX);
			
			if (!_BUFF) setWindow(x0, y0, x1, y1);
			if (!_BUFF) beginData();
			for (byte x = x0; x < x1; x++) {
				int y = y0;
				if (numBytes == 0) {			
					if (_inRange(y, 0, _maxRow)) writeData( (255 >> (8-height)) << shift, y, x, thisFill);
				} else {
					if (_inRange(y, 0, _maxRow)) writeData(255 << shift, y, x, thisFill);										// начальный кусок
					y++;
					for (byte i = 0; i < numBytes - 1; i++, y++) if (_inRange(y, 0, _maxRow)) writeData(255, y, x, thisFill);	// столбик
					if (_inRange(y, 0, _maxRow)) writeData(255 >> shift2, y, x, thisFill);										// нижний кусок			
				}
			}			
			if (!_BUFF) endData();			
		}		
	}
	
	// прямоугольник скруглённый (лев. верхн, прав. нижн)
	void roundRect(int x0, int y0, int x1, int y1, byte fill = OLED_FILL) {
		/*
		    ▅ ▅ ▅ ▅ ▅
		  ▅ ▅ ▅ ▅ ▅ ▅
		▅ ▅ ▅ ▅ ▅ ▅ ▅
		▅ ▅ ▅ ▅ ▅ ▅ ▅
		▅ ▅ ▅ ▅ ▅ ▅ ▅
		▅ ▅ ▅ ▅ ▅ ▅ ▅
		  ▅ ▅ ▅ ▅ ▅ ▅
		    ▅ ▅ ▅ ▅ ▅
		*/
		if (fill == OLED_FILL) {
			fastLineV(x0, y0+2, y1-2);
			fastLineV(x0+1, y0+1, y1-1);
			fastLineV(x1-1, y0+1, y1-1);
			fastLineV(x1, y0+2, y1-2);
			rect(x0+2, y0, x1-2, y1, OLED_FILL);			 
		} else {
			fastLineV(x0, y0+2, y1-2);
			fastLineV(x1, y0+2, y1-2);
			fastLineH(y0, x0+2, x1-2);
			fastLineH(y1, x0+2, x1-2);
			dot(x0+1, y0+1);
			dot(x1-1, y0+1);
			dot(x1-1, y1-1);
			dot(x0+1, y1-1);
		}
	}
	
	// окружность (центр х, центр у, радиус, заливка)
	void circle(int x, int y, int radius, byte fill = 0) {
		//if (!_BUFF) createBuffer(x - radius, y - radius, x + radius + 1, y + radius);
		int f = 1 - radius;
		int ddF_x = 1;
		int ddF_y = -2 * radius;
		int x1 = 0;
		int y1 = radius;
		
		byte fillLine = (fill == OLED_CLEAR) ? 0 : 1;
		dot(x, y + radius, fillLine);
		dot(x, y - radius, fillLine);
		dot(x + radius, y, fillLine);
		dot(x - radius, y, fillLine);		
		//if (fill != OLED_STROKE) fastLineH(y, x - radius, x + radius-1, fillLine);
		if (fill != OLED_STROKE) fastLineV(x, y - radius, y + radius-1, fillLine);
		while (x1 < y1) {
			if(f >= 0) {
				y1--;
				ddF_y += 2;
				f += ddF_y;
			}
			x1++;
			ddF_x += 2;
			f += ddF_x;
			if (fill == OLED_STROKE) {			
				dot(x + x1, y + y1);
				dot(x - x1, y + y1);
				dot(x + x1, y - y1);
				dot(x - x1, y - y1);			
				dot(x + y1, y + x1);
				dot(x - y1, y + x1);
				dot(x + y1, y - x1);
				dot(x - y1, y - x1);
			} else {	// FILL / CLEAR
				
				fastLineV(x + x1, y - y1, y + y1-1, fillLine);
				fastLineV(x - x1, y - y1, y + y1-1, fillLine);
				fastLineV(x + y1, y - x1, y + x1-1, fillLine);
				fastLineV(x - y1, y - x1, y + x1-1, fillLine);	
				/*
				fastLineH(y + y1, x - x1, x + x1-1, fillLine);
				fastLineH(y - y1, x - x1, x + x1-1, fillLine);
				fastLineH(y + x1, x - y1, x + y1-1, fillLine);
				fastLineH(y - x1, x - y1, x + y1-1, fillLine);	
				*/				
			}
		}
		//if (!_BUFF) sendBuffer();
	}   		
	
	// вывести битмап
	void drawBitmap(int x, int y, const uint8_t *frame, int width, int height, uint8_t invert = 0, byte mode = 0) {
		_x = 0;
		_y = 0;
		if (invert) invert = 255;
		byte left = height & 0b111;
		if (left != 0) height += (8 - left);	// округляем до ближайшего кратного степени 2
		int shiftY = (y>>3)+(height>>3);		// строка (row) крайнего байта битмапа
		setWindowShift(x, y, width, height);	// выделяем окно
		bool bottom = (_shift != 0 && shiftY >= 0 && shiftY <= _maxRow);	// рисовать ли нижний сдвинутый байт
		
		if (!_BUFF) beginData();
		for (int X = x, countX = 0; X < x+width; X++, countX++) {						// в пикселях
			byte prevData = 0;
			if (_inRange(X, 0, _maxX)) {												// мы внутри дисплея по X
				for (int Y = y>>3, countY = 0; Y < shiftY; Y++, countY++) {				// в строках (пикс/8) 					
					byte data = pgm_read_word(&(frame[countY*width+countX])) ^ invert;	// достаём байт
					if (_shift == 0) {													// без сдвига по Y
						if (_inRange(Y, 0, _maxRow)) writeData(data, Y, X, mode);		// мы внутри дисплея по Y
					} else {															// со сдвигом по Y
						if (_inRange(Y, 0, _maxRow)) writeData((prevData >> (8 - _shift)) | (data << _shift), Y, X, mode);	// задвигаем
						prevData = data;						
					}					
				}
				if (bottom) writeData(prevData >> (8 - _shift), shiftY, X, mode);		// нижний байт
			}
		}
		if (!_BUFF) endData();
	}
	
	// залить весь дисплей указанным байтом
	void fill(uint8_t data) {
		if (_TYPE < 2) {	// для SSD1306
			if (_BUFF) {
				memset(_oled_buffer, data, _bufSize);
			} else {			
				setWindow(0, 0, _maxX, _maxRow);
				beginData();
				for (int i = 0; i < (_TYPE ? 1024 : 512); i++) writeOptimised(data);				
				endData();
			}
		} else {
			// для SSH1108
		}
	}
	
	// шлёт байт в "столбик" setCursor() и setCursorXY()
	void drawByte(uint8_t data) {
		if (++_x > _maxX) return;
		if (_TYPE < 2) {								// для SSD1306
			if (!_BUFF) beginData();
			if (_shift == 0) {							// если вывод без сдвига на строку
				writeData(data);						// выводим
			} else {									// со сдвигом
				writeData(data << _shift);				// верхняя часть
				writeData(data >> (8 - _shift), 1);		// нижняя часть со сдвигом на 1
			}
			if (!_BUFF) endData();		
		} else {
			// для SSH1106
			/*			
			int h = y & 0x07;			
			if (_BUFF) {
				for (int p = 0; p < 2; p++) {
					Wire.beginTransmission(_address);
					continueCmd(0xB0 + (y >> 3) + p);        // Page
					continueCmd(0x00 + ((x + 2) & 0x0F));    // Column low nibble
					continueCmd(0x10 + ((x + 2) >> 4));      // Column high nibble
					continueCmd(0xE0);            // Read modify write
					Wire.write(OLED_ONE_DATA_MODE);
					Wire.endTransmission();
					Wire.requestFrom((int)_address, 2);
					Wire.read();                       		 // Dummy read
					int j = Wire.read();
					Wire.beginTransmission(_address);
					Wire.write(OLED_ONE_DATA_MODE);
					Wire.write((data << h) >> (p << 3) | j);
					continueCmd(0xEE);                       // Cancel read modify write
					Wire.endTransmission();
				}
			} else {
				for (int p = 0; p < 2; p++) {
					Wire.beginTransmission(_address);
					continueCmd(0xB0 + (y >> 3) + p);        // Page
					continueCmd(0x00 + ((x + 2) & 0x0F));    // Column low nibble
					continueCmd(0x10 + ((x + 2) >> 4));      // Column high nibble
					Wire.write(OLED_ONE_DATA_MODE);
					Wire.write((data << h) >> (p << 3));
					Wire.endTransmission();
				}
			}*/
		}		
	}
	
	// вывести одномерный байтовый массив (линейный битмап высотой 8)
	void drawBytes(uint8_t* data, byte size) {
		if (!_BUFF) beginData();
		for (byte i = 0; i < size; i++) {
			if (++_x > _maxX) return;			
			if (_shift == 0) {							// если вывод без сдвига на строку
				writeData(data[i]);						// выводим
			} else {									// со сдвигом
				writeData(data[i] << _shift);				// верхняя часть
				writeData(data[i] >> (8 - _shift), 1);		// нижняя часть со сдвигом на 1
			}			
		}
		if (!_BUFF) endData();
	}

	// ================================== СИСТЕМНОЕ ===================================
	// полностью обновить дисплей из буфера
	void update() {
		if (_BUFF) {
			setWindow(0, 0, _maxX, _maxRow);
			beginData();
			for (int i = 0; i < (_TYPE ? 1024 : 512); i++) writeOptimised(_oled_buffer[i]);
			endData();
		}
	}
	
	// выборочно обновить дисплей из буфера (x0, y0, x1, y1)
	void update(int x0, int y0, int x1, int y1) {
		if (_BUFF) {
			y0 >>= 3;
			y1 >>= 3;			
			setWindow(x0, y0, x1, y1);
			beginData();
			for (int x = x0; x < x1; x++)
			for (int y = y0; y < y1+1; y++)
			if (x >= 0 && x <= _maxX && y >= 0 && y <= _maxRow) 
			writeOptimised(_oled_buffer[y + x * (_TYPE ? 8 : 4)]);				
			endData();
		}
	}
	
	// отправить байт по i2с или в буфер
	void writeData(byte data, byte offsetY = 0, byte offsetX = 0, int mode = 0) {
		if (_BUFF) {
			switch (mode) {
			case 0: _oled_buffer[_bufIndex(_x+offsetX, _y) + offsetY] |= data;	// добавить
				break;
			case 1: _oled_buffer[_bufIndex(_x+offsetX, _y) + offsetY] &= ~data;	// вычесть
				break;
			case 2: _oled_buffer[_bufIndex(_x+offsetX, _y) + offsetY] = data;	// заменить
				break;				
			}
		} else {
			if (_buf_flag) {
				int x = _x - _bufX;
				int y = _y - _bufY;
				if (x < 0 || x > _bufsizeX || y < 0 || y > (_bufsizeY << 3)) return;
				switch (mode) {
				case 0: _buf_ptr[ (y>>3) + x * _bufsizeY ] |= data;		// добавить
					break;
				case 1: _buf_ptr[ (y>>3) + x * _bufsizeY ] &= ~data;	// вычесть
					break;
				case 2: _buf_ptr[ (y>>3) + x * _bufsizeY ] = data;		// заменить
					break;
				}				
			} else {
				writeOptimised(data);
			}
		}
	}
	
	// супер-костыль для либы Wire. Привет индусам!
	void writeOptimised(byte data) {
		Wire.write(data);
#if !defined(microWire_h)
		_writes++;
		if (_writes >= 16) {			
			endData();
			beginData();
		}
#endif
	}
	
	void continueCmd(uint8_t cmd) {
		Wire.write(OLED_ONE_COMMAND_MODE);
		Wire.write(cmd);
	}
	
	// отправить команду
	void sendCommand(uint8_t cmd1) {       		
		Wire.beginTransmission(_address);
		Wire.write(OLED_ONE_COMMAND_MODE);
		Wire.write(cmd1);		
		Wire.endTransmission();
	}
	
	// отправить код команды и команду
	void sendCommand(uint8_t cmd1, uint8_t cmd2) {       		
		Wire.beginTransmission(_address);
		Wire.write(OLED_COMMAND_MODE);
		Wire.write(cmd1);
		Wire.write(cmd2);
		Wire.endTransmission();
	}	
	
	// выбрать "окно" дисплея
	void setWindow(int x0, int y0, int x1, int y1) {		
		Wire.beginTransmission(_address);
		Wire.write(OLED_COMMAND_MODE);
		Wire.write(0x21);
		Wire.write(constrain(x0, 0, _maxX));
		Wire.write(constrain(x1, 0, _maxX));
		Wire.write(0x22);
		Wire.write(constrain(y0, 0, _maxRow));
		Wire.write(constrain(y1, 0, _maxRow));
		Wire.endTransmission();
	}
	
	// окно со сдвигом. x 0-127, y 0-63 (31), ширина в пикселях, высота в пикселях
	void setWindowShift(int x0, int y0, int sizeX, int sizeY) {
		_shift = y0 & 0b111;
		if (!_BUFF) setWindow(x0, (y0 >> 3), x0 + sizeX, (y0 + sizeY - 1) >> 3);
	}
	
	// ================== ДИНАМИЧЕСКИЙ БУФЕР ================
	// создать
	bool createBuffer(int x0, int y0, int x1, int y1, byte fill = 0) {
		if (!_BUFF) {
			_bufX = x0;
			_bufY = y0;
			_bufsizeX = x1 - x0 + 1;		
			_bufsizeY = ((y1 - y0) >> 3) + 1;		
			
			int bufSize = _bufsizeX * _bufsizeY;
			_buf_ptr = malloc(bufSize);
			if (_buf_ptr != NULL) {
				_buf_flag = true;
				memset(_buf_ptr, fill, bufSize);
				return true;
			} else {
				_buf_flag = false;
				return false;
			}
		}
	}
	
	// отправить
	void sendBuffer() {
		if (!_BUFF) {
			if (_buf_flag) {		
				setWindow(_bufX, _bufY>>3, _bufX + _bufsizeX, (_bufY>>3) + _bufsizeY - 1);
				beginData();
				for (int i = 0; i < _bufsizeX * _bufsizeY; i++) {
					writeOptimised(_buf_ptr[i]);
				}
				endData();
				_buf_flag = false;
				free(_buf_ptr);
			}
		}
	}

private:
	// получить "столбик-байт" буквы
	uint8_t getFont(uint8_t font, uint8_t row) {
		if (row > 4) return 0;
		font = font - '0' + 16;   // перевод код символа из таблицы ASCII
		if (font <= 90) {
			return pgm_read_byte(&(charMap[font][row])); 		// для английских букв и символов
		} else if (font >= 96 && font <= 111) {					// и пизд*ц для русских
			return pgm_read_byte(&(charMap[font + 47][row]));
		} else if (font <= 159) {              			
			return pgm_read_byte(&(charMap[font - 17][row]));
		} else {
			return pgm_read_byte(&(charMap[font - 1][row]));	// для кастомных (ё)
		}		
	}
	
	void beginData() {
		Wire.beginTransmission(_address);
		Wire.write(OLED_DATA_MODE);
	}
	
	void endData() {		
		Wire.endTransmission();
		_writes = 0;		
	}
	
	// ==================== ПЕРЕМЕННЫЕ И КОНСТАНТЫ ====================
	const uint8_t _address = 0x3C;
	const uint8_t _maxRow = (_TYPE ? 8 : 4) - 1;
	const uint8_t _maxY = (_TYPE ? 64 : 32) - 1;
	const uint8_t _maxX = (128) - 1;	// на случай добавления мелких дисплеев	
	
	// софт. буфер
	const int _bufSize = ((_BUFF == 1) ? (_TYPE ? BUFSIZE_128x64 : BUFSIZE_128x32) : 0);
	uint8_t _oled_buffer[((_BUFF == 1) ? (_TYPE ? BUFSIZE_128x64 : BUFSIZE_128x32) : 0)];
	
	// всякое
	bool _invState = 0;
	bool _println = true;
	uint8_t _scaleX = 1, _scaleY = 8;
	int _x = 0, _y = 0;
	byte _shift = 0;
	uint8_t _lastChar;
	byte _writes = 0;
	
	// дин. буфер
	int _bufsizeX, _bufsizeY;
	int _bufX, _bufY;
	uint8_t *_buf_ptr;
	bool _buf_flag = false;	
};