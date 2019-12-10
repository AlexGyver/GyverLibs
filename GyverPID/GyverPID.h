#pragma once
#include <Arduino.h>

#if defined(PID_INTEGER)
typedef int datatype;
// расчёты с целыми числами
#elif defined(PID_FLOAT)
typedef float datatype;
// расчёты с float числами
#else
typedef float datatype;
#endif

#define NORMAL 0
#define REVERSE 1
#define ON_ERROR 0
#define ON_RATE 1

/*
	GyverPID - библиотека классического PID регулятора для Arduino
	- Время одного расчёта около 70 мкс
	- Режим работы по величине или по её изменению (для интегрирующих процессов)
	- Возвращает результат по встроенному таймеру или в ручном режиме
	
	Версия 1.1 - убраны дефайны
	Версия 1.2 - возвращены дефайны
	Версия 1.3 - вычисления ускорены, библиотека облегчена
	Версия 2.0 - логика работы чуть переосмыслена, код улучшен, упрощён и облегчён
	Версия 2.1 - integral вынесен в public
*/

class GyverPID {
public:
	// ==== datatype это float или int, в зависимости от выбранного (см. пример integer_calc) ====
	GyverPID();
	GyverPID(float new_kp, float new_ki, float new_kd, int16_t new_dt = 100);		// kp, ki, kd, dt
	
	datatype setpoint = 0;		// заданная величина, которую должен поддерживать регулятор
	datatype input = 0;			// сигнал с датчика (например температура, которую мы регулируем)
	datatype output = 0;		// выход с регулятора на управляющее устройство (например величина ШИМ или угол поворота серво)
	
	datatype getResult();		// возвращает новое значение при вызове (если используем свой таймер с периодом dt!)
	datatype getResultTimer();	// возвращает новое значение не ранее, чем через dt миллисекунд (встроенный таймер с периодом dt)

	void setDirection(boolean direction);				// направление регулирования: NORMAL (0) или REVERSE (1)
	void setMode(boolean mode);							// режим: работа по входной ошибке ON_ERROR (0) или по изменению ON_RATE (1)
	void setLimits(int min_output, int max_output);		// лимит выходной величины (например для ШИМ ставим 0-255)
	void setDt(int16_t new_dt);							// установка времени дискретизации (для getResultTimer)
	float Kp = 0.0;
	float Ki = 0.0;
	float Kd = 0.0;	
	float integral = 0.0;
	
private:
	int16_t _dt = 100;		// время итерации в мс
	float _dt_s = 0.1;		// время итерации в с
	boolean _mode = 0, _direction = 0;
	int _minOut = 0, _maxOut = 255;	
	datatype prevInput = 0;	
	uint32_t pidTimer = 0;
};

// ================== CONSTRUCTOR ===================
GyverPID::GyverPID() {}

GyverPID::GyverPID(float new_kp, float new_ki, float new_kd, int16_t new_dt) {
	setDt(new_dt);
	Kp = new_kp;
	Ki = new_ki;
	Kd = new_kd;
}

// ================== SETTINGS ===================
void GyverPID::setDirection(boolean direction) {
	_direction = direction;
	setDt(_dt);
}

void GyverPID::setMode(boolean mode) {
	_mode = mode;
}

void GyverPID::setLimits(int min_output, int max_output) {	
	_minOut = min_output;
	_maxOut = max_output;
}

void GyverPID::setDt(int16_t new_dt) {
	_dt_s = new_dt / 1000.0f * (_direction ? -1 : 1);
	_dt = new_dt;
}

// ================== COMPUTE ===================
datatype GyverPID::getResult() {
	datatype error = setpoint - input;				// ошибка регулирования
	datatype delta_input = input - prevInput;		// изменение входного сигнала
	prevInput = input;
	
	output = 0;	
	if (!_mode) output += (float)error * (_direction ? -Kp : Kp);	// пропорционально ошибке регулирования
	else output -= (float)delta_input * (_direction ? -Kp : Kp);	// пропорционально изменению входного сигнала
	output -= (float)delta_input * Kd / _dt_s;						// дифференциальная составляющая	
	integral += (float)error * Ki * _dt_s;							// расчёт интегральной составляющей
	output += integral;												// прибавляем
	
	output = constrain(output, _minOut, _maxOut);    // ограничиваем
	return output;
}

datatype GyverPID::getResultTimer() {
	if (millis() - pidTimer >= _dt) {
		pidTimer = millis();
		return GyverPID::getResult();
	} else {
		return output;
	}
}