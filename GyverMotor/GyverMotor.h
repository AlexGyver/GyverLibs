#ifndef GyverMotor_h
#define GyverMotor_h
#include <Arduino.h>

/*
	Библиотека для удобного управления моторчиками через драйвер
	- Контроль скорости и направления вращения
	- Встроенный инструмент для настройки частоты ШИМ
	- Работа с 10 битным ШИМом
	- Версия 1.1 - убраны дефайны
*/

enum modes {
	NORM,
	REVERSE,
	FORWARD = 0,
	BACKWARD,
	STOP,
};


void PWM10bit();									// установка пинов 9 и 10 в режим 10 бит (управляется сигнало 0-1023)

void PWMfrequency(uint8_t pin, uint16_t mode);		// установка частоты ШИМ на пине
// пины 5 и 6 	8 bit	mode: 1 (62 500 Гц), 2 (7 812 Гц), 3 (976 Гц), 4 (244 Гц), 5 (61 Гц). ВЛИЯЕТ НА РАБОТУ millis() и delay()
// пины 9 и 10 	8 bit 	mode: 1 (62 500 Гц), 2 (7 812 Гц), 3 (976 Гц), 4 (244 Гц), 5 (61 Гц). ВЛИЯЕТ НА РАБОТУ servo
// пины 9 и 10 	10 bit 	mode: 1 (15 625 Гц), 2 (1 953 Гц), 3 (244 Гц), 4 (61 Гц),  5 (15 Гц). ВЛИЯЕТ НА РАБОТУ servo
// пины 3 и 11 	8 bit	mode: 1 (31 250 Гц), 2 (3 906 Гц), 3 (976 Гц), 4 (488 Гц), 5 (244 Гц), 6 (122 Гц), 7 (30 Гц). ВЛИЯЕТ НА РАБОТУ tone()

class GMotor
{
public:
	GMotor(uint8_t dig_pin, uint8_t pwm_pin);	// создаём объект. 
	// dig_pin - пин направления, любой пин
	// pwm_pin - ШИМ пин (у NANO/UNO/MINI это 3, 5, 6, 9, 10, 11)
	
	void setSpeed(uint8_t duty);				// установка скорости (0-255)
	
	void setSpeed10bit(uint16_t duty);			// установка скорости в режиме 10 бит (0-1023) для пинов 9 и 10
	
	void setMode(modes mode);					// режим работы мотора:
	// FORWARD - вперёд
	// BACKWARD - назад
	// STOP - остановить												
	
	void setDirection(modes direction);		// направление вращения (один раз настроить в setup вместо переподключения мотора)
	// NORM - обычное
	// REVERSE - обратное
	
private:
	void runFrw();
	void runBkw();
	void runFrw10bit();
	void runBkw10bit();
	uint8_t _dig_pin = 0, _pwm_pin = 0, _mode = 0, _reverse = 0;
	int16_t _duty = 0;
};

#endif