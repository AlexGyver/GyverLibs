#ifndef Gyver433_h
#define Gyver433_h
/*
	Суперлёгкая библиотека для радиомодулей 433 МГц
	- Не использует прерывания и таймеры (кроме нулевого, читает micros())
	- Встроенный CRC контроль целостности
	- Ускоренный алгоритм IO для AVR Arduino
	
	Передатчик:
	Gyver433_TX tx(пин) - создать объект
	sendData(data) - отправить, любой тип данных
	
	Приёмник:
	Gyver433_RX rx(пин) - создать объект
	tick() - вызывать постоянно для чтения. Асинхронный. Вернёт количество принятых байт
	tickWait() - тож самое, но блокирует выполнение, принимает более четко
	readData(data) - прочитать, любой тип данных
	size - количество принятых байтов
*/

#ifndef G433_SPEED
#define G433_SPEED 2000		// скорость бит/сек (минимальная)
#endif
#ifndef G433_BUFSIZE
#define G433_BUFSIZE 64		// размер буфера приёма и отправки
#endif

// тайминги интерфейса (компилятор посчитает)
#define HIGH_PULSE (1000000ul/G433_SPEED)
#define LOW_PULSE (HIGH_PULSE/2)
#define START_PULSE (HIGH_PULSE*2)
#define PULSE_HYST (LOW_PULSE/2)
#define START_MIN (START_PULSE-PULSE_HYST)
#define START_MAX (START_PULSE+PULSE_HYST)
#define LOW_MIN (LOW_PULSE-PULSE_HYST)
#define LOW_MAX (LOW_PULSE+PULSE_HYST)
#define HIGH_MIN (HIGH_PULSE-PULSE_HYST)
#define HIGH_MAX (HIGH_PULSE+PULSE_HYST)

// crc
byte G433_crc(byte *buffer, byte size);
void G433_crc_byte(uint8_t &crc, uint8_t data);

// ============ ПЕРЕДАТЧИК ============
class Gyver433_TX {
public:
	Gyver433_TX(byte pin) : _pin(pin) {
#if defined(__AVR__)
		_port_reg = portOutputRegister(digitalPinToPort(pin));
		_bit_mask = digitalPinToBitMask(pin);		
#endif
		pinMode(pin, OUTPUT);
	}
	
	// отправка, блокирующая. Кушает любой тип данных
	template <typename T>
	void sendData(T &data) {
		const uint8_t *ptr = (const uint8_t*) &data;
		for (uint16_t i = 0; i < sizeof(T); i++) buffer[i] = *ptr++;
		buffer[sizeof(T)] = G433_crc(buffer, sizeof(T));	// CRC последним байтом
		bool flag = 0;										// флаг дрыга
		for (byte i = 0; i < 30; i++) {						// 30 импульсов для синхронизации
			flag = !flag;
			fastDW(flag);
			delayMicroseconds(HIGH_PULSE);
		}
		fastDW(1);											// старт бит
		delayMicroseconds(START_PULSE);  					// старт бит
		for (int n = 0; n < sizeof(T) + 1; n++) {			// буфер + CRC
			for (byte b = 0; b < 8; b++) {
				fastDW(flag);
				flag = !flag;
				if (bitRead(buffer[n], b)) delayMicroseconds(HIGH_PULSE);
				else delayMicroseconds(LOW_PULSE);				
			}
		}
		fastDW(0);											// передача окончена
	}	
	
private:
	void fastDW(bool state) {
#if defined(__AVR__)
		if (state) *_port_reg |= _bit_mask;	// HIGH
		else *_port_reg &= ~_bit_mask;		// LOW
#else
		digitalWrite(_pin, state);
#endif
	}
	byte buffer[G433_BUFSIZE];
	const byte _pin;
#if defined(__AVR__)
	volatile uint8_t *_port_reg;
	volatile uint8_t _bit_mask;
#endif
};


// ============ ПРИЁМНИК ============
class Gyver433_RX {
public:
	Gyver433_RX(byte pin){
#if defined(__AVR__)
		_pin_reg = portInputRegister(digitalPinToPort(pin));
		_bit_mask = digitalPinToBitMask(pin);
#endif
	}
	
	// неблокирующий приём, вернёт кол-во успешно принятых байт
	byte tick() {
		bool newState = fastDR();			// читаем пин
		if (newState != prevState) {  		// ловим изменение сигнала
			uint32_t thisUs = micros();
			uint32_t thisPulse = thisUs - tmr;
			if (parse == 1) {   			// в прошлый раз поймали фронт
				if (thisPulse > START_MIN && thisPulse < START_MAX) {	// старт бит?
					parse = 2;	// ключ на старт
					tmr = thisUs;
					byteCount = 0;
					bitCount = 0;
					size = 0;
					for (byte i = 0; i < G433_BUFSIZE; i++) buffer[i] = 0;
				} else {												// не старт бит
					parse = 0; 				
				}
			} else if (parse == 2) {		// идёт парсинг
				if (thisPulse > LOW_MIN && thisPulse < LOW_MAX) {			// low бит
					// просто пропускаем (в буфере уже нули)
					tmr = thisUs;
					bitCount++;
					if (bitCount == 8) {						
						bitCount = 0;
						byteCount++;
						if (byteCount > G433_BUFSIZE) parse = 0;			// оверфлоу
					}
				} else if (thisPulse > HIGH_MIN && thisPulse < HIGH_MAX) {	// high бит
					bitSet(buffer[byteCount], bitCount);					// ставим бит единичку
					tmr = thisUs;					
					bitCount++;
					if (bitCount == 8) {						
						bitCount = 0;
						byteCount++;
						if (byteCount > G433_BUFSIZE) parse = 0;			// оверфлоу
					}
				} else {													// ошибка или конец передачи
					tmr = thisUs;
					parse = 0;										
					// проверяем, есть ли данные и целые ли они
					if (byteCount > 0 && G433_crc(buffer, byteCount) == 0) {
						size = byteCount - 2;		// длина даты (минус crc)
						return size;
					}
					else return 0;					
				}
			}

			if (newState && !prevState && parse == 0) { 	// ловим фронт
				parse = 1;									// в следующий раз ждём флаг
				tmr = thisUs;
			}
			prevState = newState;
		}
		return 0;
	}
	
	// блокирующий приём, вернёт кол-во успешно принятых байт
	byte tickWait() {
		do {
		tick(); 
		} while (parse == 2);
		if (byteCount > 0) {
			byteCount = 0;
			return size;
		} else return 0;
	}
	
	// прочитает буфер в любой тип данных
	template <typename T>
	bool readData(T &data) {
		if (sizeof(T) > G433_BUFSIZE) return false;		
		uint8_t *ptr = (uint8_t*) &data;	
		for (uint16_t i = 0; i < sizeof(T); i++) *ptr++ = buffer[i];
		return true;
	}
	
	int size = 0;
	
private:
	bool fastDR() {
#if defined(__AVR__)
		return bool(*_pin_reg & _bit_mask);
#else
		return digitalRead(_pin);
#endif
	}
	byte buffer[G433_BUFSIZE];
	bool prevState;
	byte parse = 0;
	uint32_t tmr = 0;
	byte bitCount = 0, byteCount = 0;
#if defined(__AVR__)
	volatile uint8_t *_pin_reg;
	volatile uint8_t _bit_mask;
#endif
};

void G433_crc_byte(uint8_t &crc, uint8_t data) {
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
	// обычный для всех остальных
	uint8_t i = 8;
	while (i--) {
		crc = ((crc ^ data) & 1) ? (crc >> 1) ^ 0x8C : (crc >> 1);
		data >>= 1;
	}
#endif
}

byte G433_crc(byte *buffer, byte size) {
	byte crc = 0;
	for (byte i = 0; i < size; i++) G433_crc_byte(crc, buffer[i]);
	return crc;
}
#endif