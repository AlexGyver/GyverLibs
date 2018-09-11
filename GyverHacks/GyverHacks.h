#ifndef GyverHacks_h
#define GyverHacks_h
#include <Arduino.h>
#define LIBRARY_VERSION	1.5

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#define HZ_15 0
#define HZ_61 1
#define HZ_244 2
#define KHZ_2 3
#define KHZ_16 4

/*
Текущая версия: 1.5 от 12.09.2018
	GyverHacks - библиотека с некоторыми удобными хаками:
		GTimer - компактная альтернатива конструкции таймера с millis()
			Класс GTimer (period) - установка периода в миллисекундах
			setInterval(period) - изменение периода вызова
			isReady() - возвращает true, когда таймер сработал. Возвращает false до следующего срабатывания
			reset() - сброс
		GParsingStream - парсинг данных из Serial
			parsingStream((int*)&intData) - автоматическая расфасовка пакетов вида $110 25 600 920; в массив intData
			dataReady() - функция-флаг принятия нового пакета данных
			sendPacket((int*)&intData, sizeof(intData)) - функция отправки в порт пакета вида $110 25 600 920; из массива
		Меняем частоту ШИМ
			setPWM10bit(mode) - настройка частоты ШИМ для 9 и 10 пинов (timer 1) на 10 бит (analogWrite 0 - 1023). mode:
				HZ_15: частота 15,26 Гц
				HZ_61: частота 61,04 Гц
				HZ_244: частота 244,14 Гц
				KHZ_2: частота 1 953,13 Гц
				KHZ_16: частота 15 625 Гц
			setPWMPrescaler(pin, prescaler) - установка частоты ШИМ для разных пинов (смотри пример!)
		Дополнительно - несколько клёвых удобных функций
			getVCC() - получить напряжение питания в милливольтах, т.е. опорное напряжение. Например с банки лития
			getVoltage(pin) - получить напряжение на аналоговом пине с учётом реального напряжения питания			
			getConstant(voltage) - авто калибровка константы (слегка различается в разных партиях микроконтроллеров). В функцию подать напряжение питания в мВ (смотри пример!!)
			setConstant(constant) - установить константу для дальнейшего использования
			getTemp() - получить примерную температуру ядра процессора
*/

class GTimer
{
  public:
	GTimer();
	GTimer(uint16_t interval);
	void setInterval(uint16_t interval);
	boolean isReady();
	void reset();
  private:
	uint32_t _timer;
	uint16_t _interval;
};

int getVCC();
void setConstant(float constant);
float getConstant(uint16_t voltage);
int getVoltage(uint8_t pin);
float getTemp();
void setPWMPrescaler(uint8_t pin, uint16_t prescale);
void setPWM10bit(uint8_t mode);
void fastAnalogWrite(uint8_t pin, uint8_t duty);

/*
   НАСТРОЙКА ЧАСТОТЫ ШИМ (частоты приведены для 16 МГц кварца)
   Ноги 5 и 6, Timer 0 (ВЛИЯЕТ НА millis() и delay() !!!)
   Константа    Делитель    Частота(Гц)
   0x01         1           62500
   0x02         8           7812
   0x03         64          976
   0x04         256         244
   0x05         1024        61

   Ноги 9 и 10, Timer 1 (ВЛИЯЕТ НА РАБОТУ SERVO !!!)
   Константа    Делитель    Частота(Гц)
   0x01         1           31250
   0x02         8           3906
   0x03         64          488
   0x04         256         122
   0x05         1024        30

   Ноги 3 и 11, Timer 2
   Константа    Делитель    Частота(Гц)
   0x01         1           31250
   0x02         8           3906
   0x03         32          976
   0x04         64          488
   0x05         128         244
   0x06         256         122
   0x07         1024        30
*/

/*
ИЛИ
Для того, чтобы задать нужный режим ШИМ на выводах 9 и 10 (Timer 1), 
необходимо разместить две соответствующие строки в функцию setup()

8 бит, 62 500 Гц 
TCCR1A = TCCR1A & 0xe0 | 1;
TCCR1B = TCCR1B & 0xe0 | 0x09; 

8 бит, 7 812,5 Гц 
TCCR1A = TCCR1A & 0xe0 | 1;
TCCR1B = TCCR1B & 0xe0 | 0x0a;

8 бит, 976,56 Гц 
TCCR1A = TCCR1A & 0xe0 | 1;
TCCR1B = TCCR1B & 0xe0 | 0x0b; 

8 бит, 244,14 Гц 
TCCR1A = TCCR1A & 0xe0 | 1;
TCCR1B = TCCR1B & 0xe0 | 0x0c; 

8 бит, 61,04 Гц 
TCCR1A = TCCR1A & 0xe0 | 1;
TCCR1B = TCCR1B & 0xe0 | 0x0d; 

9 бит, 31 250 Гц 
TCCR1A = TCCR1A & 0xe0 | 2;
TCCR1B = TCCR1B & 0xe0 | 0x09; 

9 бит, 3 906,25 Гц 
TCCR1A = TCCR1A & 0xe0 | 2;
TCCR1B = TCCR1B & 0xe0 | 0x0a; 

9 бит, 488,28 Гц 
TCCR1A = TCCR1A & 0xe0 | 2;
TCCR1B = TCCR1B & 0xe0 | 0x0b; 

9 бит, 122,07 Гц 
TCCR1A = TCCR1A & 0xe0 | 2;
TCCR1B = TCCR1B & 0xe0 | 0x0c; 

9 бит, 30,52 Гц 
TCCR1A = TCCR1A & 0xe0 | 2;
TCCR1B = TCCR1B & 0xe0 | 0x0d; 

10 бит, 15 625 Гц 
TCCR1A = TCCR1A & 0xe0 | 3;
TCCR1B = TCCR1B & 0xe0 | 0x09; 

10 бит, 1 953,13 Гц 
TCCR1A = TCCR1A & 0xe0 | 3;
TCCR1B = TCCR1B & 0xe0 | 0x0a; 

10 бит, 244,14 Гц 
TCCR1A = TCCR1A & 0xe0 | 3;
TCCR1B = TCCR1B & 0xe0 | 0x0b; 

10 бит, 61,04 Гц 
TCCR1A = TCCR1A & 0xe0 | 3;
TCCR1B = TCCR1B & 0xe0 | 0x0c; 

10 бит, 15,26 Гц
TCCR1A = TCCR1A & 0xe0 | 3;
TCCR1B = TCCR1B & 0xe0 | 0x0d;
*/


#endif