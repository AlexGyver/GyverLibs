/*
	microLED - ультра-лёгкая библиотека для работы с адресной лентой/матрицей
	Основная фишка: сжатие цвета, код занимает в разы меньше места в SRAM по сравнению с аналогами (FastLED, NeoPixel и др.)
	- Поддержка сжатия цвета: 8, 16 и 24 бита
	- Возможность работать вообще без буфера (с некоторыми ограничениями)
	- Работа с цветом:
	- RGB
	- HSV
	- HEX (WEB цвета)
	- "Цветовое колесо" (1500 или 255 самых ярких оттенков)
	- 16 встроенных цветов
	- Цвет по теплоте
	- Градиенты
	- Шум Перлина (из FastLED)
	- Возможность чтения сжатого цвета в mHEX 0xRRGGBB и массив RGB
	- Встроенная поддержка работы с адресными матрицами
	- Поддержка чипов: 2811/2812/2813/2815/2818
	- Частичная совместимость со скетчами FastLED
	- Совместимость типов данных и инструментов из FastLED
	
	by AlexGyver & Egor 'Nich1con' Zaharov
	
	Версия 1.1
	- Поправлена инициализация
	- Добавлен оранжевый цвет
	
	Версия 2.0
	- Переписан и сильно ускорен алгоритм вывода
	- Добавлено ограничение тока
	
	Версия 2.1
	- Поправлена ошибка с матрицей
	
	Версия 2.2
	- Цвет PINK заменён на MAGENTA
	
	Версия 2.3
	- Добавлена дефайн настройка MICROLED_ALLOW_INTERRUPTS
	- Исправлены мелкие ошибки, улучшена стабильность
	
	Версия 2.4
	- Добавлен ORDER_BGR
	
	Версия 2.5
	- Яркость по CRT гамме
	
	Версия 3.0
	- Добавлены функции и цвета: 
	- Цветовая температура .setKelvin() и дата Kelvin
	- getBlend(позиция, всего, цвет1, цвет2) и getBlend2(позиция, всего, цвет1, цвет2)
	- .fill(от, до)
	- .fillGradient(от, до, цвет1, цвет2)
	- Добавлен шум Перлина (вытащил из FastLED)
	- Добавлены градиенты
	- Полностью переделан и оптимизирован вывод
	- Возможность работать вообще без буфера
	- Настройка ограничения тока для всех типов лент
	- Настраиваемый запрет прерываний
	- Сохранение работы миллиса на время отправки
	- Поддержка лент 2811, 2812, 2813, 2815, 2818
	- Поддержка 4х цветных лент: WS6812
	- Инициализация переделана под шаблон, смотри примеры!
	- Много изменений в названиях, всё переделано и упрощено, читай документацию!
	
	Версия 3.1
	- Поправлены ошибки компиляции для нестандартных ядер Ардуино и Аттини
	- Добавлен класс tinyLED.h для вывода потоком с ATtiny и вообще любых AVR (см. пример)
	- Вырезаны инструменты FastLED (рандом, шум), будем работать напрямую с фастлед
	- Добавлена поддержка совместной работы с библиотекой FastLED и конвертация из её типов!
	- Добавлена поддержка ленты APA102 (а также других SPI), программная и аппаратная SPI
	
	Версия 3.2
	- Чуть оптимизации и исправлений
*/
#ifndef microLED_h
#define microLED_h

#ifndef COLOR_DEBTH
#define COLOR_DEBTH 3	// по умолчанию 24 бита
#endif

#include "color_utility.h"

#ifdef MLED_USE_SPI
#include <SPI.h>
#endif

#ifndef MLED_SPI_CLOCK
#define MLED_SPI_CLOCK 8000000
#endif

// чип
enum M_chip {
	LED_WS2811,	// GBR
	LED_WS2812,	// GRB
	LED_WS2813,	// GRB
	LED_WS2815,	// GRB
	LED_WS2818,	// RGB
	LED_WS6812,	// BGR
	LED_APA102,	// BGR
	LED_APA102_SPI,	// BGR
};

#define CHIP4COLOR (chip == LED_WS6812)

enum M_order {
	// порядок цвета: r-00, g-01, b-10
	ORDER_RGB = 0b000110,
	ORDER_RBG = 0b001001,
	ORDER_BRG = 0b100001,
	ORDER_BGR = 0b100100,
	ORDER_GRB = 0b010010,
	ORDER_GBR = 0b011000,
};

enum M_ISR {
	CLI_OFF,
	CLI_LOW,
	CLI_AVER,
	CLI_HIGH,
};

const byte SAVE_MILLIS = 1;
const int8_t MLED_NO_CLOCK = -1;

// ========== ПОДКЛЮЧЕНИЕ МАТРИЦЫ ==========
enum M_type {
	ZIGZAG,
	PARALLEL,
};
enum M_connection {
	LEFT_BOTTOM,
	LEFT_TOP,
	RIGHT_TOP,
	RIGHT_BOTTOM,
};
enum M_dir {
	DIR_RIGHT,
	DIR_UP,
	DIR_LEFT,
	DIR_DOWN,
};

void systemUptimePoll(void);	// дёрнуть миллисы

// ============================================== КЛАСС ==============================================
// <amount, pin, clock pin, chip, order, cli, mls> ()
// <amount, pin, clock pin, chip, order, cli, mls> (width, height, type, conn, dir)
// количество, пин, чип, порядок, прерывания, миллис
template<int amount, int8_t pin, int8_t pinCLK, M_chip chip, M_order order, M_ISR isr = CLI_OFF, uint8_t uptime = 0>
class microLED {
public:
	/*
		// ЛЕНТА: нет аргументов
		microLED;
		
		// МАТРИЦА: ширина матрицы, высота матрицы, тип матрицы, угол подключения, направление (см. ПОДКЛЮЧЕНИЕ МАТРИЦЫ)
		microLED(uint8_t width, uint8_t height, M_type type, M_connection conn, M_dir dir);
		
		// лента и матрица
		void set(int n, mData color);					// ставим цвет светодиода mData (равносильно leds[n] = color)					
		mData get(int num);								// получить цвет диода в mData (равносильно leds[n])
		void fill(mData color);							// заливка цветом mData
		void fill(int from, int to, mData color);		// заливка цветом mData
		void fillGradient(int from, int to, mData color1, mData color2);	// залить градиентом двух цветов
		void fade(int num, byte val);					// уменьшить яркость
		
		// матрица
		uint16_t getPixNumber(int x, int y);			// получить номер пикселя в ленте по координатам
		void set(int x, int y, mData color);			// ставим цвет пикселя x y в mData
		mData get(int x, int y);						// получить цвет пикселя в mData
		void fade(int x, int y, byte val);				// уменьшить яркость
		void drawBitmap8(int X, int Y, const uint8_t *frame, int width, int height);	// вывод битмапа (битмап 1мерный PROGMEM)
		void drawBitmap16(int X, int Y, const uint16_t *frame, int width, int height);	// вывод битмапа (битмап 1мерный PROGMEM)
		void drawBitmap32(int X, int Y, const uint32_t *frame, int width, int height);	// вывод битмапа (битмап 1мерный PROGMEM)
		
		// общее
		void setMaxCurrent(int ma);						// установить максимальный ток (автокоррекция яркости). 0 - выключено
		void setBrightness(uint8_t newBright);			// яркость 0-255
		void clear();									// очистка
		
		// вывод буфера
		void show();									// вывести весь буфер
		
		// вывод потока
		void begin();									// начать вывод потоком
		void send(mData data);							// отправить один светодиод
		void end();										// закончить вывод потоком
	*/
	
	int oneLedMax = 46;
	int oneLedIdle = 2000;
	mData leds[amount];
	byte white[(CHIP4COLOR) ? amount : 0];
	
	void init() {
		if (pin != MLED_NO_CLOCK) {
			_dat_mask = digitalPinToBitMask(pin);
			_dat_port = portOutputRegister(digitalPinToPort(pin));
			_dat_ddr = portModeRegister(digitalPinToPort(pin));	
			*_dat_ddr |= _dat_mask;
		}
		if (pinCLK != MLED_NO_CLOCK) {
			_clk_mask = digitalPinToBitMask(pinCLK);
			_clk_port = portOutputRegister(digitalPinToPort(pinCLK));
			_clk_ddr = portModeRegister(digitalPinToPort(pinCLK));	
			*_clk_ddr |= _clk_mask;
		}
		switch (chip) {
		case LED_WS2811: oneLedMax = 46; oneLedIdle = 2000; break;
		case LED_WS2812: oneLedMax = 30; oneLedIdle = 660; break;	// 28/240 для ECO, 32/700 матрица
		case LED_WS2813: oneLedMax = 30; oneLedIdle = 1266; break;
		case LED_WS2815: oneLedMax = 10; oneLedIdle = 1753; break;
		case LED_WS2818: oneLedMax = 46; oneLedIdle = 1900; break;
		}
		// oneLedMax = (ток ленты с одним горящим) - (ток выключенной ленты)
		// oneLedIdle = (ток выключенной ленты) / (количество ледов)
#ifdef MLED_USE_SPI
		SPI.begin();
#endif
	}
	
	microLED() {
		init();
	}
	
	microLED(uint8_t width, uint8_t height, M_type type, M_connection conn, M_dir dir) :
	_width(width), _height(height), _matrixConfig( (uint8_t)conn | ((uint8_t)dir << 2) ), _matrixType( (uint8_t)type ) {	
		init();	
		if (_matrixConfig == 4 || _matrixConfig == 13 || _matrixConfig == 14 || _matrixConfig == 7)	_matrixW = height;
		else _matrixW = width;	
	}
	
	void setBrightness(uint8_t newBright) {
		_bright = getCRT(newBright);
	}
	
	void clear() {
		for (int i = 0; i < amount; i++) leds[i] = 0;		
	}
	
	void fill(mData color) {
		for (int i = 0; i < amount; i++) leds[i] = color;	
	}
	
	void fill(int from, int to, mData color) {
		for (int i = from; i <= to; i++) leds[i] = color;	
	}
	
	void fillGradient(int from, int to, mData color1, mData color2) {
		for (int i = from; i < to; i++) leds[i] = getBlend(i-from, to-from, color1, color2);
	}
	
	void set(int n, mData color) {
		leds[n] = color;
	}
	
	mData get(int num) {
		return leds[num];
	}
	
	void fade(int num, byte val) {
		leds[num] = getFade(leds[num], val);
	}
	
	// ============================================== МАТРИЦА ==============================================
	uint16_t getPixNumber(int x, int y) {
		int thisX, thisY;
		switch(_matrixConfig) {
		case 0: 	thisX = x;					thisY = y; 					break;
		case 4: 	thisX = y;					thisY = x;					break;
		case 1: 	thisX = x; 					thisY = (_height - y - 1);	break;
		case 13: 	thisX = (_height - y - 1);	thisY = x;					break;
		case 10: 	thisX = (_width - x - 1);	thisY = (_height - y - 1);	break;
		case 14: 	thisX = (_height - y - 1);	thisY = (_width - x - 1);	break;
		case 11: 	thisX = (_width - x - 1);	thisY = y;					break;
		case 7:		thisX = y; 					thisY = (_width - x - 1);	break;
		}
		
		if (_matrixType || !(thisY % 2)) return (thisY * _matrixW + thisX);	// если чётная строка
		else return (thisY * _matrixW + _matrixW - thisX - 1);				// если нечётная строка	
	}
	
	void set(int x, int y, mData color) {
		if (x * y >= amount || x < 0 || y < 0 || x >= _width || y >= _height) return;
		leds[getPixNumber(x, y)] = color;
	}
	
	mData get(int x, int y) {
		return leds[getPixNumber(x, y)];
	}
	
	void fade(int x, int y, byte val) {
		int pix = getPixNumber(x, y);
		leds[pix] = getFade(leds[pix], val);
	}	

	void drawBitmap8(int X, int Y, const uint8_t *frame, int width, int height) {
		for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
		set(x + X, y + Y, pgm_read_byte(&frame[x + (height - 1 - y) * width]));
	}
	void drawBitmap16(int X, int Y, const uint16_t *frame, int width, int height) {
		for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
		set(x + X, y + Y, pgm_read_word(&frame[x + (height - 1 - y) * width]));
	}
	void drawBitmap32(int X, int Y, const uint32_t *frame, int width, int height) {
		for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
		set(x + X, y + Y, pgm_read_dword(&frame[x + (height - 1 - y) * width]));
	}

	// ============================================== УТИЛИТЫ ==============================================
	void setMaxCurrent(int ma) {
		_maxCurrent = ma;
	}
	
	uint8_t correctBright(uint8_t bright) {
		long sum = 0;
		for (int i = 0; i < amount; i++) {
			sum += fade8R(leds[i], bright);
			sum += fade8G(leds[i], bright);
			sum += fade8B(leds[i], bright);
		}		
		
		sum = ((long)sum >> 8) * oneLedMax / 3;					// текущий "активный" ток ленты	
		int idle = (long)oneLedIdle * amount / 1000;			// холостой ток ленты		
		if (sum == 0) return bright;
		if ((sum + idle) < _maxCurrent) return bright;			// ограничения нет
		else return ((float)(_maxCurrent - idle) * bright / sum);	// пересчёт яркости		
	}
	
	// ============================================== ВЫВОД ==============================================
	// асм задержки
	#define _1_NOP "NOP 		\n\t" 
	#define _2_NOP "RJMP .+0	\n\t"
	#define _4_NOP _2_NOP _2_NOP
	#define _8_NOP _4_NOP _4_NOP
	// Издержки для бита H - 5 тактов, для бита L - 3 такта
	
	void begin() {		
		if (pin != MLED_NO_CLOCK) {
			_mask_h = _dat_mask | *_dat_port;
			_mask_l = ~_mask_h & *_dat_port;
		}
		_showBright = _bright;
		if (isr == CLI_HIGH) {		// Макс приоритет, отправка всего буфера не может быть прервана
			sregSave = SREG;
			cli();
		}
#ifdef MLED_USE_SPI
		SPI.beginTransaction(SPISettings(MLED_SPI_CLOCK, MSBFIRST, SPI_MODE0));		
#endif
		if (chip == LED_APA102 || chip == LED_APA102_SPI) for (byte i = 0; i < 4; i++) sendRaw(0);
	}
	
	void show() {
		begin();
		if (_maxCurrent != 0 && amount != 0) _showBright = correctBright(_bright);
		if (CHIP4COLOR)	for (int i = 0; i < amount; i++) send(leds[i], white[i]);
		else for (int i = 0; i < amount; i++) send(leds[i]);
		end();
	}
	
	void send(mData color, byte thisWhite = 0) {
		uint8_t data[3];
		// компилятор посчитает сдвиги
		data[(order >> 4) & 0b11] = fade8R(color, _showBright);
		data[(order >> 2) & 0b11] = fade8G(color, _showBright);
		data[order & 0b11] = fade8B(color, _showBright);
		if (CHIP4COLOR) thisWhite = fade8(thisWhite, _showBright);
		
		if (isr == CLI_AVER) {		// Средний приоритет, текущий диод однозначно будет обновлен
			sregSave = SREG;
			cli();
		}
		
		if (chip == LED_APA102 || chip == LED_APA102_SPI) sendRaw(255);	// старт байт SPI лент
		
		// отправляем RGB и W если есть
		for (uint8_t i = 0; i < 3; i++) sendRaw(data[i]);
		if (CHIP4COLOR) sendRaw(thisWhite);
		
		if (isr == CLI_AVER) SREG = sregSave;		// Средний приоритет, вернуть прерывания		
		if (uptime && (isr == CLI_AVER || isr == CLI_HIGH)) systemUptimePoll();  // пнуть миллисы
	}
	
	void sendRaw(byte data) {
		if (isr == CLI_LOW) {		// Низкий приоритет, текущий байт однозначно будет отправлен
			sregSave = SREG;
			cli();
		}
		switch (chip) {
		case LED_WS2811:
			asm volatile (
			"LDI  r17 ,8        \n\t"       // Загружаем в счетчик циклов 8
			"_LOOP_START_%=:       \n\t"       // Начало основного цикла
			"ST X, %[SET_H]     \n\t"       // Устанавливаем на выходе HIGH
			"SBRS %[DATA], 7    \n\t"       // Если текущий бит установлен - пропуск след. инстр.
			"ST X, %[SET_L]     \n\t"       // Устанавливаем на выходе LOW
			"LSL  %[DATA]       \n\t"       // Двигаем данные влево на один бит
			#if(F_CPU == 16000000UL)
			_8_NOP _4_NOP _2_NOP
			#elif (F_CPU == 8000000UL)
			_4_NOP _1_NOP
			#endif
			"ST X, %[SET_L]     \n\t"       // Устанавливаем на выходе LOW
			"DEC   r17          \n\t"       // Декремент счетчика циклов
			"BRNE  _LOOP_START_%=  \n\t"       // Переход на новый цикл, если счетчик не иссяк
			:
			:[S_REG]"I"(_SFR_IO_ADDR(SREG)),
			[DATA] "r" (data),
			"x" (_dat_port),
			[SET_H] "r" (_mask_h),
			[SET_L] "r" (_mask_l)
			:"r17", "r18"
			);
			break;
		case LED_WS2812:
		case LED_WS2813:
		case LED_WS2815:
		case LED_WS2818:
		case LED_WS6812:
			asm volatile
			(
			"LDI  r17 ,8        \n\t"       // Загружаем в счетчик циклов 8
			"_LOOP_START_%=:       \n\t"       // Начало основного цикла
			"ST X, %[SET_H]     \n\t"       // Устанавливаем на выходе HIGH
			"SBRS %[DATA], 7    \n\t"       // Если текущий бит установлен - пропуск след. инстр.
			"ST X, %[SET_L]     \n\t"       // Устанавливаем на выходе LOW
			"LSL  %[DATA]       \n\t"       // Двигаем данные влево на один бит
			#if(F_CPU == 16000000UL)
			_8_NOP 
			#elif (F_CPU == 8000000UL)
			_1_NOP
			#endif
			"ST X, %[SET_L]     \n\t"       // Устанавливаем на выходе LOW
			"DEC   r17          \n\t"       // Декремент счетчика циклов
			"BRNE  _LOOP_START_%=  \n\t"       // Переход на новый цикл, если счетчик не иссяк
			:
			:[S_REG]"I"(_SFR_IO_ADDR(SREG)),
			[DATA] "r" (data),
			"x" (_dat_port),
			[SET_H] "r" (_mask_h),
			[SET_L] "r" (_mask_l)
			:"r17", "r18"
			);
			break;
		case LED_APA102:
			for (uint8_t i = 0; i < 8; i++)  {
				if (data & (1 << 7)) *_dat_port |= _dat_mask;
				else *_dat_port &= ~_dat_mask;
				*_clk_port |= _clk_mask;
				*_clk_port &= ~_clk_mask;
				data <<= 1;
			}
			break;
		case LED_APA102_SPI:
#ifdef MLED_USE_SPI
			SPI.transfer(data);
#endif
			break;
		}
		if (isr == CLI_LOW) SREG = sregSave;	// Низкий приоритет, вернуть прерывания
	}
	
	void end() {
		if (isr == CLI_HIGH) SREG = sregSave;		// Макс приоритет, вернуть прерывания
		if (chip == LED_APA102 || chip == LED_APA102_SPI) for (byte i = 0; i < 4; i++) sendRaw(0);
#ifdef MLED_USE_SPI		
		SPI.endTransaction();
#endif
	}
	
private:
	uint8_t _bright = 50, _showBright = 50;
	const uint8_t _matrixConfig = 0, _matrixType = 0, _width = 0, _height = 0;
	uint8_t _matrixW = 0;	
	int _maxCurrent = 0;
	
	volatile uint8_t *_dat_port, *_dat_ddr;
	volatile uint8_t *_clk_port, *_clk_ddr;
	uint8_t _dat_mask;
	uint8_t _clk_mask;
	uint8_t _mask_h, _mask_l;
	uint8_t sregSave = SREG;
	
};	// класс

// Ручное обслуживание функций времени на период запрета прерываний
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)
void systemUptimePoll(void) {
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	extern volatile unsigned long timer0_overflow_count;
	extern volatile unsigned long timer0_millis;
	extern unsigned char timer0_fract;
	
	if (TIFR0 & (1 << TOV0)) {
		TIFR0 |= 1 << TOV0;
		uint32_t m = timer0_millis;
		uint8_t f = timer0_fract;
		m += MILLIS_INC;
		f += FRACT_INC;
		if (f >= FRACT_MAX) {
			f -= FRACT_MAX;
			m += 1;
		}
		timer0_fract = f;
		timer0_millis = m;
		timer0_overflow_count++;
	}
#endif
}
#endif