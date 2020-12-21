#ifndef softUART_h
#define softUART_h
// Однопроводной UART с асинхронным чтением и отправкой (+ не блокирует прерывания)
// БУФЕРА НА ПРИЁМ НЕТ!!!
// Для отправки и приёма нужно почащё дёргать за available() или tick(), здесь это синонимы
// Наследует класс Print, то есть можно отправлять всё что угодно, как через обычный Serial
// Улучшенная производительность для AVR Arduino

#define SOFTUART_TX_WAIT 50			// Таймаут ожидания наполнения буфера через write, мкс 

/*
	Интерфейс: UART. start бит 0, stop бит 1. Кодирование даты: HIGH - 0x1, LOW - 0x0
	
	1  _______     ___        ___     ______     ___     .........._________
			  |___|   |______|   |___|      |___|   |___|                    конец передачи
	0         start 0   1   2  3   4   5  6   7 stop start         stop     
*/

/*
	Инициализация:
	softUART<пин, тип> объект(скорость)
	- пин: номер пина
	- тип: GBUS_FULL - двухсторонняя связь (активен по умолчанию, можно не указывать)
	- тип: GBUS_TX - только отправка (экономит память)
	- тип: GBUS_RX - только приём (экономит память)
	- скорость: скорость в бодах
*/
// =============================================================================================
#define SOFTUART_BUF_SIZE 64 		// Стандартный размер буфера на отправку, байт
#include <Stream.h>

// КОНСТАНТЫ
#define GBUS_RX 0
#define GBUS_TX 1
#define GBUS_FULL 2

template <uint8_t _PIN, uint8_t _ROLE = GBUS_FULL>
class softUART : public Stream {
public:
	using Print::write;
	
	softUART(long baud, byte bufSize = SOFTUART_BUF_SIZE) : _bufSize(bufSize) {
#if defined(__AVR__)
		_port_reg = portOutputRegister(digitalPinToPort(_PIN));
		_pin_reg = portInputRegister(digitalPinToPort(_PIN));
		_ddr_reg = portModeRegister(digitalPinToPort(_PIN));
		_bit_mask = digitalPinToBitMask(_PIN);
		*_ddr_reg &= ~_bit_mask;	// INPUT
		*_port_reg |= _bit_mask;	// HIGH
#else
		pinMode(_PIN, INPUT_PULLUP);
#endif
		_bitTime = 1000000UL / baud;
		_bitTime2 = (uint32_t)_bitTime >> 1;
		_timeout = _bitTime * 10 * 10;	// таймаут как время передачи 10 байт
		if (_ROLE == GBUS_TX || _ROLE == GBUS_FULL) buffer = (byte *)malloc(_bufSize);
	}
	
	~softUART() {
		if (_ROLE == GBUS_TX || _ROLE == GBUS_FULL) free(buffer);
	}
	
	enum BUS_stage {
		BUS_IDLE,
		BUS_START,
		BUS_SENDING,
		BUS_READING,
	};
	
	bool isBusy() {		
		return !(micros() - _tmr > _timeout);
	}
	
	int tick() {return available();}
	
	virtual int available() {		
		if (_role == GBUS_RX) {								// приёмник
			if (_ROLE == GBUS_RX || _ROLE == GBUS_FULL) {	// компилятор вырежет при выбранной роли
#if defined(__AVR__)
				byte bit = (*_pin_reg & _bit_mask);
#else
				byte bit = digitalRead(_PIN);
#endif
				switch(_busStage) {
				case BUS_IDLE:
					if (!bit) {								// старт бит?
						_tmr = micros();
						_busStage = BUS_START;
					}
					break;
					
				case BUS_START:
					if (micros() - _tmr >= _bitTime2) {
						if (!bit) {							// да, старт бит
							_busStage = BUS_READING;		// начинаем приём
							_bitCount = 0;							
							_tmr += _bitTime2;				// ждём пол-фрейма							
						} else {							// ошибка
							_busStage = BUS_IDLE;							
						}				
					}
					break;
					
				case BUS_READING:
					if (micros() - _tmr >= _bitTime) {				// таймер
						_tmr += _bitTime;							// следующий фрейм
						if (_bitCount < 8) {						// чтение битов даты (0-7)
							bitWrite(_thisByte, _bitCount, bit);  	// пишем в буфер
						} else if (_bitCount == 8) {				// проверяем стоп бит
							if (!bit) _busStage = BUS_IDLE;         // не дождались стоп бита, конец приема
							else _readFlag = 1;						// стоп бит. Байт принят
						} else if (_bitCount == 9) {				// проверяем старт бит
							if (bit) _busStage = BUS_IDLE;          // не дождались нового старт бита, конец приема
							_bitCount = -1;          				// костыль
						}
						_bitCount++;								// следующий бит
					}
					break;					
				}				
			}
			return _readFlag;										// вернули 1 если байт собран
		} else {													// передатчик			
			if (_ROLE == GBUS_TX || _ROLE == GBUS_FULL) {			// компилятор вырежет при выбранной роли
				switch(_busStage) {
				case BUS_IDLE:
					if (_writeStart && micros() - _tmr > SOFTUART_TX_WAIT) {
						_writeStart = false;						
						_busStage = BUS_START;
					}
					break;
				case BUS_START:
					if (!isBusy()) {								// ждём окончания активности на линии
						_busStage = BUS_SENDING;
						_bitCount = -1;
						_byteCount = 0;						
						_tmr = micros();
					}
					break;
					
				case BUS_SENDING:
					if (micros() - _tmr >= _bitTime) {						
						byte bit;								
						if (_bitCount < 0) bit = 0;					// старт бит
						else if (_bitCount < 8) {					// передача даты
							bit = (buffer[_byteCount] >> _bitCount) & 1; // бит даты
						} else {
							bit = 1;								// стоп бит
							_byteCount++;							// след. байт							
							_bitCount = -2;							// костыль
						}
						// дрыг
#if defined(__AVR__)
						if (bit) {
							*_ddr_reg &= ~_bit_mask;	// INPUT
							*_port_reg |= _bit_mask;	// HIGH
						} else {
							*_ddr_reg |= _bit_mask;		// OUTPUT
							*_port_reg &= ~_bit_mask;	// LOW
						}
#else
						pinMode(_PIN, !bit);
						digitalWrite(_PIN, bit);
#endif
						_bitCount++;
						_tmr += _bitTime;							// таймер
						if (_byteCount == _txSize) {				// передача окончена
							_busStage = BUS_IDLE;
							if (_ROLE == GBUS_FULL) _role = GBUS_RX;// переключение на приёмник
							_tmr = micros();						// сброс таймера							
						}
					}
					break;
				}
			}
			return 0;		// в режиме передатчика возвращаем 0
		}
	}	
	
	virtual int read() {
		_readFlag = 0;
		return _thisByte;
	}
	
	virtual size_t write(uint8_t byte) {
		// КВМ (Костыль Вселенских Масштабов)
		// наполняем буфер по таймеру
		if (_ROLE == GBUS_TX || _ROLE == GBUS_FULL) {	// компилер вырежет, если ты приёмный			
			if (!_writeStart) {
				_writeStart = true;
				_txSize = 0;
			}
			if (_txSize < _bufSize) buffer[_txSize++] = byte;
			_tmr = micros();
			_role = GBUS_TX;
		}
	}
	
	virtual void flush() {}	
	int peek(){}
	
private:
	bool _writeStart = false;
	const byte _bufSize = SOFTUART_BUF_SIZE;
	byte *buffer; //byte buffer[(_ROLE == GBUS_TX || _ROLE == GBUS_FULL) ? SOFTUART_BUF_SIZE : 1];
	int8_t _bitCount = 0;
	byte _txSize = 0;
	byte _byteCount = 0;
	byte _thisByte;
	byte _readFlag = 0;	
	uint32_t _tmr;
	byte _role = GBUS_RX;
	BUS_stage _busStage = BUS_IDLE;
	uint16_t _bitTime;
	uint16_t _bitTime2;
	long _timeout;
	
#if defined(__AVR__)
	volatile uint8_t *_port_reg;
    volatile uint8_t *_pin_reg;
    volatile uint8_t *_ddr_reg;
    volatile uint8_t _bit_mask;
#endif
};
#endif