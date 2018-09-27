#ifndef GyverPID_h
#define GyverPID_h
#include <Arduino.h>

/*
 Integrating Processes - процесс, где выход с регулятора 
 меняет скорость изменения входной величины. Например изменяем 
 величину скважности ШИМ для движка. Скорость движка уменьшится 
 и перестанет уменьшаться, дойдя до какого-то предела.
 А вот если мы измеряем линейное перемещение от движка - изменится 
 скорость её изменения! Это и есть Integrating Processes
*/

#define NORMAL 0
#define REVERSE 1
#define ON_ERROR 0
#define ON_RATE 1

class GyverPID
{
  public:
    GyverPID(float, float, float);			// kp, ki, kd
	GyverPID(float, float, float, int16_t);	// kp, ki, kd, dt
	float setpoint;
	float input;
	float output;
	float getResult();					// возвращает новое значение при вызове (если используем свой таймер)	
	float getResult(float, float);		// принимает setpoint и input
	
	float getResultTimer();				// возвращает новое значение не ранее, чем через dt миллисекунд (встроенный таймер)
	float getResultTimer(float, float);	// тож самое, но принимает setpoint и input
	
	void setDirection(boolean);			// направление регулирования: NORMAL (0) или REVERSE (1)
	void setMode(boolean);				// режим: работа по входной ошибке ON_ERROR (0) или по изменению ON_RATE (1)
	void setLimits(float, float);
	void setTime(uint16_t);
	void setPID(float, float, float);
	
	float getKp();
	float getKi();
	float getKd();
		
  private:
  	float kp;
	float ki;
	float kd;
	
	boolean _mode = 0, _direction = 0, _prev_direction = 0;	
	float _minOut = 0, _maxOut = 255;	
	uint16_t _dt_ms;
	float prevInput;
	float integral;
	uint32_t pidTimer;
};

#endif