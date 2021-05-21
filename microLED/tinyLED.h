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

// Версия 1.1
// - Переработан ASM вывод, меньше весит, легче адаптируется под другие частоты / тайминги


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
	  uint8_t _loop_count = 0;			// Счетчик для циклов отправки бит
      uint8_t _delay_loop_count = 0;	// Счетчик для циклов задержек asm	
		
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
		"LDI %[CNT],8         \n\t"   // Счетчик 8ми циклов
		"_LOOP_START_%=:      \n\t"   // Начало цикла
		"SBI %[PORT], %[PIN]  \n\t"   // HIGH на выход
		"SBRS %[DATA], 7      \n\t"   // Если бит '7' установлен, пропуск след. инструкции
		"CBI %[PORT], %[PIN]  \n\t"   // LOW на выход
//-----------------------------------------------------------------------------------------
#if (F_CPU == 8000000UL) && (TLED_CHIP != 2)
		"NOP  				  \n\t"	  // Единственный NOP для 8мгц
#else
	  
#if (F_CPU == 16000000UL)
#if (TLED_CHIP == 2)				  // 14CK delay (4 * 3CK) + LDI 1CK + NOP
		"LDI %[DELAY], 4      \n\t"
		"NOP  				  \n\t"		
#else								  // 8CK delay (2 * 3CK) + LDI 1CK + NOP
		"LDI %[DELAY], 2      \n\t"
		"NOP  				  \n\t"		
#endif	
#elif (F_CPU == 8000000UL)			  // 5CK delay (1 * 3CK) + LDI 1CK + NOP
		"LDI %[DELAY], 1      \n\t"
		"NOP  				  \n\t"	
#elif (F_CPU == 9600000UL)
#if (TLED_CHIP == 0)				  // 4CK delay (1 * 3CK) + LDI 1CK
		"LDI %[DELAY], 1      \n\t"
#elif (TLED_CHIP == 1)				  // 5CK delay (1 * 3CK) + LDI 1CK + NOP
		"LDI %[DELAY], 1      \n\t"
		"NOP  				  \n\t"	
#elif (TLED_CHIP == 2)				  // 8CK delay (2 * 3CK) + LDI 1CK + NOP
		"LDI %[DELAY], 2      \n\t"
		"NOP  				  \n\t"	
#endif  
#endif
        "_DELAY_LOOP_%=:    \n\t"     // Цикл задержки
        "DEC %[DELAY]       \n\t"     // 1CK декремент
        "BRNE _DELAY_LOOP_%=\n\t"     // 2CK переход
#endif
//-----------------------------------------------------------------------------------------
		"CBI %[PORT], %[PIN]    \n\t"   // LOW на выход
		"LSL %[DATA]            \n\t"   // Сдвигаем данные влево
		"DEC %[CNT]             \n\t"   // Декремент счетчика циклов
		"BRNE _LOOP_START_%=    \n\t"   // Переход в начало цикла
		:[CNT] "+r" (_loop_count),
        [DELAY] "+r" (_delay_loop_count)
		:[DATA]"r"(data),
		[PORT]"I"(_SFR_IO_ADDR(TLED_PORT)),
		[PIN]"I"(pin)
		);
#elif (TLED_CHIP < 20)	// 2 пин ленты
		asm volatile
		(
		"LDI %[CNT], 8          \n\t"
		"LOOP_%=:               \n\t"
		"CBI %[CLK_PORT],%[CLK] \n\t"
		"CBI %[DAT_PORT],%[DAT] \n\t"
		"SBRC %[DATA], 7        \n\t"
		"SBI %[DAT_PORT],%[DAT] \n\t"
		"SBI %[CLK_PORT],%[CLK] \n\t"
		"LSL %[DATA]            \n\t"
		"DEC %[CNT]             \n\t"
		"BRNE LOOP_%=           \n\t"
		:[CNT] "+r" (_loop_count)
		:[CLK_PORT]"I"(_SFR_IO_ADDR(TLED_CLK_PORT)),
		[DAT_PORT]"I"(_SFR_IO_ADDR(TLED_DAT_PORT)),
		[CLK]"I"(pinC),
		[DAT]"I"(pinD),
		[DATA]"r"(data)
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