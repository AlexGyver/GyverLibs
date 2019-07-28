#ifndef GyverRTOS_h
#define GyverRTOS_h
#include <Arduino.h>
#include "LowPower.h"

/*
	GyverRTOS - система реального времени для Arduino
	Текущая версия: 1.0 от 15.04.2018
	Что умеет GyverRTOS, особенности:
    - Во время сна функция millis() не работает, вместо неё используется
    переменная mainTimer, которая автоматически увеличивается при каждом
    пробуждении на время сна (SLEEP_PERIOD) В ХОЛОСТОМ РЕЖИМЕ
    - Выполнение функций занимает время, поэтому ЕСЛИ ВЫПОЛНЯЕТСЯ ЗАДАЧА,
    время выполнения тоже автоматически суммируется в mainTimer
    - МЫ создаём несколько функций с разным периодом выполнения (задачи)
    - Настраиваем период пробуждения системы (минимально 15 мс)
    Далее всё автоматически:
    - Рассчитывается время до выполнения самой "ближней" задачи
    - Система периодически просыпается и считает таймеры
    - При наступлении времени выполнения ближайшей задачи, она выполняется.
    После этого снова выполняется расчёт времени до новой ближайшей задачи
    - Как итог: Ардуино спит (в зависимости от периодов) 99.999% времени,
    просыпаясь только для проверки флага и расчёта таймера
	
	AlexGyver, https://alexgyver.ru
*/

extern "C" {
  typedef void (*task)(void);
}

class GRTOS
{
  public:
	GRTOS();
	void setPeriod(uint8_t period);
	void attachList(task newList);
	void tick(uint32_t* buf);
	void wake(uint32_t* buf, uint32_t* mainTimer);
	void tickAndSleep(uint32_t* buf, uint32_t* mainTimer, period_t SLEEP_PERIOD);
	boolean ready;
	
  private:
	uint32_t _loopTime;
	uint32_t _period;
	int32_t _timer;
	task _taskList;
};

GRTOS::GRTOS() {}

void GRTOS::attachList(task newList) {
	_taskList = newList;
}

void GRTOS::setPeriod(uint8_t period) {
	ready = true;
	switch (period) {
		case 0: _period = 15;
		break;
		case 1: _period = 30;
		break;
		case 2: _period = 60;
		break;
		case 3: _period = 120;
		break;
		case 4: _period = 250;
		break;
		case 5: _period = 500;
		break;
		case 6: _period = 1000;
		break;
		case 7: _period = 2000;
		break;
		case 8: _period = 4000;
		break;
		case 9: _period = 8000;
		break;
	}
}

void GRTOS::tickAndSleep(uint32_t* buf, uint32_t* mainTimer, period_t SLEEP_PERIOD) {
	if (ready) {
		_loopTime = millis();
		*buf = 4294967295;
		_taskList();
		_timer = *buf;		
	}
	LowPower.powerDown(SLEEP_PERIOD, ADC_OFF, BOD_OFF);
	uint32_t step;
	if (ready) {
		step = (_period + (long)millis() - _loopTime);
		ready = false;
	}
	else step = _period;
	*mainTimer += step;
	_timer -= step;
	if (_timer < 0) ready = true;
}

void GRTOS::tick(uint32_t* buf) {
	if (ready) {
		_loopTime = millis();
		*buf = 4294967295;
		_taskList();
		_timer = *buf;		
	}
}

void GRTOS::wake(uint32_t* buf, uint32_t* mainTimer) {
	uint32_t step;
	if (ready) {
		step = (_period + (long)millis() - _loopTime);
		ready = false;
	}
	else step = _period;
	*mainTimer += step;
	_timer -= step;
	if (_timer < 0) ready = true;
}

class Gtask
{
  public:
	Gtask();	
	uint32_t tick(uint32_t* buf, uint32_t* mainTimer);
	void attachTask(task newTask);
	void attachTask(task newTask, uint32_t taskPeriod);
	void setTaskPeriod(uint32_t taskPeriod);
	void resetTimer(uint32_t* mainTimer);
	
  private:
	task _newTask;
	uint32_t _taskPeriod, _taskTimer;
};

Gtask::Gtask() {
	_newTask = NULL;
}

void Gtask::attachTask(task newTask) {
	_newTask = newTask;
}

void Gtask::attachTask(task newTask, uint32_t taskPeriod) {
	_newTask = newTask;
	_taskPeriod = taskPeriod;
}

void Gtask::setTaskPeriod(uint32_t taskPeriod) {
	_taskPeriod = taskPeriod;
}

void Gtask::resetTimer(uint32_t* mainTimer) {
	_taskTimer = *mainTimer;
}

uint32_t Gtask::tick(uint32_t* buf, uint32_t* mainTimer) {
	uint32_t time_left = (long)*mainTimer - _taskTimer;
	if (time_left > _taskPeriod) {
		_taskTimer = *mainTimer;
		_newTask();
		if (*buf > _taskPeriod)
			*buf = _taskPeriod;
	} else {
		if (*buf > (long)_taskPeriod - time_left)
			*buf = ((long)_taskPeriod - time_left);
	}
}

#endif