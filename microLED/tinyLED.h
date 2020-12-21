#ifndef tinyLED_h
#define tinyLED_h
// Микро-отправка цвета на ленту потоком
// begin() - начать отправку (НУЖНО ТОЛЬКО ДЛЯ SPI ЛЕНТ ИЛИ CLI_HIGH)
// end() - закончить отправку (НУЖНО ТОЛЬКО ДЛЯ SPI ЛЕНТ ИЛИ CLI_HIGH)
// write() - отправить один байт
// sendRGB(r,g,b) - отправить цвет на 1 диод
// sendRGBW(r,g,b,w) - отправить на rgbw ленту (например WS6812)
// send(mData) - отправить цвет в формате mData
// sendBuffer(mData буфер, размер) - отправить буфер на ленту
// clear(размер) - очистить на длину
// setBrightness(0-255) - яркость для всего вывода

// Версия 1.0, спасибище Nich1con за асм

// выключить CRT, если не указано другого
#if !defined(CRT_PGM) && !defined(CRT_SQUARE) && !defined(CRT_CUBIC) && !defined(CRT_OFF)
#define CRT_OFF
#endif

#include <color_utility.h>		// работа с цветом

// дефолтные настройки (можно оверрайд дефайном из скетча)
#ifndef TLED_ORDER
#define TLED_ORDER ORDER_GRB
#endif
#ifndef TLED_CHIP
#define TLED_CHIP LED_WS2812
#endif

#ifndef TLED_PORT
#define TLED_PORT PORTB
#endif
#ifndef TLED_DDR
#define TLED_DDR DDRB
#endif

#ifndef TLED_CLK_PORT
#define TLED_CLK_PORT PORTB
#endif
#ifndef TLED_DAT_PORT
#define TLED_DAT_PORT PORTB
#endif
#ifndef TLED_CLK_DDR
#define TLED_CLK_DDR DDRB
#endif
#ifndef TLED_DAT_DDR
#define TLED_DAT_DDR DDRB
#endif

#ifndef TLED_CLI
#define TLED_CLI CLI_LOW
#endif

#ifndef TLED_SPI_CLOCK
#define TLED_SPI_CLOCK 8000000
#endif

// const
#define ORDER_RGB 0
#define ORDER_RBG 1
#define ORDER_BRG 2
#define ORDER_BGR 3
#define ORDER_GRB 4
#define ORDER_GBR 5

#define LED_WS2812 0
#define LED_WS2815 0
#define LED_WS2813 1
#define LED_WS2818 1
#define LED_WS2811 2
#define LED_APA102 10
#define LED_APA102_SPI 20

#define CLI_OFF 0
#define CLI_LOW 1
#define CLI_AVER 2
#define CLI_HIGH 3

// нопы для 1пин лент
#define _1_NOP "nop       \n\t"
#define _2_NOP "rjmp .+0  \n\t"
#define _4_NOP _2_NOP _2_NOP
#define _8_NOP _4_NOP _4_NOP

// ======== ЛИБА SPI ========
#if (TLED_CHIP == LED_APA102_SPI)	// SPI лента
#include <SPI.h>
#endif

#if (TLED_CHIP < 10)		// 1 пин ленты
template <byte pin>
#elif (TLED_CHIP < 20)	// 2 пин ленты
template <byte pinD, byte pinC>
#endif						// SPI ленты
class tinyLED {
public:
	tinyLED() {
		// OUTPUT
#if (TLED_CHIP < 10)		// 1 пин ленты
		TLED_DDR |= 1 << pin;
#elif (TLED_CHIP < 20)	// 2 пин ленты
		TLED_DAT_DDR |= 1 << pinD;
		TLED_CLK_DDR |= 1 << pinC;
#else						// SPI ленты
		SPI.begin();
#endif
	}
	
	void begin() {
#if (TLED_CLI == CLI_HIGH)
		oledSreg = SREG;
		cli();
#endif
#if (TLED_CHIP >= 20)
		SPI.beginTransaction(SPISettings(TLED_SPI_CLOCK, MSBFIRST, SPI_MODE0));
#endif
#if (TLED_CHIP >= 10)
		for (byte i = 0; i < 4; i++) write(0);	
#endif
	}
	
	void end() {
#if (TLED_CLI == CLI_HIGH)
		SREG = oledSreg;
#endif
#if (TLED_CHIP >= 10)
		for (byte i = 0; i < 4; i++) write(0);
#endif
#if (TLED_CHIP >= 20)		
		SPI.endTransaction();
#endif
	}

	void write(uint8_t data) {
#ifndef TLED_STATIC_BRIGHT
		if (_bright != 255) data = fade8(data, _bright);
#endif
#if (TLED_CLI == CLI_LOW)
		uint8_t oledSreg = SREG;
		cli();
#endif
		// ====================================================================
#if (TLED_CHIP < 10)		// 1-пин ленты
		asm volatile
		(
		"LDI r18,8            \n\t"   // Счетчик 8ми циклов
		"_LOOP_START_%=:      \n\t"   // Начало цикла
		"SBI %[PORT], %[PIN]  \n\t"   // HIGH на выход
		"SBRS %[DATA], 7      \n\t"   // Если бит '7' установлен, пропуск след. инструкции
		"CBI %[PORT], %[PIN]  \n\t"   // LOW на выход

#if (F_CPU == 9600000UL)	// тини13
		// ========================
#if (TLED_CHIP == 0)
		_4_NOP
#elif (TLED_CHIP == 1)
		_4_NOP _1_NOP
#elif (TLED_CHIP == 2)
		_8_NOP
#endif
		// ========================
#elif (F_CPU == 8000000UL)
#if (TLED_CHIP == 2)
		_4_NOP _1_NOP
#else
		_1_NOP
#endif
		// ========================	
#elif (F_CPU == 16000000UL)
#if (TLED_CHIP == 2)
		_8_NOP _4_NOP _2_NOP
#else
		_8_NOP 
#endif
		// ========================
#endif
		"CBI %[PORT], %[PIN]    \n\t"   // LOW на выход
		"LSL %[DATA]            \n\t"   // Сдвигаем данные влево
		"DEC r18                \n\t"   // Декремент счетчика циклов
		"BRNE _LOOP_START_%=    \n\t"   // Переход в начало цикла
		:
		:[DATA]"r"(data),
		[PORT]"I"(_SFR_IO_ADDR(TLED_PORT)),
		[PIN]"I"(pin)
		:"r17", "r18"
		);
#elif (TLED_CHIP < 20)	// 2 пин ленты
		asm volatile
		(
		"LDI R16, 8             \n\t"
		"LOOP_%=:               \n\t"
		"CBI %[CLK_PORT],%[CLK] \n\t"
		"CBI %[DAT_PORT],%[DAT] \n\t"
		"SBRC %[DATA], 7        \n\t"
		"SBI %[DAT_PORT],%[DAT] \n\t"
		"SBI %[CLK_PORT],%[CLK] \n\t"
		"LSL %[DATA]            \n\t"
		"DEC R16                \n\t"
		"BRNE LOOP_%=           \n\t"
		:
		:
		[CLK_PORT]"I"(_SFR_IO_ADDR(TLED_CLK_PORT)),
		[DAT_PORT]"I"(_SFR_IO_ADDR(TLED_DAT_PORT)),
		[CLK]"I"(pinC),
		[DAT]"I"(pinD),
		[DATA]"r"(data)
		: "r16"
		);
#else 						// SPI ленты
		SPI.transfer(data);
#endif
		// ====================================================================
#if (TLED_CLI == CLI_LOW)
		SREG = oledSreg;
#endif
	}
	
	void sendRGB(uint8_t r, uint8_t g, uint8_t b) {
#if (TLED_CLI == CLI_AVER)
		uint8_t oledSreg = SREG;
		cli();
#endif
#if (TLED_CHIP >= 10)
		write(255);	// старт байт для spi лент
#endif
#if (TLED_ORDER == ORDER_RGB)
		write(r); write(g); write(b);
#elif (TLED_ORDER == ORDER_RBG)
		write(r); write(b); write(g);
#elif (TLED_ORDER == ORDER_BRG)
		write(b); write(r); write(g);
#elif (TLED_ORDER == ORDER_BGR)
		write(b); write(g); write(r);
#elif (TLED_ORDER == ORDER_GRB)
		write(g); write(r); write(b);
#elif (TLED_ORDER == ORDER_GBR)
		write(g); write(b); write(r);
#endif
#if (TLED_CLI == CLI_AVER)
		SREG = oledSreg;
#endif
	}
	
	void sendRGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
		sendRGB(r, g, b);
		write(w);
	}
	
	void send(mData data) {
		sendRGB(getR(data), getG(data), getB(data));
	}
	
	void sendBuffer(mData* data, int size) {
		begin();
		for (int i = 0; i < size; i++) send(data[i]);
		end();
	}
	
	void clear(int size) {
		begin();
		for (int i = 0; i < size; i++) sendRGB(0, 0, 0);
		end();
	}
	
	void setBrightness(byte bright) {
#ifndef TLED_STATIC_BRIGHT
		_bright = getCRT_SQUARE(bright);
#endif
	}
	
private:
#ifndef TLED_STATIC_BRIGHT
	byte _bright = 255;
#endif
#if (TLED_CLI == CLI_HIGH)
	uint8_t oledSreg;
#endif
};
#endif