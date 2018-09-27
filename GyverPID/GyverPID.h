#ifndef GyverPID_h
#define GyverPID_h
#include <Arduino.h>

#define NORMAL 0
#define REVERSE 1
#define ON_ERROR 0
#define ON_RATE 1

/*
	GyverPID - библиотека классического PID регулятора для Arduino
	- Время одного расчёта около 90 мкс
	- Режим работы по величине или по её изменению (для интегрирующих процессов)
	- Возвращает результат по встроенному таймеру или в ручном режиме
*/

class GyverPID
{
  public:
	GyverPID();
    GyverPID(float new_kp, float new_ki, float new_kd);						// kp, ki, kd
	GyverPID(float new_kp, float new_ki, float new_kd, int16_t new_dt);		// kp, ki, kd, dt
	
	float setpoint;		// заданная величина, которую должен поддерживать регулятор
	float input;		// сигнал с датчика (например температура, которую мы регулируем)
	float output;		// выход с регулятора на управляющее устройство (например величина ШИМ или угол поворота серво)
	
	float getResult();											// возвращает новое значение при вызове (если используем свой таймер с периодом dt!)	
	float getResult(float new_setpoint, float new_input);		// принимает setpoint и input
	
	float getResultTimer();										// возвращает новое значение не ранее, чем через dt миллисекунд (встроенный таймер с периодом dt)
	float getResultTimer(float new_setpoint, float new_input);	// тож самое, но принимает setpoint и input
		
	void setDirection(boolean direction);						// направление регулирования: NORMAL (0) или REVERSE (1)
	void setMode(boolean mode);									// режим: работа по входной ошибке ON_ERROR (0) или по изменению ON_RATE (1)
	void setLimits(float min_output, float max_output);			// лимит выходной величины (например для ШИМ ставим 0-255)
	void setDt(int16_t new_dt);									// установка времени дискретизации (для getResultTimer)
	
	float kp;
	float ki;
	float kd;
		
  private:	
	int16_t dt;
	boolean _mode = 0, _direction = 0, _prev_direction = 0;	
	float _minOut = 0, _maxOut = 255;	
	float dt_s;
	float prevInput;
	float integral;
	uint32_t pidTimer;
};

#endif