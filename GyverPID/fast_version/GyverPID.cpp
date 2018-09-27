#include "GyverPID.h"
#include <Arduino.h>

GyverPID::GyverPID(float new_kp, float new_ki, float new_kd) {
	_dt_ms = 100;
	pidTimer = millis() + _dt_ms;
	GyverPID::setPID(new_kp, new_ki, new_kd);
}
GyverPID::GyverPID(float new_kp, float new_ki, float new_kd, int16_t new_dt) {
	_dt_ms = new_dt;	
	pidTimer = millis() + _dt_ms;
	
	GyverPID::setPID(new_kp, new_ki, new_kd);
}
void GyverPID::setDirection(boolean direction) {
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
void GyverPID::setMode(boolean mode) {
	_mode = mode;
}
void GyverPID::setLimits(float min_output, float max_output) {	
	_minOut = min_output;
	_maxOut = max_output;
}
void GyverPID::setTime(uint16_t dt) {		// меняем время
	float coef = (float)dt / _dt_ms;		// соотношение старого и нового
	_dt_ms = dt;
	
	// пересчёт коэффициентов (так как в них входит время для ускорения расчёта)
	ki *= (float)coef;
	kd /= (float)coef;
}
void GyverPID::setPID(float new_kp, float new_ki, float new_kd) {	
	kp = new_kp;
	
	// пересчёт коэффициентов с учётом времени для ускорения расчёта
	ki = (float)new_ki * (_dt_ms / 1000);
	kd = (float)new_kd / (_dt_ms / 1000);
	
	if (_direction) {
		kp = 0 - kp;
		ki = 0 - ki;
		kd = 0 - kd;
	}
}
float GyverPID::getResult() {
    float error = setpoint - input;				// ошибка регулирования
	float delta_input = input - prevInput;		// изменение входного сигнала
	prevInput = input;
	
	output = 0;
	
	if (!_mode) output += (float)error * kp;	// пропорционально ошибке регулирования
	else output -= (float)delta_input * kp;		// пропорционально изменению входного сигнала

	// в kd уже входит dt! Для ускорения расчёта (в оригинале delta_input * kd / dt)
	output -= (float)kd * delta_input;			// дифференциальная составляющая
	
	// в ki уже входит dt! Для ускорения расчёта (в оригинале error * ki * dt)
	integral += (float)error * ki;				// расчёт интегральной составляющей
	output += integral;							// прибавляем
	
    output = constrain(output, _minOut, _maxOut);    // ограничиваем
    return output;
}
float GyverPID::getResult(float new_setpoint, float new_input) {
	setpoint = new_setpoint;
	input = new_input;
	GyverPID::getResult();
}
float GyverPID::getResultTimer() {
	if ((long)millis() > pidTimer) {
		pidTimer = millis() + _dt_ms;		
		GyverPID::getResult();
	} else {
		return output;
	}
}
float GyverPID::getResultTimer(float new_setpoint, float new_input) {
	setpoint = new_setpoint;
	input = new_input;
	GyverPID::getResultTimer();
}
float GyverPID::getKp() {
	return kp;
}
float GyverPID::getKi() {
	return ki * (_dt_ms / 1000);
}
float GyverPID::getKd() {
	return kd / (_dt_ms / 1000);
}