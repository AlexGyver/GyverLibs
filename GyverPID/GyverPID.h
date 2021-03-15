#ifndef GyverPID_h
#define GyverPID_h

/*
	GyverPID - библиотека классического PID регулятора для Arduino
	Документация: https://alexgyver.ru/gyverpid/
	- Время одного расчёта около 70 мкс
	- Режим работы по величине или по её изменению (для интегрирующих процессов)
	- Возвращает результат по встроенному таймеру или в ручном режиме
	
	Версия 1.1 - убраны дефайны
	Версия 1.2 - возвращены дефайны
	Версия 1.3 - вычисления ускорены, библиотека облегчена
	Версия 2.0 - логика работы чуть переосмыслена, код улучшен, упрощён и облегчён
	Версия 2.1 - integral вынесен в public
	Версия 2.2 - оптимизация вычислений
	Версия 2.3 - добавлен режим PID_INTEGRAL_WINDOW
	Версия 2.4 - реализация внесена в класс
	Версия 3.0
		- Добавлен режим оптимизации интегральной составляющей (см. доку)
		- Добавлены автоматические калибровщики коэффициентов (см. примеры и доку)
	Версия 3.1 - исправлен режиме ON_RATE, добавлено автоограничение инт. суммы
	Версия 3.2 - чуть оптимизации, добавлена getResultNow
	Версия 3.3 - в тюнерах можно передать другой обработчик класса Stream для отладки
*/

#include <Arduino.h>

#if defined(PID_INTEGER)	// расчёты с целыми числами
typedef int datatype;
#else						// расчёты с float числами
typedef float datatype;
#endif

#define NORMAL 0
#define REVERSE 1
#define ON_ERROR 0
#define ON_RATE 1

class GyverPID {
public:
	// ==== datatype это float или int, в зависимости от выбранного (см. пример integer_calc) ====
	GyverPID(){}
	
	// kp, ki, kd, dt
	GyverPID(float new_kp, float new_ki, float new_kd, int16_t new_dt = 100) {
		setDt(new_dt);
		Kp = new_kp;
		Ki = new_ki;
		Kd = new_kd;
	}	

	// направление регулирования: NORMAL (0) или REVERSE (1)
	void setDirection(boolean direction) {				
		_direction = direction;
	}
	
	// режим: работа по входной ошибке ON_ERROR (0) или по изменению ON_RATE (1)
	void setMode(boolean mode) {						
		_mode = mode;
	}
	
	// лимит выходной величины (например для ШИМ ставим 0-255)
	void setLimits(int min_output, int max_output) {	
		_minOut = min_output;
		_maxOut = max_output;
	}
	
	// установка времени дискретизации (для getResultTimer)
	void setDt(int16_t new_dt) {						
		_dt_s = new_dt / 1000.0f;
		_dt = new_dt;
	}
	
	datatype setpoint = 0;		// заданная величина, которую должен поддерживать регулятор
	datatype input = 0;			// сигнал с датчика (например температура, которую мы регулируем)
	datatype output = 0;		// выход с регулятора на управляющее устройство (например величина ШИМ или угол поворота серво)
	float Kp = 0.0;				// коэффициент P
	float Ki = 0.0;				// коэффициент I
	float Kd = 0.0;				// коэффициент D	
	float integral = 0.0;		// интегральная сумма
	
	// возвращает новое значение при вызове (если используем свой таймер с периодом dt!)
	datatype getResult() {
		datatype error = setpoint - input;			// ошибка регулирования
		datatype delta_input = prevInput - input;	// изменение входного сигнала за dt
		prevInput = input;							// запомнили предыдущее
		if (_direction) {							// смена направления
			error = -error;
			delta_input = -delta_input;
		}
		output = _mode ? 0 : (error * Kp); 			// пропорциональая составляющая
		output += delta_input * Kd / _dt_s;			// дифференциальная составляющая

#if (PID_INTEGRAL_WINDOW > 0)
		// ЭКСПЕРИМЕНТАЛЬНЫЙ РЕЖИМ ИНТЕГРАЛЬНОГО ОКНА
		if (++t >= PID_INTEGRAL_WINDOW) t = 0; 		// перемотка t
		integral -= errors[t];     					// вычитаем старое	
		errors[t] = error * Ki * _dt_s;  			// запоминаем в массив
		integral += errors[t];         				// прибавляем новое
#else		
		integral += error * Ki * _dt_s;				// обычное суммирование инт. суммы
#endif	

#ifdef PID_OPTIMIZED_I
		// ЭКСПЕРИМЕНТАЛЬНЫЙ РЕЖИМ ОГРАНИЧЕНИЯ ИНТЕГРАЛЬНОЙ СУММЫ
		output = constrain(output, _minOut, _maxOut);
		if (Ki != 0) integral = constrain(integral, (_minOut - output) / (Ki * _dt_s), (_maxOut - output) / (Ki * _dt_s));
#endif

		if (_mode) integral += delta_input * Kp;			// режим пропорционально скорости
		integral = constrain(integral, _minOut, _maxOut);	// ограничиваем инт. сумму
		output += integral;									// интегральная составляющая
		output = constrain(output, _minOut, _maxOut);		// ограничиваем выход
		return output;
	}
	
	// возвращает новое значение не ранее, чем через dt миллисекунд (встроенный таймер с периодом dt)
	datatype getResultTimer() {
		if (millis() - pidTimer >= _dt) {
			pidTimer = millis();
			getResult();
		}
		return output;
	}
	
	// посчитает выход по реальному прошедшему времени между вызовами функции
	datatype getResultNow() {
		setDt(millis() - pidTimer);
		pidTimer = millis();
		return getResult();
	}
	
private:
	int16_t _dt = 100;		// время итерации в мс
	float _dt_s = 0.1;		// время итерации в с
	boolean _mode = 0, _direction = 0;
	int _minOut = 0, _maxOut = 255;	
	datatype prevInput = 0;	
	uint32_t pidTimer = 0;
#if (PID_INTEGRAL_WINDOW > 0)
	datatype errors[PID_INTEGRAL_WINDOW];
	int t = 0;	
#endif
};
#endif