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
	- Версия 2.1: небольшие фиксы и добавления
	- Версия 2.2: оптимизация
	- Версия 2.3: добавлена поддержка esp (исправлены ошибки)
		
	Документация: https://alexgyver.ru/gyvermotor/
	AlexGyver, 2020
*/

#define _SMOOTH_PRD 50	// таймер smoothTick, мс

enum GM_driverType {
	DRIVER2WIRE,
	DRIVER3WIRE,
	RELAY2WIRE,
};

enum GM_dir {
	NORMAL,
	REVERSE,
};

enum GM_workMode {
	FORWARD,
	BACKWARD,
	STOP,
	BRAKE,
	AUTO = 0,
};

static const int8_t _GM_NC = -1;	// not connected

class GMotor {
public:
	GMotor(GM_driverType type, int8_t param1 = _GM_NC, int8_t param2 = _GM_NC, int8_t param3 = _GM_NC, int8_t param4 = _GM_NC);
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
	// AUTO - подчиняется setSpeed (-255.. 255)
	void setMode(GM_workMode mode);
	
	// направление вращения	
	// NORM - обычное
	// REVERSE - обратное
	void setDirection(GM_dir direction);
	
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
	
	// плавное изменение к указанной скорости (к значению ШИМ)
	void smoothTick(int16_t duty);
	
	// скорость изменения скорости
	void setSmoothSpeed(uint8_t speed);	
	
	// возвращает -1 при вращении BACKWARD, 1 при FORWARD и 0 при остановке и торможении
	int getState();

	// внутренняя переменная скважности для отладки
	int16_t _duty = 0;
	
protected:
	void setPins(bool a, bool b, int c);	
	void run(GM_workMode mode, int16_t duty = 0);		// дать прямую команду мотору (без смены режима)
	
	int _minDuty = 0, _state = 0;;
	int8_t _digA = _GM_NC, _digB = _GM_NC, _pwmC = _GM_NC;
	bool _direction = false;
	int8_t _resolution = 0, _level = HIGH;
	int _maxDuty = 254;
	GM_workMode _mode = FORWARD, _lastMode = FORWARD;
	GM_driverType _type;
	uint16_t _deadtime = 0;
	uint8_t _speed = 20;
	uint32_t _tmr = 0;
};