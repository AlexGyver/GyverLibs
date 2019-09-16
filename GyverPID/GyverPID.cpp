#include "GyverPID.h"
#include <Arduino.h>

GyverPID::GyverPID() {}

GyverPID::GyverPID(float new_kp, float new_ki, float new_kd) {
	dt = 100;
	pidTimer = (long)millis() + dt;
	
	kp = new_kp;
	ki = new_ki;
	kd = new_kd;
}
GyverPID::GyverPID(float new_kp, float new_ki, float new_kd, int16_t new_dt) {
	dt = new_dt;
	dt_s = (float)dt / 1000;
	pidTimer = (long)millis() + dt;
	
	kp = new_kp;
	ki = new_ki;
	kd = new_kd;
}
void GyverPID::setDirection(uint8_t direction) {
	_direction = direction;
	if (kp > 0 && _direction) {				// смотрим по первому коэффициенту, если > 0 и надо менять на <0, меняем
		kp = 0 - kp;
		ki = 0 - ki;
		kd = 0 - kd;
	} else if (kp < 0 && !_direction) {		// если < 0 и надо менять на >0, меняем
		kp = 0 - kp;
		ki = 0 - ki;
		kd = 0 - kd;
	}	
}
void GyverPID::setMode(uint8_t mode) {
	_mode = mode;
}
void GyverPID::setLimits(float min_output, float max_output) {	
	_minOut = min_output;
	_maxOut = max_output;
}
void GyverPID::setDt(int16_t new_dt) {	
	dt = new_dt;
	dt_s = (float)dt / 1000;
}
float GyverPID::getResult() {
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
}
float GyverPID::getResult(float new_setpoint, float new_input) {
	setpoint = new_setpoint;
	input = new_input;
	return GyverPID::getResult();
}
float GyverPID::getResultTimer() {
	if ((long)millis() - pidTimer >= dt) {
		pidTimer = millis();
		return GyverPID::getResult();
	} else {
		return output;
	}
}
float GyverPID::getResultTimer(float new_setpoint, float new_input) {
	setpoint = new_setpoint;
	input = new_input;
	return GyverPID::getResultTimer();
}