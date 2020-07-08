#pragma once
#include <Arduino.h>

/*
	Библиотека для удобного управления моторчиками через драйвер
	- Контроль скорости и направления вращения
	- Работа с 10 битным ШИМом
	- Версия 1.1 - убраны дефайны
	- Версия 1.2 - возвращены дефайны
	- Версия 2.0:
		- Программный deadtime
		- Отрицательные скорости
		- Поддержка двух типов драйверов и реле
		- Плавный пуск и изменение скорости
		
	Документация: https://alexgyver.ru/gyvermotor/
	AlexGyver, 2020
*/

enum driverType {
	DRIVER2WIRE,
	DRIVER3WIRE,
	RELAY2WIRE,
};

enum dir {
	NORMAL,
	REVERSE,
};

enum workMode {
	FORWARD,
	BACKWARD,
	STOP,
	BRAKE,
};

static const int8_t NC = -1;	// not connected

class GMotor {
public:
	GMotor(driverType type, int8_t param1 = NC, int8_t param2 = NC, int8_t param3 = NC, int8_t param4 = NC);
	// три варианта создания объекта в зависимости от драйвера:
	// GMotor motor(DRIVER2WIRE, dig_pin, PWM_pin, (LOW/HIGH) )
	// GMotor motor(DRIVER3WIRE, dig_pin_A, dig_pin_B, PWM_pin, (LOW/HIGH) )
	// GMotor motor(RELAY2WIRE, dig_pin_A, dig_pin_B, (LOW/HIGH) )
	
	// установка скорости 0-255 (8 бит) и 0-1023 (10 бит)
	void setSpeed(int16_t duty);			
	
	// сменить режим работы мотора:	
	// FORWARD - вперёд
	// BACKWARD - назад
	// STOP - остановить
	// BRAKE - активное торможение
	void setMode(workMode mode);
	
	// направление вращения	
	// NORM - обычное
	// REVERSE - обратное
	void setDirection(dir direction);
	
	// установить минимальную скважность (при которой мотор начинает крутиться)
	void setMinDuty(int duty);
	
	// установить выход в 8 бит
	void set8bitMode();		

	// установить выход в 10 бит
	void set10bitMode();					
	
	// установить deadtime (в микросекундах). По умолч 0
	void setDeadtime(uint16_t deadtime);	
	
	// установить уровень драйвера (по умолч. HIGH)
	void setLevel(int8_t level);			
	
	// плавное изменение к указанной скорости
	void smoothTick(int16_t duty);
	
	// скорость изменения скорости
	void setSmoothSpeed(uint8_t speed);
	
	// дать прямую команду мотору (без смены режима)
	void run(workMode mode, int16_t duty);	

	// внутренняя переменная скважности для отладки
	int16_t _duty = 0;
	
private:
	void setPins(bool a, bool b, int c);
	int _minDuty = 0;
	int8_t _digA = NC, _digB = NC, _pwmC = NC;
	bool _direction = false;
	int8_t _resolution = 0, _level = HIGH;
	workMode _mode = 0, _lastMode = 0;
	driverType _type = 0;	
	uint16_t _deadtime = 0;
	uint8_t _speed = 0;
	uint32_t _tmr = 0;
};