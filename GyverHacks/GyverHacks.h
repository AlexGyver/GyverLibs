#ifndef GyverHacks_h
#define GyverHacks_h
#include <Arduino.h>

/*
Текущая версия: 1.2 от 20.05.2018
GyverHacks - библиотека с некоторыми удобными хаками:
	GTimer - компактная альтернатива конструкции таймера с millis()
		Настройка периода вызова
		Сброс
	GFilterRA - компактная альтернатива фильтра бегущее среднее (Running Average)
		Настройка коэффициента фильтрации
		Настройка шага дискретизации (период фильтрафии) - встроенный таймер!
	medianFilter(a, b, c) - получить среднее из трёх (медианный фильтр)
	setPWMPrescaler(pin, prescaler) - настройка частоты ШИМ для атмеги328
	getVCC() - получить напряжение питания в милливольтах (мВ)
	getVoltage(pin) - получить напряжение на аналоговом пине с учётом реального питания (мВ)
	setConstant(voltage) - авто калибровка константы. В функцию подать напряжение питания в мВ (смотри пример)
	getTemp - получить примерную температуру ядра
*/

int getVCC();
void setConstant(uint16_t voltage);
int getVoltage(uint8_t pin);
float getTemp();

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

int medianFilter(int a, int b, int c);

class GFilterRA
{
  public:
	GFilterRA();	
	void setCoef(float coef);	    // чем меньше тем плавнее
	void setStep(uint16_t interval);
	float filteredTime(int16_t value);
	float filtered(int16_t value);
  private:
	float _coef, _lastValue;
	uint32_t _filterTimer;
	uint16_t _filterInterval;
};

void setPWMPrescaler(uint8_t pin, uint16_t prescale);

/*
   НАСТРОЙКА ЧАСТОТЫ ШИМ (частоты приведены для 16 МГц кварца)
   Ноги 5 и 6 (ВЛИЯЕТ НА millis() и delay() !!!)
   Константа    Делитель    Частота(Гц)
   0x01         1           62500
   0x02         8           7812
   0x03         64          976
   0x04         256         244
   0x05         1024        61

   Ноги 9 и 10 (ВЛИЯЕТ НА РАБОТУ SERVO !!!)
   Константа    Делитель    Частота(Гц)
   0x01         1           31250
   0x02         8           3906
   0x03         64          488
   0x04         256         122
   0x05         1024        30

   Ноги 3 и 11
   Константа    Делитель    Частота(Гц)
   0x01         1           31250
   0x02         8           3906
   0x03         32          976
   0x04         64          488
   0x05         128         244
   0x06         256         122
   0x07         1024        30
   
   - Pins 3 and 11 are paired on timer0  8bit (Default prescale=64, Freq=977Hz)
   - Pins 9 and 10 are paired on timer1 16bit (Default prescale=64, Freq=490Hz)
   - Pins 6 and 13 are paired on timer4 10bit (default prescale=64, Freq=490Hz)
   - Pins 5 is exclusivly     on timer3 16bit (Default prescale=64, Freq=490Hz)
*/


#endif