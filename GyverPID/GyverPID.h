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
	- Время одного расчёта около 50 мкс
	- Режим работы по величине или по её изменению (для интегрирующих процессов)
	- Возвращает результат по встроенному таймеру или в ручном режиме
	
	Версия 1.1 - убраны дефайны
	Версия 1.2 - возвращены дефайны
	Версия 1.3 - вычисления ускорены, библиотека облегчена
*/

class GyverPID {
public:
	// ==== datatype это float или int, в зависимости от выбранного (см. пример integer_calc) ====
	GyverPID();
	GyverPID(float new_kp, float new_ki, float new_kd, int16_t new_dt = 100);		// kp, ki, kd, dt
	
	datatype setpoint = 0;		// заданная величина, которую должен поддерживать регулятор
	datatype input = 0;			// сигнал с датчика (например температура, которую мы регулируем)
	datatype output = 0;		// выход с регулятора на управляющее устройство (например величина ШИМ или угол поворота серво)
	
	datatype getResult();												// возвращает новое значение при вызове (если используем свой таймер с периодом dt!)	
	datatype getResult(datatype new_setpoint, datatype new_input);		// принимает setpoint и input
	
	datatype getResultTimer();											// возвращает новое значение не ранее, чем через dt миллисекунд (встроенный таймер с периодом dt)
	datatype getResultTimer(datatype new_setpoint, datatype new_input);	// тож самое, но принимает setpoint и input
	
	void setDirection(uint8_t direction);						// направление регулирования: NORMAL (0) или REVERSE (1)
	void setMode(uint8_t mode);									// режим: работа по входной ошибке ON_ERROR (0) или по изменению ON_RATE (1)
	void setLimits(int min_output, int max_output);				// лимит выходной величины (например для ШИМ ставим 0-255)
	void setDt(int16_t new_dt);									// установка времени дискретизации (для getResultTimer)
	void tune(float new_kp, float new_ki, float new_kd);		// перенастройка коэффициентов (П, И, Д)
	
private:
	float _kp = 0.0;
	float _ki = 0.0;
	float _kd = 0.0;
	int16_t _dt = 0;
	boolean _mode = 0, _direction = 0;	
	int _minOut = 0, _maxOut = 255;	
	float dt_s = 0.0;
	float kd_dt_s = 0;
	float ki_dt_s = 0;
	float prevInput = 0.0;
	float integral = 0.0;
	uint32_t pidTimer = 0;
};

GyverPID::GyverPID() {}

GyverPID::GyverPID(float new_kp, float new_ki, float new_kd, int16_t new_dt) {
	_dt = new_dt;
	pidTimer = (long)millis() + _dt;
	tune(new_kp, new_ki, new_kd);
}

void GyverPID::setDirection(uint8_t direction) {
	_direction = direction;
	tune(_kp, _ki, _kd);
}

void GyverPID::setMode(uint8_t mode) {
	_mode = mode;
}

void GyverPID::setLimits(int min_output, int max_output) {	
	_minOut = min_output;
	_maxOut = max_output;
}

void GyverPID::setDt(int16_t new_dt) {	
	_dt = new_dt;
	dt_s = _dt / 1000.0;
	kd_dt_s = _kd / dt_s;
	ki_dt_s = _ki * dt_s;
}

void GyverPID::tune(float new_kp, float new_ki, float new_kd) {
	int8_t sign = _direction ? -1 : 1;
	_kp = new_kp * sign;
	_ki = new_ki * sign;
	_kd = new_kd * sign;	
	setDt(_dt);
}

datatype GyverPID::getResult() {
	datatype error = setpoint - input;				// ошибка регулирования
	datatype delta_input = input - prevInput;		// изменение входного сигнала
	prevInput = input;
	
	output = 0;	
	if (!_mode) output += (float)error * _kp;	// пропорционально ошибке регулирования
	else output -= (float)delta_input * _kp;	// пропорционально изменению входного сигнала
	output -= (float)delta_input * kd_dt_s;		// дифференциальная составляющая	
	integral += (float)error * ki_dt_s;			// расчёт интегральной составляющей
	output += integral;							// прибавляем
	
	output = constrain(output, _minOut, _maxOut);    // ограничиваем
	return output;
}
/*
	// неоптимизированный вариант
	float error = setpoint - input;				// ошибка регулирования
	float delta_input = input - prevInput;		// изменение входного сигнала
	prevInput = input;
	
	output = 0;	
	if (!_mode) output += (float)error * kp;	// пропорционально ошибке регулирования
	else output -= (float)delta_input * kp;		// пропорционально изменению входного сигнала
	output -= (float)kd * delta_input / dt_s;	// дифференциальная составляющая	
	integral += (float)error * ki * dt_s;		// расчёт интегральной составляющей
	output += integral;							// прибавляем
	
	output = constrain(output, _minOut, _maxOut);    // ограничиваем
	return output;
*/
datatype GyverPID::getResult(datatype new_setpoint, datatype new_input) {
	setpoint = new_setpoint;
	input = new_input;
	return GyverPID::getResult();
}

datatype GyverPID::getResultTimer() {
	if ((long)millis() - pidTimer >= _dt) {
		pidTimer = millis();
		return GyverPID::getResult();
	} else {
		return output;
	}
}

datatype GyverPID::getResultTimer(datatype new_setpoint, datatype new_input) {
	setpoint = new_setpoint;
	input = new_input;
	return GyverPID::getResultTimer();
}