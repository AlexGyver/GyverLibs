#ifndef _GyverRelay_lib_H
#define _GyverRelay_lib_H

#pragma once
#include <Arduino.h>

/*
	GyverRelay - библиотека классического релейного регулятора для Arduino
	Документация: https://alexgyver.ru/gyverrelay/
	- Обратная связь по скорости изменения величины
	- Настройка гистерезиса, коэффициента усиления ОС, направления регулирования
	- Возвращает результат по встроенному таймеру или в ручном режиме
	
	Версия 2.0 от 04.12.2019
	Версия 2.1 от 31.01.2020 - исправлена getResultTimer
*/

#define NORMAL 0
#define REVERSE 1

class GyverRelay
{
public:
	// принимает установку, ширину гистерезиса, направление (NORMAL, REVERSE)
	// NORMAL - включаем нагрузку при переходе через значение снизу (пример: охлаждение)
	// REVERSE - включаем нагрузку при переходе через значение сверху (пример: нагрев)
	GyverRelay(boolean direction = REVERSE);
	
	// расчёт возвращает состояние для управляющего устройства (реле, транзистор) (1 вкл, 0 выкл)
	boolean compute(float dt = 0);		// моментальный расчёт. Принимает dt в секундах для режима с ОС
	boolean getResult();				// моментальный расчёт. Встроенный таймер для режима с ОС
	boolean getResultTimer();			// расчёт по встроенному таймеру

	void setDirection(boolean dir);		// направление регулирования (NORMAL, REVERSE)
	
	float input = 0;					// сигнал с датчика (например температура, которую мы регулируем)
	float setpoint = 0;					// заданная величина, которую должен поддерживать регулятор (температура)
	boolean output = 0;					// выход регулятора (0 или 1)
	
	float hysteresis = 0;				// ширина окна гистерезиса
	float k = 0;						// коэффициент усиления	по скорости (по умолч. 0)	
	int16_t dT = 1000;					// время итерации, мс (по умолч. секунда)
	
private:	
	uint32_t prevTime = 0;
	float prevInput = 0.0;
	boolean _direction = false;
};

#endif // _GyverRelay_lib_H